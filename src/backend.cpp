/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "backend.hpp"
#include "func_impl.hpp"
#include <iostream>
namespace loops
{
Mnemotr::Argutr::Argutr(const Arg& a_fixed) : tag(T_FIXED), fixed(a_fixed), srcArgnum(-1), transitFlags(0) {}
Mnemotr::Argutr::Argutr(size_t a_src_arnum, uint64_t flags) : tag(T_FROMSOURCE), srcArgnum(a_src_arnum), transitFlags(flags) {}
Mnemotr::Mnemotr(int a_tarop, std::initializer_list<Argutr> a_args) : m_tarop(a_tarop), m_argsList(a_args){}

Syntop Mnemotr::apply(const Syntop& a_source, const Backend* a_backend) const
{
    std::vector<Arg> resargs;
    resargs.reserve(m_argsList.size());
    for(const Argutr& argt : m_argsList)
        switch (argt.tag) {
            case Argutr::T_FIXED:
                resargs.push_back(argt.fixed);
                break;
            case Argutr::T_FROMSOURCE:
            {
                if(argt.srcArgnum >= a_source.size())
                    throw std::runtime_error("Mnemonic translator: non-existent argument is requested.");
                Arg toAdd = a_source.args[argt.srcArgnum];
                toAdd.flags |= argt.transitFlags;
                resargs.push_back(toAdd);
                break;
            }
            case Argutr::T_TRANSFORMTOSPILL:
            {
                Assert(argt.srcArgnum < a_source.size());
                Assert(a_source.args[argt.srcArgnum].tag == Arg::IIMMEDIATE);
                Arg toAdd = argISpilled(a_source.args[argt.srcArgnum].value);
                toAdd.flags |= argt.transitFlags;
                resargs.push_back(toAdd);
                break;
            }
            default:
                throw std::runtime_error("Mnemonic translator: unknown type of argument translation.");
        }
    return Syntop(m_tarop, resargs);
}

size_t Mnemotr::targetArgNum(size_t a_srcnum) const
{
    size_t res = 0;
    for(;res< m_argsList.size(); ++res)
        if(m_argsList[res].tag == Argutr::T_FROMSOURCE && m_argsList[res].srcArgnum == a_srcnum)
            break;
    if (res == m_argsList.size())
        res = ARG_NOT_USED;
    return res;
}

bool Backend::isImmediateFit(const Syntop& a_op, size_t argnum) const
{
    const Mnemotr& m2m = lookM2m(a_op);
    argnum = m2m.targetArgNum(argnum);
    if(argnum == Mnemotr::ARG_NOT_USED)
        return true;
    Syntop tar_op = m2m.apply(a_op);
    bool found;
    Binatr instemp = m_m2blookup(tar_op, found);
    if (!found)
        return false;
    Assert(argnum < tar_op.size() && tar_op.args[argnum].tag == Arg::IIMMEDIATE);
    bool neg = tar_op.args[argnum].value < 0;
    uint64_t val2BeFit = neg ? ~tar_op.args[argnum].value : tar_op.args[argnum].value;
    for(const Binatr::Detail& det : instemp.m_compound)
        if (det.tag != Binatr::Detail::D_STATIC && det.arVecNum == argnum)
        {
            Assert(det.tag != Binatr::Detail::D_REG);
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

size_t Backend::spillSpaceNeeded(const Syntop& a_op) const
{
    return 0;
}

std::set<size_t> Backend::getUsedRegistersIdxs(const loops::Syntop &a_op, uint64_t flagmask) const
{
    bool foundMnemotr;
    Mnemotr ret = m_m2mlookup(a_op, foundMnemotr);
    std::set<size_t> result;
    if (!foundMnemotr)
        return result;
    const Mnemotr& m2m = lookM2m(a_op);
    Syntop tarop = m2m.apply(a_op);
    const Binatr& m2b = lookM2b(tarop);
    size_t bpiecenum = 0;
    for (size_t argnum = 0; argnum < m2m.m_argsList.size(); ++argnum)
    {
        while (bpiecenum < m2b.size() && m2b.m_compound[bpiecenum].tag == Binatr::Detail::D_STATIC) ++bpiecenum;  //Drop all binatr statics
        const Mnemotr::Argutr& ar = m2m.m_argsList[m2b.m_compound[bpiecenum].arVecNum];
        if (ar.tag == Mnemotr::Argutr::T_FROMSOURCE)
        {
            if (ar.srcArgnum >= a_op.size())
                throw std::runtime_error("Binary translator: non-existent argument is requested.");
            if (a_op[ar.srcArgnum].tag == Arg::IREG && ((m2b.m_compound[bpiecenum].fieldOflags & flagmask) == flagmask))
                result.insert(ar.srcArgnum);
        }
        ++bpiecenum; //Drop one biantr argument.
    }
    return result;
}

std::set<size_t> Backend::getOutRegistersIdxs(const Syntop& a_op) const
{
    return getUsedRegistersIdxs(a_op, Binatr::Detail::D_OUTPUT);
}

std::set<size_t> Backend::getInRegistersIdxs(const Syntop& a_op) const
{
    return getUsedRegistersIdxs(a_op, Binatr::Detail::D_INPUT);
}

std::set<IRegInternal> Backend::getUsedRegisters(const Syntop& a_op, uint64_t flagmask) const
{
    std::set<size_t> preres = getUsedRegistersIdxs(a_op, flagmask);
    std::set<IRegInternal> result;
    for(size_t arnum: preres)
    {
        if(arnum >= a_op.size())
            throw std::runtime_error("Compile error: non-existent argument is requested.");
        if(a_op[arnum].tag != Arg::IREG)
            throw std::runtime_error("Compile error: constant is requested instead of register.");
        result.insert(a_op[arnum].idx);
    }
    return result;
}

std::set<IRegInternal> Backend::getOutRegisters(const Syntop& a_op) const
{
    return getUsedRegisters(a_op, Binatr::Detail::D_OUTPUT);
}

std::set<IRegInternal> Backend::getInRegisters(const Syntop& a_op) const
{
    return getUsedRegisters(a_op, Binatr::Detail::D_INPUT);
}

Syntfunc Backend::bytecode2Target(const Syntfunc& a_bcfunc) const
{
    Syntfunc result;
    result.params = a_bcfunc.params;
    result.program.reserve(a_bcfunc.program.size());
    result.name = a_bcfunc.name;
    m_m2mCurrentOffset = 0;
    for(const Syntop& op: a_bcfunc.program)
    {
        size_t curr_tar_op = result.program.size();
        if(!this->handleBytecodeOp(op, result)) //Philosophically, we have to ask map BEFORE overrules, not after.
        {
            result.program.emplace_back(lookM2m(op).apply(op, this));
        }
        for(size_t addedop = curr_tar_op; addedop<result.program.size(); addedop++)
        {
            const Syntop& lastop = result.program[addedop];
            m_m2mCurrentOffset += lookM2b(lastop).size();
        }
    }
    return result;
}

const FuncBodyBuf Backend::target2Hex(const Syntfunc& a_bcfunc) const
{
    Bitwriter bitstream(this);
    for (const Syntop& op : a_bcfunc.program)
        lookM2b(op).applyNAppend(op, &bitstream);
    return bitstream.buffer();
}

void* Backend::compile(Context* a_ctx, Func* a_func) const
{
    FuncImpl* func = static_cast<FuncImpl*>(a_func);
    Syntfunc tarcode = bytecode2Target(func->getData());
    const FuncBodyBuf body = target2Hex(tarcode);
    
    Allocator* alloc = m_exeAlloc;
    uint8_t* exebuf = alloc->allocate(body->size());
    
    memcpy(exebuf, (void*)&(body->operator[](0)), body->size()); //TODO(ch): You have to change used adressess before.
    
    alloc->protect2Execution(exebuf);
    return exebuf;
}

Binatr binatrLookup(const Syntop&, bool&)
{
    throw std::runtime_error("Binatr table is not implemented.");
}

Mnemotr mnemotrLookup(const Syntop&, bool&)
{
    throw std::runtime_error("Mnemotr table is not implemented.");
}

Backend::Backend() : m_exeAlloc(nullptr)
, m_isLittleEndianInstructions(true)
, m_isLittleEndianOperands(false)
, m_isMonowidthInstruction(false)
, m_instructionWidth(0)
, m_registersAmount(8)
, m_m2mCurrentOffset(0)
, m_m2blookup(binatrLookup)
, m_m2mlookup(mnemotrLookup)
    {}
};
