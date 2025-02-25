/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "loops/loops.hpp"
#include "collections.hpp"
#include "printer.hpp"
#include "backend_aarch64.hpp"
#include "backend_intel64.hpp"
#include "backend_riscV.hpp"
#include "backend.hpp"
#include "reg_allocator.hpp"
#include "common.hpp"
#include "func_impl.hpp"
#include <math.h>
#include <map>
#include <stack>
#include <cstring>


LOOPS_HASHMAP_STATIC(int, loops_cstring) errstrings_[] = 
{
                  /*  |                   enum_id                   |                                    string_id                                   |   */
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_SUCCESS                            , "Loops: Success."                                                              ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_NULL_POINTER                       , "Loops: Null pointer."                                                         ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_POINTER_ARITHMETIC_ERROR           , "Loops: Pointer arithmetic error."                                             ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_OUT_OF_MEMORY                      , "Loops: Out of memory."                                                        ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_UNKNOWN_FLAG                       , "Loops: Unknown flag."                                                         ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_POSITIVE_SIZE_NEEDED               , "Loops: Negative size."                                                        ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_UNIMAGINARY_BIG_STRING             , "Loops: Unpredicted very big string."                                          ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_UNPRINTABLE_OPERATION              , "Loops: Unprintable operation."                                                ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_UNKNOWN_TYPE                       , "Loops: Unknown type."                                                         ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_UNKNOWN_CONDITION                  , "Loops: Unknown condition type."                                               ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_INCORRECT_OPERATION_FORMAT         , "Loops: Incorrect operation format."                                           ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_INCORRECT_ARGUMENT                 , "Loops: Incorrect argument."                                                   ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_UNKNOWN_ARGUMENT_TYPE              , "Loops: Unknown argument type."                                                ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_INTERNAL_UNKNOWN_PRINT_DESTINATION , "Loops: Internal error: unknown type of output stream."                        ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_INTERNAL_BUFFER_SIZE_MISCALCULATION, "Loops: Internal error: printer output buffer size was calculated incorrectly."),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_INTERNAL_INCORRECT_OFFSET          , "Loops: Internal error: incorrect operation offset."                           ),
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_ELEMENT_NOT_FOUND                  , "Loops: Element not found."                                                    ), 
    LOOPS_HASHMAP_ELEM(LOOPS_ERR_INCORRECT_LANE_INDEX               , "Loops: Negative or to big lane index."                                        ), 
};
LOOPS_HASHMAP(int, loops_cstring) errstrings = NULL;

static void loops_initialize();
#if defined(_MSC_VER) && defined(_WIN64)
    #pragma section(".CRT$XCT",read)
    __declspec(allocate(".CRT$XCT")) void (*loops_initialize_)(void) = loops_initialize;
#elif defined(_MSC_VER) && !defined(_WIN64)
    #error Win32 is not supported.
#else
    static void loops_initialize_(void) __attribute__((constructor));
    static void loops_initialize_(void) { loops_initialize(); }
#endif
static void finalize(void)
{
#if __LOOPS_ARCH == __LOOPS_AARCH64
    backend_aarch64_h_deinitialize();
#elif __LOOPS_ARCH == __LOOPS_INTEL64
    backend_intel64_h_deinitialize();
#elif __LOOPS_ARCH == __LOOPS_RISCV
    backend_riscv_h_deinitialize();
#else
#error Unknown CPU
#endif
    printer_h_deinitialize();
    loops_hashmap_destruct(errstrings);
}

static void loops_initialize()
{
    int err;
    const char* init_error_msg = "Loops: Initialization error. Code: %d\n";
    err = loops_hashmap_construct_static(&errstrings, errstrings_, sizeof(errstrings_) / sizeof(errstrings_[0]));
    if(err != LOOPS_ERR_SUCCESS) printf(init_error_msg, err);
    printer_h_initialize(); if(err != LOOPS_ERR_SUCCESS) printf(init_error_msg, err);

#if __LOOPS_ARCH == __LOOPS_AARCH64
    err = backend_aarch64_h_initialize();  if(err != LOOPS_ERR_SUCCESS) printf(init_error_msg, err);
#elif __LOOPS_ARCH == __LOOPS_INTEL64
    err = backend_intel64_h_initialize();  if(err != LOOPS_ERR_SUCCESS) printf(init_error_msg, err);
#elif __LOOPS_ARCH == __LOOPS_RISCV
    err = backend_riscv_h_initialize();  if(err != LOOPS_ERR_SUCCESS) printf(init_error_msg, err);
#else
#error Unknown CPU
#endif

    atexit(finalize);
}

char* loops_strncpy(char* dest, const char* src, std::size_t count)
{
#if _MSC_VER
    strncpy_s(dest, count, src, _TRUNCATE);
    return dest;
#else
    return strncpy(dest, src, count);
#endif  
}

const char* get_errstring(int errid)
{
    static const char* unknown_err = "Loops: Unknown error. Problem in error system.";
    const char* result = NULL; 
    int err = loops_hashmap_get(errstrings, errid, &result);
    if(err == LOOPS_ERR_ELEMENT_NOT_FOUND)
        return unknown_err;
    else 
        return result;
}

namespace loops
{
#if !(__LOOPS_ARCH == __LOOPS_AARCH64 && __LOOPS_OS == __LOOPS_MAC)
    /*
    Next four functions are taken from FP16 library.
    Link: https://github.com/Maratyszcza/FP16
    */

    //License:
    /* 
    The MIT License (MIT)

    Copyright (c) 2017 Facebook Inc.
    Copyright (c) 2017 Georgia Institute of Technology
    Copyright 2019 Google LLC

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    */    
    static inline float fp32_from_bits(uint32_t w) 
    {
        union {
            uint32_t as_bits;
            float as_value;
        } fp32 = { w };
        return fp32.as_value;
    }

    static inline uint32_t fp32_to_bits(float f) {
        union {
            float as_value;
            uint32_t as_bits;
        } fp32 = { f };
        return fp32.as_bits;
    }

    static inline uint16_t fp16_ieee_from_fp32_value(float f) {
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) || defined(__GNUC__) && !defined(__STRICT_ANSI__)
        const float scale_to_inf = 0x1.0p+112f;
        const float scale_to_zero = 0x1.0p-110f;
    #else
        const float scale_to_inf = fp32_from_bits(UINT32_C(0x77800000));
        const float scale_to_zero = fp32_from_bits(UINT32_C(0x08800000));
    #endif
        float base = (fabsf(f) * scale_to_inf) * scale_to_zero;

        const uint32_t w = fp32_to_bits(f);
        const uint32_t shl1_w = w + w;
        const uint32_t sign = w & UINT32_C(0x80000000);
        uint32_t bias = shl1_w & UINT32_C(0xFF000000);
        if (bias < UINT32_C(0x71000000)) {
            bias = UINT32_C(0x71000000);
        }

        base = fp32_from_bits((bias >> 1) + UINT32_C(0x07800000)) + base;
        const uint32_t bits = fp32_to_bits(base);
        const uint32_t exp_bits = (bits >> 13) & UINT32_C(0x00007C00);
        const uint32_t mantissa_bits = bits & UINT32_C(0x00000FFF);
        const uint32_t nonsign = exp_bits + mantissa_bits;
        return (uint16_t)((sign >> 16) | (shl1_w > UINT32_C(0xFF000000) ? UINT16_C(0x7E00) : nonsign));
    }

    static inline float fp16_ieee_to_fp32_value(uint16_t h) 
    {
        const uint32_t w = (uint32_t) h << 16;
        const uint32_t sign = w & UINT32_C(0x80000000);
        const uint32_t two_w = w + w;

        const uint32_t exp_offset = UINT32_C(0xE0) << 23;
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) || defined(__GNUC__) && !defined(__STRICT_ANSI__)
        const float exp_scale = 0x1.0p-112f;
    #else
        const float exp_scale = fp32_from_bits(UINT32_C(0x7800000));
    #endif
        const float normalized_value = fp32_from_bits((two_w >> 4) + exp_offset) * exp_scale;

        const uint32_t magic_mask = UINT32_C(126) << 23;
        const float magic_bias = 0.5f;
        const float denormalized_value = fp32_from_bits((two_w >> 17) | magic_mask) - magic_bias;

        const uint32_t denormalized_cutoff = UINT32_C(1) << 27;
        const uint32_t result = sign |
            (two_w < denormalized_cutoff ? fp32_to_bits(denormalized_value) : fp32_to_bits(normalized_value));
        return fp32_from_bits(result);
    }
#endif

    f16_t::f16_t() : bits(0){}

    f16_t::f16_t(float x)
    {
        
    #if __LOOPS_ARCH == __LOOPS_AARCH64 && __LOOPS_OS == __LOOPS_MAC
        __fp16 _x = x;
        bits = *(reinterpret_cast<uint16_t*>(&_x));
    #else
        bits = fp16_ieee_from_fp32_value(x);
    #endif 
    }

    f16_t::operator float()
    {
    #if __LOOPS_ARCH == __LOOPS_AARCH64 && __LOOPS_OS == __LOOPS_MAC
        __fp16 res = *(reinterpret_cast<__fp16*>(&bits));
        return (float)res;
    #else
        return fp16_ieee_to_fp32_value(bits);
    #endif 
    }

    static FuncImpl* verify_owner(::std::initializer_list<Expr> args)
    {
        Func* inferedfunc = nullptr;
        for(Expr arg : args)
            if(arg.func() != nullptr) 
            {
                inferedfunc = arg.func();
                break;
            }
        if (inferedfunc == nullptr)
            throw std::runtime_error("Cannot find mother function in expression arguments.");
        for(Expr arg : args)
            if(arg.func() == nullptr)
                arg.infer_owner(inferedfunc);
            else if(arg.func() != inferedfunc)
                throw std::runtime_error("Registers of different functions as arguments of one expression.");
        return static_cast<FuncImpl*>(inferedfunc);
    }

    IReg::IReg() : idx(NOIDX), func(nullptr) {}

    IReg::IReg(const IExpr& fromwho)
    {
        Expr from_(fromwho.notype());
        func = verify_owner({from_});
        if(fromwho.opcode() == EXPR_LEAF && fromwho.leaf().tag == Arg::IIMMEDIATE && func == nullptr)
            throw std::runtime_error("Direct immediate assignment must be done via CONST_ operator, e.g.:\n    IReg var = CONST_(val);\n");
        Expr fromwho_(fromwho.notype());
        Arg unpacked = static_cast<FuncImpl*>(func)->get_code_collecting()->reg_constr(fromwho_);
        Assert(unpacked.tag == Arg::IREG);
        idx = unpacked.idx;
    }

    IReg::IReg(const IReg& r)
    {
        if(r.func != nullptr)
        {
            Expr fromwho = IExpr(r).notype();
            func = verify_owner({fromwho});
            Arg unpacked = static_cast<FuncImpl*>(func)->get_code_collecting()->reg_constr(fromwho);
            Assert(unpacked.tag == Arg::IREG);
            idx = unpacked.idx;
        }
        else
        {
            idx = NOIDX;
            func = nullptr;
        }
    }

    void IReg::copyidx(const IReg& from)
    {
        if(func != nullptr && func != from.func)
            throw std::runtime_error("Registers of different functions in idx assignment.");
        func = from.func;
        idx = from.idx;
    }

    void IReg::copyidx(const IExpr& from)
    {
        if(from.is_leaf() && from.leaf().tag == Arg::IREG)
        {
            Expr from_(from.notype());
            Func* newfunc = verify_owner({from_});
            if(func != nullptr && func != newfunc)
                throw std::runtime_error("Registers of different functions in idx assignment.");
            func = newfunc;
            idx = from.leaf().idx;
        }
        else
            copyidx(IReg(from));
    }

    IReg& IReg::operator=(const IReg& r)
    {
        IExpr fromwho(r);
        return operator=(fromwho);
    }

    IReg& IReg::operator=(const IExpr& fromwho)
    {
        Expr fromwho_(fromwho.notype());
        Expr me(*this);
        verify_owner({me, fromwho_})->get_code_collecting()->reg_assign(me, fromwho_);
        return (*this);
    }

    Arg::Arg(int64_t a_value) : idx(IReg::NOIDX), tag(Arg::IIMMEDIATE), value(a_value), flags(0){}

    Func* Expr::infer_owner(Func* preinfered)
    {
        Func* inferedfunc = preinfered ? preinfered : func();
        if(!is_leaf())
        {
            for(int i = 0; i < (int)children().size() && inferedfunc == nullptr; i++) 
                if(children()[i].func() != nullptr)
                    inferedfunc = children()[i].func();
            if(inferedfunc)
            {
                for(Expr& child : children())
                    if(child.func() == nullptr)
                        child.infer_owner(inferedfunc);
                    else if(child.func() != inferedfunc)
                        throw std::runtime_error("Registers of different functions as arguments of one expression.");
            }
        }
        if(inferedfunc)
            func() = inferedfunc;
        return inferedfunc;
    }

    Func::Func() : impl(nullptr) {}
    Func::Func(const Func& f) : impl(f.impl) { static_cast<FuncImpl*>(impl)->m_refcount++; }

    Func& Func::operator=(const Func& f)
    {
        FuncImpl* old_p = static_cast<FuncImpl*>(impl);
        if (impl) static_cast<FuncImpl*>(impl)->m_refcount--;
        impl = f.impl;
        if (impl != nullptr)
            static_cast<FuncImpl*>(impl)->m_refcount++;
        if (old_p && !old_p->m_refcount)
            delete old_p;
        return *this;
    }

    Func::~Func()
    {
        if (impl && !(--(static_cast<FuncImpl*>(impl)->m_refcount)))
            delete impl;
    }

    std::string Func::name() const { return static_cast<FuncImpl*>(impl)->name(); }
    int Func::signature() const { return static_cast<FuncImpl*>(impl)->signature(); }

    void* Func::ptr() { return static_cast<FuncImpl*>(impl)->ptr(); }
    void Func::printIR(std::ostream& out, int columns, const std::string& passname) const
    {
        static_cast<FuncImpl*>(impl)->printIR(out, columns, passname);
    }
    void Func::printAssembly(std::ostream& out, int columns) const
    {
        static_cast<FuncImpl*>(impl)->printAssembly(out, columns);
    }

    Func Func::make(Func* a_wrapped)
    {
        Func ret; 
        ret.impl = a_wrapped;
        static_cast<FuncImpl*>(ret.impl)->m_refcount = 1;
        return ret;
    }
    
    Context ExtractContext(const Expr& arg)
    {
        Expr arg_(arg);
        return verify_owner({arg_})->getContext()->getPublicInterface();
    }

    void newiopNoret(int opcode, ::std::initializer_list<Expr> args)
    {
        verify_owner(args)->get_code_collecting()->newiopNoret(opcode, args);
    }

    IExpr pow(const IExpr& a, int p)
    {
        Context CTX = ExtractContext(a.notype());
        USE_CONTEXT_(CTX);
        if(p == 0)
            return CONST_(1);
        IExpr _a = a;
        IExpr res;
        while (p)
            if (p & 1) 
            {
                res = (res.empty()) ? _a : res * _a;
                --p;
            }
            else 
            {
                _a = _a * _a;
                p >>= 1;
            }
        return res;
    }
    
    void loadvec_deinterleave2_(Arg& res1, Arg& res2, const IExpr& base)
    {
        verify_owner({base.notype()})->get_code_collecting()->loadvec_deinterleave2_(res1, res2, base.notype());
    }

    Context::Context() : impl(nullptr)
    {
        ContextImpl* _impl = new ContextImpl();
        _impl->m_refcount = 1;
        impl = _impl;
    }

    Context::Context(const Context& f) : impl(f.impl) { static_cast<ContextImpl*>(impl)->m_refcount++; }

    Context& Context::operator=(const Context& f)
    {
        ContextImpl* old_p = static_cast<ContextImpl*>(impl);
        if (impl) static_cast<ContextImpl*>(impl)->m_refcount--;
        impl = f.impl;
        if (impl != nullptr)
            static_cast<ContextImpl*>(impl)->m_refcount++;
        if (old_p && !old_p->m_refcount)
            delete old_p;
        return *this;
    }

    Context::~Context()
    {
        if (impl && !(--(static_cast<ContextImpl*>(impl)->m_refcount)))
            delete impl;
    }

    Func Context::getFunc(const std::string& name) { return static_cast<ContextImpl*>(impl)->getFunc(name); }
    bool Context::hasFunc(const std::string& name) { return static_cast<ContextImpl*>(impl)->hasFunc(name); }

    std::string Context::getPlatformName() const {return static_cast<ContextImpl*>(impl)->getPlatformName(); }
    int Context::vbytes() const {return static_cast<ContextImpl*>(impl)->vbytes(); }

    void Context::compileAll() {static_cast<ContextImpl*>(impl)->compileAll(); }
    void Context::debugModeOn() {static_cast<ContextImpl*>(impl)->debugModeOn(); }

    std::vector<std::string> Context::get_all_passes() { return static_cast<ContextImpl*>(impl)->get_all_passes(); }

    __Loops_CondPrefixMarker_::__Loops_CondPrefixMarker_(Context& CTX_):CTX(&CTX_) 
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_STEM_CSTART, {});
    }

    __Loops_CFScopeBracket_::__Loops_CFScopeBracket_(const __Loops_CondPrefixMarker_& inh, CFType _cftype, const IExpr& condition) : CTX(inh.CTX), cftype(_cftype)
    {
        CodeCollecting* coll = getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting();
        Expr condition_(condition.notype());
        switch (_cftype)
        {
        case(IF):
            coll->if_(condition_);
            break;
        case(ELIF):
            coll->subst_elif(condition_);
            break;
        case(WHILE):
            coll->while_(condition_);
            break;
        default:
            Assert(false);
        }
    }

    __Loops_CFScopeBracket_::__Loops_CFScopeBracket_(Context& _CTX) : CTX(&_CTX), cftype(ELSE)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->subst_else();
    }

    __Loops_CFScopeBracket_::~__Loops_CFScopeBracket_()
    {
        if(cftype == WHILE)
            getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->endwhile_();
        else
            getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->endif_();
    }

    __Loops_FuncScopeBracket_::__Loops_FuncScopeBracket_(Context* _CTX, const std::string& name, std::initializer_list<IReg*> params): CTX(_CTX)
    { getImpl(CTX)->startFunc(name, params); }

    __Loops_FuncScopeBracket_::~__Loops_FuncScopeBracket_() { getImpl(CTX)->endFunc(); }

    __Loops_CF_rvalue_::__Loops_CF_rvalue_(Context* _CTX) : CTX(_CTX) {}
    void __Loops_CF_rvalue_::break_(int depth){ getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->break_(depth); }
    void __Loops_CF_rvalue_::continue_(int depth) { getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->continue_(depth); }
    void __Loops_CF_rvalue_::return_() { getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->return_(); }
    void __Loops_CF_rvalue_::return_(const IExpr& r) { Expr r_(r.notype()); getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->return_(r_); }
    void __Loops_CF_rvalue_::return_(int64_t r) { Expr r_(r); getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->return_(r_); }

    void __Loops_CF_rvalue_::void_call(funcptr0_noret_t fptr)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { Expr(int64_t(fptr)) });
    }

    void __Loops_CF_rvalue_::void_call(funcptr1_noret_t fptr, const IExpr& arg0)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { Expr(int64_t(fptr)), arg0.notype() });
    }

    void __Loops_CF_rvalue_::void_call(funcptr2_noret_t fptr, const IExpr& arg0, const IExpr& arg1)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { Expr(int64_t(fptr)), arg0.notype(), arg1.notype() });
    }

    void __Loops_CF_rvalue_::void_call(funcptr3_noret_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { Expr(int64_t(fptr)), arg0.notype(), arg1.notype(), arg2.notype() });
    }

    void __Loops_CF_rvalue_::void_call(funcptr4_noret_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { Expr(int64_t(fptr)), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype() });
    }

    void __Loops_CF_rvalue_::void_call(funcptr5_noret_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { Expr(int64_t(fptr)), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype() });
    }

    void __Loops_CF_rvalue_::void_call(funcptr6_noret_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { Expr(int64_t(fptr)), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype(), arg5.notype() });
    }

    void __Loops_CF_rvalue_::void_call(funcptr7_noret_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5, const IExpr& arg6)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { Expr(int64_t(fptr)), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype(), arg5.notype(), arg6.notype() });
    }

    void __Loops_CF_rvalue_::void_call(funcptr8_noret_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5, const IExpr& arg6, const IExpr& arg7)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { Expr(int64_t(fptr)), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype(), arg5.notype(), arg6.notype(), arg7.notype() });
    }

    IReg __Loops_CF_rvalue_::call_(funcptr0_t fptr)
    {
        Expr fptr_ = Expr(int64_t(fptr));
        fptr_.func() = getImpl((getImpl(CTX)->getCurrentFunc()));
        IExpr res(OP_CALL, TYPE_I64, {fptr_});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(funcptr1_t fptr, const IExpr& arg0)
    {
        Expr fptr_ = Expr(int64_t(fptr));
        fptr_.func() = getImpl((getImpl(CTX)->getCurrentFunc()));
        IExpr res(OP_CALL, TYPE_I64, {fptr_, arg0.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(funcptr2_t fptr, const IExpr& arg0, const IExpr& arg1)
    {
        Expr fptr_ = Expr(int64_t(fptr));
        fptr_.func() = getImpl((getImpl(CTX)->getCurrentFunc()));
        IExpr res(OP_CALL, TYPE_I64, {fptr_, arg0.notype(), arg1.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(funcptr3_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2)
    {
        Expr fptr_ = Expr(int64_t(fptr));
        fptr_.func() = getImpl((getImpl(CTX)->getCurrentFunc()));
        IExpr res(OP_CALL, TYPE_I64, {fptr_, arg0.notype(), arg1.notype(), arg2.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(funcptr4_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3)
    {
        Expr fptr_ = Expr(int64_t(fptr));
        fptr_.func() = getImpl((getImpl(CTX)->getCurrentFunc()));
        IExpr res(OP_CALL, TYPE_I64, {fptr_, arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(funcptr5_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4)
    {
        Expr fptr_ = Expr(int64_t(fptr));
        fptr_.func() = getImpl((getImpl(CTX)->getCurrentFunc()));
        IExpr res(OP_CALL, TYPE_I64, {fptr_, arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(funcptr6_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5)
    {
        Expr fptr_ = Expr(int64_t(fptr));
        fptr_.func() = getImpl((getImpl(CTX)->getCurrentFunc()));
        IExpr res(OP_CALL, TYPE_I64, {fptr_, arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype(), arg5.notype()});
        return IReg(res);
    }
        
    IReg __Loops_CF_rvalue_::call_(funcptr7_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5, const IExpr& arg6)
    {
        Expr fptr_ = Expr(int64_t(fptr));
        fptr_.func() = getImpl((getImpl(CTX)->getCurrentFunc()));
        IExpr res(OP_CALL, TYPE_I64, {fptr_, arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype(), arg5.notype(), arg6.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(funcptr8_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5, const IExpr& arg6, const IExpr& arg7)
    {
        Expr fptr_ = Expr(int64_t(fptr));
        fptr_.func() = getImpl((getImpl(CTX)->getCurrentFunc()));
        IExpr res(OP_CALL, TYPE_I64, {fptr_, arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype(), arg5.notype(), arg6.notype(), arg7.notype()});
        return IReg(res);
    }

    void __Loops_CF_rvalue_::void_call(const IExpr& fptr)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { fptr.notype() });
    }

    void __Loops_CF_rvalue_::void_call(const IExpr& fptr, const IExpr& arg0)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { fptr.notype(), arg0.notype() });
    }

    void __Loops_CF_rvalue_::void_call(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { fptr.notype(), arg0.notype(), arg1.notype() });
    }

    void __Loops_CF_rvalue_::void_call(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { fptr.notype(), arg0.notype(), arg1.notype(), arg2.notype() });
    }

    void __Loops_CF_rvalue_::void_call(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { fptr.notype(), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype() });
    }

    void __Loops_CF_rvalue_::void_call(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { fptr.notype(), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype() });
    }

    void __Loops_CF_rvalue_::void_call(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { fptr.notype(), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype(), arg5.notype() });
    }

    void __Loops_CF_rvalue_::void_call(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5, const IExpr& arg6)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { fptr.notype(), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype(), arg5.notype(), arg6.notype() });
    }

    void __Loops_CF_rvalue_::void_call(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5, const IExpr& arg6, const IExpr& arg7)
    {
        getImpl((getImpl(CTX)->getCurrentFunc()))->get_code_collecting()->newiopNoret(OP_CALL_NORET, { fptr.notype(), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype(), arg5.notype(), arg6.notype(), arg7.notype() });
    }

    IReg __Loops_CF_rvalue_::call_(const IExpr& fptr)
    {
        IExpr res(OP_CALL, TYPE_I64, {fptr.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(const IExpr& fptr, const IExpr& arg0)
    {
        IExpr res(OP_CALL, TYPE_I64, {fptr.notype(), arg0.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1)
    {
        IExpr res(OP_CALL, TYPE_I64, {fptr.notype(), arg0.notype(), arg1.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2)
    {
        IExpr res(OP_CALL, TYPE_I64, {fptr.notype(), arg0.notype(), arg1.notype(), arg2.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3)
    {
        IExpr res(OP_CALL, TYPE_I64, {fptr.notype(), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4)
    {
        IExpr res(OP_CALL, TYPE_I64, {fptr.notype(), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5)
    {
        IExpr res(OP_CALL, TYPE_I64, {fptr.notype(), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype(), arg5.notype()});
        return IReg(res);
    }
        
    IReg __Loops_CF_rvalue_::call_(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5, const IExpr& arg6)
    {
        IExpr res(OP_CALL, TYPE_I64, {fptr.notype(), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype(), arg5.notype(), arg6.notype()});
        return IReg(res);
    }

    IReg __Loops_CF_rvalue_::call_(const IExpr& fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5, const IExpr& arg6, const IExpr& arg7)
    {
        IExpr res(OP_CALL, TYPE_I64, {fptr.notype(), arg0.notype(), arg1.notype(), arg2.notype(), arg3.notype(), arg4.notype(), arg5.notype(), arg6.notype(), arg7.notype()});
        return IReg(res);
    }

    void __setfunc_by_context_(Context* CTX, Expr& expr) { expr.func() = getImpl(getImpl(CTX)->getCurrentFunc()); }

    exp_consts::exp_consts(Context CTX)
    {
        USE_CONTEXT_(CTX);
        lo.copyidx(VReg<float>(VCONST_(float, -88.3762626647949f)));
        hi.copyidx(VReg<float>(VCONST_(float, 88.3762626647949f)));
        half.copyidx(VReg<float>(VCONST_(float, 0.5f)));
        one.copyidx(VReg<float>(VCONST_(float, 1.f)));
        LOG2EF.copyidx(VReg<float>(VCONST_(float, 1.44269504088896341f)));
        C1.copyidx(VReg<float>(VCONST_(float, -0.693359375f)));
        C2.copyidx(VReg<float>(VCONST_(float, 2.12194440e-4f)));
        p0.copyidx(VReg<float>(VCONST_(float, 1.9875691500E-4f)));
        p1.copyidx(VReg<float>(VCONST_(float, 1.3981999507E-3f)));
        p2.copyidx(VReg<float>(VCONST_(float, 8.3334519073E-3f)));
        p3.copyidx(VReg<float>(VCONST_(float, 4.1665795894E-2f)));
        p4.copyidx(VReg<float>(VCONST_(float, 1.6666665459E-1f)));
        p5.copyidx(VReg<float>(VCONST_(float, 5.0000001201E-1f)));
        _7f.copyidx(VReg<int32_t>(VCONST_(int32_t, 0x7f)));
    }

    exp_consts expInit(Context CTX)
    {
        return exp_consts(CTX);
    }

    VExpr<float> exp(const VExpr<float>& x, const exp_consts& expt)
    {
        VExpr<float> vexp_x = min(x, expt.hi); 
        vexp_x = max(vexp_x, expt.lo);
        VExpr<float> vexp_fx = fma(expt.half, vexp_x, expt.LOG2EF);
        VExpr<int32_t> vexp_mm = floor<int32_t>(vexp_fx);
        vexp_fx = cast<float>(vexp_mm);
        vexp_mm = vexp_mm + expt._7f;
        vexp_mm = vexp_mm << 23;
        vexp_x = fma(vexp_x, vexp_fx, expt.C1);
        vexp_x = fma(vexp_x, vexp_fx, expt.C2);
        VExpr<float> vexp_z = vexp_x * vexp_x;
        VExpr<float> vexp_y = fma(expt.p1, vexp_x, expt.p0);
        vexp_y = fma(expt.p2, vexp_y, vexp_x);
        vexp_y = fma(expt.p3, vexp_y, vexp_x);
        vexp_y = fma(expt.p4, vexp_y, vexp_x);
        vexp_y = fma(expt.p5, vexp_y, vexp_x);
        vexp_y = fma(vexp_x, vexp_y, vexp_z);
        vexp_y = vexp_y + expt.one;
        return vexp_y * reinterpret<float>(vexp_mm);
    }

    void VReg_constr_(const Expr& fromwho, int& idx, Func*& func, int restype)
    {
        Expr fromwho_(fromwho);
        func = verify_owner({fromwho_});
        if(fromwho.opcode() == EXPR_LEAF && fromwho.leaf().tag == Arg::IIMMEDIATE && func == nullptr)
            throw std::runtime_error("Direct immediate assignment must be done via VCONST_ operator, e.g.:\n    VReg<float> var = VCONST_(float, 3.14);\n");
        Arg unpacked = static_cast<FuncImpl*>(func)->get_code_collecting()->reg_constr(fromwho_);
        Assert(unpacked.tag == Arg::VREG && unpacked.elemtype == restype);
        idx = unpacked.idx;
    }

    void VReg_assign_(const Expr& target, const Expr& from)
    {
        Expr from_(from);
        Expr target_(target);
        verify_owner({target_, from_})->get_code_collecting()->reg_assign(target_, from_);
    }

    void VReg_copyidx_(const Expr& fromwho, int& idx, Func*& func)
    {
        Expr fromwho_(fromwho);
        Func* newfunc = verify_owner({fromwho_});
        if(func != nullptr && func != newfunc)
            throw std::runtime_error("Registers of different functions in idx assignment.");
        func = newfunc;
        idx = fromwho_.leaf().idx;
    }

    Syntop::Syntop(): opcode(OP_NOINIT), args_size(0){}
    Syntop::Syntop(const Syntop& fwho) : opcode(fwho.opcode), args_size(fwho.args_size)
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::runtime_error("Syntaxic operation: too much args!");
        std::copy(fwho.begin(), fwho.end(), args);
    }

    Syntop& Syntop::operator=(const Syntop &fwho)
    {
        opcode = fwho.opcode;
        args_size = fwho.args_size;
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::runtime_error("Syntaxic operation: too much args!");
        std::copy(fwho.begin(), fwho.end(), args);
        return *this;
    }

    Syntop::Syntop(int a_opcode, const std::vector<Arg>& a_args) : opcode(a_opcode), args_size((int)a_args.size())
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::runtime_error("Syntaxic operation: too much args!");
        std::copy(a_args.begin(), a_args.end(), args);
    }

    Syntop::Syntop(int a_opcode, std::initializer_list<Arg> a_args): opcode(a_opcode), args_size((int)a_args.size())
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::runtime_error("Syntaxic operation: too much args!");
        std::copy(a_args.begin(), a_args.end(), args);
    }

    Syntop::Syntop(int a_opcode, std::initializer_list<Arg> a_prefix, std::initializer_list<Arg> a_args): opcode(a_opcode), args_size((int)(a_args.size() + a_prefix.size()))
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::runtime_error("Syntaxic operation: too much args!");
        std::copy(a_prefix.begin(), a_prefix.end(), args);
        std::copy(a_args.begin(), a_args.end(), args + a_prefix.size());
    }

    ContextImpl::ContextImpl() : Context(nullptr), m_refcount(0), m_debug_mode(false) {
#if __LOOPS_ARCH == __LOOPS_AARCH64
        std::shared_ptr<Aarch64Backend> backend = std::make_shared<Aarch64Backend>();
#elif __LOOPS_ARCH == __LOOPS_INTEL64
        std::shared_ptr<Intel64Backend> backend = std::make_shared<Intel64Backend>();
#elif __LOOPS_ARCH == __LOOPS_RISCV
        std::shared_ptr<RiscVBackend> backend = std::make_shared<RiscVBackend>();
#endif
        m_backend = std::static_pointer_cast<Backend>(backend);
    }

    void ContextImpl::startFunc(const std::string& name, std::initializer_list<IReg*> params)
    {
        if(m_functionsStorage.find(name) != m_functionsStorage.end())
            throw std::runtime_error("Function is already registered.");  //TODO(ch): We need good exception class.
        m_currentFunc = m_functionsStorage.emplace(name, FuncImpl::makeWrapper(name, this, params)).first->second;
    }

    void ContextImpl::endFunc()
    {
        FuncImpl* func = getImpl(&m_currentFunc);
        func->endFunc();
        m_currentFunc = Func();
    }

    Func ContextImpl::getFunc(const std::string& name)
    {
        auto found = m_functionsStorage.find(name);
        if(found == m_functionsStorage.end()) 
            throw std::runtime_error("Cannot find function.");
        return found->second;
    }

    bool ContextImpl::hasFunc(const std::string& name)
    {
        auto found = m_functionsStorage.find(name);
        return found != m_functionsStorage.end();
    }
    
    std::string ContextImpl::getPlatformName() const
    { return m_backend->name(); }

    int ContextImpl::vbytes() const
    { return m_backend->getVectorRegisterBits() >> 3; }

    void ContextImpl::compileAll()
    {
        const size_t funcAlignment = 16; //TODO(ch): Get precise info from backend.
        size_t totalSize = funcAlignment;
        std::vector<FuncBodyBuf> bodies;
        bodies.reserve(m_functionsStorage.size());
        for(auto par:m_functionsStorage)
        {
            FuncImpl* func = getImpl(&par.second);
            const FuncBodyBuf body = func->get_hex_body();
            size_t bsize = body->size();
            bsize = (bsize / funcAlignment) * funcAlignment + ((bsize % funcAlignment) ? funcAlignment : 0); //TODO(ch): normal alignment expression, mkay?
            totalSize += bsize;
            bodies.push_back(body);
        }
        Allocator* alloc= m_backend->getAllocator();
        uint8_t* exebuf = alloc->allocate(totalSize);
        uint8_t* exeptr = exebuf;
        auto curBody = bodies.begin();
        for(auto par:m_functionsStorage)
        {
            FuncImpl* func = getImpl(&par.second);
            uint64_t dptr = reinterpret_cast<uint64_t>(exeptr);
            dptr %= funcAlignment;
            dptr = dptr ? funcAlignment - dptr : 0;
            exeptr += dptr;
            memcpy(exeptr, (void*)(*curBody)->data(), (*curBody)->size()); //TODO(ch): You have to change used adresses before.
            func->set_compiled_ptr(exeptr);
            exeptr += (*curBody)->size();
            ++curBody;
        }
        alloc->protect2Execution(exebuf);
    }

    std::vector<std::string> ContextImpl::get_all_passes()
    {
        FuncImpl dummy0("", this, {});
        Pipeline dummy(getBackend(), &dummy0, "", {});
        return dummy.get_all_passes();
    }

    Context ContextImpl::getPublicInterface()
    {
        Assert(m_refcount>0);
        //Trick to workaround abscence of makeWrapper function of Context as smartpointer.
        //ContextImpl simulates Context(by self-referencing) and creates a smartpointer copy.
        impl = this;
        Context ret = (*this);
        impl = nullptr;
        return ret;
    }
}
