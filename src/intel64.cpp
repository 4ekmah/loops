/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
#include "intel64.hpp"
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

    enum Intel64ArgFlags
    {
        I64AF_ADDRESS = 1,
        I64AF_LOWER32 = 2
    };

    static std::shared_ptr<M2bMap> instrucion_set;
    static void init_instrucion_set()
    {
        instrucion_set = std::make_shared<M2bMap>();
        using namespace BinatrTableConstructor;

        instrucion_set->add(INTEL64_MOVSXD,
            SFsiz({
                Sb(2, Sl({ BDsta(0x1218C, 18), BDreg(3, 0, Out), BDreg(3, 1, In) })),
                Sb(3, Sl({ BDsta(0x1218C, 18), BDreg(3, 0, Out), BDsta(0x10, 5), BDreg(3, 2, In), BDreg(3, 1, In)}))
                }));

        instrucion_set->add(INTEL64_MOV,
            SFsiz(
                {
                    Sb(2,
                        SFtyp(0,
                        {
                            Sb(Arg::IREG,
                                SFtyp(1,
                                {
                                    Sb(Arg::IREG, SFflg(1, I64AF_ADDRESS,
                                        {
                                            Sb(0, Sl({ BDsta(0x12227, 18), BDreg(3, 1, In), BDreg(3, 0, Out) })), //mov rax, rbx 
                                            Sb(I64AF_ADDRESS, SFflg(0, I64AF_LOWER32,
                                            {
                                                Sb(0, Sl({ BDsta(0x12224, 18), BDreg(3, 1, In), BDreg(3, 0, In) })), //mov [rax], rbx  
                                                Sb(I64AF_LOWER32, Sl({ BDsta(0x224, 10), BDreg(3, 1, In), BDreg(3, 0, In) })) //mov [rax], ebx
                                            }))
                                        })),
                                    Sb(Arg::ISPILLED, Sl({ BDsta(0x12225, 18), BDreg(3, 1, In), BDsta(0x424, 11), BDspl(8, 0) })) //mov [rsp + offset], rbx
                                })),
                            Sb(Arg::ISPILLED, //TODO(ch): IMPORTANT: Forbid cases with switch on I64AF_ADDRESS on 0 index.
                                SFtyp(1,
                                {
                                    Sb(Arg::IREG, Sl({ BDsta(0x1222D, 18), BDreg(3, 1, In), BDsta(0x424, 11), BDspl(8, 0) }, ROrd({1,0}))), //mov rax, [rsp + offset]
                                })),
                            Sb(Arg::ICONST,
                                SFtyp(1,
                                {
                                    Sb(Arg::IREG, Sl({ BDsta(0x918F8, 21), BDreg(3, 1, Out), BDcon(32, 0) }, ROrd({1,0}))),   //mov rax, <imm>
                                    Sb(Arg::ISPILLED, Sl({ BDsta(0x48c74424, 32), BDspl(8, 1), BDcon(32, 0) }, ROrd({1,0}))), //mov QWORD PTR [rsp + offset], <imm>
                                })
                            )
                        }))//,
                    //Sb(3, //TODO(ch): support all load/store operations.
                    //    SFflg(0, I64AF_ADDRESS,
                    //    {
                    //        Sb(0, Sl({ BDsta(0x12225, 18), BDreg(3, 0, In), BDsta(0x424, 11), BDspl(8, 1) }, ROrd({1,0}))), //mov rax, [rsp + offset]
                    //        Sb(I64AF_ADDRESS, Sl({ BDsta(0x12225, 18), BDreg(3, 0, In), BDsta(0x424, 11), BDspl(8, 1) }, ROrd({1,0}))), //mov rax, [rsp + offset]
                    //    }))
                }));


        instrucion_set->add(INTEL64_ADD,
            SFtyp(0,
                {
                    Sb(Arg::IREG,
                        SFtyp(1,
                        {
                            Sb(Arg::IREG, Sl({ BDsta(0x12007, 18), BDreg(3, 1, In), BDreg(3, 0, In | Out) })), //add rax, rbx 
                            Sb(Arg::ISPILLED, Sl({ BDsta(0x1200D, 18), BDreg(3, 1, In), BDsta(0x424, 11), BDspl(8, 0) })) //add [rsp + offset], rbx
                        })),
                    Sb(Arg::ISPILLED,
                        SFtyp(1,
                        {
                            Sb(Arg::IREG, Sl({ BDsta(0x12005, 18), BDreg(3, 0, In), BDsta(0x424, 11), BDspl(8, 1) }, ROrd({1,0}))), //add rax, [rsp + offset]
                        })),
                    Sb(Arg::ICONST,
                        SFtyp(1,
                        {
                            Sb(Arg::IREG, Sl({ BDsta(0x91078, 21), BDreg(3, 1, Out), BDcon(8, 0) }, ROrd({1,0}))),   //add rax, <imm>  
                            Sb(Arg::ISPILLED, Sl({ BDsta(0x48814424, 32), BDspl(8, 0), BDcon(8, 1) }, ROrd({1,0}))), //add QWORD PTR [rsp + offset], <imm>
                        })
                    )
                }));

        instrucion_set->add(INTEL64_SUB,
            SFtyp(0,
                {
                    Sb(Arg::IREG,
                        SFtyp(1,
                        {
                            Sb(Arg::IREG, Sl({ BDsta(0x120A7, 18), BDreg(3, 1, In), BDreg(3, 0, In | Out) })), //sub rax, rbx 
                            Sb(Arg::ISPILLED, Sl({ BDsta(0x120A5, 18), BDreg(3, 1, In), BDsta(0x424, 11), BDspl(8, 0) })) //sub [rsp + offset], rbx
                        })),
                    Sb(Arg::ISPILLED,
                        SFtyp(1,
                        {
                            Sb(Arg::IREG, Sl({ BDsta(0x120AD, 18), BDreg(3, 0, In), BDsta(0x424, 11), BDspl(8, 1) }, ROrd({1,0}))), //sub rax, [rsp + offset]
                        })),
                    Sb(Arg::ICONST,
                        SFtyp(1,
                        {
                            Sb(Arg::IREG, Sl({ BDsta(0x9107D, 21), BDreg(3, 1, Out), BDcon(8, 0) }, ROrd({1,0}))),   //sub rax, <imm>
                            Sb(Arg::ISPILLED, Sl({ BDsta(0x48836c24, 32), BDspl(8, 0), BDcon(8, 1) }, ROrd({1,0}))), //sub QWORD PTR [rsp + offset], <imm>
                        })
                    )
                }));

        //instrucion_set->add(INTEL64_MUL, Sl({ BDsta(0x4D8,11), BDreg(5, 2, In), BDsta(0x1F, 6), BDreg(5, 1, In), BDreg(5, 0, Out) }));
        //instrucion_set->add(INTEL64_SDIV, Sl({ BDsta(0x4D6,11), BDreg(5, 2, In), BDsta(0x3, 6), BDreg(5, 1, In), BDreg(5, 0, Out) }));
        
        instrucion_set->add(INTEL64_CMP,
            SFtyp(0,
                {
                    Sb(Arg::IREG,
                        SFtyp(1,
                        {
                            Sb(Arg::IREG, Sl({ BDsta(0x120E7, 18), BDreg(3, 1, In), BDreg(3, 0, In) })),                  //cmp rax, rbx 
                            Sb(Arg::ISPILLED, Sl({ BDsta(0x120E5, 18), BDreg(3, 1, In), BDsta(0x424, 11), BDspl(8, 0) })) //cmp [rsp + offset], rbx
                        })),
                    Sb(Arg::ISPILLED,
                        SFtyp(1,
                        {
                            Sb(Arg::IREG, Sl({ BDsta(0x120ED, 18), BDreg(3, 0, In), BDsta(0x424, 11), BDspl(8, 1) }, ROrd({1,0}))) //cmp rax, [rsp + offset]
                        })),
                    Sb(Arg::ICONST,
                        SFtyp(1,
                        {
                            Sb(Arg::IREG, Sl({ BDsta(0x9107F, 21), BDreg(3, 0, Out), BDcon(8, 1) }, ROrd({1,0}))),    //cmp rax, <imm>
                            Sb(Arg::ISPILLED, Sl({ BDsta(0x48817c24, 32), BDspl(8, 0), BDcon(32, 1) }, ROrd({1,0})))  //cmp QWORD PTR [rsp + offset], <imm>
                        })
                    )
                }));

        instrucion_set->add(INTEL64_JMP, Sl({ BDsta(0xE9,8), BDoff(32, 0) }));
        instrucion_set->add(INTEL64_JNE, Sl({ BDsta(0xf85,16), BDoff(32, 0) }));
        instrucion_set->add(INTEL64_JE,  Sl({ BDsta(0xf84,16), BDoff(32, 0) }));
        instrucion_set->add(INTEL64_JL,  Sl({ BDsta(0xf8c,16), BDoff(32, 0) }));
        instrucion_set->add(INTEL64_JLE, Sl({ BDsta(0xf8e,16), BDoff(32, 0) }));
        instrucion_set->add(INTEL64_JG,  Sl({ BDsta(0xf8f,16), BDoff(32, 0) }));
        instrucion_set->add(INTEL64_JGE, Sl({ BDsta(0xf8d,16), BDoff(32, 0) }));
        instrucion_set->add(INTEL64_RET, Sl({ BDsta(0xC3, 8) }));
    };

    inline M2bMap& get_instrucion_set()
    {
        if (instrucion_set.get() == nullptr)
            init_instrucion_set();
        return *instrucion_set.get();
    }

    static std::shared_ptr<M2mMap> target_mnemonics;
    static void init_target_mnemonics()
    {
        target_mnemonics = std::make_shared<M2mMap>();
        using namespace MnemotrTableConstructor;
        target_mnemonics->add(OP_LOAD,
            SFval(1,
                {
                    Sb(TYPE_I32, SFsiz(
                    {
                        Sb(3, Sl(INTEL64_MOVSXD, {MAcop(0), MAcop(2)})),
                        Sb(4, Sl(INTEL64_MOVSXD, {MAcop(0), MAcop(3), MAcop(2)}))
                    }))//, //TODO(ch): Support all loads.
                    //Sb(TYPE_U32, SFtyp(3, //TODO(ch):
                    //{
                    //    Sb(Arg::ICONST, Sl(INTEL64_LDR, {MAcon(0), MAcop(3), MAcop(2), MAcop(0)}))
                    //})),
                    //Sb(TYPE_U64, SFtyp(3,
                    //{
                    //    Sb(Arg::ICONST, Sl(INTEL64_LDR, {MAcon(1), MAcop(3), MAcop(2), MAcop(0)}))
                    //}))
                }));
        target_mnemonics->add(OP_STORE,
            SFsiz(
                {
                    Sb(3, SFval(0,
                    {
                        Sb(TYPE_I32, Sl(INTEL64_MOV, {MAcop(2, I64AF_LOWER32), MAcop(1, I64AF_ADDRESS)})),
                        Sb(TYPE_U32, Sl(INTEL64_MOV, {MAcop(2, I64AF_LOWER32), MAcop(1, I64AF_ADDRESS)})),
                        Sb(TYPE_I64, Sl(INTEL64_MOV, {MAcop(2), MAcop(1, I64AF_ADDRESS)})),
                        Sb(TYPE_U64, Sl(INTEL64_MOV, {MAcop(2), MAcop(1, I64AF_ADDRESS)}))
                    }))//,
                    //Sb(4, SFval(0,                                                               //TODO(ch): support base:offset store
                    //{
                    //    Sb(TYPE_I32, Sl(INTEL64_MOV, {MAcon(0), MAcop(2), MAcop(1), MAcop(3)})),
                    //    Sb(TYPE_U32, Sl(INTEL64_MOV, {MAcon(0), MAcop(2), MAcop(1), MAcop(3)})),
                    //    Sb(TYPE_I64, Sl(INTEL64_MOV, {MAcop(2, I64AF_ADDRESS), MAcop(1, I64AF_ADDRESS), MAcop(3)})),
                    //    Sb(TYPE_U64, Sl(INTEL64_MOV, {MAcop(2, I64AF_ADDRESS), MAcop(1, I64AF_ADDRESS), MAcop(3)}))
                    //}))
                }));
        target_mnemonics->add(OP_MOV, Sl(INTEL64_MOV, { MAcop(1), MAcop(0) }));
        target_mnemonics->add(OP_ADD, Sl(INTEL64_ADD, { MAcop(2), MAcop(0) })); //TODO(ch): check somehow that arg[0] == arg [1]
        target_mnemonics->add(OP_SUB, Sl(INTEL64_SUB, { MAcop(2), MAcop(0) }));
        //target_mnemonics->add(OP_MUL, Sl(INTEL64_MUL, { MAcop(2), MAcop(1), MAcop(0) }));
        //target_mnemonics->add(OP_DIV, Sl(INTEL64_SDIV, { MAcop(2), MAcop(1), MAcop(0) }));
        target_mnemonics->add(OP_CMP, Sl(INTEL64_CMP, { MAcop(1), MAcop(0) }));
        target_mnemonics->add(OP_UNSPILL, Sl(INTEL64_MOV, { MAcopspl(1), MAcop(0) }));
        target_mnemonics->add(OP_SPILL,   Sl(INTEL64_MOV, { MAcop(1), MAcopspl(0) }));
        target_mnemonics->add(OP_RET, Sl(INTEL64_RET, {}));

    };

    inline M2mMap& get_target_mnemonics()
    {
        if (target_mnemonics.get() == nullptr)
            init_target_mnemonics();
        return *target_mnemonics.get();
    }

    Intel64Backend::Intel64Backend()
    {
        m_2binary = get_instrucion_set();
        m_2tararch = get_target_mnemonics();
        m_exeAlloc = Allocator::getInstance();
        m_isLittleEndianInstructions = false;
        m_isLittleEndianOperands = true;
        m_isMonowidthInstruction = false;
        m_registersAmount = 40;
        m_name = "Intel64";
#if defined(_WIN32)
        m_parameterRegisters = std::vector<IRegInternal>({ RCX, RDX, R8, R9 }); //TODO(ch): IMPORTANT: Implement same for AARCH64!
        m_returnRegisters = std::vector<IRegInternal>({ RAX });
        m_callerSavedRegisters = std::vector<IRegInternal>({ R10, R11 });
        m_calleeSavedRegisters = std::vector<IRegInternal>({ RBX, RSI, RDI, RBP, R12, R13, R14, R15 });
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
            if (a_btop.size() != 1 || a_btop.args[0].tag != Arg::ICONST)
                throw std::string("Wrong JMP format.");
            m_labelRefMap[a_btop.args[0].value].emplace_back(a_formingtarget.program.size(), 0, getM2mCurrentOffset());
            int targetop = (a_btop.opcode == OP_JMP_NE) ? INTEL64_JNE : (
                (a_btop.opcode == OP_JMP_EQ) ? INTEL64_JE : (
                (a_btop.opcode == OP_JMP_LT) ? INTEL64_JL : (
                (a_btop.opcode == OP_JMP_GT) ? INTEL64_JG : (
                (a_btop.opcode == OP_JMP_GE) ? INTEL64_JGE : (
                (a_btop.opcode == OP_JMP_LE) ? INTEL64_JLE :
                                                              INTEL64_JMP)))));
            Syntop toAdd(targetop, std::initializer_list<Arg>({ getM2mCurrentOffset() }));
            toAdd.args[0].value += m_2binary[toAdd].size(a_btop);
            a_formingtarget.program.emplace_back(toAdd);
            return true;
        }
        case (OP_LABEL):
        {
            if (a_btop.size() != 1 || a_btop.args[0].tag != Arg::ICONST)
                throw std::string("Wrong LABEL format.");
            if (m_labelMap.count(a_btop.args[0].value) != 0)
                throw std::string("Label redefinition");
            m_labelMap[a_btop.args[0].value] = getM2mCurrentOffset();
            return true;
        }
        default:
            return false;
        };
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
                throw std::string("Reference to unknown label");
            const int64_t loff = static_cast<int64_t>(m_labelMap[label.first]);
            for (LabelRefInfo& lref : label.second)
            {
                if (lref.opnum >= result.program.size())
                    throw std::string("Internal error: operation number is too big");
                if (lref.argnum >= result.program[lref.opnum].size())
                    throw std::string("Internal error: operation don't have so much arguments");
                if (result.program[lref.opnum].args[lref.argnum].tag != Arg::ICONST)
                    throw std::string("Internal error: operation don't have so much arguments");
                int64_t& opoff = result.program[lref.opnum].args[lref.argnum].value;
                opoff = (loff - opoff);
            }
        }
        return result;
    }

    void Intel64Backend::writePrologue(const Syntfunc& a_srcFunc, std::vector<Syntop>& a_canvas, size_t a_regSpilled, const std::set<IRegInternal>& a_calleeSaved) const
    {
        //TODO(ch): Do something with stack-passed variables.
        size_t spAddAligned = a_regSpilled + a_calleeSaved.size(); //TODO(ch): Align to 16 or 32 if SIMD's are used.
        spAddAligned = (spAddAligned + ((spAddAligned % 2)?0:1)) * 8; //Accordingly to Agner Fog, at start of function RSP % 16 = 8, but must be aligned to 16 for inner calls.
        Arg SP = argIReg(RSP);
        Arg SPinc = argIConst(spAddAligned);
        a_canvas.push_back(Syntop(OP_SUB, { SP, SP, SPinc }));
        size_t savNum = a_regSpilled;
        for (IRegInternal toSav : a_calleeSaved)
            a_canvas.push_back(Syntop(OP_SPILL, { argIConst(savNum++), argIReg(toSav) }));
    }

    void Intel64Backend::writeEpilogue(const Syntfunc& a_srcFunc, std::vector<Syntop>& a_canvas, size_t a_regSpilled, const std::set<IRegInternal>& a_calleeSaved) const
    {
        //TODO(ch): Do something with stack-passed variables.
        size_t spAddAligned = a_regSpilled + a_calleeSaved.size(); //TODO(ch): Align to 16 or 32 if SIMD's are used.
        spAddAligned = (spAddAligned + ((spAddAligned % 2) ? 0 : 1)) * 8;
        Arg SP = argIReg(RSP);
        Arg SPinc = argIConst(spAddAligned);
        size_t savNum = a_regSpilled;
        for (IRegInternal toSav : a_calleeSaved)
            a_canvas.push_back(Syntop(OP_UNSPILL, { argIReg(toSav), argIConst(savNum++)}));
        a_canvas.push_back(Syntop(OP_ADD, { SP, SP, SPinc }));
    }

    std::unordered_map<int, std::string> Intel64Backend::getOpStrings() const
    {
        return std::unordered_map<int, std::string>({
            {INTEL64_MOVSXD,  "movsxd"},
            {INTEL64_MOV,     "mov"},
            {INTEL64_ADD,     "add"},
            {INTEL64_SUB,     "sub"},
            //{INTEL64_MUL,   "mul"}, //TODO(ch): IMPORTANT: Implement!
            //{INTEL64_SDIV,  "sdiv"},
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
            size_t opsize = m_2binary[op].size(op);
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
            size_t opsize = m_2binary[toP.program[opnum]].size(toP.program[opnum]);
            positions.push_back(oppos);
            numbersAtPositions[oppos] = opnum;
            oppos += opsize;
        }
        return [numbersAtPositions, positions](::std::ostream& out, const Syntop& toPrint, size_t rowNum, size_t argNum, const OpPrintInfo& pinfo)
        {
            OpPrintInfo::operand ainfo;
            if (pinfo.size() != 0)
                ainfo = pinfo[argNum];
            Arg arg = (ainfo.argnum == OpPrintInfo::PI_NOTASSIGNED) ? toPrint[argNum] : toPrint[ainfo.argnum];
            if ((ainfo.argnum != OpPrintInfo::PI_NOTASSIGNED) && ainfo.flags & OpPrintInfo::PI_OFFSET)
            {
                if (arg.tag != Arg::ICONST)
                    throw std::string("Printer: register offsets are not supported.");
                int64_t targetline = numbersAtPositions.at(positions[rowNum+1] + arg.value);
                out << "[" << targetline << "]";
                return;
            }
            bool w32 = (ainfo.argnum != OpPrintInfo::PI_NOTASSIGNED) && ainfo.flags & OpPrintInfo::PI_REG32;
            bool address = ((ainfo.argnum != OpPrintInfo::PI_NOTASSIGNED) && ainfo.flags & OpPrintInfo::PI_ADDRESS) || arg.flags & I64AF_ADDRESS;
            if (address)
                out << "[";
            switch (arg.tag)
            {
            case Arg::IREG:
            {
                static const std::string rnames[] = { "ax", "cx", "dx", "bx",
                                                     "sp", "bp", "si", "di", 
                                                      "8",  "9", "10", "11" , "12" , "13" , "14" , "15"};
                std::string prefix = (arg.flags & I64AF_LOWER32 && arg.idx < 8)? "e" : "r";
                std::string postfix = (arg.flags & I64AF_LOWER32 && arg.idx >= 8)? "d": "";
                out << prefix << rnames[arg.idx] << postfix;
                break;
            }
            case Arg::ICONST:
                if (arg.value == 0)
                    out << "#0";
                else
                    out << "#0x" << std::right << std::hex << std::setfill('0') << std::setw(2) << arg.value; break;
            case Arg::ISPILLED:
                if (arg.value == 0)
                    out << "[rsp]";
                else
                    out << "[rsp+#0x" << std::right << std::hex << std::setfill('0') << std::setw(2) << arg.value << "]"; break;
            default:
                throw std::string("Undefined argument type.");
            };
            if (address)
                out << "]";
        };
    }
};
