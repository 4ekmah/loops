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
