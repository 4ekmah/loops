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
    const std::vector<int64_t> scalars_ref = {65535, 65536, -32768, -32769, 0x5F3759DF, conv.val64};
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

        // newiopNoret(OP_RET, {}); //No registers, no immediates, no way to point function.

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
