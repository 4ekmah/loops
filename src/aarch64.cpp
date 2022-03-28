/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "aarch64.hpp"
#include "composer.hpp"
#include "func_impl.hpp"
#include <unordered_map>

namespace loops
{

inline Arg arg_hidden_reg_constructor(size_t reg) //TODO(ch): What the hell? Create normal walk class and make these functions method of class.
{
    Arg res;
    res.tag = Arg::IREG;
    res.idx = reg;
    return res;
}

M2bMap instrucion_set;
void init_instrucion_set()

{
    { //LDRSW:
        //TODO(ch): Ldrsw also have many options to be added here. Literal loading, register shift or post/pre-indexing. Format isn't full now and hardcoded.
        Binatr ldrswr({
            Binatr::Detail(Binatr::Detail::D_STATIC, 0x5C5,11),//10111000100
            Binatr::Detail(Binatr::Detail::D_REG,5)           ,//offset register
            Binatr::Detail(Binatr::Detail::D_STATIC, 0x1A,6)  ,//options field (presetted to 011010)
            Binatr::Detail(Binatr::Detail::D_REG,5)           ,//base register
            Binatr::Detail(Binatr::Detail::D_REG,5)            //target register
        });
        Binatr ldrswi({
            Binatr::Detail(Binatr::Detail::D_STATIC, 0x5C4,11),//10111000011
            Binatr::Detail(Binatr::Detail::D_CONST, 9)        ,//offset constant
            Binatr::Detail(Binatr::Detail::D_STATIC, 0x1,2)   ,//01
            Binatr::Detail(Binatr::Detail::D_REG,5)           ,//base register
            Binatr::Detail(Binatr::Detail::D_REG,5)            //target register
        });
        ArgIndexedArray<Binatr> ldrsw_vars;
        ldrsw_vars.m_condition = ArgIndexedArray<Binatr>::C_ARGTYPE;
        ldrsw_vars.m_argnum = 0;
        ldrsw_vars.m_branches[Arg::IREG] = ArgIndexedArray<Binatr>(ldrswr);
        ldrsw_vars.m_branches[Arg::ICONST] = ArgIndexedArray<Binatr>(ldrswi);
        instrucion_set[A64_LDRSW] = ldrsw_vars;
    }
    instrucion_set[A64_STR32_I] = ArgIndexedArray<Binatr>(Binatr({ //TODO(ch): there is no STR32_I instruction in ARM processors, it's prespecialized version of STR(immediate). We must make switchers much more flexible and functional to support real STR. Specialization is: 32 register
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x5C0,11),//10111000000
                                                                   
        Binatr::Detail(Binatr::Detail::D_CONST, 9)        ,//offset constant
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x3,2)   ,//options field (presetted to 011010)
        Binatr::Detail(Binatr::Detail::D_REG,5)           ,//target-address register
        Binatr::Detail(Binatr::Detail::D_REG,5)            //unloaded register
    }));
    {//MOV
        //TODO(ch): There is a lot variants of move: stack pointer mov, bitmask, inverted. There offered just a part of possibilities. Also, even register and wide immediate variants are specialized and hardcoded: Specialization is: 64 register.
        Binatr movi({
            Binatr::Detail(Binatr::Detail::D_STATIC, 0x694,11),//11010010100
            Binatr::Detail(Binatr::Detail::D_CONST, 16)       ,//constant fill reg
            Binatr::Detail(Binatr::Detail::D_REG, 5)           //target register
        });
        Binatr movr({ 
            Binatr::Detail(Binatr::Detail::D_STATIC, 0x550,11),//10101010000
            Binatr::Detail(Binatr::Detail::D_REG, 5)          ,//source register
            Binatr::Detail(Binatr::Detail::D_STATIC, 0x1F ,11),//00000011111
            Binatr::Detail(Binatr::Detail::D_REG, 5)           //target register
        });
        ArgIndexedArray<Binatr> mov_vars;
        mov_vars.m_condition = ArgIndexedArray<Binatr>::C_ARGTYPE;
        mov_vars.m_argnum = 0;
        mov_vars.m_branches[Arg::IREG] = ArgIndexedArray<Binatr>(movr);
        mov_vars.m_branches[Arg::ICONST] = ArgIndexedArray<Binatr>(movi);
        instrucion_set[A64_MOV] = mov_vars;

//        snippets[A64_MOV_I] = arg_indexed_array<i_snippet>(movi);
//        snippets[A64_MOV_R] = arg_indexed_array<i_snippet>(movr);
        
    }
    instrucion_set[A64_ADD_R] = ArgIndexedArray<Binatr>(Binatr({ //TODO(ch): there is no ADD_R instruction in ARM processors, it's prespecialized version of ADD(shifted register). We must make switchers much more flexible and functional to support real ADD. Specialization is: 64 registers, noshift
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x458,11),//1000000100
        Binatr::Detail(Binatr::Detail::D_REG, 5)          ,//addendum1 register
        Binatr::Detail(Binatr::Detail::D_STATIC, 0, 6)    ,//increment register
        Binatr::Detail(Binatr::Detail::D_REG, 5)          ,//addendum2 register
        Binatr::Detail(Binatr::Detail::D_REG, 5)           //target register
    }));
    instrucion_set[A64_MUL] = ArgIndexedArray<Binatr>(Binatr({ //TODO(ch): this is specialized version of MUL instruction. We must make switchers much more flexible and functional to support real MUL. Specialization is: 64 registers.
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x4D8,11),//10011011000
        Binatr::Detail(Binatr::Detail::D_REG, 5)          ,//multiplier register
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x1F, 6) ,//
        Binatr::Detail(Binatr::Detail::D_REG, 5)          ,//multiplied register
        Binatr::Detail(Binatr::Detail::D_REG, 5)           //target register
    }));
    instrucion_set[A64_SDIV] = ArgIndexedArray<Binatr>(Binatr({ //TODO(ch): this is specialized version of SDIV instruction. We must make switchers much more flexible and functional to support real SDIV. Specialization is: 64 registers.
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x4D6,11),//10011010110
        Binatr::Detail(Binatr::Detail::D_REG, 5)          ,//divisor register
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x3, 6) ,//
        Binatr::Detail(Binatr::Detail::D_REG, 5)          ,//divided register
        Binatr::Detail(Binatr::Detail::D_REG, 5)           //target register
    }));
    instrucion_set[A64_CMP_R] = ArgIndexedArray<Binatr>(Binatr({ //TODO(ch): there is no CMP_R instruction in ARM processors, it's prespecialized version of CMP(shifted register). We must make switchers much more flexible and functional to support real CMP. Specialization is: 64 register, zero shift.
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x758,11),//11101011000
        Binatr::Detail(Binatr::Detail::D_REG, 5)          ,//cmp1 register
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x0,6)   ,//000000
        Binatr::Detail(Binatr::Detail::D_REG, 5)          ,//cmp2 register
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x1F,5)  //11111
    }));
    instrucion_set[A64_B_LT] = ArgIndexedArray<Binatr>(Binatr({ //TODO(ch): there is no B_LT instruction in ARM processors, it's prespecialized version of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x54,8),//01010100
        Binatr::Detail(Binatr::Detail::D_OFFSET, 19)    ,//jump offset
        Binatr::Detail(Binatr::Detail::D_STATIC, 0xB,5)  //01011
    }));
    instrucion_set[A64_B_LE] = ArgIndexedArray<Binatr>(Binatr({ //TODO(ch): there is no B_LE instruction in ARM processors, it's prespecialized version of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x54,8),//01010100
        Binatr::Detail(Binatr::Detail::D_OFFSET, 19)    ,//jump offset
        Binatr::Detail(Binatr::Detail::D_STATIC, 0xD,5)  //01101
    }));
    instrucion_set[A64_B_GT] = ArgIndexedArray<Binatr>(Binatr({ //TODO(ch): there is no B_GT instruction in ARM processors, it's prespecialized version of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x54,8),//01010100
        Binatr::Detail(Binatr::Detail::D_OFFSET, 19)    ,//jump offset
        Binatr::Detail(Binatr::Detail::D_STATIC, 0xC,5)  //01100
    }));
    instrucion_set[A64_B_GE] = ArgIndexedArray<Binatr>(Binatr({ //TODO(ch): there is no B_GE instruction in ARM processors, it's prespecialized version of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x54,8),//01010100
        Binatr::Detail(Binatr::Detail::D_OFFSET, 19)    ,//jump offset
        Binatr::Detail(Binatr::Detail::D_STATIC, 0xA,5)  //01010
    }));
    instrucion_set[A64_RET] = ArgIndexedArray<Binatr>(Binatr({ //TODO(ch): It's good for macos, but i'm not sure, it's not defined by calling convention. Must check.
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x3597C0,22),//1101011001011111000000
        Binatr::Detail(Binatr::Detail::D_REG, 5)             ,//reg register
        Binatr::Detail(Binatr::Detail::D_STATIC, 0x0,5)       //00000
    }));
};

inline M2bMap& get_instrucion_set()
{
    if(instrucion_set.empty())
        init_instrucion_set();
    return instrucion_set;
}

M2mMap target_mnemonics;
void init_target_mnemonics()
{
    {//OP_LOAD
        Mnemotr loadbase(A64_LDRSW, {
            Mnemotr::Argutr(Arg(0)),
            Mnemotr::Argutr(size_t(2)),
            Mnemotr::Argutr(size_t(0))
            });
        Mnemotr loadoffset(A64_LDRSW, {
            Mnemotr::Argutr(size_t(3)),
            Mnemotr::Argutr(size_t(2)),
            Mnemotr::Argutr(size_t(0))
            });
        ArgIndexedArray<Mnemotr> load_vars;
        load_vars.m_condition = ArgIndexedArray<Binatr>::C_ARRSIZE;
        load_vars.m_branches[3] = ArgIndexedArray<Mnemotr>(loadbase);
        load_vars.m_branches[4] = ArgIndexedArray<Mnemotr>(loadoffset);
        target_mnemonics[OP_LOAD] = load_vars;
    }

    //TODO(ch): Add branch check: op.args[0].value must be equal to TYPE_I32. Same for many and many and many operations.
    target_mnemonics[OP_STORE] = ArgIndexedArray<Mnemotr>( Mnemotr(A64_STR32_I, {
        Mnemotr::Argutr(Arg(0)),
        Mnemotr::Argutr(size_t(1)),
        Mnemotr::Argutr(size_t(2))
    }));

    target_mnemonics[OP_MOV] = ArgIndexedArray<Mnemotr>( Mnemotr(A64_MOV, {
        Mnemotr::Argutr(size_t(1)),
        Mnemotr::Argutr(size_t(0))
    }) );

    target_mnemonics[OP_ADD] = ArgIndexedArray<Mnemotr>( Mnemotr(A64_ADD_R, {
        Mnemotr::Argutr(size_t(2)),
        Mnemotr::Argutr(size_t(1)),
        Mnemotr::Argutr(size_t(0))
    }) );

    target_mnemonics[OP_MUL] = ArgIndexedArray<Mnemotr>( Mnemotr(A64_MUL, {
        Mnemotr::Argutr(size_t(2)),
        Mnemotr::Argutr(size_t(1)),
        Mnemotr::Argutr(size_t(0))
    }) );

    target_mnemonics[OP_DIV] = ArgIndexedArray<Mnemotr>( Mnemotr(A64_SDIV, {
        Mnemotr::Argutr(size_t(2)),
        Mnemotr::Argutr(size_t(1)),
        Mnemotr::Argutr(size_t(0))
    }) );

    target_mnemonics[OP_RET] = ArgIndexedArray<Mnemotr>( Mnemotr(A64_RET, {
        Mnemotr::Argutr(arg_hidden_reg_constructor(30)),
    }) );

};

inline M2mMap& get_target_mnemonics()
{
    if(target_mnemonics.empty())
        init_target_mnemonics();
    return target_mnemonics;
}

Aarch64Backend::Aarch64Backend()
{
    m_2binary = get_instrucion_set();
    m_2tararch = get_target_mnemonics();
    m_exeAlloc = Allocator::getInstance();
    m_isLittleEndianInstructions = true;
    m_isMonowidthInstruction = true;
    m_instructionWidth = 4;

}

bool Aarch64Backend::handleBytecodeOp(const Syntop& a_btop, Syntfunc& a_formingtarget) const
{
    switch (a_btop.opcode)
    {
        case (OP_CMP_LT):
        case (OP_CMP_LE):
        case (OP_CMP_GT):
        {
            //TODO(ch): IMPORTANT. We don't have mechanism for handling complex conditions at all. Behavior difference is defined in B.cond. That's why we are ommiting target register and almost ommiting type of comparisson. For now I don't know solution and pipeline stage to be transformed(I think, it's possible to do in bytecode, at stage of initial program forming).
            if (a_btop.size() != 3 || a_btop.args[1].tag != Arg::IREG)
                throw std::string("Wrong CMP format.");
            if (a_btop.args[2].tag != Arg::IREG)
                throw std::string("Unsupported CMP format.");
            if(a_btop.opcode != OP_CMP_LT && a_btop.opcode != OP_CMP_GT)
                throw std::string("Unsupported CMP type");
            m_lastCondition = a_btop.opcode;
            Arg cmp1 = translateReg(a_btop.args[1].idx); //TODO(ch): Must exist.
            Arg cmp2 = translateReg(a_btop.args[2].idx); //TODO(ch): Must exist.
            a_formingtarget.m_program.emplace_back(A64_CMP_R, std::initializer_list<Arg>({cmp2, cmp1})); //TODO(ch): I'm not sure about inversed argument order.
            return true;
        }
        case (OP_JNE):
        case (OP_JZ):
        {
            if (a_btop.size() != 2 || a_btop.args[0].tag != Arg::IREG || a_btop.args[1].tag != Arg::ICONST)
                throw std::string("Wrong CMP format.");
            if (a_btop.opcode == OP_JNE)
            {
                if(m_lastCondition == OP_CMP_LT)
                    m_lastCondition = OP_CMP_GE;
                else if (m_lastCondition == OP_CMP_GT)
                    m_lastCondition = OP_CMP_LE;
            }
            if (m_lastCondition != OP_CMP_LT && m_lastCondition != OP_CMP_GT && m_lastCondition != OP_CMP_LE && m_lastCondition != OP_CMP_GE)
                throw std::string("Unsupported B. condition");
            m_labelRefMap[a_btop.args[1].value].emplace_back(a_formingtarget.m_program.size(), 0, getM2mCurrentOffset());
            int targetop = (m_lastCondition == OP_CMP_LT) ? A64_B_LT : (
                           (m_lastCondition == OP_CMP_GT) ? A64_B_GT : (
                           (m_lastCondition == OP_CMP_GE) ? A64_B_GE :
                                                            A64_B_LE ));
            m_lastCondition = 0; //Avoiding ability to use it twice.
            a_formingtarget.m_program.emplace_back(targetop, std::initializer_list<Arg>({getM2mCurrentOffset()}));
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
    m_nextidx = 0;
    m_regMap.clear();
    m_retReg = a_bcfunc.m_retreg;
    m_labelMap.clear();//labels offsets from start.
    m_labelRefMap.clear(); // label referenes map is needed to calculate and put in relative offsets after
    m_lastCondition = 0;
    Syntfunc result = BackendImpl::bytecode2Target(a_bcfunc);
    for(auto label: m_labelRefMap)
    {
        if(m_labelMap.count(label.first) == 0)
            throw std::string("Reference to unknown label");
        const int64_t loff = static_cast<int64_t>(m_labelMap[label.first]);
        for(label_ref_info& lref : label.second)
        {
            if (lref.opnum >= result.m_program.size())
                throw std::string("Internal error: operation number is too big");
            if (lref.argnum >= result.m_program[lref.opnum].size())
                throw std::string("Internal error: operation don't have so much arguments");
            if (result.m_program[lref.opnum].args[lref.argnum].tag != Arg::ICONST)
                throw std::string("Internal error: operation don't have so much arguments");
            int64_t& opoff = result.m_program[lref.opnum].args[lref.argnum].value;
            opoff = (loff - opoff) / 4; //AArch64 supports only multiply-4 offsets, so, for compactification, they are divided by 4. //TODO(ch): shift?
        }
    }
    return result;
}

Arg Aarch64Backend::translateReg(IRegInternal tofind) const
{
    if(tofind == m_retReg)
        return arg_hidden_reg_constructor(0); //TODO(ch): It's sad, but even on this very initial stage we have to analyse definition chains to ensure, that we are not rewriting return register(x0) earlier, than we have to read from it. Now we don't have this analysis. Ts bad and risky(simple so-so alternative is automatically copy x0 registrer to virtual argument and reassign it everywhere).
    size_t idx;
    if(m_regMap.count(tofind) != 0)
    {
        idx = m_regMap.at(tofind);
    }
    else
    {
        idx = m_nextidx++;
        m_regMap[tofind] = idx;
    }
    if (idx >= 30)
       throw std::string("Insufficient registers");
    return arg_hidden_reg_constructor(idx);
}
std::unordered_map<int, std::string> Aarch64Backend::getOpStrings() const
{
    return std::unordered_map<int, std::string>({{A64_LDRSW, "ldrsw"}, {A64_STR32_I, "str.32.i"}, {A64_MOV, "mov"}, {A64_ADD_R, "add.r"}, {A64_MUL, "mul"}, {A64_SDIV, "sdiv"}, {A64_CMP_R, "cmp.r"}, {A64_B_LT, "b.lt"}, {A64_B_GT, "b.gt"}, {A64_B_GE, "b.ge"}, {A64_B_LE, "b.le"}, {A64_RET, "ret"}});
}
};
