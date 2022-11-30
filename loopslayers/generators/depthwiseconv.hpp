/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_DEPTHWISECONV_HPP__
#define __LOOPS_DEPTHWISECONV_HPP__

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

template<typename _Tp> struct DWCGenTraits {};

template<> struct DWCGenTraits<float> {
    typedef dwconv_f32_t dwconv_t;
    inline static std::string typestring(){return "fp32";}
};

template<> struct DWCGenTraits<f16_t> {
    typedef dwconv_f16_t dwconv_t;
    inline static std::string typestring(){return "fp16";}
};

template<typename _Tp>
class DepthwiseconvGenerator
{
    typedef typename ElemTraits<_Tp>::masktype uintM;
    typedef typename ElemTraits<_Tp>::countertype intC;
public:
    DepthwiseconvGenerator(Context aCTX) : CTX(aCTX), m_done(false) {}
    typename DWCGenTraits<_Tp>::dwconv_t generate(int kh_, int kw_, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x, int activation_type, float alpha);
    dwc_algs_limits calc_dwc_algs_limits(int NC, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x);
private:
    bool m_done; 
    Context CTX;
    enum { MULTI_H = 3 };
    enum { PADHOR = 1, PADVER = 2, INITDEST = 4, PREINCREMENT_IDXS = 8 };
    void multilineHandler(const VReg<uintM>& HcondV, const VReg<uintM>& WcondV, IReg& yi, IReg& xo, const IReg& xi, IReg& base, const IReg& result_rs, const IReg& rstride, int flags);
    void onlylineHandler(const VReg<uintM>& WcondV, IReg& yi, IReg& xo, const IReg& xi, IReg& base, const IReg& result_rs, int flags);
    void loadVector(const IReg& base, int64_t offset, VReg<_Tp>& dest, VReg<intC>& horIdxs, const VReg<uintM>& verMask, const VReg<uintM>& WcondV, int flags = 0);
    void loadVectorStride(const IReg& base, std::vector<VReg<_Tp> >& dest, VReg<intC>& horIdxs, const VReg<uintM>& verMask, const VReg<uintM>& WcondV, int retsneeded, int flags = 0);
    IReg effective_const_mul(const IReg& m1, int m2);

    //Common parameters and registers
    int kh, kw, elemsize, elemshift, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, activation_type;
    IReg H, W, kernel, W0;
    float alpha;
    std::vector<VReg<_Tp> > vkernel;
    VReg<intC> countingPattern, idx_step;
    VReg<_Tp> vbias, valpha, v0, v6;
    VReg<_Tp> activationFunction(VReg<_Tp>& res);

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
typename DWCGenTraits<_Tp>::dwconv_t DepthwiseconvGenerator<_Tp>::generate(int kh_, int kw_, int padding_top_, int padding_left_, int padding_bottom_, int padding_right_, int stride_y_, int stride_x_, int activation_type_, float alpha_)
{
    if(m_done)
        throw std::runtime_error("One generator object can create only one function. Create another generator.");
    kh = kh_; kw = kw_;
    elemsize = sizeof(_Tp);
    elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
    padding_top = padding_top_; padding_left = padding_left_; padding_bottom = padding_bottom_; padding_right = padding_right_;
    stride_y = stride_y_; stride_x = stride_x_;
    activation_type = activation_type_;
    alpha = alpha_;
    if(alpha == 1) 
        activation_type = ACT_NONE;
    std::string funcname = std::string("depthwise_convolution")+DWCGenTraits<_Tp>::typestring()+"_kH";
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
        return (typename DWCGenTraits<_Tp>::dwconv_t)(CTX.getFunc(funcname).ptr());
    }
    size_t kernelRegsAmount = kh*kw;
    kernelRegsAmount = kernelRegsAmount/CTX.vlanes<_Tp>() + (kernelRegsAmount%CTX.vlanes<_Tp>()?1:0);
    vkernel.resize(kernelRegsAmount, VReg<_Tp>());
    IReg data, bias, C, NC, kCS, result, H0, algsLimits;
    USE_CONTEXT_(CTX);
    STARTFUNC_(funcname, &data, &kernel, &bias, &H, &W, &C, &NC, &kCS, &result, &H0, &W0, &algsLimits)
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
            for(int lane = 1; lane < CTX.vlanes<_Tp>(); lane++)
                setlane(countingPattern, lane, CONST_(lane*stride_x));
            idx_step.copyidx(VCONST_(intC, (kw <= 3 || stride_x > 1 ? 1 : CTX.vlanes<_Tp>())));
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

        int padded_ksize = kw*kh;
        padded_ksize = (padded_ksize + CTX.vlanes<_Tp>() - 1) & -CTX.vlanes<_Tp>();
        padded_ksize *= elemsize;
        kernel += kCS * padded_ksize;
        bias += kCS * elemsize;
        IReg channel = CONST_(0);
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
            vbias.copyidx(broadcast<_Tp>(load_<_Tp>(bias)));

            IReg y = CONST_(0);
            IReg yonelineEnd = select(yms > y, yms , H0); //SIMD + scalar.
            for(int kregnum = 0; kregnum < kernelRegsAmount; kregnum++)
                vkernel[kregnum].copyidx(loadvec<_Tp>(kernel, kregnum * CTX.vbytes()));
            WHILE_(y < H0)
            {
                IF_(y == yms)
                {
                    WHILE_(y < yme)
                    {
                        IReg data_rs = data + W * (padver ? (effective_const_mul(y, stride_y) - padding_top) << elemshift: effective_const_mul(y, elemsize*stride_y));
                        IReg result_rs = result + (W0 << elemshift) * y;
                        IReg xo = CONST_(0);
                        IReg Hcond = padver ? max(H - ((MULTI_H - 1) * stride_y + kh - 1), CONST_(0)) : IReg(); 
                        IReg Wcond = padhor ? max(W - (kw + (CTX.vlanes<_Tp>() - 1) * stride_x - 1), CONST_(0)): IReg();
                        VReg<uintM> WcondV = padhor ? broadcast<uintM>(W) : VReg<uintM>();
                        VReg<uintM> HcondV = padver ? broadcast<uintM>(H) : VReg<uintM>();
                        IReg hldx = W0 - CTX.vlanes<_Tp>();
                        IReg roffset = W0<<elemshift;
                        WHILE_(xo < W0)
                        {
                            xo = select(xo > hldx , hldx , xo);
                            IReg xi;
                            xi.copyidx(stride_x == 1 && padding_left == 0 ? xo : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
                            IReg data__ = data_rs + (xi << elemshift);
                            if(padhor||padver)
                            {
                                IReg yi = padver ? effective_const_mul(y, stride_y) - padding_top : IReg();
                                IReg xcond = padver&&padhor ? select(ult(yi,Hcond), xi, Wcond) : xi;
                                IF_(padhor?(ult(xcond, Wcond)):ult(yi, Hcond))
                                {
                                    multilineHandler(HcondV, WcondV, yi, xo, xi, data__, result_rs, roffset, 0);
                                    CONTINUE_;
                                }
                                multilineHandler(HcondV, WcondV, yi, xo, xi, data__, result_rs, roffset, handlerFlags);
                            }
                            else 
                                multilineHandler(HcondV, WcondV, y, xo, xi, data__, result_rs, roffset, 0);
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
                            IReg Wcond = padhor ? max(W - (kw + (CTX.vlanes<_Tp>() - 1) * stride_x - 1), CONST_(0)): IReg();
                            IReg Hcond = padver ? max(H - (kh - 1), CONST_(0)) : IReg();
                            VReg<uintM> WcondV = padhor ? broadcast<uintM>(W) : VReg<uintM>();
                            WHILE_(xo < xie)
                            {
                                IReg xi;
                                xi.copyidx(stride_x == 1 && padding_left == 0 ? xo : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
                                IReg data__ = data_rs + (xi << elemshift);
                                if(padhor||padver)
                                {
                                    IReg yi = padver ? effective_const_mul(y, stride_y) - padding_top : IReg();
                                    IReg xcond;
                                    xcond.copyidx(padhor && padver ? select(ult(yi, Hcond), xi, Wcond): xi);
                                    IF_(padhor?ult(xcond,Wcond):ult(yi, Hcond))
                                    {
                                        onlylineHandler(WcondV, yi, xo, xi, data__, result_rs, 0);
                                        CONTINUE_;
                                    }
                                    onlylineHandler(WcondV, yi, xo, xi, data__, result_rs, handlerFlags);
                                }
                                else 
                                    onlylineHandler(WcondV, y, xo, xi, data__, result_rs, 0);
                            }
                            scalarEnd = W0;
                        }
                        WHILE_(xo < scalarEnd)
                        {
                            IReg xi;
                            xi.copyidx(stride_x == 1 && padding_left == 0 ? xo : (padding_left ? effective_const_mul(xo, stride_x) - padding_left : effective_const_mul(xo, stride_x)));
                            VReg<_Tp> vres = vbias;
                            IReg data__ = data_rs + (xi << elemshift);
                            IReg kernel__ = kernel;
                            IReg kcol = CONST_(0);
                            IReg krow = CONST_(0);
                            IReg istride = (W - (kw-1)) << elemshift;
                            //We are working here at ends of arrays and there possibility of reading/ writing end of
                            //page, thus it's better to not use loadlane and storelane instructions: 
                            //even if base+(lane + 1)*elemsize  is inside of valid memory, tail can be outside and 
                            //this can cause memory exception. So, we are using general load/store operation instead. 
                            WHILE_(krow < kh)
                            {
                                if(padhor||padver)
                                {
                                    IReg ex = xi + kcol;
                                    IReg ey = (padver ? effective_const_mul(y, stride_y) - padding_top : y) + krow;
                                    select(ex < 0, W, ex);
                                    select(ey < 0, H, ey);
                                    IF_(ult(ex,W))
                                    {
                                        IF_(ult(ey,H))
                                        {
                                            VReg<_Tp> justloaded = broadcast<_Tp>(load_<_Tp>(data__));
                                            VReg<_Tp> w = broadcast<_Tp>(load_<_Tp>(kernel__));
                                            {
                                                VReg<_Tp> antiSpill = vres; //TODO(ch): remove it when snippet management will be better.
                                                vres = fma(antiSpill, justloaded, w);
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    VReg<_Tp> justloaded = broadcast<_Tp>(load_<_Tp>(data__));
                                    VReg<_Tp> w = broadcast<_Tp>(load_<_Tp>(kernel__));
                                    {
                                        VReg<_Tp> antiSpill = vres; //TODO(ch): remove it when snippet management will be better.
                                        vres = fma(antiSpill, justloaded, w);
                                    }
                                }
                                kernel__ += elemsize;
                                kcol = kcol + 1;
                                data__ = select(kcol == kw, data__ + istride, data__ + elemsize);
                                krow = select(kcol == kw, krow + 1, krow);
                                kcol = select(kcol == kw, 0, kcol);
                            }
                            IReg roffset = xo << elemshift;
                            store_<_Tp>(result_rs + roffset, getlane<_Tp>(activationFunction(vres), 0)); 
                            xo += 1;
                        }
                    }
                    y += 1;
                }
            }
            data += H * W << elemshift;
            result += H0 * W0 << elemshift;  
            kernel += padded_ksize;
            bias += elemsize;
            channel = channel + 1;
            IReg channelK = (channel + kCS) % C;
            kernel = select(channelK == 0, kernel - C * padded_ksize, kernel);
            bias = select(channelK == 0, bias - (C << elemshift), bias);
        }
        RETURN_(0);
    }
    m_done = true;
    return (typename DWCGenTraits<_Tp>::dwconv_t)(CTX.getFunc(funcname).ptr());
}

template<typename _Tp>
void DepthwiseconvGenerator<_Tp>::multilineHandler(const VReg<uintM>& HcondV, const VReg<uintM>& WcondV, IReg& yi, IReg& xo, const IReg& xi, IReg& base, const IReg& result_rs, const IReg& rstride, int flags)
{
    USE_CONTEXT_(CTX);
    std::vector<VReg<_Tp> > vres(MULTI_H, VReg<_Tp>());
    for(int rnum = 0; rnum<MULTI_H; rnum++)
        vres[rnum].copyidx(VReg<_Tp>(vbias)); 
    int lvflags = flags&(PADHOR|PADVER);

    if(kw <= 3 || stride_x == 2)
    {//Code without using exts.
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
            VReg<intC> horIdxs = (flags&PADHOR) ? broadcast<intC>(xi) + countingPattern : VReg<intC>();
            VReg<uintM> verMask = (flags&PADVER) ? broadcast<uintM>(yi) < HcondV : VReg<uintM>();
            if((flags&PADVER)&&(flags&PADHOR))
            {
                VReg<intC> antiSpill = horIdxs; //TODO(ch): remove it when snippet management will be better.
                horIdxs = select(verMask, antiSpill, reinterpret<intC>(WcondV));
            }
            std::vector< VReg<_Tp> > all_loaded_cols(upMultipleOf(kw, stride_x), VReg<_Tp>());
            for(int kcol = 0; kcol < kw; kcol++) 
            {
                VReg<_Tp> toAdd;
                if(kcol%stride_x == 0)
                {
                    std::vector<VReg<_Tp> > justloaded(stride_x, VReg<_Tp>());
                    loadVectorStride(base, justloaded, horIdxs, verMask, WcondV, std::min(stride_x, kw - kcol), (kcol ? PREINCREMENT_IDXS : 0) | lvflags);
                    for(int dnum = 0; dnum < stride_x; dnum++)
                        all_loaded_cols[kcol + dnum].copyidx(justloaded[dnum]);
                    if(kcol + stride_x < kw) 
                        base += stride_x * elemsize;
                }
                for(int lineNum = 0; lineNum < MULTI_H; lineNum++)
                { 
                    const int krow = lrow - lineNum * stride_y;
                    if(krow >= 0 && krow < kh)
                    {
                        const int kerelemnum = krow*kw + kcol;
                        vres[lineNum] = fma(vres[lineNum], all_loaded_cols[kcol], vkernel[kerelemnum/CTX.vlanes<_Tp>()], kerelemnum%CTX.vlanes<_Tp>());
                    }
                }
            }
            if(lrow + 1 != (MULTI_H - 1) * stride_y + kh) //Not last loaded row
            {
                base += (W<<elemshift) - (downMultipleOf(kw-1, stride_x) * elemsize);
                if(flags&PADVER)
                    yi += stride_multiplier;
            }
        }
    }
    else if(stride_x == 1)
    {//Code uses exts for doing lesser loads.
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
            VReg<intC> horIdxs = (flags&PADHOR) ? broadcast<intC>(xi) + countingPattern : VReg<intC>();
            VReg<uintM> verMask = (flags&PADVER) ? broadcast<uintM>(yi) < HcondV : VReg<uintM>();
            if((flags&PADVER)&&(flags&PADHOR))
            {
                VReg<intC> antiSpill = horIdxs; //TODO(ch): remove it when snippet management will be better.
                horIdxs = select(verMask, antiSpill, reinterpret<intC>(WcondV));
            }
            VReg<_Tp> loadedHalf0, loadedHalf1;
            loadVector(base, 0, loadedHalf0, horIdxs, verMask, WcondV, INITDEST | lvflags);
            if(kw > 1)
                loadVector(base, CTX.vbytes(), loadedHalf1, horIdxs, verMask, WcondV, INITDEST | PREINCREMENT_IDXS | lvflags);
            for(int kcol = 0; kcol < kw; kcol++) 
            {
                VReg<_Tp> toAdd;
                if(kcol%CTX.vlanes<_Tp>() == 0 && kcol > 0)
                {
                    VReg<_Tp> inter;
                    inter.copyidx(loadedHalf0);
                    loadedHalf0.copyidx(loadedHalf1);
                    loadedHalf1.copyidx(inter);
                    if(kcol + 1 < kw)
                        loadVector(base, kcol*elemsize+CTX.vbytes(), loadedHalf1, horIdxs, verMask, WcondV, PREINCREMENT_IDXS | lvflags);
                    toAdd.copyidx(loadedHalf0);
                }
                else
                    toAdd.copyidx(ext(loadedHalf0, loadedHalf1, kcol%CTX.vlanes<_Tp>()));
                for(int lineNum = 0; lineNum < MULTI_H; lineNum++)
                { 
                    const int krow = lrow - lineNum * stride_y;
                    if(krow >= 0 && krow < kh)
                    {
                        const int kerelemnum = krow*kw + kcol;
                        vres[lineNum] = fma(vres[lineNum], toAdd, vkernel[kerelemnum/CTX.vlanes<_Tp>()], kerelemnum%CTX.vlanes<_Tp>());
                    }
                }
            }
            if(lrow + 1 != (MULTI_H - 1) * stride_y + kh) //Not last loaded row
            {
                base += effective_const_mul(W, elemsize * stride_multiplier);
                if(flags&PADVER)
                    yi += stride_multiplier;
            }
        }
    }
    IReg roffset = xo << elemshift;
    for(int lineNum = 0; lineNum < MULTI_H; lineNum++)
    {
        storevec<_Tp>(result_rs, roffset, activationFunction(vres[lineNum]));
        if(lineNum + 1 < MULTI_H)
            roffset += rstride;
    }
    xo += CTX.vlanes<_Tp>();
}

template<typename _Tp>
void DepthwiseconvGenerator<_Tp>::onlylineHandler(const VReg<uintM>& WcondV, IReg& yi, IReg& xo, const IReg& xi, IReg& base, const IReg& result_rs, int flags)
{
    USE_CONTEXT_(CTX);
    VReg<_Tp> vres = vbias;
    int lvflags = flags&PADHOR;
    if(kw <= 3 || stride_x == 2)
    {//Code without using exts. 
        if(!(flags&(PADHOR|PADVER)))
        {//To generate code without vertical check loop
            for(int krow = 0; krow < kh; krow++) 
            {
                std::vector< VReg<_Tp> > loaded_by_one_instruction(upMultipleOf(kw, stride_x), VReg<_Tp>());
                for(int kcol = 0; kcol < kw; kcol++)
                {
                    if(kcol%stride_x == 0)
                    {
                        if(stride_x == 1)
                        {
                            loaded_by_one_instruction[kcol].copyidx(loadvec<_Tp>(base));
                        }
                        else
                        {
                            Assert(stride_x == 2);
                            loadvec_deinterleave2<_Tp>(loaded_by_one_instruction[kcol], loaded_by_one_instruction[kcol + 1], base);
                        }
                        if(kcol + stride_x < kw) 
                            base += stride_x * elemsize;
                    }
                    const int kerelemnum = krow*kw + kcol;
                    vres = fma(vres, loaded_by_one_instruction[kcol], vkernel[kerelemnum/CTX.vlanes<_Tp>()], kerelemnum%CTX.vlanes<_Tp>());
                }
                if(krow + 1 < kh) //Not last loaded row
                    base += (W<<elemshift) - (downMultipleOf(kw-1, stride_x) * elemsize);
            }
        }
        else
        {//Code with vertical check loop
            IReg krow = CONST_(0);
            WHILE_(krow<kh)
            {
                if(flags&PADVER)
                {
                    IF_(uge(yi,H))
                    {
                        base += W << elemshift;
                        yi += 1;
                        krow += 1;
                        CONTINUE_;
                    }
                }
                IReg kptr = kernel + krow * (kw * elemsize);
                VReg<uintM> dummy;
                VReg<intC> horIdxs = flags&PADHOR ? broadcast<intC>(xi) + countingPattern : VReg<intC>();
                std::vector< VReg<_Tp> > prev_loaded(stride_x, VReg<_Tp>());
                for(int kcol = 0; kcol < kw; kcol++) 
                {
                    std::vector< VReg<_Tp> > loaded_by_one_instruction(stride_x, VReg<_Tp>());
                    if(kcol%stride_x == 0)
                    {
                        loadVectorStride(base, loaded_by_one_instruction, horIdxs, dummy, WcondV, std::min(stride_x, kw - kcol), (kcol ? PREINCREMENT_IDXS : 0) | lvflags); 
                        for(int cnum = 0; cnum < stride_x; cnum++)
                            prev_loaded[cnum].copyidx(loaded_by_one_instruction[cnum]);
                        if(kcol + stride_x < kw) 
                            base += stride_x * elemsize;
                    }
                    VReg<_Tp> toAdd;
                    toAdd.copyidx(prev_loaded[kcol%stride_x]);
                    VReg<_Tp> w = broadcast<_Tp>(load_<_Tp>(kptr));
                    {
                        VReg<_Tp> antiSpill = vres; //TODO(ch): remove it when snippet management will be better.
                        vres = fma(antiSpill, toAdd, w);
                    }
                    kptr += elemsize;
                }
                base += (W<<elemshift) - (downMultipleOf(kw-1, stride_x) * elemsize);
                if(flags&PADVER)
                    yi += 1;
                krow += 1;
            }
        }
    }
    else
    {//Code uses exts for doing lesser loads.
        if(!(flags&(PADHOR|PADVER)))
        {//To generate code without vertical check loop
            for(int krow = 0; krow < kh; krow++) 
            {
                VReg<uintM> dummy;
                VReg<intC> dummy2;
                VReg<_Tp> loadedHalf0, loadedHalf1;
                loadVector(base, 0, loadedHalf0, dummy2, dummy, WcondV, INITDEST);
                if(kw > 1)
                    loadVector(base, CTX.vbytes(), loadedHalf1, dummy2, dummy, WcondV, INITDEST);
                for(int kcol = 0; kcol < kw; kcol++)
                {
                    VReg<_Tp> spliced;
                    VReg<_Tp> toAdd;
                    if(kcol%CTX.vlanes<_Tp>() == 0 && kcol > 0)
                    {
                        VReg<_Tp> interm;
                        interm.copyidx(loadedHalf0);
                        loadedHalf0.copyidx(loadedHalf1);
                        loadedHalf1.copyidx(interm);
                        if(kcol + 1 < kw)
                            loadVector(base, kcol*elemsize+CTX.vbytes(), loadedHalf1, dummy2, dummy, WcondV, 0);
                        toAdd.copyidx(loadedHalf0);
                    }
                    else
                        toAdd.copyidx(ext(loadedHalf0, loadedHalf1, kcol%CTX.vlanes<_Tp>()));
                    const int kerelemnum = krow*kw + kcol;
                    vres = fma(vres, toAdd, vkernel[kerelemnum/CTX.vlanes<_Tp>()], kerelemnum%CTX.vlanes<_Tp>());
                }
                if(krow + 1 < kh) //Not last loaded row
                    base += W << elemshift;
            }
        }
        else
        {//Code with vertical check loop
            IReg krow = CONST_(0);
            WHILE_(krow<kh)
            {
                if(flags&PADVER)
                {
                    IF_(uge(yi,H))
                    {
                        base += W << elemshift;
                        yi += 1;
                        krow += 1;
                        CONTINUE_;
                    }
                }
                IReg kptr = kernel + krow * (kw * elemsize);
                VReg<uintM> dummy;
                VReg<intC> horIdxs = flags&PADHOR ? broadcast<intC>(xi) + countingPattern : VReg<intC>();
                VReg<_Tp> loadedHalf0, loadedHalf1;
                loadVector(base, 0, loadedHalf0, horIdxs, dummy, WcondV, INITDEST | lvflags);
                if(kw > 1)
                    loadVector(base, CTX.vbytes(), loadedHalf1, horIdxs, dummy, WcondV, INITDEST | PREINCREMENT_IDXS | lvflags);

                for(int kcol = 0; kcol < kw; kcol++) 
                {
                    VReg<_Tp> spliced;
                    VReg<_Tp> toAdd;
                    if(kcol%CTX.vlanes<_Tp>() == 0 && kcol > 0)
                    {
                        VReg<_Tp> interm;
                        interm.copyidx(loadedHalf0);
                        loadedHalf0.copyidx(loadedHalf1);
                        loadedHalf1.copyidx(interm);
                        if(kcol + 1 < kw)
                            loadVector(base, kcol*elemsize+CTX.vbytes(), loadedHalf1, horIdxs, dummy, WcondV, PREINCREMENT_IDXS | lvflags);
                        toAdd.copyidx(loadedHalf0);
                    }
                    else
                        toAdd.copyidx(ext(loadedHalf0, loadedHalf1, kcol%CTX.vlanes<_Tp>()));
                    VReg<_Tp> w = broadcast<_Tp>(load_<_Tp>(kptr));
                    {
                        VReg<_Tp> antiSpill = vres; //TODO(ch): remove it when snippet management will be better.
                        vres = fma(antiSpill, toAdd, w);
                    }
                    kptr += elemsize;
                }
                base += W << elemshift;
                if(flags&PADVER)
                    yi += 1;
                krow += 1;
            }        
        }
    }
    storevec<_Tp>(result_rs, (xo << elemshift), activationFunction(vres));
    xo += CTX.vlanes<_Tp>();
}

template<typename _Tp>
void DepthwiseconvGenerator<_Tp>::loadVector(const IReg& base, int64_t offset, VReg<_Tp>& dest, VReg<intC>& horIdxs, const VReg<uintM>& verMask, const VReg<uintM>& WcondV, int flags)
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
void DepthwiseconvGenerator<_Tp>::loadVectorStride(const IReg& base, std::vector<VReg<_Tp> >& dest, VReg<intC>& horIdxs, const VReg<uintM>& verMask, const VReg<uintM>& WcondV, int retsneeded, int flags)
{
    USE_CONTEXT_(CTX);
    Assert(stride_x == 1 || stride_x == 2);
    if(stride_x == 1) 
        dest[0].copyidx(loadvec<_Tp>(base));
    else
    {
        Assert(stride_x == 2);
        loadvec_deinterleave2<_Tp>(dest[0], dest[1], base);
    }
    for(int dnum = 0; dnum < retsneeded; dnum++) 
        if(flags&(PADHOR|PADVER))
        {
            if((flags & PADVER) && !(flags & PADHOR))
            {
                dest[dnum] = reinterpret<_Tp>( verMask & reinterpret<uintM>(dest[dnum]));
            }
            else
            {
                if(((flags&PREINCREMENT_IDXS) || (dnum > 0)) && (flags & PADHOR))
                    horIdxs += idx_step;
                VReg<uintM> mask;
                mask.copyidx((flags & PADHOR) ? (reinterpret<uintM>(horIdxs) < WcondV) : verMask);
                dest[dnum] = reinterpret<_Tp>( mask & reinterpret<uintM>(dest[dnum]));
            }
        }
}

template<typename _Tp>
IReg DepthwiseconvGenerator<_Tp>::effective_const_mul(const IReg& m1, int m2)
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

template<typename _Tp>
VReg<_Tp> DepthwiseconvGenerator<_Tp>::activationFunction(VReg<_Tp>& res)
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
dwc_algs_limits DepthwiseconvGenerator<_Tp>::calc_dwc_algs_limits(int NC, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x)
{
    int Cms, Cme;
    int lanes = CTX.vlanes<_Tp>(); 
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
};
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
#endif //__LOOPS_DEPTHWISECONV_HPP__
