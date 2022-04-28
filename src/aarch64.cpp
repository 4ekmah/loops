/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "aarch64.hpp"
#include "composer.hpp"
#include "func_impl.hpp"
#include <unordered_map>
#include <iomanip>

namespace loops
{

static std::shared_ptr<M2bMap> instrucion_set;
static void init_instrucion_set()
{
    instrucion_set = std::make_shared<M2bMap>();
    using namespace BinatrTableConstructor;
    //TODO(ch): Ldrsw also have many options to be added here. Literal loading, register shift or post/pre-indexing. Format isn't full now and hardcoded.

    instrucion_set->add(AARCH64_LDRSW,
        SFtyp(0, {
            Sb(Arg::IREG, Sl({BDsta(0x5C5,11), BDreg(5, 2, In), BDsta(0x1A,6), BDreg(5, 1, In|PAdr), BDreg(5, 0, Out)})),
            Sb(Arg::ICONST, Sl({BDsta(0x5C4,11), BDcon(9, 2), BDsta(0x1,2), BDreg(5, 1, In|PAdr), BDreg(5, 0, Out)}))
        }));

    //TODO(ch): It's LDR's specialization: immediate offset.
    instrucion_set->add(AARCH64_LDR,
        SFtyp(1, {
            Sb(Arg::ICONST, Sl({BDsta(0x1, 1), BDcon(1), BDsta(0xE5, 8), BDcon(12, 2), BDreg(5, 1, In|PAdr), BDreg(5, 0, Out)}))
        }));

    //TODO(ch): It's STR's specialization: immediate offset)
    instrucion_set->add(AARCH64_STR,
        SFtyp(1, {
            Sb(Arg::ICONST, Sl({BDsta(0x1,  1), BDcon(1), BDsta(0xE4, 8), BDcon(12, 2), BDreg(5, 1, In|PAdr), BDreg(5, 0, In|A32D)}))
        }));

    //TODO(ch): There is a lot variants of move: stack pointer mov, bitmask, inverted. There offered just a part of possibilities. Also, even register and wide immediate variants are specialized and hardcoded: Specialization is: 64 register.
    instrucion_set->add(AARCH64_MOV,
        SFtyp(0, {
            Sb(Arg::IREG, Sl({BDsta(0x550,11), BDreg(5, 1, In), BDsta(0x1F ,11), BDreg(5, 0, Out)})),
            Sb(Arg::ICONST, Sl({BDsta(0x694,11), BDcon(16, 1), BDreg(5, 0, Out)}))
        }));

    //TODO(ch): This is specialized version of ADD: 64 bit only, noshift(for register).
    instrucion_set->add(AARCH64_ADD,
        SFtyp(0, {
            Sb(Arg::IREG, Sl({BDsta(0x458, 11), BDreg(5, 2, In), BDsta(0x0, 6), BDreg(5, 1, In), BDreg(5, 0, Out)})),
            Sb(Arg::ICONST, Sl({BDsta(0x244, 10), BDcon(12, 2), BDreg(5, 1, In), BDreg(5, 0, Out)}))
        }));

    //TODO(ch): This is specialized version of SUB: 64 bit only, noshift(for register).
    instrucion_set->add(AARCH64_SUB,
        SFtyp(0, {
            Sb(Arg::IREG, Sl({BDsta(0x658,11), BDreg(5, 2, In), BDsta(0x0,6), BDreg(5, 1, In), BDreg(5, 0, Out)})),
            Sb(Arg::ICONST, Sl({BDsta(0x344,10), BDcon(12, 2), BDreg(5, 1, In), BDreg(5, 0, Out)}))
        }));

    //TODO(ch): Specialization: 64 registers.
    instrucion_set->add(AARCH64_MUL, Sl({BDsta(0x4D8,11), BDreg(5, 2, In), BDsta(0x1F, 6), BDreg(5, 1, In), BDreg(5, 0, Out)}));
    //TODO(ch): Specialization: 64 registers.
    instrucion_set->add(AARCH64_SDIV, Sl({BDsta(0x4D6,11), BDreg(5, 2, In), BDsta(0x3, 6), BDreg(5, 1, In), BDreg(5, 0, Out)}));
    //TODO(ch): there is no CMP_R instruction in ARM processors, it's prespecialized version of CMP(shifted register). We must make switchers much more flexible and functional to support real CMP. Specialization is: 64 register, zero shift.
    instrucion_set->add(AARCH64_CMP_R, Sl({BDsta(0x758,11), BDreg(5, 1, In), BDsta(0x0,6), BDreg(5, 0, In), BDsta(0x1F,5)}));
    instrucion_set->add(AARCH64_B, Sl({BDsta(0x5,6), BDoff(26, 0)}));
    //TODO(ch): there is no B_LT, B_LE, B_GT, B_GE instructions in ARM processors, it's prespecialized versions of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
    instrucion_set->add(AARCH64_B_NE, Sl({BDsta(0x54,8), BDoff(19, 0), BDsta(0x1,5)}));
    instrucion_set->add(AARCH64_B_EQ, Sl({BDsta(0x54,8), BDoff(19, 0), BDsta(0x0,5)}));
    instrucion_set->add(AARCH64_B_LT, Sl({BDsta(0x54,8), BDoff(19, 0), BDsta(0xB,5)}));
    instrucion_set->add(AARCH64_B_LE, Sl({BDsta(0x54,8), BDoff(19, 0), BDsta(0xD,5)}));
    instrucion_set->add(AARCH64_B_GT, Sl({BDsta(0x54,8), BDoff(19, 0), BDsta(0xC,5)}));
    instrucion_set->add(AARCH64_B_GE, Sl({BDsta(0x54,8), BDoff(19, 0), BDsta(0xA,5)}));
    instrucion_set->add(AARCH64_RET, Sl({BDsta(0x3597C0, 22), BDreg(5, OpPrintInfo::PI_NOTASSIGNED, In), BDsta(0x0,5)}));
};

inline M2bMap& get_instrucion_set()
{
    if(instrucion_set.get() == nullptr)
        init_instrucion_set();
    return *instrucion_set.get();
}

static std::shared_ptr<M2mMap> target_mnemonics;
static void init_target_mnemonics()
{
    target_mnemonics = std::make_shared<M2mMap>();
    using namespace MnemotrTableConstructor;
    const IRegInternal SP = 31; //Stack pointer
    const IRegInternal RetReg = 30; //Standart filler for return. Return values must be located in I0-I7
    target_mnemonics->add(OP_LOAD,
        SFval(1,
        {
            Sb(TYPE_I32, SFsiz(
            {
                Sb(3, Sl(AARCH64_LDRSW, {MAcon(0), MAcop(2), MAcop(0)})),
                Sb(4, Sl(AARCH64_LDRSW, {MAcop(3), MAcop(2), MAcop(0)}))
            })),
            Sb(TYPE_U32, SFtyp(3,
            {
                Sb(Arg::ICONST, Sl(AARCH64_LDR, {MAcon(0), MAcop(3), MAcop(2), MAcop(0)}))
            })),
            Sb(TYPE_U64, SFtyp(3,
            {
                Sb(Arg::ICONST, Sl(AARCH64_LDR, {MAcon(1), MAcop(3), MAcop(2), MAcop(0)}))
            }))
        }));
    target_mnemonics->add(OP_STORE,
        SFsiz(
        {
            Sb(3, SFval(0,
            {
                Sb(TYPE_I32, Sl(AARCH64_STR, {MAcon(0), MAcon(0), MAcop(1), MAcop(2)})),
                Sb(TYPE_U32, Sl(AARCH64_STR, {MAcon(0), MAcon(0), MAcop(1), MAcop(2)})),
                Sb(TYPE_I64, Sl(AARCH64_STR, {MAcon(1), MAcon(0), MAcop(1), MAcop(2)})),
                Sb(TYPE_U64, Sl(AARCH64_STR, {MAcon(1), MAcon(0), MAcop(1), MAcop(2)}))
            })),
            Sb(4, SFval(0,
            {
                Sb(TYPE_I32, Sl(AARCH64_STR, {MAcon(0), MAcop(2), MAcop(1), MAcop(3)})),
                Sb(TYPE_U32, Sl(AARCH64_STR, {MAcon(0), MAcop(2), MAcop(1), MAcop(3)})),
                Sb(TYPE_I64, Sl(AARCH64_STR, {MAcon(1), MAcop(2), MAcop(1), MAcop(3)})),
                Sb(TYPE_U64, Sl(AARCH64_STR, {MAcon(1), MAcop(2), MAcop(1), MAcop(3)}))
            }))
        }));
    target_mnemonics->add(OP_MOV,     Sl(AARCH64_MOV,   {MAcop(1), MAcop(0)}));
    target_mnemonics->add(OP_ADD,     Sl(AARCH64_ADD,   {MAcop(2), MAcop(1), MAcop(0)}));
    target_mnemonics->add(OP_SUB,     Sl(AARCH64_SUB, {MAcop(2), MAcop(1), MAcop(0)}));
    target_mnemonics->add(OP_MUL,     Sl(AARCH64_MUL,   {MAcop(2), MAcop(1), MAcop(0)}));
    target_mnemonics->add(OP_DIV,     Sl(AARCH64_SDIV,  {MAcop(2), MAcop(1), MAcop(0)}));
    target_mnemonics->add(OP_CMP,     Sl(AARCH64_CMP_R, {MAcop(1), MAcop(0)}));
    target_mnemonics->add(OP_UNSPILL, Sl(AARCH64_LDR,   {MAcon(1), MAcop(1), MAreg(SP), MAcop(0)}));
    target_mnemonics->add(OP_SPILL,   Sl(AARCH64_STR,   {MAcon(1), MAcop(0), MAreg(SP), MAcop(1)}));
    target_mnemonics->add(OP_RET,     Sl(AARCH64_RET,   {MAreg(RetReg)}));

};

inline M2mMap& get_target_mnemonics()
{
    if(target_mnemonics.get() == nullptr)
        init_target_mnemonics();
    return *target_mnemonics.get();
}

Aarch64Backend::Aarch64Backend()
{
    m_2binary = get_instrucion_set();
    m_2tararch = get_target_mnemonics();
    m_exeAlloc = Allocator::getInstance();
    m_isLittleEndianInstructions = true;
    m_isLittleEndianOperands = false;
    m_isMonowidthInstruction = true;
    m_instructionWidth = 4;
    m_registersAmount = 7;
    m_name = "AArch64";
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
                throw std::string("Wrong JMP format.");
            m_labelRefMap[a_btop.args[0].value].emplace_back(a_formingtarget.program.size(), 0, getM2mCurrentOffset());
            int targetop = (a_btop.opcode == OP_JMP_NE) ? AARCH64_B_NE : (
                           (a_btop.opcode == OP_JMP_EQ) ? AARCH64_B_EQ : (
                           (a_btop.opcode == OP_JMP_LT) ? AARCH64_B_LT : (
                           (a_btop.opcode == OP_JMP_GT) ? AARCH64_B_GT : (
                           (a_btop.opcode == OP_JMP_GE) ? AARCH64_B_GE : (
                           (a_btop.opcode == OP_JMP_LE) ? AARCH64_B_LE :
                                                            AARCH64_B )))));
            a_formingtarget.program.emplace_back(targetop, std::initializer_list<Arg>({getM2mCurrentOffset()}));
            return true;
        }
        case (OP_LABEL):
        {
            if (a_btop.size() != 1 || a_btop.args[0].tag != Arg::ICONST)
                throw std::string("Wrong LABEL format.");
            if(m_labelMap.count(a_btop.args[0].value) != 0)
                throw std::string("Label redefinition");
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
            throw std::string("Reference to unknown label");
        const int64_t loff = static_cast<int64_t>(m_labelMap[label.first]);
        for(label_ref_info& lref : label.second)
        {
            if (lref.opnum >= result.program.size())
                throw std::string("Internal error: operation number is too big");
            if (lref.argnum >= result.program[lref.opnum].size())
                throw std::string("Internal error: operation don't have so much arguments");
            if (result.program[lref.opnum].args[lref.argnum].tag != Arg::ICONST)
                throw std::string("Internal error: operation don't have so much arguments");
            int64_t& opoff = result.program[lref.opnum].args[lref.argnum].value;
            opoff = (loff - opoff) / 4; //AArch64 supports only multiply-4 offsets, so, for compactification, they are divided by 4. //TODO(ch): shift?
        }
    }
    return result;
}

void Aarch64Backend::writePrologue(const Syntfunc& a_srcFunc, std::vector<Syntop>& a_canvas, size_t a_regSpilled, const std::set<IRegInternal>& a_calleeSaved) const
{
    //TODO(ch): Spill also callee-saved registers.
    if(a_regSpilled)
    {
        size_t spAligned = (a_regSpilled + (a_regSpilled%2)) * 8;
        Arg SP = argIReg(31);
        Arg SPinc = argIConst(spAligned); //TODO(ch): We are assuming here, that register is already aligned into 16 bytes before decreasing, so decreasing it with on multiple of 16, that's how keeping alignment.
        a_canvas.push_back(Syntop(OP_SUB, {SP, SP,  argIConst(spAligned)}));
    }
}

void Aarch64Backend::writeEpilogue(const Syntfunc& a_srcFunc, std::vector<Syntop>& a_canvas, size_t a_regSpilled, const std::set<IRegInternal>& a_calleeSaved) const
{
    //TODO(ch): Unspill also callee-saved registers.
    if(a_regSpilled)
    {
        size_t spAligned = (a_regSpilled + (a_regSpilled%2)) * 8;
        Arg SP = argIReg(31);
        Arg SPinc = argIConst(spAligned); //TODO(ch): We are assuming here, that register is already aligned into 16 bytes before decreasing, so decreasing it with on multiple of 16, that's how keeping alignment.
        a_canvas.push_back(Syntop(OP_ADD, {SP, SP,  argIConst(spAligned)}));
    }
}

std::unordered_map<int, std::string> Aarch64Backend::getOpStrings() const
{
    return std::unordered_map<int, std::string>({
        {AARCH64_LDRSW, "ldrsw"},
        {AARCH64_LDR,   "ldr"},
        {AARCH64_STR,   "str"},
        {AARCH64_MOV,   "mov"},
        {AARCH64_ADD,   "add"},
        {AARCH64_SUB, "sub"},
        {AARCH64_MUL,   "mul"},
        {AARCH64_SDIV,  "sdiv"},
        {AARCH64_CMP_R, "cmp"},
        {AARCH64_B,     "b"},
        {AARCH64_B_NE,  "b.ne"},
        {AARCH64_B_EQ,  "b.eq"},
        {AARCH64_B_LT,  "b.lt"},
        {AARCH64_B_GT,  "b.gt"},
        {AARCH64_B_GE,  "b.ge"},
        {AARCH64_B_LE,  "b.le"},
        {AARCH64_RET,   "ret"}});
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
    return [](::std::ostream& out, const Syntop& toPrint, size_t rowNum, size_t argNum, const OpPrintInfo& pinfo)
    {
        OpPrintInfo::operand ainfo;
        if(pinfo.size() != 0)
            ainfo = pinfo[argNum];
        Arg arg = (ainfo.argnum == OpPrintInfo::PI_NOTASSIGNED) ? toPrint[argNum] : toPrint[ainfo.argnum];
        if((ainfo.argnum != OpPrintInfo::PI_NOTASSIGNED) && ainfo.flags & OpPrintInfo::PI_OFFSET)
        {
            if (arg.tag != Arg::ICONST)
                throw std::string("Printer: register offsets are not supported.");
            int64_t targetline = rowNum + arg.value;
            out << "["<< targetline<< "]";
            return;
        }
        bool w32 = (ainfo.argnum != OpPrintInfo::PI_NOTASSIGNED) && ainfo.flags & OpPrintInfo::PI_REG32;
        bool address = (ainfo.argnum != OpPrintInfo::PI_NOTASSIGNED) && ainfo.flags & OpPrintInfo::PI_ADDRESS;
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
                throw std::string("Undefined argument type.");
        };
        if (address)
            out<<"]";
    };
}
};
