/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#ifndef __LOOPS_BACKEND_INTEL64_HPP__
#define __LOOPS_BACKEND_INTEL64_HPP__
#include "loops/defines.hpp"
#if __LOOPS_ARCH == __LOOPS_INTEL64
#include "loops/loops.hpp"
#include "backend.hpp"
#include "pipeline.hpp"

namespace loops
{
    enum {
        INTEL64_MOV    =  0,
        INTEL64_MOVSX  =  1,
        INTEL64_MOVSXD =  2,
        INTEL64_MOVZX  =  3,
        INTEL64_ADC    =  4,
        INTEL64_ADD    =  5,
        INTEL64_SUB    =  6,
        INTEL64_IMUL   =  7,
        INTEL64_IDIV   =  8,
        INTEL64_SHL    =  9,
        INTEL64_SHR    = 10,
        INTEL64_SAR    = 11,
        INTEL64_AND    = 12,
        INTEL64_OR     = 13,
        INTEL64_XOR    = 14,
        INTEL64_NOT    = 15,
        INTEL64_NEG    = 16,
        INTEL64_CQO    = 17,
        INTEL64_XCHG   = 18,
        INTEL64_CMP    = 19,
        INTEL64_CMOVE  = 20, //TODO(ch)[printer]: implement CMOVCC operation instead of this endless variations.
        INTEL64_CMOVNE = 21,
        INTEL64_CMOVL  = 22,
        INTEL64_CMOVG  = 23,
        INTEL64_CMOVLE = 24,
        INTEL64_CMOVGE = 25,
        INTEL64_CMOVS  = 26,
        INTEL64_CMOVNS = 27,
        INTEL64_SETE   = 28, //TODO(ch)[printer]: implement SETCC operation instead of this endless variations.
        INTEL64_SETNE  = 29,
        INTEL64_SETL   = 30,
        INTEL64_SETG   = 31,
        INTEL64_SETLE  = 32,
        INTEL64_SETGE  = 33,
        INTEL64_SETS   = 34,
        INTEL64_SETNS  = 35,
        INTEL64_JMP    = 36,
        INTEL64_JNE    = 37, //TODO(ch)[printer]: implement JCC operation instead of this endless variations.
        INTEL64_JE     = 38,
        INTEL64_JL     = 39,
        INTEL64_JLE    = 40,
        INTEL64_JG     = 41,
        INTEL64_JGE    = 42,
        INTEL64_CALL   = 43,
        INTEL64_RET    = 44 
    };

    enum {
        CP_INTEL64_BRA_SNIPPETS = CP_ARCH_SPECIFIC,
        CP_INTEL64_ARA_SNIPPETS
    };

    class Intel64Backend : public Backend
    {
    public:
        Intel64Backend();
        virtual std::set<size_t> filterStackPlaceable(const Syntop& a_op, const std::set<size_t>& toFilter) const override final;
        virtual size_t reusingPreferences(const Syntop& a_op, const std::set<size_t>& undefinedArgNums) const override final;
        virtual int spillSpaceNeeded(const Syntop& a_op, int basketNum) const override final;
        virtual std::set<size_t> getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask = BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) const override final;
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

};

#endif // __LOOPS_ARCH == __LOOPS_INTEL64
#endif //__LOOPS_BACKEND_INTEL64_HPP__
