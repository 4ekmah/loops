/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "backend.hpp"
#include "func_impl.hpp"

namespace loops
{

Mnemotr::Argutr::Argutr(const Arg& a_fixed) : tag(T_FIXED), fixed(a_fixed) {}
Mnemotr::Argutr::Argutr(size_t a_src_arnum) : tag(T_FROMSOURCE), srcArgnum(a_src_arnum) {}
Mnemotr::Mnemotr(int a_tarop, std::initializer_list<Argutr> a_args) : m_tarop(a_tarop), m_argsList(a_args){}

Syntop Mnemotr::apply(const Syntop& a_source, const BackendImpl* a_bcknd) const
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
                toAdd = (a_bcknd != nullptr && toAdd.tag == Arg::IREG) ? a_bcknd->translateReg(toAdd.idx): toAdd;
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

bool BackendImpl::isConstFit(const Syntop& a_op, size_t argnum) const
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

std::set<size_t> BackendImpl::getUsedRegistersIdxs(const loops::Syntop &a_op, uint64_t flagmask) const
{
    std::set<size_t> result;
    if (!m_2tararch.has(a_op.opcode))
        return result;
    const Mnemotr& m2m = m_2tararch[a_op];
    const Binatr& m2b = m_2binary[m2m.apply(a_op)];
    size_t bpiecenum = 0;
    for(size_t argnum = 0; argnum < m2m.m_argsList.size(); ++argnum)
    {
        while(bpiecenum < m2b.size() && m2b.m_compound[bpiecenum].tag == Binatr::Detail::D_STATIC) ++bpiecenum;  //Drop all binatr statics
        const Mnemotr::Argutr& ar = m2m.m_argsList[argnum];
        if(ar.tag == Mnemotr::Argutr::T_FROMSOURCE)
        {
            if (ar.srcArgnum >= a_op.size())
                throw std::string("Binary translator: non-existent argument is requested.");
            if (a_op[ar.srcArgnum].tag == Arg::IREG && m2b.m_compound[bpiecenum].fieldOflags & flagmask)
                result.insert(ar.srcArgnum);
        }
        ++bpiecenum; //Drop one biantr argument.
    }
    return result;
}

std::set<size_t> BackendImpl::getOutRegistersIdxs(const Syntop& a_op) const
{
    return getUsedRegistersIdxs(a_op, Binatr::Detail::D_OUTPUT);
}

std::set<size_t> BackendImpl::getInRegistersIdxs(const Syntop& a_op) const
{
    return getUsedRegistersIdxs(a_op, Binatr::Detail::D_INPUT);
}

std::set<IRegInternal> BackendImpl::getUsedRegisters(const Syntop& a_op, uint64_t flagmask) const
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

std::set<IRegInternal> BackendImpl::getOutRegisters(const Syntop& a_op) const
{
    return getUsedRegisters(a_op, Binatr::Detail::D_OUTPUT);
}

std::set<IRegInternal> BackendImpl::getInRegisters(const Syntop& a_op) const
{
    return getUsedRegisters(a_op, Binatr::Detail::D_INPUT);
}

Syntfunc BackendImpl::bytecode2Target(const Syntfunc& a_bcfunc) const
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
            result.program.emplace_back(m_2tararch[op].apply(op,this));
        }
        for(size_t addedop = curr_tar_op; addedop<result.program.size(); addedop++)
        {
            const Syntop& lastop = result.program[addedop];
            m_m2mCurrentOffset += m_2binary[lastop].size();
        }
    }
    return result;
}

const FuncBodyBuf BackendImpl::target2Hex(const Syntfunc& a_bcfunc) const
{
    Bitwriter bitstream(this);
    for(const Syntop& op : a_bcfunc.program)
        m_2binary[op].applyNAppend(op, &bitstream);
    return bitstream.buffer();
}

void* BackendImpl::compile(Context* a_ctx, Func* a_func) const
{
    FuncImpl* func = static_cast<FuncImpl*>(a_func);
    Syntfunc tarcode = bytecode2Target(func->getData());
    const FuncBodyBuf body = target2Hex(tarcode);
    
    Allocator* alloc = m_exeAlloc;
    uint8_t* exebuf = alloc->allocate(body->size());
    
    memcpy(exebuf, (void*)&(body->operator[](0)), body->size()); //TODO(ch): You have to change used adresses before.
    
    alloc->protect2Execution(exebuf);
    return exebuf;
}
};
