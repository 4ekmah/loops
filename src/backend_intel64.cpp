/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "backend_intel64.hpp"
#if __LOOPS_ARCH == __LOOPS_INTEL64
#include "func_impl.hpp"
#include <algorithm>
#include <iomanip>
#include <unordered_map>

namespace loops
{
    static inline cstring opstrings_getter(int opcode)
    {
        switch (opcode)
        {
        //  |enum_id                         |string_id|
        case (INTEL64_MOV         ) : return "mov"         ;
        case (INTEL64_MOVSX       ) : return "movsx"       ;
        case (INTEL64_MOVSXD      ) : return "movsxd"      ;
        case (INTEL64_MOVZX       ) : return "movzx"       ;
        case (INTEL64_ADC         ) : return "adc"         ;
        case (INTEL64_ADD         ) : return "add"         ;
        case (INTEL64_SUB         ) : return "sub"         ;
        case (INTEL64_IMUL        ) : return "imul"        ;
        case (INTEL64_IDIV        ) : return "idiv"        ;
        case (INTEL64_SHL         ) : return "shl"         ;
        case (INTEL64_SHR         ) : return "shr"         ;
        case (INTEL64_SAR         ) : return "sar"         ;
        case (INTEL64_AND         ) : return "and"         ;
        case (INTEL64_OR          ) : return "or"          ;
        case (INTEL64_XOR         ) : return "xor"         ;
        case (INTEL64_NOT         ) : return "not"         ;
        case (INTEL64_NEG         ) : return "neg"         ;
        case (INTEL64_CQO         ) : return "cqo"         ;
        case (INTEL64_XCHG        ) : return "xchg"        ;
        case (INTEL64_CMP         ) : return "cmp"         ;
        case (INTEL64_CMOVNE      ) : return "cmovne"      ;
        case (INTEL64_CMOVE       ) : return "cmove"       ;
        case (INTEL64_CMOVL       ) : return "cmovl"       ;
        case (INTEL64_CMOVG       ) : return "cmovg"       ;
        case (INTEL64_CMOVGE      ) : return "cmovge"      ;
        case (INTEL64_CMOVA       ) : return "cmova"       ;
        case (INTEL64_CMOVLE      ) : return "cmovle"      ;
        case (INTEL64_CMOVBE      ) : return "cmovbe"      ;
        case (INTEL64_CMOVS       ) : return "cmovs"       ;
        case (INTEL64_CMOVNS      ) : return "cmovns"      ;
        case (INTEL64_SETNE       ) : return "setne"       ;
        case (INTEL64_SETE        ) : return "sete"        ;
        case (INTEL64_SETL        ) : return "setl"        ;
        case (INTEL64_SETG        ) : return "setg"        ;
        case (INTEL64_SETGE       ) : return "setge"       ;
        case (INTEL64_SETA        ) : return "seta"        ;
        case (INTEL64_SETLE       ) : return "setle"       ;
        case (INTEL64_SETBE       ) : return "setbe"       ;
        case (INTEL64_SETS        ) : return "sets"        ;
        case (INTEL64_SETNS       ) : return "setns"       ;
        case (INTEL64_VMOVDQU     ) : return "vmovdqu"     ;
        case (INTEL64_VMOVUPS     ) : return "vmovups"     ;
        case (INTEL64_VMOVUPD     ) : return "vmovupd"     ;
        case (INTEL64_VEXTRACTI128) : return "vextracti128";
        case (INTEL64_VEXTRACTF128) : return "vextractf128";
        case (INTEL64_VINSERTI128 ) : return "vinserti128" ;
        case (INTEL64_VINSERTF128 ) : return "vinsertf128" ;
        case (INTEL64_VPERM2I128  ) : return "vperm2i128"  ;
        case (INTEL64_VPEXTRB     ) : return "vpextrb"     ;
        case (INTEL64_VPEXTRW     ) : return "vpextrw"     ;
        case (INTEL64_VPEXTRD     ) : return "vpextrd"     ;
        case (INTEL64_VPEXTRQ     ) : return "vpextrq"     ;
        case (INTEL64_VPINSRB     ) : return "vpinsrb"     ;
        case (INTEL64_VPINSRW     ) : return "vpinsrw"     ;
        case (INTEL64_VPINSRD     ) : return "vpinsrd"     ;
        case (INTEL64_VPINSRQ     ) : return "vpinsrq"     ;
        case (INTEL64_VMOVD       ) : return "vmovd"       ;
        case (INTEL64_VMOVQ       ) : return "vmovq"       ;
        case (INTEL64_VPBROADCASTB) : return "vpbroadcastb";
        case (INTEL64_VPBROADCASTW) : return "vpbroadcastw";
        case (INTEL64_VPBROADCASTD) : return "vpbroadcastd";
        case (INTEL64_VPBROADCASTQ) : return "vpbroadcastq";
        case (INTEL64_VPADDB      ) : return "vpaddb"      ;
        case (INTEL64_VPADDW      ) : return "vpaddw"      ;
        case (INTEL64_VPADDD      ) : return "vpaddd"      ;
        case (INTEL64_VPADDQ      ) : return "vpaddq"      ;
        case (INTEL64_VADDPS      ) : return "vaddps"      ;
        case (INTEL64_VADDPD      ) : return "vaddpd"      ;
        case (INTEL64_VPSUBB      ) : return "vpsubb"      ;
        case (INTEL64_VPSUBW      ) : return "vpsubw"      ;
        case (INTEL64_VPSUBD      ) : return "vpsubd"      ;
        case (INTEL64_VPSUBQ      ) : return "vpsubq"      ;
        case (INTEL64_VSUBPS      ) : return "vsubps"      ;
        case (INTEL64_VSUBPD      ) : return "vsubpd"      ;
        case (INTEL64_VPMULLW     ) : return "vpmullw"     ;
        case (INTEL64_VPMULLD     ) : return "vpmulld"     ;
        case (INTEL64_VMULPS      ) : return "vmulps"      ;
        case (INTEL64_VMULPD      ) : return "vmulpd"      ;
        case (INTEL64_VDIVPS      ) : return "vdivps"      ;
        case (INTEL64_VDIVPD      ) : return "vdivpd"      ;
        case (INTEL64_VFMADD231PS ) : return "vfmadd231ps" ;
        case (INTEL64_VFMADD231PD ) : return "vfmadd231pd" ;
        case (INTEL64_VPMINUB     ) : return "vpminub"     ;
        case (INTEL64_VPMINSB     ) : return "vpminsb"     ;
        case (INTEL64_VPMINUW     ) : return "vpminuw"     ;
        case (INTEL64_VPMINSW     ) : return "vpminsw"     ;
        case (INTEL64_VPMINUD     ) : return "vpminud"     ;
        case (INTEL64_VPMINSD     ) : return "vpminsd"     ;
        case (INTEL64_VMINPS      ) : return "vminps"      ;
        case (INTEL64_VMINPD      ) : return "vminpd"      ;
        case (INTEL64_VPMAXUB     ) : return "vpmaxub"     ;
        case (INTEL64_VPMAXSB     ) : return "vpmaxsb"     ;
        case (INTEL64_VPMAXUW     ) : return "vpmaxuw"     ;
        case (INTEL64_VPMAXSW     ) : return "vpmaxsw"     ;
        case (INTEL64_VPMAXUD     ) : return "vpmaxud"     ;
        case (INTEL64_VPMAXSD     ) : return "vpmaxsd"     ;
        case (INTEL64_VMAXPS      ) : return "vmaxps"      ;
        case (INTEL64_VMAXPD      ) : return "vmaxpd"      ;
        case (INTEL64_VPCMPEQB    ) : return "vpcmpeqb"    ;
        case (INTEL64_VPCMPEQW    ) : return "vpcmpeqw"    ;
        case (INTEL64_VPCMPEQD    ) : return "vpcmpeqd"    ;
        case (INTEL64_VPCMPEQQ    ) : return "vpcmpeqq"    ;
        case (INTEL64_VPCMPGTB    ) : return "vpcmpgtb"    ;
        case (INTEL64_VPCMPGTW    ) : return "vpcmpgtw"    ;
        case (INTEL64_VPCMPGTD    ) : return "vpcmpgtd"    ;
        case (INTEL64_VPCMPGTQ    ) : return "vpcmpgtq"    ;
        case (INTEL64_VCMPEQPS    ) : return "vcmpeqps"    ;
        case (INTEL64_VCMPNEQPS   ) : return "vcmpneqps"   ;
        case (INTEL64_VCMPLTPS    ) : return "vcmpltps"    ;
        case (INTEL64_VCMPLEPS    ) : return "vcmpleps"    ;
        case (INTEL64_VCMPEQPD    ) : return "vcmpeqpd"    ;
        case (INTEL64_VCMPNEQPD   ) : return "vcmpneqpd"   ;
        case (INTEL64_VCMPLTPD    ) : return "vcmpltpd"    ;
        case (INTEL64_VCMPLEPD    ) : return "vcmplepd"    ;
        case (INTEL64_VPBLENDVB   ) : return "vpblendvb"   ;
        case (INTEL64_VBLENDVPS   ) : return "vblendvps"   ;
        case (INTEL64_VBLENDVPD   ) : return "vblendvpd"   ;
        case (INTEL64_VPAND       ) : return "vpand"       ;
        case (INTEL64_VPOR        ) : return "vpor"        ;
        case (INTEL64_VPXOR       ) : return "vpxor"       ;
        case (INTEL64_VPSLLW      ) : return "vpsllw"      ;
        case (INTEL64_VPSLLD      ) : return "vpslld"      ;
        case (INTEL64_VPSLLQ      ) : return "vpsllq"      ;
        case (INTEL64_VPSLLVD     ) : return "vpsllvd"     ;
        case (INTEL64_VPSLLVQ     ) : return "vpsllvq"     ;
        case (INTEL64_VPSRAW      ) : return "vpsraw"      ;
        case (INTEL64_VPSRAD      ) : return "vpsrad"      ;
        case (INTEL64_VPSRAVD     ) : return "vpsravd"     ;
        case (INTEL64_VPSRLW      ) : return "vpsrlw"      ;
        case (INTEL64_VPSRLD      ) : return "vpsrld"      ;
        case (INTEL64_VPSRLQ      ) : return "vpsrlq"      ;
        case (INTEL64_VPSRLVD     ) : return "vpsrlvd"     ;
        case (INTEL64_VPSRLVQ     ) : return "vpsrlvq"     ;
        case (INTEL64_VROUNDPS    ) : return "vroundps"    ;
        case (INTEL64_VROUNDPD    ) : return "vroundpd"    ;
        case (INTEL64_VCVTPS2DQ   ) : return "vcvtps2dq"   ;
        case (INTEL64_VCVTPD2DQ   ) : return "vcvtpd2dq"   ;
        case (INTEL64_VCVTDQ2PS   ) : return "vcvtdq2ps"   ;
        case (INTEL64_VPMOVSXBW   ) : return "vpmovsxbw"   ;
        case (INTEL64_VPMOVSXWD   ) : return "vpmovsxwd"   ;
        case (INTEL64_VPMOVSXDQ   ) : return "vpmovsxdq"   ;
        case (INTEL64_VPMOVZXBW   ) : return "vpmovzxbw"   ;
        case (INTEL64_VPMOVZXWD   ) : return "vpmovzxwd"   ;
        case (INTEL64_VPMOVZXDQ   ) : return "vpmovzxdq"   ;
        case (INTEL64_VCVTPS2PD   ) : return "vcvtps2pd"   ;
        case (INTEL64_VPALIGNR    ) : return "vpalignr"    ;
        case (INTEL64_VPSHUFD     ) : return "vpshufd"     ;
        case (INTEL64_VPSADBW     ) : return "vpsadbw"     ;
        case (INTEL64_VPHADDD     ) : return "vphaddd"     ;
        case (INTEL64_VHADDPS     ) : return "vhaddps"     ;
        case (INTEL64_VADDSS      ) : return "vaddss"      ;
        case (INTEL64_VHADDPD     ) : return "vhaddpd"     ;
        case (INTEL64_VADDSD      ) : return "vaddsd"      ;
        case (INTEL64_JMP         ) : return "jmp"         ;
        case (INTEL64_JNE         ) : return "jne"         ;
        case (INTEL64_JE          ) : return "je"          ;
        case (INTEL64_JL          ) : return "jl"          ;
        case (INTEL64_JG          ) : return "jg"          ;
        case (INTEL64_JGE         ) : return "jge"         ;
        case (INTEL64_JA          ) : return "ja"          ;
        case (INTEL64_JLE         ) : return "jle"         ;
        case (INTEL64_JBE         ) : return "jbe"         ;
        case (INTEL64_CALL        ) : return "call"        ;
        case (INTEL64_RET         ) : return "ret"         ;
        case (INTEL64_LABEL       ) : return ""            ;
        };
        return nullptr;
    }

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

    enum 
    {
        TYPE_SAME_AS_0 = 60,
        TYPE_SAME_AS_1 = 61,
        TYPE_MASK_FOR_0 = 62,
        TYPE_MASK_FOR_1 = 63,
    };

    static inline uint64_t bm64(std::initializer_list<size_t> regNumbers)
    {
        return makeBitmask64(regNumbers);
    }

    static inline uint64_t bm64(uint64_t bitNumber)
    {
        return bm64({bitNumber});
    }

    static inline uint64_t bm64(int bitNumber)
    {
        return bm64({uint64_t(bitNumber)});
    }
    
    static inline bool bm64_exclusive(uint64_t field, uint8_t flag)
    {
        return ((field & bm64(flag)) == 0) || (field == bm64(flag));
    }

    static inline bool bm64_exists(uint64_t field, int flag)
    {
        LOOPS_ASSERT(flag >= 0);
        return (field == 0 || (bm64(flag) & field) != 0);
    }

    static inline bool bm64_exists(uint64_t field, const Syntop& index, int argnum)
    {
        LOOPS_ASSERT(argnum < index.args_size);
        return (field == 0 || (bm64(index.args[argnum].elemtype) & field) != 0);
    }

    const uint64_t BM64_ALL = bm64({TYPE_U8, TYPE_I8, TYPE_U16, TYPE_I16, TYPE_U32, TYPE_I32,
                                    TYPE_U64, TYPE_I64, TYPE_FP32, TYPE_FP64});
    const uint64_t BM64_ALL_INTS = bm64({TYPE_U8, TYPE_I8, TYPE_U16, TYPE_I16, TYPE_U32, TYPE_I32, TYPE_U64, TYPE_I64});
    const uint64_t BM64_ALL8  = bm64({TYPE_U8, TYPE_I8});
    const uint64_t BM64_ALL16 = bm64({TYPE_U16, TYPE_I16});
    const uint64_t BM64_ALL32 = bm64({TYPE_U32, TYPE_I32, TYPE_FP32});
    const uint64_t BM64_ALL64 = bm64({TYPE_U64, TYPE_I64, TYPE_FP64});
    const uint64_t BM64_ALL_FP = bm64({TYPE_FP32, TYPE_FP64});
    const uint64_t BM64_ALL_INTS32 = bm64({TYPE_U32, TYPE_I32});
    const uint64_t BM64_ALL_INTS64 = bm64({TYPE_U64, TYPE_I64});

    static inline void synchronizeTypesBitmask(const Syntop& index, uint64_t& a, uint64_t& b, int numA, int numB)
    {
        LOOPS_ASSERT(numA<numB);
        static uint64_t uint_same_size[] = {0 , bm64({TYPE_U8}), bm64({TYPE_U16}), 0, bm64({TYPE_U32}), 0, 0, 0, bm64({TYPE_U64})};
        if(numA == 0 && numB == 1 && (a == bm64(TYPE_MASK_FOR_1)))
            a = uint_same_size[elem_size(index.args[1].elemtype)];
        if(numA == 0 && (b == bm64(TYPE_SAME_AS_0)))
            b = ((uint64_t(1))<<index.args[0].elemtype & a);
        if(numA == 0 && (b == bm64(TYPE_MASK_FOR_0))) 
            b = uint_same_size[elem_size(index.args[0].elemtype)];
        if(numA == 1 && (b == bm64(TYPE_SAME_AS_1)))
            b = ((uint64_t(1))<<index.args[1].elemtype & a);
        if(numA == 1 && (b == bm64(TYPE_MASK_FOR_1))) 
            b = uint_same_size[elem_size(index.args[1].elemtype)];
    }
    enum
    {
        VX_IB_PRESENT = 1,
        VX_SWAP_MODRM_REGS = 2
    };


    static BinTranslation VEX_instuction(const Syntop& index, bool& scs, uint32_t pp_opcode, uint32_t m_opcode, uint16_t W, uint16_t L, uint64_t opcode,
                                         uint64_t mrm_opcode, uint64_t argflags0 = 0, uint64_t argflags1 = 0, uint64_t argflags2 = 0, uint64_t argflags3 = 0,
                                         uint64_t supportedTypesBitmask0 = 0, uint64_t supportedTypesBitmask1 = 0, uint64_t supportedTypesBitmask2 = 0,
                                         uint64_t supportedTypesBitmask3 = 0, uint64_t flags = 0, uint64_t ib = 0)
    {
        const uint64_t argflags[] = {argflags0, argflags1, argflags2, argflags3};
        using namespace BinTranslationConstruction;
        scs = false;
        LOOPS_ASSERT(pp_opcode == 0 || pp_opcode == 0x66 || pp_opcode == 0xF2 || pp_opcode == 0xF3);
        LOOPS_ASSERT(m_opcode == 0x0F || m_opcode == 0x0F3A || m_opcode == 0x0F38);
        //Metatypes TYPE_SAME_AS_0, TYPE_SAME_AS_1, TYPE_UINT_SAMESIZE_AS_0 and TYPE_UINT_SAMESIZE_AS_1 cannot be variation, it have to be only type!
        LOOPS_ASSERT(bm64_exclusive(supportedTypesBitmask0, TYPE_MASK_FOR_1));
        LOOPS_ASSERT(bm64_exclusive(supportedTypesBitmask1, TYPE_SAME_AS_0));
        LOOPS_ASSERT(bm64_exclusive(supportedTypesBitmask1, TYPE_MASK_FOR_0));
        LOOPS_ASSERT(bm64_exclusive(supportedTypesBitmask2, TYPE_SAME_AS_0));
        LOOPS_ASSERT(bm64_exclusive(supportedTypesBitmask2, TYPE_SAME_AS_1));
        LOOPS_ASSERT(bm64_exclusive(supportedTypesBitmask2, TYPE_MASK_FOR_0));
        LOOPS_ASSERT(index.args_size >= 2);
        if(supportedTypesBitmask1 == 0 && supportedTypesBitmask0 != 0)
            supportedTypesBitmask1 = supportedTypesBitmask0;
        if(supportedTypesBitmask2 == 0 && supportedTypesBitmask0 != 0)
            supportedTypesBitmask2 = supportedTypesBitmask0;
        if(supportedTypesBitmask3 == 0 && supportedTypesBitmask0 != 0)
            supportedTypesBitmask3 = supportedTypesBitmask0;
        uint32_t mod = 0b11;
        int modRM_reg0_argnum = 0;
        int modRM_reg1_argnum = 1;
        bool vvvv_encode_reg = false;
        int vvvv_reg_num = 0;
        bool postfix_immediate = false;
        int postfix_immediate_size = 8; 
        int postfix_immediate_argnum = 2;
        bool postfix_static = false; 
        bool postfix_register = false; 
        int postfix_register_argnum = 0 ;
        bool sib_byte_present = false;
        int sib_scale = 0;
        int sib_reg0_argnum = 2;
        int sib_reg1_argnum = 1;
        uint32_t m_mmmm = (m_opcode == 0x0F ? 0b01 : (m_opcode == 0x0F38 ? 0b10 : (/*m_opcode == 0x0F3A ?*/0b11)));
        // pp:opcode extension providing equivalent functionality of a SIMD prefix: {00: None | 01: 66 | 10: F3 | 11: F2}
        uint32_t pp = pp_opcode == 0 ? 0 : (pp_opcode == 0x66 ? 0b01 : (pp_opcode == 0xF3 ? 0b10 : (/*pp_opcode == 0xF2 ?*/ 0b11 )));
        std::vector<BinTranslation::Token> tokens;
        if(index.args_size == 2 && (mrm_opcode == 0))
        {
            if(index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG)
            {
                synchronizeTypesBitmask(index, supportedTypesBitmask0, supportedTypesBitmask1, 0, 1);
                if(!((index.args[0].tag == Arg::VREG && bm64_exists(supportedTypesBitmask0, index, 0) && bm64_exists(supportedTypesBitmask1, index, 1))))
                    return BinTranslation();
            }
            else if(index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::IREG && bm64_exists(supportedTypesBitmask0, index, 0))
            {
                if((index.args[1].flags & AF_ADDRESS) != 0)
                {
                    mod = ((index.args[modRM_reg1_argnum].idx & 0b111) == 0b101) ? 1 : 0;
                    if ((index.args[modRM_reg1_argnum].idx & 0b111) == 0b100)
                    {
                        postfix_static = true;
                        ib = 0b00100100;
                    }
                    else if ((index.args[modRM_reg1_argnum].idx & 0b111) == 0b101)
                    {
                        postfix_static = true;
                        ib = 0;
                    }
                }
            }
            else if(index.args[0].tag == Arg::IREG && index.args[1].tag == Arg::VREG && (index.args[0].flags & AF_ADDRESS) != 0 &&
                    bm64_exists(supportedTypesBitmask1, index, 1))
            {
                modRM_reg0_argnum = 1;
                modRM_reg1_argnum = 0;
                mod = ((index.args[modRM_reg1_argnum].idx & 0b111) == 0b101) ? 1 : 0;
                if ((index.args[modRM_reg1_argnum].idx & 0b111) == 0b100)
                {
                    postfix_static = true;
                    ib = 0b00100100;
                }
                else if ((index.args[modRM_reg1_argnum].idx & 0b111) == 0b101)
                {
                    postfix_static = true;
                    ib = 0;
                }
            }
            else 
                return BinTranslation();
        }
        else if(index.args_size == 3)
        {
            if(index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::IREG && index.args[2].tag == Arg::IREG && (index.args[1].flags & AF_ADDRESS) != 0 && 
               (index.args[2].flags & AF_ADDRESS) != 0 && bm64_exists(supportedTypesBitmask0, index, 0))
            {
                mod = 0;
                sib_byte_present = true;
                sib_reg0_argnum = 2;
                sib_reg1_argnum = 1;
            }
            else if(index.args[0].tag == Arg::IREG && index.args[1].tag == Arg::IREG && index.args[2].tag == Arg::VREG && (index.args[0].flags & AF_ADDRESS) != 0 && 
               (index.args[1].flags & AF_ADDRESS) != 0 && bm64_exists(supportedTypesBitmask2, index, 2))
            {
                modRM_reg0_argnum = 2;
                sib_byte_present = true;
                sib_reg0_argnum = 1;
                modRM_reg1_argnum = sib_reg1_argnum = 0;
                mod = ((index.args[modRM_reg1_argnum].idx & 0b111) == 0b101) ? 1 : 0;
                if ((index.args[modRM_reg1_argnum].idx & 0b111) == 0b101)
                {
                    postfix_static = true;
                    ib = 0;
                }                
            }
            else if(index.args[0].tag == Arg::IREG && index.args[1].tag == Arg::IIMMEDIATE && index.args[2].tag == Arg::VREG && (index.args[0].flags & AF_ADDRESS) != 0 && 
               (index.args[1].flags & AF_ADDRESS) != 0 && bm64_exists(supportedTypesBitmask2, index, 2))
            {
                modRM_reg0_argnum = 2;
                mod = 0b10;
                modRM_reg1_argnum = 0;
                if ((index.args[modRM_reg1_argnum].idx & 0b111) == 0b100)
                {
                    postfix_static = true;
                    ib = 0b00100100;
                }
                postfix_immediate = true;
                postfix_immediate_argnum = 1;
                postfix_immediate_size = 32;
            }
            else if (index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::IREG && index.args[2].tag == Arg::IIMMEDIATE && (index.args[1].flags & AF_ADDRESS) != 0 && 
               (index.args[2].flags & AF_ADDRESS) != 0 && signed_fits(index.args[2].value, 32) && 
               bm64_exists(supportedTypesBitmask0, index, 0))
            {
                mod = 0b10;
                if ((index.args[modRM_reg1_argnum].idx & 0b111) == 0b100)
                {
                    postfix_static = true;
                    ib = 0b00100100;
                }
                postfix_immediate = true;
                postfix_immediate_size = 32;
            }
            else if(index.args[0].tag == Arg::IREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::IIMMEDIATE && (index.args[0].flags & AF_ADDRESS) == 0 &&
                    bm64_exists(supportedTypesBitmask1, index, 1))
            {
                postfix_immediate = true;
                if(flags & VX_SWAP_MODRM_REGS)
                    std::swap(modRM_reg0_argnum, modRM_reg1_argnum);
            }            
            else
            {
                synchronizeTypesBitmask(index, supportedTypesBitmask0, supportedTypesBitmask1, 0, 1);
                synchronizeTypesBitmask(index, supportedTypesBitmask0, supportedTypesBitmask2, 0, 2);
                synchronizeTypesBitmask(index, supportedTypesBitmask1, supportedTypesBitmask2, 1, 2);
                if(!((index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && (index.args[2].tag == Arg::VREG || index.args[2].tag == Arg::IIMMEDIATE) &&
                    bm64_exists(supportedTypesBitmask0, index, 0) && bm64_exists(supportedTypesBitmask1, index, 1) &&
                    (index.args[2].tag == Arg::IIMMEDIATE || bm64_exists(supportedTypesBitmask2, index, 2)) &&
                    (index.args[2].tag == Arg::IIMMEDIATE || mrm_opcode == 0))))
                    return BinTranslation();
                if(index.args[2].tag != Arg::IIMMEDIATE)
                {
                    vvvv_encode_reg = true; 
                    vvvv_reg_num = 1; 
                    modRM_reg1_argnum = 2;
                }
                if(mrm_opcode != 0)
                {
                    vvvv_encode_reg = true; 
                    vvvv_reg_num = 0; 
                }
                postfix_immediate = index.args[2].tag == Arg::IIMMEDIATE;
                postfix_static = ((flags & VX_IB_PRESENT) != 0);
                if(flags & VX_SWAP_MODRM_REGS)
                    std::swap(modRM_reg0_argnum, modRM_reg1_argnum);
            }
        }
        else if(index.args_size == 4)
        {
            if(index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && (index.args[2].tag == Arg::VREG || index.args[2].tag == Arg::IREG) && index.args[3].tag == Arg::IIMMEDIATE)
            {
                synchronizeTypesBitmask(index, supportedTypesBitmask0, supportedTypesBitmask1, 0, 1);
                if(index.args[2].tag == Arg::VREG)
                {
                    synchronizeTypesBitmask(index, supportedTypesBitmask0, supportedTypesBitmask2, 0, 2);
                    synchronizeTypesBitmask(index, supportedTypesBitmask1, supportedTypesBitmask2, 1, 2);
                }
                if(!(bm64_exists(supportedTypesBitmask0, index, 0) && bm64_exists(supportedTypesBitmask1, index, 1) && 
                     (index.args[2].tag == Arg::IREG || bm64_exists(supportedTypesBitmask2, index, 2)) &&
                     mrm_opcode == 0))
                    return BinTranslation();
                vvvv_encode_reg = true; 
                vvvv_reg_num = 1;
                modRM_reg1_argnum = 2;
                postfix_immediate = true;
                postfix_immediate_argnum = 3;
            }
            else
            {
                synchronizeTypesBitmask(index, supportedTypesBitmask0, supportedTypesBitmask1, 0, 1);
                synchronizeTypesBitmask(index, supportedTypesBitmask0, supportedTypesBitmask2, 0, 2);
                synchronizeTypesBitmask(index, supportedTypesBitmask1, supportedTypesBitmask2, 1, 2);
                synchronizeTypesBitmask(index, supportedTypesBitmask0, supportedTypesBitmask3, 0, 3);
                synchronizeTypesBitmask(index, supportedTypesBitmask1, supportedTypesBitmask3, 1, 3);
                if(!((index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG && index.args[3].tag == Arg::VREG &&
                    (supportedTypesBitmask0 == 0 || (bm64(index.args[0].elemtype) & supportedTypesBitmask0) != 0) &&
                    (supportedTypesBitmask1 == 0 || (bm64(index.args[1].elemtype) & supportedTypesBitmask1) != 0) &&
                    (supportedTypesBitmask2 == 0 || (bm64(index.args[2].elemtype) & supportedTypesBitmask2) != 0) &&
                    (supportedTypesBitmask3 == 0 || (bm64(index.args[3].elemtype) & supportedTypesBitmask3) != 0) && 
                    mrm_opcode == 0)))
                    return BinTranslation();
                vvvv_encode_reg = true; 
                vvvv_reg_num = 1; 
                modRM_reg1_argnum = 2;
                postfix_register = true;
                postfix_register_argnum = 3;
            }
        }
        else
            return BinTranslation();
        const uint32_t R = mrm_opcode != 0 ? 1 : ((index.args[modRM_reg0_argnum].idx & 0b1000) == 0);
        const uint32_t X = sib_byte_present ? ((index.args[sib_reg0_argnum].idx & 0b1000) == 0) : 1;
        const uint32_t B = ((index.args[modRM_reg1_argnum].idx & 0b1000) == 0);
        //VEX prefix itself
        if(W == 0 && B == 1 && X == 1 && m_mmmm == 0b00001)
        {
            tokens.push_back(BTsta((0xc5 << 1)|R, 9));
            if(vvvv_encode_reg) //vvvv field in VEX terminology
            {
                tokens.push_back(BTomm(vvvv_reg_num, argflags[vvvv_reg_num]));
                tokens.push_back(BTsta(~(index.args[vvvv_reg_num].idx&0b1111), 4));
            }
            else
                tokens.push_back(BTsta(0b1111, 4));
            tokens.push_back(BTsta((L<<2)|pp, 3));
        }
        else
        {
            tokens.push_back(BTsta((0xc4 << 9)|(R<<8)|(X<<7)|(B<<6)|(m_mmmm<<1)|W, 17));
            //vvvv field in VEX terminology
            if(vvvv_encode_reg) 
            {
                tokens.push_back(BTomm(vvvv_reg_num, argflags[vvvv_reg_num]));
                tokens.push_back(BTsta(~(index.args[vvvv_reg_num].idx)&0b1111, 4));
            }
            else
                tokens.push_back(BTsta(0b1111, 4));
            tokens.push_back(BTsta((L<<2)|pp, 3));
        }
        tokens.push_back(BTsta(opcode, 8));
        {//ModRM
            //Non-inplace case, first source register(or "non-destructive source register") is encoded in vvvv part of vex
            //So arg[2] is considered like usual, well-known intel input and scheme look for ModRM byte like it is two argument instruction.
            tokens.push_back(BTsta(mod, 2));
            //For some instructions is wider, than 8 bits. 3 extra bits located in ModRM byte.
            if(mrm_opcode != 0)
                tokens.push_back(BTsta(mrm_opcode, 3));
            else 
                tokens.push_back(BTreg(modRM_reg0_argnum, 3, argflags[modRM_reg0_argnum]));
            if(!sib_byte_present)
            {
                tokens.push_back(BTreg(modRM_reg1_argnum, 3, argflags[modRM_reg1_argnum]));
                if(postfix_register)
                {
                    tokens.push_back(BTreg(postfix_register_argnum, 4, argflags[postfix_register_argnum]));
                    tokens.push_back(BTsta(0, 4));
                }
                else
                {
                    if(postfix_static)
                        tokens.push_back(BTsta(ib, 8));
                    if(postfix_immediate)
                        tokens.push_back(BTimm(postfix_immediate_argnum, postfix_immediate_size, argflags[postfix_immediate_argnum]));
                }
            }
            else
            {
                tokens.push_back(BTsta(0b100, 3));
            }
        }
        if(sib_byte_present)
        {
            tokens.push_back(BTsta(sib_scale, 2));
            tokens.push_back(BTreg(sib_reg0_argnum, 3, argflags[sib_reg0_argnum])); //index
            tokens.push_back(BTreg(sib_reg1_argnum, 3, argflags[sib_reg1_argnum])); //base
            if(postfix_static)
                tokens.push_back(BTsta(ib, 8));
        }
        scs = true;
        return BinTranslation(tokens);
    }

    BinTranslation i64BTLookup(const Syntop& index, bool& scs)
    {
        //TODO(ch): A lot of commands supports immediates of different widthes(8/32/64), but there are implemented just
        //fixed sizes(in most cases = 32). For space economy, it's better to implement different cases.
        using namespace BinTranslationConstruction;
        enum { Xmm = HlfLan };
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
        case (INTEL64_CMOVNE):
        case (INTEL64_CMOVE ):
        case (INTEL64_CMOVL ):
        case (INTEL64_CMOVG ):
        case (INTEL64_CMOVGE):
        case (INTEL64_CMOVA ):
        case (INTEL64_CMOVLE):
        case (INTEL64_CMOVBE):
        case (INTEL64_CMOVS ):
        case (INTEL64_CMOVNS):
            if (index.size() == 2 && index[0].tag == Arg::IREG)
            {
                static uint64_t regbytes[4] = { 0x480f00, 0x4c0f00, 0x490f00, 0x4d0f00 };
                uint64_t stat = index.opcode == INTEL64_CMOVNE ? 0x45 :
                                index.opcode == INTEL64_CMOVE  ? 0x44 :
                                index.opcode == INTEL64_CMOVL  ? 0x4c :
                                index.opcode == INTEL64_CMOVG  ? 0x4f :
                                index.opcode == INTEL64_CMOVGE ? 0x4d :
                                index.opcode == INTEL64_CMOVA  ? 0x47 :
                                index.opcode == INTEL64_CMOVLE ? 0x4e :
                                index.opcode == INTEL64_CMOVBE ? 0x46 :
                                index.opcode == INTEL64_CMOVS  ? 0x48 :
                              /*index.opcode == INTEL64_CMOVNS?*/0x49/*:*/;
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
        case (INTEL64_SETNE):
        case (INTEL64_SETE ):
        case (INTEL64_SETL ):
        case (INTEL64_SETG ):
        case (INTEL64_SETGE):
        case (INTEL64_SETA ):
        case (INTEL64_SETLE):
        case (INTEL64_SETBE):
        case (INTEL64_SETS ):
        case (INTEL64_SETNS):
            if (index.size() == 1)
            {
                static uint64_t regbytes[4] = { 0x0f00, 0x400f00, 0x410f00 };
                uint64_t stat = index.opcode == INTEL64_SETNE ? 0x95 :
                                index.opcode == INTEL64_SETE  ? 0x94 :
                                index.opcode == INTEL64_SETL  ? 0x9c :
                                index.opcode == INTEL64_SETG  ? 0x9f :
                                index.opcode == INTEL64_SETGE ? 0x9d :
                                index.opcode == INTEL64_SETA  ? 0x97 :
                                index.opcode == INTEL64_SETLE ? 0x9e :
                                index.opcode == INTEL64_SETBE ? 0x96 :
                                index.opcode == INTEL64_SETS  ? 0x98 :
                              /*index.opcode == INTEL64_SETNS?*/0x99/*:*/;
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
            if (index.args_size == 2 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG)
            {
                return VEX_instuction(index, scs, 0xF3,   0x0F, 0, 1, 0x6F, 0, Out, In, 0, 0, BM64_ALL, bm64({TYPE_SAME_AS_0}));
            }
            else if (index.args_size == 2 || index.args_size == 3)
            {
                int flags0 = (index.args[0].tag == Arg::VREG ? Out : (In | AddrVec));
                int flags1 = (index.args[1].tag != Arg::IIMMEDIATE ? In : 0) | (index.args[1].tag == Arg::VREG ? 0 : AddrVec);
                int flags2 = (index.args[2].tag != Arg::IIMMEDIATE ? In : 0) | (index.args[2].tag == Arg::VREG ? 0 : AddrVec);
                return VEX_instuction(index, scs, 0xF3,   0x0F, 0, 1, index.args[0].tag == Arg::VREG ? 0x6F : 0x7F, 0, flags0, flags1, flags2);
            }
            break;
        case (INTEL64_VMOVUPS):
            if (index.args_size == 2 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG)
            {
                return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0x10, 0, Out, In, 0, 0, bm64(TYPE_FP32), bm64(TYPE_FP32));
            }
            else if (index.args_size == 2 || index.args_size == 3)
            {
                int flags0 = (index.args[0].tag == Arg::VREG ? Out : (In | AddrVec));
                int flags1 = (index.args[1].tag != Arg::IIMMEDIATE ? In : 0) | (index.args[1].tag == Arg::VREG ? 0 : AddrVec);
                int flags2 = (index.args[2].tag != Arg::IIMMEDIATE ? In : 0) | (index.args[2].tag == Arg::VREG ? 0 : AddrVec);
                return VEX_instuction(index, scs,    0,   0x0F, 0, 1, index.args[0].tag == Arg::VREG ? 0x10 : 0x11, 0, flags0, flags1, flags2, 0, bm64(TYPE_FP32));
            }
            break;
        case (INTEL64_VMOVUPD):
            if (index.args_size == 2 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG)
            {
                return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x10, 0, Out, In, 0, 0, bm64(TYPE_FP64), bm64(TYPE_FP64));
            }
            else if (index.args_size == 2 || index.args_size == 3)
            {
                int flags0 = (index.args[0].tag == Arg::VREG ? Out : (In | AddrVec));
                int flags1 = (index.args[1].tag != Arg::IIMMEDIATE ? In : 0) | (index.args[1].tag == Arg::VREG ? 0 : AddrVec);
                int flags2 = (index.args[2].tag != Arg::IIMMEDIATE ? In : 0) | (index.args[2].tag == Arg::VREG ? 0 : AddrVec);
                return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, index.args[0].tag == Arg::VREG ? 0x10 : 0x11, 0, flags0, flags1, flags2, 0, bm64(TYPE_FP64));
            }
            break;
        case (INTEL64_VEXTRACTI128): return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 1, 0x39, 0, Out|Xmm,     In,      0,  0,           BM64_ALL_INTS, bm64({TYPE_SAME_AS_0}),                       0,                       0, VX_SWAP_MODRM_REGS);
        case (INTEL64_VEXTRACTF128): return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 1, 0x19, 0, Out|Xmm,     In,      0,  0,             BM64_ALL_FP, bm64({TYPE_SAME_AS_0}),                       0,                       0, VX_SWAP_MODRM_REGS);
        case (INTEL64_VINSERTI128):  return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 1, 0x38, 0,     Out,     In, In|Xmm,  0,           BM64_ALL_INTS, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VINSERTF128):  return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 1, 0x18, 0,     Out,     In, In|Xmm,  0,             BM64_ALL_FP, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPERM2I128):   return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 1, 0x46, 0,     Out,     In,     In,  0,                 BM64_ALL, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                      0);
        case (INTEL64_VPEXTRB):      return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 0, 0x14, 0,     Out, In|Xmm,      0,  0,                       0,              BM64_ALL8,                       0,                       0, VX_SWAP_MODRM_REGS);
        case (INTEL64_VPEXTRW):      return VEX_instuction(index, scs, 0x66,   0x0F, 0, 0, 0xC5, 0,     Out, In|Xmm,      0,  0,                       0,             BM64_ALL16,                       0,                       0);
        case (INTEL64_VPEXTRD):      return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 0, 0x16, 0,     Out, In|Xmm,      0,  0,                       0,             BM64_ALL32,                       0,                       0, VX_SWAP_MODRM_REGS);
        case (INTEL64_VPEXTRQ):      return VEX_instuction(index, scs, 0x66, 0x0F3A, 1, 0, 0x16, 0,     Out, In|Xmm,      0,  0,                       0,             BM64_ALL64,                       0,                       0, VX_SWAP_MODRM_REGS);
        case (INTEL64_VPINSRB):      return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 0, 0x20, 0, Out|Xmm, In|Xmm,     In,  0,               BM64_ALL8, bm64({TYPE_SAME_AS_0}),                       0,                       0);
        case (INTEL64_VPINSRW):      return VEX_instuction(index, scs, 0x66,   0x0F, 0, 0, 0xC4, 0, Out|Xmm, In|Xmm,     In,  0,              BM64_ALL16, bm64({TYPE_SAME_AS_0}),                       0,                       0);
        case (INTEL64_VPINSRD):      return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 0, 0x22, 0, Out|Xmm, In|Xmm,     In,  0,              BM64_ALL32, bm64({TYPE_SAME_AS_0}),                       0,                       0);
        case (INTEL64_VPINSRQ):      return VEX_instuction(index, scs, 0x66, 0x0F3A, 1, 0, 0x22, 0, Out|Xmm, In|Xmm,     In,  0,              BM64_ALL64, bm64({TYPE_SAME_AS_0}),                       0,                       0);
        case (INTEL64_VMOVD):        return VEX_instuction(index, scs, 0x66,   0x0F, 0, 0, 0x6E, 0, Out|Xmm,     In,      0,  0,              BM64_ALL32,                      0,                       0,                       0);
        case (INTEL64_VMOVQ):        return VEX_instuction(index, scs, 0x66,   0x0F, 1, 0, 0x6E, 0, Out|Xmm,     In,      0,  0,              BM64_ALL64,                      0,                       0,                       0);
        case (INTEL64_VPBROADCASTB): return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x78, 0,     Out, In|Xmm,      0,  0,               BM64_ALL8, bm64({TYPE_SAME_AS_0}),                       0,                       0);
        case (INTEL64_VPBROADCASTW): return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x79, 0,     Out, In|Xmm,      0,  0,              BM64_ALL16, bm64({TYPE_SAME_AS_0}),                       0,                       0);
        case (INTEL64_VPBROADCASTD): return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x58, 0,     Out, In|Xmm,      0,  0,              BM64_ALL32, bm64({TYPE_SAME_AS_0}),                       0,                       0);
        case (INTEL64_VPBROADCASTQ): return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x59, 0,     Out, In|Xmm,      0,  0,              BM64_ALL64, bm64({TYPE_SAME_AS_0}),                       0,                       0);
        case (INTEL64_VPADDB):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xFC, 0,     Out,     In,     In,  0,               BM64_ALL8, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPADDW):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xFD, 0,     Out,     In,     In,  0,              BM64_ALL16, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPADDD):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xFE, 0,     Out,     In,     In,  0,         BM64_ALL_INTS32, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPADDQ):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xD4, 0,     Out,     In,     In,  0,         BM64_ALL_INTS64, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VADDPS):       return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0x58, 0,     Out,     In,     In,  0,       bm64({TYPE_FP32}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VADDPD):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x58, 0,     Out,     In,     In,  0,       bm64({TYPE_FP64}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPSUBB):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xF8, 0,     Out,     In,     In,  0,               BM64_ALL8, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPSUBW):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xF9, 0,     Out,     In,     In,  0,              BM64_ALL16, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPSUBD):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xFA, 0,     Out,     In,     In,  0,         BM64_ALL_INTS32, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPSUBQ):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xFB, 0,     Out,     In,     In,  0,         BM64_ALL_INTS64, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VSUBPS):       return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0x5C, 0,     Out,     In,     In,  0,       bm64({TYPE_FP32}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VSUBPD):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x5C, 0,     Out,     In,     In,  0,       bm64({TYPE_FP64}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMULLW):      return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xD5, 0,     Out,     In,     In,  0,              BM64_ALL16, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMULLD):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x40, 0,     Out,     In,     In,  0,         BM64_ALL_INTS32, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VMULPS):       return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0x59, 0,     Out,     In,     In,  0,       bm64({TYPE_FP32}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VMULPD):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x59, 0,     Out,     In,     In,  0,       bm64({TYPE_FP64}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VDIVPS):       return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0x5E, 0,     Out,     In,     In,  0,       bm64({TYPE_FP32}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VDIVPD):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x5E, 0,     Out,     In,     In,  0,       bm64({TYPE_FP64}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VFMADD231PS):  return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0xB8, 0,      IO,     In,     In,  0,       bm64({TYPE_FP32}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VFMADD231PD):  return VEX_instuction(index, scs, 0x66, 0x0F38, 1, 1, 0xB8, 0,      IO,     In,     In,  0,       bm64({TYPE_FP64}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMINUB):      return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xDA, 0,     Out,     In,     In,  0,         bm64({TYPE_U8}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMINSB):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x38, 0,     Out,     In,     In,  0,         bm64({TYPE_I8}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMINUW):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x3A, 0,     Out,     In,     In,  0,        bm64({TYPE_U16}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMINSW):      return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xEA, 0,     Out,     In,     In,  0,        bm64({TYPE_I16}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMINUD):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x3B, 0,     Out,     In,     In,  0,        bm64({TYPE_U32}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMINSD):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x39, 0,     Out,     In,     In,  0,        bm64({TYPE_I32}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VMINPS):       return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0x5D, 0,     Out,     In,     In,  0,       bm64({TYPE_FP32}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VMINPD):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x5D, 0,     Out,     In,     In,  0,       bm64({TYPE_FP64}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMAXUB):      return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xDE, 0,     Out,     In,     In,  0,         bm64({TYPE_U8}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMAXSB):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x3C, 0,     Out,     In,     In,  0,         bm64({TYPE_I8}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMAXUW):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x3E, 0,     Out,     In,     In,  0,        bm64({TYPE_U16}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMAXSW):      return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xEE, 0,     Out,     In,     In,  0,        bm64({TYPE_I16}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMAXUD):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x3F, 0,     Out,     In,     In,  0,        bm64({TYPE_U32}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPMAXSD):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x3D, 0,     Out,     In,     In,  0,        bm64({TYPE_I32}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VMAXPS):       return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0x5F, 0,     Out,     In,     In,  0,       bm64({TYPE_FP32}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VMAXPD):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x5F, 0,     Out,     In,     In,  0,       bm64({TYPE_FP64}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPCMPEQB):     return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x74, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),              BM64_ALL8,  bm64({TYPE_SAME_AS_1}),                       0);
        case (INTEL64_VPCMPEQW):     return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x75, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),             BM64_ALL16,  bm64({TYPE_SAME_AS_1}),                       0);
        case (INTEL64_VPCMPEQD):     return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x76, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),        BM64_ALL_INTS32,  bm64({TYPE_SAME_AS_1}),                       0);
        case (INTEL64_VPCMPEQQ):     return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x29, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),        BM64_ALL_INTS64,  bm64({TYPE_SAME_AS_1}),                       0);
        case (INTEL64_VPCMPGTB):     return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x64, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),        bm64({TYPE_I8}),         bm64({TYPE_I8}),                       0);
        case (INTEL64_VPCMPGTW):     return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x65, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),       bm64({TYPE_I16}),        bm64({TYPE_I16}),                       0);
        case (INTEL64_VPCMPGTD):     return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x66, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),       bm64({TYPE_I32}),        bm64({TYPE_I32}),                       0);
        case (INTEL64_VPCMPGTQ):     return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x37, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),       bm64({TYPE_I64}),        bm64({TYPE_I64}),                       0);
        case (INTEL64_VCMPEQPS):     return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0xC2, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),      bm64({TYPE_FP32}),       bm64({TYPE_FP32}),                       0, VX_IB_PRESENT, 0);
        case (INTEL64_VCMPNEQPS):    return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0xC2, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),      bm64({TYPE_FP32}),       bm64({TYPE_FP32}),                       0, VX_IB_PRESENT, 4);
        case (INTEL64_VCMPLTPS):     return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0xC2, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),      bm64({TYPE_FP32}),       bm64({TYPE_FP32}),                       0, VX_IB_PRESENT, 1);
        case (INTEL64_VCMPLEPS):     return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0xC2, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),      bm64({TYPE_FP32}),       bm64({TYPE_FP32}),                       0, VX_IB_PRESENT, 2);
        case (INTEL64_VCMPEQPD):     return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xC2, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),      bm64({TYPE_FP64}),       bm64({TYPE_FP64}),                       0, VX_IB_PRESENT, 0);
        case (INTEL64_VCMPNEQPD):    return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xC2, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),      bm64({TYPE_FP64}),       bm64({TYPE_FP64}),                       0, VX_IB_PRESENT, 4);
        case (INTEL64_VCMPLTPD):     return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xC2, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),      bm64({TYPE_FP64}),       bm64({TYPE_FP64}),                       0, VX_IB_PRESENT, 1);
        case (INTEL64_VCMPLEPD):     return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xC2, 0,     Out,     In,     In,  0, bm64({TYPE_MASK_FOR_1}),      bm64({TYPE_FP64}),       bm64({TYPE_FP64}),                       0, VX_IB_PRESENT, 2);
        case (INTEL64_VBLENDVPS):    return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 1, 0x4A, 0,     Out,     In,     In, In,       bm64({TYPE_FP32}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}), bm64({TYPE_MASK_FOR_0}));
        case (INTEL64_VBLENDVPD):    return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 1, 0x4B, 0,     Out,     In,     In, In,       bm64({TYPE_FP64}), bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}), bm64({TYPE_MASK_FOR_0}));
        case (INTEL64_VPBLENDVB):    return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 1, 0x4C, 0,     Out,     In,     In, In,           BM64_ALL_INTS, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}), bm64({TYPE_MASK_FOR_0}));
        case (INTEL64_VPAND):        return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xDB, 0,     Out,     In,     In,  0,                BM64_ALL, bm64({TYPE_SAME_AS_0}), bm64({TYPE_MASK_FOR_0}),                       0);
        case (INTEL64_VPOR):         return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xEB, 0,     Out,     In,     In,  0,                BM64_ALL, bm64({TYPE_SAME_AS_0}), bm64({TYPE_MASK_FOR_0}),                       0);
        case (INTEL64_VPXOR):        return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xEF, 0,     Out,     In,     In,  0,                BM64_ALL, bm64({TYPE_SAME_AS_0}), bm64({TYPE_MASK_FOR_0}),                       0);
        case (INTEL64_VPSLLW):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x71, 6,     Out,     In,      0,  0,              BM64_ALL16, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPSLLD):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x72, 6,     Out,     In,      0,  0,         BM64_ALL_INTS32, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPSLLQ):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x73, 6,     Out,     In,      0,  0,         BM64_ALL_INTS64, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPSLLVD):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x47, 0,     Out,     In,     In,  0,         BM64_ALL_INTS32, bm64({TYPE_SAME_AS_0}),        bm64({TYPE_U32}),                       0);
        case (INTEL64_VPSLLVQ):      return VEX_instuction(index, scs, 0x66, 0x0F38, 1, 1, 0x47, 0,     Out,     In,     In,  0,         BM64_ALL_INTS64, bm64({TYPE_SAME_AS_0}),        bm64({TYPE_U64}),                       0);
        case (INTEL64_VPSRAW):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x71, 4,     Out,     In,      0,  0,        bm64({TYPE_I16}),       bm64({TYPE_I16}),        bm64({TYPE_U16}),                       0);
        case (INTEL64_VPSRAD):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x72, 4,     Out,     In,      0,  0,        bm64({TYPE_I32}),       bm64({TYPE_I32}),        bm64({TYPE_U32}),                       0);
        case (INTEL64_VPSRAVD):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x46, 0,     Out,     In,     In,  0,        bm64({TYPE_I32}),       bm64({TYPE_I32}),        bm64({TYPE_U32}),                       0);
        case (INTEL64_VPSRLW):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x71, 2,     Out,     In,      0,  0,              BM64_ALL16, bm64({TYPE_SAME_AS_0}),        bm64({TYPE_U16}),                       0);
        case (INTEL64_VPSRLD):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x72, 2,     Out,     In,      0,  0,         BM64_ALL_INTS32, bm64({TYPE_SAME_AS_0}),        bm64({TYPE_U32}),                       0);
        case (INTEL64_VPSRLQ):       return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x73, 2,     Out,     In,      0,  0,         BM64_ALL_INTS64, bm64({TYPE_SAME_AS_0}),        bm64({TYPE_U64}),                       0);
        case (INTEL64_VPSRLVD):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x45, 0,     Out,     In,     In,  0,         BM64_ALL_INTS32, bm64({TYPE_SAME_AS_0}),        bm64({TYPE_U32}),                       0);
        case (INTEL64_VPSRLVQ):      return VEX_instuction(index, scs, 0x66, 0x0F38, 1, 1, 0x45, 0,     Out,     In,     In,  0,         BM64_ALL_INTS64, bm64({TYPE_SAME_AS_0}),        bm64({TYPE_U64}),                       0);
        case (INTEL64_VROUNDPS):     return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 1, 0x08, 0,     Out,     In,      0,  0,       bm64({TYPE_FP32}),                      0,                       0,                       0);
        case (INTEL64_VROUNDPD):     return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 1, 0x09, 0,     Out,     In,      0,  0,       bm64({TYPE_FP64}),                      0,                       0,                       0);
        case (INTEL64_VCVTPS2DQ):    return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x5B, 0,     Out,     In,      0,  0,        bm64({TYPE_I32}),      bm64({TYPE_FP32}),                       0,                       0);
        case (INTEL64_VCVTPD2DQ):    return VEX_instuction(index, scs, 0xF2,   0x0F, 0, 1, 0xE6, 0, Out|Xmm,     In,      0,  0,        bm64({TYPE_I32}),      bm64({TYPE_FP64}),                       0,                       0);
        case (INTEL64_VCVTDQ2PS):    return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0x5B, 0,     Out,     In,      0,  0,       bm64({TYPE_FP32}),       bm64({TYPE_I32}),                       0,                       0);
        case (INTEL64_VPMOVSXBW):    return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x20, 0,     Out, In|Xmm,      0,  0,        bm64({TYPE_I16}),        bm64({TYPE_I8}),                       0,                       0);
        case (INTEL64_VPMOVSXWD):    return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x23, 0,     Out, In|Xmm,      0,  0,        bm64({TYPE_I32}),       bm64({TYPE_I16}),                       0,                       0);
        case (INTEL64_VPMOVSXDQ):    return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x25, 0,     Out, In|Xmm,      0,  0,        bm64({TYPE_I64}),       bm64({TYPE_I32}),                       0,                       0);
        case (INTEL64_VPMOVZXBW):    return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x30, 0,     Out, In|Xmm,      0,  0,        bm64({TYPE_U16}),        bm64({TYPE_U8}),                       0,                       0);
        case (INTEL64_VPMOVZXWD):    return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x33, 0,     Out, In|Xmm,      0,  0,        bm64({TYPE_U32}),       bm64({TYPE_U16}),                       0,                       0);
        case (INTEL64_VPMOVZXDQ):    return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x35, 0,     Out, In|Xmm,      0,  0,        bm64({TYPE_U64}),       bm64({TYPE_U32}),                       0,                       0);
        case (INTEL64_VCVTPS2PD):    return VEX_instuction(index, scs,    0,   0x0F, 0, 1, 0x5A, 0,     Out, In|Xmm,      0,  0,       bm64({TYPE_FP64}),      bm64({TYPE_FP32}),                       0,                       0);
        case (INTEL64_VPALIGNR):     return VEX_instuction(index, scs, 0x66, 0x0F3A, 0, 1, 0x0F, 0,     Out,     In,     In,  0,                BM64_ALL, bm64({TYPE_SAME_AS_0}),  bm64({TYPE_SAME_AS_0}),                       0);
        case (INTEL64_VPSHUFD):      return VEX_instuction(index, scs, 0x66,   0x0F, 0, 0, 0x70, 0, Out|Xmm, In|Xmm,      0,  0,              BM64_ALL32, bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VPSADBW):      return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0xF6, 0,     Out,     In,     In,  0,        bm64({TYPE_U32}),        bm64({TYPE_U8}),   bm64({TYPE_SAME_AS_1}));
        case (INTEL64_VPHADDD):      return VEX_instuction(index, scs, 0x66, 0x0F38, 0, 1, 0x02, 0,     Out,     In,     In,  0,         BM64_ALL_INTS32, bm64({TYPE_SAME_AS_0}),   bm64({TYPE_SAME_AS_0}));
        case (INTEL64_VHADDPS):      return VEX_instuction(index, scs, 0xF2,   0x0F, 0, 1, 0x7C, 0,     Out,     In,     In,  0,       bm64({TYPE_FP32}),      bm64({TYPE_FP32}),        bm64({TYPE_FP32}));
        case (INTEL64_VADDSS):       return VEX_instuction(index, scs, 0xF3,   0x0F, 0, 0, 0x58, 0, Out|Xmm, In|Xmm, In|Xmm,  0,       bm64({TYPE_FP32}),      bm64({TYPE_FP32}),        bm64({TYPE_FP32}));
        case (INTEL64_VHADDPD):      return VEX_instuction(index, scs, 0x66,   0x0F, 0, 1, 0x7C, 0,     Out,     In,     In,  0,       bm64({TYPE_FP64}),      bm64({TYPE_FP64}),        bm64({TYPE_FP64}));
        case (INTEL64_VADDSD):       return VEX_instuction(index, scs, 0xF2,   0x0F, 0, 0, 0x58, 0, Out|Xmm, In|Xmm, In|Xmm,  0,       bm64({TYPE_FP64}),      bm64({TYPE_FP64}),        bm64({TYPE_FP64}));
        case (INTEL64_JMP): return BiT({ BTsta(0xE9,8), BTimm(0, 32, Lab) });
        case (INTEL64_JNE): return BiT({ BTsta(0xf85,16), BTimm(0, 32, Lab) });
        case (INTEL64_JE):  return BiT({ BTsta(0xf84,16), BTimm(0, 32, Lab) });
        case (INTEL64_JL):  return BiT({ BTsta(0xf8c,16), BTimm(0, 32, Lab) });
        case (INTEL64_JG):  return BiT({ BTsta(0xf8f,16), BTimm(0, 32, Lab) });
        case (INTEL64_JGE): return BiT({ BTsta(0xf8d,16), BTimm(0, 32, Lab) });
        case (INTEL64_JA):  return BiT({ BTsta(0xf87,16), BTimm(0, 32, Lab) });
        case (INTEL64_JLE): return BiT({ BTsta(0xf8e,16), BTimm(0, 32, Lab) });
        case (INTEL64_JBE): return BiT({ BTsta(0xf86,16), BTimm(0, 32, Lab) });
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

    SyntopTranslation i64STLookup(const Backend* backend, const Syntop& index, bool& scs)
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
                {
                    int opcode = index[0].elemtype == TYPE_FP32 ? INTEL64_VMOVUPS :
                                 index[0].elemtype == TYPE_FP64 ? INTEL64_VMOVUPD : INTEL64_VMOVDQU;
                    return SyT(opcode,  { SAcop(0), SAcop(1) });
                }
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
                int tarcode = index[1].value == OP_NE  ? INTEL64_CMOVNE :
                              index[1].value == OP_EQ  ? INTEL64_CMOVE  :
                              index[1].value == OP_LT  ? INTEL64_CMOVL  :
                              index[1].value == OP_GT  ? INTEL64_CMOVG  :
                              index[1].value == OP_GE  ? INTEL64_CMOVGE :
                              index[1].value == OP_UGT ? INTEL64_CMOVA  :
                              index[1].value == OP_LE  ? INTEL64_CMOVLE :
                              index[1].value == OP_ULE ? INTEL64_CMOVBE :
                              index[1].value == OP_S   ? INTEL64_CMOVS  :
                              index[1].value == OP_NS  ? INTEL64_CMOVNS : -1;
                LOOPS_ASSERT(tarcode != -1);
                return SyT(tarcode, { SAcop(0), SAcop(2) });
            }
            break;
        case (OP_IVERSON): 
            if (index.size() == 2 && index[1].value >= OP_GT && index[1].value <= OP_NS)
            {
                int tarcode = index[1].value == OP_NE  ? INTEL64_SETNE :
                              index[1].value == OP_EQ  ? INTEL64_SETE  :
                              index[1].value == OP_LT  ? INTEL64_SETL  :
                              index[1].value == OP_GT  ? INTEL64_SETG  :
                              index[1].value == OP_GE  ? INTEL64_SETGE :
                              index[1].value == OP_UGT ? INTEL64_SETA :
                              index[1].value == OP_LE  ? INTEL64_SETLE :
                              index[1].value == OP_ULE ? INTEL64_SETBE :
                              index[1].value == OP_S   ? INTEL64_SETS  :
                              index[1].value == OP_NS  ? INTEL64_SETNS : -1;
                LOOPS_ASSERT(tarcode != -1);
                return SyT(tarcode, { SAcopelt(0, TYPE_U8) });
            }
            break;
        case (VOP_LOAD):
            if (index.args_size >= 2 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::IREG)
            {
                int opcode = index[0].elemtype == TYPE_FP32 ? INTEL64_VMOVUPS :
                             index[0].elemtype == TYPE_FP64 ? INTEL64_VMOVUPD : INTEL64_VMOVDQU;
                if (index.args_size == 2)
                    return SyT(opcode, { SAcop(0), SAcop(1, AF_ADDRESS) });
                else if (index.args_size == 3 && (index.args[2].tag == Arg::IREG || index.args[2].tag == Arg::IIMMEDIATE))
                    return SyT(opcode, { SAcop(0), SAcop(1, AF_ADDRESS), SAcop(2, AF_ADDRESS) });
            }
            break;
        case (VOP_STORE):
            if (index.args_size >= 2 && index.args[index.args_size-1].tag == Arg::VREG && index.args[0].tag == Arg::IREG)
            {
                int opcode = index[index.args_size-1].elemtype == TYPE_FP32 ? INTEL64_VMOVUPS :
                             index[index.args_size-1].elemtype == TYPE_FP64 ? INTEL64_VMOVUPD : INTEL64_VMOVDQU;
                if (index.args_size == 2)
                    return SyT(opcode, { SAcop(0, AF_ADDRESS), SAcop(1) });
                else if (index.args_size == 3 && (index.args[1].tag == Arg::IREG || index.args[1].tag == Arg::IIMMEDIATE))
                    return SyT(opcode, { SAcop(0, AF_ADDRESS), SAcop(1, AF_ADDRESS), SAcop(2) });
            }
            break;
        case (VOP_X86_VEXTRACT128):
            if(index.args_size == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::IIMMEDIATE && 
               index.args[0].elemtype == index.args[1].elemtype && (index.args[2].value == 0 || index.args[2].value == 1))
            {
                if(isInteger(index.args[0].elemtype))
                    return SyT(INTEL64_VEXTRACTI128, { SAcop(0), SAcop(1), SAcop(2) });
                else if(index.args[0].elemtype == TYPE_FP32 || index.args[0].elemtype == TYPE_FP64)
                    return SyT(INTEL64_VEXTRACTF128, { SAcop(0), SAcop(1), SAcop(2) });
            }
            break;
        case (VOP_X86_VINSERT128):
            if(index.args_size == 4 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::IIMMEDIATE && index.args[2].tag == Arg::VREG && 
               index.args[3].tag == Arg::VREG && index.args[0].elemtype == index.args[2].elemtype && index.args[0].elemtype == index.args[3].elemtype && 
               (index.args[1].value == 0 || index.args[1].value == 1))
            {
                if(isInteger(index.args[0].elemtype)) 
                    return SyT(INTEL64_VINSERTI128, { SAcop(0), SAcop(2), SAcop(3), SAcop(1) });
                else if(index.args[0].elemtype == TYPE_FP32 || index.args[0].elemtype == TYPE_FP64)
                    return SyT(INTEL64_VINSERTF128, { SAcop(0), SAcop(2), SAcop(3), SAcop(1) });
            }
            break;
        case (VOP_X86_VPERM2I128):
            //Permute halfs of ymm:
            //Immediate contain higher 4 bits and lower 4 bits.
            //Lower defines source of first half in destination register
            //Higher defines source of second half in destination register
            //Sources are defined:
            // 0 - first half of first source register
            // 1 - second half of first source register
            // 2 - first half of second source register
            // 3 - second half of second source register
            //There is more sophisticated options, but we don't need them currently.
            if(index.args_size == 4 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG &&
                index.args[3].tag == Arg::IIMMEDIATE && (index.args[3].value & 0b11) < 0b11 && (index.args[3].value & 0b110000) < 0b110000)
                return SyT(INTEL64_VPERM2I128, { SAcop(0), SAcop(1), SAcop(2), SAcop(3) });
            break;
        case (VOP_GETLANE):
            if(index.args_size == 3 && index.args[0].tag == Arg::IREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::IIMMEDIATE)
            {
                int elemsize = elem_size(index.args[1].elemtype);
                int opcode = elemsize == 1 ? INTEL64_VPEXTRB :
                             elemsize == 2 ? INTEL64_VPEXTRW :
                             elemsize == 4 ? INTEL64_VPEXTRD :
                             elemsize == 8 ? INTEL64_VPEXTRQ : -1;
                if(opcode!=-1 && index.args[2].value < backend->vlanes(index.args[1].elemtype)/2)
                    return SyT(opcode, { elemsize == 4 ? SAcopelt(0, TYPE_I32) : SAcop(0) , SAcop(1), SAcop(2) });
            }
            break;
        case (VOP_SETLANE):
            if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG)
            { 
                int elemsize = elem_size(index.args[0].elemtype);
                int opcode = elemsize == 1 ? INTEL64_VPINSRB :
                             elemsize == 2 ? INTEL64_VPINSRW :
                             elemsize == 4 ? INTEL64_VPINSRD :
                             elemsize == 8 ? INTEL64_VPINSRQ : -1;
                if(opcode!=-1 && index.args[1].value < backend->vlanes(index.args[0].elemtype)/2)
                    return SyT(opcode, { SAcop(0), SAcop(0), elemsize == 8 ? SAcop(2) : SAcopelt(2, TYPE_I32), SAcop(1) });
            }
            break;
        case (VOP_BROADCAST):
            if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::IIMMEDIATE && index.args[2].value == 0)
            {
                switch(index.args[0].elemtype)
                {
                    case(TYPE_U8):  case(TYPE_I8):  return SyT(INTEL64_VPBROADCASTB, { SAcop(0), SAcop(1) });
                    case(TYPE_U16): case(TYPE_I16): return SyT(INTEL64_VPBROADCASTW, { SAcop(0), SAcop(1) });
                    case(TYPE_U32): case(TYPE_I32): case(TYPE_FP32): return SyT(INTEL64_VPBROADCASTD, { SAcop(0), SAcop(1) });
                    case(TYPE_U64): case(TYPE_I64): case(TYPE_FP64): return SyT(INTEL64_VPBROADCASTQ, { SAcop(0), SAcop(1) });
                }
            }
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
        case (VOP_DIV):
            if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG
            && index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype )
            {
                if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
                    return SyT(index[0].elemtype == TYPE_FP32 ? INTEL64_VDIVPS : INTEL64_VDIVPD, { SAcop(0), SAcop(1), SAcop(2) });
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
        case (VOP_SELECT):
            if (index.args_size == 4 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG &&
                index.args[3].tag == Arg::VREG && index.args[0].elemtype == index.args[2].elemtype && index.args[0].elemtype == index.args[3].elemtype && 
                elem_size(index.args[0].elemtype) == elem_size(index.args[1].elemtype) && isUnsignedInteger(index.args[1].elemtype))
            {
                switch(index.args[0].elemtype)
                {
                    case(TYPE_FP32):  return SyT(INTEL64_VBLENDVPS, { SAcop(0), SAcop(3), SAcop(2), SAcop(1) });
                    case(TYPE_FP64):  return SyT(INTEL64_VBLENDVPD, { SAcop(0), SAcop(3), SAcop(2), SAcop(1) });
                    default: return SyT(INTEL64_VPBLENDVB, { SAcop(0), SAcop(3), SAcop(2), SAcop(1) });
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
               ((isUnsignedInteger(index.args[0].elemtype) && isUnsignedInteger(index.args[1].elemtype)) || 
                (isSignedInteger(index.args[0].elemtype) && isSignedInteger(index.args[1].elemtype)) || 
                (index.args[0].elemtype == TYPE_FP64 && index.args[1].elemtype == TYPE_FP32)) && 
                elem_size(index.args[0].elemtype) == 2 * elem_size(index.args[1].elemtype))
            {
                int opcode = -1;
                switch (index.args[0].elemtype)
                {
                    case (TYPE_I16):  opcode = INTEL64_VPMOVSXBW; break;
                    case (TYPE_I32):  opcode = INTEL64_VPMOVSXWD; break;
                    case (TYPE_I64):  opcode = INTEL64_VPMOVSXDQ; break;
                    case (TYPE_U16):  opcode = INTEL64_VPMOVZXBW; break;
                    case (TYPE_U32):  opcode = INTEL64_VPMOVZXWD; break;
                    case (TYPE_U64):  opcode = INTEL64_VPMOVZXDQ; break;
                    case (TYPE_FP64): opcode = INTEL64_VCVTPS2PD; break;
                }
                if(opcode != -1)
                    return SyT(opcode, { SAcop(0), SAcop(1) });
            }
            break;
        case (VOP_X86_VPALIGNR):
            if(index.size() == 4 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG &&
               index.args[2].tag == Arg::VREG && index.args[3].tag == Arg::IIMMEDIATE && index.args[3].value < 16)
                return SyT(INTEL64_VPALIGNR, { SAcop(0), SAcop(1), SAcop(2), SAcop(3) });
            break;
        case VOP_X86_VPSHUFD:
            if(index.size() == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG &&
               index.args[2].tag == Arg::IIMMEDIATE && index.args[2].value >= 0 && index.args[2].value < 256)
                return SyT(INTEL64_VPSHUFD, { SAcop(0), SAcop(1), SAcop(2) });
            break;
        case VOP_X86_VPSADBW:
            // Sources u8 a and b:
            // a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23, a24, a25, a26, a27, a28, a29, a30, a31
            // b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29, b30, b31
            // dest_u16[0] = abs(a0-b0) + abs(a1-b1) + abs(a2-b2) + abs(a3-b3) + abs(a4-b4) + abs(a5-b5) + abs(a6-b6) + abs(a7-b7)
            // dest_u16[4] = abs(a8-b8) + abs(a9-b9) + abs(a10-b10) + abs(a11-b11) + abs(a12-b12) + abs(a13-b13) + abs(a14-b14) + abs(a15-b15)
            // dest_u16[8] = abs(a16-b16) + abs(a17-b17) + abs(a18-b18) + abs(a19-b19) + abs(a20-b20) + abs(a21-b21) + abs(a22-b22) + abs(a23-b23)
            // dest_u16[12] = abs(a24-b24) + abs(a25-b25) + abs(a26-b26) + abs(a27-b27) + abs(a28-b28) + abs(a29-b29) + abs(a30-b30) + abs(a31-b31)
            if(index.size() == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG)
                return SyT(INTEL64_VPSADBW, { SAcop(0), SAcop(1), SAcop(2) });
            break;
        case VOP_X86_VPHADDD:
            if(index.size() == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG)
                return SyT(INTEL64_VPHADDD, { SAcop(0), SAcop(1), SAcop(2) });
            break;
        case VOP_X86_VHADDPS:
            if(index.size() == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG)
                return SyT(INTEL64_VHADDPS, { SAcop(0), SAcop(1), SAcop(2) });
            break;
        case VOP_X86_VADDSS:
            if(index.size() == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG)
                return SyT(INTEL64_VADDSS, { SAcop(0), SAcop(1), SAcop(2) });
            break;
        case VOP_X86_VHADDPD:
            if(index.size() == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG)
                return SyT(INTEL64_VHADDPD, { SAcop(0), SAcop(1), SAcop(2) });
            break;
        case VOP_X86_VADDSD:
            if(index.size() == 3 && index.args[0].tag == Arg::VREG && index.args[1].tag == Arg::VREG && index.args[2].tag == Arg::VREG)
                return SyT(INTEL64_VADDSD, { SAcop(0), SAcop(1), SAcop(2) });
            break;
        case (OP_UNSPILL):
            if(index.args_size == 2)
            {
                if(index[0].tag == Arg::IREG) 
                    return SyT(INTEL64_MOV, { SAcopelt(0, TYPE_I64), SAcopspl(1) });
                else if(index[0].tag == Arg::VREG)
                {
                    if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
                        return SyT(index[0].elemtype == TYPE_FP32 ? INTEL64_VMOVUPS : INTEL64_VMOVUPD, { SAcop(0), SAreg(RSP, AF_ADDRESS), SAcopsar(1, -3, AF_ADDRESS) });
                    else
                        return SyT(INTEL64_VMOVDQU, { SAcopelt(0, TYPE_I64), SAreg(RSP, AF_ADDRESS), SAcopsar(1, -3, AF_ADDRESS) });
                }
            }
            break;
        case (OP_SPILL):
            if(index.args_size == 2)
            {
                if(index[1].tag == Arg::IREG)
                    return SyT(INTEL64_MOV, { SAcopspl(0), SAcopelt(1, TYPE_I64) });
                else if(index[1].tag == Arg::VREG)
                {
                    if(index[1].elemtype == TYPE_FP32 || index[1].elemtype == TYPE_FP64)
                        return SyT(index[1].elemtype == TYPE_FP32 ? INTEL64_VMOVUPS : INTEL64_VMOVUPD, { SAreg(RSP, AF_ADDRESS), SAcopsar(0, -3, AF_ADDRESS), SAcop(1) });
                    else
                        return SyT(INTEL64_VMOVDQU, { SAreg(RSP, AF_ADDRESS), SAcopsar(0, -3, AF_ADDRESS), SAcopelt(1, TYPE_I64) });
                }
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
                case (OP_UGT): return SyT(INTEL64_JA,  { SAcop(1) });
                case (OP_LE):  return SyT(INTEL64_JLE, { SAcop(1) });
                case (OP_ULE): return SyT(INTEL64_JBE,  { SAcop(1) });
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
        void handle_reduce_sum32(Syntfunc& a_dest, const Arg& output, const Arg& input) const;
        void handle_mov_imm2vec(Syntfunc& a_dest, const Arg& output, int64_t input) const;
        void handle_ultugt(Syntfunc& a_dest, const Syntop& op) const;
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

#if __LOOPS_OS == __LOOPS_WINDOWS
    m_parameterRegisters[RB_VEC] = { YMM0, YMM1, YMM2, YMM3 };
    m_returnRegisters[RB_VEC] = { YMM0 };
    m_callerSavedRegisters[RB_VEC] = { YMM5, YMM6, YMM7, YMM8, YMM9, YMM10, YMM11, YMM12, YMM13, YMM14, YMM15 };
    m_calleeSavedRegisters[RB_VEC] = { YMM6, YMM7, YMM8, YMM9, YMM10, YMM11, YMM12, YMM13, YMM14, YMM15 };
#elif __LOOPS_OS == __LOOPS_LINUX || __LOOPS_OS == __LOOPS_MAC
    m_parameterRegisters[RB_VEC] = { YMM0, YMM1, YMM2, YMM3, YMM4, YMM5, YMM6, YMM7};
    m_returnRegisters[RB_VEC] = { YMM0 };
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
            LOOPS_ASSERT(a_op.size() == 3 && a_op[0].tag == Arg::IREG);
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
            LOOPS_ASSERT(a_op.size() == 2 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IREG);
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
        {
            std::set<int> res = toFilter;
            res.erase(0);
            if(regOrSpiEq(a_op.args[0], a_op.args[2]))
                res.erase(2);
            else
                res.erase(3);
            return res;
            break;
        }
        case(OP_IVERSON):
            LOOPS_ASSERT(a_op.size() == 2);
            return (toFilter.count(0) ? std::set<int>({0}) : std::set<int>({}));
            break;
        case(OP_ABS):
            LOOPS_ASSERT(a_op.size() == 2);
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
                LOOPS_ASSERT(a_op.size() == 3);
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
            case (VOP_FMA):
                if(a_op.args_size == 4)
                    return 1;
                break;                
            case (OP_CALL):
            case (OP_CALL_NORET):
#if __LOOPS_OS == __LOOPS_WINDOWS
                return 16; //15+1 for alignment
#elif __LOOPS_OS == __LOOPS_LINUX || __LOOPS_OS == __LOOPS_MAC
                return 17; //16+1 for alignment
#else
    #error Unknown OS.
#endif        
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
                LOOPS_ASSERT(a_op.size() == 3 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IREG);
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
                LOOPS_ASSERT(a_op.size() == 4 && a_op[0].tag == Arg::IREG && a_op[2].tag == Arg::IREG);
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
                LOOPS_ASSERT(a_op.size() == 2 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IREG);
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
                LOOPS_ASSERT(a_op.size() == 2);
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
                LOOPS_ASSERT(a_op.size() == 4 && a_op[0].tag == Arg::VREG && a_op[1].tag == Arg::VREG && a_op[2].tag == Arg::VREG && a_op[3].tag == Arg::VREG);
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
                LOOPS_ASSERT(allRegs);
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
            LOOPS_ASSERT(arg.tag == Arg::IREG || arg.tag == Arg::VREG);
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

    column_printer_ptr Intel64Backend::get_opname_printer() const
    {
        return std::static_pointer_cast<column_printer>(std::make_shared<col_opname_table_printer>(&opstrings_getter));
    }

    class intel64_opargs_printer : public column_printer
    {
    public:
        intel64_opargs_printer() : column_printer(&intel64_opargs_printer::print) {}
        virtual ~intel64_opargs_printer() {}
    private:
        static void print(program_printer* printer, column_printer* colprinter, const Syntfunc& func, int row);
        std::unordered_map<int, int> pos2opnum;
        std::vector<int> positions;
    };

    void intel64_opargs_printer::print(program_printer* printer, column_printer* colprinter, const Syntfunc& func, int row)
    {
        int program_size = (int)func.program.size();
        const Syntop* program = func.program.data();
        intel64_opargs_printer* opargs_printer = (intel64_opargs_printer*)colprinter;
        if (opargs_printer->positions.empty())
        {
            int oppos = 0;
            int opnum = 0;
            opargs_printer->positions.resize(program_size);
            for (; opnum < program_size; opnum++)
            {
                int opsize = (int)printer->backend->lookS2b(program[opnum]).size();
                opargs_printer->positions[opnum] = oppos;
                if(program[opnum].opcode == INTEL64_LABEL)
                    opargs_printer->pos2opnum[oppos] = opnum;
                oppos += opsize;
            }
        }

        static const char* rnames[4][16] = { { "al", "cl", "dl", "bl", "spl", "bpl", "sil", "dil", "r8b",  "r9b", "r10b", "r11b" , "r12b" , "r13b" , "r14b" , "r15b" },
            { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "r8w",  "r9w", "r10w", "r11w" , "r12w" , "r13w" , "r14w" , "r15w" },
            { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "r8d",  "r9d", "r10d", "r11d" , "r12d" , "r13d" , "r14d", "r15d" },
            { "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8",  "r9", "r10", "r11" , "r12" , "r13" , "r14" , "r15" },
            };

        const Syntop* op = program + row;
        
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
                    throw loops::exception(LOOPS_ERR_INCORRECT_ARGUMENT);
                int offset2find = opargs_printer->positions[row + 1] + (int)arg.value;
                if (opargs_printer->pos2opnum.count(offset2find) == 0)
                    throw loops::exception(LOOPS_ERR_INTERNAL_INCORRECT_OFFSET);
                else
                    targetline = opargs_printer->pos2opnum.at(offset2find);
                LOOPS_ASSERT(targetline >= 0);
                const Syntop* labelop = program + targetline;
                LOOPS_ASSERT(labelop->opcode == INTEL64_LABEL);
                LOOPS_ASSERT(labelop->opcode == INTEL64_LABEL && labelop->args_size == 1);
                LOOPS_ASSERT(labelop->opcode == INTEL64_LABEL && labelop->args_size == 1 && labelop->args[0].tag == Arg::IIMMEDIATE);
                loops_printf(printer, "__loops_label_%d", (int)(labelop->args[0].value));
                continue;
            }
            uint64_t argflags = operand_flags[anum];
            bool address = (argflags & AF_ADDRESS);
            bool address_start = address && (anum == 0 || !(operand_flags[anum - 1] & AF_ADDRESS));
            bool address_end = address && (anum == aamount - 1 || !(operand_flags[anum + 1] & AF_ADDRESS));
            static const char* address_opener_brackets[5] = {"byte ptr [", "word ptr [", "dword ptr [", "qword ptr [", "ymmword ptr ["};
            if (address_start)
            {
                int opener_idx = (argflags & AF_ADDRESS) == AF_ADDRESS8   ? 0 : 
                                 (argflags & AF_ADDRESS) == AF_ADDRESS16  ? 1 : 
                                 (argflags & AF_ADDRESS) == AF_ADDRESS32  ? 2 : 
                                 (argflags & AF_ADDRESS) == AF_ADDRESS64  ? 3 :
                               /*(argflags & AF_ADDRESS) == AF_ADDRESSVEC ?*/ 4 /*: */;
                loops_printf(printer, "%s", address_opener_brackets[opener_idx]);
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
                loops_printf(printer, "%s", rnames[regsize_idx][arg.idx]);
                break;
            }
            case Arg::VREG:
            {
                if(argflags & AF_HALFLANES) 
                    loops_printf(printer, "xmm%d", arg.idx);
                else
                    loops_printf(printer, "ymm%d", arg.idx);
                break;
            }
            case Arg::IIMMEDIATE:
                if(op->opcode == INTEL64_LABEL)
                {
                    LOOPS_ASSERT(op->args_size == 1);
                    loops_printf(printer, "__loops_label_%d:", arg.value);
                    break;
                }
#if __LOOPS_OS == __LOOPS_WINDOWS
                if (arg.value == 0)
                    loops_printf(printer, "0h");
                else
                {
                    uint32_t upper32 = ((uint64_t)arg.value) >> 32;
                    uint32_t lower32 = ((uint64_t)arg.value) & 0xffffffff;
                    if (upper32 > 0)
                        loops_printf(printer, "0%x%08xh", upper32, lower32);
                    else
                        loops_printf(printer, "0%02xh", lower32);
                }
#elif __LOOPS_OS == __LOOPS_LINUX
                if (arg.value == 0)
                    loops_printf(printer, "0");
                else
                {
                    uint32_t upper32 = ((uint64_t)arg.value) >> 32;
                    uint32_t lower32 = ((uint64_t)arg.value) & 0xffffffff;
                    if (upper32 > 0)
                        loops_printf(printer, "0x0%x%08x", upper32, lower32);
                    else
                        loops_printf(printer, "0x0%02x", lower32);
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
                    loops_printf(printer, "%srsp]", address_opener_brackets[opener_idx]);
                else
#if __LOOPS_OS == __LOOPS_WINDOWS
                    loops_printf(printer, "%srsp + 0%02xh]", address_opener_brackets[opener_idx], arg.value * 8);
#elif __LOOPS_OS == __LOOPS_LINUX
                    loops_printf(printer, "%srsp + 0x0%02x]", address_opener_brackets[opener_idx], arg.value * 8);
#else 
#error Unknown OS.
#endif
                break;
            }
            default:
                throw loops::exception(LOOPS_ERR_INCORRECT_ARGUMENT);
            };
            if(address)
            {
                if (address_end)
                    loops_printf(printer, "]");
                else
                    loops_printf(printer, " + ");
            }
            if (anum < aamount - 1 && !(address && !address_end))
                loops_printf(printer, ", ");
        }
        printer->close_printer_cell();
    }

    column_printer_ptr Intel64Backend::get_opargs_printer() const
    {
        return std::make_shared<intel64_opargs_printer>();
    }

    typedef struct pos_size_pair
    {
        int position;
        int size;
    } pos_size_pair;

    class intel64_hex_printer : public column_printer
    {
    public:
        intel64_hex_printer() : column_printer(&intel64_hex_printer::print) {}
        virtual ~intel64_hex_printer() {}
    private:
        static void print(program_printer* printer, column_printer* colprinter, const Syntfunc& func, int row);
        std::vector<pos_size_pair> pos_n_sizes;
        FuncBodyBuf binary;
    };

    void intel64_hex_printer::print(program_printer* printer, column_printer* colprinter, const Syntfunc& func, int row)
    {
        int program_size = (int)func.program.size();
        const Syntop* program = func.program.data();
        intel64_hex_printer* hex_printer = (intel64_hex_printer*)colprinter;
        if (hex_printer->pos_n_sizes.empty())
        {
            int oppos = 0;
            int opnum = 0;
            hex_printer->pos_n_sizes.resize(program_size);
            for (; opnum < program_size; opnum++)
            {
                int opsize = (int)printer->backend->lookS2b(program[opnum]).size();
                hex_printer->pos_n_sizes[opnum] = {/*position = */oppos, /*size = */opsize};
                oppos += opsize;
            }
            Assembly2Hex a2hPass(printer->backend);
            a2hPass.process(*((Syntfunc*)(nullptr)), func);
            hex_printer->binary = a2hPass.result_buffer();
        }
        const unsigned char* hexfield = hex_printer->binary->data() + hex_printer->pos_n_sizes[row].position;
        for (int pos = 0; pos < hex_printer->pos_n_sizes[row].size; pos++)
            loops_printf(printer, "%02x ", (unsigned)(*(hexfield + pos)));
        printer->close_printer_cell();
    }

    column_printer_ptr Intel64Backend::get_hex_printer() const
    {
        return std::make_shared<intel64_hex_printer>();
    }

    void Intel64BRASnippets::handle_reduce_sum32(Syntfunc& a_dest, const Arg& output, const Arg& input) const
    {
        a_dest.program.push_back(Syntop(VOP_X86_VPHADDD, { output, input, input }));
        a_dest.program.push_back(Syntop(VOP_X86_VPHADDD, { output, output, output }));
        Arg upperhalf = input;
        upperhalf.idx = a_dest.provideIdx(RB_VEC);
        a_dest.program.push_back(Syntop(VOP_X86_VEXTRACT128, { upperhalf, output, argIImm(1) }));
        a_dest.program.push_back(Syntop(VOP_ADD, { output, output, upperhalf }));
    }

    void Intel64BRASnippets::handle_mov_imm2vec(Syntfunc& a_dest, const Arg& output, int64_t input) const
    {
        LOOPS_ASSERT(output.tag == Arg::VREG && input != 0);
        Arg scalar = argReg(RB_INT, a_dest.provideIdx(RB_INT));
        a_dest.program.push_back(Syntop(OP_MOV, { scalar, argIImm(input) }));
        Arg onelane = output;
        onelane.idx = a_dest.provideIdx(RB_VEC);
        a_dest.program.push_back(Syntop(VOP_DEF, { onelane }));
        a_dest.program.push_back(Syntop(VOP_SETLANE, { onelane, argIImm(0), scalar }));
        a_dest.program.push_back(Syntop(VOP_BROADCAST, { output, onelane, argIImm(0) }));
    }

    void Intel64BRASnippets::handle_ultugt(Syntfunc& a_dest, const Syntop& op) const
    {
        int leftnum  = op.opcode == VOP_GT ? 2 : 1;
        int rightnum = op.opcode == VOP_GT ? 1 : 2;
        Arg sign_bit_changer = op.args[0];
        sign_bit_changer.idx = a_dest.provideIdx(RB_VEC);
        int64_t sign_bit_changer_val = op.args[0].elemtype == TYPE_U64 ? int64_t(0x8000000000000000) : 
                                        op.args[0].elemtype == TYPE_U32 ? int64_t(0x80000000) :
                                        op.args[0].elemtype == TYPE_U16 ? int64_t(0x8000) :
                                        /*op.args[0].elemtype == TYPE_U8 ?*/int64_t(0x80);
        handle_mov_imm2vec(a_dest, sign_bit_changer, sign_bit_changer_val);
        a_dest.program.push_back(Syntop(VOP_SUB, { op.args[0], op.args[leftnum], sign_bit_changer }));
        Arg altered2 = op.args[rightnum];
        altered2.idx = a_dest.provideIdx(RB_VEC);
        a_dest.program.push_back(Syntop(VOP_SUB, { altered2, op.args[rightnum], sign_bit_changer }));
        altered2.elemtype =  op.args[0].elemtype == TYPE_U64 ? TYPE_I64 : 
                                op.args[0].elemtype == TYPE_U32 ? TYPE_I32 :
                                op.args[0].elemtype == TYPE_U16 ? TYPE_I16 :
                            /*op.args[0].elemtype == TYPE_U8?*/ TYPE_I8;
        Arg arg0_signed = op.args[0];
        arg0_signed.elemtype = altered2.elemtype;
        a_dest.program.push_back(Syntop(VOP_GT, { op.args[0], altered2, arg0_signed }));
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
                LOOPS_ASSERT(op.size() == 2 && op[1].tag == Arg::IIMMEDIATE && (op[0].tag == Arg::IREG || op[0].tag == Arg::ISPILLED) &&
                       a_dest.program.size() && a_dest.program.back().opcode == OP_CMP);
                a_dest.program.insert(a_dest.program.end() - 1, Syntop(OP_MOV, { op[0], Arg(0) }));
                a_dest.program.push_back(op);
                break;
            case OP_MOV:
                if(op.args_size == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::IIMMEDIATE && op.args[1].value != 0)
                    handle_mov_imm2vec(a_dest, op.args[0], op.args[1].value);
                else
                    a_dest.program.push_back(op);
                break;
            case (VOP_NEG):
                LOOPS_ASSERT(op.args_size == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && op.args[0].elemtype == op.args[1].elemtype);
                {
                    Arg zero = op.args[0];
                    zero.idx = a_dest.provideIdx(RB_VEC);
                    a_dest.program.push_back(Syntop(OP_MOV, { zero, argIImm(0) }));
                    a_dest.program.push_back(Syntop(VOP_SUB, { op.args[0], zero, op.args[1] }));
                }
                break;
            case VOP_CAST:
                LOOPS_ASSERT(op.size() == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG);
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
                LOOPS_ASSERT(op.size() == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG);
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
                LOOPS_ASSERT(op.size() == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG);
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
                LOOPS_ASSERT(op.size() == 3 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && op.args[1].tag == Arg::VREG);
                if(op.args[1].elemtype == op.args[2].elemtype && isInteger(op.args[1].elemtype))
                {
                    a_dest.program.push_back(Syntop(VOP_EQ, { op.args[0], op.args[1], op.args[2] }));
                    Arg zero = op.args[0];
                    zero.idx = a_dest.provideIdx(RB_VEC);
                    a_dest.program.push_back(Syntop(OP_MOV, { zero, argIImm(0) }));
                    a_dest.program.push_back(Syntop(VOP_EQ, { op.args[0], op.args[0], zero}));
                }
                else
                    a_dest.program.push_back(op);
                break;
            case (VOP_LT):
            case (VOP_GT):
                if (op.args_size == 3 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && op.args[2].tag == Arg::VREG &&
                    op.args[0].elemtype == op.args[1].elemtype && op.args[0].elemtype == op.args[2].elemtype && isUnsignedInteger(op.args[0].elemtype))
                {
                    handle_ultugt(a_dest, op);
                }
                else
                    a_dest.program.push_back(op);
                break;
            case VOP_LE:
            case VOP_GE:
                LOOPS_ASSERT(op.size() == 3 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && op.args[1].tag == Arg::VREG);
                if(op.args[1].elemtype == op.args[2].elemtype && isInteger(op.args[1].elemtype))
                {
                    if(elem_size(op.args[1].elemtype) <= 4)
                    {
                        int leftnum  = op.opcode == VOP_GE ? 2 : 1;
                        int rightnum = op.opcode == VOP_GE ? 1 : 2;
                        Arg minres = op.args[1];
                        minres.idx = a_dest.provideIdx(RB_VEC);
                        a_dest.program.push_back(Syntop(VOP_MIN, { minres, op.args[rightnum], op.args[leftnum] }));
                        a_dest.program.push_back(Syntop(VOP_EQ, { op.args[0], op.args[leftnum], minres }));
                    }
                    else
                    {
                        Arg eq_res = op.args[0];
                        eq_res.idx = a_dest.provideIdx(RB_VEC);
                        a_dest.program.push_back(Syntop(VOP_EQ, { eq_res, op.args[1], op.args[2] }));
                        if(op.args[1].elemtype == TYPE_U64)
                        {
                            Syntop strict = op;
                            strict.opcode = op.opcode == VOP_LE ? VOP_LT : VOP_GT;
                            handle_ultugt(a_dest, strict);
                        }
                        else
                            a_dest.program.push_back(Syntop(op.opcode == VOP_LE ? VOP_LT : VOP_GT, { op.args[0], op.args[1], op.args[2] }));
                        a_dest.program.push_back(Syntop(VOP_OR, { op.args[0], op.args[0], eq_res }));
                    }
                }
                else
                    a_dest.program.push_back(op);
                break;
            case VOP_GETLANE:
            {
                LOOPS_ASSERT(op.size() == 3 && op.args[0].tag == Arg::IREG && op.args[1].tag == Arg::VREG && op.args[2].tag == Arg::IIMMEDIATE && op.args[2].value < m_backend->vlanes(op.args[1].elemtype));
                Arg halfvec = op.args[1];
                Arg halfidx = op.args[2];
                if(op.args[2].value >= m_backend->vlanes(op.args[1].elemtype)/2)
                {
                    halfvec.idx = a_dest.provideIdx(RB_VEC);
                    halfidx.value -= m_backend->vlanes(op.args[1].elemtype)/2;
                    a_dest.program.push_back(Syntop(isInteger(op.args[1].elemtype) ? VOP_X86_VEXTRACT128 : VOP_X86_VEXTRACT128, { halfvec, op.args[1], argIImm(1) }));
                }
                a_dest.program.push_back(Syntop(VOP_GETLANE, { op.args[0], halfvec, halfidx }));
                break;
            }
            case VOP_SETLANE:
            {
                LOOPS_ASSERT(op.size() == 3 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::IIMMEDIATE && op.args[2].tag == Arg::IREG && op.args[1].value < m_backend->vlanes(op.args[0].elemtype));
                Arg halfvec = op.args[0];
                Arg halfidx = op.args[1];
                bool returnHalfvec = false;
                if(op.args[1].value >= m_backend->vlanes(op.args[0].elemtype)/2)
                {
                    halfvec.idx = a_dest.provideIdx(RB_VEC);
                    halfidx.value -= m_backend->vlanes(op.args[0].elemtype)/2;
                    a_dest.program.push_back(Syntop(isInteger(op.args[0].elemtype) ? VOP_X86_VEXTRACT128 : VOP_X86_VEXTRACT128, { halfvec, op.args[0], argIImm(1) }));
                    returnHalfvec = true;
                }
                a_dest.program.push_back(Syntop(VOP_SETLANE, { halfvec, halfidx, op.args[2] }));
                if(returnHalfvec)
                    a_dest.program.push_back(Syntop(isInteger(op.args[0].elemtype) ? VOP_X86_VINSERT128 : VOP_X86_VINSERT128, { op.args[0], argIImm(1), op.args[0], halfvec }));
                break;
            }
            case VOP_CAST_HIGH:
            {
                LOOPS_ASSERT(op.size() == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && elem_size(op.args[0].elemtype) == 2 * elem_size(op.args[1].elemtype));
                Arg halfvec = op.args[1];
                halfvec.idx = a_dest.provideIdx(RB_VEC);
                a_dest.program.push_back(Syntop(isInteger(op.args[1].elemtype) ? VOP_X86_VEXTRACT128 : VOP_X86_VEXTRACT128, { halfvec, op.args[1], argIImm(1) }));
                a_dest.program.push_back(Syntop(VOP_CAST_LOW, { op.args[0],  halfvec }));
                break;
            }
            case VOP_NOT:
            {
                LOOPS_ASSERT(op.size() == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG);
                Arg allones = op.args[1]; 
                allones.elemtype = TYPE_U8;
                allones.idx = a_dest.provideIdx(RB_VEC);
                Arg scalar = argReg(RB_INT, a_dest.provideIdx(RB_INT));
                a_dest.program.push_back(Syntop(OP_MOV, { scalar, argIImm(0xff) }));
                a_dest.program.push_back(Syntop(VOP_DEF, { allones }));
                a_dest.program.push_back(Syntop(VOP_SETLANE, { allones, argIImm(0), scalar }));
                a_dest.program.push_back(Syntop(VOP_BROADCAST, { allones, allones, argIImm(0) }));
                allones.elemtype = op.args[1].elemtype;
                a_dest.program.push_back(Syntop(VOP_XOR, { op.args[0], op.args[1], allones }));
                break;                 
            }
            case VOP_EXT:
            {
                LOOPS_ASSERT(op.args_size == 4 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && op.args[2].tag == Arg::VREG &&
                       op.args[3].tag == Arg::IIMMEDIATE && op.args[0].elemtype == op.args[1].elemtype && op.args[0].elemtype == op.args[2].elemtype);
                int shift_in_bytes = elem_size(op.args[0].elemtype) * (int)op.args[3].value;
                LOOPS_ASSERT(shift_in_bytes >= 0 && shift_in_bytes <= 32);
                if(shift_in_bytes == 0)
                {
                    a_dest.program.push_back(Syntop(OP_MOV, { op.args[0], op.args[1] }));
                }
                else if(shift_in_bytes == 32)
                {
                    a_dest.program.push_back(Syntop(OP_MOV, { op.args[0], op.args[2] }));
                }
                else
                {
                    Arg center = op.args[0];
                    center.idx = shift_in_bytes == 16 ? center.idx : a_dest.provideIdx(RB_VEC);
                    a_dest.program.push_back(Syntop(VOP_X86_VPERM2I128, { center, op.args[1], op.args[2], argIImm(0x21) }));
                    if(shift_in_bytes < 16)
                        a_dest.program.push_back(Syntop(VOP_X86_VPALIGNR, { op.args[0], center, op.args[1], argIImm(shift_in_bytes)}));
                    if(shift_in_bytes > 16)
                        a_dest.program.push_back(Syntop(VOP_X86_VPALIGNR, { op.args[0], op.args[2], center, argIImm(shift_in_bytes - 16)}));
                } 
                break;
            }
            case (VOP_REDUCE_SUM):
                if(op.args_size == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && op.args[0].elemtype == op.args[1].elemtype && op.args[0].elemtype == TYPE_FP32)
                {
                    //op[0] = {op[1][0] + op[1][1], op[1][2] + op[1][3], -//-, -//-, op[1][4] + op[1][5], op[1][6] + op[1][7], -//-, -//-,};
                    a_dest.program.push_back(Syntop(VOP_X86_VHADDPS, { op.args[0], op.args[1], op.args[1] }));
                    //op[0] = {op[1][0] + op[1][1] + op[1][2] + op[1][3], -//-, -//-, -//-, op[1][4] + op[1][5] + op[1][6] + op[1][7], ...};
                    a_dest.program.push_back(Syntop(VOP_X86_VHADDPS, { op.args[0], op.args[0], op.args[0] }));
                    Arg upperhalf = op.args[0]; 
                    upperhalf.idx = a_dest.provideIdx(RB_VEC);
                    a_dest.program.push_back(Syntop(VOP_X86_VEXTRACT128, { upperhalf, op.args[0], argIImm(1) }));
                    a_dest.program.push_back(Syntop(VOP_X86_VADDSS, { op.args[0], op.args[0], upperhalf }));
                }
                else if(op.args_size == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && op.args[0].elemtype == op.args[1].elemtype && op.args[0].elemtype == TYPE_FP64)
                {
                    //op[0] = {op[1][0] + op[1][1], -//-, op[1][2] + op[1][3],  -//-, };
                    a_dest.program.push_back(Syntop(VOP_X86_VHADDPD, { op.args[0], op.args[1], op.args[1] }));
                    Arg upperhalf = op.args[0]; 
                    upperhalf.idx = a_dest.provideIdx(RB_VEC);
                    a_dest.program.push_back(Syntop(VOP_X86_VEXTRACT128, { upperhalf, op.args[0], argIImm(1) }));
                    a_dest.program.push_back(Syntop(VOP_X86_VADDSD, { op.args[0], op.args[0], upperhalf }));
                }
                else if(op.args_size == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && op.args[0].elemtype == op.args[1].elemtype && (op.args[0].elemtype == TYPE_I64 || op.args[0].elemtype == TYPE_U64))
                {
                    Arg upperhalf = op.args[1]; 
                    upperhalf.idx = a_dest.provideIdx(RB_VEC);
                    a_dest.program.push_back(Syntop(VOP_X86_VEXTRACT128, { upperhalf, op.args[1], argIImm(1) }));
                    a_dest.program.push_back(Syntop(VOP_ADD, { upperhalf, upperhalf, op.args[1] }));
                    Arg upperhalf32 = upperhalf; upperhalf32.elemtype = TYPE_U32;
                    Arg arg0_32 = op.args[0]; arg0_32.elemtype = TYPE_U32;
                    a_dest.program.push_back(Syntop(VOP_X86_VPSHUFD, { arg0_32, upperhalf32, argIImm(238) }));
                    a_dest.program.push_back(Syntop(VOP_ADD, { op.args[0], op.args[0], upperhalf }));
                }
                else if(op.args_size == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && op.args[0].elemtype == op.args[1].elemtype && (op.args[0].elemtype == TYPE_I32 || op.args[0].elemtype == TYPE_U32))
                {
                    handle_reduce_sum32(a_dest, op.args[0], op.args[1]);
                }
                else
                    a_dest.program.push_back(op);
                break;
            case (VOP_REDUCE_WSUM):
                if(op.args_size == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && 
                    ((op.args[0].elemtype == TYPE_I32 && op.args[1].elemtype == TYPE_I16) || 
                    (op.args[0].elemtype == TYPE_U32 && op.args[1].elemtype == TYPE_U16)))
                {
                    Arg upperhalf16 = op.args[1];
                    upperhalf16.idx = a_dest.provideIdx(RB_VEC);
                    a_dest.program.push_back(Syntop(VOP_X86_VEXTRACT128, { upperhalf16, op.args[1], argIImm(1) }));
                    Arg lowerhalf = op.args[0];
                    lowerhalf.idx = a_dest.provideIdx(RB_VEC);
                    a_dest.program.push_back(Syntop(VOP_CAST_LOW, { lowerhalf, op.args[1] }));
                    handle_reduce_sum32(a_dest, lowerhalf, lowerhalf);
                    Arg upperhalf = op.args[0];
                    upperhalf.idx = a_dest.provideIdx(RB_VEC);
                    a_dest.program.push_back(Syntop(VOP_CAST_LOW, { upperhalf, upperhalf16 }));
                    handle_reduce_sum32(a_dest, upperhalf, upperhalf);
                    a_dest.program.push_back(Syntop(VOP_ADD, { op.args[0], lowerhalf, upperhalf }));
                }
                else if(op.args_size == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::VREG && 
                    ((op.args[0].elemtype == TYPE_U16 && op.args[1].elemtype == TYPE_U8) ||
                     (op.args[0].elemtype == TYPE_I16 && op.args[1].elemtype == TYPE_I8)))
                {
                    Arg zero = op.args[1];
                    zero.idx = a_dest.provideIdx(RB_VEC);
                    zero.elemtype = TYPE_U8;
                    a_dest.program.push_back(Syntop(OP_MOV, { zero, argIImm(0) }));
                    Arg sumabs_arg = op.args[1];
                    if(op.args[0].elemtype == TYPE_I16)
                    {
                        Arg m128 = zero;
                        m128.idx = a_dest.provideIdx(RB_VEC);
                        handle_mov_imm2vec(a_dest, m128, 0x80);
                        sumabs_arg.idx = a_dest.provideIdx(RB_VEC);
                        a_dest.program.push_back(Syntop(VOP_XOR, { sumabs_arg, op.args[1], m128 }));
                        sumabs_arg.elemtype = TYPE_U8;
                    }
                    Arg sumabs = op.args[0];
                    sumabs.idx = a_dest.provideIdx(RB_VEC);
                    sumabs.elemtype = TYPE_U32;
                    a_dest.program.push_back(Syntop(VOP_X86_VPSADBW, { sumabs, sumabs_arg, zero }));
                    Arg upperhalf = sumabs;
                    upperhalf.idx = a_dest.provideIdx(RB_VEC);
                    a_dest.program.push_back(Syntop(VOP_X86_VEXTRACT128, { upperhalf, sumabs, argIImm(1) }));
                    a_dest.program.push_back(Syntop(VOP_ADD, { sumabs, sumabs, upperhalf }));
                    Arg arg0_32 = op.args[0];
                    arg0_32.elemtype = TYPE_U32;
                    Arg shuffled = arg0_32;
                    shuffled.idx = a_dest.provideIdx(RB_VEC);
                    arg0_32.elemtype = TYPE_U32;
                    a_dest.program.push_back(Syntop(VOP_X86_VPSHUFD, { shuffled, sumabs, argIImm(238) }));
                    a_dest.program.push_back(Syntop(VOP_ADD, { arg0_32, sumabs, shuffled }));
                    if(op.args[0].elemtype == TYPE_I16)
                    {
                        Arg scalar = argReg(RB_INT, a_dest.provideIdx(RB_INT));
                        a_dest.program.push_back(Syntop(OP_MOV, { scalar, argIImm(4096) }));
                        Arg p4096 = arg0_32;
                        p4096.idx = a_dest.provideIdx(RB_VEC);
                        a_dest.program.push_back(Syntop(VOP_DEF, { p4096 }));
                        a_dest.program.push_back(Syntop(VOP_SETLANE, { p4096, argIImm(0), scalar }));
                        a_dest.program.push_back(Syntop(VOP_SUB, { arg0_32, arg0_32, p4096 }));
                    }
                }
                else
                    a_dest.program.push_back(op);
                break;
            case (VOP_BROADCAST):
                if(op.args_size == 2 && op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::IREG)
                {
                    a_dest.program.push_back(Syntop(VOP_DEF, { op.args[0] }));
                    a_dest.program.push_back(Syntop(VOP_SETLANE, { op.args[0], argIImm(0), op.args[1] }));
                    a_dest.program.push_back(Syntop(VOP_BROADCAST, { op.args[0], op.args[0], argIImm(0) }));
                }
                else
                    a_dest.program.push_back(op);
                break;
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
        for (int opnum = 0; opnum < (int)a_source.program.size(); opnum++)
        {
            const Syntop& op = a_source.program[opnum];
            switch (op.opcode)
            {
            case OP_MOV:
                LOOPS_ASSERT(op.size() == 2); 
                if(op.args[0].tag == Arg::VREG && op.args[1].tag == Arg::IIMMEDIATE && op.args[1].value == 0)
                {
                    Arg nullified = op.args[0];
                    nullified.elemtype = TYPE_U32; 
                    a_dest.program.push_back(Syntop(VOP_XOR, { nullified, nullified, nullified }));
                }
                else if(!(((op[0].tag == Arg::IREG && op[1].tag == Arg::IREG ) || //This is not about snippets, its ommiting parasite self-assignments.
                        (op[0].tag == Arg::VREG && op[1].tag == Arg::VREG ))
                        && op[0].idx == op[1].idx))
                    a_dest.program.push_back(op);
                break;
            case OP_AND:
            case OP_OR:
            case OP_XOR:
            case OP_X86_ADC:
            case OP_ADD:
            case OP_MUL:
            {
                Syntop op_ = op;
                LOOPS_ASSERT(op_.size() == 3 && regOrSpi(op_[0]));
                if (op_[1].tag == Arg::IIMMEDIATE)
                    std::swap(op_[1], op_[2]);
                LOOPS_ASSERT(regOrSpi(op_[1]));
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
                LOOPS_ASSERT(op.size() == 3 && regOrSpi(op[0]) && (regOrSpi(op[1])||regOrSpi(op[2])));
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
                LOOPS_ASSERT(op.size() == 3 && regOrSpi(op[0]) && regOrSpi(op[1]));
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
                LOOPS_ASSERT(op.size() == 3 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG && regOrSpi(op[2]));
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
                LOOPS_ASSERT(op_.size() == 2 && regOrSpi(op_[0]) && regOrSpi(op_[1]));
                if (!regOrSpiEq(op_[0], op_[1]))
                {
                    a_dest.program.push_back(Syntop(OP_MOV, { op_[0],op_[1] }));
                    op_[1] = op_[0];
                }
                a_dest.program.push_back(op_);
                break;
            }
            case OP_SELECT:
                LOOPS_ASSERT(op.size() == 4 && op[0].tag == Arg::IREG && regOrSpi(op[2]) && op[3].tag == Arg::IREG);
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
                LOOPS_ASSERT(op_.size() == 3 && op_[0].tag == Arg::IREG && regOrSpi(op_[1]) && regOrSpi(op_[2]));
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
                LOOPS_ASSERT(op.size() == 2 && op[0].tag == Arg::IREG && regOrSpi(op[1]));
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
                LOOPS_ASSERT(op.size() == 2 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG);
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
            {
                Syntop op_= op;
                LOOPS_ASSERT(op_.size() == 4 && op_[0].tag == Arg::VREG && op_[1].tag == Arg::VREG && op_[2].tag == Arg::VREG && op_[3].tag == Arg::VREG);
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
                std::vector<int> parameterRegisters = this->m_backend->parameterRegisters(RB_INT);
                std::vector<int> returnRegisters = this->m_backend->returnRegisters(RB_INT);
                std::vector<int> callerSavedRegisters = this->m_backend->callerSavedRegisters(RB_INT);
                std::set<int> allSaved;
                allSaved.insert(parameterRegisters.begin(), parameterRegisters.end());
                allSaved.insert(returnRegisters.begin(), returnRegisters.end());
                allSaved.insert(callerSavedRegisters.begin(), callerSavedRegisters.end());

                int64_t offsetV = (allSaved.size() + 3)/4;
                std::vector<int> parameterRegistersV = this->m_backend->parameterRegisters(RB_VEC);
                std::vector<int> returnRegistersV = this->m_backend->returnRegisters(RB_VEC);
                std::vector<int> callerSavedRegistersV = this->m_backend->callerSavedRegisters(RB_VEC);
                std::set<int> allSavedV;
                allSavedV.insert(parameterRegistersV.begin(), parameterRegistersV.end());
                allSavedV.insert(returnRegistersV.begin(), returnRegistersV.end());
                allSavedV.insert(callerSavedRegistersV.begin(), callerSavedRegistersV.end());

                LOOPS_ASSERT((op.opcode == OP_CALL && op.size() >= 2 && op.size() <= ((int)parameterRegisters.size() + 2)) ||
                       (op.opcode == OP_CALL_NORET && op.size() >= 1 && op.size() <= ((int)parameterRegisters.size() + 1)));
#if __LOOPS_OS == __LOOPS_WINDOWS
                Arg sp = argReg(RB_INT, RSP);
#endif        
                int retidx = op.opcode == OP_CALL ? op[0].idx : 0;

                Arg addrkeeper = op.opcode == OP_CALL ? op[1]: op[0];
                size_t addrkeeper_spilled = size_t(-1);
                //1.) Save registers
                {
                    auto iter = allSaved.begin();
                    for(int i = 0; i < (int)allSaved.size(); i++, iter++)
                    {
                        a_dest.program.push_back(Syntop(OP_SPILL, { argIImm(i), argReg(RB_INT,  *iter)}));
                        if(*iter == addrkeeper.idx)
                            addrkeeper_spilled = i;
                    }
                    iter = allSavedV.begin();
                    for(int i = 0; i < (int)allSavedV.size(); i++, iter++)
                        a_dest.program.push_back(Syntop(OP_SPILL, { argIImm((offsetV + i) * 4), argReg(RB_VEC,  *iter)}));
                }
                //2.) Prepare arguments accordingly to ABI. Call address must not be broken
                //TODO(ch) : make this algo optimized with help of permutation analysis.
                std::set<int> brokenRegs;
                for(int fargnum = (op.opcode == OP_CALL ? 2 : 1); fargnum < op.size(); fargnum++)
                {
                    LOOPS_ASSERT(op[fargnum].tag == Arg::IREG);
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
                            LOOPS_ASSERT(spillPos < (int)allSaved.size());
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
                //5.) Restore registers
                {
                    auto iter = allSaved.begin();
                    for(int i = 0; i < (int)allSaved.size(); i++, iter++)
                        if(op.opcode == OP_CALL_NORET || *iter != retidx)
                            a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT,  *iter), argIImm(i)}));
                    iter = allSavedV.begin();
                    for(int i = 0; i < (int)allSavedV.size(); i++, iter++)
                        a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_VEC,  *iter), argIImm((offsetV + i) * 4)}));
                }

                break;
            }
            default:
                a_dest.program.push_back(op);
                break;
            }
        }
    }
}
#endif // __LOOPS_ARCH == __LOOPS_INTEL64
