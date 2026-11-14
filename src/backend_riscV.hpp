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
        RISCV_LB              =   0,
        RISCV_LBU             =   1,
        RISCV_LH              =   2,
        RISCV_LHU             =   3,
        RISCV_LW              =   4,
        RISCV_LWU             =   5,
        RISCV_LD              =   6,
        RISCV_SB              =   7,
        RISCV_SH              =   8,
        RISCV_SW              =   9,
        RISCV_SD              =  10,
        RISCV_MV              =  11,
        RISCV_LUI             =  12,
        RISCV_ADD             =  13,
        RISCV_SUB             =  14,
        RISCV_ADDI            =  15,
        RISCV_MUL             =  16,
        RISCV_DIV             =  17,
        RISCV_REM             =  18,
        RISCV_NEG             =  19,
        RISCV_SLL             =  20,
        RISCV_SLLI            =  21,
        RISCV_SRL             =  22,
        RISCV_SRLI            =  23,
        RISCV_SRA             =  24,
        RISCV_SRAI            =  25,
        RISCV_XOR             =  26,
        RISCV_XORI            =  27,
        RISCV_OR              =  28,
        RISCV_ORI             =  29,
        RISCV_AND             =  30,
        RISCV_ANDI            =  31,
        RISCV_NOT             =  32,
        RISCV_SLT             =  33,
        RISCV_SLTU            =  34,
        RISCV_SEQZ            =  35,
        RISCV_SNEZ            =  36,
        RISCV_BEQ             =  37,
        RISCV_BNE             =  38,
        RISCV_BLT             =  39,
        RISCV_BGE             =  40,
        RISCV_BLTU            =  41,
        RISCV_BGEU            =  42,
        RISCV_J               =  43,
        RISCV_JALR            =  44,
        RISCV_LABEL           =  45,
        RISCV_RET             =  46,
        RISCV_VSETVLI         =  47,
        RISCV_VLE8            =  48,
        RISCV_VLE16           =  49,
        RISCV_VLE32           =  50,
        RISCV_VLE64           =  51,
        RISCV_VSE8            =  52,
        RISCV_VSE16           =  53,
        RISCV_VSE32           =  54,
        RISCV_VSE64           =  55,
        RISCV_VL1RE8          =  56,
        RISCV_VS1R            =  57,
        RISCV_VLSEG2E8        =  58,
        RISCV_VLSEG2E16       =  59,
        RISCV_VLSEG2E32       =  60,
        RISCV_VLSEG2E64       =  61,
        RISCV_VMV_V_V         =  62,
        RISCV_VMV_V_X         =  63,
        RISCV_VMV_V_I         =  64,
        RISCV_VMV1R           =  65,
        RISCV_VRGATHER_VI     =  66,
        RISCV_VRGATHER_VX     =  67,
        RISCV_VSLIDEUP_VI     =  68,
        RISCV_VSLIDEUP_VX     =  69,
        RISCV_VSLIDEDOWN_VI   =  70,
        RISCV_VSLIDEDOWN_VX   =  71,
        RISCV_VMV_X_S         =  72,
        RISCV_VMV_S_X         =  73,
        RISCV_VMERGE_VVM      =  74,
        RISCV_VMERGE_VXM      =  75,
        RISCV_VMERGE_VIM      =  76,
        RISCV_VMSNE_VI        =  77,
        RISCV_VADD_VV         =  78,
        RISCV_VSUB_VV         =  79,
        RISCV_VMUL_VV         =  80,
        RISCV_VMIN_VV         =  81,
        RISCV_VMINU_VV        =  82,
        RISCV_VMAX_VV         =  83,
        RISCV_VMAXU_VV        =  84,
        RISCV_VSLL_VV         =  85,
        RISCV_VSLL_VI         =  86,
        RISCV_VSLL_VX         =  87,
        RISCV_VSRL_VV         =  88,
        RISCV_VSRL_VI         =  89,
        RISCV_VSRL_VX         =  90,
        RISCV_VSRA_VV         =  91,
        RISCV_VSRA_VI         =  92,
        RISCV_VSRA_VX         =  93,
        RISCV_VAND_VV         =  94,
        RISCV_VOR_VV          =  95,
        RISCV_VXOR_VV         =  96,
        RISCV_VXOR_VI         =  97,
        RISCV_VRSUB_VX        =  98,
        RISCV_VFADD_VV        =  99,
        RISCV_VFSUB_VV        = 100,
        RISCV_VFMUL_VV        = 101,
        RISCV_VFDIV_VV        = 102,
        RISCV_VFMACC_VV       = 103,
        RISCV_VFMIN_VV        = 104,
        RISCV_VFMAX_VV        = 105,
        RISCV_VFSGNJN_VV      = 106,
        RISCV_VMSEQ_VV        = 107,
        RISCV_VMSNE_VV        = 108,
        RISCV_VMSLT_VV        = 109,
        RISCV_VMSLTU_VV       = 110,
        RISCV_VMSLE_VV        = 111,
        RISCV_VMSLEU_VV       = 112,
        RISCV_VMFEQ_VV        = 113,
        RISCV_VMFNE_VV        = 114,
        RISCV_VMFLT_VV        = 115,
        RISCV_VMFLE_VV        = 116,
        RISCV_VFCVT_F_X_V     = 117,
        RISCV_VFCVT_X_F_V     = 118,
        RISCV_VFCVT_RTZ_X_F_V = 119,
        RISCV_VFNCVT_X_F_W    = 120,
        RISCV_VSEXT_VF2       = 121,
        RISCV_VZEXT_VF2       = 122,
        RISCV_VFWCVT_F_F_V    = 123,
        RISCV_VFNCVT_F_F_W    = 124,
        RISCV_VREDSUM_VS      = 125,
        RISCV_VFREDUSUM_VS    = 126,
        RISCV_VWREDSUM_VS     = 127,
        RISCV_VWREDSUMU_VS    = 128,
        RISCV_CSRRWI          = 129,
        RISCV_CSRRW           = 130,
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
