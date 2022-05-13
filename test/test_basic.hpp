/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_TEST_BASIC_HPP__
#define __LOOPS_TEST_BASIC_HPP__

#include "loops/loops.hpp"
#include "../src/common.hpp"        //TODO(ch): .. in path is bad practice. Configure project
#include "../src/reg_allocator.hpp" //TODO(ch): .. in path is bad practice. Configure project
#include "tests.hpp"
#include <iostream>

namespace loops
{
LTEST(min_max_scalar, {
    IReg ptr, n, minpos_addr, maxpos_addr;
    CTX.startFunc(TESTNAME, {&ptr, &n, &minpos_addr, &maxpos_addr});
    IReg i = CTX.const_(0);
    IReg minpos = CTX.const_(0);
    IReg maxpos = CTX.const_(0);
    IReg minval = load_<int>(ptr);
    IReg maxval = minval;
    IReg elemsize = CTX.const_((int)sizeof(int));
    n *= elemsize;
    CTX.do_();
        IReg x = load_<int>(ptr, i);
        CTX.if_(x < minval);
            minval = x;
            minpos = i;
        CTX.endif_();
        CTX.if_(x > maxval);
            maxval = x;
            maxpos = i;
        CTX.endif_();
        i += elemsize;
    CTX.while_(i < n);
    minpos /= elemsize;
    maxpos /= elemsize;
    store_<int>(minpos_addr, minpos);
    store_<int>(maxpos_addr, maxpos);
    CTX.return_(CTX.const_(0));
    CTX.endFunc();
});
LTESTexe(min_max_scalar, {
    typedef int (*minmaxscalar_f)(const int* ptr, int64_t n, int* minpos, int* maxpos);
    std::vector<int> v = { 8, 2, -5, 7, 6 };
    int minpos = -1, maxpos = -1;
    minmaxscalar_f tested = reinterpret_cast<minmaxscalar_f>(EXEPTR);
    int retval = tested(&v[0], v.size(), &minpos, &maxpos);
    int rminpos = 0, rmaxpos = 0;
    for(size_t num = 0; num < v.size(); num++)
    {
        if(v[rminpos] > v[num])
            rminpos = v[num];
        if(v[rmaxpos] < v[num])
            rmaxpos = v[num];
    }
    TEST_EQ(rmaxpos, maxpos);
    TEST_EQ(rminpos, minpos);
    TEST_EQ(retval, 0);
});

LTEST(a_plus_b, {
    IReg a, b;
    CTX.startFunc(TESTNAME, {&a, &b});
    a+=b;
    CTX.return_(a);
    CTX.endFunc();
});
LTESTexe(a_plus_b, {
    typedef int64_t (*a_plus_b_f)(int64_t a, int64_t b);
    a_plus_b_f tested = reinterpret_cast<a_plus_b_f>(EXEPTR);
    std::vector<int64_t> A = {3,2,4,2,3,4,1};
    std::vector<int64_t> B = {4,4,5,5,4,6,5};
    for(size_t n = 0; n < A.size(); n++)
        TEST_EQ(tested(A[n],B[n]), A[n]+B[n]);
});

enum {NOT_A_TRIANGLE, RIGHT_TRIANGLE, EQUILATERAL_TRIANGLE, ISOSCELES_TRIANGLE, ACUTE_TRIANGLE, OBTUSE_TRIANGLE};
LTEST(triangle_types, {
    IReg a, b, c;
    CTX.startFunc(TESTNAME, {&a,&b,&c});
    IReg zer = CTX.const_(0);
    CTX.if_(a <= zer);
        CTX.return_(CTX.const_(NOT_A_TRIANGLE));
    CTX.elif_(b <= zer);
        CTX.return_(CTX.const_(NOT_A_TRIANGLE));
    CTX.elif_(c <= zer);
        CTX.return_(CTX.const_(NOT_A_TRIANGLE));
    CTX.else_();
        IReg apb = a + b;
        IReg apc = a + c;
        IReg bpc = b + c;
        CTX.if_(a > bpc);
            CTX.return_(CTX.const_(NOT_A_TRIANGLE));
        CTX.elif_(b > apc);
            CTX.return_(CTX.const_(NOT_A_TRIANGLE));
        CTX.elif_(c > apb);
            CTX.return_(CTX.const_(NOT_A_TRIANGLE));
        CTX.else_();
            CTX.if_(a == b);
                CTX.if_(b == c);
                    CTX.return_(CTX.const_(EQUILATERAL_TRIANGLE));
                CTX.else_();
                    CTX.return_(CTX.const_(ISOSCELES_TRIANGLE));
                CTX.endif_();
            CTX.elif_(a == c);
                CTX.return_(CTX.const_(ISOSCELES_TRIANGLE));
            CTX.elif_(b == c);
                CTX.return_(CTX.const_(ISOSCELES_TRIANGLE));
            CTX.endif_();

            IReg a2 = a*a;
            IReg b2 = b*b;
            IReg c2 = c*c;
            IReg a2pb2 = a2+b2;
            IReg a2pc2 = a2+c2;
            IReg b2pc2 = b2+c2;
            CTX.if_(a2 == b2pc2);
                CTX.return_(CTX.const_(RIGHT_TRIANGLE));
            CTX.elif_(b2 == a2pc2);
                CTX.return_(CTX.const_(RIGHT_TRIANGLE));
            CTX.elif_(c2 == a2pb2);
                CTX.return_(CTX.const_(RIGHT_TRIANGLE));
            CTX.endif_();

            IReg cosa = b2pc2 - a2;
            IReg cosb = a2pc2 - b2;
            IReg cosc = a2pb2 - c2;
            CTX.if_(cosa < zer);
                CTX.return_(CTX.const_(OBTUSE_TRIANGLE));
            CTX.elif_(cosb < zer);
                CTX.return_(CTX.const_(OBTUSE_TRIANGLE));
            CTX.elif_(cosc < zer);
                CTX.return_(CTX.const_(OBTUSE_TRIANGLE));
            CTX.endif_();

        CTX.endif_();
    CTX.endif_();
    CTX.return_(CTX.const_(ACUTE_TRIANGLE));
    CTX.endFunc();
});
LTESTexe(triangle_types, {
    typedef int (*triangle_types_f)(int64_t a, int64_t b, int64_t c);
    triangle_types_f tested = reinterpret_cast<triangle_types_f>(EXEPTR);
    TEST_EQ(tested(-1,2,3), int(NOT_A_TRIANGLE));
    TEST_EQ(tested(1,-2,3), int(NOT_A_TRIANGLE));
    TEST_EQ(tested(1,2,-3), int(NOT_A_TRIANGLE));
    TEST_EQ(tested(1,2,5), int(NOT_A_TRIANGLE));
    TEST_EQ(tested(5,2,1), int(NOT_A_TRIANGLE));
    TEST_EQ(tested(1,5,2), int(NOT_A_TRIANGLE));
    TEST_EQ(tested(1,1,1), int(EQUILATERAL_TRIANGLE));
    TEST_EQ(tested(1,1,2), int(ISOSCELES_TRIANGLE));
    TEST_EQ(tested(1,2,1), int(ISOSCELES_TRIANGLE));
    TEST_EQ(tested(2,1,1), int(ISOSCELES_TRIANGLE));
    TEST_EQ(tested(3,4,5), int(RIGHT_TRIANGLE));
    TEST_EQ(tested(5,4,3), int(RIGHT_TRIANGLE));
    TEST_EQ(tested(3,5,4), int(RIGHT_TRIANGLE));
    TEST_EQ(tested(5,6,9), int(OBTUSE_TRIANGLE));
    TEST_EQ(tested(9,5,6), int(OBTUSE_TRIANGLE));
    TEST_EQ(tested(5,9,6), int(OBTUSE_TRIANGLE));
    TEST_EQ(tested(7,5,6), int(ACUTE_TRIANGLE));
});

LTEST(nonnegative_odd, {
    IReg ptr, n, toFind;
    CTX.startFunc(TESTNAME, {&ptr, &n});
    IReg i = CTX.const_(0);
    IReg zer = CTX.const_(0);
    IReg two = CTX.const_(2);
    IReg elemsize = CTX.const_((int)sizeof(int));
    IReg res = i - elemsize; // it means (-4).
    n *= elemsize;
    CTX.doif_(i < n);
        IReg x = load_<int>(ptr, i);
        CTX.if_(x < zer);
            i += elemsize;
            CTX.continue_();
        CTX.endif_();
        IReg mod = x / two;
        mod *= two;
        mod = x - mod;
        CTX.if_(mod != zer);
            res = i;
            CTX.break_();
        CTX.endif_();
        i += elemsize;
    CTX.enddo_();
    res /= elemsize;
    CTX.return_(res);
    CTX.endFunc();
});
LTESTexe(nonnegative_odd, {
    typedef int (*nonnegative_odd_f)(const int* ptr, int64_t n);
    nonnegative_odd_f tested = reinterpret_cast<nonnegative_odd_f>(EXEPTR);
    std::vector<int> v = { 8, 2, -5, 7, 6 };
    TEST_EQ(tested(&v[0], v.size()), 3);
    TEST_EQ(tested(0, 0), -1);
});

#if defined(_WIN32) //TODO(ch): It must be about target processor, not operational system
LTEST(arithm_arrs, { //There we are testing stack parameter passing.
    IReg ptrA, ptrB, n, ptrAdd, ptrSub, ptrMul;
    CTX.startFunc(TESTNAME, { &ptrA, &ptrB, &n, &ptrAdd, &ptrSub, &ptrMul });
    IReg offset = CTX.const_(0);
    IReg i = CTX.const_(0);
    IReg elemsize = CTX.const_((int)sizeof(int));
    IReg one = CTX.const_(1);
    CTX.do_();
        IReg a = load_<int>(ptrA, offset);
        IReg b = load_<int>(ptrB, offset);
        store_<int>(ptrAdd, a + b);
        store_<int>(ptrSub, a - b);
        store_<int>(ptrMul, a * b);
        i += one;
        offset += elemsize;
        ptrAdd += elemsize;
        ptrSub += elemsize;
        ptrMul += elemsize;
    CTX.while_(i < n);
    CTX.return_(CTX.const_(0));
    CTX.endFunc();
});
LTESTexe(arithm_arrs, {
    typedef int (*arithm_arrs_f)(const int* ptrA, const int* ptrB, int64_t n, int* ptrAdd, int* ptrSub, int* ptrMul);
    arithm_arrs_f tested = reinterpret_cast<arithm_arrs_f>(EXEPTR);
    std::vector<int> A = { 8, 2, -5, 7, 6 };
    std::vector<int> B = { 2, -5, 7, 6, 8 };
    int addArr[5];
    int subArr[5];
    int mulArr[5];
    TEST_EQ(tested(&A[0], &B[0], A.size(), addArr, subArr, mulArr), 0);
    for (size_t n = 0; n < 5; n++)
    {
        TEST_EQ(A[n] + B[n], addArr[n]);
        TEST_EQ(A[n] - B[n], subArr[n]);
        TEST_EQ(A[n] * B[n], mulArr[n]);
    }
});
#elif defined(__APPLE__)
LTEST(ten_args_to_sum, { //There we are testing stack parameter passing.
    IReg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
    CTX.startFunc(TESTNAME, {&a0, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9});
    IReg res = a0 * CTX.const_(1);
    res += a1 * CTX.const_(2);
    res += a2 * CTX.const_(3);
    res += a3 * CTX.const_(4);
    res += a4 * CTX.const_(5);
    res += a5 * CTX.const_(6);
    res += a6 * CTX.const_(7);
    res += a7 * CTX.const_(8);
    res += a8 * CTX.const_(3);
    res += a9 * CTX.const_(2);
    CTX.return_(res);
    getImpl(&CTX)->getRegisterAllocator()->getRegisterPool().overrideRegisterSet(makeRegBasket({ 0, 1, 2, 3, 4, 5, 6, 7 }), makeRegBasket({ 0, 1, 2, 3, 4, 5, 6, 7 }), makeRegBasket({}), makeRegBasket({ 18, 19, 20, 21, 22 }));
    CTX.endFunc();
    getImpl(&CTX)->getRegisterAllocator()->getRegisterPool().overrideRegisterSet(0, 0, 0, 0);
    });
LTESTexe(ten_args_to_sum, {
    typedef int64_t (*ten_args_to_sum_f)(int64_t a0, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6, int64_t a7, int64_t a8, int64_t a9);
    ten_args_to_sum_f tested = reinterpret_cast<ten_args_to_sum_f>(EXEPTR);
    std::vector<int> v = { 1,1,1,1,1,1,1,1,3,5 };
    TEST_EQ(tested(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9]),(int64_t)(55));
});
#endif
};

#endif//__LOOPS_TEST_BASIC_HPP__
