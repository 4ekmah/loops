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
    void loadVector(const IReg& base, int64_t offset, VReg<_Tp>& dest, VReg<intC>& horIdxs, const VReg<uintM>& verMask, const VReg<uintM>& WcondV, int flags = 0);
    void multilineInit(const VReg<uintM>& HcondV, const VReg<uintM>& WcondV, IReg& yi, IReg& x, IReg& base, std::vector<std::vector<VReg<_Tp> > >& vertMaxes, int flags);
    void multilineInitLast(const VReg<uintM>& HcondV, const VReg<uintM>& WcondV, IReg& yi, IReg& x, IReg& base, std::vector<std::vector<VReg<_Tp> > >& vertMaxes, int flags);
    void onelineInit(const VReg<uintM>& WcondV, IReg& yi, IReg& x, IReg& base, std::vector<loops::VReg<_Tp> >& vertMaxes, int flags);
    void onelineInitLast(const VReg<uintM>& WcondV, IReg& yi, IReg& x, IReg& base, std::vector<loops::VReg<_Tp> >& vertMaxes, int flags);

    //Common parameters and registers
    int kh, kw, elemsize, elemshift, padding_top, padding_left, padding_bottom, padding_right, activation_type, stride_y, stride_x, horVecsPerOut;
    const int lanes;

    IReg H, W, W0, rstride;
    float alpha;
    VReg<intC> countingPattern, idx_step;
    VReg<_Tp> valpha, v0, v6;
    VReg<_Tp> activationFunction(VReg<_Tp>& res);
    IReg effective_const_mul(const IReg& m1, int m2);

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
    Assert(stride_x_ == 1 && stride_y_ == 1);
    if(m_done)
        throw std::runtime_error("One generator object can create only one function. Create another generator.");
    stride_y = stride_x = 1;
    kh = kh_; kw = kw_;
    elemsize = sizeof(float);
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
    if(activation_type != ACT_NONE) 
        funcname += activation_type == ACT_RELU ? "_Relu" : 
                   (activation_type == ACT_RELU6 ? "_Relu6" : 
                   (/*activation_type == ACT_LRELU ?*/ (alpha < 1 ? "_LRelu" : "_LReluRev")/* : "")*/));
    if(CTX.hasFunc(funcname))
    {
        m_done = true;
        return (typename MPGenTraits<_Tp>::maxpool_t)(CTX.getFunc(funcname).ptr());
    }
    horVecsPerOut = (kw + lanes - 1)/4 + ((kw + lanes - 1)%4?1:0);
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
                setlane(countingPattern, lane, CONST_(lane));
            idx_step.copyidx(VCONST_(intC, lanes));
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
        rstride.copyidx(W << elemshift);
        
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
                        IReg data_rs = data + ((W * (padver ? y - padding_top: y)) << elemshift);
                        IReg result_rs = result + (W0 << elemshift) * y;
                        IReg Hcond = padver ? max(H - ((MULTI_H - 1) * stride_y + kh - 1), CONST_(0)) : IReg(); 
                        IReg Wcond = padhor ? max(W - (kw + (lanes - 1) * stride_x - 1), CONST_(0)): IReg();
                        VReg<uintM> WcondV = padhor ? broadcast<uintM>(W) : VReg<uintM>();
                        VReg<uintM> HcondV = padver ? broadcast<uintM>(H) : VReg<uintM>();
                        IReg hldx = W0 - lanes;

                        std::vector<std::vector<VReg<_Tp> > > vertMaxes(MULTI_H, std::vector<VReg<_Tp>>(horVecsPerOut, VReg<_Tp>()));
                        vertMaxes.resize(MULTI_H, std::vector<VReg<_Tp>>(horVecsPerOut, VReg<_Tp>()));
                        for(int n = 0; n < MULTI_H; n++)
                        {
                            for(int vRegNum = 0; vRegNum < horVecsPerOut; vRegNum++)
                                vertMaxes[n][vRegNum].copyidx(VDEF_(_Tp));
                        }
                        IReg xi = CONST_(-padding_left);
                        if(padhor||padver)
                        {
                            IReg yi = padver ? y - padding_top : IReg();
                            IReg xcond = padver&&padhor ? select(ult(yi,Hcond), xi, Wcond) : xi;
                            
                            IF_(padhor?(ult(xcond, Wcond)):ult(yi, Hcond))
                            {
                                multilineInit(HcondV, WcondV, yi, xi, data_rs, vertMaxes, 0);
                            }
                            ELSE_
                            {
                                multilineInit(HcondV, WcondV, yi, xi, data_rs, vertMaxes, handlerFlags);
                            }
                        }
                        else
                        {
                            multilineInit(HcondV, WcondV, y, xi, data_rs, vertMaxes, 0);
                        }
                        
                        IReg xo = CONST_(0);
                        WHILE_(xo < W0)
                        {
                            IF_(xo > hldx)
                            {
                                xo = hldx;
                                IReg xi;
                                xi.copyidx(stride_x == 1 && padding_left == 0 ? xo : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
                                if(padhor||padver)
                                {
                                    IReg yi = padver ? y - padding_top : IReg();
                                    IReg xcond = padver&&padhor ? select(ult(yi,Hcond), xi, Wcond) : xi;
                                    IF_(padhor?(ult(xcond, Wcond)):ult(yi, Hcond))
                                    {
                                        multilineInit(HcondV, WcondV, yi, xi, data_rs, vertMaxes, 0);
                                    }
                                    ELSE_
                                    {
                                        multilineInit(HcondV, WcondV, yi, xi, data_rs, vertMaxes, handlerFlags);
                                    }
                                }
                                else
                                {
                                    multilineInit(HcondV, WcondV, y, xi, data_rs, vertMaxes, 0);
                                }
                            }
                            IReg xi;
                            xi.copyidx(stride_x == 1 && padding_left == 0 ? xo : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
                            if(padhor||padver)
                            {
                                IReg yi = padver ? y - padding_top : IReg();
                                IReg xcond = padver&&padhor ? select(ult(yi,Hcond), xi, Wcond) : xi;
                                IF_(padhor?(ult(xcond, Wcond)):ult(yi, Hcond))
                                {
                                    
                                    multilineInitLast(HcondV, WcondV, yi, xi, data_rs, vertMaxes, 0);
                                }
                                ELSE_
                                {
                                    multilineInitLast(HcondV, WcondV, yi, xi, data_rs, vertMaxes, handlerFlags);
                                }
                            }
                            else
                            {
                                multilineInitLast(HcondV, WcondV, y, xi, data_rs, vertMaxes, 0);
                            }

                            std::vector<VReg<_Tp> > res(MULTI_H, VReg<_Tp>());
                            for(int n = 0; n < MULTI_H; n++)
                                res[n].copyidx(max(vertMaxes[n][0], ext(vertMaxes[n][0], vertMaxes[n][1], 1)));

                            for(int i = 2; i < kw; i++ )
                            {
                                for(int n = 0; n < MULTI_H; n++)
                                {
                                    int vRegNum = i / 4;
                                    if (i % 4 == 0) 
                                        res[n] = max(res[n], vertMaxes[n][vRegNum]);
                                    else
                                        res[n] = max(res[n], ext(vertMaxes[n][vRegNum], vertMaxes[n][vRegNum+1], i % 4));
                                }
                            }

                            for(int n = 0; n < MULTI_H; n++)
                                for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
                                    vertMaxes[n][vRegNum] = vertMaxes[n][vRegNum+1];
                            IReg roffset = xo << elemshift;
                            int outdiffW = padding_left + padding_right - kw + 1;
                            IReg W0_;
                            W0_.copyidx(outdiffW ? (outdiffW > 0 ? W+outdiffW: W-(-outdiffW)): W); //TODO(ch): fix it in interface of loops(there must be ability to add and sub negaitve numbers).
                            IReg offstride = W0_ << elemshift;
                            for(int lineNum = 0; lineNum < MULTI_H; lineNum++)
                            {
                                storevec<_Tp>(result_rs, roffset, activationFunction(res[lineNum]));
                                if(lineNum + 1 < MULTI_H)
                                    roffset += offstride;
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
                    WHILE_(xo < W0)
                    {
                        IF_(xo == xis)
                        {
                            IReg Wcond = padhor ? max(W - (lanes*horVecsPerOut - 1), CONST_(0)) : IReg();
                            IReg Hcond = padver ? max(H - (kh - 1), CONST_(0)) : IReg();
                            VReg<uintM> WcondV = padhor ? broadcast<uintM>(W) : VReg<uintM>();
                            std::vector<VReg<_Tp> > vertMaxes(horVecsPerOut, VReg<_Tp>());
                            for(int vRegNum = 0; vRegNum < horVecsPerOut; vRegNum++)
                                vertMaxes[vRegNum].copyidx(VDEF_(_Tp));
                            IReg xi;
                            xi.copyidx(stride_x == 1 && padding_left == 0 ? xo : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
                            if(padhor||padver)
                            {
                                IReg yi = padver ? y - padding_top : IReg();
                                IReg xcond;
                                xcond.copyidx(padhor && padver ? select(ult(yi, Hcond), xi, Wcond): xi);
                                IF_(padhor?ult(xcond,Wcond):ult(yi, Hcond))
                                {
                                    onelineInit(WcondV, yi, xi, data_rs, vertMaxes, 0);
                                }
                                ELSE_
                                {
                                    onelineInit(WcondV, yi, xi, data_rs, vertMaxes, handlerFlags);
                                }
                            }
                            else 
                                onelineInit(WcondV, y, xi, data_rs, vertMaxes, 0);
                            
                            WHILE_(xo < xie)
                            {
                                IReg xi;
                                xi.copyidx(stride_x == 1 && padding_left == 0 ? xo : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
                                IReg data__ = data_rs + (xi << elemshift);
                                if(padhor||padver)
                                {
                                    IReg yi = padver ? y - padding_top : IReg();
                                    IReg xcond;
                                    xcond.copyidx(padhor && padver ? select(ult(yi, Hcond), xi, Wcond): xi);
                                    IF_(padhor?ult(xcond,Wcond):ult(yi, Hcond))
                                    {
                                        onelineInitLast(WcondV, yi, xi, data__, vertMaxes, 0);
                                    }
                                    ELSE_
                                    {
                                        onelineInitLast(WcondV, yi, xi, data__, vertMaxes, handlerFlags);
                                    }
                                }
                                else 
                                    onelineInitLast(WcondV, y, xi, data__, vertMaxes, 0);

                                VReg<_Tp> res = max(vertMaxes[0], ext(vertMaxes[0], vertMaxes[1], 1));
                                for(int i = 2; i < kw; i++ )
                                {
                                    int vRegNum = i / 4;
                                    if (i % 4 == 0) 
                                        res = max(res, vertMaxes[vRegNum]);
                                    else
                                        res = max(res, ext(vertMaxes[vRegNum], vertMaxes[vRegNum+1], i % 4));
                                }

                                for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
                                {
                                    vertMaxes[vRegNum] = vertMaxes[vRegNum+1];
                                }

                                IReg roffset = xo << elemshift;
                                storevec<_Tp>(result_rs, roffset, activationFunction(res));
                                xo += lanes;
                            }
                            scalarEnd = W0;
                        }

                        WHILE_(xo < scalarEnd)
                        {
                            IReg xi;
                            xi.copyidx(stride_x == 1 && padding_left == 0 ? xo : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
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
                                    IReg ey = (padver ? y - padding_top : y) + krow;
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
            data += H * W << elemshift;
            result += H0 * W0 << elemshift;
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
void MaxpoolGenerator<_Tp>::loadVector(const IReg& base, int64_t offset, VReg<_Tp>& dest, VReg<intC>& horIdxs, const VReg<uintM>& verMask, const VReg<uintM>& WcondV, int flags)
{
    USE_CONTEXT_(CTX);
    if(flags&INITDEST)
        dest.copyidx(loadvec<_Tp>(base, offset));
    else
        dest = loadvec<_Tp>(base, offset);
    if(flags&(PADHOR|PADVER))
    {
        if((flags & PADVER) && !(flags & PADHOR))
        {
            dest = reinterpret<_Tp>( verMask & reinterpret<uintM>(dest));
        }
        else
        {
            if((flags&PREINCREMENT_IDXS) && (flags & PADHOR))
                horIdxs += idx_step;
            VReg<uintM> mask;
            mask.copyidx((flags & PADHOR) ? (reinterpret<uintM>(horIdxs) < WcondV) : verMask);
            dest = reinterpret<_Tp>( mask & reinterpret<uintM>(dest));
        }
    }
}

template<typename _Tp>
void MaxpoolGenerator<_Tp>::multilineInit(const VReg<uintM>& HcondV, const VReg<uintM>& WcondV, IReg& yi_, IReg& x, IReg& base, std::vector<std::vector<VReg<_Tp>>>& vertMaxes, int flags)
{
    USE_CONTEXT_(CTX);
    int lvflags = flags&(PADHOR|PADVER);
    IReg vertMaxesPtr = base + (x << elemshift);
    IReg yi = yi_;
    for(int lrow = 0; lrow < (MULTI_H - 1) * stride_y + kh; lrow++)
    {
        VReg<intC> horIdxs = (flags&PADHOR) ? broadcast<intC>(x) + countingPattern : VReg<intC>();
        VReg<uintM> vertMask = (flags&PADVER) ? broadcast<uintM>(yi) < HcondV : VReg<uintM>();
        if((flags&PADVER)&&(flags&PADHOR))
        {
            VReg<intC> antiSpill = horIdxs; //TODO(ch): remove it when snippet management will be better.
            horIdxs = select(vertMask, antiSpill, reinterpret<intC>(WcondV));
        }
        for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
        {
            VReg<_Tp> loadedVector;
            if(lrow < MULTI_H)
                loadedVector.copyidx(vertMaxes[lrow][vRegNum]);
            loadVector(vertMaxesPtr, vRegNum * CTX.vbytes(), loadedVector, horIdxs, vertMask, WcondV, lvflags | (lrow >= MULTI_H ? INITDEST : 0) | (vRegNum > 0 ? PREINCREMENT_IDXS : 0));
            if(lrow < MULTI_H - 1)
            {
                for(int prow = 0; prow<lrow; prow++)
                    vertMaxes[prow][vRegNum] = max(vertMaxes[prow][vRegNum], loadedVector);
            }
            else
            {
                if(lrow >= kh && (lrow - kh) < (MULTI_H - 1))
                    vertMaxes[lrow - kh][vRegNum] = max(vertMaxes[lrow - kh][vRegNum], vertMaxes[MULTI_H - 1][vRegNum]);
                vertMaxes[MULTI_H - 1][vRegNum] = max(vertMaxes[MULTI_H - 1][vRegNum], loadedVector);
            }
        }
        if(lrow + 1 < (MULTI_H - 1) * stride_y + kh)
        {
            vertMaxesPtr += rstride;
            if(flags&PADVER)
                yi+=1;
        }
    }
}

template<typename _Tp>
void MaxpoolGenerator<_Tp>::multilineInitLast(const VReg<uintM>& HcondV, const VReg<uintM>& WcondV, IReg& yi_, IReg& x, IReg& base, std::vector<std::vector<VReg<_Tp>>>& vertMaxes, int flags)
{
    USE_CONTEXT_(CTX);
    int lvflags = flags&(PADHOR|PADVER);
    IReg vertMaxesPtr = base + (x << elemshift);
    IReg yi = yi_;
    int vRegNum = horVecsPerOut-1;
    VReg<intC> horIdxs_ = (flags&PADHOR) ? broadcast<intC>(x+vRegNum*lanes) + countingPattern : VReg<intC>();
    for(int lrow = 0; lrow < (MULTI_H - 1) * stride_y + kh; lrow++)
    {
        VReg<uintM> vertMask = (flags&PADVER) ? broadcast<uintM>(yi) < HcondV : VReg<uintM>();
        VReg<intC> horIdxs;
        horIdxs.copyidx((flags&PADVER)&&(flags&PADHOR)? select(vertMask, horIdxs_, reinterpret<intC>(WcondV)): horIdxs_);
        VReg<_Tp> loadedVector;
        if(lrow < MULTI_H)
            loadedVector.copyidx(vertMaxes[lrow][vRegNum]);
        loadVector(vertMaxesPtr, vRegNum * CTX.vbytes(), loadedVector, horIdxs, vertMask, WcondV, lvflags | (lrow >= MULTI_H ? INITDEST : 0));
        if(lrow < MULTI_H - 1)
        {
            for(int prow = 0; prow<lrow; prow++)
                vertMaxes[prow][vRegNum] = max(vertMaxes[prow][vRegNum], loadedVector);
        }
        else
        {
            if(lrow >= kh && (lrow - kh) < (MULTI_H - 1))
                vertMaxes[lrow - kh][vRegNum] = max(vertMaxes[lrow - kh][vRegNum], vertMaxes[MULTI_H - 1][vRegNum]);
            vertMaxes[MULTI_H - 1][vRegNum] = max(vertMaxes[MULTI_H - 1][vRegNum], loadedVector);
        }

        if(lrow + 1 < (MULTI_H - 1) * stride_y + kh)
        {
            vertMaxesPtr += rstride;
            if(flags&PADVER)
                yi+=1;
        }
    }
}

template<typename _Tp>
void MaxpoolGenerator<_Tp>::onelineInit(const VReg<uintM>& WcondV, IReg& yi_, IReg& x, IReg& base, std::vector<loops::VReg<_Tp>>& vertMaxes, int flags)
{
    USE_CONTEXT_(CTX);
    int lvflags = flags&PADHOR;
    IReg krow = CONST_(0);
    VReg<uintM> dummy;
    IReg vertMaxesPtr = base + (x << elemshift);
    IReg yi = yi_;
    WHILE_(krow<kh)
    {
        VReg<intC> horIdxs = flags&PADHOR ? broadcast<intC>(x) + countingPattern : VReg<intC>();
        for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
        {
            VReg<_Tp> loadedVector;
            if(flags&PADVER)
            {
                loadedVector.copyidx(VCONST_(_Tp, 0));
                IF_(ult(yi,H))
                {
                    loadVector(vertMaxesPtr, vRegNum*CTX.vbytes(), loadedVector, horIdxs, dummy, WcondV, lvflags | (vRegNum?PREINCREMENT_IDXS:0));
                }
            }
            else
                loadVector(vertMaxesPtr, vRegNum*CTX.vbytes(), loadedVector, horIdxs, dummy, WcondV, lvflags | INITDEST | (vRegNum?PREINCREMENT_IDXS:0));

            IF_(krow > 0)
                vertMaxes[vRegNum] = max(vertMaxes[vRegNum], loadedVector);
            ELSE_
                vertMaxes[vRegNum] = loadedVector; 
        }
        vertMaxesPtr += rstride;
        if (flags&PADVER)
            yi += 1;
        krow += 1;
    }
}

template<typename _Tp>
void MaxpoolGenerator<_Tp>::onelineInitLast(const VReg<uintM>& WcondV, IReg& yi_, IReg& x, IReg& base, std::vector<loops::VReg<_Tp>>& vertMaxes, int flags)
{
    USE_CONTEXT_(CTX);
    IReg yi = yi_;
    int lvflags = flags&PADHOR;
    VReg<uintM> dummy;
    int lastVRegNum = horVecsPerOut-1;
    IReg vertMaxesPtr = base;
    IReg krow = CONST_(0);
    VReg<intC> horIdxs = flags&PADHOR ? broadcast<intC>(x+(lastVRegNum*lanes)) + countingPattern : VReg<intC>();
    
    WHILE_(krow<kh)
    {
        VReg<_Tp> loadedVector;
        if(flags&PADVER)
        {
            loadedVector.copyidx(VCONST_(_Tp, 0));
            IF_(ult(yi,H))
            {
                loadVector(vertMaxesPtr, lastVRegNum*CTX.vbytes(), loadedVector, horIdxs, dummy, WcondV, lvflags);
            }
        }
        else
            loadVector(vertMaxesPtr, lastVRegNum*CTX.vbytes(), loadedVector, horIdxs, dummy, WcondV, lvflags | INITDEST);

        IF_(krow > 0)
            vertMaxes[lastVRegNum] = max(vertMaxes[lastVRegNum], loadedVector);
        ELSE_
            vertMaxes[lastVRegNum] = loadedVector; 

        vertMaxesPtr += rstride;
        if (flags&PADVER)
            yi += 1;
        krow += 1;
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
IReg MaxpoolGenerator<_Tp>::effective_const_mul(const IReg& m1, int m2)
{
    if(m2 == 1)
        return m1;
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
