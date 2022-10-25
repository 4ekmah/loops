/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
#ifndef __LOOPS_DWCTEST_HPP__
#define __LOOPS_DWCTEST_HPP__
#include "loops/loops.hpp"
#include "loopslayers/loopslayers.h"
#if __LOOPS_ARCH ==  __LOOPS_AARCH64
namespace loops
{
class DepthwiseconvTest
{
public:
    DepthwiseconvTest(Context aCTX, std::ostream* a_out);
    ~DepthwiseconvTest(){}
    void run();
private:
    void gendata(float* data, float* kernel, float* bias, int kh, int kw, int H, int W, int C);
    void ref(float* data, float* kernel, float* bias, int H, int W, int C, float* result, int H0, int W0, float alpha, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type);
    bool compare(float* tocheck, float* ref, int C, int H, int W, float empty_value);
    bool compare_alg_limits(const dwc_algs_limits& tocheck, const dwc_algs_limits& reference);
    Context CTX;
    std::ostream* out;
};
};

#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
#endif //__LOOPS_DWCTEST_HPP__