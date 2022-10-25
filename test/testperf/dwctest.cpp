/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "dwctest.hpp"
#if __LOOPS_ARCH == __LOOPS_AARCH64

#include "loops/loops.hpp"
#include "loopslayers/loopslayers.h"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <vector>
#include <iostream>
#include <iomanip>
#include "arm_neon.h"
#include "tests.hpp"

namespace loops
{
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
        {{3, 3, 3, 10, 3, 0, 0, 0, 0}, {4, 3, 0, 2, 9, 8, 0, 6, 0, 0}},
        {{3, 3, 3, 10, 10, 0, 0, 0, 0}, {0, 2, 0, 2, 0, 6, 0, 7, 0, 4}},
        {{3, 3, 3, 10, 10, 1, 0, 1, 0}, {1, 2, 0, 2, 1, 6, 0, 8, 0, 4}},
        {{3, 3, 3, 10, 10, 0, 1, 0, 1}, {1, 2, 1, 2, 1, 6, 1, 7, 1, 4}},
        {{3, 3, 3, 10, 10, 1, 1, 1, 1}, {1, 2, 1, 2, 2, 6, 2, 8, 1, 4}},
        {{3, 3, 1, 10, 10, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 6, 0, 7, 0, 4}},
        {{3, 3, 1, 10, 10, 1, 0, 1, 0}, {1, 0, 0, 0, 1, 7, 0, 8, 0, 4}},
        {{3, 3, 1, 10, 10, 0, 1, 0, 1}, {1, 0, 1, 0, 1, 7, 1, 7, 1, 4}},
        {{3, 3, 1, 10, 10, 1, 1, 1, 1}, {1, 0, 1, 0, 2, 8, 2,  8, 1, 4}},
        {{3, 3, 1, 1, 1, 1, 1, 1, 1},   {2, 1, 2, 1, 2, 1, 2, 1, 1, 1}},
        {{3, 3, 100, 1, 1, 1, 1, 1, 1}, {101, 100, 1, 94, 2, 1, 2, 0, 1, 1}}
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
        const int H0 = H-kh+1+padding_top+padding_bottom;
        const int W0 = W-kw+1+padding_left+padding_right;

        (*out) << "Depthwise convolution "<<kh<<"x"<<kw<<", C = "<< C << ", H = "<< H << ", W = "<< W << ", pt = "<< padding_top << ", pl = "<< padding_left << ", pb = "<< padding_bottom << ", pr = "<< padding_right << std::endl;
        dwc_algs_limits tocheck;
        calc_dwc_algs_limits_f32(&tocheck, C, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right);
        if(!compare_alg_limits(tocheck, ref))
            return;
    }
    enum {PERF, REGRESS};
    enum {SMALL_ALPHA, BIG_ALPHA};
    std::vector<std::vector<int> > fixtures = {
        {3, 3, 3, 10, 10, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 1, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 0, 1, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 0, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 0, 0, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 1, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 0, 1, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10, 10, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 1, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 0, 1, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 0, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 0, 0, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 1, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 0, 1, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3,  9,  9, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {3, 3, 3, 10,  3, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},

        {5, 5, 256,  40,  40, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {7, 7, 256,  40,  40, 3, 3, 3, 3, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {9, 9, 256,  40,  40, 5, 5, 5, 5, ACT_NONE, SMALL_ALPHA, REGRESS},
        {5, 5, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {7, 7, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {9, 9, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {5, 5, 256,  39,  39, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {7, 7, 256,  39,  39, 3, 3, 3, 3, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_NONE, SMALL_ALPHA, REGRESS},
        {5, 5, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {7, 7, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS}, 
        {9, 9, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},

        {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_RELU, SMALL_ALPHA, REGRESS},
        {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_RELU6, SMALL_ALPHA, REGRESS},
        {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_LRELU, SMALL_ALPHA, REGRESS},
        {9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_LRELU, BIG_ALPHA, REGRESS},

        {5, 5, 1632, 7, 7, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, PERF},
        {3, 3, 32, 112, 112, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, PERF},
        {3, 3, 192, 56, 56, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, PERF},

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
        const int activation = fxt[9];
        const float alpha = fxt[10] == BIG_ALPHA ? 1.25 : 0.25;
        bool perf = (fxt[11] == PERF);
        const int H0 = H-kh+1+padding_top+padding_bottom;
        const int W0 = W-kw+1+padding_left+padding_right;

        const float empty_value = kh * kw * 2000 + 1;
        dwc_algs_limits algsLimits;
        calc_dwc_algs_limits_f32(&algsLimits, C, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right);

        dwconv_f32_t func = generate_dwc_f32(kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation, alpha);
        std::vector<float> indata(W*H*C);
        std::vector<float> kernel(kw*kh*C, 0);
        std::vector<float> bias(C, 0);        
        std::vector<float> outdata(H0*W0*C * 3, empty_value);
        std::vector<float> outdataref(H0*W0*C, 0);
        float* inptr = &(indata[0]);
        float* kptr = &(kernel[0]);
        float* bptr = &(bias[0]);
        float* optr = &(outdata[0]) + H0*W0*C;
        float* optrref = &(outdataref[0]);
        gendata(inptr, kptr, bptr,kh,kw, H, W, C);
        (*out) << "Depthwise convolution "<<kh<<"x"<<kw<<", C = "<< C << ", H = "<< H << ", W = "<< W << ", pt = "<< padding_top << ", pl = "<< padding_left << ", pb = "<< padding_bottom << ", pr = "<< padding_right << std::endl;
        ref(inptr, kptr, bptr, H, W, C, optrref, H0, W0, alpha, kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation);
        if(perf)
        {
            Timer t;
            int ret;
            for(int testiter = 0; testiter < TESTITERATIONS; testiter++)
            {
                t.start();
                ret = func(inptr, kptr, bptr, H, W, C, optr, H0, W0, &algsLimits);
                t.stop();
            }
            if(compare(&(outdata[0]), optrref, C, H0, W0, empty_value))
                (*out)<<"    Optimized time = "<<t.str()<<std::endl;
            else
                return;
        }
        else
        {
            int ret;
            ret = func(inptr, kptr, bptr, H, W, C, optr, H0, W0, &algsLimits);
            if(!compare(&(outdata[0]), optrref, C, H0, W0, empty_value))
            {
                (*out)<<"    FAILED!"<<std::endl;
                return;
            }
        }
    }
}
                                          
void DepthwiseconvTest::gendata(float* data, float* kernel, float* bias, int kh, int kw, int H, int W, int C)
{
    for (int i = 0 ; i < C*H*W ; i++)
        data[i] = (rand() % 10000)/5000.0f - 1;
    for (int i = 0 ; i < C*kw*kh ; i++)
        kernel[i] = (rand() % 10000)/2500.0f - 2;
    for (int i = 0 ; i < C ; i++)
        bias[i] = (rand() % 10000)/2500.0f - 2;
}

void DepthwiseconvTest::ref(float* data, float* kernel, float* bias, int H, int W, int C, float* result, int H0, int W0, float alpha, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type)
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
                switch(activation_type)
                {
                    case(ACT_NONE): break;
                    case(ACT_RELU): res = vmaxq_f32(res, vdupq_n_f32(0)); break;
                    case(ACT_RELU6): res = vmaxq_f32(vminq_f32(res, vdupq_n_f32(6)), vdupq_n_f32(0)); break;
                    case(ACT_LRELU):
                        res = alpha == 1 ? res :
                              alpha <  1 ? vmaxq_f32(vmulq_f32(res, vdupq_n_f32(alpha)), res) : 
                                           vminq_f32(vmulq_f32(res, vdupq_n_f32(alpha)), res);
                    break;
                    defaout: throw std::runtime_error("Unknown activation");
                };                
                float res_ = vgetq_lane_f32(res, 0);
                // res_ = (res_ > 0 ? res_ : (res_ * alpha));
                *resC = res_;
            }
        }
    }
}

bool DepthwiseconvTest::compare(float* tocheck, float* ref, int C, int H, int W, float empty_value)
{
    for(int k = 0; k < C; k++)
        for(int i = 0; i < H; i++)
            for(int j = 0; j < W; j++)
                if(tocheck[(k * H + i) * W + j] != empty_value) 
                {
                    (*out)<<"    Memory writing violation at output ["<< -(C - k) <<", "<< i <<", "<< j<<"]"<<std::endl;
                    return false;
                }
    for(int k = 0; k < C; k++)
        for(int i = 0; i < H; i++)
            for(int j = 0; j < W; j++)
                if(tocheck[2*C*W*H+(k * H + i) * W + j] != empty_value) 
                {
                    (*out)<<"    Memory writing violation at output ["<< C + k <<", "<< i <<", "<< j<<"]"<<std::endl;
                    return false;
                }
    tocheck += C*H*W;
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