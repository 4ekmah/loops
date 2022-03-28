/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "func_impl.hpp"
#include "backend.hpp"
#include <iomanip>

namespace loops
{

IReg iregHiddenConstructor(Func* a_func, size_t a_idx)
{
    IReg ret;
    ret.func = a_func;
    ret.idx = a_idx;
    return static_cast<IReg&&>(ret);
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

std::string type_suffixes[] = { //TODO(ch): find a better place for this
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

void printProgram(::std::ostream& str, const Syntfunc& func,
                   const std::unordered_map<int, std::string>& opstrings,
                   const std::unordered_map<int, std::function<void(::std::ostream&, const Syntop&)> >& p_overrules, size_t firstop, size_t lastop)
{
    firstop = (firstop == -1) ? 0 : firstop;
    lastop = (lastop == -1) ? func.m_program.size(): lastop;
    for(size_t opnum = firstop; opnum<lastop; opnum++)
    {
        const Syntop& op = func.m_program[opnum];
        str << "   " << std::setw(6) << opnum << " : ";
        if (p_overrules.count(op.opcode) == 0)
        {
            if (opstrings.count(op.opcode) == 0)
                throw std::string("Printer: unprintable operation");
            str<<opstrings.at(op.opcode)<<" ";
            for (size_t argnum = 0 ; argnum + 1 < op.size(); argnum++)
                str << op.args[argnum]<<", ";
            if(op.size())
                str<<op.back();
        }
        else
            p_overrules.at(op.opcode)(str, op);
        str << std::endl;
    }
}

void printProgram(::std::ostream& str, const Syntfunc& func,
                   const std::unordered_map<int, std::string>& opstrings, size_t firstop, size_t lastop)
{
    std::unordered_map<int, std::function<void(::std::ostream&, const Syntop&)> > dummy;
    printProgram(str, func, opstrings, dummy, firstop, lastop);
}

FuncImpl::FuncImpl(const std::string& name, Context* ctx, std::initializer_list<IReg*> params) : m_refcount(0)
    , m_name(name)
    , m_nextIdx(0)
    , m_nextLabelIdx(0)
    , m_context(getImpl(ctx))
{
    m_data.m_params.reserve(params.size());
    for (IReg* parreg : params)
    {
        if(parreg->func != nullptr || parreg->idx != IReg::NOIDX)
            throw std::string("Parameter index is already initilized in some other function");
        parreg->func = this;
        parreg->idx = provideIdx();
        m_data.m_params.emplace_back(parreg->idx);
    }
}

Func FuncImpl::makeWrapper(const std::string& name, Context* ctx, std::initializer_list<IReg*> params)
{
    return Func::make(new FuncImpl(name, ctx, params));
}

void* FuncImpl::ptr()
{
    return m_context->getCompiler()->compile(m_context, this);
}

void FuncImpl::printBytecode(std::ostream& out) const
{
    printFunctionHeader(out);
    std::unordered_map<int, std::string> opstrings = {{OP_MOV, "mov"}, {OP_CMP_LT, "cmp_lt"}, {OP_CMP_LE, "cmp_le"}, {OP_CMP_GT, "cmp_gt"}, {OP_ADD, "add"}, {OP_MUL, "mul"}, {OP_DIV, "div"}, {OP_JNE, "jne"}, {OP_JZ, "jz"}, {OP_RET, "ret"}};//TODO(ch): will you create at every print?
    std::unordered_map<int, std::function<void(::std::ostream&, const Syntop&)> > printoverrules = {
        {OP_LOAD, [this](::std::ostream& str, const Syntop& op){
            if ((op.size() != 3 && op.size() != 4) || op.args[1].tag != Arg::ICONST)
                throw std::string("Wrong LOAD format");
            str << "load." << type_suffixes[op.args[1].value]<<" " << op.args[0]<<", "<<op.args[2];
            if(op.size() == 4)
                str << ", " <<op.args[3];
        }},
        {OP_STORE, [this](::std::ostream& str, const Syntop& op){
            if (op.size() != 3|| op.args[0].tag != Arg::ICONST)
                throw std::string("Wrong STORE format");
            str << "store." << type_suffixes[op.args[0].value]<<" " << op.args[1]<<", "<<op.args[2];
        }},
        {OP_LABEL, [this](::std::ostream& str, const Syntop& op){
            if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                throw std::string("Wrong LABEL format");
            str << "label " << op.args[0] << ":";
        }},
        };
    printProgram(out, m_data, opstrings, printoverrules);
}
void FuncImpl::printAssembly(std::ostream& out, int colums) const
{
    printFunctionHeader(out); //TODO(ch): It's better to print header of tarcode
    BackendImpl* be = getImpl(m_context->getCompiler());
    Syntfunc tarcode = be->bytecode2Target(m_data);
    printProgram(out, tarcode, be->getOpStrings());
}

void FuncImpl::printTargetHex(std::ostream& out) const
{
//    print_function_header(out); //TODO(ch): It's better to print header of tarcode
//    backend_impl* be = getImpl(m_context->get_compiler());
//    syntfunc tarcode = be->bytecode2target(m_data);
    //                std::vector<syntop> aarch64code = bytecode2arm64(sfunc);
    //                p_canvas canvas2print;
    //                canvas2print.fromsynt(aarch64instructionset(), aarch64code);
    //                if(canvas2print.m_buffer.size()%4 != 0)
    //                    throw std::string("AArch64 binary must be multiple of 4");
    //                size_t opnum = 0;
    //                for(size_t ipos = 0; ipos < canvas2print.m_buffer.size(); ipos+=4)
    //                {
    //                    str << "   " << std::setfill(' ') << std::setw(6) << opnum++ << " : ";
    //                    for(size_t pos = ipos; pos < ipos + 4; pos++) //TODO(ch): Print variants
    //                        str << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)*(canvas2print.m_buffer.begin()+pos);
    ////                    for(size_t pos = ipos+3; pos + 1> ipos; pos--) //TODO(ch): Again, ensure, that all instructions will be 4-bytes-wide
    ////                        str << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)*(canvas2print.m_buffer.begin()+pos);
    //                    str << std::endl;
    //                }

}


size_t FuncImpl::appendLabel()
{
    size_t label_idx = m_nextLabelIdx++;
    Arg label_arg;
    label_arg.tag = Arg::ICONST;
    label_arg.value = label_idx;
    newiopNoret(OP_LABEL, { label_arg });
    return label_idx;
}
const size_t FuncImpl::EMPTYLABEL = static_cast<size_t>(-1);

void FuncImpl::endfunc()
{
    //TODO(ch): what if return are not on all control pathes(this problem exists for register returns only)??? Think out.
    //          I think, it can be effectively soluted only after deletition of after-jump-silent tails a-la jmp end; mov ..code-without-jumps...; end:
    //          After this we just need to check last operation before end mark. it must be mov to return register.
    //Handle situation, when return is just before end of function (reasonable)
    if (m_returnPositions.size() && m_returnPositions.back() + 1 == m_data.m_program.size())
    {
        m_data.m_program.pop_back();
        m_returnPositions.pop_back();
    }
    if (m_returnPositions.size())
    {
        size_t endfunclabel = appendLabel();
        for (size_t retpos : m_returnPositions)
        {
            Syntop& jmpop = m_data.m_program[retpos];
            if (jmpop.size() != 1 || jmpop.args[0].tag != Arg::ICONST)
                throw std::string("\"If\" internal error: wrong JMP command format");
            jmpop.args[0].value = endfunclabel;
        }
    }
    m_returnPositions.clear();
    newiopNoret(OP_RET, {});
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
    m_cflowStack.push(FuncImpl::cflowbracket(FuncImpl::cflowbracket::DO, appendLabel()));
}

void FuncImpl::while_(const IReg& r)
{
    if (m_cflowStack.size() == 0)
        throw std::string("Unclosed control flow bracket: there is no \"do\" for \"while\".");
    auto bracket = m_cflowStack.top();
    if (bracket.tag != FuncImpl::cflowbracket::DO)
        throw std::string("Control flow bracket error: expected corresponding \"do\" for \"while\".");
    m_cflowStack.pop();
    Arg loopstart;
    loopstart.tag = Arg::ICONST;
    loopstart.value = bracket.labelOrPos;
    newiopNoret(OP_JZ, { r, loopstart });
}
void FuncImpl::if_(const IReg& r)
{
    m_cflowStack.push(FuncImpl::cflowbracket(FuncImpl::cflowbracket::IF, m_data.m_program.size()));
    Arg ifstart;
    ifstart.tag = Arg::ICONST;
    ifstart.value = 0;
    newiopNoret(OP_JNE, { r, ifstart });
}
void FuncImpl::endif_() //TODO(ch): Corresponding tag also can be ELSE or ELIF.
{
    if (m_cflowStack.size() == 0)
        throw std::string("Unclosed control flow bracket: there is no \"if\", \"elif\" or \"else\", for \"endif\".");
    auto bracket = m_cflowStack.top();
    if (bracket.tag != FuncImpl::cflowbracket::IF)
        throw std::string("Control flow bracket error: expected corresponding \"if\", \"elif\" or \"else\" for \"endif\".");
    m_cflowStack.pop();
    size_t posnext = m_data.m_program.size();
    if (bracket.labelOrPos >= posnext)
        throw std::string("\"If\" internal error: wrong branch start address");
    const Syntop& ifop = m_data.m_program[bracket.labelOrPos];
    if (ifop.size() != 2 || ifop.args[1].tag != Arg::ICONST)
        throw std::string("\"If\" internal error: wrong JZ command format");
    m_data.m_program[bracket.labelOrPos].args[1].value = appendLabel();
}

void FuncImpl::return_(const IReg& retval)
{
    if (m_returnPositions.size() && m_data.m_retreg == IReg::NOIDX)
        throw std::string("Mixed return types.");
    m_data.m_retreg = (m_data.m_retreg == IReg::NOIDX) ? provideIdx() : m_data.m_retreg;
    Arg rettar;
    rettar.tag = Arg::IREG;
    rettar.idx = m_data.m_retreg;
    m_data.m_program.emplace_back(OP_MOV, std::initializer_list<Arg>({ rettar, retval }));
    Arg funcfinish;
    funcfinish.tag = Arg::ICONST;
    funcfinish.value = 0;
    m_returnPositions.push_back(m_data.m_program.size());
    newiopNoret(OP_JMP, { funcfinish });
}

void FuncImpl::return_()
{
    if (m_returnPositions.size() && m_data.m_retreg != IReg::NOIDX)
        throw std::string("Mixed return types.");
    Arg funcfinish;
    funcfinish.tag = Arg::ICONST;
    funcfinish.value = 0;
    m_returnPositions.push_back(m_data.m_program.size());
    newiopNoret(OP_JMP, { funcfinish });
}

void FuncImpl::printFunctionHeader(std::ostream& out) const
{
    out << name() << "(";
    for (size_t argnum = 0; argnum + 1 < m_data.m_params.size(); argnum++)
        out << "i" << m_data.m_params[argnum] << ", ";
    if (m_data.m_params.size())
        out<<"i"<<m_data.m_params.back();
    out << ")" << std::endl;
}

};
