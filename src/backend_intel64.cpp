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

    static inline BinTranslation::Token nBkb(size_t n, uint64_t bytes, size_t k, uint64_t bits)
    {
        uint64_t field = ((((uint64_t(1) << (n * 8)) - 1) & bytes) << k) | bits;
        return BinTranslation::Token(BinTranslation::Token::T_STATIC, field, n*8+k);
    }

    BinTranslation i64BTLookup(const Syntop& index, bool& scs)
    {
        //TODO(ch): A lot of commands supports immediates of different widthes(8/32/64), but there are implemented just
        //fixed sizes(in most cases = 32). For space economy, it's better to implement different cases.
        //TODO(ch): IMPORTANT: many BTspl uses 8bit form, which works only for offsets [-63, +64]. At least there needed diagnostic
        //error messages, when spill cannot fit. Next level of solution is to change form of most part of BTspl from 8bit to
        //32bit. Perfect solution is introducing adaptive encoding - 8/32/64 bit. 
        
        using namespace BinTranslationConstruction;
        scs = true;
        switch (index.opcode)
        {
        case (INTEL64_MOVSX):
            if ((index.size() != 2 && index.size() != 3) || index[0].tag != Arg::IREG || index[1].tag != Arg::IREG)
                break;
            if (index.size() == 2)
            {
                switch (index[1].flags & (AF_LOWER8))
                {
                case (AF_LOWER8):
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    static uint64_t statB[4] = { 0x480fbe, 0x4c0fbe, 0x490fbe, 0x4d0fbe };
                    return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out), BTreg(1, 3, In) });        //movsx rax, byte ptr [rcx]
                }
                case (AF_LOWER16):
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    static uint64_t statB[4] = { 0x480fbf, 0x4c0fbf, 0x490fbf, 0x4d0fbf };
                    return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out), BTreg(1, 3, In) });        //movsx rax, word ptr [rcx]
                }
                };
            }
            else if (index.size() == 3)
            {
                if (index[2].tag == Arg::IREG)
                {
                    switch (index[1].flags & (AF_LOWER8))
                    {
                    case (AF_LOWER8):
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                        static uint64_t statB[8] = { 0x480fbe, 0x4c0fbe, 0x490fbe, 0x4d0fbe, 0x4a0fbe, 0x4e0fbe, 0x4b0fbe, 0x4f0fbe };
                        return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out), BTsta(0b10000, 5), BTreg(2, 3, In), BTreg(1, 3, In) });  //movsx rax, byte ptr [rcx + rdx]
                    }
                    case (AF_LOWER16):
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                        static uint64_t statB[8] = { 0x480fbf, 0x4c0fbf, 0x490fbf, 0x4d0fbf, 0x4a0fbf, 0x4e0fbf, 0x4b0fbf, 0x4f0fbf };
                        return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out), BTsta(0b10000, 5), BTreg(2, 3, In), BTreg(1, 3, In) });  //movsx rax, byte ptr [rcx + rdx]
                    }
                    };
                }
                else if (index[2].tag == Arg::IIMMEDIATE)
                {
                    switch (index[1].flags & (AF_LOWER8))
                    {
                    case (AF_LOWER8):
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                        static uint64_t statB[4] = { 0x480fbe, 0x4c0fbe, 0x490fbe, 0x4d0fbe };
                        return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(0, 3, Out), BTreg(1, 3, In), BTimm(2, 32) });    //movsx rax, byte ptr [rcx + <offset>]
                    }
                    case (AF_LOWER16):
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                        static uint64_t statB[4] = { 0x480fbf, 0x4c0fbf, 0x490fbf, 0x4d0fbf };
                        return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(0, 3, Out), BTreg(1, 3, In), BTimm(2, 32) });     //movsx rax, word ptr [rcx + <offset>]
                    }
                    };
                }
            }
            break;
        case (INTEL64_MOVSXD):
            Assert((index.size() == 2 || index.size() == 3 ) && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG);
            if (index.size() == 2)
            {
                static uint64_t stats[4] = { 0x1218C , 0x1318C, 0x1258C, 0x1358C };
                size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                return BiT({ BTsta(stats[statn], 18), BTreg(0, 3, Out), BTreg(1, 3, In) });
            }
            else if (index.size() == 3)
            {
                if (index.args[2].tag == Arg::IREG)
                {
                    static uint64_t stats[8] = { 0x1218C, 0x1318C, 0x1258C, 0x1358C, 0x1298C, 0x1398C, 0x12D8C, 0x13D8C };
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                    return BiT({ BTsta(stats[statn], 18), BTreg(0, 3, Out), BTsta(0x10, 5), BTreg(2, 3, In), BTreg(1, 3, In) });
                }
                else if (index.args[2].tag == Arg::IIMMEDIATE)
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    static uint64_t statB[4] = { 0x4863, 0x4c63, 0x4963, 0x4d63 };
                    return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(0, 3, Out), BTreg(1, 3, In), BTimm(2, 32) });     //movsxd rax, dword ptr [rcx + <offset>]
                }
            }
            break;

        case (INTEL64_MOVZX):
            if ((index.size() != 2 && index.size() != 3) || index[0].tag != Arg::IREG || index[1].tag != Arg::IREG)
                break;
            if (index.size() == 2)
            {
                switch (index[1].flags & (AF_LOWER8))
                {
                case (AF_LOWER8):
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    static uint64_t statB[4] = { 0x480fb6, 0x4c0fb6, 0x490fb6, 0x4d0fb6 };
                    return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out), BTreg(1, 3, In) });        //movzx rax, byte ptr [rcx]
                }
                case (AF_LOWER16):
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    static uint64_t statB[4] = { 0x480fb7, 0x4c0fb7, 0x490fb7, 0x4d0fb7 };
                    return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out), BTreg(1, 3, In) });        //movzx rax, word ptr [rcx]
                }
                };
            }
            else if (index.size() == 3)
            {
                if (index[2].tag == Arg::IREG)
                {
                    switch (index[1].flags & (AF_LOWER8))
                    {
                    case (AF_LOWER8):
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                        static uint64_t statB[8] = { 0x480fb6, 0x4c0fb6, 0x490fb6, 0x4d0fb6, 0x4a0fb6, 0x4e0fb6, 0x4b0fb6, 0x4f0fb6 };
                        return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out), BTsta(0b10000, 5), BTreg(2, 3, In), BTreg(1, 3, In) });  //movzx rax, byte ptr [rcx + rdx]
                    }
                    case (AF_LOWER16):
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                        static uint64_t statB[8] = { 0x480fb7, 0x4c0fb7, 0x490fb7, 0x4d0fb7, 0x4a0fb7, 0x4e0fb7, 0x4b0fb7, 0x4f0fb7 };
                        return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(0, 3, Out), BTsta(0b10000, 5), BTreg(2, 3, In), BTreg(1, 3, In) });  //movzx rax, byte ptr [rcx + rdx]
                    }
                    };
                }
                else if (index[2].tag == Arg::IIMMEDIATE)
                {
                    switch (index[1].flags & (AF_LOWER8))
                    {
                    case (AF_LOWER8):
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                        static uint64_t statB[4] = { 0x480fb6, 0x4c0fb6, 0x490fb6, 0x4d0fb6 };
                        return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(0, 3, Out), BTreg(1, 3, In), BTimm(2, 32) });    //movzx rax, byte ptr [rcx + <offset>]
                    }
                    case (AF_LOWER16):
                    {
                        size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                        static uint64_t statB[4] = { 0x480fb7, 0x4c0fb7, 0x490fb7, 0x4d0fb7 };
                        return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(0, 3, Out), BTreg(1, 3, In), BTimm(2, 32) });     //movzx rax, word ptr [rcx + <offset>]
                    }
                    };
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
                            switch (index[1].flags & (AF_LOWER8))
                            {
                            case (AF_LOWER8):
                                if (index[0].idx == R12 || index[0].idx == R13) //mov [r12/r13], bx
                                {
                                    return BiT({ nBkb(2, ((index[1].idx < 8) ? 0x4188 : 0x4588), 2, (index[0].idx == R12 ? 0 : 1)), BTreg(1, 3, In), BTreg(0, 3, In), BTsta((index[0].idx == R12 ? 0x24 : 0), 8) });
                                }
                                else if (index[0].idx < 8 && index[1].idx >= 4 && index[1].idx < 8)
                                {
                                    return BiT({ nBkb(2, 0x4088, 2, 0), BTreg(1, 3, In), BTreg(0, 3, In) }); //mov [rax], dil
                                }
                                else
                                {
                                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                    static uint64_t statB[4] = { 0x88, 0x4188, 0x4488, 0x4588 };
                                    static uint64_t statBn[4] = { 1, 2, 2, 2 };
                                    return BiT({ nBkb(statBn[statn], statB[statn], 2, 0), BTreg(1, 3, In), BTreg(0, 3, In) }); //mov [rax], bx
                                }
                            case (AF_LOWER16):
                                if (index[0].idx == R12 || index[0].idx == R13) //mov [r12/r13], bx
                                {
                                    return BiT({ nBkb(3, ((index[1].idx < 8) ? 0x664189 : 0x664589), 2, (index[0].idx == R12 ? 0 : 1)), BTreg(1, 3, In), BTreg(0, 3, In), BTsta((index[0].idx == R12 ? 0x24 : 0), 8) });
                                }
                                else
                                {
                                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                    static uint64_t statB[4] = { 0x6689, 0x664189, 0x664489, 0x664589 };
                                    static uint64_t statBn[4] = { 2, 3, 3, 3 };
                                    return BiT({ nBkb(statBn[statn], statB[statn], 2, 0), BTreg(1, 3, In), BTreg(0, 3, In) }); //mov [rax], bx
                                }
                            case (AF_LOWER32):
                            {
                                uint64_t stat = 0x224;
                                if (index[0].idx == R12 || index[0].idx == R13) //mov [r12/r13], ebx
                                {
                                    stat = index[0].idx == R13 ? (index[1].idx < 8 ? 0x10625 : 0x11625) : (index[1].idx < 8 ? 0x10624 : 0x11624);
                                    return BiT({ BTsta(stat, 18), BTreg(1, 3, In), BTreg(0, 3, In) , BTsta(index[0].idx == R13 ? 0 : 0x24, 8) });
                                }
                                else
                                {
                                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                    static uint64_t stats[4] = { 0x224, 0x10624, 0x11224, 0x11624 };
                                    static uint64_t statw[4] = { 10, 18, 18, 18 };
                                    return BiT({ BTsta(stats[statn], statw[statn]), BTreg(1, 3, In), BTreg(0, 3, In) });
                                }
                            }
                            case (0):
                            {
                                size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                static uint64_t statB[4] = { 0x4889, 0x4989, 0x4c89, 0x4d89 };
                                if (index[0].idx == R12 || index[0].idx == R13) //mov [r12/r13], rbx
                                    return BiT({ nBkb(2, statB[statn], 2, index[0].idx == R13 ? 0b01 : 0b00), BTreg(1, 3, In), BTreg(0, 3, In), BTsta((index[0].idx == R12 ? 0x24 : 0), 8) });
                                else
                                    return BiT({ nBkb(2, statB[statn], 2, 0b00), BTreg(1, 3, In), BTreg(0, 3, In) }); //mov [rax], rbx
                            }
                            };
                        }
                        else if (index[1].flags & AF_ADDRESS)
                        {
                            switch (index[0].flags & (AF_LOWER8))
                            {
                                case (AF_LOWER32):
                                {
                                    static uint64_t statB[4] = { 0x8b, 0x448b, 0x418b, 0x458b };
                                    static size_t statBn[4] = { 1, 2, 2, 2 };
                                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                    return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b00), BTreg(0, 3, Out), BTreg(1, 3, In) }); //mov eax, [rbx]
                                }
                                case (0):
                                {
                                    static uint64_t statB[4] = { 0x488b, 0x4c8b, 0x498b, 0x4d8b };
                                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                    return BiT({ nBkb(2, statB[statn], 2, 0b00), BTreg(0, 3, Out), BTreg(1, 3, In) }); //mov rax, [rbx]
                                }
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
                        if (index[0].flags & AF_ADDRESS)
                        {
                            switch (index[1].flags & (AF_LOWER8))
                            {
                            case (AF_LOWER8):
                                if (index[0].idx == R12 || index[0].idx == R13) //mov byte ptr [r12/r13], <imm>
                                    return  BiT({ nBkb(2, 0x41c6, 5, index[0].idx == R12 ? 0 : 0b1000), BTreg(0, 3, In), BTsta(index[0].idx == R12 ? 0b00100100 : 0, 8), BTimm(1, 8) });
                                else
                                    return  BiT({ index[0].idx < 8 ? nBkb(1, 0xc6, 5, 0) : nBkb(2, 0x41c6, 5, 0), BTreg(0, 3, In), BTimm(1, 8) });//mov byte ptr [rax], <imm>
                            case (AF_LOWER16):
                                if (index[0].idx == R12 || index[0].idx == R13) //mov word ptr [r12/r13], <imm>
                                    return  BiT({ nBkb(3, 0x6641c7, 5, index[0].idx == R12 ? 0 : 0b1000), BTreg(0, 3, In), BTsta(index[0].idx == R12 ? 0b00100100 : 0, 8), BTimm(1, 16) });
                                else
                                    return  BiT({ index[0].idx < 8 ? nBkb(2, 0x66c7, 5, 0) : nBkb(3, 0x6641c7, 5, 0), BTreg(0, 3, In), BTimm(1, 16) });//mov word ptr [rax], <imm>
                            case (AF_LOWER32):
                                if (index[0].idx == R12 || index[0].idx == R13) //mov dword ptr [r12/r13], <imm>
                                    return  BiT({ nBkb(2, 0x41c7, 5, index[0].idx == R12 ? 0 : 0b1000), BTreg(0, 3, In), BTsta(index[0].idx == R12 ? 0b00100100 : 0, 8), BTimm(1, 32) });
                                else
                                    return  BiT({ index[0].idx < 8 ? nBkb(1, 0xc7, 5, 0) : nBkb(2, 0x41c7, 5, 0), BTreg(0, 3, In), BTimm(1, 32) });//mov dword ptr [rax], <imm>
                            case (0):
                                if (index[0].idx == R12 || index[0].idx == R13) //mov qword ptr [r12/r13], <imm>
                                    return  BiT({ nBkb(2, 0x49c7, 5, index[0].idx == R12 ? 0 : 0b1000), BTreg(0, 3, In), BTsta(index[0].idx == R12 ? 0b00100100 : 0, 8), BTimm(1, 32) });
                                else
                                    return  BiT({ index[0].idx < 8 ? nBkb(2, 0x48c7, 5, 0) : nBkb(2, 0x49c7, 5, 0), BTreg(0, 3, In), BTimm(1, 32) });//mov qword ptr [rax], <imm>
                            };
                        }
                        else
                            return  BiT({ BTsta(index[0].idx < 8 ? 0x918F8 : 0x938F8, 21), BTreg(0, 3, Out), BTimm(1, 32) });//mov rax, <imm>
                    }
                }
                else if (index[0].tag == Arg::ISPILLED)
                {
                    if (index[1].tag == Arg::IREG)
                        return BiT({ BTsta(index[1].idx < 8 ? 0x12225 : 0x13225, 18), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 8) });   //mov [rsp + offset], rbx
                    else if (index[1].tag == Arg::IIMMEDIATE)
                        return BiT({ BTsta(0x48c74424, 32), BTspl(0, 8), BTimm(1, 32) });  //mov QWORD PTR [rsp + offset], <imm>
                }
            }
            else if (index.size() == 3)
            {
                if (index[0].tag != Arg::IREG)
                    break;
                if ((index[0].flags & AF_ADDRESS) == 0)
                {
                    if (index[1].tag != Arg::IREG || (index[1].flags & AF_ADDRESS) == 0 || (index[2].flags & AF_ADDRESS) == 0)
                        break;
                    if (index[2].tag == Arg::IREG)
                    {
                        switch (index[0].flags & (AF_LOWER8))
                        {
                        case (AF_LOWER32):
                        {
                            static uint64_t statBn[8] = { 1, 2, 2, 2, 2, 2, 2, 2 };
                            static uint64_t statB[8] = { 0x8b, 0x448b, 0x418b, 0x458b, 0x428b, 0x468b, 0x438b, 0x478b };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                            return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b00), BTreg(0, 3, Out), BTsta(0b10000, 5), BTreg(2, 3, In), BTreg(1, 3, In)});  //mov eax, [rcx + rdx]
                        }
                        case (0):
                        {
                            static uint64_t statB[8] = { 0x488b, 0x4c8b, 0x498b, 0x4d8b, 0x4a8b, 0x4e8b, 0x4b8b, 0x4f8b };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                            return BiT({ nBkb(2, statB[statn], 2, 0b00), BTreg(0, 3, Out), BTsta(0b10000, 5), BTreg(2, 3, In), BTreg(1, 3, In) });  //mov rax, [rcx + rdx]
                        }
                        };
                    }
                    else if (index[2].tag == Arg::IIMMEDIATE)
                    {
                        switch (index[0].flags & (AF_LOWER8))
                        {
                        case (AF_LOWER32):
                        {
                            static uint64_t statB[4] = { 0x8b, 0x448b, 0x418b, 0x458b };
                            static uint64_t statBn[4] = { 1, 2, 2, 2 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b10), BTreg(0, 3, Out), BTreg(1, 3, In), BTimm(2, 32)});  //mov eax, [rcx + <offset>]
                        }
                        case (0):
                        {
                            static uint64_t statB[4] = { 0x488b, 0x4c8b, 0x498b, 0x4d8b };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(0, 3, Out), BTreg(1, 3, In), BTimm(2, 32) });  //mov rax, [rcx + <offset>]
                        }
                        };
                    }
                }
                else if (index[1].tag == Arg::IREG && (index[1].flags & AF_ADDRESS))
                {
                    if (index[2].tag == Arg::IREG)
                    {
                        switch (index[2].flags & (AF_LOWER8))
                        {
                        case (AF_LOWER8):
                        {
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                            static uint64_t statB[8] = { 0x88, 0x4188, 0x4288, 0x4388, 0x4488, 0x4588, 0x4688, 0x4788 };
                            static uint64_t statBn[8] = { 1, 2, 2, 2, 2, 2, 2, 2 };
                            if (index[0].idx == R13)
                                return BiT({ nBkb(2, statB[statn], 2, 1), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In), BTreg(0, 3, In), BTsta(0, 8) }); //mov [r13 + rbx], cl
                            else if (index[0].idx < 8 && index[2].idx >= 4 && index[2].idx < 8)
                            {
                                uint64_t stat = statn == 0 ? 0x4088 : statB[statn];
                                return BiT({ nBkb(2, stat, 2, 0), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In), BTreg(0, 3, In) }); //mov [rax + rbx], dil
                            }
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In), BTreg(0, 3, In) }); //mov [rax + rbx], cl
                        }
                        case (AF_LOWER16):
                        {
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                            static uint64_t statB[8] = { 0x6689, 0x664189, 0x664289, 0x664389, 0x664489, 0x664589, 0x664689, 0x664789 };
                            static uint64_t statBn[8] = { 2, 3, 3, 3, 3, 3, 3, 3 };
                            if (index[0].idx == R13)
                                return BiT({ nBkb(3, statB[statn], 2, 1), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In), BTreg(0, 3, In), BTsta(0, 8) }); //mov [r13 + rbx], cx
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In), BTreg(0, 3, In) }); //mov [rax + rbx], cx
                        }
                        case (AF_LOWER32):
                        {
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                            static uint64_t statB[8] = { 0x89, 0x4189, 0x4289, 0x4389, 0x4489, 0x4589, 0x4689, 0x4789 };
                            static uint64_t statBn[8] = { 1, 2, 2, 2, 2, 2, 2, 2 };
                            if (index[0].idx == R13)
                                return BiT({ nBkb(2, statB[statn], 2, 1), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In), BTreg(0, 3, In), BTsta(0, 8) }); //mov [r13 + rbx], ecx
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In), BTreg(0, 3, In) }); //mov [rax + rbx], ecx
                        }
                        case (0):
                        {
                            static uint64_t statB[8] = { 0x4889, 0x4989, 0x4a89, 0x4b89, 0x4c89, 0x4d89, 0x4e89, 0x4f89 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                            return BiT({ nBkb(2, statB[statn], 2, 0), BTreg(2, 3, In), BTsta(0b10000, 5), BTreg(1, 3, In), BTreg(0, 3, In) });  //mov [rax + rbx], rcx
                        }
                        };
                    }
                    else if (index[2].tag == Arg::IIMMEDIATE)
                    {
                        switch (index[2].flags & (AF_LOWER8))
                        {
                        case (AF_LOWER8):
                        {
                            static uint64_t statB[4] = { 0xc604, 0x41c604, 0x42c604, 0x43c604 };
                            static uint64_t statBn[4] = { 2, 3, 3, 3 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            if (index[0].idx == R13)
                                return BiT({ nBkb(3, (index[1].idx < 8) ? 0x41c644 : 0x43c644, 2, 0b00), BTreg(1, 3, In), BTreg(0, 3, In), BTsta(0, 8), BTimm(2, 8) }); //mov byte ptr [r13 + rcx], <imm>
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b00), BTreg(1, 3, In), BTreg(0, 3, In), BTimm(2, 8) });                 //mov byte ptr [rax + rcx], <imm>
                        }
                        case (AF_LOWER16):
                        {
                            static uint64_t statB[4] = { 0x66c704, 0x6641c704, 0x6642c704, 0x6643c704 };
                            static uint64_t statBn[4] = { 3, 4, 4, 4 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            if (index[0].idx == R13)
                                return BiT({ nBkb(4, (index[1].idx < 8) ? 0x6641c744 : 0x6643c744, 2, 0b00), BTreg(1, 3, In), BTreg(0, 3, In), BTsta(0, 8), BTimm(2, 16) }); //mov word ptr [r13 + rcx], <imm>
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b00), BTreg(1, 3, In), BTreg(0, 3, In), BTimm(2, 16) });                     //mov word ptr [rax + rcx], <imm>
                        }
                        case (AF_LOWER32):
                        {
                            static uint64_t statB[4] = { 0xC704, 0x41C704, 0x42C704, 0x43C704 };
                            static uint64_t statBn[4] = { 2, 3, 3, 3 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            if (index[0].idx == R13)
                                return BiT({ nBkb(3, (index[1].idx < 8) ? 0x41c744 : 0x43c744, 2, 0b00), BTreg(1, 3, In), BTreg(0, 3, In), BTsta(0, 8), BTimm(2, 32) }); //mov dword ptr [r13 + rcx], <imm>
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b00), BTreg(1, 3, In), BTreg(0, 3, In), BTimm(2, 32) });                 //mov dword ptr [rax + rcx], <imm>
                        }
                        case (0):
                        {
                            static uint64_t statB[4] = { 0x48c704, 0x49c704, 0x4ac704, 0x4bc704 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                            if (index[0].idx == R13)
                                return BiT({ nBkb(3, (index[1].idx < 8) ? 0x49c744 : 0x4bc744, 2, 0b00), BTreg(1, 3, In), BTreg(0, 3, In), BTsta(0, 8), BTimm(2, 32) }); //mov qword ptr [r13 + rcx], <imm>
                            else
                                return BiT({ nBkb(3, statB[statn], 2, 0b00), BTreg(1, 3, In), BTreg(0, 3, In), BTimm(2, 32) });                                 //mov qword ptr [rax + rcx], <imm>
                        }
                        }
                    };
                }
                else if (index[1].tag == Arg::IIMMEDIATE && (index[1].flags & AF_ADDRESS))
                {
                    if (index[2].tag == Arg::IREG)
                    {
                        switch (index[2].flags & (AF_LOWER8))
                        {
                        case (AF_LOWER8):
                        {
                            static uint64_t statB[4] = { 0x88, 0x4188, 0x4488, 0x4588 };
                            static uint64_t statBn[4] = { 1, 2, 2, 2 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[2].idx < 8) ? 0 : 2);
                            if (index[0].idx == R12)
                                return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In), BTsta(0x24, 8), BTimm(1, 32) });//mov [r12 + <offset>], cl
                            else if (index[0].idx < 8 && index[2].idx >= 4 && index[2].idx < 8)
                                return BiT({ nBkb(2, 0x4088, 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In), BTimm(1, 32) });                      //mov [rax + <offset>], dil
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In), BTimm(1, 32) });    //mov [rax + <offset>], cl
                        }
                        case (AF_LOWER16):
                        {
                            static uint64_t statB[4] = { 0x6689, 0x664189, 0x664489, 0x664589 };
                            static uint64_t statBn[4] = { 2, 3, 3, 3 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[2].idx < 8) ? 0 : 2);
                            if (index[0].idx == R12)
                                return BiT({ nBkb(3, statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In), BTsta(0x24, 8), BTimm(1, 32) });//mov [r12 + <offset>], cx
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In), BTimm(1, 32) });    //mov [rax + <offset>], cx
                        }
                        case (AF_LOWER32):
                        {
                            static uint64_t statB[4] = { 0x89, 0x4189, 0x4489, 0x4589 };
                            static uint64_t statBn[4] = { 1, 2, 2, 2 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[2].idx < 8) ? 0 : 2);
                            if (index[0].idx == R12)
                                return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In), BTsta(0x24, 8), BTimm(1, 32) });//mov [r12 + <offset>], ecx
                            else
                                return BiT({ nBkb(statBn[statn], statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In), BTimm(1, 32) });     //mov [rax + <offset>], ecx
                        }
                        case (0):
                        {
                            static uint64_t statB[4] = { 0x4889, 0x4989, 0x4c89, 0x4d89 };
                            size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[2].idx < 8) ? 0 : 2);
                            if (index[0].idx == R12)
                                return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In), BTsta(0x24, 8), BTimm(1, 32) });//mov [r12 + <offset>], rcx
                            else
                                return BiT({ nBkb(2, statB[statn], 2, 0b10), BTreg(2, 3, In), BTreg(0, 3, In), BTimm(1, 32) });                //mov [rax + <offset>], rcx
                        }
                        }
                    }
                    else if (index[2].tag == Arg::IIMMEDIATE)
                    {
                        switch (index[2].flags & (AF_LOWER8))
                        {
                        case (AF_LOWER8):
                        {
                            uint64_t stat = ((index[0].idx < 8) ? 0xc6 : 0x41c6);
                            uint64_t statw = ((index[0].idx < 8) ? 1 : 2);
                            if (index[0].idx == R12)
                                return BiT({ nBkb(2, stat, 5, 0b10000), BTreg(0, 3, In), BTsta(0x24, 8),BTimm(1, 32), BTimm(2, 8) });//mov word ptr [r12 + <offset>], <imm>    
                            else
                                return BiT({ nBkb(statw, stat, 5, 0b10000), BTreg(0, 3, In), BTimm(1, 32), BTimm(2, 8) });           //mov word ptr [rax + <offset>], <imm>
                        }
                        case (AF_LOWER16):
                        {
                            uint64_t stat = ((index[0].idx < 8) ? 0x66c7 : 0x6641c7);
                            uint64_t statw = ((index[0].idx < 8) ? 2 : 3);
                            if (index[0].idx == R12)
                                return BiT({ nBkb(3, stat, 5, 0b10000), BTreg(0, 3, In), BTsta(0x24, 8),BTimm(1, 32), BTimm(2, 16) });//mov word ptr [r12 + <offset>], <imm>    
                            else
                                return BiT({ nBkb(statw, stat, 5, 0b10000), BTreg(0, 3, In), BTimm(1, 32), BTimm(2, 16) });           //mov word ptr [rax + <offset>], <imm>
                        }
                        case (AF_LOWER32):
                        {
                            uint64_t stat = ((index[0].idx < 8) ? 0xc7 : 0x41c7);
                            uint64_t statw = ((index[0].idx < 8) ? 1 : 2);
                            if (index[0].idx == R12)
                                return BiT({ nBkb(2, stat, 5, 0b10000), BTreg(0, 3, In), BTsta(0x24, 8),BTimm(1, 32), BTimm(2, 32) });//mov dword ptr [r12 + <offset>], <imm>    
                            else
                                return BiT({ nBkb(statw, stat, 5, 0b10000), BTreg(0, 3, In), BTimm(1, 32), BTimm(2, 32) });           //mov dword ptr [rax + <offset>], <imm>
                        }
                        case (0):
                        {
                            uint64_t stat = ((index[0].idx < 8) ? 0x48c7: 0x49c7);
                            if (index[0].idx == R12)
                                return BiT({ nBkb(2, stat, 5, 0b10000), BTreg(0, 3, In), BTsta(0x24, 8 ),BTimm(1, 32), BTimm(2, 32) });//mov qword ptr [r12 + <offset>], <imm>    
                            else
                                return BiT({ nBkb(2, stat, 5, 0b10000), BTreg(0, 3, In), BTimm(1, 32), BTimm(2, 32) });                //mov qword ptr [rax + <offset>], <imm>
                        }
                        };
                    }
                }
            }
            break;
        case (INTEL64_ADD):
            Assert(index.size() == 2);
            if (index[0].tag == Arg::IREG)
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
                    return BiT({ (index[0].idx < 8) ? nBkb(2,0x4883,5,0b11000) : nBkb(2,0x4983,5,0b11000), BTreg(0, 3, In | Out), BTimm(1, 8) });
            }
            else if (index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ BTsta(index[1].idx < 8 ? 0x12006 : 0x13006, 18), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 32) });   //add [rsp + offset], rbx
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x48818424, 32), BTspl(0, 32), BTimm(1, 32) });  //add QWORD PTR [rsp + offset], <imm>
            }
            break;
        case (INTEL64_ADC):
            Assert(index.size() == 2);
            if (index[0].tag == Arg::IREG)
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
            else if (index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ nBkb(2, index[1].idx < 8 ? 0x4811 : 0x4c11, 2, 0b10), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 32) });    //adc [rsp + offset], rbx
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x48819424, 32), BTspl(0, 32), BTimm(1, 32) });  //adc QWORD PTR [rsp + offset], <imm>
            }
            break;

        case (INTEL64_SUB):
            Assert(index.size() == 2);
            if (index[0].tag == Arg::IREG)
            {
                if (index[1].tag == Arg::IREG)
                {
                    static uint64_t stats[4] = { 0x120A7, 0x124A7, 0x130A7, 0x134A7 };
                    size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                    return BiT({ BTsta(stats[statn], 18), BTreg(1, 3, In), BTreg(0, 3, In | Out) });           //sub rax, rbx
                }
                else if (index[1].tag == Arg::ISPILLED)
                    return BiT({ BTsta(index[0].idx < 8 ? 0x120AD : 0x130AD, 18), BTreg(0, 3, In), BTsta(0x424, 11), BTspl(1, 8) }); //sub rax, [rsp + offset]
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(index[0].idx < 8 ? 0x9107D : 0x9307D, 21), BTreg(0, 3, Out), BTimm(1, 8) });//sub rax, <imm>
            }
            else if (index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ BTsta(index[1].idx < 8 ? 0x120A5 : 0x130A5, 18), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 8) });   //sub [rsp + offset], rbx
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x48836c24, 32), BTspl(0, 8), BTimm(1, 8) });  //sub QWORD PTR [rsp + offset], <imm>
            }
            break;
        case (INTEL64_IMUL):
            Assert(index.size() == 2 && index[0].tag == Arg::IREG);
            if (index[1].tag == Arg::IREG)
            {
                static uint64_t stats[4] = { 0x1203EBF, 0x1303EBF, 0x1243EBF, 0x1343EBF };
                size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                return BiT({ BTsta(stats[statn], 26), BTreg(0, 3, In | Out), BTreg(1, 3, In) });
            }
            else if(index[1].tag == Arg::ISPILLED)
                return BiT({ BTsta(index[0].idx < 8 ? 0x1203EBD : 0x1303EBD, 26), BTreg(0, 3, In | Out), BTsta(0x424, 11), BTspl(1, 8) });
            else if (index[1].tag == Arg::IIMMEDIATE)
                return BiT({ index[0].idx < 8 ? nBkb(2, 0x486b, 2, 0b11) : nBkb(2, 0x4d6b, 2, 0b11), BTreg(0, 3, In | Out) , BTreg(0, 3, In | Out), BTimm(1, 8) });
            break;
        case (INTEL64_IDIV):
            Assert(index.size() == 1);
            if (index[0].tag == Arg::IREG)
                return BiT({ BTsta(index[0].idx < 8 ? 0x91EFF : 0x93EFF, 21), BTreg(0, 3, In) });
            else if(index[0].tag == Arg::ISPILLED)
                return BiT({ BTsta(0x48f77c24, 32), BTspl(0, 8) });
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
                else if (index[1].tag == Arg::IREG && index[1].idx == RCX && (index[1].flags & AF_LOWER8) == AF_LOWER8)
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
                else if (index[1].tag == Arg::IREG && index[1].idx == RCX && (index[1].flags & AF_LOWER8) == AF_LOWER8)
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
                else if (index[1].tag == Arg::IREG && index[1].idx == RCX && (index[1].flags & AF_LOWER8) == AF_LOWER8)
                {
                    if (index[0].tag == Arg::IREG)
                        return BiT({ nBkb(2, index[0].idx < 8 ? 0x48d3 : 0x49d3, 5, 0b11111), BTreg(0, 3, In | Out), BTreg(1, 0, In) });
                    else if (index[0].tag == Arg::ISPILLED)
                        return BiT({ BTsta(0x48d3bc24, 32), BTspl(0, 32), BTreg(1, 0, In) });
                }
            }
            break;
        case (INTEL64_AND):
            Assert(index.size() == 2);
            if (index[0].tag == Arg::IREG)
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
            else if (index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ index[1].idx < 8 ? nBkb(2, 0x4821, 2, 0b10) : nBkb(2, 0x4c21, 2, 0b10), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 32) });
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x4881a424, 32), BTspl(0, 32), BTimm(1, 32) });
            }
        break;
        case (INTEL64_OR):
            Assert(index.size() == 2);
            if (index[0].tag == Arg::IREG)
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
            else if (index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ index[1].idx < 8 ? nBkb(2, 0x4809, 2, 0b10) : nBkb(2, 0x4c09, 2, 0b10), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 32) });
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x48818c24, 32), BTspl(0, 32), BTimm(1, 32) });
            }
            break;
        case (INTEL64_XOR):
            Assert(index.size() == 2);
            if (index[0].tag == Arg::IREG)
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
            Assert(index.size() == 1);
            
            if (index[0].tag == Arg::IREG)
                return BiT({ (index[0].idx < 8) ? nBkb(2, 0x48f7, 5, 0b11010) : nBkb(2, 0x49f7, 5, 0b11010), BTreg(0, 3, In | Out) });
            else if(index[0].tag == Arg::ISPILLED)
                return BiT({ BTsta(0x48f79424, 32), BTspl(0, 32) });
            break;
        case (INTEL64_NEG):
            Assert(index.size() == 1);
            if (index[0].tag == Arg::IREG)
                return BiT({ BTsta((index[0].idx < 8) ? 0x91EFB : 0x93EFB, 21), BTreg(0, 3, In | Out) });
            else if(index[0].tag == Arg::ISPILLED)
                return BiT({ BTsta(0x48f75c24, 32), BTspl(0, 8) });
            break;
        case (INTEL64_CMP):
            Assert(index.size() == 2);
            if (index[0].tag == Arg::IREG)
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
            else if (index[0].tag == Arg::ISPILLED)
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
                    size_t n = index[0].idx < 4 ? 2 : 3;
                    return BiT({ nBkb(n, stat, 5, 0b11000), BTreg(0, 3, Out) });
                }
                else if (index[0].tag == Arg::ISPILLED)
                {
                    size_t statn = ((index[0].idx < 8) ? 0 : 1);
                    stat |= regbytes[statn];
                    return BiT({ BTsta(0x0F948424, 32),  BTspl(0, 32) });
                }
            }
            break;
        case (INTEL64_CQO): return BiT({ BTsta(0x4899, 16) });
        case (INTEL64_XCHG):
        {
            if (index.size() != 2)
                break;
            const bool rax0 = (index[0].tag == Arg::IREG && index[0].idx == RAX);
            const bool rax1 = (index[0].tag == Arg::IREG && index[0].idx == RAX);
            const size_t rrd0 = rax0 ? 0 : 1;
            const size_t rrd1 = rax0 ? 1 : 0;
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
                const size_t rrd0 = index[0].tag == Arg::ISPILLED ? 1 : 0;
                const size_t rrd1 = index[0].tag == Arg::ISPILLED ? 0 : 1;
                return BiT({ nBkb(2, index[rrd0].idx < 8 ? 0x4887 : 0x4c87, 2, 0b10), BTreg(rrd0, 3, IO), BTsta(0x424,11), BTspl(rrd1, 32) });
            }
            break;
        }
        case (INTEL64_JMP): return BiT({ BTsta(0xE9,8), BToff(0, 32) });
        case (INTEL64_JNE): return BiT({ BTsta(0xf85,16), BToff(0, 32) });
        case (INTEL64_JE):  return BiT({ BTsta(0xf84,16), BToff(0, 32) });
        case (INTEL64_JL):  return BiT({ BTsta(0xf8c,16), BToff(0, 32) });
        case (INTEL64_JLE): return BiT({ BTsta(0xf8e,16), BToff(0, 32) });
        case (INTEL64_JG):  return BiT({ BTsta(0xf8f,16), BToff(0, 32) });
        case (INTEL64_JGE): return BiT({ BTsta(0xf8d,16), BToff(0, 32) });
        case (INTEL64_RET): return BiT({ BTsta(0xC3, 8) });
        default:
            break;
        }
        scs = false;
        return BinTranslation();
    }

    SyntopTranslation i64STLookup(const Backend* backend, const Syntop& index, bool& scs)
    {
        using namespace SyntopTranslationConstruction;
        scs = true;
        switch (index.opcode)
        {
        case(OP_LOAD):
            Assert(index.size() > 1);
            if (index.size() == 2)
            {
                switch (index[0].elemtype)
                {
                case (TYPE_I8):  return SyT(INTEL64_MOVSX, { SAcop(0), SAcop(1, AF_LOWER8) });
                case (TYPE_U8):  return SyT(INTEL64_MOVZX, { SAcop(0), SAcop(1, AF_LOWER8) });
                case (TYPE_I16): return SyT(INTEL64_MOVSX, { SAcop(0), SAcop(1, AF_LOWER16) });
                case (TYPE_U16): return SyT(INTEL64_MOVZX, { SAcop(0), SAcop(1, AF_LOWER16) });
                case (TYPE_I32): return SyT(INTEL64_MOVSXD,{ SAcop(0), SAcop(1) });
                case (TYPE_U32): return SyT(INTEL64_MOV,   { SAcop(0, AF_LOWER32), SAcop(1, AF_ADDRESS) });
                case (TYPE_I64):
                case (TYPE_U64): return SyT(INTEL64_MOV, { SAcop(0), SAcop(1, AF_ADDRESS) });
                default: break;
                }
            }
            else if (index.size() == 3)
            {
                switch (index[0].elemtype)
                {
                case (TYPE_I8):  return SyT(INTEL64_MOVSX, { SAcop(0), SAcop(1, AF_LOWER8), SAcop(2) });
                case (TYPE_U8):  return SyT(INTEL64_MOVZX, { SAcop(0), SAcop(1, AF_LOWER8), SAcop(2) });
                case (TYPE_I16): return SyT(INTEL64_MOVSX, { SAcop(0), SAcop(1, AF_LOWER16), SAcop(2) });
                case (TYPE_U16): return SyT(INTEL64_MOVZX, { SAcop(0), SAcop(1, AF_LOWER16), SAcop(2) });
                case (TYPE_I32): return SyT(INTEL64_MOVSXD, { SAcop(0), SAcop(1), SAcop(2) });
                case (TYPE_U32): return SyT(INTEL64_MOV, { SAcop(0, AF_LOWER32), SAcop(1, AF_ADDRESS), SAcop(2, AF_ADDRESS) });
                case (TYPE_I64):
                case (TYPE_U64): return SyT(INTEL64_MOV, { SAcop(0), SAcop(1, AF_ADDRESS), SAcop(2, AF_ADDRESS) });
                default: break;
                }
            }
            break;
        case (OP_STORE):
            if (index.size() == 2)
            {
                switch (index[1].elemtype)
                {
                case (TYPE_I8):
                case (TYPE_U8): return SyT(INTEL64_MOV, { SAcop(0, AF_ADDRESS), SAcop(1, AF_LOWER8) });
                case (TYPE_I16):
                case (TYPE_U16): return SyT(INTEL64_MOV, { SAcop(0, AF_ADDRESS), SAcop(1, AF_LOWER16) });
                case (TYPE_I32): 
                case (TYPE_U32): return SyT(INTEL64_MOV, { SAcop(0, AF_ADDRESS), SAcop(1, AF_LOWER32) });
                case (TYPE_I64): 
                case (TYPE_U64): return SyT(INTEL64_MOV, { SAcop(0, AF_ADDRESS), SAcop(1) });
                default: break;
                }
            }
            else if (index.size() == 3)
            {
                switch (index[2].elemtype)
                {
                case (TYPE_I8):
                case (TYPE_U8): return SyT(INTEL64_MOV, { SAcop(0, AF_ADDRESS), SAcop(1, AF_ADDRESS), SAcop(2, AF_LOWER8) });
                case (TYPE_I16):
                case (TYPE_U16): return SyT(INTEL64_MOV, { SAcop(0, AF_ADDRESS), SAcop(1, AF_ADDRESS), SAcop(2, AF_LOWER16) });
                case (TYPE_I32):
                case (TYPE_U32): return SyT(INTEL64_MOV, { SAcop(0, AF_ADDRESS), SAcop(1, AF_ADDRESS), SAcop(2, AF_LOWER32) });
                case (TYPE_I64):
                case (TYPE_U64): return SyT(INTEL64_MOV, { SAcop(0, AF_ADDRESS), SAcop(1, AF_ADDRESS), SAcop(2) });
                default: break;
                }
            }
            break;
        case (OP_MOV):
            if(index.size() == 2)
            {
                //TODO(ch): This trick (mov ax, 0 -> xor ax, ax) must be done in different place and, obviously not in architecture-dependent code.
                if(index[0].tag == Arg::IREG && index[1].tag == Arg::IIMMEDIATE && index[1].value == 0) 
                    return SyT(INTEL64_XOR,  { SAcop(0), SAcop(0) });
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
        case (OP_SHL):     return SyT(INTEL64_SHL,  { SAcop(0), index[2].tag == Arg::IIMMEDIATE ? SAcop(2) : SAcop(2, AF_LOWER8) });
        case (OP_SHR):     return SyT(INTEL64_SHR,  { SAcop(0), index[2].tag == Arg::IIMMEDIATE ? SAcop(2) : SAcop(2, AF_LOWER8) });
        case (OP_SAR):     return SyT(INTEL64_SAR,  { SAcop(0), index[2].tag == Arg::IIMMEDIATE ? SAcop(2) : SAcop(2, AF_LOWER8) });
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
                Assert(index[1].value >= OP_GT && index[1].value <= OP_NS);
                int tarcode = index[1].value == OP_NE ? INTEL64_CMOVNE : (
                              index[1].value == OP_EQ ? INTEL64_CMOVE : (
                              index[1].value == OP_GE ? INTEL64_CMOVGE : (
                              index[1].value == OP_LE ? INTEL64_CMOVLE : (
                              index[1].value == OP_GT ? INTEL64_CMOVG : (
                              index[1].value == OP_LT ? INTEL64_CMOVL : (
                              index[1].value == OP_S  ? INTEL64_CMOVS : (
                            /*index[1].value == OP_NS?*/INTEL64_CMOVNS /* : throw error*/)))))));
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
                            /*index[1].value == OP_NS?*/INTEL64_SETNS /* : throw error*/)))))));
                return SyT(tarcode, { SAcop(0) });
            }
            break;

        case (OP_UNSPILL): return SyT(INTEL64_MOV, { SAcop(0), SAcopspl(1) });
        case (OP_SPILL):   return SyT(INTEL64_MOV, { SAcopspl(0), SAcop(1) });
        case (OP_JCC):
            Assert(index.size() == 2 && index[0].tag == Arg::IIMMEDIATE && index[1].tag == Arg::IIMMEDIATE);
            switch (index[0].value)
            {
            case (OP_NE):  return SyT(INTEL64_JNE, { SAcop(1, AF_PRINTOFFSET) });
            case (OP_EQ):  return SyT(INTEL64_JE,  { SAcop(1, AF_PRINTOFFSET) });
            case (OP_LT):  return SyT(INTEL64_JL,  { SAcop(1, AF_PRINTOFFSET) });
            case (OP_GT):  return SyT(INTEL64_JG,  { SAcop(1, AF_PRINTOFFSET) });
            case (OP_GE):  return SyT(INTEL64_JGE, { SAcop(1, AF_PRINTOFFSET) });
            case (OP_LE):  return SyT(INTEL64_JLE, { SAcop(1, AF_PRINTOFFSET) });
            default:
                break;
            };
            break;
        case (OP_JMP):     return SyT(INTEL64_JMP, { SAcop(0, AF_PRINTOFFSET) });

        case (OP_RET):     return SyT(INTEL64_RET, {});
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
        virtual PassID pass_id() const override final { return CP_INTEL64_BRA_SNIPPETS; }
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
        virtual PassID pass_id() const override final { return CP_INTEL64_ARA_SNIPPETS; }
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
        m_offsetShift = 0;
        m_callerStackIncrement = 0;
        m_postInstructionOffset = true;
        m_registersAmount = 40;
        m_name = "Intel64";
        m_beforeRegAllocPasses.push_back(Intel64BRASnippets::make(this));
        m_afterRegAllocPasses.push_back(Intel64ARASnippets::make(this));
#if __LOOPS_OS == __LOOPS_WINDOWS
        m_parameterRegisters[RB_INT] = { RCX, RDX, R8, R9 };
        m_returnRegisters[RB_INT] = { RAX };
        m_callerSavedRegisters[RB_INT] = { R10, R11 };
        m_calleeSavedRegisters[RB_INT] = { RBX, RSI, RDI, RBP, R12, R13, R14, R15 };
#elif __LOOPS_OS == __LOOPS_LINUX || __LOOPS_OS == __LOOPS_MAC
        m_parameterRegisters[RB_INT] = { RDI, RSI, RDX, RCX, R8, R9 };
        m_returnRegisters[RB_INT] = { RAX, RDX };
        m_callerSavedRegisters[RB_INT] = { R10, R11 };
        m_calleeSavedRegisters[RB_INT] = { RBX, RBP, R12, R13, R14, R15 };
#else
#error Unknown OS
#endif
    }

    std::set<size_t> Intel64Backend::filterStackPlaceable(const Syntop& a_op, const std::set<size_t>& toFilter) const
    {
        switch (a_op.opcode)
        {
        case(OP_MOV): return (toFilter.size() < 2) ? toFilter : std::set<size_t>({ 1 });
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
            std::set<size_t> res = toFilter;
            res.erase(1);
            res = (res.size() < 2) ? res : std::set<size_t>({ 0 });
            if (a_op[1].tag == Arg::IREG && a_op[0].idx == a_op[1].idx && res.count(0) || res.count(1))
            {
                res.insert(0);
                res.insert(1);
            }
            return res;
        }
        case(OP_DIV):
        case(OP_MOD):
        case(OP_MUL):
            return (toFilter.count(2)) ? std::set<size_t>({ 2 }) : std::set<size_t>({ });
        case(OP_NEG):
        case(OP_NOT):
        {
            Assert(a_op.size() == 2 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IREG);
            if (toFilter.size() == 2 && a_op[0].idx != a_op[1].idx) //{0,1}
                return std::set<size_t>({ 1 });
            else 
                return toFilter;
        }
        case(OP_CMP): return (toFilter.size() < 2) ? toFilter : std::set<size_t>({ 0 });
        case(OP_LOAD):
        case(OP_STORE): return std::set<size_t>();
            break;
        case(OP_SELECT):
            Assert(a_op.size() == 4);
            return (toFilter.count(2) && !regOrSpiEq(a_op[0], a_op[2])) ? std::set<size_t>({2}) : std::set<size_t>({});
            break;
        case(OP_IVERSON):
            Assert(a_op.size() == 2);
            return (toFilter.count(0) ? std::set<size_t>({0}) : std::set<size_t>({}));
            break;
        case(OP_ABS):
            Assert(a_op.size() == 2);
            return (toFilter.count(1) && !regOrSpiEq(a_op[0], a_op[1])) ? std::set<size_t>({ 1 }) : std::set<size_t>({});
            break;
        default:
            break;
        }
        return Backend::filterStackPlaceable(a_op, toFilter);
    }

    size_t Intel64Backend::reusingPreferences(const Syntop& a_op, const std::set<size_t>& undefinedArgNums) const
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
            if (undefinedArgNums.count(1)) //TODO(ch): Hmmm... looks like there binary mask will there works better.
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
        default:
            break;
        }
        return Backend::reusingPreferences(a_op, undefinedArgNums);
    }

    size_t Intel64Backend::spillSpaceNeeded(const Syntop& a_op, int basketNum) const
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
                if (a_op[2].tag == Arg::IREG)
                    return 1;
            case (OP_ABS):
            case (OP_SIGN):
                return 1;
                break;
            default:
                break;
            }
        return Backend::spillSpaceNeeded(a_op, basketNum);
    }

    std::set<size_t> Intel64Backend::getUsedRegistersIdxs(const Syntop& a_op, int basketNum, uint64_t flagmask) const
    {
        //TODO(ch): This specialized version of function must disappear after introducing snippets. 
        //They will give info about used registers, like now instructions answers.
        //Actually, it's easy to think, that we have to keep used registers info on level of SyntopTranslation. Hmm...

        bool bypass = true;
        uint64_t actualRegs;
        uint64_t inRegs;
        uint64_t outRegs;
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
                if (basketNum == RB_INT && (~(BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) & flagmask) == 0)
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
                if (basketNum == RB_INT && (~(BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) & flagmask) == 0)
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
                if (basketNum == RB_INT && (~(BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) & flagmask) == 0)
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
                if (basketNum == RB_INT && (~(BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) & flagmask) == 0)
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
                if ( (a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IIMMEDIATE && a_op[1].value == 0) &&
                     (basketNum == RB_INT && (~(BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) & flagmask) == 0) )
                {
                    actualRegs = makeBitmask64({ 0 });
                    inRegs = makeBitmask64({});
                    outRegs = makeBitmask64({ 0 });
                    bypass = false;
                }
                break;
            default:
                break;
        };
        if (!bypass)
        {
            std::set<size_t> res;
            auto checkAndAdd = [&res](uint64_t mask, size_t posnum)
            {
                if (mask & (uint64_t(1) << posnum))
                    res.insert(posnum);
            };
            if (BinTranslation::Token::T_INPUT & flagmask)
                actualRegs &= inRegs;
            if (BinTranslation::Token::T_OUTPUT & flagmask)
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

    void Intel64Backend::getStackParameterLayout(const Syntfunc& a_func, const std::vector<size_t> (&regParsOverride)[RB_AMOUNT], std::map<RegIdx, size_t> (&parLayout)[RB_AMOUNT]) const
    {
    #if __LOOPS_OS == __LOOPS_WINDOWS
        size_t sp2parShift = 5;; //+5 is because of return address kept in stack + 32 bytes of shadow space
    #elif __LOOPS_OS == __LOOPS_LINUX
        size_t sp2parShift = 1; //+1 is because of return address kept in stack 
    #else
        #error Unknown OS.
    #endif        

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
            parLayout[basketNum][arg.idx] = currOffset + sp2parShift;
            currOffset+=xBasket[basketNum];
        }
    }

    size_t Intel64Backend::stackGrowthAlignment(size_t stackGrowth) const
    {
        return (stackGrowth ? stackGrowth + ((stackGrowth % 2) ? 0 : 1) : stackGrowth);  //Accordingly to Agner Fog, at start of function RSP % 16 = 8, but must be aligned to 16 for inner calls.
    }

    void Intel64Backend::writeCallerPrologue(Syntfunc& prog, int stackGrowth) const
    {
        throw std::runtime_error("Loops: function call is not supported on Intel.");
    }

    void Intel64Backend::writeCallerEpilogue(Syntfunc& prog, int stackGrowth) const
    {
        throw std::runtime_error("Loops: function call is not supported on Intel.");
    }

    Arg Intel64Backend::getSParg() const
    {
        return argReg(RB_INT, RSP);
    }

    std::unordered_map<int, std::string> Intel64Backend::getOpStrings() const
    {
        return std::unordered_map<int, std::string>({
            {INTEL64_MOV,       "mov"},
            {INTEL64_MOVSX,   "movsx"},
            {INTEL64_MOVSXD, "movsxd"},
            {INTEL64_MOVZX,   "movzx"},
            {INTEL64_ADC,       "adc"},
            {INTEL64_ADD,       "add"},
            {INTEL64_SUB,       "sub"},
            {INTEL64_IMUL,     "imul"},
            {INTEL64_IDIV,     "idiv"},
            {INTEL64_SHL,       "shl"},
            {INTEL64_SHR,       "shr"},
            {INTEL64_SAR,       "sar"},
            {INTEL64_AND,       "and"},
            {INTEL64_OR,         "or"},
            {INTEL64_XOR,       "xor"},
            {INTEL64_NOT,       "not"},
            {INTEL64_NEG,       "neg"},
            {INTEL64_CQO,       "cqo"},
            {INTEL64_XCHG,     "xchg"},
            {INTEL64_CMP,       "cmp"},
            {INTEL64_CMOVE ,  "cmove"},
            {INTEL64_CMOVNE, "cmovne"},
            {INTEL64_CMOVL,   "cmovl"},
            {INTEL64_CMOVG,   "cmovg"},
            {INTEL64_CMOVLE, "cmovle"},
            {INTEL64_CMOVGE, "cmovge"},
            {INTEL64_CMOVS,   "cmovs"},
            {INTEL64_CMOVNS, "cmovns"},
            {INTEL64_SETE,     "sete"},
            {INTEL64_SETNE,   "setne"},
            {INTEL64_SETL,     "setl"},
            {INTEL64_SETG,     "setg"},
            {INTEL64_SETLE,   "setle"},
            {INTEL64_SETGE,   "setge"},
            {INTEL64_SETS,     "sets"},
            {INTEL64_SETNS,   "setns"},
            {INTEL64_JMP,       "jmp"},
            {INTEL64_JNE,       "jne"},
            {INTEL64_JE,        "je" },
            {INTEL64_JL,        "jl" },
            {INTEL64_JG,        "jg" },
            {INTEL64_JGE,       "jge"},
            {INTEL64_JLE,       "jle"},
            {INTEL64_RET,       "ret"} });
    }

    Printer::ColPrinter Intel64Backend::colHexPrinter(const Syntfunc& toP) const
    {
        std::vector<std::pair<size_t, size_t> > posNsizes;
        posNsizes.reserve(toP.program.size());
        size_t oppos = 0;
        for (const Syntop& op : toP.program)
        {
            size_t opsize = lookS2b(op).size();
            posNsizes.push_back(std::make_pair(oppos, opsize));
            oppos += opsize;
        }

        Assembly2Hex a2hPass(this);
        a2hPass.process(*((Syntfunc*)(nullptr)), toP);
        const FuncBodyBuf buffer = a2hPass.result_buffer();

        return [buffer, posNsizes](::std::ostream& out, const Syntop& toPrint, size_t rowNum, Backend*)
        {
            uint8_t* hexfield = &((*buffer)[0]) + posNsizes[rowNum].first;
            for (size_t pos = 0; pos < posNsizes[rowNum].second; pos++)
                out << std::hex << std::setfill('0') << std::setw(2) << (uint32_t) * (hexfield + pos) << " ";
        };
    }

    Printer::ArgPrinter Intel64Backend::argPrinter(const Syntfunc& toP) const
    {
        std::unordered_map<size_t, size_t> numbersAtPositions; //TODO(ch): I think, it's possible to give info about instrustion sizes through arguments of lambda. 
        std::vector<size_t> positions;
        positions.reserve(toP.program.size());
        size_t oppos = 0;
        for (size_t opnum = 0; opnum < toP.program.size(); opnum++)
        {
            size_t opsize = lookS2b(toP.program[opnum]).size();
            positions.push_back(oppos);
            numbersAtPositions[oppos] = opnum;
            oppos += opsize;
        }
        return [numbersAtPositions, positions](::std::ostream& out, const Syntop& toPrint, size_t rowNum, size_t argNum)
        {
            Arg arg = toPrint[argNum];
            if (arg.flags & AF_PRINTOFFSET)
            {
                if (arg.tag != Arg::IIMMEDIATE)
                    throw std::runtime_error("Printer: register offsets are not supported.");
                int64_t targetline = numbersAtPositions.at(positions[rowNum+1] + arg.value);
                out << "[" << targetline << "]";
                return;
            }
            bool address = arg.flags & AF_ADDRESS;
            if (address)
                out << "[";
            switch (arg.tag)
            {
            case Arg::IREG:
            {
                static const std::string rnames[4][16] = { { "rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8",  "r9", "r10", "r11" , "r12" , "r13" , "r14" , "r15" } ,
                                                           { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "r8d",  "r9d", "r10d", "r11d" , "r12d" , "r13d" , "r14d" , "r15d" },
                                                           { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "r8w",  "r9w", "r10w", "r11w" , "r12w" , "r13w" , "r14w" , "r15w" },
                                                           { "al", "cl", "dl", "bl", "spl", "bpl", "sil", "dil", "r8b",  "r9b", "r10b", "r11b" , "r12b" , "r13b" , "r14b" , "r15b" } };

                out << rnames[(arg.flags & AF_LOWER8)/AF_LOWER32][arg.idx];
                break;
            }
            case Arg::IIMMEDIATE:
                if (arg.value == 0)
                    out << "#0";
                else
                    out << "#0x" << std::right << std::hex << std::setfill('0') << std::setw(2) << arg.value; break;
            case Arg::ISPILLED:
                if (arg.value == 0)
                    out << "[rsp]";
                else
                    out << "[rsp+#0x" << std::right << std::hex << std::setfill('0') << std::setw(2) << arg.value * 8 << "]"; break;
            default:
                throw std::runtime_error("Undefined argument type.");
            };
            if (address)
                out << "]";
        };
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
            default:
                a_dest.program.push_back(op);
                break;
            }
    }
    
    void Intel64Backend::switchOnSpillStressMode()
    {
#if __LOOPS_OS == __LOOPS_WINDOWS
        m_parameterRegisters[RB_INT] = { RCX, RDX, R8, R9 };
        m_returnRegisters[RB_INT] = { RAX };
        m_callerSavedRegisters[RB_INT] = {};
        m_calleeSavedRegisters[RB_INT] = { R12, R13, R14, R15 };
#elif __LOOPS_OS == __LOOPS_LINUX || __LOOPS_OS == __LOOPS_MAC
        m_parameterRegisters[RB_INT] = { RDI, RSI, RDX, RCX };
        m_returnRegisters[RB_INT] = { RAX };
        m_callerSavedRegisters[RB_INT] = {};
        m_calleeSavedRegisters[RB_INT] = { R12, R13, R14, R15 };
#else
#error Unknown OS
#endif
    }
};
#endif // __LOOPS_ARCH == __LOOPS_INTEL64
