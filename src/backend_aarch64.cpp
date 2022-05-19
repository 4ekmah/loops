/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "backend_aarch64.hpp"
#include "composer.hpp"
#include "func_impl.hpp"
#include <unordered_map>
#include <iomanip>

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
    LR   = 30, //TODO(ch): Decide if it's possible to use frame pointer as callee-saved register.
    SP   = 31
};

Binatr a64binatrLookup(const Syntop& index, bool& scs)
{
    using namespace BinatrTableConstructor;
    scs = true;
    switch (index.opcode)
    {
        //TODO(ch): Ldrsw also have many options to be added here. Literal loading, register shift or post/pre-indexing. Format isn't full now and hardcoded.
    case (AARCH64_LDRSW):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BDsta(0x5C5,11), BDreg(2, 5, In), BDsta(0x1A,6), BDreg(1, 5, In), BDreg(0, 5, Out) });
        else if (index[2].tag == Arg::ICONST)
            return BiT({ BDsta(0x5C4,11), BDcon(2, 9), BDsta(0x1,2), BDreg(1, 5, In), BDreg(0, 5, Out) });
        break;
        //TODO(ch): It's LDR's specialization: immediate offset.
    case (AARCH64_LDR):
        Assert(index.size() == 4);
        if (index[3].tag == Arg::ICONST)
            return BiT({ BDsta(0x1, 1), BDcon(0, 1), BDsta(0xE5, 8), BDcon(3, 12), BDreg(2, 5, In), BDreg(1, 5, Out) });
        break;
        //TODO(ch): It's STR's specialization: immediate offset)
    case (AARCH64_STR):
        Assert(index.size() == 4);
        if (index[3].tag == Arg::ICONST)
            return BiT({ BDsta(0x1,  1), BDcon(0, 1), BDsta(0xE4, 8), BDcon(3, 12), BDreg(2, 5, In), BDreg(1, 5, In) });
        break;
        //TODO(ch): There is a lot variants of move: stack pointer mov, bitmask, inverted. There offered just a part of possibilities. Also, even register and wide immediate variants are specialized and hardcoded: Specialization is: 64 register.
    case (AARCH64_MOV):
        Assert(index.size() == 2);
        if (index[1].tag == Arg::IREG)
            return BiT({ BDsta(0x550,11), BDreg(1, 5, In), BDsta(0x1F ,11), BDreg(0, 5, Out) });
        else if (index[1].tag == Arg::ICONST)
            return BiT({ BDsta(0x694,11), BDcon(1, 16), BDreg(0, 5, Out) });
        break;
        //TODO(ch): This is specialized version of ADD: 64 bit only, noshift(for register).
    case (AARCH64_ADD):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BDsta(0x458, 11), BDreg(2, 5, In), BDsta(0x0, 6), BDreg(1, 5, In), BDreg(0, 5, Out) });
        else if (index[2].tag == Arg::ICONST)
            return BiT({ BDsta(0x244, 10), BDcon(2, 12), BDreg(1, 5, In), BDreg(0, 5, Out) });
        break;
        //TODO(ch): This is specialized version of SUB: 64 bit only, noshift(for register).
    case (AARCH64_SUB):
        Assert(index.size() == 3);
        if (index[2].tag == Arg::IREG)
            return BiT({ BDsta(0x658,11), BDreg(2, 5, In), BDsta(0x0,6), BDreg(1, 5, In), BDreg(0, 5, Out) });
        else if (index[2].tag == Arg::ICONST)
            return BiT({ BDsta(0x344,10), BDcon(2, 12), BDreg(1, 5, In), BDreg(0, 5, Out) });
        break;
        //TODO(ch): Specialization: 64 registers.
    case (AARCH64_MUL): return BiT({ BDsta(0x4D8,11), BDreg(2, 5, In), BDsta(0x1F, 6), BDreg(1, 5, In), BDreg(0,5,Out) });
        //TODO(ch): Specialization: 64 registers.
    case (AARCH64_SDIV): return BiT({ BDsta(0x4D6,11), BDreg(2, 5, In), BDsta(0x3, 6), BDreg(1, 5, In), BDreg(0,5,Out) });
        //TODO(ch): there is no CMP_R instruction in ARM processors, it's prespecialized version of CMP(shifted register). We must make switchers much more flexible and functional to support real CMP. Specialization is: 64 register, zero shift.
    case (AARCH64_CMP_R): return BiT({ BDsta(0x758,11), BDreg(1, 5, In), BDsta(0x0,6), BDreg(0, 5, In), BDsta(0x1F, 5) });
    case (AARCH64_B): return BiT({ BDsta(0x5, 6), BDoff(0, 26) });
        //TODO(ch): there is no B_LT, B_LE, B_GT, B_GE instructions in ARM processors, it's prespecialized versions of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
    case (AARCH64_B_NE): return BiT({ BDsta(0x54,8), BDoff(0, 19), BDsta(0x1, 5) });
    case (AARCH64_B_EQ): return BiT({ BDsta(0x54,8), BDoff(0, 19), BDsta(0x0, 5) });
    case (AARCH64_B_LT): return BiT({ BDsta(0x54,8), BDoff(0, 19), BDsta(0xB, 5) });
    case (AARCH64_B_LE): return BiT({ BDsta(0x54,8), BDoff(0, 19), BDsta(0xD, 5) });
    case (AARCH64_B_GT): return BiT({ BDsta(0x54,8), BDoff(0, 19), BDsta(0xC, 5) });
    case (AARCH64_B_GE): return BiT({ BDsta(0x54,8), BDoff(0, 19), BDsta(0xA, 5) });
    case (AARCH64_RET): return BiT({ BDsta(0x3597C0, 22), BDreg(0, 5, In), BDsta(0x0,5) });
    default:
        break;
    }
    scs = false;
    return Binatr();
}

Mnemotr a64mnemotrLookup(const Syntop& index, bool& scs)
{
    using namespace MnemotrTableConstructor;
    scs = true;
    switch (index.opcode)
    {
    case (OP_LOAD):
        Assert(index.size() >= 3);
        if (index[1].value == TYPE_I32)
        {
            if (index.size() == 3)
                return MnT(AARCH64_LDRSW, { MAcop(0), MAcop(2, AF_ADDRESS), MAcon(0) });
            else if (index.size() == 4)
                return MnT(AARCH64_LDRSW, { MAcop(0), MAcop(2, AF_ADDRESS), MAcop(3) });
        }
        else
        {
            Assert(index.size() == 4 && index[3].tag == Arg::ICONST);
            if (index[1].value == TYPE_U32)
                return MnT(AARCH64_LDR, { MAcon(0, AF_NOPRINT), MAcop(0), MAcop(2, AF_ADDRESS), MAcop(3) });
            else if (index[1].value == TYPE_U64)
                return MnT(AARCH64_LDR, { MAcon(1, AF_NOPRINT), MAcop(0), MAcop(2, AF_ADDRESS), MAcop(3) });
        }
        break;
    case (OP_STORE):
        if (index.size() == 3)
        {
            if (index[0].value == TYPE_I32 || index[1].value == TYPE_U32)
                return MnT(AARCH64_STR, { MAcon(0, AF_NOPRINT), MAcop(2, AF_LOWER32), MAcop(1, AF_ADDRESS), MAcon(0) });
            else if (index[0].value == TYPE_I64 || index[1].value == TYPE_U64)
                return MnT(AARCH64_STR, { MAcon(1, AF_NOPRINT), MAcop(2), MAcop(1, AF_ADDRESS), MAcon(0) });
        }
        else if (index.size() == 4)
        {
            if (index[0].value == TYPE_I32 || index[1].value == TYPE_U32)
                return MnT(AARCH64_STR, { MAcon(0), MAcop(3, AF_LOWER32), MAcop(1), MAcop(2) });
            else if (index[0].value == TYPE_I64 || index[1].value == TYPE_U64)
                return MnT(AARCH64_STR, { MAcon(1), MAcop(3), MAcop(1), MAcop(2) });
        }
        break;
    case (OP_MOV):    return MnT(AARCH64_MOV, { MAcop(0), MAcop(1) });
    case (OP_ADD):    return MnT(AARCH64_ADD, { MAcop(0), MAcop(1), MAcop(2) });
    case (OP_SUB):    return MnT(AARCH64_SUB, { MAcop(0), MAcop(1), MAcop(2) });
    case (OP_MUL):    return MnT(AARCH64_MUL, { MAcop(0), MAcop(1), MAcop(2) });
    case (OP_DIV):    return MnT(AARCH64_SDIV, { MAcop(0), MAcop(1), MAcop(2) });
    case (OP_CMP):    return MnT(AARCH64_CMP_R, { MAcop(0), MAcop(1) });
    case (OP_UNSPILL):return MnT(AARCH64_LDR, { MAcon(1, AF_NOPRINT), MAcop(0), MAreg(SP, AF_ADDRESS), MAcop(1) });
    case (OP_SPILL):  return MnT(AARCH64_STR, { MAcon(1, AF_NOPRINT), MAcop(1), MAreg(SP, AF_ADDRESS), MAcop(0) });
    case (OP_RET):    return MnT(AARCH64_RET, { MAreg(LR) });
    default:
        break;
    };
    scs = false;
    return Mnemotr();
}

Aarch64Backend::Aarch64Backend()
{
    m_m2blookup = a64binatrLookup;
    m_m2mlookup = a64mnemotrLookup;
    m_exeAlloc = Allocator::getInstance();
    m_isLittleEndianInstructions = true;
    m_isLittleEndianOperands = false;
    m_isMonowidthInstruction = true;
    m_instructionWidth = 4;
    m_registersAmount = 7;
    m_name = "AArch64";
    m_parameterRegisters = makeRegBasket({ R0, R1, R2, R3, R4, R5, R6, R7 });
    m_returnRegisters = makeRegBasket({ R0, R1, R2, R3, R4, R5, R6, R7 });
    m_callerSavedRegisters = makeRegBasket({ XR, R9, R10, R11, R12, R13, R14, R15, IP0, IP1 });
    m_calleeSavedRegisters = makeRegBasket({ PR, R19, R20, R21, R22, R23, R24, R25, R26, R27, R28 });
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
            if (a_btop.size() != 1 || a_btop.args[0].tag != Arg::ICONST)
                throw std::runtime_error("Wrong JMP format.");
            m_labelRefMap[a_btop.args[0].value].emplace_back(a_formingtarget.program.size(), 0, getM2mCurrentOffset());
            int targetop = (a_btop.opcode == OP_JMP_NE) ? AARCH64_B_NE : (
                           (a_btop.opcode == OP_JMP_EQ) ? AARCH64_B_EQ : (
                           (a_btop.opcode == OP_JMP_LT) ? AARCH64_B_LT : (
                           (a_btop.opcode == OP_JMP_GT) ? AARCH64_B_GT : (
                           (a_btop.opcode == OP_JMP_GE) ? AARCH64_B_GE : (
                           (a_btop.opcode == OP_JMP_LE) ? AARCH64_B_LE :
                                                            AARCH64_B )))));
            Arg jtar = getM2mCurrentOffset();
            jtar.flags = AF_PRINTOFFSET;
            a_formingtarget.program.emplace_back(targetop, std::initializer_list<Arg>({jtar}));
            return true;
        }
        case (OP_LABEL):
        {
            if (a_btop.size() != 1 || a_btop.args[0].tag != Arg::ICONST)
                throw std::runtime_error("Wrong LABEL format.");
            if(m_labelMap.count(a_btop.args[0].value) != 0)
                throw std::runtime_error("Label redefinition");
            m_labelMap[a_btop.args[0].value] = getM2mCurrentOffset();
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
            if (result.program[lref.opnum].args[lref.argnum].tag != Arg::ICONST)
                throw std::runtime_error("Internal error: operation don't have so much arguments");
            int64_t& opoff = result.program[lref.opnum].args[lref.argnum].value;
            opoff = (loff - opoff) / 4; //AArch64 supports only multiply-4 offsets, so, for compactification, they are divided by 4. //TODO(ch): shift?
        }
    }
    return result;
}

size_t Aarch64Backend::stackGrowthAlignment(size_t stackGrowth) const
{
    return stackGrowth ? stackGrowth + (stackGrowth % 2) : stackGrowth;        //TODO(ch): Align to 16 or 32 if SIMD's are used.
}

size_t Aarch64Backend::stackParamOffset(size_t a_nettoSpills, size_t a_snippetCausedSpills) const
{
    return stackGrowthAlignment(a_nettoSpills + a_snippetCausedSpills);
}

Arg Aarch64Backend::getSParg(Func* funcimpl) const
{
    return argIReg(SP, funcimpl);
}

std::unordered_map<int, std::string> Aarch64Backend::getOpStrings() const
{
    return std::unordered_map<int, std::string>({
        {AARCH64_LDRSW, "ldrsw"},
        {AARCH64_LDR,   "ldr"  },
        {AARCH64_STR,   "str"  },
        {AARCH64_MOV,   "mov"  },
        {AARCH64_ADD,   "add"  },
        {AARCH64_SUB,   "sub"  },
        {AARCH64_MUL,   "mul"  },
        {AARCH64_SDIV,  "sdiv" },
        {AARCH64_CMP_R, "cmp"  },
        {AARCH64_B,     "b"    },
        {AARCH64_B_NE,  "b.ne" },
        {AARCH64_B_EQ,  "b.eq" },
        {AARCH64_B_LT,  "b.lt" },
        {AARCH64_B_GT,  "b.gt" },
        {AARCH64_B_GE,  "b.ge" },
        {AARCH64_B_LE,  "b.le" },
        {AARCH64_RET,   "ret"  }});
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
            if (arg.tag != Arg::ICONST)
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
            case Arg::ICONST:
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
    m_parameterRegisters = makeRegBasket({ R0, R1, R2, R3 });
    m_returnRegisters = makeRegBasket({ R0, R1, R2, R3 });
    m_callerSavedRegisters = makeRegBasket({});
    m_calleeSavedRegisters = makeRegBasket({ PR, R19, R20, R21, R22 });
}

};
