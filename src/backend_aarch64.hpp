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
    AARCH64_ADDV   = 35,
    AARCH64_SADDLV = 36,
    AARCH64_UADDLV = 37,
    AARCH64_CMP    = 38,
    AARCH64_CSEL   = 39,
    AARCH64_CSET   = 40,
    AARCH64_CINC   = 41,
    AARCH64_CNEG   = 42,
    AARCH64_FADD   = 43,
    AARCH64_FSUB   = 44,
    AARCH64_FMUL   = 45,
    AARCH64_FDIV   = 46,
    AARCH64_FNEG   = 47,
    AARCH64_FMLA   = 48,
    AARCH64_SHL    = 49,
    AARCH64_USHL   = 50,
    AARCH64_SSHL   = 51,
    AARCH64_USHR   = 52,
    AARCH64_SSHR   = 53,
    AARCH64_MOVI   = 54,
    AARCH64_MVNI   = 55,
    AARCH64_CMHI   = 56,
    AARCH64_CMHS   = 57,
    AARCH64_CMEQ   = 58,
    AARCH64_CMGE   = 59,
    AARCH64_CMGT   = 60,
    AARCH64_CMLE   = 61,
    AARCH64_CMLT   = 62,
    AARCH64_FCMGT  = 63,
    AARCH64_FCMGE  = 64,
    AARCH64_FCMEQ  = 65,
    AARCH64_FCVTZS = 66,
    AARCH64_FCVTZU = 67,
    AARCH64_FCVTMS = 68,
    AARCH64_FCVTMU = 69,
    AARCH64_SCVTF  = 70,
    AARCH64_UCVTF  = 71,
    AARCH64_LD1    = 72,
    AARCH64_LD2    = 73,
    AARCH64_LD4    = 74,
    AARCH64_ST1    = 75,
    AARCH64_ST4    = 76,
    AARCH64_EXT    = 77,
    AARCH64_DUP    = 78,
    AARCH64_UMOV   = 79,
    AARCH64_INS    = 80,
    AARCH64_SSHLL  = 81,
    AARCH64_SSHLL2 = 82,
    AARCH64_USHLL  = 83,
    AARCH64_USHLL2 = 84,
    AARCH64_XTN    = 85,
    AARCH64_XTN2   = 86,
    AARCH64_CNT    = 87,
    AARCH64_B      = 88,
    AARCH64_B_NE   = 89, //TODO(ch): implement JCC operation instead of this endless variations.
    AARCH64_B_EQ   = 90,
    AARCH64_B_LT   = 91,
    AARCH64_B_LE   = 92,
    AARCH64_B_LS   = 93,
    AARCH64_B_GT   = 94,
    AARCH64_B_HI   = 95,
    AARCH64_B_GE   = 96,
    AARCH64_BLR    = 97,
    AARCH64_RET    = 98
};

enum {
    CP_AARCH64_BIG_IMMEDIATES = CP_ARCH_SPECIFIC,
    CP_AARCH64_SNIPPETS,
};


class Aarch64Backend : public Backend
{
public:
    Aarch64Backend();
    virtual ~Aarch64Backend() override;
    virtual int reusingPreferences(const Syntop& a_op, const std::set<int>& undefinedArgNums) const override final;
    virtual int spillSpaceNeeded(const Syntop& a_op, int basketNum) const override final;
    virtual std::set<int> getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask = BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) const override final;
    virtual void getStackParameterLayout(const Syntfunc& a_func, const std::vector<int> (&regParsOverride)[RB_AMOUNT], std::map<RegIdx, int> (&parLayout)[RB_AMOUNT]) const override final;
    virtual int stackGrowthAlignment(int stackGrowth) const override final;
    virtual void writeCallerPrologue(Syntfunc& prog, int stackGrowth) const override final;
    virtual void writeCallerEpilogue(Syntfunc& prog, int stackGrowth) const override final;
    virtual Arg getSParg() const override final;
    virtual std::unordered_map<int, std::string> getOpStrings() const override final;
    virtual Printer::ColPrinter colHexPrinter(const Syntfunc& toP) const override final;
    virtual Printer::ArgPrinter argPrinter(const Syntfunc& toP) const override final;
    virtual void switchOnSpillStressMode() override final;
};

}
#endif //__LOOPS_ARCH == __LOOPS_AARCH64
#endif //__LOOPS_BACKEND_AARCH64_HPP__
