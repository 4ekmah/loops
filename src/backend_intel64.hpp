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
        INTEL64_MOV          =   0,
        INTEL64_MOVSX        =   1,
        INTEL64_MOVSXD       =   2,
        INTEL64_MOVZX        =   3,
        INTEL64_ADC          =   4,
        INTEL64_ADD          =   5,
        INTEL64_SUB          =   6,
        INTEL64_IMUL         =   7,
        INTEL64_IDIV         =   8,
        INTEL64_SHL          =   9,
        INTEL64_SHR          =  10,
        INTEL64_SAR          =  11,
        INTEL64_AND          =  12,
        INTEL64_OR           =  13,
        INTEL64_XOR          =  14,
        INTEL64_NOT          =  15,
        INTEL64_NEG          =  16,
        INTEL64_CQO          =  17,
        INTEL64_XCHG         =  18,
        INTEL64_CMP          =  19,
        INTEL64_CMOVE        =  20, //TODO(ch)[printer]: implement CMOVCC operation instead of this endless variations.
        INTEL64_CMOVNE       =  21,
        INTEL64_CMOVL        =  22,
        INTEL64_CMOVG        =  23,
        INTEL64_CMOVLE       =  24,
        INTEL64_CMOVGE       =  25,
        INTEL64_CMOVS        =  26,
        INTEL64_CMOVNS       =  27,
        INTEL64_SETE         =  28, //TODO(ch)[printer]: implement SETCC operation instead of this endless variations.
        INTEL64_SETNE        =  29,
        INTEL64_SETL         =  30,
        INTEL64_SETG         =  31,
        INTEL64_SETLE        =  32,
        INTEL64_SETGE        =  33,
        INTEL64_SETS         =  34,
        INTEL64_SETNS        =  35,
        INTEL64_VMOVDQU      =  36, // DUBUG: VMOVDQA is also needed, at least for stack!
        INTEL64_VMOVUPS      =  37,
        INTEL64_VMOVUPD      =  38,
        INTEL64_VMOVD        =  39,
        INTEL64_VMOVQ        =  40,
        INTEL64_VPBROADCASTB =  41,
        INTEL64_VPBROADCASTW =  42,
        INTEL64_VPBROADCASTD =  43,
        INTEL64_VPBROADCASTQ =  44,
        INTEL64_VPADDB       =  45,
        INTEL64_VPADDW       =  46,
        INTEL64_VPADDD       =  47,
        INTEL64_VPADDQ       =  48,
        INTEL64_VADDPS       =  49,
        INTEL64_VADDPD       =  50,
        INTEL64_VPSUBB       =  51,
        INTEL64_VPSUBW       =  52,
        INTEL64_VPSUBD       =  53,
        INTEL64_VPSUBQ       =  54,
        INTEL64_VSUBPS       =  55,
        INTEL64_VSUBPD       =  56,
        INTEL64_VPMULLW      =  57,
        INTEL64_VPMULLD      =  58,
        INTEL64_VMULPS       =  59,
        INTEL64_VMULPD       =  60,
        INTEL64_VFMADD231PS  =  61,
        INTEL64_VFMADD231PD  =  62,
        INTEL64_VPMINUB      =  63,
        INTEL64_VPMINSB      =  64,
        INTEL64_VPMINUW      =  65,
        INTEL64_VPMINSW      =  66,
        INTEL64_VPMINUD      =  67,
        INTEL64_VPMINSD      =  68,
        INTEL64_VMINPS       =  69,
        INTEL64_VMINPD       =  70,
        INTEL64_VPMAXUB      =  71,
        INTEL64_VPMAXSB      =  72,
        INTEL64_VPMAXUW      =  73,
        INTEL64_VPMAXSW      =  74,
        INTEL64_VPMAXUD      =  75,
        INTEL64_VPMAXSD      =  76,
        INTEL64_VMAXPS       =  77,
        INTEL64_VMAXPD       =  78,
        INTEL64_VPCMPEQB     =  79,
        INTEL64_VPCMPEQW     =  80,
        INTEL64_VPCMPEQD     =  81,
        INTEL64_VPCMPEQQ     =  82,
        INTEL64_VPCMPGTB     =  83,
        INTEL64_VPCMPGTW     =  84,
        INTEL64_VPCMPGTD     =  85,
        INTEL64_VPCMPGTQ     =  86,
        INTEL64_VCMPEQPS     =  87,
        INTEL64_VCMPNEQPS    =  88,
        INTEL64_VCMPLTPS     =  89,
        INTEL64_VCMPLEPS     =  90,
        INTEL64_VCMPEQPD     =  91,
        INTEL64_VCMPNEQPD    =  92,
        INTEL64_VCMPLTPD     =  93,
        INTEL64_VCMPLEPD     =  94,
        INTEL64_VPBLENDVB    =  95,
        INTEL64_VBLENDVPS    =  96,
        INTEL64_VBLENDVPD    =  97,
        INTEL64_VPAND        =  98,
        INTEL64_VPOR         =  99,
        INTEL64_VPXOR        = 100,
        INTEL64_VPSLLW       = 101,
        INTEL64_VPSLLD       = 102,
        INTEL64_VPSLLQ       = 103,
        INTEL64_VPSLLVD      = 104,
        INTEL64_VPSLLVQ      = 105,
        INTEL64_VPSRAW       = 106,
        INTEL64_VPSRAD       = 107,
        INTEL64_VPSRAVD      = 108,
        INTEL64_VPSRLW       = 109,
        INTEL64_VPSRLD       = 110,
        INTEL64_VPSRLQ       = 111,
        INTEL64_VPSRLVD      = 112,
        INTEL64_VPSRLVQ      = 113,
        INTEL64_VROUNDPS     = 114,
        INTEL64_VROUNDPD     = 115,
        INTEL64_VCVTPS2DQ    = 116,
        INTEL64_VCVTPD2DQ    = 117,
        INTEL64_VCVTDQ2PS    = 118,
        INTEL64_VPMOVSXDQ    = 119,
        INTEL64_JMP          = 120,
        INTEL64_JNE          = 121, //TODO(ch)[printer]: implement JCC operation instead of this endless variations.
        INTEL64_JE           = 122,
        INTEL64_JL           = 123,
        INTEL64_JLE          = 124,
        INTEL64_JG           = 125,
        INTEL64_JGE          = 126,
        INTEL64_CALL         = 127,
        INTEL64_LABEL        = 128,
        INTEL64_RET          = 129
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
