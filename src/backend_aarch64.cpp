/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
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
    AARCH64_IC_LE = 0b1101,
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

static inline int ICbytecode2Aarch64(int ic)
{
    return ic == IC_NE ? AARCH64_IC_NE : (
           ic == IC_EQ ? AARCH64_IC_EQ: (
           ic == IC_GE ? AARCH64_IC_GE : (
           ic == IC_LE ? AARCH64_IC_LE : (
           ic == IC_GT ? AARCH64_IC_GT: (
           ic == IC_LT ? AARCH64_IC_LT: (
           ic == IC_S  ? AARCH64_IC_MI : (
         /*ic == IC_NS?*/AARCH64_IC_PL /* : throw error*/)))))));
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
        if (index.size() == 4)
        {
            if (index[3].tag == Arg::IREG)
                return BiT({ BTsta(1, 1), BTimm(0, 1), BTsta(0b111000011, 9), BTreg(3, 5, In),BTsta(0b011010, 6), BTreg(2, 5, In), BTreg(1, 5, Out) });
            else if (index[3].tag == Arg::IIMMEDIATE)
                return BiT({ BTsta(0x1, 1), BTimm(0, 1), BTsta(0xE5, 8), BTimm(3, 12), BTreg(2, 5, In), BTreg(1, 5, Out) });
        }
        else if (index.size() == 3)
        {
            if (index[0].tag == Arg::VREG && index[1].tag == Arg::IREG)
            {
                if (index[2].tag == Arg::IREG)
                    return BiT({ BTsta(0b00111100111, 11), BTreg(2, 5, In), BTsta(0b011010, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
                if (index[2].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0b0011110111, 10), BTimm(2, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
        }
        break;
    case (AARCH64_LDRSW):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b10111000101, 11), BTreg(2,5,In), BTsta(0b011010, 6), BTreg(1,5,In), BTreg(0, 5, Out) });
        else if (index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b1011100110, 10), BTimm(2, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_LDRH):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b01111000011, 11), BTreg(2, 5, In), BTsta(0b011010, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b0111100101, 10), BTimm(2, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_LDRSH):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b01111000101, 11), BTreg(2,5,In), BTsta(0b011010, 6), BTreg(1,5,In), BTreg(0, 5, Out) });
        else if (index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b0111100110, 10), BTimm(2,12), BTreg(1,5,In), BTreg(0,5,Out) });
        break;
            
    case (AARCH64_LDRB):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b00111000011, 11), BTreg(2,5,In), BTsta(0b011010, 6), BTreg(1,5,In), BTreg(0,5,Out) });
        else if (index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b0011100101, 10), BTimm(2,12), BTreg(1,5,In), BTreg(0,5,Out) });
        break;
    case (AARCH64_LDRSB):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b00111000101, 11), BTreg(2,5,In), BTsta(0b011010, 6), BTreg(1,5,In), BTreg(0,5,Out) });
        else if (index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b0011100110, 10), BTimm(2,12), BTreg(1,5,In), BTreg(0,5,Out) });
        break;
    case (AARCH64_STR):
        if(index.size() == 4)
        {
            if (index[3].tag == Arg::IREG)
                return BiT({ BTsta(0x1,  1), BTimm(0, 1), BTsta(0b111000001, 9), BTreg(3, 5, In), BTsta(0b011010, 6), BTreg(2, 5, In), BTreg(1, 5, In) });
            else if (index[3].tag == Arg::IIMMEDIATE)
                return BiT({ BTsta(0x1,  1), BTimm(0, 1), BTsta(0xE4, 8), BToff(3, 12), BTreg(2, 5, In), BTreg(1, 5, In) });
        }
        else if(index.size() == 3)
        {
            if (index[0].tag == Arg::VREG && index[1].tag == Arg::IREG)
            {
                if(index[2].tag == Arg::IREG)
                    return BiT({ BTsta(0b00111100101, 11), BTreg(2, 5, In), BTsta(0b011010, 6), BTreg(1, 5, In), BTreg(0, 5, In) });
                else if(index[2].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0b0011110110, 10), BTimm(2, 12), BTreg(1, 5, In), BTreg(0, 5, In) });
            }
        }
        break;
    case (AARCH64_STRH):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b01111000001, 11), BTreg(2, 5, In), BTsta(0b011010, 6), BTreg(1, 5, In), BTreg(0, 5, In) });
        else if (index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b0111100100, 10), BTimm(2, 12), BTreg(1, 5, In), BTreg(0, 5, In) });
        break;
    case (AARCH64_STRB):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b00111000001, 11), BTreg(2, 5, In), BTsta(0b011010, 6), BTreg(1, 5, In), BTreg(0, 5, In) });
        else if (index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b0011100100, 10), BTimm(2, 12), BTreg(1, 5, In), BTreg(0, 5, In) });
        break;
    case (AARCH64_MOV):
        Assert(index.size() == 2);
        if (index[1].tag == Arg::IREG)
            return BiT({ BTsta(0x550,11), BTreg(1, 5, In), BTsta(0x1F ,11), BTreg(0, 5, Out) });
        else if (index[1].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0x694,11), BTimm(1, 16), BTreg(0, 5, Out) });
        else if (index[0].tag == Arg::VREG && index[1].tag == Arg::VREG)
            return BiT({ BTsta(0b01001110101, 11), BTreg(1, 5, In), BTsta(0b000111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_MOVN):
        if(index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b10010010100, 11), BTimm(1, 16), BTreg(0, 5, Out) });
        break;
    case (AARCH64_MOVK):
        if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IIMMEDIATE &&
           index[2].value >= 1 && index[2].value <= 3)
            return BiT({ BTsta(0b111100101, 9), BTimm(2, 2), BTimm(1, 16), BTreg(0, 5, Out) });
    case (AARCH64_ADD):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BTsta(0x458, 11), BTreg(2, 5, In), BTsta(0x0, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0x244, 10), BTimm(2, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
                 index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype && isInteger(index[0].elemtype))
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01001110,8), BTsta(sizeStat,2), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b100001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_SUB):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BTsta(0x658,11), BTreg(2, 5, In), BTsta(0x0,6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0x344,10), BTimm(2, 12), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
                 index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype && isInteger(index[0].elemtype))
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01101110,8), BTsta(sizeStat,2), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b100001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_MUL):
        Assert(index.size() == 3);
        if(index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0x4D8,11), BTreg(2, 5, In), BTsta(0x1F, 6), BTreg(1, 5, In), BTreg(0,5,Out) });
        else if (index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
                 index[0].elemtype == index[1].elemtype && index[0].elemtype == index[2].elemtype && isInteger(index[0].elemtype) && index[0].elemtype <= TYPE_I32)
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01001110,8), BTsta(sizeStat,2), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b100111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_SDIV):
        Assert(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG);
        if(index[2].tag == Arg::IREG)
            return BiT({ BTsta(0x4D6,11), BTreg(2, 5, In), BTsta(0x3, 6), BTreg(1, 5, In), BTreg(0,5,Out) });
        break;
    case (AARCH64_LSL):
        if (index.size() == 3 && index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b10011010110, 11), BTreg(2, 5, In), BTsta(0b001000, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index.size() == 4 && index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b1101001101, 10), BTimm(2, 6), BTimm(3, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_LSR):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b10011010110, 11), BTreg(2, 5, In), BTsta(0b001001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index[2].tag == Arg::IIMMEDIATE)
            return BiT({ BTsta(0b1101001101, 10), BTimm(2, 6), BTsta(0b111111, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
    case (AARCH64_ASR):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BTsta(0b10011010110, 11), BTreg(2, 5, In), BTsta(0b001010, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        else if (index[2].tag == Arg::IIMMEDIATE)
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
            Assert(index.size() == 2 && index[0].tag == Arg::IREG);
            if(index[1].tag == Arg::IREG)
                return BiT({ BTsta(0x758,11), BTreg(1, 5, In), BTsta(0x0,6), BTreg(0, 5, In), BTsta(0x1F, 5) });
            if(index[1].tag == Arg::IIMMEDIATE)
                return BiT({ BTsta(0b1111000100, 10), BTimm(1, 12),  BTreg(0, 5, In), BTsta(0b11111,5)});
            break;
    case (AARCH64_CSEL):
            if (index.size() != 4 || index[0].tag != Arg::IREG || index[1].tag != Arg::IREG || index[2].tag != Arg::IREG || index[3].tag != Arg::IIMMEDIATE)
                break;
            return BiT({ BTsta(0b10011010100, 11), BTreg(2, 5, In),  BTimm(3, 4), BTsta(0b00,2), BTreg(1, 5, In), BTreg(0, 5, Out)});
            break;
    case (AARCH64_CINC): return BiT({ BTsta(0b10011010100, 11), BTreg(1,5,In), BTimm(2,4), BTsta(0b01, 2), BTreg(1,5,In), BTreg(0,5,Out) });
    case (AARCH64_CNEG): return BiT({ BTsta(0b11011010100, 11), BTreg(1,5,In), BTimm(2,4), BTsta(0b01, 2), BTreg(1,5,In), BTreg(0,5,Out) });
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
        if (index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && isInteger(index[0].elemtype) && isInteger(index[2].elemtype) && elemSize(index[2].elemtype) == elemSize(index[0].elemtype))
        {
            uint64_t sizeStat = intSizeStats[index[0].elemtype];
            return BiT({ BTsta(0b01101110,8), BTsta(sizeStat,2), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b010001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_SSHL):
        if (index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[0].elemtype == index[1].elemtype && isSignedInteger(index[1].elemtype) && isInteger(index[2].elemtype) && elemSize(index[2].elemtype) == elemSize(index[0].elemtype))
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
        if (index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IIMMEDIATE && elemSize(index[0].elemtype) != 8)
        {
            size_t elSize = elemSize(index[0].elemtype);
            int64_t cmodstat = elSize == 1 ? 0b1110 : (elSize == 2 ? 0b1000 : /*(elSize == 4 ?*/ 0b0000 /*: ...)*/);
            //TODO(ch):Reference mentions 64bit lanes. Try to implement this variation.
            return BiT({ BTsta(0b0100111100000, 13), BTimm(2, 3), BTsta(cmodstat, 4), BTsta(0b01, 2), BTimm(1, 5), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_MVNI):
        if (index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IIMMEDIATE && (index[0].elemtype == TYPE_I16 || index[0].elemtype == TYPE_I32))
        {
            size_t elSize = elemSize(index[0].elemtype);
            int64_t cmodstat = index[0].elemtype == TYPE_I16 ? 0b1000 : 0b0000;
            return BiT({ BTsta(0b0110111100000, 13), BTimm(2, 3), BTsta(cmodstat, 4), BTsta(0b01, 2), BTimm(1, 5), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_FCMGT):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elemSize(index[0].elemtype) == elemSize(index[1].elemtype) && isFloat(index[1].elemtype))
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
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elemSize(index[0].elemtype) == elemSize(index[1].elemtype) && isFloat(index[1].elemtype))
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
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elemSize(index[0].elemtype) == elemSize(index[1].elemtype) && isFloat(index[1].elemtype))
        {
            if(index[1].elemtype == TYPE_FP32 || index[1].elemtype == TYPE_FP64)
            {
                uint64_t sizeStat = index[1].elemtype == TYPE_FP64 ? 1 : 0;
                return BiT({ BTsta(0b010011100, 9), BTsta(sizeStat, 1), BTsta(0b1, 1), BTreg(2, 5, In), BTsta(0b111001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
            }
            else if(index[1].elemtype == TYPE_FP16)
                return BiT({ BTsta(0b01001110010, 11), BTreg(2, 5, In), BTsta(0b001001, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
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
    case (AARCH64_FCVTZS):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elemSize(index[0].elemtype) == elemSize(index[1].elemtype) && isSignedInteger(index[0].elemtype) && isFloat(index[1].elemtype))
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
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elemSize(index[0].elemtype) == elemSize(index[1].elemtype) && isUnsignedInteger(index[0].elemtype) && isFloat(index[1].elemtype))
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
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elemSize(index[0].elemtype) == elemSize(index[1].elemtype) && isSignedInteger(index[0].elemtype) && isFloat(index[1].elemtype))
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
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elemSize(index[0].elemtype) == elemSize(index[1].elemtype) && isUnsignedInteger(index[0].elemtype) && isFloat(index[1].elemtype))
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
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elemSize(index[0].elemtype) == elemSize(index[1].elemtype) && isSignedInteger(index[1].elemtype) && isFloat(index[0].elemtype))
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
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && elemSize(index[0].elemtype) == elemSize(index[1].elemtype) && isUnsignedInteger(index[1].elemtype) && isFloat(index[0].elemtype))
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
            int esize = elemSize(index[0].elemtype); 
            static int opcodes[9] = {-1, 0b000, 0b010 , -1, 0b100, -1, -1, -1 , 0b100}; 
            int opcode = opcodes[esize];
            Assert(opcode != -1);
            int size_field = esize == 8 ? 1 : 0;
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
        break;
    case (AARCH64_ST1):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG )
        {
            int esize = elemSize(index[0].elemtype); 
            static int opcodes[9] = {-1, 0b000, 0b010 , -1, 0b100, -1, -1, -1 , 0b100}; 
            int opcode = opcodes[esize];
            Assert(opcode != -1);
            int size_field = esize == 8 ? 1 : 0;
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
            uint64_t dupSizeStat = dupSizeStats[elemSize(index[0].elemtype)];
            Assert(dupSizeStat != WrongStat);
            return BiT({ BTsta(0b01001110000, 11), BTsta(dupSizeStat, 5), BTsta(0b000011, 6), BTreg(1, 5, In), BTreg(0, 5, Out) });
        }
        break;
    case (AARCH64_B): return BiT({ BTsta(0x5, 6), BToff(0, 26) });
        //TODO(ch): there is no B_LT, B_LE, B_GT, B_GE instructions in ARM processors, it's prespecialized versions of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
    case (AARCH64_B_NE): return BiT({ BTsta(0x54,8), BToff(0, 19), BTsta(0x1, 5) });
    case (AARCH64_B_EQ): return BiT({ BTsta(0x54,8), BToff(0, 19), BTsta(0x0, 5) });
    case (AARCH64_B_LT): return BiT({ BTsta(0x54,8), BToff(0, 19), BTsta(0xB, 5) });
    case (AARCH64_B_LE): return BiT({ BTsta(0x54,8), BToff(0, 19), BTsta(0xD, 5) });
    case (AARCH64_B_GT): return BiT({ BTsta(0x54,8), BToff(0, 19), BTsta(0xC, 5) });
    case (AARCH64_B_GE): return BiT({ BTsta(0x54,8), BToff(0, 19), BTsta(0xA, 5) });
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
        if (index.size() == 3)
        {
            switch(index[1].value)
            {
            case (TYPE_U8):
                return SyT(AARCH64_LDRB, { SAcop(0), SAcop(2, AF_ADDRESS), SAimm(0) });
            case (TYPE_I8):
                return SyT(AARCH64_LDRSB, { SAcop(0), SAcop(2, AF_ADDRESS), SAimm(0) });
            case (TYPE_U16):
                return SyT(AARCH64_LDRH, { SAcop(0), SAcop(2, AF_ADDRESS), SAimm(0) });
            case (TYPE_I16):
                return SyT(AARCH64_LDRSH, { SAcop(0), SAcop(2, AF_ADDRESS), SAimm(0) });
            case (TYPE_U32):
                return SyT(AARCH64_LDR, { SAimm(0, AF_NOPRINT), SAcop(0), SAcop(2, AF_ADDRESS), SAimm(0) });
            case (TYPE_I32):
                return SyT(AARCH64_LDRSW, { SAcop(0), SAcop(2, AF_ADDRESS), SAimm(0) });
            case (TYPE_U64): case (TYPE_I64):
                return SyT(AARCH64_LDR, { SAimm(1, AF_NOPRINT), SAcop(0), SAcop(2, AF_ADDRESS), SAimm(0) });
            };
        }
        else if (index.size() == 4)
        {
            switch(index[1].value)
            {
            case (TYPE_U8):
                return SyT(AARCH64_LDRB, { SAcop(0), SAcop(2, AF_ADDRESS), SAcop(3) });
            case (TYPE_I8):
                return SyT(AARCH64_LDRSB, { SAcop(0), SAcop(2, AF_ADDRESS), SAcop(3) });
            case (TYPE_U16):
                return SyT(AARCH64_LDRH, { SAcop(0), SAcop(2, AF_ADDRESS), index[3].tag == Arg::IIMMEDIATE ? SAcopsar(3, 1) : SAcop(3) });
            case (TYPE_I16):
                return SyT(AARCH64_LDRSH, { SAcop(0), SAcop(2, AF_ADDRESS), index[3].tag == Arg::IIMMEDIATE ? SAcopsar(3, 1) : SAcop(3) });
            case (TYPE_U32):
                return SyT(AARCH64_LDR, { SAimm(0, AF_NOPRINT), SAcop(0), SAcop(2, AF_ADDRESS), index[3].tag == Arg::IIMMEDIATE ? SAcopsar(3, 2) : SAcop(3) });
            case (TYPE_I32):
                return SyT(AARCH64_LDRSW, { SAcop(0), SAcop(2, AF_ADDRESS), index[3].tag == Arg::IIMMEDIATE ? SAcopsar(3, 2) : SAcop(3) });
            case (TYPE_U64): case (TYPE_I64):
                return SyT(AARCH64_LDR, { SAimm(1, AF_NOPRINT), SAcop(0), SAcop(2, AF_ADDRESS), index[3].tag == Arg::IIMMEDIATE ? SAcopsar(3, 3) : SAcop(3) });
            };
        }
        break;
    case (OP_STORE):
        if (index.size() == 3)
        {
            switch(index[0].value)
            {
            case (TYPE_U8): case (TYPE_I8):
                return SyT(AARCH64_STRB, { SAcop(2, AF_LOWER32), SAcop(1, AF_ADDRESS), SAimm(0) });
            case (TYPE_U16): case (TYPE_I16):
                return SyT(AARCH64_STRH, { SAcop(2, AF_LOWER32), SAcop(1, AF_ADDRESS), SAimm(0) });
            case (TYPE_U32): case (TYPE_I32):
                return SyT(AARCH64_STR, { SAimm(0, AF_NOPRINT), SAcop(2, AF_LOWER32), SAcop(1, AF_ADDRESS), SAimm(0) });
            case (TYPE_U64): case (TYPE_I64):
                return SyT(AARCH64_STR, { SAimm(1, AF_NOPRINT), SAcop(2), SAcop(1, AF_ADDRESS), SAimm(0) });
            };
        }
        else if (index.size() == 4)
        {
            switch(index[0].value)
            {
            case (TYPE_U8): case (TYPE_I8):
                return SyT(AARCH64_STRB, { SAcop(3, AF_LOWER32), SAcop(1), SAcop(2) });
            case (TYPE_U16): case (TYPE_I16):
                return SyT(AARCH64_STRH, { SAcop(3, AF_LOWER32), SAcop(1), index[2].tag == Arg::IIMMEDIATE ? SAcopsar(2, 1) : SAcop(2) });
            case (TYPE_U32): case (TYPE_I32):
                return SyT(AARCH64_STR, { SAimm(0), SAcop(3, AF_LOWER32), SAcop(1), index[2].tag == Arg::IIMMEDIATE ? SAcopsar(2, 2) : SAcop(2) });
            case (TYPE_U64): case (TYPE_I64):
                return SyT(AARCH64_STR, { SAimm(1), SAcop(3), SAcop(1), index[2].tag == Arg::IIMMEDIATE ? SAcopsar(2, 3) : SAcop(2) });
            };
        }
        break;
    case (OP_MOV):
        if(index[0].tag == Arg::VREG && index[1].tag == Arg::IIMMEDIATE)
        {
            uint64_t bitfield = static_cast<uint64_t>(index[1].value);
            int tarOpcode = AARCH64_MOVI;
            if(index[1].value < 0)
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
                else if(index[0].elemtype == TYPE_I64)
                    break;
            }
            int bitNeed = msb64(bitfield);
            if (bitNeed > 8)
                break;
            int64_t higher3 = (bitfield & 0b11100000) >> 5;
            int64_t lower5  = bitfield & 0b00011111;
            return SyT(tarOpcode, { SAcop(0), SAimm(lower5), SAimm(higher3) });
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
            if (bitNeed > 16)
                break;
            if(invert)
                return SyT(AARCH64_MOVN, { SAcop(0), SAimm(bitfield) });
            else
                return SyT(AARCH64_MOV, { SAcop(0), SAcop(1) });
        }
        else
            return SyT(AARCH64_MOV, { SAcop(0), SAcop(1) });
    case (OP_ARM_MOVK):
        if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IIMMEDIATE &&
           (index[2].value == 16 || index[2].value == 32 || index[2].value == 48))
            return SyT(AARCH64_MOVK, { SAcop(0), SAcop(1), SAcopsar(2, 4, AF_NOPRINT)});
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
            return SyT(AARCH64_LSL, { SAcop(0), SAcop(1), SAimm((64-shift) % 64 ), SAimm(63 - shift, AF_NOPRINT) });
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
            if (processLogicalImmediate(index[2].value, 64, N_immr_imms))
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
        if (index.size() == 4)
        {
            Assert(index[1].value >= 0 && index[1].value < IC_UNKNOWN);
            return SyT(AARCH64_CSEL, { SAcop(0), SAcop(2), SAcop(3), SAimm(ICbytecode2Aarch64(index[1].value)) });
        }
        break;
    case (OP_ARM_CINC): return SyT(AARCH64_CINC,{ SAcop(0), SAcop(1), SAimm(invertAarch64IC(ICbytecode2Aarch64(index[2].value))) });
    case (OP_ARM_CNEG): return SyT(AARCH64_CNEG,{ SAcop(0), SAcop(1), SAimm(invertAarch64IC(ICbytecode2Aarch64(index[2].value))) });
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
        break;

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
        break;
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
            int esize = elemSize(index[0].elemtype); 
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
                if(!encodeImmShift(index[2].value, index[0].elemtype, immh, immb))
                    break;
                return SyT(AARCH64_SHL, { SAcop(0), SAcop(1), SAimm(immh), SAimm(immb) });
            }
            else if(index[2].tag == Arg::VREG && isInteger(index[2].elemtype) && elemSize(index[2].elemtype) == elemSize(index[0].elemtype))
            {
                int tarOpcode = isUnsignedInteger(index[0].elemtype) ? AARCH64_USHL : AARCH64_SSHL;
                return SyT(tarOpcode, { SAcop(0), SAcop(1), SAcop(2) });
            }
        }
        break;
    case (VOP_SHL):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG &&
           index[0].elemtype == index[1].elemtype && isInteger(index[0].elemtype) && isInteger(index[2].elemtype) && elemSize(index[2].elemtype) == elemSize(index[0].elemtype))
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
//            else if(index[2].tag == Arg::VREG && isInteger(index[2].elemtype) && elemSize(index[2].elemtype) == elemSize(index[0].elemtype))
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
//            else if(index[2].tag == Arg::VREG && isInteger(index[2].elemtype) && elemSize(index[2].elemtype) == elemSize(index[0].elemtype))
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
    case (VOP_NE):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elemSize(index[0].elemtype) == elemSize(index[1].elemtype) && isUnsignedInteger(index[0].elemtype))
        {
            if(isFloat(index[1].elemtype))
            {
                int taropcode = index.opcode == VOP_GT ? AARCH64_FCMGT : (
                                index.opcode == VOP_GE ? AARCH64_FCMGE : (
                              /*index.opcode == VOP_NE ||
                                index.opcode == VOP_EQ?*/AARCH64_FCMEQ ));
                return SyT(taropcode, { SAcop(0), SAcop(1), SAcop(2) });
            }
        }
        break;
    case (VOP_LT):
    case (VOP_LE):
        if(index.size() == 3 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && index[2].tag == Arg::VREG && index[1].elemtype == index[2].elemtype && elemSize(index[0].elemtype) == elemSize(index[1].elemtype) && isUnsignedInteger(index[0].elemtype))
        {
            if(isFloat(index[1].elemtype))
            {
                int taropcode = index.opcode == VOP_LT ? AARCH64_FCMGT : (
                              /*index.opcode == VOP_LE?*/AARCH64_FCMGE );
                return SyT(taropcode, { SAcop(0), SAcop(2), SAcop(1) });
            }
        }
        break;
    case (VOP_TRUNC):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && isFloat(index[1].elemtype) && elemSize(index[0].elemtype) == elemSize(index[1].elemtype))
        {
            if(isSignedInteger(index[0].elemtype))
                return SyT(AARCH64_FCVTZS, { SAcop(0), SAcop(1) });
            else if(isUnsignedInteger(index[0].elemtype))
                return SyT(AARCH64_FCVTZU, { SAcop(0), SAcop(1) });
        }
        break;
    case (VOP_FLOOR):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && isFloat(index[1].elemtype) && elemSize(index[0].elemtype) == elemSize(index[1].elemtype))
        {
            if(isSignedInteger(index[0].elemtype))
                return SyT(AARCH64_FCVTMS, { SAcop(0), SAcop(1) });
            else if(isUnsignedInteger(index[0].elemtype))
                return SyT(AARCH64_FCVTMU, { SAcop(0), SAcop(1) });
        }
        break;
    case (VOP_CAST):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::VREG && isFloat(index[0].elemtype) && elemSize(index[0].elemtype) == elemSize(index[1].elemtype))
        {
            if(isSignedInteger(index[1].elemtype))
                return SyT(AARCH64_SCVTF, { SAcop(0), SAcop(1) });
            else if(isUnsignedInteger(index[1].elemtype))
                return SyT(AARCH64_UCVTF, { SAcop(0), SAcop(1) });
        }
        break;
    case (VOP_BROADCAST):
        if(index.size() == 2 && index[0].tag == Arg::VREG && index[1].tag == Arg::IREG)
            return SyT(AARCH64_DUP, { SAcop(0), SAcop(1) });
    case (OP_UNSPILL):
        if(index.size() == 2) 
        {
            if(index[0].tag == Arg::IREG) 
                return SyT(AARCH64_LDR, { SAimm(1, AF_NOPRINT), SAcop(0), SAreg(SP, AF_ADDRESS), SAcop(1) });
            else if(index[0].tag == Arg::VREG) 
                return SyT(AARCH64_LDR, { SAcop(0), SAreg(SP, AF_ADDRESS), SAcopsar(1,1) });
        }
    case (OP_SPILL):
        if(index.size() == 2) 
        {
            if(index[1].tag == Arg::IREG) 
                return SyT(AARCH64_STR, { SAimm(1, AF_NOPRINT), SAcop(1), SAreg(SP, AF_ADDRESS), SAcop(0) });
            else if(index[1].tag == Arg::VREG) 
                return SyT(AARCH64_STR, { SAcop(1), SAreg(SP, AF_ADDRESS), SAcopsar(0,1) });
        }
    case (OP_JMP_NE):   return SyT(AARCH64_B_NE,{ SAcopsar(0, 2, AF_PRINTOFFSET) });  //AArch64 supports only multiply-4 offsets,
    case (OP_JMP_EQ):   return SyT(AARCH64_B_EQ,{ SAcopsar(0, 2, AF_PRINTOFFSET) });  //so, for compactification, they are divided by 4.
    case (OP_JMP_LT):   return SyT(AARCH64_B_LT,{ SAcopsar(0, 2, AF_PRINTOFFSET) });
    case (OP_JMP_GT):   return SyT(AARCH64_B_GT,{ SAcopsar(0, 2, AF_PRINTOFFSET) });
    case (OP_JMP_LE):   return SyT(AARCH64_B_LE,{ SAcopsar(0, 2, AF_PRINTOFFSET) });
    case (OP_JMP_GE):   return SyT(AARCH64_B_GE,{ SAcopsar(0, 2, AF_PRINTOFFSET) });
    case (OP_JMP):      return SyT(AARCH64_B,   { SAcopsar(0, 2, AF_PRINTOFFSET) });
    case (OP_RET):      return SyT(AARCH64_RET, { SAreg(LR) });
    default:
        break;
    };
    scs = false;
    return SyntopTranslation();
}

class AArch64BigImmediates : public CompilerStage
{
public:
    AArch64BigImmediates(Backend* a_backend): m_backend(a_backend) {}
    virtual void process(Syntfunc& a_processed) const override;
    virtual ~AArch64BigImmediates() override {}
    static CompilerStagePtr make(Backend* a_backend)
    {
        return std::static_pointer_cast<CompilerStage>(std::make_shared<AArch64BigImmediates>(a_backend));
    }
private:
    Backend* m_backend;
};

class AArch64ARASnippets : public CompilerStage
{
public:
    virtual void process(Syntfunc& a_processed) const override;
    virtual ~AArch64ARASnippets() override {}
    static CompilerStagePtr make()
    {
        return std::static_pointer_cast<CompilerStage>(std::make_shared<AArch64ARASnippets>());
    }
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
    m_registersAmount = 7;
    m_name = "AArch64";
    m_beforeRegAllocStages.push_back(AArch64BigImmediates::make(this));
    m_afterRegAllocStages.push_back(AArch64ARASnippets::make());
    m_parameterRegisters[RB_INT] = { R0, R1, R2, R3, R4, R5, R6, R7 };
    m_returnRegisters[RB_INT] = { R0, R1, R2, R3, R4, R5, R6, R7 };
    m_callerSavedRegisters[RB_INT] = { XR, R9, R10, R11, R12, R13, R14, R15, IP0, IP1 };
    m_calleeSavedRegisters[RB_INT] = { PR, R19, R20, R21, R22, R23, R24, R25, R26, R27, R28 };

    m_parameterRegisters[RB_VEC] = { Q0, Q1, Q2, Q3, Q4, Q5, Q6, Q7 };
    m_returnRegisters[RB_VEC] = { Q0, Q1, Q2, Q3 };
    m_callerSavedRegisters[RB_VEC] = { Q8, Q9, Q10, Q11, Q12, Q13, Q14, Q15 };
    m_calleeSavedRegisters[RB_VEC] = { Q16, Q17, Q18, Q19, Q20, Q21, Q22, Q23, Q24, Q25, Q26, Q27, Q28, Q29, Q30, Q31 };
}

size_t Aarch64Backend::reusingPreferences(const Syntop& a_op, const std::set<size_t>& undefinedArgNums) const
{
    switch (a_op.opcode)
    {
    case (OP_MOV):
    case (VOP_FMA):
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

size_t Aarch64Backend::spillSpaceNeeded(const Syntop& a_op, int basketNum) const
{
    if(basketNum == RB_INT)
        switch (a_op.opcode)
        {
        case (OP_MOD):
            return 1;
            break;
        default:
            break;
        }
    else if(basketNum == RB_VEC)
        switch (a_op.opcode)
        {
        case (VOP_FMA):
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

std::set<size_t> Aarch64Backend::getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask) const
{
    //TODO(ch): This specialized version of function must disappear after introducing snippets.
    //They will give info about used registers, like now instructions answers.
    //Actually, it's easy to think, that we have to keep used registers info on level of SyntopTranslation. Hmm...

    uint64_t actualRegs;
    uint64_t inRegs;
    uint64_t outRegs;
    switch (a_op.opcode)
    {
        case (OP_MIN):
        case (OP_MAX):
        {
            Assert(a_op.size() == 3 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IREG && a_op[2].tag == Arg::IREG);
            if (basketNum == RB_INT && (~(BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) & flagmask) == 0)
            {
                if (BinTranslation::Token::T_INPUT & flagmask)
                    return std::set<size_t>({1,2});
                if (BinTranslation::Token::T_OUTPUT & flagmask)
                    return std::set<size_t>({0});
            }
            break;
        }
        case (OP_ABS):
        case (OP_SIGN):
        {
            Assert(a_op.size() == 2 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IREG);
            if (basketNum == RB_INT && (~(BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) & flagmask) == 0)
            {
                if (BinTranslation::Token::T_INPUT & flagmask)
                    return std::set<size_t>({1});
                if (BinTranslation::Token::T_OUTPUT & flagmask)
                    return std::set<size_t>({0});
            }
            break;
        }
        case (VOP_FMA):
        {
            Assert((a_op.size() == 4 && a_op[0].tag == Arg::VREG && a_op[1].tag == Arg::VREG && a_op[2].tag == Arg::VREG && a_op[3].tag == Arg::VREG) ||
                   (a_op.size() == 5 && a_op[0].tag == Arg::VREG && a_op[1].tag == Arg::VREG && a_op[2].tag == Arg::VREG && a_op[3].tag == Arg::VREG && a_op[4].tag == Arg::IIMMEDIATE));
            if(basketNum == RB_INT)
                return std::set<size_t>({});
            else if (basketNum == RB_VEC && (~(BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) & flagmask) == 0)
            {
                if (BinTranslation::Token::T_INPUT & flagmask)
                    return std::set<size_t>({1, 2, 3});
                else if (BinTranslation::Token::T_OUTPUT & flagmask)
                    return std::set<size_t>({0});
            }
            break;
        }
        default:
            break;
    };
    return Backend::getUsedRegistersIdxs(a_op, basketNum, flagmask);
}

bool Aarch64Backend::handleBytecodeOp(const Syntop& a_btop, Syntfunc& a_formingtarget) const
{
    switch (a_btop.opcode)
    {
        case (OP_JMP_NE):
        case (OP_JMP_EQ):
        case (OP_JMP_LT):
        case (OP_JMP_GT):
        case (OP_JMP_LE):
        case (OP_JMP_GE):
        case (OP_JMP):
        {
            Assert(a_btop.size() == 1 && a_btop.args[0].tag == Arg::IIMMEDIATE);
            m_labelRefMap[a_btop.args[0].value].emplace_back(a_formingtarget.program.size(), 0, getS2sCurrentOffset());
            Syntop toTransform(a_btop);
            toTransform[0].value = getS2sCurrentOffset();
            a_formingtarget.program.emplace_back(lookS2s(toTransform).apply(toTransform, this));
            return true;
        }
        case (OP_LABEL):
        {
            if (a_btop.size() != 1 || a_btop.args[0].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Wrong LABEL format.");
            if(m_labelMap.count(a_btop.args[0].value) != 0)
                throw std::runtime_error("Label redefinition");
            m_labelMap[a_btop.args[0].value] = getS2sCurrentOffset() >> 2;
            return true;
        }
        default:
            return false;
    };
}

Syntfunc Aarch64Backend::bytecode2Target(const Syntfunc& a_bcfunc) const
{
    m_retReg = Syntfunc::RETREG;
    m_labelMap.clear();//labels offsets from start.
    m_labelRefMap.clear(); // label referenes map is needed to calculate and put in relative offsets after
    Syntfunc result = Backend::bytecode2Target(a_bcfunc);
    for(auto label: m_labelRefMap)
    {
        if(m_labelMap.count(label.first) == 0)
            throw std::runtime_error("Reference to unknown label");
        const int64_t loff = static_cast<int64_t>(m_labelMap[label.first]);
        for(label_ref_info& lref : label.second)
        {
            if (lref.opnum >= result.program.size())
                throw std::runtime_error("Internal error: operation number is too big");
            if (lref.argnum >= result.program[lref.opnum].size())
                throw std::runtime_error("Internal error: operation don't have so much arguments");
            if (result.program[lref.opnum].args[lref.argnum].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Internal error: operation don't have so much arguments");
            int64_t& opoff = result.program[lref.opnum].args[lref.argnum].value;
            opoff = (loff - opoff);
        }
    }
    return result;
}

void Aarch64Backend::getStackParameterLayout(const Syntfunc& a_func, const std::vector<size_t> (&regParsOverride)[RB_AMOUNT], std::map<RegIdx, size_t> (&parLayout)[RB_AMOUNT]) const
{
    size_t regPassed[RB_AMOUNT];
    for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        regPassed[basketNum] = regParsOverride[basketNum].size() ? regParsOverride[basketNum].size() : m_parameterRegisters[basketNum].size();
    size_t currOffset = 0;
    size_t xBasket[RB_AMOUNT] = {1,1};
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

size_t Aarch64Backend::stackGrowthAlignment(size_t stackGrowth) const
{
    return stackGrowth ? stackGrowth + (stackGrowth % 2) : stackGrowth;        //TODO(ch): Align to 16 or 32 if SIMD's are used.
}

Arg Aarch64Backend::getSParg(Func* funcimpl) const
{
    return argReg(RB_INT, SP, funcimpl);
}

std::unordered_map<int, std::string> Aarch64Backend::getOpStrings() const
{
    static std::unordered_map<int, std::string> ret({
        {AARCH64_LDR,   "ldr"  },
        {AARCH64_LDRSW, "ldrsw"},
        {AARCH64_LDRH,  "ldrh" },
        {AARCH64_LDRSH, "ldrsh"},
        {AARCH64_LDRB,  "ldrb" },
        {AARCH64_LDRSB, "ldrsb"},
        {AARCH64_STR,   "str"  },
        {AARCH64_STRH,  "strh" },
        {AARCH64_STRB,  "strb" },
        {AARCH64_MOV,   "mov"  },
        {AARCH64_MOVN,  "movn" },
        {AARCH64_MOVK,  "movk" },
        {AARCH64_ADD,   "add"  },
        {AARCH64_SUB,   "sub"  },
        {AARCH64_MUL,   "mul"  },
        {AARCH64_SDIV,  "sdiv" },
        {AARCH64_LSL,   "lsl"  },
        {AARCH64_LSR,   "lsr"  },
        {AARCH64_ASR,   "asr"  },
        {AARCH64_AND,   "and"  },
        {AARCH64_ORR,   "orr"  },
        {AARCH64_EOR,   "eor"  },
        {AARCH64_NEG,   "neg"  },
        {AARCH64_MVN,   "mvn"  },
        {AARCH64_CMP,   "cmp"  },
        {AARCH64_CSEL,  "csel" },
        {AARCH64_CINC,  "cinc" },
        {AARCH64_CNEG,  "cneg" },
        {AARCH64_FADD,  "fadd" },
        {AARCH64_FSUB,  "fsub" },
        {AARCH64_FMUL,  "fmul" },
        {AARCH64_FDIV,  "fdiv" },
        {AARCH64_FNEG,  "fneg" },
        {AARCH64_FMLA,  "fmla" },
        {AARCH64_SHL,   "shl"  },
        {AARCH64_USHL,  "ushl" },
        {AARCH64_SSHL,  "sshl" },
        {AARCH64_USHR,  "ushr" },
        {AARCH64_SSHR,  "sshr" },
        {AARCH64_MOVI,  "movi" },
        {AARCH64_MVNI,  "mvni" },
        {AARCH64_FCMGT, "fcmgt"},
        {AARCH64_FCMGE, "fcmge"},
        {AARCH64_FCMEQ, "fcmeq"},
        {AARCH64_FMIN,  "fmin" },
        {AARCH64_FMAX,  "fmax" },
        {AARCH64_FCVTZS,"fcvtzs"},
        {AARCH64_FCVTZU,"fcvtzu"},
        {AARCH64_FCVTMS,"fcvtms"},
        {AARCH64_FCVTMU,"fcvtmu"},
        {AARCH64_SCVTF, "scvtf"},
        {AARCH64_UCVTF, "ucvtf"},
        {AARCH64_LD1,   "ld1"},
        {AARCH64_ST1,   "st1"},
        {AARCH64_EXT,   "ext"},
        {AARCH64_DUP,   "dup"},
        {AARCH64_B,     "b"    },
        {AARCH64_B_NE,  "b.ne" },
        {AARCH64_B_EQ,  "b.eq" },
        {AARCH64_B_LT,  "b.lt" },
        {AARCH64_B_GT,  "b.gt" },
        {AARCH64_B_GE,  "b.ge" },
        {AARCH64_B_LE,  "b.le" },
        {AARCH64_RET,   "ret"  }});
    return ret;
}

Printer::ColPrinter Aarch64Backend::colHexPrinter(const Syntfunc& toP) const
{
    FuncBodyBuf buffer = target2Hex(toP);
    return [buffer](::std::ostream& out, const Syntop& toPrint, size_t rowNum, Backend* )
    {
        uint8_t* hexfield = &((*buffer)[0]) + sizeof(uint32_t)*rowNum;
        for(size_t pos = 0; pos < 4; pos++) //TODO(ch): Print variants (direct or reverse order).
            out << std::hex << std::setfill('0') << std::setw(2) << (uint32_t)*(hexfield+pos)<<" ";
    };
}

Printer::ArgPrinter Aarch64Backend::argPrinter(const Syntfunc& toP) const
{
    return [](::std::ostream& out, const Syntop& toPrint, size_t rowNum, size_t argNum)
    {
        Arg arg = toPrint[argNum];
        if (arg.flags & AF_PRINTOFFSET)
        {
            if (arg.tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Printer: register offsets are not supported.");
            int64_t targetline = rowNum + arg.value;
            out << "["<< targetline<< "]";
            return;
        }
        bool w32 = arg.flags & AF_LOWER32;
        bool address = arg.flags & AF_ADDRESS;
        if (address)
            out<<"[";
        switch (arg.tag)
        {
            case Arg::IREG:
                if(arg.idx == Syntfunc::RETREG)
                    out << "xR";
                else if(arg.idx == 31)
                    out << "sp";
                else
                    out << (w32 ? "w" : "x") << arg.idx;
                break;
            case Arg::VREG:
            {
                static std::string Vsuffixes[9] = {"", "16b", "8h", "", "4s", "", "", "", "2d" };
                out << "v" << arg.idx << "." << Vsuffixes[elemSize(arg.elemtype)];
                break;
            }
            case Arg::IIMMEDIATE:
                if(arg.value == 0)
                    out << "#0";
                else
                    out << "#0x"<< std::right <<std::hex << std::setfill('0') << std::setw(2)<<arg.value; break;
            default:
                throw std::runtime_error("Undefined argument type.");
        };
        if (address)
            out<<"]";
    };
}

void Aarch64Backend::switchOnSpillStressMode()
{
    m_parameterRegisters[RB_INT] = { R0, R1, R2, R3 };
    m_returnRegisters[RB_INT] = { R0, R1, R2, R3 };
    m_callerSavedRegisters[RB_INT] = {};
    m_calleeSavedRegisters[RB_INT] = { PR, R19, R20, R21, R22 };

    m_parameterRegisters[RB_VEC] = { Q0, Q1, Q2, Q3 };
    m_returnRegisters[RB_VEC] = { Q0, Q1, Q2, Q3 };
    m_callerSavedRegisters[RB_VEC] = { };
    m_calleeSavedRegisters[RB_VEC] = { Q29, Q30, Q31 };
}

void AArch64BigImmediates::process(Syntfunc& a_processed) const
{
    std::vector<Syntop> newProg;
    newProg.reserve(2 * a_processed.program.size());
    for (Syntop& op : a_processed.program)
        switch (op.opcode)
        {
        case OP_MOV:
        {
            Assert(op.size() == 2);
            if(op[1].tag != Arg::IIMMEDIATE)
            {
                newProg.push_back(op);
                break;
            }
//            if(op[1].value == 0)   //Well, it looks like we cannot do it, since xor x0, x0, x0, when is just defined will "use" undefined register. Hmm...
//            {                      //Looks like we need ARA stage or some Liveness Analysis modification. If and when will introduce
//                int tarOpcode = op[0].tag == Arg::VREG ? VOP_XOR : OP_XOR;      // definition-without-assignment, it also will be good solution.
//                newProg.push_back(Syntop(tarOpcode, { op[0], op[0], op[0] }));
//                break;
//            }
            if(m_backend->isImmediateFit(op, 1))
            {
                newProg.push_back(op);
                break;
            }
            size_t wordAmount = 4; //8byte
            Arg idest = op[0];
            if(idest.tag == Arg::VREG)
            {
                wordAmount = elemSize(op[0].elemtype)/2;
                idest = argReg(RB_INT, a_processed.provideIdx(RB_INT), idest.func);
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
            newProg.push_back(Syntop(OP_MOV, { idest, argIImm(words[0], op[0].func) }));
            for(size_t wNum = 1; wNum < wordAmount; wNum++)
                if((negative && words[wNum]!=-1) || (!negative && words[wNum]!=0))
                    newProg.push_back(Syntop(OP_ARM_MOVK, { idest, argIImm(words[wNum], op[0].func), argIImm(wNum*16, op[0].func) }));
            if(op[0].tag == Arg::VREG)
                newProg.push_back(Syntop(VOP_BROADCAST, { op[0], idest}));
            break;
        }
        default:
            newProg.push_back(op);
            break;
        }
    a_processed.program = newProg;
}

void AArch64ARASnippets::process(Syntfunc& a_processed) const
{
    std::vector<Syntop> newProg;
    newProg.reserve(2 * a_processed.program.size());
    for (Syntop& op : a_processed.program)
        switch (op.opcode)
        {
        case OP_MIN:
        case OP_MAX:
            Assert(op.size() == 3 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG && op[2].tag == Arg::IREG);
            newProg.push_back(Syntop(OP_CMP, { op[1], op[2] }));
            newProg.push_back(Syntop(OP_SELECT, { op[0], op.opcode == OP_MIN ? IC_LT : IC_GT, op[1], op[2] }));
            break;
        case OP_MOD:
            Assert(op.size() == 3 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG && op[2].tag == Arg::IREG);
            if(op[1].idx == op[2].idx)
            {
                newProg.push_back(Syntop(OP_DIV, { op[0], op[1], op[1] }));
                newProg.push_back(Syntop(OP_MOV, { op[0], argIImm(0) }));
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
                    newProg.push_back(Syntop(OP_SPILL, { 0, divisor }));
                    newProg.push_back(Syntop(OP_MOV, { divisor, op[2] }));
                }
                if(op[0].idx == op[1].idx)
                {
                    placeholder = lsb64(~makeBitmask64({(size_t)(op[0].idx), size_t(op[2].idx)}));
                    divided = argReg(RB_INT, placeholder);
                    newProg.push_back(Syntop(OP_SPILL, { 0, divided }));
                    newProg.push_back(Syntop(OP_MOV, { divided, op[1] }));
                }
                newProg.push_back(Syntop(OP_DIV, { op[0], divided, divisor }));
                newProg.push_back(Syntop(OP_MUL, { op[0], op[0], divisor }));
                newProg.push_back(Syntop(OP_SUB, { op[0], divided, op[0] }));
                if(placeholder != IReg::NOIDX)
                    newProg.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT, placeholder), 0 }));
            }
            break;
        case OP_SIGN:
            Assert(op.size() == 2 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG);
            newProg.push_back(Syntop(OP_CMP, { op[1], argIImm(0) }));
            newProg.push_back(Syntop(OP_SAR, { op[0], op[1], argIImm(63) }));
            newProg.push_back(Syntop(OP_ARM_CINC,{ op[0], op[0], argIImm(IC_GT) }));
            break;
        case OP_ABS:
            Assert(op.size() == 2 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG);
            newProg.push_back(Syntop(OP_CMP, { op[1], argIImm(0) }));
            newProg.push_back(Syntop(OP_ARM_CNEG,{ op[0], op[1], argIImm(IC_LT) }));
            break;
        case VOP_NE:
            Assert(op.size() == 3 && op[0].tag == Arg::VREG && op[1].tag == Arg::VREG && op[2].tag == Arg::VREG && op[1].elemtype == op[2].elemtype && elemSize(op[0].elemtype) == elemSize(op[1].elemtype) && isUnsignedInteger(op[0].elemtype));
            newProg.push_back(Syntop(VOP_EQ , { op[0], op[1], op[2] }));
            newProg.push_back(Syntop(VOP_NOT, { op[0], op[0] }));
            break;
        case VOP_FMA:
        {
            Assert((op.size() == 4 && op[0].tag == Arg::VREG && op[1].tag == Arg::VREG && op[2].tag == Arg::VREG && op[3].tag == Arg::VREG) ||
                   (op.size() == 5 && op[0].tag == Arg::VREG && op[1].tag == Arg::VREG && op[2].tag == Arg::VREG && op[3].tag == Arg::VREG && op[4].tag == Arg::IIMMEDIATE));
            bool laneVersion = (op.size() == 5);
            if(op[0].idx == op[1].idx)
            {
                newProg.push_back(op);
                break;
            }
            bool unspill = false;
            bool unspillFp16 = false;
            Arg placeholder = op[2];
            Arg placeholderfp16;
            int placeholderSPoff = 0;
            if(laneVersion && op[0].elemtype == TYPE_FP16 && op[3].idx > 15)
            {
                placeholderfp16 = op[3];
                placeholderfp16.idx = lsb64(~makeBitmask64({(size_t)(op[0].idx), size_t(op[1].idx), (size_t)(op[2].idx)}));
                newProg.push_back(Syntop(OP_SPILL, { 0, placeholderfp16 })); 
                newProg.push_back(Syntop(OP_MOV , { placeholderfp16, op[3] }));
                op[3] = placeholderfp16;
                const int vectorSize = 16; //TODO(ch): replace with Backend::vectorSize call. 
                placeholderSPoff = vectorSize / 8; 
            }
            if(op[0].idx == op[2].idx || op[0].idx == op[3].idx)
            {
                placeholder.idx = lsb64(~makeBitmask64({(size_t)(op[0].idx), size_t(op[1].idx), (size_t)(op[2].idx), (size_t)(op[3].idx)}));
                newProg.push_back(Syntop(OP_SPILL, { placeholderSPoff, placeholder }));
                newProg.push_back(Syntop(OP_MOV , { placeholder, op[0] }));
                unspill = true;
                if(op[0].idx == op[2].idx)
                    op[2] = placeholder;
                if(op[0].idx == op[3].idx)
                    op[3] = placeholder;
            }
            if(op[0].idx != op[1].idx)
                newProg.push_back(Syntop(OP_MOV , { op[0], op[1] }));
            if(laneVersion)
                newProg.push_back(Syntop(VOP_FMA, { op[0], op[0], op[2], op[3], op[4] }));
            else
                newProg.push_back(Syntop(VOP_FMA, { op[0], op[0], op[2], op[3] }));
            if(unspillFp16)
                newProg.push_back(Syntop(OP_UNSPILL, { placeholderfp16, 0 }));
            if(unspill)
                newProg.push_back(Syntop(OP_UNSPILL, { placeholder, placeholderSPoff }));
            break;
        }
        default:
            newProg.push_back(op);
            break;
        }
    a_processed.program = newProg;
}

};
#endif //__LOOPS_ARCH == __LOOPS_AARCH64
