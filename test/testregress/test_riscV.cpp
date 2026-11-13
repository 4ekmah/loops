/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "loops/loops.hpp"
#if __LOOPS_ARCH == __LOOPS_RISCV
#include "loops/loops.hpp"
#include "src/common.hpp"
#include "src/func_impl.hpp"
#include "src/reg_allocator.hpp"
#include "tests.hpp"
#include <iostream>

using namespace loops;

TEST(riscV, big_immediates)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    IReg targetptr, tasknum;
    STARTFUNC_(test_info_->name(), &targetptr)
    {
        double e = 2.718281828459045235360287471352;
        store_<uint64_t>(targetptr, CONST_(0x1000)); targetptr += sizeof(uint64_t);
        store_<uint64_t>(targetptr, CONST_(0x1fff)); targetptr += sizeof(uint64_t);
        store_<uint64_t>(targetptr, CONST_(0x7ffffaaa)); targetptr += sizeof(uint64_t);
        store_<uint64_t>(targetptr, CONST_(0x100000000)); targetptr += sizeof(uint64_t);
        store_<uint64_t>(targetptr, CONST_(0x1ffffffff)); targetptr += sizeof(uint64_t);
        store_<uint64_t>(targetptr, CONST_(0x7fffffffaaaaaaaa)); targetptr += sizeof(uint64_t);
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
        RETURN_();
    }
    typedef int64_t (*big_immediates_f)(void*);
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
    const std::vector<int64_t> scalars_ref = {0x1000, 0x1fff, 0x7ffffaaa, 0x100000000, 0x1ffffffff, 0x7fffffffaaaaaaaa, 65535, 65536, -32768, -32769, 0x5F3759DF, conv.val64};
    std::vector<int64_t> scalars(scalars_ref.size(), 0);
    tested((void*)&(scalars[0]));
    for(int i = 0; i < (int)scalars.size(); i++) ASSERT_EQ(scalars[i], scalars_ref[i]);
}

TEST(riscV, instruction_set_test)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    STARTFUNC_(test_info_->name())
    {
        PREPARE_ASSEMBLY_TESTING(test_info_->name());

        DEFINE_ASSEMBLY_REG(zero, 0);
        DEFINE_ASSEMBLY_REG(t6, 31);
        DEFINE_ASSEMBLY_VREG(uint8_t, u8v0, 0);
        DEFINE_ASSEMBLY_VREG(uint8_t, u8v31, 31);
        DEFINE_ASSEMBLY_VREG(uint16_t, u16v0, 0);
        DEFINE_ASSEMBLY_VREG(uint16_t, u16v31, 31);
        DEFINE_ASSEMBLY_VREG(uint32_t, uv0, 0);
        DEFINE_ASSEMBLY_VREG(uint32_t, uv31, 31);
        DEFINE_ASSEMBLY_VREG(uint64_t, u64v0, 0);
        DEFINE_ASSEMBLY_VREG(uint64_t, u64v31, 31);
        DEFINE_ASSEMBLY_VREG(uint8_t, u8v1, 1);
        DEFINE_ASSEMBLY_VREG(uint8_t, u8v30, 30);
        DEFINE_ASSEMBLY_VREG(uint16_t, u16v1, 1);
        DEFINE_ASSEMBLY_VREG(uint16_t, u16v30, 30);
        DEFINE_ASSEMBLY_VREG(uint32_t, uv1, 1);
        DEFINE_ASSEMBLY_VREG(uint32_t, uv30, 30);
        DEFINE_ASSEMBLY_VREG(uint64_t, u64v1, 1);
        DEFINE_ASSEMBLY_VREG(uint64_t, u64v30, 30);
        DEFINE_ASSEMBLY_VREG(int8_t, i8v0, 0);
        DEFINE_ASSEMBLY_VREG(int8_t, i8v31, 31);
        DEFINE_ASSEMBLY_VREG(int16_t, i16v0, 0);
        DEFINE_ASSEMBLY_VREG(int16_t, i16v31, 31);
        DEFINE_ASSEMBLY_VREG(int32_t, iv0, 0);
        DEFINE_ASSEMBLY_VREG(int32_t, iv31, 31);
        DEFINE_ASSEMBLY_VREG(float, fv0, 0);
        DEFINE_ASSEMBLY_VREG(float, fv31, 31);
        DEFINE_ASSEMBLY_VREG(double, dv0, 0);
        DEFINE_ASSEMBLY_VREG(double, dv31, 31);

        newiopNoret(OP_MOV, { zero, zero});
        newiopNoret(OP_MOV, { t6  , zero});
        newiopNoret(OP_MOV, { zero, t6  });
        newiopNoret(OP_MOV, { zero, immtyped<int64_t>(0    , _f)});
        newiopNoret(OP_MOV, { t6  , immtyped<int64_t>(0    , _f)});
        newiopNoret(OP_MOV, { zero, immtyped<int64_t>(1    , _f)});
        newiopNoret(OP_MOV, { t6  , immtyped<int64_t>(1    , _f)});
        newiopNoret(OP_MOV, { zero, immtyped<int64_t>(2047 , _f)});
        newiopNoret(OP_MOV, { zero, immtyped<int64_t>(-2048, _f)});

        newiopNoret(OP_RV_LUI, { zero, immtyped<int64_t>(0      , _f)});
        newiopNoret(OP_RV_LUI, {   t6, immtyped<int64_t>(0      , _f)});
        newiopNoret(OP_RV_LUI, { zero, immtyped<int64_t>(1048575, _f)});

        newiopNoret(OP_ADD, { zero, zero, zero });
        newiopNoret(OP_ADD, {   t6, zero, zero });
        newiopNoret(OP_ADD, { zero,   t6, zero });
        newiopNoret(OP_ADD, { zero, zero,   t6 });
        newiopNoret(OP_ADD, { zero, zero, immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_ADD, { t6  , zero, immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_ADD, { zero, t6  , immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_ADD, { zero, zero, immtyped<int64_t>(2047 , _f) });
        newiopNoret(OP_ADD, { zero, zero, immtyped<int64_t>(-2048, _f) });

        newiopNoret(OP_SUB, { zero, zero, zero });
        newiopNoret(OP_SUB, {   t6, zero, zero });
        newiopNoret(OP_SUB, { zero,   t6, zero });
        newiopNoret(OP_SUB, { zero, zero,   t6 });
        newiopNoret(OP_SUB, { zero, zero, immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_SUB, { t6  , zero, immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_SUB, { zero, t6  , immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_SUB, { zero, zero, immtyped<int64_t>(2048 , _f) });
        newiopNoret(OP_SUB, { zero, zero, immtyped<int64_t>(-2047, _f) });

        newiopNoret(OP_NEG, { zero, zero });
        newiopNoret(OP_NEG, { t6,   zero });
        newiopNoret(OP_NEG, { zero, t6   });

        newiopNoret(OP_SHL, { zero, zero, zero });
        newiopNoret(OP_SHL, {   t6, zero, zero });
        newiopNoret(OP_SHL, { zero,   t6, zero });
        newiopNoret(OP_SHL, { zero, zero,   t6 });
        newiopNoret(OP_SHL, { zero, zero, immtyped<int64_t>(0 , _f) });
        newiopNoret(OP_SHL, { t6  , zero, immtyped<int64_t>(0 , _f) });
        newiopNoret(OP_SHL, { zero, t6  , immtyped<int64_t>(0 , _f) });
        newiopNoret(OP_SHL, { zero, zero, immtyped<int64_t>(63, _f) });

        newiopNoret(OP_SHR, { zero, zero, zero });
        newiopNoret(OP_SHR, {   t6, zero, zero });
        newiopNoret(OP_SHR, { zero,   t6, zero });
        newiopNoret(OP_SHR, { zero, zero,   t6 });
        newiopNoret(OP_SHR, { zero, zero, immtyped<int64_t>(0 , _f) });
        newiopNoret(OP_SHR, { t6  , zero, immtyped<int64_t>(0 , _f) });
        newiopNoret(OP_SHR, { zero, t6  , immtyped<int64_t>(0 , _f) });
        newiopNoret(OP_SHR, { zero, zero, immtyped<int64_t>(63, _f) });

        newiopNoret(OP_SAR, { zero, zero, zero });
        newiopNoret(OP_SAR, {   t6, zero, zero });
        newiopNoret(OP_SAR, { zero,   t6, zero });
        newiopNoret(OP_SAR, { zero, zero,   t6 });
        newiopNoret(OP_SAR, { zero, zero, immtyped<int64_t>(0 , _f) });
        newiopNoret(OP_SAR, { t6  , zero, immtyped<int64_t>(0 , _f) });
        newiopNoret(OP_SAR, { zero, t6  , immtyped<int64_t>(0 , _f) });
        newiopNoret(OP_SAR, { zero, zero, immtyped<int64_t>(63, _f) });

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

        newiopNoret(OP_XOR, { zero, zero, zero });
        newiopNoret(OP_XOR, { t6  , zero, zero });
        newiopNoret(OP_XOR, { zero, t6  , zero });
        newiopNoret(OP_XOR, { zero, zero, t6   });
        newiopNoret(OP_XOR, { zero, zero, immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_XOR, { t6  , zero, immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_XOR, { zero, t6  , immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_XOR, { zero, zero, immtyped<int64_t>(2047 , _f) });
        newiopNoret(OP_XOR, { zero, zero, immtyped<int64_t>(-2048, _f) });

        newiopNoret(OP_OR,  { zero, zero, zero });
        newiopNoret(OP_OR,  { t6  , zero, zero });
        newiopNoret(OP_OR,  { zero, t6  , zero });
        newiopNoret(OP_OR,  { zero, zero, t6   });
        newiopNoret(OP_OR,  { zero, zero, immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_OR,  { t6  , zero, immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_OR,  { zero, t6  , immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_OR,  { zero, zero, immtyped<int64_t>(2047 , _f) });
        newiopNoret(OP_OR,  { zero, zero, immtyped<int64_t>(-2048, _f) });

        newiopNoret(OP_AND, { zero, zero, zero });
        newiopNoret(OP_AND, { t6  , zero, zero });
        newiopNoret(OP_AND, { zero, t6  , zero });
        newiopNoret(OP_AND, { zero, zero, t6   });
        newiopNoret(OP_AND, { zero, zero, immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_AND, { t6  , zero, immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_AND, { zero, t6  , immtyped<int64_t>(0    , _f) });
        newiopNoret(OP_AND, { zero, zero, immtyped<int64_t>(2047 , _f) });
        newiopNoret(OP_AND, { zero, zero, immtyped<int64_t>(-2048, _f) });

        newiopNoret(OP_NOT, { zero, zero });
        newiopNoret(OP_NOT, { t6,   zero });
        newiopNoret(OP_NOT, { zero, t6   });

        newiopNoret(OP_LOAD, { iregtyped<int8_t>(zero), zero });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(t6)  , zero });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(zero), t6   });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(zero), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(t6  ), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(zero),   t6, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(zero), zero, argIImm(2047 ) });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(zero), zero, argIImm(-2048) });

        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(zero), zero });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(t6)  , zero });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(zero), t6   });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(zero), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(t6  ), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(zero),   t6, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(zero), zero, argIImm(2047 ) });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(zero), zero, argIImm(-2048) });

        newiopNoret(OP_LOAD, { iregtyped<int16_t>(zero), zero });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(t6)  , zero });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(zero), t6   });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(zero), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(t6  ), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(zero),   t6, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(zero), zero, argIImm(2047 ) });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(zero), zero, argIImm(-2048) });

        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(zero), zero });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(t6)  , zero });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(zero), t6   });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(zero), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(t6  ), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(zero),   t6, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(zero), zero, argIImm(2047 ) });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(zero), zero, argIImm(-2048) });

        newiopNoret(OP_LOAD, { iregtyped<int32_t>(zero), zero });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(t6)  , zero });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(zero), t6   });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(zero), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(t6  ), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(zero),   t6, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(zero), zero, argIImm(2047 ) });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(zero), zero, argIImm(-2048) });

        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(zero), zero });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(t6)  , zero });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(zero), t6   });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(zero), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(t6  ), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(zero),   t6, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(zero), zero, argIImm(2047 ) });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(zero), zero, argIImm(-2048) });

        newiopNoret(OP_LOAD, { iregtyped<int64_t>(zero), zero });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(t6)  , zero });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(zero), t6   });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(zero), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(t6  ), zero, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(zero),   t6, argIImm(0    ) });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(zero), zero, argIImm(2047 ) });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(zero), zero, argIImm(-2048) });

        newiopNoret(OP_STORE, { zero, iregtyped<int8_t>(zero) });
        newiopNoret(OP_STORE, { t6  , iregtyped<int8_t>(zero) });
        newiopNoret(OP_STORE, { zero, iregtyped<int8_t>(t6  ) });
        newiopNoret(OP_STORE, { zero, argIImm(0    ), iregtyped<int8_t>(zero)});
        newiopNoret(OP_STORE, { t6  , argIImm(0    ), iregtyped<int8_t>(zero)});
        newiopNoret(OP_STORE, { zero, argIImm(0    ), iregtyped<int8_t>(t6  )});
        newiopNoret(OP_STORE, { zero, argIImm(2047 ), iregtyped<int8_t>(zero)});
        newiopNoret(OP_STORE, { zero, argIImm(-2048), iregtyped<int8_t>(zero)});

        newiopNoret(OP_STORE, { zero, iregtyped<int16_t>(zero) });
        newiopNoret(OP_STORE, { t6  , iregtyped<int16_t>(zero) });
        newiopNoret(OP_STORE, { zero, iregtyped<int16_t>(t6  ) });
        newiopNoret(OP_STORE, { zero, argIImm(0    ), iregtyped<int16_t>(zero)});
        newiopNoret(OP_STORE, { t6  , argIImm(0    ), iregtyped<int16_t>(zero)});
        newiopNoret(OP_STORE, { zero, argIImm(0    ), iregtyped<int16_t>(t6  )});
        newiopNoret(OP_STORE, { zero, argIImm(2047 ), iregtyped<int16_t>(zero)});
        newiopNoret(OP_STORE, { zero, argIImm(-2048), iregtyped<int16_t>(zero)});

        newiopNoret(OP_STORE, { zero, iregtyped<int32_t>(zero) });
        newiopNoret(OP_STORE, { t6  , iregtyped<int32_t>(zero) });
        newiopNoret(OP_STORE, { zero, iregtyped<int32_t>(t6  ) });
        newiopNoret(OP_STORE, { zero, argIImm(0    ), iregtyped<int32_t>(zero)});
        newiopNoret(OP_STORE, { t6  , argIImm(0    ), iregtyped<int32_t>(zero)});
        newiopNoret(OP_STORE, { zero, argIImm(0    ), iregtyped<int32_t>(t6  )});
        newiopNoret(OP_STORE, { zero, argIImm(2047 ), iregtyped<int32_t>(zero)});
        newiopNoret(OP_STORE, { zero, argIImm(-2048), iregtyped<int32_t>(zero)});

        newiopNoret(OP_STORE, { zero, iregtyped<int64_t>(zero) });
        newiopNoret(OP_STORE, { t6  , iregtyped<int64_t>(zero) });
        newiopNoret(OP_STORE, { zero, iregtyped<int64_t>(t6  ) });
        newiopNoret(OP_STORE, { zero, argIImm(0    ), iregtyped<int64_t>(zero)});
        newiopNoret(OP_STORE, { t6  , argIImm(0    ), iregtyped<int64_t>(zero)});
        newiopNoret(OP_STORE, { zero, argIImm(0    ), iregtyped<int64_t>(t6  )});
        newiopNoret(OP_STORE, { zero, argIImm(2047 ), iregtyped<int64_t>(zero)});
        newiopNoret(OP_STORE, { zero, argIImm(-2048), iregtyped<int64_t>(zero)});

        newiopNoret(OP_IVERSON, { zero, argIImm(OP_LT), zero, zero});
        newiopNoret(OP_IVERSON, { t6  , argIImm(OP_LT), zero, zero});
        newiopNoret(OP_IVERSON, { zero, argIImm(OP_LT), t6  , zero});
        newiopNoret(OP_IVERSON, { zero, argIImm(OP_LT), zero, t6  });

        newiopNoret(OP_IVERSON, { zero, argIImm(OP_UGT), zero, zero});
        newiopNoret(OP_IVERSON, { t6  , argIImm(OP_UGT), zero, zero});
        newiopNoret(OP_IVERSON, { zero, argIImm(OP_UGT), t6  , zero});
        newiopNoret(OP_IVERSON, { zero, argIImm(OP_UGT), zero, t6  });

        newiopNoret(OP_IVERSON, { zero, argIImm(OP_EQ), zero, argIImm(0)});
        newiopNoret(OP_IVERSON, { t6  , argIImm(OP_EQ), zero, argIImm(0)});
        newiopNoret(OP_IVERSON, { zero, argIImm(OP_EQ), t6  , argIImm(0)});

        newiopNoret(OP_IVERSON, { zero, argIImm(OP_NE), zero, argIImm(0)});
        newiopNoret(OP_IVERSON, { t6  , argIImm(OP_NE), zero, argIImm(0)});
        newiopNoret(OP_IVERSON, { zero, argIImm(OP_NE), t6  , argIImm(0)});

        newiopNoret(OP_CALL_NORET, { zero });
        newiopNoret(OP_CALL_NORET, { t6   });

        newiopNoret(OP_RV_VSETVLI, { zero, zero, argIImm(0) });
        newiopNoret(OP_RV_VSETVLI, { t6, zero, argIImm(0) });
        newiopNoret(OP_RV_VSETVLI, { zero, t6, argIImm(0) });
        newiopNoret(OP_RV_VSETVLI, { zero, zero, argIImm(0x7ff) });

        newiopNoret(OP_RV_VSETIVLI, { zero, argIImm(0), argIImm(0) });
        newiopNoret(OP_RV_VSETIVLI, { t6, argIImm(0), argIImm(0) });
        newiopNoret(OP_RV_VSETIVLI, { zero, argIImm(31), argIImm(0) });
        newiopNoret(OP_RV_VSETIVLI, { zero, argIImm(0), argIImm(0x3ff) });

        newiopNoret(OP_RV_VSETVL, { zero, zero, zero });
        newiopNoret(OP_RV_VSETVL, { t6, zero, zero });
        newiopNoret(OP_RV_VSETVL, { zero, t6, zero });
        newiopNoret(OP_RV_VSETVL, { zero, zero, t6 });

        newiopNoret(VOP_LOAD, { u8v0, zero });
        newiopNoret(VOP_LOAD, { u8v31, zero });
        newiopNoret(VOP_LOAD, { u8v0, t6 });
        newiopNoret(VOP_LOAD, { u16v0, zero });
        newiopNoret(VOP_LOAD, { u16v31, zero });
        newiopNoret(VOP_LOAD, { u16v0, t6 });
        newiopNoret(VOP_LOAD, { uv0, zero });
        newiopNoret(VOP_LOAD, { uv31, zero });
        newiopNoret(VOP_LOAD, { uv0, t6 });
        newiopNoret(VOP_LOAD, { u64v0, zero });
        newiopNoret(VOP_LOAD, { u64v31, zero });
        newiopNoret(VOP_LOAD, { u64v0, t6 });

        newiopNoret(VOP_STORE, { zero, u8v0 });
        newiopNoret(VOP_STORE, { zero, u8v31 });
        newiopNoret(VOP_STORE, { t6, u8v0 });
        newiopNoret(VOP_STORE, { zero, u16v0 });
        newiopNoret(VOP_STORE, { zero, u16v31 });
        newiopNoret(VOP_STORE, { t6, u16v0 });
        newiopNoret(VOP_STORE, { zero, uv0 });
        newiopNoret(VOP_STORE, { zero, uv31 });
        newiopNoret(VOP_STORE, { t6, uv0 });
        newiopNoret(VOP_STORE, { zero, u64v0 });
        newiopNoret(VOP_STORE, { zero, u64v31 });
        newiopNoret(VOP_STORE, { t6, u64v0 });

        newiopNoret(VOP_RV_VL1RE8, { u8v0, zero });
        newiopNoret(VOP_RV_VL1RE8, { u8v31, zero });
        newiopNoret(VOP_RV_VL1RE8, { u8v0, t6 });
        newiopNoret(VOP_RV_VS1R, { u8v0, zero });
        newiopNoret(VOP_RV_VS1R, { u8v31, zero });
        newiopNoret(VOP_RV_VS1R, { u8v0, t6 });

        newiopNoret(VOP_RV_VLSEG2, { u8v0, u8v1, zero });
        newiopNoret(VOP_RV_VLSEG2, { u8v30, u8v31, zero });
        newiopNoret(VOP_RV_VLSEG2, { u8v0, u8v1, t6 });
        newiopNoret(VOP_RV_VLSEG2, { u16v0, u16v1, zero });
        newiopNoret(VOP_RV_VLSEG2, { u16v30, u16v31, zero });
        newiopNoret(VOP_RV_VLSEG2, { u16v0, u16v1, t6 });
        newiopNoret(VOP_RV_VLSEG2, { uv0, uv1, zero });
        newiopNoret(VOP_RV_VLSEG2, { uv30, uv31, zero });
        newiopNoret(VOP_RV_VLSEG2, { uv0, uv1, t6 });
        newiopNoret(VOP_RV_VLSEG2, { u64v0, u64v1, zero });
        newiopNoret(VOP_RV_VLSEG2, { u64v30, u64v31, zero });
        newiopNoret(VOP_RV_VLSEG2, { u64v0, u64v1, t6 });

        newiopNoret(OP_MOV, { uv0, uv0 });
        newiopNoret(OP_MOV, { uv31, uv0 });
        newiopNoret(OP_MOV, { uv0, uv31 });
        newiopNoret(VOP_BROADCAST, { uv0, zero });
        newiopNoret(VOP_BROADCAST, { uv31, zero });
        newiopNoret(VOP_BROADCAST, { uv0, t6 });
        newiopNoret(VOP_BROADCAST, { uv0, argIImm(0) });
        newiopNoret(VOP_BROADCAST, { uv31, argIImm(0) });
        newiopNoret(VOP_BROADCAST, { uv0, argIImm(15) });
        newiopNoret(VOP_RV_VMV1R, { uv0, uv0 });
        newiopNoret(VOP_RV_VMV1R, { uv31, uv0 });
        newiopNoret(VOP_RV_VMV1R, { uv0, uv31 });

        newiopNoret(VOP_RV_VRGATHER, { uv0, uv0, zero });
        newiopNoret(VOP_RV_VRGATHER, { uv31, uv0, zero });
        newiopNoret(VOP_RV_VRGATHER, { uv0, uv31, zero });
        newiopNoret(VOP_RV_VRGATHER, { uv0, uv0, t6 });
        newiopNoret(VOP_RV_VRGATHER, { uv0, uv0, argIImm(0) });
        newiopNoret(VOP_RV_VRGATHER, { uv31, uv0, argIImm(0) });
        newiopNoret(VOP_RV_VRGATHER, { uv0, uv31, argIImm(0) });
        newiopNoret(VOP_RV_VRGATHER, { uv0, uv0, argIImm(31) });

        newiopNoret(VOP_RV_VSLIDEUP, { uv0, uv0, zero });
        newiopNoret(VOP_RV_VSLIDEUP, { uv31, uv0, zero });
        newiopNoret(VOP_RV_VSLIDEUP, { uv0, uv31, zero });
        newiopNoret(VOP_RV_VSLIDEUP, { uv0, uv0, t6 });
        newiopNoret(VOP_RV_VSLIDEUP, { uv0, uv0, argIImm(0) });
        newiopNoret(VOP_RV_VSLIDEUP, { uv31, uv0, argIImm(0) });
        newiopNoret(VOP_RV_VSLIDEUP, { uv0, uv31, argIImm(0) });
        newiopNoret(VOP_RV_VSLIDEUP, { uv0, uv0, argIImm(31) });
        newiopNoret(VOP_RV_VSLIDEDOWN, { uv0, uv0, zero });
        newiopNoret(VOP_RV_VSLIDEDOWN, { uv31, uv0, zero });
        newiopNoret(VOP_RV_VSLIDEDOWN, { uv0, uv31, zero });
        newiopNoret(VOP_RV_VSLIDEDOWN, { uv0, uv0, t6 });
        newiopNoret(VOP_RV_VSLIDEDOWN, { uv0, uv0, argIImm(0) });
        newiopNoret(VOP_RV_VSLIDEDOWN, { uv31, uv0, argIImm(0) });
        newiopNoret(VOP_RV_VSLIDEDOWN, { uv0, uv31, argIImm(0) });
        newiopNoret(VOP_RV_VSLIDEDOWN, { uv0, uv0, argIImm(31) });

        newiopNoret(OP_RV_VMV_X_S, { zero, uv0 });
        newiopNoret(OP_RV_VMV_X_S, { t6, uv0 });
        newiopNoret(OP_RV_VMV_X_S, { zero, uv31 });
        newiopNoret(VOP_RV_VMV_S_X, { uv0, zero });
        newiopNoret(VOP_RV_VMV_S_X, { uv31, zero });
        newiopNoret(VOP_RV_VMV_S_X, { uv0, t6 });

        newiopNoret(VOP_RV_VMERGE, { uv0, uv0, uv0, uv0 });
        newiopNoret(VOP_RV_VMERGE, { uv31, uv0, uv0, uv0 });
        newiopNoret(VOP_RV_VMERGE, { uv0, uv31, uv0, uv0 });
        newiopNoret(VOP_RV_VMERGE, { uv0, uv0, uv31, uv0 });
        newiopNoret(VOP_RV_VMERGE, { uv0, uv0, zero, uv0 });
        newiopNoret(VOP_RV_VMERGE, { uv31, uv0, zero, uv0 });
        newiopNoret(VOP_RV_VMERGE, { uv0, uv31, zero, uv0 });
        newiopNoret(VOP_RV_VMERGE, { uv0, uv0, t6, uv0 });
        newiopNoret(VOP_RV_VMERGE, { uv0, uv0, argIImm(0), uv0 });
        newiopNoret(VOP_RV_VMERGE, { uv31, uv0, argIImm(0), uv0 });
        newiopNoret(VOP_RV_VMERGE, { uv0, uv31, argIImm(0), uv0 });
        newiopNoret(VOP_RV_VMERGE, { uv0, uv0, argIImm(15), uv0 });

        newiopNoret(VOP_RV_VMSNE_VI, { uv0, uv0, argIImm(0) });
        newiopNoret(VOP_RV_VMSNE_VI, { uv31, uv0, argIImm(0) });
        newiopNoret(VOP_RV_VMSNE_VI, { uv0, uv31, argIImm(0) });
        newiopNoret(VOP_RV_VMSNE_VI, { uv0, uv0, argIImm(15) });

        newiopNoret(VOP_ADD, { iv0, iv0, iv0 });
        newiopNoret(VOP_ADD, { iv31, iv0, iv0 });
        newiopNoret(VOP_ADD, { iv0, iv31, iv0 });
        newiopNoret(VOP_ADD, { iv0, iv0, iv31 });
        newiopNoret(VOP_SUB, { iv0, iv0, iv0 });
        newiopNoret(VOP_SUB, { iv31, iv0, iv0 });
        newiopNoret(VOP_SUB, { iv0, iv31, iv0 });
        newiopNoret(VOP_SUB, { iv0, iv0, iv31 });
        newiopNoret(VOP_SUB, { uv0, zero, uv0 });
        newiopNoret(VOP_SUB, { uv31, zero, uv0 });
        newiopNoret(VOP_SUB, { uv0, t6, uv0 });
        newiopNoret(VOP_SUB, { uv0, zero, uv31 });
        newiopNoret(VOP_MUL, { iv0, iv0, iv0 });
        newiopNoret(VOP_MUL, { iv31, iv0, iv0 });
        newiopNoret(VOP_MUL, { iv0, iv31, iv0 });
        newiopNoret(VOP_MUL, { iv0, iv0, iv31 });

        newiopNoret(VOP_MIN, { iv0, iv0, iv0 });
        newiopNoret(VOP_MIN, { iv31, iv0, iv0 });
        newiopNoret(VOP_MIN, { iv0, iv31, iv0 });
        newiopNoret(VOP_MIN, { iv0, iv0, iv31 });
        newiopNoret(VOP_MIN, { uv0, uv0, uv0 });
        newiopNoret(VOP_MIN, { uv31, uv0, uv0 });
        newiopNoret(VOP_MIN, { uv0, uv31, uv0 });
        newiopNoret(VOP_MIN, { uv0, uv0, uv31 });
        newiopNoret(VOP_MAX, { iv0, iv0, iv0 });
        newiopNoret(VOP_MAX, { iv31, iv0, iv0 });
        newiopNoret(VOP_MAX, { iv0, iv31, iv0 });
        newiopNoret(VOP_MAX, { iv0, iv0, iv31 });
        newiopNoret(VOP_MAX, { uv0, uv0, uv0 });
        newiopNoret(VOP_MAX, { uv31, uv0, uv0 });
        newiopNoret(VOP_MAX, { uv0, uv31, uv0 });
        newiopNoret(VOP_MAX, { uv0, uv0, uv31 });

        newiopNoret(VOP_SHL, { uv0, uv0, uv0 });
        newiopNoret(VOP_SHL, { uv31, uv0, uv0 });
        newiopNoret(VOP_SHL, { uv0, uv31, uv0 });
        newiopNoret(VOP_SHL, { uv0, uv0, uv31 });
        newiopNoret(VOP_SHL, { uv0, uv0, zero });
        newiopNoret(VOP_SHL, { uv31, uv0, zero });
        newiopNoret(VOP_SHL, { uv0, uv31, zero });
        newiopNoret(VOP_SHL, { uv0, uv0, t6 });
        newiopNoret(VOP_SHL, { uv0, uv0, argIImm(0) });
        newiopNoret(VOP_SHL, { uv31, uv0, argIImm(0) });
        newiopNoret(VOP_SHL, { uv0, uv31, argIImm(0) });
        newiopNoret(VOP_SHL, { uv0, uv0, argIImm(31) });
        newiopNoret(VOP_SHR, { uv0, uv0, uv0 });
        newiopNoret(VOP_SHR, { uv31, uv0, uv0 });
        newiopNoret(VOP_SHR, { uv0, uv31, uv0 });
        newiopNoret(VOP_SHR, { uv0, uv0, uv31 });
        newiopNoret(VOP_SHR, { uv0, uv0, zero });
        newiopNoret(VOP_SHR, { uv31, uv0, zero });
        newiopNoret(VOP_SHR, { uv0, uv31, zero });
        newiopNoret(VOP_SHR, { uv0, uv0, t6 });
        newiopNoret(VOP_SHR, { uv0, uv0, argIImm(0) });
        newiopNoret(VOP_SHR, { uv31, uv0, argIImm(0) });
        newiopNoret(VOP_SHR, { uv0, uv31, argIImm(0) });
        newiopNoret(VOP_SHR, { uv0, uv0, argIImm(31) });
        newiopNoret(VOP_SAR, { iv0, iv0, iv0 });
        newiopNoret(VOP_SAR, { iv31, iv0, iv0 });
        newiopNoret(VOP_SAR, { iv0, iv31, iv0 });
        newiopNoret(VOP_SAR, { iv0, iv0, iv31 });
        newiopNoret(VOP_SAR, { iv0, iv0, zero });
        newiopNoret(VOP_SAR, { iv31, iv0, zero });
        newiopNoret(VOP_SAR, { iv0, iv31, zero });
        newiopNoret(VOP_SAR, { iv0, iv0, t6 });
        newiopNoret(VOP_SAR, { iv0, iv0, argIImm(0) });
        newiopNoret(VOP_SAR, { iv31, iv0, argIImm(0) });
        newiopNoret(VOP_SAR, { iv0, iv31, argIImm(0) });
        newiopNoret(VOP_SAR, { iv0, iv0, argIImm(31) });

        newiopNoret(VOP_AND, { uv0, uv0, uv0 });
        newiopNoret(VOP_AND, { uv31, uv0, uv0 });
        newiopNoret(VOP_AND, { uv0, uv31, uv0 });
        newiopNoret(VOP_AND, { uv0, uv0, uv31 });
        newiopNoret(VOP_OR, { uv0, uv0, uv0 });
        newiopNoret(VOP_OR, { uv31, uv0, uv0 });
        newiopNoret(VOP_OR, { uv0, uv31, uv0 });
        newiopNoret(VOP_OR, { uv0, uv0, uv31 });
        newiopNoret(VOP_XOR, { uv0, uv0, uv0 });
        newiopNoret(VOP_XOR, { uv31, uv0, uv0 });
        newiopNoret(VOP_XOR, { uv0, uv31, uv0 });
        newiopNoret(VOP_XOR, { uv0, uv0, uv31 });
        newiopNoret(VOP_XOR, { uv0, uv0, argIImm(0) });
        newiopNoret(VOP_XOR, { uv31, uv0, argIImm(0) });
        newiopNoret(VOP_XOR, { uv0, uv31, argIImm(0) });
        newiopNoret(VOP_XOR, { uv0, uv0, argIImm(15) });

        newiopNoret(VOP_ADD, { fv0, fv0, fv0 });
        newiopNoret(VOP_ADD, { fv31, fv0, fv0 });
        newiopNoret(VOP_ADD, { fv0, fv31, fv0 });
        newiopNoret(VOP_ADD, { fv0, fv0, fv31 });
        newiopNoret(VOP_SUB, { fv0, fv0, fv0 });
        newiopNoret(VOP_SUB, { fv31, fv0, fv0 });
        newiopNoret(VOP_SUB, { fv0, fv31, fv0 });
        newiopNoret(VOP_SUB, { fv0, fv0, fv31 });
        newiopNoret(VOP_MUL, { fv0, fv0, fv0 });
        newiopNoret(VOP_MUL, { fv31, fv0, fv0 });
        newiopNoret(VOP_MUL, { fv0, fv31, fv0 });
        newiopNoret(VOP_MUL, { fv0, fv0, fv31 });
        newiopNoret(VOP_DIV, { fv0, fv0, fv0 });
        newiopNoret(VOP_DIV, { fv31, fv0, fv0 });
        newiopNoret(VOP_DIV, { fv0, fv31, fv0 });
        newiopNoret(VOP_DIV, { fv0, fv0, fv31 });
        newiopNoret(VOP_FMA, { fv0, fv0, fv0, fv0 });
        newiopNoret(VOP_FMA, { fv31, fv31, fv0, fv0 });
        newiopNoret(VOP_FMA, { fv0, fv0, fv31, fv0 });
        newiopNoret(VOP_FMA, { fv0, fv0, fv0, fv31 });
        newiopNoret(VOP_MIN, { fv0, fv0, fv0 });
        newiopNoret(VOP_MIN, { fv31, fv0, fv0 });
        newiopNoret(VOP_MIN, { fv0, fv31, fv0 });
        newiopNoret(VOP_MIN, { fv0, fv0, fv31 });
        newiopNoret(VOP_MAX, { fv0, fv0, fv0 });
        newiopNoret(VOP_MAX, { fv31, fv0, fv0 });
        newiopNoret(VOP_MAX, { fv0, fv31, fv0 });
        newiopNoret(VOP_MAX, { fv0, fv0, fv31 });
        newiopNoret(VOP_NEG, { fv0, fv0 });
        newiopNoret(VOP_NEG, { fv31, fv0 });
        newiopNoret(VOP_NEG, { fv0, fv31 });

        newiopNoret(VOP_RV_VMSEQ, { uv0, uv0, uv0 });
        newiopNoret(VOP_RV_VMSEQ, { uv31, uv0, uv0 });
        newiopNoret(VOP_RV_VMSEQ, { uv0, uv31, uv0 });
        newiopNoret(VOP_RV_VMSEQ, { uv0, uv0, uv31 });
        newiopNoret(VOP_RV_VMSNE, { uv0, uv0, uv0 });
        newiopNoret(VOP_RV_VMSNE, { uv31, uv0, uv0 });
        newiopNoret(VOP_RV_VMSNE, { uv0, uv31, uv0 });
        newiopNoret(VOP_RV_VMSNE, { uv0, uv0, uv31 });
        newiopNoret(VOP_RV_VMSLT, { iv0, iv0, iv0 });
        newiopNoret(VOP_RV_VMSLT, { iv31, iv0, iv0 });
        newiopNoret(VOP_RV_VMSLT, { iv0, iv31, iv0 });
        newiopNoret(VOP_RV_VMSLT, { iv0, iv0, iv31 });
        newiopNoret(VOP_RV_VMSLTU, { uv0, uv0, uv0 });
        newiopNoret(VOP_RV_VMSLTU, { uv31, uv0, uv0 });
        newiopNoret(VOP_RV_VMSLTU, { uv0, uv31, uv0 });
        newiopNoret(VOP_RV_VMSLTU, { uv0, uv0, uv31 });
        newiopNoret(VOP_RV_VMSLE, { iv0, iv0, iv0 });
        newiopNoret(VOP_RV_VMSLE, { iv31, iv0, iv0 });
        newiopNoret(VOP_RV_VMSLE, { iv0, iv31, iv0 });
        newiopNoret(VOP_RV_VMSLE, { iv0, iv0, iv31 });
        newiopNoret(VOP_RV_VMSLEU, { uv0, uv0, uv0 });
        newiopNoret(VOP_RV_VMSLEU, { uv31, uv0, uv0 });
        newiopNoret(VOP_RV_VMSLEU, { uv0, uv31, uv0 });
        newiopNoret(VOP_RV_VMSLEU, { uv0, uv0, uv31 });
        newiopNoret(VOP_RV_VMFEQ, { fv0, fv0, fv0 });
        newiopNoret(VOP_RV_VMFEQ, { fv31, fv0, fv0 });
        newiopNoret(VOP_RV_VMFEQ, { fv0, fv31, fv0 });
        newiopNoret(VOP_RV_VMFEQ, { fv0, fv0, fv31 });
        newiopNoret(VOP_RV_VMFNE, { fv0, fv0, fv0 });
        newiopNoret(VOP_RV_VMFNE, { fv31, fv0, fv0 });
        newiopNoret(VOP_RV_VMFNE, { fv0, fv31, fv0 });
        newiopNoret(VOP_RV_VMFNE, { fv0, fv0, fv31 });
        newiopNoret(VOP_RV_VMFLT, { fv0, fv0, fv0 });
        newiopNoret(VOP_RV_VMFLT, { fv31, fv0, fv0 });
        newiopNoret(VOP_RV_VMFLT, { fv0, fv31, fv0 });
        newiopNoret(VOP_RV_VMFLT, { fv0, fv0, fv31 });
        newiopNoret(VOP_RV_VMFLE, { fv0, fv0, fv0 });
        newiopNoret(VOP_RV_VMFLE, { fv31, fv0, fv0 });
        newiopNoret(VOP_RV_VMFLE, { fv0, fv31, fv0 });
        newiopNoret(VOP_RV_VMFLE, { fv0, fv0, fv31 });

        newiopNoret(VOP_CAST, { fv0, iv0 });
        newiopNoret(VOP_CAST, { fv31, iv0 });
        newiopNoret(VOP_CAST, { fv0, iv31 });
        newiopNoret(VOP_RV_VFCVT_X_F, { iv0, fv0 });
        newiopNoret(VOP_RV_VFCVT_X_F, { iv31, fv0 });
        newiopNoret(VOP_RV_VFCVT_X_F, { iv0, fv31 });
        newiopNoret(VOP_CAST, { iv0, fv0 });
        newiopNoret(VOP_CAST, { iv31, fv0 });
        newiopNoret(VOP_CAST, { iv0, fv31 });
        newiopNoret(VOP_CAST, { iv0, dv0 });
        newiopNoret(VOP_CAST, { iv31, dv0 });
        newiopNoret(VOP_CAST, { iv0, dv31 });
        newiopNoret(VOP_CAST_LOW, { i16v0, i8v0 });
        newiopNoret(VOP_CAST_LOW, { i16v31, i8v0 });
        newiopNoret(VOP_CAST_LOW, { i16v0, i8v31 });
        newiopNoret(VOP_CAST_LOW, { u16v0, u8v0 });
        newiopNoret(VOP_CAST_LOW, { u16v31, u8v0 });
        newiopNoret(VOP_CAST_LOW, { u16v0, u8v31 });
        newiopNoret(VOP_CAST_LOW, { dv0, fv0 });
        newiopNoret(VOP_CAST_LOW, { dv31, fv0 });
        newiopNoret(VOP_CAST_LOW, { dv0, fv31 });
        newiopNoret(VOP_CAST, { fv0, dv0 });
        newiopNoret(VOP_CAST, { fv31, dv0 });
        newiopNoret(VOP_CAST, { fv0, dv31 });

        newiopNoret(VOP_RV_VREDSUM, { iv0, iv0, iv0 });
        newiopNoret(VOP_RV_VREDSUM, { iv31, iv0, iv0 });
        newiopNoret(VOP_RV_VREDSUM, { iv0, iv31, iv0 });
        newiopNoret(VOP_RV_VREDSUM, { iv0, iv0, iv31 });
        newiopNoret(VOP_RV_VFREDUSUM, { fv0, fv0, fv0 });
        newiopNoret(VOP_RV_VFREDUSUM, { fv31, fv0, fv0 });
        newiopNoret(VOP_RV_VFREDUSUM, { fv0, fv31, fv0 });
        newiopNoret(VOP_RV_VFREDUSUM, { fv0, fv0, fv31 });
        newiopNoret(VOP_RV_VWREDSUM, { iv0, iv0, iv0 });
        newiopNoret(VOP_RV_VWREDSUM, { iv31, iv0, iv0 });
        newiopNoret(VOP_RV_VWREDSUM, { iv0, iv31, iv0 });
        newiopNoret(VOP_RV_VWREDSUM, { iv0, iv0, iv31 });
        newiopNoret(VOP_RV_VWREDSUMU, { uv0, uv0, uv0 });
        newiopNoret(VOP_RV_VWREDSUMU, { uv31, uv0, uv0 });
        newiopNoret(VOP_RV_VWREDSUMU, { uv0, uv31, uv0 });
        newiopNoret(VOP_RV_VWREDSUMU, { uv0, uv0, uv31 });
        newiopNoret(VOP_RV_VFREDOSUM, { fv0, fv0, fv0 });
        newiopNoret(VOP_RV_VFREDOSUM, { fv31, fv0, fv0 });
        newiopNoret(VOP_RV_VFREDOSUM, { fv0, fv31, fv0 });
        newiopNoret(VOP_RV_VFREDOSUM, { fv0, fv0, fv31 });

        newiopNoret(OP_RV_CSRRWI, { zero, argIImm(0), argIImm(0) });
        newiopNoret(OP_RV_CSRRWI, { t6, argIImm(0), argIImm(0) });
        newiopNoret(OP_RV_CSRRWI, { zero, argIImm(0xfff), argIImm(0) });
        newiopNoret(OP_RV_CSRRWI, { zero, argIImm(0), argIImm(31) });
        newiopNoret(OP_RV_CSRRW, { zero, argIImm(0), zero });
        newiopNoret(OP_RV_CSRRW, { t6, argIImm(0), zero });
        newiopNoret(OP_RV_CSRRW, { zero, argIImm(0xfff), zero });
        newiopNoret(OP_RV_CSRRW, { zero, argIImm(0), t6 });

        // newiopNoret(OP_RET, {}); //No registers, no immediates, no way to point function.
    }
    loops::Func func = ctx.getFunc(test_info_->name());
    EXPECT_ASSEMBLY_CORRECT(func);
}
#endif//__LOOPS_ARCH == __LOOPS_RISCV
