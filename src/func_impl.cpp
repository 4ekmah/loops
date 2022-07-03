/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "func_impl.hpp"
#include "backend.hpp"
#include "printer.hpp"
#include "reg_allocator.hpp"
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
    {OP_LOAD, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "load." << type_suffixes[op.args[1].value];
    }},
    {OP_STORE, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "store." << type_suffixes[op.args[0].value];
    }},
    {OP_LABEL, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        if (op.size() != 1 || op.args[0].tag != Arg::IIMMEDIATE)
            throw std::runtime_error("Wrong LABEL format");
        str << "label " << op.args[0] << ":";
    }}
};

std::unordered_map<int, Printer::ColPrinter > argoverrules = {
    {OP_LABEL, [](::std::ostream& str, const Syntop& op, size_t, Backend*){}}
};

std::unordered_map<int, std::string> opstrings = { //TODO(ch): will you create at every print?
    {OP_MOV,      "mov"},
    {OP_XCHG,     "xchg"},
    {OP_ADC,      "adc"},
    {OP_ADD,      "add"},
    {OP_MUL,      "mul"},
    {OP_SUB,      "sub"},
    {OP_DIV,      "div"},
    {OP_MOD,      "mod"},
    {OP_SHL,      "shl"},
    {OP_SHR,      "shr"},
    {OP_SAR,      "sar"},
    {OP_AND,      "and"},
    {OP_OR,       "or"},
    {OP_XOR,      "xor"},
    {OP_NOT,      "not"},
    {OP_NEG,      "neg"},
    {OP_CMP,      "cmp"},
    {OP_SELECT,   "select"},
    {OP_MIN,      "min"},
    {OP_MAX,      "max"},
    {OP_ABS,      "abs"},
    {OP_SIGN,     "sign"},
    {OP_X86_CQO,  "x86_cqo"},
    {OP_ARM_CINC, "arm_cinc"},
    {OP_ARM_CNEG, "arm_cneg"},
    {OP_JMP,      "jmp"},
    {OP_JMP_EQ,   "jmp_eq"},
    {OP_JMP_NE,   "jmp_ne"},
    {OP_JMP_GE,   "jmp_ge"},
    {OP_JMP_LE,   "jmp_le"},
    {OP_JMP_GT,   "jmp_gt"},
    {OP_JMP_LT,   "jmp_gt"},
    {OP_SPILL,    "spill"},
    {OP_UNSPILL,  "unspill"},
    {OP_IF,       "annotation:if"},
    {OP_ELSE,     "annotation:else"},
    {OP_ENDIF,    "annotation:endif"},
    {OP_WHILE,    "annotation:while"},
    {OP_ENDWHILE, "annotation:endwhile"},
    {OP_BREAK,    "annotation:break"},
    {OP_CONTINUE, "annotation:continue"},
    {OP_RET,      "ret"},
};

FuncImpl::FuncImpl(const std::string& name, Context* ctx, std::initializer_list<IReg*> params) : m_refcount(0)
    , m_nextIdx(0)
    , m_nextLabelIdx(0)
    , m_context(getImpl(ctx))
    , m_returnType(RT_NOTDEFINED)
    , m_compiled(nullptr)
    , m_cmpopcode(IC_UNKNOWN)
{
    m_data.name = name;
    m_data.params.reserve(params.size());
    for (IReg* parreg : params)
    {
        if(parreg->func != nullptr || parreg->idx != IReg::NOIDX)
            throw std::runtime_error("Parameter index is already initilized in some other function");
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
    Printer printer({Printer::colNumPrinter(0), Printer::colOpnamePrinter(opstrings, opnameoverrules), Printer::colArgListPrinter(m_data, argoverrules)});
    printer.print(out, m_data);
}

void FuncImpl::printAssembly(std::ostream& out, int columns) const
{
    Backend* backend = m_context->getBackend();
    Syntfunc tarcode = backend->bytecode2Target(m_data);
    std::vector<Printer::ColPrinter> columnPrs;
    columnPrs.reserve(3);
    if(columns&PC_OPNUM)
        columnPrs.push_back(Printer::colNumPrinter(0));
    if(columns&PC_OP)
    {
        columnPrs.push_back(Printer::colOpnamePrinter(backend->getOpStrings()));
        columnPrs.push_back(Printer::colArgListPrinter(tarcode));
    }
    if(columns&PC_HEX)
    {
        if(columns&PC_OP)
            columnPrs.push_back(Printer::colDelimeterPrinter());
        columnPrs.push_back(backend->colHexPrinter(tarcode));
    }
    Printer printer(columnPrs);
    printer.setBackend(backend);
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
        if (arg.func != nullptr)
        {
            if (func == nullptr)
                func = static_cast<FuncImpl*>(arg.func);
            else if(func != static_cast<FuncImpl*>(arg.func))
                throw std::runtime_error("Registers of different functions as arguments of one instruction.");
        }
    if (func == nullptr)
        throw std::runtime_error("Cannot find mother function in registers.");
    return func;
}

void FuncImpl::endfunc(bool directTranslation)
{
    if (!directTranslation)
    {
        if (m_cflowStack.size())
            throw std::runtime_error("Unclosed control flow bracket."); //TODO(ch): Look at stack for providing more detailed information.
        //TODO(ch): block somehow adding new instruction after this call.
        m_context->getRegisterAllocator()->process(this, m_data, m_nextIdx);

        controlBlocks2Jumps();
        auto afterRegAlloc = m_context->getBackend()->getAfterRegAllocStages();
        for (CompilerStagePtr araStage : afterRegAlloc)
            araStage->process(m_data);
    }
}

IReg FuncImpl::const_(int64_t value)
{
    return static_cast<IReg&&>(newiop(OP_MOV, { argIImm(value, this) }));
}

void FuncImpl::while_(const IReg& r)    //TODO(ch): Implement with jmp-alignops-body-cmp-jmptobody scheme.
{
    size_t nextPos = m_data.program.size();
    size_t contLabel = provideLabel();
    size_t brekLabel = NOLABEL;
    m_cflowStack.push_back(ControlFlowBracket(ControlFlowBracket::WHILE, nextPos));
    int jumptype = condition2jumptype(invertCondition(m_cmpopcode));
    if (jumptype == OP_JMP)
        throw std::runtime_error("Temporary condition solution failed."); //TODO(ch): IMPORTANT(CMPLCOND)
    newiopNoret(OP_WHILE, {argIImm(jumptype, this), argIImm(contLabel, this), argIImm(brekLabel, this)});
    m_cmpopcode = IC_UNKNOWN;
}

void FuncImpl::endwhile_()
{
    if (m_cflowStack.size() == 0)
        throw std::runtime_error("Unclosed control flow bracket: there is no \"while\" for \"endwhile\".");
    auto bracket = m_cflowStack.back();
    m_cflowStack.pop_back();
    if (bracket.tag != ControlFlowBracket::WHILE)
        throw std::runtime_error("Control flow bracket error: expected corresponding \"while\" for \"endwhile\".");
    size_t nextPos = m_data.program.size();
    size_t whilePos = bracket.labelOrPos;
    if(whilePos >= nextPos)
        throw std::runtime_error("\"While\" internal error: wrong branch start address.");
    Syntop& whileOp = m_data.program[whilePos];
    if (whileOp.opcode!= OP_WHILE || whileOp.size() != 3 || whileOp[0].tag != Arg::IIMMEDIATE || whileOp[1].tag != Arg::IIMMEDIATE || whileOp[2].tag != Arg::IIMMEDIATE)
        throw std::runtime_error("\"While\" internal error: wrong command format.");
    size_t brekLabel = provideLabel();
    whileOp[2].value = brekLabel;
    size_t contLabel = whileOp[1].value;
    for(size_t bre : bracket.breaks)
    {
        if (bre >= m_data.program.size())
            throw std::runtime_error("\"While\" internal error: wrong \"break\" address.");
        Syntop& breop = m_data.program[bre];
        if (breop.opcode != OP_BREAK || breop.size() != 1 || breop.args[0].tag != Arg::IIMMEDIATE)
            throw std::runtime_error("\"While\" internal error: wrong \"break\" command format.");
        breop[0].value = brekLabel;
    }
    newiopNoret(OP_ENDWHILE, {argIImm(contLabel, this), argIImm(brekLabel, this)});
}

void FuncImpl::break_()
{
    auto rator = m_cflowStack.rbegin();
    for(; rator != m_cflowStack.rend(); ++rator)
        if(rator->tag == ControlFlowBracket::WHILE)
            break;
    size_t nextPos = m_data.program.size();
    if (rator == m_cflowStack.rend())
        throw std::runtime_error("Unclosed control flow bracket: there is no \"while\" for \"break\".");
    rator->breaks.push_back(nextPos);
    newiopNoret(OP_BREAK, {argIImm(0,this)});
}

void FuncImpl::continue_()
{
    auto rator = m_cflowStack.rbegin();
    for(; rator != m_cflowStack.rend(); ++rator)
        if(rator->tag == ControlFlowBracket::WHILE)
            break;
    if (rator == m_cflowStack.rend())
        throw std::runtime_error("Unclosed control flow bracket: there is no \"while\" for \"break\".");
    size_t nextPos = m_data.program.size();
    size_t targetLabel = 0;
    if(rator->tag == ControlFlowBracket::WHILE)
    {
        size_t whilePos = rator->labelOrPos;
        if(whilePos >= m_data.program.size())
            throw std::runtime_error("\"While\" internal error: wrong branch start address.");
        Syntop& whileOp = m_data.program[whilePos];
        if (whileOp.opcode!= OP_WHILE || whileOp.size() != 3 || whileOp[0].tag != Arg::IIMMEDIATE || whileOp[1].tag != Arg::IIMMEDIATE || whileOp[2].tag != Arg::IIMMEDIATE)
            throw std::runtime_error("\"While\" internal error: wrong command format.");
        targetLabel = whileOp[1].value;
    }
    else
        rator->continues.push_back(nextPos);
    newiopNoret(OP_CONTINUE, {argIImm(targetLabel,this)});
}

void FuncImpl::if_(const IReg& r)
{
    m_cflowStack.push_back(ControlFlowBracket(ControlFlowBracket::IF, m_data.program.size()));
    int jumptype = condition2jumptype(invertCondition(m_cmpopcode));
    if (jumptype == OP_JMP)
        throw std::runtime_error("Temporary condition solution failed."); //TODO(ch): IMPORTANT(CMPLCOND)
    //IF(cmp, wrongjmp)
    newiopNoret(OP_IF, {argIImm(jumptype, this), argIImm(0, this)});
    m_cmpopcode = IC_UNKNOWN;
}

void FuncImpl::elif_(const IReg& r)
{
    size_t elifRep = 0;
    {
        if (m_cflowStack.size() == 0)
            throw std::runtime_error("Unclosed control flow bracket: there is no \"if\", for \"elif\".");
        ControlFlowBracket& bracket = m_cflowStack.back();
        if (bracket.tag != ControlFlowBracket::IF)
            throw std::runtime_error("Control flow bracket error: expected corresponding \"if\", for \"elif\".");
        elifRep = bracket.elifRepeats;
    }
    Syntop conditionBackup = m_data.program.back(); //TODO(ch): IMPORTANT(CMPLCOND): This mean that condition can be one-instruction only.
    m_data.program.pop_back();
    else_();
    m_data.program.push_back(conditionBackup);
    if_(r);
    if (m_cflowStack.size() == 0)
        throw std::runtime_error("Unclosed control flow bracket: there is no \"if\", for \"elif\".");
    ControlFlowBracket& bracket = m_cflowStack.back();
    if (bracket.tag != ControlFlowBracket::IF)
        throw std::runtime_error("Control flow bracket error: expected corresponding \"if\", for \"elif\".");
    bracket.elifRepeats = elifRep + 1;
}

void FuncImpl::else_()
{
    if (m_cflowStack.size() == 0)
        throw std::runtime_error("Unclosed control flow bracket: there is no \"if\", for \"else\".");
    ControlFlowBracket& bracket = m_cflowStack.back();
    if (bracket.tag != ControlFlowBracket::IF)
        throw std::runtime_error("Control flow bracket error: expected corresponding \"if\", for \"else\".");
    size_t posnext = m_data.program.size();
    size_t prevBranchPos = bracket.labelOrPos;
    m_cflowStack.push_back(ControlFlowBracket(ControlFlowBracket::ELSE, m_data.program.size()));
    if (prevBranchPos >= posnext)
        throw std::runtime_error("\"If\" internal error: wrong branch start address");
    const Syntop& ifop = m_data.program[prevBranchPos];
    if (ifop.opcode != OP_IF || ifop.size() != 2 || ifop.args[1].tag != Arg::IIMMEDIATE)
        throw std::runtime_error("\"If\" internal error: wrong \"if\" command format");
    size_t label = provideLabel();
    m_data.program[prevBranchPos].args[1].value = label;
    m_ifLabelMap[label] = std::pair<size_t, size_t>(prevBranchPos, 0);
    //ELSE(mylabel, endjmp)
    newiopNoret(OP_ELSE, {argIImm(label, this), argIImm(0, this)});
}

void FuncImpl::subst_elif(const IReg& r)
{
    static int num = 0;
    Syntop conditionBackup = m_data.program.back(); //TODO(ch): IMPORTANT(CMPLCOND): This mean that condition can be one-instruction only.
    m_data.program.pop_back();
    size_t elifRep = 0;
    size_t restoredElifRep = 0;
    bool gotElifRepeats = false;
    do
    {
        if(m_data.program.size() == 0 || m_data.program.back().opcode != OP_ENDIF)
            throw std::runtime_error("Control flow bracket error: there is no \"if\" or \"elif\", for \"else\" or \"elif\".");
        Assert(m_data.program.back()[0].tag == Arg::IIMMEDIATE);
        size_t label = m_data.program.back()[0].value;
        size_t ifPos = m_ifLabelMap[label].first;
        size_t elsePos = -1;
        elifRep = gotElifRepeats ? elifRep - 1: m_ifLabelMap[label].second ;
        gotElifRepeats = true;
        m_ifLabelMap.erase(label);
        m_data.program.pop_back();
        Assert(ifPos < m_data.program.size() && (m_data.program[ifPos].opcode == OP_IF || m_data.program[ifPos].opcode == OP_ELSE));
        if(m_data.program[ifPos].opcode == OP_ELSE)
        {
            Assert(m_data.program[ifPos].size() && m_data.program[ifPos][0].tag == Arg::IIMMEDIATE);
            size_t label = m_data.program[ifPos][0].value;
            elsePos = ifPos;
            ifPos = m_ifLabelMap[label].first;
        }
        m_cflowStack.push_back(ControlFlowBracket(ControlFlowBracket::IF, ifPos));
        m_cflowStack.back().elifRepeats = restoredElifRep++;
        if(elsePos != -1)
            m_cflowStack.push_back(ControlFlowBracket(ControlFlowBracket::ELSE, elsePos));
    } while(elifRep != 0);
    m_data.program.push_back(conditionBackup);
    elif_(r);
}

void FuncImpl::subst_else()
{
    size_t elifRep = 0;
    size_t restoredElifRep = 0;
    bool gotElifRepeats = false;
    do
    {
        if(m_data.program.size() == 0 || m_data.program.back().opcode != OP_ENDIF)
            throw std::runtime_error("Control flow bracket error: there is no \"if\" or \"elif\", for \"else\" or \"elif\".");
        Assert(m_data.program.back()[0].tag == Arg::IIMMEDIATE);
        size_t label = m_data.program.back()[0].value;
        size_t ifPos = m_ifLabelMap[label].first;
        size_t elsePos = -1;
        elifRep = gotElifRepeats ? elifRep - 1: m_ifLabelMap[label].second ;
        gotElifRepeats = true;
        m_ifLabelMap.erase(label);
        m_data.program.pop_back();
        Assert(ifPos < m_data.program.size() && (m_data.program[ifPos].opcode == OP_IF || m_data.program[ifPos].opcode == OP_ELSE));
        if(m_data.program[ifPos].opcode == OP_ELSE)
        {
            Assert(m_data.program[ifPos].size() && m_data.program[ifPos][0].tag == Arg::IIMMEDIATE);
            size_t label = m_data.program[ifPos][0].value;
            elsePos = ifPos;
            ifPos = m_ifLabelMap[label].first;
        }
        m_cflowStack.push_back(ControlFlowBracket(ControlFlowBracket::IF, ifPos));
        m_cflowStack.back().elifRepeats = restoredElifRep++;
        if(elsePos != -1)
            m_cflowStack.push_back(ControlFlowBracket(ControlFlowBracket::ELSE, elsePos));
    } while(elifRep != 0);
    else_();
}

void FuncImpl::endif_()
{
    if (m_cflowStack.size() == 0)
        throw std::runtime_error("Unclosed control flow bracket: there is no \"if\", \"elif\" or \"else\", for \"endif\".");
    ControlFlowBracket bracket = m_cflowStack.back();
    m_cflowStack.pop_back();
    size_t posnext = m_data.program.size();
    size_t label = provideLabel();
    bool rewriteNEAddress = true;
    if (bracket.tag == ControlFlowBracket::ELSE)
    {
        rewriteNEAddress = false;
        size_t elsePos = bracket.labelOrPos;
        if (elsePos >= posnext)
            throw std::runtime_error("\"If\" internal error: wrong \"else\" start address");
        const Syntop& elseOp = m_data.program[elsePos];
        if (elseOp.opcode != OP_ELSE || elseOp.size() != 2 || elseOp.args[1].tag != Arg::IIMMEDIATE)
            throw std::runtime_error("\"If\" internal error: wrong \"else\" command format");
        m_data.program[elsePos].args[1].value = label;
        m_ifLabelMap[label] = std::pair<size_t, size_t>(elsePos, 0);
        if (m_cflowStack.size() == 0)
            throw std::runtime_error("Unclosed control flow bracket: there is no \"if\", \"elif\" or \"else\", for \"endif\".");
        bracket = m_cflowStack.back();
        m_cflowStack.pop_back();
    }
    if (bracket.tag != ControlFlowBracket::IF)
        throw std::runtime_error("Control flow bracket error: expected corresponding \"if\", \"elif\" or \"else\" for \"endif\".");

    size_t ifPos = bracket.labelOrPos;
    if (ifPos >= posnext)
        throw std::runtime_error("\"If\" internal error: wrong \"if\" start address");
    const Syntop& ifOp = m_data.program[ifPos];
    if (ifOp.opcode != OP_IF || ifOp.size() != 2 || ifOp.args[1].tag != Arg::IIMMEDIATE)
        throw std::runtime_error("\"If\" internal error: wrong \"else\" command format");
    if(rewriteNEAddress)
    {
        m_ifLabelMap[label] = std::pair<size_t, size_t>(ifPos, 0);
        m_data.program[ifPos].args[1].value = label;
    }
    newiopNoret(OP_ENDIF, {argIImm(label, this)});
    if(bracket.elifRepeats != 0)
    {
        endif_();
        size_t justAddedIfPos = m_ifLabelMap[label + bracket.elifRepeats].first;
        m_ifLabelMap[label + bracket.elifRepeats] = std::pair<size_t, size_t>(justAddedIfPos, bracket.elifRepeats);
    }
}

void FuncImpl::return_()
{
    if (m_returnType == RT_REGISTER)
        throw std::runtime_error("Mixed return types");
    newiopNoret(OP_RET, {});
}

void FuncImpl::return_(int64_t retval)
{
    if (m_returnType == RT_VOID)
        throw std::runtime_error("Mixed return types");
    newiopNoret(OP_MOV, { argIReg(Syntfunc::RETREG, this), Arg(retval) });
    newiopNoret(OP_RET, {});
}

void FuncImpl::return_(const IReg& retval)
{
    if (m_returnType == RT_VOID)
        throw std::runtime_error("Mixed return types");
    newiopNoret(OP_MOV, {argIReg(Syntfunc::RETREG, this), retval});
    newiopNoret(OP_RET, {});
}

IReg FuncImpl::select(const IReg& cond, const IReg& truev, const IReg& falsev)
{
    if (m_cmpopcode >= IC_UNKNOWN)                        //TODO(ch): IMPORTANT(CMPLCOND)
        throw std::runtime_error("Select: first argument must be condition.");
    Arg cmpop = Arg(m_cmpopcode);
    m_cmpopcode = OP_JMP;
    return newiop(OP_SELECT, {cmpop, truev, falsev});
}

void FuncImpl::printSyntopBC(const Syntop& op) const
{
    Syntfunc toPrint;
    toPrint.program.push_back(op);
    Printer printer({Printer::colOpnamePrinter(opstrings, opnameoverrules), Printer::colArgListPrinter(m_data, argoverrules)});
    printer.print(std::cout, toPrint, false);
}

void FuncImpl::immediateImplantationAttempt(Syntop& op, size_t anumAdd, ::std::initializer_list<size_t> tryImmList)
{
    Backend* backend = m_context->getBackend();
    std::vector<size_t> arnums;
    arnums.reserve(tryImmList.size());
    for(size_t arnum : tryImmList)
        arnums.push_back(arnum + anumAdd);
    std::sort(arnums.begin(), arnums.end());
    std::set<IRegInternal> usedRegs;
    for (const Arg& ar : op)
        if (ar.tag == Arg::IREG)
            usedRegs.insert(ar.idx);
    std::vector<Arg> attempts;
    attempts.reserve(arnums.size());
    IRegInternal placeholderTop = 0;
    for (size_t arNum : arnums)
    {
        Assert(op[arNum].tag == Arg::IIMMEDIATE);
        attempts.push_back(op[arNum]);
        while (usedRegs.count(placeholderTop)) placeholderTop++;
        op[arNum] = argIReg(placeholderTop++, this);
    }
    for (size_t attemptN = 0; attemptN < arnums.size(); attemptN++)
    {
        size_t arNum = arnums[attemptN];
        op[arNum] = attempts[attemptN];
        if (!backend->isImmediateFit(op, arNum))
            op[arNum] = argIReg(const_(attempts[attemptN].value).idx, this);
    }
};


void FuncImpl::controlBlocks2Jumps()
{
    //TODO(ch): what if return are not on all control pathes(this problem exists for register returns only)??? Think out.
    //          I think, it can be effectively soluted only after deletition of after-jump-silent tails a-la jmp end; mov ..code-without-jumps...; end:
    //          After this we just need to check last operation before end mark. it must be mov to return register.
    //Handle situation, when return is just before end of function (reasonable)
    std::vector<Syntop> newProg;
    newProg.reserve(newProg.size()*2);
    size_t returnLabel = provideLabel();
    bool returnJumps = false;
    size_t epilogueSize = m_context->getRegisterAllocator()->epilogueSize();
    size_t bodySize = m_data.program.size() - epilogueSize;
    for(size_t opnum = 0; opnum < bodySize; opnum++)
    {
        const Syntop& op = m_data.program[opnum];
        switch (op.opcode) {
            case (OP_IF):
            {
                if (op.size() != 2 || op.args[0].tag != Arg::IIMMEDIATE || op.args[1].tag != Arg::IIMMEDIATE)
                   throw std::runtime_error("Internal error: wrong IF command format");
                newProg.push_back(Syntop(static_cast<int>(op.args[0].value), {op.args[1].value}));
                break;
            }
            case (OP_ELSE):
            {
                if (op.size() != 2 ||
                    op.args[0].tag != Arg::IIMMEDIATE ||
                    op.args[1].tag != Arg::IIMMEDIATE)
                   throw std::runtime_error("Internal error: wrong ELIF command format");
                newProg.push_back(Syntop(OP_JMP, {op.args[1].value}));
                newProg.push_back(Syntop(OP_LABEL, {op.args[0].value}));
                break;
            }
            case (OP_ENDIF):
            {
                if (op.size() != 1 || op.args[0].tag != Arg::IIMMEDIATE)
                   throw std::runtime_error("Internal error: wrong ENDIF command format");
                newProg.push_back(Syntop(OP_LABEL, {op.args[0].value}));
                break;
            }
            case (OP_WHILE):
            {
                if (op.size() != 3 || op.args[0].tag != Arg::IIMMEDIATE || op.args[1].tag != Arg::IIMMEDIATE || op.args[2].tag != Arg::IIMMEDIATE)
                   throw std::runtime_error("Internal error: wrong WHILE command format");
                std::vector<Syntop> conditionBackup(newProg.back().spillPrefix + 1);
                newProg.insert(newProg.end() - 1 - newProg.back().spillPrefix,  Syntop(OP_LABEL, {op.args[1].value}));//TODO(ch): IMPORTANT(CMPLCOND): This mean that condition can be one-instruction only.
                newProg.push_back(Syntop(static_cast<int>(op.args[0].value), {op.args[2].value}));
                break;
            }
            case (OP_ENDWHILE):
            {
                if (op.size() != 2 || op.args[0].tag != Arg::IIMMEDIATE || op.args[1].tag != Arg::IIMMEDIATE)
                   throw std::runtime_error("Internal error: wrong DO command format");
                newProg.push_back(Syntop(OP_JMP, {op.args[0].value}));
                newProg.push_back(Syntop(OP_LABEL, {op.args[1].value}));
                break;
            }
            case (OP_BREAK):
            {
                if (op.size() != 1 || op.args[0].tag != Arg::IIMMEDIATE)
                   throw std::runtime_error("Internal error: wrong BREAK command format");
                newProg.push_back(Syntop(OP_JMP, {op.args[0].value}));
                break;
            }
            case (OP_CONTINUE):
            {
                if (op.size() != 1 || op.args[0].tag != Arg::IIMMEDIATE)
                   throw std::runtime_error("Internal error: wrong CONTINUE command format");
                newProg.push_back(Syntop(OP_JMP, {op.args[0].value}));
                break;
            }
            case (OP_RET):
            {
                if(opnum + 1 + epilogueSize != m_data.program.size())
                {
                    newProg.push_back(Syntop(OP_JMP, {argIImm(returnLabel, this)}));
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
        newProg.push_back(Syntop(OP_LABEL, {argIImm(returnLabel, this)}));
    //Write epilogue
    for(size_t opnum = bodySize; opnum < m_data.program.size(); opnum++)
        newProg.push_back(m_data.program[opnum]);
    newProg.push_back(Syntop(OP_RET, {}));
    m_data.program = newProg;
}

int FuncImpl::condition2jumptype(int cond)
{
    return cond == IC_EQ ? OP_JMP_EQ : (
           cond == IC_NE ? OP_JMP_NE : (
           cond == IC_LT ? OP_JMP_LT : (
           cond == IC_GT ? OP_JMP_GT : (
           cond == IC_LE ? OP_JMP_LE : (
           cond == IC_GE ? OP_JMP_GE : OP_JMP )))));
         //cond == IC_S  ? OP_JMP_S  : (
         //cond == IC_NS ? OP_JMP_NS : IC_UNKNOWN)))))));
}

};
