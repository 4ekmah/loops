/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_TEST_AARCH64_HPP__
#define __LOOPS_TEST_AARCH64_HPP__
#include "loops/loops.hpp"
#if __LOOPS_ARCH == __LOOPS_AARCH64
#include "tests.hpp"
#include <iostream>

namespace loops
{
LTEST(ten_args_to_sum, { //There we are testing stack parameter passing.
   IReg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
   STARTFUNC_(TESTNAME, &a0, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9)
   {
       getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, { 0, 1, 2, 3, 4, 5, 6, 7 }, { 0, 1, 2, 3, 4, 5, 6, 7 }, {}, { 18, 19, 20, 21, 22 });
       IReg res = a0 * 1;
       res += a1 * 2;
       res += a2 * 3;
       res += a3 * 4;
       res += a4 * 5;
       res += a5 * 6;
       res += a6 * 7;
       res += a7 * 8;
       res += a8 * 3;
       res += a9 * 2;
       RETURN_(res);
   }
   });
LTESTexe(ten_args_to_sum, {
   typedef int64_t(*ten_args_to_sum_f)(int64_t a0, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6, int64_t a7, int64_t a8, int64_t a9);
   ten_args_to_sum_f tested = reinterpret_cast<ten_args_to_sum_f>(EXEPTR);
   std::vector<int> v = { 1,1,1,1,1,1,1,1,3,5 };
   EXPECT_EQ(tested(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9]),(int64_t)(55));
   });

LTEST(nullify_msb_lsb_v, {
    IReg iptr, omptr, olptr, n;
    STARTFUNC_(TESTNAME, &iptr, &omptr, &olptr, &n)
    {
        IReg offset  = CONST_(0);
        n *= sizeof(uint32_t);
        VReg<uint32_t> one  = VCONST_(uint32_t, 1);
        WHILE_(offset < n)
        {
            VReg<uint32_t> in = loadvec<uint32_t>(iptr, offset);
            VReg<uint32_t> msb = in | ushift_right(in,1);
            msb |= ushift_right(msb,  2);
            msb |= ushift_right(msb,  4);
            msb |= ushift_right(msb,  8);
            msb |= ushift_right(msb, 16);
            msb += one;  //It's assumed, that 0x80000000 bit is switched off.
            msb = ushift_right(msb, 1);
            msb ^= in;
            storevec(omptr, offset, msb);
            VReg<uint32_t> lsb = in & ~(in - one);
            lsb ^= in;
            storevec(olptr, offset, lsb);
            offset += CTX.vbytes();
        }
        RETURN_();
    }
    });
LTESTexe(nullify_msb_lsb_v, {
    typedef uint32_t (*clear_lsb_msb_v)(const uint32_t* src, uint32_t* msbdest, uint32_t* lsbdest, int64_t n);
    clear_lsb_msb_v tested = reinterpret_cast<clear_lsb_msb_v>(EXEPTR);
    std::vector<uint32_t> v =   { 0x60000000, 2, 0xf0, 7, 0x0fffffff, 0b101010101, 1234, 4321};
    std::vector<uint32_t> lsb = { 0, 0, 0, 0, 0, 0, 0, 0 };
    std::vector<uint32_t> msb = { 0, 0, 0, 0, 0, 0, 0, 0 };
    tested(&v[0], &msb[0], &lsb[0], v.size());
    for (size_t vnum = 0; vnum < v.size(); vnum++ )
    {
        uint32_t tchk = v[vnum];
        uint32_t relsb = tchk ^ (tchk & ~(tchk - 1));
        uint32_t remsb = tchk | tchk >> 1;
        remsb |= remsb >> 2;
        remsb |= remsb >> 4;
        remsb |= remsb >> 8;
        remsb |= remsb >> 16;
        remsb = (remsb + 1) >> 1;
        remsb ^= tchk;
        EXPECT_EQ(msb[vnum], remsb);
        EXPECT_EQ(lsb[vnum], relsb);
    }
    });

#define DEFINE_CERTAIN_REG(name, number) IReg name##_0; name##_0.func = _f; name##_0.idx = number; IRecipe name##_1(name##_0); Recipe name = name##_1.notype()
LTESTcomposer(instruction_set_test, {
    FuncImpl* _f = getImpl(getImpl(&CTX)->getCurrentFunc());
    DEFINE_CERTAIN_REG(x0, 0);
    DEFINE_CERTAIN_REG(w0, 0);
    DEFINE_CERTAIN_REG(x15, 15);
    DEFINE_CERTAIN_REG(w15, 15);
    Recipe v0_16u = VRecipe<uint8_t>::make(vregHid<uint8_t>(0,_f)).notype();
    Recipe v31_16u = VRecipe<uint8_t>::make(vregHid<uint8_t>(31,_f)).notype();
    Recipe v0_8u = VRecipe<uint16_t>::make(vregHid<uint16_t>(0,_f)).notype();
    Recipe v31_8u = VRecipe<uint16_t>::make(vregHid<uint16_t>(31,_f)).notype();
    Recipe v0_4u = VRecipe<uint32_t>::make(vregHid<uint32_t>(0,_f)).notype();
    Recipe v31_4u = VRecipe<uint32_t>::make(vregHid<uint32_t>(31,_f)).notype();
    Recipe v0_2u = VRecipe<uint64_t>::make(vregHid<uint64_t>(0,_f)).notype();
    Recipe v31_2u = VRecipe<uint64_t>::make(vregHid<uint64_t>(31,_f)).notype();
    Recipe v0_16s = VRecipe<int8_t>::make(vregHid<int8_t>(0,_f)).notype();
    Recipe v1_16s = VRecipe<int8_t>::make(vregHid<int8_t>(1,_f)).notype();
    Recipe v31_16s = VRecipe<int8_t>::make(vregHid<int8_t>(31,_f)).notype();
    Recipe v0_8s = VRecipe<int16_t>::make(vregHid<int16_t>(0,_f)).notype();
    Recipe v1_8s = VRecipe<int16_t>::make(vregHid<int16_t>(1,_f)).notype();
    Recipe v31_8s = VRecipe<int16_t>::make(vregHid<int16_t>(31,_f)).notype();
    Recipe v0_4s = VRecipe<int32_t>::make(vregHid<int32_t>(0,_f)).notype();
    Recipe v1_4s = VRecipe<int32_t>::make(vregHid<int32_t>(1,_f)).notype();
    Recipe v31_4s = VRecipe<int32_t>::make(vregHid<int32_t>(31,_f)).notype();
    Recipe v0_2s = VRecipe<int64_t>::make(vregHid<int64_t>(0,_f)).notype();
    Recipe v1_2s = VRecipe<int64_t>::make(vregHid<int64_t>(1,_f)).notype();
    Recipe v31_2s = VRecipe<int64_t>::make(vregHid<int64_t>(31,_f)).notype();
    Recipe v0_8f = VRecipe<f16_t>::make(vregHid<f16_t>(0,_f)).notype();
    Recipe v15_8f = VRecipe<f16_t>::make(vregHid<f16_t>(15,_f)).notype();
    Recipe v31_8f = VRecipe<f16_t>::make(vregHid<f16_t>(31,_f)).notype();
    Recipe v0_4f = VRecipe<float>::make(vregHid<float>(0,_f)).notype();
    Recipe v31_4f = VRecipe<float>::make(vregHid<float>(31,_f)).notype();
    Recipe v0_2f = VRecipe<double>::make(vregHid<double>(0,_f)).notype();
    Recipe v31_2f = VRecipe<double>::make(vregHid<double>(31,_f)).notype();
    Recipe q0 = VRecipe<uint64_t>::make(vregHid<uint64_t>(0,_f)).notype();
    Recipe q31 = VRecipe<uint64_t>::make(vregHid<uint64_t>(31,_f)).notype();
    
    newiopNoret(OP_MOV, {  x0, argIImm(         -1) });
    newiopNoret(OP_MOV, { x15, argIImm(         -1) });
    newiopNoret(OP_MOV, {  x0, argIImm(-(0xFFFF+1)) });

    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<uint64_t>(x0) });
    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<int64_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<uint64_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<int64_t>(x0) });

    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<uint32_t>(x0) });
    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<int32_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<uint32_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<int32_t>(x0) });

    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<uint16_t>(x0) });
    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<int16_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<uint16_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<int16_t>(x0) });

    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<uint8_t>(x0) });
    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<int8_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<uint8_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<int8_t>(x0) });

    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x0), x0, x15});

    newiopNoret(OP_SELECT, { x0, OP_EQ, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_NE, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_LT, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_GT, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_LE, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_GE, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_S,  x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_NS, x0, x0 });

    newiopNoret(OP_SELECT, { x15, OP_EQ, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_EQ, x15, x0 });
    newiopNoret(OP_SELECT, { x0, OP_EQ, x0, x15 });

    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_EQ });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_NE });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_LT });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_GT });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_LE });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_GE });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_S  });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_NS });
    newiopNoret(OP_ARM_CINC, { x15,  x0, OP_EQ });
    newiopNoret(OP_ARM_CINC, { x0 , x15, OP_EQ });

    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_EQ });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_NE });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_LT });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_GT });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_LE });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_GE });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_S  });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_NS });
    newiopNoret(OP_ARM_CNEG, { x15,  x0, OP_EQ });
    newiopNoret(OP_ARM_CNEG, { x0 , x15, OP_EQ });

    newiopNoret(OP_SAR, { x0,  x0, argIImm(63) });
    newiopNoret(OP_SAR, { x15, x0, argIImm(63) });
    newiopNoret(OP_SAR, { x0, x15, argIImm(63) });

    newiopNoret(OP_SAR, { x0 ,  x0,  x0 });
    newiopNoret(OP_SAR, { x15,  x0,  x0 });
    newiopNoret(OP_SAR, { x0 , x15,  x0 });
    newiopNoret(OP_SAR, { x0 ,  x0, x15 });

    newiopNoret(OP_SHR, { x0,  x0, argIImm(63) });
    newiopNoret(OP_SHR, { x15, x0, argIImm(63) });
    newiopNoret(OP_SHR, { x0, x15, argIImm(63) });

    newiopNoret(OP_SHR, { x0 ,  x0,  x0 });
    newiopNoret(OP_SHR, { x15,  x0,  x0 });
    newiopNoret(OP_SHR, { x0 , x15,  x0 });
    newiopNoret(OP_SHR, { x0 ,  x0, x15 });

    newiopNoret(OP_SHL, { x0,  x0, argIImm(63) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(31) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(15) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(7) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(1) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(0) });

    newiopNoret(OP_SHL, { x15, x0, argIImm(63) });
    newiopNoret(OP_SHL, { x0, x15, argIImm(63) });

    newiopNoret(OP_SHL, { x0 ,  x0,  x0 });
    newiopNoret(OP_SHL, { x15,  x0,  x0 });
    newiopNoret(OP_SHL, { x0 , x15,  x0 });
    newiopNoret(OP_SHL, { x0 ,  x0, x15 });

    newiopNoret(OP_AND, { x0 ,   x0,  x0 });
    newiopNoret(OP_AND, { x15,   x0,  x0 });
    newiopNoret(OP_AND, { x0 ,  x15,  x0 });
    newiopNoret(OP_AND, { x0 ,  x0 , x15 });
    newiopNoret(OP_AND, { x0 ,  x0 , argIImm(0xFF) });
    newiopNoret(OP_AND, { x0 ,  x0 , argIImm(0xFF00) });
    newiopNoret(OP_AND, { x0 ,  x0 , argIImm(0xFF00000000000000) });
    newiopNoret(OP_AND, { x15,  x0 , argIImm(0xFF) });
    newiopNoret(OP_AND, { x0,  x15 , argIImm(0xFF) });

    newiopNoret(OP_OR, { x0 ,   x0,  x0 });
    newiopNoret(OP_OR, { x15,   x0,  x0 });
    newiopNoret(OP_OR, { x0 ,  x15,  x0 });
    newiopNoret(OP_OR, { x0 ,  x0 , x15 });
    newiopNoret(OP_OR, { x0 ,  x0 , argIImm(0xFF) });
    newiopNoret(OP_OR, { x0 ,  x0 , argIImm(0xFF00) });
    newiopNoret(OP_OR, { x0 ,  x0 , argIImm(0xFF00000000000000) });
    newiopNoret(OP_OR, { x15,  x0 , argIImm(0xFF) });
    newiopNoret(OP_OR, { x0,  x15 , argIImm(0xFF) });

    newiopNoret(OP_XOR, { x0 ,   x0,  x0 });
    newiopNoret(OP_XOR, { x15,   x0,  x0 });
    newiopNoret(OP_XOR, { x0 ,  x15,  x0 });
    newiopNoret(OP_XOR, { x0 ,  x0 , x15 });
    newiopNoret(OP_XOR, { x0 ,  x0 , argIImm(0xFF) });
    newiopNoret(OP_XOR, { x0 ,  x0 , argIImm(0xFF00) });
    newiopNoret(OP_XOR, { x0 ,  x0 , argIImm(0xFF00000000000000) });
    newiopNoret(OP_XOR, { x15,  x0 , argIImm(0xFF) });
    newiopNoret(OP_XOR, { x0,  x15 , argIImm(0xFF) });

    newiopNoret(OP_NOT, { x0 , x0 });
    newiopNoret(OP_NOT, { x15 , x0});
    newiopNoret(OP_NOT, { x0 , x15});

    newiopNoret(OP_NEG, { x0 , x0 });
    newiopNoret(OP_NEG, { x15 , x0});
    newiopNoret(OP_NEG, { x0 , x15});
//
    newiopNoret(VOP_LOAD, { q0 ,  x0 });
    newiopNoret(VOP_LOAD, { q0 ,  x0, argIImm(32784) });
    newiopNoret(VOP_LOAD, { q31,  x0 });
    newiopNoret(VOP_LOAD, { q0 , x15 });
    newiopNoret(VOP_LOAD, { q0 ,  x0, x0  });
    newiopNoret(VOP_LOAD, { q31,  x0, x0  });
    newiopNoret(VOP_LOAD, { q0 , x15, x0  });
    newiopNoret(VOP_LOAD, { q0 ,  x0, x15 });

    newiopNoret(VOP_STORE, { x0, q0 });
    newiopNoret(VOP_STORE, { x0, argIImm(32784), q0 });
    newiopNoret(VOP_STORE, {  x0, q31 });
    newiopNoret(VOP_STORE, { x15, q0 });
    newiopNoret(VOP_STORE, {  x0, x0 , q0  });
    newiopNoret(VOP_STORE, {  x0, x0 , q31 });
    newiopNoret(VOP_STORE, { x15, x0 , q0  });
    newiopNoret(VOP_STORE, {  x0, x15, q0  });

    newiopNoret(VOP_ADD, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_ADD, { v31_16u, v0_16u,  v0_16u  });
    newiopNoret(VOP_ADD, { v0_16u,  v31_16u, v0_16u  });
    newiopNoret(VOP_ADD, { v0_16u,  v0_16u,  v31_16u });
    newiopNoret(VOP_ADD, { v0_8u,  v0_8u,  v0_8u  });
    newiopNoret(VOP_ADD, { v31_8u, v0_8u,  v0_8u  });
    newiopNoret(VOP_ADD, { v0_8u,  v31_8u, v0_8u  });
    newiopNoret(VOP_ADD, { v0_8u,  v0_8u,  v31_8u });
    newiopNoret(VOP_ADD, { v0_4u,  v0_4u,  v0_4u  });
    newiopNoret(VOP_ADD, { v31_4u, v0_4u,  v0_4u  });
    newiopNoret(VOP_ADD, { v0_4u,  v31_4u, v0_4u  });
    newiopNoret(VOP_ADD, { v0_4u,  v0_4u,  v31_4u });
    newiopNoret(VOP_ADD, { v0_2u,  v0_2u,  v0_2u  });
    newiopNoret(VOP_ADD, { v31_2u, v0_2u,  v0_2u  });
    newiopNoret(VOP_ADD, { v0_2u,  v31_2u, v0_2u  });
    newiopNoret(VOP_ADD, { v0_2u,  v0_2u,  v31_2u });
    newiopNoret(VOP_ADD, { v0_8f,  v0_8f,  v0_8f  });
    newiopNoret(VOP_ADD, { v31_8f, v0_8f,  v0_8f  });
    newiopNoret(VOP_ADD, { v0_8f,  v31_8f, v0_8f  });
    newiopNoret(VOP_ADD, { v0_8f,  v0_8f,  v31_8f });
    newiopNoret(VOP_ADD, { v0_4f,  v0_4f,  v0_4f  });
    newiopNoret(VOP_ADD, { v31_4f, v0_4f,  v0_4f  });
    newiopNoret(VOP_ADD, { v0_4f,  v31_4f, v0_4f  });
    newiopNoret(VOP_ADD, { v0_4f,  v0_4f,  v31_4f });
    newiopNoret(VOP_ADD, { v0_2f,  v0_2f,  v0_2f  });
    newiopNoret(VOP_ADD, { v31_2f, v0_2f,  v0_2f  });
    newiopNoret(VOP_ADD, { v0_2f,  v31_2f, v0_2f  });
    newiopNoret(VOP_ADD, { v0_2f,  v0_2f,  v31_2f });

    newiopNoret(VOP_SUB, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_SUB, { v31_16u, v0_16u,  v0_16u  });
    newiopNoret(VOP_SUB, { v0_16u,  v31_16u, v0_16u  });
    newiopNoret(VOP_SUB, { v0_16u,  v0_16u,  v31_16u });
    newiopNoret(VOP_SUB, { v0_8u,  v0_8u,  v0_8u  });
    newiopNoret(VOP_SUB, { v31_8u, v0_8u,  v0_8u  });
    newiopNoret(VOP_SUB, { v0_8u,  v31_8u, v0_8u  });
    newiopNoret(VOP_SUB, { v0_8u,  v0_8u,  v31_8u });
    newiopNoret(VOP_SUB, { v0_4u,  v0_4u,  v0_4u  });
    newiopNoret(VOP_SUB, { v31_4u, v0_4u,  v0_4u  });
    newiopNoret(VOP_SUB, { v0_4u,  v31_4u, v0_4u  });
    newiopNoret(VOP_SUB, { v0_4u,  v0_4u,  v31_4u });
    newiopNoret(VOP_SUB, { v0_2u,  v0_2u,  v0_2u  });
    newiopNoret(VOP_SUB, { v31_2u, v0_2u,  v0_2u  });
    newiopNoret(VOP_SUB, { v0_2u,  v31_2u, v0_2u  });
    newiopNoret(VOP_SUB, { v0_2u,  v0_2u,  v31_2u });
    newiopNoret(VOP_SUB, { v0_8f,  v0_8f,  v0_8f  });
    newiopNoret(VOP_SUB, { v31_8f, v0_8f,  v0_8f  });
    newiopNoret(VOP_SUB, { v0_8f,  v31_8f, v0_8f  });
    newiopNoret(VOP_SUB, { v0_8f,  v0_8f,  v31_8f });
    newiopNoret(VOP_SUB, { v0_4f,  v0_4f,  v0_4f  });
    newiopNoret(VOP_SUB, { v31_4f, v0_4f,  v0_4f  });
    newiopNoret(VOP_SUB, { v0_4f,  v31_4f, v0_4f  });
    newiopNoret(VOP_SUB, { v0_4f,  v0_4f,  v31_4f });
    newiopNoret(VOP_SUB, { v0_2f,  v0_2f,  v0_2f  });
    newiopNoret(VOP_SUB, { v31_2f, v0_2f,  v0_2f  });
    newiopNoret(VOP_SUB, { v0_2f,  v31_2f, v0_2f  });
    newiopNoret(VOP_SUB, { v0_2f,  v0_2f,  v31_2f });

    newiopNoret(VOP_MUL, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_MUL, { v31_16u, v0_16u,  v0_16u  });
    newiopNoret(VOP_MUL, { v0_16u,  v31_16u, v0_16u  });
    newiopNoret(VOP_MUL, { v0_16u,  v0_16u,  v31_16u });
    newiopNoret(VOP_MUL, { v0_8u,  v0_8u,  v0_8u  });
    newiopNoret(VOP_MUL, { v31_8u, v0_8u,  v0_8u  });
    newiopNoret(VOP_MUL, { v0_8u,  v31_8u, v0_8u  });
    newiopNoret(VOP_MUL, { v0_8u,  v0_8u,  v31_8u });
    newiopNoret(VOP_MUL, { v0_4u,  v0_4u,  v0_4u  });
    newiopNoret(VOP_MUL, { v31_4u, v0_4u,  v0_4u  });
    newiopNoret(VOP_MUL, { v0_4u,  v31_4u, v0_4u  });
    newiopNoret(VOP_MUL, { v0_4u,  v0_4u,  v31_4u });
    newiopNoret(VOP_MUL, { v0_8f,  v0_8f,  v0_8f  });
    newiopNoret(VOP_MUL, { v31_8f, v0_8f,  v0_8f  });
    newiopNoret(VOP_MUL, { v0_8f,  v31_8f, v0_8f  });
    newiopNoret(VOP_MUL, { v0_8f,  v0_8f,  v31_8f });
    newiopNoret(VOP_MUL, { v0_4f,  v0_4f,  v0_4f  });
    newiopNoret(VOP_MUL, { v31_4f, v0_4f,  v0_4f  });
    newiopNoret(VOP_MUL, { v0_4f,  v31_4f, v0_4f  });
    newiopNoret(VOP_MUL, { v0_4f,  v0_4f,  v31_4f });
    newiopNoret(VOP_MUL, { v0_2f,  v0_2f,  v0_2f  });
    newiopNoret(VOP_MUL, { v31_2f, v0_2f,  v0_2f  });
    newiopNoret(VOP_MUL, { v0_2f,  v31_2f, v0_2f  });
    newiopNoret(VOP_MUL, { v0_2f,  v0_2f,  v31_2f });

    newiopNoret(VOP_DIV, { v0_4f,  v0_4f,  v0_4f  });
    newiopNoret(VOP_DIV, { v31_4f, v0_4f,  v0_4f  });
    newiopNoret(VOP_DIV, { v0_4f,  v31_4f, v0_4f  });
    newiopNoret(VOP_DIV, { v0_4f,  v0_4f,  v31_4f });
    newiopNoret(VOP_DIV, { v0_2f,  v0_2f,  v0_2f  });
    newiopNoret(VOP_DIV, { v31_2f, v0_2f,  v0_2f  });
    newiopNoret(VOP_DIV, { v0_2f,  v31_2f, v0_2f  });
    newiopNoret(VOP_DIV, { v0_2f,  v0_2f,  v31_2f });
    newiopNoret(VOP_DIV, { v0_8f,  v0_8f,  v0_8f  });
    newiopNoret(VOP_DIV, { v31_8f, v0_8f,  v0_8f  });
    newiopNoret(VOP_DIV, { v0_8f,  v31_8f, v0_8f  });
    newiopNoret(VOP_DIV, { v0_8f,  v0_8f,  v31_8f });

    newiopNoret(VOP_SAL, { v0_16u,  v0_16u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v31_16u,  v0_16u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_16u,  v31_16u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_16u,  v0_16u,  argIImm(7)  });
    newiopNoret(VOP_SAL, { v0_8u,  v0_8u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v31_8u,  v0_8u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_8u,  v31_8u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_8u,  v0_8u,  argIImm(15)  });
    newiopNoret(VOP_SAL, { v0_4u,  v0_4u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v31_4u,  v0_4u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_4u,  v31_4u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_4u,  v0_4u,  argIImm(31)  });
    newiopNoret(VOP_SAL, { v0_2u,  v0_2u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v31_2u,  v0_2u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_2u,  v31_2u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_2u,  v0_2u,  argIImm(63)  });

    newiopNoret(VOP_SAL, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_SAL, { v31_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_SAL, { v0_16u,  v31_16u,  v0_16u  });
    newiopNoret(VOP_SAL, { v0_16u,  v0_16u,  v31_16u  });
    newiopNoret(VOP_SAL, { v0_8u,  v0_8u,  v0_8u  });
    newiopNoret(VOP_SAL, { v31_8u,  v0_8u,  v0_8u  });
    newiopNoret(VOP_SAL, { v0_8u,  v31_8u,  v0_8u  });
    newiopNoret(VOP_SAL, { v0_8u,  v0_8u,  v31_8u  });
    newiopNoret(VOP_SAL, { v0_4u,  v0_4u,  v0_4u  });
    newiopNoret(VOP_SAL, { v31_4u,  v0_4u,  v0_4u  });
    newiopNoret(VOP_SAL, { v0_4u,  v31_4u,  v0_4u  });
    newiopNoret(VOP_SAL, { v0_4u,  v0_4u,  v31_4u  });
    newiopNoret(VOP_SAL, { v0_2u,  v0_2u,  v0_2u  });
    newiopNoret(VOP_SAL, { v31_2u,  v0_2u,  v0_2u  });
    newiopNoret(VOP_SAL, { v0_2u,  v31_2u,  v0_2u  });
    newiopNoret(VOP_SAL, { v0_2u,  v0_2u,  v31_2u  });

    newiopNoret(VOP_SAL, { v0_16s,  v0_16s,  v0_16s  });
    newiopNoret(VOP_SAL, { v31_16s,  v0_16s,  v0_16s  });
    newiopNoret(VOP_SAL, { v0_16s,  v31_16s,  v0_16s  });
    newiopNoret(VOP_SAL, { v0_16s,  v0_16s,  v31_16s  });
    newiopNoret(VOP_SAL, { v0_8s,  v0_8s,  v0_8s  });
    newiopNoret(VOP_SAL, { v31_8s,  v0_8s,  v0_8s  });
    newiopNoret(VOP_SAL, { v0_8s,  v31_8s,  v0_8s  });
    newiopNoret(VOP_SAL, { v0_8s,  v0_8s,  v31_8s  });
    newiopNoret(VOP_SAL, { v0_4s,  v0_4s,  v0_4s  });
    newiopNoret(VOP_SAL, { v31_4s,  v0_4s,  v0_4s  });
    newiopNoret(VOP_SAL, { v0_4s,  v31_4s,  v0_4s  });
    newiopNoret(VOP_SAL, { v0_4s,  v0_4s,  v31_4s  });
    newiopNoret(VOP_SAL, { v0_2s,  v0_2s,  v0_2s  });
    newiopNoret(VOP_SAL, { v31_2s,  v0_2s,  v0_2s  });
    newiopNoret(VOP_SAL, { v0_2s,  v31_2s,  v0_2s  });
    newiopNoret(VOP_SAL, { v0_2s,  v0_2s,  v31_2s  });

    newiopNoret(VOP_SAR, { v0_16u,  v0_16u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_16u,  v0_16u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_16u,  v31_16u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_16u,  v0_16u,  argIImm(7)  });
    newiopNoret(VOP_SAR, { v0_8u,  v0_8u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_8u,  v0_8u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_8u,  v31_8u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_8u,  v0_8u,  argIImm(15)  });
    newiopNoret(VOP_SAR, { v0_4u,  v0_4u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_4u,  v0_4u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_4u,  v31_4u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_4u,  v0_4u,  argIImm(31)  });
    newiopNoret(VOP_SAR, { v0_2u,  v0_2u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_2u,  v0_2u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_2u,  v31_2u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_2u,  v0_2u,  argIImm(63)  });

    newiopNoret(VOP_SAR, { v0_16s,  v0_16s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_16s,  v0_16s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_16s,  v31_16s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_16s,  v0_16s,  argIImm(7)  });
    newiopNoret(VOP_SAR, { v0_8s,  v0_8s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_8s,  v0_8s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_8s,  v31_8s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_8s,  v0_8s,  argIImm(15)  });
    newiopNoret(VOP_SAR, { v0_4s,  v0_4s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_4s,  v0_4s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_4s,  v31_4s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_4s,  v0_4s,  argIImm(31)  });
    newiopNoret(VOP_SAR, { v0_2s,  v0_2s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_2s,  v0_2s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_2s,  v31_2s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_2s,  v0_2s,  argIImm(63)  });

    newiopNoret(VOP_AND, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_AND, { v31_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_AND, { v0_16u,  v31_16u,  v0_16u  });
    newiopNoret(VOP_AND, { v0_16u,  v0_16u,  v31_16u  });

    newiopNoret(VOP_OR, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_OR, { v31_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_OR, { v0_16u,  v31_16u,  v0_16u  });
    newiopNoret(VOP_OR, { v0_16u,  v0_16u,  v31_16u  });

    newiopNoret(VOP_XOR, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_XOR, { v31_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_XOR, { v0_16u,  v31_16u,  v0_16u  });
    newiopNoret(VOP_XOR, { v0_16u,  v0_16u,  v31_16u  });

    newiopNoret(VOP_NOT, { v0_16u,  v0_16u  });
    newiopNoret(VOP_NOT, { v31_16u,  v0_16u });
    newiopNoret(VOP_NOT, { v0_16u,  v31_16u });

    newiopNoret(OP_MOV, { v0_16u,  argIImm(0)  });
    newiopNoret(OP_MOV, { v31_16u,  argIImm(0)  });
    newiopNoret(OP_MOV, { v0_16u,  argIImm(255)  });
    newiopNoret(OP_MOV, { v0_8u,  argIImm(0)  });
    newiopNoret(OP_MOV, { v31_8u,  argIImm(0)  });
    newiopNoret(OP_MOV, { v0_8u,  argIImm(255)  });
    newiopNoret(OP_MOV, { v0_4u,  argIImm(0)  });
    newiopNoret(OP_MOV, { v31_4u,  argIImm(0)  });
    newiopNoret(OP_MOV, { v0_4u,  argIImm(255)  });
    newiopNoret(OP_MOV, {  v0_8s, argIImm(  -1) });
    newiopNoret(OP_MOV, { v31_8s, argIImm(  -1) });
    newiopNoret(OP_MOV, {  v0_8s, argIImm(-256) });
    newiopNoret(OP_MOV, {  v0_4s, argIImm(  -1) });
    newiopNoret(OP_MOV, { v31_4s, argIImm(  -1) });
    newiopNoret(OP_MOV, {  v0_4s, argIImm(-256) });

    newiopNoret(VOP_MIN, { v0_8f,  v0_8f, v0_8f  });
    newiopNoret(VOP_MIN, { v31_8f, v0_8f, v0_8f  });
    newiopNoret(VOP_MIN, { v0_8f, v31_8f, v0_8f  });
    newiopNoret(VOP_MIN, { v0_8f,  v0_8f, v31_8f  });
    newiopNoret(VOP_MIN, { v0_4f,  v0_4f, v0_4f  });
    newiopNoret(VOP_MIN, { v31_4f, v0_4f, v0_4f  });
    newiopNoret(VOP_MIN, { v0_4f, v31_4f, v0_4f  });
    newiopNoret(VOP_MIN, { v0_4f,  v0_4f, v31_4f  });
    newiopNoret(VOP_MIN, { v0_2f,  v0_2f, v0_2f  });
    newiopNoret(VOP_MIN, { v31_2f, v0_2f, v0_2f  });
    newiopNoret(VOP_MIN, { v0_2f, v31_2f, v0_2f  });
    newiopNoret(VOP_MIN, { v0_2f,  v0_2f, v31_2f  });

    newiopNoret(VOP_MAX, { v0_8f,  v0_8f, v0_8f  });
    newiopNoret(VOP_MAX, { v31_8f, v0_8f, v0_8f  });
    newiopNoret(VOP_MAX, { v0_8f, v31_8f, v0_8f  });
    newiopNoret(VOP_MAX, { v0_8f,  v0_8f, v31_8f  });
    newiopNoret(VOP_MAX, { v0_4f,  v0_4f, v0_4f  });
    newiopNoret(VOP_MAX, { v31_4f, v0_4f, v0_4f  });
    newiopNoret(VOP_MAX, { v0_4f, v31_4f, v0_4f  });
    newiopNoret(VOP_MAX, { v0_4f,  v0_4f, v31_4f  });
    newiopNoret(VOP_MAX, { v0_2f,  v0_2f, v0_2f  });
    newiopNoret(VOP_MAX, { v31_2f, v0_2f, v0_2f  });
    newiopNoret(VOP_MAX, { v0_2f, v31_2f, v0_2f  });
    newiopNoret(VOP_MAX, { v0_2f,  v0_2f, v31_2f  });

    newiopNoret(VOP_FMA, { v0_8f , v0_8f ,  v0_8f, v0_8f  });
    newiopNoret(VOP_FMA, { v31_8f, v31_8f,  v0_8f, v0_8f });
    newiopNoret(VOP_FMA, { v0_8f , v0_8f ,  v31_8f, v0_8f });
    newiopNoret(VOP_FMA, { v0_8f , v0_8f ,  v0_8f, v31_8f });
    newiopNoret(VOP_FMA, { v0_4f , v0_4f ,  v0_4f, v0_4f  });
    newiopNoret(VOP_FMA, { v31_4f, v31_4f,  v0_4f, v0_4f });
    newiopNoret(VOP_FMA, { v0_4f , v0_4f ,  v31_4f, v0_4f });
    newiopNoret(VOP_FMA, { v0_4f , v0_4f ,  v0_4f, v31_4f });
    newiopNoret(VOP_FMA, { v0_2f , v0_2f ,  v0_2f, v0_2f  });
    newiopNoret(VOP_FMA, { v31_2f, v31_2f,  v0_2f, v0_2f });
    newiopNoret(VOP_FMA, { v0_2f , v0_2f ,  v31_2f, v0_2f });
    newiopNoret(VOP_FMA, { v0_2f , v0_2f ,  v0_2f, v31_2f });

    newiopNoret(VOP_TRUNC, { v0_8s , v0_8f  });
    newiopNoret(VOP_TRUNC, { v31_8s, v0_8f  });
    newiopNoret(VOP_TRUNC, { v0_8s , v31_8f });
    newiopNoret(VOP_TRUNC, { v0_4s , v0_4f  });
    newiopNoret(VOP_TRUNC, { v31_4s, v0_4f  });
    newiopNoret(VOP_TRUNC, { v0_4s , v31_4f });
    newiopNoret(VOP_TRUNC, { v0_2s , v0_2f  });
    newiopNoret(VOP_TRUNC, { v31_2s, v0_2f  });
    newiopNoret(VOP_TRUNC, { v0_2s , v31_2f });
    newiopNoret(VOP_TRUNC, { v0_8u , v0_8f  });
    newiopNoret(VOP_TRUNC, { v31_8u, v0_8f  });
    newiopNoret(VOP_TRUNC, { v0_8u , v31_8f });
    newiopNoret(VOP_TRUNC, { v0_4u , v0_4f  });
    newiopNoret(VOP_TRUNC, { v31_4u, v0_4f  });
    newiopNoret(VOP_TRUNC, { v0_4u , v31_4f });
    newiopNoret(VOP_TRUNC, { v0_2u , v0_2f  });
    newiopNoret(VOP_TRUNC, { v31_2u, v0_2f  });
    newiopNoret(VOP_TRUNC, { v0_2u , v31_2f });

    newiopNoret(VOP_FLOOR, { v0_8s , v0_8f  });
    newiopNoret(VOP_FLOOR, { v31_8s , v0_8f  });
    newiopNoret(VOP_FLOOR, { v0_8s , v31_8f  });
    newiopNoret(VOP_FLOOR, { v0_4s , v0_4f  });
    newiopNoret(VOP_FLOOR, { v31_4s , v0_4f  });
    newiopNoret(VOP_FLOOR, { v0_4s , v31_4f  });
    newiopNoret(VOP_FLOOR, { v0_2s , v0_2f  });
    newiopNoret(VOP_FLOOR, { v31_2s , v0_2f  });
    newiopNoret(VOP_FLOOR, { v0_2s , v31_2f  });
    newiopNoret(VOP_FLOOR, { v0_8u , v0_8f  });
    newiopNoret(VOP_FLOOR, { v31_8u , v0_8f  });
    newiopNoret(VOP_FLOOR, { v0_8u , v31_8f  });
    newiopNoret(VOP_FLOOR, { v0_4u , v0_4f  });
    newiopNoret(VOP_FLOOR, { v31_4u , v0_4f  });
    newiopNoret(VOP_FLOOR, { v0_4u , v31_4f  });
    newiopNoret(VOP_FLOOR, { v0_2u , v0_2f  });
    newiopNoret(VOP_FLOOR, { v31_2u , v0_2f  });
    newiopNoret(VOP_FLOOR, { v0_2u , v31_2f  });

    newiopNoret(VOP_CAST, { v0_8f , v0_8s  });
    newiopNoret(VOP_CAST, { v31_8f, v0_8s  });
    newiopNoret(VOP_CAST, { v0_8f , v31_8s });
    newiopNoret(VOP_CAST, { v0_4f , v0_4s  });
    newiopNoret(VOP_CAST, { v31_4f, v0_4s  });
    newiopNoret(VOP_CAST, { v0_4f , v31_4s });
    newiopNoret(VOP_CAST, { v0_2f , v0_2s  });
    newiopNoret(VOP_CAST, { v31_2f, v0_2s  });
    newiopNoret(VOP_CAST, { v0_2f , v31_2s });
    newiopNoret(VOP_CAST, { v0_8f , v0_8u  });
    newiopNoret(VOP_CAST, { v31_8f, v0_8u  });
    newiopNoret(VOP_CAST, { v0_8f , v31_8u });
    newiopNoret(VOP_CAST, { v0_4f , v0_4u  });
    newiopNoret(VOP_CAST, { v31_4f, v0_4u  });
    newiopNoret(VOP_CAST, { v0_4f , v31_4u });
    newiopNoret(VOP_CAST, { v0_2f , v0_2u  });
    newiopNoret(VOP_CAST, { v31_2f, v0_2u  });
    newiopNoret(VOP_CAST, { v0_2f , v31_2u });

    newiopNoret(VOP_GT, { v0_8u , v0_8f , v0_8f });
    newiopNoret(VOP_GT, { v31_8u, v0_8f , v0_8f });
    newiopNoret(VOP_GT, { v0_8u , v31_8f, v0_8f });
    newiopNoret(VOP_GT, { v0_8u , v0_8f , v31_8f});
    newiopNoret(VOP_GT, { v0_4u , v0_4f , v0_4f });
    newiopNoret(VOP_GT, { v31_4u, v0_4f , v0_4f });
    newiopNoret(VOP_GT, { v0_4u , v31_4f, v0_4f });
    newiopNoret(VOP_GT, { v0_4u , v0_4f , v31_4f});
    newiopNoret(VOP_GT, { v0_2u , v0_2f , v0_2f });
    newiopNoret(VOP_GT, { v31_2u, v0_2f , v0_2f });
    newiopNoret(VOP_GT, { v0_2u , v31_2f, v0_2f });
    newiopNoret(VOP_GT, { v0_2u , v0_2f , v31_2f});

    newiopNoret(VOP_GE, { v0_8u , v0_8f , v0_8f });
    newiopNoret(VOP_GE, { v31_8u, v0_8f , v0_8f });
    newiopNoret(VOP_GE, { v0_8u , v31_8f, v0_8f });
    newiopNoret(VOP_GE, { v0_8u , v0_8f , v31_8f});
    newiopNoret(VOP_GE, { v0_4u , v0_4f , v0_4f });
    newiopNoret(VOP_GE, { v31_4u, v0_4f , v0_4f });
    newiopNoret(VOP_GE, { v0_4u , v31_4f, v0_4f });
    newiopNoret(VOP_GE, { v0_4u , v0_4f , v31_4f});
    newiopNoret(VOP_GE, { v0_2u , v0_2f , v0_2f });
    newiopNoret(VOP_GE, { v31_2u, v0_2f , v0_2f });
    newiopNoret(VOP_GE, { v0_2u , v31_2f, v0_2f });
    newiopNoret(VOP_GE, { v0_2u , v0_2f , v31_2f});

    newiopNoret(VOP_EQ, { v0_8u , v0_8f , v0_8f });
    newiopNoret(VOP_EQ, { v31_8u, v0_8f , v0_8f });
    newiopNoret(VOP_EQ, { v0_8u , v31_8f, v0_8f });
    newiopNoret(VOP_EQ, { v0_8u , v0_8f , v31_8f});
    newiopNoret(VOP_EQ, { v0_4u , v0_4f , v0_4f });
    newiopNoret(VOP_EQ, { v31_4u, v0_4f , v0_4f });
    newiopNoret(VOP_EQ, { v0_4u , v31_4f, v0_4f });
    newiopNoret(VOP_EQ, { v0_4u , v0_4f , v31_4f});
    newiopNoret(VOP_EQ, { v0_2u , v0_2f , v0_2f });
    newiopNoret(VOP_EQ, { v31_2u, v0_2f , v0_2f });
    newiopNoret(VOP_EQ, { v0_2u , v31_2f, v0_2f });
    newiopNoret(VOP_EQ, { v0_2u , v0_2f , v31_2f});

    newiopNoret(VOP_NEG, { v0_16s , v0_16s });
    newiopNoret(VOP_NEG, { v31_16s , v0_16s });
    newiopNoret(VOP_NEG, { v0_16s , v31_16s });
    newiopNoret(VOP_NEG, { v0_8s , v0_8s });
    newiopNoret(VOP_NEG, { v31_8s , v0_8s });
    newiopNoret(VOP_NEG, { v0_8s , v31_8s });
    newiopNoret(VOP_NEG, { v0_4s , v0_4s });
    newiopNoret(VOP_NEG, { v31_4s , v0_4s });
    newiopNoret(VOP_NEG, { v0_4s , v31_4s });
    newiopNoret(VOP_NEG, { v0_2s , v0_2s });
    newiopNoret(VOP_NEG, { v31_2s , v0_2s });
    newiopNoret(VOP_NEG, { v0_2s , v31_2s });
    newiopNoret(VOP_NEG, { v0_8f , v0_8f });
    newiopNoret(VOP_NEG, { v31_8f , v0_8f });
    newiopNoret(VOP_NEG, { v0_8f , v31_8f });
    newiopNoret(VOP_NEG, { v0_4f , v0_4f });
    newiopNoret(VOP_NEG, { v31_4f , v0_4f });
    newiopNoret(VOP_NEG, { v0_4f , v31_4f });
    newiopNoret(VOP_NEG, { v0_2f , v0_2f });
    newiopNoret(VOP_NEG, { v31_2f , v0_2f });
    newiopNoret(VOP_NEG, { v0_2f , v31_2f });

    newiopNoret(OP_ARM_MOVK, { x0 , argIImm(0)     , argIImm(16) });
    newiopNoret(OP_ARM_MOVK, { x15, argIImm(0)     , argIImm(16) });
    newiopNoret(OP_ARM_MOVK, { x0 , argIImm(0xffff), argIImm(16) });
    newiopNoret(OP_ARM_MOVK, { x0 , argIImm(0)     , argIImm(32) });
    newiopNoret(OP_ARM_MOVK, { x15, argIImm(0)     , argIImm(32) });
    newiopNoret(OP_ARM_MOVK, { x0 , argIImm(0xffff), argIImm(32) });
    newiopNoret(OP_ARM_MOVK, { x0 , argIImm(0)     , argIImm(48) });
    newiopNoret(OP_ARM_MOVK, { x15, argIImm(0)     , argIImm(48) });
    newiopNoret(OP_ARM_MOVK, { x0 , argIImm(0xffff), argIImm(48) });

    newiopNoret(VOP_BROADCAST, { v0_16s, w0 });
    newiopNoret(VOP_BROADCAST, { v31_16s, w0 });
    newiopNoret(VOP_BROADCAST, { v0_16s, w15 });
    newiopNoret(VOP_BROADCAST, { v0_8s, w0 });
    newiopNoret(VOP_BROADCAST, { v31_8s, w0 });
    newiopNoret(VOP_BROADCAST, { v0_8s, w15 });
    newiopNoret(VOP_BROADCAST, { v0_4s, w0 });
    newiopNoret(VOP_BROADCAST, { v31_4s, w0 });
    newiopNoret(VOP_BROADCAST, { v0_4s, w15 });
    newiopNoret(VOP_BROADCAST, { v0_2s, x0 });
    newiopNoret(VOP_BROADCAST, { v31_2s, x0 });
    newiopNoret(VOP_BROADCAST, { v0_2s, x15 });

    newiopNoret(VOP_BROADCAST, { v0_16u,  v0_16u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v31_16u, v0_16u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_16u, v31_16u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_16u,  v0_16u, argIImm(15)});
    newiopNoret(VOP_BROADCAST, { v0_8u,  v0_8u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v31_8u, v0_8u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_8u, v31_8u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_8u,  v0_8u, argIImm(7)});
    newiopNoret(VOP_BROADCAST, { v0_4u,  v0_4u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v31_4u, v0_4u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_4u, v31_4u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_4u,  v0_4u, argIImm(3)});
    newiopNoret(VOP_BROADCAST, { v0_2u,  v0_2u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v31_2u, v0_2u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_2u, v31_2u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_2u,  v0_2u, argIImm(1)});

    newiopNoret(VOP_CAST_LOW, { v0_8s, v0_16s });
    newiopNoret(VOP_CAST_LOW, { v31_8s, v0_16s });
    newiopNoret(VOP_CAST_LOW, { v0_8s, v31_16s });
    newiopNoret(VOP_CAST_LOW, { v0_4s, v0_8s });
    newiopNoret(VOP_CAST_LOW, { v31_4s, v0_8s });
    newiopNoret(VOP_CAST_LOW, { v0_4s, v31_8s });
    newiopNoret(VOP_CAST_LOW, { v0_2s, v0_4s });
    newiopNoret(VOP_CAST_LOW, { v31_2s, v0_4s });
    newiopNoret(VOP_CAST_LOW, { v0_2s, v31_4s });
    newiopNoret(VOP_CAST_LOW, { v0_8u, v0_16u });
    newiopNoret(VOP_CAST_LOW, { v31_8u, v0_16u });
    newiopNoret(VOP_CAST_LOW, { v0_8u, v31_16u });
    newiopNoret(VOP_CAST_LOW, { v0_4u, v0_8u });
    newiopNoret(VOP_CAST_LOW, { v31_4u, v0_8u });
    newiopNoret(VOP_CAST_LOW, { v0_4u, v31_8u });
    newiopNoret(VOP_CAST_LOW, { v0_2u, v0_4u });
    newiopNoret(VOP_CAST_LOW, { v31_2u, v0_4u });
    newiopNoret(VOP_CAST_LOW, { v0_2u, v31_4u });

    newiopNoret(VOP_CAST_HIGH, { v0_8s, v0_16s });
    newiopNoret(VOP_CAST_HIGH, { v31_8s, v0_16s });
    newiopNoret(VOP_CAST_HIGH, { v0_8s, v31_16s });
    newiopNoret(VOP_CAST_HIGH, { v0_4s, v0_8s });
    newiopNoret(VOP_CAST_HIGH, { v31_4s, v0_8s });
    newiopNoret(VOP_CAST_HIGH, { v0_4s, v31_8s });
    newiopNoret(VOP_CAST_HIGH, { v0_2s, v0_4s });
    newiopNoret(VOP_CAST_HIGH, { v31_2s, v0_4s });
    newiopNoret(VOP_CAST_HIGH, { v0_2s, v31_4s });
    newiopNoret(VOP_CAST_HIGH, { v0_8u, v0_16u });
    newiopNoret(VOP_CAST_HIGH, { v31_8u, v0_16u });
    newiopNoret(VOP_CAST_HIGH, { v0_8u, v31_16u });
    newiopNoret(VOP_CAST_HIGH, { v0_4u, v0_8u });
    newiopNoret(VOP_CAST_HIGH, { v31_4u, v0_8u });
    newiopNoret(VOP_CAST_HIGH, { v0_4u, v31_8u });
    newiopNoret(VOP_CAST_HIGH, { v0_2u, v0_4u });
    newiopNoret(VOP_CAST_HIGH, { v31_2u, v0_4u });
    newiopNoret(VOP_CAST_HIGH, { v0_2u, v31_4u });

    newiopNoret(VOP_SHRINK_LOW, { v0_16s, v0_8s });
    newiopNoret(VOP_SHRINK_LOW, { v31_16s, v0_8s });
    newiopNoret(VOP_SHRINK_LOW, { v0_16s, v31_8s });
    newiopNoret(VOP_SHRINK_LOW, { v0_8s, v0_4s });
    newiopNoret(VOP_SHRINK_LOW, { v31_8s, v0_4s });
    newiopNoret(VOP_SHRINK_LOW, { v0_8s, v31_4s });
    newiopNoret(VOP_SHRINK_LOW, { v0_4s, v0_2s });
    newiopNoret(VOP_SHRINK_LOW, { v31_4s, v0_2s });
    newiopNoret(VOP_SHRINK_LOW, { v0_4s, v31_2s });

    newiopNoret(VOP_SHRINK_HIGH, { v0_16s, v0_8s });
    newiopNoret(VOP_SHRINK_HIGH, { v31_16s, v0_8s });
    newiopNoret(VOP_SHRINK_HIGH, { v0_16s, v31_8s });
    newiopNoret(VOP_SHRINK_HIGH, { v0_8s, v0_4s });
    newiopNoret(VOP_SHRINK_HIGH, { v31_8s, v0_4s });
    newiopNoret(VOP_SHRINK_HIGH, { v0_8s, v31_4s });
    newiopNoret(VOP_SHRINK_HIGH, { v0_4s, v0_2s });
    newiopNoret(VOP_SHRINK_HIGH, { v31_4s, v0_2s });
    newiopNoret(VOP_SHRINK_HIGH, { v0_4s, v31_2s });

    newiopNoret(VOP_REDUCE_MAX, { v0_16s, v0_16s });
    newiopNoret(VOP_REDUCE_MAX, { v31_16s, v0_16s });
    newiopNoret(VOP_REDUCE_MAX, { v0_16s, v31_16s });
    newiopNoret(VOP_REDUCE_MAX, { v0_8s, v0_8s });
    newiopNoret(VOP_REDUCE_MAX, { v31_8s, v0_8s });
    newiopNoret(VOP_REDUCE_MAX, { v0_8s, v31_8s });
    newiopNoret(VOP_REDUCE_MAX, { v0_4s, v0_4s });
    newiopNoret(VOP_REDUCE_MAX, { v31_4s, v0_4s });
    newiopNoret(VOP_REDUCE_MAX, { v0_4s, v31_4s });
    newiopNoret(VOP_REDUCE_MAX, { v0_16u, v0_16u });
    newiopNoret(VOP_REDUCE_MAX, { v31_16u, v0_16u });
    newiopNoret(VOP_REDUCE_MAX, { v0_16u, v31_16u });
    newiopNoret(VOP_REDUCE_MAX, { v0_8u, v0_8u });
    newiopNoret(VOP_REDUCE_MAX, { v31_8u, v0_8u });
    newiopNoret(VOP_REDUCE_MAX, { v0_8u, v31_8u });
    newiopNoret(VOP_REDUCE_MAX, { v0_4u, v0_4u });
    newiopNoret(VOP_REDUCE_MAX, { v31_4u, v0_4u });
    newiopNoret(VOP_REDUCE_MAX, { v0_4u, v31_4u });
    newiopNoret(VOP_REDUCE_MAX, { v0_8f, v0_8f });
    newiopNoret(VOP_REDUCE_MAX, { v31_8f, v0_8f });
    newiopNoret(VOP_REDUCE_MAX, { v0_8f, v31_8f });
    newiopNoret(VOP_REDUCE_MAX, { v0_4f, v0_4f });
    newiopNoret(VOP_REDUCE_MAX, { v31_4f, v0_4f });
    newiopNoret(VOP_REDUCE_MAX, { v0_4f, v31_4f });

    newiopNoret(VOP_REDUCE_MIN, { v0_16s, v0_16s });
    newiopNoret(VOP_REDUCE_MIN, { v31_16s, v0_16s });
    newiopNoret(VOP_REDUCE_MIN, { v0_16s, v31_16s });
    newiopNoret(VOP_REDUCE_MIN, { v0_8s, v0_8s });
    newiopNoret(VOP_REDUCE_MIN, { v31_8s, v0_8s });
    newiopNoret(VOP_REDUCE_MIN, { v0_8s, v31_8s });
    newiopNoret(VOP_REDUCE_MIN, { v0_4s, v0_4s });
    newiopNoret(VOP_REDUCE_MIN, { v31_4s, v0_4s });
    newiopNoret(VOP_REDUCE_MIN, { v0_4s, v31_4s });
    newiopNoret(VOP_REDUCE_MIN, { v0_16u, v0_16u });
    newiopNoret(VOP_REDUCE_MIN, { v31_16u, v0_16u });
    newiopNoret(VOP_REDUCE_MIN, { v0_16u, v31_16u });
    newiopNoret(VOP_REDUCE_MIN, { v0_8u, v0_8u });
    newiopNoret(VOP_REDUCE_MIN, { v31_8u, v0_8u });
    newiopNoret(VOP_REDUCE_MIN, { v0_8u, v31_8u });
    newiopNoret(VOP_REDUCE_MIN, { v0_4u, v0_4u });
    newiopNoret(VOP_REDUCE_MIN, { v31_4u, v0_4u });
    newiopNoret(VOP_REDUCE_MIN, { v0_4u, v31_4u });
    newiopNoret(VOP_REDUCE_MIN, { v0_8f, v0_8f });
    newiopNoret(VOP_REDUCE_MIN, { v31_8f, v0_8f });
    newiopNoret(VOP_REDUCE_MIN, { v0_8f, v31_8f });
    newiopNoret(VOP_REDUCE_MIN, { v0_4f, v0_4f });
    newiopNoret(VOP_REDUCE_MIN, { v31_4f, v0_4f });
    newiopNoret(VOP_REDUCE_MIN, { v0_4f, v31_4f });

    newiopNoret(VOP_ARM_LD1, { v0_16s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v31_16s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v0_16s, argIImm(15), x0});
    newiopNoret(VOP_ARM_LD1, { v0_16s, argIImm(0), x15});

    newiopNoret(VOP_ARM_LD1, { v0_8s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v31_8s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v0_8s, argIImm(7), x0});
    newiopNoret(VOP_ARM_LD1, { v0_8s, argIImm(0), x15});

    newiopNoret(VOP_ARM_LD1, { v0_4s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v31_4s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v0_4s, argIImm(3), x0});
    newiopNoret(VOP_ARM_LD1, { v0_4s, argIImm(0), x15});

    newiopNoret(VOP_ARM_LD1, { v0_2s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v31_2s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v0_2s, argIImm(1), x0});
    newiopNoret(VOP_ARM_LD1, { v0_2s, argIImm(0), x15});

    newiopNoret(VOP_ARM_LD2, {  v0_16s, v1_16s,  x0});
    newiopNoret(VOP_ARM_LD2, { v31_16s, v0_16s,  x0});
    newiopNoret(VOP_ARM_LD2, {  v0_16s, v1_16s, x15});

    newiopNoret(VOP_ARM_LD2, {  v0_8s, v1_8s,  x0});
    newiopNoret(VOP_ARM_LD2, { v31_8s, v0_8s,  x0});
    newiopNoret(VOP_ARM_LD2, {  v0_8s, v1_8s, x15});

    newiopNoret(VOP_ARM_LD2, {  v0_4s, v1_4s,  x0});
    newiopNoret(VOP_ARM_LD2, { v31_4s, v0_4s,  x0});
    newiopNoret(VOP_ARM_LD2, {  v0_4s, v1_4s, x15});

    newiopNoret(VOP_ARM_LD2, {  v0_2s, v1_2s,  x0});
    newiopNoret(VOP_ARM_LD2, { v31_2s, v0_2s,  x0});
    newiopNoret(VOP_ARM_LD2, {  v0_2s, v1_2s, x15});

    newiopNoret(VOP_ARM_ST1, {  x0, v0_16s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v31_16s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v0_16s, argIImm(15)});
    newiopNoret(VOP_ARM_ST1, {  x15, v0_16s, argIImm(0)});

    newiopNoret(VOP_ARM_ST1, {  x0, v0_8s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v31_8s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v0_8s, argIImm(7)});
    newiopNoret(VOP_ARM_ST1, {  x15, v0_8s, argIImm(0)});

    newiopNoret(VOP_ARM_ST1, {  x0, v0_4s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v31_4s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v0_4s, argIImm(3)});
    newiopNoret(VOP_ARM_ST1, {  x15, v0_4s, argIImm(0)});

    newiopNoret(VOP_ARM_ST1, {  x0, v0_2s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v31_2s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v0_2s, argIImm(1)});
    newiopNoret(VOP_ARM_ST1, {  x15, v0_2s, argIImm(0)});

    newiopNoret(VOP_ARM_EXT, { v0_16s, v0_16s, v0_16s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v31_16s, v0_16s, v0_16s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_16s, v31_16s, v0_16s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_16s, v0_16s, v31_16s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_16s, v0_16s, v0_16s, argIImm(15)});

    newiopNoret(VOP_ARM_EXT, { v0_8s, v0_8s, v0_8s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v31_8s, v0_8s, v0_8s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_8s, v31_8s, v0_8s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_8s, v0_8s, v31_8s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_8s, v0_8s, v0_8s, argIImm(7)});

    newiopNoret(VOP_ARM_EXT, { v0_4s, v0_4s, v0_4s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v31_4s, v0_4s, v0_4s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_4s, v31_4s, v0_4s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_4s, v0_4s, v31_4s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_4s, v0_4s, v0_4s, argIImm(3)});

    newiopNoret(VOP_ARM_EXT, { v0_2s, v0_2s, v0_2s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v31_2s, v0_2s, v0_2s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_2s, v31_2s, v0_2s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_2s, v0_2s, v31_2s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_2s, v0_2s, v0_2s, argIImm(1)});

    newiopNoret(VOP_FMA, { v0_8f, v0_8f, v0_8f, v0_8f, argIImm(0)});
    newiopNoret(VOP_FMA, { v31_8f, v31_8f, v0_8f, v0_8f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_8f, v0_8f, v31_8f, v0_8f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_8f, v0_8f, v0_8f, v15_8f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_8f, v0_8f, v0_8f, v0_8f, argIImm(7)});

    newiopNoret(VOP_FMA, { v0_4f, v0_4f, v0_4f, v0_4f, argIImm(0)});
    newiopNoret(VOP_FMA, { v31_4f, v31_4f, v0_4f, v0_4f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_4f, v0_4f, v31_4f, v0_4f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_4f, v0_4f, v0_4f, v31_4f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_4f, v0_4f, v0_4f, v0_4f, argIImm(3)});

    newiopNoret(VOP_FMA, { v0_2f, v0_2f, v0_2f, v0_2f, argIImm(0)});
    newiopNoret(VOP_FMA, { v31_2f, v31_2f, v0_2f, v0_2f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_2f, v0_2f, v31_2f, v0_2f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_2f, v0_2f, v0_2f, v31_2f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_2f, v0_2f, v0_2f, v0_2f, argIImm(1)});

    newiopNoret(VOP_GT, { v0_16u, v0_16u, v0_16u});
    newiopNoret(VOP_GT, { v31_16u, v0_16u, v0_16u});
    newiopNoret(VOP_GT, { v0_16u, v31_16u, v0_16u});
    newiopNoret(VOP_GT, { v0_16u, v0_16u, v31_16u});
    newiopNoret(VOP_GT, { v0_8u, v0_8u, v0_8u});
    newiopNoret(VOP_GT, { v31_8u, v0_8u, v0_8u});
    newiopNoret(VOP_GT, { v0_8u, v31_8u, v0_8u});
    newiopNoret(VOP_GT, { v0_8u, v0_8u, v31_8u});
    newiopNoret(VOP_GT, { v0_4u, v0_4u, v0_4u});
    newiopNoret(VOP_GT, { v31_4u, v0_4u, v0_4u});
    newiopNoret(VOP_GT, { v0_4u, v31_4u, v0_4u});
    newiopNoret(VOP_GT, { v0_4u, v0_4u, v31_4u});
    newiopNoret(VOP_GT, { v0_2u, v0_2u, v0_2u});
    newiopNoret(VOP_GT, { v31_2u, v0_2u, v0_2u});
    newiopNoret(VOP_GT, { v0_2u, v31_2u, v0_2u});
    newiopNoret(VOP_GT, { v0_2u, v0_2u, v31_2u});

    newiopNoret(VOP_GE, { v0_16u, v0_16u, v0_16u});
    newiopNoret(VOP_GE, { v31_16u, v0_16u, v0_16u});
    newiopNoret(VOP_GE, { v0_16u, v31_16u, v0_16u});
    newiopNoret(VOP_GE, { v0_16u, v0_16u, v31_16u});
    newiopNoret(VOP_GE, { v0_8u, v0_8u, v0_8u});
    newiopNoret(VOP_GE, { v31_8u, v0_8u, v0_8u});
    newiopNoret(VOP_GE, { v0_8u, v31_8u, v0_8u});
    newiopNoret(VOP_GE, { v0_8u, v0_8u, v31_8u});
    newiopNoret(VOP_GE, { v0_4u, v0_4u, v0_4u});
    newiopNoret(VOP_GE, { v31_4u, v0_4u, v0_4u});
    newiopNoret(VOP_GE, { v0_4u, v31_4u, v0_4u});
    newiopNoret(VOP_GE, { v0_4u, v0_4u, v31_4u});
    newiopNoret(VOP_GE, { v0_2u, v0_2u, v0_2u});
    newiopNoret(VOP_GE, { v31_2u, v0_2u, v0_2u});
    newiopNoret(VOP_GE, { v0_2u, v31_2u, v0_2u});
    newiopNoret(VOP_GE, { v0_2u, v0_2u, v31_2u});

    newiopNoret(VOP_EQ, { v0_16u, v0_16s, v0_16s});
    newiopNoret(VOP_EQ, { v31_16u, v0_16s, v0_16s});
    newiopNoret(VOP_EQ, { v0_16u, v31_16s, v0_16s});
    newiopNoret(VOP_EQ, { v0_16u, v0_16s, v31_16s});
    newiopNoret(VOP_EQ, { v0_8u, v0_8s, v0_8s});
    newiopNoret(VOP_EQ, { v31_8u, v0_8s, v0_8s});
    newiopNoret(VOP_EQ, { v0_8u, v31_8s, v0_8s});
    newiopNoret(VOP_EQ, { v0_8u, v0_8s, v31_8s});
    newiopNoret(VOP_EQ, { v0_4u, v0_4s, v0_4s});
    newiopNoret(VOP_EQ, { v31_4u, v0_4s, v0_4s});
    newiopNoret(VOP_EQ, { v0_4u, v31_4s, v0_4s});
    newiopNoret(VOP_EQ, { v0_4u, v0_4s, v31_4s});
    newiopNoret(VOP_EQ, { v0_2u, v0_2s, v0_2s});
    newiopNoret(VOP_EQ, { v31_2u, v0_2s, v0_2s});
    newiopNoret(VOP_EQ, { v0_2u, v31_2s, v0_2s});
    newiopNoret(VOP_EQ, { v0_2u, v0_2s, v31_2s});

    newiopNoret(VOP_GE, { v0_16u, v0_16s, v0_16s});
    newiopNoret(VOP_GE, { v31_16u, v0_16s, v0_16s});
    newiopNoret(VOP_GE, { v0_16u, v31_16s, v0_16s});
    newiopNoret(VOP_GE, { v0_16u, v0_16s, v31_16s});
    newiopNoret(VOP_GE, { v0_8u, v0_8s, v0_8s});
    newiopNoret(VOP_GE, { v31_8u, v0_8s, v0_8s});
    newiopNoret(VOP_GE, { v0_8u, v31_8s, v0_8s});
    newiopNoret(VOP_GE, { v0_8u, v0_8s, v31_8s});
    newiopNoret(VOP_GE, { v0_4u, v0_4s, v0_4s});
    newiopNoret(VOP_GE, { v31_4u, v0_4s, v0_4s});
    newiopNoret(VOP_GE, { v0_4u, v31_4s, v0_4s});
    newiopNoret(VOP_GE, { v0_4u, v0_4s, v31_4s});
    newiopNoret(VOP_GE, { v0_2u, v0_2s, v0_2s});
    newiopNoret(VOP_GE, { v31_2u, v0_2s, v0_2s});
    newiopNoret(VOP_GE, { v0_2u, v31_2s, v0_2s});
    newiopNoret(VOP_GE, { v0_2u, v0_2s, v31_2s});

    newiopNoret(VOP_GT, { v0_16u, v0_16s, v0_16s});
    newiopNoret(VOP_GT, { v31_16u, v0_16s, v0_16s});
    newiopNoret(VOP_GT, { v0_16u, v31_16s, v0_16s});
    newiopNoret(VOP_GT, { v0_16u, v0_16s, v31_16s});
    newiopNoret(VOP_GT, { v0_8u, v0_8s, v0_8s});
    newiopNoret(VOP_GT, { v31_8u, v0_8s, v0_8s});
    newiopNoret(VOP_GT, { v0_8u, v31_8s, v0_8s});
    newiopNoret(VOP_GT, { v0_8u, v0_8s, v31_8s});
    newiopNoret(VOP_GT, { v0_4u, v0_4s, v0_4s});
    newiopNoret(VOP_GT, { v31_4u, v0_4s, v0_4s});
    newiopNoret(VOP_GT, { v0_4u, v31_4s, v0_4s});
    newiopNoret(VOP_GT, { v0_4u, v0_4s, v31_4s});
    newiopNoret(VOP_GT, { v0_2u, v0_2s, v0_2s});
    newiopNoret(VOP_GT, { v31_2u, v0_2s, v0_2s});
    newiopNoret(VOP_GT, { v0_2u, v31_2s, v0_2s});
    newiopNoret(VOP_GT, { v0_2u, v0_2s, v31_2s});

    newiopNoret(VOP_GETLANE, { x0,  v0_16u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x15, v0_16u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0, v31_16u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,  v0_16u, argIImm(15) });
    newiopNoret(VOP_GETLANE, { x0,   v0_8u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x15,  v0_8u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,  v31_8u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,   v0_8u, argIImm(7)});
    newiopNoret(VOP_GETLANE, { x0,   v0_4u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x15,  v0_4u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,  v31_4u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,   v0_4u, argIImm(3)});
    newiopNoret(VOP_GETLANE, { x0,   v0_2u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x15,  v0_2u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,  v31_2u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,   v0_2u, argIImm(1)});

    newiopNoret(VOP_SETLANE, {  v0_16u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, { v31_16u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {  v0_16u, argIImm(15),  x0});
    newiopNoret(VOP_SETLANE, {  v0_16u,  argIImm(0), x15});
    newiopNoret(VOP_SETLANE, {   v0_8u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {  v31_8u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {   v0_8u,  argIImm(7),  x0});
    newiopNoret(VOP_SETLANE, {   v0_8u,  argIImm(0), x15});
    newiopNoret(VOP_SETLANE, {   v0_4u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {  v31_4u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {   v0_4u,  argIImm(3),  x0});
    newiopNoret(VOP_SETLANE, {   v0_4u,  argIImm(0), x15});
    newiopNoret(VOP_SETLANE, {   v0_2u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {  v31_2u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {   v0_2u,  argIImm(1),  x0});
    newiopNoret(VOP_SETLANE, {   v0_2u,  argIImm(0), x15});

    newiopNoret(VOP_SETLANE, {  v0_16u,  argIImm(0),  v0_16u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v31_16u,  argIImm(0),  v0_16u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_16u,  argIImm(15),  v0_16u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_16u,  argIImm(0),  v31_16u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_16u,  argIImm(0),  v0_16u, argIImm(15)});
    newiopNoret(VOP_SETLANE, {  v0_8u,  argIImm(0),  v0_8u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v31_8u,  argIImm(0),  v0_8u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_8u,  argIImm(7),  v0_8u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_8u,  argIImm(0),  v31_8u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_8u,  argIImm(0),  v0_8u, argIImm(7)});
    newiopNoret(VOP_SETLANE, {  v0_4u,  argIImm(0),  v0_4u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v31_4u,  argIImm(0),  v0_4u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_4u,  argIImm(3),  v0_4u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_4u,  argIImm(0),  v31_4u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_4u,  argIImm(0),  v0_4u, argIImm(3)});
    newiopNoret(VOP_SETLANE, {  v0_2u,  argIImm(0),  v0_2u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v31_2u,  argIImm(0),  v0_2u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_2u,  argIImm(1),  v0_2u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_2u,  argIImm(0),  v31_2u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_2u,  argIImm(0),  v0_2u, argIImm(1)});

    newiopNoret(VOP_SELECT, { v0_16u, v0_16u, v0_16u,   v0_16u});
    newiopNoret(VOP_SELECT, { v31_16u, v31_16u, v0_16u, v0_16u});
    newiopNoret(VOP_SELECT, { v0_16u, v0_16u, v31_16u,  v0_16u});
    newiopNoret(VOP_SELECT, { v0_16u, v0_16u, v0_16u,  v31_16u});
});
#undef DEFINE_CERTAIN_REG
};
#endif
#endif// __LOOPS_ARCH == __LOOPS_AARCH64
