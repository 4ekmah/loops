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
SyntopTranslation::ArgTranslation::ArgTranslation(const Arg& a_fixed) : tag(T_FIXED), fixed(a_fixed), srcArgnum(UNDEFINED_ARGUMENT_NUMBER), transitFlags(0) {}
SyntopTranslation::ArgTranslation::ArgTranslation(int a_srcArgnum, uint64_t flags) : tag(T_FROMSOURCE), srcArgnum(a_srcArgnum), transitFlags(flags) {}
SyntopTranslation::ArgTranslation::ArgTranslation(int a_srcArgnum, int a_elemtype, uint64_t flags) : tag(T_SETELEMTYPE), srcArgnum(a_srcArgnum), elemtype(a_elemtype), transitFlags(flags) {}
SyntopTranslation::SyntopTranslation(int a_tarop, std::initializer_list<ArgTranslation> a_args) : m_tarop(a_tarop), m_args_size((int)a_args.size())
{
    if(m_args_size > ARGS_MAX)
        throw loops::exception(LOOPS_ERR_TOO_MUCH_ARGS);
    int argnum = 0;
    for(const ArgTranslation& arg : a_args)
        m_argsList[argnum++] = arg;
}

Syntop SyntopTranslation::apply(const Syntop& a_source, const Backend* a_backend) const
{
    Syntop result;
    apply(a_source, result, a_backend);
    return result;
}

void SyntopTranslation::apply(const Syntop& a_source, Syntop& result, const Backend*) const
{
    result.opcode = m_tarop;
    result.args_size = 0;
    for(int argnum = 0; argnum < m_args_size; ++argnum)
    {
        const ArgTranslation& argt = m_argsList[argnum];
        switch (argt.tag) {
            case ArgTranslation::T_FIXED:
                result.args[result.args_size++] = argt.fixed;
                break;
            case ArgTranslation::T_FROMSOURCE:
            {
                if(argt.srcArgnum == UNDEFINED_ARGUMENT_NUMBER || argt.srcArgnum >= (int)a_source.size())
                    throw loops::exception(LOOPS_SYN_T_NON_EXISTENT_ARG);
                Arg toAdd = a_source.args[argt.srcArgnum];
                toAdd.flags |= argt.transitFlags;
                result.args[result.args_size++] = toAdd;
                break;
            }
            case ArgTranslation::T_SETELEMTYPE:
            {
                if(argt.srcArgnum == UNDEFINED_ARGUMENT_NUMBER || argt.srcArgnum >= (int)a_source.size())
                    throw loops::exception(LOOPS_SYN_T_NON_EXISTENT_ARG);
                Arg toAdd = a_source.args[argt.srcArgnum];
                toAdd.flags |= argt.transitFlags;
                toAdd.elemtype = argt.elemtype;
                result.args[result.args_size++] = toAdd;
                break;
            }
            case ArgTranslation::T_TRANSFORMTOSPILL:
            {
                LOOPS_ASSERT(argt.srcArgnum < a_source.size() && argt.srcArgnum != UNDEFINED_ARGUMENT_NUMBER);
                LOOPS_ASSERT(a_source.args[argt.srcArgnum].tag == Arg::IIMMEDIATE);
                Arg toAdd = argSpilled(RB_INT, a_source.args[argt.srcArgnum].value);
                toAdd.flags |= argt.transitFlags;
                result.args[result.args_size++] = toAdd;
                break;
            }
            case ArgTranslation::T_COPYSHIFTRIGHT:
            {
                LOOPS_ASSERT(argt.srcArgnum < a_source.size() && argt.srcArgnum != UNDEFINED_ARGUMENT_NUMBER);
                LOOPS_ASSERT(a_source.args[argt.srcArgnum].tag == Arg::IIMMEDIATE);
                Arg toAdd = a_source.args[argt.srcArgnum];
                if (argt.fixed.value >= 0){
                    toAdd.value >>= argt.fixed.value;
                    if((toAdd.value << argt.fixed.value) != a_source.args[argt.srcArgnum].value)
                        throw loops::exception("Syntop translator: argument alignment error.");
                }
                else {
                    toAdd.value <<= std::abs(argt.fixed.value);
                    if((toAdd.value >> std::abs(argt.fixed.value)) != a_source.args[argt.srcArgnum].value)
                        throw loops::exception("Syntop translator: argument alignment error.");
                }
                toAdd.flags |= argt.transitFlags;
                result.args[result.args_size++] = toAdd;
                break;
            }
            default:
                throw loops::exception("Syntop translator: unknown type of argument translation.");
        }
    }
}

int SyntopTranslation::targetArgNum(int a_srcnum) const
{
    int res = 0;
    for(;res < m_args_size; ++res)
        if((m_argsList[res].tag == ArgTranslation::T_FROMSOURCE || m_argsList[res].tag == ArgTranslation::T_SETELEMTYPE) && m_argsList[res].srcArgnum == a_srcnum)
            break;
    if (res == m_args_size)
        res = ARG_NOT_USED;
    return res;
}

AllocationRestriction AllocationRestriction::makeNone()
{
    AllocationRestriction result;
    result.type = AllocationRestriction::AR_NONE;
    return result;
}

AllocationRestriction AllocationRestriction::makeInplace(int output_num, int input_num)
{
    AllocationRestriction result;
    result.type = AllocationRestriction::AR_INPLACE;
    result.inplace.m_output_num = output_num;
    result.inplace.m_input_num = input_num;
    result.inplace.m_commutative = false;
    return result;
}
AllocationRestriction AllocationRestriction::makeInplaceUnary(int output_num, int input_num)
{
    return makeInplaceCommutative(output_num, input_num, UNDEFINED_ARGUMENT_NUMBER);
}

AllocationRestriction AllocationRestriction::makeInplaceCommutative(int output_num, int input_num, int input_aux_num)
{
    AllocationRestriction result;
    result.type = AllocationRestriction::AR_INPLACE;
    result.inplace.m_output_num = output_num;
    result.inplace.m_input_num = input_num;
    result.inplace.m_input_aux_num = input_aux_num;
    result.inplace.m_commutative = true;
    return result;
}

AllocationRestriction AllocationRestriction::makeFixed(const std::vector<std::pair<int, RegIdx> >& pins, const std::array<std::vector<RegIdx>, RB_AMOUNT>& clobbers)
{
    AllocationRestriction result;
    result.type = AllocationRestriction::AR_FIXED;
    result.fixed.m_pins = pins;
    result.fixed.m_clobbers = clobbers;
    return result;
}

AllocationRestriction AllocationRestriction::makeConsecutive(int basket_num, int arg_start, int amount)
{
    AllocationRestriction result;
    result.type = AllocationRestriction::AR_CONSECUTIVE;
    result.consecutive.m_basket_num = basket_num;
    result.consecutive.m_arg_start = arg_start;
    result.consecutive.m_amount = amount;
    return result;
}

AllocationRestriction operator|(const AllocationRestriction& r0, const AllocationRestriction& r1)
{
    LOOPS_ASSERT((r0.type&r1.type) == 0);
    AllocationRestriction result = r0;
    result.type = result.type | r1.type;
    if(r1.type&AllocationRestriction::AR_CONSECUTIVE)
        result.consecutive = r1.consecutive; 
    if(r1.type&AllocationRestriction::AR_FIXED)
        result.fixed = r1.fixed; 
    if(r1.type&AllocationRestriction::AR_INPLACE)
        result.inplace = r1.inplace;
    return result;
}

Backend::~Backend()
{}

bool Backend::isImmediateFit(const Syntop& a_op, int argnum) const
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
    LOOPS_ASSERT(argnum < (int)tar_op.size() && tar_op.args[argnum].tag == Arg::IIMMEDIATE);
    bool neg = tar_op.args[argnum].value < 0;
    uint64_t val2BeFit = neg ? ~tar_op.args[argnum].value : tar_op.args[argnum].value;
    for(const BinTranslation::Token& det : instemp)
        if(det.srcArgnum == argnum)
        {
            if(det.tag == BinTranslation::Token::T_OMIT)
                return true; //Encoder checked everything on his side.
            if(det.tag != BinTranslation::Token::T_STATIC)
            {
                LOOPS_ASSERT(det.tag != BinTranslation::Token::T_REG);
                int bitwneeded = msb64(val2BeFit);
                bitwneeded += neg ? 1 : 0;
                return (bitwneeded < det.width);
            }
        }
    throw loops::exception("Binary translator: non-existent argument is requested.");
}

fastset Backend::filterStackPlaceable(const Syntop& /*a_op*/, const fastset& /*toFilter*/) const
{
    return fastset();
}

int Backend::reusingPreferences(const Syntop& a_op, const fastset& undefinedArgNums) const
{
    if(a_op.opcode == OP_MOV) 
        if (undefinedArgNums.count(1))
            return 1;
    return UNDEFINED_ARGUMENT_NUMBER;
}

int Backend::spillSpaceNeeded(const Syntop& /*a_op*/, int /*basketNum*/) const
{
    return 0;
}

fastset Backend::getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask) const
{
    const uint64_t key = syntopKey(a_op);
    if(flagmask == AF_INPUT || flagmask == AF_OUTPUT)
    {
        const fasthash& table = flagmask == AF_INPUT ? inputAccelerationTable() : outputAccelerationTable();
        const uint64_t* packed = table.find(key);
        if(packed)
            return unpackFastset(*packed, basketNum);
    }
    else if(flagmask == (AF_INPUT | AF_OUTPUT))
    {
        const uint64_t* inputs = inputAccelerationTable().find(key);
        const uint64_t* outputs = outputAccelerationTable().find(key);
        if(inputs && outputs)
            return unpackFastset(*inputs & *outputs, basketNum);
    }
    return getUsedRegistersIdxsUncached(a_op, basketNum, flagmask);
}

InOutRegisterIdxs Backend::getInOutRegistersIdxs(const Syntop& a_op, int basketNum) const
{
    const uint64_t key = syntopKey(a_op);
    const uint64_t* inputs = inputAccelerationTable().find(key);
    const uint64_t* outputs = outputAccelerationTable().find(key);
    if(inputs && outputs)
        return InOutRegisterIdxs{unpackFastset(*inputs, basketNum), unpackFastset(*outputs, basketNum)};
    return InOutRegisterIdxs{getUsedRegistersIdxsUncached(a_op, basketNum, AF_INPUT),
                             getUsedRegistersIdxsUncached(a_op, basketNum, AF_OUTPUT)};
}

fastset Backend::getUsedRegistersIdxsUncached(const Syntop& a_op, int basketNum, uint64_t flagmask) const
{
    fastset result;
    if(a_op.opcode == OP_DEF || a_op.opcode == VOP_DEF)
    {
        int ioflags = flagmask&(AF_OUTPUT | AF_INPUT);
        if (ioflags == AF_OUTPUT && 
            ((a_op.opcode == OP_DEF && basketNum == RB_INT) || (a_op.opcode == VOP_DEF && basketNum == RB_VEC)))
                result = fastset({0});
        return result;
    }
    bool foundSynTr;
    SyntopTranslation s2s = m_s2slookup(this, a_op, foundSynTr);
    if (!foundSynTr)
        return result;
    Syntop tarop = s2s.apply(a_op);
    const BinTranslation& s2b = lookS2b(tarop);
    int desiredRegType = ((basketNum == RB_INT) ? Arg::IREG :Arg::VREG);
    for(int bpiecenum = 0; bpiecenum < s2b.m_compound_size; ++bpiecenum)
    {
        if(s2b.m_compound[bpiecenum].tag == BinTranslation::Token::T_STATIC)
            continue;   //Drop all statics
        const SyntopTranslation::ArgTranslation& ar = s2s.m_argsList[s2b.m_compound[bpiecenum].srcArgnum];
        if (ar.tag == SyntopTranslation::ArgTranslation::T_FROMSOURCE || ar.tag == SyntopTranslation::ArgTranslation::T_SETELEMTYPE )
        {
            if (ar.srcArgnum == SyntopTranslation::ARG_NOT_USED || ar.srcArgnum >= a_op.size())
                throw loops::exception("Binary translator: non-existent argument is requested.");
            if (a_op[ar.srcArgnum].tag == desiredRegType && ((s2b.m_compound[bpiecenum].fieldOflags & flagmask) == flagmask))
                result.insert(ar.srcArgnum);
        }
    }
    return result;
}

fastset Backend::getOutRegistersIdxs(const Syntop& a_op, int basketNum) const
{
    return getUsedRegistersIdxs(a_op, basketNum, AF_OUTPUT);
}

fastset Backend::getInRegistersIdxs(const Syntop& a_op, int basketNum) const
{
    return getUsedRegistersIdxs(a_op, basketNum, AF_INPUT);
}

std::set<RegIdx> Backend::getUsedRegisters(const Syntop& a_op, int basketNum, uint64_t flagmask) const
{
    fastset preres = getUsedRegistersIdxs(a_op, basketNum, flagmask);
    std::set<RegIdx> result;
    for(int argNum: preres)
    {
        if(argNum >= a_op.size())
            throw loops::exception("Compile error: non-existent argument is requested.");
        if(a_op[argNum].tag != Arg::IREG && a_op[argNum].tag != Arg::VREG)
            throw loops::exception("Compile error: constant is requested instead of register.");
        result.insert(a_op[argNum].idx);
    }
    return result;
}

std::set<RegIdx> Backend::getOutRegisters(const Syntop& a_op, int basketNum) const
{
    return getUsedRegisters(a_op, basketNum, AF_OUTPUT);
}

std::set<RegIdx> Backend::getInRegisters(const Syntop& a_op, int basketNum) const
{
    return getUsedRegisters(a_op, basketNum, AF_INPUT);
}

AllocationRestriction Backend::getRestriction(const Syntop& a_op) const
{
    (void)a_op;
    return AllocationRestriction::makeNone();
}

void Backend::fill_native_operand_flags(const Syntop* a_op, uint64_t* result) const
{
    memset(result, 0, sizeof(uint64_t) * Syntop::SYNTOP_ARGS_MAX);
    const BinTranslation& s2b = lookS2b(*a_op);
    for(int bpiecenum = 0; bpiecenum < s2b.m_compound_size; ++bpiecenum)
    {
        if(s2b.m_compound[bpiecenum].tag == BinTranslation::Token::T_STATIC)
            continue;   //Drop all statics
        int srcNum = s2b.m_compound[bpiecenum].srcArgnum;
        result[srcNum] = result[srcNum] | s2b.m_compound[bpiecenum].fieldOflags;
    }
}

BinTranslation BTLookup(const Syntop&, bool&)
{
    throw loops::exception("Binary translation table is not implemented.");
}

SyntopTranslation STLookup(const Backend*, const Syntop&, bool&)
{
    throw loops::exception("Syntop translation table is not implemented.");
}

Backend::Backend() : m_s2blookup(BTLookup)
, m_s2slookup(STLookup)
, m_isLittleEndianInstructions(true)
, m_isLittleEndianOperands(false)
, m_isMonowidthInstruction(false)
, m_instructionWidth(0)
, m_registersAmount(8)
    {}
}
