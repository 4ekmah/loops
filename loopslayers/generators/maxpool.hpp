/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_MAXPOOL_HPP__
#define __LOOPS_MAXPOOL_HPP__

#if __LOOPS_ARCH == __LOOPS_AARCH64
#include "loops/loops.hpp"
#include <algorithm>
#include <cstddef>
#include <vector>
#include <iostream>
#include <iomanip>
#include "arm_neon.h"
#include "../test/tests.hpp"

namespace loops
{

class MaxpoolGenerator
{
public:
    MaxpoolGenerator(Context aCTX) : CTX(aCTX), m_done(false) {}
    typedef int64_t (*maxpool_t)(float* data, int64_t H, int64_t W, int64_t C, float* result, int64_t H0, int64_t W0, maxpool_algs_limits* algsLimits);
    maxpool_t generate(int kh_, int kw_, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type, float alpha);
    maxpool_algs_limits calc_maxpool_algs_limits(int C, int W, int H, int kw, int kh, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right);
private:
    bool m_done; 
    Context CTX;
    enum { MULTI_H = 3 };
    enum { PADHOR = 1, PADVER = 2, INITDEST = 4, PREINCREMENT_IDXS = 8, PADSHIFTRES = 16, MULTILINE = 32 };
    void multilineHandler(const VReg<uint32_t>& HcondV, const VReg<uint32_t>& WcondV, IReg& yi, IReg& x, IReg& base, const IReg& result_rs, int flags);
    void onlylineHandler(const VReg<uint32_t>& WcondV, IReg& yi, IReg& x, IReg& base, const IReg& result_rs, int flags);
    void loadVector(const IReg& base, int64_t offset, VReg<float>& dest, VReg<int32_t>& horIdxs, const VReg<uint32_t>& verMask, const VReg<uint32_t>& WcondV, int flags = 0);

    void multilineInit(int& horVecsPerOut, const IReg& data, const IReg& x, std::vector<std::vector<VReg<float>>>& vertMaxes, const IReg& stride, int& kh, int& init);
    void multilineInitLast(int lastVRegNum, std::vector<std::vector<VReg<float>>>& vertMaxes, const IReg& stride, int kh, const IReg& vertMaxesPtr, IReg& offset);
    void onelineInit(int& horVecsPerOut, const IReg& data, const IReg& x, std::vector<VReg<float>>& vertMaxes, const IReg& stride, int& kh, int& init);

    //Common parameters and registers
    int kh, kw, elemsize, elemshift, padding_top, padding_left, padding_bottom, padding_right, activation_type;
    IReg H, W, W0;
    float alpha;
    VReg<int32_t> countingPattern, idx_step;
    VReg<float> valpha;
    VReg<float> activationFunction(VReg<float>& res);

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

//#define HORIZONTAL_OFFSET
// MaxpoolGenerator::maxpool_t MaxpoolGenerator::generate(int kh_, int kw_, int padding_top_, int padding_left_, int padding_bottom_, int padding_right_, int activation_type_, float alpha_)
// {
//     if(m_done)
//         throw std::runtime_error("One generator object can create only one function. Create another generator.");
//     kh = kh_; kw = kw_;
//     elemsize = sizeof(float);
//     elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
//     padding_top = padding_top_; padding_left = padding_left_; padding_bottom = padding_bottom_; padding_right = padding_right_;
//     activation_type = activation_type_;
//     alpha = alpha_;
//     if(alpha == 1) 
//         activation_type = ACT_NONE;
//     std::string funcname = "max_pooling_kH";
//     const bool padver = (padding_top || padding_bottom);
//     const bool padhor = (padding_left || padding_right);
//     int handlerFlags = (padhor ? PADHOR : 0) | (padver ? PADVER : 0); 
//     int pshiftflag = (padhor ? PADSHIFTRES : 0);
//     funcname += std::to_string(kh) + "_kW" + std::to_string(kw) + "_pT" + std::to_string(padding_top) + "_pL" + std::to_string(padding_left) + "_pB" + std::to_string(padding_bottom) + "_pR" + std::to_string(padding_right);
//     if(activation_type != ACT_NONE) 
//         funcname += activation_type == ACT_RELU ? "_Relu" : 
//                    (activation_type == ACT_RELU6 ? "_Relu6" : 
//                    (/*activation_type == ACT_LRELU ?*/ (alpha < 1 ? "_LRelu" : "_LReluRev")/* : "")*/));
//     if(CTX.hasFunc(funcname))
//     {
//         m_done = true;
//         return (maxpool_t)(CTX.getFunc(funcname).ptr());
//     }
//     size_t kernelRegsAmount = kh*kw;
//     kernelRegsAmount = kernelRegsAmount/CTX.vlanes<float>() + (kernelRegsAmount%CTX.vlanes<float>()?1:0);
//     vkernel.resize(kernelRegsAmount, VReg<float>());
//     IReg data, bias, C, result, H0, algsLimits;
//     USE_CONTEXT_(CTX);
//     STARTFUNC_(funcname, &data, &kernel, &bias, &H, &W, &C, &result, &H0, &W0, &algsLimits)
//     {
//         if(activation_type == ACT_LRELU)
//         {
//             valpha.copyidx(VCONST_(float, alpha));
//         }
//         if(padhor)
//         {
//             countingPattern.copyidx(VCONST_(int32_t, 0));
//             for(int lane = 1; lane < CTX.vlanes<float>(); lane++)
//                 setlane(countingPattern, lane, CONST_(lane));
//             idx_step.copyidx(VCONST_(int32_t, CTX.vlanes<float>()));
//         }
//         IReg Cms = load_<int64_t>(algsLimits, offsetof(maxpool_algs_limits, Cms));
//         IReg Cme = load_<int64_t>(algsLimits, offsetof(maxpool_algs_limits, Cme));
//         IReg Cis = load_<int64_t>(algsLimits, offsetof(maxpool_algs_limits, Cis));
//         IReg Cie = load_<int64_t>(algsLimits, offsetof(maxpool_algs_limits, Cie));
//         IReg Yms = load_<int64_t>(algsLimits, offsetof(maxpool_algs_limits, Yms));
//         IReg Yme = load_<int64_t>(algsLimits, offsetof(maxpool_algs_limits, Yme));
//         IReg Yis = load_<int64_t>(algsLimits, offsetof(maxpool_algs_limits, Yis));
//         IReg Yie = load_<int64_t>(algsLimits, offsetof(maxpool_algs_limits, Yie));
//         IReg Xis = load_<int64_t>(algsLimits, offsetof(maxpool_algs_limits, Xis)) - padding_left;
//         IReg Xie = load_<int64_t>(algsLimits, offsetof(maxpool_algs_limits, Xie)) - padding_left;

//         IReg channel = CONST_(0);
//         WHILE_(channel < C)
//         {
//             IReg yms = select(channel == Cms - 1, Yms, H0); 
//             yms = select(channel > Cms - 1, 0, yms); 
//             yms = select(channel > Cme, H0, yms); 
//             IReg yme = select(channel >= Cms - 1, H0, 0); 
//             yme = select(channel == Cme, Yme, yme); 
//             yme = select(channel > Cme, 0, yme);
//             IReg yis = select(channel == Cis - 1, Yis, H0); 
//             yis = select(channel > Cis - 1, 0, yis); 
//             yis = select(channel > Cie, H0, yis); 
//             IReg yie = select(channel >= Cis - 1, H0, 0); 
//             yie = select(channel == Cie, Yie, yie); 
//             yie = select(channel > Cie, 0, yie);
//             vbias.copyidx(broadcast<float>(load_<float>(bias)));

//             IReg y = CONST_(0);
//             IReg yonelineEnd = select(yms > y, yms , H0); //SIMD + scalar.

//             WHILE_(y < H0)
//             {
//                 IF_(y == yms)
//                 {
//                     for(int kregnum = 0; kregnum < kernelRegsAmount; kregnum++)
//                         vkernel[kregnum].copyidx(loadvec<float>(kernel, kregnum * CTX.vbytes()));
//                     WHILE_(y < yme)
//                     {
//                         IReg data_rs = data + ((W * (padver ? y - padding_top: y)) << elemshift);
//                         IReg result_rs = result + (W0 << elemshift) * y;
//                         IReg xi = CONST_(-padding_left);
//                         IReg Hcond = padver ? max(H - (kh + MULTI_H - 2), CONST_(0)) : IReg(); 
//                         IReg Wcond = padhor ? max(W - kw - 2, CONST_(0)) : IReg();
//                         VReg<uint32_t> WcondV = padhor ? broadcast<uint32_t>(W) : VReg<uint32_t>();
//                         VReg<uint32_t> HcondV = padver ? broadcast<uint32_t>(H) : VReg<uint32_t>();
//                         IReg multilineendx;
//                         multilineendx.copyidx(padding_left ? W0-padding_left : W0);
//                         IReg hldx = (padding_left ? W0 - padding_left: W0) - CTX.vlanes<float>();
//                         WHILE_(xi < multilineendx)
//                         {
//                             xi = select(xi > hldx , hldx , xi);
//                             IReg data__ = data_rs + (xi << elemshift);
//                             if(padhor||padver)
//                             {
//                                 IReg yi = padver ? y - padding_top : IReg();
//                                 IReg xcond = padver&&padhor ? select(ult(yi,Hcond), xi, Wcond) : xi;
//                                 IF_(padhor?(ult(xcond, Wcond)):ult(yi, Hcond))
//                                 {
//                                     multilineHandler(HcondV, WcondV, yi, xi, data__, result_rs, pshiftflag);
//                                     CONTINUE_;
//                                 }
//                                 multilineHandler(HcondV, WcondV, yi, xi, data__, result_rs, handlerFlags | pshiftflag);
//                             }
//                             else 
//                                 multilineHandler(HcondV, WcondV, y, xi, data__, result_rs, 0);
//                         }
//                         y += MULTI_H;
//                     }
//                     yonelineEnd = H0;
//                 }
//                 WHILE_(y < yonelineEnd)
//                 {
//                     IReg xis = select(y < yis, Xis, 0);
//                     IReg xie = select(y >= yie, Xie, 0);
//                     if(padding_left) { xis-=padding_left; xie-=padding_left;}
//                     IReg xi = CONST_(-padding_left);
//                     IReg W0mpl;
//                     W0mpl.copyidx(padding_left? W0 - padding_left: W0);
//                     IReg scalarEnd = select(xis > xi, xis, W0mpl);

//                     IReg data_rs = data + ((W * (padver ? y - padding_top: y)) << elemshift);
//                     IReg result_rs = result + (W0 << elemshift) * y;
//                     WHILE_(xi < W0mpl)
//                     {
//                         IF_(xi == xis)
//                         {
//                             IReg Wcond = padhor ? max(W - kw - 2, CONST_(0)): IReg();
//                             IReg Hcond = padver ? max(H - (kh - 1), CONST_(0)) : IReg();
//                             VReg<uint32_t> WcondV = padhor ? broadcast<uint32_t>(W) : VReg<uint32_t>();
//                             WHILE_(xi < xie)
//                             {
//                                 IReg data__ = data_rs + (xi << elemshift);
//                                 if(padhor||padver)
//                                 {
//                                     IReg yi = padver ? y - padding_top : IReg();
//                                     IReg xcond;
//                                     xcond.copyidx(padhor && padver ? select(ult(yi, Hcond), xi, Wcond): xi);
//                                     IF_(padhor?ult(xcond,Wcond):ult(yi, Hcond))
//                                     {
//                                         onlylineHandler(WcondV, yi, xi, data__, result_rs, pshiftflag);
//                                         CONTINUE_;
//                                     }
//                                     onlylineHandler(WcondV, yi, xi, data__, result_rs, handlerFlags | pshiftflag);
//                                 }
//                                 else 
//                                     onlylineHandler(WcondV, y, xi, data__, result_rs, 0);
//                             }
//                             scalarEnd = W0mpl;
//                         }
//                         WHILE_(xi < scalarEnd)
//                         {
//                             VReg<float> vres = vbias;
//                             IReg data__ = data_rs + (xi << elemshift);
//                             IReg kernel__ = kernel;
//                             IReg kcol = CONST_(0);
//                             IReg krow = CONST_(0);
//                             IReg istride = (W - (kw-1)) << elemshift;
//                             //We are working here at ends of arrays and there possibility of reading/ writing end of
//                             //page, thus it's better to not use loadlane and storelane instructions: 
//                             //even if base+(lane + 1)*elemsize  is inside of valid memory, tail can be outside and 
//                             //this can cause memory exception. So, we are using general load/store operation instead. 
//                             WHILE_(krow < kh)
//                             {
//                                 if(padhor||padver)
//                                 {
//                                     IReg ex = xi + kcol;
//                                     IReg ey = (padver ? y - padding_top : y) + krow;
//                                     select(ex < 0, W, ex);
//                                     select(ey < 0, H, ey);
//                                     IF_(ult(ex,W))
//                                     {
//                                         IF_(ult(ey,H))
//                                         {
//                                             VReg<float> justloaded = broadcast<float>(load_<float>(data__));
//                                             VReg<float> w = broadcast<float>(load_<float>(kernel__));
//                                             {
//                                                 VReg<float> antiSpill = vres; //TODO(ch): remove it when snippet management will be better.
//                                                 vres = fma(antiSpill, justloaded, w);
//                                             }
//                                         }
//                                     }
//                                 }
//                                 else
//                                 {
//                                     VReg<float> justloaded = broadcast<float>(load_<float>(data__));
//                                     VReg<float> w = broadcast<float>(load_<float>(kernel__));
//                                     {
//                                         VReg<float> antiSpill = vres; //TODO(ch): remove it when snippet management will be better.
//                                         vres = fma(antiSpill, justloaded, w);
//                                     }
//                                 }
//                                 kernel__ += elemsize;
//                                 kcol = kcol + 1;
//                                 data__ = select(kcol == kw, data__ + istride, data__ + elemsize);
//                                 krow = select(kcol == kw, krow + 1, krow);
//                                 kcol = select(kcol == kw, 0, kcol);
//                             }
//                             IReg roffset = (((padhor||padver) ? xi + padding_left : xi) << elemshift);
//                             store_<float>(result_rs + roffset, getlane<float>(activationFunction(vres), 0));
//                             xi += 1;
//                         }
//                     }
//                     y += 1;
//                 }
//             }
//             data += H * W << elemshift;
//             result += H0 * W0 << elemshift;  
//             kernel += kw * kh << elemshift;  
//             bias += elemsize;
//             channel = channel + 1;
//         }
//         RETURN_(0);
//     }
//     m_done = true;
//     return (maxpool_t)(CTX.getFunc(funcname).ptr());
// }

// typedef int (*minmaxpos_t)(float* data, int H, int W, int C, float* result, int H0, int W0);
// loops::Func genminmaxloc(loops::Context& CTX, int64_t kh, int64_t kw)
MaxpoolGenerator::maxpool_t MaxpoolGenerator::generate(int kh_, int kw_, int padding_top_, int padding_left_, int padding_bottom_, int padding_right_, int activation_type_, float alpha_)
{
    if(m_done)
        throw std::runtime_error("One generator object can create only one function. Create another generator.");
    kh = kh_; kw = kw_;
    // elemsize = sizeof(float);
    // elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
    padding_top = padding_top_; padding_left = padding_left_; padding_bottom = padding_bottom_; padding_right = padding_right_;
    activation_type = activation_type_;
    alpha = alpha_;
    if(alpha == 1) 
        activation_type = ACT_NONE;
    std::string funcname = "max_pooling_kH";
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
        return (maxpool_t)(CTX.getFunc(funcname).ptr());
    }
    // size_t kernelRegsAmount = kh*kw;
    // kernelRegsAmount = kernelRegsAmount/CTX.vlanes<float>() + (kernelRegsAmount%CTX.vlanes<float>()?1:0);
    // vkernel.resize(kernelRegsAmount, VReg<float>());
    IReg data, bias, C, result, H0, algsLimits;
    USE_CONTEXT_(CTX);
    // STARTFUNC_(funcname, &data, &kernel, &bias, &H, &W, &C, &result, &H0, &W0, &algsLimits)


    elemsize = sizeof(float);
    elemshift = (elemsize == 8 ? 3 : (elemsize == 4 ? 2 : (elemsize == 2 ? 1 : 0)));  
    using namespace loops;

    // IReg data, H, W, C, result, H0, W0, debug_var;
    // USE_CONTEXT_(CTX);
    // STARTFUNC_("minmaxloc", &data, &H, &W, &C, &result, &H0, &W0)
    STARTFUNC_(funcname, &data, &H, &W, &C, &result, &H0, &W0, &algsLimits)
    {
        int horVecsPerOut = (kw + 3)/4 + ((kw + 3)%4?1:0);

        getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, 
            { 0, 1, 2, 3, 4, 5, 6, 7 }, 
            { 0, 1, 2, 3, 4, 5, 6, 7 }, 
            { }, 
            { 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28 });

        IReg offset = CONST_(0);
        IReg c = CONST_(0);
        IReg check = W0 << elemshift;

        std::vector<IReg> offstab(kw, IReg());
        for(int i = 0; i < kw; i++ )
            offstab[i].copyidx(IReg(CONST_(i) << elemshift));
        IReg H0x3 = H0 - H0 % 3;
        IReg stride = W << elemshift;
        IReg stride0 = W0 << elemshift;
        IReg Wcond = max(CONST_(0), W0 - CTX.vlanes<float>());
        IReg vsize = CONST_(4); // CTX.vbytes() >> elemshift;
        WHILE_(c < C)
        {
            IReg y = CONST_(0);
            IReg result_ = result + (c * W0 * H0 << elemshift);
            IReg data_ = data + (c * W * H << elemshift);

            if (kh > 2)
            {
                WHILE_(y < H0x3) 
                {
                    IReg x = CONST_(0);

                    std::vector<std::vector<VReg<float> > > vertMaxes(3, std::vector<VReg<float>>(horVecsPerOut, VReg<float>()));
                    vertMaxes.resize(3, std::vector<VReg<float>>(horVecsPerOut, VReg<float>()));
                    int init = 1;
                    multilineInit(horVecsPerOut, data_, x, vertMaxes, stride, kh, init);
                    
                    WHILE_(x < W0) 
                    {
                        IF_(x > Wcond)
                        {
                            x = Wcond;
                            init = 0;
                            multilineInit(horVecsPerOut, data_, x, vertMaxes, stride, kh, init);
                        } 

                        IReg result__ = result_ + (x << elemshift);
                        int lastVRegNum = horVecsPerOut - 1;

                        IReg vertMaxesPtr = data_ + (x << elemshift) + lastVRegNum * CTX.vbytes();
                        VReg<float> temp;

                        IReg offset = stride;
                        multilineInitLast(lastVRegNum, vertMaxes, stride, kh, vertMaxesPtr, offset);

                        vertMaxes[0][lastVRegNum] = max(vertMaxes[0][lastVRegNum], vertMaxes[2][lastVRegNum]);
                        offset += stride;
                        vertMaxes[2][lastVRegNum] = max(vertMaxes[2][lastVRegNum], loadvec<float>(vertMaxesPtr, offset));
                        vertMaxes[1][lastVRegNum] = max(vertMaxes[1][lastVRegNum], vertMaxes[2][lastVRegNum]);
                        offset += stride;
                        vertMaxes[2][lastVRegNum] = max(vertMaxes[2][lastVRegNum], loadvec<float>(vertMaxesPtr, offset));

                        std::vector<VReg<float> > res(3, VReg<float>());
                        for(int n = 0; n < 3; n++)
                            res[n].copyidx(max(vertMaxes[n][0], ext(vertMaxes[n][0], vertMaxes[n][1], 1)));

                        for(int i = 2; i < kw; i++ )
                        {
                            for(int n = 0; n < 3; n++)
                            {
                                int vRegNum = i / 4;
                                if (i % 4 == 0) 
                                    res[n] = max(res[n], vertMaxes[n][vRegNum]);
                                else
                                    res[n] = max(res[n], ext(vertMaxes[n][vRegNum], vertMaxes[n][vRegNum+1], i % 4));
                            }
                        }

                        for(int n = 0; n < 3; n++)
                        {
                            for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
                            {
                                vertMaxes[n][vRegNum] = vertMaxes[n][vRegNum+1];
                            }
                        }
                        
                        storevec(result__, res[0]);
                        storevec(result__ + stride0, res[1]);
                        storevec(result__ + 2 * stride0, res[2]);
                        
                        x += vsize;
                    }
                    y += 3;
                    result_ += W0 * 3 << elemshift;
                    data_ += W * 3 << elemshift;
                }
            }
            
            WHILE_(y < H0) 
            {
                IReg x = CONST_(0);
                int horVecsPerOut = (kw + 3)/4 + ((kw + 3)%4?1:0);
                IReg stride = W << elemshift;
                std::vector<VReg<float> > vertMaxes(horVecsPerOut, VReg<float>());
                int init = 1;
                onelineInit(horVecsPerOut, data_, x, vertMaxes, stride, kh, init);
                
                WHILE_(x < W0) 
                {
                    IF_(x > Wcond)
                    {
                        x = Wcond;
                        init = 0;
                        onelineInit(horVecsPerOut, data_, x, vertMaxes, stride, kh, init);
                    } 
                    IReg data__ = data_ + (x << elemshift);
                    IReg result__ = result_ + (x << elemshift);

                    int lastVRegNum = horVecsPerOut-1;
                    IReg vertMaxesPtr = data__ + lastVRegNum * CTX.vbytes();
                    vertMaxes[lastVRegNum].copyidx(max(loadvec<float>(vertMaxesPtr, 0), loadvec<float>(vertMaxesPtr, stride)));
                    for(int i = 2; i < kh; i++)
                        vertMaxes[lastVRegNum] = max(vertMaxes[lastVRegNum], loadvec<float>(vertMaxesPtr, (i * stride)));

                    VReg<float> res = max(vertMaxes[0], ext(vertMaxes[0], vertMaxes[1], 1));
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
                    storevec(result__, res);
                    
                    x += vsize;
                }
                y += 1;
                result_ += W0 << elemshift;
                data_ += W << elemshift;
            }
            c += 1;
        }
        RETURN_(0);
        // return CTX.getFunc("minmaxloc");
    }
    m_done = true;
    return (maxpool_t)(CTX.getFunc(funcname).ptr());
}

void MaxpoolGenerator::multilineInit(int& horVecsPerOut, const loops::IReg& data, const loops::IReg& x, std::vector<std::vector<loops::VReg<float>>>& vertMaxes, const loops::IReg& stride, int& kh, int& init)
// void multilineInit(int& horVecsPerOut, const loops::IReg& data, const loops::IReg& x, std::vector<std::vector<loops::VReg<float>>>& vertMaxes, const loops::IReg& stride, int64_t& kh, int& init)
{
    using namespace loops;
    USE_CONTEXT_(ExtractContext(data));
    for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
    {
        IReg vertMaxesPtr = data + (x << elemshift) + vRegNum * ExtractContext(data).vbytes();
        VReg<float> temp;
        IReg offset = stride;
        if (init == 1)
        {
            vertMaxes[0][vRegNum].copyidx(loadvec<float>(vertMaxesPtr));
            vertMaxes[1][vRegNum].copyidx(loadvec<float>(vertMaxesPtr, offset));
            vertMaxes[0][vRegNum] = max(vertMaxes[0][vRegNum], vertMaxes[1][vRegNum]);
            offset += stride;
            vertMaxes[2][vRegNum].copyidx(loadvec<float>(vertMaxesPtr, offset));
        }
        else
        {
            vertMaxes[0][vRegNum] = vertMaxes[0][vRegNum];//TODO(ch): Waiting for fixing liveness analysis issue(def-while-if-def-endif-use fusion).
            vertMaxes[0][vRegNum] = (loadvec<float>(vertMaxesPtr));
            vertMaxes[1][vRegNum] = vertMaxes[1][vRegNum];//TODO(ch): Waiting for fixing liveness analysis issue(def-while-if-def-endif-use fusion).
            vertMaxes[1][vRegNum] = (loadvec<float>(vertMaxesPtr, offset));
            vertMaxes[0][vRegNum] = max(vertMaxes[0][vRegNum], vertMaxes[1][vRegNum]);
            offset += stride;
            vertMaxes[2][vRegNum] = vertMaxes[2][vRegNum];//TODO(ch): Waiting for fixing liveness analysis issue(def-while-if-def-endif-use fusion).
            vertMaxes[2][vRegNum] = (loadvec<float>(vertMaxesPtr, offset));
        }

        for(int row = 3; row < kh; row++)
        {
            offset += stride;
            vertMaxes[2][vRegNum] = max(vertMaxes[2][vRegNum], loadvec<float>(vertMaxesPtr, offset));
        }

        vertMaxes[0][vRegNum] = max(vertMaxes[0][vRegNum], vertMaxes[2][vRegNum]);
        offset += stride;
        vertMaxes[2][vRegNum] = max(vertMaxes[2][vRegNum], loadvec<float>(vertMaxesPtr, offset));
        vertMaxes[1][vRegNum] = max(vertMaxes[1][vRegNum], vertMaxes[2][vRegNum]);
        offset += stride;
        vertMaxes[2][vRegNum] = max(vertMaxes[2][vRegNum], loadvec<float>(vertMaxesPtr, offset));
    }
}

void MaxpoolGenerator::multilineInitLast(int lastVRegNum, std::vector<std::vector<loops::VReg<float>>>& vertMaxes, const loops::IReg& stride, int kh, const loops::IReg& vertMaxesPtr, loops::IReg& offset)
{
    using namespace loops;
    USE_CONTEXT_(ExtractContext(stride));
    vertMaxes[0][lastVRegNum].copyidx(loadvec<float>(vertMaxesPtr));
    vertMaxes[1][lastVRegNum].copyidx(loadvec<float>(vertMaxesPtr, offset));
    vertMaxes[0][lastVRegNum] = max(vertMaxes[0][lastVRegNum], vertMaxes[1][lastVRegNum]);
    offset += stride;
    vertMaxes[2][lastVRegNum].copyidx(loadvec<float>(vertMaxesPtr, offset));
    for(int row = 3; row < kh; row++)
    {
        offset += stride;
        vertMaxes[2][lastVRegNum] = max(vertMaxes[2][lastVRegNum], loadvec<float>(vertMaxesPtr, offset));
    }
}

void MaxpoolGenerator::onelineInit(int& horVecsPerOut, const loops::IReg& data, const loops::IReg& x, std::vector<loops::VReg<float>>& vertMaxes, const loops::IReg& stride, int& kh, int& init)
{
    using namespace loops;
    USE_CONTEXT_(ExtractContext(data));
    for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
    {
        IReg vertMaxesPtr = data + (x << elemshift) + vRegNum * ExtractContext(data).vbytes();
        if (init == 1)
        {
            vertMaxes[vRegNum].copyidx(max(loadvec<float>(vertMaxesPtr, 0), loadvec<float>(vertMaxesPtr, stride)));
        }
        else
        {
            vertMaxes[vRegNum] = vertMaxes[vRegNum];//TODO(ch): Waiting for fixing liveness analysis issue(def-while-if-def-endif-use fusion).
            vertMaxes[vRegNum] = max(loadvec<float>(vertMaxesPtr, 0), loadvec<float>(vertMaxesPtr, stride));
        }
        for(int i = 2; i < kh; i++)
            vertMaxes[vRegNum] = max(vertMaxes[vRegNum], loadvec<float>(vertMaxesPtr, (i * stride)));
    }
}

void MaxpoolGenerator::multilineHandler(const VReg<uint32_t>& HcondV, const VReg<uint32_t>& WcondV, IReg& yi, IReg& x, IReg& base, const IReg& result_rs, int flags)
{
    // USE_CONTEXT_(CTX);
    // std::vector<VReg<float> > vres(MULTI_H, VReg<float>());
    // for(int rnum = 0; rnum<MULTI_H; rnum++)
    //     vres[rnum].copyidx(VReg<float>(vbias)); 
    // const int elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
    // int lvflags = flags&(PADHOR|PADVER);
    // for(int lrow = 0; lrow < kh + MULTI_H - 1; lrow++) 
    // {
    //     VReg<int32_t> horIdxs = (flags&PADHOR) ? broadcast<int32_t>(x) + countingPattern : VReg<int32_t>();
    //     VReg<uint32_t> verMask = (flags&PADVER) ? broadcast<uint32_t>(yi) < HcondV : VReg<uint32_t>();
    //     if((flags&PADVER)&&(flags&PADHOR))
    //     {
    //         VReg<int32_t> antiSpill = horIdxs; //TODO(ch): remove it when snippet management will be better.
    //         horIdxs = select(verMask, antiSpill, reinterpret<int32_t>(WcondV));
    //     }
    //     VReg<float> loadedHalf0, loadedHalf1;
    //     loadVector(base, 0, loadedHalf0, horIdxs, verMask, WcondV, INITDEST | lvflags);
    //     if(kw > 1)
    //         loadVector(base, CTX.vbytes(), loadedHalf1, horIdxs, verMask, WcondV, INITDEST | PREINCREMENT_IDXS | lvflags);

    //     for(int kcol = 0; kcol < kw; kcol++) 
    //     {
    //         VReg<float> toAdd;
    //         if(kcol%CTX.vlanes<float>() == 0 && kcol > 0)
    //         {
    //             VReg<float> inter;
    //             inter.copyidx(loadedHalf0);
    //             loadedHalf0.copyidx(loadedHalf1);
    //             loadedHalf1.copyidx(inter);
    //             if(kcol + 1 < kw)
    //                 loadVector(base, kcol*elemsize+CTX.vbytes(), loadedHalf1, horIdxs, verMask, WcondV, PREINCREMENT_IDXS | lvflags);
    //             toAdd.copyidx(loadedHalf0);
    //         }
    //         else
    //             toAdd.copyidx(ext(loadedHalf0, loadedHalf1, kcol%CTX.vlanes<float>()));
    //         for(int lineNum = 0; lineNum < MULTI_H; lineNum++)
    //         { 
    //             const int krow = lrow - lineNum;
    //             if(krow >= 0 && krow < kh)
    //             {
    //                 const int kerelemnum = krow*kw + kcol;
    //                 vres[lineNum] = fma(vres[lineNum], toAdd, vkernel[kerelemnum/CTX.vlanes<float>()], kerelemnum%CTX.vlanes<float>());
    //             }
    //         }
    //     }
    //     if(lrow + 2 < kh + MULTI_H) //Not last loaded row
    //     {
    //         base += W << elemshift;
    //         if(flags&PADVER)
    //             yi += 1;
    //     }
    // }
    // IReg roffset = (flags&PADSHIFTRES? x + padding_left : x) << elemshift;
    // int outdiffW = padding_left + padding_right - kw + 1;
    // IReg W0_;
    // W0_.copyidx(outdiffW ? (outdiffW > 0 ? W+outdiffW: W-(-outdiffW)): W); //TODO(ch): fix it in interface of loops(there must be ability to add and sub negaitve numbers).
    // IReg offstride = W0_ << elemshift;
    // for(int lineNum = 0; lineNum < MULTI_H; lineNum++)
    // {
    //     storevec<float>(result_rs, roffset, activationFunction(vres[lineNum]));
    //     if(lineNum + 1 < MULTI_H)
    //         roffset += offstride;
    // }
    // x += CTX.vlanes<float>();
}

void MaxpoolGenerator::onlylineHandler(const VReg<uint32_t>& WcondV, IReg& yi, IReg& x, IReg& base, const IReg& result_rs, int flags)
{
    // USE_CONTEXT_(CTX);
    // VReg<float> vres = vbias;
    // const int elemshift = (elemsize == 8) ? 3 : ((elemsize == 4) ? 2 : 1);
    // int lvflags = flags&PADHOR;
    // IReg krow = CONST_(0);
    // WHILE_(krow<kh)
    // {
    //     if(flags&PADVER)
    //     {
    //         IF_(uge(yi,H))
    //         {
    //             base += W << elemshift;
    //             yi += 1;
    //             krow += 1;
    //             CONTINUE_;
    //         }
    //     }
    //     IReg kptr = kernel + krow * (kw * elemsize);
    //     VReg<uint32_t> dummy;
    //     VReg<int32_t> horIdxs = flags&PADHOR ? broadcast<int32_t>(x) + countingPattern : VReg<int32_t>();
    //     VReg<float> loadedHalf0, loadedHalf1;
    //     loadVector(base, 0, loadedHalf0, horIdxs, dummy, WcondV, INITDEST | lvflags);
    //     if(kw > 1)
    //         loadVector(base, CTX.vbytes(), loadedHalf1, horIdxs, dummy, WcondV, INITDEST | PREINCREMENT_IDXS | lvflags);

    //     for(int kcol = 0; kcol < kw; kcol++) 
    //     {
    //         VReg<float> spliced;
    //         VReg<float> toAdd;
    //         if(kcol%CTX.vlanes<float>() == 0 && kcol > 0)
    //         {
    //             VReg<float> interm;
    //             interm.copyidx(loadedHalf0);
    //             loadedHalf0.copyidx(loadedHalf1);
    //             loadedHalf1.copyidx(interm);
    //             if(kcol + 1 < kw)
    //                 loadVector(base, kcol*elemsize+CTX.vbytes(), loadedHalf1, horIdxs, dummy, WcondV, PREINCREMENT_IDXS | lvflags);
    //             toAdd.copyidx(loadedHalf0);
    //         }
    //         else
    //             toAdd.copyidx(ext(loadedHalf0, loadedHalf1, kcol%CTX.vlanes<float>()));
    //         VReg<float> w = broadcast<float>(load_<float>(kptr));
    //         {
    //             VReg<float> antiSpill = vres; //TODO(ch): remove it when snippet management will be better.
    //             vres = fma(antiSpill, toAdd, w);
    //         }
    //         kptr += elemsize;
    //     }
    //     base += W << elemshift;
    //     if(flags&PADVER)
    //         yi += 1;
    //     krow += 1;
    // }
    // IReg roffset = (flags&PADSHIFTRES? x + padding_left : x) << elemshift;
    // storevec<float>(result_rs, roffset, activationFunction(vres));
    // x += CTX.vlanes<float>();
}

void MaxpoolGenerator::loadVector(const IReg& base, int64_t offset, VReg<float>& dest, VReg<int32_t>& horIdxs, const VReg<uint32_t>& verMask, const VReg<uint32_t>& WcondV, int flags)
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

VReg<float> MaxpoolGenerator::activationFunction(VReg<float>& res)
{
    USE_CONTEXT_(CTX);
    switch(activation_type)
    {
        case(ACT_NONE): return static_cast<VReg<float>&&>(res); break;
        case(ACT_RELU): return static_cast<VReg<float>&&>(max(res, VCONST_(float, 0))); break;
        case(ACT_RELU6): return static_cast<VReg<float>&&>(max(min(res, VCONST_(float, 6)),VCONST_(float, 0))); break;
        case(ACT_LRELU): return static_cast<VReg<float>&&>(alpha < 1 ? max(res,res * valpha) : min(res,res * valpha) ); break;
        defaout: throw std::runtime_error("Unknown activation");
    };
    return VReg<float>();
}

inline int MaxpoolGenerator::upDiv(int numerator, int denominator) 
{
    if(numerator < 0)
        return -((-numerator)/denominator);
    int res = numerator / denominator;
    int back = res * denominator; 
    return (numerator - back ? res + 1: res);
}

inline int MaxpoolGenerator::upMultipleOf(int numerator, int denominator) 
{
    if(numerator < 0)
        return -downMultipleOf(-numerator, denominator);
    int div = numerator / denominator;
    int back = div * denominator; 
    return (numerator - back ? back + denominator : back);
}

inline int MaxpoolGenerator::downMultipleOf(int numerator, int denominator) 
{
    if(numerator < 0)
        return -upMultipleOf(-numerator, denominator);
    numerator /= denominator;
    numerator *= denominator;
    return numerator;
}

inline int MaxpoolGenerator::downBorder(int add, int ratio)
{
    return upDiv(add, ratio);
}

inline int MaxpoolGenerator::upperBorder(int add, int ratio)
{
    return upDiv(add, ratio);
}

inline int MaxpoolGenerator::downC(int C, int H, int W, int y, int x)
{
    return downBorder(-y*W-x, H*W);
}

inline int MaxpoolGenerator::upperC(int C, int H, int W, int y, int x)
{
    return upperBorder(C*H*W - y*W - x, H*W);
}

inline int MaxpoolGenerator::downY(int C, int H, int W, int Cf, int ys, int x)
{
    Cf = std::max(Cf,0);
    return downBorder(-Cf*H*W - x - ys * W, W);
}

inline int MaxpoolGenerator::upperY(int C, int H, int W, int Cf, int ys, int x)
{
    return upperBorder((C-Cf)*H*W - x - ys * W, W);
}

inline int MaxpoolGenerator::upperY(int C, int H, int W, int Cf, int ys, int M, int y0, int x)
{
    return M * upperBorder((C-Cf)*H*W - x - (ys + y0) * W, W * M) + y0;
}

inline int MaxpoolGenerator::downX(int C, int H, int W, int Cf, int Yf, int xs)
{
    Cf = std::max(Cf,0);
    Yf = std::max(Yf,0);
    return downBorder(-Cf*H*W - Yf * W - xs, 1);
}

inline int MaxpoolGenerator::upperX(int C, int H, int W, int Cf, int Yf, int xs)
{
    return upperBorder((C-Cf)*H*W - Yf * W - xs, 1);
}

inline int MaxpoolGenerator::upperX(int C, int H, int W, int Cf, int Yf, int xs, int M, int x0)
{
    return M * upperBorder((C-Cf)*H*W - Yf * W - xs - x0, M) + x0;
}

maxpool_algs_limits MaxpoolGenerator::calc_maxpool_algs_limits(int C, int W, int H, int kw, int kh, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right)
{
    int Cms, Cme;
    int lsimd = upMultipleOf(kw + CTX.vlanes<float>() - 2, CTX.vlanes<float>()) - 1;
    int XlastMulti = (W0 - padding_left - CTX.vlanes<float>()) + lsimd;
    if(W0 > CTX.vlanes<float>())
    {
        Cms = downC(C, H, W, -padding_top, -padding_left);
        int YlastMulti = (downMultipleOf(W0, MULTI_H) - padding_top + kh + MULTI_H - 2);
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
    int Yms = H0 + 1, Yme = H0;
    if(Cms < C + 1)
    {
        Yms = downY(C, H, W, Cms - 1, -padding_top, -padding_left);
        Yme = upperY(C, H, W, Cme, MULTI_H + kh - padding_top - 2, MULTI_H, ((Cms - 1) == Cme ? Yms : 0), XlastMulti);
    }
    int Yis = H0 + 1, Yie = H0;
    if(Cis < C + 1)
    {
        Yis = (padding_left == 0) ? 0 : downY(C, H, W, Cis - 1, -padding_top, -padding_left);
        if ((H - 1) * W + Xlast < (C-Cie)*H*W)
            Yie = H0;
        else
            Yie = std::max(0, upperY(C, H, W, Cie, kh - 1 - padding_top, Xlast));
    }
    int Xis = W0, Xie = W0;
    if(Yis < H0)
    {
        Xis = downX(C, H, W, Cis-1, Yis - 1 - padding_top, -padding_left);
        Xie = upperX(C, H, W, Cie, std::min(Yie - padding_top + kh - 1, H-1), lsimd-padding_left, CTX.vlanes<float>(), ((Cis - 1) == Cie && Yis == Yie)? Xis: 0);
    }
    return maxpool_algs_limits(Cms, Cme, Cis, Cie, Yms, Yme, Yis, Yie, Xis, Xie);
}
};
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
#endif //__LOOPS_MAXPOOL_HPP__

// #ifdef HORIZONTAL_OFFSET
//         std::vector<IReg> horoff(kw, IReg());
//         for(int kcol = 0; kcol < kw; kcol++)
//             if(kcol%CTX.vlanes<float>())
//                 horoff[kcol].rawcopy(CONST_(kcol<<elemshift));
// #endif //HORIZONTAL_OFFSET
// #ifdef HORIZONTAL_OFFSET
//                 WHILE_(xi < xme)
//                 {
//                     IReg data__ = data_rs + (xi << elemshift);
//                     VReg<float> vres = vbias;
//                     for(int krow = 0; krow < kh; krow++) 
//                     {
//                         for(int kcol = 0; kcol < kw; kcol++) 
//                         {
//                             const int kerelemnum = krow*kw + kcol;
//                             VReg<float> justloaded;
//                             if(kcol%CTX.vlanes<float>())
//                                 justloaded.rawcopy(loadvec<float>(data__, horoff[kcol]));
//                             else
//                                 justloaded.rawcopy(loadvec<float>(data__, kcol*elemsize));
//                             vres = fma(vres, justloaded, vkernel[kerelemnum/CTX.vlanes<float>()], kerelemnum%CTX.vlanes<float>());
//                         }
//                         if(krow + 1 < kh)
//                             data__ += W << elemshift;
//                     }
//                     storevec<float>(result_rs, xi << elemshift, vres);
//                     xi += CTX.vlanes<float>();
//                 }
// #else// HORIZONTAL_OFFSET
