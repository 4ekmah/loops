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
#include <cstring>
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
private:
    bool m_done; 
    Context CTX;
    static const int RLinesAmount;
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
};

const int DepthwiseconvGenerator::RLinesAmount = 3;

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
        IReg residL = H0 % RLinesAmount;
        IReg restL = CONST_(RLinesAmount) - residL;
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
        ELIF_(restL == RLinesAmount) // Means H0 % RLinesAmount == 0
        {
            cmultiend = select(residX == 0, C, C - 1);
            ymultiend_ = select(residX == 0, ymultiend_, H0 - RLinesAmount);
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
                IReg Hcond = padver ? max(H - (kh + RLinesAmount - 2), CONST_(0)) : IReg(); 
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
                y += RLinesAmount;
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
    std::vector<VReg<float> > vres(RLinesAmount, VReg<float>());
    for(int rnum = 0; rnum<RLinesAmount; rnum++)
        vres[rnum].copyidx(VReg<float>(vbias)); 
    const int elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
    int lvflags = flags&(PADHOR|PADVER);
    for(int lrow = 0; lrow < kh + RLinesAmount - 1; lrow++) 
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
            for(int lineNum = 0; lineNum < RLinesAmount; lineNum++)
            { 
                const int krow = lrow - lineNum;
                if(krow >= 0 && krow < kh)
                {
                    const int kerelemnum = krow*kw + kcol;
                    vres[lineNum] = fma(vres[lineNum], toAdd, vkernel[kerelemnum/CTX.vlanes<float>()], kerelemnum%CTX.vlanes<float>());
                }
            }
        }
        if(lrow + 2 < kh + RLinesAmount) //Not last loaded row
        {
            base += W << elemshift;
            if(flags&PADVER)
                yi += 1;
        }
    }
    IReg roffset = (flags&PADSHIFTRES? x + padding_left : x) << elemshift;
    for(int lineNum = 0; lineNum < RLinesAmount; lineNum++)
    {
        storevec<float>(result_rs, roffset, vres[lineNum]);
        if(lineNum + 1 < RLinesAmount)
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
    std::vector< std::vector<int> > fixtures = {
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
};
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
