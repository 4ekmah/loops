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

enum { VCMP_EQ, VCMP_NE, VCMP_LT, VCMP_LE, VCMP_GT, VCMP_GE };

template<typename _Tp> struct CmpElemTraits {};

template<> struct CmpElemTraits<int8_t> {
    typedef uint8_t masktype;
    static inline std::string typestring() { return "i8"; } 
};

template<> struct CmpElemTraits<int16_t> {
    typedef uint16_t masktype;
    static inline std::string typestring() { return "i16"; } 
};

template<> struct CmpElemTraits<int32_t> {
    typedef uint32_t masktype;
    static inline std::string typestring() { return "i32"; } 
};

template<> struct CmpElemTraits<int64_t> {
    typedef uint64_t masktype;
    static inline std::string typestring() { return "i64"; } 
};

template<> struct CmpElemTraits<float> {
    typedef uint32_t masktype;
    static inline std::string typestring() { return "fp32"; } 
};

template<> struct CmpElemTraits<double> {
    typedef uint64_t masktype;
    static inline std::string typestring() { return "fp64"; } 
};

template<typename _Tp>
Func make_vector_cmp_func(Context ctx, int cmptype)
{
    USE_CONTEXT_(ctx);
    std::string cmptypestr = (cmptype == VCMP_EQ) ? "e" :
                             (cmptype == VCMP_NE) ? "n" :
                             (cmptype == VCMP_LT) ? "lt" :
                             (cmptype == VCMP_LE) ? "le" :
                             (cmptype == VCMP_GT) ? "gt" :
                           /*(cmptype == VCMP_GE)?*/"ge";
    std::string funcname = std::string("vector_cmp_") + cmptypestr + "_" + CmpElemTraits<_Tp>::typestring();
    IReg aptr, bptr, resptr, amount;
    STARTFUNC_(funcname, &aptr, &bptr, &resptr, &amount)
    {
        IReg i = CONST_(0);
        amount *= sizeof(_Tp);
        WHILE_(i < amount)
        {
            VReg<_Tp> a = loadvec<_Tp>(aptr, i);
            VReg<_Tp> b = loadvec<_Tp>(bptr, i);
            VReg<typename CmpElemTraits<_Tp>::masktype> res;
            switch(cmptype)
            {
                case VCMP_EQ: res.copyidx(a==b); break;
                case VCMP_NE: res.copyidx(a!=b); break;
                case VCMP_LT: res.copyidx(a<b); break;
                case VCMP_LE: res.copyidx(a<=b); break;
                case VCMP_GT: res.copyidx(a>b); break;
                case VCMP_GE: res.copyidx(a>=b); break;
            };
            storevec(resptr, i, res);
            i += ctx.vbytes();
        }
        RETURN_();
    }
    return ctx.getFunc(funcname);
}

template<typename _Tp>
void check_vector_cmp_func(int cmptype, Func tested_)
{
    std::vector<_Tp> a = {0, 4, 9, 0, -16, -24, 0, 22, 39, 0, -28, -55, -33, 0, 69, 48, 0, -82, -68, 0, 91, 87, 0, -98, -109, 0, 99, 127, 0, -97, -120, -63};
    typedef void (*vector_cmp_f)(_Tp*, _Tp*, typename CmpElemTraits<_Tp>::masktype*, int64_t);
    vector_cmp_f tested = reinterpret_cast<vector_cmp_f>(tested_.ptr());
    for(int shift = 0; shift < (int)a.size() - 1; shift++)
    {
        std::vector<_Tp> b(a.size(), 0);
        std::vector<typename CmpElemTraits<_Tp>::masktype> resvec(a.size(), 0);
        memcpy(b.data(), a.data() + shift, (a.size() - shift)*sizeof(_Tp));
        memcpy(b.data() + (a.size() - shift), a.data(), shift*sizeof(_Tp));
        tested(a.data(), b.data(), resvec.data(), a.size());
        for(int elnum = 0; elnum < (int)a.size(); elnum++)
        {
            typename CmpElemTraits<_Tp>::masktype res = 0;
            switch(cmptype)
            {
                case VCMP_EQ: res = (a[elnum] == b[elnum]); break;
                case VCMP_NE: res = (a[elnum]!= b[elnum]); break;
                case VCMP_LT: res = (a[elnum]< b[elnum]); break;
                case VCMP_LE: res = (a[elnum]<= b[elnum]); break;
                case VCMP_GT: res = (a[elnum]> b[elnum]); break;
                case VCMP_GE: res = (a[elnum]>= b[elnum]); break;
            };
            res = 0 - res;
            ASSERT_EQ(res, resvec[elnum]);
        }
    }
}

TEST(intel64, all_vector_comparings)
{
    Context ctx;
    std::vector<int> cmpoperations = {VCMP_EQ, VCMP_NE, VCMP_LT, VCMP_LE, VCMP_GT, VCMP_GE};
    for(int cnum = 0; cnum < (int)cmpoperations.size(); cnum++)
    {
        Func t = make_vector_cmp_func<int8_t>(ctx, cmpoperations[cnum]);
        check_vector_cmp_func<int8_t>(cmpoperations[cnum], t);
        t = make_vector_cmp_func<int16_t>(ctx, cmpoperations[cnum]);
        check_vector_cmp_func<int16_t>(cmpoperations[cnum], t);
        t = make_vector_cmp_func<int32_t>(ctx, cmpoperations[cnum]);
        check_vector_cmp_func<int32_t>(cmpoperations[cnum], t);
        t = make_vector_cmp_func<int64_t>(ctx, cmpoperations[cnum]);
        check_vector_cmp_func<int64_t>(cmpoperations[cnum], t);
        t = make_vector_cmp_func<float>(ctx, cmpoperations[cnum]);
        check_vector_cmp_func<float>(cmpoperations[cnum], t);
        t = make_vector_cmp_func<double>(ctx, cmpoperations[cnum]);
        check_vector_cmp_func<double>(cmpoperations[cnum], t);
    }
}

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

        Expr ymm0_32u = VExpr<uint8_t>(vregHid<uint8_t>(0,_f)).notype();
        Expr ymm7_32u = VExpr<uint8_t>(vregHid<uint8_t>(7,_f)).notype();
        Expr ymm8_32u = VExpr<uint8_t>(vregHid<uint8_t>(8,_f)).notype();
        Expr ymm15_32u = VExpr<uint8_t>(vregHid<uint8_t>(15,_f)).notype();
        Expr ymm0_32s = VExpr<int8_t>(vregHid<int8_t>(0,_f)).notype();
        Expr ymm7_32s = VExpr<int8_t>(vregHid<int8_t>(7,_f)).notype();
        Expr ymm8_32s = VExpr<int8_t>(vregHid<int8_t>(8,_f)).notype();
        Expr ymm15_32s = VExpr<int8_t>(vregHid<int8_t>(15,_f)).notype();
        Expr ymm0_16u = VExpr<uint16_t>(vregHid<uint16_t>(0,_f)).notype();
        Expr ymm7_16u = VExpr<uint16_t>(vregHid<uint16_t>(7,_f)).notype();
        Expr ymm8_16u = VExpr<uint16_t>(vregHid<uint16_t>(8,_f)).notype();
        Expr ymm15_16u = VExpr<uint16_t>(vregHid<uint16_t>(15,_f)).notype();
        Expr ymm0_16s = VExpr<int16_t>(vregHid<int16_t>(0,_f)).notype();
        Expr ymm7_16s = VExpr<int16_t>(vregHid<int16_t>(7,_f)).notype();
        Expr ymm8_16s = VExpr<int16_t>(vregHid<int16_t>(8,_f)).notype();
        Expr ymm15_16s = VExpr<int16_t>(vregHid<int16_t>(15,_f)).notype();
        Expr ymm0_8u = VExpr<uint32_t>(vregHid<uint32_t>(0,_f)).notype();
        Expr ymm7_8u = VExpr<uint32_t>(vregHid<uint32_t>(7,_f)).notype();
        Expr ymm8_8u = VExpr<uint32_t>(vregHid<uint32_t>(8,_f)).notype();
        Expr ymm15_8u = VExpr<uint32_t>(vregHid<uint32_t>(15,_f)).notype();
        Expr ymm0_8s = VExpr<int32_t>(vregHid<int32_t>(0,_f)).notype();
        Expr ymm7_8s = VExpr<int32_t>(vregHid<int32_t>(7,_f)).notype();
        Expr ymm8_8s = VExpr<int32_t>(vregHid<int32_t>(8,_f)).notype();
        Expr ymm15_8s = VExpr<int32_t>(vregHid<int32_t>(15,_f)).notype();
        Expr ymm0_4u = VExpr<uint64_t>(vregHid<uint64_t>(0,_f)).notype();
        Expr ymm7_4u = VExpr<uint64_t>(vregHid<uint64_t>(7,_f)).notype();
        Expr ymm8_4u = VExpr<uint64_t>(vregHid<uint64_t>(8,_f)).notype();
        Expr ymm15_4u = VExpr<uint64_t>(vregHid<uint64_t>(15,_f)).notype();
        Expr ymm0_4s = VExpr<int64_t>(vregHid<int64_t>(0,_f)).notype();
        Expr ymm7_4s = VExpr<int64_t>(vregHid<int64_t>(7,_f)).notype();
        Expr ymm8_4s = VExpr<int64_t>(vregHid<int64_t>(8,_f)).notype();
        Expr ymm15_4s = VExpr<int64_t>(vregHid<int64_t>(15,_f)).notype();
        Expr ymm0_8f = VExpr<float>(vregHid<float>(0,_f)).notype();
        Expr ymm7_8f = VExpr<float>(vregHid<float>(7,_f)).notype();
        Expr ymm8_8f = VExpr<float>(vregHid<float>(8,_f)).notype();
        Expr ymm15_8f = VExpr<float>(vregHid<float>(15,_f)).notype();
        Expr ymm0_4f = VExpr<double>(vregHid<double>(0,_f)).notype();
        Expr ymm7_4f = VExpr<double>(vregHid<double>(7,_f)).notype();
        Expr ymm8_4f = VExpr<double>(vregHid<double>(8,_f)).notype();
        Expr ymm15_4f = VExpr<double>(vregHid<double>(15,_f)).notype();

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

        newiopNoret(OP_MOV, {  ymm0_8s, ymm0_8s });   //DUBUG: check this block for sp/12|bp/13!
        newiopNoret(OP_MOV, {  ymm7_8s, ymm0_8s });
        newiopNoret(OP_MOV, {  ymm0_8s, ymm7_8s });
        newiopNoret(OP_MOV, {  ymm8_8s, ymm0_8s });
        newiopNoret(OP_MOV, { ymm15_8s, ymm0_8s });
        newiopNoret(OP_MOV, {  ymm8_8s, ymm7_8s });
        newiopNoret(OP_MOV, {  ymm0_8s,  ymm8_8s });
        newiopNoret(OP_MOV, {  ymm7_8s,  ymm8_8s });
        newiopNoret(OP_MOV, {  ymm0_8s, ymm15_8s });
        newiopNoret(OP_MOV, {  ymm8_8s,  ymm8_8s });
        newiopNoret(OP_MOV, { ymm15_8s,  ymm8_8s });
        newiopNoret(OP_MOV, {  ymm8_8s, ymm15_8s });

        newiopNoret(VOP_LOAD, { ymm0_8s , rax });
        newiopNoret(VOP_LOAD, { ymm7_8s , rax });
        newiopNoret(VOP_LOAD, { ymm0_8s , rsp });
        newiopNoret(VOP_LOAD, { ymm0_8s , rbp });
        newiopNoret(VOP_LOAD, { ymm0_8s , rdi });
        newiopNoret(VOP_LOAD, { ymm8_8s , rax });
        newiopNoret(VOP_LOAD, { ymm15_8s, rax });
        newiopNoret(VOP_LOAD, { ymm8_8s , rsp });
        newiopNoret(VOP_LOAD, { ymm8_8s , rbp });
        newiopNoret(VOP_LOAD, { ymm8_8s , rdi });
        newiopNoret(VOP_LOAD, { ymm0_8s , r8  });
        newiopNoret(VOP_LOAD, { ymm7_8s , r8  });
        newiopNoret(VOP_LOAD, { ymm0_8s , r12 });
        newiopNoret(VOP_LOAD, { ymm0_8s , r13 });
        newiopNoret(VOP_LOAD, { ymm0_8s , r15 });
        newiopNoret(VOP_LOAD, { ymm8_8s , r8  });
        newiopNoret(VOP_LOAD, { ymm15_8s, r8  });
        newiopNoret(VOP_LOAD, { ymm8_8s , r12 });
        newiopNoret(VOP_LOAD, { ymm8_8s , r13 });
        newiopNoret(VOP_LOAD, { ymm8_8s , r15 });

        newiopNoret(VOP_LOAD, {  ymm0_8s, rax, rax});
        newiopNoret(VOP_LOAD, {  ymm7_8s, rax, rax});
        newiopNoret(VOP_LOAD, {  ymm0_8s, rdi, rax});
        newiopNoret(VOP_LOAD, {  ymm0_8s, rax, rdi});
        newiopNoret(VOP_LOAD, {  ymm8_8s, rax, rax});
        newiopNoret(VOP_LOAD, { ymm15_8s, rax, rax});
        newiopNoret(VOP_LOAD, {  ymm8_8s, rdi, rax});
        newiopNoret(VOP_LOAD, {  ymm8_8s, rax, rdi});
        newiopNoret(VOP_LOAD, {  ymm0_8s,  r8, rax});
        newiopNoret(VOP_LOAD, {  ymm7_8s,  r8, rax});
        newiopNoret(VOP_LOAD, {  ymm0_8s, r15, rax});
        newiopNoret(VOP_LOAD, {  ymm0_8s, rax, rdi});
        newiopNoret(VOP_LOAD, {  ymm0_8s, rax,  r8});
        newiopNoret(VOP_LOAD, {  ymm7_8s, rax,  r8});
        newiopNoret(VOP_LOAD, {  ymm0_8s, rdi,  r8});
        newiopNoret(VOP_LOAD, {  ymm0_8s, rax, r15});
        newiopNoret(VOP_LOAD, {  ymm8_8s,  r8, rax});
        newiopNoret(VOP_LOAD, { ymm15_8s,  r8, rax});
        newiopNoret(VOP_LOAD, {  ymm8_8s, r15, rax});
        newiopNoret(VOP_LOAD, {  ymm8_8s,  r8, rdi});
        newiopNoret(VOP_LOAD, {  ymm8_8s, rax,  r8});
        newiopNoret(VOP_LOAD, { ymm15_8s, rax,  r8});
        newiopNoret(VOP_LOAD, {  ymm8_8s, rdi,  r8});
        newiopNoret(VOP_LOAD, {  ymm8_8s, rax, r15});
        newiopNoret(VOP_LOAD, {  ymm0_8s,  r8,  r8});
        newiopNoret(VOP_LOAD, {  ymm7_8s,  r8,  r8});
        newiopNoret(VOP_LOAD, {  ymm0_8s, r15,  r8});
        newiopNoret(VOP_LOAD, {  ymm0_8s, rax, r15});
        newiopNoret(VOP_LOAD, {  ymm8_8s,  r8,  r8});
        newiopNoret(VOP_LOAD, { ymm15_8s,  r8,  r8});
        newiopNoret(VOP_LOAD, {  ymm8_8s, r15,  r8});
        newiopNoret(VOP_LOAD, {  ymm8_8s,  r8, r15});

        newiopNoret(VOP_LOAD, { ymm0_8s , rax , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm7_8s , rax , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm0_8s , rsp , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm0_8s , rdi , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm8_8s , rax , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm15_8s, rax , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm8_8s , rsp , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm8_8s , rdi , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm0_8s , r8  , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm7_8s , r8  , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm0_8s , r12 , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm0_8s , r15 , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm8_8s , r8  , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm15_8s, r8  , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm8_8s , r12 , argIImm(0x100)});
        newiopNoret(VOP_LOAD, { ymm8_8s , r15 , argIImm(0x100)});

        newiopNoret(VOP_STORE, { rax, ymm0_8s });
        newiopNoret(VOP_STORE, { rsp, ymm0_8s });
        newiopNoret(VOP_STORE, { rbp, ymm0_8s });
        newiopNoret(VOP_STORE, { rdi, ymm0_8s });
        newiopNoret(VOP_STORE, { rax, ymm7_8s });
        newiopNoret(VOP_STORE, { rax, ymm8_8s });
        newiopNoret(VOP_STORE, { rsp, ymm8_8s });
        newiopNoret(VOP_STORE, { rbp, ymm8_8s });
        newiopNoret(VOP_STORE, { rdi, ymm8_8s });
        newiopNoret(VOP_STORE, { rax, ymm15_8s});
        newiopNoret(VOP_STORE, { r8 , ymm0_8s });
        newiopNoret(VOP_STORE, { r12, ymm0_8s });
        newiopNoret(VOP_STORE, { r13, ymm0_8s });
        newiopNoret(VOP_STORE, { r15, ymm0_8s });
        newiopNoret(VOP_STORE, { r8 , ymm7_8s });
        newiopNoret(VOP_STORE, { r8 , ymm8_8s });
        newiopNoret(VOP_STORE, { r12, ymm8_8s });
        newiopNoret(VOP_STORE, { r13, ymm8_8s });
        newiopNoret(VOP_STORE, { r15, ymm8_8s });
        newiopNoret(VOP_STORE, { r8 , ymm15_8s});

        newiopNoret(VOP_STORE, { rax, rax,  ymm0_8s });
        newiopNoret(VOP_STORE, { rdi, rax,  ymm0_8s });
        newiopNoret(VOP_STORE, { rax, rdi,  ymm0_8s });
        newiopNoret(VOP_STORE, { rax, rax,  ymm7_8s });
        newiopNoret(VOP_STORE, { rax, rax,  ymm8_8s });
        newiopNoret(VOP_STORE, { rdi, rax,  ymm8_8s });
        newiopNoret(VOP_STORE, { rax, rdi,  ymm8_8s });
        newiopNoret(VOP_STORE, { rax, rax, ymm15_8s });
        newiopNoret(VOP_STORE, {  r8, rax,  ymm0_8s });
        newiopNoret(VOP_STORE, { r15, rax,  ymm0_8s });
        newiopNoret(VOP_STORE, { rax, rdi,  ymm0_8s });
        newiopNoret(VOP_STORE, {  r8, rax,  ymm7_8s });
        newiopNoret(VOP_STORE, { rax,  r8,  ymm0_8s });
        newiopNoret(VOP_STORE, { rdi,  r8,  ymm0_8s });
        newiopNoret(VOP_STORE, { rax, r15,  ymm0_8s });
        newiopNoret(VOP_STORE, { rax,  r8,  ymm7_8s });
        newiopNoret(VOP_STORE, {  r8, rax,  ymm8_8s });
        newiopNoret(VOP_STORE, { r15, rax,  ymm8_8s });
        newiopNoret(VOP_STORE, {  r8, rdi,  ymm8_8s });
        newiopNoret(VOP_STORE, {  r8, rax, ymm15_8s });
        newiopNoret(VOP_STORE, { rax,  r8,  ymm8_8s });
        newiopNoret(VOP_STORE, { rdi,  r8,  ymm8_8s });
        newiopNoret(VOP_STORE, { rax, r15,  ymm8_8s });
        newiopNoret(VOP_STORE, { rax,  r8, ymm15_8s });
        newiopNoret(VOP_STORE, {  r8,  r8,  ymm0_8s });
        newiopNoret(VOP_STORE, { r15,  r8,  ymm0_8s });
        newiopNoret(VOP_STORE, { rax, r15,  ymm0_8s });
        newiopNoret(VOP_STORE, {  r8,  r8,  ymm7_8s });
        newiopNoret(VOP_STORE, {  r8,  r8,  ymm8_8s });
        newiopNoret(VOP_STORE, { r15,  r8,  ymm8_8s });
        newiopNoret(VOP_STORE, {  r8, r15,  ymm8_8s });
        newiopNoret(VOP_STORE, {  r8,  r8, ymm15_8s });

        newiopNoret(VOP_STORE, { rax , argIImm(0x100),  ymm0_8s });
        newiopNoret(VOP_STORE, { rsp , argIImm(0x100),  ymm0_8s });
        newiopNoret(VOP_STORE, { rdi , argIImm(0x100),  ymm0_8s });
        newiopNoret(VOP_STORE, { rax , argIImm(0x100),  ymm7_8s });
        newiopNoret(VOP_STORE, { rax , argIImm(0x100),  ymm8_8s });
        newiopNoret(VOP_STORE, { rsp , argIImm(0x100),  ymm8_8s });
        newiopNoret(VOP_STORE, { rdi , argIImm(0x100),  ymm8_8s });
        newiopNoret(VOP_STORE, { rax , argIImm(0x100), ymm15_8s });
        newiopNoret(VOP_STORE, { r8  , argIImm(0x100),  ymm0_8s });
        newiopNoret(VOP_STORE, { r12 , argIImm(0x100),  ymm0_8s });
        newiopNoret(VOP_STORE, { r15 , argIImm(0x100),  ymm0_8s });
        newiopNoret(VOP_STORE, { r8  , argIImm(0x100),  ymm7_8s });
        newiopNoret(VOP_STORE, { r8  , argIImm(0x100),  ymm8_8s });
        newiopNoret(VOP_STORE, { r12 , argIImm(0x100),  ymm8_8s });
        newiopNoret(VOP_STORE, { r15 , argIImm(0x100),  ymm8_8s });
        newiopNoret(VOP_STORE, { r8  , argIImm(0x100), ymm15_8s });

        newiopNoret(VOP_ADD, {  ymm0_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_ADD, {  ymm7_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_ADD, {  ymm0_32s,  ymm7_32s,  ymm0_32s});
        newiopNoret(VOP_ADD, {  ymm0_32s,  ymm0_32s,  ymm7_32s});
        newiopNoret(VOP_ADD, {  ymm8_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_ADD, { ymm15_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_ADD, {  ymm8_32s,  ymm7_32s,  ymm0_32s});
        newiopNoret(VOP_ADD, {  ymm8_32s,  ymm0_32s,  ymm7_32s});
        newiopNoret(VOP_ADD, {  ymm0_32s,  ymm8_32s,  ymm0_32s});
        newiopNoret(VOP_ADD, {  ymm7_32s,  ymm8_32s,  ymm0_32s});
        newiopNoret(VOP_ADD, {  ymm0_32s, ymm15_32s,  ymm0_32s});
        newiopNoret(VOP_ADD, {  ymm0_32s,  ymm8_32s,  ymm7_32s});
        newiopNoret(VOP_ADD, {  ymm0_32s,  ymm0_32s,  ymm8_32s});
        newiopNoret(VOP_ADD, {  ymm7_32s,  ymm0_32s,  ymm8_32s});
        newiopNoret(VOP_ADD, {  ymm0_32s,  ymm7_32s,  ymm8_32s});
        newiopNoret(VOP_ADD, {  ymm0_32s,  ymm0_32s, ymm15_32s});

        newiopNoret(VOP_ADD, {  ymm0_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_ADD, {  ymm7_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_ADD, {  ymm0_16s,  ymm7_16s,  ymm0_16s});
        newiopNoret(VOP_ADD, {  ymm0_16s,  ymm0_16s,  ymm7_16s});
        newiopNoret(VOP_ADD, {  ymm8_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_ADD, { ymm15_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_ADD, {  ymm8_16s,  ymm7_16s,  ymm0_16s});
        newiopNoret(VOP_ADD, {  ymm8_16s,  ymm0_16s,  ymm7_16s});
        newiopNoret(VOP_ADD, {  ymm0_16s,  ymm8_16s,  ymm0_16s});
        newiopNoret(VOP_ADD, {  ymm7_16s,  ymm8_16s,  ymm0_16s});
        newiopNoret(VOP_ADD, {  ymm0_16s, ymm15_16s,  ymm0_16s});
        newiopNoret(VOP_ADD, {  ymm0_16s,  ymm8_16s,  ymm7_16s});
        newiopNoret(VOP_ADD, {  ymm0_16s,  ymm0_16s,  ymm8_16s});
        newiopNoret(VOP_ADD, {  ymm7_16s,  ymm0_16s,  ymm8_16s});
        newiopNoret(VOP_ADD, {  ymm0_16s,  ymm7_16s,  ymm8_16s});
        newiopNoret(VOP_ADD, {  ymm0_16s,  ymm0_16s, ymm15_16s});

        newiopNoret(VOP_ADD, {  ymm0_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_ADD, {  ymm7_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_ADD, {  ymm0_8s,  ymm7_8s,  ymm0_8s});
        newiopNoret(VOP_ADD, {  ymm0_8s,  ymm0_8s,  ymm7_8s});
        newiopNoret(VOP_ADD, {  ymm8_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_ADD, { ymm15_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_ADD, {  ymm8_8s,  ymm7_8s,  ymm0_8s});
        newiopNoret(VOP_ADD, {  ymm8_8s,  ymm0_8s,  ymm7_8s});
        newiopNoret(VOP_ADD, {  ymm0_8s,  ymm8_8s,  ymm0_8s});
        newiopNoret(VOP_ADD, {  ymm7_8s,  ymm8_8s,  ymm0_8s});
        newiopNoret(VOP_ADD, {  ymm0_8s, ymm15_8s,  ymm0_8s});
        newiopNoret(VOP_ADD, {  ymm0_8s,  ymm8_8s,  ymm7_8s});
        newiopNoret(VOP_ADD, {  ymm0_8s,  ymm0_8s,  ymm8_8s});
        newiopNoret(VOP_ADD, {  ymm7_8s,  ymm0_8s,  ymm8_8s});
        newiopNoret(VOP_ADD, {  ymm0_8s,  ymm7_8s,  ymm8_8s});
        newiopNoret(VOP_ADD, {  ymm0_8s,  ymm0_8s, ymm15_8s});

        newiopNoret(VOP_ADD, {  ymm0_4s,  ymm0_4s,  ymm0_4s});
        newiopNoret(VOP_ADD, {  ymm7_4s,  ymm0_4s,  ymm0_4s});
        newiopNoret(VOP_ADD, {  ymm0_4s,  ymm7_4s,  ymm0_4s});
        newiopNoret(VOP_ADD, {  ymm0_4s,  ymm0_4s,  ymm7_4s});
        newiopNoret(VOP_ADD, {  ymm8_4s,  ymm0_4s,  ymm0_4s});
        newiopNoret(VOP_ADD, { ymm15_4s,  ymm0_4s,  ymm0_4s});
        newiopNoret(VOP_ADD, {  ymm8_4s,  ymm7_4s,  ymm0_4s});
        newiopNoret(VOP_ADD, {  ymm8_4s,  ymm0_4s,  ymm7_4s});
        newiopNoret(VOP_ADD, {  ymm0_4s,  ymm8_4s,  ymm0_4s});
        newiopNoret(VOP_ADD, {  ymm7_4s,  ymm8_4s,  ymm0_4s});
        newiopNoret(VOP_ADD, {  ymm0_4s, ymm15_4s,  ymm0_4s});
        newiopNoret(VOP_ADD, {  ymm0_4s,  ymm8_4s,  ymm7_4s});
        newiopNoret(VOP_ADD, {  ymm0_4s,  ymm0_4s,  ymm8_4s});
        newiopNoret(VOP_ADD, {  ymm7_4s,  ymm0_4s,  ymm8_4s});
        newiopNoret(VOP_ADD, {  ymm0_4s,  ymm7_4s,  ymm8_4s});
        newiopNoret(VOP_ADD, {  ymm0_4s,  ymm0_4s, ymm15_4s});

        newiopNoret(VOP_ADD, {  ymm0_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_ADD, {  ymm7_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_ADD, {  ymm0_8f,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_ADD, {  ymm0_8f,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_ADD, {  ymm8_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_ADD, { ymm15_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_ADD, {  ymm8_8f,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_ADD, {  ymm8_8f,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_ADD, {  ymm0_8f,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_ADD, {  ymm7_8f,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_ADD, {  ymm0_8f, ymm15_8f,  ymm0_8f});
        newiopNoret(VOP_ADD, {  ymm0_8f,  ymm8_8f,  ymm7_8f});
        newiopNoret(VOP_ADD, {  ymm0_8f,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_ADD, {  ymm7_8f,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_ADD, {  ymm0_8f,  ymm7_8f,  ymm8_8f});
        newiopNoret(VOP_ADD, {  ymm0_8f,  ymm0_8f, ymm15_8f});

        newiopNoret(VOP_ADD, {  ymm0_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_ADD, {  ymm7_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_ADD, {  ymm0_4f,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_ADD, {  ymm0_4f,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_ADD, {  ymm8_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_ADD, { ymm15_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_ADD, {  ymm8_4f,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_ADD, {  ymm8_4f,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_ADD, {  ymm0_4f,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_ADD, {  ymm7_4f,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_ADD, {  ymm0_4f, ymm15_4f,  ymm0_4f});
        newiopNoret(VOP_ADD, {  ymm0_4f,  ymm8_4f,  ymm7_4f});
        newiopNoret(VOP_ADD, {  ymm0_4f,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_ADD, {  ymm7_4f,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_ADD, {  ymm0_4f,  ymm7_4f,  ymm8_4f});
        newiopNoret(VOP_ADD, {  ymm0_4f,  ymm0_4f, ymm15_4f});

        newiopNoret(VOP_SUB, {  ymm0_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_SUB, {  ymm7_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_SUB, {  ymm0_32s,  ymm7_32s,  ymm0_32s});
        newiopNoret(VOP_SUB, {  ymm0_32s,  ymm0_32s,  ymm7_32s});
        newiopNoret(VOP_SUB, {  ymm8_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_SUB, { ymm15_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_SUB, {  ymm8_32s,  ymm7_32s,  ymm0_32s});
        newiopNoret(VOP_SUB, {  ymm8_32s,  ymm0_32s,  ymm7_32s});
        newiopNoret(VOP_SUB, {  ymm0_32s,  ymm8_32s,  ymm0_32s});
        newiopNoret(VOP_SUB, {  ymm7_32s,  ymm8_32s,  ymm0_32s});
        newiopNoret(VOP_SUB, {  ymm0_32s, ymm15_32s,  ymm0_32s});
        newiopNoret(VOP_SUB, {  ymm0_32s,  ymm8_32s,  ymm7_32s});
        newiopNoret(VOP_SUB, {  ymm0_32s,  ymm0_32s,  ymm8_32s});
        newiopNoret(VOP_SUB, {  ymm7_32s,  ymm0_32s,  ymm8_32s});
        newiopNoret(VOP_SUB, {  ymm0_32s,  ymm7_32s,  ymm8_32s});
        newiopNoret(VOP_SUB, {  ymm0_32s,  ymm0_32s, ymm15_32s});

        newiopNoret(VOP_SUB, {  ymm0_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_SUB, {  ymm7_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_SUB, {  ymm0_16s,  ymm7_16s,  ymm0_16s});
        newiopNoret(VOP_SUB, {  ymm0_16s,  ymm0_16s,  ymm7_16s});
        newiopNoret(VOP_SUB, {  ymm8_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_SUB, { ymm15_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_SUB, {  ymm8_16s,  ymm7_16s,  ymm0_16s});
        newiopNoret(VOP_SUB, {  ymm8_16s,  ymm0_16s,  ymm7_16s});
        newiopNoret(VOP_SUB, {  ymm0_16s,  ymm8_16s,  ymm0_16s});
        newiopNoret(VOP_SUB, {  ymm7_16s,  ymm8_16s,  ymm0_16s});
        newiopNoret(VOP_SUB, {  ymm0_16s, ymm15_16s,  ymm0_16s});
        newiopNoret(VOP_SUB, {  ymm0_16s,  ymm8_16s,  ymm7_16s});
        newiopNoret(VOP_SUB, {  ymm0_16s,  ymm0_16s,  ymm8_16s});
        newiopNoret(VOP_SUB, {  ymm7_16s,  ymm0_16s,  ymm8_16s});
        newiopNoret(VOP_SUB, {  ymm0_16s,  ymm7_16s,  ymm8_16s});
        newiopNoret(VOP_SUB, {  ymm0_16s,  ymm0_16s, ymm15_16s});

        newiopNoret(VOP_SUB, {  ymm0_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_SUB, {  ymm7_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_SUB, {  ymm0_8s,  ymm7_8s,  ymm0_8s});
        newiopNoret(VOP_SUB, {  ymm0_8s,  ymm0_8s,  ymm7_8s});
        newiopNoret(VOP_SUB, {  ymm8_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_SUB, { ymm15_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_SUB, {  ymm8_8s,  ymm7_8s,  ymm0_8s});
        newiopNoret(VOP_SUB, {  ymm8_8s,  ymm0_8s,  ymm7_8s});
        newiopNoret(VOP_SUB, {  ymm0_8s,  ymm8_8s,  ymm0_8s});
        newiopNoret(VOP_SUB, {  ymm7_8s,  ymm8_8s,  ymm0_8s});
        newiopNoret(VOP_SUB, {  ymm0_8s, ymm15_8s,  ymm0_8s});
        newiopNoret(VOP_SUB, {  ymm0_8s,  ymm8_8s,  ymm7_8s});
        newiopNoret(VOP_SUB, {  ymm0_8s,  ymm0_8s,  ymm8_8s});
        newiopNoret(VOP_SUB, {  ymm7_8s,  ymm0_8s,  ymm8_8s});
        newiopNoret(VOP_SUB, {  ymm0_8s,  ymm7_8s,  ymm8_8s});
        newiopNoret(VOP_SUB, {  ymm0_8s,  ymm0_8s, ymm15_8s});

        newiopNoret(VOP_SUB, {  ymm0_4s,  ymm0_4s,  ymm0_4s});
        newiopNoret(VOP_SUB, {  ymm7_4s,  ymm0_4s,  ymm0_4s});
        newiopNoret(VOP_SUB, {  ymm0_4s,  ymm7_4s,  ymm0_4s});
        newiopNoret(VOP_SUB, {  ymm0_4s,  ymm0_4s,  ymm7_4s});
        newiopNoret(VOP_SUB, {  ymm8_4s,  ymm0_4s,  ymm0_4s});
        newiopNoret(VOP_SUB, { ymm15_4s,  ymm0_4s,  ymm0_4s});
        newiopNoret(VOP_SUB, {  ymm8_4s,  ymm7_4s,  ymm0_4s});
        newiopNoret(VOP_SUB, {  ymm8_4s,  ymm0_4s,  ymm7_4s});
        newiopNoret(VOP_SUB, {  ymm0_4s,  ymm8_4s,  ymm0_4s});
        newiopNoret(VOP_SUB, {  ymm7_4s,  ymm8_4s,  ymm0_4s});
        newiopNoret(VOP_SUB, {  ymm0_4s, ymm15_4s,  ymm0_4s});
        newiopNoret(VOP_SUB, {  ymm0_4s,  ymm8_4s,  ymm7_4s});
        newiopNoret(VOP_SUB, {  ymm0_4s,  ymm0_4s,  ymm8_4s});
        newiopNoret(VOP_SUB, {  ymm7_4s,  ymm0_4s,  ymm8_4s});
        newiopNoret(VOP_SUB, {  ymm0_4s,  ymm7_4s,  ymm8_4s});
        newiopNoret(VOP_SUB, {  ymm0_4s,  ymm0_4s, ymm15_4s});

        newiopNoret(VOP_SUB, {  ymm0_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_SUB, {  ymm7_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_SUB, {  ymm0_8f,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_SUB, {  ymm0_8f,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_SUB, {  ymm8_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_SUB, { ymm15_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_SUB, {  ymm8_8f,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_SUB, {  ymm8_8f,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_SUB, {  ymm0_8f,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_SUB, {  ymm7_8f,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_SUB, {  ymm0_8f, ymm15_8f,  ymm0_8f});
        newiopNoret(VOP_SUB, {  ymm0_8f,  ymm8_8f,  ymm7_8f});
        newiopNoret(VOP_SUB, {  ymm0_8f,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_SUB, {  ymm7_8f,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_SUB, {  ymm0_8f,  ymm7_8f,  ymm8_8f});
        newiopNoret(VOP_SUB, {  ymm0_8f,  ymm0_8f, ymm15_8f});

        newiopNoret(VOP_SUB, {  ymm0_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_SUB, {  ymm7_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_SUB, {  ymm0_4f,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_SUB, {  ymm0_4f,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_SUB, {  ymm8_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_SUB, { ymm15_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_SUB, {  ymm8_4f,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_SUB, {  ymm8_4f,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_SUB, {  ymm0_4f,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_SUB, {  ymm7_4f,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_SUB, {  ymm0_4f, ymm15_4f,  ymm0_4f});
        newiopNoret(VOP_SUB, {  ymm0_4f,  ymm8_4f,  ymm7_4f});
        newiopNoret(VOP_SUB, {  ymm0_4f,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_SUB, {  ymm7_4f,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_SUB, {  ymm0_4f,  ymm7_4f,  ymm8_4f});
        newiopNoret(VOP_SUB, {  ymm0_4f,  ymm0_4f, ymm15_4f});

        newiopNoret(VOP_MUL, {  ymm0_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_MUL, {  ymm7_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_MUL, {  ymm0_16s,  ymm7_16s,  ymm0_16s});
        newiopNoret(VOP_MUL, {  ymm0_16s,  ymm0_16s,  ymm7_16s});
        newiopNoret(VOP_MUL, {  ymm8_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_MUL, { ymm15_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_MUL, {  ymm8_16s,  ymm7_16s,  ymm0_16s});
        newiopNoret(VOP_MUL, {  ymm8_16s,  ymm0_16s,  ymm7_16s});
        newiopNoret(VOP_MUL, {  ymm0_16s,  ymm8_16s,  ymm0_16s});
        newiopNoret(VOP_MUL, {  ymm7_16s,  ymm8_16s,  ymm0_16s});
        newiopNoret(VOP_MUL, {  ymm0_16s, ymm15_16s,  ymm0_16s});
        newiopNoret(VOP_MUL, {  ymm0_16s,  ymm8_16s,  ymm7_16s});
        newiopNoret(VOP_MUL, {  ymm0_16s,  ymm0_16s,  ymm8_16s});
        newiopNoret(VOP_MUL, {  ymm7_16s,  ymm0_16s,  ymm8_16s});
        newiopNoret(VOP_MUL, {  ymm0_16s,  ymm7_16s,  ymm8_16s});
        newiopNoret(VOP_MUL, {  ymm0_16s,  ymm0_16s, ymm15_16s});

        newiopNoret(VOP_MUL, {  ymm0_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_MUL, {  ymm7_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_MUL, {  ymm0_8s,  ymm7_8s,  ymm0_8s});
        newiopNoret(VOP_MUL, {  ymm0_8s,  ymm0_8s,  ymm7_8s});
        newiopNoret(VOP_MUL, {  ymm8_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_MUL, { ymm15_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_MUL, {  ymm8_8s,  ymm7_8s,  ymm0_8s});
        newiopNoret(VOP_MUL, {  ymm8_8s,  ymm0_8s,  ymm7_8s});
        newiopNoret(VOP_MUL, {  ymm0_8s,  ymm8_8s,  ymm0_8s});
        newiopNoret(VOP_MUL, {  ymm7_8s,  ymm8_8s,  ymm0_8s});
        newiopNoret(VOP_MUL, {  ymm0_8s, ymm15_8s,  ymm0_8s});
        newiopNoret(VOP_MUL, {  ymm0_8s,  ymm8_8s,  ymm7_8s});
        newiopNoret(VOP_MUL, {  ymm0_8s,  ymm0_8s,  ymm8_8s});
        newiopNoret(VOP_MUL, {  ymm7_8s,  ymm0_8s,  ymm8_8s});
        newiopNoret(VOP_MUL, {  ymm0_8s,  ymm7_8s,  ymm8_8s});
        newiopNoret(VOP_MUL, {  ymm0_8s,  ymm0_8s, ymm15_8s});

        newiopNoret(VOP_MUL, {  ymm0_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_MUL, {  ymm7_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_MUL, {  ymm0_8f,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_MUL, {  ymm0_8f,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_MUL, {  ymm8_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_MUL, { ymm15_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_MUL, {  ymm8_8f,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_MUL, {  ymm8_8f,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_MUL, {  ymm0_8f,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_MUL, {  ymm7_8f,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_MUL, {  ymm0_8f, ymm15_8f,  ymm0_8f});
        newiopNoret(VOP_MUL, {  ymm0_8f,  ymm8_8f,  ymm7_8f});
        newiopNoret(VOP_MUL, {  ymm0_8f,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_MUL, {  ymm7_8f,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_MUL, {  ymm0_8f,  ymm7_8f,  ymm8_8f});
        newiopNoret(VOP_MUL, {  ymm0_8f,  ymm0_8f, ymm15_8f});

        newiopNoret(VOP_MUL, {  ymm0_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_MUL, {  ymm7_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_MUL, {  ymm0_4f,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_MUL, {  ymm0_4f,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_MUL, {  ymm8_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_MUL, { ymm15_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_MUL, {  ymm8_4f,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_MUL, {  ymm8_4f,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_MUL, {  ymm0_4f,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_MUL, {  ymm7_4f,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_MUL, {  ymm0_4f, ymm15_4f,  ymm0_4f});
        newiopNoret(VOP_MUL, {  ymm0_4f,  ymm8_4f,  ymm7_4f});
        newiopNoret(VOP_MUL, {  ymm0_4f,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_MUL, {  ymm7_4f,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_MUL, {  ymm0_4f,  ymm7_4f,  ymm8_4f});
        newiopNoret(VOP_MUL, {  ymm0_4f,  ymm0_4f, ymm15_4f});

        newiopNoret(VOP_FMA, {  ymm0_8f,  ymm0_8f,  ymm0_8f,  ymm0_8f });
        newiopNoret(VOP_FMA, {  ymm7_8f,  ymm7_8f,  ymm0_8f,  ymm0_8f });
        newiopNoret(VOP_FMA, {  ymm0_8f,  ymm0_8f,  ymm7_8f,  ymm0_8f });
        newiopNoret(VOP_FMA, {  ymm0_8f,  ymm0_8f,  ymm0_8f,  ymm7_8f });
        newiopNoret(VOP_FMA, {  ymm8_8f,  ymm8_8f,  ymm0_8f,  ymm0_8f });
        newiopNoret(VOP_FMA, { ymm15_8f, ymm15_8f,  ymm0_8f,  ymm0_8f });
        newiopNoret(VOP_FMA, {  ymm8_8f,  ymm8_8f,  ymm7_8f,  ymm0_8f });
        newiopNoret(VOP_FMA, {  ymm8_8f,  ymm8_8f,  ymm0_8f,  ymm7_8f });
        newiopNoret(VOP_FMA, {  ymm0_8f,  ymm0_8f,  ymm8_8f,  ymm0_8f });
        newiopNoret(VOP_FMA, {  ymm7_8f,  ymm7_8f,  ymm8_8f,  ymm0_8f });
        newiopNoret(VOP_FMA, {  ymm0_8f,  ymm0_8f, ymm15_8f,  ymm0_8f });
        newiopNoret(VOP_FMA, {  ymm0_8f,  ymm0_8f,  ymm8_8f,  ymm7_8f });
        newiopNoret(VOP_FMA, {  ymm0_8f,  ymm0_8f,  ymm0_8f,  ymm8_8f });
        newiopNoret(VOP_FMA, {  ymm7_8f,  ymm7_8f,  ymm0_8f,  ymm8_8f });
        newiopNoret(VOP_FMA, {  ymm0_8f,  ymm0_8f,  ymm7_8f,  ymm8_8f });
        newiopNoret(VOP_FMA, {  ymm0_8f,  ymm0_8f,  ymm0_8f, ymm15_8f });

        newiopNoret(VOP_FMA, {  ymm0_4f,  ymm0_4f,  ymm0_4f,  ymm0_4f });
        newiopNoret(VOP_FMA, {  ymm7_4f,  ymm7_4f,  ymm0_4f,  ymm0_4f });
        newiopNoret(VOP_FMA, {  ymm0_4f,  ymm0_4f,  ymm7_4f,  ymm0_4f });
        newiopNoret(VOP_FMA, {  ymm0_4f,  ymm0_4f,  ymm0_4f,  ymm7_4f });
        newiopNoret(VOP_FMA, {  ymm8_4f,  ymm8_4f,  ymm0_4f,  ymm0_4f });
        newiopNoret(VOP_FMA, { ymm15_4f, ymm15_4f,  ymm0_4f,  ymm0_4f });
        newiopNoret(VOP_FMA, {  ymm8_4f,  ymm8_4f,  ymm7_4f,  ymm0_4f });
        newiopNoret(VOP_FMA, {  ymm8_4f,  ymm8_4f,  ymm0_4f,  ymm7_4f });
        newiopNoret(VOP_FMA, {  ymm0_4f,  ymm0_4f,  ymm8_4f,  ymm0_4f });
        newiopNoret(VOP_FMA, {  ymm7_4f,  ymm7_4f,  ymm8_4f,  ymm0_4f });
        newiopNoret(VOP_FMA, {  ymm0_4f,  ymm0_4f, ymm15_4f,  ymm0_4f });
        newiopNoret(VOP_FMA, {  ymm0_4f,  ymm0_4f,  ymm8_4f,  ymm7_4f });
        newiopNoret(VOP_FMA, {  ymm0_4f,  ymm0_4f,  ymm0_4f,  ymm8_4f });
        newiopNoret(VOP_FMA, {  ymm7_4f,  ymm7_4f,  ymm0_4f,  ymm8_4f });
        newiopNoret(VOP_FMA, {  ymm0_4f,  ymm0_4f,  ymm7_4f,  ymm8_4f });
        newiopNoret(VOP_FMA, {  ymm0_4f,  ymm0_4f,  ymm0_4f, ymm15_4f });

        newiopNoret(VOP_MIN, {  ymm0_32u,  ymm0_32u,  ymm0_32u});
        newiopNoret(VOP_MIN, {  ymm7_32u,  ymm0_32u,  ymm0_32u});
        newiopNoret(VOP_MIN, {  ymm0_32u,  ymm7_32u,  ymm0_32u});
        newiopNoret(VOP_MIN, {  ymm0_32u,  ymm0_32u,  ymm7_32u});
        newiopNoret(VOP_MIN, {  ymm8_32u,  ymm0_32u,  ymm0_32u});
        newiopNoret(VOP_MIN, { ymm15_32u,  ymm0_32u,  ymm0_32u});
        newiopNoret(VOP_MIN, {  ymm8_32u,  ymm7_32u,  ymm0_32u});
        newiopNoret(VOP_MIN, {  ymm8_32u,  ymm0_32u,  ymm7_32u});
        newiopNoret(VOP_MIN, {  ymm0_32u,  ymm8_32u,  ymm0_32u});
        newiopNoret(VOP_MIN, {  ymm7_32u,  ymm8_32u,  ymm0_32u});
        newiopNoret(VOP_MIN, {  ymm0_32u, ymm15_32u,  ymm0_32u});
        newiopNoret(VOP_MIN, {  ymm0_32u,  ymm8_32u,  ymm7_32u});
        newiopNoret(VOP_MIN, {  ymm0_32u,  ymm0_32u,  ymm8_32u});
        newiopNoret(VOP_MIN, {  ymm7_32u,  ymm0_32u,  ymm8_32u});
        newiopNoret(VOP_MIN, {  ymm0_32u,  ymm7_32u,  ymm8_32u});
        newiopNoret(VOP_MIN, {  ymm0_32u,  ymm0_32u, ymm15_32u});

        newiopNoret(VOP_MIN, {  ymm0_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_MIN, {  ymm7_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_MIN, {  ymm0_32s,  ymm7_32s,  ymm0_32s});
        newiopNoret(VOP_MIN, {  ymm0_32s,  ymm0_32s,  ymm7_32s});
        newiopNoret(VOP_MIN, {  ymm8_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_MIN, { ymm15_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_MIN, {  ymm8_32s,  ymm7_32s,  ymm0_32s});
        newiopNoret(VOP_MIN, {  ymm8_32s,  ymm0_32s,  ymm7_32s});
        newiopNoret(VOP_MIN, {  ymm0_32s,  ymm8_32s,  ymm0_32s});
        newiopNoret(VOP_MIN, {  ymm7_32s,  ymm8_32s,  ymm0_32s});
        newiopNoret(VOP_MIN, {  ymm0_32s, ymm15_32s,  ymm0_32s});
        newiopNoret(VOP_MIN, {  ymm0_32s,  ymm8_32s,  ymm7_32s});
        newiopNoret(VOP_MIN, {  ymm0_32s,  ymm0_32s,  ymm8_32s});
        newiopNoret(VOP_MIN, {  ymm7_32s,  ymm0_32s,  ymm8_32s});
        newiopNoret(VOP_MIN, {  ymm0_32s,  ymm7_32s,  ymm8_32s});
        newiopNoret(VOP_MIN, {  ymm0_32s,  ymm0_32s, ymm15_32s});

        newiopNoret(VOP_MIN, {  ymm0_16u,  ymm0_16u,  ymm0_16u});
        newiopNoret(VOP_MIN, {  ymm7_16u,  ymm0_16u,  ymm0_16u});
        newiopNoret(VOP_MIN, {  ymm0_16u,  ymm7_16u,  ymm0_16u});
        newiopNoret(VOP_MIN, {  ymm0_16u,  ymm0_16u,  ymm7_16u});
        newiopNoret(VOP_MIN, {  ymm8_16u,  ymm0_16u,  ymm0_16u});
        newiopNoret(VOP_MIN, { ymm15_16u,  ymm0_16u,  ymm0_16u});
        newiopNoret(VOP_MIN, {  ymm8_16u,  ymm7_16u,  ymm0_16u});
        newiopNoret(VOP_MIN, {  ymm8_16u,  ymm0_16u,  ymm7_16u});
        newiopNoret(VOP_MIN, {  ymm0_16u,  ymm8_16u,  ymm0_16u});
        newiopNoret(VOP_MIN, {  ymm7_16u,  ymm8_16u,  ymm0_16u});
        newiopNoret(VOP_MIN, {  ymm0_16u, ymm15_16u,  ymm0_16u});
        newiopNoret(VOP_MIN, {  ymm0_16u,  ymm8_16u,  ymm7_16u});
        newiopNoret(VOP_MIN, {  ymm0_16u,  ymm0_16u,  ymm8_16u});
        newiopNoret(VOP_MIN, {  ymm7_16u,  ymm0_16u,  ymm8_16u});
        newiopNoret(VOP_MIN, {  ymm0_16u,  ymm7_16u,  ymm8_16u});
        newiopNoret(VOP_MIN, {  ymm0_16u,  ymm0_16u, ymm15_16u});

        newiopNoret(VOP_MIN, {  ymm0_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_MIN, {  ymm7_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_MIN, {  ymm0_16s,  ymm7_16s,  ymm0_16s});
        newiopNoret(VOP_MIN, {  ymm0_16s,  ymm0_16s,  ymm7_16s});
        newiopNoret(VOP_MIN, {  ymm8_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_MIN, { ymm15_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_MIN, {  ymm8_16s,  ymm7_16s,  ymm0_16s});
        newiopNoret(VOP_MIN, {  ymm8_16s,  ymm0_16s,  ymm7_16s});
        newiopNoret(VOP_MIN, {  ymm0_16s,  ymm8_16s,  ymm0_16s});
        newiopNoret(VOP_MIN, {  ymm7_16s,  ymm8_16s,  ymm0_16s});
        newiopNoret(VOP_MIN, {  ymm0_16s, ymm15_16s,  ymm0_16s});
        newiopNoret(VOP_MIN, {  ymm0_16s,  ymm8_16s,  ymm7_16s});
        newiopNoret(VOP_MIN, {  ymm0_16s,  ymm0_16s,  ymm8_16s});
        newiopNoret(VOP_MIN, {  ymm7_16s,  ymm0_16s,  ymm8_16s});
        newiopNoret(VOP_MIN, {  ymm0_16s,  ymm7_16s,  ymm8_16s});
        newiopNoret(VOP_MIN, {  ymm0_16s,  ymm0_16s, ymm15_16s});

        newiopNoret(VOP_MIN, {  ymm0_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_MIN, {  ymm7_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_MIN, {  ymm0_8u,  ymm7_8u,  ymm0_8u});
        newiopNoret(VOP_MIN, {  ymm0_8u,  ymm0_8u,  ymm7_8u});
        newiopNoret(VOP_MIN, {  ymm8_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_MIN, { ymm15_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_MIN, {  ymm8_8u,  ymm7_8u,  ymm0_8u});
        newiopNoret(VOP_MIN, {  ymm8_8u,  ymm0_8u,  ymm7_8u});
        newiopNoret(VOP_MIN, {  ymm0_8u,  ymm8_8u,  ymm0_8u});
        newiopNoret(VOP_MIN, {  ymm7_8u,  ymm8_8u,  ymm0_8u});
        newiopNoret(VOP_MIN, {  ymm0_8u, ymm15_8u,  ymm0_8u});
        newiopNoret(VOP_MIN, {  ymm0_8u,  ymm8_8u,  ymm7_8u});
        newiopNoret(VOP_MIN, {  ymm0_8u,  ymm0_8u,  ymm8_8u});
        newiopNoret(VOP_MIN, {  ymm7_8u,  ymm0_8u,  ymm8_8u});
        newiopNoret(VOP_MIN, {  ymm0_8u,  ymm7_8u,  ymm8_8u});
        newiopNoret(VOP_MIN, {  ymm0_8u,  ymm0_8u, ymm15_8u});

        newiopNoret(VOP_MIN, {  ymm0_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_MIN, {  ymm7_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_MIN, {  ymm0_8s,  ymm7_8s,  ymm0_8s});
        newiopNoret(VOP_MIN, {  ymm0_8s,  ymm0_8s,  ymm7_8s});
        newiopNoret(VOP_MIN, {  ymm8_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_MIN, { ymm15_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_MIN, {  ymm8_8s,  ymm7_8s,  ymm0_8s});
        newiopNoret(VOP_MIN, {  ymm8_8s,  ymm0_8s,  ymm7_8s});
        newiopNoret(VOP_MIN, {  ymm0_8s,  ymm8_8s,  ymm0_8s});
        newiopNoret(VOP_MIN, {  ymm7_8s,  ymm8_8s,  ymm0_8s});
        newiopNoret(VOP_MIN, {  ymm0_8s, ymm15_8s,  ymm0_8s});
        newiopNoret(VOP_MIN, {  ymm0_8s,  ymm8_8s,  ymm7_8s});
        newiopNoret(VOP_MIN, {  ymm0_8s,  ymm0_8s,  ymm8_8s});
        newiopNoret(VOP_MIN, {  ymm7_8s,  ymm0_8s,  ymm8_8s});
        newiopNoret(VOP_MIN, {  ymm0_8s,  ymm7_8s,  ymm8_8s});
        newiopNoret(VOP_MIN, {  ymm0_8s,  ymm0_8s, ymm15_8s});

        newiopNoret(VOP_MIN, {  ymm0_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_MIN, {  ymm7_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_MIN, {  ymm0_8f,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_MIN, {  ymm0_8f,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_MIN, {  ymm8_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_MIN, { ymm15_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_MIN, {  ymm8_8f,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_MIN, {  ymm8_8f,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_MIN, {  ymm0_8f,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_MIN, {  ymm7_8f,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_MIN, {  ymm0_8f, ymm15_8f,  ymm0_8f});
        newiopNoret(VOP_MIN, {  ymm0_8f,  ymm8_8f,  ymm7_8f});
        newiopNoret(VOP_MIN, {  ymm0_8f,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_MIN, {  ymm7_8f,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_MIN, {  ymm0_8f,  ymm7_8f,  ymm8_8f});
        newiopNoret(VOP_MIN, {  ymm0_8f,  ymm0_8f, ymm15_8f});

        newiopNoret(VOP_MIN, {  ymm0_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_MIN, {  ymm7_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_MIN, {  ymm0_4f,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_MIN, {  ymm0_4f,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_MIN, {  ymm8_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_MIN, { ymm15_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_MIN, {  ymm8_4f,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_MIN, {  ymm8_4f,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_MIN, {  ymm0_4f,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_MIN, {  ymm7_4f,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_MIN, {  ymm0_4f, ymm15_4f,  ymm0_4f});
        newiopNoret(VOP_MIN, {  ymm0_4f,  ymm8_4f,  ymm7_4f});
        newiopNoret(VOP_MIN, {  ymm0_4f,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_MIN, {  ymm7_4f,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_MIN, {  ymm0_4f,  ymm7_4f,  ymm8_4f});
        newiopNoret(VOP_MIN, {  ymm0_4f,  ymm0_4f, ymm15_4f});

        newiopNoret(VOP_MAX, {  ymm0_32u,  ymm0_32u,  ymm0_32u});
        newiopNoret(VOP_MAX, {  ymm7_32u,  ymm0_32u,  ymm0_32u});
        newiopNoret(VOP_MAX, {  ymm0_32u,  ymm7_32u,  ymm0_32u});
        newiopNoret(VOP_MAX, {  ymm0_32u,  ymm0_32u,  ymm7_32u});
        newiopNoret(VOP_MAX, {  ymm8_32u,  ymm0_32u,  ymm0_32u});
        newiopNoret(VOP_MAX, { ymm15_32u,  ymm0_32u,  ymm0_32u});
        newiopNoret(VOP_MAX, {  ymm8_32u,  ymm7_32u,  ymm0_32u});
        newiopNoret(VOP_MAX, {  ymm8_32u,  ymm0_32u,  ymm7_32u});
        newiopNoret(VOP_MAX, {  ymm0_32u,  ymm8_32u,  ymm0_32u});
        newiopNoret(VOP_MAX, {  ymm7_32u,  ymm8_32u,  ymm0_32u});
        newiopNoret(VOP_MAX, {  ymm0_32u, ymm15_32u,  ymm0_32u});
        newiopNoret(VOP_MAX, {  ymm0_32u,  ymm8_32u,  ymm7_32u});
        newiopNoret(VOP_MAX, {  ymm0_32u,  ymm0_32u,  ymm8_32u});
        newiopNoret(VOP_MAX, {  ymm7_32u,  ymm0_32u,  ymm8_32u});
        newiopNoret(VOP_MAX, {  ymm0_32u,  ymm7_32u,  ymm8_32u});
        newiopNoret(VOP_MAX, {  ymm0_32u,  ymm0_32u, ymm15_32u});

        newiopNoret(VOP_MAX, {  ymm0_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_MAX, {  ymm7_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_MAX, {  ymm0_32s,  ymm7_32s,  ymm0_32s});
        newiopNoret(VOP_MAX, {  ymm0_32s,  ymm0_32s,  ymm7_32s});
        newiopNoret(VOP_MAX, {  ymm8_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_MAX, { ymm15_32s,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_MAX, {  ymm8_32s,  ymm7_32s,  ymm0_32s});
        newiopNoret(VOP_MAX, {  ymm8_32s,  ymm0_32s,  ymm7_32s});
        newiopNoret(VOP_MAX, {  ymm0_32s,  ymm8_32s,  ymm0_32s});
        newiopNoret(VOP_MAX, {  ymm7_32s,  ymm8_32s,  ymm0_32s});
        newiopNoret(VOP_MAX, {  ymm0_32s, ymm15_32s,  ymm0_32s});
        newiopNoret(VOP_MAX, {  ymm0_32s,  ymm8_32s,  ymm7_32s});
        newiopNoret(VOP_MAX, {  ymm0_32s,  ymm0_32s,  ymm8_32s});
        newiopNoret(VOP_MAX, {  ymm7_32s,  ymm0_32s,  ymm8_32s});
        newiopNoret(VOP_MAX, {  ymm0_32s,  ymm7_32s,  ymm8_32s});
        newiopNoret(VOP_MAX, {  ymm0_32s,  ymm0_32s, ymm15_32s});

        newiopNoret(VOP_MAX, {  ymm0_16u,  ymm0_16u,  ymm0_16u});
        newiopNoret(VOP_MAX, {  ymm7_16u,  ymm0_16u,  ymm0_16u});
        newiopNoret(VOP_MAX, {  ymm0_16u,  ymm7_16u,  ymm0_16u});
        newiopNoret(VOP_MAX, {  ymm0_16u,  ymm0_16u,  ymm7_16u});
        newiopNoret(VOP_MAX, {  ymm8_16u,  ymm0_16u,  ymm0_16u});
        newiopNoret(VOP_MAX, { ymm15_16u,  ymm0_16u,  ymm0_16u});
        newiopNoret(VOP_MAX, {  ymm8_16u,  ymm7_16u,  ymm0_16u});
        newiopNoret(VOP_MAX, {  ymm8_16u,  ymm0_16u,  ymm7_16u});
        newiopNoret(VOP_MAX, {  ymm0_16u,  ymm8_16u,  ymm0_16u});
        newiopNoret(VOP_MAX, {  ymm7_16u,  ymm8_16u,  ymm0_16u});
        newiopNoret(VOP_MAX, {  ymm0_16u, ymm15_16u,  ymm0_16u});
        newiopNoret(VOP_MAX, {  ymm0_16u,  ymm8_16u,  ymm7_16u});
        newiopNoret(VOP_MAX, {  ymm0_16u,  ymm0_16u,  ymm8_16u});
        newiopNoret(VOP_MAX, {  ymm7_16u,  ymm0_16u,  ymm8_16u});
        newiopNoret(VOP_MAX, {  ymm0_16u,  ymm7_16u,  ymm8_16u});
        newiopNoret(VOP_MAX, {  ymm0_16u,  ymm0_16u, ymm15_16u});

        newiopNoret(VOP_MAX, {  ymm0_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_MAX, {  ymm7_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_MAX, {  ymm0_16s,  ymm7_16s,  ymm0_16s});
        newiopNoret(VOP_MAX, {  ymm0_16s,  ymm0_16s,  ymm7_16s});
        newiopNoret(VOP_MAX, {  ymm8_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_MAX, { ymm15_16s,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_MAX, {  ymm8_16s,  ymm7_16s,  ymm0_16s});
        newiopNoret(VOP_MAX, {  ymm8_16s,  ymm0_16s,  ymm7_16s});
        newiopNoret(VOP_MAX, {  ymm0_16s,  ymm8_16s,  ymm0_16s});
        newiopNoret(VOP_MAX, {  ymm7_16s,  ymm8_16s,  ymm0_16s});
        newiopNoret(VOP_MAX, {  ymm0_16s, ymm15_16s,  ymm0_16s});
        newiopNoret(VOP_MAX, {  ymm0_16s,  ymm8_16s,  ymm7_16s});
        newiopNoret(VOP_MAX, {  ymm0_16s,  ymm0_16s,  ymm8_16s});
        newiopNoret(VOP_MAX, {  ymm7_16s,  ymm0_16s,  ymm8_16s});
        newiopNoret(VOP_MAX, {  ymm0_16s,  ymm7_16s,  ymm8_16s});
        newiopNoret(VOP_MAX, {  ymm0_16s,  ymm0_16s, ymm15_16s});

        newiopNoret(VOP_MAX, {  ymm0_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_MAX, {  ymm7_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_MAX, {  ymm0_8u,  ymm7_8u,  ymm0_8u});
        newiopNoret(VOP_MAX, {  ymm0_8u,  ymm0_8u,  ymm7_8u});
        newiopNoret(VOP_MAX, {  ymm8_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_MAX, { ymm15_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_MAX, {  ymm8_8u,  ymm7_8u,  ymm0_8u});
        newiopNoret(VOP_MAX, {  ymm8_8u,  ymm0_8u,  ymm7_8u});
        newiopNoret(VOP_MAX, {  ymm0_8u,  ymm8_8u,  ymm0_8u});
        newiopNoret(VOP_MAX, {  ymm7_8u,  ymm8_8u,  ymm0_8u});
        newiopNoret(VOP_MAX, {  ymm0_8u, ymm15_8u,  ymm0_8u});
        newiopNoret(VOP_MAX, {  ymm0_8u,  ymm8_8u,  ymm7_8u});
        newiopNoret(VOP_MAX, {  ymm0_8u,  ymm0_8u,  ymm8_8u});
        newiopNoret(VOP_MAX, {  ymm7_8u,  ymm0_8u,  ymm8_8u});
        newiopNoret(VOP_MAX, {  ymm0_8u,  ymm7_8u,  ymm8_8u});
        newiopNoret(VOP_MAX, {  ymm0_8u,  ymm0_8u, ymm15_8u});

        newiopNoret(VOP_MAX, {  ymm0_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_MAX, {  ymm7_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_MAX, {  ymm0_8s,  ymm7_8s,  ymm0_8s});
        newiopNoret(VOP_MAX, {  ymm0_8s,  ymm0_8s,  ymm7_8s});
        newiopNoret(VOP_MAX, {  ymm8_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_MAX, { ymm15_8s,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_MAX, {  ymm8_8s,  ymm7_8s,  ymm0_8s});
        newiopNoret(VOP_MAX, {  ymm8_8s,  ymm0_8s,  ymm7_8s});
        newiopNoret(VOP_MAX, {  ymm0_8s,  ymm8_8s,  ymm0_8s});
        newiopNoret(VOP_MAX, {  ymm7_8s,  ymm8_8s,  ymm0_8s});
        newiopNoret(VOP_MAX, {  ymm0_8s, ymm15_8s,  ymm0_8s});
        newiopNoret(VOP_MAX, {  ymm0_8s,  ymm8_8s,  ymm7_8s});
        newiopNoret(VOP_MAX, {  ymm0_8s,  ymm0_8s,  ymm8_8s});
        newiopNoret(VOP_MAX, {  ymm7_8s,  ymm0_8s,  ymm8_8s});
        newiopNoret(VOP_MAX, {  ymm0_8s,  ymm7_8s,  ymm8_8s});
        newiopNoret(VOP_MAX, {  ymm0_8s,  ymm0_8s, ymm15_8s});

        newiopNoret(VOP_MAX, {  ymm0_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_MAX, {  ymm7_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_MAX, {  ymm0_8f,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_MAX, {  ymm0_8f,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_MAX, {  ymm8_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_MAX, { ymm15_8f,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_MAX, {  ymm8_8f,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_MAX, {  ymm8_8f,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_MAX, {  ymm0_8f,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_MAX, {  ymm7_8f,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_MAX, {  ymm0_8f, ymm15_8f,  ymm0_8f});
        newiopNoret(VOP_MAX, {  ymm0_8f,  ymm8_8f,  ymm7_8f});
        newiopNoret(VOP_MAX, {  ymm0_8f,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_MAX, {  ymm7_8f,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_MAX, {  ymm0_8f,  ymm7_8f,  ymm8_8f});
        newiopNoret(VOP_MAX, {  ymm0_8f,  ymm0_8f, ymm15_8f});

        newiopNoret(VOP_MAX, {  ymm0_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_MAX, {  ymm7_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_MAX, {  ymm0_4f,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_MAX, {  ymm0_4f,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_MAX, {  ymm8_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_MAX, { ymm15_4f,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_MAX, {  ymm8_4f,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_MAX, {  ymm8_4f,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_MAX, {  ymm0_4f,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_MAX, {  ymm7_4f,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_MAX, {  ymm0_4f, ymm15_4f,  ymm0_4f});
        newiopNoret(VOP_MAX, {  ymm0_4f,  ymm8_4f,  ymm7_4f});
        newiopNoret(VOP_MAX, {  ymm0_4f,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_MAX, {  ymm7_4f,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_MAX, {  ymm0_4f,  ymm7_4f,  ymm8_4f});
        newiopNoret(VOP_MAX, {  ymm0_4f,  ymm0_4f, ymm15_4f});

        newiopNoret(VOP_FLOOR, {  ymm0_8f,  ymm0_8f });
        newiopNoret(VOP_FLOOR, {  ymm7_8f,  ymm0_8f });
        newiopNoret(VOP_FLOOR, {  ymm0_8f,  ymm7_8f });
        newiopNoret(VOP_FLOOR, {  ymm8_8f,  ymm0_8f });
        newiopNoret(VOP_FLOOR, { ymm15_8f,  ymm0_8f });
        newiopNoret(VOP_FLOOR, {  ymm8_8f,  ymm7_8f });
        newiopNoret(VOP_FLOOR, {  ymm0_8f,  ymm8_8f });
        newiopNoret(VOP_FLOOR, {  ymm7_8f,  ymm8_8f });
        newiopNoret(VOP_FLOOR, {  ymm0_8f, ymm15_8f });
        newiopNoret(VOP_FLOOR, {  ymm8_8f,  ymm8_8f });
        newiopNoret(VOP_FLOOR, { ymm15_8f,  ymm8_8f });
        newiopNoret(VOP_FLOOR, {  ymm8_8f, ymm15_8f });

        newiopNoret(VOP_FLOOR, {  ymm0_4f,  ymm0_4f });
        newiopNoret(VOP_FLOOR, {  ymm7_4f,  ymm0_4f });
        newiopNoret(VOP_FLOOR, {  ymm0_4f,  ymm7_4f });
        newiopNoret(VOP_FLOOR, {  ymm8_4f,  ymm0_4f });
        newiopNoret(VOP_FLOOR, { ymm15_4f,  ymm0_4f });
        newiopNoret(VOP_FLOOR, {  ymm8_4f,  ymm7_4f });
        newiopNoret(VOP_FLOOR, {  ymm0_4f,  ymm8_4f });
        newiopNoret(VOP_FLOOR, {  ymm7_4f,  ymm8_4f });
        newiopNoret(VOP_FLOOR, {  ymm0_4f, ymm15_4f });
        newiopNoret(VOP_FLOOR, {  ymm8_4f,  ymm8_4f });
        newiopNoret(VOP_FLOOR, { ymm15_4f,  ymm8_4f });
        newiopNoret(VOP_FLOOR, {  ymm8_4f, ymm15_4f });

        newiopNoret(VOP_TRUNC, {  ymm0_8f,  ymm0_8f });
        newiopNoret(VOP_TRUNC, {  ymm7_8f,  ymm0_8f });
        newiopNoret(VOP_TRUNC, {  ymm0_8f,  ymm7_8f });
        newiopNoret(VOP_TRUNC, {  ymm8_8f,  ymm0_8f });
        newiopNoret(VOP_TRUNC, { ymm15_8f,  ymm0_8f });
        newiopNoret(VOP_TRUNC, {  ymm8_8f,  ymm7_8f });
        newiopNoret(VOP_TRUNC, {  ymm0_8f,  ymm8_8f });
        newiopNoret(VOP_TRUNC, {  ymm7_8f,  ymm8_8f });
        newiopNoret(VOP_TRUNC, {  ymm0_8f, ymm15_8f });
        newiopNoret(VOP_TRUNC, {  ymm8_8f,  ymm8_8f });
        newiopNoret(VOP_TRUNC, { ymm15_8f,  ymm8_8f });
        newiopNoret(VOP_TRUNC, {  ymm8_8f, ymm15_8f });

        newiopNoret(VOP_TRUNC, {  ymm0_4f,  ymm0_4f });
        newiopNoret(VOP_TRUNC, {  ymm7_4f,  ymm0_4f });
        newiopNoret(VOP_TRUNC, {  ymm0_4f,  ymm7_4f });
        newiopNoret(VOP_TRUNC, {  ymm8_4f,  ymm0_4f });
        newiopNoret(VOP_TRUNC, { ymm15_4f,  ymm0_4f });
        newiopNoret(VOP_TRUNC, {  ymm8_4f,  ymm7_4f });
        newiopNoret(VOP_TRUNC, {  ymm0_4f,  ymm8_4f });
        newiopNoret(VOP_TRUNC, {  ymm7_4f,  ymm8_4f });
        newiopNoret(VOP_TRUNC, {  ymm0_4f, ymm15_4f });
        newiopNoret(VOP_TRUNC, {  ymm8_4f,  ymm8_4f });
        newiopNoret(VOP_TRUNC, { ymm15_4f,  ymm8_4f });
        newiopNoret(VOP_TRUNC, {  ymm8_4f, ymm15_4f });

        newiopNoret(VOP_CAST, {  ymm0_8s,  ymm0_8f });
        newiopNoret(VOP_CAST, {  ymm7_8s,  ymm0_8f });
        newiopNoret(VOP_CAST, {  ymm0_8s,  ymm7_8f });
        newiopNoret(VOP_CAST, {  ymm8_8s,  ymm0_8f });
        newiopNoret(VOP_CAST, { ymm15_8s,  ymm0_8f });
        newiopNoret(VOP_CAST, {  ymm8_8s,  ymm7_8f });
        newiopNoret(VOP_CAST, {  ymm0_8s,  ymm8_8f });
        newiopNoret(VOP_CAST, {  ymm7_8s,  ymm8_8f });
        newiopNoret(VOP_CAST, {  ymm0_8s, ymm15_8f });
        newiopNoret(VOP_CAST, {  ymm8_8s,  ymm8_8f });
        newiopNoret(VOP_CAST, { ymm15_8s,  ymm8_8f });
        newiopNoret(VOP_CAST, {  ymm8_8s, ymm15_8f });

        newiopNoret(VOP_CAST, {  ymm0_8s,  ymm0_4f });
        newiopNoret(VOP_CAST, {  ymm7_8s,  ymm0_4f });
        newiopNoret(VOP_CAST, {  ymm0_8s,  ymm7_4f });
        newiopNoret(VOP_CAST, {  ymm8_8s,  ymm0_4f });
        newiopNoret(VOP_CAST, { ymm15_8s,  ymm0_4f });
        newiopNoret(VOP_CAST, {  ymm8_8s,  ymm7_4f });
        newiopNoret(VOP_CAST, {  ymm0_8s,  ymm8_4f });
        newiopNoret(VOP_CAST, {  ymm7_8s,  ymm8_4f });
        newiopNoret(VOP_CAST, {  ymm0_8s, ymm15_4f });
        newiopNoret(VOP_CAST, {  ymm8_8s,  ymm8_4f });
        newiopNoret(VOP_CAST, { ymm15_8s,  ymm8_4f });
        newiopNoret(VOP_CAST, {  ymm8_8s, ymm15_4f });

        newiopNoret(VOP_CAST, {  ymm0_8f,  ymm0_8s });
        newiopNoret(VOP_CAST, {  ymm7_8f,  ymm0_8s });
        newiopNoret(VOP_CAST, {  ymm0_8f,  ymm7_8s });
        newiopNoret(VOP_CAST, {  ymm8_8f,  ymm0_8s });
        newiopNoret(VOP_CAST, { ymm15_8f,  ymm0_8s });
        newiopNoret(VOP_CAST, {  ymm8_8f,  ymm7_8s });
        newiopNoret(VOP_CAST, {  ymm0_8f,  ymm8_8s });
        newiopNoret(VOP_CAST, {  ymm7_8f,  ymm8_8s });
        newiopNoret(VOP_CAST, {  ymm0_8f, ymm15_8s });
        newiopNoret(VOP_CAST, {  ymm8_8f,  ymm8_8s });
        newiopNoret(VOP_CAST, { ymm15_8f,  ymm8_8s });
        newiopNoret(VOP_CAST, {  ymm8_8f, ymm15_8s });

        newiopNoret(VOP_CAST_LOW, {  ymm0_4s,  ymm0_8s });
        newiopNoret(VOP_CAST_LOW, {  ymm7_4s,  ymm0_8s });
        newiopNoret(VOP_CAST_LOW, {  ymm0_4s,  ymm7_8s });
        newiopNoret(VOP_CAST_LOW, {  ymm8_4s,  ymm0_8s });
        newiopNoret(VOP_CAST_LOW, { ymm15_4s,  ymm0_8s });
        newiopNoret(VOP_CAST_LOW, {  ymm8_4s,  ymm7_8s });
        newiopNoret(VOP_CAST_LOW, {  ymm0_4s,  ymm8_8s });
        newiopNoret(VOP_CAST_LOW, {  ymm7_4s,  ymm8_8s });
        newiopNoret(VOP_CAST_LOW, {  ymm0_4s, ymm15_8s });
        newiopNoret(VOP_CAST_LOW, {  ymm8_4s,  ymm8_8s });
        newiopNoret(VOP_CAST_LOW, { ymm15_4s,  ymm8_8s });
        newiopNoret(VOP_CAST_LOW, {  ymm8_4s, ymm15_8s });

        newiopNoret(VOP_AND, {  ymm0_32s,  ymm0_32s,  ymm0_32u});
        newiopNoret(VOP_AND, {  ymm7_32s,  ymm0_32s,  ymm0_32u});
        newiopNoret(VOP_AND, {  ymm0_32s,  ymm7_32s,  ymm0_32u});
        newiopNoret(VOP_AND, {  ymm0_32s,  ymm0_32s,  ymm7_32u});
        newiopNoret(VOP_AND, {  ymm8_32s,  ymm0_32s,  ymm0_32u});
        newiopNoret(VOP_AND, { ymm15_32s,  ymm0_32s,  ymm0_32u});
        newiopNoret(VOP_AND, {  ymm8_32s,  ymm7_32s,  ymm0_32u});
        newiopNoret(VOP_AND, {  ymm8_32s,  ymm0_32s,  ymm7_32u});
        newiopNoret(VOP_AND, {  ymm0_32s,  ymm8_32s,  ymm0_32u});
        newiopNoret(VOP_AND, {  ymm7_32s,  ymm8_32s,  ymm0_32u});
        newiopNoret(VOP_AND, {  ymm0_32s, ymm15_32s,  ymm0_32u});
        newiopNoret(VOP_AND, {  ymm0_32s,  ymm8_32s,  ymm7_32u});
        newiopNoret(VOP_AND, {  ymm0_32s,  ymm0_32s,  ymm8_32u});
        newiopNoret(VOP_AND, {  ymm7_32s,  ymm0_32s,  ymm8_32u});
        newiopNoret(VOP_AND, {  ymm0_32s,  ymm7_32s,  ymm8_32u});
        newiopNoret(VOP_AND, {  ymm0_32s,  ymm0_32s, ymm15_32u});

        newiopNoret(VOP_OR, {  ymm0_16s,  ymm0_16s,  ymm0_16u});
        newiopNoret(VOP_OR, {  ymm7_16s,  ymm0_16s,  ymm0_16u});
        newiopNoret(VOP_OR, {  ymm0_16s,  ymm7_16s,  ymm0_16u});
        newiopNoret(VOP_OR, {  ymm0_16s,  ymm0_16s,  ymm7_16u});
        newiopNoret(VOP_OR, {  ymm8_16s,  ymm0_16s,  ymm0_16u});
        newiopNoret(VOP_OR, { ymm15_16s,  ymm0_16s,  ymm0_16u});
        newiopNoret(VOP_OR, {  ymm8_16s,  ymm7_16s,  ymm0_16u});
        newiopNoret(VOP_OR, {  ymm8_16s,  ymm0_16s,  ymm7_16u});
        newiopNoret(VOP_OR, {  ymm0_16s,  ymm8_16s,  ymm0_16u});
        newiopNoret(VOP_OR, {  ymm7_16s,  ymm8_16s,  ymm0_16u});
        newiopNoret(VOP_OR, {  ymm0_16s, ymm15_16s,  ymm0_16u});
        newiopNoret(VOP_OR, {  ymm0_16s,  ymm8_16s,  ymm7_16u});
        newiopNoret(VOP_OR, {  ymm0_16s,  ymm0_16s,  ymm8_16u});
        newiopNoret(VOP_OR, {  ymm7_16s,  ymm0_16s,  ymm8_16u});
        newiopNoret(VOP_OR, {  ymm0_16s,  ymm7_16s,  ymm8_16u});
        newiopNoret(VOP_OR, {  ymm0_16s,  ymm0_16s, ymm15_16u});

        newiopNoret(VOP_XOR, {  ymm0_8f,  ymm0_8f,  ymm0_8u});
        newiopNoret(VOP_XOR, {  ymm7_8f,  ymm0_8f,  ymm0_8u});
        newiopNoret(VOP_XOR, {  ymm0_8f,  ymm7_8f,  ymm0_8u});
        newiopNoret(VOP_XOR, {  ymm0_8f,  ymm0_8f,  ymm7_8u});
        newiopNoret(VOP_XOR, {  ymm8_8f,  ymm0_8f,  ymm0_8u});
        newiopNoret(VOP_XOR, { ymm15_8f,  ymm0_8f,  ymm0_8u});
        newiopNoret(VOP_XOR, {  ymm8_8f,  ymm7_8f,  ymm0_8u});
        newiopNoret(VOP_XOR, {  ymm8_8f,  ymm0_8f,  ymm7_8u});
        newiopNoret(VOP_XOR, {  ymm0_8f,  ymm8_8f,  ymm0_8u});
        newiopNoret(VOP_XOR, {  ymm7_8f,  ymm8_8f,  ymm0_8u});
        newiopNoret(VOP_XOR, {  ymm0_8f, ymm15_8f,  ymm0_8u});
        newiopNoret(VOP_XOR, {  ymm0_8f,  ymm8_8f,  ymm7_8u});
        newiopNoret(VOP_XOR, {  ymm0_8f,  ymm0_8f,  ymm8_8u});
        newiopNoret(VOP_XOR, {  ymm7_8f,  ymm0_8f,  ymm8_8u});
        newiopNoret(VOP_XOR, {  ymm0_8f,  ymm7_8f,  ymm8_8u});
        newiopNoret(VOP_XOR, {  ymm0_8f,  ymm0_8f, ymm15_8u});
                
        newiopNoret(VOP_SHL, {  ymm0_16u,  ymm0_16u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm7_16u,  ymm0_16u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm0_16u,  ymm7_16u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm8_16u,  ymm0_16u, argIImm(15) });
        newiopNoret(VOP_SHL, { ymm15_16u,  ymm0_16u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm8_16u,  ymm7_16u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm0_16u,  ymm8_16u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm7_16u,  ymm8_16u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm0_16u, ymm15_16u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm8_16u,  ymm8_16u, argIImm(15) });
        newiopNoret(VOP_SHL, { ymm15_16u,  ymm8_16u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm8_16u, ymm15_16u, argIImm(15) });

        newiopNoret(VOP_SHL, {  ymm0_8u,  ymm0_8u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm7_8u,  ymm0_8u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm0_8u,  ymm7_8u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm8_8u,  ymm0_8u, argIImm(15) });
        newiopNoret(VOP_SHL, { ymm15_8u,  ymm0_8u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm8_8u,  ymm7_8u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm0_8u,  ymm8_8u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm7_8u,  ymm8_8u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm0_8u, ymm15_8u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm8_8u,  ymm8_8u, argIImm(15) });
        newiopNoret(VOP_SHL, { ymm15_8u,  ymm8_8u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm8_8u, ymm15_8u, argIImm(15) });

        newiopNoret(VOP_SHL, {  ymm0_4u,  ymm0_4u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm7_4u,  ymm0_4u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm0_4u,  ymm7_4u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm8_4u,  ymm0_4u, argIImm(15) });
        newiopNoret(VOP_SHL, { ymm15_4u,  ymm0_4u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm8_4u,  ymm7_4u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm0_4u,  ymm8_4u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm7_4u,  ymm8_4u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm0_4u, ymm15_4u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm8_4u,  ymm8_4u, argIImm(15) });
        newiopNoret(VOP_SHL, { ymm15_4u,  ymm8_4u, argIImm(15) });
        newiopNoret(VOP_SHL, {  ymm8_4u, ymm15_4u, argIImm(15) });

        newiopNoret(VOP_SHL, {  ymm0_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_SHL, {  ymm7_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_SHL, {  ymm0_8u,  ymm7_8u,  ymm0_8u});
        newiopNoret(VOP_SHL, {  ymm0_8u,  ymm0_8u,  ymm7_8u});
        newiopNoret(VOP_SHL, {  ymm8_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_SHL, { ymm15_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_SHL, {  ymm8_8u,  ymm7_8u,  ymm0_8u});
        newiopNoret(VOP_SHL, {  ymm8_8u,  ymm0_8u,  ymm7_8u});
        newiopNoret(VOP_SHL, {  ymm0_8u,  ymm8_8u,  ymm0_8u});
        newiopNoret(VOP_SHL, {  ymm7_8u,  ymm8_8u,  ymm0_8u});
        newiopNoret(VOP_SHL, {  ymm0_8u, ymm15_8u,  ymm0_8u});
        newiopNoret(VOP_SHL, {  ymm0_8u,  ymm8_8u,  ymm7_8u});
        newiopNoret(VOP_SHL, {  ymm0_8u,  ymm0_8u,  ymm8_8u});
        newiopNoret(VOP_SHL, {  ymm7_8u,  ymm0_8u,  ymm8_8u});
        newiopNoret(VOP_SHL, {  ymm0_8u,  ymm7_8u,  ymm8_8u});
        newiopNoret(VOP_SHL, {  ymm0_8u,  ymm0_8u, ymm15_8u});

        newiopNoret(VOP_SHL, {  ymm0_4u,  ymm0_4u,  ymm0_4u});
        newiopNoret(VOP_SHL, {  ymm7_4u,  ymm0_4u,  ymm0_4u});
        newiopNoret(VOP_SHL, {  ymm0_4u,  ymm7_4u,  ymm0_4u});
        newiopNoret(VOP_SHL, {  ymm0_4u,  ymm0_4u,  ymm7_4u});
        newiopNoret(VOP_SHL, {  ymm8_4u,  ymm0_4u,  ymm0_4u});
        newiopNoret(VOP_SHL, { ymm15_4u,  ymm0_4u,  ymm0_4u});
        newiopNoret(VOP_SHL, {  ymm8_4u,  ymm7_4u,  ymm0_4u});
        newiopNoret(VOP_SHL, {  ymm8_4u,  ymm0_4u,  ymm7_4u});
        newiopNoret(VOP_SHL, {  ymm0_4u,  ymm8_4u,  ymm0_4u});
        newiopNoret(VOP_SHL, {  ymm7_4u,  ymm8_4u,  ymm0_4u});
        newiopNoret(VOP_SHL, {  ymm0_4u, ymm15_4u,  ymm0_4u});
        newiopNoret(VOP_SHL, {  ymm0_4u,  ymm8_4u,  ymm7_4u});
        newiopNoret(VOP_SHL, {  ymm0_4u,  ymm0_4u,  ymm8_4u});
        newiopNoret(VOP_SHL, {  ymm7_4u,  ymm0_4u,  ymm8_4u});
        newiopNoret(VOP_SHL, {  ymm0_4u,  ymm7_4u,  ymm8_4u});
        newiopNoret(VOP_SHL, {  ymm0_4u,  ymm0_4u, ymm15_4u});

        newiopNoret(VOP_SAR, {  ymm0_16s,  ymm0_16s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm7_16s,  ymm0_16s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm0_16s,  ymm7_16s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm8_16s,  ymm0_16s, argIImm(15) });
        newiopNoret(VOP_SAR, { ymm15_16s,  ymm0_16s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm8_16s,  ymm7_16s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm0_16s,  ymm8_16s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm7_16s,  ymm8_16s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm0_16s, ymm15_16s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm8_16s,  ymm8_16s, argIImm(15) });
        newiopNoret(VOP_SAR, { ymm15_16s,  ymm8_16s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm8_16s, ymm15_16s, argIImm(15) });

        newiopNoret(VOP_SAR, {  ymm0_8s,  ymm0_8s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm7_8s,  ymm0_8s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm0_8s,  ymm7_8s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm8_8s,  ymm0_8s, argIImm(15) });
        newiopNoret(VOP_SAR, { ymm15_8s,  ymm0_8s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm8_8s,  ymm7_8s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm0_8s,  ymm8_8s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm7_8s,  ymm8_8s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm0_8s, ymm15_8s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm8_8s,  ymm8_8s, argIImm(15) });
        newiopNoret(VOP_SAR, { ymm15_8s,  ymm8_8s, argIImm(15) });
        newiopNoret(VOP_SAR, {  ymm8_8s, ymm15_8s, argIImm(15) });

        newiopNoret(VOP_SAR, {  ymm0_8s,  ymm0_8s,  ymm0_8u});
        newiopNoret(VOP_SAR, {  ymm7_8s,  ymm0_8s,  ymm0_8u});
        newiopNoret(VOP_SAR, {  ymm0_8s,  ymm7_8s,  ymm0_8u});
        newiopNoret(VOP_SAR, {  ymm0_8s,  ymm0_8s,  ymm7_8u});
        newiopNoret(VOP_SAR, {  ymm8_8s,  ymm0_8s,  ymm0_8u});
        newiopNoret(VOP_SAR, { ymm15_8s,  ymm0_8s,  ymm0_8u});
        newiopNoret(VOP_SAR, {  ymm8_8s,  ymm7_8s,  ymm0_8u});
        newiopNoret(VOP_SAR, {  ymm8_8s,  ymm0_8s,  ymm7_8u});
        newiopNoret(VOP_SAR, {  ymm0_8s,  ymm8_8s,  ymm0_8u});
        newiopNoret(VOP_SAR, {  ymm7_8s,  ymm8_8s,  ymm0_8u});
        newiopNoret(VOP_SAR, {  ymm0_8s, ymm15_8s,  ymm0_8u});
        newiopNoret(VOP_SAR, {  ymm0_8s,  ymm8_8s,  ymm7_8u});
        newiopNoret(VOP_SAR, {  ymm0_8s,  ymm0_8s,  ymm8_8u});
        newiopNoret(VOP_SAR, {  ymm7_8s,  ymm0_8s,  ymm8_8u});
        newiopNoret(VOP_SAR, {  ymm0_8s,  ymm7_8s,  ymm8_8u});
        newiopNoret(VOP_SAR, {  ymm0_8s,  ymm0_8s, ymm15_8u});

        newiopNoret(VOP_SHR, {  ymm0_16u,  ymm0_16u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm7_16u,  ymm0_16u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm0_16u,  ymm7_16u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm8_16u,  ymm0_16u, argIImm(15) });
        newiopNoret(VOP_SHR, { ymm15_16u,  ymm0_16u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm8_16u,  ymm7_16u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm0_16u,  ymm8_16u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm7_16u,  ymm8_16u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm0_16u, ymm15_16u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm8_16u,  ymm8_16u, argIImm(15) });
        newiopNoret(VOP_SHR, { ymm15_16u,  ymm8_16u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm8_16u, ymm15_16u, argIImm(15) });

        newiopNoret(VOP_SHR, {  ymm0_8u,  ymm0_8u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm7_8u,  ymm0_8u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm0_8u,  ymm7_8u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm8_8u,  ymm0_8u, argIImm(15) });
        newiopNoret(VOP_SHR, { ymm15_8u,  ymm0_8u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm8_8u,  ymm7_8u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm0_8u,  ymm8_8u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm7_8u,  ymm8_8u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm0_8u, ymm15_8u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm8_8u,  ymm8_8u, argIImm(15) });
        newiopNoret(VOP_SHR, { ymm15_8u,  ymm8_8u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm8_8u, ymm15_8u, argIImm(15) });

        newiopNoret(VOP_SHR, {  ymm0_4u,  ymm0_4u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm7_4u,  ymm0_4u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm0_4u,  ymm7_4u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm8_4u,  ymm0_4u, argIImm(15) });
        newiopNoret(VOP_SHR, { ymm15_4u,  ymm0_4u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm8_4u,  ymm7_4u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm0_4u,  ymm8_4u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm7_4u,  ymm8_4u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm0_4u, ymm15_4u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm8_4u,  ymm8_4u, argIImm(15) });
        newiopNoret(VOP_SHR, { ymm15_4u,  ymm8_4u, argIImm(15) });
        newiopNoret(VOP_SHR, {  ymm8_4u, ymm15_4u, argIImm(15) });

        newiopNoret(VOP_SHR, {  ymm0_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_SHR, {  ymm7_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_SHR, {  ymm0_8u,  ymm7_8u,  ymm0_8u});
        newiopNoret(VOP_SHR, {  ymm0_8u,  ymm0_8u,  ymm7_8u});
        newiopNoret(VOP_SHR, {  ymm8_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_SHR, { ymm15_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_SHR, {  ymm8_8u,  ymm7_8u,  ymm0_8u});
        newiopNoret(VOP_SHR, {  ymm8_8u,  ymm0_8u,  ymm7_8u});
        newiopNoret(VOP_SHR, {  ymm0_8u,  ymm8_8u,  ymm0_8u});
        newiopNoret(VOP_SHR, {  ymm7_8u,  ymm8_8u,  ymm0_8u});
        newiopNoret(VOP_SHR, {  ymm0_8u, ymm15_8u,  ymm0_8u});
        newiopNoret(VOP_SHR, {  ymm0_8u,  ymm8_8u,  ymm7_8u});
        newiopNoret(VOP_SHR, {  ymm0_8u,  ymm0_8u,  ymm8_8u});
        newiopNoret(VOP_SHR, {  ymm7_8u,  ymm0_8u,  ymm8_8u});
        newiopNoret(VOP_SHR, {  ymm0_8u,  ymm7_8u,  ymm8_8u});
        newiopNoret(VOP_SHR, {  ymm0_8u,  ymm0_8u, ymm15_8u});

        newiopNoret(VOP_SHR, {  ymm0_4u,  ymm0_4u,  ymm0_4u});
        newiopNoret(VOP_SHR, {  ymm7_4u,  ymm0_4u,  ymm0_4u});
        newiopNoret(VOP_SHR, {  ymm0_4u,  ymm7_4u,  ymm0_4u});
        newiopNoret(VOP_SHR, {  ymm0_4u,  ymm0_4u,  ymm7_4u});
        newiopNoret(VOP_SHR, {  ymm8_4u,  ymm0_4u,  ymm0_4u});
        newiopNoret(VOP_SHR, { ymm15_4u,  ymm0_4u,  ymm0_4u});
        newiopNoret(VOP_SHR, {  ymm8_4u,  ymm7_4u,  ymm0_4u});
        newiopNoret(VOP_SHR, {  ymm8_4u,  ymm0_4u,  ymm7_4u});
        newiopNoret(VOP_SHR, {  ymm0_4u,  ymm8_4u,  ymm0_4u});
        newiopNoret(VOP_SHR, {  ymm7_4u,  ymm8_4u,  ymm0_4u});
        newiopNoret(VOP_SHR, {  ymm0_4u, ymm15_4u,  ymm0_4u});
        newiopNoret(VOP_SHR, {  ymm0_4u,  ymm8_4u,  ymm7_4u});
        newiopNoret(VOP_SHR, {  ymm0_4u,  ymm0_4u,  ymm8_4u});
        newiopNoret(VOP_SHR, {  ymm7_4u,  ymm0_4u,  ymm8_4u});
        newiopNoret(VOP_SHR, {  ymm0_4u,  ymm7_4u,  ymm8_4u});
        newiopNoret(VOP_SHR, {  ymm0_4u,  ymm0_4u, ymm15_4u});

        newiopNoret(VOP_EQ, {  ymm0_32u,  ymm0_32u,  ymm0_32u});
        newiopNoret(VOP_EQ, {  ymm7_32u,  ymm0_32u,  ymm0_32u});
        newiopNoret(VOP_EQ, {  ymm0_32u,  ymm7_32u,  ymm0_32u});
        newiopNoret(VOP_EQ, {  ymm0_32u,  ymm0_32u,  ymm7_32u});
        newiopNoret(VOP_EQ, {  ymm8_32u,  ymm0_32u,  ymm0_32u});
        newiopNoret(VOP_EQ, { ymm15_32u,  ymm0_32u,  ymm0_32u});
        newiopNoret(VOP_EQ, {  ymm8_32u,  ymm7_32u,  ymm0_32u});
        newiopNoret(VOP_EQ, {  ymm8_32u,  ymm0_32u,  ymm7_32u});
        newiopNoret(VOP_EQ, {  ymm0_32u,  ymm8_32u,  ymm0_32u});
        newiopNoret(VOP_EQ, {  ymm7_32u,  ymm8_32u,  ymm0_32u});
        newiopNoret(VOP_EQ, {  ymm0_32u, ymm15_32u,  ymm0_32u});
        newiopNoret(VOP_EQ, {  ymm0_32u,  ymm8_32u,  ymm7_32u});
        newiopNoret(VOP_EQ, {  ymm0_32u,  ymm0_32u,  ymm8_32u});
        newiopNoret(VOP_EQ, {  ymm7_32u,  ymm0_32u,  ymm8_32u});
        newiopNoret(VOP_EQ, {  ymm0_32u,  ymm7_32u,  ymm8_32u});
        newiopNoret(VOP_EQ, {  ymm0_32u,  ymm0_32u, ymm15_32u});

        newiopNoret(VOP_EQ, {  ymm0_16u,  ymm0_16u,  ymm0_16u});
        newiopNoret(VOP_EQ, {  ymm7_16u,  ymm0_16u,  ymm0_16u});
        newiopNoret(VOP_EQ, {  ymm0_16u,  ymm7_16u,  ymm0_16u});
        newiopNoret(VOP_EQ, {  ymm0_16u,  ymm0_16u,  ymm7_16u});
        newiopNoret(VOP_EQ, {  ymm8_16u,  ymm0_16u,  ymm0_16u});
        newiopNoret(VOP_EQ, { ymm15_16u,  ymm0_16u,  ymm0_16u});
        newiopNoret(VOP_EQ, {  ymm8_16u,  ymm7_16u,  ymm0_16u});
        newiopNoret(VOP_EQ, {  ymm8_16u,  ymm0_16u,  ymm7_16u});
        newiopNoret(VOP_EQ, {  ymm0_16u,  ymm8_16u,  ymm0_16u});
        newiopNoret(VOP_EQ, {  ymm7_16u,  ymm8_16u,  ymm0_16u});
        newiopNoret(VOP_EQ, {  ymm0_16u, ymm15_16u,  ymm0_16u});
        newiopNoret(VOP_EQ, {  ymm0_16u,  ymm8_16u,  ymm7_16u});
        newiopNoret(VOP_EQ, {  ymm0_16u,  ymm0_16u,  ymm8_16u});
        newiopNoret(VOP_EQ, {  ymm7_16u,  ymm0_16u,  ymm8_16u});
        newiopNoret(VOP_EQ, {  ymm0_16u,  ymm7_16u,  ymm8_16u});
        newiopNoret(VOP_EQ, {  ymm0_16u,  ymm0_16u, ymm15_16u});

        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_EQ, {  ymm7_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm7_8u,  ymm0_8u});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm0_8u,  ymm7_8u});
        newiopNoret(VOP_EQ, {  ymm8_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_EQ, { ymm15_8u,  ymm0_8u,  ymm0_8u});
        newiopNoret(VOP_EQ, {  ymm8_8u,  ymm7_8u,  ymm0_8u});
        newiopNoret(VOP_EQ, {  ymm8_8u,  ymm0_8u,  ymm7_8u});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm8_8u,  ymm0_8u});
        newiopNoret(VOP_EQ, {  ymm7_8u,  ymm8_8u,  ymm0_8u});
        newiopNoret(VOP_EQ, {  ymm0_8u, ymm15_8u,  ymm0_8u});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm8_8u,  ymm7_8u});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm0_8u,  ymm8_8u});
        newiopNoret(VOP_EQ, {  ymm7_8u,  ymm0_8u,  ymm8_8u});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm7_8u,  ymm8_8u});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm0_8u, ymm15_8u});

        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm0_4u,  ymm0_4u});
        newiopNoret(VOP_EQ, {  ymm7_4u,  ymm0_4u,  ymm0_4u});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm7_4u,  ymm0_4u});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm0_4u,  ymm7_4u});
        newiopNoret(VOP_EQ, {  ymm8_4u,  ymm0_4u,  ymm0_4u});
        newiopNoret(VOP_EQ, { ymm15_4u,  ymm0_4u,  ymm0_4u});
        newiopNoret(VOP_EQ, {  ymm8_4u,  ymm7_4u,  ymm0_4u});
        newiopNoret(VOP_EQ, {  ymm8_4u,  ymm0_4u,  ymm7_4u});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm8_4u,  ymm0_4u});
        newiopNoret(VOP_EQ, {  ymm7_4u,  ymm8_4u,  ymm0_4u});
        newiopNoret(VOP_EQ, {  ymm0_4u, ymm15_4u,  ymm0_4u});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm8_4u,  ymm7_4u});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm0_4u,  ymm8_4u});
        newiopNoret(VOP_EQ, {  ymm7_4u,  ymm0_4u,  ymm8_4u});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm7_4u,  ymm8_4u});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm0_4u, ymm15_4u});

        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_EQ, {  ymm7_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_EQ, {  ymm8_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_EQ, { ymm15_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_EQ, {  ymm8_8u,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_EQ, {  ymm8_8u,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_EQ, {  ymm7_8u,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_EQ, {  ymm0_8u, ymm15_8f,  ymm0_8f});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm8_8f,  ymm7_8f});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_EQ, {  ymm7_8u,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm7_8f,  ymm8_8f});
        newiopNoret(VOP_EQ, {  ymm0_8u,  ymm0_8f, ymm15_8f});

        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_EQ, {  ymm7_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_EQ, {  ymm8_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_EQ, { ymm15_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_EQ, {  ymm8_4u,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_EQ, {  ymm8_4u,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_EQ, {  ymm7_4u,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_EQ, {  ymm0_4u, ymm15_4f,  ymm0_4f});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm8_4f,  ymm7_4f});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_EQ, {  ymm7_4u,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm7_4f,  ymm8_4f});
        newiopNoret(VOP_EQ, {  ymm0_4u,  ymm0_4f, ymm15_4f});

        newiopNoret(VOP_NE, {  ymm0_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_NE, {  ymm7_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_NE, {  ymm0_8u,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_NE, {  ymm0_8u,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_NE, {  ymm8_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_NE, { ymm15_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_NE, {  ymm8_8u,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_NE, {  ymm8_8u,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_NE, {  ymm0_8u,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_NE, {  ymm7_8u,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_NE, {  ymm0_8u, ymm15_8f,  ymm0_8f});
        newiopNoret(VOP_NE, {  ymm0_8u,  ymm8_8f,  ymm7_8f});
        newiopNoret(VOP_NE, {  ymm0_8u,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_NE, {  ymm7_8u,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_NE, {  ymm0_8u,  ymm7_8f,  ymm8_8f});
        newiopNoret(VOP_NE, {  ymm0_8u,  ymm0_8f, ymm15_8f});

        newiopNoret(VOP_NE, {  ymm0_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_NE, {  ymm7_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_NE, {  ymm0_4u,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_NE, {  ymm0_4u,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_NE, {  ymm8_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_NE, { ymm15_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_NE, {  ymm8_4u,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_NE, {  ymm8_4u,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_NE, {  ymm0_4u,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_NE, {  ymm7_4u,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_NE, {  ymm0_4u, ymm15_4f,  ymm0_4f});
        newiopNoret(VOP_NE, {  ymm0_4u,  ymm8_4f,  ymm7_4f});
        newiopNoret(VOP_NE, {  ymm0_4u,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_NE, {  ymm7_4u,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_NE, {  ymm0_4u,  ymm7_4f,  ymm8_4f});
        newiopNoret(VOP_NE, {  ymm0_4u,  ymm0_4f, ymm15_4f});

        newiopNoret(VOP_LT, {  ymm0_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_LT, {  ymm7_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_LT, {  ymm0_8u,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_LT, {  ymm0_8u,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_LT, {  ymm8_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_LT, { ymm15_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_LT, {  ymm8_8u,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_LT, {  ymm8_8u,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_LT, {  ymm0_8u,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_LT, {  ymm7_8u,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_LT, {  ymm0_8u, ymm15_8f,  ymm0_8f});
        newiopNoret(VOP_LT, {  ymm0_8u,  ymm8_8f,  ymm7_8f});
        newiopNoret(VOP_LT, {  ymm0_8u,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_LT, {  ymm7_8u,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_LT, {  ymm0_8u,  ymm7_8f,  ymm8_8f});
        newiopNoret(VOP_LT, {  ymm0_8u,  ymm0_8f, ymm15_8f});

        newiopNoret(VOP_LT, {  ymm0_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_LT, {  ymm7_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_LT, {  ymm0_4u,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_LT, {  ymm0_4u,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_LT, {  ymm8_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_LT, { ymm15_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_LT, {  ymm8_4u,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_LT, {  ymm8_4u,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_LT, {  ymm0_4u,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_LT, {  ymm7_4u,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_LT, {  ymm0_4u, ymm15_4f,  ymm0_4f});
        newiopNoret(VOP_LT, {  ymm0_4u,  ymm8_4f,  ymm7_4f});
        newiopNoret(VOP_LT, {  ymm0_4u,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_LT, {  ymm7_4u,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_LT, {  ymm0_4u,  ymm7_4f,  ymm8_4f});
        newiopNoret(VOP_LT, {  ymm0_4u,  ymm0_4f, ymm15_4f});
        
        newiopNoret(VOP_LE, {  ymm0_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_LE, {  ymm7_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_LE, {  ymm0_8u,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_LE, {  ymm0_8u,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_LE, {  ymm8_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_LE, { ymm15_8u,  ymm0_8f,  ymm0_8f});
        newiopNoret(VOP_LE, {  ymm8_8u,  ymm7_8f,  ymm0_8f});
        newiopNoret(VOP_LE, {  ymm8_8u,  ymm0_8f,  ymm7_8f});
        newiopNoret(VOP_LE, {  ymm0_8u,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_LE, {  ymm7_8u,  ymm8_8f,  ymm0_8f});
        newiopNoret(VOP_LE, {  ymm0_8u, ymm15_8f,  ymm0_8f});
        newiopNoret(VOP_LE, {  ymm0_8u,  ymm8_8f,  ymm7_8f});
        newiopNoret(VOP_LE, {  ymm0_8u,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_LE, {  ymm7_8u,  ymm0_8f,  ymm8_8f});
        newiopNoret(VOP_LE, {  ymm0_8u,  ymm7_8f,  ymm8_8f});
        newiopNoret(VOP_LE, {  ymm0_8u,  ymm0_8f, ymm15_8f});

        newiopNoret(VOP_LE, {  ymm0_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_LE, {  ymm7_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_LE, {  ymm0_4u,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_LE, {  ymm0_4u,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_LE, {  ymm8_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_LE, { ymm15_4u,  ymm0_4f,  ymm0_4f});
        newiopNoret(VOP_LE, {  ymm8_4u,  ymm7_4f,  ymm0_4f});
        newiopNoret(VOP_LE, {  ymm8_4u,  ymm0_4f,  ymm7_4f});
        newiopNoret(VOP_LE, {  ymm0_4u,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_LE, {  ymm7_4u,  ymm8_4f,  ymm0_4f});
        newiopNoret(VOP_LE, {  ymm0_4u, ymm15_4f,  ymm0_4f});
        newiopNoret(VOP_LE, {  ymm0_4u,  ymm8_4f,  ymm7_4f});
        newiopNoret(VOP_LE, {  ymm0_4u,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_LE, {  ymm7_4u,  ymm0_4f,  ymm8_4f});
        newiopNoret(VOP_LE, {  ymm0_4u,  ymm7_4f,  ymm8_4f});
        newiopNoret(VOP_LE, {  ymm0_4u,  ymm0_4f, ymm15_4f});

        newiopNoret(VOP_GT, {  ymm0_32u,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_GT, {  ymm7_32u,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_GT, {  ymm0_32u,  ymm7_32s,  ymm0_32s});
        newiopNoret(VOP_GT, {  ymm0_32u,  ymm0_32s,  ymm7_32s});
        newiopNoret(VOP_GT, {  ymm8_32u,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_GT, { ymm15_32u,  ymm0_32s,  ymm0_32s});
        newiopNoret(VOP_GT, {  ymm8_32u,  ymm7_32s,  ymm0_32s});
        newiopNoret(VOP_GT, {  ymm8_32u,  ymm0_32s,  ymm7_32s});
        newiopNoret(VOP_GT, {  ymm0_32u,  ymm8_32s,  ymm0_32s});
        newiopNoret(VOP_GT, {  ymm7_32u,  ymm8_32s,  ymm0_32s});
        newiopNoret(VOP_GT, {  ymm0_32u, ymm15_32s,  ymm0_32s});
        newiopNoret(VOP_GT, {  ymm0_32u,  ymm8_32s,  ymm7_32s});
        newiopNoret(VOP_GT, {  ymm0_32u,  ymm0_32s,  ymm8_32s});
        newiopNoret(VOP_GT, {  ymm7_32u,  ymm0_32s,  ymm8_32s});
        newiopNoret(VOP_GT, {  ymm0_32u,  ymm7_32s,  ymm8_32s});
        newiopNoret(VOP_GT, {  ymm0_32u,  ymm0_32s, ymm15_32s});

        newiopNoret(VOP_GT, {  ymm0_16u,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_GT, {  ymm7_16u,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_GT, {  ymm0_16u,  ymm7_16s,  ymm0_16s});
        newiopNoret(VOP_GT, {  ymm0_16u,  ymm0_16s,  ymm7_16s});
        newiopNoret(VOP_GT, {  ymm8_16u,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_GT, { ymm15_16u,  ymm0_16s,  ymm0_16s});
        newiopNoret(VOP_GT, {  ymm8_16u,  ymm7_16s,  ymm0_16s});
        newiopNoret(VOP_GT, {  ymm8_16u,  ymm0_16s,  ymm7_16s});
        newiopNoret(VOP_GT, {  ymm0_16u,  ymm8_16s,  ymm0_16s});
        newiopNoret(VOP_GT, {  ymm7_16u,  ymm8_16s,  ymm0_16s});
        newiopNoret(VOP_GT, {  ymm0_16u, ymm15_16s,  ymm0_16s});
        newiopNoret(VOP_GT, {  ymm0_16u,  ymm8_16s,  ymm7_16s});
        newiopNoret(VOP_GT, {  ymm0_16u,  ymm0_16s,  ymm8_16s});
        newiopNoret(VOP_GT, {  ymm7_16u,  ymm0_16s,  ymm8_16s});
        newiopNoret(VOP_GT, {  ymm0_16u,  ymm7_16s,  ymm8_16s});
        newiopNoret(VOP_GT, {  ymm0_16u,  ymm0_16s, ymm15_16s});

        newiopNoret(VOP_GT, {  ymm0_8u,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_GT, {  ymm7_8u,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_GT, {  ymm0_8u,  ymm7_8s,  ymm0_8s});
        newiopNoret(VOP_GT, {  ymm0_8u,  ymm0_8s,  ymm7_8s});
        newiopNoret(VOP_GT, {  ymm8_8u,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_GT, { ymm15_8u,  ymm0_8s,  ymm0_8s});
        newiopNoret(VOP_GT, {  ymm8_8u,  ymm7_8s,  ymm0_8s});
        newiopNoret(VOP_GT, {  ymm8_8u,  ymm0_8s,  ymm7_8s});
        newiopNoret(VOP_GT, {  ymm0_8u,  ymm8_8s,  ymm0_8s});
        newiopNoret(VOP_GT, {  ymm7_8u,  ymm8_8s,  ymm0_8s});
        newiopNoret(VOP_GT, {  ymm0_8u, ymm15_8s,  ymm0_8s});
        newiopNoret(VOP_GT, {  ymm0_8u,  ymm8_8s,  ymm7_8s});
        newiopNoret(VOP_GT, {  ymm0_8u,  ymm0_8s,  ymm8_8s});
        newiopNoret(VOP_GT, {  ymm7_8u,  ymm0_8s,  ymm8_8s});
        newiopNoret(VOP_GT, {  ymm0_8u,  ymm7_8s,  ymm8_8s});
        newiopNoret(VOP_GT, {  ymm0_8u,  ymm0_8s, ymm15_8s});

        newiopNoret(VOP_GT, {  ymm0_4u,  ymm0_4s,  ymm0_4s});
        newiopNoret(VOP_GT, {  ymm7_4u,  ymm0_4s,  ymm0_4s});
        newiopNoret(VOP_GT, {  ymm0_4u,  ymm7_4s,  ymm0_4s});
        newiopNoret(VOP_GT, {  ymm0_4u,  ymm0_4s,  ymm7_4s});
        newiopNoret(VOP_GT, {  ymm8_4u,  ymm0_4s,  ymm0_4s});
        newiopNoret(VOP_GT, { ymm15_4u,  ymm0_4s,  ymm0_4s});
        newiopNoret(VOP_GT, {  ymm8_4u,  ymm7_4s,  ymm0_4s});
        newiopNoret(VOP_GT, {  ymm8_4u,  ymm0_4s,  ymm7_4s});
        newiopNoret(VOP_GT, {  ymm0_4u,  ymm8_4s,  ymm0_4s});
        newiopNoret(VOP_GT, {  ymm7_4u,  ymm8_4s,  ymm0_4s});
        newiopNoret(VOP_GT, {  ymm0_4u, ymm15_4s,  ymm0_4s});
        newiopNoret(VOP_GT, {  ymm0_4u,  ymm8_4s,  ymm7_4s});
        newiopNoret(VOP_GT, {  ymm0_4u,  ymm0_4s,  ymm8_4s});
        newiopNoret(VOP_GT, {  ymm7_4u,  ymm0_4s,  ymm8_4s});
        newiopNoret(VOP_GT, {  ymm0_4u,  ymm7_4s,  ymm8_4s});
        newiopNoret(VOP_GT, {  ymm0_4u,  ymm0_4s, ymm15_4s});

        // for(int half0 = 0; half0 < 2; half0++)   //DUBUG: good checking code piece.
        // for(int half1 = 0; half1 < 2; half1++) 
        // for(int half2 = 0; half2 < 2; half2++)
        // {
        //     int numfix[4] = {0,4,5,7};
        //     for(int nf0 = 0; nf0 < 4; nf0++) 
        //     for(int nf1 = 0; nf1 < 4; nf1++) 
        //     for(int nf2 = 0; nf2 < 4; nf2++)
        //     {
        //         int num0 = half0 * 8 + numfix[nf0];
        //         int num1 = half1 * 8 + numfix[nf1];
        //         int num2 = half2 * 8 + numfix[nf2];
        //         Expr ymm0 = VExpr<uint8_t>(vregHid<uint8_t>(num0,_f)).notype();
        //         Expr ymm1 = VExpr<uint8_t>(vregHid<uint8_t>(num1,_f)).notype();
        //         Expr ymm2 = VExpr<uint8_t>(vregHid<uint8_t>(num2,_f)).notype();

        //         newiopNoret(VOP_MIN, {  ymm0,  ymm1,  ymm2});
        //     }
        // }
    }

    loops::Func func = ctx.getFunc(test_info_->name());
    EXPECT_ASSEMBLY_CORRECT(func);
}
#endif//__LOOPS_ARCH == __LOOPS_INTEL64