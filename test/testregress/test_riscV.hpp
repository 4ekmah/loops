/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_TEST_RISCV_HPP__
#define __LOOPS_TEST_RISCV_HPP__

#include "loops/loops.hpp"
#if __LOOPS_ARCH == __LOOPS_RISCV
#include "loops/loops.hpp"
#include "src/common.hpp"
#include "src/func_impl.hpp"
#include "src/reg_allocator.hpp"
#include "tests.hpp"
#include <iostream>

using namespace loops;

// TEST(intel64, arithm_arrs)
// {
//     Context ctx;
//     USE_CONTEXT_(ctx);
//     {
//         IReg ptrA, ptrB, n, ptrAdd, ptrSub, ptrMul, ptrDiv;
//         STARTFUNC_(test_info_->name(), &ptrA, &ptrB, &n, &ptrAdd, &ptrSub, &ptrMul, &ptrDiv)
//         {
//             IReg offset = CONST_(0);
//             IReg i = CONST_(0);
//             WHILE_(i < n)
//             {
//                 IReg a = load_<int>(ptrA, offset);
//                 IReg b = load_<int>(ptrB, offset);
//                 store_<int>(ptrAdd, a + b);
//                 store_<int>(ptrSub, a - b);
//                 store_<int>(ptrMul, a * b);
//                 store_<int>(ptrDiv, a / b);
//                 i += 1;
//                 offset += sizeof(int);
//                 ptrAdd += sizeof(int);
//                 ptrSub += sizeof(int);
//                 ptrMul += sizeof(int);
//                 ptrDiv += sizeof(int);
//             };
//             RETURN_(0);
//         }
//     }
//     typedef int (*arithm_arrs_f)(const int* ptrA, const int* ptrB, int64_t n, int* ptrAdd, int* ptrSub, int* ptrMul, int* ptrDiv);
//     loops::Func func = ctx.getFunc(test_info_->name());
//     switch_spill_stress_test_mode_on(func);
//     EXPECT_IR_CORRECT(func);
//     EXPECT_ASSEMBLY_CORRECT(func);
//     arithm_arrs_f tested = reinterpret_cast<arithm_arrs_f>(func.ptr());
//     std::vector<int> A = { 8, 2, -5, 7, 6 };
//     std::vector<int> B = { 2, -5, 7, 6, 8 };
//     int addArr[5];
//     int subArr[5];
//     int mulArr[5];
//     int divArr[5];
//     ASSERT_EQ(tested(&A[0], &B[0], A.size(), addArr, subArr, mulArr, divArr), 0);
//     for (size_t n = 0; n < 5; n++)
//     {
//         ASSERT_EQ(A[n] + B[n], addArr[n]);
//         ASSERT_EQ(A[n] - B[n], subArr[n]);
//         ASSERT_EQ(A[n] * B[n], mulArr[n]);
//         ASSERT_EQ(A[n] / B[n], divArr[n]);
//     }
// }

TEST(riscV, instruction_set_test)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    STARTFUNC_(test_info_->name())
    {
        PREPARE_ASSEMBLY_TESTING(test_info_->name());

        DEFINE_ASSEMBLY_REG(zero, 0);
        DEFINE_ASSEMBLY_REG(t6, 31);

        newiopNoret(OP_MOV, { zero, zero});
        newiopNoret(OP_MOV, { t6  , zero});
        newiopNoret(OP_MOV, { zero, t6  });
        newiopNoret(OP_MOV, { zero, immtyped<int64_t>(0    , _f)});
        newiopNoret(OP_MOV, { t6  , immtyped<int64_t>(0    , _f)});
        newiopNoret(OP_MOV, { zero, immtyped<int64_t>(1    , _f)});
        newiopNoret(OP_MOV, { t6  , immtyped<int64_t>(1    , _f)});
        newiopNoret(OP_MOV, { zero, immtyped<int64_t>(2047 , _f)});
        newiopNoret(OP_MOV, { zero, immtyped<int64_t>(-2048, _f)});

        newiopNoret(OP_ADD, { zero, zero, zero });
        newiopNoret(OP_ADD, {   t6, zero, zero });
        newiopNoret(OP_ADD, { zero,   t6, zero });
        newiopNoret(OP_ADD, { zero, zero,   t6 });
        newiopNoret(OP_ADD, { zero, zero, immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_ADD, { t6  , zero, immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_ADD, { zero, t6  , immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_ADD, { zero, zero, immtyped<int64_t>(2047 , _f) });
        newiopNoret(OP_ADD, { zero, zero, immtyped<int64_t>(-2048, _f) });

        newiopNoret(OP_MUL, { zero, zero, zero });
        newiopNoret(OP_MUL, { t6  , zero, zero });
        newiopNoret(OP_MUL, { zero, t6  , zero });
        newiopNoret(OP_MUL, { zero, zero, t6   });

        newiopNoret(OP_DIV, { zero, zero, zero });
        newiopNoret(OP_DIV, { t6  , zero, zero });
        newiopNoret(OP_DIV, { zero, t6  , zero });
        newiopNoret(OP_DIV, { zero, zero, t6   });

        newiopNoret(OP_MOD, { zero, zero, zero });
        newiopNoret(OP_MOD, { t6  , zero, zero });
        newiopNoret(OP_MOD, { zero, t6  , zero });
        newiopNoret(OP_MOD, { zero, zero, t6   });

        // DUBUG: make this compilable by normal gcc assembler[fix printer], check HEXes.
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(zero), zero });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(t6)  , zero });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(zero), t6   });

        newiopNoret(OP_LOAD, { iregtyped<int32_t>(zero), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(t6  ), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(zero),   t6, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(zero), zero, argIImm(2047 ) });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(zero), zero, argIImm(-2048) });

        newiopNoret(OP_STORE, { zero, iregtyped<int32_t>(zero) });
        newiopNoret(OP_STORE, { t6  , iregtyped<int32_t>(zero) });
        newiopNoret(OP_STORE, { zero, iregtyped<int32_t>(t6  ) });

        newiopNoret(OP_STORE, { zero, argIImm(0    ), iregtyped<int32_t>(zero)});
        newiopNoret(OP_STORE, { t6  , argIImm(0    ), iregtyped<int32_t>(zero)});
        newiopNoret(OP_STORE, { zero, argIImm(0    ), iregtyped<int32_t>(t6  )});
        newiopNoret(OP_STORE, { zero, argIImm(2047 ), iregtyped<int32_t>(zero)});
        newiopNoret(OP_STORE, { zero, argIImm(-2048), iregtyped<int32_t>(zero)});

        // newiopNoret(OP_RET, {}); //No registers, no immediates, no way to point function.




        // Expr spilled32(argSpilled(RB_INT, 32));
        // Expr spilled0x1FFF(argSpilled(RB_INT, 0x1FFF));
        // spilled32.func() = _f;
        // spilled0x1FFF.func() = _f;

        // newiopNoret(OP_MOV, { rax, immtyped<int64_t>(0x10fffffff, _f) });
        // newiopNoret(OP_MOV, { rdi, immtyped<int64_t>(0x10fffffff, _f) });
        // newiopNoret(OP_MOV, {  r8, immtyped<int64_t>(0x10fffffff, _f) });
        // newiopNoret(OP_MOV, { r15, immtyped<int64_t>(0x10fffffff, _f) });
        // newiopNoret(OP_MOV, { spilled32, rax });
        // newiopNoret(OP_MOV, { spilled32, rdi });
        // newiopNoret(OP_MOV, { spilled32,  r8 });
        // newiopNoret(OP_MOV, { spilled32, r15 });
        // newiopNoret(OP_MOV, { spilled32, immtyped<int64_t>(0x101, _f) });

        // newiopNoret(OP_ADD, { rax, rax, immtyped<int64_t>(0x100, _f) });
        // newiopNoret(OP_ADD, { rcx, rcx, immtyped<int64_t>(0x100, _f) });
        // newiopNoret(OP_ADD, { rdi, rdi, immtyped<int64_t>(0x100, _f) });
        // newiopNoret(OP_ADD, {  r8,  r8, immtyped<int64_t>(0x100, _f) });
        // newiopNoret(OP_ADD, { r15, r15, immtyped<int64_t>(0x100, _f) });
        // newiopNoret(OP_ADD, { rax, rax, spilled32 });
        // newiopNoret(OP_ADD, { rdi, rdi, spilled32 });
        // newiopNoret(OP_ADD, { r8 , r8 , spilled32 });
        // newiopNoret(OP_ADD, { r15, r15, spilled32 });
        // newiopNoret(OP_ADD, { spilled32, spilled32, rax });
        // newiopNoret(OP_ADD, { spilled32, spilled32, rdi });
        // newiopNoret(OP_ADD, { spilled32, spilled32, r8  });
        // newiopNoret(OP_ADD, { spilled32, spilled32, r15 });
        // newiopNoret(OP_ADD, { spilled32, spilled32, immtyped<int64_t>(0x101, _f) });

        // newiopNoret(OP_SUB, { rax, rax, immtyped<int64_t>(0x100, _f) });
        // newiopNoret(OP_SUB, { rcx, rcx, immtyped<int64_t>(0x100, _f) });
        // newiopNoret(OP_SUB, { rdi, rdi, immtyped<int64_t>(0x100, _f) });
        // newiopNoret(OP_SUB, {  r8,  r8, immtyped<int64_t>(0x100, _f) });
        // newiopNoret(OP_SUB, { r15, r15, immtyped<int64_t>(0x100, _f) });
        // newiopNoret(OP_SUB, { rax, rax, spilled32 });
        // newiopNoret(OP_SUB, { rdi, rdi, spilled32 });
        // newiopNoret(OP_SUB, {  r8,  r8, spilled32 });
        // newiopNoret(OP_SUB, { r15, r15, spilled32 });
        // newiopNoret(OP_SUB, { spilled32, spilled32, rax });
        // newiopNoret(OP_SUB, { spilled32, spilled32, rdi });
        // newiopNoret(OP_SUB, { spilled32, spilled32,  r8 });
        // newiopNoret(OP_SUB, { spilled32, spilled32, r15 });
        // newiopNoret(OP_SUB, { spilled32, spilled32, immtyped<int64_t>(0x101, _f) });

        // newiopNoret(OP_MUL, { rax, rax, spilled32 });
        // newiopNoret(OP_MUL, { rdi, rdi, spilled32 });
        // newiopNoret(OP_MUL, {  r8,  r8, spilled32 });
        // newiopNoret(OP_MUL, { r15, r15, spilled32 });

        // newiopNoret(OP_DIV, { rax, rax, spilled32 });

        // newiopNoret(OP_NEG, { spilled32 });

        // newiopNoret(OP_STORE, { rax, immtyped<uint64_t>(256, _f) });
        // newiopNoret(OP_STORE, { rax, immtyped<int64_t>(256, _f) });
        // newiopNoret(OP_STORE, { rdi, immtyped<int64_t>(256, _f) });
        // newiopNoret(OP_STORE, { r8, immtyped<int64_t>(256, _f) });
        // newiopNoret(OP_STORE, { r12, immtyped<int64_t>(256, _f) });
        // newiopNoret(OP_STORE, { r13, immtyped<int64_t>(256, _f) });

        // newiopNoret(OP_STORE, { rax, immtyped<uint32_t>(256, _f) });
        // newiopNoret(OP_STORE, { rax, immtyped<int32_t>(256, _f) });
        // newiopNoret(OP_STORE, { rdi, immtyped<int32_t>(256, _f) });
        // newiopNoret(OP_STORE, { r8, immtyped<int32_t>(256, _f) });
        // newiopNoret(OP_STORE, { r12, immtyped<int32_t>(256, _f) });
        // newiopNoret(OP_STORE, { r13, immtyped<int32_t>(256, _f) });

        // newiopNoret(OP_STORE, { rax, immtyped<uint16_t>(256, _f) });
        // newiopNoret(OP_STORE, { rax, immtyped<int16_t>(256, _f) });
        // newiopNoret(OP_STORE, { rdi, immtyped<int16_t>(256, _f) });
        // newiopNoret(OP_STORE, { r8, immtyped<int16_t>(256, _f) });
        // newiopNoret(OP_STORE, { r12, immtyped<int16_t>(256, _f) });
        // newiopNoret(OP_STORE, { r13, immtyped<int16_t>(256, _f) });

        // newiopNoret(OP_STORE, { rax, immtyped<uint8_t>(255, _f) });
        // newiopNoret(OP_STORE, { rax, immtyped<int8_t>(255, _f) });
        // newiopNoret(OP_STORE, { rdi, immtyped<int8_t>(255, _f) });
        // newiopNoret(OP_STORE, { r8, immtyped<int8_t>(255, _f) });
        // newiopNoret(OP_STORE, { r12, immtyped<int8_t>(255, _f) });
        // newiopNoret(OP_STORE, { r13, immtyped<int8_t>(255, _f) });

        // newiopNoret(OP_STORE, { rax, iregtyped<uint64_t>(rax)});
        // newiopNoret(OP_STORE, { rax, iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { rax, iregtyped<int64_t>(rdi)});
        // newiopNoret(OP_STORE, { rax, iregtyped<int64_t>(r8)});
        // newiopNoret(OP_STORE, { rdi, iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { r8, iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { r8, iregtyped<int64_t>(r8)});
        // newiopNoret(OP_STORE, { r12, iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { r12, iregtyped<int64_t>(rdi)});
        // newiopNoret(OP_STORE, { r12, iregtyped<int64_t>(r8)});
        // newiopNoret(OP_STORE, { r13, iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { r13, iregtyped<int64_t>(rdi)});
        // newiopNoret(OP_STORE, { r13, iregtyped<int64_t>(r8)});
        
        // // // TODO(ch): Add here store_<uint32/int32>(rax, ax) just for symmetry;

        // newiopNoret(OP_STORE, { rax, iregtyped<uint16_t>(ax)});
        // newiopNoret(OP_STORE, { rax, iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { rax, iregtyped<int16_t>(di)});
        // newiopNoret(OP_STORE, { rax, iregtyped<int16_t>(r8w)});
        // newiopNoret(OP_STORE, { rdi, iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { r8 , iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { r8 , iregtyped<int16_t>(r8w)});
        // newiopNoret(OP_STORE, { r12, iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { r12, iregtyped<int16_t>(di)});
        // newiopNoret(OP_STORE, { r12, iregtyped<int16_t>(r8w)});
        // newiopNoret(OP_STORE, { r13, iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { r13, iregtyped<int16_t>(di)});
        // newiopNoret(OP_STORE, { r13, iregtyped<int16_t>(r8w)});

        // newiopNoret(OP_STORE, { rax, iregtyped<uint8_t>(al)});
        // newiopNoret(OP_STORE, { rax, iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { rax, iregtyped<int8_t>(dil)});
        // newiopNoret(OP_STORE, { rax, iregtyped<int8_t>(r8b)});
        // newiopNoret(OP_STORE, { rdi, iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { r8 , iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { r8 , iregtyped<int8_t>(r8b)});
        // newiopNoret(OP_STORE, { r12, iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { r12, iregtyped<int8_t>(dil)});
        // newiopNoret(OP_STORE, { r12, iregtyped<int8_t>(r8b)});
        // newiopNoret(OP_STORE, { r13, iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { r13, iregtyped<int8_t>(dil)});
        // newiopNoret(OP_STORE, { r13, iregtyped<int8_t>(r8b)});

        // newiopNoret(OP_STORE, { rax, rax, iregtyped<uint64_t>(rax)});
        // newiopNoret(OP_STORE, { rax, rax, iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { rax, rax, iregtyped<int64_t>(rdi)});
        // newiopNoret(OP_STORE, { rax, rax, iregtyped<int64_t>(r8)});
        // newiopNoret(OP_STORE, { rax, rdi, iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { rax, r8, iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { rax, r8, iregtyped<int64_t>(r8)});
        // newiopNoret(OP_STORE, { rdi, rax, iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { r8, rax, iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { r8, rax, iregtyped<int64_t>(r8)});
        // newiopNoret(OP_STORE, { r8, r8, iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { r8, r8, iregtyped<int64_t>(r8)});

        // newiopNoret(OP_STORE, { rax, rax, iregtyped<uint32_t>(eax)});
        // newiopNoret(OP_STORE, { rax, rax, iregtyped<int32_t>(eax)});
        // newiopNoret(OP_STORE, { rax, rax, iregtyped<int32_t>(edi)});
        // newiopNoret(OP_STORE, { rax, rax, iregtyped<int32_t>(r8d)});
        // newiopNoret(OP_STORE, { rax, rdi, iregtyped<int32_t>(eax)});
        // newiopNoret(OP_STORE, { rax, r8, iregtyped<int32_t>(eax)});
        // newiopNoret(OP_STORE, { rax, r8, iregtyped<int32_t>(r8d)});
        // newiopNoret(OP_STORE, { rdi, rax, iregtyped<int32_t>(eax)});
        // newiopNoret(OP_STORE, { r8, rax, iregtyped<int32_t>(eax)});
        // newiopNoret(OP_STORE, { r8, rax, iregtyped<int32_t>(r8d)});
        // newiopNoret(OP_STORE, { r8, r8, iregtyped<int32_t>(eax)});
        // newiopNoret(OP_STORE, { r8, r8, iregtyped<int32_t>(r8d)});
        // newiopNoret(OP_STORE, { r13, rax, iregtyped<int32_t>(eax)});
        // newiopNoret(OP_STORE, { r13, rax, iregtyped<int32_t>(r8d)});
        // newiopNoret(OP_STORE, { r13, r8, iregtyped<int32_t>(eax)});
        // newiopNoret(OP_STORE, { r13, r8, iregtyped<int32_t>(r8d)});

        // newiopNoret(OP_STORE, { rax, rax, iregtyped<uint16_t>(ax)});
        // newiopNoret(OP_STORE, { rax, rax, iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { rax, rax, iregtyped<int16_t>(di)});
        // newiopNoret(OP_STORE, { rax, rax, iregtyped<int16_t>(r8w)});
        // newiopNoret(OP_STORE, { rax, rdi, iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { rax, r8, iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { rax, r8, iregtyped<int16_t>(r8w)});
        // newiopNoret(OP_STORE, { rdi, rax, iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { r8, rax, iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { r8, rax, iregtyped<int16_t>(r8w)});
        // newiopNoret(OP_STORE, { r8, r8, iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { r8, r8, iregtyped<int16_t>(r8w)});
        // newiopNoret(OP_STORE, { r13, rax, iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { r13, r8, iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { r13, rax, iregtyped<int16_t>(r8w)});
        // newiopNoret(OP_STORE, { r13, r8, iregtyped<int16_t>(r8w)});

        // newiopNoret(OP_STORE, { rax, rax, iregtyped<uint8_t>(al)});
        // newiopNoret(OP_STORE, { rax, rax, iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { rax, rax, iregtyped<int8_t>(dil)});
        // newiopNoret(OP_STORE, { rax, rax, iregtyped<int8_t>(r8b)});
        // newiopNoret(OP_STORE, { rax, rdi, iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { rax, r8, iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { rax, r8, iregtyped<int8_t>(dil)});
        // newiopNoret(OP_STORE, { rax, r8, iregtyped<int8_t>(r8b)});
        // newiopNoret(OP_STORE, { rdi, rax, iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { r8, rax, iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { r8, rax, iregtyped<int8_t>(dil)});
        // newiopNoret(OP_STORE, { r8, rax, iregtyped<int8_t>(r8b)});
        // newiopNoret(OP_STORE, { r8, r8, iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { r8, r8, iregtyped<int8_t>(r8b)});
        // newiopNoret(OP_STORE, { r8, r8, iregtyped<int8_t>(dil)});
        // newiopNoret(OP_STORE, { r13, rax, iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { r13, rax, iregtyped<int8_t>(r8b)});
        // newiopNoret(OP_STORE, { r13, r8, iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { r13, r8, iregtyped<int8_t>(r8b)});

        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<uint64_t>(rax)});
        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int64_t>(rdi)});
        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int64_t>(r8)});
        // newiopNoret(OP_STORE, { rdi, argIImm(256), iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int64_t>(r8)});
        // newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int64_t>(rax)});
        // newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int64_t>(rdi)});
        // newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int64_t>(r8)});

        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<uint32_t>(eax)});
        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int32_t>(eax)});
        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int32_t>(edi)});
        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int32_t>(r8d)});
        // newiopNoret(OP_STORE, { rdi, argIImm(256), iregtyped<int32_t>(eax)});
        // newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int32_t>(eax)});
        // newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int32_t>(r8d)});
        // newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int32_t>(eax)});
        // newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int32_t>(r8d)});

        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<uint16_t>(ax)});
        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int16_t>(di)});
        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int16_t>(r8w)});
        // newiopNoret(OP_STORE, { rdi, argIImm(256), iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int16_t>(r8w)});
        // newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int16_t>(ax)});
        // newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int16_t>(r8w)});

        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<uint8_t>(al)});
        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int8_t>(dil)});
        // newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int8_t>(r8b)});
        // newiopNoret(OP_STORE, { rdi, argIImm(256), iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int8_t>(r8b)});
        // newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int8_t>(al)});
        // newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int8_t>(r8b)});

        // newiopNoret(OP_STORE, { rax, rax, immtyped<uint64_t>(256, _f)});
        // newiopNoret(OP_STORE, { rax, rax, immtyped<int64_t>(256, _f)});
        // newiopNoret(OP_STORE, { rax, rdi, immtyped<int64_t>(256, _f)});
        // newiopNoret(OP_STORE, { rax, r8, immtyped<int64_t>(256, _f)});
        // newiopNoret(OP_STORE, { rdi, rax, immtyped<int64_t>(256, _f)});
        // newiopNoret(OP_STORE, { r8, rax, immtyped<int64_t>(256, _f)});
        // newiopNoret(OP_STORE, { r8, r8, immtyped<int64_t>(256, _f)});
        // newiopNoret(OP_STORE, { r13, rax, immtyped<int64_t>(256, _f)});
        // newiopNoret(OP_STORE, { r13, r8, immtyped<int64_t>(256, _f)});

        // newiopNoret(OP_STORE, { rax, rax, immtyped<uint32_t>(256, _f)});
        // newiopNoret(OP_STORE, { rax, rax, immtyped<int32_t>(256, _f)});
        // newiopNoret(OP_STORE, { rax, rdi, immtyped<int32_t>(256, _f)});
        // newiopNoret(OP_STORE, { rax, r8, immtyped<int32_t>(256, _f)});
        // newiopNoret(OP_STORE, { rdi, rax, immtyped<int32_t>(256, _f)});
        // newiopNoret(OP_STORE, { r8, rax, immtyped<int32_t>(256, _f)});
        // newiopNoret(OP_STORE, { r8, r8, immtyped<int32_t>(256, _f)});
        // newiopNoret(OP_STORE, { r13, rax, immtyped<int32_t>(256, _f)});
        // newiopNoret(OP_STORE, { r13, r8, immtyped<int32_t>(256, _f)});

        // newiopNoret(OP_STORE, { rax, rax, immtyped<uint16_t>(256, _f)});
        // newiopNoret(OP_STORE, { rax, rax, immtyped<int16_t>(256, _f)});
        // newiopNoret(OP_STORE, { rax, rdi, immtyped<int16_t>(256, _f)});
        // newiopNoret(OP_STORE, { rax, r8, immtyped<int16_t>(256, _f)});
        // newiopNoret(OP_STORE, { rdi, rax, immtyped<int16_t>(256, _f)});
        // newiopNoret(OP_STORE, { r8, rax, immtyped<int16_t>(256, _f)});
        // newiopNoret(OP_STORE, { r8, r8, immtyped<int16_t>(256, _f)});
        // newiopNoret(OP_STORE, { r13, rax, immtyped<int16_t>(256, _f)});
        // newiopNoret(OP_STORE, { r13, r8, immtyped<int16_t>(256, _f)});

        // newiopNoret(OP_STORE, { rax, rax, immtyped<uint8_t>(255, _f)});
        // newiopNoret(OP_STORE, { rax, rax, immtyped<int8_t>(255, _f)});
        // newiopNoret(OP_STORE, { rax, rdi, immtyped<int8_t>(255, _f)});
        // newiopNoret(OP_STORE, { rax, r8, immtyped<int8_t>(255, _f)});
        // newiopNoret(OP_STORE, { rdi, rax, immtyped<int8_t>(255, _f)});
        // newiopNoret(OP_STORE, { r8, rax, immtyped<int8_t>(255, _f)});
        // newiopNoret(OP_STORE, { r8, r8, immtyped<int8_t>(255, _f)});
        // newiopNoret(OP_STORE, { r13, rax, immtyped<int8_t>(255, _f)});
        // newiopNoret(OP_STORE, { r13, r8, immtyped<int8_t>(255, _f)});

        // newiopNoret(OP_STORE, { rax, argIImm(257), immtyped<uint64_t>(256, _f)});
        // newiopNoret(OP_STORE, { rax, argIImm(257), immtyped<int64_t>(256, _f)});
        // newiopNoret(OP_STORE, { rdi, argIImm(257), immtyped<int64_t>(256, _f)});
        // newiopNoret(OP_STORE, { r8, argIImm(257), immtyped<int64_t>(256, _f)});
        // newiopNoret(OP_STORE, { r12, argIImm(257), immtyped<int64_t>(256, _f)});

        // newiopNoret(OP_STORE, { rax, argIImm(257), immtyped<uint32_t>(256, _f)});
        // newiopNoret(OP_STORE, { rax, argIImm(257), immtyped<int32_t>(256, _f)});
        // newiopNoret(OP_STORE, { rdi, argIImm(257), immtyped<int32_t>(256, _f)});
        // newiopNoret(OP_STORE, { r8, argIImm(257), immtyped<int32_t>(256, _f)});
        // newiopNoret(OP_STORE, { r12, argIImm(257), immtyped<int32_t>(256, _f)});

        // newiopNoret(OP_STORE, { rax, argIImm(257), immtyped<uint16_t>(256, _f)});
        // newiopNoret(OP_STORE, { rax, argIImm(257), immtyped<int16_t>(256, _f)});
        // newiopNoret(OP_STORE, { rdi, argIImm(257), immtyped<int16_t>(256, _f)});
        // newiopNoret(OP_STORE, { r8, argIImm(257), immtyped<int16_t>(256, _f)});
        // newiopNoret(OP_STORE, { r12, argIImm(257), immtyped<int16_t>(256, _f)});

        // newiopNoret(OP_STORE, { ax, argIImm(257), immtyped<uint8_t>(255, _f)});
        // newiopNoret(OP_STORE, { ax, argIImm(257), immtyped<int8_t>(255, _f)});
        // newiopNoret(OP_STORE, { di, argIImm(257), immtyped<int8_t>(255, _f)});
        // newiopNoret(OP_STORE, { r8, argIImm(257), immtyped<int8_t>(255, _f)});
        // newiopNoret(OP_STORE, { r12, argIImm(257), immtyped<int8_t>(255, _f)});

        // newiopNoret(OP_LOAD, { iregtyped<uint64_t>(rax), rax });
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax), rax });
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rdi), rax });
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax), rdi });
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax), rax });
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) , rax });
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax), r8  });
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) , r8  });
                                              
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rax });
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(edi), rax });
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rdi });
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rax });
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), rax });
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r8  });
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r8  });

        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rax });
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(di) , rax });
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rdi });
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), rax });
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , r8  });
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r8  });

        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rax });
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(di) , rax });
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rdi });
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), rax });
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , r8  });
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r8  });

        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(dil), rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rdi});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r8 });
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r8 });

        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rax});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(dil), rax});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rdi});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), rax});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r8 });
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r8 });

        // newiopNoret(OP_LOAD, { iregtyped<uint64_t>(rax), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rdi),  rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rdi, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  r8 , argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  r8 , argIImm(256)});

        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(edi), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rdi, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r8 , argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r8 , argIImm(256)});

        // newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int32_t>(edi), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), rdi, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), r8 , argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), r8 , argIImm(256)});

        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(di) , rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rdi, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , r8 , argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r8 , argIImm(256)});

        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(di) , rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rdi, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , r8 , argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r8 , argIImm(256)});

        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(dil), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rdi, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r8 , argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r8 , argIImm(256)});

        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(dil), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rdi, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), rax, argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r8 , argIImm(256)});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r8 , argIImm(256)});

        // newiopNoret(OP_LOAD, { iregtyped<uint64_t>(rax), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rdi),  rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rdi, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rax, rdi});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  r8 , rax});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  r8 , rax});
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rax, r8 });
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  rax, r8 });
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  r8 , r8 });
        // newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  r8 , r8 });

        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(edi), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rdi, rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rax, rdi});
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r8 , rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r8 , rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rax, r8 });
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), rax, r8 });
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r8 , r8 });
        // newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r8 , r8 });

        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(di), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax), rdi, rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax), rax, rdi});
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax), r8 , rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r8 , rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax), rax, r8 });
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), rax, r8 });
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax), r8 , r8 });
        // newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r8 , r8 });

        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(di), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax), rdi, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax), rax, rdi});
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax), r8 , rax});
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r8 , rax});
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax), rax, r8 });
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), rax, r8 });
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax), r8 , r8 });
        // newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r8 , r8 });

        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(dil), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rdi, rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rax, rdi});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r8 , rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r8 , rax});
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rax, r8 });
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), rax, r8 });
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r8 , r8 });
        // newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r8 , r8 });

        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(dil), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rdi, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rax, rdi});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), rax, rax});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r8 , rax});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r8 , rax});
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rax, r8 });
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), rax, r8 });
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r8 , r8 });
        // newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r8 , r8 });

        // newiopNoret(OP_XCHG, { rax, rax });
        // newiopNoret(OP_XCHG, { rax, rdi });
        // newiopNoret(OP_XCHG, { rax, r8 });

        // newiopNoret(OP_XCHG, { rcx, rcx });
        // newiopNoret(OP_XCHG, { rdi, rcx });
        // newiopNoret(OP_XCHG, { rcx, rdi });
        // newiopNoret(OP_XCHG, { rcx, rcx });
        // newiopNoret(OP_XCHG, { r8 , rcx });
        // newiopNoret(OP_XCHG, { rcx,  r8 });
        // newiopNoret(OP_XCHG, { r8 ,  r8 });

        // newiopNoret(OP_XCHG, { rcx,  spilled32 });
        // newiopNoret(OP_XCHG, { rdi,  spilled32 });
        // newiopNoret(OP_XCHG, { r8 ,  spilled32 });
        // newiopNoret(OP_XCHG, { r15,  spilled32 });

        // newiopNoret(OP_XCHG, { spilled32, rcx });
        // newiopNoret(OP_XCHG, { spilled32, rdi });
        // newiopNoret(OP_XCHG, { spilled32, r8 });
        // newiopNoret(OP_XCHG, { spilled32, r15 });

        // newiopNoret(OP_SHL, { rax, rax, argIImm(15) });
        // newiopNoret(OP_SHL, { rdi, rdi, argIImm(15) });
        // newiopNoret(OP_SHL, { r8 , r8, argIImm(15) });

        // newiopNoret(OP_SHL, { spilled32, spilled32, argIImm(15) });

        // newiopNoret(OP_SHL, { rax, rax, cl });
        // newiopNoret(OP_SHL, { rdi, rdi, cl });
        // newiopNoret(OP_SHL, { r8, r8, cl });
        // newiopNoret(OP_SHL, { spilled32, spilled32, cl });

        // newiopNoret(OP_SHR, { rax,rax, argIImm(15) });
        // newiopNoret(OP_SHR, { rdi,rdi, argIImm(15) });
        // newiopNoret(OP_SHR, { r8 , r8, argIImm(15) });
        // newiopNoret(OP_SHR, { spilled32, spilled32, argIImm(15) });

        // newiopNoret(OP_SHR, { rax, rax, cl });
        // newiopNoret(OP_SHR, { rdi, rdi, cl });
        // newiopNoret(OP_SHR, { r8, r8, cl });
        // newiopNoret(OP_SHR, { spilled32, spilled32, cl });

        // newiopNoret(OP_SAR, { rax,rax, argIImm(15) });
        // newiopNoret(OP_SAR, { rdi,rdi, argIImm(15) });
        // newiopNoret(OP_SAR, { r8 , r8, argIImm(15) });
        // newiopNoret(OP_SAR, { spilled32, spilled32, argIImm(15) });

        // newiopNoret(OP_SAR, { rax, rax, cl });
        // newiopNoret(OP_SAR, { rdi, rdi, cl });
        // newiopNoret(OP_SAR, { r8, r8, cl });
        // newiopNoret(OP_SAR, { spilled32, spilled32, cl });

        // newiopNoret(OP_AND, { rax, rax, rax });
        // newiopNoret(OP_AND, { rax, rax, rdi });
        // newiopNoret(OP_AND, { rdi, rdi, rax });
        // newiopNoret(OP_AND, { rax, rax, r8 });
        // newiopNoret(OP_AND, { r8 , r8 , rax });
        // newiopNoret(OP_AND, { r8 , r8 , r8 });

        // newiopNoret(OP_AND, { rax, rax, spilled32 });
        // newiopNoret(OP_AND, { rdi, rdi, spilled32 });
        // newiopNoret(OP_AND, { r8 , r8 , spilled32 });

        // newiopNoret(OP_AND, { rax, rax, argIImm(256) });
        // newiopNoret(OP_AND, { rcx, rcx, argIImm(256) });
        // newiopNoret(OP_AND, { rdi, rdi, argIImm(256) });
        // newiopNoret(OP_AND, { r8 , r8 , argIImm(256) });

        // newiopNoret(OP_AND, { spilled32, spilled32, rax });
        // newiopNoret(OP_AND, { spilled32, spilled32, rdi });
        // newiopNoret(OP_AND, { spilled32, spilled32, r8 });

        // newiopNoret(OP_AND, { spilled32, spilled32, argIImm(257) });

        // newiopNoret(OP_OR, { rax, rax, rax });
        // newiopNoret(OP_OR, { rax, rax, rdi });
        // newiopNoret(OP_OR, { rdi, rdi, rax });
        // newiopNoret(OP_OR, { rax, rax, r8 });
        // newiopNoret(OP_OR, { r8 , r8 , rax });

        // newiopNoret(OP_OR, { spilled32, spilled32, rax });
        // newiopNoret(OP_OR, { spilled32, spilled32, r8 });

        // newiopNoret(OP_OR, { rax, rax, spilled32 });
        // newiopNoret(OP_OR, { r8, r8, spilled32 });

        // newiopNoret(OP_OR, { rax, rax, argIImm(256) });
        // newiopNoret(OP_OR, { r8, r8, argIImm(256) });
        // newiopNoret(OP_OR, { spilled32, spilled32, argIImm(257) });

        // newiopNoret(OP_XOR, { rax, rax, rax });
        // newiopNoret(OP_XOR, { rax, rax, rdi });
        // newiopNoret(OP_XOR, { rdi, rdi, rax });
        // newiopNoret(OP_XOR, { rax, rax, r8 });
        // newiopNoret(OP_XOR, { r8 , r8 , rax });
        // newiopNoret(OP_XOR, { r8 , r8 , r8 });

        // newiopNoret(OP_XOR, { rax, rax, spilled32 });
        // newiopNoret(OP_XOR, { rdi, rdi, spilled32 });
        // newiopNoret(OP_XOR, { r8 , r8 , spilled32 });

        // newiopNoret(OP_XOR, { rax, rax, argIImm(256) });
        // newiopNoret(OP_XOR, { rcx, rcx, argIImm(256) });
        // newiopNoret(OP_XOR, { rdi, rdi, argIImm(256) });
        // newiopNoret(OP_XOR, { r8 , r8 , argIImm(256) });

        // newiopNoret(OP_XOR, { spilled32, spilled32, rax });
        // newiopNoret(OP_XOR, { spilled32, spilled32, rdi });
        // newiopNoret(OP_XOR, { spilled32, spilled32, r8 });

        // newiopNoret(OP_XOR, { spilled32, spilled32, argIImm(257) });

        // newiopNoret(OP_SELECT, { rax, argIImm(OP_EQ), rax, rax });
        // newiopNoret(OP_SELECT, { rax, argIImm(OP_NE), rax, rax });
        // newiopNoret(OP_SELECT, { rax, argIImm(OP_LT), rax, rax });
        // newiopNoret(OP_SELECT, { rax, argIImm(OP_GT), rax, rax });
        // newiopNoret(OP_SELECT, { rax, argIImm(OP_LE), rax, rax });
        // newiopNoret(OP_SELECT, { rax, argIImm(OP_GE), rax, rax });
        // newiopNoret(OP_SELECT, { rax, argIImm(OP_S ), rax, rax });
        // newiopNoret(OP_SELECT, { rax, argIImm(OP_NS), rax, rax });

        // newiopNoret(OP_SELECT, { rdi, OP_EQ, rax, rdi });
        // newiopNoret(OP_SELECT, { rax, OP_EQ, rdi, rax });

        // newiopNoret(OP_SELECT, { r8 , OP_EQ, rax, r8 });
        // newiopNoret(OP_SELECT, { rax, OP_EQ, r8 , rax });
        // newiopNoret(OP_SELECT, { r8 , OP_EQ, r8 , r8 });

        // newiopNoret(OP_SELECT, { rax, OP_EQ, spilled32, rax });
        // newiopNoret(OP_SELECT, { rdi, OP_EQ, spilled32, rdi });
        // newiopNoret(OP_SELECT, { r8,  OP_EQ, spilled32, r8 });

        // newiopNoret(OP_IVERSON, { rax, argIImm(OP_EQ)});
        // newiopNoret(OP_IVERSON, { rax, argIImm(OP_NE)});
        // newiopNoret(OP_IVERSON, { rax, argIImm(OP_LT)});
        // newiopNoret(OP_IVERSON, { rax, argIImm(OP_GT)});
        // newiopNoret(OP_IVERSON, { rax, argIImm(OP_LE)});
        // newiopNoret(OP_IVERSON, { rax, argIImm(OP_GE)});
        // newiopNoret(OP_IVERSON, { rax, argIImm(OP_S )});
        // newiopNoret(OP_IVERSON, { rax, argIImm(OP_NS)});

        // newiopNoret(OP_IVERSON, { rdi, argIImm(OP_EQ) });
        // newiopNoret(OP_IVERSON, { r8 , argIImm(OP_EQ) });
        // newiopNoret(OP_IVERSON, { spilled32, argIImm(OP_EQ) });

        // newiopNoret(OP_X86_ADC, { rax, rax, rax });
        // newiopNoret(OP_X86_ADC, { rdi, rdi, rax });
        // newiopNoret(OP_X86_ADC, { rax, rax, rdi });
        // newiopNoret(OP_X86_ADC, { r8 , r8 , rax });
        // newiopNoret(OP_X86_ADC, { rax, rax, r8 });
        // newiopNoret(OP_X86_ADC, { r8 , r8 , r8 });

        // newiopNoret(OP_X86_ADC, { rax, rax, argIImm(256) });
        // newiopNoret(OP_X86_ADC, { rcx, rcx, argIImm(256) });
        // newiopNoret(OP_X86_ADC, { rdi, rdi, argIImm(256) });
        // newiopNoret(OP_X86_ADC, { r8 , r8 , argIImm(256) });
        // newiopNoret(OP_X86_ADC, { rax, rax, spilled32 });
        // newiopNoret(OP_X86_ADC, { rdi, rdi, spilled32 });
        // newiopNoret(OP_X86_ADC, { r8 , r8 , spilled32 });
        // newiopNoret(OP_X86_ADC, { spilled32, spilled32, rax });
        // newiopNoret(OP_X86_ADC, { spilled32, spilled32, rdi });
        // newiopNoret(OP_X86_ADC, { spilled32, spilled32, r8 });
        // newiopNoret(OP_X86_ADC, { spilled32, spilled32, argIImm(256) });

        // newiopNoret(OP_CMP, { rax, argSpilled(RB_INT, 0x1FFF) });
        // newiopNoret(OP_CMP, { rdi, argSpilled(RB_INT, 0x1FFF) });
        // newiopNoret(OP_CMP, { r8,  argSpilled(RB_INT, 0x1FFF) });
        // newiopNoret(OP_CMP, { r15, argSpilled(RB_INT, 0x1FFF) });
        // newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF), rax });
        // newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF), rdi });
        // newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF), r8  });
        // newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF), r15 });
        // newiopNoret(OP_CMP, { spilled0x1FFF, argIImm(0x8888) });

        // newiopNoret(OP_CALL_NORET, { rax });
        // newiopNoret(OP_CALL_NORET, { rdi });
        // newiopNoret(OP_CALL_NORET, {  r8 });
        // newiopNoret(OP_CALL_NORET, { r15 });
        // newiopNoret(OP_CALL_NORET, { spilled32 });
    }
    loops::Func func = ctx.getFunc(test_info_->name());
    EXPECT_ASSEMBLY_CORRECT(func);
}
#endif//__LOOPS_ARCH == __LOOPS_RISCV
#endif//__LOOPS_TEST_RISCV_HPP__
