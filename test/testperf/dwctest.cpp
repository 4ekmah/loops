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

class DepthwiseconvTestImpl : public DepthwiseconvTest
{
public:
    DepthwiseconvTestImpl(Context aCTX, std::ostream* a_out): DepthwiseconvTest(), CTX(aCTX), out(a_out) {}
    virtual ~DepthwiseconvTestImpl() {};
    virtual void run();
private:
    template<typename _Tp>
    void gendata(_Tp* data, _Tp* kernel, _Tp* bias, int kh, int kw, int H, int W, int C);
    template<typename _Tp>
    void ref(_Tp* data, _Tp* kernel, _Tp* bias, int H, int W, int C, _Tp* result, int H0, int W0, float alpha, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type);
    template<typename _Tp>
    bool compare(_Tp* tocheck, _Tp* ref, int C, int H, int W, _Tp empty_value);
    bool compare_alg_limits(const dwc_algs_limits& tocheck, const dwc_algs_limits& reference);
    void print_alg_limits(const dwc_algs_limits& tocheck);
    template<typename _Tp>
    bool handleFixture(const std::vector<int>& fxt);
    enum {PERF, REGRESS};
    enum {SMALL_ALPHA, BIG_ALPHA};
    Context CTX;
    std::ostream* out;
}; 

template<typename _Tp> struct DWCTestTraits {};

template<> struct DWCTestTraits<float> {
    typedef float32x4_t neon_ftype;
    typedef float ftype;
    typedef dwconv_f32_t dwconv_t;
    static inline neon_ftype dup(ftype td) { return vdupq_n_f32(td); } 
    static inline ftype vget0(const neon_ftype& tg) { return vgetq_lane_f32(tg, 0); } 
    static inline neon_ftype max(const neon_ftype& a, const neon_ftype& b) { return vmaxq_f32(a,b); } 
    static inline neon_ftype min(const neon_ftype& a, const neon_ftype& b) { return vminq_f32(a,b); }
    static inline neon_ftype mul(const neon_ftype& a, const neon_ftype& b) { return vmulq_f32(a,b); }
    static inline neon_ftype fma(const neon_ftype& a, const neon_ftype& m1, const neon_ftype& m2) { return vfmaq_f32(a,m1,m2); }
    static inline bool cmp_ne(ftype a, ftype b) { return (a!=b); }
    static inline float tflt(ftype a) { return a; }
    static inline void calc_dwc_algs_limits(struct dwc_algs_limits* out, int C, int W, int H, int kw, int kh, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right)
    { calc_dwc_algs_limits_f32(out, C, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right);}
    static inline dwconv_t generate_dwc(int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type, float alpha)
    { return generate_dwc_f32(kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation_type, alpha); }
};

inline __fp16 f16_t2armf16(f16_t tc)
{
    return *(reinterpret_cast<__fp16*>(&(tc.bits)));
}

template<> struct DWCTestTraits<f16_t> {
    typedef float16x8_t neon_ftype;
    typedef f16_t ftype;
    typedef dwconv_f16_t dwconv_t;
    static inline neon_ftype dup(ftype td) { return vdupq_n_f16(f16_t2armf16(td)); } 
    static inline ftype vget0(const neon_ftype& tg) { return ftype((float)vgetq_lane_f16(tg, 0)); } 
    static inline neon_ftype max(const neon_ftype& a, const neon_ftype& b) { return vmaxq_f16(a,b); } 
    static inline neon_ftype min(const neon_ftype& a, const neon_ftype& b) { return vminq_f16(a,b); }
    static inline neon_ftype mul(const neon_ftype& a, const neon_ftype& b) { return vmulq_f16(a,b); }
    static inline neon_ftype fma(const neon_ftype& a, const neon_ftype& m1, const neon_ftype& m2) { return vfmaq_f16(a,m1,m2); }
    static inline bool cmp_ne(ftype a, ftype b) { return (f16_t2armf16(a)!=f16_t2armf16(b)); }
    static inline float tflt(ftype a) { return f16_t2armf16(a); }
    static inline void calc_dwc_algs_limits(struct dwc_algs_limits* out, int C, int W, int H, int kw, int kh, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right)
    { calc_dwc_algs_limits_f16(out, C, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right);}
    static inline dwconv_t generate_dwc(int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type, float alpha)
    { return generate_dwc_f16(kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation_type, alpha); }
};

template<class T>
void printData(T* data, int H, int W) { //TODO(ch): move it to test.hpp 
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
            std::cout << std::setw(6) << DWCTestTraits<T>::tflt(data[i*W+j]);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template<class T>
void printData(T* data, int H, int W, int stride) { //TODO(ch): move it to test.hpp 
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
            std::cout << std::setw(15) << DWCTestTraits<T>::tflt(data[i*stride+j]);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

DepthwiseconvTest::DepthwiseconvTest(Context aCTX, std::ostream* a_out)
{ impl = new DepthwiseconvTestImpl(aCTX, a_out); }

DepthwiseconvTest::~DepthwiseconvTest()
{ delete impl; }

void DepthwiseconvTest::run()
{ impl->run(); }

template<typename _Tp>
void DepthwiseconvTestImpl::gendata(_Tp* data, _Tp* kernel, _Tp* bias, int kh, int kw, int H, int W, int C)
{
    for (int i = 0 ; i < C*H*W ; i++)
        data[i] = _Tp((rand() % 10000)/5000.0f - 1);
    for (int i = 0 ; i < C*kw*kh ; i++)
        kernel[i] = _Tp((rand() % 10000)/2500.0f - 2);
    for (int i = 0 ; i < C ; i++)
        bias[i] = _Tp((rand() % 10000)/2500.0f - 2);
//DUBUGGG
    // for (int i = 0 ; i < C*H*W ; i++)
    //     data[i] = _Tp((rand() % 200) - 100);
    // for (int i = 0 ; i < C*kw*kh ; i++)
    //     kernel[i] = _Tp(1);
    // for (int i = 0 ; i < C ; i++)
    //     bias[i] = _Tp(0);
}

template<typename _Tp>
void DepthwiseconvTestImpl::ref(_Tp* data, _Tp* kernel, _Tp* bias, int H, int W, int C, _Tp* result, int H0, int W0, float alpha, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type)
{
    std::vector<_Tp> padded;
    {
        const int WP = (W+padding_left+padding_right);
        const int HP = (H+padding_top+padding_bottom);
        padded.resize(WP*HP*C, _Tp(0));
        for(int ch = 0; ch < C; ch++)
        {
            _Tp* dest_ = &(padded[WP*HP*ch]);
            _Tp* data_ = &(data[W*H*ch]);
            for(int nprow = 0; nprow < H; nprow++)
            {
                _Tp* dest = dest_ + (nprow + padding_top) * WP + padding_left;
                memcpy(dest, data_ + W * nprow, W * sizeof(_Tp));
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
        _Tp* ker = kernel + c * kw * kh;
        _Tp* inL = data + c * W * H;
        _Tp* resL = result + c * W0 * H0;
        typename DWCTestTraits<_Tp>::neon_ftype b = DWCTestTraits<_Tp>::dup(bias[c]);
        for(int i = 0; i < H0; i++) 
        {
            _Tp* inR = inL + i * W;
            _Tp* resR = resL + i * W0;
            for(int j = 0; j < W0; j++)
            {
                _Tp* inC = inR + j;
                _Tp* resC = resR + j;
                typename DWCTestTraits<_Tp>::neon_ftype res = b;
                for(int k = 0; k < kh * kw; k++)
                {
                    int krow = k/kw;
                    int kcol = k - krow * kw;
                    typename DWCTestTraits<_Tp>::neon_ftype toAdd = DWCTestTraits<_Tp>::dup(inC[koffsets[k]]);
                    typename DWCTestTraits<_Tp>::neon_ftype weight = DWCTestTraits<_Tp>::dup(ker[k]);
                    res = DWCTestTraits<_Tp>::fma(res, toAdd, weight);
                }
                switch(activation_type)
                {
                    case(ACT_NONE): break;
                    case(ACT_RELU): res = DWCTestTraits<_Tp>::max(res, DWCTestTraits<_Tp>::dup(_Tp(0))); break;
                    case(ACT_RELU6): res = DWCTestTraits<_Tp>::max(DWCTestTraits<_Tp>::min(res, DWCTestTraits<_Tp>::dup(_Tp(6))), DWCTestTraits<_Tp>::dup(_Tp(0))); break;
                    case(ACT_LRELU):
                        res = alpha == 1 ? res :
                              alpha <  1 ? DWCTestTraits<_Tp>::max(DWCTestTraits<_Tp>::mul(res, DWCTestTraits<_Tp>::dup(_Tp(alpha))), res) : 
                                           DWCTestTraits<_Tp>::min(DWCTestTraits<_Tp>::mul(res, DWCTestTraits<_Tp>::dup(_Tp(alpha))), res);
                    break;
                    default: throw std::runtime_error("Unknown activation");
                };                
                _Tp res_ = DWCTestTraits<_Tp>::vget0(res);
                *resC = res_;
            }
        }
    }
}

template<typename _Tp>
bool DepthwiseconvTestImpl::compare(_Tp* tocheck, _Tp* ref, int C, int H, int W, _Tp empty_value)
{
    for(int k = 0; k < C; k++)
        for(int i = 0; i < H; i++)
            for(int j = 0; j < W; j++)
                if(DWCTestTraits<_Tp>::cmp_ne(tocheck[(k * H + i) * W + j], empty_value)) 
                {
                    (*out)<<"    Memory writing violation at output ["<< -(C - k) <<", "<< i <<", "<< j<<"]"<<std::endl;
                    return false;
                }
    for(int k = 0; k < C; k++)
        for(int i = 0; i < H; i++)
            for(int j = 0; j < W; j++)
                if(DWCTestTraits<_Tp>::cmp_ne(tocheck[2*C*W*H+(k * H + i) * W + j], empty_value)) 
                {
                    (*out)<<"    Memory writing violation at output ["<< C + k <<", "<< i <<", "<< j<<"]"<<std::endl;
                    return false;
                }
    tocheck += C*H*W;
    _Tp maxdiff(0);
    for(int k = 0; k < C; k++)
        for(int i = 0; i < H; i++)
            for(int j = 0; j < W; j++)
            {
                _Tp tchk = tocheck[(k * H + i) * W + j];
                _Tp r = ref[(k * H + i) * W + j];
                if(DWCTestTraits<_Tp>::cmp_ne(tchk, r))
                {
                    float _tchk = DWCTestTraits<_Tp>::tflt(tchk);
                    float _r = DWCTestTraits<_Tp>::tflt(r);
                    float absErr = std::abs(_tchk - _r);
                    float relErr = absErr / std::abs(_r);
                    (*out)<<"    Result non-equal to reference at ["<< k <<", "<< i <<", "<< j<<"]"<<std::endl;
                    (*out)<<"        Result got:"<< _tchk <<std::endl;
                    (*out)<<"        Reference:"<< _r <<std::endl;
                    (*out)<<"        Absolute error:"<< absErr <<std::endl;
                    (*out)<<"        Relative error:"<< relErr <<std::endl;
                    return false;
                }
            }
    return true;
}

bool DepthwiseconvTestImpl::compare_alg_limits(const dwc_algs_limits& tocheck, const dwc_algs_limits& reference)
{
    bool res = true;
    if(tocheck.Cms != reference.Cms) {(*out)<<"    Cms:ref = " << reference.Cms << " | checked =  " << tocheck.Cms<<std::endl; res = false;}
    if(tocheck.Cme != reference.Cme) {(*out)<<"    Cme:ref = " << reference.Cme << " | checked =  " << tocheck.Cme<<std::endl; res = false;}
    if(tocheck.Cis != reference.Cis) {(*out)<<"    Cis:ref = " << reference.Cis << " | checked =  " << tocheck.Cis<<std::endl; res = false;}
    if(tocheck.Cie != reference.Cie) {(*out)<<"    Cie:ref = " << reference.Cie << " | checked =  " << tocheck.Cie<<std::endl; res = false;}
    if(tocheck.Yms != reference.Yms) {(*out)<<"    Yms:ref = " << reference.Yms << " | checked =  " << tocheck.Yms<<std::endl; res = false;}
    if(tocheck.Yme != reference.Yme) {(*out)<<"    Yme:ref = " << reference.Yme << " | checked =  " << tocheck.Yme<<std::endl; res = false;}
    if(tocheck.Yis != reference.Yis) {(*out)<<"    Yis:ref = " << reference.Yis << " | checked =  " << tocheck.Yis<<std::endl; res = false;}
    if(tocheck.Yie != reference.Yie) {(*out)<<"    Yie:ref = " << reference.Yie << " | checked =  " << tocheck.Yie<<std::endl; res = false;}
    if(tocheck.Xis != reference.Xis) {(*out)<<"    Xis:ref = " << reference.Xis << " | checked =  " << tocheck.Xis<<std::endl; res = false;}
    if(tocheck.Xie != reference.Xie) {(*out)<<"    Xie:ref = " << reference.Xie << " | checked =  " << tocheck.Xie<<std::endl; res = false;}
    return res;
}

void DepthwiseconvTestImpl::print_alg_limits(const dwc_algs_limits& toprint)
{
    (*out)<<"    Cms: = " << toprint.Cms<<std::endl;
    (*out)<<"    Cme: = " << toprint.Cme<<std::endl;
    (*out)<<"    Cis: = " << toprint.Cis<<std::endl;
    (*out)<<"    Cie: = " << toprint.Cie<<std::endl;
    (*out)<<"    Yms: = " << toprint.Yms<<std::endl;
    (*out)<<"    Yme: = " << toprint.Yme<<std::endl;
    (*out)<<"    Yis: = " << toprint.Yis<<std::endl;
    (*out)<<"    Yie: = " << toprint.Yie<<std::endl;
    (*out)<<"    Xis: = " << toprint.Xis<<std::endl;
    (*out)<<"    Xie: = " << toprint.Xie<<std::endl;
}

template<typename _Tp>
bool DepthwiseconvTestImpl::handleFixture(const std::vector<int>& fxt)
{
    const int TESTITERATIONS = 30;
    int kh = fxt[1];
    int kw = fxt[2];
    const int C = fxt[3];
    const int H = fxt[4];
    const int W = fxt[5];
    const int padding_top = fxt[6];
    const int padding_left = fxt[7];
    const int padding_bottom = fxt[8];
    const int padding_right = fxt[9];
    const int activation = fxt[10];
    const float alpha = fxt[11] == BIG_ALPHA ? 1.25 : 0.25;
    bool perf = (fxt[12] == PERF);
    const int H0 = H-kh+1+padding_top+padding_bottom;
    const int W0 = W-kw+1+padding_left+padding_right;

    const _Tp empty_value(kh * kw * 2000 + 1);
    dwc_algs_limits algsLimits;
    DWCTestTraits<_Tp>::calc_dwc_algs_limits(&algsLimits, C, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right);

    typename DWCTestTraits<_Tp>::dwconv_t func = DWCTestTraits<_Tp>::generate_dwc(kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation, alpha);
    std::vector<_Tp> indata(W*H*C);
    std::vector<_Tp> kernel(kw*kh*C, _Tp(0));
    std::vector<_Tp> bias(C, _Tp(0));
    std::vector<_Tp> outdata(H0*W0*C * 3, empty_value);
    std::vector<_Tp> outdataref(H0*W0*C, _Tp(0));
    _Tp* inptr = &(indata[0]);
    _Tp* kptr = &(kernel[0]);
    _Tp* bptr = &(bias[0]);
    _Tp* optr = &(outdata[0]) + H0*W0*C;
    _Tp* optrref = &(outdataref[0]);
    gendata(inptr, kptr, bptr,kh,kw, H, W, C);
    (*out) << "Depthwise convolution "<<(fxt[0]==TYPE_FP16?"FP16 ":"FP32 ")<<kh<<"x"<<kw<<", C = "<< C << ", H = "<< H << ", W = "<< W << ", pt = "<< padding_top << ", pl = "<< padding_left << ", pb = "<< padding_bottom << ", pr = "<< padding_right << std::endl;
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
        {
            (*out)<<"    FAILED!"<<std::endl;
            return false;
        }
    }
    else
    {
        int ret;
        ret = func(inptr, kptr, bptr, H, W, C, optr, H0, W0, &algsLimits);
        // std::cout<<"DUBUGret = "<< ret << std::endl;
        if(!compare(&(outdata[0]), optrref, C, H0, W0, empty_value))
        {
            printData(inptr, H, W); //DUBUGGG
            printData(optrref, H0, W0);
            printData(optr, H0, W0);
            (*out)<<"    FAILED!"<<std::endl;
            return false;
        }
    }
    return true;
}

void DepthwiseconvTestImpl::run() 
{
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
    std::vector<std::vector<int> > fixtures = {
        // {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3, 10, 10, 1, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3, 10, 10, 0, 1, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3, 10, 10, 1, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3, 10, 10, 0, 1, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3, 10, 10, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3,  9,  9, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3,  9,  9, 1, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3,  9,  9, 0, 1, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3,  9,  9, 0, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3,  9,  9, 0, 0, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3,  9,  9, 1, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3,  9,  9, 0, 1, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3,  9,  9, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 3, 3, 3, 10,  3, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 5, 5, 256,  40,  40, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 7, 7, 256,  40,  40, 3, 3, 3, 3, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 9, 9, 256,  40,  40, 5, 5, 5, 5, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 5, 5, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 7, 7, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 9, 9, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 5, 5, 256,  39,  39, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 7, 7, 256,  39,  39, 3, 3, 3, 3, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 5, 5, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 7, 7, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 9, 9, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_RELU, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_RELU6, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_LRELU, SMALL_ALPHA, REGRESS},
        // {TYPE_FP16, 9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_LRELU, BIG_ALPHA, REGRESS},

        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 1, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 1, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3,  9,  9, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3,  9,  9, 1, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3,  9,  9, 0, 1, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3,  9,  9, 0, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3,  9,  9, 0, 0, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3,  9,  9, 1, 0, 1, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3,  9,  9, 0, 1, 0, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3,  9,  9, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10,  3, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 5, 5, 256,  40,  40, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 7, 7, 256,  40,  40, 3, 3, 3, 3, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 9, 9, 256,  40,  40, 5, 5, 5, 5, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 5, 5, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 7, 7, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 9, 9, 256,  40,  40, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 5, 5, 256,  39,  39, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 7, 7, 256,  39,  39, 3, 3, 3, 3, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 5, 5, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 7, 7, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 9, 9, 256,  39,  39, 0, 0, 0, 0, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_RELU, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_RELU6, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_LRELU, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 9, 9, 256,  39,  39, 5, 5, 5, 5, ACT_LRELU, BIG_ALPHA, REGRESS},

        {TYPE_FP16, 5, 5, 1632, 7, 7, 2, 2, 2, 2, ACT_RELU6, SMALL_ALPHA, PERF},
        {TYPE_FP16, 3, 3, 32, 112, 112, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, PERF},
        {TYPE_FP16, 3, 3, 192, 56, 56, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, PERF},
        {TYPE_FP32, 5, 5, 1632, 7, 7, 2, 2, 2, 2, ACT_RELU6, SMALL_ALPHA, PERF},
        {TYPE_FP32, 3, 3, 32, 112, 112, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, PERF},
        {TYPE_FP32, 3, 3, 192, 56, 56, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, PERF},
//      {kh,kw, C, H, W, padding_top, padding_left, padding_bottom, padding_right}
    };
    for(auto fxt: fixtures)
        if(fxt[0] == TYPE_FP32)
        {
            if(!handleFixture<float>(fxt))
                break;
        }
        else if(fxt[0] == TYPE_FP16)
        {
            if(!handleFixture<f16_t>(fxt))
                break;
        }
}
};
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64