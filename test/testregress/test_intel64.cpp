/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "loops/loops.hpp"
#if __LOOPS_ARCH == __LOOPS_INTEL64
#include "loops/loops.hpp"
#include "src/common.hpp"
#include "src/func_impl.hpp"
#include "src/reg_allocator.hpp"
#include "tests.hpp"
#include <iostream>

using namespace loops;

TEST(intel64, instruction_set_test)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    STARTFUNC_(test_info_->name())
    {
        PREPARE_ASSEMBLY_TESTING(test_info_->name());

        DEFINE_ASSEMBLY_REG(rax, 0);
        DEFINE_ASSEMBLY_REG(rcx, 1 );
        DEFINE_ASSEMBLY_REG(rsp, 4 );
        DEFINE_ASSEMBLY_REG(rbp, 5 );
        DEFINE_ASSEMBLY_REG(rdi, 7 );
        DEFINE_ASSEMBLY_REG(r8 , 8 );
        DEFINE_ASSEMBLY_REG(r12, 12);
        DEFINE_ASSEMBLY_REG(r13, 13);
        DEFINE_ASSEMBLY_REG(r15, 15);

        DEFINE_ASSEMBLY_REG(eax , 0);
        DEFINE_ASSEMBLY_REG(edi , 7);
        DEFINE_ASSEMBLY_REG(r8d , 8);
        DEFINE_ASSEMBLY_REG(r12d, 12);
        DEFINE_ASSEMBLY_REG(r13d, 13);

        DEFINE_ASSEMBLY_REG(ax  , 0 );
        DEFINE_ASSEMBLY_REG(sp  , 4 );
        DEFINE_ASSEMBLY_REG(di  , 7 );
        DEFINE_ASSEMBLY_REG(r8w , 8 );
        DEFINE_ASSEMBLY_REG(r12w, 12);
        DEFINE_ASSEMBLY_REG(r13w, 13);

        DEFINE_ASSEMBLY_REG(al  , 0 );
        DEFINE_ASSEMBLY_REG(cl  , 1 );
        DEFINE_ASSEMBLY_REG(dil , 7 );
        DEFINE_ASSEMBLY_REG(r8b , 8 );
        DEFINE_ASSEMBLY_REG(r12b, 12);
        DEFINE_ASSEMBLY_REG(r13b, 13);

        Expr spilled32(argSpilled(RB_INT, 32));
        Expr spilled0x1FFF(argSpilled(RB_INT, 0x1FFF));
        spilled32.func() = _f;
        spilled0x1FFF.func() = _f;

        Expr ymm0_4s = VExpr<int32_t>(vregHid<int32_t>(0,_f)).notype();
        Expr ymm7_4s = VExpr<int32_t>(vregHid<int32_t>(7,_f)).notype();
        Expr ymm8_4s = VExpr<int32_t>(vregHid<int32_t>(8,_f)).notype();
        Expr ymm15_4s = VExpr<int32_t>(vregHid<int32_t>(15,_f)).notype();

        newiopNoret(OP_MOV, { rax, immtyped<int64_t>(0x10fffffff, _f) });
        newiopNoret(OP_MOV, { rdi, immtyped<int64_t>(0x10fffffff, _f) });
        newiopNoret(OP_MOV, {  r8, immtyped<int64_t>(0x10fffffff, _f) });
        newiopNoret(OP_MOV, { r15, immtyped<int64_t>(0x10fffffff, _f) });
        newiopNoret(OP_MOV, { spilled32, rax });
        newiopNoret(OP_MOV, { spilled32, rdi });
        newiopNoret(OP_MOV, { spilled32,  r8 });
        newiopNoret(OP_MOV, { spilled32, r15 });
        newiopNoret(OP_MOV, { spilled32, immtyped<int64_t>(0x101, _f) });

        newiopNoret(OP_ADD, { rax, rax, immtyped<int64_t>(0x100, _f) });
        newiopNoret(OP_ADD, { rcx, rcx, immtyped<int64_t>(0x100, _f) });
        newiopNoret(OP_ADD, { rdi, rdi, immtyped<int64_t>(0x100, _f) });
        newiopNoret(OP_ADD, {  r8,  r8, immtyped<int64_t>(0x100, _f) });
        newiopNoret(OP_ADD, { r15, r15, immtyped<int64_t>(0x100, _f) });
        newiopNoret(OP_ADD, { rax, rax, spilled32 });
        newiopNoret(OP_ADD, { rdi, rdi, spilled32 });
        newiopNoret(OP_ADD, { r8 , r8 , spilled32 });
        newiopNoret(OP_ADD, { r15, r15, spilled32 });
        newiopNoret(OP_ADD, { spilled32, spilled32, rax });
        newiopNoret(OP_ADD, { spilled32, spilled32, rdi });
        newiopNoret(OP_ADD, { spilled32, spilled32, r8  });
        newiopNoret(OP_ADD, { spilled32, spilled32, r15 });
        newiopNoret(OP_ADD, { spilled32, spilled32, immtyped<int64_t>(0x101, _f) });

        newiopNoret(OP_SUB, { rax, rax, immtyped<int64_t>(0x100, _f) });
        newiopNoret(OP_SUB, { rcx, rcx, immtyped<int64_t>(0x100, _f) });
        newiopNoret(OP_SUB, { rdi, rdi, immtyped<int64_t>(0x100, _f) });
        newiopNoret(OP_SUB, {  r8,  r8, immtyped<int64_t>(0x100, _f) });
        newiopNoret(OP_SUB, { r15, r15, immtyped<int64_t>(0x100, _f) });
        newiopNoret(OP_SUB, { rax, rax, spilled32 });
        newiopNoret(OP_SUB, { rdi, rdi, spilled32 });
        newiopNoret(OP_SUB, {  r8,  r8, spilled32 });
        newiopNoret(OP_SUB, { r15, r15, spilled32 });
        newiopNoret(OP_SUB, { spilled32, spilled32, rax });
        newiopNoret(OP_SUB, { spilled32, spilled32, rdi });
        newiopNoret(OP_SUB, { spilled32, spilled32,  r8 });
        newiopNoret(OP_SUB, { spilled32, spilled32, r15 });
        newiopNoret(OP_SUB, { spilled32, spilled32, immtyped<int64_t>(0x101, _f) });

        newiopNoret(OP_MUL, { rax, rax, spilled32 });
        newiopNoret(OP_MUL, { rdi, rdi, spilled32 });
        newiopNoret(OP_MUL, {  r8,  r8, spilled32 });
        newiopNoret(OP_MUL, { r15, r15, spilled32 });

        newiopNoret(OP_DIV, { rax, rax, spilled32 });

        newiopNoret(OP_NEG, { spilled32 });

        newiopNoret(OP_STORE, { rax, immtyped<uint64_t>(256, _f) });
        newiopNoret(OP_STORE, { rax, immtyped<int64_t>(256, _f) });
        newiopNoret(OP_STORE, { rdi, immtyped<int64_t>(256, _f) });
        newiopNoret(OP_STORE, { r8, immtyped<int64_t>(256, _f) });
        newiopNoret(OP_STORE, { r12, immtyped<int64_t>(256, _f) });
        newiopNoret(OP_STORE, { r13, immtyped<int64_t>(256, _f) });

        newiopNoret(OP_STORE, { rax, immtyped<uint32_t>(256, _f) });
        newiopNoret(OP_STORE, { rax, immtyped<int32_t>(256, _f) });
        newiopNoret(OP_STORE, { rdi, immtyped<int32_t>(256, _f) });
        newiopNoret(OP_STORE, { r8, immtyped<int32_t>(256, _f) });
        newiopNoret(OP_STORE, { r12, immtyped<int32_t>(256, _f) });
        newiopNoret(OP_STORE, { r13, immtyped<int32_t>(256, _f) });

        newiopNoret(OP_STORE, { rax, immtyped<uint16_t>(256, _f) });
        newiopNoret(OP_STORE, { rax, immtyped<int16_t>(256, _f) });
        newiopNoret(OP_STORE, { rdi, immtyped<int16_t>(256, _f) });
        newiopNoret(OP_STORE, { r8, immtyped<int16_t>(256, _f) });
        newiopNoret(OP_STORE, { r12, immtyped<int16_t>(256, _f) });
        newiopNoret(OP_STORE, { r13, immtyped<int16_t>(256, _f) });

        newiopNoret(OP_STORE, { rax, immtyped<uint8_t>(255, _f) });
        newiopNoret(OP_STORE, { rax, immtyped<int8_t>(255, _f) });
        newiopNoret(OP_STORE, { rdi, immtyped<int8_t>(255, _f) });
        newiopNoret(OP_STORE, { r8, immtyped<int8_t>(255, _f) });
        newiopNoret(OP_STORE, { r12, immtyped<int8_t>(255, _f) });
        newiopNoret(OP_STORE, { r13, immtyped<int8_t>(255, _f) });

        newiopNoret(OP_STORE, { rax, iregtyped<uint64_t>(rax)});
        newiopNoret(OP_STORE, { rax, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { rax, iregtyped<int64_t>(rdi)});
        newiopNoret(OP_STORE, { rax, iregtyped<int64_t>(r8)});
        newiopNoret(OP_STORE, { rdi, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { r8, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { r8, iregtyped<int64_t>(r8)});
        newiopNoret(OP_STORE, { r12, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { r12, iregtyped<int64_t>(rdi)});
        newiopNoret(OP_STORE, { r12, iregtyped<int64_t>(r8)});
        newiopNoret(OP_STORE, { r13, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { r13, iregtyped<int64_t>(rdi)});
        newiopNoret(OP_STORE, { r13, iregtyped<int64_t>(r8)});
        
        // // TODO(ch): Add here store_<uint32/int32>(rax, ax) just for symmetry;

        newiopNoret(OP_STORE, { rax, iregtyped<uint16_t>(ax)});
        newiopNoret(OP_STORE, { rax, iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { rax, iregtyped<int16_t>(di)});
        newiopNoret(OP_STORE, { rax, iregtyped<int16_t>(r8w)});
        newiopNoret(OP_STORE, { rdi, iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { r8 , iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { r8 , iregtyped<int16_t>(r8w)});
        newiopNoret(OP_STORE, { r12, iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { r12, iregtyped<int16_t>(di)});
        newiopNoret(OP_STORE, { r12, iregtyped<int16_t>(r8w)});
        newiopNoret(OP_STORE, { r13, iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { r13, iregtyped<int16_t>(di)});
        newiopNoret(OP_STORE, { r13, iregtyped<int16_t>(r8w)});

        newiopNoret(OP_STORE, { rax, iregtyped<uint8_t>(al)});
        newiopNoret(OP_STORE, { rax, iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { rax, iregtyped<int8_t>(dil)});
        newiopNoret(OP_STORE, { rax, iregtyped<int8_t>(r8b)});
        newiopNoret(OP_STORE, { rdi, iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { r8 , iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { r8 , iregtyped<int8_t>(r8b)});
        newiopNoret(OP_STORE, { r12, iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { r12, iregtyped<int8_t>(dil)});
        newiopNoret(OP_STORE, { r12, iregtyped<int8_t>(r8b)});
        newiopNoret(OP_STORE, { r13, iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { r13, iregtyped<int8_t>(dil)});
        newiopNoret(OP_STORE, { r13, iregtyped<int8_t>(r8b)});

        newiopNoret(OP_STORE, { rax, rax, iregtyped<uint64_t>(rax)});
        newiopNoret(OP_STORE, { rax, rax, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { rax, rax, iregtyped<int64_t>(rdi)});
        newiopNoret(OP_STORE, { rax, rax, iregtyped<int64_t>(r8)});
        newiopNoret(OP_STORE, { rax, rdi, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { rax, r8, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { rax, r8, iregtyped<int64_t>(r8)});
        newiopNoret(OP_STORE, { rdi, rax, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { r8, rax, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { r8, rax, iregtyped<int64_t>(r8)});
        newiopNoret(OP_STORE, { r8, r8, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { r8, r8, iregtyped<int64_t>(r8)});
        newiopNoret(OP_STORE, { r13, rax, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { r13, rax, iregtyped<int64_t>(r8)});
        newiopNoret(OP_STORE, { r13, r8, iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { r13, r8, iregtyped<int64_t>(r8)});

        newiopNoret(OP_STORE, { rax, rax, iregtyped<uint32_t>(eax)});
        newiopNoret(OP_STORE, { rax, rax, iregtyped<int32_t>(eax)});
        newiopNoret(OP_STORE, { rax, rax, iregtyped<int32_t>(edi)});
        newiopNoret(OP_STORE, { rax, rax, iregtyped<int32_t>(r8d)});
        newiopNoret(OP_STORE, { rax, rdi, iregtyped<int32_t>(eax)});
        newiopNoret(OP_STORE, { rax, r8, iregtyped<int32_t>(eax)});
        newiopNoret(OP_STORE, { rax, r8, iregtyped<int32_t>(r8d)});
        newiopNoret(OP_STORE, { rdi, rax, iregtyped<int32_t>(eax)});
        newiopNoret(OP_STORE, { r8, rax, iregtyped<int32_t>(eax)});
        newiopNoret(OP_STORE, { r8, rax, iregtyped<int32_t>(r8d)});
        newiopNoret(OP_STORE, { r8, r8, iregtyped<int32_t>(eax)});
        newiopNoret(OP_STORE, { r8, r8, iregtyped<int32_t>(r8d)});
        newiopNoret(OP_STORE, { r13, rax, iregtyped<int32_t>(eax)});
        newiopNoret(OP_STORE, { r13, rax, iregtyped<int32_t>(r8d)});
        newiopNoret(OP_STORE, { r13, r8, iregtyped<int32_t>(eax)});
        newiopNoret(OP_STORE, { r13, r8, iregtyped<int32_t>(r8d)});

        newiopNoret(OP_STORE, { rax, rax, iregtyped<uint16_t>(ax)});
        newiopNoret(OP_STORE, { rax, rax, iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { rax, rax, iregtyped<int16_t>(di)});
        newiopNoret(OP_STORE, { rax, rax, iregtyped<int16_t>(r8w)});
        newiopNoret(OP_STORE, { rax, rdi, iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { rax, r8, iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { rax, r8, iregtyped<int16_t>(r8w)});
        newiopNoret(OP_STORE, { rdi, rax, iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { r8, rax, iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { r8, rax, iregtyped<int16_t>(r8w)});
        newiopNoret(OP_STORE, { r8, r8, iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { r8, r8, iregtyped<int16_t>(r8w)});
        newiopNoret(OP_STORE, { r13, rax, iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { r13, r8, iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { r13, rax, iregtyped<int16_t>(r8w)});
        newiopNoret(OP_STORE, { r13, r8, iregtyped<int16_t>(r8w)});

        newiopNoret(OP_STORE, { rax, rax, iregtyped<uint8_t>(al)});
        newiopNoret(OP_STORE, { rax, rax, iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { rax, rax, iregtyped<int8_t>(dil)});
        newiopNoret(OP_STORE, { rax, rax, iregtyped<int8_t>(r8b)});
        newiopNoret(OP_STORE, { rax, rdi, iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { rax, r8, iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { rax, r8, iregtyped<int8_t>(dil)});
        newiopNoret(OP_STORE, { rax, r8, iregtyped<int8_t>(r8b)});
        newiopNoret(OP_STORE, { rdi, rax, iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { r8, rax, iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { r8, rax, iregtyped<int8_t>(dil)});
        newiopNoret(OP_STORE, { r8, rax, iregtyped<int8_t>(r8b)});
        newiopNoret(OP_STORE, { r8, r8, iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { r8, r8, iregtyped<int8_t>(r8b)});
        newiopNoret(OP_STORE, { r8, r8, iregtyped<int8_t>(dil)});
        newiopNoret(OP_STORE, { r13, rax, iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { r13, rax, iregtyped<int8_t>(r8b)});
        newiopNoret(OP_STORE, { r13, r8, iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { r13, r8, iregtyped<int8_t>(r8b)});

        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<uint64_t>(rax)});
        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int64_t>(rdi)});
        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int64_t>(r8)});
        newiopNoret(OP_STORE, { rdi, argIImm(256), iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int64_t>(r8)});
        newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int64_t>(rax)});
        newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int64_t>(rdi)});
        newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int64_t>(r8)});

        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<uint32_t>(eax)});
        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int32_t>(eax)});
        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int32_t>(edi)});
        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int32_t>(r8d)});
        newiopNoret(OP_STORE, { rdi, argIImm(256), iregtyped<int32_t>(eax)});
        newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int32_t>(eax)});
        newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int32_t>(r8d)});
        newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int32_t>(eax)});
        newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int32_t>(r8d)});

        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<uint16_t>(ax)});
        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int16_t>(di)});
        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int16_t>(r8w)});
        newiopNoret(OP_STORE, { rdi, argIImm(256), iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int16_t>(r8w)});
        newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int16_t>(ax)});
        newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int16_t>(r8w)});

        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<uint8_t>(al)});
        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int8_t>(dil)});
        newiopNoret(OP_STORE, { rax, argIImm(256), iregtyped<int8_t>(r8b)});
        newiopNoret(OP_STORE, { rdi, argIImm(256), iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { r8, argIImm(256), iregtyped<int8_t>(r8b)});
        newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int8_t>(al)});
        newiopNoret(OP_STORE, { r12, argIImm(256), iregtyped<int8_t>(r8b)});

        newiopNoret(OP_STORE, { rax, rax, immtyped<uint64_t>(256, _f)});
        newiopNoret(OP_STORE, { rax, rax, immtyped<int64_t>(256, _f)});
        newiopNoret(OP_STORE, { rax, rdi, immtyped<int64_t>(256, _f)});
        newiopNoret(OP_STORE, { rax, r8, immtyped<int64_t>(256, _f)});
        newiopNoret(OP_STORE, { rdi, rax, immtyped<int64_t>(256, _f)});
        newiopNoret(OP_STORE, { r8, rax, immtyped<int64_t>(256, _f)});
        newiopNoret(OP_STORE, { r8, r8, immtyped<int64_t>(256, _f)});
        newiopNoret(OP_STORE, { r13, rax, immtyped<int64_t>(256, _f)});
        newiopNoret(OP_STORE, { r13, r8, immtyped<int64_t>(256, _f)});

        newiopNoret(OP_STORE, { rax, rax, immtyped<uint32_t>(256, _f)});
        newiopNoret(OP_STORE, { rax, rax, immtyped<int32_t>(256, _f)});
        newiopNoret(OP_STORE, { rax, rdi, immtyped<int32_t>(256, _f)});
        newiopNoret(OP_STORE, { rax, r8, immtyped<int32_t>(256, _f)});
        newiopNoret(OP_STORE, { rdi, rax, immtyped<int32_t>(256, _f)});
        newiopNoret(OP_STORE, { r8, rax, immtyped<int32_t>(256, _f)});
        newiopNoret(OP_STORE, { r8, r8, immtyped<int32_t>(256, _f)});
        newiopNoret(OP_STORE, { r13, rax, immtyped<int32_t>(256, _f)});
        newiopNoret(OP_STORE, { r13, r8, immtyped<int32_t>(256, _f)});

        newiopNoret(OP_STORE, { rax, rax, immtyped<uint16_t>(256, _f)});
        newiopNoret(OP_STORE, { rax, rax, immtyped<int16_t>(256, _f)});
        newiopNoret(OP_STORE, { rax, rdi, immtyped<int16_t>(256, _f)});
        newiopNoret(OP_STORE, { rax, r8, immtyped<int16_t>(256, _f)});
        newiopNoret(OP_STORE, { rdi, rax, immtyped<int16_t>(256, _f)});
        newiopNoret(OP_STORE, { r8, rax, immtyped<int16_t>(256, _f)});
        newiopNoret(OP_STORE, { r8, r8, immtyped<int16_t>(256, _f)});
        newiopNoret(OP_STORE, { r13, rax, immtyped<int16_t>(256, _f)});
        newiopNoret(OP_STORE, { r13, r8, immtyped<int16_t>(256, _f)});

        newiopNoret(OP_STORE, { rax, rax, immtyped<uint8_t>(255, _f)});
        newiopNoret(OP_STORE, { rax, rax, immtyped<int8_t>(255, _f)});
        newiopNoret(OP_STORE, { rax, rdi, immtyped<int8_t>(255, _f)});
        newiopNoret(OP_STORE, { rax, r8, immtyped<int8_t>(255, _f)});
        newiopNoret(OP_STORE, { rdi, rax, immtyped<int8_t>(255, _f)});
        newiopNoret(OP_STORE, { r8, rax, immtyped<int8_t>(255, _f)});
        newiopNoret(OP_STORE, { r8, r8, immtyped<int8_t>(255, _f)});
        newiopNoret(OP_STORE, { r13, rax, immtyped<int8_t>(255, _f)});
        newiopNoret(OP_STORE, { r13, r8, immtyped<int8_t>(255, _f)});

        newiopNoret(OP_STORE, { rax, argIImm(257), immtyped<uint64_t>(256, _f)});
        newiopNoret(OP_STORE, { rax, argIImm(257), immtyped<int64_t>(256, _f)});
        newiopNoret(OP_STORE, { rsp, argIImm(257), immtyped<int64_t>(256, _f)});
        newiopNoret(OP_STORE, { rdi, argIImm(257), immtyped<int64_t>(256, _f)});
        newiopNoret(OP_STORE, { r8, argIImm(257), immtyped<int64_t>(256, _f)});
        newiopNoret(OP_STORE, { r12, argIImm(257), immtyped<int64_t>(256, _f)});

        newiopNoret(OP_STORE, { rax, argIImm(257), immtyped<uint32_t>(256, _f)});
        newiopNoret(OP_STORE, { rax, argIImm(257), immtyped<int32_t>(256, _f)});
        newiopNoret(OP_STORE, { rsp, argIImm(257), immtyped<int32_t>(256, _f)});
        newiopNoret(OP_STORE, { rdi, argIImm(257), immtyped<int32_t>(256, _f)});
        newiopNoret(OP_STORE, { r8, argIImm(257), immtyped<int32_t>(256, _f)});
        newiopNoret(OP_STORE, { r12, argIImm(257), immtyped<int32_t>(256, _f)});

        newiopNoret(OP_STORE, { rax, argIImm(257), immtyped<uint16_t>(256, _f)});
        newiopNoret(OP_STORE, { rax, argIImm(257), immtyped<int16_t>(256, _f)});
        newiopNoret(OP_STORE, { rsp, argIImm(257), immtyped<int16_t>(256, _f)});
        newiopNoret(OP_STORE, { rdi, argIImm(257), immtyped<int16_t>(256, _f)});
        newiopNoret(OP_STORE, { r8, argIImm(257), immtyped<int16_t>(256, _f)});
        newiopNoret(OP_STORE, { r12, argIImm(257), immtyped<int16_t>(256, _f)});

        newiopNoret(OP_STORE, { ax, argIImm(257), immtyped<uint8_t>(255, _f)});
        newiopNoret(OP_STORE, { sp, argIImm(257), immtyped<int8_t>(255, _f)});
        newiopNoret(OP_STORE, { ax, argIImm(257), immtyped<int8_t>(255, _f)});
        newiopNoret(OP_STORE, { di, argIImm(257), immtyped<int8_t>(255, _f)});
        newiopNoret(OP_STORE, { r8, argIImm(257), immtyped<int8_t>(255, _f)});
        newiopNoret(OP_STORE, { r12, argIImm(257), immtyped<int8_t>(255, _f)});

        newiopNoret(OP_LOAD, { iregtyped<uint64_t>(rax), rax});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax), rax });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rdi), rax });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax), rdi });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax), rax });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) , rax });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax), r8  });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax), r12 });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax), r13 });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) , r8  });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) , r12 });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) , r13  });

        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rax });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(edi), rax });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rdi });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rax });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), rax });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r8  });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r12 });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r13 });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r8  });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r12 });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r13 });

        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), rax });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(edi), rax });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), rdi });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), rax });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), r8  });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), r12 });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), r13 });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), r8  });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), r12 });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), r13 });

        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rax });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(di) , rax });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rdi });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), rax });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , r8  });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , r12  });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , r13  });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r8  });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r12  });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r13  });
        
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rax });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(di) , rax });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rdi });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), rax });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , r8  });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , r12 });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , r13 });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r8  });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r12  });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r13  });

        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rax});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(dil), rax});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rdi});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), rax});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r12 });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r13 });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r12 });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r13 });

        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rax});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(dil), rax});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rdi});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), rax});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r8 });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r12 });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r13 });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r8 });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r12 });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r13 });

        newiopNoret(OP_LOAD, { iregtyped<uint64_t>(rax), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rdi),  rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rsp, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rdi, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  r12, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  r12, argIImm(256)});

        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(edi), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rsp, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rdi, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r12, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r12, argIImm(256)});

        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(edi), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), rsp, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), rdi, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), r12, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), r12, argIImm(256)});

        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(di) , rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rsp, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rdi, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , r12, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r12, argIImm(256)});

        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(di) , rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rsp, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rdi, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , r12, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r12, argIImm(256)});

        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(dil), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rsp, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rdi, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r12, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r12, argIImm(256)});

        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(dil), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rsp, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rdi, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), rax, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r12, argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r8 , argIImm(256)});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r12, argIImm(256)});

        newiopNoret(OP_LOAD, { iregtyped<uint64_t>(rax), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rdi),  rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rdi, rax});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rax, rdi});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  r13, rax});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  r13, rax});
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(rax),  r13, r8 });
        newiopNoret(OP_LOAD, { iregtyped<int64_t>(r8) ,  r13, r8 });

        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(edi), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rdi, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rax, rdi});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r13, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r13, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(eax), r13, r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint32_t>(r8d), r13, r8 });

        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(edi), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), rdi, rax});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), rax, rdi});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), r13, rax});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), r13, rax});
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(eax), r13, r8 });
        newiopNoret(OP_LOAD, { iregtyped<int32_t>(r8d), r13, r8 });

        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(di) , rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rdi, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rax, rdi});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , r13, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r13, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(ax) , r13 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint16_t>(r8w), r13 , r8 });

        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(di) , rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rdi, rax});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rax, rdi});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , r13, rax});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r13, rax});
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(ax) , r13, r8 });
        newiopNoret(OP_LOAD, { iregtyped<int16_t>(r8w), r13, r8 });

        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(dil), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rdi, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rax, rdi});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r13 , rax});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r13 , rax});
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(al) , r13 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<uint8_t>(r8b), r13 , r8 });

        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(dil), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rdi, rax});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rax, rdi});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), rax, rax});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r8 , rax});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r13, rax});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r13, rax});
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), rax, r8 });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r8 , r8 });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(al) , r13, r8 });
        newiopNoret(OP_LOAD, { iregtyped<int8_t>(r8b), r13, r8 });

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

        newiopNoret(OP_XCHG, { rcx,  spilled32 });
        newiopNoret(OP_XCHG, { rdi,  spilled32 });
        newiopNoret(OP_XCHG, { r8 ,  spilled32 });
        newiopNoret(OP_XCHG, { r15,  spilled32 });

        newiopNoret(OP_XCHG, { spilled32, rcx });
        newiopNoret(OP_XCHG, { spilled32, rdi });
        newiopNoret(OP_XCHG, { spilled32, r8 });
        newiopNoret(OP_XCHG, { spilled32, r15 });

        newiopNoret(OP_SHL, { rax, rax, argIImm(15) });
        newiopNoret(OP_SHL, { rdi, rdi, argIImm(15) });
        newiopNoret(OP_SHL, { r8 , r8, argIImm(15) });

        newiopNoret(OP_SHL, { spilled32, spilled32, argIImm(15) });

        newiopNoret(OP_SHL, { rax, rax, cl });
        newiopNoret(OP_SHL, { rdi, rdi, cl });
        newiopNoret(OP_SHL, { r8, r8, cl });
        newiopNoret(OP_SHL, { spilled32, spilled32, cl });

        newiopNoret(OP_SHR, { rax,rax, argIImm(15) });
        newiopNoret(OP_SHR, { rdi,rdi, argIImm(15) });
        newiopNoret(OP_SHR, { r8 , r8, argIImm(15) });
        newiopNoret(OP_SHR, { spilled32, spilled32, argIImm(15) });

        newiopNoret(OP_SHR, { rax, rax, cl });
        newiopNoret(OP_SHR, { rdi, rdi, cl });
        newiopNoret(OP_SHR, { r8, r8, cl });
        newiopNoret(OP_SHR, { spilled32, spilled32, cl });

        newiopNoret(OP_SAR, { rax,rax, argIImm(15) });
        newiopNoret(OP_SAR, { rdi,rdi, argIImm(15) });
        newiopNoret(OP_SAR, { r8 , r8, argIImm(15) });
        newiopNoret(OP_SAR, { spilled32, spilled32, argIImm(15) });

        newiopNoret(OP_SAR, { rax, rax, cl });
        newiopNoret(OP_SAR, { rdi, rdi, cl });
        newiopNoret(OP_SAR, { r8, r8, cl });
        newiopNoret(OP_SAR, { spilled32, spilled32, cl });

        newiopNoret(OP_AND, { rax, rax, rax });
        newiopNoret(OP_AND, { rax, rax, rdi });
        newiopNoret(OP_AND, { rdi, rdi, rax });
        newiopNoret(OP_AND, { rax, rax, r8 });
        newiopNoret(OP_AND, { r8 , r8 , rax });
        newiopNoret(OP_AND, { r8 , r8 , r8 });

        newiopNoret(OP_AND, { rax, rax, spilled32 });
        newiopNoret(OP_AND, { rdi, rdi, spilled32 });
        newiopNoret(OP_AND, { r8 , r8 , spilled32 });

        newiopNoret(OP_AND, { rax, rax, argIImm(256) });
        newiopNoret(OP_AND, { rcx, rcx, argIImm(256) });
        newiopNoret(OP_AND, { rdi, rdi, argIImm(256) });
        newiopNoret(OP_AND, { r8 , r8 , argIImm(256) });

        newiopNoret(OP_AND, { spilled32, spilled32, rax });
        newiopNoret(OP_AND, { spilled32, spilled32, rdi });
        newiopNoret(OP_AND, { spilled32, spilled32, r8 });

        newiopNoret(OP_AND, { spilled32, spilled32, argIImm(257) });

        newiopNoret(OP_OR, { rax, rax, rax });
        newiopNoret(OP_OR, { rax, rax, rdi });
        newiopNoret(OP_OR, { rdi, rdi, rax });
        newiopNoret(OP_OR, { rax, rax, r8 });
        newiopNoret(OP_OR, { r8 , r8 , rax });

        newiopNoret(OP_OR, { spilled32, spilled32, rax });
        newiopNoret(OP_OR, { spilled32, spilled32, r8 });

        newiopNoret(OP_OR, { rax, rax, spilled32 });
        newiopNoret(OP_OR, { r8, r8, spilled32 });

        newiopNoret(OP_OR, { rax, rax, argIImm(256) });
        newiopNoret(OP_OR, { r8, r8, argIImm(256) });
        newiopNoret(OP_OR, { spilled32, spilled32, argIImm(257) });

        newiopNoret(OP_XOR, { rax, rax, rax });
        newiopNoret(OP_XOR, { rax, rax, rdi });
        newiopNoret(OP_XOR, { rdi, rdi, rax });
        newiopNoret(OP_XOR, { rax, rax, r8 });
        newiopNoret(OP_XOR, { r8 , r8 , rax });
        newiopNoret(OP_XOR, { r8 , r8 , r8 });

        newiopNoret(OP_XOR, { rax, rax, spilled32 });
        newiopNoret(OP_XOR, { rdi, rdi, spilled32 });
        newiopNoret(OP_XOR, { r8 , r8 , spilled32 });

        newiopNoret(OP_XOR, { rax, rax, argIImm(256) });
        newiopNoret(OP_XOR, { rcx, rcx, argIImm(256) });
        newiopNoret(OP_XOR, { rdi, rdi, argIImm(256) });
        newiopNoret(OP_XOR, { r8 , r8 , argIImm(256) });

        newiopNoret(OP_XOR, { spilled32, spilled32, rax });
        newiopNoret(OP_XOR, { spilled32, spilled32, rdi });
        newiopNoret(OP_XOR, { spilled32, spilled32, r8 });

        newiopNoret(OP_XOR, { spilled32, spilled32, argIImm(257) });

        newiopNoret(OP_SELECT, { rax, argIImm(OP_EQ), rax, rax });
        newiopNoret(OP_SELECT, { rax, argIImm(OP_NE), rax, rax });
        newiopNoret(OP_SELECT, { rax, argIImm(OP_LT), rax, rax });
        newiopNoret(OP_SELECT, { rax, argIImm(OP_GT), rax, rax });
        newiopNoret(OP_SELECT, { rax, argIImm(OP_LE), rax, rax });
        newiopNoret(OP_SELECT, { rax, argIImm(OP_GE), rax, rax });
        newiopNoret(OP_SELECT, { rax, argIImm(OP_S ), rax, rax });
        newiopNoret(OP_SELECT, { rax, argIImm(OP_NS), rax, rax });

        newiopNoret(OP_SELECT, { rdi, OP_EQ, rax, rdi });
        newiopNoret(OP_SELECT, { rax, OP_EQ, rdi, rax });

        newiopNoret(OP_SELECT, { r8 , OP_EQ, rax, r8 });
        newiopNoret(OP_SELECT, { rax, OP_EQ, r8 , rax });
        newiopNoret(OP_SELECT, { r8 , OP_EQ, r8 , r8 });

        newiopNoret(OP_SELECT, { rax, OP_EQ, spilled32, rax });
        newiopNoret(OP_SELECT, { rdi, OP_EQ, spilled32, rdi });
        newiopNoret(OP_SELECT, { r8,  OP_EQ, spilled32, r8 });

        newiopNoret(OP_IVERSON, { rax, argIImm(OP_EQ)});
        newiopNoret(OP_IVERSON, { rax, argIImm(OP_NE)});
        newiopNoret(OP_IVERSON, { rax, argIImm(OP_LT)});
        newiopNoret(OP_IVERSON, { rax, argIImm(OP_GT)});
        newiopNoret(OP_IVERSON, { rax, argIImm(OP_LE)});
        newiopNoret(OP_IVERSON, { rax, argIImm(OP_GE)});
        newiopNoret(OP_IVERSON, { rax, argIImm(OP_S )});
        newiopNoret(OP_IVERSON, { rax, argIImm(OP_NS)});

        newiopNoret(OP_IVERSON, { rdi, argIImm(OP_EQ) });
        newiopNoret(OP_IVERSON, { r8 , argIImm(OP_EQ) });
        newiopNoret(OP_IVERSON, { spilled32, argIImm(OP_EQ) });

        newiopNoret(OP_X86_ADC, { rax, rax, rax });
        newiopNoret(OP_X86_ADC, { rdi, rdi, rax });
        newiopNoret(OP_X86_ADC, { rax, rax, rdi });
        newiopNoret(OP_X86_ADC, { r8 , r8 , rax });
        newiopNoret(OP_X86_ADC, { rax, rax, r8 });
        newiopNoret(OP_X86_ADC, { r8 , r8 , r8 });

        newiopNoret(OP_X86_ADC, { rax, rax, argIImm(256) });
        newiopNoret(OP_X86_ADC, { rcx, rcx, argIImm(256) });
        newiopNoret(OP_X86_ADC, { rdi, rdi, argIImm(256) });
        newiopNoret(OP_X86_ADC, { r8 , r8 , argIImm(256) });
        newiopNoret(OP_X86_ADC, { rax, rax, spilled32 });
        newiopNoret(OP_X86_ADC, { rdi, rdi, spilled32 });
        newiopNoret(OP_X86_ADC, { r8 , r8 , spilled32 });
        newiopNoret(OP_X86_ADC, { spilled32, spilled32, rax });
        newiopNoret(OP_X86_ADC, { spilled32, spilled32, rdi });
        newiopNoret(OP_X86_ADC, { spilled32, spilled32, r8 });
        newiopNoret(OP_X86_ADC, { spilled32, spilled32, argIImm(256) });

        newiopNoret(OP_CMP, { rax, argSpilled(RB_INT, 0x1FFF) });
        newiopNoret(OP_CMP, { rdi, argSpilled(RB_INT, 0x1FFF) });
        newiopNoret(OP_CMP, { r8,  argSpilled(RB_INT, 0x1FFF) });
        newiopNoret(OP_CMP, { r15, argSpilled(RB_INT, 0x1FFF) });
        newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF), rax });
        newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF), rdi });
        newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF), r8  });
        newiopNoret(OP_CMP, { argSpilled(RB_INT, 0x1FFF), r15 });
        newiopNoret(OP_CMP, { spilled0x1FFF, argIImm(0x8888) });

        newiopNoret(OP_CALL_NORET, { rax });
        newiopNoret(OP_CALL_NORET, { rdi });
        newiopNoret(OP_CALL_NORET, {  r8 });
        newiopNoret(OP_CALL_NORET, { r15 });
        newiopNoret(OP_CALL_NORET, { spilled32 });

        newiopNoret(VOP_LOAD, { ymm0_4s , rax });
        newiopNoret(VOP_LOAD, { ymm7_4s , rax });
        newiopNoret(VOP_LOAD, { ymm0_4s , rsp });
        newiopNoret(VOP_LOAD, { ymm0_4s , rbp });
        newiopNoret(VOP_LOAD, { ymm0_4s , rdi });
        newiopNoret(VOP_LOAD, { ymm8_4s , rax });
        newiopNoret(VOP_LOAD, { ymm15_4s, rax });
        newiopNoret(VOP_LOAD, { ymm8_4s , rsp });
        newiopNoret(VOP_LOAD, { ymm8_4s , rbp });
        newiopNoret(VOP_LOAD, { ymm8_4s , rdi });
        newiopNoret(VOP_LOAD, { ymm0_4s , r8  });
        newiopNoret(VOP_LOAD, { ymm7_4s , r8  });
        newiopNoret(VOP_LOAD, { ymm0_4s , r12 });
        newiopNoret(VOP_LOAD, { ymm0_4s , r13 });
        newiopNoret(VOP_LOAD, { ymm0_4s , r15 });
        newiopNoret(VOP_LOAD, { ymm8_4s , r8  });
        newiopNoret(VOP_LOAD, { ymm15_4s, r8  });
        newiopNoret(VOP_LOAD, { ymm8_4s , r12 });
        newiopNoret(VOP_LOAD, { ymm8_4s , r13 });
        newiopNoret(VOP_LOAD, { ymm8_4s , r15 });

        newiopNoret(VOP_LOAD, {  ymm0_4s, rax, rax});
        newiopNoret(VOP_LOAD, {  ymm7_4s, rax, rax});
        newiopNoret(VOP_LOAD, {  ymm0_4s, rdi, rax});
        newiopNoret(VOP_LOAD, {  ymm0_4s, rax, rdi});
        newiopNoret(VOP_LOAD, {  ymm8_4s, rax, rax});
        newiopNoret(VOP_LOAD, { ymm15_4s, rax, rax});
        newiopNoret(VOP_LOAD, {  ymm8_4s, rdi, rax});
        newiopNoret(VOP_LOAD, {  ymm8_4s, rax, rdi});
        newiopNoret(VOP_LOAD, {  ymm0_4s,  r8, rax});
        newiopNoret(VOP_LOAD, {  ymm7_4s,  r8, rax});
        newiopNoret(VOP_LOAD, {  ymm0_4s, r15, rax});
        newiopNoret(VOP_LOAD, {  ymm0_4s, rax, rdi});
        newiopNoret(VOP_LOAD, {  ymm0_4s, rax,  r8});
        newiopNoret(VOP_LOAD, {  ymm7_4s, rax,  r8});
        newiopNoret(VOP_LOAD, {  ymm0_4s, rdi,  r8});
        newiopNoret(VOP_LOAD, {  ymm0_4s, rax, r15});
        newiopNoret(VOP_LOAD, {  ymm8_4s,  r8, rax});
        newiopNoret(VOP_LOAD, { ymm15_4s,  r8, rax});
        newiopNoret(VOP_LOAD, {  ymm8_4s, r15, rax});
        newiopNoret(VOP_LOAD, {  ymm8_4s,  r8, rdi});
        newiopNoret(VOP_LOAD, {  ymm8_4s, rax,  r8});
        newiopNoret(VOP_LOAD, { ymm15_4s, rax,  r8});
        newiopNoret(VOP_LOAD, {  ymm8_4s, rdi,  r8});
        newiopNoret(VOP_LOAD, {  ymm8_4s, rax, r15});
        newiopNoret(VOP_LOAD, {  ymm0_4s,  r8,  r8});
        newiopNoret(VOP_LOAD, {  ymm7_4s,  r8,  r8});
        newiopNoret(VOP_LOAD, {  ymm0_4s, r15,  r8});
        newiopNoret(VOP_LOAD, {  ymm0_4s, rax, r15});
        newiopNoret(VOP_LOAD, {  ymm8_4s,  r8,  r8});
        newiopNoret(VOP_LOAD, { ymm15_4s,  r8,  r8});
        newiopNoret(VOP_LOAD, {  ymm8_4s, r15,  r8});
        newiopNoret(VOP_LOAD, {  ymm8_4s,  r8, r15});

        newiopNoret(VOP_LOAD, { ymm0_4s , rax , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm7_4s , rax , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm0_4s , rsp , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm0_4s , rdi , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm8_4s , rax , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm15_4s, rax , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm8_4s , rsp , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm8_4s , rdi , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm0_4s , r8  , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm7_4s , r8  , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm0_4s , r12 , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm0_4s , r15 , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm8_4s , r8  , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm15_4s, r8  , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm8_4s , r12 , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm8_4s , r15 , argIImm(0x100)});

        newiopNoret(VOP_STORE, { rax, ymm0_4s });
        newiopNoret(VOP_STORE, { rsp, ymm0_4s });
        newiopNoret(VOP_STORE, { rbp, ymm0_4s });
        newiopNoret(VOP_STORE, { rdi, ymm0_4s });
        newiopNoret(VOP_STORE, { rax, ymm7_4s });
        newiopNoret(VOP_STORE, { rax, ymm8_4s });
        newiopNoret(VOP_STORE, { rsp, ymm8_4s });
        newiopNoret(VOP_STORE, { rbp, ymm8_4s });
        newiopNoret(VOP_STORE, { rdi, ymm8_4s });
        newiopNoret(VOP_STORE, { rax, ymm15_4s});
        newiopNoret(VOP_STORE, { r8 , ymm0_4s });
        newiopNoret(VOP_STORE, { r12, ymm0_4s });
        newiopNoret(VOP_STORE, { r13, ymm0_4s });
        newiopNoret(VOP_STORE, { r15, ymm0_4s });
        newiopNoret(VOP_STORE, { r8 , ymm7_4s });
        newiopNoret(VOP_STORE, { r8 , ymm8_4s });
        newiopNoret(VOP_STORE, { r12, ymm8_4s });
        newiopNoret(VOP_STORE, { r13, ymm8_4s });
        newiopNoret(VOP_STORE, { r15, ymm8_4s });
        newiopNoret(VOP_STORE, { r8 , ymm15_4s});

        newiopNoret(VOP_STORE, { rax, rax,  ymm0_4s });
        newiopNoret(VOP_STORE, { rdi, rax,  ymm0_4s });
        newiopNoret(VOP_STORE, { rax, rdi,  ymm0_4s });
        newiopNoret(VOP_STORE, { rax, rax,  ymm7_4s });
        newiopNoret(VOP_STORE, { rax, rax,  ymm8_4s });
        newiopNoret(VOP_STORE, { rdi, rax,  ymm8_4s });
        newiopNoret(VOP_STORE, { rax, rdi,  ymm8_4s });
        newiopNoret(VOP_STORE, { rax, rax, ymm15_4s });
        newiopNoret(VOP_STORE, {  r8, rax,  ymm0_4s });
        newiopNoret(VOP_STORE, { r15, rax,  ymm0_4s });
        newiopNoret(VOP_STORE, { rax, rdi,  ymm0_4s });
        newiopNoret(VOP_STORE, {  r8, rax,  ymm7_4s });
        newiopNoret(VOP_STORE, { rax,  r8,  ymm0_4s });
        newiopNoret(VOP_STORE, { rdi,  r8,  ymm0_4s });
        newiopNoret(VOP_STORE, { rax, r15,  ymm0_4s });
        newiopNoret(VOP_STORE, { rax,  r8,  ymm7_4s });
        newiopNoret(VOP_STORE, {  r8, rax,  ymm8_4s });
        newiopNoret(VOP_STORE, { r15, rax,  ymm8_4s });
        newiopNoret(VOP_STORE, {  r8, rdi,  ymm8_4s });
        newiopNoret(VOP_STORE, {  r8, rax, ymm15_4s });
        newiopNoret(VOP_STORE, { rax,  r8,  ymm8_4s });
        newiopNoret(VOP_STORE, { rdi,  r8,  ymm8_4s });
        newiopNoret(VOP_STORE, { rax, r15,  ymm8_4s });
        newiopNoret(VOP_STORE, { rax,  r8, ymm15_4s });
        newiopNoret(VOP_STORE, {  r8,  r8,  ymm0_4s });
        newiopNoret(VOP_STORE, { r15,  r8,  ymm0_4s });
        newiopNoret(VOP_STORE, { rax, r15,  ymm0_4s });
        newiopNoret(VOP_STORE, {  r8,  r8,  ymm7_4s });
        newiopNoret(VOP_STORE, {  r8,  r8,  ymm8_4s });
        newiopNoret(VOP_STORE, { r15,  r8,  ymm8_4s });
        newiopNoret(VOP_STORE, {  r8, r15,  ymm8_4s });
        newiopNoret(VOP_STORE, {  r8,  r8, ymm15_4s });

        newiopNoret(VOP_STORE, { rax , argIImm(0x100),  ymm0_4s });
        newiopNoret(VOP_STORE, { rsp , argIImm(0x100),  ymm0_4s });
        newiopNoret(VOP_STORE, { rdi , argIImm(0x100),  ymm0_4s });
        newiopNoret(VOP_STORE, { rax , argIImm(0x100),  ymm7_4s });
        newiopNoret(VOP_STORE, { rax , argIImm(0x100),  ymm8_4s });
        newiopNoret(VOP_STORE, { rsp , argIImm(0x100),  ymm8_4s });
        newiopNoret(VOP_STORE, { rdi , argIImm(0x100),  ymm8_4s });
        newiopNoret(VOP_STORE, { rax , argIImm(0x100), ymm15_4s });
        newiopNoret(VOP_STORE, { r8  , argIImm(0x100),  ymm0_4s });
        newiopNoret(VOP_STORE, { r12 , argIImm(0x100),  ymm0_4s });
        newiopNoret(VOP_STORE, { r15 , argIImm(0x100),  ymm0_4s });
        newiopNoret(VOP_STORE, { r8  , argIImm(0x100),  ymm7_4s });
        newiopNoret(VOP_STORE, { r8  , argIImm(0x100),  ymm8_4s });
        newiopNoret(VOP_STORE, { r12 , argIImm(0x100),  ymm8_4s });
        newiopNoret(VOP_STORE, { r15 , argIImm(0x100),  ymm8_4s });
        newiopNoret(VOP_STORE, { r8  , argIImm(0x100), ymm15_4s });

        newiopNoret(VOP_MUL, {  ymm0_4s,  ymm0_4s,  ymm0_4s});// c4 e2 7d 40 c0
        newiopNoret(VOP_MUL, {  ymm7_4s,  ymm0_4s,  ymm0_4s});// c4 e2 7d 40 f8
        newiopNoret(VOP_MUL, {  ymm0_4s,  ymm7_4s,  ymm0_4s});// c4 e2 45 40 c0
        newiopNoret(VOP_MUL, {  ymm0_4s,  ymm0_4s,  ymm7_4s});// c4 e2 7d 40 c7
        newiopNoret(VOP_MUL, {  ymm8_4s,  ymm0_4s,  ymm0_4s});// c4 62 7d 40 c0
        newiopNoret(VOP_MUL, { ymm15_4s,  ymm0_4s,  ymm0_4s});// c4 62 7d 40 f8
        newiopNoret(VOP_MUL, {  ymm8_4s,  ymm7_4s,  ymm0_4s});// c4 62 45 40 c0
        newiopNoret(VOP_MUL, {  ymm8_4s,  ymm0_4s,  ymm7_4s});// c4 62 7d 40 c7
        newiopNoret(VOP_MUL, {  ymm0_4s,  ymm8_4s,  ymm0_4s});// c4 e2 3d 40 c0
        newiopNoret(VOP_MUL, {  ymm7_4s,  ymm8_4s,  ymm0_4s});// c4 e2 3d 40 f8
        newiopNoret(VOP_MUL, {  ymm0_4s, ymm15_4s,  ymm0_4s});// c4 e2 05 40 c0
        newiopNoret(VOP_MUL, {  ymm0_4s,  ymm8_4s,  ymm7_4s});// c4 e2 3d 40 c7
        newiopNoret(VOP_MUL, {  ymm0_4s,  ymm0_4s,  ymm8_4s});// c4 c2 7d 40 c0
        newiopNoret(VOP_MUL, {  ymm7_4s,  ymm0_4s,  ymm8_4s});// c4 c2 7d 40 f8
        newiopNoret(VOP_MUL, {  ymm0_4s,  ymm7_4s,  ymm8_4s});// c4 c2 45 40 c0
        newiopNoret(VOP_MUL, {  ymm0_4s,  ymm0_4s, ymm15_4s});// c4 c2 7d 40 c7
    }
    loops::Func func = ctx.getFunc(test_info_->name());
    EXPECT_ASSEMBLY_CORRECT(func);
}
#endif//__LOOPS_ARCH == __LOOPS_INTEL64