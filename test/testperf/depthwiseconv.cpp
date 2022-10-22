/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
//TODO(ch): See https://github.com/vpisarev/loops/LICENSE -> https://github.com/4ekmah/loops/LICENSE
#include "depthwiseconv.hpp"
#if __LOOPS_ARCH == __LOOPS_AARCH64

#include "loops/loops.hpp"
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include "arm_neon.h"
#include "../test/tests.hpp"

namespace loops
{

class DepthwiseconvGenerator
{
public:
    DepthwiseconvGenerator(Context aCTX) : CTX(aCTX), m_done(false) {}
    typedef int64_t (*dwconv_t)(float* data, float* kernel, float* bias, int64_t H, int64_t W, int64_t C, float* result, int64_t H0, int64_t W0, int64_t padding_top, int64_t padding_left, int64_t padding_bottom, int64_t padding_right);
    dwconv_t generate(int kh_, int kw_, int padding_top_, int padding_left_, int padding_bottom_, int padding_right_);
    dwc_algs_limits calcAlgsLimits(int C, int W, int H, int kw, int kh, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right);
private:
    bool m_done; 
    Context CTX;
    static const int MultiH;
    enum { PADHOR = 1, PADVER = 2, INITDEST = 4, PREINCREMENT_IDXS = 8, PADSHIFTRES = 16, MULTILINE = 32 };
    void multilineHandler(const VReg<uint32_t>& HcondV, const VReg<uint32_t>& WcondV, IReg& yi, IReg& x, IReg& base, const IReg& result_rs, int flags);
    void onlylineHandler(const VReg<uint32_t>& HcondV, const VReg<uint32_t>& WcondV, IReg& yi, IReg& x, IReg& base, const IReg& result_rs, int flags);
    void loadVector(const IReg& base, int64_t offset, VReg<float>& dest, VReg<int32_t>& horIdxs, const VReg<uint32_t>& verMask, const VReg<uint32_t>& WcondV, int flags = 0);

    //Common parameters and registers
    int kh, kw, elemsize, elemshift;
    IReg H, W, W0, kernel, padding_left;
    std::vector<VReg<float> > vkernel;
    VReg<int32_t> countingPattern, idx_step;
    VReg<float> vbias;

    inline int upDiv(int numerator, int denominator);
    inline int upMultipleOf(int numerator, int denominator); 
    inline int downMultipleOf(int numerator, int denominator);
    //Find such a minimum U, that for each u>=U
    //inequality: u*ratio >= add is always correct 
    inline int downBorder(int add, int ratio);
    //Find such a maximum U, that for each u<U
    //inequality: u*ratio < add is always correct 
    inline int upperBorder(int add, int ratio);
    //lower border for all c, satisfies inequality
    //c*H*W + Y*W + X >= 0
    inline int downC(int C, int H, int W, int y, int x);
    //upper border for all c, satisfies inequality
    //c*H*W + Y*W + X < C*W*H
    inline int upperC(int C, int H, int W, int y, int x);
    //lower border for all y, satisfies inequality
    //Cf*H*W + (y + ys)*W + X >= 0
    inline int downY(int C, int H, int W, int Cf, int ys, int x);
    //upper border for all y, satisfies inequality
    //Cf*H*W + (y + ys)*W + X < C*W*H
    inline int upperY(int C, int H, int W, int Cf, int ys, int x);
    //upper border for all y, satisfies inequality
    //Cf*H*W + (y + ys)*W + X < C*W*H, where y is a multiple of M, started from y0:
    //y = M * r + y0 
    inline int upperY(int C, int H, int W, int Cf, int ys, int M, int y0, int x);
    //lower border for all x, satisfies inequality
    //Cf*H*W + Yf*W + x + xs  >= 0
    inline int downX(int C, int H, int W, int Cf, int Yf, int xs);
    //upper border for all y, satisfies inequality
    //Cf*H*W + Yf*W + x + xf < C*W*H
    inline int upperX(int C, int H, int W, int Cf, int Yf, int xs);
    //upper border for all y, satisfies inequality
    //Cf*H*W + Yf*W + x + xf < C*W*H, where x is a multiple of M, started from x0:
    //y = M * r + y0 
    inline int upperX(int C, int H, int W, int Cf, int Yf, int xs, int M, int x0);
};

const int DepthwiseconvGenerator::MultiH = 3;

//#define HORIZONTAL_OFFSET
DepthwiseconvGenerator::dwconv_t DepthwiseconvGenerator::generate(int kh_, int kw_, int padding_top_, int padding_left_, int padding_bottom_, int padding_right_)
{
    if(m_done)
        throw std::runtime_error("One generator object can create only one function. Create another generator.");
    kh = kh_; kw = kw_;
    elemsize = sizeof(float);
    elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
    std::string funcname = "depthwise_convolution_";
    const bool padver = (padding_top_ || padding_bottom_);
    const bool padhor = (padding_left_ || padding_right_);
    int handlerFlags = (padhor ? PADHOR : 0) | (padver ? PADVER : 0); 
    int pshiftflag = (padhor ? PADSHIFTRES : 0);
    funcname += std::to_string(kh) + "_" + std::to_string(kw);
    if(padhor) funcname += "_padhor";
    if(padver) funcname += "_padver";
    if(CTX.hasFunc(funcname))
    {
        m_done = true;
        return (dwconv_t)(CTX.getFunc(funcname).ptr());
    }
    size_t kernelRegsAmount = kh*kw;
    kernelRegsAmount = kernelRegsAmount/CTX.vlanes<float>() + (kernelRegsAmount%CTX.vlanes<float>()?1:0);
    vkernel.resize(kernelRegsAmount, VReg<float>());
    IReg data, bias, C, result, H0, padding_top, padding_bottom, padding_right;
    USE_CONTEXT_(CTX);
    STARTFUNC_(funcname, &data, &kernel, &bias, &H, &W, &C, &result, &H0, &W0, &padding_top, &padding_left, &padding_bottom, &padding_right)
    {
        if(padhor)
        {
            countingPattern.copyidx(VCONST_(int32_t, 0));
            for(int lane = 1; lane < CTX.vlanes<float>(); lane++)
                setlane(countingPattern, lane, CONST_(lane));
            idx_step.copyidx(VCONST_(int32_t, CTX.vlanes<float>()));
        }
        std::vector<IReg> horoff(kw, IReg());
#ifdef HORIZONTAL_OFFSET
        for(int kcol = 0; kcol < kw; kcol++)
            if(kcol%CTX.vlanes<float>())
                horoff[kcol].rawcopy(CONST_(kcol<<elemshift));
#endif //HORIZONTAL_OFFSET
        IReg residX = W0 % CTX.vlanes<float>();
        IReg xvectorend_ = padhor ? W0 - residX - padding_left : W0 - residX; //Also it is halide trick condition
        IReg hldx = padhor ? W0 - CTX.vlanes<float>() - padding_left : W0 - CTX.vlanes<float>();;
        IReg residL = H0 % MultiH;
        IReg restL = CONST_(MultiH) - residL;
        IReg cmultiend = C;
        IReg cvectorend = select(residX == 0, C, C - 1); //End of region handled by only vector cycles
        IReg ymultiend_ = H0 - residL;
        IReg H0m1 = H0 - 1;
        IF_(H0 == 1)
        {
            cmultiend = CONST_(0);
            ymultiend_ = CONST_(0);
        }
        ELIF_(W0 < CTX.vlanes<float>())
        {//Multiline mode uses Halide trick.
            cmultiend = CONST_(0);
            ymultiend_ = CONST_(0);
        }
        ELIF_(restL == MultiH) // Means H0 % RLinesAmount == 0
        {
            cmultiend = select(residX == 0, C, C - 1);
            ymultiend_ = select(residX == 0, ymultiend_, H0 - MultiH);
        }
        ELSE_
        {
            IReg lastTouchableLine = CONST_(1);
            lastTouchableLine = select(residX == 0, lastTouchableLine, 2);
            lastTouchableLine += restL;
            IReg LTLmod = restL % H0;
            cmultiend = restL / H0;
            cmultiend = select(LTLmod == 0, cmultiend, cmultiend + 1);
            cmultiend = C - cmultiend;
        }

        IReg channel = CONST_(0);
        WHILE_(channel < C)
        {
            IReg ymultiend = select(channel >= cmultiend, ymultiend_, H0); //End of region handled by multiline cycles
            IReg yvectorend = select(channel >= cvectorend, H0m1, H0); //End of region handled by only vector cycles
            vbias.copyidx(broadcast<float>(load_<float>(bias)));
            for(int kregnum = 0; kregnum < kernelRegsAmount; kregnum++)
                vkernel[kregnum].copyidx(loadvec<float>(kernel, kregnum * CTX.vbytes()));
            IReg y = CONST_(0);
            WHILE_(y < ymultiend)
            {
                IReg data_rs = data + ((W * (padver ? y - padding_top: y)) << elemshift); //TODO(ch): It's possible to rewrite via afterline increment.
                IReg result_rs = result + (W0 << elemshift) * y;
                IReg xi = padhor ? -padding_left : CONST_(0);
                IReg Hcond = padver ? max(H - (kh + MultiH - 2), CONST_(0)) : IReg(); 
                IReg Wcond = padhor ? max(W - kw - 2, CONST_(0)) : IReg();
                VReg<uint32_t> WcondV = padhor ? broadcast<uint32_t>(W) : VReg<uint32_t>();
                VReg<uint32_t> HcondV = padver ? broadcast<uint32_t>(H) : VReg<uint32_t>();
                IReg multilineendx;
                multilineendx.copyidx(padhor ? W0-padding_left : W0);
                WHILE_(xi < multilineendx)
                {
                    xi = select(xi == xvectorend_, hldx,xi);
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
                y += MultiH;
            }
            WHILE_(y < H0)
            {
                IReg xvectorend = select( y >= yvectorend, xvectorend_, (padhor ? W0-padding_left : W0)); //x < xmiddle must be handled by scalar code, and x >= xmiddle by vector code
                IReg data_rs = data + ((W * (padver ? y - padding_top: y)) << elemshift);
                IReg result_rs = result + (W0 << elemshift) * y;
                IReg xi = (padhor ? -padding_left : CONST_(0));
#ifdef HORIZONTAL_OFFSET
                WHILE_(xi < xme)
                {
                    IReg data__ = data_rs + (xi << elemshift);
                    VReg<float> vres = vbias;
                    for(int krow = 0; krow < kh; krow++) 
                    {
                        for(int kcol = 0; kcol < kw; kcol++) 
                        {
                            const int kerelemnum = krow*kw + kcol;
                            VReg<float> justloaded;
                            if(kcol%CTX.vlanes<float>())
                                justloaded.rawcopy(loadvec<float>(data__, horoff[kcol]));
                            else
                                justloaded.rawcopy(loadvec<float>(data__, kcol*elemsize));
                            vres = fma(vres, justloaded, vkernel[kerelemnum/CTX.vlanes<float>()], kerelemnum%CTX.vlanes<float>());
                        }
                        if(krow + 1 < kh)
                            data__ += W << elemshift;
                    }
                    storevec<float>(result_rs, xi << elemshift, vres);
                    xi += CTX.vlanes<float>();
                }
#else// HORIZONTAL_OFFSET
                IReg Wcond = padhor ? max(W - kw - 2, CONST_(0)): IReg();
                IReg Hcond = padver ? max(H - (kh - 1), CONST_(0)) : IReg();
                VReg<uint32_t> HcondV = padver ? broadcast<uint32_t>(H) : VReg<uint32_t>();
                VReg<uint32_t> WcondV = padhor ? broadcast<uint32_t>(W) : VReg<uint32_t>();
                WHILE_(xi < xvectorend)
                {
                    IReg data__ = data_rs + (xi << elemshift);
                    if(padhor||padver)
                    {
                        IReg yi = padver ? y - padding_top : IReg();
                        IReg xcond;
                        xcond.copyidx(padhor && padver ? select(ult(yi, Hcond), xi, Wcond): xi);
                        IF_(padhor?ult(xcond,Wcond):ult(yi, Hcond))
                        {
                            onlylineHandler(HcondV, WcondV, yi, xi, data__, result_rs, 
                                                pshiftflag);
                            CONTINUE_;
                        }
                        onlylineHandler(HcondV, WcondV, yi, xi, data__, result_rs, 
                                        handlerFlags | pshiftflag);
                    }
                    else 
                        onlylineHandler(HcondV, WcondV, y, xi, data__, result_rs, 
                                            0);
                }
#endif// HORIZONTAL_OFFSET
                IReg scalarend;
                scalarend.copyidx( padhor ? W0-padding_left : W0);
                WHILE_(xi < scalarend)
                {
                    VReg<float> vres = vbias;
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
                        if(padhor | padver)
                        {
                            IReg ex = xi + kcol;
                            IReg ey = (padver ? y - padding_top : y) + krow;
                            select(ex < 0, W, ex);
                            select(ey < 0, H, ey);
                            IF_(ex<W)
                            {
                                IF_(ey<H)
                                {
                                    VReg<float> justloaded = broadcast<float>(load_<float>(data__));
                                    VReg<float> w = broadcast<float>(load_<float>(kernel__));
                                    {
                                        VReg<float> antiSpill = vres; //TODO(ch): remove it when snippet management will be better.
                                        vres = fma(antiSpill, justloaded, w);
                                    }
                                }
                            }
                        }
                        else
                        {
                            VReg<float> justloaded = broadcast<float>(load_<float>(data__));
                            VReg<float> w = broadcast<float>(load_<float>(kernel__));
                            {
                                VReg<float> antiSpill = vres; //TODO(ch): remove it when snippet management will be better.
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
                    store_<float>(result_rs + roffset, getlane<float>(vres, 0));
                    xi += 1;
                }
                y += 1;
            }
            data += H * W << elemshift;
            result += H0 * W0 << elemshift;  
            kernel += kw * kh << elemshift;  
            bias += elemsize;
            channel = channel + 1;
        }
        RETURN_(0);
    }
    m_done = true;
    return (dwconv_t)(CTX.getFunc(funcname).ptr());
}

void DepthwiseconvGenerator::multilineHandler(const VReg<uint32_t>& HcondV, const VReg<uint32_t>& WcondV, IReg& yi, IReg& x, IReg& base, const IReg& result_rs, int flags)
{
    USE_CONTEXT_(CTX);
    std::vector<VReg<float> > vres(MultiH, VReg<float>());
    for(int rnum = 0; rnum<MultiH; rnum++)
        vres[rnum].copyidx(VReg<float>(vbias)); 
    const int elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
    int lvflags = flags&(PADHOR|PADVER);
    for(int lrow = 0; lrow < kh + MultiH - 1; lrow++) 
    {
        VReg<int32_t> horIdxs = (flags&PADHOR) ? broadcast<int32_t>(x) + countingPattern : VReg<int32_t>();
        VReg<uint32_t> verMask = (flags&PADVER) ? broadcast<uint32_t>(yi) < HcondV : VReg<uint32_t>();
        if((flags&PADVER)&&(flags&PADHOR))
        {
            VReg<int32_t> antiSpill = horIdxs; //TODO(ch): remove it when snippet management will be better.
            horIdxs = select(verMask, antiSpill, reinterpret<int32_t>(WcondV));
        }
        VReg<float> loadedHalf0, loadedHalf1;
        loadVector(base, 0, loadedHalf0, horIdxs, verMask, WcondV, INITDEST | lvflags);
        if(kw > 1)
            loadVector(base, CTX.vbytes(), loadedHalf1, horIdxs, verMask, WcondV, INITDEST | PREINCREMENT_IDXS | lvflags);

        for(int kcol = 0; kcol < kw; kcol++) 
        {
            VReg<float> toAdd;
            if(kcol%CTX.vlanes<float>() == 0 && kcol > 0)
            {
                VReg<float> inter;
                inter.copyidx(loadedHalf0);
                loadedHalf0.copyidx(loadedHalf1);
                loadedHalf1.copyidx(inter);
                if(kcol + 1 < kw)
                    loadVector(base, kcol*elemsize+CTX.vbytes(), loadedHalf1, horIdxs, verMask, WcondV, PREINCREMENT_IDXS | lvflags);
                toAdd.copyidx(loadedHalf0);
            }
            else
                toAdd.copyidx(ext(loadedHalf0, loadedHalf1, kcol%CTX.vlanes<float>()));
            for(int lineNum = 0; lineNum < MultiH; lineNum++)
            { 
                const int krow = lrow - lineNum;
                if(krow >= 0 && krow < kh)
                {
                    const int kerelemnum = krow*kw + kcol;
                    vres[lineNum] = fma(vres[lineNum], toAdd, vkernel[kerelemnum/CTX.vlanes<float>()], kerelemnum%CTX.vlanes<float>());
                }
            }
        }
        if(lrow + 2 < kh + MultiH) //Not last loaded row
        {
            base += W << elemshift;
            if(flags&PADVER)
                yi += 1;
        }
    }
    IReg roffset = (flags&PADSHIFTRES? x + padding_left : x) << elemshift;
    for(int lineNum = 0; lineNum < MultiH; lineNum++)
    {
        storevec<float>(result_rs, roffset, vres[lineNum]);
        if(lineNum + 1 < MultiH)
            roffset += W0 << elemshift;
    }
    x += CTX.vlanes<float>();
}

void DepthwiseconvGenerator::onlylineHandler(const VReg<uint32_t>& HcondV, const VReg<uint32_t>& WcondV, IReg& yi, IReg& x, IReg& base, const IReg& result_rs, int flags)
{
    USE_CONTEXT_(CTX);
    VReg<float> vres = vbias;
    const int elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
    int lvflags = flags&PADHOR;
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
        VReg<uint32_t> dummy;
        VReg<int32_t> horIdxs = flags&PADHOR ? broadcast<int32_t>(x) + countingPattern : VReg<int32_t>();
        VReg<float> loadedHalf0, loadedHalf1;
        loadVector(base, 0, loadedHalf0, horIdxs, dummy, WcondV, INITDEST | lvflags);
        if(kw > 1)
            loadVector(base, CTX.vbytes(), loadedHalf1, horIdxs, dummy, WcondV, INITDEST | PREINCREMENT_IDXS | lvflags);

        for(int kcol = 0; kcol < kw; kcol++) 
        {
            VReg<float> spliced;
            VReg<float> toAdd;
            if(kcol%CTX.vlanes<float>() == 0 && kcol > 0)
            {
                VReg<float> interm;
                interm.copyidx(loadedHalf0);
                loadedHalf0.copyidx(loadedHalf1);
                loadedHalf1.copyidx(interm);
                if(kcol + 1 < kw)
                    loadVector(base, kcol*elemsize+CTX.vbytes(), loadedHalf1, horIdxs, dummy, WcondV, PREINCREMENT_IDXS | lvflags);
                toAdd.copyidx(loadedHalf0);
            }
            else
                toAdd.copyidx(ext(loadedHalf0, loadedHalf1, kcol%CTX.vlanes<float>()));
            VReg<float> w = broadcast<float>(load_<float>(kptr));
            {
                VReg<float> antiSpill = vres; //TODO(ch): remove it when snippet management will be better.
                vres = fma(antiSpill, toAdd, w);
            }
            kptr += elemsize;
        }
        base += W << elemshift;
        if(flags&PADVER)
            yi += 1;
        krow += 1;
    }
    IReg roffset = (flags&PADSHIFTRES? x + padding_left : x) << elemshift;
    storevec<float>(result_rs, roffset, vres);
    x += CTX.vlanes<float>();
}

void DepthwiseconvGenerator::loadVector(const IReg& base, int64_t offset, VReg<float>& dest, VReg<int32_t>& horIdxs, const VReg<uint32_t>& verMask, const VReg<uint32_t>& WcondV, int flags)
{
    USE_CONTEXT_(CTX);
    if(flags&INITDEST) 
        dest.copyidx(loadvec<float>(base, offset));
    else
        dest = loadvec<float>(base, offset);
    if(flags&(PADHOR|PADVER))
    {
        if((flags & PADVER) && !(flags & PADHOR))
        {
            dest = reinterpret<float>( verMask & reinterpret<uint32_t>(dest));
        }
        else
        {
            if((flags&PREINCREMENT_IDXS) && (flags & PADHOR))
                horIdxs += idx_step;
            VReg<uint32_t> mask;
            mask.copyidx((flags & PADHOR) ? (reinterpret<uint32_t>(horIdxs) < WcondV) : verMask);
            dest = reinterpret<float>( mask & reinterpret<uint32_t>(dest));
        }
    }
}

inline int DepthwiseconvGenerator::upDiv(int numerator, int denominator) 
{
    if(numerator < 0)
        return -((-numerator)/denominator);
    int res = numerator / denominator;
    int back = res * denominator; 
    return (numerator - back ? res + 1: res);
}

inline int DepthwiseconvGenerator::upMultipleOf(int numerator, int denominator) 
{
    if(numerator < 0)
        return -downMultipleOf(-numerator, denominator);
    int div = numerator / denominator;
    int back = div * denominator; 
    return (numerator - back ? back + denominator : back);
}

inline int DepthwiseconvGenerator::downMultipleOf(int numerator, int denominator) 
{
    if(numerator < 0)
        return -upMultipleOf(-numerator, denominator);
    numerator /= denominator;
    numerator *= denominator;
    return numerator;
}

inline int DepthwiseconvGenerator::downBorder(int add, int ratio)
{
    return upDiv(add, ratio);
}

inline int DepthwiseconvGenerator::upperBorder(int add, int ratio)
{
    return upDiv(add, ratio);
}

inline int DepthwiseconvGenerator::downC(int C, int H, int W, int y, int x)
{
    return downBorder(-y*W-x, H*W);
}

inline int DepthwiseconvGenerator::upperC(int C, int H, int W, int y, int x)
{
    return upperBorder(C*H*W - y*W - x, H*W);
}

inline int DepthwiseconvGenerator::downY(int C, int H, int W, int Cf, int ys, int x)
{
    Cf = std::max(Cf,0);
    return downBorder(-Cf*H*W - x - ys * W, W);
}

inline int DepthwiseconvGenerator::upperY(int C, int H, int W, int Cf, int ys, int x)
{
    return upperBorder((C-Cf)*H*W - x - ys * W, W);
}

inline int DepthwiseconvGenerator::upperY(int C, int H, int W, int Cf, int ys, int M, int y0, int x)
{
    return M * upperBorder((C-Cf)*H*W - x - (ys + y0) * W, W * M) + y0;
}

inline int DepthwiseconvGenerator::downX(int C, int H, int W, int Cf, int Yf, int xs)
{
    Cf = std::max(Cf,0);
    Yf = std::max(Yf,0);
    return downBorder(-Cf*H*W - Yf * W - xs, 1);
}

inline int DepthwiseconvGenerator::upperX(int C, int H, int W, int Cf, int Yf, int xs)
{
    return upperBorder((C-Cf)*H*W - Yf * W - xs, 1);
}

inline int DepthwiseconvGenerator::upperX(int C, int H, int W, int Cf, int Yf, int xs, int M, int x0)
{
    return M * upperBorder((C-Cf)*H*W - Yf * W - xs - x0, M) + x0;
}

dwc_algs_limits DepthwiseconvGenerator::calcAlgsLimits(int C, int W, int H, int kw, int kh, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right)
{
    //TODO(ch): Rewrite Cms, Yms and other **s to value, in which starts behviour, not the value, wher it behaviour is only behaviour. It would be simpler to work with.
    //Reconsider one-line conditions and using of padding-top. There needed max/min functions on row's numbers and borders of rows - 0 and H. 
    int Cms, Cme;
    int lsimd = upMultipleOf(kw + CTX.vlanes<float>() - 2, CTX.vlanes<float>()) - 1;
    int XlastMulti = (W0 - padding_left - CTX.vlanes<float>()) + lsimd;
    if(H0 > CTX.vlanes<float>())
    {
        Cms = downC(C, H, W, -padding_top, -padding_left);
        int YlastMulti = (downMultipleOf(W0, MultiH) - padding_top + kh + MultiH - 2);
        Cme = upperC(C, H, W, YlastMulti, XlastMulti);
    }
    else
    {
        Cms = C + 1;
        Cme = C;
    }
    int Xlast = downMultipleOf(W0-1, CTX.vlanes<float>()) + lsimd - padding_left;
    int Cis = downC(C, H, W, 0, -padding_left);
    int Cie = upperC(C, H, W, (H-1), Xlast);
    if(Cie < (Cis - 1))
    {
        Cis = C + 1;
        Cie = C;
    }
    int Yms = H0 + 1, Yme = 0;
    if(Cms < C + 1)
    {
        Yms = downY(C, H, W, Cms - 1, -padding_top, -padding_left);
        Yme = upperY(C, H, W, Cme, MultiH + kh - padding_top - 2, MultiH, ((Cms - 1) == Cme ? Yms : 0), XlastMulti);
    }
    int Yis = H0 + 1, Yie = 0;
    if(Cis < C + 1)
    {
        Yis = (padding_left == 0) ? 0 : downY(C, H, W, Cis - 1, -padding_top, -padding_left);
        if ((H - 1) * W + Xlast < (C-Cie)*H*W)
            Yie = H0;
        else
            Yie = std::max(0, upperY(C, H, W, Cie, kh - 1 - padding_top, Xlast));
    }
    int Xis = W0, Xie = 0;
    if(Yis < H0)
    {
        Xis = downX(C, H, W, Cis-1, Yis - 1 - padding_top, -padding_left);
        Xie = upperX(C, H, W, Cie, std::min(Yie - padding_top + kh - 1, H-1), lsimd-padding_left, CTX.vlanes<float>(), ((Cis - 1) == Cie && Yis == Yie)? Xis: 0);
    }
    return dwc_algs_limits(Cms, Cme, Cis, Cie, Yms, Yme, Yis, Yie, Xis, Xie);
}

DepthwiseconvTest::DepthwiseconvTest(Context aCTX, std::ostream* a_out): CTX(aCTX), out(a_out) {}

template<class T>
void printData(T* data, int H, int W) { //TODO(ch): move it to test.hpp 
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
            std::cout << std::setw(6) << data[i*W+j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template<class T>
void printData(T* data, int H, int W, int stride) { //TODO(ch): move it to test.hpp 
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
            std::cout << std::setw(15) << data[i*stride+j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
void DepthwiseconvTest::run() 
{
    const int TESTITERATIONS = 30;

    std::vector< std::pair<std::vector<int>, std::vector<int>> > limitsFixtures = {
        {{3, 3, 3, 10, 10, 0, 0, 0, 0}, {0, 2, 0, 2, 0, 6, 0, 7, 0, 4}},
        {{3, 3, 3, 10, 10, 1, 0, 1, 0}, {1, 2, 0, 2, 1, 6, 0, 8, 0, 4}},
        {{3, 3, 3, 10, 10, 0, 1, 0, 1}, {1, 2, 1, 2, 1, 6, 1, 7, 1, 4}},
        {{3, 3, 3, 10, 10, 1, 1, 1, 1}, {1, 2, 1, 2, 2, 6, 2, 8, 1, 4}},
        {{3, 3, 1, 10, 10, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 6, 0, 7, 0, 4}},
        {{3, 3, 1, 10, 10, 1, 0, 1, 0}, {1, 0, 0, 0, 1, 7, 0, 8, 0, 4}},
        {{3, 3, 1, 10, 10, 0, 1, 0, 1}, {1, 0, 1, 0, 1, 7, 1, 7, 1, 4}},
        {{3, 3, 1, 10, 10, 1, 1, 1, 1}, {1, 0, 1, 0, 2, 8, 2,  8, 1, 4}},
        {{3, 3, 1, 1, 1, 1, 1, 1, 1},   {2, 1, 2, 1, 2, 0, 2, 0, 1, 0}},
        {{3, 3, 100, 1, 1, 1, 1, 1, 1}, {101, 100, 1, 94, 2, 0, 2, 0, 1, 0}}
        };
    for(auto fxt: limitsFixtures)
    {
        int kh = fxt.first[0];
        int kw = fxt.first[1];
        const int C = fxt.first[2];
        const int H = fxt.first[3];
        const int W = fxt.first[4];
        const int padding_top = fxt.first[5];
        const int padding_left = fxt.first[6];
        const int padding_bottom = fxt.first[7];
        const int padding_right = fxt.first[8];
        const dwc_algs_limits ref(fxt.second[0], fxt.second[1], fxt.second[2], fxt.second[3], fxt.second[4], fxt.second[5], fxt.second[6], fxt.second[7], fxt.second[8], fxt.second[9]);
        bool padding = padding_top || padding_left || padding_bottom || padding_right;
        const int H0 = H-kh+1+padding_top+padding_bottom;
        const int W0 = W-kw+1+padding_left+padding_right;

        DepthwiseconvGenerator generator(CTX);
        (*out) << "Depthwise convolution "<<kh<<"x"<<kw<<", C = "<< C << ", H = "<< H << ", W = "<< W << ", pt = "<< padding_top << ", pl = "<< padding_left << ", pb = "<< padding_bottom << ", pr = "<< padding_right << std::endl;
        dwc_algs_limits tocheck = generator.calcAlgsLimits(C, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right);
        if(!compare_alg_limits(tocheck, ref))
            return;
    }

    std::vector< std::vector<int> > fixtures = {
        {5, 5, 1632, 7, 7, 2, 2, 2, 2},
        {3, 3, 32, 112, 112, 1, 1, 1, 1},
        {3, 3, 192, 56, 56, 1, 1, 1, 1},

        {3, 3, 3, 10, 10, 0, 0, 0, 0},
        {3, 3, 3, 10, 10, 1, 0, 0, 0},
        {3, 3, 3, 10, 10, 0, 1, 0, 0},
        {3, 3, 3, 10, 10, 0, 0, 1, 0},
        {3, 3, 3, 10, 10, 0, 0, 0, 1},
        {3, 3, 3, 10, 10, 1, 0, 1, 0},
        {3, 3, 3, 10, 10, 0, 1, 0, 1},
        {3, 3, 3, 10, 10, 1, 1, 1, 1},
        {3, 3, 3, 9, 9, 0, 0, 0, 0},
        {3, 3, 3, 9, 9, 1, 0, 0, 0},
        {3, 3, 3, 9, 9, 0, 1, 0, 0},
        {3, 3, 3, 9, 9, 0, 0, 1, 0},
        {3, 3, 3, 9, 9, 0, 0, 0, 1},
        {3, 3, 3, 9, 9, 1, 0, 1, 0},
        {3, 3, 3, 9, 9, 0, 1, 0, 1},
        {3, 3, 3, 9, 9, 1, 1, 1, 1},

        {3, 3,  32, 200, 200, 0, 1, 0, 1}, 
        {3, 3, 256,  40,  40, 0, 1, 0, 1}, 
        {5, 5,  32, 200, 200, 0, 1, 0, 1}, 
        {5, 5, 256,  40,  40, 0, 1, 0, 1}, 
        {7, 7,  32, 200, 200, 0, 1, 0, 1}, 
        {7, 7, 256,  40,  40, 0, 1, 0, 1}, 
        {9, 9,  32, 200, 200, 0, 1, 0, 1},
        {9, 9, 256,  40,  40, 0, 1, 0, 1},

        {3, 3,  32, 200, 200, 0, 0, 0, 0}, 
        {3, 3, 256,  40,  40, 0, 0, 0, 0}, 
        {5, 5,  32, 200, 200, 0, 0, 0, 0}, 
        {5, 5, 256,  40,  40, 0, 0, 0, 0}, 
        {7, 7,  32, 200, 200, 0, 0, 0, 0}, 
        {7, 7, 256,  40,  40, 0, 0, 0, 0}, 
        {9, 9,  32, 200, 200, 0, 0, 0, 0},
        {9, 9, 256,  40,  40, 0, 0, 0, 0}
//      {kh,kw, C, H, W, padding_top, padding_left, padding_bottom, padding_right}
    };
    for(auto fxt: fixtures)
    {
        int kh = fxt[0];
        int kw = fxt[1];
        const int C = fxt[2];
        const int H = fxt[3];
        const int W = fxt[4];
        const int padding_top = fxt[5];
        const int padding_left = fxt[6];
        const int padding_bottom = fxt[7];
        const int padding_right = fxt[8];
        bool padding = padding_top || padding_left || padding_bottom || padding_right;
        const int H0 = H-kh+1+padding_top+padding_bottom;
        const int W0 = W-kw+1+padding_left+padding_right;

        DepthwiseconvGenerator generator(CTX);
        DepthwiseconvGenerator::dwconv_t func = generator.generate(kh, kw, padding_top, padding_left, padding_bottom, padding_right);
        std::vector<float> indata(W*H*C * 3, (float)555.0); //DUBUGGG: Delete this 3 and 555
        std::vector<float> kernel(kw*kh*C, 0);
        std::vector<float> bias(C, 0);        
        std::vector<float> outdata(H0*W0*C * 3, 0); //DUBUGGG: Delete this 3.
        std::vector<float> outdataref(H0*W0*C, 0);
        float* inptr = &(indata[0]) + (W*H*C); //DUBUGGG: Delete this index
        float* kptr = &(kernel[0]);
        float* bptr = &(bias[0]);
        float* optr = &(outdata[0]) + H0*W0*C; //DUBUGGG: Delete this index
        float* optrref = &(outdataref[0]);
        gendata(inptr, kptr, bptr,kh,kw, H, W, C);
        (*out) << "Depthwise convolution "<<kh<<"x"<<kw<<", C = "<< C << ", H = "<< H << ", W = "<< W << (padding ? " with padding" : "") << std::endl;
        ref(inptr, kptr, bptr, H, W, C, optrref, H0, W0, kh, kw, padding_top, padding_left, padding_bottom, padding_right);
        Timer t;
        int ret;
        for(int testiter = 0; testiter < TESTITERATIONS; testiter++)
        {
            t.start();
            ret = func(inptr, kptr, bptr, H, W, C, optr, H0, W0, padding_top, padding_left, padding_bottom, padding_right);
            t.stop();
        }
        // std::cout<<"DUBUGGG:"<< ret <<std::endl;// return;
        if(compare(optr, optrref, C, H0, W0))
            (*out)<<"    Optimized time = "<<t.str()<<std::endl;
        else
        {
            // printData(inptr, H, W);
            // printData(optrref, H0, W0);
            // printData(optr, H0, W0);
            return;
        }
    }
}
                                          
void DepthwiseconvTest::gendata(float* data, float* kernel, float* bias, int kh, int kw, int H, int W, int C)
{
    for (int i = 0 ; i < C*H*W ; i++)
        data[i] = rand() % 1000;
    // for (int i = 0 ; i < C*kw*kh ; i++)
    //     kernel[i] = 1;
    // for (int i = 0 ; i < C ; i++)
    //     bias[i] = 0;
//DUBUGGG
    for (int i = 0 ; i < C*kw*kh ; i++)
        kernel[i] = (rand() % 10000)/2500.0f - 2;
    for (int i = 0 ; i < C ; i++)
        bias[i] = (rand() % 10000)/2500.0f - 2;
}

void DepthwiseconvTest::ref(float* data, float* kernel, float* bias, int H, int W, int C, float* result, int H0, int W0, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right)
{
    std::vector<float> padded;
    {
        const int WP = (W+padding_left+padding_right);
        const int HP = (H+padding_top+padding_bottom);
        padded.resize(WP*HP*C, 0);
        for(int ch = 0; ch < C; ch++)
        {
            float* dest_ = &(padded[WP*HP*ch]);
            float* data_ = &(data[W*H*ch]);
            for(int nprow = 0; nprow < H; nprow++)
            {
                float* dest = dest_ + (nprow + padding_top) * WP + padding_left;
                memcpy(dest, data_ + W * nprow, W * sizeof(float));
            }
        }
        W = WP;
        H = HP;
        data = &padded[0];
    }
    std::vector<int> koffsets(kh*kw, 0);
    for(int r = 0; r < kh; r++)
        for(int c = 0; c < kw; c++)
            koffsets[r*kw + c] = W * r + c;
    for(int c = 0; c < C; c++)
    {
        float* ker = kernel + c * kw * kh;
        float* inL = data + c * W * H;
        float* resL = result + c * W0 * H0;
        float32x4_t b = vdupq_n_f32(bias[c]);
        for(int i = 0; i < H0; i++) 
        {
            float* inR = inL + i * W;
            float* resR = resL + i * W0;
            for(int j = 0; j < W0; j++)
            {
                float* inC = inR + j;
                float* resC = resR + j;
                float32x4_t res = b;
                for(int k = 0; k < kh * kw; k++)
                {
                    int krow = k/kw;
                    int kcol = k - krow * kw;
                    float32x4_t toAdd = vdupq_n_f32(inC[koffsets[k]]);
                    float32x4_t weight = vdupq_n_f32(ker[k]);
                    res = vfmaq_f32(res, toAdd, weight);
                }
                float res_ = vgetq_lane_f32(res, 0);
                *resC = res_;
            }
        }
    }
}

bool DepthwiseconvTest::compare(float* tocheck, float* ref, int C, int H, int W)
{
    float maxdiff = 0;
    for(int k = 0; k < C; k++)
        for(int i = 0; i < H; i++)
            for(int j = 0; j < W; j++)
            {
                float tchk = tocheck[(k * H + i) * W + j];
                float r = ref[(k * H + i) * W + j];
                if(tchk != r)
                {
                    float absErr = std::abs(tchk - r);
                    float relErr = absErr / std::abs(r);
                    (*out)<<"    Result non-equal to reference at ["<< k <<", "<< i <<", "<< j<<"]"<<std::endl;
                    (*out)<<"        Result got:"<< tchk <<std::endl;
                    (*out)<<"        Reference:"<< r <<std::endl;
                    (*out)<<"        Absolute error:"<< absErr <<std::endl;
                    (*out)<<"        Relative error:"<< relErr <<std::endl;
                    return false;
                }
            }
    return true;
}

bool DepthwiseconvTest::compare_alg_limits(const dwc_algs_limits& tocheck, const dwc_algs_limits& reference)
{
    bool res = true;
    if(tocheck.Cms != reference.Cms) {std::cout<<"    Cms:ref = " << reference.Cms << " | checked =  " << tocheck.Cms<<std::endl; res = false;}
    if(tocheck.Cme != reference.Cme) {std::cout<<"    Cme:ref = " << reference.Cme << " | checked =  " << tocheck.Cme<<std::endl; res = false;}
    if(tocheck.Cis != reference.Cis) {std::cout<<"    Cis:ref = " << reference.Cis << " | checked =  " << tocheck.Cis<<std::endl; res = false;}
    if(tocheck.Cie != reference.Cie) {std::cout<<"    Cie:ref = " << reference.Cie << " | checked =  " << tocheck.Cie<<std::endl; res = false;}
    if(tocheck.Yms != reference.Yms) {std::cout<<"    Yms:ref = " << reference.Yms << " | checked =  " << tocheck.Yms<<std::endl; res = false;}
    if(tocheck.Yme != reference.Yme) {std::cout<<"    Yme:ref = " << reference.Yme << " | checked =  " << tocheck.Yme<<std::endl; res = false;}
    if(tocheck.Yis != reference.Yis) {std::cout<<"    Yis:ref = " << reference.Yis << " | checked =  " << tocheck.Yis<<std::endl; res = false;}
    if(tocheck.Yie != reference.Yie) {std::cout<<"    Yie:ref = " << reference.Yie << " | checked =  " << tocheck.Yie<<std::endl; res = false;}
    if(tocheck.Xis != reference.Xis) {std::cout<<"    Xis:ref = " << reference.Xis << " | checked =  " << tocheck.Xis<<std::endl; res = false;}
    if(tocheck.Xie != reference.Xie) {std::cout<<"    Xie:ref = " << reference.Xie << " | checked =  " << tocheck.Xie<<std::endl; res = false;}
    return res; 
}

};
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
