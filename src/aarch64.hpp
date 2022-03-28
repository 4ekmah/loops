/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_AARCH64_HPP__
#define __LOOPS_AARCH64_HPP__

#include "composer.hpp"
#include "backend.hpp"
#include "common.hpp"
#include <vector>
#include <map>

namespace loops
{
enum {
    A64_LDRSW = 0,
    A64_STR32_I,
    A64_MOV,
    A64_ADD_R,
    A64_MUL,
    A64_SDIV,
    A64_CMP_R,
    A64_B_LT,
    A64_B_LE,
    A64_B_GT,
    A64_B_GE,
    A64_RET
};

class Aarch64Backend : public BackendImpl
{
public:
    Aarch64Backend();
    virtual Syntfunc bytecode2Target(const Syntfunc& a_bcfunc) const override final;
    virtual Arg translateReg(IRegInternal tofind) const override final;
    virtual std::unordered_map<int, std::string> getOpStrings() const override final;
private:
    struct label_ref_info
    {
        size_t opnum;
        size_t argnum;
        size_t opoffset;
        label_ref_info(): opnum(0), argnum(0), opoffset(0) {}
        label_ref_info(size_t a_opnum, size_t a_argnum, size_t a_opoffset): opnum(a_opnum), argnum(a_argnum), opoffset(a_opoffset) {}
    };

    mutable size_t m_nextidx; //TODO(ch): Do something with thread-safety.
    mutable std::unordered_map<size_t, size_t> m_regMap;
    mutable IRegInternal m_retReg;
    mutable int m_lastCondition; //TODO(ch): scheme of using this variable isn't ok. Think, we have to avoid OP_CMP_GT etc. logic. Use standard OP_CMP instead.
    mutable std::unordered_map<size_t, size_t> m_labelMap;
    mutable std::unordered_map<size_t, std::vector<label_ref_info> > m_labelRefMap; // label

    virtual bool handleBytecodeOp(const Syntop& a_btop, Syntfunc& a_formingtarget) const override final;
};

};
#endif //__LOOPS_AARCH64_HPP__
