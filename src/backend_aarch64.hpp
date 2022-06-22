/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_BACKEND_AARCH64_HPP__
#define __LOOPS_BACKEND_AARCH64_HPP__

#include "composer.hpp"
#include "backend.hpp"
#include "common.hpp"
#include <vector>
#include <map>

namespace loops
{
enum {
    AARCH64_LDR = 0,
    AARCH64_LDRSW,
    AARCH64_LDRH,
    AARCH64_LDRSH,
    AARCH64_LDRB,
    AARCH64_LDRSB,
    AARCH64_STR,
    AARCH64_STRH,
    AARCH64_STRB,
    AARCH64_MOV,
    AARCH64_ADD,
    AARCH64_SUB,
    AARCH64_MUL,
    AARCH64_SDIV,
    AARCH64_LSL,
    AARCH64_LSR,
    AARCH64_ASR,
    AARCH64_AND,
    AARCH64_ORR,
    AARCH64_EOR,
    AARCH64_MVN,
    AARCH64_NEG,
    AARCH64_CMP,
    AARCH64_CSEL,
    AARCH64_CINC,
    AARCH64_CNEG,
    AARCH64_B,
    AARCH64_B_NE,
    AARCH64_B_EQ,
    AARCH64_B_LT,
    AARCH64_B_LE,
    AARCH64_B_GT,
    AARCH64_B_GE,
    AARCH64_RET
};

class Aarch64Backend : public Backend
{
public:
    Aarch64Backend();
    virtual size_t spillSpaceNeeded(const Syntop& a_op) const override final;
    virtual std::set<size_t> getUsedRegistersIdxs(const Syntop& a_op, uint64_t flagmask = BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) const override final;
    virtual Syntfunc bytecode2Target(const Syntfunc& a_bcfunc) const override final;
    virtual size_t stackGrowthAlignment(size_t stackGrowth) const override final;
    virtual size_t stackParamOffset(size_t a_nettoSpills, size_t a_snippetCausedSpills) const override final;
    virtual Arg getSParg(Func* funcimpl) const override final;
    virtual std::unordered_map<int, std::string> getOpStrings() const override final;
    virtual Printer::ColPrinter colHexPrinter(const Syntfunc& toP) const override final;
    virtual Printer::ArgPrinter argPrinter(const Syntfunc& toP) const override final;
    virtual void switchOnSpillStressMode() override final;
private:
    struct label_ref_info
    {
        size_t opnum;
        size_t argnum;
        size_t opoffset;
        label_ref_info(): opnum(0), argnum(0), opoffset(0) {}
        label_ref_info(size_t a_opnum, size_t a_argnum, size_t a_opoffset): opnum(a_opnum), argnum(a_argnum), opoffset(a_opoffset) {}
    };

    mutable IRegInternal m_retReg;
    mutable std::unordered_map<size_t, size_t> m_labelMap;
    mutable std::unordered_map<size_t, std::vector<label_ref_info> > m_labelRefMap; // label

    virtual bool handleBytecodeOp(const Syntop& a_btop, Syntfunc& a_formingtarget) const override final;
};

};
#endif //__LOOPS_BACKEND_AARCH64_HPP__
