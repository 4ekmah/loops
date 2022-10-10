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
    typedef int64_t (*dwconv_t)(float* data, float* kernel, float* bias, int64_t H, int64_t W, int64_t C, float* result, int64_t H0, int64_t W0, int64_t padding_top, int64_t padding_left, int64_t padding_bottom, int64_t padding_right, int32_t* countingPatternPtr); //DUBUG: Delete counting template
    dwconv_t generate(int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right);
    enum { PADCOND = 1, INITDEST = 2, PREINCREMENT_IDXS = 4, PADSHIFTRES = 8 };
    void fixedVolumeHandler(const VReg<float>& vbias, const VReg<int32_t>& countingPattern, const VReg<int32_t>& idx_step,
                            const VReg<uint32_t>& WcondV, const std::vector<VReg<float> >& vkernel, IReg& x, 
                            IReg& base, const IReg& W, const IReg& result_rs, const IReg& padding_left,
                            int elemsize, int kh, int kw,  int flags);
    void loadVector(const IReg& base, int64_t offset, VReg<float>& dest, VReg<int32_t>& horIdxs, const VReg<int32_t>& idx_step, const VReg<uint32_t>& WcondV, int flags = 0);
    void gendata(float* data, float* kernel, float* bias, int kh, int kw, int H, int W, int C);
    void ref(float* data, float* kernel, float* bias, int H, int W, int C, float* result, int H0, int W0, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right);
    bool compare(float* tocheck, float* ref, int C, int H, int W);
    Context CTX;
    std::ostream* out;
};
};

#endif //__LOOPS_ARCH ==  __LOOPS_AARCH64
#endif //__LOOPS_DEPTHWISECONV_HPP__