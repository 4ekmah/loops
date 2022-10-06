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
class DepthwiseconvTest
{
public:
    DepthwiseconvTest(Context aCTX, std::ostream* a_out);
    ~DepthwiseconvTest(){}
    void run();
private:
    typedef int (*dwconv_t)(float* data, float* kernel, float* bias, int64_t H, int64_t W, int64_t C, float* result, int64_t H0, int64_t W0);
    dwconv_t generate(int kh, int kw);
    void gendata(float* data, float* kernel, float* bias, int kh, int kw, int H, int W, int C);
    void ref(float* data, float* kernel, float* bias, int H, int W, int C, float* result, int H0, int W0, int kh, int kw);
    bool compare(float* tocheck, float* ref, int C, int H, int W);
    Context CTX;
    std::ostream* out;
};
};

#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
#endif //__LOOPS_DEPTHWISECONV_HPP__