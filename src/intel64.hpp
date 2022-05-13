/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
#ifndef __LOOPS_INTEL64_HPP__
#define __LOOPS_INTEL64_HPP__
#if defined(_WIN32) //TODO(ch): It must be about target processor, not operational system
#include "loops/loops.hpp"
#include "backend.hpp"

namespace loops
{
    enum {
        INTEL64_MOVSXD = 0,
        INTEL64_MOV,
        INTEL64_ADD,
        INTEL64_SUB,
        INTEL64_IMUL,
        INTEL64_IDIV,
        INTEL64_NEG,
        INTEL64_CQO,
        INTEL64_CMP,
        INTEL64_JMP,
        INTEL64_JNE,
        INTEL64_JE,
        INTEL64_JL,
        INTEL64_JLE,
        INTEL64_JG,
        INTEL64_JGE,
        INTEL64_RET
    };

    class Intel64Backend : public Backend
    {
    public:
        Intel64Backend(uint64_t flags);
        virtual std::set<size_t> filterStackPlaceable(const Syntop& a_op, const std::set<size_t>& toFilter) const override final;
        virtual size_t reusingPreferences(const Syntop& a_op, const std::set<size_t>& undefinedArgNums) const override final;
        virtual size_t spillSpaceNeeded(const Syntop& a_op) const override final;
        virtual std::set<size_t> getUsedRegistersIdxs(const Syntop& a_op, uint64_t flagmask = Binatr::Detail::D_INPUT | Binatr::Detail::D_OUTPUT) const override final;
        virtual Syntfunc bytecode2Target(const Syntfunc& a_bcfunc) const override final;
        virtual size_t stackGrowthAlignment(size_t stackGrowth) const override final;
        virtual size_t stackParamOffset(size_t a_nettoSpills, size_t a_snippetCausedSpills) const override final;
        virtual Arg getSParg(Func* funcimpl) const override final;
        virtual std::unordered_map<int, std::string> getOpStrings() const override final;
        virtual Printer::ColPrinter colHexPrinter(const Syntfunc& toP) const override final;
        virtual Printer::ArgPrinter argPrinter(const Syntfunc& toP) const override final;
    private:
        struct LabelRefInfo //TODO(ch): we have this implementation in both: in aarhc64 and in intel64.
        {
            size_t opnum;
            size_t argnum;
            size_t opoffset;
            LabelRefInfo() : opnum(0), argnum(0), opoffset(0) {}
            LabelRefInfo(size_t a_opnum, size_t a_argnum, size_t a_opoffset) : opnum(a_opnum), argnum(a_argnum), opoffset(a_opoffset) {}
        };

        mutable IRegInternal m_retReg;
        mutable std::unordered_map<size_t, size_t> m_labelMap;
        mutable std::unordered_map<size_t, std::vector<LabelRefInfo> > m_labelRefMap; // label

        virtual bool handleBytecodeOp(const Syntop& a_btop, Syntfunc& a_formingtarget) const override final;
    };

};

#endif
#endif //__LOOPS_INTEL64_HPP__
