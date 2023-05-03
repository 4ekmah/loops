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
        INTEL64_MOV = 0,
        INTEL64_MOVSX,
        INTEL64_MOVSXD,
        INTEL64_MOVZX,
        INTEL64_ADC,
        INTEL64_ADD,
        INTEL64_SUB,
        INTEL64_IMUL,
        INTEL64_IDIV,
        INTEL64_SHL,
        INTEL64_SHR,
        INTEL64_SAR,
        INTEL64_AND,
        INTEL64_OR,
        INTEL64_XOR,
        INTEL64_NOT,
        INTEL64_NEG,
        INTEL64_CQO,
        INTEL64_XCHG,
        INTEL64_CMP,
        INTEL64_CMOVE,
        INTEL64_CMOVNE,
        INTEL64_CMOVL,
        INTEL64_CMOVG,
        INTEL64_CMOVLE,
        INTEL64_CMOVGE,
        INTEL64_CMOVS,
        INTEL64_CMOVNS,
        INTEL64_JMP,
        INTEL64_JNE,//TODO(ch): implement JCC operation instead of this endless variations.
        INTEL64_JE,
        INTEL64_JL,
        INTEL64_JLE,
        INTEL64_JG,
        INTEL64_JGE,
        INTEL64_RET
    };

    enum {
        CP_INTEL64_SNIPPETS = CP_ARCH_SPECIFIC
    };

    class Intel64Backend : public Backend
    {
    public:
        Intel64Backend();
        virtual std::set<size_t> filterStackPlaceable(const Syntop& a_op, const std::set<size_t>& toFilter) const override final;
        virtual size_t reusingPreferences(const Syntop& a_op, const std::set<size_t>& undefinedArgNums) const override final;
        virtual size_t spillSpaceNeeded(const Syntop& a_op, int basketNum) const override final;
        virtual std::set<size_t> getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask = BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) const override final;
        virtual void getStackParameterLayout(const Syntfunc& a_func, const std::vector<size_t> (&regParsOverride)[RB_AMOUNT], std::map<RegIdx, size_t> (&parLayout)[RB_AMOUNT]) const override final;
        virtual size_t stackGrowthAlignment(size_t stackGrowth) const override final;
        virtual Arg getSParg() const override final;
        virtual std::unordered_map<int, std::string> getOpStrings() const override final;
        virtual Printer::ColPrinter colHexPrinter(const Syntfunc& toP) const override final;
        virtual Printer::ArgPrinter argPrinter(const Syntfunc& toP) const override final;
        virtual void switchOnSpillStressMode() override final;
    };

};

#endif // __LOOPS_ARCH == __LOOPS_INTEL64
#endif //__LOOPS_BACKEND_INTEL64_HPP__
