/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "code_collecting.hpp"
#include "func_impl.hpp"
#include "backend.hpp"
#include "pipeline.hpp"
#include "printer.hpp"
#include "reg_allocator.hpp"
#include <algorithm>
#include <iomanip>
#include <deque>
#include <iostream>

namespace loops
{

CodeCollecting::CodeCollecting(Syntfunc& a_data, Func* a_func) : CompilerStage(((FuncImpl*)a_func)->getContext()->getBackend())
    , m_data(a_data)
    , m_func(a_func)
    , m_returnType(RT_NOTDEFINED)
    , m_cmpopcode(IC_UNKNOWN)
    , m_conditionStart(-1)
    , m_substConditionBypass(false)
{
    
}

void CodeCollecting::process(Syntfunc& a_dest, const Syntfunc& a_source)
{
    if (m_cflowStack.size())
        throw std::runtime_error("Unclosed control flow bracket."); //TODO(ch): Look at stack for providing more detailed information.
}
StageID CodeCollecting::stage_id() const { return CS_COLLECTING; }

IReg CodeCollecting::const_(int64_t value)
{
    return static_cast<IReg&&>(newiop(OP_MOV, { argIImm(value, m_func) }));
}

IReg CodeCollecting::def_()
{
    return static_cast<IReg&&>(newiop(OP_DEF, {}));
}

void CodeCollecting::while_(const IReg& r)    //TODO(ch): Implement with jmp-alignops-body-cmp-jmptobody scheme.
{
    if (m_conditionStart + 1 != m_data.program.size()) //TODO(ch): IMPORTANT(CMPLCOND)
        throw std::runtime_error("Temporary condition solution: conditions bigger than one comparison of two simple arguments are not supported.");
    m_conditionStart = -1;
    size_t nextPos = m_data.program.size();
    size_t contLabel = m_data.provideLabel();
    size_t brekLabel = Syntfunc::NOLABEL;
    m_cflowStack.push_back(ControlFlowBracket(ControlFlowBracket::WHILE, nextPos));
    int jumptype = condition2jumptype(invertCondition(m_cmpopcode));
    if (jumptype == OP_JMP)
        throw std::runtime_error("Temporary condition solution failed."); //TODO(ch): IMPORTANT(CMPLCOND)
    newiopNoret(OP_WHILE, {argIImm(jumptype, m_func), argIImm(contLabel, m_func), argIImm(brekLabel, m_func)});
    m_cmpopcode = IC_UNKNOWN;
}

void CodeCollecting::endwhile_()
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
    size_t brekLabel = m_data.provideLabel();
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
    newiopNoret(OP_ENDWHILE, {argIImm(contLabel, m_func), argIImm(brekLabel, m_func)});
}

void CodeCollecting::break_()
{
    auto rator = m_cflowStack.rbegin();
    for(; rator != m_cflowStack.rend(); ++rator)
        if(rator->tag == ControlFlowBracket::WHILE)
            break;
    size_t nextPos = m_data.program.size();
    if (rator == m_cflowStack.rend())
        throw std::runtime_error("Unclosed control flow bracket: there is no \"while\" for \"break\".");
    rator->breaks.push_back(nextPos);
    newiopNoret(OP_BREAK, {argIImm(0,m_func)});
}

void CodeCollecting::continue_()
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
    newiopNoret(OP_CONTINUE, {argIImm(targetLabel,m_func)});
}

void CodeCollecting::if_(const IReg& r)
{
    if (!m_substConditionBypass && ((m_conditionStart + 1) != m_data.program.size())) //TODO(ch): IMPORTANT(CMPLCOND)
        throw std::runtime_error("Temporary condition solution: conditions bigger than one comparison of two simple arguments are not supported.");
    m_conditionStart = -1;
    m_cflowStack.push_back(ControlFlowBracket(ControlFlowBracket::IF, m_data.program.size()));
    int jumptype = condition2jumptype(invertCondition(m_cmpopcode));
    if (jumptype == OP_JMP)
        throw std::runtime_error("Temporary condition solution failed."); //TODO(ch): IMPORTANT(CMPLCOND)
    //IF(cmp, wrongjmp)
    newiopNoret(OP_IF, {argIImm(jumptype, m_func), argIImm(0, m_func)});
    m_cmpopcode = IC_UNKNOWN;
}

void CodeCollecting::elif_(const IReg& r)
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

void CodeCollecting::else_()
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
    size_t label = m_data.provideLabel();
    m_data.program[prevBranchPos].args[1].value = label;
    m_ifLabelMap[label] = std::pair<size_t, size_t>(prevBranchPos, 0);
    //ELSE(mylabel, endjmp)
    newiopNoret(OP_ELSE, {argIImm(label, m_func), argIImm(0, m_func)});
}

void CodeCollecting::subst_elif(const IReg& r)
{
    if (m_conditionStart + 1 != m_data.program.size()) //TODO(ch): IMPORTANT(CMPLCOND)
        throw std::runtime_error("Temporary condition solution: conditions bigger than one comparison of two simple arguments are not supported.");
    m_conditionStart = -1;
    m_substConditionBypass = true;
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
    m_substConditionBypass = false;
}

void CodeCollecting::subst_else()
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

void CodeCollecting::endif_()
{
    if (m_cflowStack.size() == 0)
        throw std::runtime_error("Unclosed control flow bracket: there is no \"if\", \"elif\" or \"else\", for \"endif\".");
    ControlFlowBracket bracket = m_cflowStack.back();
    m_cflowStack.pop_back();
    size_t posnext = m_data.program.size();
    size_t label = m_data.provideLabel();
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
    newiopNoret(OP_ENDIF, {argIImm(label, m_func)});
    if(bracket.elifRepeats != 0)
    {
        endif_();
        size_t justAddedIfPos = m_ifLabelMap[label + bracket.elifRepeats].first;
        m_ifLabelMap[label + bracket.elifRepeats] = std::pair<size_t, size_t>(justAddedIfPos, bracket.elifRepeats);
    }
}

void CodeCollecting::return_()
{
    if (m_returnType == RT_REGISTER)
        throw std::runtime_error("Mixed return types");
    newiopNoret(OP_RET, {});
}

void CodeCollecting::return_(int64_t retval)
{
    if (m_returnType == RT_VOID)
        throw std::runtime_error("Mixed return types");
    newiopNoret(OP_MOV, { argReg(RB_INT, (int)Syntfunc::RETREG, m_func), Arg(retval) });
    newiopNoret(OP_RET, {});
}

void CodeCollecting::return_(const IReg& retval)
{
    if (m_returnType == RT_VOID)
        throw std::runtime_error("Mixed return types");
    newiopNoret(OP_MOV, {argReg(RB_INT, (int)Syntfunc::RETREG, m_func), retval});
    newiopNoret(OP_RET, {});
}

void CodeCollecting::markConditionStart()
{
    m_conditionStart = m_data.program.size();
}

IReg CodeCollecting::select(const IReg& cond, const IReg& truev, const IReg& falsev)
{
    if (m_cmpopcode >= IC_UNKNOWN)                        //TODO(ch): IMPORTANT(CMPLCOND)
        throw std::runtime_error("Select: first argument must be condition.");
    Arg cmpop = Arg(m_cmpopcode);
    m_cmpopcode = OP_JMP;
    return newiop(OP_SELECT, {cmpop, truev, falsev});
}

void CodeCollecting::immediateImplantationAttempt(Syntop& op, size_t anumAdd, ::std::initializer_list<size_t> tryImmList)
{
    std::vector<size_t> arnums;
    arnums.reserve(tryImmList.size());
    for(size_t arnum : tryImmList)
        arnums.push_back(arnum + anumAdd);
    std::sort(arnums.begin(), arnums.end());
    std::set<RegIdx> usedRegs;
    for (const Arg& ar : op)
        if (ar.tag == Arg::IREG)
            usedRegs.insert(ar.idx);
    std::vector<Arg> attempts;
    attempts.reserve(arnums.size());
    RegIdx placeholderTop = 0;
    for (size_t arNum : arnums)
    {
        Assert(op[arNum].tag == Arg::IIMMEDIATE);
        attempts.push_back(op[arNum]);
        while (usedRegs.count(placeholderTop)) placeholderTop++;
        op[arNum] = argReg(RB_INT, placeholderTop++, m_func);
    }
    for (size_t attemptN = 0; attemptN < arnums.size(); attemptN++)
    {
        size_t arNum = arnums[attemptN];
        op[arNum] = attempts[attemptN];
        if (!m_backend->isImmediateFit(op, arNum))
            op[arNum] = argReg(RB_INT, const_(attempts[attemptN].value).idx, m_func);
    }
};

int CodeCollecting::condition2jumptype(int cond)
{
    return cond == IC_EQ  ? OP_JMP_EQ  : (
           cond == IC_NE  ? OP_JMP_NE  : (
           cond == IC_LT  ? OP_JMP_LT  : (
           cond == IC_GT  ? OP_JMP_GT  : (
           cond == IC_UGT ? OP_JMP_UGT : (
           cond == IC_LE  ? OP_JMP_LE  : (
           cond == IC_ULE ? OP_JMP_ULE : (
           cond == IC_GE  ? OP_JMP_GE  : OP_JMP )))))));
         //cond == IC_S   ? OP_JMP_S   : (
         //cond == IC_NS  ? OP_JMP_NS  : IC_UNKNOWN)))))));
}
};