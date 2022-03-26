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

class aarch64_backend : public backend_impl
{
    struct label_ref_info
    {
        size_t m_opnum;
        size_t m_argnum;
        size_t m_opoffset;
        label_ref_info(): m_opnum(0), m_argnum(0), m_opoffset(0) {}
        label_ref_info(size_t a_opnum, size_t a_argnum, size_t a_opoffset): m_opnum(a_opnum), m_argnum(a_argnum), m_opoffset(a_opoffset) {}
    };

    mutable size_t m_nextidx; //TODO(ch): Do something with thread-safety.
    mutable std::unordered_map<size_t, size_t> m_regmap;
    mutable IRegInternal m_retreg;
    mutable int m_lastcondition; //TODO(ch): scheme of using this variable isn't ok. Think, we have to avoid OP_CMP_GT etc. logic. Use standard OP_CMP instead.
    mutable std::unordered_map<size_t, size_t> m_labelmap;
    mutable std::unordered_map<size_t, std::vector<label_ref_info> > m_labelrefmap; // label

    virtual bool handle_bytecodeop(const syntop& a_btop, syntfunc& a_formingtarget) const override final;
public:
    aarch64_backend();
    virtual void* compile(Context* a_ctx, Func* a_func) const override final;
    virtual syntfunc bytecode2target(const syntfunc& a_bcfunc) const override final;
    virtual Arg translate_reg(IRegInternal tofind) const override final;
    virtual std::unordered_map<int, std::string> get_op_strings() const override final;
};

};
#endif //__LOOPS_AARCH64_HPP__
