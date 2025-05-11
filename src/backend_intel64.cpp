/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "backend_intel64.hpp"
#if __LOOPS_ARCH == __LOOPS_INTEL64
#include "func_impl.hpp"
#include "collections.hpp"
#include <algorithm>
#include <iomanip>

enum 
{
    TYPE_SAME_AS_0 = 62,
    TYPE_SAME_AS_1 = 63,
};

LOOPS_HASHMAP_STATIC(int, loops_cstring) opstrings_[] = 
{
                  /*  |       enum_id             |string_id      |*/
    LOOPS_HASHMAP_ELEM(loops::INTEL64_MOV         , "mov"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_MOVSX       , "movsx"       ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_MOVSXD      , "movsxd"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_MOVZX       , "movzx"       ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_ADC         , "adc"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_ADD         , "add"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_SUB         , "sub"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_IMUL        , "imul"        ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_IDIV        , "idiv"        ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_SHL         , "shl"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_SHR         , "shr"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_SAR         , "sar"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_AND         , "and"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_OR          , "or"          ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_XOR         , "xor"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_NOT         , "not"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_NEG         , "neg"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_CQO         , "cqo"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_XCHG        , "xchg"        ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_CMP         , "cmp"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_CMOVE       , "cmove"       ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_CMOVNE      , "cmovne"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_CMOVL       , "cmovl"       ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_CMOVG       , "cmovg"       ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_CMOVLE      , "cmovle"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_CMOVGE      , "cmovge"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_CMOVS       , "cmovs"       ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_CMOVNS      , "cmovns"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_SETE        , "sete"        ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_SETNE       , "setne"       ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_SETL        , "setl"        ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_SETG        , "setg"        ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_SETLE       , "setle"       ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_SETGE       , "setge"       ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_SETS        , "sets"        ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_SETNS       , "setns"       ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VMOVDQU     , "vmovdqu"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPADDB      , "vpaddb"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPADDW      , "vpaddw"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPADDD      , "vpaddd"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPADDQ      , "vpaddq"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VADDPS      , "vaddps"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VADDPD      , "vaddpd"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSUBB      , "vpsubb"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSUBW      , "vpsubw"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSUBD      , "vpsubd"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSUBQ      , "vpsubq"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VSUBPS      , "vsubps"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VSUBPD      , "vsubpd"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMULLW     , "vpmullw"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMULLD     , "vpmulld"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VMULPS      , "vmulps"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VMULPD      , "vmulpd"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VFMADD231PS , "vfmadd231ps" ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VFMADD231PD , "vfmadd231pd" ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMINUB     , "vpminub"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMINSB     , "vpminsb"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMINUW     , "vpminuw"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMINSW     , "vpminsw"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMINUD     , "vpminud"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMINSD     , "vpminsd"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VMINPS      , "vminps"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VMINPD      , "vminpd"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMAXUB     , "vpmaxub"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMAXSB     , "vpmaxsb"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMAXUW     , "vpmaxuw"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMAXSW     , "vpmaxsw"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMAXUD     , "vpmaxud"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMAXSD     , "vpmaxsd"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VMAXPS      , "vmaxps"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VMAXPD      , "vmaxpd"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPCMPEQB    , "vpcmpeqb"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPCMPEQW    , "vpcmpeqw"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPCMPEQD    , "vpcmpeqd"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPCMPEQQ    , "vpcmpeqq"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPCMPGTB    , "vpcmpgtb"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPCMPGTW    , "vpcmpgtw"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPCMPGTD    , "vpcmpgtd"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPCMPGTQ    , "vpcmpgtq"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VCMPEQPS    , "vcmpeqps"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VCMPNEQPS   , "vcmpneqps"   ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VCMPLTPS    , "vcmpltps"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VCMPLEPS    , "vcmpleps"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VCMPEQPD    , "vcmpeqpd"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VCMPNEQPD   , "vcmpneqpd"   ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VCMPLTPD    , "vcmpltpd"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VCMPLEPD    , "vcmplepd"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPAND       , "vpand"       ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPOR        , "vpor"        ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPXOR       , "vpxor"       ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSLLW      , "vpsllw"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSLLD      , "vpslld"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSLLQ      , "vpsllq"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSLLVD     , "vpsllvd"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSLLVQ     , "vpsllvq"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSRAW      , "vpsraw"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSRAD      , "vpsrad"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSRAVD     , "vpsravd"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSRLW      , "vpsrlw"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSRLD      , "vpsrld"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSRLQ      , "vpsrlq"      ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSRLVD     , "vpsrlvd"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPSRLVQ     , "vpsrlvq"     ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VROUNDPS    , "vroundps"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VROUNDPD    , "vroundpd"    ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VCVTPS2DQ   , "vcvtps2dq"   ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VCVTPD2DQ   , "vcvtpd2dq"   ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VCVTDQ2PS   , "vcvtdq2ps"   ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_VPMOVSXDQ   , "vpmovsxdq"   ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_JMP         , "jmp"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_JNE         , "jne"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_JE          , "je"          ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_JL          , "jl"          ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_JLE         , "jle"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_JG          , "jg"          ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_JGE         , "jge"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_CALL        , "call"        ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_RET         , "ret"         ),
    LOOPS_HASHMAP_ELEM(loops::INTEL64_LABEL       , ""            ),
};

static LOOPS_HASHMAP(int, loops_cstring) opstrings = NULL;

int backend_intel64_h_initialize()
{
    LOOPS_CALL_THROW(loops_hashmap_construct_static(&opstrings, opstrings_, sizeof(opstrings_) / sizeof(opstrings_[0])));
    return LOOPS_ERR_SUCCESS;
}

void backend_intel64_h_deinitialize()
{
    loops_hashmap_destruct(opstrings);
}

namespace loops
{
    enum Intel64Reg
    {
        RAX =  0,
        RCX =  1,
        RDX =  2,
        RBX =  3,
        RSP =  4,
        RBP =  5,
        RSI =  6,
        RDI =  7,
        R8  =  8,
        R9  =  9,
        R10 = 10,
        R11 = 11,
        R12 = 12,
        R13 = 13,
        R14 = 14,
        R15 = 15
    }; 

    enum Intel64VReg
    {
        YMM0  =  0,
        YMM1  =  1,
        YMM2  =  2,
        YMM3  =  3,
        YMM4  =  4,
        YMM5  =  5,
        YMM6  =  6,
        YMM7  =  7,
        YMM8  =  8,
        YMM9  =  9,
        YMM10 = 10,
        YMM11 = 11,
        YMM12 = 12,
        YMM13 = 13,
        YMM14 = 14,
        YMM15 = 15,
    };    

    static inline BinTranslation::Token nBkb(int n, uint64_t bytes, int k, uint64_t bits)
    {
        uint64_t field = ((((uint64_t(1) << (n * 8)) - 1) & bytes) << k) | bits;
        return BinTranslation::Token(BinTranslation::Token::T_STATIC, field, n*8+k);
    }

    static inline uint64_t bm64(std::initializer_list<size_t> regNumbers)
    {
        return makeBitmask64(regNumbers);
    }

    static inline BinTranslation VEX_instuction(const Syntop& index, bool& scs, uint32_t pp_opcode, uint32_t m_opcode, uint64_t opcode)
    {
        using namespace BinTranslationConstruction;
        Assert(pp_opcode == 0 || pp_opcode == 0x66 || pp_opcode == 0xF2 || pp_opcode == 0xF3);
        Assert(m_opcode == 0x0F || m_opcode == 0x0F3A || m_opcode == 0x0F38);
        
        if(index.args_size != 2 && index.args_size != 3)
        {
            scs = false;
            return BinTranslation();
        }
        int nar = 0; //non-address register.
        int far = 1; // first address register.
        int sar = 2; // second address register.
        int arIOFlag = In; 
        int narIOFlag = Out; 
        if (index.args[0].flags & AF_ADDRESS)
        {
            far = 0;
            sar = 1;
            nar = 1 + (index.args_size == 3);
            arIOFlag = Out; 
            narIOFlag = In;
        }
        if(!(index.args[nar].tag == Arg::VREG && index.args[far].tag == Arg::IREG && (index.args_size == 2 || 
             index.args[sar].tag == Arg::IREG || (index.args[sar].tag == Arg::IIMMEDIATE && signed_fits(index.args[sar].value,32)))))
        {
            scs = false;
            return BinTranslation();
        }

        std::vector<BinTranslation::Token> tokens;
        const uint32_t R = ((index.args[nar].idx & 0b1000) == 0);
        const uint32_t X = ((index.args_size == 3 && index.args[sar].tag == Arg::IREG) ? ((index.args[sar].idx & 0b1000) == 0) : 1);
        const uint32_t B = ((index.args[far].idx & 0b1000) == 0);
        const uint32_t W = 0; // DUBUG: WIG is for W ignored, means W = 0 always. Move WIG to arguments?
        const uint32_t L = 1;
        const uint32_t m_mmmm = (m_opcode == 0x0F ? 0b01 : (m_opcode == 0x0F38 ? 0b10 : (/*m_opcode == 0x0F3A ?*/0b11)));
        // pp:opcode extension providing equivalent functionality of a SIMD prefix: {00: None | 01: 66 | 10: F3 | 11: F2}
        const uint32_t pp = pp_opcode == 0 ? 0 : (pp_opcode == 0x66 ? 0b01 : (pp_opcode == 0xF3 ? 0b10 : (/*pp_opcode == 0xF2 ?*/ 0b11 )));

        const uint32_t vvvv = 0b1111;
        uint32_t vex = 0;
        int vex_size;
        if(W == 0 && B == 1 && X == 1 && m_mmmm == 0b00001)
        {
            vex_size = 16;
            vex = (0xc5 << 8)|(R<<7)|(vvvv<<3)|(L<<2)|pp;
        }
        else
        {
            vex_size = 24;
            vex = (0xc4 << 16)|(R<<15)|(X<<14)|(B<<13)|(m_mmmm<<8)|(W<<7)|(vvvv<<3)|(L<<2)|pp;
        }

        //OPCODE construction: <prefix><leading bytes><other bytes>
        // R, X and B bits are complements of the REX prefix's R, X and B bits; these provide a fourth (high) bit for register index fields
        // (ModRM reg, SIB index, and ModRM r/m; SIB base; or opcode reg fields, respectively) allowing access to 16 instead of 8 registers.
        tokens.push_back(BTsta(vex, vex_size));
        tokens.push_back(BTsta(opcode, 8));
        {//ModRM+SIB
            if(index.args_size == 2)
            {
                if ((index.args[far].idx & 0b111) == 0b100)
                {
                    const uint32_t mod = 0;
                    tokens.push_back(BTsta(mod, 2));
                    tokens.push_back(BTreg(nar, 3, narIOFlag));
                    tokens.push_back(BTsta(0b10000100, 8));
                    tokens.push_back(BTreg(far, 3, arIOFlag | AF_ADDRESS));
                }
                else if ((index.args[far].idx & 0b111) == 0b101)
                {
                    const uint32_t mod = 1;
                    tokens.push_back(BTsta(mod, 2));
                    tokens.push_back(BTreg(nar, 3, narIOFlag));
                    tokens.push_back(BTreg(far, 3, arIOFlag | AF_ADDRESS));
                    tokens.push_back(BTsta(0, 8));
                }
                else 
                {
                    const uint32_t mod = 0;
                    tokens.push_back(BTsta(mod, 2));
                    tokens.push_back(BTreg(nar, 3, narIOFlag));
                    tokens.push_back(BTreg(far, 3, arIOFlag | AF_ADDRESS));
                }
            }
            else if(index.args_size == 3 && index.args[sar].tag == Arg::IREG)
            {
                const uint32_t mod = 0;
                tokens.push_back(BTsta(mod, 2));
                tokens.push_back(BTreg(nar, 3, narIOFlag));
                const uint32_t r_m = 0b100;
                const uint32_t scale = 0;
                tokens.push_back(BTsta(r_m, 3));
                tokens.push_back(BTsta(scale, 2));
                tokens.push_back(BTreg(sar, 3, arIOFlag | AF_ADDRESS)); //index
                tokens.push_back(BTreg(far, 3, arIOFlag | AF_ADDRESS)); //base
            }
            else if(index.args_size == 3 && index.args[sar].tag == Arg::IIMMEDIATE)
            {
                const uint32_t mod = 0b10;
                tokens.push_back(BTsta(mod, 2));
                tokens.push_back(BTreg(nar, 3, narIOFlag));
                if ((index.args[far].idx & 0b111) == 0b100)
                    tokens.push_back(BTsta(0b10000100, 8));
                tokens.push_back(BTreg(far, 3, arIOFlag | AF_ADDRESS));  //base
                tokens.push_back(BTimm(sar, 32, arIOFlag | AF_ADDRESS)); //index
            }
        }
        return BinTranslation(tokens);
    }

    //DUBUG: Is it possible to splice VEX_instuction_V and VEX_instuction?
    static inline BinTranslation VEX_instuction_V(const Syntop& index, bool& scs, uint32_t pp_opcode, uint32_t m_opcode, uint16_t W, uint64_t opcode, uint64_t mrm_opcode, uint64_t supportedTypesBitmask0 = 0, uint64_t supportedTypesBitmask1 = 0, uint64_t supportedTypesBitmask2 = 0, bool ib_present = false, uint64_t ib = 0)
    {
        using namespace BinTranslationConstruction;
        Assert(pp_opcode == 0 || pp_opcode == 0x66 || pp_opcode == 0xF2 || pp_opcode == 0xF3);
        Assert(m_opcode == 0x0F || m_opcode == 0x0F3A || m_opcode == 0x0F38);
        //Metatypes TYPE_SAME_AS_0 and TYPE_SAME_AS_1 cannot be variation, it have to be only type!
        Assert(((supportedTypesBitmask1 & (uint64_t(1) << TYPE_SAME_AS_0)) == 0) || (supportedTypesBitmask1 == (uint64_t(1) << TYPE_SAME_AS_0)));
        Assert(((supportedTypesBitmask2 & (uint64_t(1) << TYPE_SAME_AS_0)) == 0) || (supportedTypesBitmask2 == (uint64_t(1) << TYPE_SAME_AS_0)));
        Assert(((supportedTypesBitmask2 & (uint64_t(1) << TYPE_SAME_AS_1)) == 0) || (supportedTypesBitmask2 == (uint64_t(1) << TYPE_SAME_AS_1)));
        Assert(m_opcode == 0x0F || m_opcode == 0x0F3A || m_opcode == 0x0F38);
        if(supportedTypesBitmask1 == 0 && supportedTypesBitmask0 != 0)
            supportedTypesBitmask1 = supportedTypesBitmask0;
        if(supportedTypesBitmask2 == 0 && supportedTypesBitmask0 != 0)
            supportedTypesBitmask2 = supportedTypesBitmask0;
        if((supportedTypesBitmask1 == (uint64_t(1) << TYPE_SAME_AS_0)))
            supportedTypesBitmask1 = ((uint64_t(1))<<index.args[0].elemtype & supportedTypesBitmask0);
        if((supportedTypesBitmask2 == (uint64_t(1) << TYPE_SAME_AS_0)))
            supportedTypesBitmask2 = ((uint64_t(1))<<index.args[0].elemtype & supportedTypesBitmask0);
        if((supportedTypesBitmask2 == (uint64_t(1) << TYPE_SAME_AS_1)))
            supportedTypesBitmask2 = ((uint64_t(1))<<index.args[1].elemtype & supportedTypesBitmask1);
        if(!((index.args_size == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && (index.args[2].tag == Arg::VREG || index.args[2].tag == Arg::IIMMEDIATE) &&
             (supportedTypesBitmask0 == 0 || ((uint64_t(1))<<index.args[0].elemtype & supportedTypesBitmask0) != 0) &&
             (supportedTypesBitmask1 == 0 || ((uint64_t(1))<<index.args[1].elemtype & supportedTypesBitmask1) != 0) &&
             (index.args[2].tag == Arg::IIMMEDIATE || supportedTypesBitmask2 == 0 || ((uint64_t(1))<<index.args[2].elemtype & supportedTypesBitmask2) != 0) &&
             (index.args[2].tag == Arg::IIMMEDIATE || mrm_opcode == 0)) ||
             (index.args_size == 2 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG &&
             (supportedTypesBitmask0 == 0 || ((uint64_t(1))<<index.args[0].elemtype & supportedTypesBitmask0) != 0) &&
             (supportedTypesBitmask1 == 0 || ((uint64_t(1))<<index.args[1].elemtype & supportedTypesBitmask1) != 0) &&
             (mrm_opcode == 0))))
        {
            scs = false;
            return BinTranslation();
        }

        const int ordinaryIn = (index.args_size == 2 || index.args[2].tag == Arg::IIMMEDIATE) ? 1 : 2;

        std::vector<BinTranslation::Token> tokens;
        const uint32_t R = mrm_opcode != 0 ? 1 : ((index.args[0].idx & 0b1000) == 0);
        const uint32_t X = 1; //Alway in non-inplace operations.
        const uint32_t B = ((index.args[ordinaryIn].idx & 0b1000) == 0);
        const uint32_t L = 1;
        const uint32_t m_mmmm = (m_opcode == 0x0F ? 0b01 : (m_opcode == 0x0F38 ? 0b10 : (/*m_opcode == 0x0F3A ?*/0b11)));
        // pp:opcode extension providing equivalent functionality of a SIMD prefix: {00: None | 01: 66 | 10: F3 | 11: F2}
        const uint32_t pp = pp_opcode == 0 ? 0 : (pp_opcode == 0x66 ? 0b01 : (pp_opcode == 0xF3 ? 0b10 : (/*pp_opcode == 0xF2 ?*/ 0b11 )));

        if(W == 0 && B == 1 && X == 1 && m_mmmm == 0b00001)
        {
            tokens.push_back(BTsta((0xc5 << 1)|R, 9));
            if(ordinaryIn == 2 || mrm_opcode != 0) //vvvv field in VEX terminology
            {
                int vvvv_num = mrm_opcode != 0 ? 0 : 1;
                tokens.push_back(BTomm(vvvv_num, vvvv_num == 0 ? Out : In));
                tokens.push_back(BTsta(~(index.args[vvvv_num].idx&0b1111), 4));
            }
            else
                tokens.push_back(BTsta(0b1111, 4));
            tokens.push_back(BTsta((L<<2)|pp, 3));
        }
        else
        {
            tokens.push_back(BTsta((0xc4 << 9)|(R<<8)|(X<<7)|(B<<6)|(m_mmmm<<1)|W, 17));
            if(ordinaryIn == 2 || mrm_opcode != 0) //vvvv field in VEX terminology
            {
                int vvvv_num = mrm_opcode != 0 ? 0 : 1;
                tokens.push_back(BTomm(vvvv_num, vvvv_num == 0 ? Out : In));
                tokens.push_back(BTsta(~(index.args[vvvv_num].idx)&0b1111, 4));
            }
            else
                tokens.push_back(BTsta(0b1111, 4));
            tokens.push_back(BTsta((L<<2)|pp, 3));
        }

        //OPCODE construction: <prefix><leading bytes><other bytes>
        // R, X and B bits are complements of the REX prefix's R, X and B bits; these provide a fourth (high) bit for register index fields
        // (ModRM reg, SIB index, and ModRM r/m; SIB base; or opcode reg fields, respectively) allowing access to 16 instead of 8 registers.
        tokens.push_back(BTsta(opcode, 8));
        {//ModRM
            //Non-inplace case, first source register(or "non-destructive source register") is encoded in vvvv part of vex
            //So arg[2] is considered like usual, well-known intel input and scheme look for ModRM byte like it is two argument instruction
            const uint32_t mod = 0b11; //DUBUG:of course, it's not clear how we have to choose mod bits.
            tokens.push_back(BTsta(mod, 2));
            if(mrm_opcode != 0)
                tokens.push_back(BTsta(mrm_opcode, 3));
            else 
                tokens.push_back(BTreg(0, 3, Out));
            tokens.push_back(BTreg(ordinaryIn, 3, In));
            if(index.args_size == 3 && index.args[2].tag == Arg::IIMMEDIATE) 
                tokens.push_back(BTimm(2, 8));
            else if(index.args_size == 3 && ib_present) 
                tokens.push_back(BTsta(ib, 8));
        }
        return BinTranslation(tokens);
    }    

    BinTranslation i64BTLookup(const Syntop& index, bool& scs)
    {
        //TODO(ch): A lot of commands supports immediates of different widthes(8/32/64), but there are implemented just
        //fixed sizes(in most cases = 32). For space economy, it's better to implement different cases.
        using namespace BinTranslationConstruction;
        scs = true;
        switch (index.opcode)
        {
        case (INTEL64_MOVSX):
            if ((index.size() != 2 && index.size() != 3) || index[0].tag != Arg::IREG || index[1].tag != Arg::IREG)
                break;
            if (index.size() == 2)
            {
                if(index[0].elemtype == TYPE_I8)
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    static uint64_t statB[4] = { 0x480fbe, 0x4c0fbe, 0x490fbe, 0x4d0fbe };
                    if(index.args[1].idx == 12 || index.args[1].idx == 13)
                        return BiT({ nBkb(3, statB[statn], 2, index.args[1].idx == R13 ? 0b01: 0b00), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr8), BTsta(index.args[1].idx == R12 ? 0x24 : 0, 8) });//movsx rax, byte ptr [r12/r13]
                    else
                        return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr8) });        //movsx rax, byte ptr [rcx]
                }
                else if(index[0].elemtype == TYPE_I16)
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    static uint64_t statB[4] = { 0x480fbf, 0x4c0fbf, 0x490fbf, 0x4d0fbf };
                    if(index.args[1].idx == 12 || index.args[1].idx == 13)
                        return BiT({ nBkb(3, statB[statn], 2, index.args[1].idx == R13 ? 0b01: 0b00), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr16), BTsta(index.args[1].idx == R12 ? 0x24 : 0, 8) });//movsx rax, byte ptr [r12/r13]
                    else
                        return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr16) });        //movsx rax, word ptr [rcx]
                }
            }
            else if (index.size() == 3)
            {
                if (index[2].tag == Arg::IREG)
                {
                    if(index[0].elemtype == TYPE_I8)
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                        static uint64_t statB[8] = { 0x480fbe, 0x4c0fbe, 0x490fbe, 0x4d0fbe, 0x4a0fbe, 0x4e0fbe, 0x4b0fbe, 0x4f0fbe };
                        if(index[1].idx == R13)
                            return BiT({ nBkb(3, statB[statn], 2, 0b01), BTreg(0, 3, Out | Eff64), BTsta(0b10000, 5), BTreg(2, 3, In | Addr8), BTreg(1, 3, In | Addr8), BTsta(0, 8) });  //movsx rax, byte ptr [r13 + rdx]
                        else
                            return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out | Eff64), BTsta(0b10000, 5), BTreg(2, 3, In | Addr8), BTreg(1, 3, In | Addr8) });  //movsx rax, byte ptr [rcx + rdx]
                    }
                    else if(index[0].elemtype == TYPE_I16)
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                        static uint64_t statB[8] = { 0x480fbf, 0x4c0fbf, 0x490fbf, 0x4d0fbf, 0x4a0fbf, 0x4e0fbf, 0x4b0fbf, 0x4f0fbf };
                        if(index[1].idx == R13)
                            return BiT({ nBkb(3, statB[statn], 2, 0b01), BTreg(0, 3, Out | Eff64), BTsta(0b10000, 5), BTreg(2, 3, In | Addr16), BTreg(1, 3, In | Addr16), BTsta(0, 8) });  //movsx rax, word ptr [r13 + rdx]
                        else
                            return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out | Eff64), BTsta(0b10000, 5), BTreg(2, 3, In | Addr16), BTreg(1, 3, In | Addr16) });  //movsx rax, word ptr [rcx + rdx]
                    }
                }
                else if (index[2].tag == Arg::IIMMEDIATE)
                {
                    if(index[0].elemtype == TYPE_I8)
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                        static uint64_t statB[4] = { 0x480fbe, 0x4c0fbe, 0x490fbe, 0x4d0fbe };
                        if(index[1].idx == RSP || index[1].idx == R12)
                            return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr8), BTsta(0x24, 8), BTimm(2, 32, Addr8) });    //movsx rax, byte ptr [r12 + <offset>]
                        else
                            return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr8), BTimm(2, 32, Addr8) });    //movsx rax, byte ptr [rcx + <offset>]
                    }
                    else if(index[0].elemtype == TYPE_I16)
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                        static uint64_t statB[4] = { 0x480fbf, 0x4c0fbf, 0x490fbf, 0x4d0fbf };
                        if(index[1].idx == RSP || index[1].idx == R12)
                            return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr16), BTsta(0x24, 8), BTimm(2, 32, Addr16) });    //movsx rax, word ptr [r12 + <offset>]
                        else
                            return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr16), BTimm(2, 32, Addr16) });     //movsx rax, word ptr [rcx + <offset>]
                    }
                }
            }
            break;
        case (INTEL64_MOVSXD):
            if (index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG)
            {
                static uint64_t stats[4] = { 0x4863, 0x4c63, 0x4963, 0x4d63 };
                size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                if(index.args[1].idx == 12 || index.args[1].idx == 13)
                    return BiT({ nBkb(2, stats[statn], 2, index.args[1].idx == R13 ? 0b01: 0b00), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr32), BTsta(index.args[1].idx == R12 ? 0x24 : 0, 8) });  //movsxd rax, dword ptr [r12/r13]
                else
                    return BiT({ nBkb(2,stats[statn],2,0), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr32) }); //movsxd rax, dword ptr [rcx]
            }
            else if (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG)
            {
                if (index.args[2].tag == Arg::IREG)
                {
                    static uint64_t stats[8] = { 0x4863, 0x4c63, 0x4963, 0x4d63, 0x4a63, 0x4e63, 0x4b63, 0x4f63 };
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                    if(index[1].idx == R13)
                        return BiT({ nBkb(2,stats[statn],2,0b01), BTreg(0, 3, Out | Eff64), BTsta(0x10, 5), BTreg(2, 3, In | Addr32), BTreg(1, 3, In | Addr32), BTsta(0, 8) }); //movsxd rax, dword ptr [r13 + rdx]
                    else
                        return BiT({ nBkb(2,stats[statn],2,0), BTreg(0, 3, Out | Eff64), BTsta(0x10, 5), BTreg(2, 3, In | Addr32), BTreg(1, 3, In | Addr32) }); //movsxd rax, dword ptr [rcx + rdx]
                }
                else if (index.args[2].tag == Arg::IIMMEDIATE)
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    static uint64_t statB[4] = { 0x4863, 0x4c63, 0x4963, 0x4d63 };
                    if(index[1].idx == RSP || index[1].idx == R12)
                        return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr32), BTsta(0x24, 8), BTimm(2, 32, Addr32) });     //movsxd rax, dword ptr [r13 + <offset>]
                    else
                        return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr32), BTimm(2, 32, Addr32) });     //movsxd rax, dword ptr [rcx + <offset>]
                }
            }
            break;
        case (INTEL64_MOVZX):
            if ((index.size() != 2 && index.size() != 3) || index[0].tag != Arg::IREG || index[1].tag != Arg::IREG)
                break;
            if (index.size() == 2)
            {
                if(index[0].elemtype == TYPE_U8)
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    static uint64_t statB[4] = { 0x480fb6, 0x4c0fb6, 0x490fb6, 0x4d0fb6 };
                    if(index.args[1].idx == 12 || index.args[1].idx == 13)
                        return BiT({ nBkb(3, statB[statn], 2, index.args[1].idx == R13 ? 0b01: 0b00), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr8), BTsta(index.args[1].idx == R12 ? 0x24 : 0, 8) });//movzx rax, byte ptr [r12/r13]
                    else
                        return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr8) });        //movzx rax, byte ptr [rcx]

                }
                else if(index[0].elemtype == TYPE_U16)
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    static uint64_t statB[4] = { 0x480fb7, 0x4c0fb7, 0x490fb7, 0x4d0fb7 };
                    if(index.args[1].idx == 12 || index.args[1].idx == 13)
                        return BiT({ nBkb(3, statB[statn], 2, index.args[1].idx == R13 ? 0b01: 0b00), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr16), BTsta(index.args[1].idx == R12 ? 0x24 : 0, 8) });//movzx rax, word ptr [r12/r13]
                    else
                        return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr16) });        //movzx rax, word ptr [rcx]
                }
            }
            else if (index.size() == 3)
            {
                if (index[2].tag == Arg::IREG)
                {
                    if(index[0].elemtype == TYPE_U8)
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                        static uint64_t statB[8] = { 0x480fb6, 0x4c0fb6, 0x490fb6, 0x4d0fb6, 0x4a0fb6, 0x4e0fb6, 0x4b0fb6, 0x4f0fb6 };
                        if(index[1].idx == R13)
                            return BiT({ nBkb(3, statB[statn], 2, 0b01), BTreg(0, 3, Out | Eff64), BTsta(0b10000, 5), BTreg(2, 3, In | Addr8), BTreg(1, 3, In | Addr8), BTsta(0, 8) });  //movzx rax, byte ptr [r13 + rdx]
                        else
                            return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out | Eff64), BTsta(0b10000, 5), BTreg(2, 3, In | Addr8), BTreg(1, 3, In | Addr8) });  //movzx rax, byte ptr [rcx + rdx]
                    }
                    else if(index[0].elemtype == TYPE_U16)
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                        static uint64_t statB[8] = { 0x480fb7, 0x4c0fb7, 0x490fb7, 0x4d0fb7, 0x4a0fb7, 0x4e0fb7, 0x4b0fb7, 0x4f0fb7 };
                        if(index[1].idx == R13)
                            return BiT({ nBkb(3, statB[statn], 2, 0b01), BTreg(0, 3, Out | Eff64), BTsta(0b10000, 5), BTreg(2, 3, In | Addr16), BTreg(1, 3, In | Addr16), BTsta(0, 8) });  //movzx rax, word ptr [r13 + rdx]
                        else
                            return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out | Eff64), BTsta(0b10000, 5), BTreg(2, 3, In | Addr16), BTreg(1, 3, In | Addr16) });  //movzx rax, word ptr [rcx + rdx]
                    }
                }
                else if (index[2].tag == Arg::IIMMEDIATE)
                {
                    if(index[0].elemtype == TYPE_U8)
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                        static uint64_t statB[4] = { 0x480fb6, 0x4c0fb6, 0x490fb6, 0x4d0fb6 };
                        if(index[1].idx == RSP || index[1].idx == R12)
                            return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr8), BTsta(0x24, 8), BTimm(2, 32, Addr8) });//movzx rax, byte ptr [r12 + <offset>]
                        else
                            return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr8), BTimm(2, 32, Addr8) });    //movzx rax, byte ptr [rcx + <offset>]
                    }
                    else if(index[0].elemtype == TYPE_U16)
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                        static uint64_t statB[4] = { 0x480fb7, 0x4c0fb7, 0x490fb7, 0x4d0fb7 };
                        if(index[1].idx == RSP || index[1].idx == R12)
                            return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr16), BTsta(0x24, 8), BTimm(2, 32, Addr16) });//movzx rax, word ptr [r12 + <offset>]
                        else
                            return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(0, 3, Out | Eff64), BTreg(1, 3, In | Addr16), BTimm(2, 32, Addr16) });     //movzx rax, word ptr [rcx + <offset>]
                    }
                }
            }
            break;
        case (INTEL64_MOV):
            if (index.size() == 2)
            {
                if (index[0].tag == Arg::IREG)
                {
                    if (index[1].tag == Arg::IREG)
                    {
                        if (index[0].flags & AF_ADDRESS)
                        {
                            if(elem_size(index[1].elemtype) == 1)
                            {
                                if (index[0].idx == R12 || index[0].idx == R13) //mov byte ptr [r12/r13], bx
                                {
                                    return BiT({ nBkb(2, ((index[1].idx < 8) ? 0x4188 : 0x4588), 2, (index[0].idx == R12 ? 0 : 1)), BTreg(1, 3, In), BTreg(0, 3, In | Addr8), BTsta((index[0].idx == R12 ? 0x24 : 0), 8) });
                                }
                                else if (index[0].idx < 8 && index[1].idx >= 4 && index[1].idx < 8)
                                {
                                    return BiT({ nBkb(2, 0x4088, 2, 0), BTreg(1, 3, In), BTreg(0, 3, In | Addr8) }); //mov byte ptr [rax], dil
                                }
                                else
                                {
                                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                    static uint64_t statB[4] = { 0x88, 0x4188, 0x4488, 0x4588 };
                                    static int statBn[4] = { 1, 2, 2, 2 };
                                    return BiT({ nBkb(statBn[statn], statB[statn], 2, 0), BTreg(1, 3, In), BTreg(0, 3, In | Addr8) }); //mov byte ptr [rax], al
                                }                                
                            }
                            else if(elem_size(index[1].elemtype) == 2)
                            {
                                if (index[0].idx == R12 || index[0].idx == R13) //mov word ptr [r12/r13], bx
                                {
                                    return BiT({ nBkb(3, ((index[1].idx < 8) ? 0x664189 : 0x664589), 2, (index[0].idx == R12 ? 0 : 1)), BTreg(1, 3, In), BTreg(0, 3, In | Addr16), BTsta((index[0].idx == R12 ? 0x24 : 0), 8) });
                                }
                                else
                                {
                                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                    static uint64_t statB[4] = { 0x6689, 0x664189, 0x664489, 0x664589 };
                                    static int statBn[4] = { 2, 3, 3, 3 };
                                    return BiT({ nBkb(statBn[statn], statB[statn], 2, 0), BTreg(1, 3, In), BTreg(0, 3, In | Addr16) }); //mov word ptr [rax], bx
                                }                                
                            }
                            else if(elem_size(index[1].elemtype) == 4)
                            {
                                if (index[0].idx == R12 || index[0].idx == R13) //mov dword ptr [r12/r13], ebx
                                {
                                    static uint64_t statB[4] = { 0x4189, 0x4589, 0x4189, 0x4589 };
                                    size_t statn = index[0].idx == R13 ? (index[1].idx < 8 ? 0 : 1) : (index[1].idx < 8 ? 2 : 3);
                                    return BiT({ nBkb(2, statB[statn], 2, index[0].idx == R13 ? 0b01 : 0b00), BTreg(1, 3, In), BTreg(0, 3, In | Addr32) , BTsta(index[0].idx == R13 ? 0 : 0x24, 8) });
                                }
                                else //mov dword ptr [rax], ebx
                                {
                                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                    static uint64_t stats[4] = { 0x224, 0x10624, 0x11224, 0x11624 };
                                    static int statw[4] = { 10, 18, 18, 18 };
                                    return BiT({ BTsta(stats[statn], statw[statn]), BTreg(1, 3, In), BTreg(0, 3, In | Addr32) });
                                }
                            }
                            else if(elem_size(index[1].elemtype) == 8)
                            {
                                size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                static uint64_t statB[4] = { 0x4889, 0x4989, 0x4c89, 0x4d89 };
                                if (index[0].idx == R12 || index[0].idx == R13) //mov qword ptr [r12/r13], rbx
                                    return BiT({ nBkb(2, statB[statn], 2, index[0].idx == R13 ? 0b01 : 0b00), BTreg(1, 3, In), BTreg(0, 3, In | Addr64), BTsta((index[0].idx == R12 ? 0x24 : 0), 8) });
                                else
                                    return BiT({ nBkb(2, statB[statn], 2, 0b00), BTreg(1, 3, In), BTreg(0, 3, In | Addr64) }); //mov qword ptr [rax], rbx
                            }
                        }
                        else if(index[1].flags & AF_ADDRESS)
                        {
                            if(index[0].elemtype == TYPE_U32 || index[0].elemtype == TYPE_FP32)
                            {
                                static uint64_t statB[4] = { 0x8b, 0x448b, 0x418b, 0x458b };
                                static int statBn[4] = { 1, 2, 2, 2 };
                                size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                if(index.args[1].idx == 12 || index.args[1].idx == 13)
                                    return BiT({ nBkb(statBn[statn], statB[statn], 2, index.args[1].idx == R13 ? 0b01: 0b00), BTreg(0, 3, Out), BTreg(1, 3, In | Addr32), BTsta(index.args[1].idx == R12 ? 0x24 : 0, 8) }); //mov eax, dword ptr [r12/r13]
                                else
                                    return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b00), BTreg(0, 3, Out), BTreg(1, 3, In | Addr32) }); //mov eax, dword ptr [rbx]
                            } 
                            else if(elem_size(index[0].elemtype) == 8)
                            {
                                static uint64_t statB[4] = { 0x488b, 0x4c8b, 0x498b, 0x4d8b };
                                size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                if(index.args[1].idx == 12 || index.args[1].idx == 13)
                                    return BiT({ nBkb(2, statB[statn], 2, index.args[1].idx == R13 ? 0b01: 0b00), BTreg(0, 3, Out), BTreg(1, 3, In | Addr64), BTsta(index.args[1].idx == R12 ? 0x24 : 0, 8) }); //mov rax, qword ptr [r12/r13]
                                else
                                    return BiT({ nBkb(2, statB[statn], 2, 0b00), BTreg(0, 3, Out), BTreg(1, 3, In | Addr64) }); //mov rax, qword ptr [rbx]
                            }
                        }
                        else
                        {
                            static uint64_t stats[4] = { 0x12227, 0x12627, 0x13227, 0x13627 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            return BiT({ BTsta(stats[statn], 18), BTreg(1, 3, In), BTreg(0, 3, Out) }); //mov rax, rbx
                        }
                    }
                    else if (index[1].tag == Arg::ISPILLED)
                        return BiT({ BTsta(index[0].idx < 8 ? 0x1222E : 0x1322E, 18), BTreg(0, 3, In), BTsta(0x424, 11), BTspl(1, 32) }); //mov rax, [rsp + offset]
                    else if (index[1].tag == Arg::IIMMEDIATE)
                    {
                        if(index[0].flags & AF_ADDRESS)
                        {
                            if(elem_size(index[1].elemtype) == 1)
                            {
                                if (index[0].idx == R12 || index[0].idx == R13) //mov byte ptr [r12/r13], <imm>
                                    return  BiT({ nBkb(2, 0x41c6, 5, index[0].idx == R12 ? 0 : 0b1000), BTreg(0, 3, In | Addr8), BTsta(index[0].idx == R12 ? 0b00100100 : 0, 8), BTimm(1, 8) });
                                else
                                    return  BiT({ index[0].idx < 8 ? nBkb(1, 0xc6, 5, 0) : nBkb(2, 0x41c6, 5, 0), BTreg(0, 3, In | Addr8), BTimm(1, 8) });//mov byte ptr [rax], <imm>
                            }
                            else if(elem_size(index[1].elemtype) == 2)
                            {
                                if (index[0].idx == R12 || index[0].idx == R13) //mov word ptr [r12/r13], <imm>
                                    return  BiT({ nBkb(3, 0x6641c7, 5, index[0].idx == R12 ? 0 : 0b1000), BTreg(0, 3, In | Addr16), BTsta(index[0].idx == R12 ? 0b00100100 : 0, 8), BTimm(1, 16) });
                                else
                                    return  BiT({ index[0].idx < 8 ? nBkb(2, 0x66c7, 5, 0) : nBkb(3, 0x6641c7, 5, 0), BTreg(0, 3, In | Addr16), BTimm(1, 16) });//mov word ptr [rax], <imm>
                            }
                            else if(elem_size(index[1].elemtype) == 4)
                            {
                                if (index[0].idx == R12 || index[0].idx == R13) //mov dword ptr [r12/r13], <imm>
                                    return  BiT({ nBkb(2, 0x41c7, 5, index[0].idx == R12 ? 0 : 0b1000), BTreg(0, 3, In | Addr32), BTsta(index[0].idx == R12 ? 0b00100100 : 0, 8), BTimm(1, 32) });
                                else
                                    return  BiT({ index[0].idx < 8 ? nBkb(1, 0xc7, 5, 0) : nBkb(2, 0x41c7, 5, 0), BTreg(0, 3, In | Addr32), BTimm(1, 32) });//mov dword ptr [rax], <imm>
                            }
                            else if(elem_size(index[1].elemtype) == 8)
                            {
                                if (index[0].idx == R12 || index[0].idx == R13) //mov qword ptr [r12/r13], <imm>
                                    return  BiT({ nBkb(2, 0x49c7, 5, index[0].idx == R12 ? 0 : 0b1000), BTreg(0, 3, In | Addr64), BTsta(index[0].idx == R12 ? 0b00100100 : 0, 8), BTimm(1, 32) });
                                else
                                    return  BiT({ nBkb(2, index[0].idx < 8 ? 0x48c7 : 0x49c7, 5, 0), BTreg(0, 3, In | Addr64), BTimm(1, 32) });//mov qword ptr [rax], <imm>
                            }
                        }
                        else if(index[1].value > int64_t(0x7fffffff) || index[1].value < (-(int64_t(0x7fffffff) + 1)))
                        {
                            return BiT({ nBkb(1, index[0].idx < 8 ? 0x48 : 0x49, 5, 0b10111), BTreg(0, 3, Out), BTimm(1, 64) });   //mov rax, <imm64>
                        }
                        else 
                            return  BiT({ nBkb(2, index[0].idx < 8 ? 0x48c7 : 0x49c7, 5, 0b11000), BTreg(0, 3, Out), BTimm(1, 32) });//mov rax, <imm32>
                    }
                }
                else if (index[0].tag == Arg::ISPILLED)
                {
                    if (index[1].tag == Arg::IREG)
                        return BiT({ nBkb(2, index[1].idx < 8 ? 0x4889 : 0x4c89, 2, 0b10), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 32) });   //mov qword ptr [rsp + offset], rbx
                    else if (index[1].tag == Arg::IIMMEDIATE)
                        return BiT({ BTsta(0x48c78424, 32), BTspl(0, 32), BTimm(1, 32) });  //mov qword ptr [rsp + offset], <imm>
                }
            }
            else if (index.size() == 3)
            {
                if (index[0].tag != Arg::IREG)
                    break;
                if (!(index[0].flags & AF_ADDRESS))
                {
                    if (index[1].tag != Arg::IREG || !(index[1].flags & AF_ADDRESS) || !(index[2].flags & AF_ADDRESS))
                        break;
                    if (index[2].tag == Arg::IREG)
                    {
                        if(index[0].elemtype == TYPE_U32 || index[0].elemtype == TYPE_FP32)
                        {
                            static int statBn[8] = { 1, 2, 2, 2, 2, 2, 2, 2 };
                            static uint64_t statB[8] = { 0x8b, 0x448b, 0x418b, 0x458b, 0x428b, 0x468b, 0x438b, 0x478b };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                            if(index[1].idx == R13)
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b01), BTreg(0, 3, Out), BTsta(0b10000, 5), BTreg(2, 3, In | Addr32), BTreg(1, 3, In | Addr32), BTsta(0, 8)});  //mov eax, dword ptr [r13 + rdx]
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b00), BTreg(0, 3, Out), BTsta(0b10000, 5), BTreg(2, 3, In | Addr32), BTreg(1, 3, In | Addr32)});  //mov eax, dword ptr [rcx + rdx]
                        }
                        else if(elem_size(index[0].elemtype)== 8)
                        {
                            static uint64_t statB[8] = { 0x488b, 0x4c8b, 0x498b, 0x4d8b, 0x4a8b, 0x4e8b, 0x4b8b, 0x4f8b };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                            if(index[1].idx == R13)
                                return BiT({ nBkb(2, statB[statn], 2, 0b01), BTreg(0, 3, Out), BTsta(0b10000, 5), BTreg(2, 3, In | Addr64), BTreg(1, 3, In | Addr64), BTsta(0, 8) });  //mov rax, qword ptr [r13 + rdx]
                            else
                                return BiT({ nBkb(2, statB[statn], 2, 0b00), BTreg(0, 3, Out), BTsta(0b10000, 5), BTreg(2, 3, In | Addr64), BTreg(1, 3, In | Addr64) });  //mov rax, qword ptr [rcx + rdx]
                        }
                    }
                    else if (index[2].tag == Arg::IIMMEDIATE)
                    {
                        if(index[0].elemtype == TYPE_U32 || index[0].elemtype == TYPE_FP32)
                        {
                            static uint64_t statB[4] = { 0x8b, 0x448b, 0x418b, 0x458b };
                            static int statBn[4] = { 1, 2, 2, 2 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            if(index[1].idx == RSP || index[1].idx == R12)
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b10), BTreg(0, 3, Out), BTreg(1, 3, In | Addr32), BTsta(0x24, 8), BTimm(2, 32, Addr32)});  //mov eax, dword ptr [r12 + <offset>]
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b10), BTreg(0, 3, Out), BTreg(1, 3, In | Addr32), BTimm(2, 32, Addr32)});  //mov eax, dword ptr [rcx + <offset>]
                        }
                        else if(elem_size(index[0].elemtype) == 8)
                        {
                            static uint64_t statB[4] = { 0x488b, 0x4c8b, 0x498b, 0x4d8b };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            if(index[1].idx == RSP || index[1].idx == R12)
                                return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(0, 3, Out), BTreg(1, 3, In | Addr64), BTsta(0x24, 8), BTimm(2, 32, Addr64) });  //mov rax, qword ptr [r12 + <offset>]
                            else
                                return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(0, 3, Out), BTreg(1, 3, In | Addr64), BTimm(2, 32, Addr64) });  //mov rax, qword ptr [rcx + <offset>]
                        }
                    }
                }
                else if (index[1].tag == Arg::IREG && (index[1].flags & AF_ADDRESS))
                {
                    if (index[2].tag == Arg::IREG)
                    {
                        if(elem_size(index[2].elemtype) == 1)
                        {
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                            static uint64_t statB[8] = { 0x88, 0x4188, 0x4288, 0x4388, 0x4488, 0x4588, 0x4688, 0x4788 };
                            static int statBn[8] = { 1, 2, 2, 2, 2, 2, 2, 2 };
                            if (index[0].idx == R13)
                                return BiT({ nBkb(2, statB[statn], 2, 1), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In | Addr8), BTreg(0, 3, In | Addr8), BTsta(0, 8) }); //mov byte ptr [r13 + rbx], cl
                            else if (index[0].idx < 8 && index[2].idx >= 4 && index[2].idx < 8)
                            {
                                uint64_t stat = statn == 0 ? 0x4088 : statB[statn];
                                return BiT({ nBkb(2, stat, 2, 0), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In | Addr8), BTreg(0, 3, In | Addr8) }); //mov byte ptr [rax + rbx], dil
                            }
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In | Addr8), BTreg(0, 3, In | Addr8) }); //mov byte ptr [rax + rbx], cl
                        }
                        else if(elem_size(index[2].elemtype) == 2)
                        {
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                            static uint64_t statB[8] = { 0x6689, 0x664189, 0x664289, 0x664389, 0x664489, 0x664589, 0x664689, 0x664789 };
                            static int statBn[8] = { 2, 3, 3, 3, 3, 3, 3, 3 };
                            if (index[0].idx == R13)
                                return BiT({ nBkb(3, statB[statn], 2, 1), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In | Addr16), BTreg(0, 3, In | Addr16), BTsta(0, 8) }); //mov word ptr [r13 + rbx], cx
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In | Addr16), BTreg(0, 3, In | Addr16) }); //mov word ptr [rax + rbx], cx
                        }
                        else if(elem_size(index[2].elemtype) == 4)
                        {
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                            static uint64_t statB[8] = { 0x89, 0x4189, 0x4289, 0x4389, 0x4489, 0x4589, 0x4689, 0x4789 };
                            static int statBn[8] = { 1, 2, 2, 2, 2, 2, 2, 2 };
                            if (index[0].idx == R13)
                                return BiT({ nBkb(2, statB[statn], 2, 1), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In | Addr32), BTreg(0, 3, In | Addr32), BTsta(0, 8) }); //mov dword ptr [r13 + rbx], ecx
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In | Addr32), BTreg(0, 3, In | Addr32) }); //mov dword ptr[rax + rbx], ecx
                        }
                        else if(elem_size(index[2].elemtype) == 8)
                        {
                            static uint64_t statB[8] = { 0x4889, 0x4989, 0x4a89, 0x4b89, 0x4c89, 0x4d89, 0x4e89, 0x4f89 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                            if (index[0].idx == R13)
                                return BiT({ nBkb(2, statB[statn], 2, 1), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In | Addr64), BTreg(0, 3, In | Addr64), BTsta(0, 8) }); //mov qword ptr [r13 + rbx], rcx
                            else
                                return BiT({ nBkb(2, statB[statn], 2, 0), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In | Addr64), BTreg(0, 3, In | Addr64) });  //mov qword ptr [rax + rbx], rcx
                        }
                    }
                    else if (index[2].tag == Arg::IIMMEDIATE)
                    {
                        if(elem_size(index[2].elemtype) == 1)
                        {
                            static uint64_t statB[4] = { 0xc604, 0x41c604, 0x42c604, 0x43c604 };
                            static int statBn[4] = { 2, 3, 3, 3 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            if (index[0].idx == R13)
                                return BiT({ nBkb(3, (index[1].idx < 8) ? 0x41c644 : 0x43c644, 2, 0b00), BTreg(1, 3, In | Addr8), BTreg(0, 3, In | Addr8), BTsta(0, 8), BTimm(2, 8) }); //mov byte ptr [r13 + rcx], <imm>
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b00), BTreg(1, 3, In | Addr8), BTreg(0, 3, In | Addr8), BTimm(2, 8) });                 //mov byte ptr [rax + rcx], <imm>
                        }
                        else if(elem_size(index[2].elemtype) == 2)
                        {
                            static uint64_t statB[4] = { 0x66c704, 0x6641c704, 0x6642c704, 0x6643c704 };
                            static int statBn[4] = { 3, 4, 4, 4 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            if (index[0].idx == R13)
                                return BiT({ nBkb(4, (index[1].idx < 8) ? 0x6641c744 : 0x6643c744, 2, 0b00), BTreg(1, 3, In | Addr16), BTreg(0, 3, In | Addr16), BTsta(0, 8), BTimm(2, 16) }); //mov word ptr [r13 + rcx], <imm>
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b00), BTreg(1, 3, In | Addr16), BTreg(0, 3, In | Addr16), BTimm(2, 16) });                     //mov word ptr [rax + rcx], <imm>
                        }
                        else if(elem_size(index[2].elemtype) == 4)
                        {
                            static uint64_t statB[4] = { 0xC704, 0x41C704, 0x42C704, 0x43C704 };
                            static int statBn[4] = { 2, 3, 3, 3 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            if (index[0].idx == R13)
                                return BiT({ nBkb(3, (index[1].idx < 8) ? 0x41c744 : 0x43c744, 2, 0b00), BTreg(1, 3, In | Addr32), BTreg(0, 3, In | Addr32), BTsta(0, 8), BTimm(2, 32) }); //mov dword ptr [r13 + rcx], <imm>
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b00), BTreg(1, 3, In | Addr32), BTreg(0, 3, In | Addr32), BTimm(2, 32) });                 //mov dword ptr [rax + rcx], <imm>
                        }
                        else if(elem_size(index[2].elemtype) == 8)
                        {
                            static uint64_t statB[4] = { 0x48c704, 0x49c704, 0x4ac704, 0x4bc704 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            if (index[0].idx == R13)
                                return BiT({ nBkb(3, (index[1].idx < 8) ? 0x49c744 : 0x4bc744, 2, 0b00), BTreg(1, 3, In | Addr64), BTreg(0, 3, In | Addr64), BTsta(0, 8), BTimm(2, 32) }); //mov qword ptr [r13 + rcx], <imm>
                            else
                                return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(1, 3, In | Addr64), BTreg(0, 3, In | Addr64), BTimm(2, 32) });                                 //mov qword ptr [rax + rcx], <imm>
                        }
                    };
                }
                else if (index[1].tag == Arg::IIMMEDIATE && (index[1].flags & AF_ADDRESS))
                {
                    if (index[2].tag == Arg::IREG)
                    {
                        if(elem_size(index[2].elemtype) == 1)
                        {
                            static uint64_t statB[4] = { 0x88, 0x4188, 0x4488, 0x4588 };
                            static int statBn[4] = { 1, 2, 2, 2 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[2].idx < 8) ? 0 : 2);
                            if (index[0].idx == R12)
                                return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In | Addr8), BTsta(0x24, 8), BTimm(1, 32, Addr8) });//mov byte ptr[r12 + <offset>], cl
                            else if (index[0].idx < 8 && index[2].idx >= 4 && index[2].idx < 8)
                                return BiT({ nBkb(2, 0x4088, 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In | Addr8), BTimm(1, 32, Addr8) });                      //mov byte ptr[rax + <offset>], dil
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In | Addr8), BTimm(1, 32, Addr8) });    //mov byte ptr[rax + <offset>], cl
                        }
                        else if(elem_size(index[2].elemtype) == 2)
                        {
                            static uint64_t statB[4] = { 0x6689, 0x664189, 0x664489, 0x664589 };
                            static int statBn[4] = { 2, 3, 3, 3 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[2].idx < 8) ? 0 : 2);
                            if (index[0].idx == R12)
                                return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In | Addr16), BTsta(0x24, 8), BTimm(1, 32, Addr16) });//mov word ptr [r12 + <offset>], cx
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In | Addr16), BTimm(1, 32, Addr16) });    //mov word ptr [rax + <offset>], cx
                        }
                        else if(elem_size(index[2].elemtype) == 4)
                        {
                            static uint64_t statB[4] = { 0x89, 0x4189, 0x4489, 0x4589 };
                            static int statBn[4] = { 1, 2, 2, 2 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[2].idx < 8) ? 0 : 2);
                            if (index[0].idx == R12)
                                return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In | Addr32), BTsta(0x24, 8), BTimm(1, 32, Addr32) });//mov dword ptr [r12 + <offset>], ecx
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In | Addr32), BTimm(1, 32, Addr32) });    //mov dword ptr [rax + <offset>], ecx
                        }
                        else if(elem_size(index[2].elemtype) == 8)
                        {
                            static uint64_t statB[4] = { 0x4889, 0x4989, 0x4c89, 0x4d89 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[2].idx < 8) ? 0 : 2);
                            if (index[0].idx == R12)
                                return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In | Addr64), BTsta(0x24, 8), BTimm(1, 32, Addr64) });//mov qword ptr [r12 + <offset>], rcx
                            else
                                return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In | Addr64), BTimm(1, 32, Addr64) });                //mov qword ptr [rax + <offset>], rcx
                        }
                    }
                    else if (index[2].tag == Arg::IIMMEDIATE)
                    {
                        if(elem_size(index[2].elemtype) == 1)
                        {
                            uint64_t stat = ((index[0].idx < 8) ? 0xc6 : 0x41c6);
                            int statw = ((index[0].idx < 8) ? 1 : 2);
                            if (index[0].idx == RSP || index[0].idx == R12)
                                return BiT({ nBkb(statw, stat, 5, 0b10000), BTreg(0, 3, In | Addr8), BTsta(0x24, 8), BTimm(1, 32, Addr8), BTimm(2, 8) });//mov byte ptr [r12 + <offset>], <imm>    
                            else
                                return BiT({ nBkb(statw, stat, 5, 0b10000), BTreg(0, 3, In | Addr8), BTimm(1, 32, Addr8), BTimm(2, 8) });            //mov byte ptr [rax + <offset>], <imm>
                        }
                        else if(elem_size(index[2].elemtype) == 2)
                        {
                            uint64_t stat = ((index[0].idx < 8) ? 0x66c7 : 0x6641c7);
                            int statw = ((index[0].idx < 8) ? 2 : 3);
                            if (index[0].idx == RSP || index[0].idx == R12)
                                return BiT({ nBkb(statw, stat, 5, 0b10000), BTreg(0, 3, In | Addr16), BTsta(0x24, 8),BTimm(1, 32, Addr16), BTimm(2, 16) });//mov word ptr [r12 + <offset>], <imm>    
                            else
                                return BiT({ nBkb(statw, stat, 5, 0b10000), BTreg(0, 3, In | Addr16), BTimm(1, 32, Addr16), BTimm(2, 16) });           //mov word ptr [rax + <offset>], <imm>
                        }
                        else if(elem_size(index[2].elemtype) == 4)
                        {
                            uint64_t stat = ((index[0].idx < 8) ? 0xc7 : 0x41c7);
                            int statw = ((index[0].idx < 8) ? 1 : 2);
                            if (index[0].idx == RSP || index[0].idx == R12)
                                return BiT({ nBkb(statw, stat, 5, 0b10000), BTreg(0, 3, In | Addr32), BTsta(0x24, 8), BTimm(1, 32, Addr32), BTimm(2, 32) });//mov dword ptr [r12 + <offset>], <imm>    
                            else
                                return BiT({ nBkb(statw, stat, 5, 0b10000), BTreg(0, 3, In | Addr32), BTimm(1, 32, Addr32), BTimm(2, 32) });            //mov dword ptr [rax + <offset>], <imm>
                        }
                        else if(elem_size(index[2].elemtype) == 8)
                        {
                            uint64_t stat = ((index[0].idx < 8) ? 0x48c7: 0x49c7);
                            if (index[0].idx == RSP || index[0].idx == R12)
                                return BiT({ nBkb(2, stat, 5, 0b10000), BTreg(0, 3, In | Addr64), BTsta(0x24, 8 ), BTimm(1, 32, Addr64), BTimm(2, 32) });//mov qword ptr [r12 + <offset>], <imm>    
                            else
                                return BiT({ nBkb(2, stat, 5, 0b10000), BTreg(0, 3, In | Addr64), BTimm(1, 32, Addr64), BTimm(2, 32) });                 //mov qword ptr [rax + <offset>], <imm>
                        }
                    }
                }
            }
            break;
        case (INTEL64_ADD):
            if (index.size() == 2 && index[0].tag == Arg::IREG)
            {
                if (index[1].tag == Arg::IREG)
                {
                    static uint64_t stats[4] = { 0x12007, 0x12407, 0x13007, 0x13407 };
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    return BiT({ BTsta(stats[statn], 18), BTreg(1, 3, In), BTreg(0, 3, In | Out) });           //add rax, rbx
                }
                else if (index[1].tag == Arg::ISPILLED)
                    return BiT({ BTsta(index[0].idx < 8 ? 0x1200E : 0x1300E, 18), BTreg(0, 3, In), BTsta(0x424, 11), BTspl(1, 32) }); //add rax, [rsp + offset]
                else if (index[1].tag == Arg::IIMMEDIATE)
                {
                    if(index[0].idx == RAX) 
                        return BiT({ BTsta(0b01001, 5), BTreg(0, 3, Out), BTsta(0x05, 8), BTimm(1, 32) }); //add rax, <imm32>
                    else
                        return BiT({ nBkb(2, index[0].idx < 8 ? 0x4881 : 0x4981, 5,0b11000), BTreg(0, 3, Out), BTimm(1, 32) });  //add rcx, <imm32>
                }
            }
            else if (index.size() == 2 && index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ BTsta(index[1].idx < 8 ? 0x12006 : 0x13006, 18), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 32) });   //add [rsp + offset], rbx
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x48818424, 32), BTspl(0, 32), BTimm(1, 32) });  //add QWORD PTR [rsp + offset], <imm>
            }
            break;
        case (INTEL64_ADC):
            if (index.size() == 2 && index[0].tag == Arg::IREG)
            {
                if (index[1].tag == Arg::IREG)
                {
                    static uint64_t statB[4] = { 0x4811, 0x4911, 0x4c11, 0x4d11 };
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    return BiT({ nBkb(2, statB[statn], 2, 0b11), BTreg(1, 3, In), BTreg(0, 3, IO) });        //adc rax, rbx
                }
                else if (index[1].tag == Arg::IIMMEDIATE)
                {
                    if(index[0].idx == 0)
                        return BiT({ BTsta(0x48, 8), BTreg(0, 3, IO), BTsta(0b10101, 5), BTimm(1, 32) });    //adc rax, <imm>
                    else
                        return BiT({ nBkb(2, index[0].idx < 8 ? 0x4881 : 0x4981, 5, 0b11010), BTreg(0, 3, IO), BTimm(1, 32) });    //adc rcx, <imm>
                }
                else if (index[1].tag == Arg::ISPILLED)
                    return BiT({ nBkb(2, (index[0].idx < 8) ? 0x4813 : 0x4c13, 2, 0b10), BTreg(0, 3, IO), BTsta(0x424, 11), BTspl(1, 32) }); //adc rax, [rsp + offset]
            }
            else if (index.size() == 2 && index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ nBkb(2, index[1].idx < 8 ? 0x4811 : 0x4c11, 2, 0b10), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 32) });    //adc [rsp + offset], rbx
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x48819424, 32), BTspl(0, 32), BTimm(1, 32) });  //adc QWORD PTR [rsp + offset], <imm>
            }
            break;
        case (INTEL64_SUB):
            if (index.size() == 2 && index[0].tag == Arg::IREG)
            {
                if (index[1].tag == Arg::IREG)
                {
                    static uint64_t stats[4] = { 0x120A7, 0x124A7, 0x130A7, 0x134A7 };
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    return BiT({ BTsta(stats[statn], 18), BTreg(1, 3, In), BTreg(0, 3, In | Out) });           //sub rax, rbx
                }
                else if (index[1].tag == Arg::ISPILLED)
                    return BiT({ nBkb(2, index[0].idx < 8 ? 0x482b : 0x4c2b, 2, 0b10), BTreg(0, 3, In), BTsta(0x424, 11), BTspl(1, 32) }); //sub rax, [rsp + offset]
                else if (index[1].tag == Arg::IIMMEDIATE)
                {
                    if(index[0].idx == RAX) 
                        return BiT({ BTsta(0b01001, 5), BTreg(0, 3, Out), BTsta(0x2d, 8), BTimm(1, 32) }); //sub rax, <imm32>
                    else
                        return BiT({ nBkb(2, index[0].idx < 8 ? 0x4881 : 0x4981, 5,0b11101), BTreg(0, 3, Out), BTimm(1, 32) });  //sub rcx, <imm32>
                }
            }
            else if (index.size() == 2 && index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ nBkb(2, index[1].idx < 8 ? 0x4829 : 0x4c29, 2, 0b10), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 32) });   //sub [rsp + offset], rbx
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x4881ac24, 32), BTspl(0, 32), BTimm(1, 32) });  //sub QWORD PTR [rsp + offset], <imm>
            }
            break;
        case (INTEL64_IMUL):
            if (index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG)
            {
                static uint64_t stats[4] = { 0x1203EBF, 0x1303EBF, 0x1243EBF, 0x1343EBF };
                size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                return BiT({ BTsta(stats[statn], 26), BTreg(0, 3, In | Out), BTreg(1, 3, In) });
            }
            else if(index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::ISPILLED)
                return BiT({ nBkb(3, index[0].idx < 8 ? 0x480faf : 0x4c0faf, 2, 0b10), BTreg(0, 3, In), BTsta(0x424, 11), BTspl(1, 32) }); //imul rax, [rsp + offset]
            else if (index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE)
                return BiT({ index[0].idx < 8 ? nBkb(2, 0x486b, 2, 0b11) : nBkb(2, 0x4d6b, 2, 0b11), BTreg(0, 3, In | Out) , BTreg(0, 3, In | Out), BTimm(1, 8) });
            break;
        case (INTEL64_IDIV):
            if (index.size() == 1 && index[0].tag == Arg::IREG)
                return BiT({ BTsta(index[0].idx < 8 ? 0x91EFF : 0x93EFF, 21), BTreg(0, 3, In) });
            else if(index.size() == 1 && index[0].tag == Arg::ISPILLED)
                return BiT({ BTsta(0x48f7bc24, 32), BTspl(0, 32) });
            break;
        case (INTEL64_SHL):
            if (index.size() == 2)
            {
                if (index[1].tag == Arg::IIMMEDIATE)
                {
                    if (index[0].tag == Arg::IREG)
                        return BiT({ nBkb(2, index[0].idx < 8 ? 0x48c1 : 0x49c1, 5, 0b11100), BTreg(0, 3, In | Out), BTimm(1, 8) });
                    else if (index[0].tag == Arg::ISPILLED)
                        return BiT({ BTsta(0x48c1a424, 32), BTspl(0, 32), BTimm(1, 8) });
                }
                else if (index[1].tag == Arg::IREG && index[1].idx == RCX)
                {
                    if (index[0].tag == Arg::IREG)
                        return BiT({ nBkb(2, index[0].idx < 8 ? 0x48d3 : 0x49d3, 5, 0b11100), BTreg(0, 3, In | Out), BTreg(1, 0, In) });
                    else if (index[0].tag == Arg::ISPILLED)
                        return BiT({ BTsta(0x48d3a424, 32), BTspl(0, 32), BTreg(1, 0, In) });
                }
            }
            break;
        case (INTEL64_SHR):
            if (index.size() == 2)
            {
                if (index[1].tag == Arg::IIMMEDIATE)
                {
                    if (index[0].tag == Arg::IREG)
                        return BiT({ nBkb(2, index[0].idx < 8 ? 0x48c1 : 0x49c1, 5, 0b11101), BTreg(0, 3, In | Out), BTimm(1, 8) });
                    else if (index[0].tag == Arg::ISPILLED)
                        return BiT({ BTsta(0x48c1ac24, 32), BTspl(0, 32), BTimm(1, 8) });
                }
                else if (index[1].tag == Arg::IREG && index[1].idx == RCX)
                {
                    if (index[0].tag == Arg::IREG)
                        return BiT({ nBkb(2, index[0].idx < 8 ? 0x48d3 : 0x49d3, 5, 0b11101), BTreg(0, 3, In | Out), BTreg(1, 0, In) });
                    else if (index[0].tag == Arg::ISPILLED)
                        return BiT({ BTsta(0x48d3ac24, 32), BTspl(0, 32), BTreg(1, 0, In) });
                }
            }
            break;
        case (INTEL64_SAR):
            if (index.size() == 2)
            {
                if (index[1].tag == Arg::IIMMEDIATE)
                {
                    if (index[0].tag == Arg::IREG)
                        return BiT({ nBkb(2, index[0].idx < 8 ? 0x48c1 : 0x49c1, 5, 0b11111), BTreg(0, 3, In | Out), BTimm(1, 8) });
                    else if (index[0].tag == Arg::ISPILLED)
                        return BiT({ BTsta(0x48c1bc24, 32), BTspl(0, 32), BTimm(1, 8) });
                }
                else if (index[1].tag == Arg::IREG && index[1].idx == RCX)
                {
                    if (index[0].tag == Arg::IREG)
                        return BiT({ nBkb(2, index[0].idx < 8 ? 0x48d3 : 0x49d3, 5, 0b11111), BTreg(0, 3, In | Out), BTreg(1, 0, In) });
                    else if (index[0].tag == Arg::ISPILLED)
                        return BiT({ BTsta(0x48d3bc24, 32), BTspl(0, 32), BTreg(1, 0, In) });
                }
            }
            break;
        case (INTEL64_AND):
            if (index.size() == 2 && index[0].tag == Arg::IREG)
            {
                if (index[1].tag == Arg::IREG)
                {
                    static BinTranslation::Token stats[4] = { nBkb(2, 0x4821, 2, 0b11), nBkb(2, 0x4921, 2, 0b11), nBkb(2, 0x4c21, 2, 0b11), nBkb(2, 0x4d21, 2, 0b11) };
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    return BiT({ stats[statn], BTreg(1, 3, In), BTreg(0, 3, IO) });
                }
                else if (index[1].tag == Arg::ISPILLED)
                    return BiT({ (index[0].idx < 8) ? nBkb(2, 0x4823, 2, 0b10) : nBkb(2, 0x4c23, 2, 0b10), BTreg(0, 3, In), BTsta(0x424, 11), BTspl(1, 32) });
                else if (index[1].tag == Arg::IIMMEDIATE)
                {
                    if (index[0].idx == 0)
                        return BiT({ BTsta(0b100100, 7), BTreg(0, 3, Out), BTsta(0b100101, 6), BTimm(1, 32) });
                    else
                        return BiT({ (index[0].idx < 8) ? nBkb(2, 0x4881, 5, 0b11100) : nBkb(2, 0x4981, 5, 0b11100), BTreg(0, 3, Out), BTimm(1, 32) });
                }
            }
            else if (index.size() == 2 && index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ index[1].idx < 8 ? nBkb(2, 0x4821, 2, 0b10) : nBkb(2, 0x4c21, 2, 0b10), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 32) });
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x4881a424, 32), BTspl(0, 32), BTimm(1, 32) });
            }
            break;
        case (INTEL64_OR):
            if (index.size() == 2 && index[0].tag == Arg::IREG)
            {
                if (index[1].tag == Arg::IREG)
                {
                    static BinTranslation::Token stats[4] = { nBkb(2, 0x4809, 2, 0b11), nBkb(2, 0x4909, 2, 0b11), nBkb(2, 0x4c09, 2, 0b11), nBkb(2, 0x4d09, 2, 0b11) };
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    return BiT({ stats[statn], BTreg(1, 3, In), BTreg(0, 3, IO) });
                }
                else if (index[1].tag == Arg::ISPILLED)
                    return BiT({ (index[0].idx < 8) ? nBkb(2, 0x480b, 2, 0b10) : nBkb(2, 0x4c0b, 2, 0b10), BTreg(0, 3, In), BTsta(0x424, 11), BTspl(1, 32) });
                else if (index[1].tag == Arg::IIMMEDIATE)
                {
                    if (index[0].idx == 0)
                        return BiT({ nBkb(1, 0x48, 1, 0), BTreg(0, 3, Out), BTsta(0xD, 4), BTimm(1, 32) });
                    else
                        return BiT({ (index[0].idx < 8) ? nBkb(2, 0x4881, 5, 0b11001) : nBkb(2, 0x4981, 5, 0b11001), BTreg(0, 3, Out), BTimm(1, 32) });
                }
            }
            else if (index.size() == 2 && index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ index[1].idx < 8 ? nBkb(2, 0x4809, 2, 0b10) : nBkb(2, 0x4c09, 2, 0b10), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 32) });
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x48818c24, 32), BTspl(0, 32), BTimm(1, 32) });
            }
            break;
        case (INTEL64_XOR):
            if (index.size() == 2 && index[0].tag == Arg::IREG)
            {
                if (index[1].tag == Arg::IREG)
                {
                    static BinTranslation::Token stats[4] = { nBkb(2, 0x4831, 2, 0b11), nBkb(2, 0x4931, 2, 0b11), nBkb(2, 0x4c31, 2, 0b11), nBkb(2, 0x4d31, 2, 0b11) };
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    return BiT({ stats[statn], BTreg(1, 3, In), BTreg(0, 3, IO)});
                }
                else if (index[1].tag == Arg::ISPILLED)
                    return BiT({ (index[0].idx < 8) ? nBkb(2, 0x4833, 2, 0b10) : nBkb(2, 0x4c33, 2, 0b10), BTreg(0, 3, In), BTsta(0x424, 11), BTspl(1, 32) });
                else if (index[1].tag == Arg::IIMMEDIATE)
                {
                    if (index[0].idx == 0)
                        return BiT({ BTsta(0b100100, 7), BTreg(0, 3, Out), BTsta(0b110101, 6), BTimm(1, 32) });
                    else 
                        return BiT({ (index[0].idx < 8) ? nBkb(2, 0x4881, 5, 0b11110) : nBkb(2, 0x4981, 5, 0b11110), BTreg(0, 3, Out), BTimm(1, 32) });
                }
            }
            else if (index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ index[1].idx < 8 ? nBkb(2, 0x4831, 2, 0b10) : nBkb(2, 0x4c31, 2, 0b10), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 32) });
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x4881b424, 32), BTspl(0, 32), BTimm(1, 32) });
            }
            break;
        case (INTEL64_NOT):
            if (index.size() == 1 && index[0].tag == Arg::IREG)
                return BiT({ (index[0].idx < 8) ? nBkb(2, 0x48f7, 5, 0b11010) : nBkb(2, 0x49f7, 5, 0b11010), BTreg(0, 3, In | Out) });
            else if(index.size() == 1 && index[0].tag == Arg::ISPILLED)
                return BiT({ BTsta(0x48f79424, 32), BTspl(0, 32) });
            break;
        case (INTEL64_NEG):
            if (index.size() == 1 && index[0].tag == Arg::IREG)
                return BiT({ BTsta((index[0].idx < 8) ? 0x91EFB : 0x93EFB, 21), BTreg(0, 3, In | Out) });
            else if(index.size() == 1 && index[0].tag == Arg::ISPILLED)
                return BiT({ BTsta(0x48f79c24, 32), BTspl(0, 32) });
            break;
        case (INTEL64_CMP):
            if (index.size() == 2 && index[0].tag == Arg::IREG)
            {
                if (index[1].tag == Arg::IREG)
                {
                    static uint64_t stats[4] = { 0x120E7, 0x124E7, 0x130E7, 0x134E7 };
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    return BiT({ BTsta(stats[statn], 18), BTreg(1, 3, In), BTreg(0, 3, In) });
                }
                else if (index[1].tag == Arg::ISPILLED)
                    return BiT({ BTsta((index[0].idx < 8) ? 0x120EE : 0x130EE, 18), BTreg(0, 3, In), BTsta(0x424, 11), BTspl(1, 32) });
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ nBkb(2, index[0].idx < 8 ? 0x4883 : 0x4983, 5, 0b11111), BTreg(0, 3, In), BTimm(1, 8) });
            }
            else if (index.size() == 2 && index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ BTsta(index[1].idx < 8 ? 0x120E6 : 0x130E6, 18), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 32) });
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x4881bc24, 32), BTspl(0, 32), BTimm(1, 32) });
            }
            break;
        case (INTEL64_CMOVE ):
        case (INTEL64_CMOVNE):
        case (INTEL64_CMOVL ):
        case (INTEL64_CMOVG ):
        case (INTEL64_CMOVLE):
        case (INTEL64_CMOVGE):
        case (INTEL64_CMOVS):
        case (INTEL64_CMOVNS):
            if (index.size() == 2 && index[0].tag == Arg::IREG)
            {
                static uint64_t regbytes[4] = { 0x480f00, 0x4c0f00, 0x490f00, 0x4d0f00 };
                uint64_t stat = index.opcode == INTEL64_CMOVE  ? 0x44 : (
                                index.opcode == INTEL64_CMOVNE ? 0x45 : (
                                index.opcode == INTEL64_CMOVL  ? 0x4c : (
                                index.opcode == INTEL64_CMOVG  ? 0x4f : (
                                index.opcode == INTEL64_CMOVLE ? 0x4e : (
                                index.opcode == INTEL64_CMOVGE ? 0x4d : (
                                index.opcode == INTEL64_CMOVS  ? 0x48 : (
                              /*index.opcode == INTEL64_CMOVNS?*/0x49 /*: (*/)))))));
                if (index[1].tag == Arg::IREG)
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    stat |= regbytes[statn];
                    return BiT({ nBkb(3, stat, 2, 0b11), BTreg(0, 3, IO), BTreg(1, 3, In) });
                }
                else if (index[1].tag == Arg::ISPILLED)
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1);
                    stat |= regbytes[statn];
                    return BiT({ nBkb(3, stat, 2, 0b10), BTreg(0, 3, IO), BTsta(0x424, 11), BTspl(1, 32) });
                }
            }
            break;
        case (INTEL64_SETE ):
        case (INTEL64_SETNE):
        case (INTEL64_SETL ):
        case (INTEL64_SETG ):
        case (INTEL64_SETLE):
        case (INTEL64_SETGE):
        case (INTEL64_SETS):
        case (INTEL64_SETNS):
            if (index.size() == 1)
            {
                static uint64_t regbytes[4] = { 0x0f00, 0x400f00, 0x410f00 };
                uint64_t stat = index.opcode == INTEL64_SETE  ? 0x94 : (
                                index.opcode == INTEL64_SETNE ? 0x95 : (
                                index.opcode == INTEL64_SETL  ? 0x9c : (
                                index.opcode == INTEL64_SETG  ? 0x9f : (
                                index.opcode == INTEL64_SETLE ? 0x9e : (
                                index.opcode == INTEL64_SETGE ? 0x9d : (
                                index.opcode == INTEL64_SETS  ? 0x98 : (
                              /*index.opcode == INTEL64_SETNS?*/0x99 /*: (*/)))))));
                if (index[0].tag == Arg::IREG)
                {
                    size_t statn = index[0].idx < 4 ? 0 : (index[0].idx < 8 ? 1 : 2);
                    stat |= regbytes[statn];
                    int n = index[0].idx < 4 ? 2 : 3;
                    return BiT({ nBkb(n, stat, 5, 0b11000), BTreg(0, 3, Out) });
                }
                else if (index[0].tag == Arg::ISPILLED)
                    return BiT({ BTsta(0x0F948424, 32),  BTspl(0, 32) });
            }
            break;
        case (INTEL64_CQO): return BiT({ BTsta(0x4899, 16) });
        case (INTEL64_XCHG):
        {
            if (index.size() != 2)
                break;
            const bool rax0 = (index[0].tag == Arg::IREG && index[0].idx == RAX);
            const bool rax1 = (index[0].tag == Arg::IREG && index[0].idx == RAX);
            int rrd0 = rax0 ? 0 : 1;
            int rrd1 = rax0 ? 1 : 0;
            if (index[0].tag == Arg::IREG && index[1].tag == Arg::IREG)
            {
                if (!rax0 && !rax1)
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    static uint64_t statB[4] = { 0x4887, 0x4987, 0x4c87, 0x4d87 };
                    return BiT({ nBkb(2, statB[statn], 2, 0b11), BTreg(1, 3, IO), BTreg(0, 3, IO) });
                }
                else
                    return BiT({ nBkb(1, index[rrd1].idx < 8 ? 0x48 : 0x49, 1, 1), BTreg(rrd0, 2, IO), BTsta(0b10,2), BTreg(rrd1, 3, IO) });
            }
            else if ((index[0].tag == Arg::ISPILLED && index[1].tag == Arg::IREG) || (index[1].tag == Arg::ISPILLED && index[0].tag == Arg::IREG))
            {
                rrd0 = index[0].tag == Arg::ISPILLED ? 1 : 0;
                rrd1 = index[0].tag == Arg::ISPILLED ? 0 : 1;
                return BiT({ nBkb(2, index[rrd0].idx < 8 ? 0x4887 : 0x4c87, 2, 0b10), BTreg(rrd0, 3, IO), BTsta(0x424,11), BTspl(rrd1, 32) });
            }
            break;
        }
        case (INTEL64_VMOVDQU):
            if (index.args_size == 2 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[0].elemtype == index.args[1].elemtype)
            {
                size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                static uint64_t statB[4] = { 0xc5fe6f, 0xc57e6f, 0xc57e7f, 0xc4417e6f }; 
                static uint64_t statW[4] = { 3, 3, 3, 4 }; 
                if(statn == 2)
                    return BiT({ nBkb(statW[statn], statB[statn], 2, 0b11), BTreg(1, 3, In), BTreg(0, 3, Out) });
                else
                    return BiT({ nBkb(statW[statn], statB[statn], 2, 0b11), BTreg(0, 3, Out), BTreg(1, 3, In) });
            }
            else if (index.args_size > 1)
                return VEX_instuction(index, scs, 0xF3, 0x0F, index.args[0].tag == Arg::VREG ? 0x6F : 0x7F);
            break;
        case (INTEL64_VPADDB):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xFC, 0,   bm64({TYPE_U8, TYPE_I8}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPADDW):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xFD, 0, bm64({TYPE_U16, TYPE_I16}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPADDD):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xFE, 0, bm64({TYPE_U32, TYPE_I32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPADDQ):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xD4, 0, bm64({TYPE_U64, TYPE_I64}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VADDPS):      return VEX_instuction_V(index, scs,    0,   0x0F, 0, 0x58, 0,          bm64({TYPE_FP32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VADDPD):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x58, 0,          bm64({TYPE_FP64}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPSUBB):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xF8, 0,   bm64({TYPE_U8, TYPE_I8}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPSUBW):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xF9, 0, bm64({TYPE_U16, TYPE_I16}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPSUBD):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xFA, 0, bm64({TYPE_U32, TYPE_I32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPSUBQ):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xFB, 0, bm64({TYPE_U64, TYPE_I64}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VSUBPS):      return VEX_instuction_V(index, scs,    0,   0x0F, 0, 0x5C, 0,          bm64({TYPE_FP32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VSUBPD):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x5C, 0,          bm64({TYPE_FP64}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMULLW):     return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xD5, 0, bm64({TYPE_U16, TYPE_I16}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMULLD):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x40, 0, bm64({TYPE_U32, TYPE_I32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VMULPS):      return VEX_instuction_V(index, scs,    0,   0x0F, 0, 0x59, 0,          bm64({TYPE_FP32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VMULPD):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x59, 0,          bm64({TYPE_FP64}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VFMADD231PS): return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0xB8, 0,          bm64({TYPE_FP32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VFMADD231PD): return VEX_instuction_V(index, scs, 0x66, 0x0F38, 1, 0xB8, 0,          bm64({TYPE_FP64}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMINUB):     return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xDA, 0,            bm64({TYPE_U8}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMINSB):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x38, 0,            bm64({TYPE_I8}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMINUW):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x3A, 0,           bm64({TYPE_U16}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMINSW):     return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xEA, 0,           bm64({TYPE_I16}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMINUD):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x3B, 0,           bm64({TYPE_U32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMINSD):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x39, 0,           bm64({TYPE_I32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VMINPS):      return VEX_instuction_V(index, scs,    0,   0x0F, 0, 0x5D, 0,          bm64({TYPE_FP32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VMINPD):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x5D, 0,          bm64({TYPE_FP64}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMAXUB):     return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xDE, 0,            bm64({TYPE_U8}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMAXSB):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x3C, 0,            bm64({TYPE_I8}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMAXUW):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x3E, 0,           bm64({TYPE_U16}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMAXSW):     return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xEE, 0,           bm64({TYPE_I16}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMAXUD):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x3F, 0,           bm64({TYPE_U32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPMAXSD):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x3D, 0,           bm64({TYPE_I32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VMAXPS):      return VEX_instuction_V(index, scs,    0,   0x0F, 0, 0x5F, 0,          bm64({TYPE_FP32}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VMAXPD):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x5F, 0,          bm64({TYPE_FP64}),     bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPCMPEQB):    return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x74, 0,            bm64({TYPE_U8}),   bm64({TYPE_I8, TYPE_U8}), bm64({TYPE_SAME_AS_1}));
        case (INTEL64_VPCMPEQW):    return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x75, 0,           bm64({TYPE_U16}), bm64({TYPE_I16, TYPE_U16}), bm64({TYPE_SAME_AS_1}));
        case (INTEL64_VPCMPEQD):    return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x76, 0,           bm64({TYPE_U32}), bm64({TYPE_I32, TYPE_U32}), bm64({TYPE_SAME_AS_1}));
        case (INTEL64_VPCMPEQQ):    return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x29, 0,           bm64({TYPE_U64}), bm64({TYPE_I64, TYPE_U64}), bm64({TYPE_SAME_AS_1}));
        case (INTEL64_VPCMPGTB):    return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x64, 0,            bm64({TYPE_U8}),            bm64({TYPE_I8}),        bm64({TYPE_I8}));
        case (INTEL64_VPCMPGTW):    return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x65, 0,           bm64({TYPE_U16}),           bm64({TYPE_I16}),       bm64({TYPE_I16}));
        case (INTEL64_VPCMPGTD):    return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x66, 0,           bm64({TYPE_U32}),           bm64({TYPE_I32}),       bm64({TYPE_I32}));
        case (INTEL64_VPCMPGTQ):    return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x37, 0,           bm64({TYPE_U64}),           bm64({TYPE_I64}),       bm64({TYPE_I64}));
        case (INTEL64_VCMPEQPS):    return VEX_instuction_V(index, scs,    0,   0x0F, 0, 0xC2, 0,           bm64({TYPE_U32}),          bm64({TYPE_FP32}),      bm64({TYPE_FP32}), true, 0);
        case (INTEL64_VCMPNEQPS):   return VEX_instuction_V(index, scs,    0,   0x0F, 0, 0xC2, 0,           bm64({TYPE_U32}),          bm64({TYPE_FP32}),      bm64({TYPE_FP32}), true, 4);
        case (INTEL64_VCMPLTPS):    return VEX_instuction_V(index, scs,    0,   0x0F, 0, 0xC2, 0,           bm64({TYPE_U32}),          bm64({TYPE_FP32}),      bm64({TYPE_FP32}), true, 1);
        case (INTEL64_VCMPLEPS):    return VEX_instuction_V(index, scs,    0,   0x0F, 0, 0xC2, 0,           bm64({TYPE_U32}),          bm64({TYPE_FP32}),      bm64({TYPE_FP32}), true, 2);
        case (INTEL64_VCMPEQPD):    return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xC2, 0,           bm64({TYPE_U64}),          bm64({TYPE_FP64}),      bm64({TYPE_FP64}), true, 0);
        case (INTEL64_VCMPNEQPD):   return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xC2, 0,           bm64({TYPE_U64}),          bm64({TYPE_FP64}),      bm64({TYPE_FP64}), true, 4);
        case (INTEL64_VCMPLTPD):    return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xC2, 0,           bm64({TYPE_U64}),          bm64({TYPE_FP64}),      bm64({TYPE_FP64}), true, 1);
        case (INTEL64_VCMPLEPD):    return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xC2, 0,           bm64({TYPE_U64}),          bm64({TYPE_FP64}),      bm64({TYPE_FP64}), true, 2);
        
        case (INTEL64_VPAND): if(index.args_size == 3 && index.args[0].elemtype == index.args[1].elemtype && 
                                 elem_size(index.args[0].elemtype) == elem_size(index.args[2].elemtype) &&
                                 isUnsignedInteger(index.args[2].elemtype))
                                    return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xDB, 0);
            break;
        case (INTEL64_VPOR): if(index.args_size == 3 && index.args[0].elemtype == index.args[1].elemtype && 
                                 elem_size(index.args[0].elemtype) == elem_size(index.args[2].elemtype) &&
                                 isUnsignedInteger(index.args[2].elemtype))
                                    return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xEB, 0);
            break;
        case (INTEL64_VPXOR): if(index.args_size == 3 && index.args[0].elemtype == index.args[1].elemtype && 
                                 elem_size(index.args[0].elemtype) == elem_size(index.args[2].elemtype) &&
                                 isUnsignedInteger(index.args[2].elemtype))
                                    return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0xEF, 0);
            break;
        case (INTEL64_VPSLLW):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x71, 6, bm64({TYPE_U16, TYPE_I16}), bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPSLLD):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x72, 6, bm64({TYPE_U32, TYPE_I32}), bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPSLLQ):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x73, 6, bm64({TYPE_U64, TYPE_I64}), bm64({TYPE_SAME_AS_0}), bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPSLLVD):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x47, 0, bm64({TYPE_U32, TYPE_I32}), bm64({TYPE_SAME_AS_0}), bm64({TYPE_U32}));
        case (INTEL64_VPSLLVQ):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 1, 0x47, 0, bm64({TYPE_U64, TYPE_I64}), bm64({TYPE_SAME_AS_0}), bm64({TYPE_U64}));
        case (INTEL64_VPSRAW):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x71, 4,           bm64({TYPE_I16}),       bm64({TYPE_I16}), bm64({TYPE_U16}));
        case (INTEL64_VPSRAD):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x72, 4,           bm64({TYPE_I32}),       bm64({TYPE_I32}), bm64({TYPE_U32}));
        case (INTEL64_VPSRAVD):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x46, 0,           bm64({TYPE_I32}),       bm64({TYPE_I32}), bm64({TYPE_U32}));
        case (INTEL64_VPSRLW):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x71, 2, bm64({TYPE_U16, TYPE_I16}), bm64({TYPE_SAME_AS_0}), bm64({TYPE_U16}));
        case (INTEL64_VPSRLD):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x72, 2, bm64({TYPE_U32, TYPE_I32}), bm64({TYPE_SAME_AS_0}), bm64({TYPE_U32}));
        case (INTEL64_VPSRLQ):      return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x73, 2, bm64({TYPE_U64, TYPE_I64}), bm64({TYPE_SAME_AS_0}), bm64({TYPE_U64}));
        case (INTEL64_VPSRLVD):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x45, 0, bm64({TYPE_U32, TYPE_I32}), bm64({TYPE_SAME_AS_0}), bm64({TYPE_U32}));
        case (INTEL64_VPSRLVQ):     return VEX_instuction_V(index, scs, 0x66, 0x0F38, 1, 0x45, 0, bm64({TYPE_U64, TYPE_I64}), bm64({TYPE_SAME_AS_0}), bm64({TYPE_U64}));
        case (INTEL64_VROUNDPS):    return VEX_instuction_V(index, scs, 0x66, 0x0F3A, 0, 0x08, 0,          bm64({TYPE_FP32}));
        case (INTEL64_VROUNDPD):    return VEX_instuction_V(index, scs, 0x66, 0x0F3A, 0, 0x09, 0,          bm64({TYPE_FP64}));
        case (INTEL64_VCVTPS2DQ):   return VEX_instuction_V(index, scs, 0x66,   0x0F, 0, 0x5B, 0,           bm64({TYPE_I32}),      bm64({TYPE_FP32}));
        case (INTEL64_VCVTPD2DQ):   return VEX_instuction_V(index, scs, 0xF2,   0x0F, 0, 0xE6, 0,           bm64({TYPE_I32}),      bm64({TYPE_FP64})); //DUBUG: have to fix printer from "vcvtpd2dq ymm0, ymm0" to "vcvtpd2dq xmm0, ymm0"
        case (INTEL64_VCVTDQ2PS):   return VEX_instuction_V(index, scs,    0,   0x0F, 0, 0x5B, 0,          bm64({TYPE_FP32}),       bm64({TYPE_I32})); 
        case (INTEL64_VPMOVSXDQ):   return VEX_instuction_V(index, scs, 0x66, 0x0F38, 0, 0x25, 0,           bm64({TYPE_I64}),       bm64({TYPE_I32})); //DUBUG: have to fix printer from "vpmovsxdq ymm0, ymm0" to "vpmovsxdq ymm0, xmm0"

        case (INTEL64_JMP): return BiT({ BTsta(0xE9,8), BTimm(0, 32, Lab) });
        case (INTEL64_JNE): return BiT({ BTsta(0xf85,16), BTimm(0, 32, Lab) });
        case (INTEL64_JE):  return BiT({ BTsta(0xf84,16), BTimm(0, 32, Lab) });
        case (INTEL64_JL):  return BiT({ BTsta(0xf8c,16), BTimm(0, 32, Lab) });
        case (INTEL64_JLE): return BiT({ BTsta(0xf8e,16), BTimm(0, 32, Lab) });
        case (INTEL64_JG):  return BiT({ BTsta(0xf8f,16), BTimm(0, 32, Lab) });
        case (INTEL64_JGE): return BiT({ BTsta(0xf8d,16), BTimm(0, 32, Lab) });
        case (INTEL64_CALL):
            if (index.size() == 1)
            {
                if (index[0].tag == Arg::IREG)
                {
                    int n = index[0].idx < 8 ? 1 : 2;
                    size_t stat = index[0].idx < 8 ? 0xff : 0x41ff;
                    return BiT({ nBkb(n, stat, 5, 0b11010), BTreg(0, 3, In) });
                }
                else if (index[0].tag == Arg::ISPILLED)
                    return BiT({ BTsta(0xff9424, 24),  BTspl(0, 32) });
            }
            break;
        case (INTEL64_RET): return BiT({ BTsta(0xC3, 8) });
        case (INTEL64_LABEL): return BiT({});
        default:
            break;
        }
        scs = false;
        return BinTranslation();
    }

    enum
    {
        ROUND_NEAREST = 0b00,
        ROUND_DOWN    = 0b01,
        ROUND_UP      = 0b10,
        ROUND_TOZERO  = 0b11,
    };

    SyntopTranslation i64STLookup(const Backend* /*backend*/, const Syntop& index, bool& scs)
    {
        using namespace SyntopTranslationConstruction;
        scs = true;
        switch (index.opcode)
        {
        case(OP_LOAD):
            if (index.size() == 2)
            {
                switch (index[0].elemtype)
                {
                case (TYPE_I8):
                case (TYPE_I16): return SyT(INTEL64_MOVSX, { SAcop(0), SAcop(1) });
                case (TYPE_U8):
                case (TYPE_U16): return SyT(INTEL64_MOVZX, { SAcop(0), SAcop(1) });
                case (TYPE_I32): return SyT(INTEL64_MOVSXD,{ SAcop(0), SAcop(1) });
                case (TYPE_U32): case (TYPE_I64): case (TYPE_U64):
                case (TYPE_FP32): case (TYPE_FP64):
                    return SyT(INTEL64_MOV, { SAcop(0), SAcop(1, AF_ADDRESS) });
                default: break;
                }
            }
            else if (index.size() == 3)
            {
                switch (index[0].elemtype)
                {
                case (TYPE_I8):
                case (TYPE_I16): return SyT(INTEL64_MOVSX, { SAcop(0), SAcop(1), SAcop(2) });
                case (TYPE_U8):
                case (TYPE_U16): return SyT(INTEL64_MOVZX, { SAcop(0), SAcop(1), SAcop(2) });
                case (TYPE_I32): return SyT(INTEL64_MOVSXD, { SAcop(0), SAcop(1), SAcop(2) });
                case (TYPE_U32): case (TYPE_I64): case (TYPE_U64):
                case (TYPE_FP32): case (TYPE_FP64):
                    return SyT(INTEL64_MOV, { SAcop(0), SAcop(1, AF_ADDRESS), SAcop(2, AF_ADDRESS) });
                default: break;
                }
            }
            break;
        case (OP_STORE):
            if (index.size() == 2)
                return SyT(INTEL64_MOV, { SAcop(0, AF_ADDRESS), SAcop(1) });
            else if (index.size() == 3)
                return SyT(INTEL64_MOV, { SAcop(0, AF_ADDRESS), SAcop(1, AF_ADDRESS), SAcop(2) });
            break;
        case (OP_MOV):
            if(index.size() == 2)
            {
                //TODO(ch): This trick (mov ax, 0 -> xor ax, ax) must be done in different place and, obviously not in architecture-dependent code.
                if(index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[1].value == 0) 
                    return SyT(INTEL64_XOR,  { SAcop(0), SAcop(0) });
                else if(index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype)
                    return SyT(INTEL64_VMOVDQU,  { SAcop(0), SAcop(1) }); //DUBUG: check if this is correct approach for vector mov!
                else
                    return SyT(INTEL64_MOV,  { SAcop(0), SAcop(1) });
            }
            break;
        case (OP_XCHG):    return SyT(INTEL64_XCHG, { SAcop(0), SAcop(1) });   //TODO(ch): It's very recommended to don't use this instruction (xchg reg, mem variation). See "Instruction tables" by Agner fog.
        case (OP_X86_ADC):     return SyT(INTEL64_ADC,  { SAcop(0), SAcop(2) });
        case (OP_ADD):     return SyT(INTEL64_ADD,  { SAcop(0), SAcop(2) });
        case (OP_SUB):     return SyT(INTEL64_SUB,  { SAcop(0), SAcop(2) });
        case (OP_MUL):     return SyT(INTEL64_IMUL, { SAcop(0), SAcop(2) });
        case (OP_MOD):     
        case (OP_DIV):     return SyT(INTEL64_IDIV, { SAcop(2) });
        case (OP_SHL):     return SyT(INTEL64_SHL,  { SAcop(0), index[2].tag == Arg::IIMMEDIATE ? SAcop(2) : SAcopelt(2, TYPE_I8) });
        case (OP_SHR):     return SyT(INTEL64_SHR,  { SAcop(0), index[2].tag == Arg::IIMMEDIATE ? SAcop(2) : SAcopelt(2, TYPE_I8) });
        case (OP_SAR):     return SyT(INTEL64_SAR,  { SAcop(0), index[2].tag == Arg::IIMMEDIATE ? SAcop(2) : SAcopelt(2, TYPE_I8) });
        case (OP_AND):     return SyT(INTEL64_AND,  { SAcop(0), SAcop(2) });
        case (OP_OR):      return SyT(INTEL64_OR,   { SAcop(0), SAcop(2) });
        case (OP_XOR):     return SyT(INTEL64_XOR,  { SAcop(0), SAcop(2) });
        case (OP_NOT):     return SyT(INTEL64_NOT,  { SAcop(0) });
        case (OP_NEG):     return SyT(INTEL64_NEG,  { SAcop(0) });
        case (OP_X86_CQO): return SyT(INTEL64_CQO,  {});
        case (OP_CMP):     return SyT(INTEL64_CMP,  { SAcop(0), SAcop(1) });
        case (OP_SELECT): 
            if (index.size() == 4)
            {
                int tarcode = index[1].value == OP_NE ? INTEL64_CMOVNE : (
                              index[1].value == OP_EQ ? INTEL64_CMOVE : (
                              index[1].value == OP_GE ? INTEL64_CMOVGE : (
                              index[1].value == OP_LE ? INTEL64_CMOVLE : (
                              index[1].value == OP_GT ? INTEL64_CMOVG : (
                              index[1].value == OP_LT ? INTEL64_CMOVL : (
                              index[1].value == OP_S  ? INTEL64_CMOVS : (
                              index[1].value == OP_NS ? INTEL64_CMOVNS : -1)))))));
                Assert(tarcode != -1);
                return SyT(tarcode, { SAcop(0), SAcop(2) });
            }
            break;
        case (OP_IVERSON): 
            if (index.size() == 2 && index[1].value >= OP_GT && index[1].value <= OP_NS)
            {
                int tarcode = index[1].value == OP_NE ? INTEL64_SETNE : (
                              index[1].value == OP_EQ ? INTEL64_SETE : (
                              index[1].value == OP_GE ? INTEL64_SETGE : (
                              index[1].value == OP_LE ? INTEL64_SETLE : (
                              index[1].value == OP_GT ? INTEL64_SETG : (
                              index[1].value == OP_LT ? INTEL64_SETL : (
                              index[1].value == OP_S  ? INTEL64_SETS : (
                              index[1].value == OP_NS ? INTEL64_SETNS : -1)))))));
                Assert(tarcode != -1);
                return SyT(tarcode, { SAcopelt(0, TYPE_U8) });
            }
            break;
        case (VOP_LOAD):
            if (index.args_size == 2)
                return SyT(INTEL64_VMOVDQU, { SAcop(0), SAcop(1, AF_ADDRESS) });
            else if (index.args_size == 3)
                return SyT(INTEL64_VMOVDQU, { SAcop(0), SAcop(1, AF_ADDRESS), SAcop(2, AF_ADDRESS) });
            break;
        case (VOP_STORE):
            if (index.args_size == 2)
                return SyT(INTEL64_VMOVDQU, { SAcop(0, AF_ADDRESS), SAcop(1) });
            else if (index.args_size == 3)
                return SyT(INTEL64_VMOVDQU, { SAcop(0, AF_ADDRESS), SAcop(1, AF_ADDRESS), SAcop(2) });
            break;

        case (VOP_ADD):
            if (index.args_size == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG &&
                index.args[0].elemtype == index.args[1].elemtype && index.args[0].elemtype == index.args[2].elemtype)
            {
                switch(index.args[0].elemtype)
                {
                    case(TYPE_U8):  case(TYPE_I8):  return SyT(INTEL64_VPADDB, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_U16): case(TYPE_I16): return SyT(INTEL64_VPADDW, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_U32): case(TYPE_I32): return SyT(INTEL64_VPADDD, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_U64): case(TYPE_I64): return SyT(INTEL64_VPADDQ, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_FP32): return SyT(INTEL64_VADDPS, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_FP64): return SyT(INTEL64_VADDPD, { SAcop(0), SAcop(1), SAcop(2) });
                }
            }
            break;
        case (VOP_SUB):
        if (index.args_size == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG &&
            index.args[0].elemtype == index.args[1].elemtype && index.args[0].elemtype == index.args[2].elemtype)
        {
            switch(index.args[0].elemtype)
            {
                case(TYPE_U8):  case(TYPE_I8):  return SyT(INTEL64_VPSUBB, { SAcop(0), SAcop(1), SAcop(2) });
                case(TYPE_U16): case(TYPE_I16): return SyT(INTEL64_VPSUBW, { SAcop(0), SAcop(1), SAcop(2) });
                case(TYPE_U32): case(TYPE_I32): return SyT(INTEL64_VPSUBD, { SAcop(0), SAcop(1), SAcop(2) });
                case(TYPE_U64): case(TYPE_I64): return SyT(INTEL64_VPSUBQ, { SAcop(0), SAcop(1), SAcop(2) });
                case(TYPE_FP32): return SyT(INTEL64_VSUBPS, { SAcop(0), SAcop(1), SAcop(2) });
                case(TYPE_FP64): return SyT(INTEL64_VSUBPD, { SAcop(0), SAcop(1), SAcop(2) });
            }
        }
        break;
        case (VOP_MUL):
            if (index.args_size == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG &&
                index.args[0].elemtype == index.args[1].elemtype && index.args[0].elemtype == index.args[2].elemtype)
            {
                switch(index.args[0].elemtype)
                {
                    case(TYPE_U16): case(TYPE_I16): return SyT(INTEL64_VPMULLW, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_U32): case(TYPE_I32): return SyT(INTEL64_VPMULLD, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_FP32): return SyT(INTEL64_VMULPS, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_FP64): return SyT(INTEL64_VMULPD, { SAcop(0), SAcop(1), SAcop(2) });
                }
            }
            break;
        case (VOP_FMA):
            if (index.args_size == 4 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG &&
                index.args[3].tag == Arg::VREG && index.args[0].elemtype == index.args[1].elemtype && index.args[0].elemtype == index.args[2].elemtype &&
                index.args[0].elemtype == index.args[3].elemtype && index.args[0].idx == index.args[1].idx)
            {
                switch(index.args[0].elemtype)
                {
                    case(TYPE_FP32): return SyT(INTEL64_VFMADD231PS, { SAcop(1), SAcop(2), SAcop(3) });
                    case(TYPE_FP64): return SyT(INTEL64_VFMADD231PD, { SAcop(1), SAcop(2), SAcop(3) });
                }
            }
            break;
        case (VOP_MIN):
            if (index.args_size == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG &&
                index.args[0].elemtype == index.args[1].elemtype && index.args[0].elemtype == index.args[2].elemtype)
            {
                switch(index.args[0].elemtype)
                {
                    case(TYPE_U8):   return SyT(INTEL64_VPMINUB, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_I8):   return SyT(INTEL64_VPMINSB, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_U16):  return SyT(INTEL64_VPMINUW, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_I16):  return SyT(INTEL64_VPMINSW, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_U32):  return SyT(INTEL64_VPMINUD, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_I32):  return SyT(INTEL64_VPMINSD, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_FP32): return SyT(INTEL64_VMINPS , { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_FP64): return SyT(INTEL64_VMINPD , { SAcop(0), SAcop(1), SAcop(2) });
                }
            }
            break;
        case (VOP_MAX):
            if (index.args_size == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG &&
                index.args[0].elemtype == index.args[1].elemtype && index.args[0].elemtype == index.args[2].elemtype)
            {
                switch(index.args[0].elemtype)
                {
                    case(TYPE_U8):   return SyT(INTEL64_VPMAXUB, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_I8):   return SyT(INTEL64_VPMAXSB, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_U16):  return SyT(INTEL64_VPMAXUW, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_I16):  return SyT(INTEL64_VPMAXSW, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_U32):  return SyT(INTEL64_VPMAXUD, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_I32):  return SyT(INTEL64_VPMAXSD, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_FP32): return SyT(INTEL64_VMAXPS , { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_FP64): return SyT(INTEL64_VMAXPD , { SAcop(0), SAcop(1), SAcop(2) });
                }
            }
            break;
        case (VOP_EQ):
        if (index.args_size == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG &&
            index.args[1].elemtype == index.args[2].elemtype && elem_size(index.args[0].elemtype) == elem_size(index.args[1].elemtype) &&
            isUnsignedInteger(index.args[0].elemtype))
            {
                switch(index.args[1].elemtype)
                {
                    case(TYPE_U8):  case(TYPE_I8):  return SyT(INTEL64_VPCMPEQB, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_U16): case(TYPE_I16): return SyT(INTEL64_VPCMPEQW, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_U32): case(TYPE_I32): return SyT(INTEL64_VPCMPEQD, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_U64): case(TYPE_I64): return SyT(INTEL64_VPCMPEQQ, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_FP32):  return SyT(INTEL64_VCMPEQPS, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_FP64):  return SyT(INTEL64_VCMPEQPD, { SAcop(0), SAcop(1), SAcop(2) });
                }
            }
            break;
        case (VOP_NE):
        if (index.args_size == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG &&
            index.args[1].elemtype == index.args[2].elemtype && elem_size(index.args[0].elemtype) == elem_size(index.args[1].elemtype) &&
            isUnsignedInteger(index.args[0].elemtype))
            {
                switch(index.args[1].elemtype)
                {
                    case(TYPE_FP32):  return SyT(INTEL64_VCMPNEQPS, { SAcop(0), SAcop(1), SAcop(2) });
                    case(TYPE_FP64):  return SyT(INTEL64_VCMPNEQPD, { SAcop(0), SAcop(1), SAcop(2) });
                }
            }
            break;            
        case (VOP_LT):
        case (VOP_GT):
        if (index.args_size == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG &&
            index.args[1].elemtype == index.args[2].elemtype && elem_size(index.args[0].elemtype) == elem_size(index.args[1].elemtype) &&
            isUnsignedInteger(index.args[0].elemtype))
            {
                int first = (index.opcode == VOP_GT ? 1 : 2);
                int second = (index.opcode == VOP_GT ? 2 : 1);

                switch(index.args[1].elemtype)
                {
                    case(TYPE_I8):  return SyT(INTEL64_VPCMPGTB, { SAcop(0), SAcop(first), SAcop(second) });
                    case(TYPE_I16): return SyT(INTEL64_VPCMPGTW, { SAcop(0), SAcop(first), SAcop(second) });
                    case(TYPE_I32): return SyT(INTEL64_VPCMPGTD, { SAcop(0), SAcop(first), SAcop(second) });
                    case(TYPE_I64): return SyT(INTEL64_VPCMPGTQ, { SAcop(0), SAcop(first), SAcop(second) });
                    case(TYPE_FP32):  return SyT(INTEL64_VCMPLTPS, { SAcop(0), SAcop(second), SAcop(first) });
                    case(TYPE_FP64):  return SyT(INTEL64_VCMPLTPD, { SAcop(0), SAcop(second), SAcop(first) });
                }
            }
            break;
        case (VOP_LE):
        case (VOP_GE):
        if (index.args_size == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG &&
            index.args[1].elemtype == index.args[2].elemtype && elem_size(index.args[0].elemtype) == elem_size(index.args[1].elemtype) &&
            isUnsignedInteger(index.args[0].elemtype))
            {
                int first = (index.opcode == VOP_GE ? 2 : 1);
                int second = (index.opcode == VOP_GE ? 1 : 2);
                switch(index.args[1].elemtype)
                {
                    case(TYPE_FP32):  return SyT(INTEL64_VCMPLEPS, { SAcop(0), SAcop(first), SAcop(second) });
                    case(TYPE_FP64):  return SyT(INTEL64_VCMPLEPD, { SAcop(0), SAcop(first), SAcop(second) });
                }
            }
            break;
        case (VOP_AND):
        case (VOP_OR):
        case (VOP_XOR):
            if(index.args_size == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG  &&
                index.args[0].elemtype == index.args[1].elemtype && elem_size(index.args[0].elemtype) == elem_size(index.args[2].elemtype) &&
                isUnsignedInteger(index.args[2].elemtype))
            {
                int taropcode = index.opcode == VOP_AND ?   INTEL64_VPAND:
                                index.opcode == VOP_OR  ?   INTEL64_VPOR :
                                /*index.opcode == VOP_XOR?*/INTEL64_VPXOR;
                return SyT(taropcode, { SAcop(0), SAcop(1), SAcop(2) });
            }
            break;
        case (VOP_SAL):
        case (VOP_SHL):
            if (index.size() == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[0].elemtype == index.args[1].elemtype &&
                isInteger(index.args[1].elemtype))
            {
                int elsize = elem_size(index.args[0].elemtype);
                if(index.args[2].tag == Arg::VREG && isUnsignedInteger(index.args[2].elemtype) && elsize == elem_size(index.args[2].elemtype) && 
                  (elsize == 4 || elsize == 8))
                    return SyT(elsize == 4 ? INTEL64_VPSLLVD : INTEL64_VPSLLVQ, { SAcop(0), SAcop(1), SAcop(2) });
                else if(index.args[2].tag == Arg::IIMMEDIATE && (elsize == 2 || elsize == 4 || elsize == 8))
                    return SyT(elsize == 2 ? INTEL64_VPSLLW : (elsize == 4 ? INTEL64_VPSLLD : INTEL64_VPSLLQ), { SAcop(0), SAcop(1), SAcop(2) });
            }
            break;
        case (VOP_SAR):
            if (index.size() == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[0].elemtype == index.args[1].elemtype &&
                isInteger(index.args[1].elemtype))
            {
                int elsize = elem_size(index.args[0].elemtype);
                if(index.args[2].tag == Arg::VREG && isUnsignedInteger(index.args[2].elemtype) && elsize == elem_size(index.args[2].elemtype) && elsize == 4)
                    return SyT(index.args[0].elemtype == TYPE_U32 ? INTEL64_VPSRLVD : INTEL64_VPSRAVD, { SAcop(0), SAcop(1), SAcop(2) });
                else if(index.args[2].tag == Arg::IIMMEDIATE)
                {
                    int opcode = index.args[0].elemtype == TYPE_U16 ? INTEL64_VPSRLW :
                                 index.args[0].elemtype == TYPE_I16 ? INTEL64_VPSRAW :
                                 index.args[0].elemtype == TYPE_U32 ? INTEL64_VPSRLD :
                                 index.args[0].elemtype == TYPE_I32 ? INTEL64_VPSRAD :
                                 index.args[0].elemtype == TYPE_U64 ? INTEL64_VPSRLQ : -1;
                    if(opcode != -1) 
                        return SyT(opcode, { SAcop(0), SAcop(1), SAcop(2) });
                }
            }
            break;
        case (VOP_SHR):
            if (index.size() == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[0].elemtype == index.args[1].elemtype &&
                isInteger(index.args[1].elemtype))
            {
                int elsize = elem_size(index.args[0].elemtype);
                if(index.args[2].tag == Arg::VREG && isUnsignedInteger(index.args[2].elemtype) && elsize == elem_size(index.args[2].elemtype) && 
                  (elsize == 4 || elsize == 8))
                    return SyT(elsize == 4 ? INTEL64_VPSRLVD : INTEL64_VPSRLVQ, { SAcop(0), SAcop(1), SAcop(2) });
                else if(index.args[2].tag == Arg::IIMMEDIATE && (elsize == 2 || elsize == 4 || elsize == 8))
                    return SyT(elsize == 2 ? INTEL64_VPSRLW : (elsize == 4 ? INTEL64_VPSRLD : INTEL64_VPSRLQ), { SAcop(0), SAcop(1), SAcop(2) });
            }
            break;
        case (VOP_FLOOR):
        case (VOP_TRUNC):
            if(index.size() == 2 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[0].elemtype == index.args[1].elemtype)
            {
                switch(index.args[1].elemtype)
                {
                    case(TYPE_FP32): return SyT(INTEL64_VROUNDPS, { SAcop(0), SAcop(1), SAimm(index.opcode == VOP_FLOOR ? ROUND_DOWN : ROUND_TOZERO) });
                    case(TYPE_FP64): return SyT(INTEL64_VROUNDPD, { SAcop(0), SAcop(1), SAimm(index.opcode == VOP_FLOOR ? ROUND_DOWN : ROUND_TOZERO) });
                }
            }
            break;
        case (VOP_CAST):  
            if(index.size() == 2 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG)
            {
                if(index.args[0].elemtype == TYPE_I32 && index.args[1].elemtype == TYPE_FP32)
                    return SyT(INTEL64_VCVTPS2DQ, { SAcop(0), SAcop(1) });
                else if(index.args[0].elemtype == TYPE_I32 && index.args[1].elemtype == TYPE_FP64)
                    return SyT(INTEL64_VCVTPD2DQ, { SAcop(0), SAcop(1) });
                else if(index.args[0].elemtype == TYPE_FP32 && index.args[1].elemtype == TYPE_I32)
                    return SyT(INTEL64_VCVTDQ2PS, { SAcop(0), SAcop(1) });
            }
            break;
        case (VOP_CAST_LOW):  
            if(index.size() == 2 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG &&
               index.args[0].elemtype == TYPE_I64 && index.args[1].elemtype == TYPE_I32)
               return SyT(INTEL64_VPMOVSXDQ, { SAcop(0), SAcop(1) });
            {
                if(index.args[0].elemtype == TYPE_I32 && index.args[1].elemtype == TYPE_FP32)
                    return SyT(INTEL64_VCVTPS2DQ, { SAcop(0), SAcop(1) });
                else if(index.args[0].elemtype == TYPE_FP32 && index.args[1].elemtype == TYPE_I32)
                    return SyT(INTEL64_VCVTDQ2PS, { SAcop(0), SAcop(1) });
            }
            break;
//DUBUG: Implement V's list:
//VOP_DIV: vdivps, vdivpd (floats only??)
//VOP_CAST: vcvttps2dq and check all abilities!
//VOP_SELECT:
// def vpblendvb(rd, rs1, s2, rsmask):
// def vblendvps(rd, rs1, s2, rsmask):
// def vblendvpd(rd, rs1, s2, rsmask):
//VOP_BROADCAST: 
// def vbroadcast_(rd, opcode, s):
// def vpbroadcastb(rd, imm):
// def vpbroadcastw(rd, imm):
// def vpbroadcastd(rd, imm):
// def vpbroadcastq(rd, imm):
// def vbroadcastss(rd, imm):
// def vbroadcastsd(rd, imm):
// Something about loads and stores(you obviously need some instructions for expanding preloaded constants, probably, that's it!):
// def vmovups(d, s):
// def vmovupd(d, s):
// def vmovq(d, s):
//DUBUG: Check rest instructions from Arm's list.

        case (OP_UNSPILL): if(index.args_size == 2)
            {
                if(index[0].tag == Arg::IREG) 
                    return SyT(INTEL64_MOV, { SAcopelt(0, TYPE_I64), SAcopspl(1) });
                else if(index[0].tag == Arg::VREG) 
                    return SyT(INTEL64_VMOVDQU, { SAcopelt(0, TYPE_I64), SAreg(RSP, AF_ADDRESS), SAcopsar(1, -3, AF_ADDRESS) });
            }
            break;
        case (OP_SPILL): if(index.args_size == 2)
            {
                if(index[1].tag == Arg::IREG)
                    return SyT(INTEL64_MOV, { SAcopspl(0), SAcopelt(1, TYPE_I64) });
                else if(index[1].tag == Arg::VREG)
                    return SyT(INTEL64_VMOVDQU, { SAreg(RSP, AF_ADDRESS), SAcopsar(0, -3, AF_ADDRESS), SAcopelt(1, TYPE_I64) });
            }
            break;
        case (OP_JCC):
            if(index.size() == 2 && index[0].tag == Arg::IIMMEDIATE && index[1].tag == Arg::IIMMEDIATE)
            {
                switch (index[0].value)
                {
                case (OP_NE):  return SyT(INTEL64_JNE, { SAcop(1) });
                case (OP_EQ):  return SyT(INTEL64_JE,  { SAcop(1) });
                case (OP_LT):  return SyT(INTEL64_JL,  { SAcop(1) });
                case (OP_GT):  return SyT(INTEL64_JG,  { SAcop(1) });
                case (OP_GE):  return SyT(INTEL64_JGE, { SAcop(1) });
                case (OP_LE):  return SyT(INTEL64_JLE, { SAcop(1) });
                default:
                    break;
                };
            }
            break;
        case (OP_JMP):     return SyT(INTEL64_JMP, { SAcop(0) });
        case (OP_CALL_NORET):
            if(index.size() == 1 && (index[0].tag == Arg::IREG || index[0].tag == Arg::ISPILLED))
                return SyT(INTEL64_CALL, { SAcop(0) });
            break;
        case (OP_RET):     return SyT(INTEL64_RET, {});
        case (OP_LABEL):   return SyT(INTEL64_LABEL, { SAcop(0) });
        default:
            break;
        }
        scs = false;
        return SyntopTranslation();
    }

    class Intel64BRASnippets : public CompilerPass
    {
    public:
        virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override;
        virtual ~Intel64BRASnippets() override {}
        virtual bool is_inplace() const override final { return false; }
        virtual std::string pass_id() const override final { return "CP_INTEL64_BRA_SNIPPETS"; }
        static CompilerPassPtr make(const Backend* a_backend)
        {
            std::shared_ptr<Intel64BRASnippets> res;
            res.reset(new Intel64BRASnippets(a_backend));
            return std::static_pointer_cast<CompilerPass>(res);
        } 
    private: 
        Intel64BRASnippets(const Backend* a_backend) : CompilerPass(a_backend) {}
    };

    class Intel64ARASnippets : public CompilerPass
    {
    public:
        virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override;
        virtual ~Intel64ARASnippets() override {}
        virtual bool is_inplace() const override final { return false; }
        virtual std::string pass_id() const override final { return "CP_INTEL64_ARA_SNIPPETS"; }
        static CompilerPassPtr make(const Backend* a_backend)
        {
            std::shared_ptr<Intel64ARASnippets> res;
            res.reset(new Intel64ARASnippets(a_backend));
            return std::static_pointer_cast<CompilerPass>(res);
        } 
    private: 
        Intel64ARASnippets(const Backend* a_backend) : CompilerPass(a_backend) {}
    };

    Intel64Backend::Intel64Backend()
    {
        m_s2blookup = i64BTLookup;
        m_s2slookup = i64STLookup;
        m_vectorRegisterBits = 256; // AVX2???
        m_isLittleEndianInstructions = false;
        m_isLittleEndianOperands = true;
        m_isMonowidthInstruction = false;
        m_callerStackIncrement = 1;
        m_postInstructionOffset = true;
        m_registersAmount = 40;
        m_name = "Intel64";
        m_beforeRegAllocPasses.push_back(Intel64BRASnippets::make(this));
        m_afterRegAllocPasses.push_back(Intel64ARASnippets::make(this));
#if __LOOPS_OS == __LOOPS_WINDOWS
        m_parameterRegisters[RB_INT] = { RCX, RDX, R8, R9 };
        m_returnRegisters[RB_INT] = { RAX };
        m_callerSavedRegisters[RB_INT] = { R10, R11 };
        m_calleeSavedRegisters[RB_INT] = { RBX, RSI, RDI, R12, R13, R14, R15 };
#elif __LOOPS_OS == __LOOPS_LINUX || __LOOPS_OS == __LOOPS_MAC
        m_parameterRegisters[RB_INT] = { RDI, RSI, RDX, RCX, R8, R9 };
        m_returnRegisters[RB_INT] = { RAX, RDX };
        m_callerSavedRegisters[RB_INT] = { R10, R11 };
        m_calleeSavedRegisters[RB_INT] = { RBX, R12, R13, R14, R15 };
#else
#error Unknown OS
#endif

    m_returnRegisters[RB_VEC] = { YMM0 };
#if __LOOPS_OS == __LOOPS_WINDOWS
    m_parameterRegisters[RB_VEC] = { YMM0, YMM1, YMM2, YMM3 };
#error DUBUG Not Supported!
#elif __LOOPS_OS == __LOOPS_LINUX || __LOOPS_OS == __LOOPS_MAC
    m_parameterRegisters[RB_VEC] = { YMM0, YMM1, YMM2, YMM3, YMM4, YMM5, YMM6, YMM7};
    m_callerSavedRegisters[RB_VEC] = { YMM8, YMM9, YMM10, YMM11, YMM12, YMM13, YMM14, YMM15 };
    m_calleeSavedRegisters[RB_VEC] = {  };
#else
#error Unknown OS
#endif
    }

    Intel64Backend::~Intel64Backend()
    {}

    std::set<int> Intel64Backend::filterStackPlaceable(const Syntop& a_op, const std::set<int>& toFilter) const
    {
        switch (a_op.opcode)
        {
        case(OP_MOV):
            if(toFilter.size() == 1 && a_op.size() == 2 && a_op[0].tag == Arg::IREG //This restriction is imm64 support
                && a_op[1].tag == Arg::IIMMEDIATE && (a_op[1].value & uint64_t(0xFFFFFFFF00000000)) != 0)
                return std::set<int>({});
            else if(toFilter.size() >= 1 && (a_op.args[*toFilter.begin()].tag == Arg::VREG || 
                    (toFilter.size() == 2 && a_op.args[*(++toFilter.begin())].tag == Arg::VREG))) 
                return std::set<int>({});            
            else 
                return (toFilter.size() < 2) ? toFilter : std::set<int>({ 1 });
        case(OP_AND):
        case(OP_OR):
        case(OP_XOR):
        case(OP_X86_ADC):
        case(OP_ADD):
        case(OP_SUB):
        case(OP_SHL):
        case(OP_SHR):
        case(OP_SAR):
        {
            Assert(a_op.size() == 3 && a_op[0].tag == Arg::IREG);
            std::set<int> res = toFilter;
            res.erase(1);
            res = (res.size() < 2) ? res : std::set<int>({ 0 });
            if (a_op[1].tag == Arg::IREG && a_op[0].idx == a_op[1].idx && (res.count(0) || res.count(1)))
            {
                res.insert(0);
                res.insert(1);
            }
            return res;
        }
        case(OP_DIV):
        case(OP_MOD):
        case(OP_MUL):
            return (toFilter.count(2)) ? std::set<int>({ 2 }) : std::set<int>({ });
        case(OP_NEG):
        case(OP_NOT):
        {
            Assert(a_op.size() == 2 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IREG);
            if (toFilter.size() == 2 && a_op[0].idx != a_op[1].idx) //{0,1}
                return std::set<int>({ 1 });
            else 
                return toFilter;
        }
        case(OP_CMP): return (toFilter.size() < 2) ? toFilter : std::set<int>({ 0 });
        case(OP_LOAD):
        case(OP_STORE): return std::set<int>();
            break;
        case(OP_SELECT):
            Assert(a_op.size() == 4);
            return (toFilter.count(2) && !regOrSpiEq(a_op[0], a_op[2])) ? std::set<int>({2}) : std::set<int>({});
            break;
        case(OP_IVERSON):
            Assert(a_op.size() == 2);
            return (toFilter.count(0) ? std::set<int>({0}) : std::set<int>({}));
            break;
        case(OP_ABS):
            Assert(a_op.size() == 2);
            return (toFilter.count(1) && !regOrSpiEq(a_op[0], a_op[1])) ? std::set<int>({ 1 }) : std::set<int>({});
            break;
        case(OP_CALL):
        case(OP_CALL_NORET):
            return std::set<int>({});
        default:
            break;
        }
        return Backend::filterStackPlaceable(a_op, toFilter);
    }

    int Intel64Backend::reusingPreferences(const Syntop& a_op, const std::set<int>& undefinedArgNums) const
    {
        switch (a_op.opcode)
        {
        case OP_X86_ADC:
        case OP_ADD:
        case OP_MUL:
        case OP_AND:
        case OP_OR:
        case OP_XOR:
        case OP_MIN:
        case OP_MAX:
        {
            if (undefinedArgNums.count(1)) //TODO(ch): Hmmm... looks like there binary mask will there works faster.
                return 1;
            if (undefinedArgNums.count(2))
                return 2;
            break;
        }
        case OP_NEG:
        case OP_SUB:
        case OP_SHL:
        case OP_SHR:
        case OP_SAR:
        case OP_NOT:
        case OP_SIGN:
        {
            if (undefinedArgNums.count(1))
                return 1;
            break;
        }
        case OP_SELECT:
        {
            if (undefinedArgNums.count(3))
                return 3;
            break;
        }
        case (VOP_FMA):
            if (undefinedArgNums.count(1))
                return 1;
            break;
        default:
            break;
        }
        return Backend::reusingPreferences(a_op, undefinedArgNums);
    }

    int Intel64Backend::spillSpaceNeeded(const Syntop& a_op, int basketNum) const
    {
        if(basketNum == RB_INT)
            switch (a_op.opcode)
            {
            case (OP_DIV):
            case (OP_MOD):
                return 2;
                break;
            case (OP_SHL):
            case (OP_SHR):
            case (OP_SAR):
                Assert(a_op.size() == 3);
                return a_op[2].tag == Arg::IREG ? 1 : 0;
            case (OP_ABS):
            case (OP_SIGN):
                return 1;
                break;
            case (OP_CALL):
            case (OP_CALL_NORET):
#if __LOOPS_OS == __LOOPS_WINDOWS
                return 7;
#elif __LOOPS_OS == __LOOPS_LINUX || __LOOPS_OS == __LOOPS_MAC
                return 9;
#else
    #error Unknown OS.
#endif        
            default:
                break;
            }
        else if(basketNum == RB_VEC)
            switch (a_op.opcode)
            {
            case (OP_MOV): //DUBUG: Temporary solution for mov v0, <const>
                if(a_op.args_size == 2 && a_op.args[0].tag == Arg::VREG && a_op.args[1].tag == Arg::IIMMEDIATE)
                    return 1;
                break;
            case (VOP_FMA):
                if(a_op.args_size == 4)
                    return 1;
                break;                
            default:
                break;
            }

        return Backend::spillSpaceNeeded(a_op, basketNum);
    }

    std::set<int> Intel64Backend::getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask) const
    {
        //TODO(ch): This specialized version of function must disappear after introducing snippets. 
        //They will give info about used registers, like now instructions answers.
        //Actually, it's easy to think, that we have to keep used registers info on level of SyntopTranslation. Hmm...

        bool bypass = true;
        uint64_t actualRegs = 0;
        uint64_t inRegs  = 0;
        uint64_t outRegs = 0;
        switch (a_op.opcode)
        {
            case (OP_X86_ADC):
            case (OP_ADD):
            case (OP_SUB):
            case (OP_MUL):
            case (OP_MOD):
            case (OP_DIV):
            case (OP_AND):
            case (OP_OR):
            case (OP_XOR):
            case (OP_MIN):
            case (OP_MAX):
            case (OP_SHL):
            case (OP_SHR):
            case (OP_SAR):
            {
                Assert(a_op.size() == 3 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IREG);
                if (basketNum == RB_INT && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
                {
                    actualRegs = (a_op[2].tag == Arg::IREG ? makeBitmask64({ 0,1,2 }) : makeBitmask64({ 0,1 }));
                    inRegs = makeBitmask64({ 1, 2 });
                    outRegs = makeBitmask64({ 0 });
                    bypass = false;
                }
                break;
            }
            case (OP_SELECT):
            {
                Assert(a_op.size() == 4 && a_op[0].tag == Arg::IREG && a_op[2].tag == Arg::IREG);
                if (basketNum == RB_INT && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
                {
                    actualRegs = (a_op[3].tag == Arg::IREG ? makeBitmask64({ 0,2,3 }) : makeBitmask64({ 0,2 }));
                    inRegs = makeBitmask64({ 2, 3 });
                    outRegs = makeBitmask64({ 0 });
                    bypass = false;
                }
                break;
            }
            case (OP_NEG):
            case (OP_NOT):
            case (OP_ABS):
            case (OP_SIGN):
            {
                Assert(a_op.size() == 2 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IREG);
                if (basketNum == RB_INT && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
                {
                    actualRegs = makeBitmask64({ 0,1 });
                    inRegs = makeBitmask64({ 1 });
                    outRegs = makeBitmask64({ 0 });
                    bypass = false;
                }
                break;
            }
            case (OP_IVERSON):
            {
                Assert(a_op.size() == 2);
                if (basketNum == RB_INT && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
                {
                    actualRegs = makeBitmask64({ 0 });
                    inRegs = makeBitmask64({ 0 });     //Note: This is lie, appended because Iverson bracket on intel work only with preliminarly zeroing of output. 
                    outRegs = makeBitmask64({ 0 });
                    bypass = false;
                }
                break;
            }
            case (OP_MOV):
                //mov ax, 0 is represented as xor ax, ax. Such approach changes default in/out register distribution. There we are fixing it.
                if ( (a_op.size() == 2 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IIMMEDIATE && a_op[1].value == 0) &&
                     (basketNum == RB_INT && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0) )
                {
                    actualRegs = makeBitmask64({ 0 });
                    inRegs = makeBitmask64({});
                    outRegs = makeBitmask64({ 0 });
                    bypass = false;
                }
                else if(a_op.size() == 2 && a_op[0].tag == Arg::VREG && a_op[1].tag == Arg::IIMMEDIATE)
                {
                    if(basketNum == RB_VEC)
                    {
                        actualRegs = makeBitmask64({ 0 });
                        inRegs = makeBitmask64({});
                        outRegs = makeBitmask64({ 0 });
                    }
                    else //if(basketNum == RB_INT)
                        actualRegs = inRegs = outRegs = makeBitmask64({});
                    bypass = false;
                }
                break;
            case (VOP_FMA):
            {
                Assert(a_op.size() == 4 && a_op[0].tag == Arg::VREG && a_op[1].tag == Arg::VREG && a_op[2].tag == Arg::VREG && a_op[3].tag == Arg::VREG);
                if(basketNum == RB_VEC)
                {
                    actualRegs = makeBitmask64({ 0, 1, 2, 3 });
                    inRegs = makeBitmask64({ 1, 2, 3 });
                    outRegs = makeBitmask64({ 0 });
                }
                else  //if(basketNum == RB_INT)
                    actualRegs = inRegs = outRegs = makeBitmask64({});
                bypass = false;
                break;
            }                
            case (OP_CALL):
            case (OP_CALL_NORET):
            {
                bool allRegs = true;
                for(int arnum = 0; arnum < a_op.size(); arnum++)
                    if(a_op[arnum].tag != Arg::IREG)
                    {
                        allRegs = false;
                        break;
                    }
                Assert(allRegs);
                if (basketNum == RB_INT && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
                {
                    outRegs = actualRegs = makeBitmask64({ 0 });
                    inRegs = makeBitmask64({});
                    for(int arnum = (a_op.opcode == OP_CALL? 1 : 0); arnum < a_op.size(); arnum++ )
                    {
                        inRegs |= ((uint64_t)(1) << arnum);
                        actualRegs |= ((uint64_t)(1) << arnum);
                    }
                    bypass = false;
                }
                break;
            }
            default:
                break;
        };
        if (!bypass)
        {
            std::set<int> res;
            auto checkAndAdd = [&res](uint64_t mask, int posnum)
            {
                if (mask & (uint64_t(1) << posnum))
                    res.insert(posnum);
            };
            if (AF_INPUT & flagmask)
                actualRegs &= inRegs;
            if (AF_OUTPUT & flagmask)
                actualRegs &= outRegs;
            checkAndAdd(actualRegs, 0);
            checkAndAdd(actualRegs, 1);
            checkAndAdd(actualRegs, 2);
            checkAndAdd(actualRegs, 3);
            bypass = false;
            return res;
        }
        else
            return Backend::getUsedRegistersIdxs(a_op, basketNum, flagmask);
    }

    void Intel64Backend::getStackParameterLayout(const Syntfunc& a_func, const std::vector<int> (&regParsOverride)[RB_AMOUNT], std::map<RegIdx, int> (&parLayout)[RB_AMOUNT]) const
    {
    #if __LOOPS_OS == __LOOPS_WINDOWS
        int sp2parShift = 5; //+5 is because of return address kept in stack + 32 bytes of shadow space
    #elif __LOOPS_OS == __LOOPS_LINUX
        size_t sp2parShift = 1; //+1 is because of return address kept in stack 
    #else
        #error Unknown OS.
    #endif        

        int regPassed[RB_AMOUNT];
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            regPassed[basketNum] = (int)(regParsOverride[basketNum].size() ? regParsOverride[basketNum].size() : m_parameterRegisters[basketNum].size());
        int currOffset = 0;
        int xBasket[RB_AMOUNT] = {1,1};
        xBasket[RB_VEC] = getVectorRegisterBits() / 64;
        for(const Arg& arg : a_func.params)
        {
            Assert(arg.tag == Arg::IREG || arg.tag == Arg::VREG);
            int basketNum = ( arg.tag == Arg::IREG ? RB_INT : RB_VEC );
            if (regPassed[basketNum] > 0)
            {
                regPassed[basketNum]--;
                continue;
            }
            if(currOffset%xBasket[basketNum])
                currOffset = currOffset - currOffset%xBasket[basketNum] + xBasket[basketNum];
            parLayout[basketNum][arg.idx] = currOffset + sp2parShift;
            currOffset+=xBasket[basketNum];
        }
    }

    int Intel64Backend::stackGrowthAlignment(int stackGrowth) const
    {
        return (stackGrowth ? stackGrowth + ((stackGrowth % 2) ? 0 : 1) : stackGrowth);  //Accordingly to Agner Fog, at start of function RSP % 16 = 8, but must be aligned to 16 for inner calls.
    }

    void Intel64Backend::writeCallerPrologue(Syntfunc& prog, int stackGrowth) const
    {
        prog.program.push_back(Syntop(OP_SPILL, { argIImm(stackGrowth-1), argReg(RB_INT, RBP) }));
        prog.program.push_back(Syntop(OP_MOV,   { argReg(RB_INT, RBP), argReg(RB_INT, RSP) }));
        prog.program.push_back(Syntop(OP_ADD,   { argReg(RB_INT, RBP), argReg(RB_INT, RBP), argIImm((stackGrowth-1) * 8) }));
    }

    void Intel64Backend::writeCallerEpilogue(Syntfunc& prog, int stackGrowth) const
    {
        prog.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT, RBP), argIImm(stackGrowth-1) }));
    }

    Arg Intel64Backend::getSParg() const
    {
        return argReg(RB_INT, RSP);
    }

    column_printer Intel64Backend::get_opname_printer() const
    {
        column_printer ret = { /*func = */ &col_opname_table_printer, /*auxdata = */ opstrings , /*free_func = */ NULL };
        return ret;
    }

    typedef struct intel64_opargs_printer_aux
    {
        LOOPS_HASHMAP(int, int) pos2opnum;
        LOOPS_SPAN(int) positions;
    } intel64_opargs_printer_aux;

    static int intel64_opargs_printer(program_printer* printer, column_printer* colprinter, syntfunc2print* func, int row)
    {
        int program_size = func->program->size;
        loops::Syntop* program = func->program->data;
        int err;
        intel64_opargs_printer_aux* argaux = (intel64_opargs_printer_aux*)colprinter->auxdata;
        if (argaux == NULL)
        {
            int oppos = 0;
            int opnum = 0;
            argaux = (intel64_opargs_printer_aux*)malloc(sizeof(intel64_opargs_printer_aux));
            if (argaux == NULL)
                LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
            memset(argaux, 0, sizeof(intel64_opargs_printer_aux));
            err = loops_hashmap_construct(&(argaux->pos2opnum));
            if(err != LOOPS_ERR_SUCCESS)
            {
                free(argaux);
                LOOPS_THROW(err);
            }
            err = loops_span_construct_alloc(&(argaux->positions), program_size);
            if(err != LOOPS_ERR_SUCCESS) 
            {
                loops_hashmap_destruct(argaux->pos2opnum);
                free(argaux);
                LOOPS_THROW(err);
            }
            for (; opnum < program_size; opnum++)
            {
                int opsize = (int)printer->backend->lookS2b(program[opnum]).size();
                argaux->positions->data[opnum] = oppos;
                if(program[opnum].opcode == INTEL64_LABEL)
                    loops_hashmap_add(argaux->pos2opnum, oppos, opnum);
                oppos += opsize;
            }
            colprinter->auxdata = argaux;
        }

        static const char* rnames[4][16] = { { "al", "cl", "dl", "bl", "spl", "bpl", "sil", "dil", "r8b",  "r9b", "r10b", "r11b" , "r12b" , "r13b" , "r14b" , "r15b" },
            { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "r8w",  "r9w", "r10w", "r11w" , "r12w" , "r13w" , "r14w" , "r15w" },
            { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "r8d",  "r9d", "r10d", "r11d" , "r12d" , "r13d" , "r14d", "r15d" },
            { "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8",  "r9", "r10", "r11" , "r12" , "r13" , "r14" , "r15" },
            };

        Syntop* op = program + row;
        
        uint64_t operand_flags[Syntop::SYNTOP_ARGS_MAX];
        printer->backend->fill_native_operand_flags(op, operand_flags);
        int aamount = op->args_size;

        for(int anum = 0; anum < aamount ; anum++)
        {
            Arg arg = op->args[anum];
            if (operand_flags[anum] & AF_PRINTOFFSET)
            {
                int targetline;
                if (arg.tag != Arg::IIMMEDIATE)
                    LOOPS_THROW(LOOPS_ERR_INCORRECT_ARGUMENT);
                int offset2find = argaux->positions->data[row + 1] + (int)arg.value;
                err = loops_hashmap_get(argaux->pos2opnum, offset2find, &targetline);
                if(err == LOOPS_ERR_ELEMENT_NOT_FOUND)
                    LOOPS_THROW(LOOPS_ERR_INTERNAL_INCORRECT_OFFSET);
                else if(err != LOOPS_ERR_SUCCESS)
                    LOOPS_THROW(err);
                Assert(targetline >= 0);
                Syntop* labelop = program + targetline;
                Assert(labelop->opcode == INTEL64_LABEL);
                Assert(labelop->opcode == INTEL64_LABEL && labelop->args_size == 1);
                Assert(labelop->opcode == INTEL64_LABEL && labelop->args_size == 1 && labelop->args[0].tag == Arg::IIMMEDIATE);
                LOOPS_CALL_THROW(loops_printf(printer, "__loops_label_%d", (int)(labelop->args[0].value)));
                continue;
            }
            uint64_t argflags = operand_flags[anum];
            bool address = (argflags & AF_ADDRESS);
            bool address_start = address && (anum == 0 || !(operand_flags[anum - 1] & AF_ADDRESS));
            bool address_end = address && (anum == aamount - 1 || !(operand_flags[anum + 1] & AF_ADDRESS));
            static const char* address_opener_brackets[5] = {"byte ptr [", "word ptr [", "dword ptr [", "qword ptr [", "["};
            if (address_start)
            {
                int opener_idx = (argflags & AF_ADDRESS) == AF_ADDRESS8  ? 0 : 
                                 (argflags & AF_ADDRESS) == AF_ADDRESS16 ? 1 : 
                                 (argflags & AF_ADDRESS) == AF_ADDRESS32 ? 2 : 
                                 (argflags & AF_ADDRESS) == AF_ADDRESS64 ? 3 : 4;
                LOOPS_CALL_THROW(loops_printf(printer, "%s", address_opener_brackets[opener_idx]));
            }
            switch (arg.tag)
            {
            case Arg::IREG:
            {
                int regsize_idx = 3;
                if(argflags & AF_EFFECTIVE64)
                    regsize_idx = 3;
                else if(arg.elemtype >= TYPE_U8 && arg.elemtype <= TYPE_FP64)
                    regsize_idx = elem_size(arg.elemtype) == 1 ? 0 :
                                    (elem_size(arg.elemtype) == 2 ? 1 : 
                                    (elem_size(arg.elemtype) == 4 ? 2 : 
                                  /*(elem_size(arg.elemtype) == 8*/ 3));
                LOOPS_CALL_THROW(loops_printf(printer, "%s", rnames[regsize_idx][arg.idx]));
                break;
            }
            case Arg::VREG:
            {
                LOOPS_CALL_THROW(loops_printf(printer, "ymm%d", arg.idx));
                break;
            }
            case Arg::IIMMEDIATE:
                if(op->opcode == INTEL64_LABEL)
                {
                    Assert(op->args_size == 1);
                    LOOPS_CALL_THROW(loops_printf(printer, "__loops_label_%d:", arg.value));
                    break;
                }
#if __LOOPS_OS == __LOOPS_WINDOWS
                if (arg.value == 0)
                    LOOPS_CALL_THROW(loops_printf(printer, "0h"));
                else
                {
                    uint32_t upper32 = ((uint64_t)arg.value) >> 32;
                    uint32_t lower32 = ((uint64_t)arg.value) & 0xffffffff;
                    if (upper32 > 0)
                        LOOPS_CALL_THROW(loops_printf(printer, "0%x%08xh", upper32, lower32));
                    else
                        LOOPS_CALL_THROW(loops_printf(printer, "0%02xh", lower32));
                }
#elif __LOOPS_OS == __LOOPS_LINUX
                if (arg.value == 0)
                    LOOPS_CALL_THROW(loops_printf(printer, "0"));
                else
                {
                    uint32_t upper32 = ((uint64_t)arg.value) >> 32;
                    uint32_t lower32 = ((uint64_t)arg.value) & 0xffffffff;
                    if (upper32 > 0)
                        LOOPS_CALL_THROW(loops_printf(printer, "0x0%x%08x", upper32, lower32));
                    else
                        LOOPS_CALL_THROW(loops_printf(printer, "0x0%02x", lower32));
                }
#else 
#error Unknown OS.
#endif
                break;
            case Arg::ISPILLED:
            {
                int opener_idx = 3;
                if (op->opcode == INTEL64_SETNE || op->opcode == INTEL64_SETE || op->opcode == INTEL64_SETGE || op->opcode == INTEL64_SETLE ||
                    op->opcode == INTEL64_SETG  || op->opcode == INTEL64_SETL || op->opcode == INTEL64_SETS  || op->opcode == INTEL64_SETNS) 
                    opener_idx = 0;
                if (arg.value == 0)
                    LOOPS_CALL_THROW(loops_printf(printer, "%srsp]", address_opener_brackets[opener_idx]));
                else
#if __LOOPS_OS == __LOOPS_WINDOWS
                    LOOPS_CALL_THROW(loops_printf(printer, "%srsp + 0%02xh]", address_opener_brackets[opener_idx], arg.value * 8));
#elif __LOOPS_OS == __LOOPS_LINUX
                    LOOPS_CALL_THROW(loops_printf(printer, "%srsp + 0x0%02x]", address_opener_brackets[opener_idx], arg.value * 8));
#else 
#error Unknown OS.
#endif
                break;
            }
            default:
                LOOPS_THROW(LOOPS_ERR_INCORRECT_ARGUMENT);
            };
            if(address)
            {
                if (address_end)
                    LOOPS_CALL_THROW(loops_printf(printer, "]"));
                else
                    LOOPS_CALL_THROW(loops_printf(printer, " + "));
            }
            if (anum < aamount - 1 && !(address && !address_end))
                LOOPS_CALL_THROW(loops_printf(printer, ", "));
        }
        LOOPS_CALL_THROW(close_printer_cell(printer));
        return LOOPS_ERR_SUCCESS;
    }
    
    static void free_intel64_oparg_printer(column_printer* colprinter)
    {
        if (colprinter->auxdata != NULL)
        {
            intel64_opargs_printer_aux* argaux = (intel64_opargs_printer_aux*)colprinter->auxdata;
            loops_hashmap_destruct(argaux->pos2opnum);
            loops_span_destruct(argaux->positions);
            free(argaux);
            colprinter->auxdata = NULL;
        }
    }

    column_printer Intel64Backend::get_opargs_printer() const
    {
        column_printer ret = { /*func = */ &intel64_opargs_printer, /*auxdata = */ NULL, /*free_func = */ &free_intel64_oparg_printer };
        return ret;
    }

    typedef struct pos_size_pair
    {
        int position;
        int size;
    } pos_size_pair;

    LOOPS_SPAN_DECLARE(pos_size_pair);
    LOOPS_SPAN_DEFINE(pos_size_pair)

    typedef struct intel64_hex_printer_aux
    {
        LOOPS_SPAN(pos_size_pair) pos_n_sizes;
        LOOPS_SPAN(uint8_t) binary;
    } intel64_hex_printer_aux;

    static int intel64_hex_printer(program_printer* printer, column_printer* colprinter, syntfunc2print* func, int row)
    {
        int err;
        int program_size = func->program->size;
        loops::Syntop* program = func->program->data;
        int params_size = func->params->size;
        loops::Arg* params = func->params->data;

        intel64_hex_printer_aux* argaux = (intel64_hex_printer_aux*)colprinter->auxdata;
        if (argaux == NULL)
        {
            int oppos = 0;
            int opnum = 0;
            argaux = (intel64_hex_printer_aux*)malloc(sizeof(intel64_hex_printer_aux));
            if (argaux == NULL)
                LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
            memset(argaux, 0, sizeof(intel64_hex_printer_aux));
            err = loops_span_construct_alloc(&(argaux->pos_n_sizes), program_size); 
            if (err != LOOPS_ERR_SUCCESS)
            {
                free(argaux);
                LOOPS_THROW(err);
            }
            for (; opnum < program_size; opnum++)
            {
                int opsize = (int)printer->backend->lookS2b(program[opnum]).size();
                argaux->pos_n_sizes->data[opnum] = {/*position = */oppos, /*size = */opsize};
                oppos += opsize;
            }
            {//TODO[CPP2ANSIC]: This ugly code have to disappear, when syntop, syntfunc and other stuff will be implemented, as C entities.
                Syntfunc tmpfunc;
                tmpfunc.program.resize(program_size);
                memcpy((void*)tmpfunc.program.data(), (void*)program, program_size * sizeof(Syntop));
                tmpfunc.params.resize(params_size);
                memcpy((void*)tmpfunc.params.data(), (void*)params, params_size * sizeof(Arg));
                Assembly2Hex a2hPass(printer->backend);
                a2hPass.process(*((Syntfunc*)(nullptr)), tmpfunc);
                const FuncBodyBuf buffer = a2hPass.result_buffer();
                err = loops_span_construct_alloc(&(argaux->binary), (int)buffer->size());
                if (err != LOOPS_ERR_SUCCESS)
                {
                    loops_span_destruct(argaux->pos_n_sizes);
                    free(argaux);
                    LOOPS_THROW(err);
                }
                memcpy(argaux->binary->data, buffer->data(), argaux->binary->size);
            }
            colprinter->auxdata = argaux;
        }
        unsigned char* hexfield = argaux->binary->data + argaux->pos_n_sizes->data[row].position;
        for (int pos = 0; pos < argaux->pos_n_sizes->data[row].size; pos++)
            LOOPS_CALL_THROW(loops_printf(printer, "%02x ", (unsigned)(*(hexfield + pos))));
        LOOPS_CALL_THROW(close_printer_cell(printer));
        return LOOPS_ERR_SUCCESS;
    }

    static void free_intel64_hex_printer(column_printer* colprinter)
    {
        if (colprinter->auxdata != NULL)
        {
            intel64_hex_printer_aux* argaux = (intel64_hex_printer_aux*)colprinter->auxdata;
            loops_span_destruct(argaux->pos_n_sizes);
            loops_span_destruct(argaux->binary);
            free(argaux);
            colprinter->auxdata = NULL;
        }
    }

    column_printer Intel64Backend::get_hex_printer() const
    {
        column_printer ret = { /*func = */ &intel64_hex_printer, /*auxdata = */ NULL, /*free_func = */ &free_intel64_hex_printer };
        return ret;
    }

    void Intel64BRASnippets::process(Syntfunc& a_dest, const Syntfunc& a_source)
    {
        a_dest.name = a_source.name;
        a_dest.params = a_source.params;
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            a_dest.regAmount[basketNum] = a_source.regAmount[basketNum];
        a_dest.program.reserve(2 * a_source.program.size());
        for (const Syntop& op : a_source.program)
            switch (op.opcode)
            {
            case OP_IVERSON:
                //Unfortunately, Intel's setcc works only with 8-bit wide reigsters, like al or r8b, so register must be preliminarily zeroed.
                Assert(op.size() == 2 && op[1].tag == Arg::IIMMEDIATE && (op[0].tag == Arg::IREG || op[0].tag == Arg::ISPILLED) &&
                       a_dest.program.size() && a_dest.program.back().opcode == OP_CMP);
                a_dest.program.insert(a_dest.program.end() - 1, Syntop(OP_MOV, { op[0], Arg(0) }));
                a_dest.program.push_back(op);
                break;
            case VOP_CAST:
                Assert(op.size() == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG);
                if(op.args[0].elemtype == TYPE_I64 && op.args[1].elemtype == TYPE_FP64)
                {
                    Arg a0_i32 = op.args[0];
                    a0_i32.elemtype = TYPE_I32;
                    a_dest.program.push_back(Syntop(VOP_CAST, { a0_i32, op.args[1] }));
                    a_dest.program.push_back(Syntop(VOP_CAST_LOW, { op.args[0], a0_i32 }));
                }
                else
                    a_dest.program.push_back(op);
                break;
            case VOP_FLOOR:
                Assert(op.size() == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG);
                if(op.args[0].elemtype == TYPE_I64 && op.args[1].elemtype == TYPE_FP64)
                {
                    Arg a0_fp64 = op.args[0];
                    a0_fp64.elemtype = TYPE_FP64;
                    Arg a0_i32 = op.args[0];
                    a0_i32.elemtype = TYPE_I32;
                    a_dest.program.push_back(Syntop(VOP_FLOOR,{ a0_fp64, op.args[1] }));
                    a_dest.program.push_back(Syntop(VOP_CAST, { a0_i32, a0_fp64 }));
                    a_dest.program.push_back(Syntop(VOP_CAST_LOW, { op.args[0], a0_i32 }));
                }
                else if(op.args[0].elemtype == TYPE_I32 && op.args[1].elemtype == TYPE_FP32)
                {
                    Arg a0_fp32 = op.args[0];
                    a0_fp32.elemtype = TYPE_FP32;
                    a_dest.program.push_back(Syntop(VOP_FLOOR,{ a0_fp32, op.args[1] }));
                    a_dest.program.push_back(Syntop(VOP_CAST, { op.args[0], a0_fp32 }));
                }
                else
                    a_dest.program.push_back(op);
                break;
            case VOP_TRUNC:
                Assert(op.size() == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG);
                if(op.args[0].elemtype == TYPE_I64 && op.args[1].elemtype == TYPE_FP64)
                {
                    Arg a0_fp64 = op.args[0];
                    a0_fp64.elemtype = TYPE_FP64;
                    Arg a0_i32 = op.args[0];
                    a0_i32.elemtype = TYPE_I32;
                    a_dest.program.push_back(Syntop(VOP_TRUNC,{ a0_fp64, op.args[1] }));
                    a_dest.program.push_back(Syntop(VOP_CAST, { a0_i32, a0_fp64 }));
                    a_dest.program.push_back(Syntop(VOP_CAST_LOW, { op.args[0], a0_i32 }));
                }
                else if(op.args[0].elemtype == TYPE_I32 && op.args[1].elemtype == TYPE_FP32)
                {
                    Arg a0_fp32 = op.args[0];
                    a0_fp32.elemtype = TYPE_FP32;
                    a_dest.program.push_back(Syntop(VOP_TRUNC,{ a0_fp32, op.args[1] }));
                    a_dest.program.push_back(Syntop(VOP_CAST, { op.args[0], a0_fp32 }));
                }
                else
                    a_dest.program.push_back(op);
                break;
            case VOP_NE:
                Assert(op.size() == 3 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && op.args[1].tag == Arg::VREG);
                if(op.args[1].elemtype == op.args[2].elemtype && isInteger(op.args[1].elemtype))
                {
                    Arg a_signed = op.args[1];
                    Arg b_signed = op.args[2];
                    int etype = a_signed.elemtype;
                    if(isUnsignedInteger(etype))
                        etype = etype == TYPE_U8  ? TYPE_I8  :
                                etype == TYPE_U16 ? TYPE_I8  :
                                etype == TYPE_U32 ? TYPE_I8  :
                              /*etype == TYPE_U64?*/TYPE_I64;
                    a_signed.elemtype = etype;
                    b_signed.elemtype = etype;
                    Arg lt_res = op.args[0];
                    lt_res.idx = a_dest.provideIdx(RB_VEC);
                    a_dest.program.push_back(Syntop(VOP_LT, { lt_res, a_signed, b_signed }));
                    a_dest.program.push_back(Syntop(VOP_GT, { op.args[0], a_signed, b_signed }));
                    a_dest.program.push_back(Syntop(VOP_OR, { op.args[0], op.args[0], lt_res }));
                }
                else
                    a_dest.program.push_back(op);
                break;
            case VOP_LE:
            case VOP_GE:
                Assert(op.size() == 3 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && op.args[1].tag == Arg::VREG);
                if(op.args[1].elemtype == op.args[2].elemtype && isSignedInteger(op.args[1].elemtype))
                {
                    Arg eq_res = op.args[0];
                    eq_res.idx = a_dest.provideIdx(RB_VEC);
                    a_dest.program.push_back(Syntop(VOP_EQ, { eq_res, op.args[1], op.args[2] }));
                    a_dest.program.push_back(Syntop(op.opcode == VOP_LE ? VOP_LT : VOP_GT, { op.args[0], op.args[1], op.args[2] }));
                    a_dest.program.push_back(Syntop(VOP_OR, { op.args[0], op.args[0], eq_res }));
                }
                else
                    a_dest.program.push_back(op);
                break;
            case VOP_NOT:
            {//DUBUG: Temporary solution for mov v0, <const>
                Assert(op.size() == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG);
                Arg allones = op.args[1]; 
                allones.elemtype = TYPE_U8;
                allones.idx = a_dest.provideIdx(RB_VEC);
                a_dest.program.push_back(Syntop(OP_MOV, { allones, argIImm(0xff) }));
                allones.elemtype = op.args[1].elemtype;
                a_dest.program.push_back(Syntop(VOP_XOR, { op.args[0], op.args[1], allones }));
                break;                 
            }
            default:
                a_dest.program.push_back(op);
                break;
            }
    }

    void Intel64ARASnippets::process(Syntfunc& a_dest, const Syntfunc& a_source)
    {
        a_dest.name = a_source.name;
        a_dest.params = a_source.params;
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            a_dest.regAmount[basketNum] = a_source.regAmount[basketNum];
        a_dest.program.reserve(2 * a_source.program.size());
        for (const Syntop& op : a_source.program)
            switch (op.opcode)
            {
            case OP_MOV:
                //This is not about snippets, its ommiting parasite self-assignments.
                Assert(op.size() == 2); 
                if(!(((op[0].tag == Arg::IREG && op[1].tag == Arg::IREG ) || 
                    (op[0].tag == Arg::VREG && op[1].tag == Arg::VREG ))
                    && op[0].idx == op[1].idx))
                {
                    if(op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::IIMMEDIATE) //DUBUG: Temporary solution for mov v0, <const>
                    {
                        const int elements = m_backend->vlanes(op.args[0].elemtype);
                        int lanesize = elem_size(op.args[0].elemtype);
                        Arg laneval = op.args[1];
                        laneval.elemtype = op.args[0].elemtype;
                        for(int lane = 0; lane < elements; lane++)
                        {
                            if(lanesize == 8) 
                            {
                                Arg first = argIImm(((uint64_t)(laneval.value)) & 0xffffffff); first.elemtype = TYPE_U32;
                                Arg second = argIImm(((uint64_t)(laneval.value)) >> 32); second.elemtype = TYPE_U32;
                                a_dest.program.push_back(Syntop(OP_STORE, { argReg(RB_INT, RSP), argIImm(lane*lanesize), first }));
                                a_dest.program.push_back(Syntop(OP_STORE, { argReg(RB_INT, RSP), argIImm(lane*lanesize+4), second }));
                            }
                            else
                                a_dest.program.push_back(Syntop(OP_STORE, { argReg(RB_INT, RSP), argIImm(lane*lanesize), laneval }));
                        }
                        a_dest.program.push_back(Syntop(VOP_LOAD, { op.args[0], argReg(RB_INT, RSP) }));
                    }
                    else
                        a_dest.program.push_back(op);
                }
                break;
            case OP_AND:
            case OP_OR:
            case OP_XOR:
            case OP_X86_ADC:
            case OP_ADD:
            case OP_MUL:
            {
                Syntop op_ = op;
                Assert(op_.size() == 3 && regOrSpi(op_[0]));
                if (op_[1].tag == Arg::IIMMEDIATE)
                    std::swap(op_[1], op_[2]);
                Assert(regOrSpi(op_[1]));
                if (regOrSpi(op_[2]) && regOrSpiEq(op_[0], op_[2]) && !regOrSpiEq(op_[0], op_[1]))
                    std::swap(op_[1], op_[2]);
                if (!regOrSpiEq(op_[0], op_[1]))
                {
                    a_dest.program.push_back(Syntop(OP_MOV, { op_[0],op_[1] }));
                    op_[1] = op_[0];
                }
                a_dest.program.push_back(op_);
                break;
            }
            case OP_SUB:
            {
                Assert(op.size() == 3 && regOrSpi(op[0]) && (regOrSpi(op[1])||regOrSpi(op[2])));
                if (regOrSpi(op[1]) && regOrSpiEq(op[0], op[1]))
                {
                    a_dest.program.push_back(op);
                }
                else if (!regOrSpi(op[2]) || !regOrSpiEq(op[0], op[2]))
                {
                    a_dest.program.push_back(Syntop(OP_MOV, { op[0], op[1] }));
                    a_dest.program.push_back(Syntop(OP_SUB, { op[0], op[0], op[2] }));
                } 
                else //op[0] == op[2] != op[0]
                {
                    a_dest.program.push_back(Syntop(OP_SUB, { op[0], op[0], op[1] }));
                    a_dest.program.push_back(Syntop(OP_NEG, { op[0], op[0] }));
                }
                break;
            }
            case OP_SHL:
            case OP_SHR:
            case OP_SAR:
            {
                Assert(op.size() == 3 && regOrSpi(op[0]) && regOrSpi(op[1]));
                if (op[2].tag == Arg::IIMMEDIATE)
                {
                    if (!regOrSpiEq(op[0], op[1]))
                    {
                        a_dest.program.push_back(Syntop(OP_MOV, { op[0], op[1] }));
                    }
                    a_dest.program.push_back(Syntop(op.opcode, { op[0], op[0], op[2] }));
                }
                else
                {
                    const bool rcx0 = op[0].tag == Arg::IREG && op[0].idx == RCX;
                    const bool rcx1 = op[1].tag == Arg::IREG && op[1].idx == RCX;
                    const bool rcx2 = op[2].tag == Arg::IREG && op[2].idx == RCX;
                    if (rcx0 && rcx1 && rcx2)
                    {
                        a_dest.program.push_back(op);
                    }
                    else if (rcx0)
                    {
                        if (op[1].tag == Arg::ISPILLED)
                        {
                            a_dest.program.push_back(Syntop(OP_SPILL, { 0, op[2] }));
                            a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT, RCX), op[1].value }));
                            a_dest.program.push_back(Syntop(OP_XCHG, { argReg(RB_INT, RCX), argSpilled(RB_INT, 0) }));
                        }
                        else
                            a_dest.program.push_back(Syntop(OP_SPILL, { 0, op[0] }));
                        if(!regOrSpiEq(argReg(RB_INT, RCX), op[2]))
                            a_dest.program.push_back(Syntop(OP_MOV, { argReg(RB_INT, RCX), op[2] }));
                        a_dest.program.push_back(Syntop(op.opcode, { argSpilled(RB_INT, 0), argSpilled(RB_INT, 0), argReg(RB_INT, RCX) }));
                        a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT, RCX), 0 }));
                    }
                    else
                    {
                        a_dest.program.push_back(Syntop(OP_SPILL, { 0, argReg(RB_INT, RCX) }));
                        if(!regOrSpiEq(op[0], op[1]))
                            a_dest.program.push_back(Syntop(OP_MOV, { op[0], op[1] }));
                        if (!regOrSpiEq(argReg(RB_INT, RCX), op[2]))
                            a_dest.program.push_back(Syntop(OP_MOV, { argReg(RB_INT, RCX), op[2] }));
                        a_dest.program.push_back(Syntop(op.opcode, { op[0], op[0], argReg(RB_INT, RCX)}));
                        a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT, RCX), 0 }));
                    }
                }
                break;
            }
            case OP_DIV:
            case OP_MOD:
            {
                Assert(op.size() == 3 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG && regOrSpi(op[2]));
                bool unspillRax = false;;
                if (op[0].idx != RAX)
                {
                    a_dest.program.push_back(Syntop(OP_SPILL, { 0, argReg(RB_INT, RAX) }));
                    unspillRax = true;
                }
                bool unspillRdx = false;
                if (op[0].idx != RDX)
                {
                    a_dest.program.push_back(Syntop(OP_SPILL, { 1, argReg(RB_INT, RDX) }));
                    unspillRdx = true;
                }
                Arg effectiveDivider = op[2];
                if (op[2].tag == Arg::IREG && op[2].idx == RAX)
                {
                    if (!unspillRax)
                        a_dest.program.push_back(Syntop(OP_SPILL, { 0, argReg(RB_INT, RAX) }));
                    effectiveDivider = argSpilled(RB_INT, 0);
                }
                else if (op[2].tag == Arg::IREG && op[2].idx == RDX)
                {
                    if (!unspillRdx)
                        a_dest.program.push_back(Syntop(OP_SPILL, { 1, argReg(RB_INT, RDX) }));
                    effectiveDivider = argSpilled(RB_INT, 1);
                }
                if (op[1].idx != RAX)
                    a_dest.program.push_back(Syntop(OP_MOV, { argReg(RB_INT, RAX), op[1] }));
                a_dest.program.push_back(Syntop(OP_X86_CQO, {}));
                a_dest.program.push_back(Syntop(op.opcode, { argReg(RB_INT, RAX), argReg(RB_INT, RAX), effectiveDivider }));
                if(op.opcode == OP_DIV && op[0].idx != RAX)
                    a_dest.program.push_back(Syntop(OP_MOV, { op[0], argReg(RB_INT, RAX) }));
                if (op.opcode == OP_MOD && op[0].idx != RDX)
                    a_dest.program.push_back(Syntop(OP_MOV, { op[0], argReg(RB_INT, RDX) }));
                if (unspillRax)
                    a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT, RAX), 0 }));
                if (unspillRdx)
                    a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT, RDX), 1 }));
                break;
            }
            case OP_NOT:
            case OP_NEG:
            {
                Syntop op_ = op;
                Assert(op_.size() == 2 && regOrSpi(op_[0]) && regOrSpi(op_[1]));
                if (!regOrSpiEq(op_[0], op_[1]))
                {
                    a_dest.program.push_back(Syntop(OP_MOV, { op_[0],op_[1] }));
                    op_[1] = op_[0];
                }
                a_dest.program.push_back(op_);
                break;
            }
            case OP_SELECT:
                Assert(op.size() == 4 && regOrSpi(op[0]) && op[2].tag == Arg::IREG && op[3].tag == Arg::IREG);
                if (regOrSpiEq(op[2], op[3]))
                {
                    if (!regOrSpiEq(op[0], op[2]))
                        a_dest.program.push_back(Syntop(OP_MOV, { op[0], op[2]}));
                }
                else if (!regOrSpiEq(op[0], op[2]))
                {
                    if (!regOrSpiEq(op[0], op[3]))
                        a_dest.program.push_back(Syntop(OP_MOV, { op[0], op[3] }));
                    a_dest.program.push_back(Syntop(OP_SELECT, { op[0], op[1], op[2], op[0] }));
                }
                else
                    a_dest.program.push_back(Syntop(OP_SELECT, { op[0], argIImm(invertCondition((int)op[1].value)), op[3], op[2]}));
                break;
            case OP_MIN:
            case OP_MAX:
            {
                Syntop op_ = op;
                Assert(op_.size() == 3 && op_[0].tag == Arg::IREG && regOrSpi(op_[1]) && regOrSpi(op_[2]));
                if (regOrSpiEq(op_[0], op_[1]))
                    std::swap(op_[1], op_[2]);
                if (op_[2].tag == Arg::ISPILLED)
                    std::swap(op_[1], op_[2]);
                if (!regOrSpiEq(op_[0], op_[2]))
                    a_dest.program.push_back(Syntop(OP_MOV, { op_[0], op_[2] }));
                a_dest.program.push_back(Syntop(OP_CMP, { op_[0], op_[1] }));
                a_dest.program.push_back(Syntop(OP_SELECT, { op_[0], op_.opcode == OP_MIN ? OP_GT : OP_LT, op_[1], op_[0] }));
                break;
            }
            case OP_ABS:
            {
                Assert(op.size() == 2 && op[0].tag == Arg::IREG && regOrSpi(op[1]));
                bool augAbs = regOrSpiEq(op[0], op[1]);
                if (regOrSpiEq(op[0], op[1]))
                    a_dest.program.push_back(Syntop(OP_SPILL, { 0, op[0] }));
                else
                    a_dest.program.push_back(Syntop(OP_MOV, { op[0], op[1] }));
                a_dest.program.push_back(Syntop(OP_NEG, { op[0], op[0] }));
                a_dest.program.push_back(Syntop(OP_SELECT, { op[0], OP_S, augAbs ? argSpilled(RB_INT, 0) : op[1] , op[0]}));
                break;
            }
            case OP_SIGN:
            {
                Assert(op.size() == 2 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG);
                Arg scratch = argReg(RB_INT, op[0].idx != RCX && op[1].idx != RCX ? RCX : (op[0].idx != RDX && op[1].idx != RDX ? RDX : RAX));
                a_dest.program.push_back(Syntop(OP_SPILL, { 0, scratch })); //TODO(ch): there we could try ask register pool about free regs instead of spilling arbitrary register.
                if (!regOrSpiEq(op[0], op[1]))
                    a_dest.program.push_back(Syntop(OP_MOV, { op[0], op[1] }));
                a_dest.program.push_back(Syntop(OP_MOV, { scratch, op[0] }));
                a_dest.program.push_back(Syntop(OP_SAR, { op[0], op[0], argIImm(63) }));
                a_dest.program.push_back(Syntop(OP_NEG, { scratch, scratch }));
                a_dest.program.push_back(Syntop(OP_X86_ADC, { op[0], op[0], op[0] }));
                a_dest.program.push_back(Syntop(OP_UNSPILL, { scratch, 0 }));
                break;
            }
            case VOP_FMA:
            case VOP_SELECT:
            {
                Syntop op_= op;
                Assert(op_.size() == 4 && op_[0].tag == Arg::VREG && op_[1].tag == Arg::VREG && op_[2].tag == Arg::VREG && op_[3].tag == Arg::VREG);
                if(op_[0].idx == op_[1].idx)
                {
                    a_dest.program.push_back(op_);
                    break;
                }
                bool unspill = false;
                Arg placeholder = op_[2];
                int placeholderSPoff = 0;
                if(op_[0].idx == op_[2].idx || op_[0].idx == op_[3].idx)
                {
                    placeholder.idx = lsb64(~makeBitmask64({(size_t)(op_[0].idx), size_t(op_[1].idx), (size_t)(op_[2].idx), (size_t)(op_[3].idx)}));
                    a_dest.program.push_back(Syntop(OP_SPILL, { placeholderSPoff, placeholder }));
                    a_dest.program.push_back(Syntop(OP_MOV , { placeholder, op_[0] }));
                    unspill = true;
                    if(op_[0].idx == op_[2].idx)
                        op_[2] = placeholder;
                    if(op_[0].idx == op_[3].idx)
                        op_[3] = placeholder;
                }
                if(op_[0].idx != op_[1].idx)
                    a_dest.program.push_back(Syntop(OP_MOV , { op_[0], op_[1] }));
                a_dest.program.push_back(Syntop(op_.opcode, { op_[0], op_[0], op_[2], op_[3] }));
                if(unspill)
                    a_dest.program.push_back(Syntop(OP_UNSPILL, { placeholder, placeholderSPoff }));
                break;
            }
            case OP_CALL:
            case OP_CALL_NORET:
            {

#if __LOOPS_OS == __LOOPS_WINDOWS
                std::vector<int> parameterRegisters = { RCX, RDX, R8, R9 };
                std::vector<int> returnRegisters = { RAX };
                std::vector<int> callerSavedRegisters = { R10, R11 };
#elif __LOOPS_OS == __LOOPS_LINUX || __LOOPS_OS == __LOOPS_MAC
                std::vector<int> parameterRegisters = { RDI, RSI, RDX, RCX, R8, R9 };
                std::vector<int> returnRegisters = { RAX, RDX };
                std::vector<int> callerSavedRegisters = { R10, R11 };
#else
#error Unknown OS
#endif
                std::set<int> allSaved;
                allSaved.insert(parameterRegisters.begin(), parameterRegisters.end());
                allSaved.insert(returnRegisters.begin(), returnRegisters.end());
                allSaved.insert(callerSavedRegisters.begin(), callerSavedRegisters.end());
                Assert((op.opcode == OP_CALL && op.size() >= 2 && op.size() <= ((int)parameterRegisters.size() + 2)) ||
                       (op.opcode == OP_CALL_NORET && op.size() >= 1 && op.size() <= ((int)parameterRegisters.size() + 1)));
#if __LOOPS_OS == __LOOPS_WINDOWS
                Arg sp = argReg(RB_INT, RSP);
#endif        
                int retidx = op.opcode == OP_CALL ? op[0].idx : 0;

                Arg addrkeeper = op.opcode == OP_CALL ? op[1]: op[0];
                size_t addrkeeper_spilled = size_t(-1);
                //1.) Save scalar registers
                {
                    auto iter = allSaved.begin();
                    for(int i = 0; i < (int)allSaved.size(); i++, iter++)
                    {
                        a_dest.program.push_back(Syntop(OP_SPILL, { argIImm(i), argReg(RB_INT,  *iter)}));
                        if(*iter == addrkeeper.idx)
                            addrkeeper_spilled = i;
                    }
                }
                //2.) Prepare arguments accordingly to ABI. Call address must not be broken
                //TODO(ch) : make this algo optimized with help of permutation analysis.
                std::set<int> brokenRegs;
                for(int fargnum = (op.opcode == OP_CALL ? 2 : 1); fargnum < op.size(); fargnum++)
                {
                    Assert(op[fargnum].tag == Arg::IREG);
                    int regidx = parameterRegisters[fargnum - (op.opcode == OP_CALL ? 2 : 1)];
                    if(op[fargnum].idx != regidx)
                    {
                        if(brokenRegs.find(op[fargnum].idx) == brokenRegs.end())
                            a_dest.program.push_back(Syntop(OP_MOV, { argReg(RB_INT,  regidx), argReg(RB_INT,  op[fargnum].idx)}));
                        else
                        {
                            int spillPos = 0;
                            for(auto iter = allSaved.begin(); spillPos < (int)allSaved.size(); spillPos++, iter++)
                                if(*iter == op[fargnum].idx)
                                    break;
                            Assert(spillPos < (int)allSaved.size());
                            a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT,  regidx), argIImm(spillPos)}));
                        }
                        brokenRegs.insert(regidx);
                    }
                }
                if(brokenRegs.find(addrkeeper.idx) != brokenRegs.end())
                {
                    addrkeeper.idx = R10;
                    a_dest.program.push_back(Syntop(OP_UNSPILL, { addrkeeper, argIImm(addrkeeper_spilled)}));
                }
                //3.) Call function
#if __LOOPS_OS == __LOOPS_WINDOWS
                a_dest.program.push_back(Syntop(OP_SUB, { sp, sp, argIImm(32)})); //Reserving shadow space
#endif        
                a_dest.program.push_back(Syntop(OP_CALL_NORET, { addrkeeper }));
#if __LOOPS_OS == __LOOPS_WINDOWS
                a_dest.program.push_back(Syntop(OP_ADD, { sp, sp, argIImm(32)})); //Freeing shadow space
#endif        
                //4.) Move result to output register
                if(op.opcode == OP_CALL && retidx != returnRegisters[0])
                    a_dest.program.push_back(Syntop(OP_MOV, { op[0], argReg(RB_INT, returnRegisters[0])}));
                //5.) Restore scalar registers
                {
                    auto iter = allSaved.begin();
                    for(int i = 0; i < (int)allSaved.size(); i++, iter++)
                        if(op.opcode == OP_CALL_NORET || *iter != retidx)
                            a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT,  *iter), argIImm(i)}));

                }
                break;
            }
            default:
                a_dest.program.push_back(op);
                break;
            }
    }
}
#endif // __LOOPS_ARCH == __LOOPS_INTEL64
