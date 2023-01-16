/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_TEST_INTEL64_HPP__
#define __LOOPS_TEST_INTEL64_HPP__

#include "loops/loops.hpp"
#if __LOOPS_ARCH == __LOOPS_INTEL64
#include "loops/loops.hpp"
#include "src/common.hpp"
#include "src/func_impl.hpp"
#include "src/reg_allocator.hpp"
#include "tests.hpp"
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

        newiopNoret(OP_STORE, { argIImm(TYPE_U64, _f), rax, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rdi, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r8, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r12, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r13, argIImm(256, _f) });

        newiopNoret(OP_STORE, { argIImm(TYPE_U32, _f), rax, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rdi, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r8, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r12, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r13, argIImm(256, _f) });

        newiopNoret(OP_STORE, { argIImm(TYPE_U16, _f), rax, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rdi, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r8, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r12, argIImm(256, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r13, argIImm(256, _f) });

        newiopNoret(OP_STORE, { argIImm(TYPE_U8, _f), rax, argIImm(255, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, argIImm(255, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rdi, argIImm(255, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8, argIImm(255, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r12, argIImm(255, _f) });
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r13, argIImm(255, _f) });

        newiopNoret(OP_STORE, { argIImm(TYPE_U64, _f), rax, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, rdi});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, r8});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rdi, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r8, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r8, r8});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r12, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r12, rdi});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r12, r8});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r13, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r13, rdi});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r13, r8});
        
        // TODO(ch): Add here store_<uint32/int32>(rax, ax) just for symmetry;

        newiopNoret(OP_STORE, { argIImm(TYPE_U16, _f), rax, ax });
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, di});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, r8w});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rdi, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r8 , ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r8 , r8w});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r12, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r12, di});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r12, r8w});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r13, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r13, di});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r13, r8w});

        newiopNoret(OP_STORE, { argIImm(TYPE_U8, _f), rax, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, dil});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, r8b });
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rdi, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8 , al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8 , r8b });
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r12, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r12, dil});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r12, r8b });
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r13, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r13, dil});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r13, r8b });

        newiopNoret(OP_STORE, { argIImm(TYPE_U64, _f), rax, rax, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, rax, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, rax, rdi});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, rax, r8});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, rdi, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, r8, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, r8, r8});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rdi, rax, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r8, rax, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r8, rax, r8});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r8, r8, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r8, r8, r8});

        newiopNoret(OP_STORE, { argIImm(TYPE_U32, _f), rax, rax, eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, rax, eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, rax, edi});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, rax, r8d});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, rdi, eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, r8, eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, r8, r8d});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rdi, rax, eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r8, rax, eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r8, rax, r8d});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r8, r8, eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r8, r8, r8d});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r13, rax, eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r13, rax, r8d});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r13, r8, rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r13, r8, r8d});

        newiopNoret(OP_STORE, { argIImm(TYPE_U16, _f), rax, rax, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, rax, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, rax, di});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, rax, r8w});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, rdi, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, r8, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, r8, r8w});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rdi, rax, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r8, rax, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r8, rax, r8w});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r8, r8, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r8, r8, r8w});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r13, rax, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r13, r8, ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r13, rax, r8w});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r13, r8, r8w});

        newiopNoret(OP_STORE, { argIImm(TYPE_U8, _f), rax, rax, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, rax, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, rax, dil});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, rax, r8b});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, rdi, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, r8, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, r8, dil});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, r8, r8b});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rdi, rax, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8, rax, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8, rax, dil});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8, rax, r8b});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8, r8, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8, r8, r8b});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8, r8, dil});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r13, rax, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r13, rax, r8b});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r13, r8, al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r13, r8, r8b});

        newiopNoret(OP_STORE, { argIImm(TYPE_U64, _f), rax, argIImm(256, _f), rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, argIImm(256, _f), rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, argIImm(256, _f), rdi});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, argIImm(256, _f), r8});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rdi, argIImm(256, _f), rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r8, argIImm(256, _f), rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r8, argIImm(256, _f), r8});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r12, argIImm(256, _f), rax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r12, argIImm(256, _f), rdi});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r12, argIImm(256, _f), r8});

        newiopNoret(OP_STORE, { argIImm(TYPE_U32, _f), rax, argIImm(256, _f), eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, argIImm(256, _f), eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, argIImm(256, _f), edi});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, argIImm(256, _f), r8d});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rdi, argIImm(256, _f), eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r8, argIImm(256, _f), eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r8, argIImm(256, _f), r8d});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r12, argIImm(256, _f), eax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r12, argIImm(256, _f), r8d});

        newiopNoret(OP_STORE, { argIImm(TYPE_U16, _f), rax, argIImm(256, _f), ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, argIImm(256, _f), ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, argIImm(256, _f), di});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, argIImm(256, _f), r8w});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rdi, argIImm(256, _f), ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r8, argIImm(256, _f), ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r8, argIImm(256, _f), r8w});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r12, argIImm(256, _f), ax});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r12, argIImm(256, _f), r8w});

        newiopNoret(OP_STORE, { argIImm(TYPE_U8, _f), rax, argIImm(256, _f), al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, argIImm(256, _f), al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, argIImm(256, _f), dil});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, argIImm(256, _f), r8b});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rdi, argIImm(256, _f), al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8, argIImm(256, _f), al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8, argIImm(256, _f), r8b});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r12, argIImm(256, _f), al});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r12, argIImm(256, _f), r8b});

        newiopNoret(OP_STORE, { argIImm(TYPE_U64, _f), rax, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, rdi, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, r8, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rdi, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r8, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r8, r8, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r13, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r13, r8, argIImm(256, _f)});

        newiopNoret(OP_STORE, { argIImm(TYPE_U32, _f), rax, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, rdi, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, r8, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rdi, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r8, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r8, r8, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r13, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r13, r8, argIImm(256, _f)});

        newiopNoret(OP_STORE, { argIImm(TYPE_U16, _f), rax, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, rdi, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, r8, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rdi, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r8, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r8, r8, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r13, rax, argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r13, r8, argIImm(256, _f)});

        newiopNoret(OP_STORE, { argIImm(TYPE_U8, _f), rax, rax, argIImm(255, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, rax, argIImm(255, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, rdi, argIImm(255, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, r8, argIImm(255, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rdi, rax, argIImm(255, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8, rax, argIImm(255, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8, r8, argIImm(255, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r13, rax, argIImm(255, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r13, r8, argIImm(255, _f)});

        newiopNoret(OP_STORE, { argIImm(TYPE_U64, _f), rax, argIImm(257, _f), argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rax, argIImm(257, _f), argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), rdi, argIImm(257, _f), argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r8, argIImm(257, _f), argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I64, _f), r12, argIImm(257, _f), argIImm(256, _f)});

        newiopNoret(OP_STORE, { argIImm(TYPE_U32, _f), rax, argIImm(257, _f), argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rax, argIImm(257, _f), argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), rdi, argIImm(257, _f), argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r8, argIImm(257, _f), argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I32, _f), r12, argIImm(257, _f), argIImm(256, _f)});

        newiopNoret(OP_STORE, { argIImm(TYPE_U16, _f), rax, argIImm(257, _f), argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rax, argIImm(257, _f), argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), rdi, argIImm(257, _f), argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r8, argIImm(257, _f), argIImm(256, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I16, _f), r12, argIImm(257, _f), argIImm(256, _f)});

        newiopNoret(OP_STORE, { argIImm(TYPE_U8, _f), rax, argIImm(257, _f), argIImm(255, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rax, argIImm(257, _f), argIImm(255, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), rdi, argIImm(257, _f), argIImm(255, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r8, argIImm(257, _f), argIImm(255, _f)});
        newiopNoret(OP_STORE, { argIImm(TYPE_I8, _f), r12, argIImm(257, _f), argIImm(255, _f)});

        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_U64, _f), rax });
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), rax });
        newiopNoret(OP_LOAD, { rdi, argIImm(TYPE_I64, _f), rax });
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), rdi });
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), rax });
        newiopNoret(OP_LOAD, { r8 , argIImm(TYPE_I64, _f), rax });
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), r8  });
        newiopNoret(OP_LOAD, { r8 , argIImm(TYPE_I64, _f), r8  });
                                              
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), rax });
        newiopNoret(OP_LOAD, { edi, argIImm(TYPE_U32, _f), rax });
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), rdi });
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), rax });
        newiopNoret(OP_LOAD, { r8d, argIImm(TYPE_U32, _f), rax });
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), r8  });
        newiopNoret(OP_LOAD, { r8d, argIImm(TYPE_U32, _f), r8  });

        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_U16, _f), rax });
        newiopNoret(OP_LOAD, { di , argIImm(TYPE_U16, _f), rax });
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_U16, _f), rdi });
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_U16, _f), rax });
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_U16, _f), r8  });
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_U16, _f), r8  });

        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_I16, _f), rax });
        newiopNoret(OP_LOAD, { di , argIImm(TYPE_I16, _f), rax });
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_I16, _f), rdi });
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_I16, _f), rax });
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I16, _f), r8  });
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_I16, _f), r8  });

        newiopNoret(OP_LOAD, { al , argIImm(TYPE_U8, _f), rax});
        newiopNoret(OP_LOAD, { dil, argIImm(TYPE_U8, _f), rax});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_U8, _f), rdi});
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_U8, _f), rax});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_U8, _f), r8 });
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_U8, _f), r8 });

        newiopNoret(OP_LOAD, { al , argIImm(TYPE_I8, _f), rax});
        newiopNoret(OP_LOAD, { dil, argIImm(TYPE_I8, _f), rax});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_I8, _f), rdi});
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_I8, _f), rax});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_I8, _f), r8 });
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_I8, _f), r8 });

        newiopNoret(OP_LOAD, { rax , argIImm(TYPE_U64, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { rdi, argIImm(TYPE_I64, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), rdi, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8 , argIImm(TYPE_I64, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), r8 , argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8 , argIImm(TYPE_I64, _f), r8 , argIImm(256, _f)});

        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { edi, argIImm(TYPE_U32, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), rdi, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8d, argIImm(TYPE_U32, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), r8 , argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8d, argIImm(TYPE_U32, _f), r8 , argIImm(256, _f)});

        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_I32, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { edi, argIImm(TYPE_I32, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_I32, _f), rdi, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8d, argIImm(TYPE_I32, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_I32, _f), r8 , argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8d, argIImm(TYPE_I32, _f), r8 , argIImm(256, _f)});

        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_U16, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { di , argIImm(TYPE_U16, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_U16, _f), rdi, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_U16, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_U16, _f), r8 , argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_U16, _f), r8 , argIImm(256, _f)});

        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_I16, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { di , argIImm(TYPE_I16, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_I16, _f), rdi, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_I16, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_I16, _f), r8 , argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_I16, _f), r8 , argIImm(256, _f)});

        newiopNoret(OP_LOAD, { al , argIImm(TYPE_U8, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { dil, argIImm(TYPE_U8, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_U8, _f), rdi, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_U8, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_U8, _f), r8 , argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_U8, _f), r8 , argIImm(256, _f)});

        newiopNoret(OP_LOAD, { al , argIImm(TYPE_I8, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { dil, argIImm(TYPE_I8, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_I8, _f), rdi, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_I8, _f), rax, argIImm(256, _f)});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_I8, _f), r8 , argIImm(256, _f)});
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_I8, _f), r8 , argIImm(256, _f)});

        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_U64, _f), rax, rax});
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), rax, rax});
        newiopNoret(OP_LOAD, { rdi, argIImm(TYPE_I64, _f), rax, rax});
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), rdi, rax});
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), rax, rdi});
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), rax, rax});
        newiopNoret(OP_LOAD, { r8 , argIImm(TYPE_I64, _f), rax, rax});
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), r8 , rax});
        newiopNoret(OP_LOAD, { r8 , argIImm(TYPE_I64, _f), r8 , rax});
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), rax, r8 });
        newiopNoret(OP_LOAD, { r8 , argIImm(TYPE_I64, _f), rax, r8 });
        newiopNoret(OP_LOAD, { rax, argIImm(TYPE_I64, _f), r8 , r8 });
        newiopNoret(OP_LOAD, { r8 , argIImm(TYPE_I64, _f), r8 , r8 });

        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), rax, rax});
        newiopNoret(OP_LOAD, { edi, argIImm(TYPE_U32, _f), rax, rax});
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), rdi, rax});
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), rax, rdi});
        newiopNoret(OP_LOAD, { r8d, argIImm(TYPE_U32, _f), rax, rax});
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), r8 , rax});
        newiopNoret(OP_LOAD, { r8d, argIImm(TYPE_U32, _f), r8 , rax});
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), rax, r8 });
        newiopNoret(OP_LOAD, { r8d, argIImm(TYPE_U32, _f), rax, r8 });
        newiopNoret(OP_LOAD, { eax, argIImm(TYPE_U32, _f), r8 , r8 });
        newiopNoret(OP_LOAD, { r8d, argIImm(TYPE_U32, _f), r8 , r8 });

        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_U16, _f), rax, rax});
        newiopNoret(OP_LOAD, { di , argIImm(TYPE_U16, _f), rax, rax});
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_U16, _f), rdi, rax});
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_U16, _f), rax, rdi});
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_U16, _f), rax, rax});
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_U16, _f), r8 , rax});
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_U16, _f), r8 , rax});
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_U16, _f), rax, r8 });
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_U16, _f), rax, r8 });
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_U16, _f), r8 , r8 });
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_U16, _f), r8 , r8 });

        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_I16, _f), rax, rax});
        newiopNoret(OP_LOAD, { di , argIImm(TYPE_I16, _f), rax, rax});
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_I16, _f), rdi, rax});
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_I16, _f), rax, rdi});
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_I16, _f), rax, rax});
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_I16, _f), r8 , rax});
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_I16, _f), r8 , rax});
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_I16, _f), rax, r8 });
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_I16, _f), rax, r8 });
        newiopNoret(OP_LOAD, { ax , argIImm(TYPE_I16, _f), r8 , r8 });
        newiopNoret(OP_LOAD, { r8w, argIImm(TYPE_I16, _f), r8 , r8 });

        newiopNoret(OP_LOAD, { al , argIImm(TYPE_U8, _f), rax, rax});
        newiopNoret(OP_LOAD, { dil, argIImm(TYPE_U8, _f), rax, rax});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_U8, _f), rdi, rax});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_U8, _f), rax, rdi});
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_U8, _f), rax, rax});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_U8, _f), r8 , rax});
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_U8, _f), r8 , rax});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_U8, _f), rax, r8 });
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_U8, _f), rax, r8 });
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_U8, _f), r8 , r8 });
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_U8, _f), r8 , r8 });

        newiopNoret(OP_LOAD, { al , argIImm(TYPE_I8, _f), rax, rax});
        newiopNoret(OP_LOAD, { dil, argIImm(TYPE_I8, _f), rax, rax});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_I8, _f), rdi, rax});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_I8, _f), rax, rdi});
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_I8, _f), rax, rax});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_I8, _f), r8 , rax});
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_I8, _f), r8 , rax});
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_I8, _f), rax, r8 });
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_I8, _f), rax, r8 });
        newiopNoret(OP_LOAD, { al , argIImm(TYPE_I8, _f), r8 , r8 });
        newiopNoret(OP_LOAD, { r8b, argIImm(TYPE_I8, _f), r8 , r8 });

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
        //
        newiopNoret(OP_X86_ADC, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), rax });
        newiopNoret(OP_X86_ADC, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), rdi });
        newiopNoret(OP_X86_ADC, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), r8 });
        newiopNoret(OP_X86_ADC, { argSpilled(RB_INT, 32, _f), argSpilled(RB_INT, 32, _f), argIImm(256, _f) });

        newiopNoret(OP_CMP, { rax, argSpilled(RB_INT, 0x1FFF, _f) });
        newiopNoret(OP_CMP, { rdi, argSpilled(RB_INT, 0x1FFF, _f) });
        newiopNoret(OP_CMP, { r8,  argSpilled(RB_INT, 0x1FFF, _f) });
        newiopNoret(OP_CMP, { r15, argSpilled(RB_INT, 0x1FFF, _f) });
        newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF, _f), rax });
        newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF, _f), rdi });
        newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF, _f), r8  });
        newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF, _f), r15 });
        newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF, _f), argIImm(0x8888, _f) });

        newiopNoret(OP_ADD, { rax, rax, argSpilled(RB_INT, 0x1FFF, _f) });
        newiopNoret(OP_ADD, { rdi, rdi, argSpilled(RB_INT, 0x1FFF, _f) });
        newiopNoret(OP_ADD, { r8 , r8 , argSpilled(RB_INT, 0x1FFF, _f) });
        newiopNoret(OP_ADD, { r15, r15, argSpilled(RB_INT, 0x1FFF, _f) });
        newiopNoret(OP_ADD, { argSpilled(RB_INT, 0x1FFF, _f), argSpilled(RB_INT, 0x1FFF, _f), rax });
        newiopNoret(OP_ADD, { argSpilled(RB_INT, 0x1FFF, _f), argSpilled(RB_INT, 0x1FFF, _f), rdi });
        newiopNoret(OP_ADD, { argSpilled(RB_INT, 0x1FFF, _f), argSpilled(RB_INT, 0x1FFF, _f), r8  });
        newiopNoret(OP_ADD, { argSpilled(RB_INT, 0x1FFF, _f), argSpilled(RB_INT, 0x1FFF, _f), r15 });
        newiopNoret(OP_ADD, { argSpilled(RB_INT, 0x1FFF, _f), argSpilled(RB_INT, 0x1FFF, _f), argIImm(0x8888, _f) });
        });

};
#endif//__LOOPS_ARCH == __LOOPS_INTEL64
#endif//__LOOPS_TEST_INTEL64_HPP__
