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
    if(m_2tararch.count(a_op.opcode) == 0)
        throw std::string("Mnemonic translator: undefined operation.");
    const Mnemotr& m2m = m_2tararch.at(a_op.opcode)[a_op];
    argnum = m2m.targetArgNum(argnum);
    if(argnum == Mnemotr::ARG_NOT_USED)
        return true;
    Syntop tar_op = m2m.apply(a_op);
    if(m_2binary.count(tar_op.opcode) == 0)
        throw std::string("Binary translator: undefined operation.");
    const Binatr& instemp = m_2binary.at(tar_op.opcode)[tar_op];
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

Syntfunc BackendImpl::bytecode2Target(const Syntfunc& a_bcfunc) const
{
    Syntfunc result;
    result.m_params = a_bcfunc.m_params;
    result.m_retreg = a_bcfunc.m_retreg;
    result.m_program.reserve(a_bcfunc.m_program.size());
    for(size_t parreg = 0; parreg<a_bcfunc.m_params.size(); parreg++ ) //TODO(ch): You need to analyze arguments better.
        translateReg(parreg);
    //TODO(ch): there must be function prologue definition.
    m_m2mCurrentOffset = 0;
    for(const Syntop& op: a_bcfunc.m_program)
    {
        size_t curr_tar_op = result.m_program.size();
        if(!this->handleBytecodeOp(op, result)) //Philosophically, we have to ask map BEFORE overrules, not after.
        {
            if(m_2tararch.count(op.opcode) == 0)
                throw std::string("AArch64: unsupported bytecode instruction.");
            result.m_program.emplace_back(m_2tararch.at(op.opcode)[op].apply(op,this));
        }
        for(size_t addedop = curr_tar_op; addedop<result.m_program.size(); addedop++)
        {
            const Syntop& lastop = result.m_program[addedop];
            m_m2mCurrentOffset += m_2binary.at(lastop.opcode)[lastop].size();
        }
    }
    return result;
}

const FuncBodyBuf BackendImpl::target2Hex(const Syntfunc& a_bcfunc) const
{
    Bitwriter bitstream(this);
    for(const Syntop& op : a_bcfunc.m_program)
    {
        auto snippetrator = m_2binary.find(op.opcode);
        if (snippetrator == m_2binary.end())
            throw std::string("Synt2bin translation: unknown instruction");
        const Binatr& bitexpansioner = snippetrator->second[op];
        bitexpansioner.applyNAppend(op, &bitstream);
    }
    
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
