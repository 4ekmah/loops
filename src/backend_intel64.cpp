/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
#include "backend_intel64.hpp"
#if defined(_M_AMD64) //TODO(ch): It must be about target processor, not operational system
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
        using namespace BinTranslationConstruction;
        scs = true;
        switch (index.opcode)
        {
        case (INTEL64_MOVSXD):
            Assert((index.size() == 2 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG) ||
                (index.size() == 3 && index[0].tag == Arg::IREG && index[1].tag == Arg::IREG && index[2].tag == Arg::IREG));
            if (index.size() == 2)
            {
                static uint64_t stats[4] = { 0x1218C , 0x1318C, 0x1258C, 0x1358C };
                size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                return BiT({ BTsta(stats[statn], 18), BTreg(0, 3, Out), BTreg(1, 3, In) });
            }
            else //if(index.size() == 3)
            {
                static uint64_t stats[8] = { 0x1218C, 0x1318C, 0x1258C, 0x1358C, 0x1298C, 0x1398C, 0x12D8C, 0x13D8C };
                size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2) | ((index[2].idx < 8) ? 0 : 4);
                return BiT({ BTsta(stats[statn], 18), BTreg(0, 3, Out), BTsta(0x10, 5), BTreg(2, 3, In), BTreg(1, 3, In) });
            }
            break;
        case (INTEL64_MOV):
            Assert(index.size() == 2);
            if (index[0].tag == Arg::IREG)
            {
                if (index[1].tag == Arg::IREG)
                {
                    if (index[0].flags & AF_ADDRESS)
                    {
                        if (index[1].flags & AF_LOWER32)  //mov [rax], ebx
                        {
                            uint64_t stat = 0x224;
                            if (index[0].idx == 13 || index[0].idx == 13) //mov [r12/r13], ebx
                            {
                                stat = index[0].idx == 13 ? (index[1].idx < 8 ? 0x10625 : 0x11625) : (index[1].idx < 8 ? 0x10624 : 0x11624);
                                return BiT({ BTsta(stat, 18), BTreg(1, 3, In), BTreg(0, 3, In) , BTsta(index[0].idx == 13 ? 0 : 0x24, 8) });
                            }
                            else
                            {
                                size_t statn = ((index[0].idx < 8) ? 0 : 1) | ((index[1].idx < 8) ? 0 : 2);
                                static uint64_t stats[4] = { 0x224, 0x10624, 0x11224, 0x11624 };
                                static uint64_t statw[4] = { 10, 18, 18, 18 };
                                return BiT({ BTsta(stats[statn], statw[statn]), BTreg(1, 3, In), BTreg(0, 3, In) });
                            }
                        }
                        else
                            return BiT({ BTsta(0x12224, 18), BTreg(1, 3, In), BTreg(0, 3, In) }); //mov [rax], rbx
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
                    return  BiT({ BTsta(index[0].idx < 8 ? 0x918F8 : 0x938F8, 21), BTreg(0, 3, Out), BTimm(1, 32) });//mov rax, <imm>

            }
            else if (index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ BTsta(index[1].idx < 8 ? 0x12225 : 0x13225, 18), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 8) });   //mov [rsp + offset], rbx
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x48c74424, 32), BTspl(0, 8), BTimm(1, 32) });  //mov QWORD PTR [rsp + offset], <imm>
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
                    return BiT({ BTsta(index[0].idx < 8 ? 0x1200D : 0x1300D, 18), BTreg(0, 3, In), BTsta(0x424, 11), BTspl(1, 8) }); //add rax, [rsp + offset]
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ (index[0].idx < 8) ? nBkb(2,0x4883,5,0b11000) : nBkb(2,0x4983,5,0b11000), BTreg(0, 3, In | Out), BTimm(1, 8) });
            }
            else if (index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ BTsta(index[1].idx < 8 ? 0x12005 : 0x13005, 18), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 8) });   //add [rsp + offset], rbx
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x48834424, 32), BTspl(0, 8), BTimm(1, 8) });  //add QWORD PTR [rsp + offset], <imm>
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
                    return BiT({ BTsta((index[0].idx < 8) ? 0x120ED : 0x130ED, 18), BTreg(0, 3, In), BTsta(0x424, 11), BTspl(1, 8) });
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x9107F, 21), BTreg(0, 3, Out), BTimm(1, 8) });

            }
            else if (index[0].tag == Arg::ISPILLED)
            {
                if (index[1].tag == Arg::IREG)
                    return BiT({ BTsta(index[1].idx < 8 ? 0x120E5 : 0x130E5, 18), BTreg(1, 3, In), BTsta(0x424, 11), BTspl(0, 8) });
                else if (index[1].tag == Arg::IIMMEDIATE)
                    return BiT({ BTsta(0x48817c24, 32), BTspl(0, 8), BTimm(1, 32) });
            }
            break;
        case (INTEL64_NEG):
            Assert(index.size() == 1 && (index[0].tag == Arg::IREG || index[0].tag == Arg::ISPILLED));
            if (index[0].tag == Arg::IREG)
                return BiT({ BTsta((index[0].idx < 8) ? 0x91EFB : 0x93EFB, 21), BTreg(0, 3, In | Out) });
            else //if(index[0].tag == Arg::ISPILLED)
                return BiT({ BTsta(0x48f75c24, 32), BTspl(0, 8) });
        case (INTEL64_CQO): return BiT({ BTsta(0x4899, 16) });
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

    SyntopTranslation i64STLookup(const Syntop& index, bool& scs)
    {
        using namespace SyntopTranslationConstruction;
        scs = true;
        switch (index.opcode)
        {
        case(OP_LOAD):
            Assert(index.size() > 1);
            if (index[1].value == TYPE_I32)
            {
                if (index.size() == 3)
                    return SyT(INTEL64_MOVSXD, { SAcop(0), SAcop(2) });
                else
                    return SyT(INTEL64_MOVSXD, { SAcop(0), SAcop(2), SAcop(3) });
            }//TODO(ch): Support all loads(64 and base:offset).
            break;
        case (OP_STORE):
            if (index.size() == 3)
            {
                switch (index[0].value)
                {
                case (TYPE_I32): return SyT(INTEL64_MOV, { SAcop(1, AF_ADDRESS), SAcop(2, AF_LOWER32) });
                case (TYPE_U32): return SyT(INTEL64_MOV, { SAcop(1, AF_ADDRESS), SAcop(2, AF_LOWER32) });
                case (TYPE_I64): return SyT(INTEL64_MOV, { SAcop(1, AF_ADDRESS), SAcop(2) });
                case (TYPE_U64): return SyT(INTEL64_MOV, { SAcop(1, AF_ADDRESS), SAcop(2) });
                default: break;
                }
            }
            //else if (index.size() == 4) //TODO(ch): support base:offset store
            //{
                    //switch (index[0].value)
                    //{
                    //case (TYPE_I32): return Sl(INTEL64_MOV, { SAcop(1, AF_ADDRESS), SAcop(2, AF_LOWER32), SAcop(3) });
                    //case (TYPE_U32): return Sl(INTEL64_MOV, { SAcop(1, AF_ADDRESS), SAcop(2, AF_LOWER32), SAcop(3) });
                    //case (TYPE_I64): return Sl(INTEL64_MOV, { SAcop(1, AF_ADDRESS), SAcop(2), SAcop(3) });
                    //case (TYPE_U64): return Sl(INTEL64_MOV, { SAcop(1, AF_ADDRESS), SAcop(2), SAcop(3) });
                    //default: break;
                    //}
            //}
            break;
        case (OP_MOV): return SyT(INTEL64_MOV, { SAcop(0), SAcop(1) });
        case (OP_ADD): return SyT(INTEL64_ADD, { SAcop(0), SAcop(2) });
        case (OP_SUB): return SyT(INTEL64_SUB, { SAcop(0), SAcop(2) });
        case (OP_MUL): return SyT(INTEL64_IMUL, { SAcop(0), SAcop(2) });
        case (OP_DIV): return SyT(INTEL64_IDIV, { SAcop(2) });
        case (OP_NEG): return SyT(INTEL64_NEG, { SAcop(0) });
        case (OP_CQO): return SyT(INTEL64_CQO, {});
        case (OP_CMP): return SyT(INTEL64_CMP, { SAcop(0), SAcop(1) });
        case (OP_UNSPILL): return SyT(INTEL64_MOV, { SAcop(0), SAcopspl(1) });
        case (OP_SPILL): return SyT(INTEL64_MOV, { SAcopspl(0), SAcop(1) });
        case (OP_RET): return SyT(INTEL64_RET, {});
        default:
            break;
        }
        scs = false;
        return SyntopTranslation();
    }

    class Three2Two : public CompilerStage
    {
    public:
        virtual void process(Syntfunc& a_processed) const override;
        virtual ~Three2Two() override {}
        static CompilerStagePtr make()
        {
            return std::static_pointer_cast<CompilerStage>(std::make_shared<Three2Two>());
        } 
    };

    Intel64Backend::Intel64Backend()
    {
        m_s2blookup = i64BTLookup;
        m_s2slookup = i64STLookup;
        m_exeAlloc = Allocator::getInstance();
        m_isLittleEndianInstructions = false;
        m_isLittleEndianOperands = true;
        m_isMonowidthInstruction = false;
        m_registersAmount = 40;
        m_name = "Intel64";
        m_afterRegAllocStages.push_back(Three2Two::make());
#if defined(_WIN32)
        m_parameterRegisters = makeBitmask64({ RCX, RDX, R8, R9 });
        m_returnRegisters = makeBitmask64({ RAX });
        m_callerSavedRegisters = makeBitmask64({ R10, R11 });
        m_calleeSavedRegisters = makeBitmask64({ RBX, RSI, RDI, RBP, R12, R13, R14, R15 });
#else
#error Linux is not supported
#endif
    }

    bool Intel64Backend::handleBytecodeOp(const Syntop& a_btop, Syntfunc& a_formingtarget) const
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
            if (a_btop.size() != 1 || a_btop.args[0].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Wrong JMP format.");
            m_labelRefMap[a_btop.args[0].value].emplace_back(a_formingtarget.program.size(), 0, getS2sCurrentOffset());
            int targetop = (a_btop.opcode == OP_JMP_NE) ? INTEL64_JNE : (
                (a_btop.opcode == OP_JMP_EQ) ? INTEL64_JE : (
                (a_btop.opcode == OP_JMP_LT) ? INTEL64_JL : (
                (a_btop.opcode == OP_JMP_GT) ? INTEL64_JG : (
                (a_btop.opcode == OP_JMP_GE) ? INTEL64_JGE : (
                (a_btop.opcode == OP_JMP_LE) ? INTEL64_JLE :
                                                              INTEL64_JMP)))));
            Arg jtar = getS2sCurrentOffset();
            jtar.flags = AF_PRINTOFFSET;
            Syntop toAdd(targetop, std::initializer_list<Arg>({ jtar }));
            toAdd.args[0].value += lookS2b(toAdd).size();
            a_formingtarget.program.emplace_back(toAdd);
            return true;
        }
        case (OP_LABEL):
        {
            if (a_btop.size() != 1 || a_btop.args[0].tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Wrong LABEL format.");
            if (m_labelMap.count(a_btop.args[0].value) != 0)
                throw std::runtime_error("Label redefinition");
            m_labelMap[a_btop.args[0].value] = getS2sCurrentOffset();
            return true;
        }
        default:
            return false;
        };
    }

    std::set<size_t> Intel64Backend::filterStackPlaceable(const Syntop& a_op, const std::set<size_t>& toFilter) const
    {
        switch (a_op.opcode)
        {
        case(OP_MOV): return (toFilter.size() < 2) ? toFilter : std::set<size_t>({ 1 });
        case(OP_ADD):
        case(OP_SUB):
        case(OP_MUL):
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
        case(OP_DIV): return (toFilter.count(2)) ? std::set<size_t>({ 2 }) : std::set<size_t>({ });
        case(OP_NEG): return toFilter;
        case(OP_CMP): return (toFilter.size() < 2) ? toFilter : std::set<size_t>({ 0 });
        case(OP_LOAD):
        case(OP_STORE): return std::set<size_t>();
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
        case OP_ADD:
        case OP_MUL:
        {
            if (undefinedArgNums.count(1))
                return 1;
            if (undefinedArgNums.count(2))
                return 2;
            break;
        }
        case OP_SUB:
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

    size_t Intel64Backend::spillSpaceNeeded(const Syntop& a_op) const
    {
        switch (a_op.opcode)
        {
        case (OP_DIV):
            return 2;
            break;
        default:
            break;
        }
        return Backend::spillSpaceNeeded(a_op);
    }

    std::set<size_t> Intel64Backend::getUsedRegistersIdxs(const Syntop& a_op, uint64_t flagmask) const
    {
        //TODO(ch): This specialized version of function must disappear after introducing snippets. 
        //They will give info about used registers, like now instructions answers.
        //Actually, it's easy to think, that we have to keep used registers info on level of SyntopTranslation. Hmm...

        std::set<size_t> res;
        bool bypass = true;
        switch (a_op.opcode)
        {
            case (OP_ADD):
            case (OP_SUB):
            case (OP_MUL):
            {
                Assert(a_op.size() == 3 && a_op[0].tag == Arg::IREG && a_op[1].tag == Arg::IREG);
                if (a_op[0].idx != a_op[1].idx && a_op[2].tag == Arg::IREG && ((~(BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) & flagmask) == 0))
                {
                    bool in = BinTranslation::Token::T_INPUT & flagmask;
                    bool out = BinTranslation::Token::T_OUTPUT & flagmask;
                    if (in && out)
                        in = out = false;
                    if (out)
                        res.insert(0);
                    if (in)
                    {
                        res.insert(1);
                        res.insert(2);
                    }
                    bypass = false;
                }
                break;
            }
            case (OP_DIV):
            {
                Assert(a_op.size() == 3 && a_op[0].tag == Arg::IREG);
                if ((~(BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT) & flagmask) == 0)
                {
                    bool in = BinTranslation::Token::T_INPUT & flagmask;
                    bool out = BinTranslation::Token::T_OUTPUT & flagmask;
                    if (in && out)
                        in = out = false;
                    if (out)
                        res.insert(0);
                    if (in)
                    {
                        if(a_op[1].tag == Arg::IREG)
                            res.insert(1);
                        if (a_op[2].tag == Arg::IREG)
                            res.insert(2);
                    }
                    bypass = false;
                }
                break;
            }
            default:
                break;
        };
        if (bypass)
            return Backend::getUsedRegistersIdxs(a_op, flagmask);
        return res;
    }

    Syntfunc Intel64Backend::bytecode2Target(const Syntfunc& a_bcfunc) const
    {
        m_retReg = Syntfunc::RETREG;
        m_labelMap.clear();//labels offsets from start.
        m_labelRefMap.clear(); // label referenes map is needed to calculate and put in relative offsets after
        Syntfunc result = Backend::bytecode2Target(a_bcfunc);
        for (auto label : m_labelRefMap)
        {
            if (m_labelMap.count(label.first) == 0)
                throw std::runtime_error("Reference to unknown label");
            const int64_t loff = static_cast<int64_t>(m_labelMap[label.first]);
            for (LabelRefInfo& lref : label.second)
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

    size_t Intel64Backend::stackGrowthAlignment(size_t stackGrowth) const
    {
        return (stackGrowth ? stackGrowth + ((stackGrowth % 2) ? 0 : 1) : stackGrowth);  //Accordingly to Agner Fog, at start of function RSP % 16 = 8, but must be aligned to 16 for inner calls.
    }

    size_t Intel64Backend::stackParamOffset(size_t a_nettoSpills, size_t a_snippetCausedSpills) const
    {
        return stackGrowthAlignment(a_nettoSpills + a_snippetCausedSpills) + 5; //TODO(ch): I have to understand, why it's 5??? I just got it by experiments.
    }

    Arg Intel64Backend::getSParg(Func* funcimpl) const
    {
        return argIReg(RSP, funcimpl);
    }

    std::unordered_map<int, std::string> Intel64Backend::getOpStrings() const
    {
        return std::unordered_map<int, std::string>({
            {INTEL64_MOVSXD,  "movsxd"},
            {INTEL64_MOV,     "mov"},
            {INTEL64_ADD,     "add"},
            {INTEL64_SUB,     "sub"},
            {INTEL64_IMUL,    "imul"},
            {INTEL64_IDIV,    "idiv"},
            {INTEL64_NEG,     "neg"},
            {INTEL64_CQO,     "cqo"},
            {INTEL64_CMP,     "cmp"},
            {INTEL64_JMP,     "jmp"},
            {INTEL64_JNE,     "jne"},
            {INTEL64_JE,      "je" },
            {INTEL64_JL,      "jl" },
            {INTEL64_JG,      "jg" },
            {INTEL64_JGE,     "jge"},
            {INTEL64_JLE,     "jle"},
            {INTEL64_RET,   "ret"} });
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
        FuncBodyBuf buffer = target2Hex(toP);
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
            bool w32 = arg.flags & AF_LOWER32;
            bool address = arg.flags & AF_ADDRESS;
            if (address)
                out << "[";
            switch (arg.tag)
            {
            case Arg::IREG:
            {
                static const std::string rnames[] = { "ax", "cx", "dx", "bx",
                                                     "sp", "bp", "si", "di", 
                                                      "8",  "9", "10", "11" , "12" , "13" , "14" , "15"};
                std::string prefix = (w32 && arg.idx < 8)? "e" : "r";
                std::string postfix = (w32 && arg.idx >= 8)? "d": "";
                out << prefix << rnames[arg.idx] << postfix;
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

    void Three2Two::process(Syntfunc& a_processed) const
    {
        std::vector<Syntop> newProg;
        newProg.reserve(2 * a_processed.program.size());
        for (Syntop& op : a_processed.program)
            switch (op.opcode)
            {
            case OP_ADD: 
            case OP_MUL:
            {
                Assert(op.size() == 3 && regOrSpi(op[0]));
                if (op[1].tag == Arg::IIMMEDIATE)
                    std::swap(op[1], op[2]);
                Assert(regOrSpi(op[1]));
                if (regOrSpi(op[2]) && regOrSpiEq(op[0], op[2]) && !regOrSpiEq(op[0], op[1]))
                    std::swap(op[1], op[2]);
                if (!regOrSpiEq(op[0], op[1]))
                {
                    newProg.push_back(Syntop(OP_MOV, { op[0],op[1] }));
                    op[1] = op[0];
                }
                newProg.push_back(op);
                break;
            }
            case OP_SUB:
            {
                Assert(op.size() == 3 && regOrSpi(op[0]) && (regOrSpi(op[1])||regOrSpi(op[2])));
                if (regOrSpi(op[1]) && regOrSpiEq(op[0], op[1]))
                {
                    newProg.push_back(op);
                }
                else if (!regOrSpi(op[2]) || !regOrSpiEq(op[0], op[2]))
                {
                    newProg.push_back(Syntop(OP_MOV, { op[0], op[1] }));
                    newProg.push_back(Syntop(OP_SUB, { op[0], op[0], op[2] }));
                } 
                else //op[0] == op[2] != op[0]
                {
                    newProg.push_back(Syntop(OP_SUB, { op[0], op[0], op[1] }));
                    newProg.push_back(Syntop(OP_NEG, { op[0] }));
                }
                break;
            }
            case OP_DIV:
            {
                Assert(op.size() == 3 && op[0].tag == Arg::IREG && op[1].tag == Arg::IREG && regOrSpi(op[2]));
                bool unspillRax = false;;
                if (op[0].idx != RAX)
                {
                    newProg.push_back(Syntop(OP_SPILL, { 0, argIReg(RAX) }));
                    unspillRax = true;
                }
                bool unspillRdx = false;
                if (op[0].idx != RDX)
                {
                    newProg.push_back(Syntop(OP_SPILL, { 1, argIReg(RDX) }));
                    unspillRdx = true;
                }
                Arg effectiveDivider = op[2];
                if (op[2].tag == Arg::IREG && op[2].idx == RAX)
                {
                    if (!unspillRax)
                        newProg.push_back(Syntop(OP_SPILL, { 0, argIReg(RAX) }));
                    effectiveDivider = argISpilled(0);
                }
                else if (op[2].tag == Arg::IREG && op[2].idx == RDX)
                {
                    if (!unspillRdx)
                        newProg.push_back(Syntop(OP_SPILL, { 1, argIReg(RDX) }));
                    effectiveDivider = argISpilled(1);
                }
                if (op[1].idx != RAX)
                    newProg.push_back(Syntop(OP_MOV, { argIReg(RAX), op[1] }));
                newProg.push_back(Syntop(OP_CQO, {}));
                newProg.push_back(Syntop(OP_DIV, { argIReg(RAX), argIReg(RAX), effectiveDivider }));
                if(op[0].idx != RAX)
                    newProg.push_back(Syntop(OP_MOV, { op[0], argIReg(RAX) }));
                if (unspillRax)
                    newProg.push_back(Syntop(OP_UNSPILL, { argIReg(RAX), 0 }));
                if (unspillRdx)
                    newProg.push_back(Syntop(OP_UNSPILL, { argIReg(RDX), 1 }));
                break;
            }

            default:
                newProg.push_back(op);
                break;
            }
        a_processed.program = newProg;
    }
    
    void Intel64Backend::switchOnSpillStressMode()
    {
        m_parameterRegisters = makeBitmask64({ RCX, RDX, R8, R9 });
        m_returnRegisters = makeBitmask64({ RAX });
        m_callerSavedRegisters = makeBitmask64({});
        m_calleeSavedRegisters = makeBitmask64({ R12, R13, R14, R15 });
    }

};
#endif
