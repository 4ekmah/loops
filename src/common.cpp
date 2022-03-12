/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "loops/loops.hpp"
#include <map>
#include <stack>
#include <sstream>
#include <iomanip>

namespace loops
{
    typedef size_t IRegInternal;
    class ContextImpl;
    class FuncImpl : public Func
    {
    public:
        FuncImpl(const std::string& name, Context* ctx, std::initializer_list<IReg*> params);
        static Func makeWrapper(const std::string& name, Context* ctx, std::initializer_list<IReg*> params);

        std::string name() const { return m_name; }; //TODO(ch): what for we need name here? 
        void call(std::initializer_list<int64_t> args) const;
        void* ptr();

        size_t refcount; //TODO: I must check if refcounting and impl logic is threadsafe.
        inline size_t provide_idx() { return m_nextidx++; }
        size_t append_label();
        static const size_t EMPTYLABEL;

        inline IReg newiop(int opcode, ::std::initializer_list<Arg> args);
        inline IReg newiop(int opcode, int depth, ::std::initializer_list<Arg> args);
        inline IReg newiop(int opcode, ::std::initializer_list<Arg> args, IRegInternal retreg);
        inline void newiop_noret(int opcode, ::std::initializer_list<Arg> args);
        inline void newiop_noret(int opcode, int depth, std::initializer_list<Arg> args);

        void endfunc();

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
        void return_(const IReg& retval);
        void return_();

        struct op
        {
            int opcode;
            IRegInternal retidx;
            std::vector<Arg> args;
            op(const op& fwho);
            op(FuncImpl* func, int a_opcode, std::initializer_list<Arg> a_args);
            op(FuncImpl* func, int a_opcode, const std::vector<Arg>& a_args);
            op(IRegInternal a_ret, int a_opcode, std::initializer_list<Arg> a_args);
            op(IRegInternal a_ret, int a_opcode, const std::vector<Arg>& a_args);
        };

        inline const std::vector<IRegInternal>& getParams() const { return m_params_idx; }
        inline const std::vector<op>& getProgram() const { return m_program; }
    private:

        struct cflowbracket
        {
            enum { DO, DOIF, IF, ELSE };
            size_t m_tag;
            size_t m_label_or_pos;
            cflowbracket(size_t a_tag, size_t a_label_or_pos) : m_tag(a_tag), m_label_or_pos(a_label_or_pos) {}
        };
        std::stack<cflowbracket> m_cflowstack;

        std::vector<op> m_program;
        std::vector<IRegInternal> m_params_idx;
        IRegInternal m_return_idx;
        std::vector<size_t> m_return_positions;
        std::string m_name;
        ContextImpl* m_context;
        size_t m_nextidx;
        size_t m_nextlabelidx;

        inline std::vector<Arg> depthed_args(int depth, std::initializer_list<Arg> args);
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
        virtual void* compile(Context* a_ctx, Func* a_func) const override final;
    private:
        void print_ireg(::std::ostream& str, IRegInternal idx) const;
        void print_arg(::std::ostream& str, const Arg& arg) const;
        void print_instruction(::std::ostream& str, const FuncImpl::op& operation, size_t innum) const;
        static std::string type_suffixes[];
        mutable std::vector<std::string> m_listings;
    };

    class ContextImpl : public Context
    {
    private:
        std::map<std::string, Func> m_functionsStorage; //TODO(ch): unordered map?
        Func m_currentFunc;
        Context* m_owner;
        Compiler m_cmpl;
    public:
        ContextImpl(Context* owner, Compiler cmpl) : m_owner(owner), m_cmpl(cmpl) {}
        void startfunc(const std::string& name, std::initializer_list<IReg*> params);
        void endfunc();
        Func getfunc(const std::string& name);

        int refcount;
        inline Func* get_current_func() { return &m_currentFunc; }
        inline Compiler get_compiler() const { return m_cmpl; }
        inline Context* get_owner() const { return m_owner; }
    };

    inline Func* _getImpl(Func* wrapper) { return wrapper->impl; };
    inline FuncImpl* getImpl(Func* wrapper) 
    {
        if (!wrapper)
            throw std::string("Null context pointer.");
        return static_cast<FuncImpl*>(_getImpl(wrapper)); 
    }
    inline Context* _getImpl(Context* wrapper) { return wrapper->impl; };
    inline ContextImpl* getImpl(Context* wrapper) 
    {
        if(!wrapper) 
            throw std::string("Null context pointer.");
        return static_cast<ContextImpl*>(_getImpl(wrapper));
    }

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

    IReg ireg_hidden_constructor(Func* a_func, size_t a_idx)
    {
        IReg ret;
        ret.m_func = a_func;
        ret.idx = a_idx;
        return static_cast<IReg&&>(ret);
    }

    Arg::Arg() : idx(IReg::NOIDX), m_func(nullptr), tag(EMPTY), value(0) {}
    Arg::Arg(const IReg& r) : idx(r.idx), m_func(r.m_func), tag(r.m_func ? Arg::IREG : Arg::EMPTY), value(0) {}

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

    void* Compiler::compile(Context* ctx, Func* a_func) const { return p->compile(ctx, a_func); }

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
    void Context::endfunc() { static_cast<ContextImpl*>(impl)->endfunc(); }
    Func Context::getfunc(const std::string& name) { return static_cast<ContextImpl*>(impl)->getfunc(name); }

    IReg Context::const_(int64_t value)    { return getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->const_(value);    }
    void Context::do_() { getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->do_(); }
    void Context::while_(const IReg& r) { getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->while_(r); }

    void Context::if_(const IReg& r) { getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->if_(r); }
    void Context::endif_() { getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->endif_(); }

    void Context::return_(const IReg& retval) { getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->return_(retval); }
    void Context::return_() { getImpl(static_cast<ContextImpl*>(impl)->get_current_func())->return_(); }

    FuncImpl::FuncImpl(const std::string& name, Context* ctx, std::initializer_list<IReg*> params) : refcount(0)
        , m_name(name)
        , m_nextidx(0)
        , m_nextlabelidx(0)
        , m_context(getImpl(ctx))
        , m_return_idx(IReg::NOIDX)
    {
        m_params_idx.reserve(params.size());
        for (IReg* parreg : params)
        {
            if(parreg->m_func != nullptr || parreg->idx != IReg::NOIDX) 
                throw std::string("Parameter index is already initilized in some other function");
            parreg->m_func = this;
            parreg->idx = provide_idx();
            m_params_idx.emplace_back(parreg->idx);
        }
    }

    Func FuncImpl::makeWrapper(const std::string& name, Context* ctx, std::initializer_list<IReg*> params)
    {
        return Func::make(new FuncImpl(name, ctx, params));
    }
    
    void* FuncImpl::ptr()
    {
        return m_context->get_compiler().compile(m_context, this);
    }

    size_t FuncImpl::append_label()
    {
        size_t label_idx = m_nextlabelidx++;
        Arg label_arg;
        label_arg.tag = Arg::ICONST;
        label_arg.value = label_idx;
        newiop_noret(OP_LABEL, { label_arg });
        return label_idx;
    }
    const size_t FuncImpl::EMPTYLABEL = static_cast<size_t>(-1);

    inline IReg FuncImpl::newiop(int opcode, ::std::initializer_list<Arg> args)
    {
        m_program.emplace_back(this, opcode, args);
        return ireg_hidden_constructor(this, m_program.back().retidx);
    }

    inline IReg FuncImpl::newiop(int opcode, int depth, ::std::initializer_list<Arg> args)
    {
        m_program.emplace_back(this, opcode, depthed_args(depth, args));
        return ireg_hidden_constructor(this, m_program.back().retidx);
    }

    inline IReg FuncImpl::newiop(int opcode, ::std::initializer_list<Arg> args, IRegInternal retreg)
    {
        m_program.emplace_back(retreg, opcode, args);
        return ireg_hidden_constructor(this, m_program.back().retidx);
    }

    inline void FuncImpl::newiop_noret(int opcode, ::std::initializer_list<Arg> args)
    {
        m_program.emplace_back(IReg::NOIDX, opcode, args);
    }

    inline void FuncImpl::newiop_noret(int opcode, int depth, std::initializer_list<Arg> args)
    {
        m_program.emplace_back(IReg::NOIDX, opcode, depthed_args(depth, args));
    }

    void FuncImpl::endfunc()
    {
        //TODO(ch): what if return are not on all control pathes(this problem exists for register returns only)??? Think out.
        //          I think, it can be effectively soluted only after deletition of after-jump-silent tails a-la jmp end; mov ..code-without-jumps...; end:
        //          After this we just need to check last operation before end mark. it must be mov to return register.
        //Handle situation, when return is just before end of function (reasonable)
        if (m_return_positions.size() && m_return_positions.back() + 1 == m_program.size())
        {
            m_program.pop_back();
            m_return_positions.pop_back();
        }
        if (m_return_positions.size())
        {
            size_t endfunclabel = append_label();
            for (size_t retpos : m_return_positions)
            {
                FuncImpl::op& jmpop = m_program[retpos];
                if (jmpop.args.size() != 1 || jmpop.args[0].tag != Arg::ICONST)
                    throw std::string("\"If\" internal error: wrong JMP command format");
                jmpop.args[0].value = endfunclabel;
            }
        }
        m_return_positions.clear();
        newiop_noret(OP_RET, {});
    }

    IReg FuncImpl::const_(int64_t value)
    {
        Arg initval;
        initval.tag = Arg::ICONST;
        initval.value = value;
        return static_cast<IReg&&>(newiop(OP_MOV, { initval }));
    }

    void FuncImpl::do_()
    {
        m_cflowstack.push(FuncImpl::cflowbracket(FuncImpl::cflowbracket::DO, append_label()));
    }

    void FuncImpl::while_(const IReg& r)
    {
        if (m_cflowstack.size() == 0)
            throw std::string("Unclosed control flow bracket: there is no \"do\" for \"while\".");
        auto bracket = m_cflowstack.top();
        if (bracket.m_tag != FuncImpl::cflowbracket::DO)
            throw std::string("Control flow bracket error: expected corresponding \"do\" for \"while\".");
        m_cflowstack.pop();
        Arg loopstart;
        loopstart.tag = Arg::ICONST;
        loopstart.value = bracket.m_label_or_pos;
        newiop_noret(OP_JNE, { r, loopstart });
    }
    void FuncImpl::if_(const IReg& r)
    {
        m_cflowstack.push(FuncImpl::cflowbracket(FuncImpl::cflowbracket::IF, m_program.size()));
        Arg ifstart;
        ifstart.tag = Arg::ICONST;
        ifstart.value = 0;
        newiop_noret(OP_JZ, { r, ifstart });
    }
    void FuncImpl::endif_() //TODO(ch): Corresponding tag also can be ELSE or ELIF.
    {
        if (m_cflowstack.size() == 0)
            throw std::string("Unclosed control flow bracket: there is no \"if\", \"elif\" or \"else\", for \"endif\".");
        auto bracket = m_cflowstack.top();
        if (bracket.m_tag != FuncImpl::cflowbracket::IF)
            throw std::string("Control flow bracket error: expected corresponding \"if\", \"elif\" or \"else\" for \"endif\".");
        m_cflowstack.pop();
        size_t posnext = m_program.size();
        if (bracket.m_label_or_pos >= posnext)
            throw std::string("\"If\" internal error: wrong branch start address");
        FuncImpl::op& ifop = m_program[bracket.m_label_or_pos];
        if (ifop.args.size() != 2 || ifop.args[1].tag != Arg::ICONST)
            throw std::string("\"If\" internal error: wrong JZ command format");
        ifop.args[1].value = append_label();
    }

    void FuncImpl::return_(const IReg& retval)
    {
        if (m_return_positions.size() && m_return_idx == IReg::NOIDX)
            throw std::string("Mixed return types.");
        IRegInternal ret_idx = m_return_idx;
        ret_idx = (ret_idx == IReg::NOIDX) ? provide_idx() : ret_idx;
        m_program.emplace_back(ret_idx, (int)OP_MOV, std::initializer_list<Arg>({ retval }));
        Arg funcfinish;
        funcfinish.tag = Arg::ICONST;
        funcfinish.value = 0;
        m_return_positions.push_back(m_program.size());
        newiop_noret(OP_JMP, { funcfinish });
    }

    void FuncImpl::return_()
    {
        if (m_return_positions.size() && m_return_idx != IReg::NOIDX)
            throw std::string("Mixed return types.");
        Arg funcfinish;
        funcfinish.tag = Arg::ICONST;
        funcfinish.value = 0;
        m_return_positions.push_back(m_program.size());
        newiop_noret(OP_JMP, { funcfinish });
    }

    inline std::vector<Arg> FuncImpl::depthed_args(int depth, std::initializer_list<Arg> args)
    {
        std::vector<Arg> interargs(args.size() + 1, Arg());
        std::copy(args.begin(), args.end(), interargs.begin() + 1);
        interargs[0].tag = Arg::ICONST;
        interargs[0].value = depth;
        return interargs;
    }

    FuncImpl::op::op(const op& fwho) : opcode(fwho.opcode), retidx(fwho.retidx), args(fwho.args) {}

    FuncImpl::op::op(FuncImpl* func, int a_opcode, std::initializer_list<Arg> a_args) : opcode(a_opcode), retidx(func->provide_idx())
    {
        args.reserve(a_args.size());
        for (auto arg : a_args)
        {
            args.push_back(arg);
        }
    }
    
    FuncImpl::op::op(FuncImpl* func, int a_opcode, const std::vector<Arg>& a_args) : opcode(a_opcode), retidx(func->provide_idx()), args(a_args) {}
    
    FuncImpl::op::op(IRegInternal a_ret, int a_opcode, std::initializer_list<Arg> a_args) : opcode(a_opcode), retidx(a_ret)
    {
        args.reserve(a_args.size());
        for (auto arg : a_args)
        {
            args.push_back(arg);
        }
    }

    FuncImpl::op::op(IRegInternal a_ret, int a_opcode, const std::vector<Arg>& a_args) : opcode(a_opcode), retidx(a_ret), args(a_args) {}

    void* DumpCompiler::compile(Context* a_ctx, Func* a_func) const
    {
        FuncImpl* func = static_cast<FuncImpl*>(a_func);
        ::std::string listing;
        {
            ::std::ostringstream str(listing, ::std::ios::out);
            str << func->name() << "(";
            for (size_t argnum = 0; argnum + 1 < func->getParams().size(); argnum++)
            {
                print_ireg(str, func->getParams()[argnum]);
                str << ", ";
            }
            if (func->getParams().size())
                print_ireg(str, func->getParams().back());
            str << ")" << std::endl;
            for (size_t innum = 0; innum < func->getProgram().size(); innum++)
                print_instruction(str, func->getProgram()[innum], innum);
            str.flush();
            listing = str.str();
        }
        m_listings.push_back(listing);
        return &(m_listings.back());
    }

    std::string DumpCompiler::type_suffixes[] = {
        "u8",
        "i8",
        "u16",
        "i16",
        "u32",
        "i32",
        "u64",
        "i64",
        "fp16",
        "bf16",
        "fp32",
        "fp64",
    };

    void DumpCompiler::print_ireg(::std::ostream& str, IRegInternal idx) const
    {
        str << "i" << idx;
    }

    void DumpCompiler::print_arg(::std::ostream& str, const Arg& arg) const
    {
        switch (arg.tag)
        {
        case Arg::IREG: print_ireg(str, arg.idx); break;
        case Arg::ICONST: str << arg.value; break;
        default:
            throw std::string("Undefined argument type.");
        };
    }

    void DumpCompiler::print_instruction(::std::ostream& str, const FuncImpl::op& operation, size_t innum) const
    {
        str << "   " << std::setw(6) << innum << " : ";
        switch (operation.opcode)
        {
        case OP_LOAD:
            if(operation.args.size() < 1 || operation.args[0].tag != Arg::ICONST)
                throw std::string("Depthed operation don't have depth.");
            str << "load." << type_suffixes[operation.args[0].value]<<" ";
            break;
        case OP_STORE:
            if (operation.args.size() < 1 || operation.args[0].tag != Arg::ICONST)
                throw std::string("Depthed operation don't have depth.");
            str << "store." << type_suffixes[operation.args[0].value] << " ";
            break;
        case OP_MOV: str << "mov "; break;
        case OP_CMP_LT: str << "cmp_lt "; break;
        case OP_CMP_LE : str << "cmp_le "; break;
        case OP_CMP_GT: str << "cmp_gt "; break;
        case OP_AUG_ADD : str << "aug_add "; break;
        case OP_JNE : str << "jne "; break;
        case OP_JZ : str << "jz "; break;
        case OP_RET : str << "ret "; break;
        case OP_LABEL : str << "label "; break;
        default:
            throw std::string("Undefined operation type.");
        }

        if (operation.retidx != IReg::NOIDX)
        {
            print_ireg(str, operation.retidx);
            if (operation.args.size())
                str << ", ";
        }

        size_t argnum = 0;
        if (operation.opcode == OP_STORE ||
            operation.opcode == OP_LOAD)
            argnum = 1;
            
        for (; argnum + 1 < operation.args.size(); argnum++)
        {
            print_arg(str, operation.args[argnum]);
            str << ", ";
        }
        if (operation.args.size())
            print_arg(str, operation.args.back());
        if (operation.opcode == OP_LABEL) 
            str << ":";

        str << std::endl;
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
