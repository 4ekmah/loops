/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
//TODO(ch): See https://github.com/vpisarev/loops/LICENSE -> https://github.com/4ekmah/loops/LICENSE
#include "depthwiseconv.hpp"
#if __LOOPS_ARCH ==  __LOOPS_AARCH64

#include "loops/loops.hpp"
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include "arm_neon.h"
#include "../test/tests.hpp"

namespace loops
{

const int DepthwiseconvTest::RLinesAmount = 3;

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
        /*const*/ int H = fxt[3]; //DUBUGGG: uncomment
        /*const*/ int W = fxt[4];
        const int padding_top = fxt[5];
        const int padding_left = fxt[6];
        const int padding_bottom = fxt[7];
        const int padding_right = fxt[8];
        bool padding = padding_top || padding_left || padding_bottom || padding_right;
        /*const*/ int H0 = H-kh+1+padding_top+padding_bottom;
        /*const*/ int W0 = W-kw+1+padding_left+padding_right;
        // if(W0%4)
        // { 
        //     W += 4 - W0%4;
        //     W0 += 4 - W0%4;
        // }


        dwconv_t func = generate(kh, kw, padding_top, padding_left, padding_bottom, padding_right);
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
        int32_t countingPattern[] = {0,1,2,3};
        Timer t;
        int ret;
        for(int testiter = 0; testiter < TESTITERATIONS; testiter++)
        {
            t.start();
            ret = func(inptr, kptr, bptr, H, W, C, optr, H0, W0, padding_top, padding_left, padding_bottom, padding_right, countingPattern);
            t.stop();
        }
        // std::cout<<"DUBUGGG:"<< ret <<std::endl;// return;
        if(compare(optr, optrref, C, H0, W0))
            (*out)<<"    Optimized time = "<<t.str()<<std::endl;
        else
        {
            // printData(inptr + H*W, H, W);
            // printData(optrref + H0*W0, H0, W0);
            // printData(optr + H0*W0, H0, W0);
            return;
        }
    }
}

//#define HORIZONTAL_OFFSET
DepthwiseconvTest::dwconv_t DepthwiseconvTest::generate(int kh, int kw, int padding_top_, int padding_left_, int padding_bottom_, int padding_right_)
{
    std::string funcname = "depthwise_convolution_";
    const bool padver = (padding_top_ || padding_bottom_);
    const bool padhor = (padding_left_ || padding_right_);
    funcname += std::to_string(kh) + "_" + std::to_string(kw);
    if(padhor) funcname += "_padhor";
    if(padver) funcname += "_padver";
    if(CTX.hasFunc(funcname))
        return (dwconv_t)(CTX.getFunc(funcname).ptr());
    size_t kernelRegsAmount = kh*kw;
    kernelRegsAmount = kernelRegsAmount/CTX.vlanes<float>() + (kernelRegsAmount%CTX.vlanes<float>()?1:0);
    const int elemsize = sizeof(float);
    const int elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
    IReg data, kernel, bias, H, W, C, result, H0, W0, padding_top, padding_left, padding_bottom, padding_right, countingPatternPtr;
    USE_CONTEXT_(CTX);
    STARTFUNC_(funcname, &data, &kernel, &bias, &H, &W, &C, &result, &H0, &W0, &padding_top, &padding_left, &padding_bottom, &padding_right, &countingPatternPtr)
    {
        VReg<int32_t> countingPattern;
        if(padhor)
            countingPattern.rawcopy(loadvec<int32_t>(countingPatternPtr));
        IReg channel = CONST_(0);
        std::vector<IReg> horoff(kw, IReg());
#ifdef HORIZONTAL_OFFSET
        for(int kcol = 0; kcol < kw; kcol++)
            if(kcol%CTX.vlanes<float>())
                horoff[kcol].rawcopy(CONST_(kcol*elemsize));
#endif //HORIZONTAL_OFFSET
        IReg residX = W0 % CTX.vlanes<float>();
        IReg xvectorend_ = padhor ? W0 - residX - padding_left : W0 - residX; //Also it is halide trick condition
        IReg hldx = W0 - CTX.vlanes<float>();
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
        ELIF_(hldx < 0)
        {
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

        WHILE_(channel < C)
        {
            IReg ymultiend = select(channel >= cmultiend, ymultiend_, H0); //End of region handled by multiline cycles
            ymultiend = CONST_(0); //DUBUGGG Delete on enabling 3line.

            IReg yvectorend = select(channel >= cvectorend, H0m1, H0); //End of region handled by only vector cycles
            IReg ibias = load_<float>(bias);
            VReg<float> vbias(ibias);
            std::vector<VReg<float> > vkernel(kernelRegsAmount, VReg<float>());
            for(int kregnum = 0; kregnum < kernelRegsAmount; kregnum++)
                vkernel[kregnum].rawcopy(loadvec<float>(kernel, kregnum * CTX.vbytes()));

            IReg y = CONST_(0);
            // WHILE_(y < ymultiend)
            // {
            //     IReg data_rs = data + W * elemsize * y; //TODO(ch): It's possible to rewrite via afterline increment.
            //     IReg result_rs = result + W0 * elemsize * y;
            //     IReg x = CONST_(0);
            //     WHILE_(x < W0)
            //     {
            //         x = select(x == xvectorend_, hldx,x);
            //         IReg data__ = data_rs + x * elemsize;
            //         VReg<float> vres[RLinesAmount];
            //         for(int resLine = 0; resLine < RLinesAmount; resLine++)
            //             vres[resLine].rawcopy(VReg<float>(vbias));
            //         for(int readLine = 0; readLine < kh + RLinesAmount - 1; readLine++) 
            //         {
            //             VReg<float> loadedHalf0_ = loadvec<float>(data__);
            //             VReg<float> loadedHalf1_;
            //             if(kw > 1)
            //                 loadedHalf1_.rawcopy(loadvec<float>(data__, CTX.vbytes()));
            //             VReg<float>* loadedHalf0 = &loadedHalf0_;
            //             VReg<float>* loadedHalf1 = &loadedHalf1_;
            //             for(int kcol = 0; kcol < kw; kcol++) 
            //             {
            //                 VReg<float> spliced; //rename to "spliced".
            //                 VReg<float>* toAdd;
            //                 if(kcol%CTX.vlanes<float>() == 0 && kcol > 0)
            //                 {
            //                     std::swap(loadedHalf0, loadedHalf1);
            //                     if(kcol + 1 < kw)
            //                         *loadedHalf1 = loadvec<float>(data__, kcol*elemsize + CTX.vbytes());
            //                     toAdd = loadedHalf0;
            //                 }
            //                 else
            //                 {
            //                     spliced.rawcopy(ext(*loadedHalf0, *loadedHalf1, kcol%CTX.vlanes<float>()));
            //                     toAdd = &spliced;
            //                 }
            //                 for(int resLine = 0; resLine < RLinesAmount; resLine++) 
            //                 {
            //                     const int krow = readLine - resLine;
            //                     if(krow >= 0 && krow < kh)
            //                     {
            //                         const int kerelemnum = krow*kw + kcol;
            //                         vres[resLine] = fma(vres[resLine], *toAdd, vkernel[kerelemnum/CTX.vlanes<float>()], kerelemnum%CTX.vlanes<float>());
            //                     }
            //                 }
            //             }
            //             if(readLine + 2 < kh + RLinesAmount)
            //                 data__ += W * elemsize;
            //         }
            //         IReg roffset = x * elemsize;
            //         for(int resLine = 0; resLine < RLinesAmount; resLine++)
            //         {
            //             storevec<float>(result_rs, roffset, vres[resLine]);
            //             if(resLine + 1 < RLinesAmount) 
            //                 roffset += W0 * elemsize;
            //         }
            //         x += CTX.vlanes<float>();
            //     }
            //     y += RLinesAmount;
            // }
            WHILE_(y < H0)
            {
                IReg xvectorend = select( y >= yvectorend, xvectorend_, (padhor ? W0-padding_left : W0)); //x < xmiddle must be handled by scalar code, and x >= xmiddle by vector code
                IReg data_rs = data + W * elemsize * y;
                IReg result_rs = result + W0 * elemsize * y;
                IReg x = (padhor ? -padding_left : CONST_(0));
#ifdef HORIZONTAL_OFFSET
                WHILE_(x < xme)
                {
                    IReg data__ = data_rs + x * elemsize;
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
                            data__ += W * elemsize;
                    }
                    storevec<float>(result_rs, x * elemsize, vres);
                    x += CTX.vlanes<float>();
                }
#else// HORIZONTAL_OFFSET
                IReg Wcond;
                VReg<uint32_t> WcondV;
                VReg<int32_t> idx_step;
                if(padhor)
                {
                    Wcond.rawcopy(max(W - kw - 2, CONST_(0)));
                    WcondV.rawcopy(VReg<uint32_t>(W));
                    idx_step.rawcopy(VCONST_(int32_t, CTX.vlanes<float>()));
                }
                WHILE_(x < xvectorend)
                {
                    IReg data__ = data_rs + (x << elemshift);
                    if(padhor)
                    {
                        IReg xcond = select(x < 0, Wcond, x);//DUBUGGG: just find unsigned comparisson.
                        IF_(xcond<Wcond)
                        {
                            fixedVolumeHandler(vbias, countingPattern, idx_step, WcondV, vkernel, x, data__, W, result_rs, 
                                                padding_left, elemsize, kh, kw, PADSHIFTRES);
                            CONTINUE_;
                        }
                        ELSE_
                        {
                            fixedVolumeHandler(vbias, countingPattern, idx_step, WcondV, vkernel, x, data__, W, result_rs, 
                                            padding_left, elemsize, kh, kw,  PADCOND | PADSHIFTRES);
                        }
                    }
                    else 
                        fixedVolumeHandler(vbias, countingPattern, idx_step, WcondV, vkernel, x, data__, W, result_rs, 
                                            padding_left, elemsize, kh, kw, 0);
                }
#endif// HORIZONTAL_OFFSET
                IReg scalarend_;
                IReg* scalarend;
                if(padhor || padver)
                {
                    scalarend_.rawcopy(W0-padding_left);
                    scalarend = &scalarend_;
                }
                else
                    scalarend = &W0;
                WHILE_(x < *scalarend)
                {
                    VReg<float> vres = vbias;
                    IReg data__ = data_rs + x * elemsize;
                    IReg kernel__ = kernel;
                    IReg kcol = CONST_(0);
                    IReg krow = CONST_(0);
                    IReg istride = (W - (kw-1)) * elemsize;
                    //We are working here at ends of arrays and there possibility of reading/ writing end of
                    //page, thus it's better to not use loadlane and storelane instructions: 
                    //even if base+(lane + 1)*elemsize  is inside of valid memory, tail can be outside and 
                    //this can cause memory exception. So, we are using general load/store operation instead. 
                    WHILE_(krow < kh)
                    {
                        if(padhor | padver)
                        {
                            IReg ex = x + kcol;
                            IReg ey = y + krow;
                            select(ex < 0, W, ex);
                            select(ey < 0, H, ey);
                            IF_(ex<W)
                                IF_(ey<H)
                                {
                                    VReg<float> justloaded = load_<float>(data__);
                                    VReg<float> w = load_<float>(kernel__);
                                    vres = fma(vres, justloaded, w);
                                }
                        }
                        else
                        {
                            VReg<float> justloaded = load_<float>(data__);
                            VReg<float> w = load_<float>(kernel__);
                            vres = fma(vres, justloaded, w);
                        }
                        kernel__ += elemsize;
                        kcol = kcol + 1;
                        data__ = select(kcol == kw, data__ + istride, data__ + elemsize);
                        krow = select(kcol == kw, krow + 1, krow);
                        kcol = select(kcol == kw, 0, kcol);
                    }
                    store_<float>(result_rs + (((padhor||padver) ? x + padding_left : x)<<elemshift), getlane<float>(vres, 0));
                    x += 1;
                }
                y += 1;
            }
            data += H * W * elemsize;
            result += H0 * W0 * elemsize;  
            kernel += kw * kh * elemsize;  
            bias += elemsize;
            channel = channel + 1;
        }
        RETURN_(0);
    }
    return (dwconv_t)(CTX.getFunc(funcname).ptr());
}

void DepthwiseconvTest::fixedVolumeHandler(const VReg<float>& vbias, const VReg<int32_t>& countingPattern, const VReg<int32_t>& idx_step,
                                           const VReg<uint32_t>& WcondV, const std::vector<VReg<float> >& vkernel, IReg& x, 
                                           IReg& base, const IReg& W, const IReg& result_rs, const IReg& padding_left,
                                           int elemsize, int kh, int kw,  int flags)
{
    USE_CONTEXT_(CTX);
    VReg<float> vres = vbias;
    const int elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
    VReg<int32_t> horIdxs;
    int lvflags = flags&PADCOND;
    if(flags&PADCOND) 
    {
        horIdxs.rawcopy(VReg<int32_t>(x));//DUBUGGG: motivation to forbid broadcast via operator= or at least add explicit broadcast function.
        horIdxs += countingPattern;
    }
    for(int krow = 0; krow < kh; krow++) 
    {
        VReg<float> loadedHalf0_, loadedHalf1_;
        loadVector(base, 0, loadedHalf0_, horIdxs, idx_step, WcondV, INITDEST | lvflags); 
        if(kw > 1)
            loadVector(base, CTX.vbytes(), loadedHalf1_, horIdxs, idx_step, WcondV, INITDEST | PREINCREMENT_IDXS | lvflags);
        VReg<float>* loadedHalf0 = &loadedHalf0_;
        VReg<float>* loadedHalf1 = &loadedHalf1_;
        for(int kcol = 0; kcol < kw; kcol++) 
        {
            VReg<float> spliced; //rename to "spliced".
            VReg<float>* toAdd;
            if(kcol%CTX.vlanes<float>() == 0 && kcol > 0)
            {
                std::swap(loadedHalf0, loadedHalf1);
                if(kcol + 1 < kw)
                    loadVector(base, kcol*elemsize+CTX.vbytes(), (*loadedHalf1), horIdxs, idx_step, WcondV, PREINCREMENT_IDXS | lvflags);
                toAdd = loadedHalf0;
            }
            else
            {
                spliced.rawcopy(ext(*loadedHalf0, *loadedHalf1, kcol%CTX.vlanes<float>()));
                toAdd = &spliced;
            }
            const int kerelemnum = krow*kw + kcol;
            vres = fma(vres, *toAdd, vkernel[kerelemnum/CTX.vlanes<float>()], kerelemnum%CTX.vlanes<float>());
        }
        if(krow + 1 < kh)
        {
            base += W * elemsize;
            if(flags&PADCOND) 
                horIdxs = countingPattern + VReg<int32_t>(x);
        }

    }
    storevec<float>(result_rs, ((flags&PADSHIFTRES) ? x + padding_left : x) << elemshift, vres);
    x += CTX.vlanes<float>();
}

void DepthwiseconvTest::loadVector(const IReg& base, int64_t offset, VReg<float>& dest, VReg<int32_t>& horIdxs, const VReg<int32_t>& idx_step, const VReg<uint32_t>& WcondV, int flags)
{
    USE_CONTEXT_(CTX);
    if(flags&INITDEST) 
        dest.rawcopy(loadvec<float>(base, offset));
    else
        dest = loadvec<float>(base, offset);
    if(flags&PADCOND)
    {
        if(flags&PREINCREMENT_IDXS)
            horIdxs += idx_step;
        VReg<uint32_t> mask = (reinterpret<uint32_t>(horIdxs) < WcondV);
        dest = reinterpret<float>( mask & reinterpret<uint32_t>(dest));
    }
}

void DepthwiseconvTest::gendata(float* data, float* kernel, float* bias, int kh, int kw, int H, int W, int C)
{
    for (int i = 0 ; i < C*H*W ; i++)
        data[i] = rand() % 1000;
    for (int i = 0 ; i < C*kw*kh ; i++)
        kernel[i] = 1;
    for (int i = 0 ; i < C ; i++)
        bias[i] = 0;
//DUBUGGG
    // for (int i = 0 ; i < C*kw*kh ; i++)
    //     kernel[i] = (rand() % 10000)/2500.0f - 2;
    // for (int i = 0 ; i < C ; i++)
    //     bias[i] = (rand() % 10000)/2500.0f - 2;
}

void DepthwiseconvTest::ref(float* data, float* kernel, float* bias, int H, int W, int C, float* result, int H0, int W0, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right)
{
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
                    if ((j - padding_left + kcol >= 0) &&
                        (j - padding_left + kcol  < W) &&
                        (i - padding_top  + krow >= 0) &&
                        (i - padding_top  + krow  < H))
                    {
                        float* inC__ = inC - padding_top * H - padding_left;
                        float32x4_t toAdd = vdupq_n_f32(inC__[koffsets[k]]);
                        float32x4_t weight = vdupq_n_f32(ker[k]);
                        res = vfmaq_f32(res, toAdd, weight);
                    }
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