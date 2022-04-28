/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
#ifndef __LOOPS_INTEL64_HPP__
#define __LOOPS_INTEL64_HPP__

#include "loops/loops.hpp"
#include "backend.hpp"

namespace loops
{
    enum {
        INTEL64_MOVSXD = 0,
        INTEL64_MOV,
        INTEL64_ADD,
        INTEL64_SUB,
        //INTEL64_MUL,
        //INTEL64_SDIV,
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
        Intel64Backend();
        virtual Syntfunc bytecode2Target(const Syntfunc& a_bcfunc) const override final;
        virtual void writePrologue(const Syntfunc& a_srcFunc, std::vector<Syntop>& a_canvas, size_t a_regSpilled, const std::set<IRegInternal>& a_calleeSaved) const override final;
        virtual void writeEpilogue(const Syntfunc& a_srcFunc, std::vector<Syntop>& a_canvas, size_t a_regSpilled, const std::set<IRegInternal>& a_calleeSaved) const override final;
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


#endif //__LOOPS_INTEL64_HPP__
