/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "loops/loops.hpp"
#include "aarch64.hpp"
#include "intel64.hpp"
#include "backend.hpp"
#include "reg_allocator.hpp"
#include "common.hpp"
#include "func_impl.hpp"
#include <map>
#include <stack>

namespace loops
{
    const size_t IReg::NOIDX = static_cast<size_t>(-1);

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
            throw std::string("Cannot find motherfunction in registers.");
        FuncImpl* funcimpl = static_cast<FuncImpl*>(func);
        funcimpl->newiop(OP_MOV, { r }, idx);
        return (*this);
    }

    Arg::Arg() : idx(IReg::NOIDX), func(nullptr), tag(EMPTY), value(0), flags(0) {}
    Arg::Arg(const IReg& r) : idx(r.idx), func(r.func), tag(r.func ? Arg::IREG : Arg::EMPTY), value(0), flags(0) {}
    Arg::Arg(int64_t a_value) : idx(IReg::NOIDX), func(nullptr), tag(Arg::ICONST), value(a_value), flags(0) {}

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
    void Func::printBytecode(std::ostream& out) const { static_cast<FuncImpl*>(impl)->printBytecode(out); }
    void Func::printAssembly(std::ostream& out, int columns) const { static_cast<FuncImpl*>(impl)->printAssembly(out, columns); }

    Func Func::make(Func* a_wrapped)
    {
        Func ret; 
        ret.impl = a_wrapped;
        static_cast<FuncImpl*>(ret.impl)->m_refcount = 1;
        return ret;
    }
    
    IReg newiop(int opcode, std::initializer_list<Arg> args)
    {
        return static_cast<IReg&&>(FuncImpl::verifyArgs(args)->newiop(opcode, args));
    }

    IReg newiop(int opcode, int depth, std::initializer_list<Arg> args)
    {
        return FuncImpl::verifyArgs(args)->newiop(opcode,depth,args);
    }

    void newiopNoret(int opcode, ::std::initializer_list<Arg> args)
    {
        FuncImpl::verifyArgs(args)->newiopNoret(opcode, args);
    }

    void newiopNoret(int opcode, int depth, std::initializer_list<Arg> args)
    {
        FuncImpl::verifyArgs(args)->newiopNoret(opcode, depth, args);
    }
    
    void newiopAug(int opcode, ::std::initializer_list<Arg> args)
    {
        return newiopNoret(opcode, args);
    }

    IReg operator == (const IReg& a, const IReg& b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({a,b});
        fnc->m_cmpopcode = OP_JMP_EQ;
        newiopNoret(OP_CMP, {a, b});
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator != (const IReg& a, const IReg& b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({a,b});
        fnc->m_cmpopcode = OP_JMP_NE;
        newiopNoret(OP_CMP, {a, b});
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator <= (const IReg& a, const IReg& b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({a,b});
        fnc->m_cmpopcode = OP_JMP_LE;
        newiopNoret(OP_CMP, {a, b});
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator >= (const IReg& a, const IReg& b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({a,b});
        fnc->m_cmpopcode = OP_JMP_GE;
        newiopNoret(OP_CMP, {a, b});
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator > (const IReg& a, const IReg& b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({a,b});
        fnc->m_cmpopcode = OP_JMP_GT;
        newiopNoret(OP_CMP, {a, b});
        return IReg(); //TODO(ch): IMPORTANT(CMPLCOND)
    }
    IReg operator < (const IReg& a, const IReg& b)
    {
        FuncImpl* fnc = FuncImpl::verifyArgs({a,b});
        fnc->m_cmpopcode = OP_JMP_LT;
        newiopNoret(OP_CMP, {a, b});
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
    void Context::do_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->do_(); }
    void Context::while_(const IReg& r) { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->while_(r); }
    void Context::doif_(const IReg& r)  { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->doif_(r); }
    void Context::enddo_()  { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->enddo_(); }
    void Context::break_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->break_(); }
    void Context::continue_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->continue_(); }

    void Context::if_(const IReg& r) { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->if_(r); }
    void Context::elif_(const IReg& r) { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->elif_(r); };
    void Context::else_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->else_(); };
    void Context::endif_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->endif_(); }

    void Context::return_(const IReg& retval) { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->return_(retval); }
    void Context::return_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->return_(); }
    std::string Context::getPlatformName() const {return static_cast<ContextImpl*>(impl)->getPlatformName(); }
    void Context::compileAll() {static_cast<ContextImpl*>(impl)->compileAll(); }


    Syntop::Syntop(): opcode(OP_NOINIT), args_size(0){}
    Syntop::Syntop(const Syntop& fwho) : ExpandableClass(fwho), opcode(fwho.opcode), args_size(fwho.args_size)
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::string("Syntaxic operation: too much args!");
        std::copy(fwho.begin(), fwho.end(), args);
    }

    Syntop::Syntop(int a_opcode, const std::vector<Arg>& a_args) : opcode(a_opcode), args_size(a_args.size())
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::string("Syntaxic operation: too much args!");
        std::copy(a_args.begin(), a_args.end(), args);
    }

    Syntop::Syntop(int a_opcode, std::initializer_list<Arg> a_args): opcode(a_opcode), args_size(a_args.size())
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::string("Syntaxic operation: too much args!");
        std::copy(a_args.begin(), a_args.end(), args);
    }

    Syntop::Syntop(int a_opcode, std::initializer_list<Arg> a_prefix, std::initializer_list<Arg> a_args): opcode(a_opcode), args_size(a_args.size() + a_prefix.size())
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::string("Syntaxic operation: too much args!");
        std::copy(a_prefix.begin(), a_prefix.end(), args);
        std::copy(a_args.begin(), a_args.end(), args + a_prefix.size());
    }

    ContextImpl::ContextImpl(Context* owner) : Context(nullptr), m_owner(owner), m_refcount(0) {
#if defined(__APPLE__) //TODO(ch): this conditions are actually inaccurate. We have to ask system about type of compiler. 
        std::shared_ptr<Aarch64Backend> backend = std::make_shared<Aarch64Backend>();
#elif defined(_WIN32)
        std::shared_ptr<Intel64Backend> backend = std::make_shared<Intel64Backend>();
#endif
        m_bcknd = std::static_pointer_cast<Backend>(backend);
        m_registerAllocator = std::make_shared<RegisterAllocator>(this);
    }

    void ContextImpl::startFunc(const std::string& name, std::initializer_list<IReg*> params)
    {
        if(m_functionsStorage.find(name) != m_functionsStorage.end())
            throw std::string("Function is already registered.");  //TODO(ch): We need good exception class.
        m_currentFunc = m_functionsStorage.emplace(name, FuncImpl::makeWrapper(name, m_owner, params)).first->second;
    }

    void ContextImpl::endFunc()
    {
        FuncImpl* func = getImpl(&m_currentFunc);
        func->endfunc();
        m_currentFunc = Func();
    }

    Func ContextImpl::getFunc(const std::string& name)
    {
        auto found = m_functionsStorage.find(name);
        if(found == m_functionsStorage.end()) 
            throw std::string("Cannot find function.");
        return found->second;
    }
    
    std::string ContextImpl::getPlatformName() const
    {
        return m_bcknd->name();
    }

    void ContextImpl::compileAll()
    {
        const size_t funcAlignment = 16; //TODO(ch): Get precise info from backend.
        size_t totalSize = funcAlignment;
        std::vector<FuncBodyBuf> bodies;
        bodies.reserve(m_functionsStorage.size());
        for(auto par:m_functionsStorage)
        {
            FuncImpl* func = getImpl(&par.second);
            FuncBodyBuf body = m_bcknd->target2Hex(m_bcknd->bytecode2Target(func->getData()));
            size_t bsize = body->size();
            bsize = (bsize / funcAlignment) * funcAlignment + ((bsize % funcAlignment) ? funcAlignment : 0); //TODO(ch): normal alignment expression, mkay?
            totalSize += bsize;
            bodies.push_back(body);
        }
        Allocator* alloc= m_bcknd->getAllocator();
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
