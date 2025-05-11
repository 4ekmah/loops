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

int backend_intel64_h_initialize();
void backend_intel64_h_deinitialize();

namespace loops
{
    enum {
        INTEL64_MOV         =   0,
        INTEL64_MOVSX       =   1,
        INTEL64_MOVSXD      =   2,
        INTEL64_MOVZX       =   3,
        INTEL64_ADC         =   4,
        INTEL64_ADD         =   5,
        INTEL64_SUB         =   6,
        INTEL64_IMUL        =   7,
        INTEL64_IDIV        =   8,
        INTEL64_SHL         =   9,
        INTEL64_SHR         =  10,
        INTEL64_SAR         =  11,
        INTEL64_AND         =  12,
        INTEL64_OR          =  13,
        INTEL64_XOR         =  14,
        INTEL64_NOT         =  15,
        INTEL64_NEG         =  16,
        INTEL64_CQO         =  17,
        INTEL64_XCHG        =  18,
        INTEL64_CMP         =  19,
        INTEL64_CMOVE       =  20, //TODO(ch)[printer]: implement CMOVCC operation instead of this endless variations.
        INTEL64_CMOVNE      =  21,
        INTEL64_CMOVL       =  22,
        INTEL64_CMOVG       =  23,
        INTEL64_CMOVLE      =  24,
        INTEL64_CMOVGE      =  25,
        INTEL64_CMOVS       =  26,
        INTEL64_CMOVNS      =  27,
        INTEL64_SETE        =  28, //TODO(ch)[printer]: implement SETCC operation instead of this endless variations.
        INTEL64_SETNE       =  29,
        INTEL64_SETL        =  30,
        INTEL64_SETG        =  31,
        INTEL64_SETLE       =  32,
        INTEL64_SETGE       =  33,
        INTEL64_SETS        =  34,
        INTEL64_SETNS       =  35,
        INTEL64_VMOVDQU     =  36, // DUBUG: VMOVDQA is also needed, at least for stack!
        INTEL64_VPADDB      =  37,
        INTEL64_VPADDW      =  38,
        INTEL64_VPADDD      =  39,
        INTEL64_VPADDQ      =  40,
        INTEL64_VADDPS      =  41,
        INTEL64_VADDPD      =  42,
        INTEL64_VPSUBB      =  43,
        INTEL64_VPSUBW      =  44,
        INTEL64_VPSUBD      =  45,
        INTEL64_VPSUBQ      =  46,
        INTEL64_VSUBPS      =  47,
        INTEL64_VSUBPD      =  48,
        INTEL64_VPMULLW     =  49,
        INTEL64_VPMULLD     =  50,
        INTEL64_VMULPS      =  51,
        INTEL64_VMULPD      =  52,
        INTEL64_VFMADD231PS =  53,
        INTEL64_VFMADD231PD =  54,
        INTEL64_VPMINUB     =  55,
        INTEL64_VPMINSB     =  56,
        INTEL64_VPMINUW     =  57,
        INTEL64_VPMINSW     =  58,
        INTEL64_VPMINUD     =  59,
        INTEL64_VPMINSD     =  60,
        INTEL64_VMINPS      =  61,
        INTEL64_VMINPD      =  62,
        INTEL64_VPMAXUB     =  63,
        INTEL64_VPMAXSB     =  64,
        INTEL64_VPMAXUW     =  65,
        INTEL64_VPMAXSW     =  66,
        INTEL64_VPMAXUD     =  67,
        INTEL64_VPMAXSD     =  68,
        INTEL64_VMAXPS      =  69,
        INTEL64_VMAXPD      =  70,
        INTEL64_VPCMPEQB    =  71,
        INTEL64_VPCMPEQW    =  72,
        INTEL64_VPCMPEQD    =  73,
        INTEL64_VPCMPEQQ    =  74,
        INTEL64_VPCMPGTB    =  75,
        INTEL64_VPCMPGTW    =  76,
        INTEL64_VPCMPGTD    =  77,
        INTEL64_VPCMPGTQ    =  78,
        INTEL64_VCMPEQPS    =  79,
        INTEL64_VCMPNEQPS   =  80,
        INTEL64_VCMPLTPS    =  81,
        INTEL64_VCMPLEPS    =  82,
        INTEL64_VCMPEQPD    =  83,
        INTEL64_VCMPNEQPD   =  84,
        INTEL64_VCMPLTPD    =  85,
        INTEL64_VCMPLEPD    =  86,
        INTEL64_VPAND       =  87,
        INTEL64_VPOR        =  88,
        INTEL64_VPXOR       =  89,
        INTEL64_VPSLLW      =  90,
        INTEL64_VPSLLD      =  91,
        INTEL64_VPSLLQ      =  92,
        INTEL64_VPSLLVD     =  93,
        INTEL64_VPSLLVQ     =  94,
        INTEL64_VPSRAW      =  95,
        INTEL64_VPSRAD      =  96,
        INTEL64_VPSRAVD     =  97,
        INTEL64_VPSRLW      =  98,
        INTEL64_VPSRLD      =  99,
        INTEL64_VPSRLQ      = 100,
        INTEL64_VPSRLVD     = 101,
        INTEL64_VPSRLVQ     = 102,
        INTEL64_VROUNDPS    = 103,
        INTEL64_VROUNDPD    = 104,
        INTEL64_VCVTPS2DQ   = 105,
        INTEL64_VCVTPD2DQ   = 106,
        INTEL64_VCVTDQ2PS   = 107,
        INTEL64_VPMOVSXDQ   = 108,
        INTEL64_JMP         = 109,
        INTEL64_JNE         = 110, //TODO(ch)[printer]: implement JCC operation instead of this endless variations.
        INTEL64_JE          = 111,
        INTEL64_JL          = 112,
        INTEL64_JLE         = 113,
        INTEL64_JG          = 114,
        INTEL64_JGE         = 115,
        INTEL64_CALL        = 116,
        INTEL64_LABEL       = 117,
        INTEL64_RET         = 118
    };

    class Intel64Backend : public Backend
    {
    public:
        Intel64Backend();
        virtual ~Intel64Backend() override;
        virtual std::set<int> filterStackPlaceable(const Syntop& a_op, const std::set<int>& toFilter) const override final;
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

#endif // __LOOPS_ARCH == __LOOPS_INTEL64
#endif //__LOOPS_BACKEND_INTEL64_HPP__
