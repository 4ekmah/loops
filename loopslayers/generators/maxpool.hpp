/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_MAXPOOL_HPP__
#define __LOOPS_MAXPOOL_HPP__

#if __LOOPS_ARCH == __LOOPS_AARCH64
#include "loops/loops.hpp"
#include "loopslayers/loopslayers.h"
#include <algorithm>
#include <cstddef>
#include <vector>
#include <iostream>
#include <iomanip>
#include "arm_neon.h"
#include "test/tests.hpp"
//TODO(ch): There must not be Recipes in user code.
namespace loops
{

template<typename _Tp> struct MPGenTraits {};

template<> struct MPGenTraits<float> {
    typedef maxpool_f32_t maxpool_t;
    inline static std::string typestring(){return "fp32";}
};

template<> struct MPGenTraits<f16_t> {
    typedef maxpool_f16_t maxpool_t;
    inline static std::string typestring(){return "fp16";}
};

template<typename _Tp>
class MaxpoolGenerator
{
    typedef typename ElemTraits<_Tp>::masktype uintM;
    typedef typename ElemTraits<_Tp>::countertype intC;
public:
    MaxpoolGenerator(Context aCTX) : CTX(aCTX), m_done(false), lanes(CTX.vlanes<_Tp>()) {}
    typename MPGenTraits<_Tp>::maxpool_t generate(int kh_, int kw_, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x, int activation_type, float alpha);
    dwc_algs_limits calc_maxpool_algs_limits(int NC, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x);
private:
    bool m_done; 
    Context CTX;
    enum { MULTI_H = 3 };
    enum { PADHOR = 1, PADVER = 2, INITDEST = 4, MULTILINE = 8, PREINCREMENT_IDXS = 16};
    void loadVector(const IReg& base, std::vector<VReg<_Tp> >& dest, VReg<intC>& horIdxs, const VReg<uintM>& verMask, const VReg<uintM>& WcondV, int flags);

    void multilineInit(const VReg<uintM>& HcondV, const VReg<uintM>& WcondV, IReg& yi, IReg& x, IReg& base, std::vector< std::vector<std::vector<VReg<_Tp> > > >& vertMaxes, int flags);
    void multilineInitLast(const VReg<uintM>& HcondV, const VReg<uintM>& WcondV, IReg& yi, IReg& x, IReg& base, std::vector< std::vector<std::vector<VReg<_Tp> > > >& vertMaxes, int flags);
    void onlylineInit(const VReg<uintM>& WcondV, IReg& yi, IReg& x, IReg& base, std::vector<std::vector<VReg<_Tp> > >& vertMaxes, int flags);
    void onlylineInitLast(const VReg<uintM>& WcondV, IReg& yi, IReg& x, IReg& base, std::vector<std::vector<VReg<_Tp> > >& vertMaxes, int flags);

    //Common parameters and registers
    int kh, kw, elemsize, elemshift, padding_top, padding_left, padding_bottom, padding_right;
    int activation_type, stride_y, stride_x, horVecsPerOut, defect_s;
    const int lanes;

    IReg H, W, W0, dstride;
    float alpha;
    VReg<intC> countingPattern, idx_step, idx_step1;
    VReg<_Tp> valpha, v0, v6;
    VReg<_Tp> activationFunction(VReg<_Tp>& res);
    IRecipe effective_const_mul(const IReg& m1, int m2);

    inline int upDiv(int numerator, int denominator)
    {
        if(numerator < 0)
            return -((-numerator)/denominator);
        int res = numerator / denominator;
        int back = res * denominator; 
        return (numerator - back ? res + 1: res);
    }
    inline int upMultipleOf(int numerator, int denominator)
    {
        if(numerator < 0)
            return -downMultipleOf(-numerator, denominator);
        int div = numerator / denominator;
        int back = div * denominator; 
        return (numerator - back ? back + denominator : back);
    }
    inline int downMultipleOf(int numerator, int denominator)
    {
        if(numerator < 0)
            return -upMultipleOf(-numerator, denominator);
        numerator /= denominator;
        numerator *= denominator;
        return numerator;
    }
    //Find such a minimum U, that for each u>=U
    //inequality: u*ratio >= add is always correct 
    inline int downBorder(int add, int ratio)
    { return upDiv(add, ratio); }
    //Find such a maximum U, that for each u<U
    //inequality: u*ratio < add is always correct 
    inline int upperBorder(int add, int ratio)
    { return upDiv(add, ratio); }
    //lower border for all c, satisfies inequality
    //c*H*W + Y*W + X >= 0
    inline int downC(int C, int H, int W, int y, int x)
    { return downBorder(-y*W-x, H*W); }
    //upper border for all c, satisfies inequality
    //c*H*W + Y*W + X < C*W*H
    inline int upperC(int C, int H, int W, int y, int x)
    { return upperBorder(C*H*W - y*W - x, H*W);}
    //lower border for all y, satisfies inequality
    //Cf*H*W + (y * stride_y + ys)*W + X >= 0, where y
    inline int downY(int C, int H, int W, int Cf, int stride_y, int ys, int x)
    {
        Cf = std::max(Cf,0);
        return downBorder(-Cf*H*W-x-ys*W, W * stride_y);
    }
    //upper border for all y, satisfies inequality
    //Cf*H*W + (y*stride_y + ys)*W + X < C*W*H
    inline int upperY(int C, int H, int W, int Cf, int stride_y, int ys, int x)
    {
        return Cf < C ? upperBorder((C-Cf)*H*W - x - ys * W, W * stride_y) : 0;
    }
    //upper border for all y, satisfies inequality
    //Cf*H*W + (y*stride_y + ys)*W + X < C*W*H, where y is a multiple of M, started from y0:
    //y = M * r + y0 
    inline int upperY(int C, int H, int W, int Cf, int stride_y, int ys, int M, int y0, int x)
    { return Cf < C ? M * upperBorder((C-Cf)*H*W - x - (ys + y0) * W, W * M * stride_y) + y0 : 0; }
    //lower border for all x, satisfies inequality
    //Cf*H*W + Yf*W + x * stride_x + xs  >= 0
    inline int downX(int C, int H, int W, int Cf, int Yf, int stride_x, int xs)
    {
        Cf = std::max(Cf,0);
        Yf = std::max(Yf,0);
        return downBorder(-Cf*H*W - Yf * W - xs, stride_x);
    }
    //upper border for all y, satisfies inequality
    //Cf*H*W + Yf*W + x + xf < C*W*H
    inline int upperX(int C, int H, int W, int Cf, int Yf, int xs)
    { return (Cf < C && Yf < H) ? upperBorder((C-Cf)*H*W - Yf * W - xs, 1) : 0; }
    //upper border for all y, satisfies inequality
    //Cf*H*W + Yf*W + x * stride_x + xf < C*W*H, where x is a multiple of M, started from x0:
    //x = M * r + x0 
    inline int upperX(int C, int H, int W, int Cf, int Yf, int stride_x, int xs, int M, int x0)
    { return (Cf < C && Yf < H) ? M * upperBorder((C-Cf)*H*W - Yf * W - xs - x0, M * stride_x) + x0 * stride_x: 0 ;}
};

template<typename _Tp>
typename MPGenTraits<_Tp>::maxpool_t MaxpoolGenerator<_Tp>::generate(int kh_, int kw_, int padding_top_, int padding_left_, int padding_bottom_, int padding_right_, int stride_y_, int stride_x_, int activation_type_, float alpha_)
{
    if(m_done)
        throw std::runtime_error("One generator object can create only one function. Create another generator.");
    stride_y = stride_y_;
    stride_x = stride_x_;
    kh = kh_; kw = kw_;
    elemsize = sizeof(_Tp);
    elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
    padding_top = padding_top_; padding_left = padding_left_; padding_bottom = padding_bottom_; padding_right = padding_right_;
    activation_type = activation_type_;
    alpha = alpha_;
    if(alpha == 1) 
        activation_type = ACT_NONE;
    std::string funcname = "max_pooling_kH"+MPGenTraits<_Tp>::typestring();
    const bool padver = (padding_top || padding_bottom);
    const bool padhor = (padding_left || padding_right);
    int handlerFlags = (padhor ? PADHOR : 0) | (padver ? PADVER : 0); 
    funcname += std::to_string(kh) + "_kW" + std::to_string(kw) + "_pT" + std::to_string(padding_top) + "_pL" + std::to_string(padding_left) + "_pB" + std::to_string(padding_bottom) + "_pR" + std::to_string(padding_right);
    funcname += std::string("_strY") + std::to_string(stride_y) + std::string("_strX") + std::to_string(stride_x);
    if(activation_type != ACT_NONE) 
        funcname += activation_type == ACT_RELU ? "_Relu" : 
                   (activation_type == ACT_RELU6 ? "_Relu6" : 
                   (/*activation_type == ACT_LRELU ?*/ (alpha < 1 ? "_LRelu" : "_LReluRev")/* : "")*/));
    if(CTX.hasFunc(funcname))
    {
        m_done = true;
        return (typename MPGenTraits<_Tp>::maxpool_t)(CTX.getFunc(funcname).ptr());
    }
    horVecsPerOut = upDiv((lanes - 1)*stride_x + kw, lanes * stride_x);
    defect_s = (std::max(horVecsPerOut - 2, 0)*CTX.vbytes()*stride_x);
    IReg data, NC, result, H0, algsLimits;
    USE_CONTEXT_(CTX);
    STARTFUNC_(funcname, &data, &H, &W, &NC, &result, &H0, &W0, &algsLimits)
    {
        switch(activation_type)
        {
        case (ACT_RELU):
            v0.copyidx(VCONST_(_Tp, 0));
            break;
        case (ACT_RELU6):
            v0.copyidx(VCONST_(_Tp, 0));
            v6.copyidx(VCONST_(_Tp, 6));
            break;
        case (ACT_LRELU):
            valpha.copyidx(VCONST_(_Tp, _Tp(alpha)));
            break;
        };
        if(padhor)
        {
            countingPattern.copyidx(VCONST_(intC, 0));
            for(int lane = 1; lane < lanes; lane++)
                setlane(countingPattern, lane, CONST_(lane*stride_x));
            idx_step.copyidx(VCONST_(intC, lanes - stride_x + 1));
            if(stride_x > 1)
                idx_step1.copyidx(VCONST_(intC, 1));
        }
        IReg Cms = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Cms));
        IReg Cme = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Cme));
        IReg Cis = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Cis));
        IReg Cie = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Cie));
        IReg Yms = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Yms));
        IReg Yme = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Yme));
        IReg Yis = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Yis));
        IReg Yie = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Yie));
        IReg Xis = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Xis));
        IReg Xie = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Xie));

        IReg offset = CONST_(0);
        IReg channel = CONST_(0);
        dstride.copyidx(W << elemshift);
        
        WHILE_(channel < NC)
        {
            IReg yms = select(channel == Cms - 1, Yms, H0); 
            yms = select(channel > Cms - 1, 0, yms); 
            yms = select(channel > Cme, H0, yms); 
            IReg yme = select(channel >= Cms - 1, H0, 0); 
            yme = select(channel == Cme, Yme, yme); 
            yme = select(channel > Cme, 0, yme);
            IReg yis = select(channel == Cis - 1, Yis, H0); 
            yis = select(channel > Cis - 1, 0, yis); 
            yis = select(channel > Cie, H0, yis); 
            IReg yie = select(channel >= Cis - 1, H0, 0); 
            yie = select(channel == Cie, Yie, yie); 
            yie = select(channel > Cie, 0, yie);

            IReg y = CONST_(0);
            IReg yonelineEnd = select(yms > y, yms , H0); //SIMD + scalar.

            WHILE_(y < H0)
            {
                IF_(y == yms)
                {
                    WHILE_(y < yme)
                    {
                        IReg data_rs = data + W * (padver ? (effective_const_mul(y, stride_y) - padding_top) << elemshift: effective_const_mul(y, elemsize*stride_y));
                        IReg result_rs = result + (W0 << elemshift) * y;
                        IReg Hcond; if (padver) Hcond.copyidx(max(H - ((MULTI_H - 1) * stride_y + kh - 1), CONST_(0)));
                        IReg Wcond; if (padhor) Wcond.copyidx(max(W - (kw + (lanes - 1) * stride_x - 1), CONST_(0)));
                        VReg<uintM> WcondV = padhor ? broadcast<uintM>(W) : VReg<uintM>();
                        VReg<uintM> HcondV = padver ? broadcast<uintM>(H) : VReg<uintM>();
                        IReg hldx = W0 - lanes;

                        //[Multiline line number][SIMD-complete set of vectors][Deinterleaved load]
                        std::vector< std::vector<std::vector<VReg<_Tp> > > > vertMaxes(MULTI_H, std::vector<std::vector<VReg<_Tp> > >(horVecsPerOut, std::vector<VReg<_Tp> >(stride_x, VReg<_Tp>())));
                        for(int n = 0; n < MULTI_H; n++)
                            for(int vRegNum = 0; vRegNum < horVecsPerOut; vRegNum++)
                                for(int mxd = 0; mxd < stride_x; mxd++)
                                    vertMaxes[n][vRegNum][mxd].copyidx(VDEF_(_Tp));
                        IReg xi = CONST_(-padding_left);
                        IReg yi; if(padver) yi.copyidx(effective_const_mul(y, stride_y) - padding_top);
                        if(padhor||padver)
                        {
                            IReg xcond = padver&&padhor ? select(ult(yi,Hcond), xi, Wcond) : IRecipe(xi);
                            
                            IF_(padhor?(ult(xcond, Wcond)):ult(yi, Hcond))
                                multilineInit(HcondV, WcondV, yi, xi, data_rs, vertMaxes, 0);
                            ELSE_
                                multilineInit(HcondV, WcondV, yi, xi, data_rs, vertMaxes, handlerFlags);
                        }
                        else
                            multilineInit(HcondV, WcondV, y, xi, data_rs, vertMaxes, 0);
                        
                        IReg xo = CONST_(0);
                        WHILE_(xo < W0)
                        {
                            IF_(xo > hldx)
                            {
                                xo = hldx;
                                IReg xi;
                                xi.copyidx(stride_x == 1 && padding_left == 0 ? IRecipe(xo) : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
                                if(padhor||padver)
                                {
                                    IReg xcond = padver&&padhor ? select(ult(yi,Hcond), xi, Wcond) : IRecipe(xi);
                                    IF_(padhor?(ult(xcond, Wcond)):ult(yi, Hcond))
                                        multilineInit(HcondV, WcondV, yi, xi, data_rs, vertMaxes, 0);
                                    ELSE_
                                        multilineInit(HcondV, WcondV, yi, xi, data_rs, vertMaxes, handlerFlags);
                                }
                                else
                                    multilineInit(HcondV, WcondV, y, xi, data_rs, vertMaxes, 0);
                            }
                            IReg xi;
                            xi.copyidx(stride_x == 1 && padding_left == 0 ? IRecipe(xo) : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
                            if(padhor||padver)
                            {
                                IReg xcond = padver&&padhor ? select(ult(yi,Hcond), xi, Wcond) : IRecipe(xi);
                                IF_(padhor?(ult(xcond, Wcond)):ult(yi, Hcond))
                                    multilineInitLast(HcondV, WcondV, yi, xi, data_rs, vertMaxes, 0);
                                ELSE_
                                    multilineInitLast(HcondV, WcondV, yi, xi, data_rs, vertMaxes, handlerFlags);
                            }
                            else
                                multilineInitLast(HcondV, WcondV, y, xi, data_rs, vertMaxes, 0);

                            std::vector<VReg<_Tp> > res(MULTI_H, VReg<_Tp>());
                            int cur_str_lane = 0;
                            int cur_ext_lane = 0;
                            int cur_vr = 0;
                            for(int i = 0; i < kw; i++ )
                            {
                                if(i > 0)
                                    for(int n = 0; n < MULTI_H; n++)
                                    {
                                        VReg<_Tp> arg0;
                                        arg0.copyidx(i==1 ? vertMaxes[n][0][0] : res[n]); 
                                        VReg<_Tp> arg1;
                                        arg1.copyidx(cur_ext_lane == 0 ? vertMaxes[n][cur_vr][cur_str_lane] : ext(vertMaxes[n][cur_vr][cur_str_lane], vertMaxes[n][cur_vr+1][cur_str_lane], cur_ext_lane));
                                        if(i == 1)
                                            res[n].copyidx(max(arg0, arg1));
                                        else
                                            res[n] = max(arg0, arg1);
                                    }
                                cur_str_lane++;
                                if(cur_str_lane == stride_x)
                                {
                                    cur_str_lane = 0;
                                    cur_ext_lane++;
                                    if(cur_ext_lane == lanes)
                                    {
                                        cur_ext_lane = 0;
                                        cur_vr++;
                                    }
                                }
                            }

                            for(int n = 0; n < MULTI_H; n++)
                                for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
                                    for(int mxd = 0; mxd < stride_x; mxd++)
                                        vertMaxes[n][vRegNum][mxd] = vertMaxes[n][vRegNum+1][mxd];
                            IReg roffset = xo << elemshift;
                            IReg rstride = W0 << elemshift;
                            for(int lineNum = 0; lineNum < MULTI_H; lineNum++)
                            {
                                storevec<_Tp>(result_rs, roffset, activationFunction(res[lineNum]));
                                if(lineNum + 1 < MULTI_H)
                                    roffset += rstride;
                            }
                            xo += lanes;
                        }
                        y += MULTI_H;
                    }
                    yonelineEnd = H0;
                }
                
                WHILE_(y < yonelineEnd)
                {
                    IReg xo = CONST_(0);
                    IReg xis = select(y < yis, Xis, xo);
                    IReg xie = select(y >= yie, Xie, W0);
                    IReg scalarEnd = select(xis > xo, xis, W0);
                    IReg data_rs = data + W * (padver ? (effective_const_mul(y, stride_y) - padding_top) << elemshift: effective_const_mul(y, elemsize*stride_y));
                    IReg result_rs = result + (W0 << elemshift) * y;
                    IReg yi; if(padver) yi.copyidx(effective_const_mul(y, stride_y) - padding_top);
                    WHILE_(xo < W0)
                    {
                        IF_(xo == xis)
                        {
                            IReg Wcond; if(padhor) Wcond.copyidx(max(W - (kw + (lanes - 1) * stride_x - 1), CONST_(0)));
                            IReg Hcond; if(padver) Hcond.copyidx(max(H - (kh - 1), CONST_(0)));
                            VReg<uintM> WcondV = padhor ? broadcast<uintM>(W) : VReg<uintM>();
                            //[SIMD-complete set of vectors][Deinterleaved load]
                            std::vector<std::vector<VReg<_Tp> > > vertMaxes(horVecsPerOut, 
                                                    std::vector<VReg<_Tp> >(stride_x, VReg<_Tp>()));
                            for(int vRegNum = 0; vRegNum < horVecsPerOut; vRegNum++)
                                for(int sNum = 0; sNum < stride_x; sNum++)
                                    vertMaxes[vRegNum][sNum].copyidx(VDEF_(_Tp));
                            IReg xi;
                            xi.copyidx(stride_x == 1 && padding_left == 0 ? IRecipe(xo) : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
                            if(padhor||padver)
                            {
                                IReg xcond;
                                xcond.copyidx(padhor && padver ? select(ult(yi, Hcond), xi, Wcond): IRecipe(xi));
                                IF_(padhor?ult(xcond,Wcond):ult(yi, Hcond))
                                    onlylineInit(WcondV, yi, xi, data_rs, vertMaxes, 0);
                                ELSE_
                                    onlylineInit(WcondV, yi, xi, data_rs, vertMaxes, handlerFlags);
                            }
                            else 
                                onlylineInit(WcondV, y, xi, data_rs, vertMaxes, 0);
                            
                            if(padhor)
                                Wcond = max(W - (horVecsPerOut * lanes * stride_x - 1), CONST_(0));
                            WHILE_(xo < xie)
                            {
                                IReg xi;
                                xi.copyidx(stride_x == 1 && padding_left == 0 ? IRecipe(xo) : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
                                if(padhor||padver)
                                {
                                    IReg xcond;
                                    xcond.copyidx(padhor && padver ? select(ult(yi, Hcond), xi, Wcond): IRecipe(xi));
                                    IF_(padhor?ult(xcond,Wcond):ult(yi, Hcond))
                                        onlylineInitLast(WcondV, yi, xi, data_rs, vertMaxes, 0);
                                    ELSE_
                                        onlylineInitLast(WcondV, yi, xi, data_rs, vertMaxes, handlerFlags);
                                }
                                else 
                                    onlylineInitLast(WcondV, y, xi, data_rs, vertMaxes, 0);

                                VReg<_Tp> res;
                                int cur_str_lane = 0;
                                int cur_ext_lane = 0;
                                int cur_vr = 0;
                                for(int i = 0; i < kw; i++ )
                                {
                                    if(i > 0)
                                    {
                                        VReg<_Tp> arg0;
                                        arg0.copyidx(i==1 ? vertMaxes[0][0] : res); 
                                        VReg<_Tp> arg1;
                                        arg1.copyidx(cur_ext_lane == 0 ? vertMaxes[cur_vr][cur_str_lane] : ext(vertMaxes[cur_vr][cur_str_lane], vertMaxes[cur_vr+1][cur_str_lane], cur_ext_lane)); 
                                        if(i == 1)
                                            res.copyidx(max(arg0, arg1));
                                        else
                                            res = max(arg0, arg1);
                                    }
                                    cur_str_lane++;
                                    if(cur_str_lane == stride_x)
                                    {
                                        cur_str_lane = 0;
                                        cur_ext_lane++;
                                        if(cur_ext_lane == lanes)
                                        {
                                            cur_ext_lane = 0;
                                            cur_vr++;
                                        }
                                    }
                                }

                                for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
                                    for(int mxd = 0; mxd < stride_x; mxd++) 
                                        vertMaxes[vRegNum][mxd] = vertMaxes[vRegNum+1][mxd];

                                IReg roffset = xo << elemshift;
                                storevec<_Tp>(result_rs, roffset, activationFunction(res));
                                xo += lanes;
                            }
                            scalarEnd = W0;
                        }

                        WHILE_(xo < scalarEnd)
                        {
                            IReg xi;
                            xi.copyidx(stride_x == 1 && padding_left == 0 ? IRecipe(xo) : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
                            VReg<_Tp> vres = VDEF_(_Tp);
                            IReg res_already_init = CONST_(0);
                            IReg data__ = data_rs + (xi << elemshift);
                            IReg kcol = CONST_(0);
                            IReg krow = CONST_(0);
                            IReg istride = (W - (kw-1)) << elemshift;
                            
                            WHILE_(krow < kh)
                            {
                                if(padhor||padver)
                                {
                                    VReg<_Tp> justloaded = VCONST_(_Tp, 0);
                                    IReg ex = xi + kcol;
                                    IReg ey = (padver ? effective_const_mul(y, stride_y) - padding_top : IRecipe(y)) + krow;
                                    select(ex < 0, W, ex);
                                    select(ey < 0, H, ey);
                                    IF_(ult(ex,W))
                                    {
                                        IF_(ult(ey,H))
                                        {
                                            justloaded = broadcast<_Tp>(load_<_Tp>(data__));
                                        }
                                    }
                                    IF_(res_already_init == 1)
                                    {
                                        vres = max(vres, justloaded);
                                    }
                                    ELSE_
                                    {
                                        vres = justloaded;
                                        res_already_init = CONST_(1);
                                    }
                                }
                                else
                                {
                                    VReg<_Tp> justloaded = broadcast<_Tp>(load_<_Tp>(data__));
                                    IF_(res_already_init == 1)
                                    {
                                        vres = max(vres, justloaded);
                                    }
                                    ELSE_
                                    {
                                        vres = justloaded;
                                        res_already_init = CONST_(1);
                                    }
                                }
                                kcol = kcol + 1;
                                data__ = select(kcol == kw, data__ + istride, data__ + elemsize);
                                krow = select(kcol == kw, krow + 1, krow);
                                kcol = select(kcol == kw, 0, kcol);
                            }
                            IReg roffset = (xo << elemshift);
                            store_<_Tp>(result_rs + roffset, getlane<_Tp>(activationFunction(vres), 0));
                            xo += 1;
                        }
                    }
                    y += 1;
                }
            }
            data += H * dstride;
            result += H0 * (W0 << elemshift);
            channel = channel + 1;
        }
        RETURN_(0);
    }
    m_done = true;
    return (typename MPGenTraits<_Tp>::maxpool_t)(CTX.getFunc(funcname).ptr());
}

template<typename _Tp>
dwc_algs_limits MaxpoolGenerator<_Tp>::calc_maxpool_algs_limits(int NC, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x)
{
    int Cms, Cme;
    int lsimd = stride_x == 1 ? upMultipleOf(kw + lanes - 2, lanes) - 1:
                                downMultipleOf(kw - 1, stride_x) + lanes * stride_x - 1;
    int XlastMulti = ((W0 - lanes) * stride_x - padding_left) + lsimd;
    if(W0 >= lanes)
    {
        Cms = downC(NC, H, W, -padding_top, -padding_left);
        int YlastMulti = (downMultipleOf(H0 - 1, MULTI_H) + (MULTI_H - 1)) * stride_y - (stride_y - 1) - padding_top + kh - 1;
        Cme = upperC(NC, H, W, YlastMulti, XlastMulti);
        int Cme_write = upperC(NC, H0, W0, downMultipleOf(H0 - 1, MULTI_H) + (MULTI_H - 1), W0 - 1);
        Cme = std::min(Cme, Cme_write);
    }
    else
    {
        Cms = NC + 1;
        Cme = 0;
    }
    int Xlast = downMultipleOf(W0-1, lanes) * stride_x + lsimd - padding_left;
    int Cis = downC(NC, H, W, 0, -padding_left);
    int Cie = upperC(NC, H, W, std::min((int)((H0-1)*stride_y + kh - 1), H - 1), Xlast);
    int Cie_write = upperC(NC, H0, W0, (H0-1), upMultipleOf(W0, lanes) - 1);
    Cie = std::min(Cie, Cie_write);
    if(Cie < (Cis - 1))
    {
        Cis = NC + 1;
        Cie = 0;
    }
    int Yms = H0 + 1, Yme = 0;
    if(Cms < NC + 1)
    {
        Yms = downY(NC, H, W, Cms - 1, stride_y, -padding_top, -padding_left);
        Yme = upperY(NC, H, W, Cme, stride_y, (MULTI_H - 1) * stride_y + kh - padding_top - 1, MULTI_H, ((Cms - 1) == Cme ? Yms : 0), XlastMulti);
        int Yme_write = upperY(NC, H0, W0, Cme, 1, (MULTI_H - 1), MULTI_H, ((Cms - 1) == Cme ? Yms : 0), W0 - 1);
        Yme = std::min(Yme, Yme_write);
    }
    int Yis = H0 + 1, Yie = 0;
    if(Cis < NC + 1)
    {
        Yis = (padding_left == 0) ? 0 : downY(NC, H, W, Cis - 1, stride_y, -padding_top, -padding_left);
        if ((H - 1) * W + Xlast < (NC-Cie)*H*W)
            Yie = H0;
        else
            Yie = std::max(0, upperY(NC, H, W, Cie, stride_y, kh - 1 - padding_top, Xlast));
        int Yie_write = upperY(NC, H0, W0, Cie, 1, 0, upMultipleOf(W0, lanes) - 1);
        Yie = std::min(Yie, Yie_write);
    }
    int Xis = W0 + 1, Xie = 0;
    if(Yis < H0 + 1)
    {
        Xis = downX(NC, H, W, Cis-1, (Yis - 1) * stride_y - padding_top, stride_x, -padding_left);
        Xie = upperX(NC, H, W, Cie, std::min(Yie * stride_y - padding_top + kh - 1, H-1), stride_x, lsimd-padding_left, lanes, ((Cis - 1) == Cie && Yis == Yie)? Xis: 0);
        int Xie_write = upperX(NC, H0, W0, Cie, Yie, 1, lanes - 1, lanes, ((Cis - 1) == Cie && Yis == Yie)? Xis: 0);
        Xie = std::min(Xie, Xie_write);
    }
    return dwc_algs_limits(Cms, Cme, Cis, Cie, Yms, Yme, Yis, Yie, Xis, Xie);
}

template<typename _Tp>
void MaxpoolGenerator<_Tp>::loadVector(const IReg& base, std::vector<VReg<_Tp> >& dest, VReg<intC>& horIdxs, const VReg<uintM>& verMask, const VReg<uintM>& WcondV, int flags)
{
    USE_CONTEXT_(CTX);
    Assert(stride_x == 1 || stride_x == 2);
    if(stride_x == 1)
    {
        if(flags&INITDEST)
            dest[0].copyidx(loadvec<_Tp>(base));
        else
            dest[0] = loadvec<_Tp>(base);
    }
    else
    {
        Assert(stride_x == 2);
        if(flags&INITDEST)
        {
            loadvec_deinterleave2<_Tp>(dest[0], dest[1], base);
        }
        else
        {
            VReg<_Tp> r0, r1;
            loadvec_deinterleave2<_Tp>(r0, r1, base);
            dest[0] = r0;
            dest[1] = r1;
        }
    }
    for(int dnum = 0; dnum < stride_x; dnum++) 
        if(flags&(PADHOR|PADVER))
        {
            if((flags & PADVER) && !(flags & PADHOR))
            {
                dest[dnum] = reinterpret<_Tp>( verMask & reinterpret<uintM>(dest[dnum]));
            }
            else
            {
                if(flags&PREINCREMENT_IDXS && dnum == 0)
                    horIdxs += idx_step;
                else if(dnum > 0)
                    horIdxs += idx_step1;
                VReg<uintM> mask;
                mask.copyidx((flags & PADHOR) ? (reinterpret<uintM>(horIdxs) < WcondV) : verMask);
                dest[dnum] = reinterpret<_Tp>( mask & reinterpret<uintM>(dest[dnum]));
            }
        }
}

template<typename _Tp>
void MaxpoolGenerator<_Tp>::multilineInit(const VReg<uintM>& HcondV, const VReg<uintM>& WcondV, IReg& yi_, IReg& x, IReg& base, std::vector< std::vector<std::vector<VReg<_Tp> > > >& vertMaxes, int flags)
{
    USE_CONTEXT_(CTX);
    int lvflags = flags&(PADHOR|PADVER);
    IReg vertMaxesPtr = base + (x << elemshift);
    IReg yi = yi_;
    for(int lrow = 0; lrow < (MULTI_H - 1) * stride_y + kh; lrow++)
    {
        bool current_line_is_needed = false;
        int stride_multiplier = 1;
        for(int lineNum = 0; lineNum < MULTI_H; lineNum++) 
            if(lrow >= lineNum * stride_y && lrow < lineNum * stride_y + kh)
            {
                if(lrow + 1 == lineNum * stride_y + kh && stride_y > kh)
                    stride_multiplier = stride_y - kh + 1;
                current_line_is_needed = true;
            }
        if(!current_line_is_needed)
            continue;

        VReg<intC> horIdxs = (flags&PADHOR) ? broadcast<intC>(x) + countingPattern : VReg<intC>();
        VReg<uintM> vertMask = (flags&PADVER) ? broadcast<uintM>(yi) < HcondV : VReg<uintM>();
        if((flags&PADVER)&&(flags&PADHOR))
        {
            VReg<intC> antiSpill = horIdxs; //TODO(ch): remove it when snippet management will be better.
            horIdxs = select(vertMask, antiSpill, reinterpret<intC>(WcondV));
        }
        for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
        {
            bool load_into_vmax = (lrow%stride_y == 0) && (lrow/stride_y < MULTI_H);
            std::vector< VReg<_Tp> > loadedVector(stride_x, VReg<_Tp>());
            if(load_into_vmax)
                for(int mxd = 0; mxd < stride_x; mxd++) 
                    loadedVector[mxd].copyidx(vertMaxes[lrow/stride_y][vRegNum][mxd]);
            loadVector(vertMaxesPtr, loadedVector, horIdxs, vertMask, WcondV, lvflags | (!load_into_vmax ? INITDEST : 0) | (vRegNum > 0 ? PREINCREMENT_IDXS : 0));
            if(lrow < (MULTI_H - 1)*stride_y)
            {
                for(int line_num = 0; line_num < upDiv(lrow, stride_y); line_num++)
                {
                    const int krow = lrow - line_num * stride_y;
                    if(krow>0 && krow < kh)
                        for(int mxd = 0; mxd < stride_x; mxd++) 
                            vertMaxes[line_num][vRegNum][mxd] = max(vertMaxes[line_num][vRegNum][mxd], loadedVector[mxd]);
                }
            }
            else
            {
                if(lrow > (MULTI_H - 1) * stride_y)
                    for(int mxd = 0; mxd < stride_x; mxd++) 
                        vertMaxes[MULTI_H - 1][vRegNum][mxd] = max(vertMaxes[MULTI_H - 1][vRegNum][mxd], loadedVector[mxd]);
                if((lrow - kh + 1)%stride_y == 0)
                {
                    int line_num = (lrow - kh + 1)/stride_y;
                    if(line_num >= 0 && line_num < MULTI_H -1)
                        for(int mxd = 0; mxd < stride_x; mxd++) 
                            vertMaxes[line_num][vRegNum][mxd] = max(vertMaxes[line_num][vRegNum][mxd], vertMaxes[MULTI_H - 1][vRegNum][mxd]); 
                }
            }
            if(vRegNum + 2 < horVecsPerOut)
                vertMaxesPtr += CTX.vbytes()*stride_x;
        }
        if(lrow + 1 != (MULTI_H - 1) * stride_y + kh)
        {
            vertMaxesPtr += defect_s ? (effective_const_mul(dstride, stride_multiplier) - defect_s) : effective_const_mul(dstride, stride_multiplier);
            if(flags&PADVER)
                yi += stride_multiplier;
        }
    }
}

template<typename _Tp>
void MaxpoolGenerator<_Tp>::multilineInitLast(const VReg<uintM>& HcondV, const VReg<uintM>& WcondV, IReg& yi_, IReg& x, IReg& base, std::vector< std::vector<std::vector<VReg<_Tp> > > >& vertMaxes, int flags)
{
    USE_CONTEXT_(CTX);
    int lvflags = flags&(PADHOR|PADVER);
    int vRegNum = horVecsPerOut-1;
    IReg vertMaxesPtr = base + (vRegNum*CTX.vbytes()*stride_x) + (x << elemshift);
    IReg yi = yi_;
    VReg<intC> horIdxs_ = (flags&PADHOR) ? broadcast<intC>(x+vRegNum*lanes*stride_x) + countingPattern : VReg<intC>();
    for(int lrow = 0; lrow < (MULTI_H - 1) * stride_y + kh; lrow++)
    {
        bool current_line_is_needed = false;
        int stride_multiplier = 1;
        for(int lineNum = 0; lineNum < MULTI_H; lineNum++) 
            if(lrow >= lineNum * stride_y && lrow < lineNum * stride_y + kh)
            {
                if(lrow + 1 == lineNum * stride_y + kh && stride_y > kh)
                    stride_multiplier = stride_y - kh + 1;
                current_line_is_needed = true;
            }
        if(!current_line_is_needed)
            continue;

        VReg<uintM> vertMask = (flags&PADVER) ? broadcast<uintM>(yi) < HcondV : VReg<uintM>();
        VReg<intC> horIdxs;
        horIdxs.copyidx((flags&PADVER)&&(flags&PADHOR)? select(vertMask, horIdxs_, reinterpret<intC>(WcondV)): horIdxs_);
        bool load_into_vmax = (lrow%stride_y == 0) && (lrow/stride_y < MULTI_H);
        std::vector< VReg<_Tp> > loadedVector(stride_x, VReg<_Tp>());
        if(load_into_vmax)
            for(int mxd = 0; mxd < stride_x; mxd++) 
                loadedVector[mxd].copyidx(vertMaxes[lrow/stride_y][vRegNum][mxd]);
        loadVector(vertMaxesPtr, loadedVector, horIdxs, vertMask, WcondV, lvflags | (!load_into_vmax ? INITDEST : 0));

        if(lrow < (MULTI_H - 1)*stride_y)
        {
            for(int line_num = 0; line_num < upDiv(lrow, stride_y); line_num++)
            {
                const int krow = lrow - line_num * stride_y;
                if(krow>0 && krow < kh)
                    for(int mxd = 0; mxd < stride_x; mxd++) 
                        vertMaxes[line_num][vRegNum][mxd] = max(vertMaxes[line_num][vRegNum][mxd], loadedVector[mxd]);
            }
        }
        else
        {
            if(lrow > (MULTI_H - 1) * stride_y)
                for(int mxd = 0; mxd < stride_x; mxd++) 
                    vertMaxes[MULTI_H - 1][vRegNum][mxd] = max(vertMaxes[MULTI_H - 1][vRegNum][mxd], loadedVector[mxd]);
            if((lrow - kh + 1)%stride_y == 0)
            {
                int line_num = (lrow - kh + 1)/stride_y;
                if(line_num >= 0 && line_num < MULTI_H -1)
                    for(int mxd = 0; mxd < stride_x; mxd++) 
                        vertMaxes[line_num][vRegNum][mxd] = max(vertMaxes[line_num][vRegNum][mxd], vertMaxes[MULTI_H - 1][vRegNum][mxd]);
            }
        }
        if(lrow + 1 < (MULTI_H - 1) * stride_y + kh)
        {
            vertMaxesPtr += effective_const_mul(dstride, stride_multiplier);
            if(flags&PADVER)
                yi += stride_multiplier;
        }
    }
}

template<typename _Tp>
void MaxpoolGenerator<_Tp>::onlylineInit(const VReg<uintM>& WcondV, IReg& yi_, IReg& x, IReg& base, std::vector<std::vector<VReg<_Tp> > >& vertMaxes, int flags)
{
    USE_CONTEXT_(CTX);
    int lvflags = flags&PADHOR;
    VReg<uintM> dummy;
    IReg vertMaxesPtr = base + (x << elemshift);
    if(flags&PADVER)
    {
        IReg krow = CONST_(0);
        IReg yi = yi_;
        WHILE_(krow<kh)
        {
            VReg<intC> horIdxs = flags&PADHOR ? broadcast<intC>(x) + countingPattern : VReg<intC>();
            for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
            {
                std::vector<VReg<_Tp> > loadedVector(stride_x, VReg<_Tp>());
                for(int mxd = 0; mxd < stride_x; mxd++)
                    loadedVector[mxd].copyidx(VDEF_(_Tp));
                IF_(ult(yi,H))
                    loadVector(vertMaxesPtr, loadedVector, horIdxs, dummy, WcondV, lvflags | (vRegNum?PREINCREMENT_IDXS:0));
                ELSE_
                    for(int mxd = 0; mxd < stride_x; mxd++)
                        loadedVector[mxd] = VCONST_(_Tp, 0);
                IF_(krow > 0)
                    for(int mxd = 0; mxd < stride_x; mxd++)
                        vertMaxes[vRegNum][mxd] = max(vertMaxes[vRegNum][mxd], loadedVector[mxd]);
                ELSE_
                    for(int mxd = 0; mxd < stride_x; mxd++)
                        vertMaxes[vRegNum][mxd] = loadedVector[mxd];
                if(vRegNum + 2 < horVecsPerOut)
                    vertMaxesPtr += CTX.vbytes()*stride_x;
            }
            vertMaxesPtr += defect_s ? (dstride - defect_s) : IRecipe(dstride);
            yi += 1;
            krow += 1;
        }
    }
    else
    {
        for(int krow = 0; krow < kh; krow++) 
        {
            VReg<intC> horIdxs = flags&PADHOR ? broadcast<intC>(x) + countingPattern : VReg<intC>();
            for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
            {
                if(krow > 0)
                {
                    std::vector<VReg<_Tp> > loadedVector(stride_x, VReg<_Tp>());
                    loadVector(vertMaxesPtr, loadedVector, horIdxs, dummy, WcondV, lvflags | INITDEST | (vRegNum?PREINCREMENT_IDXS:0));
                    for(int mxd = 0; mxd < stride_x; mxd++)
                        vertMaxes[vRegNum][mxd] = max(vertMaxes[vRegNum][mxd], loadedVector[mxd]);
                }
                else
                    loadVector(vertMaxesPtr, vertMaxes[vRegNum], horIdxs, dummy, WcondV, lvflags | (vRegNum?PREINCREMENT_IDXS:0));
                if(vRegNum + 2 < horVecsPerOut)
                    vertMaxesPtr += CTX.vbytes()*stride_x;
            }
            if(krow + 1 < kh)
                vertMaxesPtr += defect_s ? (dstride - defect_s) : IRecipe(dstride);
        }
    }
}

template<typename _Tp>
void MaxpoolGenerator<_Tp>::onlylineInitLast(const VReg<uintM>& WcondV, IReg& yi_, IReg& x, IReg& base, std::vector<std::vector<VReg<_Tp> > >& vertMaxes, int flags)
{
    USE_CONTEXT_(CTX);
    int lvflags = flags&PADHOR;
    VReg<uintM> dummy;
    int lastVRegNum = horVecsPerOut-1;
    IReg vertMaxesPtr = base + lastVRegNum*CTX.vbytes()*stride_x + (x << elemshift);
    VReg<intC> horIdxs_ = flags&PADHOR ? broadcast<intC>(x+(lastVRegNum*lanes*stride_x)) + countingPattern : VReg<intC>();
    int ldretsneeded = std::min(((lanes - 1)*stride_x + kw - 1)%(lanes*stride_x), stride_x);
    
    if(flags&PADVER)
    {
        IReg yi = yi_;
        IReg krow = CONST_(0);
        WHILE_(krow<kh)
        {
            VReg<intC> horIdxs;
            horIdxs.copyidx(flags&PADHOR?(stride_x == 1 ? horIdxs_ : VReg<intC>(horIdxs_)) : VReg<intC>());
            std::vector<VReg<_Tp> > loadedVector(stride_x, VReg<_Tp>());
            for(int scol = 0; scol < stride_x; scol++)
                loadedVector[scol].copyidx(VDEF_(_Tp));
            IF_(ult(yi,H))
                loadVector(vertMaxesPtr, loadedVector, horIdxs, dummy, WcondV, lvflags);
            ELSE_
                for(int mxd = 0; mxd < stride_x; mxd++)
                    loadedVector[mxd] = VCONST_(_Tp,0);

            IF_(krow > 0)
                for(int mxd = 0; mxd < stride_x; mxd++)
                    vertMaxes[lastVRegNum][mxd] = max(vertMaxes[lastVRegNum][mxd], loadedVector[mxd]);
            ELSE_
                for(int mxd = 0; mxd < stride_x; mxd++)
                    vertMaxes[lastVRegNum][mxd] = loadedVector[mxd];
            vertMaxesPtr += dstride;
            yi += 1;
            krow += 1;
        }
    }
    else
    {
        for(int krow = 0; krow < kh; krow++) 
        {
            VReg<intC> horIdxs;
            horIdxs.copyidx(flags&PADHOR?(stride_x == 1 ? horIdxs_ : VReg<intC>(horIdxs_)) : VReg<intC>());
            if(krow > 0) 
            {
                std::vector<VReg<_Tp> > loadedVector(stride_x, VReg<_Tp>());
                loadVector(vertMaxesPtr, loadedVector, horIdxs, dummy, WcondV, lvflags | INITDEST);
                for(int mxd = 0; mxd < stride_x; mxd++)
                    vertMaxes[lastVRegNum][mxd] = max(vertMaxes[lastVRegNum][mxd], loadedVector[mxd]);
            }
            else
                loadVector(vertMaxesPtr, vertMaxes[lastVRegNum], horIdxs, dummy, WcondV, lvflags);
            if(krow + 1 < kh)
                vertMaxesPtr += dstride;
        }
    }
}

template<typename _Tp>
VReg<_Tp> MaxpoolGenerator<_Tp>::activationFunction(VReg<_Tp>& res)
{
    USE_CONTEXT_(CTX);
    switch(activation_type)
    {
        case(ACT_NONE): return static_cast<VReg<_Tp>&&>(res); break;
        case(ACT_RELU): return static_cast<VReg<_Tp>&&>(max(res, v0)); break;
        case(ACT_RELU6): return static_cast<VReg<_Tp>&&>(max(min(res, v6),v0)); break;
        case(ACT_LRELU): return static_cast<VReg<_Tp>&&>(alpha < 1 ? max(res,res * valpha) : min(res,res * valpha) ); break;
        defaout: throw std::runtime_error("Unknown activation");
    };
    return VReg<_Tp>();
}

template<typename _Tp>
IRecipe MaxpoolGenerator<_Tp>::effective_const_mul(const IReg& m1, int m2)
{
    if(m2 == 1)
        return IRecipe(m1);
    else if(m2>0 && (((m2 - 1) & m2) == 0))
    {
        int degree = -1;
        while(m2 > 0)
        {
            degree++;
            m2 >>= 1;
        }
        return m1 << degree;
    }
    else
        return m1*m2;
}
};
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
#endif //__LOOPS_MAXPOOL_HPP__
