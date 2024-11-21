/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "loops/loops.hpp"
#if __LOOPS_ARCH == __LOOPS_AARCH64
#include "tests.hpp"
#include <iostream>
#include <vector>

using namespace loops;

TEST(aarch64, nullify_msb_lsb_v)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    IReg iptr, omptr, olptr, n;
    STARTFUNC_(test_info_->name(), &iptr, &omptr, &olptr, &n)
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
            offset += ctx.vbytes();
        }
        RETURN_();
    }
    typedef int64_t (*nullify_msb_lsb_v_f)(const uint32_t* src, uint32_t* msbdest, uint32_t* lsbdest, int64_t n);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    nullify_msb_lsb_v_f tested = reinterpret_cast<nullify_msb_lsb_v_f>(func.ptr());
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
        ASSERT_EQ(msb[vnum], remsb);
        ASSERT_EQ(lsb[vnum], relsb);
    }
}


TEST(aarch64, big_immediates)
{
    enum {TBI_SCALARS, TBI_I8_0, TBI_I8_1, TBI_I16_0, TBI_I16_1, TBI_I32_0, TBI_I32_1, TBI_I64_0, TBI_I64_1, TBI_I64_2};
    Context ctx;
    USE_CONTEXT_(ctx);
    IReg targetptr, tasknum;
    STARTFUNC_(test_info_->name(), &targetptr, &tasknum)
    {
        IF_(tasknum == TBI_SCALARS)
        {
            double e = 2.718281828459045235360287471352;
            store_<uint64_t>(targetptr, CONST_(65535)); targetptr += sizeof(uint64_t);
            store_<uint64_t>(targetptr, CONST_(65536)); targetptr += sizeof(uint64_t);
            store_<int64_t>(targetptr, CONST_(-32768)); targetptr += sizeof(uint64_t);
            store_<int64_t>(targetptr, CONST_(-32769)); targetptr += sizeof(uint64_t);
            store_<uint64_t>(targetptr, CONST_(0x5F3759DF)); targetptr += sizeof(uint64_t);
            union uconv_
            {
                uint64_t val64;
                double val;
                uconv_() : val64(0) {} 
            } conv;
            conv.val = e;
            store_<uint64_t>(targetptr, CONST_(conv.val64)); targetptr += sizeof(uint64_t);
        }
        ELIF_(tasknum == TBI_I8_0)
        {
            storevec(targetptr, VCONST_(uint8_t, 0));   targetptr += ctx.vbytes();
            storevec(targetptr, VCONST_(uint8_t, 255)); targetptr += ctx.vbytes();
        }
        ELIF_(tasknum == TBI_I8_1)
        {
            storevec(targetptr, VCONST_(int8_t, -128)); targetptr += ctx.vbytes();
        }
        ELIF_(tasknum == TBI_I16_0)
        {
            storevec(targetptr, VCONST_(uint16_t, 0));   targetptr += ctx.vbytes();
            storevec(targetptr, VCONST_(uint16_t, 255)); targetptr += ctx.vbytes();
            storevec(targetptr, VCONST_(uint16_t, 256)); targetptr += ctx.vbytes();
        }
        ELIF_(tasknum == TBI_I16_1)
        {
            storevec(targetptr, VCONST_(int16_t, -128)); targetptr += ctx.vbytes();
            storevec(targetptr, VCONST_(int16_t, -129)); targetptr += ctx.vbytes();
        }
        ELIF_(tasknum == TBI_I32_0)
        {
            storevec(targetptr, VCONST_(uint32_t, 0));   targetptr += ctx.vbytes();
            storevec(targetptr, VCONST_(uint32_t, 255)); targetptr += ctx.vbytes();
            storevec(targetptr, VCONST_(uint32_t, 256)); targetptr += ctx.vbytes();
        }
        ELIF_(tasknum == TBI_I32_1)
        {
            storevec(targetptr, VCONST_(int32_t, -128)); targetptr += ctx.vbytes();
            storevec(targetptr, VCONST_(int32_t, -129)); targetptr += ctx.vbytes();
        }
        ELIF_(tasknum == TBI_I64_0)
        {
            storevec(targetptr, VCONST_(uint64_t, 0));   targetptr += ctx.vbytes();
            storevec(targetptr, VCONST_(uint64_t, 255)); targetptr += ctx.vbytes();
            storevec(targetptr, VCONST_(uint64_t, 0xff00)); targetptr += ctx.vbytes();
        }
        ELIF_(tasknum == TBI_I64_1)
        {
            storevec(targetptr, VCONST_(int64_t, -256)); targetptr += ctx.vbytes();
        }
        ELIF_(tasknum == TBI_I64_2)
        {
            storevec(targetptr, VCONST_(uint64_t, 256)); targetptr += ctx.vbytes();
            storevec(targetptr, VCONST_(uint64_t, 254)); targetptr += ctx.vbytes();
        }
        RETURN_();
    }
    typedef int64_t (*big_immediates_f)(void*, int64_t);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    big_immediates_f tested = reinterpret_cast<big_immediates_f>(func.ptr());
    double e = 2.718281828459045235360287471352;
    union uconv_
    {
        int64_t val64;
        double val;
        uconv_() : val64(0) {} 
    } conv;
    conv.val = e;
    const std::vector<int64_t> scalars_ref = {65535, 65536, -32768, -32769, 0x5F3759DF, conv.val64};
    std::vector<int64_t> scalars(scalars_ref.size(), 0);
    tested((void*)&(scalars[0]), TBI_SCALARS);
    for(int i = 0; i < (int)scalars.size(); i++) ASSERT_EQ(scalars[i], scalars_ref[i]);

    const std::vector<uint8_t> i8_0_ref = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                           255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
    std::vector<uint8_t> i8_0(i8_0_ref.size(), 0);
    tested((void*)&(i8_0[0]), TBI_I8_0);
    for(int i = 0; i < (int)i8_0.size(); i++) ASSERT_EQ(i8_0[i], i8_0_ref[i]);

    const std::vector<int8_t> i8_1_ref = {-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128};
    std::vector<int8_t> i8_1(i8_1_ref.size(), 0);
    tested((void*)&(i8_1[0]), TBI_I8_1);
    for(int i = 0; i < (int)i8_1.size(); i++) ASSERT_EQ(i8_1[i], i8_1_ref[i]);


    const std::vector<uint16_t> i16_0_ref = {0,0,0,0,0,0,0,0,
                                             255,255,255,255,255,255,255,255,
                                             256,256,256,256,256,256,256,256};
    std::vector<uint16_t> i16_0(i16_0_ref.size(), 0);
    tested((void*)&(i16_0[0]), TBI_I16_0);
    for(int i = 0; i < (int)i16_0.size(); i++) ASSERT_EQ(i16_0[i], i16_0_ref[i]);


    const std::vector<int16_t> i16_1_ref = {-128,-128,-128,-128,-128,-128,-128,-128,
                                            -129,-129,-129,-129,-129,-129,-129,-129};
    std::vector<int16_t> i16_1(i16_1_ref.size(), 0);
    tested((void*)&(i16_1[0]), TBI_I16_1);
    for(int i = 0; i < (int)i16_1.size(); i++) ASSERT_EQ(i16_1[i], i16_1_ref[i]);


    const std::vector<uint32_t> i32_0_ref = {0,0,0,0,
                                             255,255,255,255,
                                             256,256,256,256};
    std::vector<uint32_t> i32_0(i32_0_ref.size(), 0);
    tested((void*)&(i32_0[0]), TBI_I32_0);
    for(int i = 0; i < (int)i32_0.size(); i++) ASSERT_EQ(i32_0[i], i32_0_ref[i]);

    const std::vector<int32_t> i32_1_ref = {-128,-128,-128,-128,
                                            -129,-129,-129,-129};
    std::vector<int32_t> i32_1(i32_1_ref.size(), 0);
    tested((void*)&(i32_1[0]), TBI_I32_1);
    for(int i = 0; i < (int)i32_1.size(); i++) ASSERT_EQ(i32_1[i], i32_1_ref[i]);


    const std::vector<uint64_t> i64_0_ref= {0,0,
                                            255,255,
                                            0xff00,0xff00};
    std::vector<uint64_t> i64_0(i64_0_ref.size(), 0);
    tested((void*)&(i64_0[0]), TBI_I64_0);
    for(int i = 0; i < (int)i64_0.size(); i++) ASSERT_EQ(i64_0[i], i64_0_ref[i]);

    const std::vector<int64_t> i64_1_ref = {-256,-256};
    std::vector<int64_t> i64_1(i64_1_ref.size(), 0);
    tested((void*)&(i64_1[0]), TBI_I64_1);
    for(int i = 0; i < (int)i64_1.size(); i++) ASSERT_EQ(i64_1[i], i64_1_ref[i]);

    const std::vector<uint64_t> i64_2_ref = {256,256,
                                             254,254};
    std::vector<uint64_t> i64_2(i64_2_ref.size(), 0);
    tested((void*)&(i64_2[0]), TBI_I64_2);
    for(int i = 0; i < (int)i64_2.size(); i++) ASSERT_EQ(i64_2[i], i64_2_ref[i]);
}
//TODO(ch)[IMPORTANT]: There obviously needed test, which prooves, that CALL_ of function, which uses vector, doesn't corrupt vector registers.

TEST(aarch64, instruction_set_test)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    STARTFUNC_(test_info_->name())
    {
        PREPARE_ASSEMBLY_TESTING(test_info_->name());

        DEFINE_ASSEMBLY_REG(x0, 0);
        DEFINE_ASSEMBLY_REG(w0, 0);
        DEFINE_ASSEMBLY_REG(x7, 7);
        DEFINE_ASSEMBLY_REG(x15, 15);
        DEFINE_ASSEMBLY_REG(w15, 15);
        Expr v0_16u = VExpr<uint8_t>(vregHid<uint8_t>(0,_f)).notype();
        Expr v31_16u = VExpr<uint8_t>(vregHid<uint8_t>(31,_f)).notype();
        Expr v0_8u = VExpr<uint16_t>(vregHid<uint16_t>(0,_f)).notype();
        Expr v31_8u = VExpr<uint16_t>(vregHid<uint16_t>(31,_f)).notype();
        Expr v0_4u = VExpr<uint32_t>(vregHid<uint32_t>(0,_f)).notype();
        Expr v31_4u = VExpr<uint32_t>(vregHid<uint32_t>(31,_f)).notype();
        Expr v0_2u = VExpr<uint64_t>(vregHid<uint64_t>(0,_f)).notype();
        Expr v31_2u = VExpr<uint64_t>(vregHid<uint64_t>(31,_f)).notype();
        Expr v0_16s = VExpr<int8_t>(vregHid<int8_t>(0,_f)).notype();
        Expr v1_16s = VExpr<int8_t>(vregHid<int8_t>(1,_f)).notype();
        Expr v31_16s = VExpr<int8_t>(vregHid<int8_t>(31,_f)).notype();
        Expr v0_8s = VExpr<int16_t>(vregHid<int16_t>(0,_f)).notype();
        Expr v1_8s = VExpr<int16_t>(vregHid<int16_t>(1,_f)).notype();
        Expr v31_8s = VExpr<int16_t>(vregHid<int16_t>(31,_f)).notype();
        Expr v0_4s = VExpr<int32_t>(vregHid<int32_t>(0,_f)).notype();
        Expr v1_4s = VExpr<int32_t>(vregHid<int32_t>(1,_f)).notype();
        Expr v31_4s = VExpr<int32_t>(vregHid<int32_t>(31,_f)).notype();
        Expr v0_2s = VExpr<int64_t>(vregHid<int64_t>(0,_f)).notype();
        Expr v1_2s = VExpr<int64_t>(vregHid<int64_t>(1,_f)).notype();
        Expr v31_2s = VExpr<int64_t>(vregHid<int64_t>(31,_f)).notype();
        Expr v0_8f = VExpr<f16_t>(vregHid<f16_t>(0,_f)).notype();
        Expr v15_8f = VExpr<f16_t>(vregHid<f16_t>(15,_f)).notype();
        Expr v31_8f = VExpr<f16_t>(vregHid<f16_t>(31,_f)).notype();
        Expr v0_4f = VExpr<float>(vregHid<float>(0,_f)).notype();
        Expr v31_4f = VExpr<float>(vregHid<float>(31,_f)).notype();
        Expr v0_2f = VExpr<double>(vregHid<double>(0,_f)).notype();
        Expr v31_2f = VExpr<double>(vregHid<double>(31,_f)).notype();
        Expr q0 = VExpr<uint64_t>(vregHid<uint64_t>(0,_f)).notype();
        Expr q31 = VExpr<uint64_t>(vregHid<uint64_t>(31,_f)).notype();

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

        newiopNoret(OP_ARM_STP, {  x0, argIImm(      0),  x0,  x0 });
        newiopNoret(OP_ARM_STP, { x15, argIImm(      0),  x0,  x0 });
        newiopNoret(OP_ARM_STP, {  x0, argIImm( 63 * 8),  x0,  x0 });
        newiopNoret(OP_ARM_STP, {  x0, argIImm(-64 * 8),  x0,  x0 });
        newiopNoret(OP_ARM_STP, {  x0, argIImm(      0), x15,  x0 });
        newiopNoret(OP_ARM_STP, {  x0, argIImm(      0),  x0, x15 });

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

        newiopNoret(OP_ARM_LDP, { x0, x7,  x15, argIImm(  0) });
        newiopNoret(OP_ARM_LDP, { x7, x0,  x15, argIImm(  0) });
        newiopNoret(OP_ARM_LDP, { x7, x15,  x0, argIImm(  0) });
        newiopNoret(OP_ARM_LDP, { x0, x7,  x15, argIImm( 63 * 8) });
        newiopNoret(OP_ARM_LDP, { x0, x7,  x15, argIImm(-64 * 8) });

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

        newiopNoret(OP_IVERSON, { x0, OP_EQ });
        newiopNoret(OP_IVERSON, { x0, OP_NE });
        newiopNoret(OP_IVERSON, { x0, OP_LT });
        newiopNoret(OP_IVERSON, { x0, OP_GT });
        newiopNoret(OP_IVERSON, { x0, OP_LE });
        newiopNoret(OP_IVERSON, { x0, OP_GE });
        newiopNoret(OP_IVERSON, { x0, OP_S  });
        newiopNoret(OP_IVERSON, { x0, OP_NS });

        newiopNoret(OP_IVERSON, { x15, OP_EQ });

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
        newiopNoret(OP_MOV, { v0_2u,  argIImm(0)  });
        newiopNoret(OP_MOV, { v31_2u,  argIImm(0)  });
        newiopNoret(OP_MOV, { v0_2u,  argIImm(255)  });
        newiopNoret(OP_MOV, { v31_2u,  argIImm(255)  });
        newiopNoret(OP_MOV, { v0_2u,  argIImm(static_cast<int64_t>(uint64_t(0xff00ff00ff00ff00)))});
        newiopNoret(OP_MOV, { v31_2u,  argIImm(static_cast<int64_t>(uint64_t(0xff00ff00ff00ff00)))});

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

        newiopNoret(VOP_EQ, { v0_16u, v0_16u, v0_16u});
        newiopNoret(VOP_EQ, { v31_16u, v0_16u, v0_16u});
        newiopNoret(VOP_EQ, { v0_16u, v31_16u, v0_16u});
        newiopNoret(VOP_EQ, { v0_16u, v0_16u, v31_16u});
        newiopNoret(VOP_EQ, { v0_8u, v0_8u, v0_8u});
        newiopNoret(VOP_EQ, { v31_8u, v0_8u, v0_8u});
        newiopNoret(VOP_EQ, { v0_8u, v31_8u, v0_8u});
        newiopNoret(VOP_EQ, { v0_8u, v0_8u, v31_8u});
        newiopNoret(VOP_EQ, { v0_4u, v0_4u, v0_4u});
        newiopNoret(VOP_EQ, { v31_4u, v0_4u, v0_4u});
        newiopNoret(VOP_EQ, { v0_4u, v31_4u, v0_4u});
        newiopNoret(VOP_EQ, { v0_4u, v0_4u, v31_4u});
        newiopNoret(VOP_EQ, { v0_2u, v0_2u, v0_2u});
        newiopNoret(VOP_EQ, { v31_2u, v0_2u, v0_2u});
        newiopNoret(VOP_EQ, { v0_2u, v31_2u, v0_2u});
        newiopNoret(VOP_EQ, { v0_2u, v0_2u, v31_2u});

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

        newiopNoret(VOP_ARM_SHRINK_LOW, { v0_16s, v0_8s });
        newiopNoret(VOP_ARM_SHRINK_LOW, { v31_16s, v0_8s });
        newiopNoret(VOP_ARM_SHRINK_LOW, { v0_16s, v31_8s });
        newiopNoret(VOP_ARM_SHRINK_LOW, { v0_8s, v0_4s });
        newiopNoret(VOP_ARM_SHRINK_LOW, { v31_8s, v0_4s });
        newiopNoret(VOP_ARM_SHRINK_LOW, { v0_8s, v31_4s });
        newiopNoret(VOP_ARM_SHRINK_LOW, { v0_4s, v0_2s });
        newiopNoret(VOP_ARM_SHRINK_LOW, { v31_4s, v0_2s });
        newiopNoret(VOP_ARM_SHRINK_LOW, { v0_4s, v31_2s });

        newiopNoret(VOP_ARM_SHRINK_HIGH, { v0_16s, v0_8s });
        newiopNoret(VOP_ARM_SHRINK_HIGH, { v31_16s, v0_8s });
        newiopNoret(VOP_ARM_SHRINK_HIGH, { v0_16s, v31_8s });
        newiopNoret(VOP_ARM_SHRINK_HIGH, { v0_8s, v0_4s });
        newiopNoret(VOP_ARM_SHRINK_HIGH, { v31_8s, v0_4s });
        newiopNoret(VOP_ARM_SHRINK_HIGH, { v0_8s, v31_4s });
        newiopNoret(VOP_ARM_SHRINK_HIGH, { v0_4s, v0_2s });
        newiopNoret(VOP_ARM_SHRINK_HIGH, { v31_4s, v0_2s });
        newiopNoret(VOP_ARM_SHRINK_HIGH, { v0_4s, v31_2s });

        newiopNoret(VOP_POPCOUNT, { v0_16u, v0_16u});
        newiopNoret(VOP_POPCOUNT, { v31_16u, v0_16u});
        newiopNoret(VOP_POPCOUNT, { v0_16u, v31_16u});

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
        
        newiopNoret(VOP_REDUCE_SUM, { v0_16u, v0_16u});
        newiopNoret(VOP_REDUCE_SUM, { v31_16u, v0_16u});
        newiopNoret(VOP_REDUCE_SUM, { v0_16u, v31_16u});
        newiopNoret(VOP_REDUCE_SUM, { v0_8u, v0_8u});
        newiopNoret(VOP_REDUCE_SUM, { v31_8u, v0_8u});
        newiopNoret(VOP_REDUCE_SUM, { v0_8u, v31_8u});
        newiopNoret(VOP_REDUCE_SUM, { v0_4u, v0_4u});
        newiopNoret(VOP_REDUCE_SUM, { v31_4u, v0_4u});
        newiopNoret(VOP_REDUCE_SUM, { v0_4u, v31_4u});

        newiopNoret(VOP_REDUCE_WSUM, { v0_8u, v0_16u});
        newiopNoret(VOP_REDUCE_WSUM, { v31_8u, v0_16u});
        newiopNoret(VOP_REDUCE_WSUM, { v0_8u, v31_16u});
        newiopNoret(VOP_REDUCE_WSUM, { v0_4u, v0_8u});
        newiopNoret(VOP_REDUCE_WSUM, { v31_4u, v0_8u});
        newiopNoret(VOP_REDUCE_WSUM, { v0_4u, v31_8u});
        newiopNoret(VOP_REDUCE_WSUM, { v0_2u, v0_4u});
        newiopNoret(VOP_REDUCE_WSUM, { v31_2u, v0_4u});
        newiopNoret(VOP_REDUCE_WSUM, { v0_2u, v31_4u});

        newiopNoret(VOP_REDUCE_WSUM, { v0_8s, v0_16s});
        newiopNoret(VOP_REDUCE_WSUM, { v31_8s, v0_16s});
        newiopNoret(VOP_REDUCE_WSUM, { v0_8s, v31_16s});
        newiopNoret(VOP_REDUCE_WSUM, { v0_4s, v0_8s});
        newiopNoret(VOP_REDUCE_WSUM, { v31_4s, v0_8s});
        newiopNoret(VOP_REDUCE_WSUM, { v0_4s, v31_8s});
        newiopNoret(VOP_REDUCE_WSUM, { v0_2s, v0_4s});
        newiopNoret(VOP_REDUCE_WSUM, { v31_2s, v0_4s});
        newiopNoret(VOP_REDUCE_WSUM, { v0_2s, v31_4s});

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

        newiopNoret(VOP_ARM_LD1, {  v0_16s,  x0 });
        newiopNoret(VOP_ARM_LD1, { v31_16s,  x0 });
        newiopNoret(VOP_ARM_LD1, {  v0_16s, x15 });

        newiopNoret(VOP_ARM_LD1, {  v0_8s , x0 });
        newiopNoret(VOP_ARM_LD1, { v31_8s , x0 });
        newiopNoret(VOP_ARM_LD1, {  v0_8s ,x15 });

        newiopNoret(VOP_ARM_LD1, {  v0_4s , x0 });
        newiopNoret(VOP_ARM_LD1, { v31_4s , x0 });
        newiopNoret(VOP_ARM_LD1, {  v0_4s ,x15 });

        newiopNoret(VOP_ARM_LD1, {  v0_2s , x0 });
        newiopNoret(VOP_ARM_LD1, { v31_2s , x0 });
        newiopNoret(VOP_ARM_LD1, {  v0_2s ,x15 });

    //    newiopNoret(VOP_ARM_LD4, {  v0_16s,  x0 });
    //    newiopNoret(VOP_ARM_LD4, { v31_16s,  x0 });
    //    newiopNoret(VOP_ARM_LD4, {  v0_16s, x15 });
    //
    //    newiopNoret(VOP_ARM_LD4, {  v0_8s , x0 });
    //    newiopNoret(VOP_ARM_LD4, { v31_8s , x0 });
    //    newiopNoret(VOP_ARM_LD4, {  v0_8s ,x15 });
    //
    //    newiopNoret(VOP_ARM_LD4, {  v0_4s , x0 });
    //    newiopNoret(VOP_ARM_LD4, { v31_4s , x0 });
    //    newiopNoret(VOP_ARM_LD4, {  v0_4s ,x15 });
    //
    //    newiopNoret(VOP_ARM_LD4, {  v0_2s , x0 });
    //    newiopNoret(VOP_ARM_LD4, { v31_2s , x0 });
    //    newiopNoret(VOP_ARM_LD4, {  v0_2s ,x15 });

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

        newiopNoret(VOP_ARM_ST1, {  x0,  v0_16s });
        newiopNoret(VOP_ARM_ST1, { x15,  v0_16s });
        newiopNoret(VOP_ARM_ST1, {  x0, v31_16s });

        newiopNoret(VOP_ARM_ST1, {  x0,  v0_8s });
        newiopNoret(VOP_ARM_ST1, { x15,  v0_8s });
        newiopNoret(VOP_ARM_ST1, {  x0, v31_8s });

        newiopNoret(VOP_ARM_ST1, {  x0,  v0_4s });
        newiopNoret(VOP_ARM_ST1, { x15,  v0_4s });
        newiopNoret(VOP_ARM_ST1, {  x0, v31_4s });

        newiopNoret(VOP_ARM_ST1, {  x0,  v0_2s });
        newiopNoret(VOP_ARM_ST1, { x15,  v0_2s });
        newiopNoret(VOP_ARM_ST1, {  x0, v31_2s });

    //    newiopNoret(VOP_ARM_ST4, {  x0,  v0_16s });
    //    newiopNoret(VOP_ARM_ST4, { x15,  v0_16s });
    //    newiopNoret(VOP_ARM_ST4, {  x0, v31_16s });
    //
    //    newiopNoret(VOP_ARM_ST4, {  x0,  v0_8s });
    //    newiopNoret(VOP_ARM_ST4, { x15,  v0_8s });
    //    newiopNoret(VOP_ARM_ST4, {  x0, v31_8s });
    //
    //    newiopNoret(VOP_ARM_ST4, {  x0,  v0_4s });
    //    newiopNoret(VOP_ARM_ST4, { x15,  v0_4s });
    //    newiopNoret(VOP_ARM_ST4, {  x0, v31_4s });
    //
    //    newiopNoret(VOP_ARM_ST4, {  x0,  v0_2s });
    //    newiopNoret(VOP_ARM_ST4, { x15,  v0_2s });
    //    newiopNoret(VOP_ARM_ST4, {  x0, v31_2s });

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

        newiopNoret(OP_CALL_NORET, {  x0 });
        newiopNoret(OP_CALL_NORET, { x15 });
    }
    loops::Func func = ctx.getFunc(test_info_->name());
    EXPECT_ASSEMBLY_CORRECT(func);
}
#endif