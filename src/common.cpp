/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "loops/loops.hpp"
#include "backend_aarch64.hpp"
#include "backend_intel64.hpp"
#include "backend.hpp"
#include "reg_allocator.hpp"
#include "common.hpp"
#include "func_impl.hpp"
#include <map>
#include <stack>
#include <cstring>

namespace loops
{
    f16_t::f16_t(float x)
    {
    #if __LOOPS_ARCH == __LOOPS_INTEL64 && __LOOPS_OS == __LOOPS_MAC
        (*(reinterpret_cast<__fp16*>(*bits))) = __fp16(x);
    #endif
    }

    IReg::IReg() : idx(NOIDX), func(nullptr) {}

    IReg::IReg(const IReg& r)
    {
        IReg selfval = newiop(OP_MOV, { r });
        idx = selfval.idx;
        func = selfval.func;
    }

    IReg::IReg(IReg&& a) noexcept : func(a.func), idx(a.idx) {}

    IReg& IReg::operator=(const IReg& r)
    {
        if (r.func == nullptr)
            throw std::runtime_error("Cannot find motherfunction in registers.");
        FuncImpl* funcimpl = static_cast<FuncImpl*>(func);
        funcimpl->newiopPreret(OP_MOV, { r }, idx);
        return (*this);
    }

    Arg::Arg() : idx(IReg::NOIDX), func(nullptr), tag(EMPTY), value(0), flags(0) {}
    Arg::Arg(const IReg& r) : idx(r.idx), func(r.func), tag(r.func ? Arg::IREG : Arg::EMPTY), value(0), flags(0) {}
    Arg::Arg(int64_t a_value, Context* ctx) : idx(IReg::NOIDX), func(nullptr), tag(Arg::IIMMEDIATE), value(a_value), flags(0)
    {
        if(ctx)
            func = getImpl(getImpl(ctx)->getCurrentFunc());
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

    void* Func::ptr() { return static_cast<FuncImpl*>(impl)->ptr(); }
    void Func::printBytecode(std::ostream& out) const
    {
        static_cast<FuncImpl*>(impl)->applySyntopStages();
        static_cast<FuncImpl*>(impl)->printBytecode(out);
    }
    void Func::printAssembly(std::ostream& out, int columns) const
    {
        static_cast<FuncImpl*>(impl)->applySyntopStages();
        static_cast<FuncImpl*>(impl)->printAssembly(out, columns);
    }

    Func Func::make(Func* a_wrapped)
    {
        Func ret; 
        ret.impl = a_wrapped;
        static_cast<FuncImpl*>(ret.impl)->m_refcount = 1;
        return ret;
    }
    
    IReg newiop(int opcode, std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {
        return static_cast<IReg&&>(FuncImpl::verifyArgs(args)->newiop(opcode, args, tryImmList));
    }

    IReg newiop(int opcode, int depth, std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {
        return FuncImpl::verifyArgs(args)->newiop(opcode,depth,args, tryImmList);
    }

    void newiopNoret(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {
        FuncImpl::verifyArgs(args)->newiopNoret(opcode, args, tryImmList);
    }

    void newiopNoret(int opcode, int depth, std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {
        FuncImpl::verifyArgs(args)->newiopNoret(opcode, depth, args, tryImmList);
    }
    
    void newiopAug(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {
        return newiopNoret(opcode, args, tryImmList);
    }

    VReg<uint8_t> newiopV_U8  (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {   return FuncImpl::verifyArgs(args)->newiopV<uint8_t>(opcode, args, tryImmList);  }
    VReg<int8_t> newiopV_I8  (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {   return FuncImpl::verifyArgs(args)->newiopV<int8_t>(opcode, args, tryImmList);  }
    VReg<uint16_t> newiopV_U16 (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {   return FuncImpl::verifyArgs(args)->newiopV<uint16_t>(opcode, args, tryImmList);  }
    VReg<int16_t> newiopV_I16 (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {   return FuncImpl::verifyArgs(args)->newiopV<int16_t>(opcode, args, tryImmList);  }
    VReg<uint32_t> newiopV_U32 (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {   return FuncImpl::verifyArgs(args)->newiopV<uint32_t>(opcode, args, tryImmList);  }
    VReg<int32_t> newiopV_I32 (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {   return FuncImpl::verifyArgs(args)->newiopV<int32_t>(opcode, args, tryImmList);  }
    VReg<uint64_t> newiopV_U64 (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {   return FuncImpl::verifyArgs(args)->newiopV<uint64_t>(opcode, args, tryImmList);  }
    VReg<int64_t> newiopV_I64 (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {   return FuncImpl::verifyArgs(args)->newiopV<int64_t>(opcode, args, tryImmList);  }
    VReg<f16_t> newiopV_FP16(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {   return FuncImpl::verifyArgs(args)->newiopV<f16_t>(opcode, args, tryImmList);  }
    //VReg<...> newiopV_BF16(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList);
    VReg<float> newiopV_FP32(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {   return FuncImpl::verifyArgs(args)->newiopV<float>(opcode, args, tryImmList);  }
    VReg<double> newiopV_FP64(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
    {   return FuncImpl::verifyArgs(args)->newiopV<double>(opcode, args, tryImmList);  }

    IReg select(const IReg& cond, const IReg& truev, const IReg& falsev)
    {
        return FuncImpl::verifyArgs({ truev, falsev })->select(cond, truev, falsev); //TODO(ch): IMPORTANT(CMPLCOND)
    }

    IReg select(const IReg& cond, int64_t truev, const IReg& falsev)
    {
        return FuncImpl::verifyArgs({ falsev })->select(cond, truev, falsev); //TODO(ch): IMPORTANT(CMPLCOND)
    }

    IReg select(const IReg& cond, const IReg& truev, int64_t falsev)
    {
        return FuncImpl::verifyArgs({ truev })->select(cond, truev, falsev); //TODO(ch): IMPORTANT(CMPLCOND)
        //TODO(ch): IMPORTANT(CMPLCOND)
    }

    IReg operator == (const IReg& a, const IReg& b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({a,b});
        fnc->m_cmpopcode = IC_EQ;
        newiopNoret(OP_CMP, {a, b});
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator == (const IReg& a, int64_t b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({ a });
        fnc->m_cmpopcode = IC_EQ;
        newiopNoret(OP_CMP, { a, Arg(b) }, { 1 });
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator != (const IReg& a, const IReg& b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({a,b});
        fnc->m_cmpopcode = IC_NE;
        newiopNoret(OP_CMP, {a, b});
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator != (const IReg& a, int64_t b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({ a });
        fnc->m_cmpopcode = IC_NE;
        newiopNoret(OP_CMP, { a, Arg(b) }, { 1 });
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator <= (const IReg& a, const IReg& b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({a,b});
        fnc->m_cmpopcode = IC_LE;
        newiopNoret(OP_CMP, {a, b});
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator <= (const IReg& a, int64_t b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({ a });
        fnc->m_cmpopcode = IC_LE;
        newiopNoret(OP_CMP, { a, Arg(b) }, { 1 });
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator >= (const IReg& a, const IReg& b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({a,b});
        fnc->m_cmpopcode = IC_GE;
        newiopNoret(OP_CMP, {a, b});
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator >= (const IReg& a, int64_t b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({ a });
        fnc->m_cmpopcode = IC_GE;
        newiopNoret(OP_CMP, { a, Arg(b) }, { 1 });
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator > (const IReg& a, const IReg& b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({a,b});
        fnc->m_cmpopcode = IC_GT;
        newiopNoret(OP_CMP, {a, b});
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator > (const IReg& a, int64_t b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({ a });
        fnc->m_cmpopcode = IC_GT;
        newiopNoret(OP_CMP, { a, Arg(b) },{ 1 });
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator < (const IReg& a, const IReg& b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({a,b});
        fnc->m_cmpopcode = IC_LT;
        newiopNoret(OP_CMP, {a, b});
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator < (const IReg& a, int64_t b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({ a });
        fnc->m_cmpopcode = IC_LT;
        newiopNoret(OP_CMP, { a, Arg(b) }, { 1 });
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }

    Context::Context() : impl(nullptr)
    {
        impl = new ContextImpl(this);
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

    void Context::startFunc(const std::string& name, std::initializer_list<IReg*> params) { static_cast<ContextImpl*>(impl)->startFunc(name, params);    }
    void Context::endFunc() { static_cast<ContextImpl*>(impl)->endFunc(); }
    Func Context::getFunc(const std::string& name) { return static_cast<ContextImpl*>(impl)->getFunc(name); }

    IReg Context::const_(int64_t value)    { return getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->const_(value);    }
    void Context::while_(const IReg& r)  { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->while_(r); }
    void Context::endwhile_()  { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->endwhile_(); }
    void Context::break_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->break_(); }
    void Context::continue_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->continue_(); }

    void Context::if_(const IReg& r) { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->if_(r); }
    void Context::elif_(const IReg& r) { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->elif_(r); };
    void Context::else_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->else_(); };
    void Context::endif_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->endif_(); }

    void Context::return_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->return_(); }
    void Context::return_(int64_t retval) { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->return_(retval); }
    void Context::return_(const IReg& retval) { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->return_(retval); }
    std::string Context::getPlatformName() const {return static_cast<ContextImpl*>(impl)->getPlatformName(); }
    size_t Context::vectorRegisterSize() const {return static_cast<ContextImpl*>(impl)->vectorRegisterSize(); }

    void Context::compileAll() {static_cast<ContextImpl*>(impl)->compileAll(); }

    __Loops_CFScopeBracket_::__Loops_CFScopeBracket_(Context* _CTX, CFType _cftype, const IReg& condition) : CTX(_CTX), cftype(_cftype)
    {
        switch (_cftype)
        {
        case(IF):
            CTX->if_(condition);
            break;
        case(ELIF):
            getImpl(getImpl(CTX)->getCurrentFunc())->subst_elif(condition);
            break;
        case(ELSE):
            getImpl(getImpl(CTX)->getCurrentFunc())->subst_else();
            break;
        case(WHILE):
            CTX->while_(condition);
            break;
        default:
            Assert(false);
        }
    }

    __Loops_CFScopeBracket_::~__Loops_CFScopeBracket_()
    {
        if(cftype == WHILE)
            CTX->endwhile_();
        else
            CTX->endif_();
    }

    __Loops_FuncScopeBracket_::__Loops_FuncScopeBracket_(Context* _CTX, const std::string& name, std::initializer_list<IReg*> params): CTX(_CTX)
    {
        CTX->startFunc(name, params);
    }

    __Loops_FuncScopeBracket_::~__Loops_FuncScopeBracket_() { CTX->endFunc(); }

    Syntop::Syntop(): opcode(OP_NOINIT), args_size(0), spillPrefix(0), spillPostfix(0){}
    Syntop::Syntop(const Syntop& fwho) : opcode(fwho.opcode), args_size(fwho.args_size), spillPrefix(fwho.spillPrefix), spillPostfix(fwho.spillPostfix)
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::runtime_error("Syntaxic operation: too much args!");
        std::copy(fwho.begin(), fwho.end(), args);
    }

    Syntop::Syntop(int a_opcode, const std::vector<Arg>& a_args) : opcode(a_opcode), args_size(a_args.size()), spillPrefix(0), spillPostfix(0)
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::runtime_error("Syntaxic operation: too much args!");
        std::copy(a_args.begin(), a_args.end(), args);
    }

    Syntop::Syntop(int a_opcode, std::initializer_list<Arg> a_args): opcode(a_opcode), args_size(a_args.size()), spillPrefix(0), spillPostfix(0)
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::runtime_error("Syntaxic operation: too much args!");
        std::copy(a_args.begin(), a_args.end(), args);
    }

    Syntop::Syntop(int a_opcode, std::initializer_list<Arg> a_prefix, std::initializer_list<Arg> a_args): opcode(a_opcode), args_size(a_args.size() + a_prefix.size()), spillPrefix(0), spillPostfix(0)
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::runtime_error("Syntaxic operation: too much args!");
        std::copy(a_prefix.begin(), a_prefix.end(), args);
        std::copy(a_args.begin(), a_args.end(), args + a_prefix.size());
    }

    ContextImpl::ContextImpl(Context* owner) : Context(nullptr), m_owner(owner), m_refcount(0) {
#if __LOOPS_ARCH == __LOOPS_AARCH64
        std::shared_ptr<Aarch64Backend> backend = std::make_shared<Aarch64Backend>();
#elif __LOOPS_ARCH == __LOOPS_INTEL64
        std::shared_ptr<Intel64Backend> backend = std::make_shared<Intel64Backend>();
#endif
        m_backend = std::static_pointer_cast<Backend>(backend);
        m_registerAllocator = std::make_shared<RegisterAllocator>(this);
    }

    void ContextImpl::startFunc(const std::string& name, std::initializer_list<IReg*> params)
    {
        if(m_functionsStorage.find(name) != m_functionsStorage.end())
            throw std::runtime_error("Function is already registered.");  //TODO(ch): We need good exception class.
        m_currentFunc = m_functionsStorage.emplace(name, FuncImpl::makeWrapper(name, m_owner, params)).first->second;
    }

    void ContextImpl::endFunc()
    {
        FuncImpl* func = getImpl(&m_currentFunc);
        m_currentFunc = Func();
    }

    Func ContextImpl::getFunc(const std::string& name)
    {
        auto found = m_functionsStorage.find(name);
        if(found == m_functionsStorage.end()) 
            throw std::runtime_error("Cannot find function.");
        return found->second;
    }
    
    std::string ContextImpl::getPlatformName() const
    { return m_backend->name(); }

    size_t ContextImpl::vectorRegisterSize() const
    { return m_backend->getVectorRegisterSize() >> 3; }

    void ContextImpl::compileAll()
    {
        const size_t funcAlignment = 16; //TODO(ch): Get precise info from backend.
        size_t totalSize = funcAlignment;
        std::vector<FuncBodyBuf> bodies;
        bodies.reserve(m_functionsStorage.size());
        for(auto par:m_functionsStorage)
        {
            FuncImpl* func = getImpl(&par.second);
            func->applySyntopStages();
            FuncBodyBuf body = m_backend->target2Hex(m_backend->bytecode2Target(func->getData()));
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
            func->setCompiledPtr(exeptr);
            exeptr += (*curBody)->size();
            ++curBody;
        }
        alloc->protect2Execution(exebuf);
    }
}
