/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "loops/loops.hpp"
#include "common.hpp"
#include "aarch64.hpp"
#include <map>
#include <stack>
#include <sstream>
#include <iomanip>

namespace loops
{
    class DumpCompiler : public RCCompiler
    {
    public:
        DumpCompiler() : m_aarch64(false),m_aarch64bin(false) {}
        virtual void* compile(Context* a_ctx, Func* a_func) const override final;
        inline void setAarch64AsTarget() {m_aarch64 = true;}
        inline void setAarch64BinAsTarget() {m_aarch64bin = true;}
    private:
        bool m_aarch64; //TODO(ch) : Kolkhoz i isolenta.
        bool m_aarch64bin;
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

    Compiler Compiler::make_aarch64_compiler()
    {
        Compiler res;
        AArch64Compiler* content = new AArch64Compiler();
        content->refcount = 1;
        res.p = content;
        return res;
    }

    Compiler Compiler::make_virtual_dump()
    {
        Compiler res;
        DumpCompiler* content = new DumpCompiler();
        content->refcount = 1;
        res.p = content;
        return res;
    }

    Compiler Compiler::make_aarch64_dump()
    {
        Compiler res;
        DumpCompiler* content = new DumpCompiler();
        content->setAarch64AsTarget();
        content->refcount = 1;
        res.p = content;
        return res;
    }

    Compiler Compiler::make_aarch64_bin_dump()
    {
        Compiler res;
        DumpCompiler* content = new DumpCompiler();
        content->setAarch64BinAsTarget();
        content->refcount = 1;
        res.p = content;
        return res;
    }

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
        size_t retidx = provide_idx();
        m_program.emplace_back(opcode, std::initializer_list<Arg>({ireg_hidden_constructor(this, retidx)}), args);
        return ireg_hidden_constructor(this, retidx);
    }

    inline IReg FuncImpl::newiop(int opcode, int depth, ::std::initializer_list<Arg> args)
    {
        size_t retidx = provide_idx();
        m_program.emplace_back(opcode, std::initializer_list<Arg>({ireg_hidden_constructor(this, retidx), depth}), args);
        return ireg_hidden_constructor(this, retidx);
    }

    inline IReg FuncImpl::newiop(int opcode, ::std::initializer_list<Arg> args, IRegInternal retreg)
    {
        Arg retarg;
        retarg.tag = Arg::IREG;
        retarg.idx = retreg;
        m_program.emplace_back(opcode, std::initializer_list<Arg>({retarg}), args);
        return ireg_hidden_constructor(this, retreg);
    }

    inline void FuncImpl::newiop_noret(int opcode, ::std::initializer_list<Arg> args)
    {
        m_program.emplace_back(opcode, args);
    }

    inline void FuncImpl::newiop_noret(int opcode, int depth, std::initializer_list<Arg> args)
    {
        m_program.emplace_back(opcode, std::initializer_list<Arg>({Arg(depth)}), args);
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
                syntop& jmpop = m_program[retpos];
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
        newiop_noret(OP_JZ, { r, loopstart });
    }
    void FuncImpl::if_(const IReg& r)
    {
        m_cflowstack.push(FuncImpl::cflowbracket(FuncImpl::cflowbracket::IF, m_program.size()));
        Arg ifstart;
        ifstart.tag = Arg::ICONST;
        ifstart.value = 0;
        newiop_noret(OP_JNE, { r, ifstart });
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
        const syntop& ifop = m_program[bracket.m_label_or_pos];
        if (ifop.args.size() != 2 || ifop.args[1].tag != Arg::ICONST)
            throw std::string("\"If\" internal error: wrong JZ command format");
        m_program[bracket.m_label_or_pos].args[1].value = append_label();
    }

    void FuncImpl::return_(const IReg& retval)
    {
        if (m_return_positions.size() && m_return_idx == IReg::NOIDX)
            throw std::string("Mixed return types.");
        m_return_idx = (m_return_idx == IReg::NOIDX) ? provide_idx() : m_return_idx;
        Arg rettar;
        rettar.tag = Arg::IREG;
        rettar.idx = m_return_idx;
        m_program.emplace_back(OP_MOV, std::initializer_list<Arg>({ rettar, retval }));
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

    syntop::syntop(const syntop& fwho) : opcode(fwho.opcode), args(fwho.args) {}

    syntop::syntop(int a_opcode, std::initializer_list<Arg> a_args): opcode(a_opcode), args(a_args){}
        
    syntop::syntop(int a_opcode, std::initializer_list<Arg> a_prefix,                                             std::initializer_list<Arg> a_args): opcode(a_opcode)
    {
        args.reserve(a_prefix.size() + a_args.size());
        for(auto arg:a_prefix) args.emplace_back(arg);
        for(auto arg:a_args) args.emplace_back(arg);
    }

    inline ::std::ostream& operator<<(::std::ostream& str, const Arg& arg)
    {
        switch (arg.tag)
        {
        case Arg::IREG: str << "i" << arg.idx; break;
        case Arg::ICONST: str << arg.value; break;
        default:
            throw std::string("Undefined argument type.");
        };
        return str;
    }

    void print_program(::std::ostream& str, const syntprogram& prog,
                       const std::unordered_map<int, std::string>& opstrings,
                       const std::unordered_map<int, std::function<void(::std::ostream&, const syntop&)> >& p_overrules, size_t firstop, size_t lastop)
    {
        firstop = (firstop == -1) ? 0 : firstop;
        lastop = (lastop == -1) ? prog.size(): lastop;
        for(size_t opnum = firstop; opnum<lastop; opnum++)
        {
            const syntop& op = prog[opnum];
            str << "   " << std::setw(6) << opnum << " : ";
            if (p_overrules.count(op.opcode) == 0)
            {
                if (opstrings.count(op.opcode) == 0)
                    throw std::string("Printer: unprintable operation");
                str<<opstrings.at(op.opcode)<<" ";
                for (size_t argnum = 0 ; argnum + 1 < op.args.size(); argnum++)
                    str << op.args[argnum]<<", ";
                if(op.args.size())
                    str<<op.args.back();
            }
            else
                p_overrules.at(op.opcode)(str, op);
            str << std::endl;
        }
    }

    void print_program(::std::ostream& str, const syntprogram& prog,
                       const std::unordered_map<int, std::string>& opstrings, size_t firstop, size_t lastop)
    {
        std::unordered_map<int, std::function<void(::std::ostream&, const syntop&)> > dummy;
        print_program(str, prog, opstrings, dummy, firstop, lastop);
    }

    void* DumpCompiler::compile(Context* a_ctx, Func* a_func) const
    {
        FuncImpl* func = static_cast<FuncImpl*>(a_func);
        ::std::string listing;
        {
            ::std::ostringstream str(listing, ::std::ios::out);
            str << func->name() << "(";
            for (size_t argnum = 0; argnum + 1 < func->getParams().size(); argnum++)
                str << "i" << (func->getParams())[argnum] << ", ";
            if (func->getParams().size())
                str<<"i"<<func->getParams().back();
            str << ")" << std::endl;
            if(m_aarch64bin)
            {
                std::vector<syntop> aarch64code = bytecode2arm64(func->getProgram(), func->getParams(), func->getRetReg());
                p_canvas canvas2print;
                canvas2print.fromsynt(aarch64instructionset(), aarch64code);
                if(canvas2print.m_buffer.size()%4 != 0)
                    throw std::string("AArch64 binary must be multiple of 4");
                size_t opnum = 0;
                for(size_t ipos = 0; ipos < canvas2print.m_buffer.size(); ipos+=4)
                {
                    str << "   " << std::setfill(' ') << std::setw(6) << opnum++ << " : ";
                    for(size_t pos = ipos; pos < ipos + 4; pos++) //TODO(ch): Print variants
                        str << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)*(canvas2print.m_buffer.begin()+pos);
//                    for(size_t pos = ipos+3; pos + 1> ipos; pos--) //TODO(ch): Again, ensure, that all instructions will be 4-bytes-wide
//                        str << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)*(canvas2print.m_buffer.begin()+pos);
                    str << std::endl;
                }
            }
            else if(m_aarch64)
            {
                std::vector<syntop> aarch64code = bytecode2arm64(func->getProgram(), func->getParams(),func->getRetReg());
                std::unordered_map<int, std::string> opstrings = {{A64_LDRSW_R, "ldrsw.r"}, {A64_LDRSW_I, "ldrsw.i"}, {A64_STR32_I, "str.32.i"}, {A64_MOV_I, "mov.i"}, {A64_MOV_R, "mov.r"}, {A64_ADD_R, "add.r"}, {A64_MUL, "mul"}, {A64_SDIV, "sdiv"}, {A64_CMP_R, "cmp.r"}, {A64_B_LT, "b.lt"}, {A64_B_GT, "b.gt"}, {A64_B_GE, "b.ge"}, {A64_B_LE, "b.le"}, {A64_RET, "ret"}};
                //TODO(ch): add overrule for ret("ret i30" -> "ret")
                print_program(str, aarch64code, opstrings);
            }
            else
            {
                std::unordered_map<int, std::string> opstrings = {{OP_MOV, "mov"}, {OP_CMP_LT, "cmp_lt"}, {OP_CMP_LE, "cmp_le"}, {OP_CMP_GT, "cmp_gt"}, {OP_AUG_ADD, "aug_add"}, {OP_AUG_MUL, "aug_mul"}, {OP_AUG_DIV, "aug_div"}, {OP_JNE, "jne"}, {OP_JZ, "jz"}, {OP_RET, "ret"}};//TODO(ch): will you create at every print?
                std::unordered_map<int, std::function<void(::std::ostream&, const syntop&)> > printoverrules = {
                    {OP_LOAD, [this](::std::ostream& str, const syntop& op){
                        if ((op.args.size() != 3 && op.args.size() != 4) || op.args[1].tag != Arg::ICONST)
                            throw std::string("Wrong LOAD format");
                        str << "load." << type_suffixes[op.args[1].value]<<" " << op.args[0]<<", "<<op.args[2];
                        if(op.args.size() == 4)
                            str << ", " <<op.args[3];
                    }},
                    {OP_STORE, [this](::std::ostream& str, const syntop& op){
                        if (op.args.size() != 3|| op.args[0].tag != Arg::ICONST)
                            throw std::string("Wrong STORE format");
                        str << "store." << type_suffixes[op.args[0].value]<<" " << op.args[1]<<", "<<op.args[2];
                    }},
                    {OP_LABEL, [this](::std::ostream& str, const syntop& op){
                        if (op.args.size() != 1 || op.args[0].tag != Arg::ICONST)
                            throw std::string("Wrong LABEL format");
                        str << "label " << op.args[0] << ":";
                    }},
                    };
                print_program(str, func->getProgram(), opstrings, printoverrules);
            }
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
