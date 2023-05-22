/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_BACKEND_AARCH64_HPP__
#define __LOOPS_BACKEND_AARCH64_HPP__
#include "loops/defines.hpp"
#if __LOOPS_ARCH == __LOOPS_AARCH64

#include "backend.hpp"
#include "common.hpp"
#include "composer.hpp"
#include "pipeline.hpp"
#include <vector>
#include <map>

namespace loops
{
enum {
    AARCH64_LDR    =  0,
    AARCH64_LDRSW  =  1,
    AARCH64_LDRH   =  2,
    AARCH64_LDRSH  =  3,
    AARCH64_LDRB   =  4,
    AARCH64_LDRSB  =  5,
    AARCH64_LDP    =  6,
    AARCH64_STR    =  7,
    AARCH64_STRH   =  8,
    AARCH64_STRB   =  9,
    AARCH64_STP    = 10,
    AARCH64_MOV    = 11,
    AARCH64_MOVN   = 12,
    AARCH64_MOVK   = 13,
    AARCH64_ADD    = 14,
    AARCH64_SUB    = 15,
    AARCH64_MUL    = 16,
    AARCH64_SDIV   = 17,
    AARCH64_LSL    = 18,
    AARCH64_LSR    = 19,
    AARCH64_ASR    = 20,
    AARCH64_AND    = 21,
    AARCH64_ORR    = 22,
    AARCH64_EOR    = 23,
    AARCH64_MVN    = 24,
    AARCH64_BSL    = 25,
    AARCH64_NEG    = 26,
    AARCH64_FMIN   = 27,
    AARCH64_FMAX   = 28,
    AARCH64_SMINV  = 29,
    AARCH64_SMAXV  = 30,
    AARCH64_UMINV  = 31,
    AARCH64_UMAXV  = 32,
    AARCH64_FMINV  = 33,
    AARCH64_FMAXV  = 34,
    AARCH64_CMP    = 35,
    AARCH64_CSEL   = 36,
    AARCH64_CSET   = 37,
    AARCH64_CINC   = 38,
    AARCH64_CNEG   = 39,
    AARCH64_FADD   = 40,
    AARCH64_FSUB   = 41,
    AARCH64_FMUL   = 42,
    AARCH64_FDIV   = 43,
    AARCH64_FNEG   = 44,
    AARCH64_FMLA   = 45,
    AARCH64_SHL    = 46,
    AARCH64_USHL   = 47,
    AARCH64_SSHL   = 48,
    AARCH64_USHR   = 49,
    AARCH64_SSHR   = 50,
    AARCH64_MOVI   = 51,
    AARCH64_MVNI   = 52,
    AARCH64_CMHI   = 53,
    AARCH64_CMHS   = 54,
    AARCH64_CMEQ   = 55,
    AARCH64_CMGE   = 56,
    AARCH64_CMGT   = 57,
    AARCH64_CMLE   = 58,
    AARCH64_CMLT   = 59,
    AARCH64_FCMGT  = 60,
    AARCH64_FCMGE  = 61,
    AARCH64_FCMEQ  = 62,
    AARCH64_FCVTZS = 63,
    AARCH64_FCVTZU = 64,
    AARCH64_FCVTMS = 65,
    AARCH64_FCVTMU = 66,
    AARCH64_SCVTF  = 67,
    AARCH64_UCVTF  = 68,
    AARCH64_LD1    = 69,
    AARCH64_LD2    = 70,
    AARCH64_LD4    = 71,
    AARCH64_ST1    = 72,
    AARCH64_ST4    = 73,
    AARCH64_EXT    = 74,
    AARCH64_DUP    = 75,
    AARCH64_UMOV   = 76,
    AARCH64_INS    = 77,
    AARCH64_SSHLL  = 78,
    AARCH64_SSHLL2 = 79,
    AARCH64_USHLL  = 80,
    AARCH64_USHLL2 = 81,
    AARCH64_XTN    = 82,
    AARCH64_XTN2   = 83,
    AARCH64_B      = 84,
    AARCH64_B_NE   = 85, //TODO(ch): implement JCC operation instead of this endless variations.
    AARCH64_B_EQ   = 86,
    AARCH64_B_LT   = 87,
    AARCH64_B_LE   = 88,
    AARCH64_B_LS   = 89,
    AARCH64_B_GT   = 90,
    AARCH64_B_HI   = 91,
    AARCH64_B_GE   = 92,
    AARCH64_BLR    = 93,
    AARCH64_RET    = 94
};

enum {
    CP_AARCH64_BIG_IMMEDIATES = CP_ARCH_SPECIFIC,
    CP_AARCH64_SNIPPETS,
};


class Aarch64Backend : public Backend
{
public:
    Aarch64Backend();
    virtual size_t reusingPreferences(const Syntop& a_op, const std::set<size_t>& undefinedArgNums) const override final;
    virtual size_t spillSpaceNeeded(const Syntop& a_op, int basketNum) const override final;
    virtual std::set<size_t> getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask = BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) const override final;
    virtual void getStackParameterLayout(const Syntfunc& a_func, const std::vector<size_t> (&regParsOverride)[RB_AMOUNT], std::map<RegIdx, size_t> (&parLayout)[RB_AMOUNT]) const override final;
    virtual size_t stackGrowthAlignment(size_t stackGrowth) const override final;
    virtual void writeCallerPrologue(Syntfunc& prog, int stackGrowth) const override final;
    virtual void writeCallerEpilogue(Syntfunc& prog, int stackGrowth) const override final;
    virtual Arg getSParg() const override final;
    virtual std::unordered_map<int, std::string> getOpStrings() const override final;
    virtual Printer::ColPrinter colHexPrinter(const Syntfunc& toP) const override final;
    virtual Printer::ArgPrinter argPrinter(const Syntfunc& toP) const override final;
    virtual void switchOnSpillStressMode() override final;
};

};
#endif //__LOOPS_ARCH == __LOOPS_AARCH64
#endif //__LOOPS_BACKEND_AARCH64_HPP__
