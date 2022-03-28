/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "loops/loops.hpp"
#include "aarch64.hpp"
#include "backend.hpp"
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

    IReg::IReg(IReg&& a) : func(a.func), idx(a.idx) {}

    IReg& IReg::operator=(const IReg& r)
    {
        if (r.func == nullptr)
            throw std::string("Cannot find motherfunction in registers.");
        FuncImpl* funcimpl = static_cast<FuncImpl*>(func);
        funcimpl->newiop(OP_MOV, { r }, idx);
        return (*this);
    }

    Arg::Arg() : idx(IReg::NOIDX), func(nullptr), tag(EMPTY), value(0) {}
    Arg::Arg(const IReg& r) : idx(r.idx), func(r.func), tag(r.func ? Arg::IREG : Arg::EMPTY), value(0) {}
    Arg::Arg(int64_t a_value) : tag(Arg::ICONST), value(a_value){}

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
void Func::printAssembly(std::ostream& out) const { static_cast<FuncImpl*>(impl)->printAssembly(out); }
void Func::printTargetHex(std::ostream& out) const { static_cast<FuncImpl*>(impl)->printTargetHex(out); }

    Func Func::make(Func* a_wrapped)
    {
        Func ret; 
        ret.impl = a_wrapped;
        static_cast<FuncImpl*>(ret.impl)->m_refcount = 1;
        return ret;
    }
    
    FuncImpl* verifyArgs(std::initializer_list<Arg> args) //TODO(ch): Can it be FuncImpl's method?
    {
        FuncImpl* func = nullptr;
        for (const Arg& arg : args)
            if (arg.tag == Arg::IREG)
            {
                if (func == nullptr) 
                    func = static_cast<FuncImpl*>(arg.func);
                else if(func != static_cast<FuncImpl*>(arg.func)) 
                    throw std::string("Registers of different functions as arguments of one instruction.");
            }
        if (func == nullptr)
            throw std::string("Cannot find mother function in registers.");
        return func;
    }

    IReg newiop(int opcode, std::initializer_list<Arg> args)
    {
        return static_cast<IReg&&>(verifyArgs(args)->newiop(opcode, args));
    }

    IReg newiop(int opcode, int depth, std::initializer_list<Arg> args)
    {
        return verifyArgs(args)->newiop(opcode,depth,args);
    }

    void newiopNoret(int opcode, ::std::initializer_list<Arg> args)
    {
        verifyArgs(args)->newiopNoret(opcode, args);
    }

    void newiopNoret(int opcode, int depth, std::initializer_list<Arg> args)
    {
        verifyArgs(args)->newiopNoret(opcode, depth, args);
    }
    
    void newiopAug(int opcode, ::std::initializer_list<Arg> args)
    {
        return newiopNoret(opcode, args);
    }

    Backend::Backend() : impl(nullptr) {}
    Backend::Backend(Backend* a_p):impl(a_p)
    {
        BackendImpl* rcb = dynamic_cast<BackendImpl*>(a_p);
        if (rcb == nullptr)
            throw std::string("Backend: wrong backend implementation pointer.");
        rcb->m_refcount = 1;
    }

    Backend::Backend(const Backend& f) : impl(f.impl) { (static_cast<BackendImpl*>(impl))->m_refcount++; }

    Backend& Backend::operator=(const Backend& f)
    {
        BackendImpl* old_p = static_cast<BackendImpl*>(impl);
        if ((static_cast<BackendImpl*>(impl))) (static_cast<BackendImpl*>(impl))->m_refcount--;
        impl = f.impl;
        if (impl != nullptr)
            (static_cast<BackendImpl*>(impl))->m_refcount++;
        if (old_p && !old_p->m_refcount)
            delete old_p;
        return *this;
    }

    Backend::~Backend()
    {
        BackendImpl* _p = static_cast<BackendImpl*>(impl);
        if (_p && !(--(_p->m_refcount)))
            delete _p;
    }

    void* Backend::compile(Context* ctx, Func* a_func) const { return impl->compile(ctx, a_func); }

    Backend Backend::makeAarch64Compiler()
    {
        return Backend(new Aarch64Backend());
    }

    Context::Context() : impl(nullptr) {}
    Context::Context(Backend cmpl) : impl(new ContextImpl(this, cmpl)) { static_cast<ContextImpl*>(impl)->m_refcount = 1; }
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

    void Context::if_(const IReg& r) { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->if_(r); }
    void Context::endif_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->endif_(); }

    void Context::return_(const IReg& retval) { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->return_(retval); }
    void Context::return_() { getImpl(static_cast<ContextImpl*>(impl)->getCurrentFunc())->return_(); }

    Syntop::Syntop(): opcode(OP_NOINIT), args_size(0){}
    Syntop::Syntop(const Syntop& fwho) : opcode(fwho.opcode), args_size(fwho.args_size)
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

    Syntop::Syntop(int a_opcode, std::initializer_list<Arg> a_prefix,                                             std::initializer_list<Arg> a_args): opcode(a_opcode), args_size(a_args.size() + a_prefix.size())
    {
        if(args_size > SYNTOP_ARGS_MAX)
            throw std::string("Syntaxic operation: too much args!");
        std::copy(a_prefix.begin(), a_prefix.end(), args);
        std::copy(a_args.begin(), a_args.end(), args + a_prefix.size());
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
}
