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
        INTEL64_CMOVNE       =  20,
        INTEL64_CMOVE        =  21,
        INTEL64_CMOVL        =  22,
        INTEL64_CMOVG        =  23,
        INTEL64_CMOVGE       =  24,
        INTEL64_CMOVA        =  25,
        INTEL64_CMOVLE       =  26,
        INTEL64_CMOVBE       =  27,
        INTEL64_CMOVS        =  28,
        INTEL64_CMOVNS       =  29,
        INTEL64_SETNE        =  30,
        INTEL64_SETE         =  31,
        INTEL64_SETL         =  32,
        INTEL64_SETG         =  33,
        INTEL64_SETGE        =  34,
        INTEL64_SETA         =  35,
        INTEL64_SETLE        =  36,
        INTEL64_SETBE        =  37,
        INTEL64_SETS         =  38,
        INTEL64_SETNS        =  39,
        INTEL64_VMOVDQU      =  40, // TODO(ch): Would be nice to have VMOVDQA for spilling commands.
        INTEL64_VMOVUPS      =  41,
        INTEL64_VMOVUPD      =  42,
        INTEL64_VEXTRACTI128 =  43,
        INTEL64_VEXTRACTF128 =  44,
        INTEL64_VINSERTI128  =  45,
        INTEL64_VINSERTF128  =  46,
        INTEL64_VPERM2I128   =  47,
        INTEL64_VPEXTRB      =  48,
        INTEL64_VPEXTRW      =  49,
        INTEL64_VPEXTRD      =  50,
        INTEL64_VPEXTRQ      =  51,
        INTEL64_VPINSRB      =  52,
        INTEL64_VPINSRW      =  53,
        INTEL64_VPINSRD      =  54,
        INTEL64_VPINSRQ      =  55,
        INTEL64_VMOVD        =  56,
        INTEL64_VMOVQ        =  57,
        INTEL64_VPBROADCASTB =  58,
        INTEL64_VPBROADCASTW =  59,
        INTEL64_VPBROADCASTD =  60,
        INTEL64_VPBROADCASTQ =  61,
        INTEL64_VPADDB       =  62,
        INTEL64_VPADDW       =  63,
        INTEL64_VPADDD       =  64,
        INTEL64_VPADDQ       =  65,
        INTEL64_VADDPS       =  66,
        INTEL64_VADDPD       =  67,
        INTEL64_VPSUBB       =  68,
        INTEL64_VPSUBW       =  69,
        INTEL64_VPSUBD       =  70,
        INTEL64_VPSUBQ       =  71,
        INTEL64_VSUBPS       =  72,
        INTEL64_VSUBPD       =  73,
        INTEL64_VPMULLW      =  74,
        INTEL64_VPMULLD      =  75,
        INTEL64_VMULPS       =  76,
        INTEL64_VMULPD       =  77,
        INTEL64_VDIVPS       =  78,
        INTEL64_VDIVPD       =  79,
        INTEL64_VFMADD231PS  =  80,
        INTEL64_VFMADD231PD  =  81,
        INTEL64_VPMINUB      =  82,
        INTEL64_VPMINSB      =  83,
        INTEL64_VPMINUW      =  84,
        INTEL64_VPMINSW      =  85,
        INTEL64_VPMINUD      =  86,
        INTEL64_VPMINSD      =  87,
        INTEL64_VMINPS       =  88,
        INTEL64_VMINPD       =  89,
        INTEL64_VPMAXUB      =  90,
        INTEL64_VPMAXSB      =  91,
        INTEL64_VPMAXUW      =  92,
        INTEL64_VPMAXSW      =  93,
        INTEL64_VPMAXUD      =  94,
        INTEL64_VPMAXSD      =  95,
        INTEL64_VMAXPS       =  96,
        INTEL64_VMAXPD       =  97,
        INTEL64_VPCMPEQB     =  98,
        INTEL64_VPCMPEQW     =  99,
        INTEL64_VPCMPEQD     = 100,
        INTEL64_VPCMPEQQ     = 101,
        INTEL64_VPCMPGTB     = 102,
        INTEL64_VPCMPGTW     = 103,
        INTEL64_VPCMPGTD     = 104,
        INTEL64_VPCMPGTQ     = 105,
        INTEL64_VCMPEQPS     = 106,
        INTEL64_VCMPNEQPS    = 107,
        INTEL64_VCMPLTPS     = 108,
        INTEL64_VCMPLEPS     = 109,
        INTEL64_VCMPEQPD     = 110,
        INTEL64_VCMPNEQPD    = 111,
        INTEL64_VCMPLTPD     = 112,
        INTEL64_VCMPLEPD     = 113,
        INTEL64_VPBLENDVB    = 114,
        INTEL64_VBLENDVPS    = 115,
        INTEL64_VBLENDVPD    = 116,
        INTEL64_VPAND        = 117,
        INTEL64_VPOR         = 118,
        INTEL64_VPXOR        = 119,
        INTEL64_VPSLLW       = 120,
        INTEL64_VPSLLD       = 121,
        INTEL64_VPSLLQ       = 122,
        INTEL64_VPSLLVD      = 123,
        INTEL64_VPSLLVQ      = 124,
        INTEL64_VPSRAW       = 125,
        INTEL64_VPSRAD       = 126,
        INTEL64_VPSRAVD      = 127,
        INTEL64_VPSRLW       = 128,
        INTEL64_VPSRLD       = 129,
        INTEL64_VPSRLQ       = 130,
        INTEL64_VPSRLVD      = 131,
        INTEL64_VPSRLVQ      = 132,
        INTEL64_VROUNDPS     = 133,
        INTEL64_VROUNDPD     = 134,
        INTEL64_VCVTPS2DQ    = 135,
        INTEL64_VCVTPD2DQ    = 136,
        INTEL64_VCVTDQ2PS    = 137,
        INTEL64_VPMOVSXBW    = 138,
        INTEL64_VPMOVSXWD    = 139,
        INTEL64_VPMOVSXDQ    = 140,
        INTEL64_VPMOVZXBW    = 141,
        INTEL64_VPMOVZXWD    = 142,
        INTEL64_VPMOVZXDQ    = 143,
        INTEL64_VCVTPS2PD    = 144,
        INTEL64_VPALIGNR     = 145,
        INTEL64_VPSHUFD      = 146,
        INTEL64_VPSADBW      = 147,
        INTEL64_VPHADDD      = 148,
        INTEL64_VHADDPS      = 149,
        INTEL64_VADDSS       = 150,
        INTEL64_VHADDPD      = 151,
        INTEL64_VADDSD       = 152,
        INTEL64_JMP          = 153,
        INTEL64_JNE          = 154,
        INTEL64_JE           = 155,
        INTEL64_JL           = 156,
        INTEL64_JG           = 157,
        INTEL64_JGE          = 158,
        INTEL64_JA           = 159,
        INTEL64_JLE          = 160,
        INTEL64_JBE          = 161,
        INTEL64_CALL         = 162,
        INTEL64_LABEL        = 163,
        INTEL64_RET          = 164
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
