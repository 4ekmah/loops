/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "pipeline.hpp"
#include "backend.hpp"
#include "composer.hpp"
#include "printer.hpp"
#include "reg_allocator.hpp"
#include <algorithm>
#include <iomanip>
#include <deque>
#include <iostream>

namespace loops
{
    ImmediateImplantation::ImmediateImplantation(const Backend *a_backend) : CompilerPass(a_backend) {}

    void ImmediateImplantation::process(Syntfunc &a_dest, const Syntfunc &a_source)
    {
        Assert(&a_dest != &a_source);
        a_dest.name = a_source.name;
        a_dest.nextLabel = a_source.nextLabel;
        a_dest.params = a_source.params;
        for (int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            a_dest.regAmount[basketNum] = a_source.regAmount[basketNum];
        a_dest.program.clear();
        a_dest.program.reserve(a_source.program.size() * 2);
        for (const Syntop &op : a_source.program)
            switch (op.opcode)
            {
            case (OP_MOV):
            case (OP_IF_CSTART):
            case (OP_ELIF_CSTART):
            case (OP_ELSE):
            case (OP_ENDIF):
            case (OP_WHILE_CSTART):
            case (OP_ENDWHILE):
            case (OP_BREAK):
            case (OP_CONTINUE):
            case (OP_JMP):
            case (OP_JCC):
            case (OP_DEF):
            case (VOP_DEF):
            case (OP_LABEL):
                a_dest.program.push_back(op);
                break;
            default:
            {
                Syntop op_probe = op;
                std::vector<size_t> arnums;
                arnums.reserve(op_probe.size());
                for (size_t arnum = 0; arnum < op_probe.size(); arnum++)
                    if (op_probe[arnum].tag == Arg::IIMMEDIATE)
                        arnums.push_back(arnum);
                if (op.opcode == OP_SELECT) // DUBUGGG: don't forget to reconsider after finishing conditions task.
                {
                    Assert(arnums[0] == 1);
                    arnums.erase(arnums.begin());
                }
                std::set<RegIdx> usedRegs;
                for (const Arg &ar : op_probe)
                    if (ar.tag == Arg::IREG)
                        usedRegs.insert(ar.idx);
                std::vector<Arg> attempts;
                attempts.reserve(arnums.size());
                RegIdx placeholderTop = 0;
                for (size_t arNum : arnums)
                {
                    Assert(op_probe[arNum].tag == Arg::IIMMEDIATE);
                    attempts.push_back(op_probe[arNum]);
                    while (usedRegs.count(placeholderTop))
                        placeholderTop++;
                    op_probe[arNum] = argReg(RB_INT, placeholderTop++);
                }
                for (size_t attemptN = 0; attemptN < arnums.size(); attemptN++)
                {
                    size_t arNum = arnums[attemptN];
                    op_probe[arNum] = attempts[attemptN];
                    if (!m_backend->isImmediateFit(op_probe, arNum))
                    {
                        Arg cons = argReg(RB_INT, a_dest.provideIdx(RB_INT));
                        Syntop move(OP_MOV, {cons, attempts[attemptN]});
                        a_dest.program.push_back(move);
                        op_probe[arNum] = cons;
                    }
                }
                a_dest.program.push_back(op_probe);
                break;
            }
            };
    }

    ElifElimination::ElifElimination(const Backend *a_backend) : CompilerPass(a_backend) {}

    void ElifElimination::process(Syntfunc &a_dest, const Syntfunc &a_source)
    {
        Assert(&a_dest != &a_source);
        a_dest.name = a_source.name;
        a_dest.nextLabel = a_source.nextLabel;
        a_dest.params = a_source.params;
        for (int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            a_dest.regAmount[basketNum] = a_source.regAmount[basketNum];
        a_dest.program.clear();
        a_dest.program.reserve(a_source.program.size() * 2);
        std::deque<ControlFlowBracket> m_cflowStack;
        for (const Syntop &op : a_source.program)
            switch (op.opcode)
            {
            case (OP_IF_CSTART):
                m_cflowStack.emplace_back(ControlFlowBracket(ControlFlowBracket::IF, 0));
                a_dest.program.push_back(op);
                break;
            case (OP_ELIF_CSTART):
            {
                Assert(m_cflowStack.size());
                if (m_cflowStack.back().tag == ControlFlowBracket::ELSE)
                    throw std::runtime_error("Control flow bracket error: \"else\" before \"endif\".");
                Assert(m_cflowStack.back().tag == ControlFlowBracket::IF);
                Assert(op.size() == 2);
                int64_t elselabel = op[0].value;
                int64_t outlabel = op[1].value;
                ControlFlowBracket ifcf(ControlFlowBracket::IF, outlabel, m_cflowStack.back().auxfield + 1);
                a_dest.program.push_back(Syntop(OP_ELSE, {Arg(elselabel), Arg(outlabel)}));
                m_cflowStack.emplace_back(ControlFlowBracket(ControlFlowBracket::ELSE, 0));
                a_dest.program.push_back(Syntop(OP_IF_CSTART, {}));
                m_cflowStack.emplace_back(ifcf);
                break;
            }
            case (OP_ELSE):
                Assert(m_cflowStack.size() && m_cflowStack.back().tag == ControlFlowBracket::IF);
                m_cflowStack.emplace_back(ControlFlowBracket(ControlFlowBracket::ELSE, 0));
                a_dest.program.push_back(op);
                break;
            case (OP_ENDIF):
            {
                Assert(m_cflowStack.size());
                Assert(op.size() == 1);
                std::deque<int64_t> reversed_endifs;
                reversed_endifs.push_back(op[0].value);
                if (m_cflowStack.back().tag == ControlFlowBracket::ELSE)
                {
                    m_cflowStack.pop_back();
                    Assert(m_cflowStack.size());
                }
                int last_elif_repeats = 0;
                do
                {
                    ControlFlowBracket bracket = m_cflowStack.back();
                    m_cflowStack.pop_back();
                    Assert(bracket.tag == ControlFlowBracket::IF);
                    last_elif_repeats = bracket.auxfield;
                    if (last_elif_repeats)
                    {
                        reversed_endifs.push_back(bracket.label_or_pos);
                        Assert(m_cflowStack.size() && m_cflowStack.back().tag == ControlFlowBracket::ELSE);
                        m_cflowStack.pop_back();
                    }
                } while (last_elif_repeats);
                while (reversed_endifs.size())
                {
                    a_dest.program.push_back(Syntop(OP_ENDIF, {Arg(reversed_endifs.back())}));
                    reversed_endifs.pop_back();
                }
                break;
            }
            default:
                a_dest.program.push_back(op);
                break;
            };
    }

    Cf2jumps::Cf2jumps(const Backend *a_backend, int a_epilogueSize) : CompilerPass(a_backend), m_epilogueSize(a_epilogueSize)
    {
    }

    void Cf2jumps::process(Syntfunc &a_dest, const Syntfunc &a_source)
    {
        // TODO(ch): what if return are not on all control pathes(this problem exists for register returns only)??? Think out.
        //           I think, it can be effectively soluted only after deletition of after-jump-silent tails a-la jmp end; mov ..code-without-jumps...; end:
        //           After this we just need to check last operation before end mark. it must be mov to return register.
        // Handle situation, when return is just before end of function (reasonable)
        Assert(&a_dest != &a_source);
        a_dest.name = a_source.name;
        a_dest.nextLabel = a_source.nextLabel;
        a_dest.params = a_source.params;
        for (int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            a_dest.regAmount[basketNum] = a_source.regAmount[basketNum];
        a_dest.program.clear();
        a_dest.program.reserve(a_source.program.size() * 2);
        size_t returnLabel = a_dest.provideLabel();
        bool returnJumps = false;
        size_t bodySize = a_source.program.size() - m_epilogueSize;
        for (size_t opnum = 0; opnum < bodySize; opnum++)
        {
            const Syntop &op = a_source.program[opnum];
            switch (op.opcode)
            {
            case (OP_IF_CSTART):
            case (OP_IF_CEND):
            case (OP_WHILE_CEND):
                break;
            case (OP_ELSE):
            {
                Assert(op.size() == 2 && op.args[0].tag == Arg::IIMMEDIATE && op.args[1].tag == Arg::IIMMEDIATE);
                if (a_dest.program.size() == 0 || a_dest.program.back().opcode != OP_JMP) // Eliminating sequential jumps
                    a_dest.program.push_back(Syntop(OP_JMP, {op.args[1].value}));
                a_dest.program.push_back(Syntop(OP_LABEL, {op.args[0].value}));
                break;
            }
            case (OP_WHILE_CSTART):
            case (OP_ENDIF):
            {
                Assert(op.size() == 1 && op.args[0].tag == Arg::IIMMEDIATE);
                a_dest.program.push_back(Syntop(OP_LABEL, {op.args[0].value}));
                break;
            }
            case (OP_ENDWHILE):
            {
                Assert(op.size() == 2 && op.args[0].tag == Arg::IIMMEDIATE && op.args[1].tag == Arg::IIMMEDIATE);
                if (a_dest.program.size() == 0 || a_dest.program.back().opcode != OP_JMP) // Eliminating sequential jumps
                    a_dest.program.push_back(Syntop(OP_JMP, {op.args[0].value}));
                a_dest.program.push_back(Syntop(OP_LABEL, {op.args[1].value}));
                break;
            }
            case (OP_BREAK):
            {
                Assert(op.size() == 1 && op.args[0].tag == Arg::IIMMEDIATE);
                if (a_dest.program.size() == 0 || a_dest.program.back().opcode != OP_JMP) // Eliminating sequential jumps
                    a_dest.program.push_back(Syntop(OP_JMP, {op.args[0].value}));
                break;
            }
            case (OP_CONTINUE):
            {
                Assert(op.size() != 1 || op.args[0].tag != Arg::IIMMEDIATE);
                if (a_dest.program.size() == 0 || a_dest.program.back().opcode != OP_JMP) // Eliminating sequential jumps
                    a_dest.program.push_back(Syntop(OP_JMP, {op.args[0].value}));
                break;
            }
            case (OP_RET):
            {
                if (opnum + 1 + m_epilogueSize != a_source.program.size())
                {
                    if (a_dest.program.size() == 0 || a_dest.program.back().opcode != OP_JMP) // Eliminating sequential jumps
                    {
                        a_dest.program.push_back(Syntop(OP_JMP, {argIImm(returnLabel)}));
                        returnJumps = true;
                    }
                }
                break;
            }
            default:
                a_dest.program.push_back(op);
                break;
            }
        }
        if (returnJumps)
            a_dest.program.push_back(Syntop(OP_LABEL, {argIImm(returnLabel)}));
        // Write epilogue
        for (size_t opnum = bodySize; opnum < a_source.program.size(); opnum++)
            a_dest.program.push_back(a_source.program[opnum]);
        a_dest.program.push_back(Syntop(OP_RET, {}));
    }

    void Bytecode2Assembly::process(Syntfunc &a_dest, const Syntfunc &a_source)
    {
        std::unordered_map<size_t, size_t> label_map;
        std::unordered_map<size_t, std::vector<label_ref_info>> label_ref_map;
        size_t current_offset = 0;
        a_dest.name = a_source.name;
        a_dest.params = a_source.params;
        for (int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            a_dest.regAmount[basketNum] = a_source.regAmount[basketNum];
        a_dest.program.reserve(2 * a_source.program.size());
        for (const Syntop &op : a_source.program)
        {
            size_t curr_tar_op = a_dest.program.size();
            switch (op.opcode)
            {
            case (OP_JCC):
            case (OP_JMP):
            {
                // TODO(ch): We need for this purposes something like label/offset manager with transparent logic.
                // AArch64 supports only multiply-4 offsets,
                // so, for compactification, they are divided by 4.
                size_t current_offset_ = current_offset >> m_backend->offsetShift();
                Assert((op.opcode == OP_JMP && op.size() == 1 && op[0].tag == Arg::IIMMEDIATE) ||
                       (op.opcode == OP_JCC && op.size() == 2 && op[1].tag == Arg::IIMMEDIATE));
                int64_t target_label = op[op.opcode == OP_JCC ? 1 : 0].value;
                label_ref_map[target_label].emplace_back(a_dest.program.size(), 0, current_offset_);
                Syntop toTransform(op);
                toTransform[op.opcode == OP_JCC ? 1 : 0].value = current_offset_;
                Syntop tarop = m_backend->lookS2s(toTransform).apply(toTransform, m_backend);
                if (m_backend->postInstructionOffset())
                    tarop[0].value += m_backend->lookS2b(tarop).size();
                a_dest.program.emplace_back(tarop);
                break;
            }
            case (OP_LABEL):
            {
                size_t current_offset_ = current_offset >> m_backend->offsetShift();
                if (op.size() != 1 || op[0].tag != Arg::IIMMEDIATE)
                    throw std::runtime_error("Wrong LABEL format.");
                if (label_map.count(op[0].value) != 0)
                    throw std::runtime_error("Label redefinition");
                label_map[op[0].value] = current_offset_;
                break;
            }
            case (OP_DEF):
            case (VOP_DEF):
                break;
            default:
                a_dest.program.emplace_back(m_backend->lookS2s(op).apply(op, m_backend));
                break;
            };
            for (size_t addedop = curr_tar_op; addedop < a_dest.program.size(); addedop++)
            {
                const Syntop &lastop = a_dest.program[addedop];
                current_offset += m_backend->lookS2b(lastop).size();
            }
        }
        for (auto label : label_ref_map)
        {
            if (label_map.count(label.first) == 0)
                throw std::runtime_error("Reference to unknown label");
            const int64_t loff = static_cast<int64_t>(label_map[label.first]);
            for (label_ref_info &lref : label.second)
            {
                if (lref.opnum >= a_dest.program.size())
                    throw std::runtime_error("Internal error: operation number is too big");
                if (lref.argnum >= a_dest.program[lref.opnum].size())
                    throw std::runtime_error("Internal error: operation don't have so much arguments");
                if (a_dest.program[lref.opnum][lref.argnum].tag != Arg::IIMMEDIATE)
                    throw std::runtime_error("Internal error: operation don't have so much arguments");
                int64_t &opoff = a_dest.program[lref.opnum][lref.argnum].value;
                opoff = (loff - opoff);
            }
        }
    }

    void Assembly2Hex::process(Syntfunc &a_dest, const Syntfunc &a_source)
    {
        for (const Syntop &op : a_source.program)
            m_backend->lookS2b(op).applyNAppend(op, &m_bitstream);
    }

    const FuncBodyBuf Assembly2Hex::result_buffer() const
    {
        return m_bitstream.buffer();
    }

    Pipeline::Pipeline(Backend *a_backend, Func *a_func, const std::string &name, std::initializer_list<IReg *> params) : m_backend(a_backend), m_codecol(m_data, a_func), m_func(a_func), m_mode(PM_REGULAR), m_current_pass(0)
    {
        m_data.name = name;
        m_data.params.reserve(params.size());
        for (IReg *parreg : params)
        {
            if (parreg->func != nullptr || parreg->idx != IReg::NOIDX)
                throw std::runtime_error("Parameter index is already initilized in some other function");
            parreg->func = a_func;
            parreg->idx = m_data.provideIdx(RB_INT);
            m_data.params.emplace_back(*parreg);
        }
        m_mode = PM_FINDORDER;
        run();
        m_mode = PM_REGULAR;
    }

    void Pipeline::run_until(PassID a_passID)
    {
        int target_pass = m_pass_ordering.at(a_passID) - 1;
        m_target_pass = target_pass;
        run();
        m_current_pass = target_pass + 1;
    }

    void Pipeline::run_until_including(PassID a_passID)
    {
        int target_pass = m_pass_ordering.at(a_passID);
        m_target_pass = target_pass;
        run();
        m_current_pass = target_pass + 1;
    }

    void Pipeline::pass_until(PassID a_passID)
    {
        m_current_pass = m_pass_ordering.at(a_passID);
    }

    CodeCollecting *Pipeline::get_code_collecting()
    {
        AssertMsg(m_current_pass <= CP_COLLECTING, "Attempt to add instruction to already finished function.");
        return &m_codecol;
    }

    void Pipeline::overrideRegisterSet(int basketNum, const std::vector<size_t> &a_parameterRegisters, const std::vector<size_t> &a_returnRegisters, const std::vector<size_t> &a_callerSavedRegisters, const std::vector<size_t> &a_calleeSavedRegisters)
    {
        m_parameterRegistersO[basketNum] = a_parameterRegisters;
        m_returnRegistersO[basketNum] = a_returnRegisters;
        m_callerSavedRegistersO[basketNum] = a_callerSavedRegisters;
        m_calleeSavedRegistersO[basketNum] = a_calleeSavedRegisters;
    }

    void Pipeline::run()
    {
        run_pass(&m_codecol);
        ImmediateImplantation immImplantation(m_backend);
        run_pass(&immImplantation);
        ElifElimination eilfElimination(m_backend);
        run_pass(&eilfElimination);
        auto beforeRegAlloc = m_backend->getBeforeRegAllocPasses();
        for (CompilerPassPtr braPass : beforeRegAlloc)
            run_pass(braPass.get());
        LivenessAnalysisAlgo LAalgo(m_backend);
        run_pass(&LAalgo); // inplace
        RegisterAllocator regalloc(m_backend, LAalgo.live_intervals(), LAalgo.getSnippetCausedSpills());
        for (int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            if (m_parameterRegistersO[basketNum].size() != 0 || m_returnRegistersO[basketNum].size() != 0 || m_callerSavedRegistersO[basketNum].size() != 0 || m_calleeSavedRegistersO[basketNum].size() != 0)
                regalloc.getRegisterPool().overrideRegisterSet(basketNum, m_parameterRegistersO[basketNum], m_returnRegistersO[basketNum], m_callerSavedRegistersO[basketNum], m_calleeSavedRegistersO[basketNum]);
        run_pass(&regalloc);
        for (int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            if (m_parameterRegistersO[basketNum].size() != 0 || m_returnRegistersO[basketNum].size() != 0 || m_callerSavedRegistersO[basketNum].size() != 0 || m_calleeSavedRegistersO[basketNum].size() != 0)
                regalloc.getRegisterPool().overrideRegisterSet(basketNum, {}, {}, {}, {});
        Cf2jumps cf2jmps(m_backend, regalloc.epilogueSize());
        run_pass(&cf2jmps);
        auto afterRegAlloc = m_backend->getAfterRegAllocPasses();
        for (CompilerPassPtr araPass : afterRegAlloc)
            run_pass(araPass.get());
        Bytecode2Assembly b2aPass(m_backend);
        run_pass(&b2aPass);
        Assembly2Hex a2hPass(m_backend);
        run_pass(&a2hPass);
        m_buffer = a2hPass.result_buffer();
    }

    void Pipeline::run_pass(CompilerPass *a_pass)
    {
        if (m_mode == PM_FINDORDER)
        {
            Assert(m_pass_ordering.find(a_pass->pass_id()) == m_pass_ordering.end());
            m_pass_ordering.insert(std::pair<int, int>(a_pass->pass_id(), (int)m_pass_ordering.size()));
            return;
        }
        int passnum = m_pass_ordering.at(a_pass->pass_id());
        if (passnum >= m_current_pass && passnum <= m_target_pass)
        {
            if (a_pass->is_inplace())
                a_pass->process(m_data, m_data);
            else
            {
                Syntfunc oldcode = m_data;
                m_data.program.clear();
                a_pass->process(m_data, oldcode);
            }
        }
    }
};
