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
#include <unordered_set>
#include <map>
#include <iomanip>
#include <deque>
#include <iostream>

namespace loops
{

    CodeCollecting::CodeCollecting(Syntfunc &a_data, Func *a_func) : CompilerPass(((FuncImpl *)a_func)->getContext()->getBackend()), m_data(a_data), m_func(a_func), m_returnType(RT_NOTDEFINED)
    {
    }
    void CodeCollecting::loadvec_deinterleave2_(Arg &res1, Arg &res2, const Recipe &base)
    {
        Recipe b(base);
        Arg base_ = unpack_recipe(b);
        res1.idx = m_data.provideIdx(RB_VEC);
        res2.idx = m_data.provideIdx(RB_VEC);
        m_data.program.emplace_back(Syntop(VOP_ARM_LD2, {res1, res2, base_}));
    }

    void CodeCollecting::process(Syntfunc &a_dest, const Syntfunc &a_source)
    {
        if (m_cflowStack.size())
            throw std::runtime_error("Unclosed control flow bracket."); // TODO(ch): Look at stack for providing more detailed information.
    }
    PassID CodeCollecting::pass_id() const { return CP_COLLECTING; }

    Arg CodeCollecting::reg_constr(Recipe &fromwho)
    {
        Arg res;
        if (fromwho.is_leaf())
        {
            std::vector<Arg> args;
            args.reserve(2);
            res.idx = m_data.provideIdx(fromwho.is_vector() ? RB_VEC : RB_INT);
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

    void CodeCollecting::reg_assign(Recipe& target, Recipe &fromwho)
    {
        Arg res;
        Assert(target.is_leaf() && ((target.leaf().tag == Arg::IREG && !fromwho.is_vector()) || (target.leaf().tag == Arg::VREG && fromwho.type() == target.leaf().elemtype)));
        std::vector<Arg> args;
        if (fromwho.is_leaf())
        {
            args.reserve(2);
            args.push_back(target.leaf());
            args.push_back(fromwho.leaf());
            Syntop toAdd(OP_MOV, args);
            m_data.program.emplace_back(toAdd);
        }
        else
        {
            unpack_recipe(fromwho, UR_NONEWIDX);
            m_data.program.back()[0].idx = target.leaf().idx;
        }
    }

    void CodeCollecting::while_(Recipe &r) // TODO(ch): Implement with jmp-alignops-body-cmp-jmptobody scheme.
    {
        int continuelabel = m_data.provideLabel();
        int bodylabel = m_data.provideLabel();
        int breaklabel = m_data.provideLabel();
        int stem_cstart_pos = m_data.program.size() - 1;
        while (stem_cstart_pos >= 0 && m_data.program[stem_cstart_pos].opcode != OP_STEM_CSTART)
            stem_cstart_pos--;
        Assert(stem_cstart_pos >= 0);
        m_data.program[stem_cstart_pos] = Syntop(OP_WHILE_CSTART, {Arg(continuelabel)});
        unpack_condition(r, bodylabel, breaklabel);
        m_data.program.push_back(Syntop(OP_WHILE_CEND, {}));
        m_cflowStack.emplace_back(ControlFlowBracket(ControlFlowBracket::WHILE, continuelabel, breaklabel));
    }

    void CodeCollecting::endwhile_()
    {
        if (m_cflowStack.size() == 0 || m_cflowStack.back().tag != ControlFlowBracket::WHILE)
            throw std::runtime_error("Unclosed control flow bracket: there is no \"while\" for \"endwhile\".");
        auto bracket = m_cflowStack.back();
        m_cflowStack.pop_back();
        int continuelabel = bracket.label_or_pos;
        int breaklabel = bracket.auxfield;
        m_data.program.push_back(Syntop(OP_ENDWHILE, {argIImm(continuelabel), argIImm(breaklabel)}));
    }

    void CodeCollecting::break_()
    {
        auto rator = m_cflowStack.rbegin();
        for (; rator != m_cflowStack.rend(); ++rator)
            if (rator->tag == ControlFlowBracket::WHILE)
                break;
        size_t nextPos = m_data.program.size();
        if (rator == m_cflowStack.rend())
            throw std::runtime_error("Control flow bracket issue: there is no \"while\" for \"break\".");
        int breaklabel = rator->auxfield;
        m_data.program.push_back(Syntop(OP_BREAK, {Arg(breaklabel)}));
    }

    void CodeCollecting::continue_()
    {
        auto rator = m_cflowStack.rbegin();
        for (; rator != m_cflowStack.rend(); ++rator)
            if (rator->tag == ControlFlowBracket::WHILE)
                break;
        size_t nextPos = m_data.program.size();
        if (rator == m_cflowStack.rend())
            throw std::runtime_error("Control flow bracket issue: there is no \"while\" for \"continue\".");
        int continuelabel = rator->label_or_pos;
        m_data.program.push_back(Syntop(OP_BREAK, {Arg(continuelabel)}));
    }

    void CodeCollecting::if_(Recipe &r)
    {
        int thenlabel = m_data.provideLabel();
        int endlabel = m_data.provideLabel();
        int stem_cstart_pos = m_data.program.size() - 1;
        while (stem_cstart_pos >= 0 && m_data.program[stem_cstart_pos].opcode != OP_STEM_CSTART)
            stem_cstart_pos--;
        Assert(stem_cstart_pos >= 0);
        m_data.program[stem_cstart_pos] = Syntop(OP_IF_CSTART, {});
        unpack_condition(r, thenlabel, endlabel);
        m_data.program.push_back(Syntop(OP_IF_CEND, {}));
        m_cflowStack.emplace_back(ControlFlowBracket(ControlFlowBracket::IF, endlabel));
    }

    void CodeCollecting::elif_(Recipe &r)
    {
        if (m_cflowStack.size() == 0)
            throw std::runtime_error("Unclosed control flow bracket: there is no \"if\" for \"elif\".");
        ControlFlowBracket bracket = m_cflowStack.back();
        m_cflowStack.pop_back();
        if (bracket.tag != ControlFlowBracket::IF)
            throw std::runtime_error("Control flow bracket error: expected corresponding \"if\" for \"elif\".");
        int outlabel = m_data.provideLabel();
        int stem_cstart_pos = m_data.program.size() - 1;
        while (stem_cstart_pos >= 0 && m_data.program[stem_cstart_pos].opcode != OP_STEM_CSTART)
            stem_cstart_pos--;
        Assert(stem_cstart_pos >= 0);
        m_data.program[stem_cstart_pos] = Syntop(OP_ELIF_CSTART, {Arg(bracket.label_or_pos), Arg(outlabel)});
        int thenlabel = m_data.provideLabel();
        int new_end_label = m_data.provideLabel();
        unpack_condition(r, thenlabel, new_end_label);
        m_data.program.push_back(Syntop(OP_IF_CEND, {}));
        m_cflowStack.emplace_back(ControlFlowBracket(ControlFlowBracket::IF, new_end_label));
    }

    void CodeCollecting::else_()
    {
        if (m_cflowStack.size() == 0)
            throw std::runtime_error("Unclosed control flow bracket: there is no \"if\" for \"else\".");
        ControlFlowBracket bracket = m_cflowStack.back();
        m_cflowStack.pop_back();
        if (bracket.tag != ControlFlowBracket::IF)
            throw std::runtime_error("Control flow bracket error: expected corresponding \"if\" for \"else\".");
        int outlabel = m_data.provideLabel();
        m_data.program.push_back(Syntop(OP_ELSE, {Arg(bracket.label_or_pos), Arg(outlabel)}));
        m_cflowStack.emplace_back(ControlFlowBracket(ControlFlowBracket::ELSE, outlabel));
    }

    void CodeCollecting::subst_elif(Recipe &r)
    {
        reopen_endif(true);
        elif_(r);
    }

    void CodeCollecting::subst_else()
    {
        reopen_endif();
        else_();
    }

    void CodeCollecting::endif_()
    {
        if (m_cflowStack.size() == 0)
            throw std::runtime_error("Unclosed control flow bracket: there is no \"if\", \"elif\" or \"else\", for \"endif\".");
        ControlFlowBracket bracket = m_cflowStack.back();
        m_cflowStack.pop_back();
        if (bracket.tag != ControlFlowBracket::IF && bracket.tag != ControlFlowBracket::ELSE)
            throw std::runtime_error("Control flow bracket error: expected corresponding \"if\", \"elif\" or \"else\" for \"endif\".");
        m_data.program.push_back(Syntop(OP_ENDIF, {Arg(bracket.label_or_pos)}));
    }

    void CodeCollecting::return_()
    {
        if (m_returnType == RT_REGISTER)
            throw std::runtime_error("Mixed return types");
        m_data.program.push_back(Syntop(OP_RET, {}));
    }

    void CodeCollecting::return_(Recipe &retval)
    {
        if (m_returnType == RT_VOID)
            throw std::runtime_error("Mixed return types");
        if (retval.is_vector())
            throw std::runtime_error("Vector return is not supported.");
        m_data.program.push_back(Syntop(OP_MOV, {argReg(RB_INT, (int)Syntfunc::RETREG), unpack_recipe(retval)}));
        m_data.program.push_back(Syntop(OP_RET, {}));
    }

    Arg CodeCollecting::unpack_recipe(Recipe &rcp, int flags, Syntfunc *buffer)
    {
        Arg res;
        Syntfunc &outbuf = buffer == nullptr ? m_data : *buffer;
        switch (rcp.opcode())
        {
        case (RECIPE_LEAF):
        {
            if (flags & UR_WRAPIIMM && rcp.leaf().tag == Arg::IIMMEDIATE)
            {
                res.idx = flags & UR_NONEWIDX ? 0 : outbuf.provideIdx(RB_INT);
                res.tag = Arg::IREG;
                Syntop toAdd(OP_MOV, {res, rcp.leaf()});
                outbuf.program.emplace_back(toAdd);
            }
            else
                res = rcp.leaf();
            if (res.tag == Arg::IREG || res.tag == Arg::IIMMEDIATE)
                res.elemtype = rcp.type();
            break;
        }
        case (VOP_REINTERPRET):
        {
            Assert(rcp.is_vector() && rcp.children().size() == 1);
            res = unpack_recipe(rcp.children()[0], flags & UR_NONEWIDX, &outbuf);
            res.tag = Arg::VREG;
            res.elemtype = rcp.type();
            break;
        }
        case (OP_SELECT):
        {
            Assert(rcp.children().size() == 3);
            // TODO(ch): In truth, i'm not sure, that select doesn't support immediates on all the archs, so, this usage of UR_WRAPIIMM must be reconsidered
            Arg truev = unpack_recipe(rcp.children()[1], UR_WRAPIIMM, &outbuf);
            Arg falsev = unpack_recipe(rcp.children()[2], UR_WRAPIIMM, &outbuf);
            Syntop cmpop = unpack_condition_old(rcp.children()[0]);
            outbuf.program.push_back(Syntop(OP_CMP, {cmpop[0], cmpop[1]}));
            res.idx = flags & UR_NONEWIDX ? 0 : outbuf.provideIdx(RB_INT);
            res.tag = Arg::IREG;
            res.elemtype = rcp.type();
            outbuf.program.push_back(Syntop(OP_SELECT, {res, argIImm(cmpop.opcode), truev, falsev}));
            break;
        }
        case (VOP_SHRINK):
        {
            Assert(rcp.children().size() == 2);
            Arg v0 = unpack_recipe(rcp.children()[0], UR_WRAPIIMM, &outbuf);
            Arg v1 = unpack_recipe(rcp.children()[1], UR_WRAPIIMM, &outbuf);
            if (v0.idx == v1.idx)
                throw std::runtime_error("Shrink two same halfes into vector is not supported.");
            res.idx = outbuf.provideIdx(RB_VEC);
            res.tag = Arg::VREG;
            res.elemtype = rcp.type();
            outbuf.program.push_back(Syntop(OP_CMP, {res, v0, v1}));
            break;
        }
        default:
        {
            std::vector<Arg> args;
            args.reserve(rcp.children().size() + 1);
            res.idx = flags & UR_NONEWIDX ? 0 : outbuf.provideIdx(rcp.is_vector() ? RB_VEC : RB_INT);
            res.tag = rcp.is_vector() ? Arg::VREG : Arg::IREG;
            res.elemtype = rcp.type();
            args.push_back(res);
            for (int child_num = 0; child_num < rcp.children().size(); child_num++)
                args.push_back(unpack_recipe(rcp.children()[child_num], 0, &outbuf));
            Syntop toAdd(rcp.opcode(), args);
            outbuf.program.emplace_back(toAdd);
            break;
        }
        };
        // Duplicated members must be calculated only once, so they are replaced with result after calculation.
        // Since all member mentions are smart pointers to same object, next time unpack algorithm will meet
        // the member, it will be already calculated.
        if (!rcp.is_leaf())
            rcp.children().clear();
        rcp.opcode() = RECIPE_LEAF;
        rcp.leaf() = res;
        return res;
    }

    Recipe CodeCollecting::eliminate_not(Recipe &rcp, bool inverseflag)
    {
        // Additional non-obvious effect of this function is elimination of internal multirefercences,
        // for now only on level of logic, but futher they must be eliminated deeper, since similiar
        // of if structure can be calculated in different branches, not in given order, so every
        // subexpression must be calculated in each condition branch.
        // TODO(ch): dereference algebraic expressions.
        switch (rcp.opcode())
        {
        case (OP_LOGICAL_NOT):
            Assert(rcp.children().size() == 1);
            return eliminate_not(rcp.children()[0], !inverseflag);
        case (OP_LOGICAL_AND):
            Assert(rcp.children().size() == 2);
            return IRecipe(inverseflag ? OP_LOGICAL_OR : OP_LOGICAL_AND, TYPE_BOOLEAN, {eliminate_not(rcp.children()[0], inverseflag), eliminate_not(rcp.children()[1], inverseflag)}).notype();
        case (OP_LOGICAL_OR):
            Assert(rcp.children().size() == 2);
            return IRecipe(inverseflag ? OP_LOGICAL_AND : OP_LOGICAL_OR, TYPE_BOOLEAN, {eliminate_not(rcp.children()[0], inverseflag), eliminate_not(rcp.children()[1], inverseflag)}).notype();
        case (OP_GT):
        case (OP_UGT):
        case (OP_GE):
        case (OP_LT):
        case (OP_LE):
        case (OP_ULE):
        case (OP_NE):
        case (OP_EQ):
            Assert(rcp.children().size() == 2);
            return IRecipe(inverseflag ? invertCondition(rcp.opcode()) : rcp.opcode(), TYPE_BOOLEAN, {rcp.children()[0], rcp.children()[1]}).notype();
        case (OP_S):
        case (OP_NS):
            Assert(rcp.children().size() == 1);
            return IRecipe(inverseflag ? invertCondition(rcp.opcode()) : rcp.opcode(), TYPE_BOOLEAN, {rcp.children()[0]}).notype();
        default:
            return IRecipe((inverseflag ? OP_EQ : OP_NE), TYPE_BOOLEAN, {rcp, Recipe(0)}).notype();
        };
    }

    Syntop CodeCollecting::unpack_condition_old(Recipe &rcp)
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

    void CodeCollecting::unpack_condition(Recipe &rcp, int true_jmp, int false_jmp)
    {
        Recipe r = eliminate_not(rcp);
        Syntfunc condition_buffer;
        for (int i = 0; i < RB_AMOUNT; i++)
            condition_buffer.regAmount[i] = m_data.regAmount[i];
        condition_buffer.nextLabel = 2; // 0 and 1 are reserved for then and else branches labels

        unpack_condition_(condition_buffer, r, 0, 1);
        condition_buffer.program.emplace_back(Syntop(OP_LABEL, {Arg(0)}));

        // 1.) Delete chains "JMP x , JMP y"
        int targetOpnum = 0;
        for (int opnum = 0; opnum < condition_buffer.program.size();)
        {
            int opcode = condition_buffer.program[opnum].opcode;
            condition_buffer.program[targetOpnum++] = condition_buffer.program[opnum++];
            if (opcode == OP_JMP)
                while (opnum < condition_buffer.program.size() && condition_buffer.program[opnum].opcode == OP_JMP)
                    opnum++;
        }
        condition_buffer.program.resize(targetOpnum);
        // 2.) Delete chains "JMP x, LABEL x"
        targetOpnum = 0;
        for (int opnum = 0; opnum < condition_buffer.program.size();)
        {
            if (opnum + 1 < condition_buffer.program.size() && condition_buffer.program[opnum].opcode == OP_JMP &&
                condition_buffer.program[opnum + 1].opcode == OP_LABEL && condition_buffer.program[opnum][0].value == condition_buffer.program[opnum + 1][0].value)
            {
                opnum++;
                continue;
            }
            condition_buffer.program[targetOpnum++] = condition_buffer.program[opnum++];
        }
        condition_buffer.program.resize(targetOpnum);
        // 3.) Delete consecutive labels
        std::unordered_map<int, int> labelRenaming;
        targetOpnum = 0;
        for (int opnum = 0; opnum < condition_buffer.program.size(); opnum++)
        {
            if (opnum > 0 && condition_buffer.program[opnum].opcode == OP_LABEL && condition_buffer.program[opnum - 1].opcode == OP_LABEL)
            {
                labelRenaming[condition_buffer.program[opnum][0].value] = condition_buffer.program[opnum - 1][0].value;
                continue;
            }
            condition_buffer.program[targetOpnum] = condition_buffer.program[opnum];
            targetOpnum++;
        }
        condition_buffer.program.resize(targetOpnum);
        for (int opnum = 0; opnum < condition_buffer.program.size(); opnum++)
        {
            int opcode = condition_buffer.program[opnum].opcode;
            if (opcode == OP_JCC || opcode == OP_JMP)
            {
                int labelkeeper = (opcode == OP_JCC ? 1 : 0);
                int labeldest = condition_buffer.program[opnum][labelkeeper].value;
                if (labelRenaming.find(labeldest) != labelRenaming.end())
                    condition_buffer.program[opnum][labelkeeper].value = labelRenaming[labeldest];
            }
        }

        bool changed = true;
        while (changed)
        {
            changed = false;
            std::unordered_set<int> used_labels;
            used_labels.insert(1);
            // 4.) Delete non-used labels.
            for (int opnum = 0; opnum < condition_buffer.program.size(); opnum++)
            {
                int opcode = condition_buffer.program[opnum].opcode;
                if (opcode == OP_JCC || opcode == OP_JMP)
                    used_labels.insert(condition_buffer.program[opnum][opcode == OP_JCC ? 1 : 0].value);
            }
            targetOpnum = 0;
            for (int opnum = 0; opnum < condition_buffer.program.size(); opnum++)
            {
                if (condition_buffer.program[opnum].opcode == OP_LABEL && used_labels.find(condition_buffer.program[opnum][0].value) == used_labels.end())
                    continue;
                condition_buffer.program[targetOpnum] = condition_buffer.program[opnum];
                targetOpnum++;
            }
            if (targetOpnum != condition_buffer.program.size())
            {
                condition_buffer.program.resize(targetOpnum);
                changed = true;
            }
            // 5.) Transform sequences J(Cond, label0);  J(label1) ; label0: -> J(!Cond, label1); label0:
            targetOpnum = 0;

            for (int opnum = 0; opnum < condition_buffer.program.size();)
            {
                if (opnum + 2 < condition_buffer.program.size() &&
                    condition_buffer.program[opnum].opcode == OP_JCC &&
                    condition_buffer.program[opnum + 1].opcode == OP_JMP &&
                    condition_buffer.program[opnum + 2].opcode == OP_LABEL &&
                    condition_buffer.program[opnum][1].value == condition_buffer.program[opnum + 2][0].value)
                {
                    int cond = condition_buffer.program[opnum][0].value;
                    int label0 = condition_buffer.program[opnum][1].value;
                    int label1 = condition_buffer.program[opnum + 1][0].value;
                    condition_buffer.program[targetOpnum++] = Syntop(OP_JCC, {Arg(invertCondition(cond)), Arg(label1)});
                    condition_buffer.program[targetOpnum++] = condition_buffer.program[opnum + 2];
                    opnum += 3;
                    continue;
                }
                condition_buffer.program[targetOpnum++] = condition_buffer.program[opnum++];
            }
            condition_buffer.program.resize(targetOpnum);
        }

        // 6.) Append precompiled condition buffer into program buffer.
        m_data.program.reserve(m_data.program.size() + condition_buffer.program.size());
        std::unordered_map<int, int> used_labels;
        used_labels.insert(std::pair<int, int>(0, true_jmp));
        used_labels.insert(std::pair<int, int>(1, false_jmp));
        for (int opnum = 0; opnum < condition_buffer.program.size(); opnum++)
        {
            Syntop &op = condition_buffer.program[opnum];
            if (op.opcode == OP_LABEL || op.opcode == OP_JCC || op.opcode == OP_JMP)
            {
                int labelkeeper = (op.opcode == OP_JCC ? 1 : 0);
                int labelsrc = condition_buffer.program[opnum][labelkeeper].value;
                if (used_labels.find(labelsrc) == used_labels.end())
                    used_labels.insert(std::pair<int, int>(labelsrc, m_data.provideLabel()));
                int labeldest = used_labels.at(labelsrc);
                op[labelkeeper].value = labeldest;
            }
            m_data.program.push_back(op);
        }
        for (int i = 0; i < RB_AMOUNT; i++)
            m_data.regAmount[i] = condition_buffer.regAmount[i];
    }

    void CodeCollecting::unpack_condition_(Syntfunc &condition_buffer, Recipe &rcp, int labeltrue, int labelfalse, int flags)
    {
        switch (rcp.opcode())
        {
        case (OP_LOGICAL_AND):
        {
            Assert(rcp.children().size() == 2);
            int innerlabel = condition_buffer.provideLabel();
            unpack_condition_(condition_buffer, rcp.children()[0], innerlabel, labelfalse);
            condition_buffer.program.emplace_back(Syntop(OP_LABEL, {innerlabel}));
            unpack_condition_(condition_buffer, rcp.children()[1], labeltrue, labelfalse);
            condition_buffer.program.emplace_back(Syntop(OP_JMP, {labeltrue}));
            break;
        }
        case (OP_LOGICAL_OR):
        {
            Assert(rcp.children().size() == 2);
            int innerlabel = condition_buffer.provideLabel();
            unpack_condition_(condition_buffer, rcp.children()[0], labeltrue, innerlabel, UC_CORRECT_PREFFERED);
            condition_buffer.program.emplace_back(Syntop(OP_LABEL, {innerlabel}));
            unpack_condition_(condition_buffer, rcp.children()[1], labeltrue, labelfalse);
            // if(flags & UC_NO_ALTERNATIVE_JUMP == 0)
            condition_buffer.program.emplace_back(Syntop(OP_JMP, {labeltrue}));
            break;
        }
        case (OP_LOGICAL_NOT):
            throw std::runtime_error("Condition deployment: unexpected NOT operator.");
        case (OP_GT):
        case (OP_UGT):
        case (OP_GE):
        case (OP_LT):
        case (OP_LE):
        case (OP_ULE):
        case (OP_NE):
        case (OP_EQ):
        {
            Assert(rcp.children().size() == 2);
            std::vector<Arg> args;
            args.reserve(2);
            args.push_back(unpack_recipe(rcp.children()[0], 0, &condition_buffer));
            args.push_back(unpack_recipe(rcp.children()[1], 0, &condition_buffer));
            Assert(args[0].tag == Arg::IREG || args[1].tag == Arg::IREG);
            condition_buffer.program.emplace_back(Syntop(OP_CMP, args));
            args.clear();
            args.push_back(flags & UC_CORRECT_PREFFERED ? rcp.opcode() : invertCondition(rcp.opcode()));
            args.push_back(flags & UC_CORRECT_PREFFERED ? labeltrue : labelfalse);
            condition_buffer.program.emplace_back(Syntop(OP_JCC, args));
            condition_buffer.program.emplace_back(Syntop(OP_JMP, {flags & UC_CORRECT_PREFFERED ? labelfalse : labeltrue}));
            break;
        }
        case (OP_S):
        case (OP_NS):
            throw std::runtime_error("Condition deployment: unexpected Signed/Unsigned check.");
        default:
            throw std::runtime_error("Condition deployment: unexpected operation: must be comparisson or logical.");
        }
    }

    void CodeCollecting::reopen_endif(bool cond_prefix_allowed)
    {
        if (!m_data.program.size())
            throw std::runtime_error("Trying to substitute branch end in empty program.");
        int endif_to_reopen;
        if (cond_prefix_allowed)
        {
            int stem_cstart_pos = m_data.program.size() - 1;
            while (stem_cstart_pos >= 0 && m_data.program[stem_cstart_pos].opcode != OP_STEM_CSTART)
                stem_cstart_pos--;
            Assert(stem_cstart_pos >= 0);
            if (stem_cstart_pos == 0 || m_data.program[stem_cstart_pos - 1].opcode != OP_ENDIF)
                throw std::runtime_error("Branch end substitution can be done only immediately after embranchment end.");
            endif_to_reopen = m_data.program[stem_cstart_pos - 1][0].value;
            m_data.program.erase(m_data.program.begin() + stem_cstart_pos - 1);
        }
        else
        {
            if (m_data.program.back().opcode != OP_ENDIF)
                throw std::runtime_error("Branch end substitution can be done only immediately after embranchment end.");
            Assert(m_data.program.back().size() == 1);
            endif_to_reopen = m_data.program.back()[0].value;
            m_data.program.pop_back();
        }
        m_cflowStack.emplace_back(ControlFlowBracket(ControlFlowBracket::IF, endif_to_reopen));
    }
};
