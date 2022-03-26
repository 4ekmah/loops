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

    IReg::IReg() : idx(NOIDX), m_func(nullptr) {}

    IReg::IReg(const IReg& r)
    {
        IReg selfval = newiop(OP_MOV, { r });
        idx = selfval.idx;
        m_func = selfval.m_func;
    }

    IReg::IReg(IReg&& a) : m_func(a.m_func), idx(a.idx) {}

    IReg& IReg::operator=(const IReg& r)
    {
        if (r.m_func == nullptr)
            throw std::string("Cannot find motherfunction in registers.");
        FuncImpl* func = static_cast<FuncImpl*>(m_func);
        func->newiop(OP_MOV, { r }, idx);
        return (*this);
    }

    Arg::Arg() : idx(IReg::NOIDX), m_func(nullptr), tag(EMPTY), value(0) {}
    Arg::Arg(const IReg& r) : idx(r.idx), m_func(r.m_func), tag(r.m_func ? Arg::IREG : Arg::EMPTY), value(0) {}
    Arg::Arg(int64_t a_value) : tag(Arg::ICONST), value(a_value){}

    Func::Func() : impl(nullptr) {}
    Func::Func(const Func& f) : impl(f.impl) { static_cast<FuncImpl*>(impl)->refcount++; }

    Func& Func::operator=(const Func& f)
    {
        FuncImpl* old_p = static_cast<FuncImpl*>(impl);
        if (impl) static_cast<FuncImpl*>(impl)->refcount--;
        impl = f.impl;
        if (impl != nullptr)
            static_cast<FuncImpl*>(impl)->refcount++;
        if (old_p && !old_p->refcount)
            delete old_p;
        return *this;
    }

    Func::~Func()
    {
        if (impl && !(--(static_cast<FuncImpl*>(impl)->refcount)))
            delete impl;
    }

    std::string Func::name() const { return static_cast<FuncImpl*>(impl)->name(); }

    void* Func::ptr() { return static_cast<FuncImpl*>(impl)->ptr(); }
void Func::print_bytecode(std::ostream& out) const { static_cast<FuncImpl*>(impl)->print_bytecode(out); }
void Func::print_target_mnemonics(std::ostream& out) const { static_cast<FuncImpl*>(impl)->print_target_mnemonics(out); }
void Func::print_target_hex(std::ostream& out) const { static_cast<FuncImpl*>(impl)->print_target_hex(out); }

    Func Func::make(Func* a_wrapped)
    {
        Func ret; 
        ret.impl = a_wrapped;
        static_cast<FuncImpl*>(ret.impl)->refcount = 1;
        return ret;
    }
    
    FuncImpl* verify_args(std::initializer_list<Arg> args) //TODO(ch): Can it be FuncImpl's method?
    {
        FuncImpl* func = nullptr;
        for (const Arg& arg : args)
            if (arg.tag == Arg::IREG)
            {
                if (func == nullptr) 
                    func = static_cast<FuncImpl*>(arg.m_func);
                else if(func != static_cast<FuncImpl*>(arg.m_func)) 
                    throw std::string("Registers of different functions as arguments of one instruction.");
            }
        if (func == nullptr)
            throw std::string("Cannot find mother function in registers.");
        return func;
    }

    IReg newiop(int opcode, std::initializer_list<Arg> args)
    {
        return static_cast<IReg&&>(verify_args(args)->newiop(opcode, args));
    }

    IReg newiop(int opcode, int depth, std::initializer_list<Arg> args)
    {
        return verify_args(args)->newiop(opcode,depth,args);
    }

    void newiop_noret(int opcode, ::std::initializer_list<Arg> args)
    {
        verify_args(args)->newiop_noret(opcode, args);
    }

    void newiop_noret(int opcode, int depth, std::initializer_list<Arg> args)
    {
        verify_args(args)->newiop_noret(opcode, depth, args);
    }

    Backend::Backend() : impl(nullptr) {}
    Backend::Backend(Backend* a_p):impl(a_p)
    {
        backend_impl* rcb = dynamic_cast<backend_impl*>(a_p);
        if (rcb == nullptr)
            throw std::string("Backend: wrong backend implementation pointer.");
        rcb->refcount = 1;
    }

    Backend::Backend(const Backend& f) : impl(f.impl) { (static_cast<backend_impl*>(impl))->refcount++; }

    Backend& Backend::operator=(const Backend& f)
    {
        backend_impl* old_p = static_cast<backend_impl*>(impl);
        if ((static_cast<backend_impl*>(impl))) (static_cast<backend_impl*>(impl))->refcount--;
        impl = f.impl;
        if (impl != nullptr)
            (static_cast<backend_impl*>(impl))->refcount++;
        if (old_p && !old_p->refcount)
            delete old_p;
        return *this;
    }

    Backend::~Backend()
    {
        backend_impl* _p = static_cast<backend_impl*>(impl);
        if (_p && !(--(_p->refcount)))
            delete _p;
    }

    void* Backend::compile(Context* ctx, Func* a_func) const { return impl->compile(ctx, a_func); }

    Backend Backend::make_aarch64_compiler()
    {
        return Backend(new aarch64_backend());
    }

    Context::Context() : impl(nullptr) {}
    Context::Context(Backend cmpl) : impl(new ContextImpl(this, cmpl)) { static_cast<ContextImpl*>(impl)->refcount = 1; }
    Context::Context(const Context& f) : impl(f.impl) { static_cast<ContextImpl*>(impl)->refcount++; }

    Context& Context::operator=(const Context& f)
    {
        ContextImpl* old_p = static_cast<ContextImpl*>(impl);
        if (impl) static_cast<ContextImpl*>(impl)->refcount--;
        impl = f.impl;
        if (impl != nullptr)
            static_cast<ContextImpl*>(impl)->refcount++;
        if (old_p && !old_p->refcount)
            delete old_p;
        return *this;
    }

    Context::~Context()
    {
        if (impl && !(--(static_cast<ContextImpl*>(impl)->refcount)))
            delete impl;
    }

    void Context::startfunc(const std::string& name, std::initializer_list<IReg*> params) { static_cast<ContextImpl*>(impl)->startfunc(name, params);    }
    void Context::endfunc() { static_cast<ContextImpl*>(impl)->endfunc(); }
    Func Context::getfunc(const std::string& name) { return static_cast<ContextImpl*>(impl)->getfunc(name); }

    IReg Context::const_(int64_t value)    { return getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->const_(value);    }
    void Context::do_() { getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->do_(); }
    void Context::while_(const IReg& r) { getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->while_(r); }

    void Context::if_(const IReg& r) { getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->if_(r); }
    void Context::endif_() { getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->endif_(); }

    void Context::return_(const IReg& retval) { getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->return_(retval); }
    void Context::return_() { getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->return_(); }

    syntop::syntop(): opcode(OP_NOINIT){}
    syntop::syntop(const syntop& fwho) : opcode(fwho.opcode), args(fwho.args) {}

    syntop::syntop(int a_opcode, const std::vector<Arg>& a_args) : opcode(a_opcode), args(a_args) {}

    syntop::syntop(int a_opcode, std::initializer_list<Arg> a_args): opcode(a_opcode), args(a_args){}

    syntop::syntop(int a_opcode, std::initializer_list<Arg> a_prefix,                                             std::initializer_list<Arg> a_args): opcode(a_opcode)
    {
        args.reserve(a_prefix.size() + a_args.size());
        for(auto arg:a_prefix) args.emplace_back(arg);
        for(auto arg:a_args) args.emplace_back(arg);
    }

    void ContextImpl::startfunc(const std::string& name, std::initializer_list<IReg*> params)
    {
        if(m_functionsStorage.find(name) != m_functionsStorage.end())
            throw std::string("Function is already registered.");  //TODO(ch): We need good exception class.
        m_currentFunc = m_functionsStorage.emplace(name, FuncImpl::makeWrapper(name, m_owner, params)).first->second;
    }

    void ContextImpl::endfunc()
    {
        FuncImpl* func = getImpl(&m_currentFunc);
        func->endfunc();
        m_currentFunc = Func();
    }

    Func ContextImpl::getfunc(const std::string& name)
    {
        auto found = m_functionsStorage.find(name);
        if(found == m_functionsStorage.end()) 
            throw std::string("Cannot find function.");
        return found->second;
    }
}
