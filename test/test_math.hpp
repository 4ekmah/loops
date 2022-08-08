/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_TEST_MATH_HPP__
#define __LOOPS_TEST_MATH_HPP__

#include "tests.hpp"
#include <math.h>

namespace loops
{
PTEST_1(exponentiation_by_squaring, int64_t, _p, {
    int p = _p;
    IReg ptrA, n, ptrPow;
    STARTFUNC_(TESTNAME, &ptrA, &n, &ptrPow)
    {
        IReg res = CONST_(1);
        IReg offset = CONST_(0);
        IReg i = CONST_(0);
        WHILE_(i < n)
            {
                IReg x = load_<int>(ptrA, offset);
                store_<int>(ptrPow, pow(x, p));
                i += 1;
                offset += sizeof(int);
                ptrPow += sizeof(int);
            };
        RETURN_(0);
    }
});

PTESTexe_1(exponentiation_by_squaring, int64_t, _p, {
    typedef int64_t(*exponential_by_squaring_f)(const int* ptrA, int64_t n, int* ptrPow);
    exponential_by_squaring_f tested = reinterpret_cast<exponential_by_squaring_f>(EXEPTR);
    std::vector<int> X = {3,-5,7,-3,2,0,-1};
    int resArr[7];
    EXPECT_EQ(tested(&X[0], X.size(), resArr), (int64_t)(0));
    for (size_t n = 0; n < X.size(); n++) {
        int tmp = static_cast<int>(::pow(X[n], _p));
        EXPECT_EQ(resArr[n], tmp);
    }
});

PTESTfix_1(exponentiation_by_squaring, 0);
PTESTfix_1(exponentiation_by_squaring, 1);
PTESTfix_1(exponentiation_by_squaring, 9);

#if __LOOPS_ARCH == __LOOPS_AARCH64
PTEST_2(exponentiation_by_squaring_v, typename, _Tp, int64_t, _p, {
    IReg src, powdest, v_size;
    int p = _p;
    
    STARTFUNC_(TESTNAME, &src, &powdest, &v_size)
    {
        IReg offset  = CONST_(0);
        v_size *= sizeof(_Tp);
        WHILE_(offset < v_size)
        {
            VReg<_Tp> in = loadvx<_Tp>(src, offset);
            VReg<_Tp> res = pow(in, p);

            storevx(powdest, offset, res);
            offset += CTX.vectorRegisterSize();
        }
        RETURN_(0);
    }
});

PTESTexe_2(exponentiation_by_squaring_v, typename, _Tp, int64_t, _p, {
    typedef int (*exponential_by_squaring_v)(const _Tp* src, _Tp* powdest, int64_t v_size);
    exponential_by_squaring_v tested = reinterpret_cast<exponential_by_squaring_v>(EXEPTR);
    std::vector<_Tp> v =   { 0, 1, 2, 3, 4, 5, 6, 7};
    std::vector<_Tp> res_pow = { 0, 0, 0, 0, 0, 0, 0, 0 };
    EXPECT_EQ(tested(&v[0], &res_pow[0], v.size()), (int)(0));
    for (size_t vnum = 0; vnum < v.size(); vnum++) {
        _Tp tmp = static_cast<int>(::pow(v[vnum], _p));
        EXPECT_EQ(res_pow[vnum], tmp);
    }
});

PTESTfix_2(exponentiation_by_squaring_v, uint32_t, 0);
PTESTfix_2(exponentiation_by_squaring_v, uint32_t, 4);
PTESTfix_2(exponentiation_by_squaring_v, uint32_t, 9);
PTESTfix_2(exponentiation_by_squaring_v, int32_t, 0);
PTESTfix_2(exponentiation_by_squaring_v, int32_t, 4);
PTESTfix_2(exponentiation_by_squaring_v, int32_t, 9);
PTESTfix_2(exponentiation_by_squaring_v, float, 0);
PTESTfix_2(exponentiation_by_squaring_v, float, 4);
PTESTfix_2(exponentiation_by_squaring_v, float, 9);
PTESTfix_2(exponentiation_by_squaring_v, double, 0);
PTESTfix_2(exponentiation_by_squaring_v, double, 4);
PTESTfix_2(exponentiation_by_squaring_v, double, 9);

LTEST(exp_f32, {
    IReg dest, src, n;
    USE_CONTEXT_(CTX);
    STARTFUNC_(TESTNAME, &dest, &src, &n)
    {
        auto expc = expInitConsts(CTX);
        IReg offset = CONST_(0);
        WHILE_(n > 0)
        {
            VReg<float> x = loadvx<float>(src,offset);
            storevx(dest, offset, exp(x, expc));
            offset += CTX.vectorRegisterSize();
            n -= CTX.vlanes<float>();
        }
        RETURN_();
    }
    });

LTESTexe(exp_f32, {
    typedef void (*exp_f32_f)(float* dest, const float* src, int n);
    exp_f32_f tested = reinterpret_cast<exp_f32_f>(EXEPTR);
    const float ln15 = ::log(15);
    std::vector<float> src  = { 88.3762626647949f, -90, 1, 2, -15, 4.6, 23.1, -3, 13.7, -14.8, 18.2, 56, 22.12, 85.05, -12.6, -36.6,
                                9.9, -12.5, 44, 1.7, 64.2, 34.8, -15.7, 55.5, 69, -34, ln15, 9, 0.2, 62.13, -74.5, -18.1 };
    std::vector<float> dest(src.size(), 0);
    tested(&dest[0], &src[0], src.size());
    for (size_t i = 0; i < src.size(); i++ )
        EXPECT_NEAR((float)(dest[i]), (float)(::exp(src[i])), 1.e-39f);
    });
#endif //__LOOPS_ARCH == __LOOPS_AARCH64

};
#endif//__LOOPS_TEST_MATH_HPP__
