/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "func_impl.hpp"
#include "backend.hpp"
#include "printer.hpp"
#include <algorithm>
#include <iomanip>
#include <deque>
#include <iostream>

namespace loops
{
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

std::unordered_map<int, Printer::ColPrinter > opnameoverrules = {
    {OP_LOAD, [](::std::ostream& str, const Syntop& op, size_t, BackendImpl*){
        str << "load." << type_suffixes[op.args[1].value];
    }},
    {OP_STORE, [](::std::ostream& str, const Syntop& op, size_t, BackendImpl*){
        str << "store." << type_suffixes[op.args[0].value];
    }},
    {OP_LABEL, [](::std::ostream& str, const Syntop& op, size_t, BackendImpl*){
        if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
            throw std::string("Wrong LABEL format");
        str << "label " << op.args[0] << ":";
    }}
};

std::unordered_map<int, Printer::ColPrinter > argoverrules = {
    {OP_LABEL, [](::std::ostream& str, const Syntop& op, size_t, BackendImpl*){}}
};

std::unordered_map<int, std::string> opstrings = { //TODO(ch): will you create at every print?
    {OP_MOV, "mov"},
    {OP_CMP, "cmp"},
    {OP_ADD, "add"},
    {OP_MUL, "mul"},
    {OP_DIV, "div"},
    {OP_SUB, "sub"},
    {OP_JMP, "jmp"},
    {OP_JMP_EQ, "jmp_eq"},
    {OP_JMP_NE, "jmp_ne"},
    {OP_JMP_GE, "jmp_ge"},
    {OP_JMP_LE, "jmp_le"},
    {OP_JMP_GT, "jmp_gt"},
    {OP_JMP_LT, "jmp_gt"},
    {OP_SPILL, "spill"},
    {OP_UNSPILL, "unspill"},
    {OP_IF, "annotation:if"},
    {OP_ELSE, "annotation:else"},
    {OP_ENDIF,"annotation:endif"},
    {OP_DO, "annotation:do"},
    {OP_WHILE, "annotation:while"},
    {OP_DOIF, "annotation:doif"},
    {OP_ENDDO, "annotation:enddo"},
    {OP_BREAK, "annotation:break"},
    {OP_CONTINUE, "annotation:continue"},
    {OP_RET, "ret"},
};

FuncImpl::FuncImpl(const std::string& name, Context* ctx, std::initializer_list<IReg*> params) : m_refcount(0)
    , m_nextIdx(0)
    , m_nextLabelIdx(0)
    , m_context(getImpl(ctx))
    , m_returnType(RT_NOTDEFINED)
    , m_compiled(nullptr)
{
    m_data.name = name;
    m_data.params.reserve(params.size());
    for (IReg* parreg : params)
    {
        if(parreg->func != nullptr || parreg->idx != IReg::NOIDX)
            throw std::string("Parameter index is already initilized in some other function");
        parreg->func = this;
        parreg->idx = provideIdx();
        m_data.params.emplace_back(parreg->idx);
    }
}

Func FuncImpl::makeWrapper(const std::string& name, Context* ctx, std::initializer_list<IReg*> params)
{
    return Func::make(new FuncImpl(name, ctx, params));
}

void* FuncImpl::ptr()
{
    if(!m_compiled)
        m_compiled = m_context->getBackend()->compile(m_context, this);
    return m_compiled;
}

void FuncImpl::printBytecode(std::ostream& out) const
{
    Printer printer({Printer::colNumPrinter(0), Printer::colOpnamePrinter(opstrings, opnameoverrules), Printer::colArgListPrinter(argoverrules)});
    printer.print(out, m_data);
}

void FuncImpl::printAssembly(std::ostream& out, int columns) const
{
    BackendImpl* be = getImpl(m_context->getBackend());
    Syntfunc tarcode = be->bytecode2Target(m_data);
    std::vector<Printer::ColPrinter> columnPrs;
    columnPrs.reserve(3);
    if(columns&PC_OPNUM)
        columnPrs.push_back(Printer::colNumPrinter(0));
    if(columns&PC_OP)
    {
        columnPrs.push_back(Printer::colOpnamePrinter(be->getOpStrings()));
        columnPrs.push_back(Printer::colArgListPrinter());
    }
    if(columns&PC_HEX)
    {
        if(columns&PC_OP)
            columnPrs.push_back(Printer::colDelimeterPrinter());
        columnPrs.push_back(be->colHexPrinter(tarcode));
    }
    Printer printer(columnPrs);
    printer.setBackend(be);
    printer.print(out, tarcode);
}

size_t FuncImpl::provideLabel()
{
    return m_nextLabelIdx++;
}

FuncImpl* FuncImpl::verifyArgs(std::initializer_list<Arg> args)
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

void FuncImpl::endfunc()
{
    if(m_cflowStack.size())
        throw std::string("Unclosed control flow bracket."); //TODO(ch): Look at stack for providing more detailed information.
    //TODO(ch): block somehow adding new instruction after this call.
    allocateRegisters();
    jumpificate();
}

IReg FuncImpl::const_(int64_t value)
{
    return static_cast<IReg&&>(newiop(OP_MOV, { argIConst(value, this) }));
}

void FuncImpl::do_()
{
    size_t label = provideLabel();
    m_cflowStack.push_back(FuncImpl::cflowbracket(FuncImpl::cflowbracket::DO, label));
    newiopNoret(OP_DO, {argIConst(label,this)});
}

void FuncImpl::while_(const IReg& r)
{
    if (m_cflowStack.size() == 0)
        throw std::string("Unclosed control flow bracket: there is no \"do\" for \"while\".");
    auto bracket = m_cflowStack.back();
    if (bracket.tag != FuncImpl::cflowbracket::DO)
        throw std::string("Control flow bracket error: expected corresponding \"do\" for \"while\".");
    m_cflowStack.pop_back();
    size_t brekLabel = NOLABEL;
    size_t contLabel = NOLABEL;
    if(!bracket.continues.empty())
    {
        contLabel = provideLabel();
        for(size_t con : bracket.continues)
        {
            if (con >= m_data.program.size())
                throw std::string("\"Do\" internal error: wrong \"continue\" address.");
            Syntop& conop = m_data.program[con];
            if (conop.opcode != OP_CONTINUE || conop.size() != 1 || conop.args[0].tag != Arg::ICONST)
                throw std::string("\"Do\" internal error: wrong \"continue\" command format.");
            conop[0].value = contLabel;
        }
    }
    if(!bracket.breaks.empty())
    {
        brekLabel = provideLabel();
        for(size_t bre : bracket.breaks)
        {
            if (bre >= m_data.program.size())
                throw std::string("\"Do\" internal error: wrong \"break\" address.");
            Syntop& breop = m_data.program[bre];
            if (breop.opcode != OP_BREAK || breop.size() != 1 || breop.args[0].tag != Arg::ICONST)
                throw std::string("\"Do\" internal error: wrong \"break\" command format.");
            breop[0].value = brekLabel;
        }
    }
    if(m_cmpopcode == OP_JMP)
        throw std::string("Temporary condition solution failed."); //TODO(ch): IMPORTANT(CMPLCOND)
    newiopNoret(OP_WHILE, {argIConst(m_cmpopcode, this), argIConst(bracket.labelOrPos, this), argIConst(contLabel, this), argIConst(brekLabel, this)});
    m_cmpopcode = OP_JMP;
}

void FuncImpl::doif_(const IReg& r)
{
    size_t nextPos = m_data.program.size();
    size_t contLabel = provideLabel();
    size_t brekLabel = NOLABEL;
    m_cflowStack.push_back(FuncImpl::cflowbracket(FuncImpl::cflowbracket::DOIF, nextPos));
    m_cmpopcode = invertCondition(m_cmpopcode);
    if(m_cmpopcode == OP_JMP)
        throw std::string("Temporary condition solution failed."); //TODO(ch): IMPORTANT(CMPLCOND)
    newiopNoret(OP_DOIF, {argIConst(m_cmpopcode, this), argIConst(contLabel, this), argIConst(brekLabel, this)});
    m_cmpopcode = OP_JMP;
}

void FuncImpl::enddo_()
{
    if (m_cflowStack.size() == 0)
        throw std::string("Unclosed control flow bracket: there is no \"doif\" for \"enddo\".");
    auto bracket = m_cflowStack.back();
    m_cflowStack.pop_back();
    if (bracket.tag != FuncImpl::cflowbracket::DOIF)
        throw std::string("Control flow bracket error: expected corresponding \"doif\" for \"enddo\".");
    size_t nextPos = m_data.program.size();
    size_t doifPos = bracket.labelOrPos;
    if(doifPos >= nextPos)
        throw std::string("\"Doif\" internal error: wrong branch start address.");
    Syntop& doifOp = m_data.program[doifPos];
    if (doifOp.opcode!= OP_DOIF || doifOp.size() != 3 || doifOp[0].tag != Arg::ICONST || doifOp[1].tag != Arg::ICONST || doifOp[2].tag != Arg::ICONST)
        throw std::string("\"Doif\" internal error: wrong command format.");
    size_t brekLabel = provideLabel();
    doifOp[2].value = brekLabel;
    size_t contLabel = doifOp[1].value;
    for(size_t bre : bracket.breaks)
    {
        if (bre >= m_data.program.size())
            throw std::string("\"Doif\" internal error: wrong \"break\" address.");
        Syntop& breop = m_data.program[bre];
        if (breop.opcode != OP_BREAK || breop.size() != 1 || breop.args[0].tag != Arg::ICONST)
            throw std::string("\"Doif\" internal error: wrong \"break\" command format.");
        breop[0].value = brekLabel;
    }
    newiopNoret(OP_ENDDO, {argIConst(contLabel, this), argIConst(brekLabel, this)});
}

void FuncImpl::break_()
{
    auto rator = m_cflowStack.rbegin();
    for(; rator != m_cflowStack.rend(); ++rator)
        if(rator->tag == FuncImpl::cflowbracket::DO || rator->tag == FuncImpl::cflowbracket::DOIF)
            break;
    size_t nextPos = m_data.program.size();
    if (rator == m_cflowStack.rend())
        throw std::string("Unclosed control flow bracket: there is no \"do\" or \"doif\" for \"break\".");
    rator->breaks.push_back(nextPos);
    newiopNoret(OP_BREAK, {argIConst(0,this)});
}

void FuncImpl::continue_()
{
    auto rator = m_cflowStack.rbegin();
    for(; rator != m_cflowStack.rend(); ++rator)
        if(rator->tag == FuncImpl::cflowbracket::DO || rator->tag == FuncImpl::cflowbracket::DOIF)
            break;
    if (rator == m_cflowStack.rend())
        throw std::string("Unclosed control flow bracket: there is no \"do\" or \"doif\" for \"break\".");
    size_t nextPos = m_data.program.size();
    size_t targetLabel = 0;
    if(rator->tag == FuncImpl::cflowbracket::DOIF)
    {
        size_t doifPos = rator->labelOrPos;
        if(doifPos >= m_data.program.size())
            throw std::string("\"Doif\" internal error: wrong branch start address.");
        Syntop& doifOp = m_data.program[doifPos];
        if (doifOp.opcode!= OP_DOIF || doifOp.size() != 3 || doifOp[0].tag != Arg::ICONST || doifOp[1].tag != Arg::ICONST || doifOp[2].tag != Arg::ICONST)
            throw std::string("\"Doif\" internal error: wrong command format.");
        targetLabel = doifOp[1].value;
    }
    else
        rator->continues.push_back(nextPos);
    newiopNoret(OP_CONTINUE, {argIConst(targetLabel,this)});
}

void FuncImpl::if_(const IReg& r)
{
    m_cflowStack.push_back(FuncImpl::cflowbracket(FuncImpl::cflowbracket::IF, m_data.program.size()));
    m_cmpopcode = invertCondition(m_cmpopcode);
    if(m_cmpopcode == OP_JMP)
        throw std::string("Temporary condition solution failed."); //TODO(ch): IMPORTANT(CMPLCOND)
    //IF(cmp, wrongjmp)
    newiopNoret(OP_IF, {argIConst(m_cmpopcode, this), argIConst(0, this)});
    m_cmpopcode = OP_JMP;
}

void FuncImpl::elif_(const IReg& r)
{
    size_t elifRep = 0;
    {
        if (m_cflowStack.size() == 0)
            throw std::string("Unclosed control flow bracket: there is no \"if\", for \"elif\".");
        cflowbracket& bracket = m_cflowStack.back();
        if (bracket.tag != FuncImpl::cflowbracket::IF)
            throw std::string("Control flow bracket error: expected corresponding \"if\", for \"elif\".");
        size_t elifRep = bracket.elifRepeats;
    }
    Syntop conditionBackup = m_data.program.back(); //TODO(ch): IMPORTANT(CMPLCOND): This mean that condition can be one-instruction only.
    m_data.program.pop_back();
    else_();
    m_data.program.push_back(conditionBackup);
    if_(r);
    if (m_cflowStack.size() == 0)
        throw std::string("Unclosed control flow bracket: there is no \"if\", for \"elif\".");
    cflowbracket& bracket = m_cflowStack.back();
    if (bracket.tag != FuncImpl::cflowbracket::IF)
        throw std::string("Control flow bracket error: expected corresponding \"if\", for \"elif\".");
    bracket.elifRepeats = elifRep + 1;
}

void FuncImpl::else_()
{
    if (m_cflowStack.size() == 0)
        throw std::string("Unclosed control flow bracket: there is no \"if\", for \"else\".");
    cflowbracket& bracket = m_cflowStack.back();
    if (bracket.tag != FuncImpl::cflowbracket::IF)
        throw std::string("Control flow bracket error: expected corresponding \"if\", for \"else\".");
    size_t posnext = m_data.program.size();
    size_t prevBranchPos = bracket.labelOrPos;
    m_cflowStack.push_back(FuncImpl::cflowbracket(FuncImpl::cflowbracket::ELSE, m_data.program.size()));
    if (prevBranchPos >= posnext)
        throw std::string("\"If\" internal error: wrong branch start address");
    const Syntop& ifop = m_data.program[prevBranchPos];
    if (ifop.opcode != OP_IF || ifop.size() != 2 || ifop.args[1].tag != Arg::ICONST)
        throw std::string("\"If\" internal error: wrong \"if\" command format");
    size_t label = provideLabel();
    m_data.program[prevBranchPos].args[1].value = label;
    //ELSE(mylabel, endjmp)
    newiopNoret(OP_ELSE, {argIConst(label, this), argIConst(0, this)});
}

void FuncImpl::endif_()
{
    if (m_cflowStack.size() == 0)
        throw std::string("Unclosed control flow bracket: there is no \"if\", \"elif\" or \"else\", for \"endif\".");
    cflowbracket bracket = m_cflowStack.back();
    m_cflowStack.pop_back();
    size_t posnext = m_data.program.size();
    size_t label = provideLabel();
    bool rewriteNEAddress = true;
    if (bracket.tag == FuncImpl::cflowbracket::ELSE)
    {
        rewriteNEAddress = false;
        size_t elsePos = bracket.labelOrPos;
        if (elsePos >= posnext)
            throw std::string("\"If\" internal error: wrong \"else\" start address");
        const Syntop& elseOp = m_data.program[elsePos];
        if (elseOp.opcode != OP_ELSE || elseOp.size() != 2 || elseOp.args[1].tag != Arg::ICONST)
            throw std::string("\"If\" internal error: wrong \"else\" command format");
        m_data.program[elsePos].args[1].value = label;
        if (m_cflowStack.size() == 0)
            throw std::string("Unclosed control flow bracket: there is no \"if\", \"elif\" or \"else\", for \"endif\".");
        bracket = m_cflowStack.back();
        m_cflowStack.pop_back();
    }
    if (bracket.tag != FuncImpl::cflowbracket::IF)
        throw std::string("Control flow bracket error: expected corresponding \"if\", \"elif\" or \"else\" for \"endif\".");

    size_t ifPos = bracket.labelOrPos;
    if (ifPos >= posnext)
        throw std::string("\"If\" internal error: wrong \"if\" start address");
    const Syntop& ifOp = m_data.program[ifPos];
    if (ifOp.opcode != OP_IF || ifOp.size() != 2 || ifOp.args[1].tag != Arg::ICONST)
        throw std::string("\"If\" internal error: wrong \"else\" command format");
    if(rewriteNEAddress)
        m_data.program[ifPos].args[1].value = label;
    newiopNoret(OP_ENDIF, {argIConst(label, this)});
    if(bracket.elifRepeats != 0)
        endif_();
}

void FuncImpl::return_(const IReg& retval)
{
    if (m_returnType == RT_VOID)
        throw std::string("Mixed return types");
    newiopNoret(OP_MOV, {argIReg(Syntfunc::RETREG, this), retval});
    newiopNoret(OP_RET, {});
}

void FuncImpl::return_()
{
    if (m_returnType == RT_REGISTER)
        throw std::string("Mixed return types");
    newiopNoret(OP_RET, {});
}

struct LiveInterval
{
    size_t start, end;
    IRegInternal idx;
    size_t subinterval;
    LiveInterval(IRegInternal a_idx, size_t a_start) : start(a_start), end(a_start), idx(a_idx){}
    LiveInterval(std::pair<IRegInternal,std::pair<size_t, size_t> > a_tuple) : start(a_tuple.second.first), end(a_tuple.second.second), idx(a_tuple.first) {}
};

struct startordering
{
    bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.start < b.start; }
};

struct endordering
{
    bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.end < b.end; }
};

struct LiveAnEvent
{
    enum {LAE_STARTLOOP, LAE_STARTBRANCH, LAE_STARTSUBINT, LAE_SWITCHSUBINT, LAE_ENDSUBINT, NONDEF = -1};
    int eventType;
    IRegInternal idx;
    size_t subInterval;
    size_t elsePos;
    size_t endNesting;
    LiveAnEvent(): eventType(NONDEF), idx(IReg::NOIDX), subInterval(NONDEF), elsePos(NONDEF), endNesting(NONDEF) {}
};

typedef std::vector<LiveInterval> LALayout; //TODO(ch): find better solution for detecting pseudonames per line number.

std::map<IRegInternal, std::pair<size_t, size_t> > FuncImpl::livenessAnalysis()
{
    //IMPORTANT: Think around situation 1-0-1, when register is defined inside of block and redefined in another of same depth.(0-1-0, obviously doesn't matter).
    const BackendImpl* backend = getImpl(m_context->getBackend());
    std::multimap<size_t, LiveAnEvent> loopQueue;
    std::multimap<size_t, LiveAnEvent> branchQueue;
    std::vector<LALayout> subintervals(m_nextIdx, LALayout());
    { //1.) Calculation of simplest [def-use] subintervals and collect precise info about borders of loops and branches.
        std::deque<cflowbracket> flowstack;
        for(IRegInternal par = 0; par < m_data.params.size(); par++)
            subintervals[par].push_back(LiveInterval(par, 0));
        for(size_t opnum = 0; opnum < m_data.program.size(); opnum++)
        {
            const Syntop& op = m_data.program[opnum];
            switch (op.opcode)
            {
                case (OP_IF):
                {
                    if (op.size() != 2 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST)
                       throw std::string("Internal error: wrong IF command format");
                    flowstack.push_back(FuncImpl::cflowbracket(FuncImpl::cflowbracket::IF, opnum));
                    LiveAnEvent toAdd;
                    toAdd.eventType = LiveAnEvent::LAE_STARTBRANCH;
                    branchQueue.insert(std::make_pair(opnum, toAdd));
                    continue;
                }
                case (OP_ELSE):
                {
                    if (op.size() != 2 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST)
                       throw std::string("Internal error: wrong \"endif\" command format");
                    if(!flowstack.size() || flowstack.back().tag != FuncImpl::cflowbracket::IF)
                        throw std::string("Internal error: control brackets error.");
                    flowstack.push_back(FuncImpl::cflowbracket(FuncImpl::cflowbracket::ELSE, opnum));
                    continue;
                }
                case (OP_ENDIF):
                {
                    if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                       throw std::string("Internal error: wrong \"endif\" command format");
                    if(!flowstack.size())
                        throw std::string("Internal error: control brackets error.");
                    cflowbracket bracket = flowstack.back();
                    flowstack.pop_back();
                    size_t elsePos = LiveAnEvent::NONDEF;
                    if(bracket.tag == FuncImpl::cflowbracket::ELSE)
                    {
                        elsePos = bracket.labelOrPos;
                        if(!flowstack.size())
                            throw std::string("Internal error: control brackets error.");
                        bracket = flowstack.back();
                        flowstack.pop_back();
                    }
                    if(bracket.tag != FuncImpl::cflowbracket::IF)
                        throw std::string("Internal error: control brackets error.");
                    auto rator = branchQueue.find(bracket.labelOrPos);
                    if (rator == branchQueue.end())
                        throw std::string("Internal error: control flow queue doesn't contain corresponding IF for given ENDIF.");
                    rator->second.endNesting = opnum;
                    rator->second.elsePos = elsePos;
                    continue;
                }
                case (OP_DO):
                {
                    if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                       throw std::string("Internal error: wrong DO command format");
                    flowstack.push_back(FuncImpl::cflowbracket(FuncImpl::cflowbracket::DO, opnum));
                    LiveAnEvent toAdd;
                    toAdd.eventType = LiveAnEvent::LAE_STARTLOOP;
                    loopQueue.insert(std::make_pair(opnum, toAdd));
                    continue;
                }
                case (OP_WHILE):
                {
                    if (op.size() != 4 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST || op.args[2].tag != Arg::ICONST || op.args[3].tag != Arg::ICONST)
                       throw std::string("Internal error: wrong WHILE command format");
                    if(!flowstack.size() || flowstack.back().tag != FuncImpl::cflowbracket::DO)
                        throw std::string("Internal error: control brackets error.");
                    const cflowbracket& bracket = flowstack.back();
                    flowstack.pop_back();
                    auto rator = loopQueue.find(bracket.labelOrPos);
                    if (rator == loopQueue.end())
                        throw std::string("Internal error: control flow queue doesn't contain corresponding DO for given WHILE.");
                    rator->second.endNesting = opnum;
                    continue;
                }
                case (OP_DOIF):
                {
                    if (op.size() != 3 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST || op.args[2].tag != Arg::ICONST)
                       throw std::string("Internal error: wrong DOIF command format");
                    if(opnum < 2)
                        throw std::string("Temporary condition solution needs one instruction before DOIF cycle.");
                    flowstack.push_back(FuncImpl::cflowbracket(FuncImpl::cflowbracket::DOIF, opnum-2));
                    LiveAnEvent toAdd;
                    toAdd.eventType = LiveAnEvent::LAE_STARTLOOP;
                    loopQueue.insert(std::make_pair(opnum-2, toAdd)); //TODO(ch): IMPORTANT(CMPLCOND): This mean that condition can be one-instruction only.
                    continue;
                }
                case (OP_ENDDO):
                {
                    if (op.size() != 2 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST)
                       throw std::string("Internal error: wrong ENDDO command format");
                    if(!flowstack.size() || flowstack.back().tag != FuncImpl::cflowbracket::DOIF)
                        throw std::string("Internal error: control brackets error.");
                    const cflowbracket& bracket = flowstack.back();
                    flowstack.pop_back();
                    auto rator = loopQueue.find(bracket.labelOrPos);
                    if (rator == loopQueue.end())
                        throw std::string("Internal error: control flow queue doesn't contain corresponding DOIF for given ENDDO.");
                    rator->second.endNesting = opnum;
                    continue;
                }
                default:
                {
                    std::set<IRegInternal> inRegs = backend->getInRegisters(op);
                    std::set<IRegInternal> outRegs = backend->getOutRegisters(op);
                    std::set<IRegInternal> inOutRegs = backend->getUsedRegisters(op, Binatr::Detail::D_INPUT | Binatr::Detail::D_OUTPUT);
                    for(IRegInternal inreg : inRegs)
                    {
                        if(inreg == Syntfunc::RETREG) //TODO(ch): At some day we will need to work with different types of return.
                            continue;
                        if (subintervals[inreg].size() == 0) //TODO(ch): Isn't it too strict?
                            throw std::string("Compile error: using uninitialized register");
                        LiveInterval& intrvl = subintervals[inreg].back();
                        intrvl.end = opnum;
                    }
                    for(IRegInternal outreg : outRegs)
                    {
                        if(outreg == Syntfunc::RETREG)
                            continue;
                        if(inOutRegs.count(outreg) != 0)
                            continue;
                        subintervals[outreg].push_back(LiveInterval(outreg, opnum));
                    }
                    break;
                }
            }
        }
    }
    
    { //2.) Expanding loop intervals, which are crossing loops borders.
        std::multiset<LiveInterval, endordering> active;
        for(size_t idx = 0; idx < m_nextIdx; idx++)
        {
            const size_t sintStart = subintervals[idx][0].start;
            if(sintStart == 0)
            {
                LiveInterval toActive = subintervals[idx][0];
                toActive.subinterval = 0;
                active.insert(toActive);
                const size_t sintEnd = subintervals[idx][0].end;
                LiveAnEvent toAdd;
                toAdd.eventType = LiveAnEvent::LAE_ENDSUBINT;
                toAdd.idx = idx;
                toAdd.subInterval = 0;
                loopQueue.insert(std::make_pair(sintEnd, toAdd));
            }
            else
            {
                LiveAnEvent toAdd;
                toAdd.eventType = LiveAnEvent::LAE_STARTSUBINT;
                toAdd.idx = idx;
                toAdd.subInterval = 0;
                loopQueue.insert(std::make_pair(sintStart, toAdd));
            }
        }
        while(!loopQueue.empty())
        {
            auto loopRator = loopQueue.begin();
            size_t opnum = loopQueue.begin()->first;
            LiveAnEvent event = loopQueue.begin()->second;
            loopQueue.erase(loopQueue.begin());
            switch (event.eventType)
            {
                case (LiveAnEvent::LAE_STARTSUBINT):
                {
                    LiveInterval li = subintervals[event.idx][event.subInterval];
                    li.subinterval = event.subInterval;
                    active.insert(li);
                    const size_t sintEnd = li.end;
                    LiveAnEvent toAdd;
                    toAdd.eventType = LiveAnEvent::LAE_ENDSUBINT;
                    toAdd.idx = event.idx;
                    toAdd.subInterval = event.subInterval;
                    loopQueue.insert(std::make_pair(sintEnd, toAdd));
                    break;
                };
                case (LiveAnEvent::LAE_ENDSUBINT):
                {
                    auto removerator = active.begin();
                    while(removerator!= active.end() && removerator->idx != event.idx && removerator->end == opnum)
                        ++removerator;
                    if (removerator == active.end() || removerator->idx != event.idx)
                        throw std::string("Internal error: finishing non-active interval.");
                    active.erase(removerator);
                    if(subintervals[event.idx].size() < event.subInterval + 1)
                    {
                        const LiveInterval& newli = subintervals[event.idx][event.subInterval + 1];
                        LiveAnEvent toAdd;
                        toAdd.eventType = LiveAnEvent::LAE_STARTSUBINT;
                        toAdd.idx = event.idx;
                        toAdd.subInterval = event.subInterval + 1;
                        loopQueue.insert(std::make_pair(newli.start, toAdd));
                    }
                    break;
                };
                case (LiveAnEvent::LAE_STARTLOOP):
                {
                    std::multiset<LiveInterval, endordering> changed_active;
                    auto arator = active.begin();
                    for(;arator != active.end() ; ++arator)
                        if(arator->end < event.endNesting)
                        {
                            IRegInternal idx = arator->idx;
                            size_t sinum = arator->subinterval;
                            size_t si2ers = sinum + 1;
                            size_t newEnd = event.endNesting;
                            for(; si2ers < subintervals[idx].size();si2ers++)
                                if(subintervals[idx][si2ers].start > event.endNesting)
                                    break;
                                else
                                    newEnd = std::max(subintervals[idx][si2ers].end, newEnd);
                            subintervals[idx].erase(subintervals[idx].begin() + sinum + 1, subintervals[idx].begin() + si2ers);
                            subintervals[idx][sinum].end = newEnd;
                            subintervals[idx][sinum].subinterval = sinum;
                            changed_active.insert(subintervals[idx][sinum]);
                            auto qremrator = loopQueue.find(arator->end);
                            while(qremrator != loopQueue.end() && qremrator->first == arator->end)
                                if(qremrator->second.eventType == LiveAnEvent::LAE_ENDSUBINT && qremrator->second.idx == arator->idx)
                                    break;
                            if (qremrator == loopQueue.end() || qremrator->first != arator->end)
                                throw std::string("Internal error: end interval event not found in queue.");
                            LiveAnEvent toReadd = qremrator->second;
                            loopQueue.erase(qremrator);
                            loopQueue.insert(std::make_pair(newEnd, toReadd));
                        }
                        else
                            break;
                    active.erase(active.begin(), arator);
                    active.insert(changed_active.begin(), changed_active.end());
                    break;
                }
                default:
                    throw std::string("Internal error: unexpected event in loop queue.");
            }
        }
    }
    
    { //3.) Calculating intervals crossing if branches.
        std::multiset<LiveInterval, endordering> lastActive; // NOTE: In this part of code LiveInterval::end means not end position of subinterval, but deactivation position, position, when starts new subinterval or ends final one.
        for(size_t idx = 0; idx < m_nextIdx; idx++)
        {
            if(subintervals[idx].size() == 0)
                continue;
            const size_t sintStart = subintervals[idx][0].start;
            LiveAnEvent toAdd;
            toAdd.eventType = LiveAnEvent::LAE_SWITCHSUBINT;
            toAdd.idx = idx;
            size_t eventPos;
            if(sintStart == 0)
            {
                eventPos = subintervals[idx].size() == 1 ? subintervals[idx][0].end : subintervals[idx][1].start;
                LiveInterval toActive = subintervals[idx][0];
                toActive.subinterval = 0;
                toActive.end = eventPos;
                lastActive.insert(toActive);
                toAdd.subInterval = 1; //Destination interval
            }
            else
            {
                eventPos = subintervals[idx][0].start;
                toAdd.subInterval = 0; //Destination interval
            }
            branchQueue.insert(std::make_pair(eventPos, toAdd));
        }

        while(!branchQueue.empty())
        {
            size_t opnum = branchQueue.begin()->first;
            LiveAnEvent event = branchQueue.begin()->second;
            branchQueue.erase(branchQueue.begin());
            switch (event.eventType)
            {
                case (LiveAnEvent::LAE_SWITCHSUBINT):
                {
                    if(event.subInterval > 0)
                    {
                        auto removerator = lastActive.begin();
                        while (removerator!= lastActive.end() && removerator->idx != event.idx && removerator->end == opnum)
                            ++removerator;
                        if (removerator != lastActive.end() && removerator->idx == event.idx)
                            lastActive.erase(removerator);
                    }
                    if(event.subInterval < subintervals[event.idx].size())
                    {
                        LiveAnEvent toAdd;
                        toAdd.eventType = LiveAnEvent::LAE_SWITCHSUBINT;
                        toAdd.idx = event.idx;
                        toAdd.subInterval = event.subInterval + 1;
                        size_t eventPos = (event.subInterval + 1 < subintervals[event.idx].size()) ?
                                             subintervals[event.idx][event.subInterval+1].start    :
                                             subintervals[event.idx][event.subInterval].end        ;
                        LiveInterval toActive = subintervals[event.idx][event.subInterval];
                        toActive.subinterval = event.subInterval;
                        toActive.end = eventPos;
                        lastActive.insert(toActive);
                        branchQueue.insert(std::make_pair(eventPos, toAdd));
                    }
                    break;
                };
                case (LiveAnEvent::LAE_STARTBRANCH):
                {
                    const size_t ifPos = opnum;
                    const size_t endifPos = event.endNesting;
                    const bool haveElse = (event.elsePos != LiveAnEvent::NONDEF);
                    const size_t elsePos = haveElse ? event.elsePos : endifPos;
                    std::multiset<LiveInterval, endordering> lastActiveChanged;
                    std::multiset<LiveInterval, endordering>::iterator larator = lastActive.begin();
                    for(;larator != lastActive.end() && larator->end<endifPos;)
                    {
                        size_t firstUseMain = -1;
                        size_t firstDefMain = -1;
                        size_t firstUseElse = -1;
                        size_t firstDefElse = -1;
                        const IRegInternal idx = larator->idx;
                        bool afterlife = (subintervals[idx].back().end > endifPos);
                        if(!haveElse && !afterlife)
                        {
                            larator++;
                            continue;
                        }
                        const size_t initSinum = larator->subinterval;
                        size_t sinum = initSinum;
                        for (;sinum<subintervals[idx].size();++sinum)
                        {
                            const size_t sistart = subintervals[idx][sinum].start;
                            const size_t siend = subintervals[idx][sinum].end;
                            if(sistart > endifPos)
                                break;
                            if(sistart > ifPos)
                            {
                                if(haveElse && sistart > elsePos && firstDefElse == -1)
                                    firstDefElse = sistart;
                                else if(firstDefMain == -1)
                                    firstDefMain = sistart;
                            }
                            if(siend > endifPos)
                                break;
                            if(siend > ifPos)
                            {
                                if(haveElse && siend > elsePos && firstUseElse == -1)
                                    firstUseElse = siend;
                                else if(firstUseMain == -1)
                                    firstUseMain = siend;
                            }
                        }
                        if(firstUseMain != -1 && firstUseMain > firstDefMain)
                            firstUseMain = -1;
                        if(firstUseElse != -1 && firstUseElse > firstDefElse)
                            firstUseElse = -1;
                        bool splice = false;
                        if(firstDefMain != -1 && firstUseElse != -1) // Abscence of linear separability.
                        {
                            //if(!afterlife && firstDefElse != -1)//TODO(ch): than you can splice just upto firstUseElse by changing sinum.
                            splice = true;
                        }
                        else if(!afterlife)
                        {
                            splice = false;
                        }
                        else if (firstDefElse == -1 && firstUseElse == -1)
                        {
                            if(firstDefMain == -1)
                                splice = false;
                            splice = true;
                        }
                        else if ((firstDefMain == -1 && firstDefElse != -1)|| //One-of-branch redefinition with
                                 (firstDefMain != -1 && firstDefElse == -1))  //afterusage means splicing.
                        {
                            splice = true;
                        }
                        else
                            splice = false;
                        if(splice)
                        {
                            const size_t switchIpos = larator->end;
                            if(sinum == subintervals[idx].size())
                                sinum = subintervals[idx].size() - 1;
                            else if(subintervals[idx][sinum].start > endifPos)
                                sinum--;
                            subintervals[idx][initSinum].end = subintervals[idx][sinum].end;
                            subintervals[idx].erase(subintervals[idx].begin() + initSinum + 1, subintervals[idx].begin() + sinum + 1);
                            LiveInterval changedOne = subintervals[idx][initSinum];
                            changedOne.end = (initSinum + 1 < subintervals[idx].size())? subintervals[idx][initSinum + 1].start: changedOne.end;
                            larator = lastActive.erase(larator);
                            lastActiveChanged.insert(changedOne);

                            auto qremrator = branchQueue.find(switchIpos);
                            while(qremrator != branchQueue.end() && qremrator->first == switchIpos)
                                if(qremrator->second.eventType == LiveAnEvent::LAE_SWITCHSUBINT && qremrator->second.idx == idx)
                                    break;
                            if (qremrator == branchQueue.end() || qremrator->first != switchIpos)
                                throw std::string("Internal error: end interval event not found in queue.");
                            LiveAnEvent toReadd = qremrator->second;
                            branchQueue.erase(qremrator);
                            branchQueue.insert(std::make_pair(changedOne.end, toReadd));
                        }
                        else
                            larator++;
                    }
                    lastActive.insert(lastActiveChanged.begin(), lastActiveChanged.end());
                    break;
                }
                default:
                    throw std::string("Internal error: unexpected event in branch queue.");
            }
        }
    }

    { //4.) Renaming splitted registers.
        size_t pseudIdx = m_data.params.size();
        for(IRegInternal idx = 0; idx < m_data.params.size(); idx++)
            for(size_t si = 1; si < subintervals[idx].size(); si++)
                subintervals[idx][si].idx = pseudIdx++;
        for(IRegInternal idx = m_data.params.size(); idx < subintervals.size(); idx++)
            for(LiveInterval& li:subintervals[idx])
                li.idx = pseudIdx++;
        for(size_t opnum = 0; opnum < m_data.program.size(); opnum++)
        {
            Syntop& op = m_data.program[opnum];
            std::set<size_t> outRegArnums = backend->getOutRegistersIdxs(op);
            for(size_t arnum = 0; arnum < op.size(); arnum++)
            {
                Arg& arg = op.args[arnum];
                if(arg.tag == Arg::IREG)
                {
                    if(arg.idx == Syntfunc::RETREG)
                        continue;
                    bool isOut = (outRegArnums.count(arnum) > 0);
                    if (subintervals.size() == 0)
                        throw std::string("Liveness analysis: unknown register is referenced on renaming.");
                    size_t linum = 0;
                    if (subintervals[arg.idx].size() > 1)
                        for(; linum < subintervals[arg.idx].size(); linum++)
                        {
                            LiveInterval& li = subintervals[arg.idx][linum];
                            if (opnum >= li.start && opnum <= li.end &&
                               (!isOut || ((linum+1) >= subintervals[arg.idx].size()) || subintervals[arg.idx][linum+1].start > opnum))
                                    break;
                        }
                    if (linum == subintervals[arg.idx].size())
                        throw std::string("Liveness analysis: register isn't active at given line number.");
                    arg.idx = subintervals[arg.idx][linum].idx;
                }
            }
        }
    }

    std::map<IRegInternal, std::pair<size_t, size_t> > result;
    for(auto res : subintervals)
        for(auto pseud:res)
        {
            LiveInterval toAdd(pseud.idx, pseud.start);
            toAdd.end = pseud.end;
            result.insert(std::make_pair(pseud.idx, std::make_pair(toAdd.start,toAdd.end)));
        }
    
    return result;
}

int FuncImpl::invertCondition(int condition) const
{
    return condition == OP_JMP_EQ ? OP_JMP_NE : (
           condition == OP_JMP_NE ? OP_JMP_EQ : (
           condition == OP_JMP_LT ? OP_JMP_GE : (
           condition == OP_JMP_GT ? OP_JMP_LE : (
           condition == OP_JMP_LE ? OP_JMP_GT : (
           condition == OP_JMP_GE ? OP_JMP_LT : condition)))));
}

void FuncImpl::printSyntopBC(const Syntop& op) const
{
    Syntfunc toPrint;
    toPrint.program.push_back(op);
    Printer printer({Printer::colOpnamePrinter(opstrings, opnameoverrules), Printer::colArgListPrinter(argoverrules)});
    printer.print(std::cout, toPrint, false);
}

void FuncImpl::allocateRegisters()
{
    const BackendImpl* backend = getImpl(m_context->getBackend());
    const size_t maximumSpills = 3; //TODO(ch): At this very initial stage I just consider last three available registers as spill-used registers.
    if(backend->registersAmount() < m_data.params.size())
        throw std::string("Register allocator: not enough registers for passing function parameters.");
    const size_t availableSpills = std::min(backend->registersAmount() - m_data.params.size(), maximumSpills); //TODO(ch): This condition will become softer when we will trace livetime of input registers.
    const size_t R = backend->registersAmount() - availableSpills;
    size_t registersUsed = backend->registersAmount();
    std::multiset<LiveInterval, startordering> liveintervals;
    std::multiset<LiveInterval, endordering> parActive;
    {
        std::map<IRegInternal, std::pair<size_t, size_t> > analysisResult = livenessAnalysis();
        registersUsed = std::min(registersUsed, analysisResult.size());
        std::map<IRegInternal, LiveInterval> _liveintervals;
        std::map<IRegInternal, LiveInterval> _liveparams;
        for(auto interval:analysisResult)
            if(interval.first < m_data.params.size())
                _liveparams.insert(std::make_pair(interval.first, LiveInterval(interval)));
            else
                _liveintervals.insert(std::make_pair(interval.first, LiveInterval(interval)));
        for(auto interval : _liveintervals)
            liveintervals.insert(interval.second);
        for(auto interval : _liveparams)
            parActive.insert(interval.second);
    }
    int64_t spoffset = 0;
    std::map<IRegInternal, int64_t> spilled;
    std::map<IRegInternal, IRegInternal> renamingMap;
    { // Looking spilled registers and reusing registers with renaming map.
        for(IRegInternal parreg = 0; parreg < m_data.params.size(); parreg ++)
            renamingMap[parreg] = parreg;
        renamingMap[Syntfunc::RETREG] = Syntfunc::RETREG;
        std::set<IRegInternal> registerPool;
        for(IRegInternal freeReg = m_data.params.size(); freeReg < R; ++freeReg) registerPool.insert(freeReg);
        std::multiset<LiveInterval, endordering> active;
        for(auto interval : liveintervals)
        {
            { //Dropping expired registers.
                {
                    auto removerator = active.begin();
                    for(;removerator != active.end(); ++removerator)
                        if (removerator->end <= interval.start)
                        {
                            if(renamingMap.count(removerator->idx) == 1)
                                registerPool.insert(renamingMap.at(removerator->idx));
                            else
                                registerPool.insert(removerator->idx);
                        }
                        else
                            break;
                    active.erase(active.begin(), removerator);
                }
                {
                    auto removerator = parActive.begin();
                    for(;removerator != parActive.end(); ++removerator)
                        if (removerator->end <= interval.start)
                            registerPool.insert(removerator->idx);
                        else
                            break;
                    parActive.erase(parActive.begin(), removerator);
                }
            }
            if(registerPool.empty())
            {
                if(!active.empty() && active.rbegin()->end > interval.end)
                {
                    renamingMap[interval.idx] = renamingMap[active.rbegin()->idx];
                    renamingMap.erase(active.rbegin()->idx);
                    spilled[active.rbegin()->idx] = spoffset;
                    active.erase(--(active.end()));
                    active.insert(interval);
                }
                else
                    spilled[interval.idx] = spoffset;
                spoffset++;
            }
            else
            {
                active.insert(interval);
                renamingMap[interval.idx] = *registerPool.begin();
                registerPool.erase(registerPool.begin());
            }
        }
    }

    std::vector<Syntop> newProg;
    newProg.reserve(m_data.program.size() * 3);
    backend->writePrologue(m_data, newProg, registersUsed, spilled.size());

    // TODO(ch):
    // 1.) Let's consider sequence of instructions, where it's used one register. Obviously, it can be unspilled only once at start of sequence and
    // spilled only once at end. But for now it will spilled/unpilled at each instruction. I think, this unefficiency can be easily avoided by using some
    // variable-spill map.
    // 2.) Also, we have to take into account live intervals of spilled variables. At some moment place in memory for one variable can be used for
    // another variable.
    {//Renaming registers and adding spill operations
        for(size_t opnum = 0; opnum < m_data.program.size(); ++opnum)
        {
            Syntop op = m_data.program[opnum];
            std::set<IRegInternal> unspilledArgs = backend->getInRegisters(op);
            std::set<IRegInternal> spilledArgs = backend->getOutRegisters(op);
            std::map<IRegInternal,IRegInternal> argRenaming;
            IRegInternal currSpRegIndex = R;
            for(IRegInternal regAr: unspilledArgs)
                if(spilled.count(regAr))
                {
                    argRenaming[regAr] = currSpRegIndex;
                    newProg.push_back(Syntop(OP_UNSPILL, {argIReg(currSpRegIndex, this), spilled[regAr]}));
                    ++currSpRegIndex;
                }
            for(IRegInternal regAr: spilledArgs)
                if(spilled.count(regAr) != 0 && unspilledArgs.count(regAr) == 0)
                {
                    argRenaming[regAr] = currSpRegIndex;
                    ++currSpRegIndex;
                }

            if(availableSpills < (currSpRegIndex - R))
                throw std::string("Register allocator : not enough free registers.");
            for(Arg& ar: op)
                if (ar.tag == Arg::IREG)
                    ar.idx = (argRenaming.count(ar.idx) == 0) ? renamingMap[ar.idx] : argRenaming[ar.idx];
            newProg.push_back(op);
            for(IRegInternal regAr: spilledArgs)
                if(spilled.count(regAr))
                    newProg.push_back(Syntop(OP_SPILL, {spilled[regAr], argIReg(argRenaming[regAr], this)}));
        }
    }
    m_epilogueSize = newProg.size();
    backend->writeEpilogue(m_data, newProg, registersUsed, spilled.size());
    m_epilogueSize = newProg.size() - m_epilogueSize;
    m_data.program = newProg;
}

void FuncImpl::jumpificate()
{
    //TODO(ch): what if return are not on all control pathes(this problem exists for register returns only)??? Think out.
    //          I think, it can be effectively soluted only after deletition of after-jump-silent tails a-la jmp end; mov ..code-without-jumps...; end:
    //          After this we just need to check last operation before end mark. it must be mov to return register.
    //Handle situation, when return is just before end of function (reasonable)
    std::vector<Syntop> newProg;
    newProg.reserve(newProg.size()*2);
    size_t returnLabel = provideLabel();
    bool returnJumps = false;
    size_t bodySize = m_data.program.size() - m_epilogueSize;
    for(size_t opnum = 0; opnum < bodySize; opnum++)
    {
        const Syntop& op = m_data.program[opnum];
        switch (op.opcode) {
            case (OP_IF):
            {
                if (op.size() != 2 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong IF command format");
                newProg.push_back(Syntop(op.args[0].value, {op.args[1].value}));
                break;
            }
            case (OP_ELSE):
            {
                if (op.size() != 2 ||
                    op.args[0].tag != Arg::ICONST ||
                    op.args[1].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong ELIF command format");
                newProg.push_back(Syntop(OP_JMP, {op.args[1].value}));
                newProg.push_back(Syntop(OP_LABEL, {op.args[0].value}));
                break;
            }
            case (OP_ENDIF):
            {
                if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong ENDIF command format");
                newProg.push_back(Syntop(OP_LABEL, {op.args[0].value}));
                break;
            }
            case (OP_DO):
            {
                if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong DO command format");
                newProg.push_back(Syntop(OP_LABEL, {op.args[0].value}));
                break;
            }
            case (OP_WHILE):
            {
                if (op.size() != 4 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST || op.args[2].tag != Arg::ICONST || op.args[3].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong WHILE command format");
                if(op.args[2].value != NOLABEL)
                {
                    Syntop conditionBackup = newProg.back(); //TODO(ch): IMPORTANT(CMPLCOND): This mean that condition can be one-instruction only.
                    newProg.pop_back();
                    newProg.push_back(Syntop(OP_LABEL, {op.args[2].value}));
                    newProg.push_back(conditionBackup);
                }
                newProg.push_back(Syntop(op.args[0].value, {op.args[1].value}));
                if(op.args[3].value != NOLABEL)
                    newProg.push_back(Syntop(OP_LABEL, {op.args[3].value}));
                break;
            }
            case (OP_DOIF):
            {
                if (op.size() != 3 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST || op.args[2].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong WHILE command format");
                Syntop conditionBackup = newProg.back(); //TODO(ch): IMPORTANT(CMPLCOND): This mean that condition can be one-instruction only.
                newProg.pop_back();
                newProg.push_back(Syntop(OP_LABEL, {op.args[1].value}));
                newProg.push_back(conditionBackup);
                newProg.push_back(Syntop(op.args[0].value, {op.args[2].value}));
                break;
            }
            case (OP_ENDDO):
            {
                if (op.size() != 2 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong DO command format");
                newProg.push_back(Syntop(OP_JMP, {op.args[0].value}));
                newProg.push_back(Syntop(OP_LABEL, {op.args[1].value}));
                break;
            }
            case (OP_BREAK):
            {
                if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong BREAK command format");
                newProg.push_back(Syntop(OP_JMP, {op.args[0].value}));
                break;
            }
            case (OP_CONTINUE):
            {
                if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong CONTINUE command format");
                newProg.push_back(Syntop(OP_JMP, {op.args[0].value}));
                break;
            }
            case (OP_RET):
            {
                if(opnum + 1 + m_epilogueSize != m_data.program.size())
                {
                    newProg.push_back(Syntop(OP_JMP, {argIConst(returnLabel, this)}));
                    returnJumps = true;
                }
                break;
            }
            default:
                newProg.push_back(op);
                break;
        }
    }
    if(returnJumps)
        newProg.push_back(Syntop(OP_LABEL, {argIConst(returnLabel, this)}));
    //Write epilogue
    for(size_t opnum = bodySize; opnum < m_data.program.size(); opnum++)
        newProg.push_back(m_data.program[opnum]);
    newProg.push_back(Syntop(OP_RET, {}));
    m_data.program = newProg;
}
};
