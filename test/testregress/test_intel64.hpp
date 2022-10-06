/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_TEST_INTEL64_HPP__
#define __LOOPS_TEST_INTEL64_HPP__

#include "loops/loops.hpp"
#if __LOOPS_ARCH == __LOOPS_INTEL64
#include "loops/loops.hpp"
#include "../src/common.hpp"        //TODO(ch): .. in path is bad practice. Configure project
#include "../src/func_impl.hpp"     //TODO(ch): .. in path is bad practice. Configure project
#include "../src/reg_allocator.hpp" //TODO(ch): .. in path is bad practice. Configure project
#include "../tests.hpp"
#include <iostream>

namespace loops
{
    LTEST(arithm_arrs, { //There we are testing stack parameter passing.
        IReg ptrA, ptrB, n, ptrAdd, ptrSub, ptrMul, ptrDiv;
        STARTFUNC_(TESTNAME, &ptrA, &ptrB, &n, &ptrAdd, &ptrSub, &ptrMul, &ptrDiv )
        {
            if(OSname() == "Linux") 
                getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, { 7,6,2,1,8,9 }, { 0 }, {}, { 12, 13, 14, 15 });
            IReg offset = CONST_(0);
            IReg i = CONST_(0);
            WHILE_(i < n)
            {
                IReg a = load_<int>(ptrA, offset);
                IReg b = load_<int>(ptrB, offset);
                store_<int>(ptrAdd, a + b);
                store_<int>(ptrSub, a - b);
                store_<int>(ptrMul, a * b);
                store_<int>(ptrDiv, a / b);
                i += 1;
                offset += sizeof(int);
                ptrAdd += sizeof(int);
                ptrSub += sizeof(int);
                ptrMul += sizeof(int);
                ptrDiv += sizeof(int);
            };
            RETURN_(0);
        }
        });
    LTESTexe(arithm_arrs, {
        typedef int (*arithm_arrs_f)(const int* ptrA, const int* ptrB, int64_t n, int* ptrAdd, int* ptrSub, int* ptrMul, int* ptrDiv);
        arithm_arrs_f tested = reinterpret_cast<arithm_arrs_f>(EXEPTR);
        std::vector<int> A = { 8, 2, -5, 7, 6 };
        std::vector<int> B = { 2, -5, 7, 6, 8 };
        int addArr[5];
        int subArr[5];
        int mulArr[5];
        int divArr[5];
        EXPECT_EQ(tested(&A[0], &B[0], A.size(), addArr, subArr, mulArr, divArr), 0);
        for (size_t n = 0; n < 5; n++)
        {
            EXPECT_EQ(A[n] + B[n], addArr[n]);
            EXPECT_EQ(A[n] - B[n], subArr[n]);
            EXPECT_EQ(A[n] * B[n], mulArr[n]);
            EXPECT_EQ(A[n] / B[n], divArr[n]);
        }
        });

    LTESTcomposer(instruction_set_test, {
        Func * _f = getImpl(getImpl(&CTX)->getCurrentFunc());
        IReg rax = iregHid(0,_f);
        IReg rcx = iregHid(1, _f);
        IReg rdi = iregHid(7,_f);
        IReg r8  = iregHid(8, _f);
        IReg r12 = iregHid(12, _f);
        IReg r13 = iregHid(13, _f);
        IReg r15 = iregHid(15, _f);

        IReg eax = iregHid(0, _f);
        IReg edi = iregHid(7, _f);
        IReg r8d = iregHid(8, _f);
        IReg r12d = iregHid(12, _f);
        IReg r13d = iregHid(13, _f);

        IReg ax = iregHid(0, _f);
        IReg di = iregHid(7, _f);
        IReg r8w = iregHid(8, _f);
        IReg r12w = iregHid(12, _f);
        IReg r13w = iregHid(13, _f);

        IReg al = iregHid(0, _f);
        IReg cl = iregHid(1, _f);
        IReg dil = iregHid(7, _f);
        IReg r8b = iregHid(8, _f);
        IReg r12b = iregHid(12, _f);
        IReg r13b = iregHid(13, _f);

        store_<uint64_t>(rax, 256);
        store_<int64_t>(rax, 256);
        store_<int64_t>(rdi, 256);
        store_<int64_t>(r8,  256);
        store_<int64_t>(r12, 256);
        store_<int64_t>(r13, 256);

        store_<uint32_t>(rax, 256);
        store_<int32_t>(rax, 256);
        store_<int32_t>(rdi, 256);
        store_<int32_t>(r8,  256);
        store_<int32_t>(r12, 256);
        store_<int32_t>(r13, 256);

        store_<uint16_t>(rax, 256);
        store_<int16_t>(rax, 256);
        store_<int16_t>(rdi, 256);
        store_<int16_t>(r8, 256);
        store_<int16_t>(r12, 256);
        store_<int16_t>(r13, 256);

        store_<uint8_t>(rax, 255);
        store_<int8_t>(rax, 255);
        store_<int8_t>(rdi, 255);
        store_<int8_t>(r8, 255);
        store_<int8_t>(r12, 255);
        store_<int8_t>(r13, 255);

        // TODO(ch): Add here store_<uint32/64_t>(rax, ax) just for symmetry;

        store_<uint64_t>(rax, rax);
        store_<int64_t>(rax, rax);
        store_<int64_t>(rdi, rax);
        store_<int64_t>(rax, rdi);
        store_<int64_t>(r8, rax);
        store_<int64_t>(rax, r8);
        store_<int64_t>(r8, r8);
        store_<int64_t>(r12, rax);
        store_<int64_t>(r13, rax);
        store_<int64_t>(r12, r8);
        store_<int64_t>(r13, r8);

        store_<uint16_t>(rax, ax);
        store_<int16_t>(rax, ax);
        store_<int16_t>(rdi, ax);
        store_<int16_t>(rax, di);
        store_<int16_t>(rax, ax);
        store_<int16_t>(r8 , ax);
        store_<int16_t>(rax, r8w);
        store_<int16_t>(r8 , r8w);
        store_<int16_t>(r8 , ax);
        store_<int16_t>(r12, ax);
        store_<int16_t>(r12, di);
        store_<int16_t>(r13, ax);
        store_<int16_t>(r13, di);
        store_<int16_t>(r13, ax);
        store_<int16_t>(r8 , r8w);
        store_<int16_t>(r12, r8w);
        store_<int16_t>(r13, r8w);

        store_<uint8_t>(rax,al);
        store_<int8_t>(rax, al);
        store_<int8_t>(rdi, al);
        store_<int8_t>(rax, dil);
        store_<int8_t>(rax, al);
        store_<int8_t>(r8 , al);
        store_<int8_t>(rax, r8b );
        store_<int8_t>(r8 , r8b );
        store_<int8_t>(r12, al);
        store_<int8_t>(r12, dil);
        store_<int8_t>(r12, r8b );
        store_<int8_t>(r13, al);
        store_<int8_t>(r13, dil);
        store_<int8_t>(r13, r8b );

        store_<uint64_t>(rax, rax, rax);
        store_<int64_t>(rax, rax, rax);
        store_<int64_t>(rdi, rax, rax);
        store_<int64_t>(rax, rdi, rax);
        store_<int64_t>(rax, rax, rdi);
        store_<int64_t>(rax, rax, rax);
        store_<int64_t>(r8, rax, rax);
        store_<int64_t>(rax, r8, rax);
        store_<int64_t>(r8, r8, rax);
        store_<int64_t>(rax, rax, r8);
        store_<int64_t>(r8, rax, r8);
        store_<int64_t>(rax, r8, r8);
        store_<int64_t>(r8, r8, r8);

        store_<uint32_t>(rax, rax, eax);
        store_<int32_t>(rax, rax, eax);
        store_<int32_t>(rdi, rax, eax);
        store_<int32_t>(rax, rdi, eax);
        store_<int32_t>(rax, rax, edi);
        store_<int32_t>(rax, rax, eax);
        store_<int32_t>(r8, rax, eax);
        store_<int32_t>(rax, r8, eax);
        store_<int32_t>(r8, r8, eax);
        store_<int32_t>(rax, rax, r8d);
        store_<int32_t>(r8, rax, r8d);
        store_<int32_t>(rax, r8, r8d);
        store_<int32_t>(r8, r8, r8d);
        store_<int32_t>(r13, rax, eax);
        store_<int32_t>(r13, r8, rax);
        store_<int32_t>(r13, rax, r8d);
        store_<int32_t>(r13, r8, r8d);

        store_<uint16_t>(rax, rax, ax);
        store_<int16_t>(rax, rax, ax);
        store_<int16_t>(rdi, rax, ax);
        store_<int16_t>(rax, rdi, ax);
        store_<int16_t>(rax, rax, di);
        store_<int16_t>(rax, rax, ax);
        store_<int16_t>(r8, rax, ax);
        store_<int16_t>(rax, r8, ax);
        store_<int16_t>(r8, r8, ax);
        store_<int16_t>(rax, rax, r8w);
        store_<int16_t>(r8, rax, r8w);
        store_<int16_t>(rax, r8, r8w);
        store_<int16_t>(r8, r8, r8w);
        store_<int16_t>(r13, rax, ax);
        store_<int16_t>(r13, r8, ax);
        store_<int16_t>(r13, rax, r8w);
        store_<int16_t>(r13, r8, r8w);

        store_<uint8_t>(rax, rax, al);
        store_<int8_t>(rax, rax, al);
        store_<int8_t>(rdi, rax, al);
        store_<int8_t>(rax, rdi, al);
        store_<int8_t>(rax, rax, dil);
        store_<int8_t>(rax, rax, al);
        store_<int8_t>(r8, rax, al);
        store_<int8_t>(rax, r8, al);
        store_<int8_t>(r8, r8, al);
        store_<int8_t>(rax, rax, r8b);
        store_<int8_t>(r8, rax, r8b);
        store_<int8_t>(rax, r8, r8b);
        store_<int8_t>(r8, r8, r8b);
        store_<int8_t>(r13, rax, al);
        store_<int8_t>(r13, r8, al);
        store_<int8_t>(r13, rax, r8b);
        store_<int8_t>(r13, r8, r8b);
        store_<int8_t>(rax, rax, dil);
        store_<int8_t>(r8, rax, dil);
        store_<int8_t>(rax, r8, dil);
        store_<int8_t>(r8, r8, dil);

        store_<uint64_t>(rax, 256, rax);
        store_<int64_t>(rax, 256, rax);
        store_<int64_t>(rdi, 256, rax);
        store_<int64_t>(rax, 256, rdi);
        store_<int64_t>(rax, 256, rax);
        store_<int64_t>(r8, 256, rax);
        store_<int64_t>(rax, 256, r8);
        store_<int64_t>(r8, 256, r8);
        store_<int64_t>(r12, 256, rax);
        store_<int64_t>(r12, 256, rdi);
        store_<int64_t>(r12, 256, r8);

        store_<uint32_t>(rax, 256, eax);
        store_<int32_t>(rax, 256, eax);
        store_<int32_t>(rdi, 256, eax);
        store_<int32_t>(rax, 256, edi);
        store_<int32_t>(rax, 256, eax);
        store_<int32_t>(r8, 256, eax);
        store_<int32_t>(rax, 256, r8d);
        store_<int32_t>(r8, 256, r8d);
        store_<int32_t>(r12, 256, eax);
        store_<int32_t>(r12, 256, r8d);

        store_<uint16_t>(rax, 256, ax);
        store_<int16_t>(rax, 256, ax);
        store_<int16_t>(rdi, 256, ax);
        store_<int16_t>(rax, 256, di);
        store_<int16_t>(rax, 256, ax);
        store_<int16_t>(r8, 256, ax);
        store_<int16_t>(rax, 256, r8w);
        store_<int16_t>(r8, 256, r8w);
        store_<int16_t>(r12, 256, ax);
        store_<int16_t>(r12, 256, r8w);

        store_<uint8_t>(rax, 256, al);
        store_<int8_t>(rax, 256, al);
        store_<int8_t>(rdi, 256, al);
        store_<int8_t>(rax, 256, dil);
        store_<int8_t>(rax, 256, al);
        store_<int8_t>(r8, 256, al);
        store_<int8_t>(rax, 256, r8b);
        store_<int8_t>(r8, 256, r8b);
        store_<int8_t>(r12, 256, al);
        store_<int8_t>(r12, 256, r8b);

        store_<uint64_t>(rax, rax, 256);
        store_<int64_t>(rax, rax, 256);
        store_<int64_t>(rdi, rax, 256);
        store_<int64_t>(rax, rdi, 256);
        store_<int64_t>(rax, rax, 256);
        store_<int64_t>(r8, rax, 256);
        store_<int64_t>(rax, r8, 256);
        store_<int64_t>(r8, r8, 256);
        store_<int64_t>(r13, rax, 256);
        store_<int64_t>(r13, r8, 256);

        store_<uint32_t>(rax, rax, 256);
        store_<int32_t>(rax, rax, 256);
        store_<int32_t>(rdi, rax, 256);
        store_<int32_t>(rax, rdi, 256);
        store_<int32_t>(rax, rax, 256);
        store_<int32_t>(r8, rax, 256);
        store_<int32_t>(rax, r8, 256);
        store_<int32_t>(r8, r8, 256);
        store_<int32_t>(r13, rax, 256);
        store_<int32_t>(r13, r8, 256);

        store_<uint16_t>(rax, rax, 256);
        store_<int16_t>(rax, rax, 256);
        store_<int16_t>(rdi, rax, 256);
        store_<int16_t>(rax, rdi, 256);
        store_<int16_t>(rax, rax, 256);
        store_<int16_t>(r8, rax, 256);
        store_<int16_t>(rax, r8, 256);
        store_<int16_t>(r8, r8, 256);
        store_<int16_t>(r13, rax, 256);
        store_<int16_t>(r13, r8, 256);

        store_<uint8_t>(rax, rax, 255);
        store_<int8_t>(rax, rax, 255);
        store_<int8_t>(rdi, rax, 255);
        store_<int8_t>(rax, rdi, 255);
        store_<int8_t>(rax, rax, 255);
        store_<int8_t>(r8, rax, 255);
        store_<int8_t>(rax, r8, 255);
        store_<int8_t>(r8, r8, 255);
        store_<int8_t>(r13, rax, 255);
        store_<int8_t>(r13, r8, 255);

        store_<uint64_t>(rax, 257, 256);
        store_<int64_t>(rax, 257, 256);
        store_<int64_t>(rdi, 257, 256);
        store_<int64_t>(rax, 257, 256);
        store_<int64_t>(r8, 257, 256);
        store_<int64_t>(r12, 257, 256);

        store_<uint32_t>(rax, 257, 256);
        store_<int32_t>(rax, 257, 256);
        store_<int32_t>(rdi, 257, 256);
        store_<int32_t>(r8, 257, 256);
        store_<int32_t>(r12, 257, 256);

        store_<uint16_t>(rax, 257, 256);
        store_<int16_t>(rax, 257, 256);
        store_<int16_t>(rdi, 257, 256);
        store_<int16_t>(r8, 257, 256);
        store_<int16_t>(r12, 257, 256);

        store_<uint8_t>(rax, 257, 255);
        store_<int8_t>(rax, 257, 255);
        store_<int8_t>(rdi, 257, 255);
        store_<int8_t>(r8, 257, 255);
        store_<int8_t>(r12, 257, 255);

        load2<uint64_t>(rax, rax);
        load2<int64_t>(rax, rax);
        load2<int64_t>(rdi, rax);
        load2<int64_t>(rax, rdi);
        load2<int64_t>(rax, rax);
        load2<int64_t>(r8, rax);
        load2<int64_t>(rax, r8);
        load2<int64_t>(r8, r8);

        load2<uint32_t>(eax, rax);
        load2<uint32_t>(edi, rax);
        load2<uint32_t>(eax, rdi);
        load2<uint32_t>(eax, rax);
        load2<uint32_t>(r8d, rax);
        load2<uint32_t>(eax, r8);
        load2<uint32_t>(r8d, r8);

        load2<uint16_t>(ax, rax);
        load2<uint16_t>(di, rax);
        load2<uint16_t>(ax, rdi);
        load2<uint16_t>(r8w, rax);
        load2<uint16_t>(ax, r8);
        load2<uint16_t>(r8w, r8);

        load2<int16_t>(ax, rax);
        load2<int16_t>(di, rax);
        load2<int16_t>(ax, rdi);
        load2<int16_t>(r8w, rax);
        load2<int16_t>(rax, r8);
        load2<int16_t>(r8w, r8);

        load2<uint8_t>(al, rax);
        load2<uint8_t>(dil, rax);
        load2<uint8_t>(al, rdi);
        load2<uint8_t>(r8b, rax);
        load2<uint8_t>(al, r8);
        load2<uint8_t>(r8b, r8);

        load2<int8_t>(al, rax);
        load2<int8_t>(dil, rax);
        load2<int8_t>(al, rdi);
        load2<int8_t>(r8b, rax);
        load2<int8_t>(al, r8);
        load2<int8_t>(r8b, r8);

        load2<uint64_t>(rax, rax, 256);
        load2<int64_t>(rax, rax, 256);
        load2<int64_t>(rdi, rax, 256);
        load2<int64_t>(rax, rdi, 256);
        load2<int64_t>(rax, rax, 256);
        load2<int64_t>(r8, rax, 256);
        load2<int64_t>(rax, r8, 256);
        load2<int64_t>(r8, r8, 256);

        load2<uint32_t>(eax, rax, 256);
        load2<uint32_t>(edi, rax, 256);
        load2<uint32_t>(eax, rdi, 256);
        load2<uint32_t>(r8d, rax, 256);
        load2<uint32_t>(eax, r8, 256);
        load2<uint32_t>(r8d, r8, 256);

        load2<int32_t>(eax, rax, 256);
        load2<int32_t>(edi, rax, 256);
        load2<int32_t>(eax, rdi, 256);
        load2<int32_t>(r8d, rax, 256);
        load2<int32_t>(eax, r8, 256);
        load2<int32_t>(r8d, r8, 256);

        load2<uint16_t>(ax, rax, 256);
        load2<uint16_t>(di, rax, 256);
        load2<uint16_t>(ax, rdi, 256);
        load2<uint16_t>(r8w, rax, 256);
        load2<uint16_t>(ax, r8, 256);
        load2<uint16_t>(r8w, r8, 256);

        load2<int16_t>(ax, rax, 256);
        load2<int16_t>(di, rax, 256);
        load2<int16_t>(ax, rdi, 256);
        load2<int16_t>(r8w, rax, 256);
        load2<int16_t>(ax, r8, 256);
        load2<int16_t>(r8w, r8, 256);

        load2<uint8_t>(al, rax, 256);
        load2<uint8_t>(dil, rax, 256);
        load2<uint8_t>(al, rdi, 256);
        load2<uint8_t>(r8b, rax, 256);
        load2<uint8_t>(al, r8, 256);
        load2<uint8_t>(r8b, r8, 256);

        load2<int8_t>(al, rax, 256);
        load2<int8_t>(dil, rax, 256);
        load2<int8_t>(al, rdi, 256);
        load2<int8_t>(r8b, rax, 256);
        load2<int8_t>(al, r8, 256);
        load2<int8_t>(r8b, r8, 256);

        load2<uint64_t>(rax, rax, rax);
        load2<int64_t>(rax, rax, rax);
        load2<int64_t>(rdi, rax, rax);
        load2<int64_t>(rax, rdi, rax);
        load2<int64_t>(rax, rax, rdi);
        load2<int64_t>(rax, rax, rax);
        load2<int64_t>(r8, rax, rax);
        load2<int64_t>(rax, r8, rax);
        load2<int64_t>(r8, r8, rax);
        load2<int64_t>(rax, rax, r8);
        load2<int64_t>(r8, rax, r8);
        load2<int64_t>(rax, r8, r8);
        load2<int64_t>(r8, r8, r8);

        load2<uint32_t>(eax, rax, rax);
        load2<uint32_t>(edi, rax, rax);
        load2<uint32_t>(eax, rdi, rax);
        load2<uint32_t>(eax, rax, rdi);
        load2<uint32_t>(r8d, rax, rax);
        load2<uint32_t>(eax, r8, rax);
        load2<uint32_t>(r8d, r8, rax);
        load2<uint32_t>(eax, rax, r8);
        load2<uint32_t>(r8d, rax, r8);
        load2<uint32_t>(eax, r8, r8);
        load2<uint32_t>(r8d, r8, r8);

        load2<uint16_t>(ax, rax, rax);
        load2<uint16_t>(di, rax, rax);
        load2<uint16_t>(ax, rdi, rax);
        load2<uint16_t>(ax, rax, rdi);
        load2<uint16_t>(r8w, rax, rax);
        load2<uint16_t>(ax, r8, rax);
        load2<uint16_t>(r8w, r8, rax);
        load2<uint16_t>(ax, rax, r8);
        load2<uint16_t>(r8w, rax, r8);
        load2<uint16_t>(ax, r8, r8);
        load2<uint16_t>(r8w, r8, r8);

        load2<int16_t>(ax, rax, rax);
        load2<int16_t>(di, rax, rax);
        load2<int16_t>(ax, rdi, rax);
        load2<int16_t>(ax, rax, rdi);
        load2<int16_t>(r8w, rax, rax);
        load2<int16_t>(ax, r8, rax);
        load2<int16_t>(r8w, r8, rax);
        load2<int16_t>(ax, rax, r8);
        load2<int16_t>(r8w, rax, r8);
        load2<int16_t>(ax, r8, r8);
        load2<int16_t>(r8w, r8, r8);

        load2<uint8_t>(al, rax, rax);
        load2<uint8_t>(dil, rax, rax);
        load2<uint8_t>(al, rdi, rax);
        load2<uint8_t>(al, rax, rdi);
        load2<uint8_t>(r8b, rax, rax);
        load2<uint8_t>(al, r8, rax);
        load2<uint8_t>(r8b, r8, rax);
        load2<uint8_t>(al, rax, r8);
        load2<uint8_t>(r8b, rax, r8);
        load2<uint8_t>(al, r8, r8);
        load2<uint8_t>(r8b, r8, r8);

        load2<int8_t>(al, rax, rax);
        load2<int8_t>(dil, rax, rax);
        load2<int8_t>(al, rdi, rax);
        load2<int8_t>(al, rax, rdi);
        load2<int8_t>(r8b, rax, rax);
        load2<int8_t>(al, r8, rax);
        load2<int8_t>(r8b, r8, rax);
        load2<int8_t>(al, rax, r8);
        load2<int8_t>(r8b, rax, r8);
        load2<int8_t>(al, r8, r8);
        load2<int8_t>(r8b, r8, r8);

        newiopNoret(OP_XCHG, { rax, rax });
        newiopNoret(OP_XCHG, { rax, rdi });
        newiopNoret(OP_XCHG, { rax, r8 });

        newiopNoret(OP_XCHG, { rcx, rcx });
        newiopNoret(OP_XCHG, { rdi, rcx });
        newiopNoret(OP_XCHG, { rcx, rdi });
        newiopNoret(OP_XCHG, { rcx, rcx });
        newiopNoret(OP_XCHG, { r8 , rcx });
        newiopNoret(OP_XCHG, { rcx,  r8 });
        newiopNoret(OP_XCHG, { r8 ,  r8 });

        newiopNoret(OP_XCHG, { rcx,  argSpilled(RB_INT, 32, _f) });
        newiopNoret(OP_XCHG, { rdi,  argSpilled(RB_INT, 32, _f) });
        newiopNoret(OP_XCHG, { r8 ,  argSpilled(RB_INT, 32, _f) });
        newiopNoret(OP_XCHG, { r15,  argSpilled(RB_INT, 32, _f) });

        newiopNoret(OP_XCHG, { argSpilled(RB_INT, 32, _f), rcx });
        newiopNoret(OP_XCHG, { argSpilled(RB_INT, 32, _f), rdi });
        newiopNoret(OP_XCHG, { argSpilled(RB_INT, 32, _f), r8 });
        newiopNoret(OP_XCHG, { argSpilled(RB_INT, 32, _f), r15 });

        newiopNoret(OP_SHL, { rax, rax, argIImm(15, _f) });
        newiopNoret(OP_SHL, { rdi, rdi, argIImm(15, _f) });
        newiopNoret(OP_SHL, { r8 , r8, argIImm(15, _f) });
        newiopNoret(OP_SHL, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), argIImm(15, _f) });

        newiopNoret(OP_SHL, { rax, rax, cl });
        newiopNoret(OP_SHL, { rdi, rdi, cl });
        newiopNoret(OP_SHL, { r8, r8, cl });
        newiopNoret(OP_SHL, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), cl });

        newiopNoret(OP_SHR, { rax,rax, argIImm(15, _f) });
        newiopNoret(OP_SHR, { rdi,rdi, argIImm(15, _f) });
        newiopNoret(OP_SHR, { r8 , r8, argIImm(15, _f) });
        newiopNoret(OP_SHR, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), argIImm(15, _f) });

        newiopNoret(OP_SHR, { rax, rax, cl });
        newiopNoret(OP_SHR, { rdi, rdi, cl });
        newiopNoret(OP_SHR, { r8, r8, cl });
        newiopNoret(OP_SHR, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), cl });

        newiopNoret(OP_SAR, { rax,rax, argIImm(15, _f) });
        newiopNoret(OP_SAR, { rdi,rdi, argIImm(15, _f) });
        newiopNoret(OP_SAR, { r8 , r8, argIImm(15, _f) });
        newiopNoret(OP_SAR, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), argIImm(15, _f) });

        newiopNoret(OP_SAR, { rax, rax, cl });
        newiopNoret(OP_SAR, { rdi, rdi, cl });
        newiopNoret(OP_SAR, { r8, r8, cl });
        newiopNoret(OP_SAR, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), cl });

        newiopNoret(OP_AND, { rax, rax, rax });
        newiopNoret(OP_AND, { rax, rax, rdi });
        newiopNoret(OP_AND, { rdi, rdi, rax });
        newiopNoret(OP_AND, { rax, rax, r8 });
        newiopNoret(OP_AND, { r8 , r8 , rax });
        newiopNoret(OP_AND, { r8 , r8 , r8 });

        newiopNoret(OP_AND, { rax, rax, argSpilled(RB_INT, 32, _f) });
        newiopNoret(OP_AND, { rdi, rdi, argSpilled(RB_INT, 32, _f) });
        newiopNoret(OP_AND, { r8 , r8 , argSpilled(RB_INT, 32, _f) });

        newiopNoret(OP_AND, { rax, rax, argIImm(256, _f) });
        newiopNoret(OP_AND, { rcx, rcx, argIImm(256, _f) });
        newiopNoret(OP_AND, { rdi, rdi, argIImm(256, _f) });
        newiopNoret(OP_AND, { r8 , r8 , argIImm(256, _f) });

        newiopNoret(OP_AND, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), rax });
        newiopNoret(OP_AND, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), rdi });
        newiopNoret(OP_AND, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), r8 });

        newiopNoret(OP_AND, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), argIImm(257, _f) });

        newiopNoret(OP_OR, { rax, rax, rax });
        newiopNoret(OP_OR, { rax, rax, rdi });
        newiopNoret(OP_OR, { rdi, rdi, rax });
        newiopNoret(OP_OR, { rax, rax, r8 });
        newiopNoret(OP_OR, { r8 , r8 , rax });

        newiopNoret(OP_OR, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), rax });
        newiopNoret(OP_OR, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), r8 });

        newiopNoret(OP_OR, { rax, rax, argSpilled(RB_INT, 32, _f) });
        newiopNoret(OP_OR, { r8, r8, argSpilled(RB_INT, 32, _f) });

        newiopNoret(OP_OR, { rax, rax, argIImm(256, _f) });
        newiopNoret(OP_OR, { r8, r8, argIImm(256, _f) });
        newiopNoret(OP_OR, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), argIImm(257, _f) });

        newiopNoret(OP_XOR, { rax, rax, rax });
        newiopNoret(OP_XOR, { rax, rax, rdi });
        newiopNoret(OP_XOR, { rdi, rdi, rax });
        newiopNoret(OP_XOR, { rax, rax, r8 });
        newiopNoret(OP_XOR, { r8 , r8 , rax });
        newiopNoret(OP_XOR, { r8 , r8 , r8 });

        newiopNoret(OP_XOR, { rax, rax, argSpilled(RB_INT, 32, _f) });
        newiopNoret(OP_XOR, { rdi, rdi, argSpilled(RB_INT, 32, _f) });
        newiopNoret(OP_XOR, { r8 , r8 , argSpilled(RB_INT, 32, _f) });

        newiopNoret(OP_XOR, { rax, rax, argIImm(256, _f) });
        newiopNoret(OP_XOR, { rcx, rcx, argIImm(256, _f) });
        newiopNoret(OP_XOR, { rdi, rdi, argIImm(256, _f) });
        newiopNoret(OP_XOR, { r8 , r8 , argIImm(256, _f) });

        newiopNoret(OP_XOR, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), rax });
        newiopNoret(OP_XOR, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), rdi });
        newiopNoret(OP_XOR, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), r8 });

        newiopNoret(OP_XOR, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), argIImm(257, _f) });

        newiopNoret(OP_SELECT, { rax, IC_EQ, rax, rax });
        newiopNoret(OP_SELECT, { rax, IC_NE, rax, rax });
        newiopNoret(OP_SELECT, { rax, IC_LT, rax, rax });
        newiopNoret(OP_SELECT, { rax, IC_GT, rax, rax });
        newiopNoret(OP_SELECT, { rax, IC_LE, rax, rax });
        newiopNoret(OP_SELECT, { rax, IC_GE, rax, rax });
        newiopNoret(OP_SELECT, { rax, IC_S, rax, rax });
        newiopNoret(OP_SELECT, { rax, IC_NS, rax, rax });

        newiopNoret(OP_SELECT, { rdi, IC_EQ, rax, rdi });
        newiopNoret(OP_SELECT, { rax, IC_EQ, rdi, rax });

        newiopNoret(OP_SELECT, { r8 , IC_EQ, rax, r8 });
        newiopNoret(OP_SELECT, { rax, IC_EQ, r8 , rax });
        newiopNoret(OP_SELECT, { r8 , IC_EQ, r8 , r8 });

        newiopNoret(OP_SELECT, { rax, IC_EQ, argSpilled(RB_INT, 32, _f), rax });
        newiopNoret(OP_SELECT, { rdi, IC_EQ, argSpilled(RB_INT, 32, _f), rdi });
        newiopNoret(OP_SELECT, { r8,  IC_EQ, argSpilled(RB_INT, 32, _f), r8 });

        newiopNoret(OP_X86_ADC, { rax, rax, rax });
        newiopNoret(OP_X86_ADC, { rdi, rdi, rax });
        newiopNoret(OP_X86_ADC, { rax, rax, rdi });
        newiopNoret(OP_X86_ADC, { r8 , r8 , rax });
        newiopNoret(OP_X86_ADC, { rax, rax, r8 });
        newiopNoret(OP_X86_ADC, { r8 , r8 , r8 });

        newiopNoret(OP_X86_ADC, { rax, rax, argIImm(256, _f) });
        newiopNoret(OP_X86_ADC, { rcx, rcx, argIImm(256, _f) });
        newiopNoret(OP_X86_ADC, { rdi, rdi, argIImm(256, _f) });
        newiopNoret(OP_X86_ADC, { r8 , r8 , argIImm(256, _f) });
        newiopNoret(OP_X86_ADC, { rax, rax, argSpilled(RB_INT, 32, _f) });
        newiopNoret(OP_X86_ADC, { rdi, rdi, argSpilled(RB_INT, 32, _f) });
        newiopNoret(OP_X86_ADC, { r8 , r8 , argSpilled(RB_INT, 32, _f) });
        
        newiopNoret(OP_X86_ADC, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), rax });
        newiopNoret(OP_X86_ADC, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), rdi });
        newiopNoret(OP_X86_ADC, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), r8 });
        newiopNoret(OP_X86_ADC, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), argIImm(256, _f) });
        });

};
#endif//__LOOPS_ARCH == __LOOPS_INTEL64
#endif//__LOOPS_TEST_INTEL64_HPP__
