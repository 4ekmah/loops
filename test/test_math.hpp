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
IReg __pow__(const IReg& _x, int p, loops::Context& CTX)
{
    USE_CONTEXT_(CTX);
    IReg x = _x;
    IReg res = CONST_(1);
    while (p)
        if (p & 1) {
            res *= x;
            --p;
        }
        else {
            x *= x;
            p >>= 1;
        }
    return res;
}

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
                store_<int>(ptrPow, __pow__(x, p, CTX));
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
    TEST_EQ(tested(&X[0], X.size(), resArr), (int64_t)(0));
    for (size_t n = 0; n < X.size(); n++) {
        int tmp = static_cast<int>(pow(X[n], _p));
        TEST_EQ(resArr[n], tmp);
    }
});

PTESTfix_1(exponentiation_by_squaring, 0);
PTESTfix_1(exponentiation_by_squaring, 1);
PTESTfix_1(exponentiation_by_squaring, 9);
};
#endif//__LOOPS_TEST_MATH_HPP__
