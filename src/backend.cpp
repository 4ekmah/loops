/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "backend.hpp"

namespace loops
{

mnemotr::argutr::argutr(const Arg& a_fixed) : m_tag(T_FIXED), m_fixed(a_fixed) {}
mnemotr::argutr::argutr(size_t a_src_arnum) : m_tag(T_FROMSOURCE), m_src_arnum(a_src_arnum) {}
mnemotr::mnemotr(int a_tarop, std::initializer_list<argutr> a_args) : m_tarop(a_tarop), m_argslist(a_args){}

syntop mnemotr::apply(const syntop& a_source, const backend_impl* a_bcknd) const
{
    std::vector<Arg> resargs;
    resargs.reserve(m_argslist.size());
    for(const argutr& argt : m_argslist)
        switch (argt.m_tag) {
            case argutr::T_FIXED:
                resargs.push_back(argt.m_fixed);
                break;
            case argutr::T_FROMSOURCE:
            {
                if(argt.m_src_arnum >= a_source.args.size())
                    throw std::string("Mnemonic translator: non-existent argument is requested.");
                Arg toAdd = a_source.args[argt.m_src_arnum];
                toAdd = (a_bcknd != nullptr && toAdd.tag == Arg::IREG) ? a_bcknd->translate_reg(toAdd.idx): toAdd;
                resargs.push_back(toAdd);
                break;
            }
            default:
                throw std::string("Mnemonic translator: unknown type of argument translation.");
        }
    return syntop(m_tarop, resargs);
}

size_t mnemotr::target_arg_num(size_t a_srcnum) const
{
    size_t res = 0;
    for(;res< m_argslist.size(); ++res)
        if(m_argslist[res].m_tag == argutr::T_FROMSOURCE && m_argslist[res].m_src_arnum == a_srcnum)
            break;
    if (res == m_argslist.size())
        res = ARG_NOT_USED;
    return res;
}

bool backend_impl::is_const_fit(const syntop& a_op, size_t argnum) const
{
    if(m_2tararch.count(a_op.opcode) == 0)
        throw std::string("Mnemonic translator: undefined operation.");
    const mnemotr& m2m = m_2tararch.at(a_op.opcode)[a_op.args];
    argnum = m2m.target_arg_num(argnum);
    if(argnum == mnemotr::ARG_NOT_USED)
        return true;
    syntop tar_op = m2m.apply(a_op);
    if(m_2binary.count(tar_op.opcode) == 0)
        throw std::string("Binary translator: undefined operation.");
    return m_2binary.at(tar_op.opcode)[tar_op.args].is_const_fit(tar_op, argnum);
}

syntfunc backend_impl::bytecode2target(const syntfunc& a_bcfunc) const
{
    syntfunc result;
    result.m_params = a_bcfunc.m_params;
    result.m_retreg = a_bcfunc.m_retreg;
    result.m_program.reserve(a_bcfunc.m_program.size());
    for(size_t parreg = 0; parreg<a_bcfunc.m_params.size(); parreg++ ) //TODO(ch): You need to analyze arguments better.
        translate_reg(parreg);
    //TODO(ch): there must be function prologue definition.
    m_m2m_currentoffset = 0;
    for(const syntop& op: a_bcfunc.m_program)
    {
        size_t curr_tar_op = result.m_program.size();
        if(!this->handle_bytecodeop(op, result)) //Philosophically, we have to ask map BEFORE overrules, not after.
        {
            if(m_2tararch.count(op.opcode) == 0)
                throw std::string("AArch64: unsupported bytecode instruction.");
            result.m_program.emplace_back(m_2tararch.at(op.opcode)[op.args].apply(op,this));
        }
        for(size_t addedop = curr_tar_op; addedop<result.m_program.size(); addedop++)
        {
            const syntop& lastop = result.m_program[addedop];
            m_m2m_currentoffset += m_2binary.at(lastop.opcode)[lastop.args].size();
        }
    }
    return result;
}

};
