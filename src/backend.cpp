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

Syntop Mnemotr::apply(const Syntop& a_source, const Backend* a_bcknd) const
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
                    throw std::string("Mnemonic translator: non-existent argument is requested.");
                Arg toAdd = a_source.args[argt.srcArgnum];
                toAdd.flags |= argt.transitFlags;
                resargs.push_back(toAdd);
                break;
            }
            case Argutr::T_TRANSFORMTOSPILL:
            {
                Assert(argt.srcArgnum < a_source.size());
                Assert(a_source.args[argt.srcArgnum].tag == Arg::ICONST);
                Arg toAdd = argISpilled(a_source.args[argt.srcArgnum].value);
                toAdd.flags |= argt.transitFlags;
                toAdd.value *= 8;//TODO(ch): It's intel-specific(well, actually ISPILLED is also intel specific.) 
                resargs.push_back(toAdd);
                break;
            }
            default:
                throw std::string("Mnemonic translator: unknown type of argument translation.");
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

bool Backend::isConstFit(const Syntop& a_op, size_t argnum) const
{
    const Mnemotr& m2m = m_2tararch[a_op];
    argnum = m2m.targetArgNum(argnum);
    if(argnum == Mnemotr::ARG_NOT_USED)
        return true;
    Syntop tar_op = m2m.apply(a_op);
    const Binatr& instemp = m_2binary[tar_op];
    if(argnum >= a_op.size())
        throw std::string("Binary translator: non-existent argument is requested.");
    if(a_op.args[argnum].tag != Arg::ICONST)
        throw std::string("Binary translator: requested register instead of const.");

    uint64_t val2BeFit = a_op.args[argnum].value;
    for(const Binatr::Detail& det : instemp. m_compound)
        if(det.tag != Binatr::Detail::D_STATIC)
        {
            if(argnum == 0)
            {
                if(det.tag == Binatr::Detail::D_REG)
                    throw std::string("Binary translator: register instead of const.");
                size_t bitwneeded = 0;
                for (;bitwneeded < 63; bitwneeded++) //TODO(ch): what a shame! Give normal implementation! AND!!! Use info about type(SIGNED/UNSIGNED offsets, adresses, etc.)
                    if(val2BeFit <= (((uint64_t)(1))<<bitwneeded))
                        break;
                return (bitwneeded <= det.width);
            }
            argnum--;
        }
    throw std::string("Binary translator: non-existent argument is requested.");
}

std::set<size_t> Backend::filterStackPlaceable(const Syntop& a_op, const std::set<size_t>& toFilter) const
{
    std::set<size_t> result;
    if (!m_2tararch.has(a_op.opcode))
        return result;
    std::map<size_t, size_t> backArgMap;
    std::set<size_t> tarTFilter;
    const Mnemotr& m2m = m_2tararch[a_op];
    Syntop tar_op = m2m.apply(a_op);
    for (size_t tF : toFilter)
    {
        Assert(tF < a_op.size() && a_op[tF].tag == Arg::IREG);
        size_t tArgnum = m2m.targetArgNum(tF);
        if (tArgnum == Mnemotr::ARG_NOT_USED)
            continue;
        tarTFilter.insert(tArgnum);
        backArgMap[tArgnum] = tF;
    }
    tarTFilter = m_2binary.filterStackPlaceable(tar_op, tarTFilter);
    for (size_t tR : tarTFilter)
        result.insert(backArgMap[tR]);
    return result;
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
    std::set<size_t> result;
    if (!m_2tararch.has(a_op.opcode))
        return result;
    const Mnemotr& m2m = m_2tararch[a_op];
    Syntop tarop = m2m.apply(a_op);
    const Binatr& m2b = m_2binary[tarop];
    size_t bpiecenum = 0;
    std::vector<size_t> rodr = m2b.m_reordering;
    if (rodr.empty())
    {
        rodr.reserve(tarop.size());
        for (size_t n = 0; n < tarop.size(); n++) 
            rodr.push_back(n);
    }

    for (size_t argnum = 0; argnum < m2m.m_argsList.size(); ++argnum)
    {
        while (bpiecenum < m2b.size(a_op) && m2b.m_compound[bpiecenum].tag == Binatr::Detail::D_STATIC) ++bpiecenum;  //Drop all binatr statics
        const Mnemotr::Argutr& ar = m2m.m_argsList[rodr[argnum]];
        if(ar.tag == Mnemotr::Argutr::T_FROMSOURCE)
        {
            if (ar.srcArgnum >= a_op.size())
                throw std::string("Binary translator: non-existent argument is requested.");
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
            throw std::string("Compile error: non-existent argument is requested.");
        if(a_op[arnum].tag != Arg::IREG)
            throw std::string("Compile error: constant is requested instead of register.");
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
            OpPrintInfo pinfo;
            result.program.emplace_back(m_2tararch[op].apply(op, this));
        }
        for(size_t addedop = curr_tar_op; addedop<result.program.size(); addedop++)
        {
            const Syntop& lastop = result.program[addedop];
            m_m2mCurrentOffset += m_2binary[lastop].size(lastop);
        }
    }
    return result;
}

const FuncBodyBuf Backend::target2Hex(const Syntfunc& a_bcfunc) const
{
    Bitwriter bitstream(this);
    for (const Syntop& op : a_bcfunc.program)
        m_2binary[op].applyNAppend(op, &bitstream);
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

OpPrintInfo Backend::getPrintInfo(const Syntop& op)
{
    OpPrintInfo res;
    if(m_2binary.has(op.opcode))
        res = m_2binary[op].getPrintInfo(op);
    return res;
}

size_t Backend::registersAmount() const
{ //TODO(ch): Implement more effective.
    std::set<IRegInternal> allRegisters;
    allRegisters.insert(m_parameterRegisters.begin(), m_parameterRegisters.end());
    allRegisters.insert(m_returnRegisters.begin(), m_returnRegisters.end());
    allRegisters.insert(m_calleeSavedRegisters.begin(), m_calleeSavedRegisters.end());
    allRegisters.insert(m_callerSavedRegisters.begin(), m_callerSavedRegisters.end());
    return allRegisters.size();
}

Backend::Backend() : m_exeAlloc(nullptr)
, m_isLittleEndianInstructions(true)
, m_isLittleEndianOperands(false)
, m_isMonowidthInstruction(false)
, m_instructionWidth(0)
, m_registersAmount(8)
, m_m2mCurrentOffset(0)
    {}
};
