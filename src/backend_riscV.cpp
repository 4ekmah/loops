/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "backend_riscV.hpp"
#if __LOOPS_ARCH == __LOOPS_RISCV
#include "func_impl.hpp"
#include "collections.hpp"
#include <algorithm>
#include <iomanip>

//DUBUG: Release build for some reason cannot be done now.

LOOPS_HASHMAP_STATIC(int, loops_cstring) opstrings_[] = 
{
                  /*  |       enum_id       |string_id|    */
    LOOPS_HASHMAP_ELEM(loops::RISCV_LB   , "lb"   ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_LBU  , "lbu"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_LH   , "lh"   ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_LHU  , "lhu"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_LW   , "lw"   ), 
    LOOPS_HASHMAP_ELEM(loops::RISCV_LWU  , "lwu"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_LD   , "ld"   ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SB   , "sb"   ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SH   , "sh"   ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SW   , "sw"   ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SD   , "sd"   ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_MV   , "mv"   ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_LUI  , "lui"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_ADD  , "add"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SUB  , "sub"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_ADDI , "addi" ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_MUL  , "mul"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_DIV  , "div"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_REM  , "rem"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_NEG  , "neg"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SLL  , "sll"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SLLI , "slli" ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SRL  , "srl"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SRLI , "srli" ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SRA  , "sra"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SRAI , "srai" ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_XOR  , "xor"  ), 
    LOOPS_HASHMAP_ELEM(loops::RISCV_XORI , "xori" ), 
    LOOPS_HASHMAP_ELEM(loops::RISCV_OR   , "or"   ), 
    LOOPS_HASHMAP_ELEM(loops::RISCV_ORI  , "ori"  ), 
    LOOPS_HASHMAP_ELEM(loops::RISCV_AND  , "and"  ), 
    LOOPS_HASHMAP_ELEM(loops::RISCV_ANDI , "andi" ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_NOT  , "not"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SLT  , "slt"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SLTU , "sltu" ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SEQZ , "seqz" ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_SNEZ , "snez" ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_BEQ  , "beq"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_BNE  , "bne"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_BLT  , "blt"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_BGE  , "bge"  ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_BLTU , "bltu" ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_BGEU , "bgeu" ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_J    , "j"    ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_JALR , "jalr" ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_LABEL, ""     ),
    LOOPS_HASHMAP_ELEM(loops::RISCV_RET  , "ret"  ),
};

static LOOPS_HASHMAP(int, loops_cstring) opstrings = NULL;

int backend_riscv_h_initialize()
{
    LOOPS_CALL_THROW(loops_hashmap_construct_static(&opstrings, opstrings_, sizeof(opstrings_) / sizeof(opstrings_[0])));
    return LOOPS_ERR_SUCCESS;
}

void backend_riscv_h_deinitialize()
{
    loops_hashmap_destruct(opstrings);
}

namespace loops
{
    enum RiscVReg
    {
        ZERO =  0,
        RA   =  1,
        SP   =  2,
        GP   =  3,
        TP   =  4,
        LR   =  5,
        T1   =  6,
        T2   =  7,
        FP   =  8,
        S1   =  9,
        A0   = 10,
        A1   = 11,
        A2   = 12,
        A3   = 13,
        A4   = 14,
        A5   = 15,
        A6   = 16,
        A7   = 17,
        S2   = 18,
        S3   = 19,
        S4   = 20,
        S5   = 21,
        S6   = 22,
        S7   = 23,
        S8   = 24,
        S9   = 25,
        S10  = 26,
        S11  = 27,
        T3   = 28,
        T4   = 29,
        T5   = 30,
        T6   = 31,
    }; 

//DUBUG: There is a lot of repeatitive code in risc-v formats handlers. Do something.

    enum {ARG0_FIXED = 1, ARG1_FIXED = 2, ARG2_FIXED = 4 };
    //R-type:
    //|immediate   |register|register|static|immediate  |static|
    //|imm[12|10:5]|rs2     |rs1     |funct3|imm[4:1|11]|opcode|
    //|7 bits      |5 bits  |5 bits  |3 bits|5 bits     |7 bit |
    static inline BinTranslation btype(const Syntop& index, bool& scs, uint64_t funct3, uint64_t opcode, uint64_t flags0, uint64_t flags1, uint64_t flags2, uint64_t fixed, uint64_t fixed0, uint64_t fixed1, uint64_t fixed2)
    {
        using namespace BinTranslationConstruction;
        Assert(((funct3 & ~(uint64_t(0b111))) == 0) && ((opcode & ~(uint64_t(0b1111111))) == 0));
        scs = true;
        int effargidxs[3] = {0,1,2};
        int effargnum = 3;
        if(fixed & ARG0_FIXED)
        {
            Assert((fixed0 >= ZERO && fixed0 <= T6));
            effargidxs[0] = -1;
            effargidxs[1]--;
            effargidxs[2]--;
            effargnum--;
        }
        if(fixed & ARG1_FIXED)
        {
            Assert((fixed1 >= ZERO && fixed1 <= T6));
            effargidxs[1] = -1;
            effargidxs[2]--;
            effargnum--;
        }
        if(fixed & ARG2_FIXED)
        {
            effargidxs[2] = -1;
            effargnum--;
        }

        if(index.size() == effargnum && (effargidxs[0] == -1 || index.args[effargidxs[0]].tag == Arg::IREG) && (effargidxs[1] == -1 || index.args[effargidxs[1]].tag == Arg::IREG) && (effargidxs[2] == -1 || index.args[effargidxs[2]].tag == Arg::IIMMEDIATE))
        {
            std::vector<BinTranslation::Token> tokens;
            tokens.reserve(7);
            uint64_t value2 = (fixed & ARG2_FIXED) ? fixed2 : (uint64_t)index.args[effargidxs[2]].value;
            if((value2 & 0b1) == 0 && signed_fits(value2, 13))
            {
                uint64_t low5;
                uint64_t high7;
                {
                    uint64_t sign_bit = (value2 >> 63) << 6; 
                    low5 = value2 & 0x1F;
                    high7 = value2 >> 5;
                    uint64_t highest_bit = (high7 >> 6) & 0b1;
                    low5 = (low5 & 0b11110) | highest_bit;
                    high7 = (high7 & 0b0111111) | sign_bit; ////imm[12|10:5] rs2 rs1 000 imm[4:1|11] 1100011 BEQ
                }
                if((fixed & ARG2_FIXED) == 0) 
                    tokens.push_back(BTomm(effargidxs[2], flags2));
                tokens.push_back(BTsta(high7, 7)); 
                if(fixed & ARG1_FIXED)
                    tokens.push_back(BTsta(fixed1, 5));
                else
                    tokens.push_back(BTreg(effargidxs[1], 5, flags1));
                if(fixed & ARG0_FIXED)
                    tokens.push_back(BTsta(fixed0, 5));
                else
                    tokens.push_back(BTreg(effargidxs[0], 5, flags0));
                tokens.push_back(BTsta(funct3, 3)); 
                tokens.push_back(BTsta(low5, 5));
                tokens.push_back(BTsta(opcode, 7));
                return BinTranslation(tokens);
            }
        }
        scs = false;
        return BinTranslation();
    }

    //J-type:
    //|immediate            |register|static|
    //|imm[20|10:1|11|19:12]|rd      |opcode|
    //|20 bits              |5 bits  |7 bit |
    static inline BinTranslation jtype(const Syntop& index, bool& scs, uint64_t opcode, uint64_t flags0, uint64_t flags1, uint64_t fixed, uint64_t fixed0, uint64_t fixed1)
    {
        using namespace BinTranslationConstruction;
        Assert((opcode & ~(uint64_t(0b1111111))) == 0);
        scs = true;
        int effargidxs[2] = {0,1};
        int effargnum = 2;
        if(fixed & ARG0_FIXED)
        {
            Assert((fixed0 >= ZERO && fixed0 <= T6));
            effargidxs[0] = -1;
            effargidxs[1]--;
            effargnum--;
        }
        if(fixed & ARG1_FIXED)
        {
            effargidxs[1] = -1;
            effargnum--;
        }
        if(index.size() == effargnum && (effargidxs[0] == -1 || index.args[effargidxs[0]].tag == Arg::IREG) && (effargidxs[1] == -1 || index.args[effargidxs[1]].tag == Arg::IIMMEDIATE))
        {
            std::vector<BinTranslation::Token> tokens;
            tokens.reserve(4);
            uint64_t value1 = (fixed & ARG1_FIXED) ? fixed1 : (uint64_t)index.args[effargidxs[1]].value;
            if((value1 & 0b1) == 0 && signed_fits(value1, 21))
            {
                {
                    //imm[20|10:1|11|19:12]
                    uint64_t imm = value1 & 0b11111111111111111111;
                    uint64_t sign_bit = (value1 >> 63) << 19; 
                    imm = sign_bit                                | 
                            ((imm & 0b000000000011111111110) << 8 ) | 
                            ((imm & 0b000000000100000000000) >> 3 ) | 
                            ((imm & 0b011111111000000000000) >> 12);
                    if((fixed & ARG1_FIXED) == 0)
                        tokens.push_back(BTomm(effargidxs[1], flags1));
                    tokens.push_back(BTsta(imm, 20));
                }
                if(fixed & ARG0_FIXED)
                    tokens.push_back(BTsta(fixed0, 5));
                else
                    tokens.push_back(BTreg(effargidxs[0], 5, flags0));
                tokens.push_back(BTsta(opcode, 7));
                return BinTranslation(tokens);
            }
        }
        scs = false;
        return BinTranslation();
    }    

    //U-type:
    //|immediate |register|static|
    //|imm[31:12]|rd      |opcode|
    //|20 bits   |5 bits  |7 bit |
    static inline BinTranslation utype(const Syntop& index, bool& scs, uint64_t opcode, uint64_t flags0, uint64_t flags1, uint64_t fixed, uint64_t fixed0, uint64_t fixed1)
    {
        using namespace BinTranslationConstruction;
        Assert((opcode & ~(uint64_t(0b1111111))) == 0);
        scs = true;
        int effargidxs[2] = {0,1};
        int effargnum = 2;
        if(fixed & ARG0_FIXED)
        {
            Assert((fixed0 >= ZERO && fixed0 <= T6));
            effargidxs[0] = -1;
            effargidxs[1]--;
            effargnum--;
        }
        if(fixed & ARG1_FIXED)
        {
            effargidxs[1] = -1;
            effargnum--;
        }
        if(index.size() == effargnum && (effargidxs[0] == -1 || index.args[effargidxs[0]].tag == Arg::IREG) && (effargidxs[1] == -1 || index.args[effargidxs[1]].tag == Arg::IIMMEDIATE))
        {
            std::vector<BinTranslation::Token> tokens;
            tokens.reserve(4);
            uint64_t value1 = (fixed & ARG1_FIXED) ? fixed1 : (uint64_t)index.args[effargidxs[1]].value;
            if((value1 & ~(uint64_t(0b11111111111111111111))) == 0)
            {
                {
                    //imm[20|10:1|11|19:12]
                    uint64_t imm = value1 & 0b11111111111111111111;
                    if((fixed & ARG1_FIXED) == 0)
                        tokens.push_back(BTomm(effargidxs[1], flags1));
                    tokens.push_back(BTsta(imm, 20));
                }
                if(fixed & ARG0_FIXED)
                    tokens.push_back(BTsta(fixed0, 5));
                else
                    tokens.push_back(BTreg(effargidxs[0], 5, flags0));
                tokens.push_back(BTsta(opcode, 7));
                return BinTranslation(tokens);
            }
        }
        scs = false;
        return BinTranslation();
    }    

    //R-type:
    //|static|register|register|static|register|static|
    //|funct7|rs2     |rs1     |funct3|rd      |opcode|
    //|7 bits|5 bits  |5 bits  |3 bits|5 bits  |7 bit |
    static inline BinTranslation rtype(const Syntop& index, bool& scs, uint64_t funct7, uint64_t funct3, uint64_t opcode, uint64_t flags0, uint64_t flags1, uint64_t flags2, uint64_t fixed, uint64_t fixed0, uint64_t fixed1, uint64_t fixed2)
    {
        using namespace BinTranslationConstruction;
        Assert(((funct7 & ~(uint64_t(0b1111111))) == 0) && ((funct3 & ~(uint64_t(0b111))) == 0) && ((opcode & ~(uint64_t(0b1111111))) == 0));
        scs = true;
        int effargidxs[3] = {0,1,2};
        int effargnum = 3;
        if(fixed & ARG0_FIXED)
        {
            Assert((fixed0 >= ZERO && fixed0 <= T6));
            effargidxs[0] = -1;
            effargidxs[1]--;
            effargidxs[2]--;
            effargnum--;
        }
        if(fixed & ARG1_FIXED)
        {
            Assert((fixed1 >= ZERO && fixed1 <= T6));
            effargidxs[1] = -1;
            effargidxs[2]--;
            effargnum--;
        }
        if(fixed & ARG2_FIXED)
        {
            Assert((fixed2 >= ZERO && fixed2 <= T6));
            effargidxs[2] = -1;
            effargnum--;
        }
        if(index.size() == effargnum && (effargidxs[0] == -1 || index.args[effargidxs[0]].tag == Arg::IREG) && (effargidxs[1] == -1 || index.args[effargidxs[1]].tag == Arg::IREG) && (effargidxs[2] == -1 || index.args[effargidxs[2]].tag == Arg::IREG))
        {
            std::vector<BinTranslation::Token> tokens;
            tokens.reserve(6);
            tokens.push_back(BTsta(funct7, 7)); 
            if(fixed & ARG2_FIXED)
                tokens.push_back(BTsta(fixed2, 5));
            else
                tokens.push_back(BTreg(effargidxs[2], 5, flags2));
            if(fixed & ARG1_FIXED)
                tokens.push_back(BTsta(fixed1, 5));
            else
                tokens.push_back(BTreg(effargidxs[1], 5, flags1));
            tokens.push_back(BTsta(funct3, 3)); 
            if(fixed & ARG0_FIXED)
                tokens.push_back(BTsta(fixed0, 5));
            else
                tokens.push_back(BTreg(effargidxs[0], 5, flags0));
            tokens.push_back(BTsta(opcode, 7));
            return BinTranslation(tokens);
        }
        scs = false;
        return BinTranslation();
    }

    //I-type:
    //|immediate|register|static|register|static|
    //|imm[11:0]|rs1     |funct3|rd      |opcode|
    //|12 bits  |5 bits  |3 bits|5 bits  |7 bit |
    static inline BinTranslation itype(const Syntop& index, bool& scs, uint64_t funct3, uint64_t opcode, uint64_t flags0, uint64_t flags1, uint64_t flags2, uint64_t fixed, uint64_t fixed0, uint64_t fixed1, uint64_t fixed2)
    {
        using namespace BinTranslationConstruction;
        Assert(((funct3 & ~(uint64_t(0b111))) == 0) && ((opcode & ~(uint64_t(0b1111111))) == 0));
        scs = true;
        int effargidxs[3] = {0,1,2};
        int effargnum = 3;
        if(fixed & ARG0_FIXED)
        {
            Assert((fixed0 >= ZERO && fixed0 <= T6));
            effargidxs[0] = -1;
            effargidxs[1]--;
            effargidxs[2]--;
            effargnum--;
        }
        if(fixed & ARG1_FIXED)
        {
            Assert((fixed1 >= ZERO && fixed1 <= T6));
            effargidxs[1] = -1;
            effargidxs[2]--;
            effargnum--;
        }
        if(fixed & ARG2_FIXED)
        {
            Assert(signed_fits(fixed2, 12));
            effargidxs[2] = -1;
            effargnum--;
        }
        if(index.size() == effargnum && (effargidxs[0] == -1 || index.args[effargidxs[0]].tag == Arg::IREG) && (effargidxs[1] == -1 || index.args[effargidxs[1]].tag == Arg::IREG) && (effargidxs[2] == -1 || index.args[effargidxs[2]].tag == Arg::IIMMEDIATE))
        {
            std::vector<BinTranslation::Token> tokens;
            tokens.reserve(5);
            if((fixed & ARG2_FIXED) || signed_fits((uint64_t)index.args[effargidxs[2]].value, 12))
            {
                if(fixed & ARG2_FIXED)
                    tokens.push_back(BTsta(fixed2, 12));
                else
                    tokens.push_back(BTimm(effargidxs[2], 12, flags2));
                if(fixed & ARG1_FIXED)
                    tokens.push_back(BTsta(fixed1, 5));
                else
                    tokens.push_back(BTreg(effargidxs[1], 5, flags1));
                tokens.push_back(BTsta(funct3, 3)); 
                if(fixed & ARG0_FIXED)
                    tokens.push_back(BTsta(fixed0, 5));
                else
                    tokens.push_back(BTreg(effargidxs[0], 5, flags0));
                tokens.push_back(BTsta(opcode, 7));
                return BinTranslation(tokens);

            }
        }
        scs = false;
        return BinTranslation();
    }

    //I-type(shift specialization):
    //|static|immediate|register|static|register|static|
    //|funct6|imm[5:0] |rs1     |funct3|rd      |opcode|
    //|6 bits|6 bits   |5 bits  |3 bits|5 bits  |7 bit |
    static inline BinTranslation istype(const Syntop& index, bool& scs, uint64_t funct6, uint64_t funct3, uint64_t opcode, uint64_t flags0, uint64_t flags1, uint64_t flags2, uint64_t fixed, uint64_t fixed0, uint64_t fixed1, uint64_t fixed2)
    {
        using namespace BinTranslationConstruction;
        Assert(((funct6 & ~(uint64_t(0b111111))) == 0) && ((funct3 & ~(uint64_t(0b111))) == 0) && ((opcode & ~(uint64_t(0b1111111))) == 0));
        scs = true;
        int effargidxs[3] = {0,1,2};
        int effargnum = 3;
        if(fixed & ARG0_FIXED)
        {
            Assert((fixed0 >= ZERO && fixed0 <= T6));
            effargidxs[0] = -1;
            effargidxs[1]--;
            effargidxs[2]--;
            effargnum--;
        }
        if(fixed & ARG1_FIXED)
        {
            Assert((fixed1 >= ZERO && fixed1 <= T6));
            effargidxs[1] = -1;
            effargidxs[2]--;
            effargnum--;
        }
        if(fixed & ARG2_FIXED)
        {
            Assert(((fixed2 & ~(uint64_t(0b111111))) == 0));
            effargidxs[2] = -1;
            effargnum--;
        }
        if(index.size() == effargnum && (effargidxs[0] == -1 || index.args[effargidxs[0]].tag == Arg::IREG) && (effargidxs[1] == -1 || index.args[effargidxs[1]].tag == Arg::IREG) && (effargidxs[2] == -1 || index.args[effargidxs[2]].tag == Arg::IIMMEDIATE))
        {
            std::vector<BinTranslation::Token> tokens;
            tokens.reserve(6);
            if((fixed & ARG2_FIXED) || ((((uint64_t)index.args[effargidxs[2]].value) & ~(uint64_t(0b111111))) == 0))
            {
                tokens.push_back(BTsta(funct6, 6)); 
                if(fixed & ARG2_FIXED)
                    tokens.push_back(BTsta(fixed2, 6));
                else
                    tokens.push_back(BTimm(effargidxs[2], 6, flags2));
                if(fixed & ARG1_FIXED)
                    tokens.push_back(BTsta(fixed1, 5));
                else
                    tokens.push_back(BTreg(effargidxs[1], 5, flags1));
                tokens.push_back(BTsta(funct3, 3)); 
                if(fixed & ARG0_FIXED)
                    tokens.push_back(BTsta(fixed0, 5));
                else
                    tokens.push_back(BTreg(effargidxs[0], 5, flags0));
                tokens.push_back(BTsta(opcode, 7));
            }
            return BinTranslation(tokens);
        }
        scs = false;
        return BinTranslation();
    }

    //S-type:
    //|immediate|register|register|static|immediate|static|
    //|imm[11:5]|rs2     |rs1     |funct3|imm[4:0] |opcode|
    //|7 bits   |5 bits  |5 bits  |3 bits|5 bits   |7 bit |
    static inline BinTranslation stype(const Syntop& index, bool& scs, uint64_t funct3, uint64_t opcode, uint64_t flags0, uint64_t flags1, uint64_t flags2, uint64_t fixed, uint64_t fixed0, uint64_t fixed1, uint64_t fixed2)
    {
        using namespace BinTranslationConstruction;
        Assert(((funct3 & ~(uint64_t(0b111))) == 0) && ((opcode & ~(uint64_t(0b1111111))) == 0));
        scs = true;
        int effargidxs[3] = {0,1,2};
        int effargnum = 3;
        if(fixed & ARG0_FIXED)
        {
            Assert((fixed0 >= ZERO && fixed0 <= T6));
            effargidxs[0] = -1;
            effargidxs[1]--;
            effargidxs[2]--;
            effargnum--;
        }
        if(fixed & ARG1_FIXED)
        {
            Assert(signed_fits(fixed1, 12));
            effargidxs[1] = -1;
            effargidxs[2]--;
            effargnum--;
        }
        if(fixed & ARG2_FIXED)
        {
            Assert((fixed2 >= ZERO && fixed2 <= T6));
            effargidxs[2] = -1;
            effargnum--;
        }
        if(index.size() == effargnum && (effargidxs[0] == -1 || index.args[effargidxs[0]].tag == Arg::IREG) && (effargidxs[1] == -1 || index.args[effargidxs[1]].tag == Arg::IIMMEDIATE) && (effargidxs[2] == -1 || index.args[effargidxs[2]].tag == Arg::IREG))
        {
            std::vector<BinTranslation::Token> tokens;
            tokens.reserve(7);
            if((fixed & ARG1_FIXED) || signed_fits((uint64_t)index.args[effargidxs[1]].value, 12))
            {
                uint64_t low5 = index[1].value & 0x1F;
                uint64_t high7 = index[1].value >> 5;
                if((fixed & ARG1_FIXED) == 0) 
                    tokens.push_back(BTomm(effargidxs[1], flags1));
                tokens.push_back(BTsta(high7, 7));
                if(fixed & ARG0_FIXED)
                    tokens.push_back(BTsta(fixed0, 5));
                else
                    tokens.push_back(BTreg(effargidxs[0], 5, flags0));
                if(fixed & ARG2_FIXED)
                    tokens.push_back(BTsta(fixed2, 5));
                else
                    tokens.push_back(BTreg(effargidxs[2], 5, flags2));
                tokens.push_back(BTsta(funct3, 3)); 
                tokens.push_back(BTsta(low5, 5));
                tokens.push_back(BTsta(opcode, 7));
                return BinTranslation(tokens);
            }
        }
        scs = false;
        return BinTranslation();
    }

    BinTranslation i64BTLookup(const Syntop& index, bool& scs)
    {
        using namespace BinTranslationConstruction;
        scs = true;
        switch (index.opcode)
        {//                                                                     | flags0| flags1|    flags2|               fixed                 |fixed0|fixed1|fixed2|
        case (RISCV_SB):   return  stype(index, scs, 0b000, 0b0100011,                In, Addr64, Addr64|In,                                    0,     0,     0,     0);
        case (RISCV_SH):   return  stype(index, scs, 0b001, 0b0100011,                In, Addr64, Addr64|In,                                    0,     0,     0,     0);
        case (RISCV_SW):   return  stype(index, scs, 0b010, 0b0100011,                In, Addr64, Addr64|In,                                    0,     0,     0,     0);
        case (RISCV_SD):   return  stype(index, scs, 0b011, 0b0100011,                In, Addr64, Addr64|In,                                    0,     0,     0,     0);
        case (RISCV_MV):   return  itype(index, scs, 0b000, 0b0010011,               Out,     In,         0,                           ARG2_FIXED,     0,     0,     0); //ADDI <rd>, <rs>, 0
        case (RISCV_LUI):  return  utype(index, scs, 0b0110111,                      Out,      0,                                               0,     0,     0);
        case (RISCV_ADD):  return  rtype(index, scs, 0b0000000, 0b000, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_SUB):  return  rtype(index, scs, 0b0100000, 0b000, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_NEG):  return  rtype(index, scs, 0b0100000, 0b000, 0b0110011,    Out,      0,        In,                           ARG1_FIXED,     0,     0,     0);
        case (RISCV_ADDI): return  itype(index, scs, 0b000, 0b0010011,               Out,     In,         0,                                    0,     0,     0,     0);
        case (RISCV_MUL):  return  rtype(index, scs, 0b0000001, 0b000, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_DIV):  return  rtype(index, scs, 0b0000001, 0b100, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_REM):  return  rtype(index, scs, 0b0000001, 0b110, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_SLL):  return  rtype(index, scs, 0b0000000, 0b001, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_SLLI): return istype(index, scs,  0b000000, 0b001, 0b0010011,    Out,     In,         0,                                    0,     0,     0,     0);
        case (RISCV_SRL):  return  rtype(index, scs, 0b0000000, 0b101, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_SRLI): return istype(index, scs,  0b000000, 0b101, 0b0010011,    Out,     In,         0,                                    0,     0,     0,     0);
        case (RISCV_SRA):  return  rtype(index, scs, 0b0100000, 0b101, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_SRAI): return istype(index, scs,  0b010000, 0b101, 0b0010011,    Out,     In,         0,                                    0,     0,     0,     0);
        case (RISCV_XOR):  return  rtype(index, scs, 0b0000000, 0b100, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_XORI): return  itype(index, scs, 0b100, 0b0010011,               Out,     In,         0,                                    0,     0,     0,     0);
        case (RISCV_OR):   return  rtype(index, scs, 0b0000000, 0b110, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_ORI):  return  itype(index, scs, 0b110, 0b0010011,               Out,     In,         0,                                    0,     0,     0,     0);
        case (RISCV_AND):  return  rtype(index, scs, 0b0000000, 0b111, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_ANDI): return  itype(index, scs, 0b111, 0b0010011,               Out,     In,         0,                                    0,     0,     0,     0);
        case (RISCV_NOT):  return  itype(index, scs, 0b100, 0b0010011,               Out,     In,         0,                           ARG2_FIXED,     0,     0,    -1);
        case (RISCV_SLT):  return  rtype(index, scs, 0b0000000, 0b010, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_SLTU): return  rtype(index, scs, 0b0000000, 0b011, 0b0110011,    Out,     In,        In,                                    0,     0,     0,     0);
        case (RISCV_SEQZ): return  itype(index, scs, 0b011, 0b0010011,               Out,     In,         0,                           ARG2_FIXED,     0,     0,     1);
        case (RISCV_SNEZ): return  rtype(index, scs, 0b0000000, 0b011, 0b0110011,    Out,     In,        In,                           ARG1_FIXED,     0,     0,     0);
        case (RISCV_BEQ):  return  btype(index, scs, 0b000, 0b1100011,                In,     In,       Lab,                                    0,     0,     0,     0);
        case (RISCV_BNE):  return  btype(index, scs, 0b001, 0b1100011,                In,     In,       Lab,                                    0,     0,     0,     0);
        case (RISCV_BLT):  return  btype(index, scs, 0b100, 0b1100011,                In,     In,       Lab,                                    0,     0,     0,     0);
        case (RISCV_BGE):  return  btype(index, scs, 0b101, 0b1100011,                In,     In,       Lab,                                    0,     0,     0,     0);
        case (RISCV_BLTU): return  btype(index, scs, 0b110, 0b1100011,                In,     In,       Lab,                                    0,     0,     0,     0);
        case (RISCV_BGEU): return  btype(index, scs, 0b111, 0b1100011,                In,     In,       Lab,                                    0,     0,     0,     0);
        case (RISCV_J):    return  jtype(index, scs, 0b1101111,                        0,    Lab,                                      ARG0_FIXED,     0,     0);
        case (RISCV_JALR): return  itype(index, scs, 0b000, 0b1100111,                 0,     In,         0,              ARG0_FIXED | ARG2_FIXED,    RA,     0,     0);
        case (RISCV_LABEL): return BiT({});
        case (RISCV_RET):  return  itype(index, scs, 0b000, 0b1100111,                 0,      0,         0, ARG0_FIXED | ARG1_FIXED | ARG2_FIXED,  ZERO,    RA,     0);
        case (RISCV_LB):
            if (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG && signed_fits((uint64_t)index.args[1].value, 12))
                return BiT({ BTimm(1, 12, Addr64), BTreg(2, 5, In| Addr64), BTsta(0b000, 3), BTreg(0, 5, Out), BTsta(0b0000011, 7) });
            break;
        case (RISCV_LBU): 
            if (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG && signed_fits((uint64_t)index.args[1].value, 12))
                return BiT({ BTimm(1, 12, Addr64), BTreg(2, 5, In| Addr64), BTsta(0b100, 3), BTreg(0, 5, Out), BTsta(0b0000011, 7) });
            break;
        case (RISCV_LH): 
            if (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG && signed_fits((uint64_t)index.args[1].value, 12))
                return BiT({ BTimm(1, 12, Addr64), BTreg(2, 5, In| Addr64), BTsta(0b001, 3), BTreg(0, 5, Out), BTsta(0b0000011, 7) });
            break;
        case (RISCV_LHU): 
            if (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG && signed_fits((uint64_t)index.args[1].value, 12))
                return BiT({ BTimm(1, 12, Addr64), BTreg(2, 5, In| Addr64), BTsta(0b101, 3), BTreg(0, 5, Out), BTsta(0b0000011, 7) });
            break;
        case (RISCV_LW): 
            if (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG && signed_fits((uint64_t)index.args[1].value, 12))
                return BiT({ BTimm(1, 12, Addr64), BTreg(2, 5, In| Addr64), BTsta(0b010, 3), BTreg(0, 5, Out), BTsta(0b0000011, 7) });
            break;
        case (RISCV_LWU): 
            if (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG && signed_fits((uint64_t)index.args[1].value, 12))
                return BiT({ BTimm(1, 12, Addr64), BTreg(2, 5, In| Addr64), BTsta(0b110, 3), BTreg(0, 5, Out), BTsta(0b0000011, 7) });
            break;
        case (RISCV_LD): 
            if (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG && signed_fits((uint64_t)index.args[1].value, 12))
                return BiT({ BTimm(1, 12, Addr64), BTreg(2, 5, In| Addr64), BTsta(0b011, 3), BTreg(0, 5, Out), BTsta(0b0000011, 7) });
            break;
        default:
            break;
        }
        scs = false;
        return BinTranslation();
    }

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
                case (TYPE_I8):  return SyT(RISCV_LB ,{ SAcop(0), SAimm(0), SAcop(1) });
                case (TYPE_U8):  return SyT(RISCV_LBU,{ SAcop(0), SAimm(0), SAcop(1) });
                case (TYPE_I16): return SyT(RISCV_LH ,{ SAcop(0), SAimm(0), SAcop(1) });
                case (TYPE_U16): case (TYPE_FP16): return SyT(RISCV_LHU,{ SAcop(0), SAimm(0), SAcop(1) });
                case (TYPE_I32): return SyT(RISCV_LW,{ SAcop(0), SAimm(0), SAcop(1) });
                case (TYPE_U32): case (TYPE_FP32): return SyT(RISCV_LWU,{ SAcop(0), SAimm(0), SAcop(1) });
                case (TYPE_FP64): case (TYPE_I64): case (TYPE_U64): return SyT(RISCV_LD,{ SAcop(0), SAimm(0), SAcop(1) });
                default: break;
                }
            }
            else if (index.size() == 3 && index[2].tag == Arg::IIMMEDIATE)
            {
                switch (index[0].elemtype)
                {
                case (TYPE_I8):  return SyT(RISCV_LB ,{ SAcop(0), SAcop(2), SAcop(1) });
                case (TYPE_U8):  return SyT(RISCV_LBU,{ SAcop(0), SAcop(2), SAcop(1) });
                case (TYPE_I16): return SyT(RISCV_LH ,{ SAcop(0), SAcop(2), SAcop(1) });
                case (TYPE_U16): case (TYPE_FP16): return SyT(RISCV_LHU,{ SAcop(0), SAcop(2), SAcop(1) });
                case (TYPE_I32): return SyT(RISCV_LW,{ SAcop(0), SAcop(2), SAcop(1) });
                case (TYPE_U32): case (TYPE_FP32): return SyT(RISCV_LWU,{ SAcop(0), SAcop(2), SAcop(1) });
                case (TYPE_FP64): case (TYPE_I64): case (TYPE_U64): return SyT(RISCV_LD,{ SAcop(0), SAcop(2), SAcop(1) });
                default: break;
                }
            }
            break;    
        case (OP_STORE):
            if (index.size() == 2)
            {
                switch (index[1].elemtype)
                {
                    case (TYPE_I8): case (TYPE_U8):
                        return SyT(RISCV_SB, { SAcop(1), SAimm(0), SAcop(0) });
                    case (TYPE_I16): case (TYPE_U16): case (TYPE_FP16):
                        return SyT(RISCV_SH, { SAcop(1), SAimm(0), SAcop(0) });
                    case (TYPE_I32): case (TYPE_U32): case (TYPE_FP32):
                        return SyT(RISCV_SW, { SAcop(1), SAimm(0), SAcop(0) });
                    case (TYPE_I64): case (TYPE_U64): case (TYPE_FP64):
                        return SyT(RISCV_SD, { SAcop(1), SAimm(0), SAcop(0) });
                    default: break;
                }
            }
            else if (index.size() == 3 && index[1].tag == Arg::IIMMEDIATE)
            {
                switch (index[2].elemtype)
                {
                    case (TYPE_I8): case (TYPE_U8):
                        return SyT(RISCV_SB, { SAcop(2), SAcop(1), SAcop(0) });
                    case (TYPE_I16): case (TYPE_U16): case (TYPE_FP16):
                        return SyT(RISCV_SH, { SAcop(2), SAcop(1), SAcop(0) });
                    case (TYPE_I32): case (TYPE_U32): case (TYPE_FP32):
                        return SyT(RISCV_SW, { SAcop(2), SAcop(1), SAcop(0) });
                    case (TYPE_I64): case (TYPE_U64): case (TYPE_FP64):
                        return SyT(RISCV_SD, { SAcop(2), SAcop(1), SAcop(0) });
                    default: break;
                }
            }
            break;
        case (OP_ADD):
            if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG)
            {
                if(index[2].tag == Arg::IREG)
                    return SyT(RISCV_ADD,  { SAcop(0), SAcop(1), SAcop(2) });
                else if(index[2].tag == Arg::IIMMEDIATE)
                    return SyT(RISCV_ADDI,  { SAcop(0), SAcop(1), SAcop(2) });
            }
            break;
        case (OP_SUB):
            if(index.args_size == 3 && index.args[0].tag == Arg::IREG)
            {
                if(index.args[1].tag == Arg::IREG)
                {
                    if(index[2].tag == Arg::IREG )
                        return SyT(RISCV_SUB,  { SAcop(0), SAcop(1), SAcop(2) });
                    else if(index[2].tag == Arg::IIMMEDIATE && signed_fits(-index.args[2].value, 12))
                        return SyT(RISCV_ADDI,  { SAcop(0), SAcop(1), SAimm(-index.args[2].value) });
                }
                else if(index.args[1].tag == Arg::IIMMEDIATE && index.args[1].value == 0 && index.args[2].tag == Arg::IREG)
                    return SyT(RISCV_SUB,  { SAcop(0), SAreg(0), SAcop(2) });
            }
            break;
        case (OP_MOV):
            if(index.size() == 2 && index[0].tag == Arg::IREG)
            {
                if(index[1].tag == Arg::IREG)
                    return SyT(RISCV_MV,  { SAcop(0), SAcop(1) });
                else if(index[1].tag == Arg::IIMMEDIATE)
                {
                    if(index[1].value == 0)
                        return SyT(RISCV_MV,  { SAcop(0), SAreg(ZERO) });
                    else
                        return SyT(RISCV_ADDI,  { SAcop(0), SAreg(ZERO), SAcop(1)});
                }
            }
            break;
        case (OP_RV_LUI):
            if(index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE)
                return SyT(RISCV_LUI,  { SAcop(0), SAcop(1) });
            break;
        case (OP_MUL): return SyT(RISCV_MUL,  { SAcop(0), SAcop(1), SAcop(2) });
        case (OP_DIV): return SyT(RISCV_DIV,  { SAcop(0), SAcop(1), SAcop(2) });
        case (OP_MOD): return SyT(RISCV_REM,  { SAcop(0), SAcop(1), SAcop(2) });
        case (OP_NEG): return SyT(RISCV_NEG,  { SAcop(0), SAcop(1) });
        case (OP_SHL):
            if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && (index[2].tag == Arg::IREG || index[2].tag == Arg::IIMMEDIATE))
                return SyT(index[2].tag == Arg::IREG ? RISCV_SLL : RISCV_SLLI,  { SAcop(0), SAcop(1), SAcop(2) });
            break;
        case (OP_SHR):
            if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && (index[2].tag == Arg::IREG || index[2].tag == Arg::IIMMEDIATE))
                return SyT(index[2].tag == Arg::IREG ? RISCV_SRL : RISCV_SRLI,  { SAcop(0), SAcop(1), SAcop(2) });
            break;
        case (OP_SAR):
            if(index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && (index[2].tag == Arg::IREG || index[2].tag == Arg::IIMMEDIATE))
                return SyT(index[2].tag == Arg::IREG ? RISCV_SRA : RISCV_SRAI,  { SAcop(0), SAcop(1), SAcop(2) });
            break;
        case (OP_XOR): 
            if(index.size() == 3 && index[0].tag == Arg::IREG)
            {
                if(index[1].tag == Arg::IREG && index[2].tag == Arg::IREG)
                    return SyT(RISCV_XOR,  { SAcop(0), SAcop(1), SAcop(2) });
                if(index[1].tag == Arg::IREG && index[2].tag == Arg::IIMMEDIATE)
                    return SyT(RISCV_XORI,  { SAcop(0), SAcop(1), SAcop(2) });
                if(index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG)
                    return SyT(RISCV_XORI,  { SAcop(0), SAcop(2), SAcop(1) });
            }
            break;
        case (OP_OR):
            if(index.size() == 3 && index[0].tag == Arg::IREG)
            {
                if(index[1].tag == Arg::IREG && index[2].tag == Arg::IREG)
                    return SyT(RISCV_OR,  { SAcop(0), SAcop(1), SAcop(2) });
                if(index[1].tag == Arg::IREG && index[2].tag == Arg::IIMMEDIATE)
                    return SyT(RISCV_ORI,  { SAcop(0), SAcop(1), SAcop(2) });
                if(index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG)
                    return SyT(RISCV_ORI,  { SAcop(0), SAcop(2), SAcop(1) });
            }
            break;
        case (OP_AND):
            if(index.size() == 3 && index[0].tag == Arg::IREG)
            {
                if(index[1].tag == Arg::IREG && index[2].tag == Arg::IREG)
                    return SyT(RISCV_AND,  { SAcop(0), SAcop(1), SAcop(2) });
                if(index[1].tag == Arg::IREG && index[2].tag == Arg::IIMMEDIATE)
                    return SyT(RISCV_ANDI,  { SAcop(0), SAcop(1), SAcop(2) });
                if(index[1].tag == Arg::IIMMEDIATE && index[2].tag == Arg::IREG)
                    return SyT(RISCV_ANDI,  { SAcop(0), SAcop(2), SAcop(1) });
            }
            break;
        case (OP_NOT): return SyT(RISCV_NOT,  { SAcop(0), SAcop(1) });
        case (OP_IVERSON):
            if (index.args_size == 4 && index.args[0].tag == Arg::IREG && index.args[1].tag == Arg::IIMMEDIATE)
            {
                if((index.args[1].value == OP_EQ || index.args[1].value == OP_NE) && index.args[2].tag == Arg::IREG && index.args[3].tag == Arg::IIMMEDIATE && index.args[3].value == 0)
                    return SyT(index.args[1].value == OP_EQ ? RISCV_SEQZ : RISCV_SNEZ,  { SAcop(0), SAcop(2) });
                else if((index.args[2].tag == Arg::IREG || (index.args[2].tag == Arg::IIMMEDIATE && index.args[2].value == 0)) && 
                        (index.args[3].tag == Arg::IREG || (index.args[3].tag == Arg::IIMMEDIATE && index.args[3].value == 0)))
                {
                    SyntopTranslation::ArgTranslation arg2 = (index.args[2].tag == Arg::IREG ? SAcop(2) : SAreg(ZERO));
                    SyntopTranslation::ArgTranslation arg3 = (index.args[3].tag == Arg::IREG ? SAcop(3) : SAreg(ZERO));
                    switch (index.args[1].value)
                    {//TODO(ch): Sometimes it's possible to handle immediate on second positions with SLTI and SLTIU.
                    case OP_LT: return SyT(RISCV_SLT,  { SAcop(0), arg2, arg3 });
                    case OP_GT: return SyT(RISCV_SLT,  { SAcop(0), arg3, arg2 });
                    case OP_UGT: return SyT(RISCV_SLTU,  { SAcop(0), arg3, arg2 });
                    default: break;
                    }
                }
            }
            break;
        case (OP_UNSPILL):
            if(index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE)
                return SyT(RISCV_LD, { SAcopelt(0, TYPE_I64), SAcopsar(1, -3), SAreg(SP) });
            break;
        case (OP_SPILL):
            if(index.size() == 2 && index[0].tag == Arg::IIMMEDIATE && index[1].tag == Arg::IREG) 
                return SyT(RISCV_SD, { SAcopelt(1, TYPE_I64), SAcopsar(0,-3), SAreg(SP) });
            break;        
        case (OP_JCC): //TODO(ch)[1]: Sometimes immediate zero can be encoded with hardwired zero.
            if(index.size() == 4 && index[0].tag == Arg::IIMMEDIATE && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG && index[3].tag == Arg::IIMMEDIATE)
            {
                switch (index[0].value)
                {
                case (OP_EQ):  return SyT(RISCV_BEQ,  { SAcop(1), SAcop(2), SAcop(3) });
                case (OP_NE):  return SyT(RISCV_BNE,  { SAcop(1), SAcop(2), SAcop(3) });
                case (OP_LT):  return SyT(RISCV_BLT,  { SAcop(1), SAcop(2), SAcop(3) });
                case (OP_GT):  return SyT(RISCV_BLT,  { SAcop(2), SAcop(1), SAcop(3) });
                case (OP_GE):  return SyT(RISCV_BGE,  { SAcop(1), SAcop(2), SAcop(3) });
                case (OP_LE):  return SyT(RISCV_BGE,  { SAcop(2), SAcop(1), SAcop(3) });
                case (OP_UGT): return SyT(RISCV_BLTU, { SAcop(2), SAcop(1), SAcop(3) });
                case (OP_ULE): return SyT(RISCV_BGEU, { SAcop(2), SAcop(1), SAcop(3) });
                default:
                    break;
                };
            }
            break;
        case (OP_CALL_NORET):
            if(index.size() == 1 && index.args[0].tag == Arg::IREG)
                return SyT(RISCV_JALR, { SAcop(0) });
            break;
        case (OP_JMP):     return SyT(RISCV_J, { SAcop(0) });
        case (OP_LABEL):   return SyT(RISCV_LABEL, { SAcop(0) });
        case (OP_RET):     return SyT(RISCV_RET, {});
        default:
            break;
        }
        scs = false;
        return SyntopTranslation();
    }

    class RiscVBRASnippets1 : public CompilerPass
    {
    public:
        virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override;
        virtual ~RiscVBRASnippets1() override {}
        virtual bool is_inplace() const override final { return false; }
        virtual std::string pass_id() const override final { return "CP_RISCV_BRA_SNIPPETS1"; }
        static CompilerPassPtr make(const Backend* a_backend)
        {
            std::shared_ptr<RiscVBRASnippets1> res;
            res.reset(new RiscVBRASnippets1(a_backend));
            return std::static_pointer_cast<CompilerPass>(res);
        } 
    private: 
        RiscVBRASnippets1(const Backend* a_backend) : CompilerPass(a_backend) {}
    };

    class RiscVBRASnippets2 : public CompilerPass
    {
    public:
        virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override;
        virtual ~RiscVBRASnippets2() override {}
        virtual bool is_inplace() const override final { return false; }
        virtual std::string pass_id() const override final { return "CP_RISCV_BRA_SNIPPETS2"; }
        static CompilerPassPtr make(const Backend* a_backend)
        {
            std::shared_ptr<RiscVBRASnippets2> res;
            res.reset(new RiscVBRASnippets2(a_backend));
            return std::static_pointer_cast<CompilerPass>(res);
        } 
    private: 
        RiscVBRASnippets2(const Backend* a_backend) : CompilerPass(a_backend) {}
        void mov32(Syntfunc& a_dest, Arg destarg, int64_t val);
    };    

    class RiscVARASnippets : public CompilerPass
    {
    public:
        virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override;
        virtual ~RiscVARASnippets() override {}
        virtual bool is_inplace() const override final { return false; }
        virtual std::string pass_id() const override final { return "CP_RISCV_ARA_SNIPPETS"; }
        static CompilerPassPtr make(const Backend* a_backend)
        {
            std::shared_ptr<RiscVARASnippets> res;
            res.reset(new RiscVARASnippets(a_backend));
            return std::static_pointer_cast<CompilerPass>(res);
        } 
    private: 
        RiscVARASnippets(const Backend* a_backend) : CompilerPass(a_backend) {}
    };

    RiscVBackend::RiscVBackend()
    {
        m_s2blookup = i64BTLookup;
        m_s2slookup = i64STLookup;
        m_vectorRegisterBits = 256; // AVX2???
        m_isLittleEndianInstructions = true;
        m_isLittleEndianOperands = false;
        m_isMonowidthInstruction = false;
        m_callerStackIncrement = 1;
        m_postInstructionOffset = false;
        m_registersAmount = 40;
        m_name = "Risc-V";
        m_beforeRegAllocPasses.push_back(RiscVBRASnippets1::make(this));
        m_beforeRegAllocPasses.push_back(RiscVBRASnippets2::make(this));
        m_afterRegAllocPasses.push_back(RiscVARASnippets::make(this));
#if __LOOPS_OS == __LOOPS_LINUX
        m_parameterRegisters[RB_INT] = { A0, A1, A2, A3, A4, A5, A6, A7 };
        m_returnRegisters[RB_INT] = { A0, A1 };
        m_callerSavedRegisters[RB_INT] = { T1, T2, T3, T4, T5, T6 };
        m_calleeSavedRegisters[RB_INT] = { S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, S11 };
#else
#error Unknown OS
#endif
    }

    RiscVBackend::~RiscVBackend()
    {}

    int RiscVBackend::spillSpaceNeeded(const Syntop& a_op, int basketNum) const
    {
        if(basketNum == RB_INT)
            switch (a_op.opcode)
            {
            case (OP_CALL):
            case (OP_CALL_NORET):
                return 14;
            default:
                break;
            }
        return Backend::spillSpaceNeeded(a_op, basketNum);
    }

    std::set<int> RiscVBackend::getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask) const
    {
        //TODO(ch): This specialized version of function must disappear after introducing snippets. 
        //They will give info about used registers, like now instructions answers.
        //Actually, it's easy to think, that we have to keep used registers info on level of SyntopTranslation. Hmm...
        switch (a_op.opcode)
        {
            case (OP_JCC):
                Assert(a_op.size() == 4 && a_op[0].tag == Arg::IIMMEDIATE && a_op[1].tag == Arg::IREG && a_op[2].tag == Arg::IREG && a_op[3].tag == Arg::IIMMEDIATE);
                if (basketNum == RB_INT && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
                {
                    if (AF_INPUT & flagmask)
                        return std::set<int>({1,2});
                    else 
                        return std::set<int>({});
                } else if(basketNum == RB_VEC)
                    return std::set<int>({});
                break;
            case (OP_IVERSON):
            {
                Assert(a_op.size() == 4);
                if (basketNum == RB_INT && (~(AF_INPUT | AF_OUTPUT) & flagmask) == 0)
                {
                    if (AF_OUTPUT & flagmask && ((AF_INPUT & flagmask) == 0))
                        return std::set<int>({0});
                    else if(((AF_OUTPUT & flagmask) == 0) && (AF_INPUT & flagmask))
                        return ((a_op[3].tag == Arg::IIMMEDIATE) ? std::set<int>({2}) : ((a_op[2].tag == Arg::IIMMEDIATE) ? std::set<int>({3}) : std::set<int>({2, 3})));
                    else
                        return std::set<int>({});
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
            default:
                break;
        };
        return Backend::getUsedRegistersIdxs(a_op, basketNum, flagmask);
    }

    void RiscVBackend::getStackParameterLayout(const Syntfunc& a_func, const std::vector<int> (&regParsOverride)[RB_AMOUNT], std::map<RegIdx, int> (&parLayout)[RB_AMOUNT]) const
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

    int RiscVBackend::stackGrowthAlignment(int stackGrowth) const
    {
        return stackGrowth ? stackGrowth + (stackGrowth % 2) : stackGrowth;
    }

    void RiscVBackend::writeCallerPrologue(Syntfunc& prog, int stackGrowth) const
    {
        //TODO(ch): there is some hesitation, that this stack epilogue/prologue will provide some correct
        //ability to unwind stack. On the other hand, there is hesitation, that frame pointer spill is needed
        //at all(gcc always use it like usual callee-saved variable). Probably code, generated by clang will 
        //clear out the situation.
        prog.program.push_back(Syntop(OP_SPILL, { argIImm(stackGrowth-2), argReg(RB_INT, FP) }));
        prog.program.push_back(Syntop(OP_SPILL, { argIImm(stackGrowth-1), argReg(RB_INT, RA) }));
    }

    void RiscVBackend::writeCallerEpilogue(Syntfunc& prog, int stackGrowth) const
    {
        prog.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT, FP),   argIImm(stackGrowth-2) }));
        prog.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT, RA),   argIImm(stackGrowth-1) }));
    }

    Arg RiscVBackend::getSParg() const
    {
        return argReg(RB_INT, SP);
    }

    column_printer RiscVBackend::get_opname_printer() const
    {
        column_printer ret = { /*func = */ &col_opname_table_printer, /*auxdata = */ opstrings , /*free_func = */ NULL };
        return ret;
    }

    typedef struct riscV_opargs_printer_aux
    {
        LOOPS_HASHMAP(int, int) pos2opnum;
        LOOPS_SPAN(int) positions;
    } riscV_opargs_printer_aux;

    static int riscV_opargs_printer(program_printer* printer, column_printer* colprinter, syntfunc2print* func, int row)
    {
        int program_size = func->program->size;
        loops::Syntop* program = func->program->data;
        int err;
        riscV_opargs_printer_aux* argaux = (riscV_opargs_printer_aux*)colprinter->auxdata;
        if (argaux == NULL)
        {
            int oppos = 0;
            int opnum = 0;
            argaux = (riscV_opargs_printer_aux*)malloc(sizeof(riscV_opargs_printer_aux));
            if (argaux == NULL)
                LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
            memset(argaux, 0, sizeof(riscV_opargs_printer_aux));
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
                int opsize = (opcode == RISCV_LABEL ? 0 : 4);
                argaux->positions->data[opnum] = oppos;
                if(opcode == RISCV_LABEL)
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
            if (operand_flags[anum] & AF_PRINTOFFSET)
            {
                int targetline;
                if (arg.tag != Arg::IIMMEDIATE)
                    LOOPS_THROW(LOOPS_ERR_INCORRECT_ARGUMENT);
                int offset2find = argaux->positions->data[row + 1] + (int)arg.value - 4;
                err = loops_hashmap_get(argaux->pos2opnum, offset2find, &targetline);
                if(err == LOOPS_ERR_ELEMENT_NOT_FOUND)
                    LOOPS_THROW(LOOPS_ERR_INTERNAL_INCORRECT_OFFSET);
                else if(err != LOOPS_ERR_SUCCESS)
                    LOOPS_THROW(err);
                Assert(targetline >= 0);
                Syntop* labelop = program + targetline;
                Assert(labelop->opcode == RISCV_LABEL);
                Assert(labelop->opcode == RISCV_LABEL && labelop->args_size == 1);
                Assert(labelop->opcode == RISCV_LABEL && labelop->args_size == 1 && labelop->args[0].tag == Arg::IIMMEDIATE);
                LOOPS_CALL_THROW(loops_printf(printer, "__loops_label_%d", (int)(labelop->args[0].value)));
                continue;
            }
            uint64_t argflags = operand_flags[anum];
            bool address = (argflags & AF_ADDRESS);
            bool inhibit_comma = false;
            switch (arg.tag)
            {
                case Arg::IREG:
                {
                    if(address)
                        LOOPS_CALL_THROW(loops_printf(printer, "("));
                    static const char* rnames[32] = { "zero", "ra", "sp", "gp", "tp", "lr", "t1", "t2", "fp", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6" };
                    LOOPS_CALL_THROW(loops_printf(printer, "%s", rnames[arg.idx]));
                    if(address)
                        LOOPS_CALL_THROW(loops_printf(printer, ")"));
                    break;
                }
                case Arg::IIMMEDIATE:
                    if(op->opcode == RISCV_LABEL)
                    {
                        LOOPS_CALL_THROW(loops_printf(printer, "__loops_label_%d:", arg.value));
                    }
                    else
                    {
                        if(arg.value == 0)
                            LOOPS_CALL_THROW(loops_printf(printer, "0"));
                        else
                        {
                            bool negative = (!(argflags & AF_UNSIGNED) && arg.value < 0);
                            uint32_t upper32;
                            uint32_t lower32;
                            if(negative)
                            {
                                uint64_t ival = ~((uint64_t)arg.value);
                                uint64_t lower32_ = (ival & 0xffffffff) + 1;
                                upper32 = (ival >> 32) + (lower32_ & 0x100000000 ? 1 : 0);
                                lower32 = lower32_ & 0xffffffff;
                            }
                            else
                            {
                                upper32 = ((uint64_t)arg.value) >> 32;
                                lower32 = ((uint64_t)arg.value) & 0xffffffff;
                            }
                            if (upper32 > 0)
                                LOOPS_CALL_THROW(loops_printf(printer, "%s0x%x%08x", (negative ? "-": ""), upper32, lower32));
                            else
                                LOOPS_CALL_THROW(loops_printf(printer, "%s0x%02x", (negative ? "-": ""), lower32));
                        }
                        if(address)
                            inhibit_comma = true;
                    }
                    break;
                default:
                    LOOPS_THROW(LOOPS_ERR_UNKNOWN_ARGUMENT_TYPE);
            };
            if (anum < aamount - 1 && !inhibit_comma)
                LOOPS_CALL_THROW(loops_printf(printer, ", "));
        }
        LOOPS_CALL_THROW(close_printer_cell(printer));
        return LOOPS_ERR_SUCCESS;
    }

    static void free_riscv_opargs_printer(column_printer* colprinter)
    {
        if (colprinter->auxdata != NULL)
        {
            riscV_opargs_printer_aux* argaux = (riscV_opargs_printer_aux*)colprinter->auxdata;
            loops_hashmap_destruct(argaux->pos2opnum);
            loops_span_destruct(argaux->positions);
            free(argaux);
            colprinter->auxdata = NULL;
        }
    }

    column_printer RiscVBackend::get_opargs_printer() const
    {
        column_printer ret = { /*func = */ &riscV_opargs_printer, /*auxdata = */ NULL, /*free_func = */ &free_riscv_opargs_printer };
        return ret;
    }

    typedef struct riscV_hex_printer_aux
    {
        LOOPS_SPAN(int) positions;
        LOOPS_SPAN(uint8_t) binary;
    } riscV_hex_printer_aux;

    static int riscV_hex_printer(program_printer* printer, column_printer* colprinter, syntfunc2print* func, int row)
    {
        int err;
        int program_size = func->program->size;
        loops::Syntop* program = func->program->data;
        int params_size = func->params->size;
        loops::Arg* params = func->params->data;

        riscV_hex_printer_aux* argaux = (riscV_hex_printer_aux*)colprinter->auxdata;
        if (argaux == NULL)
        {
            int oppos = 0;
            int opnum = 0;
            argaux = (riscV_hex_printer_aux*)malloc(sizeof(riscV_hex_printer_aux));
            if (argaux == NULL)
                LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);
            memset(argaux, 0, sizeof(riscV_hex_printer_aux));
            err = loops_span_construct_alloc(&(argaux->positions), program_size);
            if (err != LOOPS_ERR_SUCCESS)
            {
                free(argaux);
                LOOPS_THROW(err);
            }
            for (; opnum < program_size; opnum++)
            {
                int opsize = (program[opnum].opcode == RISCV_LABEL ? 0 : 4);
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
                Syntfunc dummy;
                a2hPass.process(dummy, tmpfunc);
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
        if(program[row].opcode != RISCV_LABEL)
        {
            unsigned char* hexfield = argaux->binary->data + argaux->positions->data[row];
            for(size_t pos = 0; pos < 4; pos++) //TODO(ch): Print variants (direct or reverse order).
                LOOPS_CALL_THROW(loops_printf(printer, "%02x ", (unsigned)(*(hexfield + pos))));
        }
        LOOPS_CALL_THROW(close_printer_cell(printer));
        return LOOPS_ERR_SUCCESS;
    }

    static void free_riscV_hex_printer(column_printer* colprinter)
    {
        if (colprinter->auxdata != NULL)
        {
            riscV_hex_printer_aux* argaux = (riscV_hex_printer_aux*)colprinter->auxdata;
            loops_span_destruct(argaux->positions);
            loops_span_destruct(argaux->binary);
            free(argaux);
            colprinter->auxdata = NULL;
        }
    }

    column_printer RiscVBackend::get_hex_printer() const
    {
        column_printer ret = { /*func = */ &riscV_hex_printer, /*auxdata = */ NULL, /*free_func = */ &free_riscV_hex_printer };
        return ret;
    }

    void RiscVBRASnippets1::process(Syntfunc& a_dest, const Syntfunc& a_source)
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
            case OP_SELECT:
            {
                Assert(op.args_size == 4 && op.args[0].tag == Arg::IREG && op.args[1].tag == Arg::IIMMEDIATE && op.args[2].tag == Arg::IREG && op.args[3].tag == Arg::IREG);
                Arg zero_or_one = op.args[0]; zero_or_one.idx = a_dest.provideIdx(RB_INT);
                a_dest.program.push_back(Syntop(OP_IVERSON,  { zero_or_one, op.args[1] }));
                Arg diap = op.args[0]; diap.idx = a_dest.provideIdx(RB_INT);
                a_dest.program.push_back(Syntop(OP_SUB,  { diap, op.args[2], op.args[3] }));
                a_dest.program.push_back(Syntop(OP_SUB,  { zero_or_one, argIImm(0), zero_or_one }));
                a_dest.program.push_back(Syntop(OP_AND,  { zero_or_one, zero_or_one, diap }));
                a_dest.program.push_back(Syntop(OP_ADD,  { op.args[0], op.args[3], zero_or_one }));
                break;
            }
            case OP_MIN:
            {
                Assert(op.args_size == 3 && op.args[0].tag == Arg::IREG && op.args[1].tag == Arg::IREG && op.args[2].tag == Arg::IREG);
                a_dest.program.push_back(Syntop(OP_CMP,  { op.args[1], op.args[2] }));
                Arg zero_or_one = op.args[0]; zero_or_one.idx = a_dest.provideIdx(RB_INT);
                a_dest.program.push_back(Syntop(OP_IVERSON,  { zero_or_one, argIImm(OP_LT) }));
                Arg diap = op.args[0]; diap.idx = a_dest.provideIdx(RB_INT);
                a_dest.program.push_back(Syntop(OP_SUB,  { diap, op.args[1], op.args[2] }));
                a_dest.program.push_back(Syntop(OP_SUB,  { zero_or_one, argIImm(0), zero_or_one }));
                a_dest.program.push_back(Syntop(OP_AND,  { zero_or_one, zero_or_one, diap }));
                a_dest.program.push_back(Syntop(OP_ADD,  { op.args[0], op.args[2], zero_or_one }));
                break;
            }
            case OP_MAX:
            {
                Assert(op.args_size == 3 && op.args[0].tag == Arg::IREG && op.args[1].tag == Arg::IREG && op.args[2].tag == Arg::IREG);
                a_dest.program.push_back(Syntop(OP_CMP,  { op.args[1], op.args[2] }));
                Arg zero_or_one = op.args[0]; zero_or_one.idx = a_dest.provideIdx(RB_INT);
                a_dest.program.push_back(Syntop(OP_IVERSON,  { zero_or_one, argIImm(OP_LT) }));
                Arg diap = op.args[0]; diap.idx = a_dest.provideIdx(RB_INT);
                a_dest.program.push_back(Syntop(OP_SUB,  { diap, op.args[2], op.args[1] }));
                a_dest.program.push_back(Syntop(OP_SUB,  { zero_or_one, argIImm(0), zero_or_one }));
                a_dest.program.push_back(Syntop(OP_AND,  { zero_or_one, zero_or_one, diap }));
                a_dest.program.push_back(Syntop(OP_ADD,  { op.args[0], op.args[1], zero_or_one }));
                break;
            }
            case OP_ABS:
            {
                Assert(op.args_size == 2 && op.args[0].tag == Arg::IREG && op.args[1].tag == Arg::IREG);
                a_dest.program.push_back(Syntop(OP_CMP,  { argIImm(0), op.args[1] }));
                Arg zero_or_one = op.args[0]; zero_or_one.idx = a_dest.provideIdx(RB_INT);
                a_dest.program.push_back(Syntop(OP_IVERSON,  { zero_or_one, argIImm(OP_LT) }));
                a_dest.program.push_back(Syntop(OP_SUB,  { zero_or_one, argIImm(0), zero_or_one }));
                a_dest.program.push_back(Syntop(OP_AND,  { zero_or_one, zero_or_one, op.args[1] }));
                a_dest.program.push_back(Syntop(OP_SUB,  { op.args[0], argIImm(0), op.args[1] }));
                a_dest.program.push_back(Syntop(OP_ADD,  { op.args[0], op.args[0], zero_or_one }));
                a_dest.program.push_back(Syntop(OP_ADD,  { op.args[0], op.args[0], zero_or_one }));
                break;
            }
            case OP_SIGN:            
            {
                Assert(op.args_size == 2 && op.args[0].tag == Arg::IREG && op.args[1].tag == Arg::IREG);
                Arg negative = op.args[0]; negative.idx = a_dest.provideIdx(RB_INT);
                a_dest.program.push_back(Syntop(OP_CMP,  { op.args[1], argIImm(0) }));
                a_dest.program.push_back(Syntop(OP_IVERSON,  { negative, argIImm(OP_LT) }));
                Arg positive = op.args[0]; positive.idx = a_dest.provideIdx(RB_INT);
                a_dest.program.push_back(Syntop(OP_CMP,  { argIImm(0), op.args[1] }));
                a_dest.program.push_back(Syntop(OP_IVERSON,  { positive, argIImm(OP_LT) }));
                a_dest.program.push_back(Syntop(OP_SUB,  { op.args[0], positive, negative }));
                break;
            }
            default:
                a_dest.program.push_back(op);
                break;
            }
        }
    }

    void RiscVBRASnippets2::process(Syntfunc& a_dest, const Syntfunc& a_source)
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
            case OP_LOAD:
                if(op.size() == 3 && op.args[0].tag == Arg::IREG && op.args[1].tag == Arg::IREG && op.args[2].tag == Arg::IREG)
                {
                    Arg newaddr = op.args[1]; newaddr.idx = a_dest.provideIdx(RB_INT);
                    a_dest.program.push_back(Syntop(OP_ADD,  { newaddr, op.args[1], op.args[2] }));
                    a_dest.program.push_back(Syntop(OP_LOAD, { op.args[0], newaddr}));
                }
                else
                    a_dest.program.push_back(op);
                break;
            case OP_STORE:
                if(op.size() == 3 && op.args[0].tag == Arg::IREG && op.args[1].tag == Arg::IREG && op.args[2].tag == Arg::IREG)
                {
                    Arg newaddr = op.args[0]; newaddr.idx = a_dest.provideIdx(RB_INT);
                    a_dest.program.push_back(Syntop(OP_ADD,  { newaddr, op.args[0], op.args[1] }));
                    a_dest.program.push_back(Syntop(OP_STORE, { newaddr, op.args[2] }));
                }
                else
                    a_dest.program.push_back(op);
                break;
            case OP_CMP:
                Assert(opnum + 1 < (int)a_source.program.size());
                switch(a_source.program[opnum + 1].opcode)
                {
                    case OP_JCC:
                    {
                        const Syntop& jccop = a_source.program[opnum + 1];
                        a_dest.program.push_back(Syntop(OP_JCC, { jccop.args[0], op.args[0], op.args[1], jccop.args[1]}));
                        break;
                    }
                    case OP_IVERSON:
                    {
                        const Syntop& ivop = a_source.program[opnum + 1];
                        Assert(ivop.args_size == 2 && ivop.args[0].tag == Arg::IREG && ivop.args[1].tag == Arg::IIMMEDIATE);
                        switch(ivop.args[1].value)
                        {
                            case OP_LT:
                            case OP_GT:
                            case OP_UGT:
                               a_dest.program.push_back(Syntop(OP_IVERSON, { ivop.args[0], ivop.args[1], op.args[0], op.args[1]}));
                               break;
                            case OP_LE:
                            case OP_ULE:
                            case OP_GE:
                                a_dest.program.push_back(Syntop(OP_IVERSON, { ivop.args[0], (ivop.args[1].value == OP_LE ? OP_GT :
                                                                                            ivop.args[1].value == OP_ULE ? OP_UGT :
                                                                                          /*ivop.args[1].value == OP_GE ?*/OP_LT), op.args[0], op.args[1]}));
                                a_dest.program.push_back(Syntop(OP_NEG, { ivop.args[0], ivop.args[0]}));
                                a_dest.program.push_back(Syntop(OP_ADD, { ivop.args[0], ivop.args[0], argIImm(1)}));
                                break;
                            case OP_EQ:
                            case OP_NE:
                                a_dest.program.push_back(Syntop(OP_SUB, { ivop.args[0], op.args[0], op.args[1]}));
                                a_dest.program.push_back(Syntop(OP_IVERSON, { ivop.args[0], ivop.args[1], ivop.args[0], argIImm(0)}));
                                break;
                            default:
                                throw std::runtime_error("Unsupported condition type.");
                        }
                        break;
                    }
                    default:
                        throw std::runtime_error("Unknown CMP postoperation.");
                }
                break;
            case OP_JCC: case OP_IVERSON: break; //it have to be handled in cmp option.
            case OP_MOV:
                if(op.args_size == 2 && op.args[0].tag == Arg::IREG && op.args[1].tag == Arg::IIMMEDIATE && !signed_fits(op.args[1].value, 12))
                {
                    if(signed_fits(uint64_t(op.args[1].value), 32))
                    {
                        mov32(a_dest, op.args[0], op.args[1].value);
                    }
                    else
                    {
                        uint64_t upper32 = uint64_t(op.args[1].value >> 32);
                        int64_t lower32 = (int64_t)(uint64_t(op.args[1].value) & 0xffffffff);
                        bool negative_lower32 = (lower32 & (1 << 31)) != 0;
                        if(negative_lower32)
                            lower32 = int64_t(uint64_t(lower32) | 0xffffffff00000000); //Expand to negative.
                        if(negative_lower32 && upper32 == 0x7fffffff)
                        {
                            a_dest.program.push_back(Syntop(OP_MOV, { op.args[0], argIImm(1) }));
                            a_dest.program.push_back(Syntop(OP_SHL, { op.args[0], op.args[0], argIImm(63) }));
                        }
                        else
                        {
                            if(negative_lower32)
                                upper32 += 1;
                            mov32(a_dest, op.args[0], upper32);
                            a_dest.program.push_back(Syntop(OP_SHL, { op.args[0], op.args[0], argIImm(32) }));
                        }
                        Arg lowerkeeper = op.args[0]; lowerkeeper.idx = a_dest.provideIdx(RB_INT);
                        mov32(a_dest, lowerkeeper, lower32);
                        a_dest.program.push_back(Syntop((negative_lower32 && upper32 == 0x7fffffff) ? OP_XOR : OP_ADD, { op.args[0], op.args[0], lowerkeeper }));
                    }
                }
                else
                    a_dest.program.push_back(op);
                break;
            default:
                a_dest.program.push_back(op);
                break;
            }
        }
    }

    void RiscVBRASnippets2::mov32(Syntfunc& a_dest, Arg destarg, int64_t val)
    {
        Assert(destarg.tag == Arg::IREG && signed_fits(uint64_t(val), 32));
        if(!signed_fits(uint64_t(val), 12))
        {
            uint64_t upper20 = (uint64_t(val) >> 12) & 0b11111111111111111111;
            int64_t lower12 = (int64_t)(uint64_t(val) & 0b111111111111);
            bool negative_lower12 = (lower12 & (1 << 11)) != 0;
            if(negative_lower12)
                lower12 = int64_t(uint64_t(lower12) | 0xfffffffffffff000); //Expand to negative.
            if(negative_lower12 && (upper20 == 0b01111111111111111111))
            {
                a_dest.program.push_back(Syntop(OP_RV_LUI, { destarg, argIImm(0x80000) }));
                a_dest.program.push_back(Syntop(OP_XOR, { destarg, destarg, lower12}));
            }
            else
            {
                if(negative_lower12)
                    upper20 += 1;
                a_dest.program.push_back(Syntop(OP_RV_LUI, { destarg, argIImm(upper20) }));
                a_dest.program.push_back(Syntop(OP_ADD, { destarg, destarg, argIImm(lower12)}));
            }
        }
        else
        {
            a_dest.program.push_back(Syntop(OP_MOV, { destarg, argIImm(val) }));
        }
    }

    void RiscVARASnippets::process(Syntfunc& a_dest, const Syntfunc& a_source)
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
                    a_dest.program.push_back(op);
                break;
            case OP_CALL:
            case OP_CALL_NORET:
            {
                std::vector<int> parameterRegisters = { A0, A1, A2, A3, A4, A5, A6, A7 };
                std::vector<int> returnRegisters = { A0, A1 };
                std::vector<int> callerSavedRegisters = { T1, T2, T3, T4, T5, T6 };

                std::set<int> allSaved;
                allSaved.insert(parameterRegisters.begin(), parameterRegisters.end());
                allSaved.insert(returnRegisters.begin(), returnRegisters.end());
                allSaved.insert(callerSavedRegisters.begin(), callerSavedRegisters.end());
                Assert((op.opcode == OP_CALL && op.size() >= 2 && op.size() <= ((int)parameterRegisters.size() + 2)) ||
                       (op.opcode == OP_CALL_NORET && op.size() >= 1 && op.size() <= ((int)parameterRegisters.size() + 1)));
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
                            a_dest.program.push_back(Syntop(OP_UNSPILL, { argReg(RB_INT,  regidx), argIImm(op[fargnum].idx)}));
                        brokenRegs.insert(regidx);
                    }
                }
                if(brokenRegs.find(addrkeeper.idx) != brokenRegs.end())
                {
                    addrkeeper.idx = T1;
                    a_dest.program.push_back(Syntop(OP_UNSPILL, { addrkeeper, argIImm(addrkeeper_spilled)}));
                }
                //3.) Call function
                a_dest.program.push_back(Syntop(OP_CALL_NORET, { addrkeeper }));
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
#endif // __LOOPS_ARCH == __LOOPS_RISCV
