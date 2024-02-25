/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_TEST_MATH_HPP__
#define __LOOPS_TEST_MATH_HPP__

#include "tests.hpp"
#include <math.h>
#if __LOOPS_ARCH == __LOOPS_AARCH64
#include "arm_neon.h"
#endif

using namespace loops;

class fixed_power : public ::testing::TestWithParam<int> {};

TEST_P(fixed_power, fixed_power)
{
    int p = GetParam();
    std::string testname = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    testname.erase(testname.begin() + testname.find('/'), testname.end());
    testname += std::string("_") + std::to_string(p);
    Context ctx;
    USE_CONTEXT_(ctx);
    IReg x;
    STARTFUNC_(testname, &x)
    {
        RETURN_(pow(x,p));
    }
    typedef int64_t (*fixed_power_f)(int64_t x);
    loops::Func func = ctx.getFunc(testname);
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    fixed_power_f tested = reinterpret_cast<fixed_power_f>(func.ptr());
    std::vector<int64_t> X = {3,-5,7,-3,2,0,-1};
    for (size_t n = 0; n < X.size(); n++)
        ASSERT_EQ(tested(X[n]), ::pow(X[n], p));
}

INSTANTIATE_TEST_SUITE_P(math, fixed_power, ::testing::Values(0, 1, 9));

#if __LOOPS_ARCH == __LOOPS_AARCH64
template <typename _Tp, int _p>
struct fpv_param
{
    typedef _Tp elemtype;
    enum { power = _p };
};
template <typename param>
class fixed_power_v_suite : public testing::Test
{
private:
    typedef typename param::elemtype _Tp;
    enum { p = param::power };
public:
    void run()
    {
        std::string testname = ::testing::UnitTest::GetInstance()->current_test_info()->name();
        testname += std::string("_") + typestring<_Tp>::get() + "_" + std::to_string(p);
        Context ctx;
        USE_CONTEXT_(ctx);
        IReg src, powdest, v_size;
        STARTFUNC_(testname, &src, &powdest, &v_size)
        {
            IReg offset  = CONST_(0);
            v_size *= sizeof(_Tp);
            WHILE_(offset < v_size)
            {
                VReg<_Tp> in = loadvec<_Tp>(src, offset);
                VReg<_Tp> res = pow(in, p);

                storevec(powdest, offset, res);
                offset += ctx.vbytes();
            }
            RETURN_(0);
        }
        typedef int (*fixed_power_v_f)(const _Tp* src, _Tp* powdest, int64_t v_size);
        loops::Func func = ctx.getFunc(testname);
        switch_spill_stress_test_mode_on(func);
        EXPECT_IR_CORRECT(func);
        EXPECT_ASSEMBLY_CORRECT(func);
        fixed_power_v_f tested = reinterpret_cast<fixed_power_v_f>(func.ptr());
        std::vector<_Tp> v = { 0, 1, 2, 3, 4, 5, 6, 7};
        std::vector<_Tp> res_pow = { 0, 0, 0, 0, 0, 0, 0, 0 };
        ASSERT_EQ(tested(&v[0], &res_pow[0], v.size()), (int)(0));
        for (size_t vnum = 0; vnum < v.size(); vnum++)
        {
            _Tp tmp = static_cast<int>(::pow(v[vnum], static_cast<_Tp>(p)));
            ASSERT_EQ(res_pow[vnum], tmp);
        }
    };
};
TYPED_TEST_SUITE_P(fixed_power_v_suite);
TYPED_TEST_P(fixed_power_v_suite, fixed_power_v)
{
    this->run();
}
REGISTER_TYPED_TEST_SUITE_P(fixed_power_v_suite, fixed_power_v);
typedef testing::Types<
fpv_param<uint32_t, 0>,
fpv_param<uint32_t, 4>,
fpv_param<uint32_t, 9>,
fpv_param<int32_t, 0>,
fpv_param<int32_t, 4>,
fpv_param<int32_t, 9>,
fpv_param<float, 0>,
fpv_param<float, 4>,
fpv_param<float, 9>,
fpv_param<double, 0>,
fpv_param<double, 4>,
fpv_param<double, 9>
> fpv_cases;
INSTANTIATE_TYPED_TEST_SUITE_P(math, fixed_power_v_suite, fpv_cases);

TEST(math, exp_f32)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    {
        IReg dest, src, n;
        STARTFUNC_(test_info_->name(), &dest, &src, &n)
        {
            auto expc = expInit(ctx);
            IReg offset = CONST_(0);
            WHILE_(n > 0)
            {
                VReg<float> x = loadvec<float>(src,offset);
                storevec(dest, offset, exp(x, expc));
                offset += ctx.vbytes();
                n -= ctx.vlanes<float>();
            }
            RETURN_();
        }
    }
    typedef void (*exp_f32_f)(float* dest, const float* src, int64_t n);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    exp_f32_f tested = reinterpret_cast<exp_f32_f>(func.ptr());

    const float ln15 = ::log(15);
    std::vector<float> src  = { 88.3762626647949f, -90, 1, 2, -15, 4.6, 23.1, -3, 13.7, -14.8, 18.2, 56, 22.12, 85.05, -12.6, -36.6,
                                9.9, -12.5, 44, 1.7, 64.2, 34.8, -15.7, 55.5, 69, -34, ln15, 9, 0.2, 62.13, -74.5, -18.1 };
    std::vector<float> dest(src.size(), 0);
    tested(&dest[0], &src[0], src.size());
    for (size_t i = 0; i < src.size(); i++ )
        ASSERT_NEAR((float)(dest[i]), (float)(::exp(src[i])), 1.e-39f);
}
#endif //__LOOPS_ARCH == __LOOPS_AARCH64
#endif//__LOOPS_TEST_MATH_HPP__
