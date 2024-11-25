/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#ifndef __LOOPS_BACKEND_RISCV_HPP__
#define __LOOPS_BACKEND_RISCV_HPP__
#include "loops/defines.hpp"
#if __LOOPS_ARCH == __LOOPS_RISCV
#include "loops/loops.hpp"
#include "backend.hpp"
#include "pipeline.hpp"

int backend_riscv_h_initialize();
void backend_riscv_h_deinitialize();

namespace loops
{
    enum {
        RISCV_LB       =  0,
        RISCV_LBU      =  1,
        RISCV_LH       =  2,
        RISCV_LHU      =  3,
        RISCV_LW       =  4, 
        RISCV_LWU      =  5,
        RISCV_LD       =  6, 
        RISCV_SB       =  7, 
        RISCV_SH       =  8, 
        RISCV_SW       =  9, 
        RISCV_SD       = 10, 
        RISCV_MV       = 11, 
        RISCV_ADD      = 12,
        RISCV_SUB      = 13,
        RISCV_ADDI     = 14,
        RISCV_MUL      = 15,
        RISCV_DIV      = 16,
        RISCV_REM      = 17,
        RISCV_NEG      = 18,
        RISCV_SLL      = 19,
        RISCV_SLLI     = 20,
        RISCV_SRL      = 21,
        RISCV_SRLI     = 22,
        RISCV_SRA      = 23,
        RISCV_SRAI     = 24,
        RISCV_XOR      = 25, 
        RISCV_XORI     = 26, 
        RISCV_OR       = 27, 
        RISCV_ORI      = 28, 
        RISCV_AND      = 29, 
        RISCV_ANDI     = 30, 
        RISCV_NOT      = 31,
        RISCV_SLT      = 32,
        RISCV_SLTU     = 33,
        RISCV_SEQZ     = 34,
        RISCV_SNEZ     = 35,
        RISCV_BEQ      = 36,
        RISCV_BNE      = 37,
        RISCV_BLT      = 38,
        RISCV_BGE      = 39,
        RISCV_BLTU     = 40,
        RISCV_BGEU     = 41,
        RISCV_J        = 42,
        RISCV_LABEL    = 43,
        RISCV_RET      = 44,


        INTEL64_MOV    =  0+100,
        INTEL64_MOVSX  =  1+100,
        INTEL64_MOVSXD =  2+100,
        INTEL64_MOVZX  =  3+100,
        INTEL64_ADC    =  4+100,
        INTEL64_ADD    =  5+100,
        INTEL64_SUB    =  6+100,
        INTEL64_IMUL   =  7+100,
        INTEL64_IDIV   =  8+100,
        INTEL64_SHL    =  9+100,
        INTEL64_SHR    = 10+100,
        INTEL64_SAR    = 11+100,
        INTEL64_AND    = 12+100,
        INTEL64_OR     = 13+100,
        INTEL64_XOR    = 14+100,
        INTEL64_NOT    = 15+100,
        INTEL64_NEG    = 16+100,
        INTEL64_CQO    = 17+100,
        INTEL64_XCHG   = 18+100,
        INTEL64_CMP    = 19+100,
        INTEL64_CMOVE  = 20+100, //TODO(ch)[printer]: implement CMOVCC operation instead of this endless variations.
        INTEL64_CMOVNE = 21+100,
        INTEL64_CMOVL  = 22+100,
        INTEL64_CMOVG  = 23+100,
        INTEL64_CMOVLE = 24+100,
        INTEL64_CMOVGE = 25+100,
        INTEL64_CMOVS  = 26+100,
        INTEL64_CMOVNS = 27+100,
        INTEL64_SETE   = 28+100, //TODO(ch)[printer]: implement SETCC operation instead of this endless variations.
        INTEL64_SETNE  = 29+100,
        INTEL64_SETL   = 30+100,
        INTEL64_SETG   = 31+100,
        INTEL64_SETLE  = 32+100,
        INTEL64_SETGE  = 33+100,
        INTEL64_SETS   = 34+100,
        INTEL64_SETNS  = 35+100,
        INTEL64_JMP    = 36+100,
        INTEL64_JNE    = 37+100, //TODO(ch)[printer]: implement JCC operation instead of this endless variations.
        INTEL64_JE     = 38+100,
        INTEL64_JL     = 39+100,
        INTEL64_JLE    = 40+100,
        INTEL64_JG     = 41+100,
        INTEL64_JGE    = 42+100,
        INTEL64_CALL   = 43+100,
        INTEL64_LABEL  = 44+100, 
        INTEL64_RET    = 45+100 
    };

    class RiscVBackend : public Backend
    {
    public:
        RiscVBackend();
        virtual ~RiscVBackend() override;
        virtual int reusingPreferences(const Syntop& a_op, const std::set<int>& undefinedArgNums) const override final;
        virtual int spillSpaceNeeded(const Syntop& a_op, int basketNum) const override final;
        virtual std::set<int> getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask = AF_INPUT | AF_OUTPUT) const override final;
        virtual void getStackParameterLayout(const Syntfunc& a_func, const std::vector<int> (&regParsOverride)[RB_AMOUNT], std::map<RegIdx, int> (&parLayout)[RB_AMOUNT]) const override final;
        virtual int stackGrowthAlignment(int stackGrowth) const override final;
        virtual void writeCallerPrologue(Syntfunc& prog, int stackGrowth) const override final;
        virtual void writeCallerEpilogue(Syntfunc& prog, int stackGrowth) const override final;
        virtual Arg getSParg() const override final;
        virtual column_printer get_opname_printer() const override final;
        virtual column_printer get_opargs_printer() const override final;
        virtual column_printer get_hex_printer() const override final;
    };
}

#endif // __LOOPS_ARCH == __LOOPS_RISCV
#endif //__LOOPS_BACKEND_RISCV_HPP__
