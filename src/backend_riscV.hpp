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
    enum
    {
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
        RISCV_LUI      = 12, 
        RISCV_ADD      = 13,
        RISCV_SUB      = 14,
        RISCV_ADDI     = 15,
        RISCV_MUL      = 16,
        RISCV_DIV      = 17,
        RISCV_REM      = 18,
        RISCV_NEG      = 19,
        RISCV_SLL      = 20,
        RISCV_SLLI     = 21,
        RISCV_SRL      = 22,
        RISCV_SRLI     = 23,
        RISCV_SRA      = 24,
        RISCV_SRAI     = 25,
        RISCV_XOR      = 26, 
        RISCV_XORI     = 27, 
        RISCV_OR       = 28, 
        RISCV_ORI      = 29, 
        RISCV_AND      = 30, 
        RISCV_ANDI     = 31, 
        RISCV_NOT      = 32,
        RISCV_SLT      = 33,
        RISCV_SLTU     = 34,
        RISCV_SEQZ     = 35,
        RISCV_SNEZ     = 36,
        RISCV_BEQ      = 37,
        RISCV_BNE      = 38,
        RISCV_BLT      = 39,
        RISCV_BGE      = 40,
        RISCV_BLTU     = 41,
        RISCV_BGEU     = 42,
        RISCV_J        = 43,
        RISCV_LABEL    = 44,
        RISCV_RET      = 45,
    };

    class RiscVBackend : public Backend
    {
    public:
        RiscVBackend();
        virtual ~RiscVBackend() override;
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
