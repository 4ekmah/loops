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
#include "../test/tests.hpp"

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
    typename DWCGenTraits<_Tp>::dwconv_t generate(int kh_, int kw_, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type, float alpha);
    dwc_algs_limits calc_dwc_algs_limits(int NC, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right);
private:
    bool m_done; 
    Context CTX;
    enum { MULTI_H = 3 };
    enum { PADHOR = 1, PADVER = 2, INITDEST = 4, PREINCREMENT_IDXS = 8, PADSHIFTRES = 16, MULTILINE = 32 };
    void multilineHandler(const VReg<uintM>& HcondV, const VReg<uintM>& WcondV, IReg& yi, IReg& x, IReg& base, const IReg& result_rs, int flags);
    void onlylineHandler(const VReg<uintM>& WcondV, IReg& yi, IReg& x, IReg& base, const IReg& result_rs, int flags);
    void loadVector(const IReg& base, int64_t offset, VReg<_Tp>& dest, VReg<intC>& horIdxs, const VReg<uintM>& verMask, const VReg<uintM>& WcondV, int flags = 0);

    //Common parameters and registers
    int kh, kw, elemsize, elemshift, padding_top, padding_left, padding_bottom, padding_right, activation_type;
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
    //Cf*H*W + (y + ys)*W + X >= 0
    inline int downY(int C, int H, int W, int Cf, int ys, int x)
    {
        Cf = std::max(Cf,0);
        return downBorder(-Cf*H*W - x - ys * W, W);
    }
    //upper border for all y, satisfies inequality
    //Cf*H*W + (y + ys)*W + X < C*W*H
    inline int upperY(int C, int H, int W, int Cf, int ys, int x)
    {
        return upperBorder((C-Cf)*H*W - x - ys * W, W);
    }
    //upper border for all y, satisfies inequality
    //Cf*H*W + (y + ys)*W + X < C*W*H, where y is a multiple of M, started from y0:
    //y = M * r + y0 
    inline int upperY(int C, int H, int W, int Cf, int ys, int M, int y0, int x)
    { return M * upperBorder((C-Cf)*H*W - x - (ys + y0) * W, W * M) + y0; }
    //lower border for all x, satisfies inequality
    //Cf*H*W + Yf*W + x + xs  >= 0
    inline int downX(int C, int H, int W, int Cf, int Yf, int xs)
    {
        Cf = std::max(Cf,0);
        Yf = std::max(Yf,0);
        return downBorder(-Cf*H*W - Yf * W - xs, 1);
    }
    //upper border for all y, satisfies inequality
    //Cf*H*W + Yf*W + x + xf < C*W*H
    inline int upperX(int C, int H, int W, int Cf, int Yf, int xs)
    { return upperBorder((C-Cf)*H*W - Yf * W - xs, 1); }
    //upper border for all y, satisfies inequality
    //Cf*H*W + Yf*W + x + xf < C*W*H, where x is a multiple of M, started from x0:
    //y = M * r + y0 
    inline int upperX(int C, int H, int W, int Cf, int Yf, int xs, int M, int x0)
    { return M * upperBorder((C-Cf)*H*W - Yf * W - xs - x0, M) + x0; }
};

template<typename _Tp>
typename DWCGenTraits<_Tp>::dwconv_t DepthwiseconvGenerator<_Tp>::generate(int kh_, int kw_, int padding_top_, int padding_left_, int padding_bottom_, int padding_right_, int activation_type_, float alpha_)
{
    if(m_done)
        throw std::runtime_error("One generator object can create only one function. Create another generator.");
    kh = kh_; kw = kw_;
    elemsize = sizeof(_Tp);
    elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
    padding_top = padding_top_; padding_left = padding_left_; padding_bottom = padding_bottom_; padding_right = padding_right_;
    activation_type = activation_type_;
    alpha = alpha_;
    if(alpha == 1) 
        activation_type = ACT_NONE;
    std::string funcname = std::string("depthwise_convolution")+DWCGenTraits<_Tp>::typestring()+"_kH";
    const bool padver = (padding_top || padding_bottom);
    const bool padhor = (padding_left || padding_right);
    int handlerFlags = (padhor ? PADHOR : 0) | (padver ? PADVER : 0); 
    int pshiftflag = (padhor ? PADSHIFTRES : 0);
    
    funcname += std::to_string(kh) + "_kW" + std::to_string(kw) + "_pT" + std::to_string(padding_top) + "_pL" + std::to_string(padding_left) + "_pB" + std::to_string(padding_bottom) + "_pR" + std::to_string(padding_right);
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
                setlane(countingPattern, lane, CONST_(lane));
            idx_step.copyidx(VCONST_(intC, CTX.vlanes<_Tp>()));
        }
        IReg Cms = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Cms));
        IReg Cme = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Cme));
        IReg Cis = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Cis));
        IReg Cie = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Cie));
        IReg Yms = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Yms));
        IReg Yme = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Yme));
        IReg Yis = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Yis));
        IReg Yie = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Yie));
        IReg Xis = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Xis)) - padding_left;
        IReg Xie = load_<int64_t>(algsLimits, offsetof(dwc_algs_limits, Xie)) - padding_left;

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
                        IReg data_rs = data + ((W * (padver ? y - padding_top: y)) << elemshift);
                        IReg result_rs = result + (W0 << elemshift) * y;
                        IReg xi = CONST_(-padding_left);
                        IReg Hcond = padver ? max(H - (kh + MULTI_H - 2), CONST_(0)) : IReg(); 
                        IReg Wcond = padhor ? max(W - (kw + CTX.vlanes<_Tp>() - 2), CONST_(0)) : IReg();
                        VReg<uintM> WcondV = padhor ? broadcast<uintM>(W) : VReg<uintM>();
                        VReg<uintM> HcondV = padver ? broadcast<uintM>(H) : VReg<uintM>();
                        IReg multilineendx;
                        multilineendx.copyidx(padding_left ? W0-padding_left : W0);
                        IReg hldx = (padding_left ? W0 - padding_left: W0) - CTX.vlanes<_Tp>();
                        WHILE_(xi < multilineendx)
                        {
                            xi = select(xi > hldx , hldx , xi);
                            IReg data__ = data_rs + (xi << elemshift);
                            if(padhor||padver)
                            {
                                IReg yi = padver ? y - padding_top : IReg();
                                IReg xcond = padver&&padhor ? select(ult(yi,Hcond), xi, Wcond) : xi;
                                IF_(padhor?(ult(xcond, Wcond)):ult(yi, Hcond))
                                {
                                    multilineHandler(HcondV, WcondV, yi, xi, data__, result_rs, pshiftflag);
                                    CONTINUE_;
                                }
                                multilineHandler(HcondV, WcondV, yi, xi, data__, result_rs, handlerFlags | pshiftflag);
                            }
                            else 
                                multilineHandler(HcondV, WcondV, y, xi, data__, result_rs, 0);
                        }
                        y += MULTI_H;
                    }
                    yonelineEnd = H0;
                }
                WHILE_(y < yonelineEnd)
                {
                    IReg xi = CONST_(-padding_left);
                    IReg W0mpl;
                    W0mpl.copyidx(padding_left? W0 - padding_left: W0);
                    IReg xis = select(y < yis, Xis, xi);
                    IReg xie = select(y >= yie, Xie, W0mpl);
                    IReg scalarEnd = select(xis > xi, xis, W0mpl);
                    IReg data_rs = data + ((W * (padver ? y - padding_top: y)) << elemshift);
                    IReg result_rs = result + (W0 << elemshift) * y;
                    WHILE_(xi < W0mpl)
                    {
                        IF_(xi == xis)
                        {
                            IReg Wcond = padhor ? max(W - (kw + CTX.vlanes<_Tp>() - 2), CONST_(0)): IReg();
                            IReg Hcond = padver ? max(H - (kh - 1), CONST_(0)) : IReg();
                            VReg<uintM> WcondV = padhor ? broadcast<uintM>(W) : VReg<uintM>();
                            WHILE_(xi < xie)
                            {
                                IReg data__ = data_rs + (xi << elemshift);
                                if(padhor||padver)
                                {
                                    IReg yi = padver ? y - padding_top : IReg();
                                    IReg xcond;
                                    xcond.copyidx(padhor && padver ? select(ult(yi, Hcond), xi, Wcond): xi);
                                    IF_(padhor?ult(xcond,Wcond):ult(yi, Hcond))
                                    {
                                        onlylineHandler(WcondV, yi, xi, data__, result_rs, pshiftflag);
                                        CONTINUE_;
                                    }
                                    onlylineHandler(WcondV, yi, xi, data__, result_rs, handlerFlags | pshiftflag);
                                }
                                else 
                                    onlylineHandler(WcondV, y, xi, data__, result_rs, 0);
                            }
                            scalarEnd = W0mpl;
                        }
                        WHILE_(xi < scalarEnd)
                        {
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
                                    IReg ey = (padver ? y - padding_top : y) + krow;
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
                            IReg roffset = (((padhor||padver) ? xi + padding_left : xi) << elemshift);
                            store_<_Tp>(result_rs + roffset, getlane<_Tp>(activationFunction(vres), 0));
                            xi += 1;
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
void DepthwiseconvGenerator<_Tp>::multilineHandler(const VReg<uintM>& HcondV, const VReg<uintM>& WcondV, IReg& yi, IReg& x, IReg& base, const IReg& result_rs, int flags)
{
    USE_CONTEXT_(CTX);
    std::vector<VReg<_Tp> > vres(MULTI_H, VReg<_Tp>());
    for(int rnum = 0; rnum<MULTI_H; rnum++)
        vres[rnum].copyidx(VReg<_Tp>(vbias)); 
    int lvflags = flags&(PADHOR|PADVER);
    if(kw < 4 && !(flags&(PADHOR|PADVER)))
    {//For 3x3 case approach with loading vectors one-by-one uses less operations.(*In central part of picture)
        for(int lrow = 0; lrow < kh + MULTI_H - 1; lrow++) 
        {
            for(int kcol = 0; kcol < kw; kcol++)
            {
                VReg<_Tp> loaded = loadvec<_Tp>(base);
                if(kcol + 1 < kw) 
                    base += elemsize;
                for(int lineNum = 0; lineNum < MULTI_H; lineNum++)
                { 
                    const int krow = lrow - lineNum;
                    if(krow >= 0 && krow < kh)
                    {
                        const int kerelemnum = krow*kw + kcol;
                        vres[lineNum] = fma(vres[lineNum], loaded, vkernel[kerelemnum/CTX.vlanes<_Tp>()], kerelemnum%CTX.vlanes<_Tp>());
                    }
                }
            }
            if(lrow + 2 < kh + MULTI_H) //Not last loaded row
                base += (W<<elemshift) - ((kw-1)*elemsize);
        }
    }
    else
    {
        for(int lrow = 0; lrow < kh + MULTI_H - 1; lrow++) 
        {
            VReg<intC> horIdxs = (flags&PADHOR) ? broadcast<intC>(x) + countingPattern : VReg<intC>();
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
                    const int krow = lrow - lineNum;
                    if(krow >= 0 && krow < kh)
                    {
                        const int kerelemnum = krow*kw + kcol;
                        vres[lineNum] = fma(vres[lineNum], toAdd, vkernel[kerelemnum/CTX.vlanes<_Tp>()], kerelemnum%CTX.vlanes<_Tp>());
                    }
                }
            }
            if(lrow + 2 < kh + MULTI_H) //Not last loaded row
            {
                base += W << elemshift;
                if(flags&PADVER)
                    yi += 1;
            }
        }
    }
    IReg roffset = (flags&PADSHIFTRES? x + padding_left : x) << elemshift;
    int outdiffW = padding_left + padding_right - kw + 1;
    IReg W0_;
    W0_.copyidx(outdiffW ? (outdiffW > 0 ? W+outdiffW: W-(-outdiffW)): W); //TODO(ch): fix it in interface of loops(there must be ability to add and sub negaitve numbers).
    IReg offstride = W0_ << elemshift;
    for(int lineNum = 0; lineNum < MULTI_H; lineNum++)
    {
        storevec<_Tp>(result_rs, roffset, activationFunction(vres[lineNum]));
        if(lineNum + 1 < MULTI_H)
            roffset += offstride;
    }
    x += CTX.vlanes<_Tp>();
}

template<typename _Tp>
void DepthwiseconvGenerator<_Tp>::onlylineHandler(const VReg<uintM>& WcondV, IReg& yi, IReg& x, IReg& base, const IReg& result_rs, int flags)
{
    USE_CONTEXT_(CTX);
    VReg<_Tp> vres = vbias;
    int lvflags = flags&PADHOR;
    IReg krow = CONST_(0);
    if(kw < 4 && !(flags&(PADHOR|PADVER)))
    {//For 3x3 case approach with loading vectors one-by-one uses less operations.(*In central part of picture)
        for(int krow = 0; krow < kh; krow++) 
        {
            for(int kcol = 0; kcol < kw; kcol++)
            {
                const int kerelemnum = krow*kw + kcol;
                VReg<_Tp> loaded = loadvec<_Tp>(base);
                if(kcol + 1 < kw) 
                    base += elemsize;
                vres = fma(vres, loaded, vkernel[kerelemnum/CTX.vlanes<_Tp>()], kerelemnum%CTX.vlanes<_Tp>());
            }
            if(krow + 1 < kh) //Not last loaded row
                base += (W<<elemshift) - ((kw-1)*elemsize);
        }
    }
    else
    {
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
            VReg<intC> horIdxs = flags&PADHOR ? broadcast<intC>(x) + countingPattern : VReg<intC>();
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
    IReg roffset = (flags&PADSHIFTRES? x + padding_left : x) << elemshift;
    storevec<_Tp>(result_rs, roffset, activationFunction(vres));
    x += CTX.vlanes<_Tp>();
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
dwc_algs_limits DepthwiseconvGenerator<_Tp>::calc_dwc_algs_limits(int NC, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right)
{
    int Cms, Cme;
    int lsimd = upMultipleOf(kw + CTX.vlanes<_Tp>() - 2, CTX.vlanes<_Tp>()) - 1;
    int XlastMulti = (W0 - padding_left - CTX.vlanes<_Tp>()) + lsimd;
    if(W0 >= CTX.vlanes<_Tp>())
    {
        Cms = downC(NC, H, W, -padding_top, -padding_left);
        int YlastMulti = (downMultipleOf(H0, MULTI_H) - padding_top + kh + MULTI_H - 2);
        Cme = upperC(NC, H, W, YlastMulti, XlastMulti);
    }
    else
    {
        Cms = NC + 1;
        Cme = NC;
    }
    int Xlast = downMultipleOf(W0-1, CTX.vlanes<_Tp>()) + lsimd - padding_left;
    int Cis = downC(NC, H, W, 0, -padding_left);
    int Cie = upperC(NC, H, W, (H-1), Xlast);
    int Cie_write = upperC(NC, H0, W0, (H0-1), upMultipleOf(W0, CTX.vlanes<_Tp>()) - 1);
    Cie = std::min(Cie, Cie_write);
    if(Cie < (Cis - 1))
    {
        Cis = NC + 1;
        Cie = NC;
    }
    int Yms = H0 + 1, Yme = H0;
    if(Cms < NC + 1)
    {
        Yms = downY(NC, H, W, Cms - 1, -padding_top, -padding_left);
        Yme = upperY(NC, H, W, Cme, MULTI_H + kh - padding_top - 2, MULTI_H, ((Cms - 1) == Cme ? Yms : 0), XlastMulti);
    }
    int Yis = H0 + 1, Yie = H0;
    if(Cis < NC + 1)
    {
        Yis = (padding_left == 0) ? 0 : downY(NC, H, W, Cis - 1, -padding_top, -padding_left);
        if ((H - 1) * W + Xlast < (NC-Cie)*H*W)
            Yie = H0;
        else
            Yie = std::max(0, upperY(NC, H, W, Cie, kh - 1 - padding_top, Xlast));
        int Yie_write = upperY(NC, H0, W0, Cie, 0, upMultipleOf(W0, CTX.vlanes<_Tp>()) - 1);
        Yie = std::min(Yie, Yie_write);
    }
    int Xis = W0, Xie = W0;
    if(Yis < H0)
    {
        Xis = downX(NC, H, W, Cis-1, Yis - 1 - padding_top, -padding_left);
        Xie = upperX(NC, H, W, Cie, std::min(Yie - padding_top + kh - 1, H-1), lsimd-padding_left, CTX.vlanes<_Tp>(), ((Cis - 1) == Cie && Yis == Yie)? Xis: 0);
        int Xie_write = upperX(NC, H0, W0, Cie, Yie, CTX.vlanes<_Tp>() - 1, CTX.vlanes<_Tp>(), ((Cis - 1) == Cie && Yis == Yie)? Xis: 0);
        Xie = std::min(Xie, Xie_write);
    }
    return dwc_algs_limits(Cms, Cme, Cis, Cie, Yms, Yme, Yis, Yie, Xis, Xie);
}
};
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
#endif //__LOOPS_DEPTHWISECONV_HPP__