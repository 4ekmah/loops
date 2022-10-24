/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
#ifndef __LOOPS_DEPTHWISECONV_HPP__
#define __LOOPS_DEPTHWISECONV_HPP__
#include "loops/loops.hpp"
#if __LOOPS_ARCH ==  __LOOPS_AARCH64
namespace loops
{

struct dwc_algs_limits
{
    int64_t Cms; //Channel on which starts clear multiline scheme behaviour
    int64_t Cme; //Channel after clear multiline scheme behaviour
    int64_t Cis; //Channel on which starts clear oneline SIMD scheme behaviour
    int64_t Cie; //Channel after clear oneline SIMD scheme behaviour
    int64_t Yms; //Y on (Cms - 1) on which starts multiline scheme behaviour
    int64_t Yme; //Y on Cme after multiline scheme behaviour
    int64_t Yis; //Y on (Cis - 1) on which starts oneline SIMD scheme behaviour
    int64_t Yie; //Y on Cie after oneline SIMD scheme behaviour
    int64_t Xis; //X on (Cis - 1) on Y < Yis on which starts oneline SIMD scheme behaviour
    int64_t Xie; //X on Cie on Y >= Yie after oneline SIMD scheme behaviour
    dwc_algs_limits(int64_t _Cms, int64_t _Cme, int64_t _Cis, int64_t _Cie, int64_t _Yms, int64_t _Yme, int64_t _Yis, int64_t _Yie, int64_t _Xis, int64_t _Xie):
                    Cms(_Cms),Cme(_Cme),Cis(_Cis),Cie(_Cie),Yms(_Yms),Yme(_Yme),Yis(_Yis),Yie(_Yie),Xis(_Xis),Xie(_Xie){}
};

class DepthwiseconvTest
{
public:
    DepthwiseconvTest(Context aCTX, std::ostream* a_out);
    ~DepthwiseconvTest(){}
    void run();
private:
    void gendata(float* data, float* kernel, float* bias, int kh, int kw, int H, int W, int C);
    void ref(float* data, float* kernel, float* bias, int H, int W, int C, float* result, int H0, int W0, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right);
    bool compare(float* tocheck, float* ref, int C, int H, int W, float empty_value);
    bool compare_alg_limits(const dwc_algs_limits& tocheck, const dwc_algs_limits& reference);
    Context CTX;
    std::ostream* out;
};
};

#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
#endif //__LOOPS_DEPTHWISECONV_HPP__