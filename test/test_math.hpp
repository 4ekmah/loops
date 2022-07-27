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
    IReg x;
    STARTFUNC_(TESTNAME, &x)
    {
        RETURN_(__pow__(x, p, CTX));
    }
});

PTESTexe_1(exponentiation_by_squaring, int64_t, _p, {
    typedef int64_t(*exponential_by_squaring_f)(int64_t x);
    exponential_by_squaring_f tested = reinterpret_cast<exponential_by_squaring_f>(EXEPTR);
    const int p = _p;
    std::vector<int64_t> X = {3,-5,7,-3,2,0,-1};
    for (size_t n = 0; n < X.size(); n++) {
        int64_t tmp = pow(X[n], p);
        TEST_EQ(tested(X[n]), tmp);
    }
});

PTESTfix_1(exponentiation_by_squaring, 0);
PTESTfix_1(exponentiation_by_squaring, 1);
PTESTfix_1(exponentiation_by_squaring, 9);
};
#endif//__LOOPS_TEST_MATH_HPP__
