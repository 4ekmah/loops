/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "loops/loops.hpp"
#include <map>
#include <stack>
#include <sstream>

namespace loops
{
    //TODO(ch): Actually, I don't like current approach of belonging variables(IRegs) to context. They must belong to function. Otherwise we will have ability to create Var in 1 function, but use it in another.

    class FuncImpl : public Func
    {
        friend class DumpCompiler;
        friend struct IReg;
        friend class ContextImpl;
        friend IReg newiop(Context* ctx, int opcode, ::std::initializer_list<Arg> args);
        friend IReg newiop(int opcode, int depth, ::std::initializer_list<Arg> args);
    public:
        FuncImpl(const std::string& name, Context* ctx, std::initializer_list<IReg*> params);

        std::string name() const { return m_name; }; //TODO(ch): what for we need name here? 
        void call(std::initializer_list<int64_t> args) const;

        size_t refcount; //TODO: I must check if refcounting and impl logic is threadsafe.
        inline ContextImpl* get_context() { return m_context; }
    private:
        struct op
        {
            int opcode;
            size_t retidx;
            std::vector<Arg> args;
            op(const op& fwho);
            op(Context* ctx, int a_opcode, std::initializer_list<Arg> a_args);
            op(Context* ctx, int a_opcode, const std::vector<Arg>& a_args);
            op(const IReg& a_ret, int a_opcode, std::initializer_list<Arg> a_args);
        };

        struct cflowbracket
        {
            enum { DO, DOIF, IF, ELSE };
            int tag;
            size_t position; //TODO(ch): Probably, it's better to use labels instead of positions. They are shift-sustainable. 
            cflowbracket(int a_tag, size_t a_position) : tag(a_tag), position(a_position) {}
        };
        std::stack<cflowbracket> m_cflowstack;

        std::vector<op> m_program; //TODO(ch): Maybe list???
        std::vector<size_t> m_params_idx;
        std::string m_name;
        ContextImpl* m_context; //TODO(ch): I'm not sure it's needed.
    };

    class RCCompiler : public Compiler
    {
    public:
        size_t refcount;
        RCCompiler() : Compiler(), refcount(0) {}
    };

    class DumpCompiler : public RCCompiler
    {
    public:
        DumpCompiler() {}
        virtual void* compile(Context* ctx, Func a_func) const override final;
    private:
        void print_ireg(::std::ostringstream& str, const IReg& ireg) const;
        void print_arg(::std::ostringstream& str, const Arg& arg) const;
        void print_instruction(::std::ostringstream& str, const FuncImpl::op& operation, size_t innum) const;
        mutable std::vector<std::string> m_listings;
    };

    class ContextImpl : public Context
    {
    private:
        std::map<std::string, Func> m_functionsStorage; //TODO(ch): unordered map?
        Func m_currentFunc;
        Context* m_owner;
        size_t m_nextidx;
        Compiler m_cmpl;
    public:
        ContextImpl(Context* owner, Compiler cmpl) : m_owner(owner), m_cmpl(cmpl) {}
        inline size_t provideIdx() { return m_nextidx++; }
        void startfunc(const std::string& name, std::initializer_list<IReg*> params);
        void endfunc(const IReg& retval);
        Func getfunc(const std::string& name);

        IReg const_(int64_t value);

        void do_();
        //void do_if_(const IReg& r);
        void while_(const IReg& r);
        //void break_();
        //void continue_();
        void if_(const IReg& r);
        //void elif_(const IReg& r);
        //void else_();
        void endif_();

        int refcount; //TODO: What it is for? Is it for Context smartpointing? 
        inline Func get_current_func() { return m_currentFunc; }
        inline Compiler get_compiler() const { return m_cmpl; }
        inline Context* get_owner() const { return m_owner; }
    };

    inline Func* _getImpl(const Func& wrapper) { return wrapper.impl; };
    inline FuncImpl* getImpl(const Func& wrapper) { return static_cast<FuncImpl*>(_getImpl(wrapper)); }
    inline Context* _getImpl(const Context& wrapper) { return wrapper.impl; };
    inline ContextImpl* getImpl(const Context& wrapper) { return static_cast<ContextImpl*>(_getImpl(wrapper)); }

    IReg::IReg() : idx(0), ctx(nullptr) {}
    IReg::IReg(Context* a_ctx) : idx(a_ctx->provideIdx()), ctx(a_ctx) {}
    IReg::IReg(const IReg& r)
    {
        IReg selfval = newiop(OP_MOV, { r });
        idx = selfval.idx;
        ctx = selfval.ctx;
    }

    IReg::IReg(IReg&& a) : ctx(a.ctx), idx(a.idx) {}

    IReg& IReg::operator=(const IReg& r)
    {
        if (r.ctx == nullptr)
            throw std::string("Cannot find ctx in registers.");
        FuncImpl* func = getImpl(getImpl(*ctx)->get_current_func());
        func->m_program.emplace_back(*this, (int)OP_MOV, std::initializer_list<Arg>({ r }));
        return (*this);
    }

    IReg ireg_hidden_constructor(Context* a_ctx, size_t a_idx)
    {
        IReg ret;
        ret.ctx = a_ctx;
        ret.idx = a_idx;
        return static_cast<IReg&&>(ret);
    }

    Arg::Arg() : idx(0), ctx(nullptr), tag(EMPTY), value(0) {}
    Arg::Arg(const IReg& r) : idx(r.idx), ctx(r.ctx), tag(r.ctx ? Arg::IREG : Arg::EMPTY), value(0) {}

    Func::Func() : impl(nullptr) {}
    Func::Func(const std::string& name, Context* ctx, std::initializer_list<IReg*> params) : impl(new FuncImpl(name, ctx, params)) { static_cast<FuncImpl*>(impl)->refcount = 1; }
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
    //void Func::call(std::initializer_list<int64_t> args) const { p->call(args); }
    void* Func::ptr()
    {
        ContextImpl* ctx = static_cast<FuncImpl*>(impl)->get_context();
        return ctx->get_compiler().compile(ctx->get_owner(), *this); //TODO(ch): Too much get's. Looks like wormholes interweaving.
    }

    size_t Context::provideIdx() { return static_cast<ContextImpl*>(impl)->provideIdx(); }

    IReg newiop(Context* ctx, int opcode, ::std::initializer_list<Arg> args)
    {
        FuncImpl* func = getImpl(getImpl(*ctx)->get_current_func());
        func->m_program.emplace_back(ctx, opcode, args);
        return ireg_hidden_constructor(ctx, func->m_program.back().retidx);
    }

    IReg newiop(int opcode, std::initializer_list<Arg> args)
    {
        Context* ctx = nullptr;
        for (const Arg& arg : args)
            if (arg.tag == Arg::IREG)
            {
                ctx = arg.ctx;
                break;
            }
        if(ctx == nullptr) 
            throw std::string("Cannot find ctx in registers.");
        return static_cast<IReg&&>(newiop(ctx, opcode, args));
    }

    IReg newiop(int opcode, int depth, std::initializer_list<Arg> args)
    {
        std::vector<Arg> interargs(args.size() + 1, Arg());
        std::copy(args.begin(), args.end(), interargs.begin() + 1);
        interargs[0].tag = Arg::ICONST;
        interargs[0].value = depth;
        Context* ctx = nullptr;
        for (const Arg& arg : args)
            if (arg.tag == Arg::IREG)
            {
                ctx = arg.ctx;
                break;
            }
        if (ctx == nullptr)
            throw std::string("Cannot find ctx in registers.");
        FuncImpl* func = getImpl(getImpl(*ctx)->get_current_func());
        func->m_program.emplace_back(ctx, opcode, interargs);
        return ireg_hidden_constructor(ctx, func->m_program.back().retidx);
    }

    Compiler::Compiler() : p(nullptr) {}
    Compiler::Compiler(const Compiler& f) : p(f.p) { (static_cast<RCCompiler*>(p))->refcount++; }

    Compiler& Compiler::operator=(const Compiler& f)
    {
        RCCompiler* old_p = static_cast<RCCompiler*>(p);
        if ((static_cast<RCCompiler*>(p))) (static_cast<RCCompiler*>(p))->refcount--;
        p = f.p;
        if (p != nullptr)
            (static_cast<RCCompiler*>(p))->refcount++;
        if (old_p && !old_p->refcount)
            delete old_p;
        return *this;
    }

    Compiler::~Compiler()
    {
        RCCompiler* _p = static_cast<RCCompiler*>(p);
        if (_p && !(--(_p->refcount)))
            delete _p;
    }

    void* Compiler::compile(Context* ctx, Func a_func) const { return p->compile(ctx, a_func); }

    Compiler Compiler::make_virtual_dump()
    {
        Compiler res;
        DumpCompiler* content = new DumpCompiler();
        content->refcount = 1;
        res.p = content;
        return res;
    } ;

    Context::Context() : impl(nullptr) {}
    Context::Context(Compiler cmpl) : impl(new ContextImpl(this, cmpl)) { static_cast<ContextImpl*>(impl)->refcount = 1; }
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
    void Context::endfunc(const IReg& retval) { static_cast<ContextImpl*>(impl)->endfunc(retval); }
    Func Context::getfunc(const std::string& name) { return static_cast<ContextImpl*>(impl)->getfunc(name); }

    IReg Context::const_(int64_t value)    { return static_cast<ContextImpl*>(impl)->const_(value);    }
    void Context::do_() { static_cast<ContextImpl*>(impl)->do_(); }
    void Context::while_(const IReg& r) { static_cast<ContextImpl*>(impl)->while_(r); }

    void Context::if_(const IReg& r) { static_cast<ContextImpl*>(impl)->if_(r); }
    void Context::endif_() { static_cast<ContextImpl*>(impl)->endif_(); }

    FuncImpl::FuncImpl(const std::string& name, Context* ctx, std::initializer_list<IReg*> params): refcount(0), m_name(name), m_context(getImpl(*ctx)) //TODO(ch): Should we check if ctx == nullptr?
    {
        m_params_idx.reserve(params.size());
        for (IReg* argreg : params)
        {
            argreg->ctx = ctx;
            argreg->idx = m_context->provideIdx(); //TODO(ch): It's hard to pretend situation when idx isn't 0 here, but you'd better check.
            m_params_idx.emplace_back(argreg->idx);
        }
    }

    FuncImpl::op::op(const op& fwho) : opcode(fwho.opcode), retidx(fwho.retidx), args(fwho.args) {}

    FuncImpl::op::op(Context* ctx, int a_opcode, std::initializer_list<Arg> a_args) : opcode(a_opcode), retidx(getImpl(*ctx)->provideIdx())
    {
        args.reserve(a_args.size());
        for (auto arg : a_args)
        {
            args.push_back(arg);
        }
    }
    
    FuncImpl::op::op(Context* ctx, int a_opcode, const std::vector<Arg>& a_args) : opcode(a_opcode), retidx(getImpl(*ctx)->provideIdx()), args(a_args) {}
    
    FuncImpl::op::op(const IReg& a_ret, int a_opcode, std::initializer_list<Arg> a_args) : opcode(a_opcode)
    {
        retidx = a_ret.idx;
        args.reserve(a_args.size());
        for (auto arg : a_args)
        {
            args.push_back(arg);
        }
    }

    void* DumpCompiler::compile(Context* ctx, Func a_func) const
    {
        FuncImpl* funcImpl = getImpl(a_func);
        ::std::string listing;
        {
            ::std::ostringstream str(listing, ::std::ios::out);
            str << funcImpl->name() << "(";
            for (size_t argnum = 0; argnum + 1 < funcImpl->m_params_idx.size(); argnum++)
            {
                print_ireg(str, ireg_hidden_constructor(ctx, funcImpl->m_params_idx[argnum]));
                str << ", ";
            }
            if (funcImpl->m_params_idx.size())
                print_ireg(str, ireg_hidden_constructor(ctx, funcImpl->m_params_idx.back()));
            str << ")" << std::endl;
            for (size_t innum = 0; innum < funcImpl->m_program.size(); innum++)
                print_instruction(str, funcImpl->m_program[innum], innum);
            str.flush();
            listing = str.str();
        }
        m_listings.push_back(listing);
        return &(m_listings.back());
    }

    void DumpCompiler::print_ireg(::std::ostringstream& str, const IReg& ireg) const
    {
        str << "i" << ireg.idx;
    }

    void DumpCompiler::print_arg(::std::ostringstream& str, const Arg& arg) const
    {
        switch (arg.tag)
        {
        case Arg::IREG: str << "i" << arg.idx; break;
        case Arg::ICONST: str << arg.value; break;
        default:
            throw std::string("Undefined argument type.");
        };
    }

    void DumpCompiler::print_instruction(::std::ostringstream& str, const FuncImpl::op& operation, size_t innum) const
    {
        str << "    " << innum << " : ";
        print_ireg(str, ireg_hidden_constructor(nullptr, operation.retidx)); //TODO(ch): nullptr? Ugly
        str << " = ";
        switch (operation.opcode)
        {
        case OP_LOAD: str << "load "; break;
        case OP_STORE: str << "store "; break;
        case OP_MOV: str << "mov "; break;
        case OP_CMP_LT: str << "cmp_lt "; break;
        case OP_CMP_LE : str << "cmp_le "; break;
        case OP_CMP_GT: str << "cmp_gt "; break;
        case OP_AUG_ADD : str << "aug_add "; break;
        case OP_JNE : str << "jne "; break;
        case OP_JZ : str << "jz "; break;
        case OP_RET : str << "ret "; break;
        default:
            throw std::string("Undefined operation type.");
        }

        for (size_t argnum = 0; argnum + 1 < operation.args.size(); argnum++)
        {
            print_arg(str, operation.args[argnum]);
            str << ", ";
        }
        if (operation.args.size())
            print_arg(str, operation.args.back());
        str << std::endl;
    }

    void ContextImpl::startfunc(const std::string& name, std::initializer_list<IReg*> params)
    {
        m_nextidx = 1;
        if(m_functionsStorage.find(name) != m_functionsStorage.end())
            throw std::string("Function is already registered.");  //TODO(ch): We need good exception class.
        m_currentFunc = m_functionsStorage.emplace(name, Func(name, m_owner, params)).first->second;
    }

    void ContextImpl::endfunc(const IReg& retval)
    {
        newiop(m_owner, OP_RET, { retval });
        m_currentFunc = Func();
    }

    Func ContextImpl::getfunc(const std::string& name)
    {
        auto found = m_functionsStorage.find(name);
        if(found == m_functionsStorage.end()) 
            throw std::string("Cannot find function.");
        return found->second;
    }

    IReg ContextImpl::const_(int64_t value)
    {
        Arg initval;
        initval.tag = Arg::ICONST;
        initval.value = value;
        return static_cast<IReg&&>(newiop(m_owner, OP_MOV, {initval}));
    }

    void ContextImpl::do_()
    {
        FuncImpl* func = getImpl(m_currentFunc);
        func->m_cflowstack.push(FuncImpl::cflowbracket(FuncImpl::cflowbracket::DO, func->m_program.size()));
    }
    
    void ContextImpl::while_(const IReg& r)
    {
        FuncImpl* func = getImpl(m_currentFunc);
        if (func->m_cflowstack.size() == 0)
            throw std::string("Unclosed control flow bracket: there is no \"do\" for \"while\".");
        auto bracket = func->m_cflowstack.top();
        if (bracket.tag != FuncImpl::cflowbracket::DO)
            throw std::string("Control flow bracket error: expected corresponding \"do\" for \"while\".");
        func->m_cflowstack.pop();
        Arg loopstart;
        loopstart.tag = Arg::ICONST;
        loopstart.value = bracket.position;
        newiop(m_owner, OP_JNE, { loopstart });
    }
    void ContextImpl::if_(const IReg& r)
    {
        FuncImpl* func = getImpl(m_currentFunc);
        func->m_cflowstack.push(FuncImpl::cflowbracket(FuncImpl::cflowbracket::IF, func->m_program.size()));
        Arg ifstart;
        ifstart.tag = Arg::ICONST;
        ifstart.value = 0;
        newiop(m_owner, OP_JZ, { ifstart });
    }
    void ContextImpl::endif_() //TODO(ch): Corresponding tag also can be ELSE or ELIF.
    {
        FuncImpl* func = getImpl(m_currentFunc);
        if (func->m_cflowstack.size() == 0)
            throw std::string("Unclosed control flow bracket: there is no \"if\", \"elif\" or \"else\", for \"endif\".");
        auto bracket = func->m_cflowstack.top();
        if (bracket.tag != FuncImpl::cflowbracket::IF) 
            throw std::string("Control flow bracket error: expected corresponding \"if\", \"elif\" or \"else\" for \"endif\".");
        func->m_cflowstack.pop();
        size_t posnext = func->m_program.size();
        if (bracket.position >= posnext)
            throw std::string("\"If\" internal error: wrong branch start address");
        FuncImpl::op& ifop = func->m_program[bracket.position];
        if (ifop.args.size() != 1)
            throw std::string("\"If\" internal error: wrong JZ command format");
        ifop.args[0].value = func->m_program.size();
    }
}
