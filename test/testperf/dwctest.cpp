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
#include <thread>
#include "arm_neon.h"
#include "tests.hpp"

namespace loops
{

class DepthwiseconvTestImpl : public DepthwiseconvTest
{
public:
    DepthwiseconvTestImpl(std::ostream* a_out): DepthwiseconvTest(), out(a_out)
    {
        CTX = (Context*)create_context();
    }
    virtual ~DepthwiseconvTestImpl() {free_context(CTX);};
    virtual void run();
private:
    template<typename _Tp>
    void gendata(_Tp* data, _Tp* kernel, _Tp* bias, int kh, int kw, int H, int W, int C);
    template<typename _Tp>
    void ref(_Tp* data, _Tp* kernel, _Tp* bias, int H, int W, int C, _Tp* result, int H0, int W0, float alpha, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x, int activation_type);
    template<typename _Tp>
    dwc_algs_limits ref_calc_algs_limits(int NC, int H, int W, int kh, int kw, int H0, int W0, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x, int dilation_y, int dilation_x);
    template<typename _Tp>
    bool compare(_Tp* tocheck, _Tp* ref, int C, int H, int W, _Tp empty_value);
    bool compare_algs_limits(const dwc_algs_limits& tocheck, const dwc_algs_limits& reference);
    void print_algs_limits(const dwc_algs_limits& tocheck);
    template<typename _Tp>
    bool handleFixture(const std::vector<int>& fxt);
    template<typename _Tp>
    bool handleFixtureMultithread(const std::vector<int>& fxt);
    enum {PERF, REGRESS};
    enum {SMALL_ALPHA, BIG_ALPHA};
    Context* CTX;
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
    static inline void calc_dwc_algs_limits(Context* CTX, struct dwc_algs_limits* out, int C, int W, int H, int kw, int kh, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x)
    { calc_dwc_algs_limits_f32(CTX, out, C, H, W, kh, kw, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, 1, 1);}
    static inline dwconv_t generate_dwc(Context* CTX, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x, int activation_type, float alpha)
    { return generate_dwc_f32(CTX, kh, kw, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, 1, 1, activation_type, alpha); }
    static inline int padded_ksize(int kh, int kw, Context& CTX)
    {
        int padded_ksize = kw*kh;
        padded_ksize = (padded_ksize + CTX.vlanes<ftype>() - 1) & -CTX.vlanes<ftype>();
        return padded_ksize;
    }
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
    static inline void calc_dwc_algs_limits(Context* CTX, struct dwc_algs_limits* out, int C, int W, int H, int kw, int kh, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x)
    { calc_dwc_algs_limits_f16(CTX, out, C, H, W, kh, kw, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, 1, 1);}
    static inline dwconv_t generate_dwc(Context* CTX, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x, int activation_type, float alpha)
    { return generate_dwc_f16(CTX, kh, kw, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, 1, 1, activation_type, alpha); }
    static inline int padded_ksize(int kh, int kw, Context& CTX)
    {
        int padded_ksize = kw*kh;
        padded_ksize = (padded_ksize + CTX.vlanes<ftype>() - 1) & -CTX.vlanes<ftype>();
        return padded_ksize;
    }
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

DepthwiseconvTest::DepthwiseconvTest(std::ostream* a_out)
{ impl = new DepthwiseconvTestImpl(a_out); }

DepthwiseconvTest::~DepthwiseconvTest()
{ delete impl; }

void DepthwiseconvTest::run()
{ impl->run(); }

template<typename _Tp>
void DepthwiseconvTestImpl::gendata(_Tp* data, _Tp* kernel, _Tp* bias, int kh, int kw, int H, int W, int C)
{
    for (int i = 0 ; i < C*H*W ; i++)
        data[i] = _Tp((rand() % 10000)/5000.0f - 1);
    for (int ch = 0 ; ch < C ; ch++)
        for (int i = 0 ; i < kw*kh ; i++)
            kernel[DWCTestTraits<_Tp>::padded_ksize(kh, kw, *CTX) * ch + i] = _Tp((rand() % 10000)/2500.0f - 2);
    for (int i = 0 ; i < C ; i++)
        bias[i] = _Tp((rand() % 10000)/2500.0f - 2);
}

template<typename _Tp>
void DepthwiseconvTestImpl::ref(_Tp* data, _Tp* kernel, _Tp* bias, int H, int W, int C, _Tp* result, int H0, int W0, float alpha, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x, int activation_type)
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
        _Tp* ker = kernel + c * DWCTestTraits<_Tp>::padded_ksize(kh, kw, *CTX);
        _Tp* inL = data + c * W * H;
        _Tp* resL = result + c * W0 * H0;
        typename DWCTestTraits<_Tp>::neon_ftype b = DWCTestTraits<_Tp>::dup(bias[c]);
        for(int i = 0; i < H0; i++) 
        {
            _Tp* inR = inL + i * W * stride_y;
            _Tp* resR = resL + i * W0;
            for(int j = 0; j < W0; j++)
            {
                _Tp* inC = inR + j * stride_x;
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
dwc_algs_limits DepthwiseconvTestImpl::ref_calc_algs_limits(int NC, int H, int W, int kh, int kw, int H0, int W0, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x, int dilation_y, int dilation_x)
{
    //TODO(ch): Just written, already need refactoring via introducing set of operations on tensor masks/maps and defining precise rules about
    //corner case values(e.g. Cms = NC+1, Cme = 0). Also, after all, I don't like maps of start and end conditions correctness. There must be only map of ability
    //of usage certain scheme at certain row or at certain cell. Now looks oversophisticated.
    const int MultiH = 3;
    const int lanes = (int)CTX->vlanes<_Tp>();

    //Last horizontal loaded element from current position for every simd scheme
    int lsimd = kw + lanes - 2;
    lsimd = ((lsimd%lanes ? 1 : 0) + lsimd/lanes)*lanes - 1;
    lsimd = stride_x == 1 ? lsimd: ((kw-1)/stride_x)*stride_x + lanes * stride_x - 1;
    int lastloadedM = 0;
    for(int col = 0; col < W0; col += lanes)
    {
        col = std::min(W0 - lanes, col); 
        int lastloaded_ = col * stride_x - padding_left + lsimd;
        lastloadedM = std::max(lastloaded_, lastloadedM);
    }
    std::vector<std::vector<bool> > map_good_start_M(NC, std::vector<bool>(H0, false ) );
    std::vector<std::vector<bool> > map_good_end_M(NC, std::vector<bool>(H0, false ) ); 
    std::vector<std::vector<std::vector<bool> > > map_good_start_I(NC, std::vector<std::vector<bool> >(H0, std::vector<bool>(W0, false) ) );
    std::vector<std::vector<std::vector<bool> > > map_good_end_I(NC, std::vector<std::vector<bool> >(H0, std::vector<bool>(W0, false) ) );
    for(int ch = 0; ch < NC; ch++)
        for(int orow = 0; orow < H0; orow++)
        {
            bool goodstartM = (ch * H * W + (orow * stride_y - padding_top) * W - padding_left) >= 0;
            bool endMBroken = false; 
            for(int mrow = orow; mrow < orow + MultiH; mrow++)
            {
                if((ch*H0*W0 + mrow * W0 + (W0 - 1)) >= NC * H0 * W0)
                {
                    endMBroken = true;
                    break;
                }
                for(int krow = mrow * stride_y; krow < mrow * stride_y + kh; krow++)
                    if((ch*H*W + (krow - padding_top) * W + lastloadedM) >= NC * H * W) 
                    {
                        endMBroken = true;
                        break;
                    }
                if(endMBroken) break;
            }
            map_good_start_M[ch][orow] = goodstartM;
            map_good_end_M[ch][orow] = !endMBroken;
            for(int col = 0; col < W0; col++)
            {
                int minY = std::max(orow * stride_y - padding_top, 0);
                int minX = col * stride_x - padding_left;
                int maxY = std::min(minY + kh - 1, H - 1);
                int maxX = minX + lsimd;
                map_good_start_I[ch][orow][col] = (ch * H * W + minY * W + minX >= 0);
                map_good_end_I[ch][orow][col] = (ch * H * W + maxY * W + maxX < NC*H*W) &&
                                                (ch *H0 *W0 + orow * W0 + col + lanes - 1 < NC*H0*W0);
            }
        }
    int Cms = NC + 1, Cme = 0, Cis = NC + 1, Cie = 0;
    int Yms = H0 + 1, Yme = 0, Yis = H0 + 1, Yie = 0;
    int Xis = W0 + 1, Xie = 0; 
    std::vector<int> map_good_start_M_num(NC, 0); 
    std::vector<int> map_good_end_M_num(NC, -1); 
    std::vector<std::vector<int> > map_good_start_I_num(NC, std::vector<int>(H0, 0)); 
    std::vector<std::vector<int> > map_good_end_I_num(NC, std::vector<int>(H0, -1)); 
    for(int ch = 0; ch < NC; ch++)
    {
        int good_start_M_num = H0 + 1;
        int good_end_M_num = -1;
        for(int orow = 0; orow < H0; orow++)
        {
            good_start_M_num = map_good_start_M[ch][orow] ? std::min(orow, good_start_M_num): good_start_M_num; 
            good_end_M_num = map_good_end_M[ch][orow] ? orow : good_end_M_num;
            int good_start_I_num = W0 + 1;
            int good_end_I_num = -1;
            for(int col = 0; col < W0; col++)
            {
                good_start_I_num = map_good_start_I[ch][orow][col] ? std::min(col, good_start_I_num): good_start_I_num;
                good_end_I_num = map_good_end_I[ch][orow][col] ? col : good_end_I_num;
            }
            map_good_start_I_num[ch][orow] = good_start_I_num;
            map_good_end_I_num[ch][orow] = good_end_I_num;
        }
        map_good_start_M_num[ch] = good_start_M_num;
        map_good_end_M_num[ch] = good_end_M_num;
    }

    for(int ch = 0; ch < NC; ch++)
    {
        if(W0 >= lanes)
        {
            if(Cms == NC + 1)
            { 
                if (map_good_start_M_num[ch] == 0)
                    Cms = ch;
                else if (map_good_start_M_num[ch] < H0 + 1 && ch == NC - 1)
                    Cms = NC;
            }
            if(map_good_end_M_num[ch] != -1)
                Cme = ((map_good_end_M_num[ch]/MultiH + 1) * MultiH >= H0) ? ch + 1 : ch;
        }
        int start_row_I = H0 + 1;
        int start_full_row_I = H0 + 1;
        int end_row_I = -1;
        int end_full_row_I = -1;
        for(int orow = 0; orow < H0; orow++)
        {
            start_full_row_I = ((start_full_row_I == H0 + 1) && map_good_start_I_num[ch][orow] == 0) ? orow : start_full_row_I; 
            start_row_I = ((start_row_I == H0 + 1) && map_good_start_I_num[ch][orow] >= 0) ? orow : start_row_I; 
            end_row_I = map_good_end_I_num[ch][orow] != -1 ? orow : end_row_I;
            end_full_row_I = ((map_good_end_I_num[ch][orow] != -1) && ((map_good_end_I_num[ch][orow]/lanes+1)*lanes >= W0)) ? orow : end_full_row_I;
        }
        Cis = (Cis == NC + 1) ? (start_full_row_I == 0 ? ch : (start_row_I != H0 - 1 ? ch + 1 : Cis)) : Cis;
        Cie = (end_row_I != -1 ?
                 (end_full_row_I == H0 - 1 ? ch + 1 : ch ) :
              Cie);
    }
    if(Cms < NC + 1)
    {
        Yms = Cms > 0 ? map_good_start_M_num[Cms - 1] : 0;
        Yme = (Cme < NC && map_good_end_M_num[Cme] != -1) ? ((Cme == Cms - 1) ? ((map_good_end_M_num[Cme] - Yms)/MultiH + 1)*MultiH + Yms: (map_good_end_M_num[Cme]/MultiH + 1)*MultiH) : 0;
    }

    if(Cis < NC + 1)
    {
        int start_full_row_I = H0 + 1;
        int end_full_row_I = -1;
        for(int orow = 0; orow < H0; orow++)
        {
            int sCis = (Cis > 0 ? Cis-1 : Cis);    //Just for memory safety  
            int sCie = (Cie < NC ? Cie : NC - 1);  //Useless calculations are allowed.
            start_full_row_I = ((start_full_row_I == H0 + 1) && map_good_start_I_num[sCis][orow] == 0) ? orow : start_full_row_I; 
            end_full_row_I = ((map_good_end_I_num[sCie][orow] != -1) && ((map_good_end_I_num[sCie][orow]/lanes+1)*lanes >= W0)) ? orow : end_full_row_I;
        }
        Yis = Cis > 0 ? start_full_row_I : 0;
        Yie = (Cie < NC) ? (end_full_row_I != H0 ? end_full_row_I + 1: 0): 0;
        if(Yis < H0 + 1)
        {	
            Xis = Cis > 0 && Yis > 0 ? (Yis < H0 + 1 ? map_good_start_I_num[Cis-1][Yis-1] : W0 + 1) : 0;
            Xie = ((Cie < NC) && (Yie < H0) && (map_good_end_I_num[Cie][Yie] != -1)) ?  
                    ((Cie == Cis - 1) && (Yie == Yis - 1) ? 
                    ((map_good_end_I_num[Cie][Yie] - Xis)/lanes + 1) * lanes + Xis: 
                    (map_good_end_I_num[Cie][Yie]/lanes + 1) * lanes):
                0;
        }
    }
    return dwc_algs_limits(Cms, Cme, Cis, Cie, Yms, Yme, Yis, Yie, Xis, Xie);
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

bool DepthwiseconvTestImpl::compare_algs_limits(const dwc_algs_limits& tocheck, const dwc_algs_limits& reference)
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

void DepthwiseconvTestImpl::print_algs_limits(const dwc_algs_limits& toprint)
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
    const int NC = fxt[3];
    const int C = NC;
    const int kCS = 0;
    const int H = fxt[4];
    const int W = fxt[5];
    const int padding_top = fxt[6];
    const int padding_left = fxt[7];
    const int padding_bottom = fxt[8];
    const int padding_right = fxt[9];
    const int stride_y = fxt[10];
    const int stride_x = fxt[11];
    const int activation = fxt[12];
    const float alpha = fxt[13] == BIG_ALPHA ? 1.25 : 0.25;
    bool perf = (fxt[14] == PERF);
    const int H0 = (H + padding_top + padding_bottom - kh) / stride_y + 1;
    const int W0 = (W + padding_left + padding_right - kw) / stride_x + 1;

    (*out) << "Depthwise convolution "<<(fxt[0]==TYPE_FP16?"FP16 ":"FP32 ")<<kh<<"x"<<kw<<", C = "<< NC << ", H = "<< H << ", W = "<< W << ", pt = "<< padding_top << ", pl = "<< padding_left << ", pb = "<< padding_bottom << ", pr = "<< padding_right << ", stride_y = " << stride_y << ", stride_x = " << stride_x  << std::endl;
    const _Tp empty_value(kh * kw * 2000 + 1);
    dwc_algs_limits algs_limits;
    const dwc_algs_limits ref_limits = ref_calc_algs_limits<_Tp>(NC, H, W, kh, kw, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, 1, 1);
    DWCTestTraits<_Tp>::calc_dwc_algs_limits(CTX, &algs_limits, NC, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x);

    if(!compare_algs_limits(algs_limits, ref_limits))
    {
        (*out)<<"    ALGS_LIMITS CHECK FAILED!"<<std::endl;
        return false;
    }
    typename DWCTestTraits<_Tp>::dwconv_t func = DWCTestTraits<_Tp>::generate_dwc(CTX, kh, kw, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, activation, alpha);
    std::vector<_Tp> indata(W*H*NC);
    std::vector<_Tp> kernel(C*DWCTestTraits<_Tp>::padded_ksize(kh, kw, *CTX), _Tp(0));
    std::vector<_Tp> bias(C, _Tp(0));
    std::vector<_Tp> outdata(H0*W0*NC * 3, empty_value);
    std::vector<_Tp> outdataref(H0*W0*NC, _Tp(0));
    _Tp* inptr = &(indata[0]);
    _Tp* kptr = &(kernel[0]);
    _Tp* bptr = &(bias[0]);
    _Tp* optr = &(outdata[0]) + H0*W0*NC;
    _Tp* optrref = &(outdataref[0]);
    gendata(inptr, kptr, bptr,kh,kw, H, W, NC);
    ref(inptr, kptr, bptr, H, W, NC, optrref, H0, W0, alpha, kh, kw, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, activation);
    if(perf)
    {
        Timer t;
        int ret;
        for(int testiter = 0; testiter < TESTITERATIONS; testiter++)
        {
            t.start();
            ret = func(inptr, kptr, bptr, H, W, C, NC, kCS, optr, H0, W0, &algs_limits);
            t.stop();
        }
        if(compare(&(outdata[0]), optrref, NC, H0, W0, empty_value))
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
        ret = func(inptr, kptr, bptr, H, W, C, NC, kCS, optr, H0, W0, &algs_limits);
        if(!compare(&(outdata[0]), optrref, NC, H0, W0, empty_value))
        {
            (*out)<<"    FAILED!"<<std::endl;
            return false;
        }
    }
    return true;
}

inline int upDiv(int numerator, int denominator)
{
    if(numerator < 0)
        return -((-numerator)/denominator);
    int res = numerator / denominator;
    int back = res * denominator; 
    return (numerator - back ? res + 1: res);
}

template <typename _Tp>
struct mtCallData
{
    typename DWCTestTraits<_Tp>::dwconv_t func;
    int H, W, C, NC, kCS, H0, W0;
    _Tp *data, *result, *kernel, *bias;
    dwc_algs_limits* algs_limits;
    mtCallData(){}
    mtCallData(typename DWCTestTraits<_Tp>::dwconv_t func_, _Tp* data_, _Tp* kernel_, _Tp* bias_, int H_, int W_, int C_, int NC_, int kCS_, _Tp* result_, int H0_, int W0_, dwc_algs_limits* algs_limits_):
        func(func_), data(data_), kernel(kernel_), bias(bias_), H(H_), W(W_), C(C_), NC(NC_), kCS(kCS_), result(result_), H0(H0_), W0(W0_), algs_limits(algs_limits_){}
};

template <typename _Tp>
void mtCall(void* data_)
{
    mtCallData<_Tp>* data = (mtCallData<_Tp>*)data_;
    data->func(data->data, data->kernel, data->bias, data->H, data->W, data->C, data->NC, data->kCS, data->result, data->H0, data->W0, data->algs_limits);
}

template <typename _Tp>
bool DepthwiseconvTestImpl::handleFixtureMultithread(const std::vector<int>& fxt)
{
    const int TESTITERATIONS = 30;
    int kh = fxt[1];
    int kw = fxt[2];
    const int N = fxt[3];
    const int C = fxt[4];
    const int NC = N*C;
    const int H = fxt[5];
    const int W = fxt[6];
    const int padding_top = fxt[7];
    const int padding_left = fxt[8];
    const int padding_bottom = fxt[9];
    const int padding_right = fxt[10];
    const int stride_y = fxt[11];
    const int stride_x = fxt[12];
    const int activation = fxt[13];
    const float alpha = fxt[14] == BIG_ALPHA ? 1.25 : 0.25;
    int threads = fxt[15];
    
    const int H0 = (H + padding_top + padding_bottom - kh) / stride_y + 1;
    const int W0 = (W + padding_left + padding_right - kw) / stride_x + 1;

    const _Tp empty_value(kh * kw * 2000 + 1);
    int NCtask_ = NC / threads;
    int tailTaskNum = NC % threads;

    (*out) << "Depthwise convolution "<< ( fxt[0]==TYPE_FP16 ? "FP16 " : "FP32 ")<<kh<<"x"<<kw<<", Batch = "<<N<<", C = "<< C << ", H = "<< H << ", W = "<< W << ", pt = "<< padding_top << ", pl = "<< padding_left << ", pb = "<< padding_bottom << ", pr = "<< padding_right << ", stride_y = " << stride_y << ", stride_x = " << stride_x << ", threads = " << threads << std::endl;
    dwc_algs_limits algs_limits_;
    DWCTestTraits<_Tp>::calc_dwc_algs_limits(CTX, &algs_limits_, tailTaskNum ? NCtask_ + 1 : NCtask_, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x);
    const dwc_algs_limits ref_algs_limits_ = ref_calc_algs_limits<_Tp>(tailTaskNum ? NCtask_ + 1 : NCtask_, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, 1, 1);
    if(!compare_algs_limits(algs_limits_, ref_algs_limits_))
    {
        (*out)<<"    ALGS_LIMITS CHECK FAILED!"<<std::endl;
        return false;
    }
    dwc_algs_limits algs_limits_tail;
    if(tailTaskNum)
    {
        DWCTestTraits<_Tp>::calc_dwc_algs_limits(CTX, &algs_limits_tail, NCtask_, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x);
        const dwc_algs_limits ref_algs_limits_tail = ref_calc_algs_limits<_Tp>(NCtask_, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, 1, 1);
        if(!compare_algs_limits(algs_limits_tail, ref_algs_limits_tail))
        {
            (*out)<<"    ALGS_LIMITS CHECK FAILED!"<<std::endl;
            return false;
        }
    }

    typename DWCTestTraits<_Tp>::dwconv_t func = DWCTestTraits<_Tp>::generate_dwc(CTX, kh, kw, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, activation, alpha);
    std::vector<_Tp> indata(W*H*NC);
    std::vector<_Tp> kernel(C * DWCTestTraits<_Tp>::padded_ksize(kh, kw, *CTX), _Tp(0));
    std::vector<_Tp> bias(C, _Tp(0));
    std::vector<_Tp> outdataref(H0*W0*NC, _Tp(0));
    _Tp* inptr = &(indata[0]);
    _Tp* kptr = &(kernel[0]);
    _Tp* bptr = &(bias[0]);
    _Tp* optrref = &(outdataref[0]);
    gendata(inptr, kptr, bptr,kh,kw, H, W, NC);
    ref(inptr, kptr, bptr, H, W, NC, optrref, H0, W0, alpha, kh, kw, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, activation);

    //First, separate calls of threads and checking portions of results
    int NC0 = 0;
    for(int ntask = 0; ntask < threads; ntask++)
    {
        int NCtask = (ntask < tailTaskNum) ? NCtask_ + 1: NCtask_;
        int kCS = NC0 % C;
        dwc_algs_limits* algs_limits = (ntask < tailTaskNum) ? &algs_limits_ : &algs_limits_tail;

        std::vector<_Tp> outdata(H0*W0*NCtask * 3, empty_value);
        _Tp* optr = &(outdata[0]) + H0*W0*NCtask;

        int NCtaskTail  = NC % NCtask_;
        int tailTaskNum = NC / NCtask_;

        int ret;
        ret = func(inptr + NC0 * H * W, kptr, bptr, H, W, C, NCtask, kCS, optr, H0, W0, algs_limits);
        if(!compare(&(outdata[0]), optrref + NC0 * H0 * W0, NCtask, H0, W0, empty_value))
        {
            (*out)<<"    FAILED! Portion " << ntask <<std::endl;
            return false;
        }
        NC0 += NCtask;
    }

    //Second, parallel for
    std::vector< mtCallData<_Tp> > mtdatas(threads, mtCallData<_Tp>());
    std::vector<std::thread> tpool; 
    std::vector<_Tp> outdata(H0*W0*NC*3, empty_value);
    _Tp* optr = &(outdata[0]) + H0*W0*NC;
    NC0 = 0;
    for(int ntask = 0; ntask < threads; ntask++)
    {
        int NCtask = (ntask < tailTaskNum) ? NCtask_ + 1: NCtask_;
        int kCS = NC0 % C;
        dwc_algs_limits* algs_limits = (ntask < tailTaskNum) ? &algs_limits_ : &algs_limits_tail;
        mtdatas[ntask] = mtCallData<_Tp>(func, inptr + NC0 * H * W, kptr, bptr, H, W, C, NCtask, kCS, optr + NC0 * H0 * W0, H0, W0, algs_limits);
        tpool.push_back(std::thread(&(mtCall<_Tp>), &(mtdatas[ntask])));
        NC0 += NCtask;
    }
    for(int ntask = 0; ntask < threads; ntask++)
        tpool[ntask].join();
    if(!compare(&(outdata[0]), optrref, NC, H0, W0, empty_value))
    {
        (*out)<<"    FAILED in multithreading mode!" <<std::endl;
        return false;
    }
    return true;
}


void DepthwiseconvTestImpl::run() 
{
    std::cout << "==========================================  ALGORITHM LIMITS CALCULATION  ==========================================================="<<std::endl;
    std::vector<std::vector<int>> limitsFixtures = {
        {3, 3, 3, 10, 3, 0, 0, 0, 0, 1, 1},
        {3, 3, 3, 3, 10, 0, 0, 0, 0, 1, 1},
        {5, 5, 3, 10, 12, 0, 0, 0, 0, 1, 1},
        {3, 3, 3, 10, 10, 0, 0, 0, 0, 1, 1},
        {3, 3, 3, 10, 10, 1, 0, 1, 0, 1, 1},
        {3, 3, 3, 10, 10, 0, 1, 0, 1, 1, 1},
        {3, 3, 3, 10, 10, 1, 1, 1, 1, 1, 1},
        {3, 3, 1, 10, 10, 0, 0, 0, 0, 1, 1},
        {3, 3, 1, 10, 10, 1, 0, 1, 0, 1, 1},
        {3, 3, 1, 10, 10, 0, 1, 0, 1, 1, 1},
        {3, 3, 1, 10, 10, 1, 1, 1, 1, 1, 1},
        {3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {3, 3, 100, 1, 1, 1, 1, 1, 1, 1, 1},

        {3, 3, 3, 10, 3, 0, 0, 0, 0, 2, 1},
        {3, 3, 3, 3, 10, 0, 0, 0, 0, 2, 1},
        {5, 5, 3, 10, 12, 0, 0, 0, 0, 2, 1},
        {3, 3, 3, 10, 10, 0, 0, 0, 0, 2, 1},
        {3, 3, 3, 10, 10, 1, 0, 1, 0, 2, 1},
        {3, 3, 3, 10, 10, 0, 1, 0, 1, 2, 1},
        {3, 3, 3, 10, 10, 1, 1, 1, 1, 2, 1},
        {3, 3, 1, 10, 10, 0, 0, 0, 0, 2, 1},
        {3, 3, 1, 10, 10, 1, 0, 1, 0, 2, 1},
        {3, 3, 1, 10, 10, 0, 1, 0, 1, 2, 1},
        {3, 3, 1, 10, 10, 1, 1, 1, 1, 2, 1},

        {3, 3, 3, 10, 3, 0, 0, 0, 0, 1, 2},
        {3, 3, 3, 3, 10, 0, 0, 0, 0, 1, 2},
        {5, 5, 3, 10, 12, 0, 0, 0, 0, 1, 2},
        {3, 3, 3, 10, 10, 0, 0, 0, 0, 1, 2},
        {3, 3, 3, 10, 10, 1, 0, 1, 0, 1, 2},
        {3, 3, 3, 10, 10, 0, 1, 0, 1, 1, 2},
        {3, 3, 3, 10, 10, 1, 1, 1, 1, 1, 2},
        {3, 3, 1, 10, 10, 0, 0, 0, 0, 1, 2},
        {3, 3, 1, 10, 10, 1, 0, 1, 0, 1, 2},
        {3, 3, 1, 10, 10, 0, 1, 0, 1, 1, 2},
        {3, 3, 1, 10, 10, 1, 1, 1, 1, 1, 2},

        {3, 3, 3, 10, 3, 0, 0, 0, 0, 2, 2},
        {3, 3, 3, 3, 10, 0, 0, 0, 0, 2, 2},
        {5, 5, 3, 10, 12, 0, 0, 0, 0, 2, 2},
        {3, 3, 3, 10, 10, 0, 0, 0, 0, 2, 2},
        {3, 3, 3, 10, 10, 1, 0, 1, 0, 2, 2},
        {3, 3, 3, 10, 10, 0, 1, 0, 1, 2, 2},
        {3, 3, 3, 10, 10, 1, 1, 1, 1, 2, 2},
        {3, 3, 1, 10, 10, 0, 0, 0, 0, 2, 2},
        {3, 3, 1, 10, 10, 1, 0, 1, 0, 2, 2},
        {3, 3, 1, 10, 10, 0, 1, 0, 1, 2, 2},
        {3, 3, 1, 10, 10, 1, 1, 1, 1, 2, 2},
        };
//      {kh,kw, NC, H, W, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x}

    for(auto fxt: limitsFixtures)
    {
        int kh = fxt[0];
        int kw = fxt[1];
        const int NC = fxt[2];
        const int H = fxt[3];
        const int W = fxt[4];
        const int padding_top = fxt[5];
        const int padding_left = fxt[6];
        const int padding_bottom = fxt[7];
        const int padding_right = fxt[8];
        const int stride_y = fxt[9];
        const int stride_x = fxt[10];
        const int H0 = (H + padding_top + padding_bottom - kh) / stride_y + 1;
        const int W0 = (W + padding_left + padding_right - kw) / stride_x + 1;
        const dwc_algs_limits ref = ref_calc_algs_limits<float>(NC, H, W, kh, kw, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, 1, 1);

        (*out) << "Depthwise convolution "<<kh<<"x"<<kw<<", NC = "<< NC << ", H = "<< H << ", W = "<< W << ", pt = "<< padding_top << ", pl = "<< padding_left << ", pb = "<< padding_bottom << ", pr = "<< padding_right << ", stride_y = " << stride_y << ", stride_x = " << stride_x << std::endl;
        dwc_algs_limits tocheck;

        calc_dwc_algs_limits_f32(CTX, &tocheck, NC, H, W, kh, kw, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, 1, 1);
        if(!compare_algs_limits(tocheck, ref))
            return;
    }
    std::cout << "=================================================  SINGLETHREAD TESTS  =============================================================="<<std::endl;
    std::vector<std::vector<int> > fixtures = {
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 1, 0, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 1, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 1, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 0, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 1, 0, 1, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 1, 0, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 1, 1, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10,  3, 0, 0, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 0, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 1, 0, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 1, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 0, 1, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 0, 0, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 1, 0, 1, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 1, 0, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 1, 1, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 5, 5, 5, 11, 11, 2, 2, 2, 2, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 7, 7, 5, 11, 11, 3, 3, 3, 3, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 9, 9, 5, 11, 11, 5, 5, 5, 5, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 1, 0, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 1, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 1, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 0, 1, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 1, 0, 1, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 1, 0, 1, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 1, 1, 1, 1, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10,  3, 0, 0, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 0, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 1, 0, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 1, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 0, 1, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 0, 0, 1, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 1, 0, 1, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 1, 0, 1, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 1, 1, 1, 1, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 5, 5, 5, 11, 11, 2, 2, 2, 2, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 7, 7, 5, 11, 11, 3, 3, 3, 3, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 9, 9, 5, 11, 11, 5, 5, 5, 5, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 1, 0, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 1, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 1, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 0, 1, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 1, 0, 1, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 1, 0, 1, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 1, 1, 1, 1, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10,  3, 0, 0, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 0, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 1, 0, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 1, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 0, 1, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 0, 0, 1, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 1, 0, 1, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 1, 0, 1, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 1, 1, 1, 1, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 5, 5, 5, 11, 11, 2, 2, 2, 2, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 7, 7, 5, 11, 11, 3, 3, 3, 3, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 1, 0, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 1, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 1, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 0, 0, 1, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 1, 0, 1, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 0, 1, 0, 1, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10, 10, 1, 1, 1, 1, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 3, 10,  3, 0, 0, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 0, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 1, 0, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 1, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 0, 1, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 0, 0, 1, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 1, 0, 1, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 0, 1, 0, 1, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 1, 1, 1, 1, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 5, 5, 5, 11, 11, 2, 2, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 7, 7, 5, 11, 11, 3, 3, 3, 3, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 5, 5, 5, 5, 1, 1, ACT_RELU, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 5, 5, 5, 5, 1, 1, ACT_RELU6, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 5, 5, 5, 5, 1, 1, ACT_LRELU, SMALL_ALPHA, REGRESS},
        {TYPE_FP16, 3, 3, 5, 11, 11, 5, 5, 5, 5, 1, 1, ACT_LRELU, BIG_ALPHA, REGRESS},

        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 0, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 1, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 1, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 0, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 0, 1, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 1, 0, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 1, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10,  3, 0, 0, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 0, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 1, 0, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 1, 0, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 0, 1, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 0, 0, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 1, 0, 1, 0, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 1, 0, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 1, 1, 1, 1, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 5, 5, 5, 11, 11, 2, 2, 2, 2, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 7, 7, 5, 11, 11, 3, 3, 3, 3, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 9, 9, 5, 11, 11, 5, 5, 5, 5, 1, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 0, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 1, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 1, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 0, 1, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 0, 1, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 1, 0, 1, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 1, 1, 1, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10,  3, 0, 0, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 0, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 1, 0, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 1, 0, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 0, 1, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 0, 0, 1, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 1, 0, 1, 0, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 1, 0, 1, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 1, 1, 1, 1, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 5, 5, 5, 11, 11, 2, 2, 2, 2, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 7, 7, 5, 11, 11, 3, 3, 3, 3, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 9, 9, 5, 11, 11, 5, 5, 5, 5, 2, 1, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 0, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 1, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 1, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 0, 1, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 0, 1, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 1, 0, 1, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 1, 1, 1, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10,  3, 0, 0, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 0, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 1, 0, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 1, 0, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 0, 1, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 0, 0, 1, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 1, 0, 1, 0, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 1, 0, 1, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 1, 1, 1, 1, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 5, 5, 5, 11, 11, 2, 2, 2, 2, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 7, 7, 5, 11, 11, 3, 3, 3, 3, 1, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 0, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 1, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 1, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 0, 0, 1, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 0, 1, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 0, 1, 0, 1, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10, 10, 1, 1, 1, 1, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 3, 10,  3, 0, 0, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 0, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 1, 0, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 1, 0, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 0, 1, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 0, 0, 1, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 1, 0, 1, 0, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 0, 1, 0, 1, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 1, 1, 1, 1, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 5, 5, 5, 11, 11, 2, 2, 2, 2, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 7, 7, 5, 11, 11, 3, 3, 3, 3, 2, 2, ACT_NONE, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 5, 5, 5, 5, 1, 1, ACT_RELU, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 5, 5, 5, 5, 1, 1, ACT_RELU6, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 5, 5, 5, 5, 1, 1, ACT_LRELU, SMALL_ALPHA, REGRESS},
        {TYPE_FP32, 3, 3, 5, 11, 11, 5, 5, 5, 5, 1, 1, ACT_LRELU, BIG_ALPHA, REGRESS},

        {TYPE_FP16, 5, 5, 1632, 7, 7, 2, 2, 2, 2, 1, 1, ACT_RELU6, SMALL_ALPHA, PERF},
        {TYPE_FP16, 3, 3, 32, 112, 112, 1, 1, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, PERF},
        {TYPE_FP16, 3, 3, 192, 56, 56, 1, 1, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, PERF},
        {TYPE_FP32, 5, 5, 1632, 7, 7, 2, 2, 2, 2, 1, 1, ACT_RELU6, SMALL_ALPHA, PERF},
        {TYPE_FP32, 3, 3, 32, 112, 112, 1, 1, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, PERF},
        {TYPE_FP32, 3, 3, 192, 56, 56, 1, 1, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, PERF},
//      {kh,kw, NC, H, W, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x}
    };
    for(auto fxt: fixtures)
        if(fxt[0] == TYPE_FP32)
        {
            if(!handleFixture<float>(fxt))
                return;
        }
        else if(fxt[0] == TYPE_FP16)
        {
            if(!handleFixture<f16_t>(fxt))
                return;
        }
    std::cout << "=================================================  MULTITHREAD TESTS  =============================================================="<<std::endl;
    std::vector<std::vector<int> > fixtures_mt = {
        {TYPE_FP16, 5, 5, 1, 1632, 7, 7, 2, 2, 2, 2, 1, 1, ACT_RELU6, SMALL_ALPHA, 8},
        {TYPE_FP16, 5, 5, 1, 1633, 7, 7, 2, 2, 2, 2, 1, 1, ACT_RELU6, SMALL_ALPHA, 8},
        {TYPE_FP16, 3, 3, 1, 32, 112, 112, 1, 1, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, 8},
        {TYPE_FP16, 3, 3, 1, 192, 56, 56, 1, 1, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, 8},
        {TYPE_FP16, 3, 3, 1, 193, 56, 56, 1, 1, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, 8},
        {TYPE_FP32, 3, 3, 1, 33, 112, 112, 1, 1, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, 8},
        {TYPE_FP32, 5, 5, 1, 1632, 7, 7, 2, 2, 2, 2, 1, 1, ACT_RELU6, SMALL_ALPHA, 8},
        {TYPE_FP32, 3, 3, 1, 32, 112, 112, 1, 1, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, 8},
        {TYPE_FP32, 3, 3, 1, 192, 56, 56, 1, 1, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, 8},
        {TYPE_FP32, 3, 3, 1, 193, 56, 56, 1, 1, 1, 1, 1, 1, ACT_RELU6, SMALL_ALPHA, 8},
//      {type, kh,kw, N, C, H, W, padding_top, padding_left, padding_bottom, padding_right}
    };
    for(auto fxt: fixtures_mt)
    {
        if(fxt[0] == TYPE_FP32)
        {
            if(!handleFixtureMultithread<float>(fxt))
                return;
        }
        else if(fxt[0] == TYPE_FP16)
        {
            if(!handleFixtureMultithread<f16_t>(fxt))
                return;
        }
    };
}
};
#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
