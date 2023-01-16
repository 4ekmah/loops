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

Cf2jumps::Cf2jumps(const Backend* a_backend, int a_epilogueSize) : CompilerStage(a_backend)
    , m_epilogueSize(a_epilogueSize)
{}

void Cf2jumps::process(Syntfunc& a_dest, const Syntfunc& a_source)
{
    //TODO(ch): what if return are not on all control pathes(this problem exists for register returns only)??? Think out.
    //          I think, it can be effectively soluted only after deletition of after-jump-silent tails a-la jmp end; mov ..code-without-jumps...; end:
    //          After this we just need to check last operation before end mark. it must be mov to return register.
    //Handle situation, when return is just before end of function (reasonable)
    Assert(&a_dest != &a_source);
    a_dest.name = a_source.name;
    a_dest.nextLabel = a_source.nextLabel;
    a_dest.params = a_source.params;
    for (int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        a_dest.regAmount[basketNum] = a_source.regAmount[basketNum];
    a_dest.program.clear();
    a_dest.program.reserve(a_source.program.size()*2);
    size_t returnLabel = a_dest.provideLabel();
    bool returnJumps = false;
    size_t bodySize = a_source.program.size() - m_epilogueSize;
    for(size_t opnum = 0; opnum < bodySize; opnum++)
    {
        const Syntop& op = a_source.program[opnum];
        switch (op.opcode) {
        case (OP_IF):
        {
            if (op.size() != 2 || op.args[0].tag != Arg::IIMMEDIATE || op.args[1].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Internal error: wrong IF command format");
            a_dest.program.push_back(Syntop(static_cast<int>(op.args[0].value), {op.args[1].value}));
            break;
        }
        case (OP_ELSE):
        {
            if (op.size() != 2 ||
                op.args[0].tag != Arg::IIMMEDIATE ||
                op.args[1].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Internal error: wrong ELIF command format");
            a_dest.program.push_back(Syntop(OP_JMP, {op.args[1].value}));
            a_dest.program.push_back(Syntop(OP_LABEL, {op.args[0].value}));
            break;
        }
        case (OP_ENDIF):
        {
            if (op.size() != 1 || op.args[0].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Internal error: wrong ENDIF command format");
            a_dest.program.push_back(Syntop(OP_LABEL, {op.args[0].value}));
            break;
        }
        case (OP_WHILE):
        {
            if (op.size() != 3 || op.args[0].tag != Arg::IIMMEDIATE || op.args[1].tag != Arg::IIMMEDIATE || op.args[2].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Internal error: wrong WHILE command format");
            std::vector<Syntop> conditionBackup(a_dest.program.back().spillPrefix + 1);
            a_dest.program.insert(a_dest.program.end() - 1 - a_dest.program.back().spillPrefix,  Syntop(OP_LABEL, {op.args[1].value}));//TODO(ch): IMPORTANT(CMPLCOND): This mean that condition can be one-instruction only.
            a_dest.program.push_back(Syntop(static_cast<int>(op.args[0].value), {op.args[2].value}));
            break;
        }
        case (OP_ENDWHILE):
        {
            if (op.size() != 2 || op.args[0].tag != Arg::IIMMEDIATE || op.args[1].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Internal error: wrong DO command format");
            a_dest.program.push_back(Syntop(OP_JMP, {op.args[0].value}));
            a_dest.program.push_back(Syntop(OP_LABEL, {op.args[1].value}));
            break;
        }
        case (OP_BREAK):
        {
            if (op.size() != 1 || op.args[0].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Internal error: wrong BREAK command format");
            a_dest.program.push_back(Syntop(OP_JMP, {op.args[0].value}));
            break;
        }
        case (OP_CONTINUE):
        {
            if (op.size() != 1 || op.args[0].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Internal error: wrong CONTINUE command format");
            a_dest.program.push_back(Syntop(OP_JMP, {op.args[0].value}));
            break;
        }
        case (OP_RET):
        {
            if(opnum + 1 + m_epilogueSize != a_source.program.size())
            {
                a_dest.program.push_back(Syntop(OP_JMP, {argIImm(returnLabel)}));
                returnJumps = true;
            }
            break;
        }
        default:
            a_dest.program.push_back(op);
            break;
        }
    }
    if(returnJumps)
        a_dest.program.push_back(Syntop(OP_LABEL, {argIImm(returnLabel)}));
    //Write epilogue
    for(size_t opnum = bodySize; opnum < a_source.program.size(); opnum++)
        a_dest.program.push_back(a_source.program[opnum]);
    a_dest.program.push_back(Syntop(OP_RET, {}));
}

void Bytecode2Assembly::process(Syntfunc& a_dest, const Syntfunc& a_source)
{
    std::unordered_map<size_t, size_t> label_map;
    std::unordered_map<size_t, std::vector<label_ref_info> > label_ref_map;
    size_t current_offset = 0;
    a_dest.name = a_source.name;
    a_dest.params = a_source.params;
    for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        a_dest.regAmount[basketNum] = a_source.regAmount[basketNum];
    a_dest.program.reserve(2 * a_source.program.size());
    for(const Syntop& op: a_source.program)
    {
        size_t curr_tar_op = a_dest.program.size();
        switch (op.opcode)
        {
        case (OP_JMP_NE):
        case (OP_JMP_EQ):
        case (OP_JMP_LT):
        case (OP_JMP_GT):
        case (OP_JMP_UGT):
        case (OP_JMP_LE):
        case (OP_JMP_ULE):
        case (OP_JMP_GE):
        case (OP_JMP):
        {
            //TODO(ch): We need for this purposes something like label/offset manager with transparent logic.
            //AArch64 supports only multiply-4 offsets,
            //so, for compactification, they are divided by 4.
            size_t current_offset_ = current_offset >> m_backend->offsetShift();
            Assert(op.size() == 1 && op.args[0].tag == Arg::IIMMEDIATE);
            label_ref_map[op.args[0].value].emplace_back(a_dest.program.size(), 0, current_offset_);
            Syntop toTransform(op);
            toTransform[0].value = current_offset_;
            Syntop tarop = m_backend->lookS2s(toTransform).apply(toTransform, m_backend);
            if(m_backend->postInstructionOffset()) 
                tarop[0].value += m_backend->lookS2b(tarop).size();
            a_dest.program.emplace_back(tarop);
            break;
        }
        case (OP_LABEL):
        {
            size_t current_offset_ = current_offset >> m_backend->offsetShift();
            if (op.size() != 1 || op.args[0].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Wrong LABEL format.");
            if (label_map.count(op.args[0].value) != 0)
                throw std::runtime_error("Label redefinition");
            label_map[op.args[0].value] = current_offset_;
            break;
        }
        case (OP_DEF):
        case (VOP_DEF):
            break;
        default:
            a_dest.program.emplace_back(m_backend->lookS2s(op).apply(op, m_backend));
            break;
        };
        for(size_t addedop = curr_tar_op; addedop<a_dest.program.size(); addedop++)
        {
            const Syntop& lastop = a_dest.program[addedop];
            current_offset += m_backend->lookS2b(lastop).size();
        }
    }
    for (auto label : label_ref_map)
    {
        if (label_map.count(label.first) == 0)
            throw std::runtime_error("Reference to unknown label");
        const int64_t loff = static_cast<int64_t>(label_map[label.first]);
        for (label_ref_info& lref : label.second)
        {
            if (lref.opnum >= a_dest.program.size())
                throw std::runtime_error("Internal error: operation number is too big");
            if (lref.argnum >= a_dest.program[lref.opnum].size())
                throw std::runtime_error("Internal error: operation don't have so much arguments");
            if (a_dest.program[lref.opnum].args[lref.argnum].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Internal error: operation don't have so much arguments");
            int64_t& opoff = a_dest.program[lref.opnum].args[lref.argnum].value;
            opoff = (loff - opoff);
        }
    }
}

void Assembly2Hex::process(Syntfunc& a_dest, const Syntfunc& a_source)
{
    for (const Syntop& op : a_source.program)
        m_backend->lookS2b(op).applyNAppend(op, &m_bitstream);
}

const FuncBodyBuf Assembly2Hex::result_buffer() const
{
    return m_bitstream.buffer();
}

Pipeline::Pipeline(Backend* a_backend, Func* a_func, const std::string& name, std::initializer_list<IReg*> params): m_backend(a_backend)
    , m_codecol(m_data, a_func)
    , m_func(a_func)
    , m_mode(PM_REGULAR)
    , m_current_stage(0)
{
    m_data.name = name;
    m_data.params.reserve(params.size());
    for (IReg* parreg : params)
    {
        if(parreg->func != nullptr || parreg->idx != IReg::NOIDX)
            throw std::runtime_error("Parameter index is already initilized in some other function");
        parreg->func = a_func;
        parreg->idx = m_data.provideIdx(RB_INT);
        m_data.params.emplace_back(*parreg);
    }
    m_mode = PM_FINDORDER;
    run();
    m_mode = PM_REGULAR;
}

void Pipeline::run_until(StageID a_stageID)
{
    int target_stage = m_stage_ordering.at(a_stageID) - 1;
    m_target_stage = target_stage;
    run();
    m_current_stage = target_stage+1;
}

void Pipeline::run_until_including(StageID a_stageID)
{
    int target_stage = m_stage_ordering.at(a_stageID);
    m_target_stage = target_stage;
    run();
    m_current_stage = target_stage+1;
}

void Pipeline::pass_until(StageID a_stageID)
{
    m_current_stage = m_stage_ordering.at(a_stageID);
}

CodeCollecting* Pipeline::get_code_collecting()
{
    AssertMsg(m_current_stage <= CS_COLLECTING, "Attempt to add instruction to already finished function.");
    return &m_codecol;
}

void Pipeline::overrideRegisterSet(int basketNum, const std::vector<size_t>& a_parameterRegisters, const std::vector<size_t>& a_returnRegisters, const std::vector<size_t>& a_callerSavedRegisters, const std::vector<size_t>& a_calleeSavedRegisters)
{
    m_parameterRegistersO[basketNum]   = a_parameterRegisters;
    m_returnRegistersO[basketNum]      = a_returnRegisters;
    m_callerSavedRegistersO[basketNum] = a_callerSavedRegisters;
    m_calleeSavedRegistersO[basketNum] = a_calleeSavedRegisters;
}

void Pipeline::run()
{
    run_stage(&m_codecol);
    auto beforeRegAlloc = m_backend->getBeforeRegAllocStages();
    for (CompilerStagePtr braStage : beforeRegAlloc)
        run_stage(braStage.get());
    LivenessAnalysisAlgo LAalgo(m_backend);
    run_stage(&LAalgo);        //inplace
    RegisterAllocator regalloc(m_backend, LAalgo.live_intervals(), LAalgo.getSnippetCausedSpills());
    for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        if(m_parameterRegistersO[basketNum].size() != 0 || m_returnRegistersO[basketNum].size() != 0 || m_callerSavedRegistersO[basketNum].size() != 0 || m_calleeSavedRegistersO[basketNum].size() != 0)
            regalloc.getRegisterPool().overrideRegisterSet(basketNum, m_parameterRegistersO[basketNum], m_returnRegistersO[basketNum], m_callerSavedRegistersO[basketNum], m_calleeSavedRegistersO[basketNum]);
    run_stage(&regalloc);
    for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        if(m_parameterRegistersO[basketNum].size() != 0 || m_returnRegistersO[basketNum].size() != 0 || m_callerSavedRegistersO[basketNum].size() != 0 || m_calleeSavedRegistersO[basketNum].size() != 0)
            regalloc.getRegisterPool().overrideRegisterSet(basketNum, {}, {}, {}, {});
    Cf2jumps cf2jmps(m_backend, regalloc.epilogueSize());
    run_stage(&cf2jmps);
    auto afterRegAlloc = m_backend->getAfterRegAllocStages();
    for (CompilerStagePtr araStage : afterRegAlloc)
        run_stage(araStage.get());
    Bytecode2Assembly b2aStage(m_backend);
    run_stage(&b2aStage);
    Assembly2Hex a2hStage(m_backend);
    run_stage(&a2hStage);
    m_buffer = a2hStage.result_buffer();
}

void Pipeline::run_stage(CompilerStage* a_stage)
{
    if (m_mode == PM_FINDORDER)
    {
        Assert(m_stage_ordering.find(a_stage->stage_id()) == m_stage_ordering.end());
        m_stage_ordering.insert(std::pair<int, int>(a_stage->stage_id(), (int)m_stage_ordering.size()));
        return;
    }
    int stagenum = m_stage_ordering.at(a_stage->stage_id());
    if (stagenum >= m_current_stage && stagenum <= m_target_stage)
    {
        if (a_stage->is_inplace())
            a_stage->process(m_data, m_data);
        else
        {
            Syntfunc oldcode = m_data;
            m_data.program.clear();
            a_stage->process(m_data, oldcode);
        }
    }
}
};
