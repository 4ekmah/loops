/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "backend_aarch64.hpp"
#if __LOOPS_ARCH == __LOOPS_AARCH64
#include "composer.hpp"
#include "func_impl.hpp"
#include <unordered_map>
#include <iomanip>
/*
The file uses bits (related to ARM machine code encoding) from LLVM project, licensed under
Apache 2 license. Please, see https://github.com/llvm/llvm-project/blob/main/llvm/LICENSE.TXT for details.
*/

LOOPS_HASHMAP_STATIC(int, loops_cstring) opstrings_[] = 
{
                  /*  |       enum_id       |string_id|    */
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LDR   , "ldr"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LDRSW , "ldrsw" ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LDRH  , "ldrh"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LDRSH , "ldrsh" ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LDRB  , "ldrb"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LDRSB , "ldrsb" ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LDP   , "ldp"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_STR   , "str"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_STRH  , "strh"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_STRB  , "strb"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_STP   , "stp"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_MOV   , "mov"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_MOVN  , "movn"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_MOVK  , "movk"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_ADD   , "add"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_SUB   , "sub"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_MUL   , "mul"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_SDIV  , "sdiv"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LSL   , "lsl"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LSR   , "lsr"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_ASR   , "asr"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_AND   , "and"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_ORR   , "orr"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_EOR   , "eor"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_NEG   , "neg"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_MVN   , "mvn"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_BSL   , "bsl"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CMP   , "cmp"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CSEL  , "csel"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CSET  , "cset"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CINC  , "cinc"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CNEG  , "cneg"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FADD  , "fadd"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FSUB  , "fsub"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FMUL  , "fmul"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FDIV  , "fdiv"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FNEG  , "fneg"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FMLA  , "fmla"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_SHL   , "shl"   ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_USHL  , "ushl"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_SSHL  , "sshl"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_USHR  , "ushr"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_SSHR  , "sshr"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_MOVI  , "movi"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_MVNI  , "mvni"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CMHI  , "cmhi"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CMHS  , "cmhs"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CMEQ  , "cmeq"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CMGE  , "cmge"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CMGT  , "cmgt"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CMLE  , "cmle"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CMLT  , "cmlt"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FCMGT , "fcmgt" ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FCMGE , "fcmge" ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FCMEQ , "fcmeq" ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FMIN  , "fmin"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FMAX  , "fmax"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_SMINV , "sminv" ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_SMAXV , "smaxv" ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_UMINV , "uminv" ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_UMAXV , "umaxv" ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FMINV , "fminv" ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FMAXV , "fmaxv" ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_ADDV  , "addv"  ),
    LOOPS_HASHMAP_ELEM(loops::AARCH64_SADDLV, "saddlv"), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_UADDLV, "uaddlv"), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FCVTZS, "fcvtzs"), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FCVTZU, "fcvtzu"), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FCVTMS, "fcvtms"), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_FCVTMU, "fcvtmu"), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_SCVTF , "scvtf" ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_UCVTF , "ucvtf" ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LD1   , "ld1"   ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LD2   , "ld2"   ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LD4   , "ld4"   ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_ST1   , "st1"   ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_ST4   , "st4"   ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_EXT   , "ext"   ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_DUP   , "dup"   ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_UMOV  , "umov"  ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_INS   , "ins"   ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_SSHLL , "sshll" ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_SSHLL2, "sshll2"), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_USHLL , "ushll" ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_USHLL2, "ushll2"), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_XTN   , "xtn"   ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_XTN2  , "xtn2"  ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_CNT   , "cnt"   ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_B     , "b"     ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_B_NE  , "b.ne"  ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_B_EQ  , "b.eq"  ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_B_LT  , "b.lt"  ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_B_GT  , "b.gt"  ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_B_HI  , "b.hi"  ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_B_GE  , "b.ge"  ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_B_LE  , "b.le"  ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_B_LS  , "b.ls"  ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_BLR   , "blr"   ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_RET   , "ret"   ), 
    LOOPS_HASHMAP_ELEM(loops::AARCH64_LABEL , ""      ),
};


static LOOPS_HASHMAP(int, loops_cstring) opstrings = NULL;

int backend_aarch64_h_initialize()
{
    LOOPS_CALL_THROW(loops_hashmap_construct_static(&opstrings, opstrings_, sizeof(opstrings_) / sizeof(opstrings_[0])));
    return LOOPS_ERR_SUCCESS;
}

void backend_aarch64_h_deinitialize()
{
    loops_hashmap_destruct(opstrings);
}

namespace loops
{

enum Aarch64Reg
{
    R0   =  0,
    R1   =  1,
    R2   =  2,
    R3   =  3,
    R4   =  4,
    R5   =  5,
    R6   =  6,
    R7   =  7,
    XR   =  8,
    R9   =  9,
    R10  = 10,
    R11  = 11,
    R12  = 12,
    R13  = 13,
    R14  = 14,
    R15  = 15,
    IP0  = 16,
    IP1  = 17,
    PR   = 18,
    R19  = 19,
    R20  = 20,
    R21  = 21,
    R22  = 22,
    R23  = 23,
    R24  = 24,
    R25  = 25,
    R26  = 26,
    R27  = 27,
    R28  = 28,
    FP   = 29,
    LR   = 30,
    SP   = 31
};

enum Aarch64VReg
{
    Q0   =  0,
    Q1   =  1,
    Q2   =  2,
    Q3   =  3,
    Q4   =  4,
    Q5   =  5,
    Q6   =  6,
    Q7   =  7,
    Q8   =  8,
    Q9   =  9,
    Q10   =  10,
    Q11   =  11,
    Q12   =  12,
    Q13   =  13,
    Q14   =  14,
    Q15   =  15,
    Q16   =  16,
    Q17   =  17,
    Q18   =  18,
    Q19   =  19,
    Q20   =  20,
    Q21   =  21,
    Q22   =  22,
    Q23   =  23,
    Q24   =  24,
    Q25   =  25,
    Q26   =  26,
    Q27   =  27,
    Q28   =  28,
    Q29   =  29,
    Q30   =  30,
    Q31   =  31,
};

enum AArch64IC
{
    AARCH64_IC_EQ = 0b0000,
    AARCH64_IC_NE = 0b0001,
    AARCH64_IC_LT = 0b1011,
    AARCH64_IC_GT = 0b1100,
    AARCH64_IC_HI = 0b1000,
    AARCH64_IC_LE = 0b1101,
    AARCH64_IC_LS = 0b1001,
    AARCH64_IC_GE = 0b1010,
    AARCH64_IC_MI = 0b0100,
    AARCH64_IC_PL = 0b0101,
};

bool encodeImmShift(int64_t shift, int etyp, uint64_t& immh, uint64_t& immb)
{
    static uint64_t immhHi[] = {0b0001, 0b0001, 0b0010, 0b0010, 0b0100, 0b0100, 0b1000, 0b1000};
    immh = immhHi[etyp];
    uint64_t ashift = std::abs(shift);
    if( (((immh << 3) - 1) & ashift) != ashift ) //Shift doesn't fit(e.g. (byte << 8) cannot be implemented[and doesn't have any sense])
        return false;
    immh |= (shift & ((immh - 1) << 3)) >> 3;
    immb = shift & 0b111;
    return true;
}

static inline int IC_IR2Aarch64(int ic)
{
    return ic == OP_NE  ? AARCH64_IC_NE : (
           ic == OP_EQ  ? AARCH64_IC_EQ : (
           ic == OP_GE  ? AARCH64_IC_GE : (
           ic == OP_LE  ? AARCH64_IC_LE : (
           ic == OP_ULE ? AARCH64_IC_LS : (
           ic == OP_GT  ? AARCH64_IC_GT : (
           ic == OP_UGT ? AARCH64_IC_HI : (
           ic == OP_LT  ? AARCH64_IC_LT : (
           ic == OP_S   ? AARCH64_IC_MI : (
         /*ic == OP_NS?*/AARCH64_IC_PL /* : throw error*/)))))))));
}

static inline int invertAarch64IC(int ic) { return ic^0b1; }

BinTranslation a64BTLookup(const Syntop& index, bool& scs)
{
    static const uint64_t intSizeStats[] = {0b00, 0b00, 0b01, 0b01, 0b10, 0b10, 0b11, 0b11};
    using namespace BinTranslationConstruction;
    scs = true;
    switch (index.opcode)
    {
    case (AARCH64_LDR):
        if(index.size() == 3)
        {
            if (index[0].tag == Arg::VREG)
            {
                if(index[1].tag != Arg::IREG)
                    break;
                if (index[2].tag == Arg::IREG)
                    return BiT({ BTsta(0b00111100111, 11), BTreg(2, 5, In), BTsta(0b011010, 6), BTreg(1, 5, In | Addr), BTreg(0, 5, Out) });
                if (index[2].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0b0011110111, 10), BTimm(2, 12), BTreg(1, 5, In | Addr), BTreg(0, 5, Out) });
            }
            else if(index[0].tag == Arg::IREG)
            {
                if (index[2].tag == Arg::IREG)
                {
                    uint64_t stat = elem_size(index[0].elemtype) <= 4 ? 0b10111000011 : 0b11111000011;
                    return BiT({ BTsta(stat, 11), BTreg(2, 5, In), BTsta(0b011010, 6), BTreg(1, 5, In | Addr), BTreg(0, 5, Out) });
                }
                else if (index[2].tag == Arg::IIMMEDIATE)
                {
                    uint64_t stat = elem_size(index[0].elemtype) <= 4 ? 0b1011100101 : 0b1111100101;
                    return BiT({ BTsta(stat, 10), BTimm(2, 12), BTreg(1, 5, In | Addr), BTreg(0, 5, Out) });
                }
            }
        }
        break;
    case (AARCH64_LDRSW):
        if (index.size() == 3 && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b10111000101, 11), BTreg(2,5,In), BTsta(0b011010, 6), BTreg(1,5,In | Addr), BTreg(0, 5, Out | Eff64) });
        else if (index.size() == 3 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b1011100110, 10), BTimm(2, 12), BTreg(1, 5, In | Addr), BTreg(0, 5, Out | Eff64) });
        break;
    case (AARCH64_LDRH):
        if (index.size() == 3 && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b01111000011, 11), BTreg(2, 5, In), BTsta(0b011010, 6), BTreg(1, 5, In | Addr), BTreg(0, 5, Out) });
        else if (index.size() == 3 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b0111100101, 10), BTimm(2, 12), BTreg(1, 5, In | Addr), BTreg(0, 5, Out) });
        break;
    case (AARCH64_LDRSH):
        if (index.size() == 3 && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b01111000101, 11), BTreg(2,5,In), BTsta(0b011010, 6), BTreg(1,5,In | Addr), BTreg(0, 5, Out | Eff64) });
        else if (index.size() == 3 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b0111100110, 10), BTimm(2,12), BTreg(1,5,In | Addr), BTreg(0,5,Out | Eff64) });
        break;
    case (AARCH64_LDRB):
        if (index.size() == 3 && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b00111000011, 11), BTreg(2,5,In), BTsta(0b011010, 6), BTreg(1,5,In | Addr), BTreg(0,5,Out) });
        else if (index.size() == 3 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b0011100101, 10), BTimm(2,12), BTreg(1,5,In | Addr), BTreg(0,5,Out) });
        break;
    case (AARCH64_LDRSB):
        if (index.size() == 3 && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b00111000101, 11), BTreg(2,5,In), BTsta(0b011010, 6), BTreg(1,5,In | Addr), BTreg(0,5,Out | Eff64) });
        else if (index.size() == 3 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b0011100110, 10), BTimm(2,12), BTreg(1,5,In | Addr), BTreg(0,5,Out | Eff64) });
        break;
    case (AARCH64_STR):
        if(index.size() == 3)
        {
            if (index[0].tag == Arg::VREG)
            {
                if(index[1].tag != Arg::IREG)
                    break;
                if(index[2].tag == Arg::IREG)
                    return BiT({ BTsta(0b00111100101, 11), BTreg(2, 5, In), BTsta(0b011010, 6), BTreg(1, 5, In | Addr), BTreg(0, 5, In) });
                else if(index[2].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0b0011110110, 10), BTimm(2, 12), BTreg(1, 5, In | Addr ), BTreg(0, 5, In) });
            }
            else if(index[0].tag == Arg::IREG)
            {
                if (index[2].tag == Arg::IREG)
                {
                    uint64_t stat = elem_size(index[0].elemtype) <= 4 ? 0b10111000001 : 0b11111000001;
                    return BiT({ BTsta(stat,  11), BTreg(2, 5, In), BTsta(0b011010, 6), BTreg(1, 5, In | Addr), BTreg(0, 5, In) });
                }
                else if (index[2].tag == Arg::IIMMEDIATE)
                {
                    uint64_t stat = elem_size(index[0].elemtype) <= 4 ? 0b1011100100 : 0b1111100100;
                    return BiT({ BTsta(stat,  10), BTimm(2, 12), BTreg(1, 5, In | Addr), BTreg(0, 5, In) });
                }
            }
        }
        break;
    case (AARCH64_STRH):
        if (index.size() == 3 && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b01111000001, 11), BTreg(2, 5, In), BTsta(0b011010, 6), BTreg(1, 5, In | Addr), BTreg(0, 5, In) });
        else if (index.size() == 3 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b0111100100, 10), BTimm(2, 12), BTreg(1, 5, In | Addr), BTreg(0, 5, In) });
        break;
    case (AARCH64_STRB):
        if (index.size() == 3 && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b00111000001, 11), BTreg(2, 5, In), BTsta(0b011010, 6), BTreg(1, 5, In | Addr), BTreg(0, 5, In) });
        else if (index.size() == 3 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b0011100100, 10), BTimm(2, 12), BTreg(1, 5, In | Addr), BTreg(0, 5, In) });
        break;
    case (AARCH64_MOV):
        if (index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG)
            return BiT({ BTsta(0x550,11), BTreg(1, 5, In), BTsta(0x1F ,11), BTreg(0, 5, Out) });
        else if (index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0x694,11), BTimm(1, 16), BTreg(0, 5, Out) });
        else if (index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG)
            return BiT({ BTsta(0b01001110101, 11), BTreg(1, 5, In), BTsta(0b000111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_MOVN):
        if(index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b10010010100, 11), BTimm(1, 16), BTreg(0, 5, Out) });
        break;
    case (AARCH64_MOVK):
        if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IIMMEDIATE &&
           index[2].value >= 1 && index[2].value <= 3)
            return BiT({ BTsta(0b111100101, 9), BTimm(2, 2), BTimm(1, 16), BTreg(0, 5, IO) });
        break;
    case (AARCH64_ADD):
        if (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0x458, 11), BTreg(2, 5, In), BTsta(0x0, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IIMMEDIATE && index[2].value >= 0)
            return BiT({ BTsta(0x244, 10), BTimm(2, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
                 index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype && isInteger(index[0].elemtype))
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01001110,8), BTsta(sizeStat,2), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b100001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_SUB):
        if (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0x658,11), BTreg(2, 5, In), BTsta(0x0,6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IIMMEDIATE && index[2].value >= 0 )
            return BiT({ BTsta(0x344,10), BTimm(2, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
                 index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype && isInteger(index[0].elemtype))
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01101110,8), BTsta(sizeStat,2), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b100001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_MUL):
        if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0x4D8,11), BTreg(2, 5, In), BTsta(0x1F, 6), BTreg(1, 5, In), BTreg(0,5,Out) });
        else if (index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
                 index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype && isInteger(index[0].elemtype) && index[0].elemtype <= TYPE_I32)
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01001110,8), BTsta(sizeStat,2), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b100111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_SDIV):
        if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0x4D6,11), BTreg(2, 5, In), BTsta(0x3, 6), BTreg(1, 5, In), BTreg(0,5,Out) });
        break;
    case (AARCH64_LSL):
        if (index.size() == 3 && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b10011010110, 11), BTreg(2, 5, In), BTsta(0b001000, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index.size() == 3 && index[2].tag == Arg::IIMMEDIATE)
        {
            int64_t shift = index[2].value;
            if(shift >= 0 && shift < 64)
                return BiT({ BTsta(0b1101001101, 10), BTsta((64-shift) % 64, 6), BTsta(63 - shift, 6), BTreg(1, 5, In), BTreg(0, 5, Out), BTomm(2) });
        }
        break;
    case (AARCH64_LSR):
        if (index.size() == 3 && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b10011010110, 11), BTreg(2, 5, In), BTsta(0b001001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index.size() == 3 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b1101001101, 10), BTimm(2, 6), BTsta(0b111111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_ASR):
        if (index.size() == 3 && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b10011010110, 11), BTreg(2, 5, In), BTsta(0b001010, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index.size() == 3 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b1001001101, 10), BTimm(2, 6), BTsta(0b111111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_AND):
        if (index.size() == 3)
        {
            if(index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG)
                return BiT({ BTsta(0b10001010000, 11), BTreg(2, 5, In), BTsta(0b000000, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            else if(index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG)
                return BiT({ BTsta(0b01001110001, 11), BTreg(2, 5, In), BTsta(0b000111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        else if (index.size() == 5 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b100100100, 9), BTimm(2, 1), BTimm(3, 6), BTimm(4, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_ORR):
        if (index.size() == 3)
        {
            if(index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG)
                return BiT({ BTsta(0b10101010000, 11), BTreg(2, 5, In), BTsta(0b000000, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            else if(index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG)
                return BiT({ BTsta(0b01001110101, 11), BTreg(2, 5, In), BTsta(0b000111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        else if (index.size() == 5 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b101100100, 9), BTimm(2, 1), BTimm(3, 6), BTimm(4, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_EOR):
        if (index.size() == 3)
        {
            if(index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG)
                return BiT({ BTsta(0b11001010000, 11), BTreg(2, 5, In), BTsta(0b000000, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            else if(index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG)
                return BiT({ BTsta(0b01101110001, 11), BTreg(2, 5, In), BTsta(0b000111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        else if (index.size() == 5 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b110100100, 9), BTimm(2, 1), BTimm(3, 6), BTimm(4, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_MVN):
        if (index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG)
            return BiT({ BTsta(0b10101010001, 11), BTreg(1, 5, In), BTsta(0b00000011111, 11), BTreg(0, 5, Out) });
        else if (index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype &&
                 isInteger(index[0].elemtype))
            return BiT({ BTsta(0b0110111000100000010110, 22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_BSL):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
           elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && index[1].elemtype == index[2].elemtype)
            return BiT({ BTsta(0b01101110011, 11), BTreg(2, 5, In), BTsta(0b000111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_NEG):
        if (index.size() == 2)
        {
            if(index[0].tag == Arg::IREG && index[1].tag == Arg::IREG)
                return BiT({ BTsta(0b11001011000, 11), BTreg(1, 5, In), BTsta(0b00000011111, 11), BTreg(0, 5, Out) });
            else if(index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && isSignedInteger(index[0].elemtype))
            {
                uint64_t sizeStat = intSizeStats[index[0].elemtype];
                return BiT({ BTsta(0b01101110, 8), BTsta(sizeStat, 2), BTsta(0b100000101110, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
        }
        break;
    case (AARCH64_CMP):
        if(index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG)
            return BiT({ BTsta(0x758,11), BTreg(1, 5, In), BTsta(0x0,6), BTreg(0, 5, In), BTsta(0x1F, 5) });
        if(index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[1].value >= 0)
            return BiT({ BTsta(0b1111000100, 10), BTimm(1, 12),  BTreg(0, 5, In), BTsta(0b11111,5)});
        break;
    case (AARCH64_CSEL):
        if (index.size() == 4 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG && index[3].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b10011010100, 11), BTreg(2, 5, In),  BTimm(3, 4), BTsta(0b00,2), BTreg(1, 5, In), BTreg(0, 5, Out)});
        break;
    case (AARCH64_CSET):
        if (index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b1001101010011111, 16), BTimm(1,4), BTsta(0b0111111, 7), BTreg(0,5,Out) });
        break;
    case (AARCH64_CINC): return BiT({ BTsta(0b10011010100, 11), BTreg(1,5,In), BTimm(2,4), BTsta(0b01, 2), BTreg(1,5,In), BTreg(0,5,Out) });
    case (AARCH64_CNEG): return BiT({ BTsta(0b11011010100, 11), BTreg(1,5,In), BTimm(2,4), BTsta(0b01, 2), BTreg(1,5,In), BTreg(0,5,Out) });
    case (AARCH64_LDP):
        if (index.size() == 4 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG && index[3].tag == Arg::IIMMEDIATE && index[2].idx != index[0].idx && index[2].idx != index[1].idx && index[0].idx != index[1].idx)
            return BiT({ BTsta(0b1010100101, 10), BTimm(3,7), BTreg(1,5,Out), BTreg(2,5,In), BTreg(0,5,Out) });
        break;
    case (AARCH64_STP):
        if (index.size() == 4 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG && index[3].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b1010100100, 10), BTimm(3,7), BTreg(1,5,In), BTreg(2,5,In), BTreg(0,5,In) });
        break;
    case (AARCH64_FADD):
        if (index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
                 index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype)
        {
            if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[0].elemtype == TYPE_FP64 ? 1 : 0 ;
                return BiT({ BTsta(0b010011100,9), BTsta(sizeStat,1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b110101, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[0].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b01001110010, 11), BTreg(2, 5, In), BTsta(0b000101, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FSUB):
        if (index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
                 index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype)
        {
            if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[0].elemtype == TYPE_FP64 ? 1 : 0 ;
                return BiT({ BTsta(0b010011101,9), BTsta(sizeStat,1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b110101, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[0].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b01001110110, 11), BTreg(2, 5, In), BTsta(0b000101, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FMUL):
        if (index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
                 index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype)
        {
            if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[0].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b011011100,9), BTsta(sizeStat,1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b110111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[0].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b01101110010, 11), BTreg(2, 5, In), BTsta(0b000111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FDIV):
        if (index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
                 index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype)
        {
            if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[0].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b011011100,9), BTsta(sizeStat,1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b111111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[0].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b01101110010, 11), BTreg(2, 5, In), BTsta(0b001111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FNEG):
        if (index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype)
        {
            if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[0].elemtype == TYPE_FP64 ? 1 : 0 ;
                return BiT({ BTsta(0b011011101,9), BTsta(sizeStat,1), BTsta(0b100000111110, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[0].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b0110111011111000111110, 22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FMLA):
        if (index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
                 index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype)
        {
            if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[0].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b010011100,9), BTsta(sizeStat,1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b110011, 6), BTreg(1, 5, In), BTreg(0, 5, IO) });
            }
            else if(index[0].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b01001110010, 11), BTreg(2, 5, In), BTsta(0b000011, 6), BTreg(1, 5, In), BTreg(0, 5, IO) });
        }
        if (index.size() == 4 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
            index[3].tag == Arg::IIMMEDIATE && index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype)
        {
            if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[0].elemtype == TYPE_FP64 ? 1 : 0;
                int H = 0;
                int L = 0;
                if(sizeStat == 0)
                {
                    H = (index[3].value & 0b10) >> 1;
                    L = index[3].value & 1;
                }
                else if(sizeStat == 1)
                {
                    H = index[3].value & 1;
                }
                else 
                    break;
                return BiT({ BTsta(0b010011111, 9), BTsta(sizeStat, 1), BTsta(L, 1), BTreg(2, 5, In), BTsta(0b0001, 4), BTsta(H, 1), BTsta(0b0, 1), BTreg(1, 5, In), BTreg(0, 5, IO), BTomm(3)});
            }
            else if(index[0].elemtype == TYPE_FP16)
            {
                if (index[2].idx > 15) // Limitation: Rm register must be <15
                    break;
                int H = 0;
                int L = 0;
                int M = 0;
                if (index[3].value >= 0 && index[3].value <= 7)
                {
                    H = (index[3].value & 0b100) >> 2;
                    L = (index[3].value & 0b10) >> 1;
                    M = index[3].value & 1;
                }
                else 
                    break;
                return BiT({ BTsta(0b0100111100, 10), BTsta(L, 1), BTsta(M, 1), BTreg(2, 4, In), BTsta(0b0001, 4), BTsta(H, 1), BTsta(0b0, 1), BTreg(1, 5, In), BTreg(0, 5, IO), BTomm(3) });
            }
        }
        break;
    case (AARCH64_SHL):
        if (index.size() > 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && isInteger(index[1].elemtype))
        {
            if(index.size() == 4 && index[2].tag == Arg::IIMMEDIATE && index[3].tag == Arg::IIMMEDIATE)
                return BiT({ BTsta(0b010011110,9), BTimm(2, 4), BTimm(3, 3), BTsta(0b010101,6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_USHL):
        if (index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && isInteger(index[0].elemtype) && isInteger(index[2].elemtype) && elem_size(index[2].elemtype) == elem_size(index[0].elemtype))
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01101110,8), BTsta(sizeStat,2), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b010001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_SSHL):
        if (index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && isSignedInteger(index[1].elemtype) && isInteger(index[2].elemtype) && elem_size(index[2].elemtype) == elem_size(index[0].elemtype))
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01001110,8), BTsta(sizeStat,2), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b010001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_USHR):
        {
        if (index.size() == 4 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::IIMMEDIATE && index[0].elemtype == index[1].elemtype && isInteger(index[0].elemtype))
            return BiT({ BTsta(0b011011110,9), BTimm(2, 4), BTimm(3, 3), BTsta(0b000001,6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
        }
    case (AARCH64_SSHR):
        if (index.size() == 4 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::IIMMEDIATE && index[0].elemtype == index[1].elemtype && isSignedInteger(index[0].elemtype))
            return BiT({ BTsta(0b010011110,9), BTimm(2, 4), BTimm(3, 3), BTsta(0b000001,6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_MOVI):
        if (index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IIMMEDIATE)
        {
            size_t elSize = elem_size(index[0].elemtype);
            int64_t mainstat = elSize == 8 ? 0b0110111100000 : 0b0100111100000;
            int64_t cmodstat = (elSize == 1 || elSize == 8) ? 0b1110 : (elSize == 2 ? 0b1000 : /*(elSize == 4 ?*/ 0b0000 /*: ...)*/);
            return BiT({ BTsta(mainstat, 13), BTimm(2, 3), BTsta(cmodstat, 4), BTsta(0b01, 2), BTimm(1, 5), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_MVNI):
        if (index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IIMMEDIATE && (index[0].elemtype == TYPE_I16 || index[0].elemtype == TYPE_I32))
        {
            int64_t cmodstat = index[0].elemtype == TYPE_I16 ? 0b1000 : 0b0000;
            return BiT({ BTsta(0b0110111100000, 13), BTimm(2, 3), BTsta(cmodstat, 4), BTsta(0b01, 2), BTimm(1, 5), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_CMHI):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isUnsignedInteger(index[1].elemtype))
        {
            if(index[1].elemtype == TYPE_U32 || index[1].elemtype == TYPE_U64)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_U64 ? 1 : 0;
                return BiT({ BTsta(0b011011101, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b001101, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[1].elemtype == TYPE_U8 || index[1].elemtype == TYPE_U16)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_U16 ? 1 : 0;
                return BiT({ BTsta(0b011011100, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b001101, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
        }
        break;
    case (AARCH64_CMHS):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isUnsignedInteger(index[1].elemtype))
        {
            if(index[1].elemtype == TYPE_U32 || index[1].elemtype == TYPE_U64)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_U64 ? 1 : 0;
                return BiT({ BTsta(0b011011101, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b001111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[1].elemtype == TYPE_U8 || index[1].elemtype == TYPE_U16)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_U16 ? 1 : 0;
                return BiT({ BTsta(0b011011100, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b001111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
        }
        break;
    case (AARCH64_CMGT):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isSignedInteger(index[1].elemtype))
        {
            if(index[1].elemtype == TYPE_I32 || index[1].elemtype == TYPE_I64)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_I64 ? 1 : 0;
                return BiT({ BTsta(0b010011101, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b001101, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[1].elemtype == TYPE_I8 || index[1].elemtype == TYPE_I16)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_I16 ? 1 : 0;
                return BiT({ BTsta(0b010011100, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b001101, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
        }
        break;
    case (AARCH64_CMGE):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isSignedInteger(index[1].elemtype))
        {
            if(index[1].elemtype == TYPE_I32 || index[1].elemtype == TYPE_I64)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_I64 ? 1 : 0;
                return BiT({ BTsta(0b010011101, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b001111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[1].elemtype == TYPE_I8 || index[1].elemtype == TYPE_I16)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_I16 ? 1 : 0;
                return BiT({ BTsta(0b010011100, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b001111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
        }
        break;
    case (AARCH64_CMEQ):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isInteger(index[1].elemtype))
        {
            size_t esz = elem_size(index[1].elemtype);
            if(esz <= 2)
            {
                uint64_t sizeStat = (esz == 2 ? 1 : 0);
                return BiT({ BTsta(0b011011100, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b100011, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(esz <= 8)
            {
                uint64_t sizeStat = (esz == 8 ? 1 : 0);
                return BiT({ BTsta(0b011011101, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b100011, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
        }
        break;
    case (AARCH64_FCMGT):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isFloat(index[1].elemtype))
        {
            if(index[1].elemtype == TYPE_FP32 || index[1].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b011011101, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b111001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[1].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b01101110110, 11), BTreg(2, 5, In), BTsta(0b001001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FCMGE):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isFloat(index[1].elemtype))
        {
            if(index[1].elemtype == TYPE_FP32 || index[1].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b011011100, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b111001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[1].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b01101110010, 11), BTreg(2, 5, In), BTsta(0b001001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FCMEQ):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isFloat(index[1].elemtype))
        {
            if(index[1].elemtype == TYPE_FP32 || index[1].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b010011100, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b111001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[1].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b01001110010, 11), BTreg(2, 5, In), BTsta(0b001001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FMIN):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype && isFloat(index[0].elemtype))
        {
            if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[0].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b010011101, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b111101, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[0].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b01001110110, 11), BTreg(2, 5, In), BTsta(0b001101, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FMAX):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype && isFloat(index[0].elemtype))
        {
            if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[0].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b010011100, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b111101, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[0].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b01001110010, 11), BTreg(2, 5, In), BTsta(0b001101, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_SMINV):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && isSignedInteger(index[0].elemtype) && elem_size(index[0].elemtype) <= 4)
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01001110,8), BTsta(sizeStat,2), BTsta(0b110001101010, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_SMAXV):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && isSignedInteger(index[0].elemtype) && elem_size(index[0].elemtype) <= 4)
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01001110,8), BTsta(sizeStat,2), BTsta(0b110000101010, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_UMINV):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && isUnsignedInteger(index[0].elemtype) && elem_size(index[0].elemtype) <= 4)
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01101110,8), BTsta(sizeStat,2), BTsta(0b110001101010, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_UMAXV):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && isUnsignedInteger(index[0].elemtype) && elem_size(index[0].elemtype) <= 4)
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01101110,8), BTsta(sizeStat,2), BTsta(0b110000101010, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FMINV):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && index[0].elemtype == TYPE_FP32)
            return BiT({ BTsta(0b0110111010110000111110, 22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && index[0].elemtype == TYPE_FP16)
            return BiT({ BTsta(0b0100111010110000111110,22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_ADDV):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG &&            index[0].elemtype == index[1].elemtype && elem_size(index[0].elemtype) <= 4 && isInteger(index[0].elemtype))
        {
            static int size_imms[9] = {-1, 0b00, 0b01 , -1, 0b10};
            int size_imm = size_imms[elem_size(index[0].elemtype)];
            Assert(size_imm != -1);
            return BiT({ BTsta(0b01001110, 8), BTsta(size_imm, 2), BTsta(0b110001101110, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_UADDLV):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elem_size(index[0].elemtype) == 2 * elem_size(index[1].elemtype) && elem_size(index[0].elemtype) >= 2 && isUnsignedInteger(index[0].elemtype) && isUnsignedInteger(index[1].elemtype))
        {
            static int size_imms[9] = {-1, 0b00, 0b01 , -1, 0b10};
            int size_imm = size_imms[elem_size(index[1].elemtype)];
            Assert(size_imm != -1);
            return BiT({ BTsta(0b01101110, 8), BTsta(size_imm, 2), BTsta(0b110000001110, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_SADDLV):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elem_size(index[0].elemtype) == 2 * elem_size(index[1].elemtype) && elem_size(index[0].elemtype) >= 2 && isSignedInteger(index[0].elemtype) && isSignedInteger(index[1].elemtype))
        {
            static int size_imms[9] = {-1, 0b00, 0b01 , -1, 0b10};
            int size_imm = size_imms[elem_size(index[1].elemtype)];
            Assert(size_imm != -1);
            return BiT({ BTsta(0b01001110, 8), BTsta(size_imm, 2), BTsta(0b110000001110, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FMAXV):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && index[0].elemtype == TYPE_FP32)
            return BiT({ BTsta(0b0110111000110000111110, 22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && index[0].elemtype == TYPE_FP16)
            return BiT({ BTsta(0b0100111000110000111110,22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_FCVTZS):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isSignedInteger(index[0].elemtype) && isFloat(index[1].elemtype))
        {
            if(index[1].elemtype == TYPE_FP32 || index[1].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b010011101, 9), BTsta(sizeStat, 1), BTsta(0b100001101110, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[1].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b0100111011111001101110, 22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FCVTZU):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isUnsignedInteger(index[0].elemtype) && isFloat(index[1].elemtype))
        {
            if(index[1].elemtype == TYPE_FP32 || index[1].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b011011101, 9), BTsta(sizeStat, 1), BTsta(0b100001101110, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[1].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b0110111011111001101110, 22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FCVTMS):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isSignedInteger(index[0].elemtype) && isFloat(index[1].elemtype))
        {
            if(index[1].elemtype == TYPE_FP32 || index[1].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b010011100, 9), BTsta(sizeStat, 1), BTsta(0b100001101110, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[1].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b0100111001111001101110, 22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FCVTMU):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isUnsignedInteger(index[0].elemtype) && isFloat(index[1].elemtype))
        {
            if(index[1].elemtype == TYPE_FP32 || index[1].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b011011100, 9), BTsta(sizeStat, 1), BTsta(0b100001101110, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[1].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b0110111001111001101110, 22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_SCVTF):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isSignedInteger(index[1].elemtype) && isFloat(index[0].elemtype))
        {
            if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[0].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b010011100, 9), BTsta(sizeStat, 1), BTsta(0b100001110110, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[0].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b0100111001111001110110, 22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_UCVTF):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isUnsignedInteger(index[1].elemtype) && isFloat(index[0].elemtype))
        {
            if(index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[0].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b011011100, 9), BTsta(sizeStat, 1), BTsta(0b100001110110, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[0].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b0110111001111001110110, 22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_LD1):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IIMMEDIATE)
        {
            int esize = elem_size(index[0].elemtype);
            static int opcodes[9] = {-1, 0b000, 0b010 , -1, 0b100, -1, -1, -1 , 0b100};
            int opcode = opcodes[esize];
            Assert(opcode != -1);
            uint64_t size_field = esize == 8 ? 1 : 0;
            int S = 0;
            int Q = 0;
            if(esize == 8)
            {
                Q = index[2].value & 1;
            }
            else if(esize == 4)
            {
                Q = (index[2].value & 0b10) >> 1;
                S = index[2].value & 1;
            }
            else if(esize == 2)
            {
                Q = (index[2].value & 0b100) >> 2;
                S = (index[2].value & 0b10) >> 1;
                size_field = size_field | ((index[2].value & 0b1) << 1);
            }
            else if(esize == 1)
            {
                Q = (index[2].value & 0b1000) >> 3;
                S = (index[2].value & 0b100) >> 2;
                size_field = index[2].value & 0b11;
            }
            else
                break;
            return BiT({ BTsta(0b0, 1), BTsta(Q, 1), BTsta(0b00110101000000, 14), BTsta(opcode, 3), BTsta(S, 1), BTsta(size_field, 2), BTreg(1, 5, In), BTreg(0, 5, Out), BTomm(2) });
        }
        //WARNING: This is post-index form, post-incrementing pointing scalar register on 64 bytes.
        else if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::IREG)
        {//This is post-index form, post-incrementing pointing scalar register on 64 bytes.
            static int size_imms[9] = {-1, 0b00, 0b01 , -1, 0b10, -1, -1, -1 , 0b11};
            int size_imm = size_imms[elem_size(index[0].elemtype)];
            Assert(size_imm != -1);
            return BiT({ BTsta(0b01001100110111110010, 20), BTsta(size_imm, 2), BTreg(1, 5, In), BTreg(0, 5, In) });
        }
        break;
    case (AARCH64_LD2):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::IREG && index[0].elemtype == index[1].elemtype && index[1].idx == ((index[0].idx + 1) % 32))
        {
            static int size_imms[9] = {-1, 0b00, 0b01 , -1, 0b10, -1, -1, -1 , 0b11};
            int size_imm = size_imms[elem_size(index[0].elemtype)];
            return BiT({ BTsta(0b01001100010000001000, 20), BTsta(size_imm, 2), BTreg(2, 5, In), BTreg(0, 5, Out), BTomm(1)});
        }
        break;
    case (AARCH64_LD4):
        //WARNING: This is post-index form, post-incrementing pointing scalar register on 64 bytes.
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::IREG)
        {
            static int size_imms[9] = {-1, 0b00, 0b01 , -1, 0b10, -1, -1, -1 , 0b11};
            int size_imm = size_imms[elem_size(index[0].elemtype)];
            Assert(size_imm != -1);
            return BiT({ BTsta(0b01001100110111110000, 20), BTsta(size_imm, 2), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_ST1):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG )
        {
            int esize = elem_size(index[0].elemtype); 
            static int opcodes[9] = {-1, 0b000, 0b010 , -1, 0b100, -1, -1, -1 , 0b100}; 
            int opcode = opcodes[esize];
            Assert(opcode != -1);
            uint64_t size_field = esize == 8 ? 1 : 0;
            int S = 0;
            int Q = 0;
            if(esize == 8)
            {
                Q = index[1].value & 1;
            }
            else if(esize == 4)
            {
                Q = (index[1].value & 0b10) >> 1;
                S = index[1].value & 1;
            }
            else if(esize == 2)
            {
                Q = (index[1].value & 0b100) >> 2;
                S = (index[1].value & 0b10) >> 1;
                size_field = size_field | ((index[1].value & 0b1) << 1);
            }
            else if(esize == 1)
            {
                Q = (index[1].value & 0b1000) >> 3;
                S = (index[1].value & 0b100) >> 2;
                size_field = index[1].value & 0b11;
            }
            else 
                break;
            return BiT({ BTsta(0b0, 1), BTsta(Q, 1), BTsta(0b00110100000000, 14), BTsta(opcode, 3), BTsta(S, 1), BTsta(size_field, 2), BTreg(2, 5, In), BTreg(0, 5, In), BTomm(1) });
        }
        else if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::IREG)
        {//This is post-index form, post-incrementing pointing scalar register on 64 bytes.
            static int size_imms[9] = {-1, 0b00, 0b01 , -1, 0b10, -1, -1, -1 , 0b11};
            int size_imm = size_imms[elem_size(index[0].elemtype)];
            Assert(size_imm != -1);
            return BiT({ BTsta(0b01001100100111110010, 20), BTsta(size_imm, 2), BTreg(1, 5, In), BTreg(0, 5, In) });
        }
        break;
    case (AARCH64_ST4):
        //WARNING: This is post-index form, post-incrementing pointing scalar register on 64 bytes.
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::IREG)
        {
            static int size_imms[9] = {-1, 0b00, 0b01 , -1, 0b10, -1, -1, -1 , 0b11};
            int size_imm = size_imms[elem_size(index[0].elemtype)];
            Assert(size_imm != -1);
            return BiT({ BTsta(0b01001100100111110000, 20), BTsta(size_imm, 2), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_EXT):
        if(index.size() == 4 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[3].tag == Arg::IIMMEDIATE)
        {
            return BiT({ BTsta(0b01101110000, 11), BTreg(2, 5, In), BTsta(0, 1), BTimm(3, 4, In), BTsta(0, 1), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_DUP):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::IREG)
        {
            const uint64_t WrongStat = 0xFFFFFFFF;
            static const uint64_t dupSizeStats[] = {WrongStat, 0b00001, 0b00010, WrongStat, 0b00100, WrongStat, WrongStat, WrongStat, 0b01000 };
            uint64_t dupSizeStat = dupSizeStats[elem_size(index[0].elemtype)];
            Assert(dupSizeStat != WrongStat);
            return BiT({ BTsta(0b01001110000, 11), BTsta(dupSizeStat, 5), BTsta(0b000011, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        else if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::IIMMEDIATE && index[0].elemtype == index[1].elemtype)
        {
            int elemsize = elem_size(index[0].elemtype);
            const uint64_t WrongStat = 0xFFFFFFFF;
            static const uint64_t sizIdxs[] = {WrongStat, 0, 1, WrongStat, 2, WrongStat, WrongStat, WrongStat, 3 };
            uint64_t sizIdx = sizIdxs[elemsize];
            Assert(sizIdx != WrongStat);
            static const uint64_t sizStat[] = {0b1, 0b10, 0b100, 0b1000};
            static const int sizStatSiz[] = {1, 2, 3, 4};
            return BiT({ BTsta(0b01001110000, 11), BTimm(2, 5-sizStatSiz[sizIdx]), BTsta(sizStat[sizIdx], sizStatSiz[sizIdx]), BTsta(0b000001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_UMOV):
        if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::VREG && index[2].tag == Arg::IIMMEDIATE &&
          (index[1].elemtype == TYPE_U8   ||
           index[1].elemtype == TYPE_FP16 || index[1].elemtype == TYPE_U16 ||
           index[1].elemtype == TYPE_FP32 || index[1].elemtype == TYPE_U32 ||
           index[1].elemtype == TYPE_FP64 || index[1].elemtype == TYPE_U64 || index[1].elemtype == TYPE_I64))
        {
            int elemsize = elem_size(index[1].elemtype);
            uint64_t mainSta = elemsize == 8 ? 0b01001110000 : 0b00001110000;
            const int WrongStat = -1;
            static const int dupSizeSWidthes[] = {WrongStat, 1, 2, WrongStat, 3, WrongStat, WrongStat, WrongStat, 4 };
            int dupSizeSWidth= dupSizeSWidthes[elemsize];
            Assert(dupSizeSWidth != WrongStat);
            static const uint64_t dupSizeStats[] = {0b1, 0b10, 0b00100, 0b1000 };
            uint64_t dupSizeStat = dupSizeStats[dupSizeSWidth - 1];
            return BiT({ BTsta(mainSta, 11), BTimm(2,5 - dupSizeSWidth), BTsta(dupSizeStat, dupSizeSWidth),BTsta(0b001111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;    
    case (AARCH64_INS):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG)
        {
            int elemsize = elem_size(index[0].elemtype);
            const int WrongStat = -1;
            static const int dupSizeSWidthes[] = {WrongStat, 1, 2, WrongStat, 3, WrongStat, WrongStat, WrongStat, 4 };
            int dupSizeSWidth = dupSizeSWidthes[elemsize];
            Assert(dupSizeSWidth != WrongStat);
            static const uint64_t dupSizeStats[] = {0b1, 0b10, 0b00100, 0b1000 };
            uint64_t dupSizeStat = dupSizeStats[dupSizeSWidth - 1];
            return BiT({ BTsta(0b01001110000, 11), BTimm(1,5 - dupSizeSWidth), BTsta(dupSizeStat, dupSizeSWidth), BTsta(0b000111, 6), BTreg(2, 5, In), BTreg(0, 5, IO) });
        }
        else if(index.size() == 4 && index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::VREG && index[3].tag == Arg::IIMMEDIATE && index[0].elemtype == index[2].elemtype)
        {
            int elemsize = elem_size(index[0].elemtype);
            const uint64_t WrongStat = 0xFFFFFFFF;
            static const uint64_t sizIdxs[] = {WrongStat, 0, 1, WrongStat, 2, WrongStat, WrongStat, WrongStat, 3 };
            uint64_t sizIdx = sizIdxs[elemsize];
            Assert(sizIdx != WrongStat);
            static const uint64_t sizStat[] = {0b1, 0b10, 0b100, 0b1000};
            static const int sizStatSiz[] = {1, 2, 3, 4};
            return BiT({ BTsta(0b01101110000, 11), BTimm(1, 5-sizStatSiz[sizIdx]), BTsta(sizStat[sizIdx], sizStatSiz[sizIdx]), BTsta(0, 1), BTimm(3, 5-sizStatSiz[sizIdx]), BTsta(0, sizStatSiz[sizIdx] - 1), BTsta(1, 1), BTreg(2, 5, In), BTreg(0, 5, IO) });
        }

        break;
    case (AARCH64_SSHLL):
    case (AARCH64_SSHLL2):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::IIMMEDIATE &&
           isSignedInteger(index[0].elemtype) && isSignedInteger(index[1].elemtype) &&
           elem_size(index[0].elemtype) == 2 * elem_size(index[1].elemtype))
        {
            int opprefix = index.opcode == AARCH64_SSHLL2 ? 0b010011110 : 0b000011110;
            int elemsize = elem_size(index[1].elemtype);
            const uint64_t WrongStat = 0xFFFFFFFF;
            static const uint64_t esizIdxs[] = {WrongStat, 0, 1, WrongStat, 2, WrongStat, WrongStat, WrongStat, WrongStat };
            uint64_t esizIdx = esizIdxs[elemsize];
            Assert(esizIdx != WrongStat);
            static const int esizeStatSizes[] = {4, 3, 2};
            int esizSSiz = esizeStatSizes[esizIdx];
            static const int64_t shiftFieldMask[] = {0b111, 0b1111, 0b11111};
            if(index[2].value > shiftFieldMask[esizIdx])
                break;
            return BiT({ BTsta(opprefix, 9), BTsta(1, esizSSiz), BTimm(2, 7 - esizSSiz), BTsta(0b101001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_USHLL):
    case (AARCH64_USHLL2):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::IIMMEDIATE &&
           isUnsignedInteger(index[0].elemtype) && isUnsignedInteger(index[1].elemtype) &&
           elem_size(index[0].elemtype) == 2 * elem_size(index[1].elemtype))
        {
            int opprefix = index.opcode == AARCH64_USHLL2 ? 0b011011110 : 0b001011110;
            int elemsize = elem_size(index[1].elemtype);
            const uint64_t WrongStat = 0xFFFFFFFF;
            static const uint64_t esizIdxs[] = {WrongStat, 0, 1, WrongStat, 2, WrongStat, WrongStat, WrongStat, WrongStat };
            uint64_t esizIdx = esizIdxs[elemsize];
            Assert(esizIdx != WrongStat);
            static const int esizeStatSizes[] = {4, 3, 2};
            int esizSSiz = esizeStatSizes[esizIdx];
            static const int64_t shiftFieldMask[] = {0b111, 0b1111, 0b11111};
            if(index[2].value > shiftFieldMask[esizIdx])
                break;
            return BiT({ BTsta(opprefix, 9), BTsta(1, esizSSiz), BTimm(2, 7 - esizSSiz), BTsta(0b101001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_XTN):
    case (AARCH64_XTN2):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG &&
           isInteger(index[0].elemtype) && 2 * elem_size(index[0].elemtype) == elem_size(index[1].elemtype))
        {
            bool high = index.opcode == AARCH64_XTN2;
            int opprefix = high ? 0b01001110 : 0b00001110;
            int elemsize = elem_size(index[0].elemtype);
            const uint64_t WrongStat = 0xFFFFFFFF;
            static const uint64_t esizStats[] = {WrongStat, 0, 1, WrongStat, 2, WrongStat, WrongStat, WrongStat, WrongStat };
            uint64_t esizStat = esizStats[elemsize];
            Assert(esizStat != WrongStat);
            return BiT({ BTsta(opprefix, 8), BTsta(esizStat, 2), BTsta(0b100001001010, 12), BTreg(1, 5, In), BTreg(0, 5, high ? IO : Out) });
        }
        break;
    case (AARCH64_CNT):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == TYPE_U8 && elem_size(index[1].elemtype) == 1 && isInteger(index[1].elemtype))
            return BiT({ BTsta(0b0100111000100000010110, 22), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_B): return BiT({ BTsta(0x5, 6), BTimm(0, 26, Lab) });
        //TODO(ch): there is no B_LT, B_LE, B_GT, B_GE instructions in ARM processors, it's prespecialized versions of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
    case (AARCH64_B_NE): return BiT({ BTsta(0x54,8), BTimm(0, 19, Lab), BTsta(AARCH64_IC_NE, 5) });
    case (AARCH64_B_EQ): return BiT({ BTsta(0x54,8), BTimm(0, 19, Lab), BTsta(AARCH64_IC_EQ, 5) });
    case (AARCH64_B_LT): return BiT({ BTsta(0x54,8), BTimm(0, 19, Lab), BTsta(AARCH64_IC_LT, 5) });
    case (AARCH64_B_LE): return BiT({ BTsta(0x54,8), BTimm(0, 19, Lab), BTsta(AARCH64_IC_LE, 5) });
    case (AARCH64_B_LS): return BiT({ BTsta(0x54,8), BTimm(0, 19, Lab), BTsta(AARCH64_IC_LS, 5) });
    case (AARCH64_B_GT): return BiT({ BTsta(0x54,8), BTimm(0, 19, Lab), BTsta(AARCH64_IC_GT, 5) });
    case (AARCH64_B_HI): return BiT({ BTsta(0x54,8), BTimm(0, 19, Lab), BTsta(AARCH64_IC_HI, 5) });
    case (AARCH64_B_GE): return BiT({ BTsta(0x54,8), BTimm(0, 19, Lab), BTsta(AARCH64_IC_GE, 5) });
    case (AARCH64_BLR):
        if(index.size() == 1 && index[0].tag == Arg::IREG)
            return BiT({ BTsta(0b1101011000111111000000, 22), BTreg(0, 5, In | Addr), BTsta(0b00000, 5) });
        break;
    case (AARCH64_LABEL): return BiT({});
    case (AARCH64_RET): return BiT({ BTsta(0x3597C0, 22), BTreg(0, 5, In), BTsta(0x0,5) });
    default:
        break;
    }
    scs = false;
    return BinTranslation();
}

//TODO(ch): write own version of this func instead of LLVM's.
/*
Next function is copied from LLVM project and adopted. See
https://github.com/llvm/llvm-project/blob/main/llvm/lib/Target/AArch64/MCTargetDesc/AArch64AddressingModes.h
*/
static inline bool processLogicalImmediate(uint64_t Imm, unsigned RegSize, uint64_t &Encoding)
{
  auto countTrailingZeros = [](uint64_t imm) {return (unsigned)lsb64(imm);};
  auto countTrailingOnes  = [](uint64_t imm) {return (unsigned)lsb64(~imm);};
  auto countLeadingOnes   = [](uint64_t imm) {return (unsigned)(63-msb64(~imm));};
  auto isShiftedMask_64   = [](uint64_t imm) {
      auto isMask_64      = [](uint64_t imm) { return imm && ((imm + 1) & imm) == 0; };
      return imm && isMask_64((imm - 1) | imm);
  };
  if (Imm == 0ULL || Imm == ~0ULL ||
      (RegSize != 64 &&
        (Imm >> RegSize != 0 || Imm == (~0ULL >> (64 - RegSize)))))
    return false;

  // First, determine the element size.
  unsigned Size = RegSize;

  do {
    Size /= 2;
    uint64_t Mask = (1ULL << Size) - 1;

    if ((Imm & Mask) != ((Imm >> Size) & Mask)) {
      Size *= 2;
      break;
    }
  } while (Size > 2);

  // Second, determine the rotation to make the element be: 0^m 1^n.
  uint32_t CTO, I;
  uint64_t Mask = ((uint64_t)-1LL) >> (64 - Size);
  Imm &= Mask;

  if (isShiftedMask_64(Imm)) {
    I = countTrailingZeros(Imm);
    Assert(I < 64 && "undefined behavior");
    CTO = countTrailingOnes(Imm >> I);
  } else {
    Imm |= ~Mask;
    if (!isShiftedMask_64(~Imm))
      return false;

    unsigned CLO = countLeadingOnes(Imm);
    I = 64 - CLO;
    CTO = CLO + countTrailingOnes(Imm) - (64 - Size);
  }

  // Encode in Immr the number of RORs it would take to get *from* 0^m 1^n
  // to our target value, where I is the number of RORs to go the opposite
  // direction.
  Assert(Size > I && "I should be smaller than element size");
  unsigned Immr = (Size - I) & (Size - 1);

  // If size has a 1 in the n'th bit, create a value that has zeroes in
  // bits [0, n] and ones above that.
  uint64_t NImms = ~(Size-1) << 1;

  // Or the CTO value into the low bits, which must be below the Nth bit
  // bit mentioned above.
  NImms |= (CTO-1);

  // Extract the seventh bit and toggle it to create the N field.
  unsigned N = ((NImms >> 6) & 1) ^ 1;

  Encoding = (N << 12) | (Immr << 6) | (NImms & 0x3f);
  return true;
}

SyntopTranslation a64STLookup(const Backend* backend, const Syntop& index, bool& scs)
{
    using namespace SyntopTranslationConstruction;
    scs = true;
    switch (index.opcode)
    {
    case (OP_LOAD):
        if (index.size() == 2)
        {
            switch(index[0].elemtype)
            {
            case (TYPE_U8):
                return SyT(AARCH64_LDRB, { SAcop(0), SAcop(1), SAimm(0) });
            case (TYPE_I8):
                return SyT(AARCH64_LDRSB, { SAcop(0), SAcop(1), SAimm(0) });
            case (TYPE_U16):
            case (TYPE_FP16):
                return SyT(AARCH64_LDRH, { SAcop(0), SAcop(1), SAimm(0) });
            case (TYPE_I16):
                return SyT(AARCH64_LDRSH, { SAcop(0), SAcop(1), SAimm(0) });
            case (TYPE_I32):
                return SyT(AARCH64_LDRSW, { SAcop(0), SAcop(1), SAimm(0) });
            case (TYPE_U32):
            case (TYPE_FP32):
            case (TYPE_U64): case (TYPE_I64): case (TYPE_FP64):
                return SyT(AARCH64_LDR, { SAcop(0), SAcop(1), SAimm(0) });
            };
        }
        else if (index.size() == 3)
        {
            switch(index[0].elemtype)
            {
            case (TYPE_U8):
                return SyT(AARCH64_LDRB, { SAcop(0), SAcop(1), SAcop(2) });
            case (TYPE_I8):
                return SyT(AARCH64_LDRSB, { SAcop(0), SAcop(1), SAcop(2) });
            case (TYPE_U16):
            case (TYPE_FP16):
                return SyT(AARCH64_LDRH, { SAcop(0), SAcop(1), index[2].tag == Arg::IIMMEDIATE ? SAcopsar(2, 1) : SAcop(2) });
            case (TYPE_I16):
                return SyT(AARCH64_LDRSH, { SAcop(0), SAcop(1), index[2].tag == Arg::IIMMEDIATE ? SAcopsar(2, 1) : SAcop(2) });
            case (TYPE_I32):
                return SyT(AARCH64_LDRSW, { SAcop(0), SAcop(1), index[2].tag == Arg::IIMMEDIATE ? SAcopsar(2, 2) : SAcop(2) });
            case (TYPE_U32): case (TYPE_FP32):
            case (TYPE_U64): case (TYPE_I64): case (TYPE_FP64):
                return SyT(AARCH64_LDR, { SAcop(0), SAcop(1), index[2].tag == Arg::IIMMEDIATE ? SAcopsar(2, elem_size(index[0].elemtype) <= 4 ? 2 : 3) : SAcop(2) });
            };
        }
        break;
    case (OP_STORE):
        if (index.size() == 2)
        {
            switch(index[1].elemtype)
            {
            case (TYPE_U8): case (TYPE_I8):
                return SyT(AARCH64_STRB, { SAcop(1), SAcop(0), SAimm(0) });
            case (TYPE_U16): case (TYPE_I16): case (TYPE_FP16):
                return SyT(AARCH64_STRH, { SAcop(1), SAcop(0), SAimm(0) });
            case (TYPE_U32): case (TYPE_I32): case (TYPE_FP32):
            case (TYPE_U64): case (TYPE_I64): case (TYPE_FP64):
                return SyT(AARCH64_STR, { SAcop(1), SAcop(0), SAimm(0) });
            };
        }
        else if (index.size() == 3)
        {
            switch(index[2].elemtype)
            {
            case (TYPE_U8): case (TYPE_I8):
                return SyT(AARCH64_STRB, { SAcop(2), SAcop(0), SAcop(1) });
            case (TYPE_U16): case (TYPE_I16): case (TYPE_FP16):
                return SyT(AARCH64_STRH, { SAcop(2), SAcop(0), index[1].tag == Arg::IIMMEDIATE ? SAcopsar(1, 1) : SAcop(1) });
            case (TYPE_U32): case (TYPE_I32): case (TYPE_FP32):
            case (TYPE_U64): case (TYPE_I64): case (TYPE_FP64):
                return SyT(AARCH64_STR, { SAcop(2), SAcop(0), index[1].tag == Arg::IIMMEDIATE ? SAcopsar(1, elem_size(index[2].elemtype) <= 4 ? 2 : 3) : SAcop(1) });
            };
        }
        break;
    case (OP_MOV):
        if(index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE)
        {
            uint64_t bitfield = static_cast<uint64_t>(index[1].value);
            int tarOpcode = AARCH64_MOVI;
            if(elem_size(index[0].elemtype) == 8)
            {
                bool correct = true;
                uint64_t bitfieldnew = 0;
                uint8_t* bytes = (uint8_t*)(&bitfield);
                for(int i = 0; i < 8; i++)
                    if(bytes[i] != 0 && bytes[i] != 0xff)
                    {
                        correct = false;
                        break;
                    }
                    else
                        bitfieldnew = bitfieldnew | ((bytes[i] & 1) << i);
                if(!correct) break;
                bitfield = bitfieldnew;
            }
            else if(index[1].value < 0)
            {
                if(index[0].elemtype == TYPE_I8)
                {
                    if((~(bitfield & 0xFFFFFFFFFFFF0000)) == 0)
                        bitfield &= 0xFF;
                    else
                        break;
                }
                else if(index[0].elemtype == TYPE_I16 || index[0].elemtype == TYPE_I32)
                {
                    bitfield = ~bitfield;
                    tarOpcode = AARCH64_MVNI;
                }
            }
            int bitNeed = msb64(bitfield);
            if (bitNeed >= 8)
                break;
            int64_t higher3 = (bitfield & 0b11100000) >> 5;
            int64_t lower5  = bitfield & 0b00011111;
            return SyT(tarOpcode, { SAcop(0), SAimm(lower5), SAimm(higher3) });
        }
        else if(index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[1].value == 0)
        {
            return SyT(AARCH64_EOR, { SAcop(0), SAcop(0), SAcop(0) });
        }
        else if(index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE)
        {
            uint64_t bitfield = static_cast<uint64_t>(index[1].value);
            bool invert = false;
            if(index[1].value < 0)
            {
                invert = true;
                bitfield = ~bitfield;
            }
            int bitNeed = msb64(bitfield);
            if (bitNeed >= 16)
                break;
            if(invert)
                return SyT(AARCH64_MOVN, { SAcop(0), SAimm(bitfield) });
            else
                return SyT(AARCH64_MOV, { SAcop(0), SAcop(1) });
        }
        else
            return SyT(AARCH64_MOV, { SAcop(0), SAcop(1) });
        break;;
    case (OP_ARM_MOVK):
        if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IIMMEDIATE &&
           (index[2].value == 16 || index[2].value == 32 || index[2].value == 48))
            return SyT(AARCH64_MOVK, { SAcop(0), SAcop(1), SAcopsar(2, 4)});
        break;
    case (OP_ADD):    return SyT(AARCH64_ADD, { SAcop(0), SAcop(1), SAcop(2) });
    case (OP_SUB):    return SyT(AARCH64_SUB, { SAcop(0), SAcop(1), SAcop(2) });
    case (OP_MUL):    return SyT(AARCH64_MUL, { SAcop(0), SAcop(1), SAcop(2) });
    case (OP_MOD):
    case (OP_DIV):    return SyT(AARCH64_SDIV, { SAcop(0), SAcop(1), SAcop(2) });
    case (OP_SHL):
        if(index[2].tag == Arg::IIMMEDIATE)
        {
            int64_t shift = index[2].value;
            if(shift < 0 || shift > 63)
                break;
            return SyT(AARCH64_LSL, { SAcop(0), SAcop(1), SAcop(2) });
        }
        else if(index[2].tag == Arg::IREG)
            return SyT(AARCH64_LSL, { SAcop(0), SAcop(1), SAcop(2) });
        else
            break;
    case (OP_SHR):    return SyT(AARCH64_LSR, { SAcop(0), SAcop(1), SAcop(2) });
    case (OP_SAR):    return SyT(AARCH64_ASR, { SAcop(0), SAcop(1), SAcop(2) }    );
    case (OP_AND):
    case (OP_OR):
    case (OP_XOR):
    {
        int taropcode = index.opcode == OP_AND ? AARCH64_AND : (
                        index.opcode == OP_OR  ? AARCH64_ORR : (
                      /*index.opcode == OP_XOR?*/AARCH64_EOR ));
        if(index.size() != 3)
            break;
        if(index[2].tag == Arg::IIMMEDIATE)
        {
            uint64_t N_immr_imms;
            if (processLogicalImmediate(index[2].value, 64, N_immr_imms)) //DUBUG: Move this complicated processing down to BinaryTranslation lookup.
            {
                uint64_t N = (N_immr_imms & 0x1000)>>12;
                uint64_t immr = (N_immr_imms & 0xfc0)>>6;
                uint64_t imms = N_immr_imms & 0x3f;
                return SyT(taropcode, { SAcop(0), SAcop(1), SAimm(N), SAimm(immr), SAimm(imms) });
            }
        }
        else if(index[2].tag == Arg::IREG)
            return SyT(taropcode, { SAcop(0), SAcop(1), SAcop(2) });
        break;
    }
    case (OP_NOT):      return SyT(AARCH64_MVN, { SAcop(0), SAcop(1) });
    case (OP_NEG):      return SyT(AARCH64_NEG, { SAcop(0), SAcop(1) });
    case (OP_CMP):      return SyT(AARCH64_CMP, { SAcop(0), SAcop(1) });
    case (OP_SELECT):
        if (index.size() == 4 && index[1].value >= OP_GT && index[1].value <= OP_NS)
            return SyT(AARCH64_CSEL, { SAcop(0), SAcop(2), SAcop(3), SAimm(IC_IR2Aarch64((int)index[1].value)) });
        break;
    case (OP_IVERSON):
        if (index.size() == 2 && index[1].value >= OP_GT && index[1].value <= OP_NS)
            return SyT(AARCH64_CSET, { SAcop(0), SAimm(invertAarch64IC(IC_IR2Aarch64((int)index[1].value))) }); //DUBUG: Move this complicated processing down to BinaryTranslation lookup.
        break;
    case (OP_ARM_CINC): return SyT(AARCH64_CINC,{ SAcop(0), SAcop(1), SAimm(invertAarch64IC(IC_IR2Aarch64((int)index[2].value))) });
    case (OP_ARM_CNEG): return SyT(AARCH64_CNEG,{ SAcop(0), SAcop(1), SAimm(invertAarch64IC(IC_IR2Aarch64((int)index[2].value))) });
    case (OP_ARM_LDP):
        if(index.size() == 4 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG &&
           index[2].tag == Arg::IREG && index[3].tag == Arg::IIMMEDIATE && index[2].idx != index[0].idx && index[2].idx != index[1].idx && index[0].idx != index[1].idx)
            return SyT(AARCH64_LDP,{ SAcop(0), SAcop(1), SAcop(2), SAcop(3) });
        break;
    case (OP_ARM_STP):
        if(index.size() == 4 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE &&
                                index[2].tag == Arg::IREG && index[3].tag == Arg::IREG)
            return SyT(AARCH64_STP,{ SAcop(2), SAcop(3), SAcop(0), SAcop(1) });
        break;
    case (VOP_LOAD):
        if(index.size() == 3)
        {
            if(index[2].tag == Arg::IREG)
                return SyT(AARCH64_LDR, { SAcop(0), SAcop(1), SAcop(2) });
            else if(index[2].tag == Arg::IIMMEDIATE && (index[2].value&0xF) == 0 )
                return SyT(AARCH64_LDR, { SAcop(0), SAcop(1), SAcopsar(2, 4) });
        }
        else if(index.size() == 2)
            return SyT(AARCH64_LDR, { SAcop(0), SAcop(1), SAimm(0) });
        break;
    case (VOP_ARM_LD1):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG)
        {
            if(index[1].value < backend->vlanes(index[0].elemtype))
                return SyT(AARCH64_LD1, { SAcop(0), SAcop(2), SAcop(1) });
        }
        else if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::IREG)
            return SyT(AARCH64_LD1, { SAcop(0), SAcop(1) });

        break;
    case (VOP_ARM_LD2):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::IREG && index[0].elemtype == index[1].elemtype && index[1].idx == ((index[0].idx + 1) % 32))
            return SyT(AARCH64_LD2, { SAcop(0), SAcop(1), SAcop(2) });
        break;
//    case (VOP_ARM_LD4):
//        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::IREG)
//            return SyT(AARCH64_LD4, { SAcop(0), SAcop(1) });
//        break;
    case (VOP_STORE):
        if(index.size() == 3)
        {
            if(index[1].tag == Arg::IREG)
                return SyT(AARCH64_STR, { SAcop(2), SAcop(0), SAcop(1) });
            else if(index[1].tag == Arg::IIMMEDIATE && (index[1].value&0xF) == 0 )
                return SyT(AARCH64_STR, { SAcop(2), SAcop(0), SAcopsar(1, 4) });
        }
        else if(index.size() == 2)
        {
            return SyT(AARCH64_STR, { SAcop(1), SAcop(0), SAimm(0) });
        }
        break;
    case (VOP_ARM_ST1):
        if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::VREG && index[2].tag == Arg::IIMMEDIATE)
        {
            if(index[2].value < backend->vlanes(index[1].elemtype))
                return SyT(AARCH64_ST1, { SAcop(1), SAcop(2), SAcop(0) });
        }
        else if(index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::VREG)
            return SyT(AARCH64_ST1, { SAcop(1), SAcop(0) });
        break;
//    case (VOP_ARM_ST4):
//        if(index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::VREG)
//            return SyT(AARCH64_ST4, { SAcop(1), SAcop(0) });
//        break;
    case (VOP_ADD):
    case (VOP_SUB):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG
           && index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype)
        {
            if(isInteger(index[0].elemtype))
            {
                int tarOpcode = (index.opcode == VOP_ADD ? AARCH64_ADD : AARCH64_SUB );
                return SyT(tarOpcode, { SAcop(0), SAcop(1), SAcop(2) });
            }
            else if(index[0].elemtype == TYPE_FP16 || index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                int tarOpcode = (index.opcode == VOP_ADD ? AARCH64_FADD : AARCH64_FSUB );
                return SyT(tarOpcode, { SAcop(0), SAcop(1), SAcop(2) });
            }
        }
        break;
    case (VOP_MUL):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG
           && index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype)
        {
            if(isInteger(index[0].elemtype) && index[0].elemtype <= TYPE_I32)
                return SyT(AARCH64_MUL, { SAcop(0), SAcop(1), SAcop(2) });
            else if(index[0].elemtype == TYPE_FP16 || index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
            {
                return SyT(AARCH64_FMUL, { SAcop(0), SAcop(1), SAcop(2) });
            }
        }
        break;
    case (VOP_DIV):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG
           && index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype)
        {
            if(index[0].elemtype == TYPE_FP16 || index[0].elemtype == TYPE_FP32 || index[0].elemtype == TYPE_FP64)
                return SyT(AARCH64_FDIV, { SAcop(0), SAcop(1), SAcop(2) });
        }
        break;
    case (VOP_NEG):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype )
        {
            if(isFloat(index[0].elemtype))
                return SyT(AARCH64_FNEG, { SAcop(0), SAcop(1) });
            else if(isSignedInteger(index[0].elemtype))
                return SyT(AARCH64_NEG, { SAcop(0), SAcop(1) });
        }
        break;
    case (VOP_FMA):
        if(index.size() == 4 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[3].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype && index[0].elemtype == index[3].elemtype && index[0].idx == index[1].idx && isFloat(index[0].elemtype))
            return SyT(AARCH64_FMLA, { SAcop(0), SAcop(2), SAcop(3) });
        else
        {
            if (index.size() == 5 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && 
                index[3].tag == Arg::VREG && index[4].tag == Arg::IIMMEDIATE && index[0].elemtype == index[1].elemtype && 
                index[1].elemtype == index[2].elemtype && index[2].elemtype == index[3].elemtype && index[0].idx == index[1].idx && 
                isFloat(index[0].elemtype))
            return SyT(AARCH64_FMLA, { SAcop(0), SAcop(2), SAcop(3), SAcop(4) });
        }
        break;
    case (VOP_ARM_EXT):
        if(index.size() == 4 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[3].tag == Arg::IIMMEDIATE && index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype)
        { 
            int esize = elem_size(index[0].elemtype); 
            static int shifts[9] = {1, 0, -1 , 1, -2, 1, 1, 1, -4}; 
            int64_t shift = shifts[esize];
            if(index[3].value < backend->vlanes(index[0].elemtype))
                return SyT(AARCH64_EXT, { SAcop(0), SAcop(1), SAcop(2), SAcopsar(3, shift) });
        }
        break;
    case (VOP_SAL):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG &&
           index[0].elemtype == index[1].elemtype && isInteger(index[0].elemtype))
        {
            if(index[2].tag == Arg::IIMMEDIATE)
            {
                uint64_t immh, immb;
                if(!encodeImmShift(index[2].value, index[0].elemtype, immh, immb)) //DUBUG: Move this complicated processing down to BinaryTranslation lookup.
                    break;
                return SyT(AARCH64_SHL, { SAcop(0), SAcop(1), SAimm(immh), SAimm(immb) });
            }
            else if(index[2].tag == Arg::VREG && isInteger(index[2].elemtype) && elem_size(index[2].elemtype) == elem_size(index[0].elemtype))
            {
                int tarOpcode = isUnsignedInteger(index[0].elemtype) ? AARCH64_USHL : AARCH64_SSHL;
                return SyT(tarOpcode, { SAcop(0), SAcop(1), SAcop(2) });
            }
        }
        break;
    case (VOP_SHL):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
           index[0].elemtype == index[1].elemtype && isInteger(index[0].elemtype) && isInteger(index[2].elemtype) && elem_size(index[2].elemtype) == elem_size(index[0].elemtype))
            return SyT(AARCH64_USHL, { SAcop(0), SAcop(1), SAcop(2) });
        break;
    case (VOP_SAR):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG &&
           index[0].elemtype == index[1].elemtype && isInteger(index[0].elemtype))
        {
            if(index[2].tag == Arg::IIMMEDIATE)
            {
                uint64_t immh, immb;
                if(!encodeImmShift(-index[2].value, index[0].elemtype, immh, immb))
                    break;
                int tarOpcode = isUnsignedInteger(index[0].elemtype) ? AARCH64_USHR : AARCH64_SSHR;
                return SyT(tarOpcode, { SAcop(0), SAcop(1), SAimm(immh), SAimm(immb) });
            } //TODO(ch): need negate-based snippet for implementation of this case
//            else if(index[2].tag == Arg::VREG && isInteger(index[2].elemtype) && elem_size(index[2].elemtype) == elem_size(index[0].elemtype))
//            {
//            }
        }
        break;
    case (VOP_SHR):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG &&
           index[0].elemtype == index[1].elemtype && isInteger(index[0].elemtype))
        {
            if(index[2].tag == Arg::IIMMEDIATE)
            {
                uint64_t immh, immb;
                if(!encodeImmShift(-index[2].value, index[0].elemtype, immh, immb))
                    break;
                return SyT(AARCH64_USHR, { SAcop(0), SAcop(1), SAimm(immh), SAimm(immb) });
            } //TODO(ch): need negate-based snippet for implementation of this case
//            else if(index[2].tag == Arg::VREG && isInteger(index[2].elemtype) && elem_size(index[2].elemtype) == elem_size(index[0].elemtype))
//            {
//            }
        }
        break;
    case (VOP_AND):
    case (VOP_OR):
    case (VOP_XOR):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
           index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype && isInteger(index[0].elemtype))
        {
            int taropcode = index.opcode == VOP_AND ? AARCH64_AND : (
                            index.opcode == VOP_OR  ? AARCH64_ORR : (
                          /*index.opcode == VOP_XOR?*/AARCH64_EOR ));
            return SyT(taropcode, { SAcop(0), SAcop(1), SAcop(2) });
        }
        break;
    case (VOP_NOT):     return SyT(AARCH64_MVN, { SAcop(0), SAcop(1) });
    case (VOP_SELECT): 
        if(index.size() == 4 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[3].tag == Arg::VREG &&
           elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && index[0].elemtype == index[2].elemtype && index[0].elemtype == index[3].elemtype && index[0].idx == index[1].idx)
            return SyT(AARCH64_BSL, { SAcop(0), SAcop(2), SAcop(3) });
        break;
    case (VOP_MIN):
    case (VOP_MAX):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
           index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype && isFloat(index[0].elemtype))
        {
            int taropcode = index.opcode == VOP_MIN  ? AARCH64_FMIN : (
                          /*index.opcode == VOP_MAX ?*/AARCH64_FMAX );
            return SyT(taropcode, { SAcop(0), SAcop(1), SAcop(2) });
        }
        break;
    case (VOP_GT):
    case (VOP_GE):
    case (VOP_EQ):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isUnsignedInteger(index[0].elemtype))
        {
            if(isFloat(index[1].elemtype))
            {
                int taropcode = index.opcode == VOP_GT ? AARCH64_FCMGT :
                                index.opcode == VOP_GE ? AARCH64_FCMGE :
                                index.opcode == VOP_EQ ? AARCH64_FCMEQ : -1;
                Assert(taropcode >= 0);
                return SyT(taropcode, { SAcop(0), SAcop(1), SAcop(2) });
            }
            else if(isUnsignedInteger(index[1].elemtype))
            {
                int taropcode = index.opcode == VOP_GT ? AARCH64_CMHI :
                                index.opcode == VOP_GE ? AARCH64_CMHS :
                                index.opcode == VOP_EQ ? AARCH64_CMEQ : -1;
                Assert(taropcode >= 0);
                return SyT(taropcode, { SAcop(0), SAcop(1), SAcop(2) });
            }
            else if(isSignedInteger(index[1].elemtype))
            {
                int taropcode = index.opcode == VOP_GT ? AARCH64_CMGT :
                                index.opcode == VOP_GE ? AARCH64_CMGE :
                                index.opcode == VOP_EQ ? AARCH64_CMEQ : -1;
                Assert(taropcode >= 0);
                return SyT(taropcode, { SAcop(0), SAcop(1), SAcop(2) });
            }
        }
        break;
    case (VOP_LT):
    case (VOP_LE):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isUnsignedInteger(index[0].elemtype))
        {
            if(isFloat(index[1].elemtype))
            {
                int taropcode = index.opcode == VOP_LT ? AARCH64_FCMGT : (
                              /*index.opcode == VOP_LE?*/AARCH64_FCMGE );
                return SyT(taropcode, { SAcop(0), SAcop(2), SAcop(1) });
            }
            else if(isUnsignedInteger(index[1].elemtype))
            {
                int taropcode = index.opcode == VOP_LT ? AARCH64_CMHI : (
                              /*index.opcode == VOP_LE?*/AARCH64_CMHS );
                return SyT(taropcode, { SAcop(0), SAcop(2), SAcop(1) });
            }            
            else if(isSignedInteger(index[1].elemtype))
            {
                int taropcode = index.opcode == VOP_LT ? AARCH64_CMGT : (
                              /*index.opcode == VOP_LE?*/AARCH64_CMGE );
                return SyT(taropcode, { SAcop(0), SAcop(2), SAcop(1) });
            }
        }
        break;
    case (VOP_TRUNC):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && isFloat(index[1].elemtype) && elem_size(index[0].elemtype) == elem_size(index[1].elemtype))
        {
            if(isSignedInteger(index[0].elemtype))
                return SyT(AARCH64_FCVTZS, { SAcop(0), SAcop(1) });
            else if(isUnsignedInteger(index[0].elemtype))
                return SyT(AARCH64_FCVTZU, { SAcop(0), SAcop(1) });
        }
        break;
    case (VOP_FLOOR):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && isFloat(index[1].elemtype) && elem_size(index[0].elemtype) == elem_size(index[1].elemtype))
        {
            if(isSignedInteger(index[0].elemtype))
                return SyT(AARCH64_FCVTMS, { SAcop(0), SAcop(1) });
            else if(isUnsignedInteger(index[0].elemtype))
                return SyT(AARCH64_FCVTMU, { SAcop(0), SAcop(1) });
        }
        break;
    case (VOP_CAST):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && isFloat(index[0].elemtype) && elem_size(index[0].elemtype) == elem_size(index[1].elemtype))
        {
            if(isSignedInteger(index[1].elemtype))
                return SyT(AARCH64_SCVTF, { SAcop(0), SAcop(1) });
            else if(isUnsignedInteger(index[1].elemtype))
                return SyT(AARCH64_UCVTF, { SAcop(0), SAcop(1) });
        }
        break;
    case (VOP_BROADCAST):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::IIMMEDIATE && index[0].elemtype == index[1].elemtype)
        {
            int lanes = 16 / elem_size(index[0].elemtype);
            if(index[2].value < lanes)
                return SyT(AARCH64_DUP, { SAcop(0), SAcop(1), SAcop(2)});
        }
        else if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::IREG)
            return SyT(AARCH64_DUP, { SAcop(0), SAcop(1) });
        break;
    case (VOP_CAST_LOW):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elem_size(index[0].elemtype) == 2 * elem_size(index[1].elemtype))
        {
            if(isSignedInteger(index[0].elemtype) && isSignedInteger(index[1].elemtype))
                return SyT(AARCH64_SSHLL, { SAcop(0), SAcop(1), SAimm(0)});
            else if(isUnsignedInteger(index[0].elemtype) && isUnsignedInteger(index[1].elemtype))
                return SyT(AARCH64_USHLL, { SAcop(0), SAcop(1), SAimm(0)});
        }
        break;
    case (VOP_CAST_HIGH):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elem_size(index[0].elemtype) == 2 * elem_size(index[1].elemtype))
        {
            if(isSignedInteger(index[0].elemtype) && isSignedInteger(index[1].elemtype))
                return SyT(AARCH64_SSHLL2, { SAcop(0), SAcop(1), SAimm(0)}); //TODO(ch): shift operation can be added to IR instruction set for better USSHL/SSHL representation.
            else if(isUnsignedInteger(index[0].elemtype) && isUnsignedInteger(index[1].elemtype))
                return SyT(AARCH64_USHLL2, { SAcop(0), SAcop(1), SAimm(0) });
        }
        break;
    case (VOP_ARM_SHRINK_LOW):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && 2 * elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isInteger(index[0].elemtype))
            return SyT(AARCH64_XTN, { SAcop(0), SAcop(1) });
        break;
    case (VOP_ARM_SHRINK_HIGH):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && 2 * elem_size(index[0].elemtype) == elem_size(index[1].elemtype) && isInteger(index[0].elemtype))
            return SyT(AARCH64_XTN2, { SAcop(0), SAcop(1) });
        break;
    case (VOP_POPCOUNT):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG &&            index[0].elemtype == TYPE_U8 && elem_size(index[1].elemtype) == 1 && isInteger(index[1].elemtype))
            return SyT(AARCH64_CNT, { SAcop(0), SAcop(1) });
        break;
    case (VOP_REDUCE_MAX):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && elem_size(index[0].elemtype) <= 4)
        {
            if(isSignedInteger(index[0].elemtype))
                return SyT(AARCH64_SMAXV, { SAcop(0), SAcop(1) });
            else if(isUnsignedInteger(index[0].elemtype))
                return SyT(AARCH64_UMAXV, { SAcop(0), SAcop(1) });
            else if(index[0].elemtype == TYPE_FP16 || index[0].elemtype == TYPE_FP32 )
                return SyT(AARCH64_FMAXV, { SAcop(0), SAcop(1) });
        }
        break;
    case (VOP_REDUCE_MIN):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && elem_size(index[0].elemtype) <= 4)
        {
            if(isSignedInteger( index[0].elemtype))
                return SyT(AARCH64_SMINV, { SAcop(0), SAcop(1) });
            else if(isUnsignedInteger(index[0].elemtype))
                return SyT(AARCH64_UMINV, { SAcop(0), SAcop(1) });
            else if(index[0].elemtype == TYPE_FP16 || index[0].elemtype == TYPE_FP32 )
                return SyT(AARCH64_FMINV, { SAcop(0), SAcop(1) });
        }
        break;
    case (VOP_REDUCE_SUM):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG &&            index[0].elemtype == index[1].elemtype  && elem_size(index[0].elemtype) <= 4 && isInteger(index[0].elemtype))
            return SyT(AARCH64_ADDV, { SAcop(0), SAcop(1) });
        break;
    case (VOP_REDUCE_WSUM):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elem_size(index[0].elemtype) == 2 * elem_size(index[1].elemtype) && elem_size(index[0].elemtype) >= 2)
        {
            if(isUnsignedInteger(index[0].elemtype) && isUnsignedInteger(index[1].elemtype))
                return SyT(AARCH64_UADDLV, { SAcop(0), SAcop(1) });
            else if(isSignedInteger(index[0].elemtype) && isSignedInteger(index[1].elemtype))
                return SyT(AARCH64_SADDLV, { SAcop(0), SAcop(1) });
        }
        break;
    case (VOP_GETLANE):
        if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::VREG && index[2].tag == Arg::IIMMEDIATE &&
          (index[1].elemtype == TYPE_U8   ||
           index[1].elemtype == TYPE_FP16 || index[1].elemtype == TYPE_U16 ||
           index[1].elemtype == TYPE_FP32 || index[1].elemtype == TYPE_U32 ||
           index[1].elemtype == TYPE_FP64 || index[1].elemtype == TYPE_U64 || index[1].elemtype == TYPE_I64))
            return SyT(AARCH64_UMOV, { SAcop(0), SAcop(1), SAcop(2)});
        break;
    case (VOP_SETLANE):
        if(index.size() == 4 && index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::VREG && index[3].tag == Arg::IIMMEDIATE && index[0].elemtype == index[2].elemtype)
        {
            int lanes = 16 / elem_size(index[0].elemtype);
            if(index[1].value < lanes && index[3].value < lanes)
                return SyT(AARCH64_INS, { SAcop(0), SAcop(1), SAcop(2), SAcop(3)});
        }
        else if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG)
            return SyT(AARCH64_INS, { SAcop(0), SAcop(1), SAcop(2) });
        break;
    case (OP_UNSPILL):
        if(index.size() == 2) 
        {
            if(index[0].tag == Arg::IREG) 
                return SyT(AARCH64_LDR, { SAcopelt(0, TYPE_I64), SAreg(SP), SAcop(1) });
            else if(index[0].tag == Arg::VREG) 
                return SyT(AARCH64_LDR, { SAcop(0), SAreg(SP), SAcopsar(1,1) });
        }
        break;
    case (OP_SPILL):
        if(index.size() == 2) 
        {
            if(index[1].tag == Arg::IREG) 
                return SyT(AARCH64_STR, { SAcopelt(1, TYPE_I64), SAreg(SP), SAcop(0) });
            else if(index[1].tag == Arg::VREG) 
                return SyT(AARCH64_STR, { SAcop(1), SAreg(SP), SAcopsar(0,1) });
        }
        break;
    case (OP_JCC):
        if(index.size() == 2 && index[0].tag == Arg::IIMMEDIATE && index[1].tag == Arg::IIMMEDIATE)
            switch (index[0].value)
            {
            case (OP_NE):   return SyT(AARCH64_B_NE,{ SAcop(1) });
            case (OP_EQ):   return SyT(AARCH64_B_EQ,{ SAcop(1) });
            case (OP_LT):   return SyT(AARCH64_B_LT,{ SAcop(1) });
            case (OP_GT):   return SyT(AARCH64_B_GT,{ SAcop(1) });
            case (OP_UGT):  return SyT(AARCH64_B_HI,{ SAcop(1) });
            case (OP_LE):   return SyT(AARCH64_B_LE,{ SAcop(1) });
            case (OP_ULE):  return SyT(AARCH64_B_LS,{ SAcop(1) });
            case (OP_GE):   return SyT(AARCH64_B_GE,{ SAcop(1) });
            default:
                break;
            };
        break;
    case (OP_CALL_NORET):
        if(index.size() == 1 && index[0].tag == Arg::IREG)
             return SyT(AARCH64_BLR, { SAcop(0) });
        break;
    case (OP_JMP):      return SyT(AARCH64_B,   { SAcop(0) });
    case (OP_LABEL):    return SyT(AARCH64_LABEL, { SAcop(0) });
    case (OP_RET):      return SyT(AARCH64_RET, { SAreg(LR) });
    default:
        break;
    };
    scs = false;
    return SyntopTranslation();
}

class AArch64BigImmediates : public CompilerPass
{
public:
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override;
    virtual ~AArch64BigImmediates() override {}
    virtual bool is_inplace() const override final { return false; }
    virtual std::string pass_id() const override final { return "CP_AARCH64_BIG_IMMEDIATES"; }
    static CompilerPassPtr make(const Backend* a_backend)
    {
        std::shared_ptr<AArch64BigImmediates> res;
        res.reset(new AArch64BigImmediates(a_backend));
        return std::static_pointer_cast<CompilerPass>(res);
    } 
private:
    AArch64BigImmediates(const Backend* a_backend) : CompilerPass(a_backend) {}
};

class AArch64ARASnippets : public CompilerPass
{
public:
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override;
    virtual bool is_inplace() const override final { return false; }
    virtual std::string pass_id() const override final { return "CP_AARCH64_SNIPPETS"; }
    virtual ~AArch64ARASnippets() override {}
    static CompilerPassPtr make(const Backend* a_backend)
    {
        std::shared_ptr<AArch64ARASnippets> res;
        res.reset(new AArch64ARASnippets(a_backend));
        return std::static_pointer_cast<CompilerPass>(res);
    } 
private: 
    AArch64ARASnippets(const Backend* a_backend) : CompilerPass(a_backend) {}
};

Aarch64Backend::Aarch64Backend()
{
    m_s2blookup = a64BTLookup;
    m_s2slookup = a64STLookup;
    m_vectorRegisterBits = 128;
    m_isLittleEndianInstructions = true;
    m_isLittleEndianOperands = false;
    m_isMonowidthInstruction = true;
    m_instructionWidth = 4;
    m_offsetShift = 2;
    m_callerStackIncrement = 2;
    m_postInstructionOffset = false;
    m_registersAmount = 7;
    m_name = "AArch64";
    m_beforeRegAllocPasses.push_back(AArch64BigImmediates::make(this));
    m_afterRegAllocPasses.push_back(AArch64ARASnippets::make(this));
    m_parameterRegisters[RB_INT] = { R0, R1, R2, R3, R4, R5, R6, R7 };
    m_returnRegisters[RB_INT] = { R0, R1, R2, R3, R4, R5, R6, R7 };
    m_callerSavedRegisters[RB_INT] = { XR, R9, R10, R11, R12, R13, R14, R15, IP0, IP1 };
    m_calleeSavedRegisters[RB_INT] = { /*PR, TODO: sort out with calling convention and meaning of this register.*/ R19, R20, R21, R22, R23, R24, R25, R26, R27, R28 };

    m_parameterRegisters[RB_VEC] = { Q0, Q1, Q2, Q3, Q4, Q5, Q6, Q7 };
    m_returnRegisters[RB_VEC] = { Q0, Q1, Q2, Q3 };
#if __LOOPS_OS == __LOOPS_MAC
    m_callerSavedRegisters[RB_VEC] = { Q8, Q9, Q10, Q11, Q12, Q13, Q14, Q15 };
    m_calleeSavedRegisters[RB_VEC] = { Q16, Q17, Q18, Q19, Q20, Q21, Q22, Q23, Q24, Q25, Q26, Q27, Q28, Q29, Q30, Q31 };
#elif __LOOPS_OS == __LOOPS_LINUX
    m_callerSavedRegisters[RB_VEC] = { };
    m_calleeSavedRegisters[RB_VEC] = { Q8, Q9, Q10, Q11, Q12, Q13, Q14, Q15, Q16, Q17, Q18, Q19, Q20, Q21, Q22, Q23, Q24, Q25, Q26, Q27, Q28, Q29, Q30, Q31 };
#else
#error Unsupported OS
#endif
}

Aarch64Backend::~Aarch64Backend()
{}

int Aarch64Backend::reusingPreferences(const Syntop& a_op, const std::set<int>& undefinedArgNums) const
{
    switch (a_op.opcode)
    {
    case (OP_MOV):
    case (VOP_FMA):
    case (VOP_SELECT):
    {
        if (undefinedArgNums.count(1))
            return 1;
        break;
    }
    default:
        break;
    }
    return Backend::reusingPreferences(a_op, undefinedArgNums);
}

#define LOOPS_VCALLER_SAVED_AMOUNT 16

int Aarch64Backend::spillSpaceNeeded(const Syntop& a_op, int basketNum) const
{
    if(basketNum == RB_INT)
        switch (a_op.opcode)
        {
        case (OP_CALL):
        case (OP_CALL_NORET):
            return 18;
        case (OP_MOD):
            return 1;
            break;
        default:
            break;
        }
    else if(basketNum == RB_VEC)
        switch (a_op.opcode)
        {
        case (OP_CALL):
        case (OP_CALL_NORET):
            return LOOPS_VCALLER_SAVED_AMOUNT;
        case (VOP_FMA):
        case (VOP_SELECT):
            if(a_op.size() == 5) 
                return 2;
            if(a_op.size() == 4) 
                return 1;
            break;
        default:
            break;
        }
    return Backend::spillSpaceNeeded(a_op, basketNum);
}

std::set<int> Aarch64Backend::getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask) const
{
    //TODO(ch): This specialized version of function must disappear after introducing snippets.
    //They will give info about used registers, like now instructions answers.
    //Actually, it's easy to think, that we have to keep used registers info on level of SyntopTranslation. Hmm...
    switch (a_op.opcode)
    {
        case (OP_MIN):
        case (OP_MAX):
        {
            Assert(a_op.size() == 3 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IREG && a_op[2].tag == Arg::IREG);
            if (basketNum == RB_INT && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
            {
                if (AF_INPUT & flagmask)
                    return std::set<int>({1,2});
                if (AF_OUTPUT & flagmask)
                    return std::set<int>({0});
            }
            break;
        }
        case (OP_ABS):
        case (OP_SIGN):
        {
            Assert(a_op.size() == 2 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IREG);
            if (basketNum == RB_INT && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
            {
                if (AF_INPUT & flagmask)
                    return std::set<int>({1});
                if (AF_OUTPUT & flagmask)
                    return std::set<int>({0});
            }
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
            if(basketNum == RB_VEC)
                return std::set<int>({});
            if (basketNum == RB_INT && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
            {
                if (AF_INPUT & flagmask)
                {
                    std::set<int> res;
                    for(int arnum = (a_op.opcode == OP_CALL? 1 : 0); arnum < a_op.size(); arnum++ ) res.insert(arnum);
                    return res;
                }
                if (AF_OUTPUT & flagmask)
                {
                    if(a_op.opcode == OP_CALL)
                        return std::set<int>({0});
                    else
                        return std::set<int>({});
                }
            }
            break;
        }
        case (VOP_FMA):
        case (VOP_SELECT):
        {
            Assert((a_op.size() == 4 && a_op[0].tag == Arg::VREG && a_op[1].tag == Arg::VREG && a_op[2].tag == Arg::VREG && a_op[3].tag == Arg::VREG) ||
                   (a_op.size() == 5 && a_op[0].tag == Arg::VREG && a_op[1].tag == Arg::VREG && a_op[2].tag == Arg::VREG && a_op[3].tag == Arg::VREG && a_op[4].tag == Arg::IIMMEDIATE));
            if(basketNum == RB_INT)
                return std::set<int>({});
            else if (basketNum == RB_VEC && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
            {
                if (AF_INPUT & flagmask)
                    return std::set<int>({1, 2, 3});
                else if (AF_OUTPUT & flagmask)
                    return std::set<int>({0});
            }
            break;
        }
        case (VOP_ARM_LD2):
        {
            Assert(a_op.size() == 3 && a_op[0].tag == Arg::VREG && a_op[1].tag == Arg::VREG && a_op[2].tag == Arg::IREG && a_op[0].elemtype == a_op[1].elemtype);
            if (basketNum == RB_INT && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
            {
                if (AF_INPUT & flagmask)
                    return std::set<int>({2});
                if (AF_OUTPUT & flagmask)
                    return std::set<int>({});
            }
            else if (basketNum == RB_VEC && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
            {
                if (AF_INPUT & flagmask)
                    return std::set<int>({});
                if (AF_OUTPUT & flagmask)
                    return std::set<int>({0,1});
            }
            break;
        }
        case (OP_MOV):
            //mov ax, 0 is represented as xor ax, ax. Such approach changes default in/out register distribution. There we are fixing it.
            if ( (a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IIMMEDIATE && a_op[1].value == 0) &&
                 ((~(AF_INPUT | AF_OUTPUT) & flagmask) == 0) )
            {
                if(basketNum == RB_VEC)
                    return std::set<int>({});
                if (AF_INPUT & flagmask)
                    return std::set<int>({});
                if (AF_OUTPUT & flagmask)
                    return std::set<int>({0});
            }
            break;
        default:
            break;
    };
    return Backend::getUsedRegistersIdxs(a_op, basketNum, flagmask);
}

void Aarch64Backend::getStackParameterLayout(const Syntfunc& a_func, const std::vector<int> (&regParsOverride)[RB_AMOUNT], std::map<RegIdx, int> (&parLayout)[RB_AMOUNT]) const
{
    size_t regPassed[RB_AMOUNT];
    for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        regPassed[basketNum] = regParsOverride[basketNum].size() ? regParsOverride[basketNum].size() : m_parameterRegisters[basketNum].size();
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
        parLayout[basketNum][arg.idx] = currOffset;
        currOffset+=xBasket[basketNum];
    }
}

int Aarch64Backend::stackGrowthAlignment(int stackGrowth) const
{
    return stackGrowth ? stackGrowth + (stackGrowth % 2) : stackGrowth;        //TODO(ch): Align to 16 or 32 if SIMD's are used.
}

void Aarch64Backend::writeCallerPrologue(Syntfunc& prog, int stackGrowth) const
{
    if((stackGrowth - 2) < 64)
    {
        prog.program.push_back(Syntop(OP_ARM_STP, { argReg(RB_INT, SP), argIImm(stackGrowth-2), argReg(RB_INT, FP), argReg(RB_INT, LR) }));
    }
    else
    {
        prog.program.push_back(Syntop(OP_SPILL, { argIImm(stackGrowth-2), argReg(RB_INT, FP) }));
        prog.program.push_back(Syntop(OP_SPILL, { argIImm(stackGrowth-1), argReg(RB_INT, LR) }));
    }
    prog.program.push_back(Syntop(OP_MOV,   { argReg(RB_INT, FP), argReg(RB_INT, SP)}));
}

void Aarch64Backend::writeCallerEpilogue(Syntfunc& prog, int stackGrowth) const
{
    if((stackGrowth - 2) < 64)
    {
        prog.program.push_back(Syntop(OP_ARM_LDP, { argReg(RB_INT, FP), argReg(RB_INT, LR), argReg(RB_INT, SP), argIImm(stackGrowth-2) }));
    }
    else
    {
        prog.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT, FP),   argIImm(stackGrowth-2) }));
        prog.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT, LR),   argIImm(stackGrowth-1) }));
    }
}

Arg Aarch64Backend::getSParg() const
{
    return argReg(RB_INT, SP);
}

column_printer Aarch64Backend::get_opname_printer() const
{
    column_printer ret = { /*func = */ &col_opname_table_printer, /*auxdata = */ opstrings , /*free_func = */ NULL };
    return ret;
}

typedef struct aarch64_opargs_printer_aux
{
    LOOPS_HASHMAP(int, int) pos2opnum;
    LOOPS_SPAN(int) positions;
} aarch64_opargs_printer_aux;

static int aarch64_opargs_printer(program_printer* printer, column_printer* colprinter, syntfunc2print* func, int row)
{
    int program_size = func->program->size;
    loops::Syntop* program = func->program->data;
    int err;
    aarch64_opargs_printer_aux* argaux = (aarch64_opargs_printer_aux*)colprinter->auxdata;
    if (argaux == NULL)
    {
        int oppos = 0;
        int opnum = 0;
        argaux = (aarch64_opargs_printer_aux*)malloc(sizeof(aarch64_opargs_printer_aux));
        if (argaux == NULL)
            LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
        memset(argaux, 0, sizeof(aarch64_opargs_printer_aux));
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
            int opcode = program[opnum].opcode;
            int opsize = (opcode == AARCH64_LABEL ? 0 : 4);
            argaux->positions->data[opnum] = oppos;
            if(opcode == AARCH64_LABEL)
                loops_hashmap_add(argaux->pos2opnum, oppos, opnum);
            oppos += opsize;
        }
        colprinter->auxdata = argaux;
    }
    
    Syntop* op = program + row;
    uint64_t operand_flags[Syntop::SYNTOP_ARGS_MAX];
    printer->backend->fill_native_operand_flags(op, operand_flags);
    int aamount = op->args_size;

    for(int anum = 0; anum < aamount ; anum++)
    {
        Arg arg = op->args[anum];
        //Exceptions block:
        if(op->opcode == AARCH64_MOVK && anum == 2)
        {
            Assert(arg.tag == Arg::IIMMEDIATE);
            LOOPS_CALL_THROW(loops_printf(printer, "lsl #%d", (int)(arg.value << 4)));
            continue;
        }
        if (operand_flags[anum] & AF_PRINTOFFSET)
        {
            int targetline;
            if (arg.tag != Arg::IIMMEDIATE)
                LOOPS_THROW(LOOPS_ERR_INCORRECT_ARGUMENT);
            int offset2find = argaux->positions->data[row + 1] + 4 * (int)arg.value - 4;
            err = loops_hashmap_get(argaux->pos2opnum, offset2find, &targetline);
            if(err == LOOPS_ERR_ELEMENT_NOT_FOUND)
                LOOPS_THROW(LOOPS_ERR_INTERNAL_INCORRECT_OFFSET);
            else if(err != LOOPS_ERR_SUCCESS)
                LOOPS_THROW(err);
            Assert(targetline >= 0);
            Syntop* labelop = program + targetline;
            Assert(labelop->opcode == AARCH64_LABEL);
            Assert(labelop->opcode == AARCH64_LABEL && labelop->args_size == 1);
            Assert(labelop->opcode == AARCH64_LABEL && labelop->args_size == 1 && labelop->args[0].tag == Arg::IIMMEDIATE);
            LOOPS_CALL_THROW(loops_printf(printer, "__loops_label_%d", (int)(labelop->args[0].value)));
            continue;
        }
        //DUBUG: 
        // 1.) There have to be correspondance of printed arguments and Syntop arglist. Options can be transmitted via flags of arguments.
        // 2.) Make everything compilable in debian's assembler.
        bool address = (operand_flags[anum] & AF_ADDRESS) == AF_ADDRESS;
        if (address)
            LOOPS_CALL_THROW(loops_printf(printer, "["));
        switch (arg.tag)
        {
            case Arg::IREG:
                if(arg.idx == (int)Syntfunc::RETREG)
                    LOOPS_CALL_THROW(loops_printf(printer, "xR"));
                else if(arg.idx == 31)
                    LOOPS_CALL_THROW(loops_printf(printer, "sp"));
                else
                {
                    bool w32 = false;
                    if((operand_flags[anum] & AF_EFFECTIVE64) == 0)
                    w32 = (arg.elemtype == TYPE_FP32) || (arg.elemtype == TYPE_U32) || (arg.elemtype == TYPE_I32)  ||
                          (arg.elemtype == TYPE_FP16) || (arg.elemtype == TYPE_U16) || (arg.elemtype == TYPE_I16)  ||
                                                         (arg.elemtype == TYPE_U8)  || (arg.elemtype == TYPE_I8);
                    LOOPS_CALL_THROW(loops_printf(printer, "%s%d", (w32 ? "w" : "x"), arg.idx));
                }
                break;
            case Arg::VREG:
            {
                static const char* Vsuffixes[] = {"", "16b", "8h", "", "4s", "", "", "", "2d" };
                LOOPS_CALL_THROW(loops_printf(printer, "v%d.%s", arg.idx, Vsuffixes[elem_size(arg.elemtype)]));
                break;
            }
            case Arg::IIMMEDIATE:
                if(op->opcode == AARCH64_LABEL)
                {
                    Assert(op->args_size == 1);
                    LOOPS_CALL_THROW(loops_printf(printer, "__loops_label_%d:", arg.value));
                    break;
                }
                if(arg.value == 0)
                    LOOPS_CALL_THROW(loops_printf(printer, "#0"));
                else
                {
                    uint32_t upper32 = ((uint64_t)arg.value) >> 32;
                    uint32_t lower32 = ((uint64_t)arg.value) & 0xffffffff;
                    if (upper32 > 0)
                        LOOPS_CALL_THROW(loops_printf(printer, "#0x%x%08x", upper32, lower32));
                    else
                        LOOPS_CALL_THROW(loops_printf(printer, "#0x%02x", lower32));
                }
                break;
            default:
                throw std::runtime_error("Undefined argument type."); //DUBUG: wat?
        };
        if (address)
            LOOPS_CALL_THROW(loops_printf(printer, "]"));
        if (anum < aamount - 1)
            LOOPS_CALL_THROW(loops_printf(printer, ", "));
    }
    LOOPS_CALL_THROW(close_printer_cell(printer));
    return LOOPS_ERR_SUCCESS;
}

static void free_aarch64_opargs_printer(column_printer* colprinter)
{
    if (colprinter->auxdata != NULL)
    {
        aarch64_opargs_printer_aux* argaux = (aarch64_opargs_printer_aux*)colprinter->auxdata;
        loops_hashmap_destruct(argaux->pos2opnum);
        loops_span_destruct(argaux->positions);
        free(argaux);
        colprinter->auxdata = NULL;
    }
}

column_printer Aarch64Backend::get_opargs_printer() const
{
    column_printer ret = { /*func = */ &aarch64_opargs_printer, /*auxdata = */ NULL, /*free_func = */ &free_aarch64_opargs_printer };
    return ret;
}

typedef struct aarch64_hex_printer_aux
{
    LOOPS_SPAN(int) positions;
    LOOPS_SPAN(uint8_t) binary;
} aarch64_hex_printer_aux;

static int aarch64_hex_printer(program_printer* printer, column_printer* colprinter, syntfunc2print* func, int row)
{
    int err;
    int program_size = func->program->size;
    loops::Syntop* program = func->program->data;
    int params_size = func->params->size;
    loops::Arg* params = func->params->data;

    aarch64_hex_printer_aux* argaux = (aarch64_hex_printer_aux*)colprinter->auxdata;
    if (argaux == NULL)
    {
        int oppos = 0;
        int opnum = 0;
        argaux = (aarch64_hex_printer_aux*)malloc(sizeof(aarch64_hex_printer_aux));
        if (argaux == NULL)
            LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
        memset(argaux, 0, sizeof(aarch64_hex_printer_aux));
        err = loops_span_construct_alloc(&(argaux->positions), program_size);
        if (err != LOOPS_ERR_SUCCESS)
        {
            free(argaux);
            LOOPS_THROW(err);
        }
        for (; opnum < program_size; opnum++)
        {
            int opsize = (program[opnum].opcode == AARCH64_LABEL ? 0 : 4);
            argaux->positions->data[opnum] = oppos;
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
                loops_span_destruct(argaux->positions);
                free(argaux);
                LOOPS_THROW(err);
            }
            memcpy(argaux->binary->data, buffer->data(), argaux->binary->size);
        }
        colprinter->auxdata = argaux;
    }
    if(program[row].opcode != AARCH64_LABEL)
    {
        unsigned char* hexfield = argaux->binary->data + argaux->positions->data[row];
        for(size_t pos = 0; pos < 4; pos++) //TODO(ch): Print variants (direct or reverse order).
            LOOPS_CALL_THROW(loops_printf(printer, "%02x ", (unsigned)(*(hexfield + pos))));
    }
    LOOPS_CALL_THROW(close_printer_cell(printer));
    return LOOPS_ERR_SUCCESS;
}

static void free_aarch64_hex_printer(column_printer* colprinter)
{
    if (colprinter->auxdata != NULL)
    {
        aarch64_hex_printer_aux* argaux = (aarch64_hex_printer_aux*)colprinter->auxdata;
        loops_span_destruct(argaux->positions);
        loops_span_destruct(argaux->binary);
        free(argaux);
        colprinter->auxdata = NULL;
    }
}

column_printer Aarch64Backend::get_hex_printer() const
{
    column_printer ret = { /*func = */ &aarch64_hex_printer, /*auxdata = */ NULL, /*free_func = */ &free_aarch64_hex_printer };
    return ret;
}

void AArch64BigImmediates::process(Syntfunc& a_dest, const Syntfunc& a_source)
{
    //Note: Also handle some before-register-allocation snippets.
    a_dest.name = a_source.name;
    a_dest.params = a_source.params;
    for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        a_dest.regAmount[basketNum] = a_source.regAmount[basketNum];
    a_dest.program.reserve(2 * a_source.program.size());
    for (const Syntop& op : a_source.program)
        switch (op.opcode)
        {
        case OP_MOV:
        {
            Assert(op.size() == 2);
            if(op[1].tag != Arg::IIMMEDIATE)
            {
                a_dest.program.push_back(op);
                break;
            }
            if(m_backend->isImmediateFit(op, 1))
            {
                a_dest.program.push_back(op);
                break;
            }
            size_t wordAmount = 4; //8byte
            Arg idest = op[0];
            if(idest.tag == Arg::VREG)
            {
                wordAmount = elem_size(op[0].elemtype)/2;
                idest = argReg(RB_INT, a_dest.provideIdx(RB_INT));
            }
            bool negative = (((op[0].tag == Arg::VREG && isSignedInteger(op[0].elemtype)) || op[0].tag == Arg::IREG ) && op[0].value < 0 );
            std::vector<int64_t> words(wordAmount, negative ? -1: 0);
            for(size_t wNum = wordAmount - 1; wNum > 0; wNum--)
            {
                int64_t word = (op[1].value >> (wNum*16))&0xFFFF;
                if((negative && ((~word)&0xFFFF)) || (!negative && word))
                    words[wNum] = word;
            }
            words[0] = negative ? ~((~op[1].value) & 0xFFFF) : (op[1].value & 0xFFFF);
            a_dest.program.push_back(Syntop(OP_MOV, { idest, argIImm(words[0]) }));
            for(size_t wNum = 1; wNum < wordAmount; wNum++)
                if((negative && words[wNum]!=-1) || (!negative && words[wNum]!=0))
                    a_dest.program.push_back(Syntop(OP_ARM_MOVK, { idest, argIImm(words[wNum]), argIImm(wNum*16) }));
            if(op[0].tag == Arg::VREG)
                a_dest.program.push_back(Syntop(VOP_BROADCAST, { op[0], idest}));
            break;
        }
        case VOP_SHRINK:
        {
            Assert(op.size() == 3 && op[0].tag == Arg::VREG && op[1].tag == Arg::VREG && op[2].tag == Arg::VREG &&
                   op[1].elemtype == op[2].elemtype && 2 * elem_size(op[0].elemtype) == elem_size(op[1].elemtype));
            a_dest.program.push_back(Syntop(VOP_ARM_SHRINK_LOW, { op[0], op[1]}));
            a_dest.program.push_back(Syntop(VOP_ARM_SHRINK_HIGH, { op[0], op[2]}));
            break;
        }
        default:

            a_dest.program.push_back(op);
            break;
        }
}

void AArch64ARASnippets::process(Syntfunc& a_dest, const Syntfunc& a_source)
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
            //TODO(ch): also kill UNSPILL and SPILL.
            Assert(op.size() == 2); 
            if(!(((op[0].tag == Arg::IREG && op[1].tag == Arg::IREG ) || 
                (op[0].tag == Arg::VREG && op[1].tag == Arg::VREG ))
                && op[0].idx == op[1].idx))
                a_dest.program.push_back(op);
            break;
        case OP_MIN:
        case OP_MAX:
            Assert(op.size() == 3 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG && op[2].tag == Arg::IREG);
            a_dest.program.push_back(Syntop(OP_CMP, { op[1], op[2] }));
            a_dest.program.push_back(Syntop(OP_SELECT, { op[0], op.opcode == OP_MIN ? OP_LT : OP_GT, op[1], op[2] }));
            break;
        case OP_MOD:
            Assert(op.size() == 3 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG && op[2].tag == Arg::IREG);
            if(op[1].idx == op[2].idx)
            {
                a_dest.program.push_back(Syntop(OP_DIV, { op[0], op[1], op[1] }));
                a_dest.program.push_back(Syntop(OP_MOV, { op[0], argIImm(0) }));
            }
            else
            {
                Arg divided = op[1];
                Arg divisor = op[2];
                RegIdx placeholder = IReg::NOIDX;
                if(op[0].idx == op[2].idx)
                {
                    placeholder = lsb64(~makeBitmask64({(size_t)(op[0].idx), size_t(op[1].idx)}));
                    divisor = argReg(RB_INT, placeholder);
                    a_dest.program.push_back(Syntop(OP_SPILL, { 0, divisor }));
                    a_dest.program.push_back(Syntop(OP_MOV, { divisor, op[2] }));
                }
                if(op[0].idx == op[1].idx)
                {
                    placeholder = lsb64(~makeBitmask64({(size_t)(op[0].idx), size_t(op[2].idx)}));
                    divided = argReg(RB_INT, placeholder);
                    a_dest.program.push_back(Syntop(OP_SPILL, { 0, divided }));
                    a_dest.program.push_back(Syntop(OP_MOV, { divided, op[1] }));
                }
                a_dest.program.push_back(Syntop(OP_DIV, { op[0], divided, divisor }));
                a_dest.program.push_back(Syntop(OP_MUL, { op[0], op[0], divisor }));
                a_dest.program.push_back(Syntop(OP_SUB, { op[0], divided, op[0] }));
                if(placeholder != IReg::NOIDX)
                    a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT, placeholder), 0 }));
            }
            break;
        case OP_SIGN:
            Assert(op.size() == 2 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG);
            a_dest.program.push_back(Syntop(OP_CMP, { op[1], argIImm(0) }));
            a_dest.program.push_back(Syntop(OP_SAR, { op[0], op[1], argIImm(63) }));
            a_dest.program.push_back(Syntop(OP_ARM_CINC,{ op[0], op[0], argIImm(OP_GT) }));
            break;
        case OP_ABS:
            Assert(op.size() == 2 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG);
            a_dest.program.push_back(Syntop(OP_CMP, { op[1], argIImm(0) }));
            a_dest.program.push_back(Syntop(OP_ARM_CNEG,{ op[0], op[1], argIImm(OP_LT) }));
            break;
        case VOP_NE:
            Assert(op.size() == 3 && op[0].tag == Arg::VREG && op[1].tag == Arg::VREG && op[2].tag == Arg::VREG && op[1].elemtype == op[2].elemtype && elem_size(op[0].elemtype) == elem_size(op[1].elemtype) && isUnsignedInteger(op[0].elemtype));
            a_dest.program.push_back(Syntop(VOP_EQ , { op[0], op[1], op[2] }));
            a_dest.program.push_back(Syntop(VOP_NOT, { op[0], op[0] }));
            break;
        case VOP_FMA:
        case VOP_SELECT:
        {
            Syntop op_= op;
            Assert((op_.size() == 4 && op_[0].tag == Arg::VREG && op_[1].tag == Arg::VREG && op_[2].tag == Arg::VREG && op_[3].tag == Arg::VREG) ||
                (op_.size() == 5 && op_[0].tag == Arg::VREG && op_[1].tag == Arg::VREG && op_[2].tag == Arg::VREG && op_[3].tag == Arg::VREG && op_[4].tag == Arg::IIMMEDIATE));
            bool laneVersion = (op_.size() == 5);
            if(op_[0].idx == op_[1].idx)
            {
                a_dest.program.push_back(op_);
                break;
            }
            bool unspill = false;
            bool unspillFp16 = false;
            Arg placeholder = op_[2];
            Arg placeholderfp16;
            int placeholderSPoff = 0;
            if(laneVersion && op_[0].elemtype == TYPE_FP16 && op_[3].idx > 15)
            {
                placeholderfp16 = op_[3];
                placeholderfp16.idx = lsb64(~makeBitmask64({(size_t)(op_[0].idx), size_t(op_[1].idx), (size_t)(op_[2].idx)}));
                a_dest.program.push_back(Syntop(OP_SPILL, { 0, placeholderfp16 })); 
                a_dest.program.push_back(Syntop(OP_MOV , { placeholderfp16, op_[3] }));
                op_[3] = placeholderfp16;
                const int vectorSize = 16; //TODO(ch): replace with Backend::vectorSize call. 
                placeholderSPoff = vectorSize / 8; 
            }
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
            if(laneVersion)
                a_dest.program.push_back(Syntop(op_.opcode, { op_[0], op_[0], op_[2], op_[3], op_[4] }));
            else
                a_dest.program.push_back(Syntop(op_.opcode, { op_[0], op_[0], op_[2], op_[3] }));
            if(unspillFp16)
                a_dest.program.push_back(Syntop(OP_UNSPILL, { placeholderfp16, 0 }));
            if(unspill)
                a_dest.program.push_back(Syntop(OP_UNSPILL, { placeholder, placeholderSPoff }));
            break;
        }
        case OP_CALL:
        case OP_CALL_NORET:
        {
            Assert((op.opcode == OP_CALL && op.size() >= 2 && op.size() <= 10) ||
                   (op.opcode == OP_CALL_NORET && op.size() >= 1 && op.size() < 10) );
            Arg sp = argReg(RB_INT, SP);
            int retidx = op.opcode == OP_CALL ? op[0].idx : 0;
            std::vector<std::pair<int, std::pair<int, int> > > spillLayout = {
                {0, {R0, R1}}, {2, {R2, R3}}, {4, {R4, R5}}, {6, {R6, R7}},{8, {XR, R9}}, {10, {R10, R11}}, {12, {R12, R13}}, {14, {R14, R15}}, {16, {IP0, IP1}}} ;
            //1.) Save scalar registers
            for(auto layPair : spillLayout)
                a_dest.program.push_back(Syntop(OP_ARM_STP, { sp, argIImm(layPair.first), argReg(RB_INT,  layPair.second.first), argReg(RB_INT,  layPair.second.second) }));
            //2.) Prepare arguments accordingly to ABI. Call address must not be broken
            Arg addrkeeper = op.opcode == OP_CALL ? op[1]: op[0];
            //TODO(ch) : make this algo optimized with help of permutation analysis.
            std::set<int> brokenRegs;
            for(int fargnum = (op.opcode == OP_CALL ? 2 : 1); fargnum < op.size(); fargnum++)
            {
                Assert(op[fargnum].tag == Arg::IREG);
                int regnum = fargnum - (op.opcode == OP_CALL ? 2 : 1);
                if(op[fargnum].idx != regnum)
                {
                    if(brokenRegs.find(op[fargnum].idx) == brokenRegs.end())
                        a_dest.program.push_back(Syntop(OP_MOV, { argReg(RB_INT,  regnum), argReg(RB_INT,  op[fargnum].idx)}));
                    else
                        a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT,  regnum), argIImm(op[fargnum].idx)}));
                    brokenRegs.insert(regnum);
                }
            }
            if(brokenRegs.find(addrkeeper.idx) != brokenRegs.end())
            {
                addrkeeper.idx = R9;
                a_dest.program.push_back(Syntop(OP_UNSPILL, { addrkeeper, argIImm((op.opcode == OP_CALL ? op[1]: op[0]).idx)}));
            }
            //3.) Save vector registers
            Arg vcaller_ptr = argReg(RB_INT, addrkeeper.idx == R10 ? R11 : R10);//addrkeeper = op.opcode == OP_CALL ? op[1]: op[0];
            a_dest.program.push_back(Syntop(OP_ADD, { vcaller_ptr, argReg(RB_INT, SP), argIImm((spillLayout.back().first + 2) * 8)}));
            for(int _0idx = 0; _0idx < LOOPS_VCALLER_SAVED_AMOUNT; _0idx+=4 )
                a_dest.program.push_back(Syntop(VOP_ARM_ST1, { vcaller_ptr, vregHid<uint64_t>(_0idx, 0)}));
            //4.) Call function
            a_dest.program.push_back(Syntop(OP_CALL_NORET, { addrkeeper }));
            //5.) Restore vector registers
            a_dest.program.push_back(Syntop(OP_ADD, { vcaller_ptr, argReg(RB_INT, SP), argIImm((spillLayout.back().first + 2) * 8)}));
            for(int _0idx = 0; _0idx < LOOPS_VCALLER_SAVED_AMOUNT; _0idx+=4 )
                a_dest.program.push_back(Syntop(VOP_ARM_LD1, { vregHid<uint64_t>(_0idx, 0), vcaller_ptr}));
            //6.) Move result to output register
            if(op.opcode == OP_CALL && retidx != 0)
                a_dest.program.push_back(Syntop(OP_MOV, { op[0], argReg(RB_INT, R0)}));
            //7.) Restore scalar registers
            for(auto layPair : spillLayout)
            {
                if(op.opcode == OP_CALL && (retidx == layPair.second.first || retidx == layPair.second.second))
                {
                    int unspillidx = retidx == layPair.second.first ? layPair.second.second : layPair.second.first;
                    int unspillspc = layPair.first + (retidx == layPair.second.first ? 1 : 0);
                    a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT,  unspillidx), argIImm(unspillspc)}));
                }
                else
                    a_dest.program.push_back(Syntop(OP_ARM_LDP, { argReg(RB_INT,  layPair.second.first), argReg(RB_INT,  layPair.second.second), argReg(RB_INT, SP), argIImm(layPair.first) }));
            }
            break;
        }
        default:
            a_dest.program.push_back(op);
            break;
        }
}
}
#endif //__LOOPS_ARCH == __LOOPS_AARCH64
