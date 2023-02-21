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
    , m_substConditionBypass(false)
{
    
}
void CodeCollecting::loadvec_deinterleave2_(Arg& res1, Arg& res2, const Recipe& base)
{
    Recipe b(base);
    Arg base_ = unpack_recipe(b);
    res1.func = res2.func = m_func;
    res1.idx = m_data.provideIdx(RB_VEC);
    res2.idx = m_data.provideIdx(RB_VEC);
    m_data.program.emplace_back(Syntop(VOP_ARM_LD2, {res1, res2, base_}));
}

void CodeCollecting::process(Syntfunc& a_dest, const Syntfunc& a_source)
{
    if (m_cflowStack.size())
        throw std::runtime_error("Unclosed control flow bracket."); //TODO(ch): Look at stack for providing more detailed information.
}
StageID CodeCollecting::stage_id() const { return CS_COLLECTING; }

Arg CodeCollecting::reg_constr(Recipe& fromwho)
{
    Arg res;
    if(fromwho.opcode() == RECIPE_LEAF)
    {
        std::vector<Arg> args;
        args.reserve(2);
        res.idx = m_data.provideIdx(fromwho.is_vector() ? RB_VEC : RB_INT);
        res.func = m_func;
        res.tag = fromwho.is_vector() ? Arg::VREG : Arg::IREG;
        res.elemtype = fromwho.type();
        args.push_back(res);
        args.push_back(fromwho.leaf());
        Syntop toAdd(OP_MOV, args);
        m_data.program.emplace_back(toAdd);
    }
    else
        res = unpack_recipe(fromwho);
    return res;
}

void CodeCollecting::reg_assign(const Arg& target, Recipe& fromwho)
{
    Arg res;
    Assert((target.tag == Arg::IREG && !fromwho.is_vector()) || (target.tag == Arg::VREG && fromwho.type() == target.elemtype));
    std::vector<Arg> args;
    if(fromwho.opcode() == RECIPE_LEAF)
    {
        args.reserve(2);
        args.push_back(target);
        args.push_back(fromwho.leaf());
        Syntop toAdd(OP_MOV, args);
        m_data.program.emplace_back(toAdd);
    }
    else
    {
        unpack_recipe(fromwho, UR_NONEWIDX);
        m_data.program.back()[0].idx = target.idx;
    }
}

void CodeCollecting::while_(Recipe& r)    //TODO(ch): Implement with jmp-alignops-body-cmp-jmptobody scheme.
{
    Syntop cmpop = unpack_condition(r);
    m_data.program.push_back(Syntop(OP_CMP, {cmpop[0], cmpop[1]}));
    size_t nextPos = m_data.program.size();
    size_t contLabel = m_data.provideLabel();
    size_t brekLabel = Syntfunc::NOLABEL;
    m_cflowStack.push_back(ControlFlowBracket(ControlFlowBracket::WHILE, nextPos));
    int jumptype = comparisson2jumptype(invertCondition(cmpop.opcode));
    if (jumptype == OP_JMP)
        throw std::runtime_error("Temporary condition solution failed."); //TODO(ch): IMPORTANT(CMPLCOND)
    m_data.program.push_back(Syntop(OP_WHILE, {argIImm(jumptype, m_func), argIImm(contLabel, m_func), argIImm(brekLabel, m_func)}));
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
    m_data.program.push_back(Syntop(OP_ENDWHILE, {argIImm(contLabel, m_func), argIImm(brekLabel, m_func)}));
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
    m_data.program.push_back(Syntop(OP_BREAK, {argIImm(0,m_func)}));
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
    m_data.program.push_back(Syntop(OP_CONTINUE, {argIImm(targetLabel,m_func)}));
}

void CodeCollecting::if_(Recipe& r)
{
    Syntop cmpop = unpack_condition(r);
    m_data.program.push_back(Syntop(OP_CMP, {cmpop[0], cmpop[1]}));
    m_cflowStack.push_back(ControlFlowBracket(ControlFlowBracket::IF, m_data.program.size()));
    int jumptype = comparisson2jumptype(invertCondition(cmpop.opcode));
    if (jumptype == OP_JMP)
        throw std::runtime_error("Temporary condition solution failed."); //TODO(ch): IMPORTANT(CMPLCOND)
    //IF(cmp, wrongjmp)
    m_data.program.push_back(Syntop(OP_IF, {argIImm(jumptype, m_func), argIImm(0, m_func)}));
}

void CodeCollecting::elif_(Recipe& r)
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
    else_();
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
    m_data.program.push_back(Syntop(OP_ELSE, {argIImm(label, m_func), argIImm(0, m_func)}));
}

void CodeCollecting::subst_elif(Recipe& r)
{
    m_substConditionBypass = true;
    static int num = 0;
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
    m_data.program.push_back(Syntop(OP_ENDIF, {argIImm(label, m_func)}));
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
    m_data.program.push_back(Syntop(OP_RET, {}));
}

void CodeCollecting::return_(Recipe& retval)
{
    if (m_returnType == RT_VOID)
        throw std::runtime_error("Mixed return types");
    if (retval.is_vector())
        throw std::runtime_error("Vector return is not supported.");
    m_data.program.push_back(Syntop(OP_MOV, {argReg(RB_INT, (int)Syntfunc::RETREG, m_func), unpack_recipe(retval)}));
    m_data.program.push_back(Syntop(OP_RET, {}));
}

Arg CodeCollecting::unpack_recipe(Recipe& rcp, int flags)
{
    Arg res;
    switch(rcp.opcode())
    {
        case (RECIPE_LEAF):
        {
            if(flags & UR_WRAPIIMM && rcp.leaf().tag == Arg::IIMMEDIATE)
            {
                res.idx = flags & UR_NONEWIDX ? 0 : m_data.provideIdx(RB_INT);
                res.func = m_func;
                res.tag = Arg::IREG;
                Syntop toAdd(OP_MOV, {res, rcp.leaf()});
                m_data.program.emplace_back(toAdd);
            }
            else 
                res = rcp.leaf();
            if(res.tag == Arg::IREG || res.tag == Arg::IIMMEDIATE)
                res.elemtype = rcp.type();
            break;
        }
        case (VOP_REINTERPRET):
        {
            Assert(rcp.is_vector() && rcp.children().size() == 1);
            res = unpack_recipe(rcp.children()[0], flags & UR_NONEWIDX);
            res.tag = Arg::VREG;
            res.elemtype = rcp.type();
            break;
        }
        case (OP_SELECT):
        {
            Assert(rcp.children().size() == 3);
            //TODO(ch): In truth, i'm not sure, that select doesn't support immediates on all the archs, so, this usage of UR_WRAPIIMM must be reconsidered
            Arg truev = unpack_recipe(rcp.children()[1], UR_WRAPIIMM);
            Arg falsev = unpack_recipe(rcp.children()[2], UR_WRAPIIMM);
            Syntop cmpop = unpack_condition(rcp.children()[0], UC_ARITHMARGS);
            m_data.program.push_back(Syntop(OP_CMP, {cmpop[0], cmpop[1]}));
            res.idx = flags & UR_NONEWIDX ? 0 : m_data.provideIdx(RB_INT);
            res.func = m_func;
            res.tag = Arg::IREG;
            res.elemtype = rcp.type();
            m_data.program.push_back(Syntop(OP_SELECT, {res, argIImm(cmpop.opcode), truev, falsev}));
            break;
        }
        default:
        {
            std::vector<Arg> args;
            args.reserve(rcp.children().size() + 1);
            res.idx = flags & UR_NONEWIDX ? 0 : m_data.provideIdx(rcp.is_vector() ? RB_VEC : RB_INT );
            res.func = m_func;
            res.tag = rcp.is_vector() ? Arg::VREG : Arg::IREG;
            res.elemtype = rcp.type();
            args.push_back(res);
            for(int child_num = 0; child_num < rcp.children().size(); child_num++)
                args.push_back(unpack_recipe(rcp.children()[child_num]));
            Syntop toAdd(rcp.opcode(), args);
            m_data.program.emplace_back(toAdd);
            break;
        }
    };
    //Duplicated members must be calculated only once, so they are replaced with result after calculation.
    //Since all member mentions are smart pointers to same object, next time unpack algorithm will meet 
    //the member, it will be already calculated.
    if(rcp.opcode() != RECIPE_LEAF) 
        rcp.children().clear();
    rcp.opcode() = RECIPE_LEAF;
    rcp.leaf() = res;
    return res;
}

Syntop CodeCollecting::unpack_condition(Recipe& rcp, int flags)
{
    switch (rcp.opcode())
    {
    case (OP_GT):
    case (OP_UGT):
    case (OP_GE):
    case (OP_LT):
    case (OP_LE):
    case (OP_ULE):
    case (OP_NE):
    case (OP_EQ):
    case (OP_S):
    case (OP_NS):
    {
        Assert(rcp.children().size() == 2);
        if(rcp.children()[0].opcode() != RECIPE_LEAF ||rcp.children()[1].opcode() != RECIPE_LEAF && (flags & UC_ARITHMARGS == 0))
            throw std::runtime_error("Temporary condition solution: conditions other than one comparison of two simple arguments are not supported.");
        std::vector<Arg> args;
        args.reserve(2);
        args.push_back(unpack_recipe(rcp.children()[0]));
        args.push_back(unpack_recipe(rcp.children()[1]));
        Assert(args[0].tag == Arg::IREG || args[1].tag == Arg::IREG);
        return Syntop(rcp.opcode(), args);
    }
    default:
        throw std::runtime_error("Temporary condition solution: conditions other than one comparison of two simple arguments are not supported.");
        break;
    }

}

int CodeCollecting::comparisson2jumptype(int cond)
{
    return cond == OP_EQ  ? OP_JMP_EQ  : (
           cond == OP_NE  ? OP_JMP_NE  : (
           cond == OP_LT  ? OP_JMP_LT  : (
           cond == OP_GT  ? OP_JMP_GT  : (
           cond == OP_UGT ? OP_JMP_UGT : (
           cond == OP_LE  ? OP_JMP_LE  : (
           cond == OP_ULE ? OP_JMP_ULE : (
           cond == OP_GE  ? OP_JMP_GE  : (
           cond == OP_S   ? OP_JMP_S   : (
           cond == OP_NS  ? OP_JMP_NS  : OP_JMP)))))))));
}
};