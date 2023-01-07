/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "backend.hpp"
#include "func_impl.hpp"
#include <iostream>
#include <cstring>
namespace loops
{
SyntopTranslation::ArgTranslation::ArgTranslation(const Arg& a_fixed) : tag(T_FIXED), fixed(a_fixed), srcArgnum(-1), transitFlags(0) {}
SyntopTranslation::ArgTranslation::ArgTranslation(size_t a_src_arnum, uint64_t flags) : tag(T_FROMSOURCE), srcArgnum(a_src_arnum), transitFlags(flags) {}
SyntopTranslation::SyntopTranslation(int a_tarop, std::initializer_list<ArgTranslation> a_args) : m_tarop(a_tarop), m_argsList(a_args){}

Syntop SyntopTranslation::apply(const Syntop& a_source, const Backend* a_backend) const
{
    std::vector<Arg> resargs;
    resargs.reserve(m_argsList.size());
    for(const ArgTranslation& argt : m_argsList)
        switch (argt.tag) {
            case ArgTranslation::T_FIXED:
                resargs.push_back(argt.fixed);
                break;
            case ArgTranslation::T_FROMSOURCE:
            {
                if(argt.srcArgnum >= a_source.size())
                    throw std::runtime_error("Syntop translator: non-existent argument is requested.");
                Arg toAdd = a_source.args[argt.srcArgnum];
                toAdd.flags |= argt.transitFlags;
                resargs.push_back(toAdd);
                break;
            }
            case ArgTranslation::T_TRANSFORMTOSPILL:
            {
                Assert(argt.srcArgnum < a_source.size());
                Assert(a_source.args[argt.srcArgnum].tag == Arg::IIMMEDIATE);
                Arg toAdd = argSpilled(RB_INT, a_source.args[argt.srcArgnum].value);
                toAdd.flags |= argt.transitFlags;
                resargs.push_back(toAdd);
                break;
            }
            case ArgTranslation::T_COPYSHIFTRIGHT:
            {
                Assert(argt.srcArgnum < a_source.size());
                Assert(a_source.args[argt.srcArgnum].tag == Arg::IIMMEDIATE);
                Arg toAdd = a_source.args[argt.srcArgnum];
                if (argt.fixed.value >= 0){
                    toAdd.value >>= argt.fixed.value;
                    if((toAdd.value << argt.fixed.value) != a_source.args[argt.srcArgnum].value)
                        throw std::runtime_error("Syntop translator: argument alignment error.");
                }
                else {
                    toAdd.value <<= std::abs(argt.fixed.value);
                    if((toAdd.value >> std::abs(argt.fixed.value)) != a_source.args[argt.srcArgnum].value)
                        throw std::runtime_error("Syntop translator: argument alignment error.");
                }
                toAdd.flags |= argt.transitFlags;
                resargs.push_back(toAdd);
                break;
            }
            default:
                throw std::runtime_error("Syntop translator: unknown type of argument translation.");
        }
    return Syntop(m_tarop, resargs);
}

size_t SyntopTranslation::targetArgNum(size_t a_srcnum) const
{
    size_t res = 0;
    for(;res< m_argsList.size(); ++res)
        if(m_argsList[res].tag == ArgTranslation::T_FROMSOURCE && m_argsList[res].srcArgnum == a_srcnum)
            break;
    if (res == m_argsList.size())
        res = ARG_NOT_USED;
    return res;
}

bool Backend::isImmediateFit(const Syntop& a_op, size_t argnum) const
{
    bool found;
    SyntopTranslation s2s = m_s2slookup(this, a_op, found);
    if (!found)
        return false;
    argnum = s2s.targetArgNum(argnum);
    if(argnum == SyntopTranslation::ARG_NOT_USED)
        return true;
    Syntop tar_op = s2s.apply(a_op);
    BinTranslation instemp = m_s2blookup(tar_op, found);
    if (!found)
        return false;
    Assert(argnum < tar_op.size() && tar_op.args[argnum].tag == Arg::IIMMEDIATE);
    bool neg = tar_op.args[argnum].value < 0;
    uint64_t val2BeFit = neg ? ~tar_op.args[argnum].value : tar_op.args[argnum].value;
    for(const BinTranslation::Token& det : instemp.m_compound)
        if (det.tag != BinTranslation::Token::T_STATIC && det.arVecNum == argnum)
        {
            Assert(det.tag != BinTranslation::Token::T_REG);
            size_t bitwneeded = msb64(val2BeFit);
            bitwneeded += neg ? 1 : 0;
            return (bitwneeded < det.width);
        }
    throw std::runtime_error("Binary translator: non-existent argument is requested.");
}

std::set<size_t> Backend::filterStackPlaceable(const Syntop& a_op, const std::set<size_t>& toFilter) const
{
    return std::set<size_t>();
}

size_t Backend::reusingPreferences(const Syntop& a_op, const std::set<size_t>& undefinedArgNums) const
{
    return -1;
}

size_t Backend::spillSpaceNeeded(const Syntop& a_op, int basketNum) const
{
    return 0;
}

std::set<size_t> Backend::getUsedRegistersIdxs(const loops::Syntop &a_op, int basketNum, uint64_t flagmask) const
{
    std::set<size_t> result;
    if(a_op.opcode == OP_DEF || a_op.opcode == VOP_DEF)
    {
        int ioflags = flagmask&(BinTranslation::Token::T_OUTPUT | BinTranslation::Token::T_INPUT);
        if (ioflags == BinTranslation::Token::T_OUTPUT && 
            ((a_op.opcode == OP_DEF && basketNum == RB_INT) || (a_op.opcode == VOP_DEF && basketNum == RB_VEC)))
                result = std::set<size_t>({0});
        return result;
    }
    bool foundSynTr;
    SyntopTranslation s2s = m_s2slookup(this, a_op, foundSynTr);
    if (!foundSynTr)
        return result;
    Syntop tarop = s2s.apply(a_op);
    const BinTranslation& s2b = lookS2b(tarop);
    int desiredRegType = ((basketNum == RB_INT) ? Arg::IREG :Arg::VREG);
    for(size_t bpiecenum = 0; bpiecenum < s2b.m_compound.size(); ++bpiecenum)
    {
        if(s2b.m_compound[bpiecenum].tag == BinTranslation::Token::T_STATIC)
            continue;   //Drop all statics
        const SyntopTranslation::ArgTranslation& ar = s2s.m_argsList[s2b.m_compound[bpiecenum].arVecNum];
        if (ar.tag == SyntopTranslation::ArgTranslation::T_FROMSOURCE)
        {
            if (ar.srcArgnum >= a_op.size())
                throw std::runtime_error("Binary translator: non-existent argument is requested.");
            if (a_op[ar.srcArgnum].tag == desiredRegType && ((s2b.m_compound[bpiecenum].fieldOflags & flagmask) == flagmask))
                result.insert(ar.srcArgnum);
        }
    }
    return result;
}

std::set<size_t> Backend::getOutRegistersIdxs(const Syntop& a_op, int basketNum) const
{
    return getUsedRegistersIdxs(a_op, basketNum, BinTranslation::Token::T_OUTPUT);
}

std::set<size_t> Backend::getInRegistersIdxs(const Syntop& a_op, int basketNum) const
{
    return getUsedRegistersIdxs(a_op, basketNum, BinTranslation::Token::T_INPUT);
}

std::set<RegIdx> Backend::getUsedRegisters(const Syntop& a_op, int basketNum, uint64_t flagmask) const
{
    std::set<size_t> preres = getUsedRegistersIdxs(a_op, basketNum, flagmask);
    std::set<RegIdx> result;
    for(size_t arnum: preres)
    {
        if(arnum >= a_op.size())
            throw std::runtime_error("Compile error: non-existent argument is requested.");
        if(a_op[arnum].tag != Arg::IREG && a_op[arnum].tag != Arg::VREG)
            throw std::runtime_error("Compile error: constant is requested instead of register.");
        result.insert(a_op[arnum].idx);
    }
    return result;
}

std::set<RegIdx> Backend::getOutRegisters(const Syntop& a_op, int basketNum) const
{
    return getUsedRegisters(a_op, basketNum, BinTranslation::Token::T_OUTPUT);
}

std::set<RegIdx> Backend::getInRegisters(const Syntop& a_op, int basketNum) const
{
    return getUsedRegisters(a_op, basketNum, BinTranslation::Token::T_INPUT);
}

const FuncBodyBuf Backend::target2Hex(const Syntfunc& a_bcfunc) const
{
    Bitwriter bitstream(this);
    for (const Syntop& op : a_bcfunc.program)
        lookS2b(op).applyNAppend(op, &bitstream);
    return bitstream.buffer();
}

void* Backend::compile(Context* a_ctx, Func* a_func)
{
    FuncImpl* func = static_cast<FuncImpl*>(a_func);
    Syntfunc tarcode;
    this->getBytecode2TargetStage()->process(tarcode, func->getData());
    const FuncBodyBuf body = target2Hex(tarcode);
    
    uint8_t* exebuf = m_exeAlloc.allocate(body->size());
    
    memcpy(exebuf, (void*)&(body->operator[](0)), body->size()); //TODO(ch): You have to change used adressess before.
    
    m_exeAlloc.protect2Execution(exebuf);
    return exebuf;
}

class Bytecode2TargetStage : public CompilerStage
{
public:
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual ~Bytecode2TargetStage() override {}
    static CompilerStagePtr make(const Backend* a_backend)
    {
        std::shared_ptr<Bytecode2TargetStage> res;
        res.reset(new Bytecode2TargetStage(a_backend));
        return std::static_pointer_cast<CompilerStage>(res);
    } 
protected:
    struct label_ref_info
    {
        size_t opnum;
        size_t argnum;
        size_t opoffset;
        label_ref_info() : opnum(0), argnum(0), opoffset(0) {}
        label_ref_info(size_t a_opnum, size_t a_argnum, size_t a_opoffset) : opnum(a_opnum), argnum(a_argnum), opoffset(a_opoffset) {}
    };
    Bytecode2TargetStage(const Backend* a_backend) : CompilerStage(a_backend) {}
};

void Bytecode2TargetStage::process(Syntfunc& a_dest, const Syntfunc& a_source)
{
    //DUBUGGG: Introduce mechanism, which forbids double stage execution.
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

CompilerStagePtr Backend::getBytecode2TargetStage() const
{
    return Bytecode2TargetStage::make(this);
}

BinTranslation BTLookup(const Syntop&, bool&)
{
    throw std::runtime_error("Binary translation table is not implemented.");
}

SyntopTranslation STLookup(const Backend*, const Syntop&, bool&)
{
    throw std::runtime_error("Syntop translation table is not implemented.");
}

Backend::Backend() : m_isLittleEndianInstructions(true)
, m_isLittleEndianOperands(false)
, m_isMonowidthInstruction(false)
, m_instructionWidth(0)
, m_registersAmount(8)
, m_s2blookup(BTLookup)
, m_s2slookup(STLookup)
    {}
};
