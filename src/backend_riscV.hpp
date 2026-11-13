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
        RISCV_JALR     = 44,
        RISCV_LABEL    = 45,
        RISCV_RET      = 46,
        RISCV_VSETVLI  = 47,
        RISCV_VSETIVLI = 48,
        RISCV_VSETVL   = 49,
        RISCV_VLE8     = 50,
        RISCV_VLE16    = 51,
        RISCV_VLE32    = 52,
        RISCV_VLE64    = 53,
        RISCV_VSE8     = 54,
        RISCV_VSE16    = 55,
        RISCV_VSE32    = 56,
        RISCV_VSE64    = 57,
        RISCV_VL1RE8   = 58,
        RISCV_VS1R     = 59,
        RISCV_VLSEG2E8  = 60,
        RISCV_VLSEG2E16 = 61,
        RISCV_VLSEG2E32 = 62,
        RISCV_VLSEG2E64 = 63,
        RISCV_VMV_V_V   = 64,
        RISCV_VMV_V_X   = 65,
        RISCV_VMV_V_I   = 66,
        RISCV_VMV1R     = 67,
        RISCV_VRGATHER_VI = 68,
        RISCV_VRGATHER_VX = 69,
        RISCV_VSLIDEUP_VI = 70,
        RISCV_VSLIDEUP_VX = 71,
        RISCV_VSLIDEDOWN_VI = 72,
        RISCV_VSLIDEDOWN_VX = 73,
        RISCV_VMV_X_S   = 74,
        RISCV_VMV_S_X   = 75,
        RISCV_VMERGE_VVM = 76,
        RISCV_VMERGE_VXM = 77,
        RISCV_VMERGE_VIM = 78,
        RISCV_VMSNE_VI  = 79,
        RISCV_VADD_VV   = 80,
        RISCV_VSUB_VV   = 81,
        RISCV_VMUL_VV   = 82,
        RISCV_VMIN_VV   = 83,
        RISCV_VMINU_VV  = 84,
        RISCV_VMAX_VV   = 85,
        RISCV_VMAXU_VV  = 86,
        RISCV_VSLL_VV   = 87,
        RISCV_VSLL_VI   = 88,
        RISCV_VSLL_VX   = 89,
        RISCV_VSRL_VV   = 90,
        RISCV_VSRL_VI   = 91,
        RISCV_VSRL_VX   = 92,
        RISCV_VSRA_VV   = 93,
        RISCV_VSRA_VI   = 94,
        RISCV_VSRA_VX   = 95,
        RISCV_VAND_VV   = 96,
        RISCV_VOR_VV    = 97,
        RISCV_VXOR_VV   = 98,
        RISCV_VXOR_VI   = 99,
        RISCV_VRSUB_VX  = 100,
        RISCV_VFADD_VV  = 101,
        RISCV_VFSUB_VV  = 102,
        RISCV_VFMUL_VV  = 103,
        RISCV_VFDIV_VV  = 104,
        RISCV_VFMACC_VV = 105,
        RISCV_VFMIN_VV  = 106,
        RISCV_VFMAX_VV  = 107,
        RISCV_VFSGNJN_VV = 108,
        RISCV_VMSEQ_VV  = 109,
        RISCV_VMSNE_VV  = 110,
        RISCV_VMSLT_VV  = 111,
        RISCV_VMSLTU_VV = 112,
        RISCV_VMSLE_VV  = 113,
        RISCV_VMSLEU_VV = 114,
        RISCV_VMFEQ_VV  = 115,
        RISCV_VMFNE_VV  = 116,
        RISCV_VMFLT_VV  = 117,
        RISCV_VMFLE_VV  = 118,
        RISCV_VFCVT_F_X_V = 119,
        RISCV_VFCVT_X_F_V = 120,
        RISCV_VFCVT_RTZ_X_F_V = 121,
        RISCV_VFNCVT_X_F_W = 122,
        RISCV_VSEXT_VF2 = 123,
        RISCV_VZEXT_VF2 = 124,
        RISCV_VFWCVT_F_F_V = 125,
        RISCV_VFNCVT_F_F_W = 126,
        RISCV_VREDSUM_VS = 127,
        RISCV_VFREDUSUM_VS = 128,
        RISCV_VWREDSUM_VS = 129,
        RISCV_VWREDSUMU_VS = 130,
        RISCV_VFREDOSUM_VS = 131,
        RISCV_CSRRWI    = 132,
        RISCV_CSRRW     = 133,
    };

    class RiscVBackend : public Backend
    {
    public:
        RiscVBackend();
        virtual ~RiscVBackend() override;
        virtual int spillSpaceNeeded(const Syntop& a_op, int basketNum) const override final;
        virtual std::set<int> getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask = AF_INPUT | AF_OUTPUT) const override final;
        virtual std::array<std::map<RegIdx, int>, RB_AMOUNT> getStackParameterLayout(const Syntfunc& a_func, const std::array<std::vector<int>, RB_AMOUNT>& regParsOverride) const;
        virtual int stackGrowthAlignment(int stackGrowth) const override final;
        virtual void writeCallerPrologue(Syntfunc& prog, int stackGrowth) const override final;
        virtual void writeCallerEpilogue(Syntfunc& prog, int stackGrowth) const override final;
        virtual Arg getSParg() const override final;
        virtual column_printer_ptr get_opname_printer() const override final;
        virtual column_printer_ptr get_opargs_printer() const override final;
        virtual column_printer_ptr get_hex_printer() const override final;
    };
}

#endif // __LOOPS_ARCH == __LOOPS_RISCV
#endif //__LOOPS_BACKEND_RISCV_HPP__
