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
    typedef int64_t (*maxpool_t)(float* data, int64_t H, int64_t W, int64_t C, float* result, int64_t H0, int64_t W0, dwc_algs_limits* algsLimits);
    maxpool_t generate(int kh_, int kw_, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type, float alpha);
    dwc_algs_limits calc_maxpool_algs_limits(int C, int W, int H, int kw, int kh, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right);
private:
    bool m_done; 
    Context CTX;
    enum { MULTI_H = 3 };
    enum { PADHOR = 1, PADVER = 2, INITDEST = 4, MULTILINE = 8, PREINCREMENT_IDXS = 16};
    void loadVector(const IReg& base, int64_t offset, VReg<float>& dest, VReg<int32_t>& horIdxs, const VReg<uint32_t>& verMask, const VReg<uint32_t>& WcondV, int flags);
    void multilineInit(const VReg<uint32_t>& HcondV, const VReg<uint32_t>& WcondV, IReg& yi, IReg& x, IReg& base, int& horVecsPerOut, std::vector<std::vector<VReg<float>>>& vertMaxes, const IReg& stride, int& kh, int flags);
    void multilineInitLast(const VReg<uint32_t>& HcondV, const VReg<uint32_t>& WcondV, IReg& yi, IReg& x, IReg& base, int& horVecsPerOut, std::vector<std::vector<VReg<float>>>& vertMaxes, const IReg& stride, int& kh, int flags);
    void onelineInit(const VReg<uint32_t>& WcondV, IReg& yi, IReg& x, IReg& base, int& horVecsPerOut, std::vector<loops::VReg<float>>& vertMaxes, const IReg& stride, int& kh, int flags);
    void onelineInitLast(const VReg<uint32_t>& WcondV, IReg& yi, IReg& x, IReg& base, int& horVecsPerOut, std::vector<loops::VReg<float>>& vertMaxes, const IReg& stride, int& kh, int flags);

    //Common parameters and registers
    int kh, kw, elemsize, elemshift, padding_top, padding_left, padding_bottom, padding_right, activation_type, stride_y, stride_x;
    IReg H, W, W0;
    float alpha;
    VReg<int32_t> countingPattern, idx_step;
    VReg<float> valpha;
    VReg<float> activationFunction(VReg<float>& res);
    IReg DUBUGresult;
    IReg DUBUGchannel;
    IReg DUBUGy;

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

MaxpoolGenerator::maxpool_t MaxpoolGenerator::generate(int kh_, int kw_, int padding_top_, int padding_left_, int padding_bottom_, int padding_right_, int activation_type_, float alpha_)
{
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
    std::string funcname = "max_pooling_kH";
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
        return (maxpool_t)(CTX.getFunc(funcname).ptr());
    }

    IReg data, NC, result, H0, algsLimits;
    USE_CONTEXT_(CTX);
    STARTFUNC_(funcname, &data, &H, &W, &NC, &result, &H0, &W0, &algsLimits)
    {
        if(activation_type == ACT_LRELU)
        {
            valpha.copyidx(VCONST_(float, alpha));
        }
        if(padhor)
        {
            countingPattern.copyidx(VCONST_(int32_t, 0));
            for(int lane = 1; lane < CTX.vlanes<float>(); lane++)
                setlane(countingPattern, lane, CONST_(lane));
            idx_step.copyidx(VCONST_(int32_t, CTX.vlanes<float>()));
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
        int horVecsPerOut = (kw + 3)/4 + ((kw + 3)%4?1:0);

        IReg offset = CONST_(0);
        IReg channel = CONST_(0);
        IReg stride = W << elemshift;
        
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
                        IReg xi = CONST_(-padding_left);
                        IReg Hcond = padver ? max(H - (kh + MULTI_H - 2), CONST_(0)) : IReg(); 
                        IReg Wcond = padhor ? max(W - (CTX.vlanes<float>()*horVecsPerOut - 1), CONST_(0)) : IReg();
                        VReg<uint32_t> WcondV = padhor ? broadcast<uint32_t>(W) : VReg<uint32_t>();
                        VReg<uint32_t> HcondV = padver ? broadcast<uint32_t>(H) : VReg<uint32_t>();
                        IReg multilineendx;
                        multilineendx.copyidx(padding_left ? W0-padding_left : W0);
                        IReg hldx = W0 - (CTX.vlanes<float>()+padding_left);

                        std::vector<std::vector<VReg<float> > > vertMaxes(MULTI_H, std::vector<VReg<float>>(horVecsPerOut, VReg<float>()));
                        vertMaxes.resize(MULTI_H, std::vector<VReg<float>>(horVecsPerOut, VReg<float>()));
                        for(int n = 0; n < MULTI_H; n++)
                        {
                            for(int vRegNum = 0; vRegNum < horVecsPerOut; vRegNum++)
                                vertMaxes[n][vRegNum].copyidx(VDEF_(float));
                        }
                        if(padhor||padver)
                        {
                            IReg yi = padver ? y - padding_top : IReg();
                            IReg xcond = padver&&padhor ? select(ult(yi,Hcond), xi, Wcond) : xi;
                            
                            IF_(padhor?(ult(xcond, Wcond)):ult(yi, Hcond))
                            {
                                multilineInit(HcondV, WcondV, yi, xi, data_rs, horVecsPerOut, vertMaxes, stride, kh, 0);
                            }
                            ELSE_
                            {
                                multilineInit(HcondV, WcondV, yi, xi, data_rs, horVecsPerOut, vertMaxes, stride, kh, handlerFlags);
                            }
                        }
                        else
                        {
                            multilineInit(HcondV, WcondV, y, xi, data_rs, horVecsPerOut, vertMaxes, stride, kh, 0);
                        }
                        
                        WHILE_(xi < multilineendx)
                        {
                            IF_(xi > hldx)
                            {
                                xi = hldx;
                                if(padhor||padver)
                                {
                                    IReg yi = padver ? y - padding_top : IReg();
                                    IReg xcond = padver&&padhor ? select(ult(yi,Hcond), xi, Wcond) : xi;
                                    IF_(padhor?(ult(xcond, Wcond)):ult(yi, Hcond))
                                    {
                                        multilineInit(HcondV, WcondV, yi, xi, data_rs, horVecsPerOut, vertMaxes, stride, kh, 0);
                                    }
                                    ELSE_
                                    {
                                        multilineInit(HcondV, WcondV, yi, xi, data_rs, horVecsPerOut, vertMaxes, stride, kh, handlerFlags);
                                    }
                                }
                                else
                                {
                                    multilineInit(HcondV, WcondV, y, xi, data_rs, horVecsPerOut, vertMaxes, stride, kh, 0);
                                }
                            }

DUBUGresult.copyidx(result);
DUBUGchannel.copyidx(channel);
DUBUGy.copyidx(y);
                            
                            if(padhor||padver)
                            {
                                IReg yi = padver ? y - padding_top : IReg();
                                IReg xcond = padver&&padhor ? select(ult(yi,Hcond), xi, Wcond) : xi;
                                IF_(padhor?(ult(xcond, Wcond)):ult(yi, Hcond))
                                {
                                    
                                    multilineInitLast(HcondV, WcondV, yi, xi, data_rs, horVecsPerOut, vertMaxes, stride, kh, 0);
                                }
                                ELSE_
                                {
                                    multilineInitLast(HcondV, WcondV, yi, xi, data_rs, horVecsPerOut, vertMaxes, stride, kh, handlerFlags);
                                }
                            }
                            else
                            {
                                multilineInitLast(HcondV, WcondV, y, xi, data_rs, horVecsPerOut, vertMaxes, stride, kh, 0);
                            }

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
                            IReg roffset = (PADHOR & handlerFlags ? xi + padding_left : xi) << elemshift;
                            int outdiffW = padding_left + padding_right - kw + 1;
                            IReg W0_;
                            W0_.copyidx(outdiffW ? (outdiffW > 0 ? W+outdiffW: W-(-outdiffW)): W); //TODO(ch): fix it in interface of loops(there must be ability to add and sub negaitve numbers).
                            IReg offstride = W0_ << elemshift;
                            for(int lineNum = 0; lineNum < MULTI_H; lineNum++)
                            {
                                storevec<float>(result_rs, roffset, activationFunction(res[lineNum]));
                                if(lineNum + 1 < MULTI_H)
                                    roffset += offstride;
                            }
                            xi += CTX.vlanes<float>();
                        }
                        y += MULTI_H;
                    }
                    yonelineEnd = H0;
                }
                
                WHILE_(y < yonelineEnd)
                {
                    IReg xis = select(y < yis, Xis, 0);
                    IReg xie = select(y >= yie, Xie, 0);
                    if(padding_left) { xis-=padding_left; xie-=padding_left;}
                    IReg xi = CONST_(-padding_left);
                    IReg W0mpl;
                    W0mpl.copyidx(padding_left? W0 - padding_left: W0);
                    IReg scalarEnd = select(xis > xi, xis, W0mpl);

                    IReg data_rs = data + ((W * (padver ? y - padding_top: y)) << elemshift);
                    IReg result_rs = result + (W0 << elemshift) * y;

                    int horVecsPerOut = (kw + 3)/4 + ((kw + 3)%4?1:0);
                    std::vector<VReg<float> > vertMaxes(horVecsPerOut, VReg<float>());
                    
                    for(int vRegNum = 0; vRegNum < horVecsPerOut; vRegNum++)
                        vertMaxes[vRegNum].copyidx(VDEF_(float));

                    WHILE_(xi < W0mpl)
                    {
                        IF_(xi == xis)
                        {
                            IReg Wcond = padhor ? max(W - (CTX.vlanes<float>()*horVecsPerOut - 1), CONST_(0)) : IReg();
                            IReg Hcond = padver ? max(H - (kh - 1), CONST_(0)) : IReg();
                            VReg<uint32_t> WcondV = padhor ? broadcast<uint32_t>(W) : VReg<uint32_t>();
                            if(padhor||padver)
                            {
                                IReg yi = padver ? y - padding_top : IReg();
                                IReg xcond;
                                xcond.copyidx(padhor && padver ? select(ult(yi, Hcond), xi, Wcond): xi);
                                IF_(padhor?ult(xcond,Wcond):ult(yi, Hcond))
                                {
                                    onelineInit(WcondV, yi, xi, data_rs, horVecsPerOut, vertMaxes, stride, kh, 0);
                                }
                                ELSE_
                                {
                                    onelineInit(WcondV, yi, xi, data_rs, horVecsPerOut, vertMaxes, stride, kh, handlerFlags);
                                }
                            }
                            else 
                                onelineInit(WcondV, y, xi, data_rs, horVecsPerOut, vertMaxes, stride, kh, 0);
                            
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
                                        onelineInitLast(WcondV, yi, xi, data__, horVecsPerOut, vertMaxes, stride, kh, 0);
                                    }
                                    ELSE_
                                    {
                                        onelineInitLast(WcondV, yi, xi, data__, horVecsPerOut, vertMaxes, stride, kh, handlerFlags);
                                    }
                                }
                                else 
                                    onelineInitLast(WcondV, y, xi, data__, horVecsPerOut, vertMaxes, stride, kh, 0);

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

                                IReg roffset = (padhor ? xi + padding_left : xi) << elemshift;
                                storevec<float>(result_rs, roffset, activationFunction(res));
                                xi += CTX.vlanes<float>();
                            }
                            scalarEnd = W0mpl;
                        }
                        
                        WHILE_(xi < scalarEnd)
                        {
                            VReg<float> vres = VDEF_(float);
                            IReg res_already_init = CONST_(0);
                            IReg data__ = data_rs + (xi << elemshift);
                            IReg kcol = CONST_(0);
                            IReg krow = CONST_(0);
                            IReg istride = (W - (kw-1)) << elemshift;
                            
                            WHILE_(krow < kh)
                            {
                                if(padhor||padver)
                                {
                                    VReg<float> justloaded = VCONST_(float, 0);
                                    IReg ex = xi + kcol;
                                    IReg ey = (padver ? y - padding_top : y) + krow;
                                    select(ex < 0, W, ex);
                                    select(ey < 0, H, ey);
                                    IF_(ult(ex,W))
                                    {
                                        IF_(ult(ey,H))
                                        {
                                            justloaded = broadcast<float>(load_<float>(data__));
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
                                    VReg<float> justloaded = broadcast<float>(load_<float>(data__));
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
                            IReg roffset = (((padhor||padver) ? xi + padding_left : xi) << elemshift);
                            store_<float>(result_rs + roffset, getlane<float>(activationFunction(vres), 0));
                            xi += 1;
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
    return (maxpool_t)(CTX.getFunc(funcname).ptr());
}

void MaxpoolGenerator::multilineInit(const VReg<uint32_t>& HcondV, const VReg<uint32_t>& WcondV, IReg& yi_, IReg& x, IReg& base, int& horVecsPerOut, std::vector<std::vector<VReg<float>>>& vertMaxes, const IReg& stride, int& kh, int flags)
{
    USE_CONTEXT_(CTX);
    int lvflags = flags&(PADHOR|PADVER);
    IReg vertMaxesPtr = base + (x << elemshift);
    IReg yi = yi_;
    for(int lrow = 0; lrow < (MULTI_H - 1) * stride_y + kh; lrow++)
    {
        VReg<int32_t> horIdxs = (flags&PADHOR) ? broadcast<int32_t>(x) + countingPattern : VReg<int32_t>();
        VReg<uint32_t> vertMask = (flags&PADVER) ? broadcast<uint32_t>(yi) < HcondV : VReg<uint32_t>();
        if((flags&PADVER)&&(flags&PADHOR))
        {
            VReg<int32_t> antiSpill = horIdxs; //TODO(ch): remove it when snippet management will be better.
            horIdxs = select(vertMask, antiSpill, reinterpret<int32_t>(WcondV));
        }
        for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
        {
            VReg<float> loadedVector;
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
            vertMaxesPtr += stride;
            if(flags&PADVER)
                yi+=1;
        }
    }
}

void MaxpoolGenerator::multilineInitLast(const VReg<uint32_t>& HcondV, const VReg<uint32_t>& WcondV, IReg& yi_, IReg& x, IReg& base, int& horVecsPerOut, std::vector<std::vector<VReg<float>>>& vertMaxes, const IReg& stride, int& kh, int flags)
{
    USE_CONTEXT_(CTX);
    int lvflags = flags&(PADHOR|PADVER);
    IReg vertMaxesPtr = base + (x << elemshift);
    IReg yi = yi_;
    int vRegNum = horVecsPerOut-1;
    VReg<int32_t> horIdxs_ = (flags&PADHOR) ? broadcast<int32_t>(x+vRegNum*CTX.vlanes<float>()) + countingPattern : VReg<int32_t>();
    for(int lrow = 0; lrow < (MULTI_H - 1) * stride_y + kh; lrow++)
    {
        VReg<uint32_t> vertMask = (flags&PADVER) ? broadcast<uint32_t>(yi) < HcondV : VReg<uint32_t>();
        VReg<int32_t> horIdxs;
        horIdxs.copyidx((flags&PADVER)&&(flags&PADHOR)? select(vertMask, horIdxs_, reinterpret<int32_t>(WcondV)): horIdxs_);
        VReg<float> loadedVector;
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
            vertMaxesPtr += stride;
            if(flags&PADVER)
                yi+=1;
        }
    }
}

void MaxpoolGenerator::onelineInit(const VReg<uint32_t>& WcondV, IReg& yi_, IReg& x, IReg& base, int& horVecsPerOut, std::vector<loops::VReg<float>>& vertMaxes, const IReg& stride, int& kh, int flags)
{
    USE_CONTEXT_(CTX);
    int lvflags = flags&PADHOR;
    IReg krow = CONST_(0);
    VReg<uint32_t> dummy;
    IReg vertMaxesPtr = base + (x << elemshift);
    IReg yi = yi_;
    WHILE_(krow<kh)
    {
        VReg<int32_t> horIdxs = flags&PADHOR ? broadcast<int32_t>(x) + countingPattern : VReg<int32_t>();
        for(int vRegNum = 0; vRegNum < horVecsPerOut-1; vRegNum++)
        {
            VReg<float> loadedVector;
            if(flags&PADVER)
            {
                loadedVector.copyidx(VCONST_(float, 0));
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
        vertMaxesPtr += stride;
        if (flags&PADVER)
            yi += 1;
        krow += 1;
    }
}

void MaxpoolGenerator::onelineInitLast(const VReg<uint32_t>& WcondV, IReg& yi_, IReg& x, IReg& base, int& horVecsPerOut, std::vector<loops::VReg<float>>& vertMaxes, const IReg& stride, int& kh, int flags)
{
    USE_CONTEXT_(CTX);
    IReg yi = yi_;
    int lvflags = flags&PADHOR;
    VReg<uint32_t> dummy;
    int lastVRegNum = horVecsPerOut-1;
    IReg vertMaxesPtr = base;
    IReg krow = CONST_(0);
    VReg<int32_t> horIdxs = flags&PADHOR ? broadcast<int32_t>(x+(lastVRegNum*CTX.vlanes<float>())) + countingPattern : VReg<int32_t>();
    
    WHILE_(krow<kh)
    {
        VReg<float> loadedVector;
        if(flags&PADVER)
        {
            loadedVector.copyidx(VCONST_(float, 0));
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

        vertMaxesPtr += stride;
        if (flags&PADVER)
            yi += 1;
        krow += 1;
    }
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

dwc_algs_limits MaxpoolGenerator::calc_maxpool_algs_limits(int NC, int W, int H, int kw, int kh, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right)
{
    int Cms, Cme;
    int lsimd = upMultipleOf(kw + CTX.vlanes<float>() - 2, CTX.vlanes<float>()) - 1;
    int XlastMulti = (W0 - padding_left - CTX.vlanes<float>()) + lsimd;
    if(W0 > CTX.vlanes<float>())
    {
        Cms = downC(NC, H, W, -padding_top, -padding_left);
        int YlastMulti = (downMultipleOf(W0, MULTI_H) - padding_top + kh + MULTI_H - 2);
        Cme = upperC(NC, H, W, YlastMulti, XlastMulti);
    }
    else
    {
        Cms = NC + 1;
        Cme = NC;
    }
    int Xlast = downMultipleOf(W0-1, CTX.vlanes<float>()) + lsimd - padding_left;
    int Cis = downC(NC, H, W, 0, -padding_left);
    int Cie = upperC(NC, H, W, (H-1), Xlast);
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
    }
    int Xis = W0, Xie = W0;
    if(Yis < H0)
    {
        Xis = downX(NC, H, W, Cis-1, Yis - 1 - padding_top, -padding_left);
        Xie = upperX(NC, H, W, Cie, std::min(Yie - padding_top + kh - 1, H-1), lsimd-padding_left, CTX.vlanes<float>(), ((Cis - 1) == Cie && Yis == Yie)? Xis: 0);
    }
    return dwc_algs_limits(Cms, Cme, Cis, Cie, Yms, Yme, Yis, Yie, Xis, Xie);
}
};
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
#endif //__LOOPS_MAXPOOL_HPP__
