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

m2b_map instrucion_set;
void init_instrucion_set()

{
    { //LDRSW:
        //TODO(ch): Ldrsw also have many options to be added here. Literal loading, register shift or post/pre-indexing. Format isn't full now and hardcoded.
        binatr ldrswr({
            binatr::detail(binatr::detail::D_STATIC, 0x5C5,11),//10111000100
            binatr::detail(binatr::detail::D_REG,5)           ,//offset register
            binatr::detail(binatr::detail::D_STATIC, 0x1A,6)  ,//options field (presetted to 011010)
            binatr::detail(binatr::detail::D_REG,5)           ,//base register
            binatr::detail(binatr::detail::D_REG,5)            //target register
        });
        binatr ldrswi({
            binatr::detail(binatr::detail::D_STATIC, 0x5C4,11),//10111000011
            binatr::detail(binatr::detail::D_CONST, 9)        ,//offset constant
            binatr::detail(binatr::detail::D_STATIC, 0x1,2)   ,//01
            binatr::detail(binatr::detail::D_REG,5)           ,//base register
            binatr::detail(binatr::detail::D_REG,5)            //target register
        });
        arg_indexed_array<binatr> ldrsw_vars;
        ldrsw_vars.m_condition = arg_indexed_array<binatr>::C_ARGTYPE;
        ldrsw_vars.m_argnum = 0;
        ldrsw_vars.m_branches[Arg::IREG] = arg_indexed_array<binatr>(ldrswr);
        ldrsw_vars.m_branches[Arg::ICONST] = arg_indexed_array<binatr>(ldrswi);
        instrucion_set[A64_LDRSW] = ldrsw_vars;
    }
    instrucion_set[A64_STR32_I] = arg_indexed_array<binatr>(binatr({ //TODO(ch): there is no STR32_I instruction in ARM processors, it's prespecialized version of STR(immediate). We must make switchers much more flexible and functional to support real STR. Specialization is: 32 register
        binatr::detail(binatr::detail::D_STATIC, 0x5C0,11),//10111000000
                                                                   
        binatr::detail(binatr::detail::D_CONST, 9)        ,//offset constant
        binatr::detail(binatr::detail::D_STATIC, 0x3,2)   ,//options field (presetted to 011010)
        binatr::detail(binatr::detail::D_REG,5)           ,//target-address register
        binatr::detail(binatr::detail::D_REG,5)            //unloaded register
    }));
    {//MOV
        //TODO(ch): There is a lot variants of move: stack pointer mov, bitmask, inverted. There offered just a part of possibilities. Also, even register and wide immediate variants are specialized and hardcoded: Specialization is: 64 register.
        binatr movi({
            binatr::detail(binatr::detail::D_STATIC, 0x694,11),//11010010100
            binatr::detail(binatr::detail::D_CONST, 16)       ,//constant fill reg
            binatr::detail(binatr::detail::D_REG, 5)           //target register
        });
        binatr movr({ 
            binatr::detail(binatr::detail::D_STATIC, 0x550,11),//10101010000
            binatr::detail(binatr::detail::D_REG, 5)          ,//source register
            binatr::detail(binatr::detail::D_STATIC, 0x1F ,11),//00000011111
            binatr::detail(binatr::detail::D_REG, 5)           //target register
        });
        arg_indexed_array<binatr> mov_vars;
        mov_vars.m_condition = arg_indexed_array<binatr>::C_ARGTYPE;
        mov_vars.m_argnum = 0;
        mov_vars.m_branches[Arg::IREG] = arg_indexed_array<binatr>(movr);
        mov_vars.m_branches[Arg::ICONST] = arg_indexed_array<binatr>(movi);
        instrucion_set[A64_MOV] = mov_vars;

//        snippets[A64_MOV_I] = arg_indexed_array<i_snippet>(movi);
//        snippets[A64_MOV_R] = arg_indexed_array<i_snippet>(movr);
        
    }
    instrucion_set[A64_ADD_R] = arg_indexed_array<binatr>(binatr({ //TODO(ch): there is no ADD_R instruction in ARM processors, it's prespecialized version of ADD(shifted register). We must make switchers much more flexible and functional to support real ADD. Specialization is: 64 registers, noshift
        binatr::detail(binatr::detail::D_STATIC, 0x458,11),//1000000100
        binatr::detail(binatr::detail::D_REG, 5)          ,//addendum1 register
        binatr::detail(binatr::detail::D_STATIC, 0, 6)    ,//increment register
        binatr::detail(binatr::detail::D_REG, 5)          ,//addendum2 register
        binatr::detail(binatr::detail::D_REG, 5)           //target register
    }));
    instrucion_set[A64_MUL] = arg_indexed_array<binatr>(binatr({ //TODO(ch): this is specialized version of MUL instruction. We must make switchers much more flexible and functional to support real MUL. Specialization is: 64 registers.
        binatr::detail(binatr::detail::D_STATIC, 0x4D8,11),//10011011000
        binatr::detail(binatr::detail::D_REG, 5)          ,//multiplier register
        binatr::detail(binatr::detail::D_STATIC, 0x1F, 6) ,//
        binatr::detail(binatr::detail::D_REG, 5)          ,//multiplied register
        binatr::detail(binatr::detail::D_REG, 5)           //target register
    }));
    instrucion_set[A64_SDIV] = arg_indexed_array<binatr>(binatr({ //TODO(ch): this is specialized version of SDIV instruction. We must make switchers much more flexible and functional to support real SDIV. Specialization is: 64 registers.
        binatr::detail(binatr::detail::D_STATIC, 0x4D6,11),//10011010110
        binatr::detail(binatr::detail::D_REG, 5)          ,//divisor register
        binatr::detail(binatr::detail::D_STATIC, 0x3, 6) ,//
        binatr::detail(binatr::detail::D_REG, 5)          ,//divided register
        binatr::detail(binatr::detail::D_REG, 5)           //target register
    }));
    instrucion_set[A64_CMP_R] = arg_indexed_array<binatr>(binatr({ //TODO(ch): there is no CMP_R instruction in ARM processors, it's prespecialized version of CMP(shifted register). We must make switchers much more flexible and functional to support real CMP. Specialization is: 64 register, zero shift.
        binatr::detail(binatr::detail::D_STATIC, 0x758,11),//11101011000
        binatr::detail(binatr::detail::D_REG, 5)          ,//cmp1 register
        binatr::detail(binatr::detail::D_STATIC, 0x0,6)   ,//000000
        binatr::detail(binatr::detail::D_REG, 5)          ,//cmp2 register
        binatr::detail(binatr::detail::D_STATIC, 0x1F,5)  //11111
    }));
    instrucion_set[A64_B_LT] = arg_indexed_array<binatr>(binatr({ //TODO(ch): there is no B_LT instruction in ARM processors, it's prespecialized version of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
        binatr::detail(binatr::detail::D_STATIC, 0x54,8),//01010100
        binatr::detail(binatr::detail::D_OFFSET, 19)    ,//jump offset
        binatr::detail(binatr::detail::D_STATIC, 0xB,5)  //01011
    }));
    instrucion_set[A64_B_LE] = arg_indexed_array<binatr>(binatr({ //TODO(ch): there is no B_LE instruction in ARM processors, it's prespecialized version of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
        binatr::detail(binatr::detail::D_STATIC, 0x54,8),//01010100
        binatr::detail(binatr::detail::D_OFFSET, 19)    ,//jump offset
        binatr::detail(binatr::detail::D_STATIC, 0xD,5)  //01101
    }));
    instrucion_set[A64_B_GT] = arg_indexed_array<binatr>(binatr({ //TODO(ch): there is no B_GT instruction in ARM processors, it's prespecialized version of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
        binatr::detail(binatr::detail::D_STATIC, 0x54,8),//01010100
        binatr::detail(binatr::detail::D_OFFSET, 19)    ,//jump offset
        binatr::detail(binatr::detail::D_STATIC, 0xC,5)  //01100
    }));
    instrucion_set[A64_B_GE] = arg_indexed_array<binatr>(binatr({ //TODO(ch): there is no B_GE instruction in ARM processors, it's prespecialized version of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
        binatr::detail(binatr::detail::D_STATIC, 0x54,8),//01010100
        binatr::detail(binatr::detail::D_OFFSET, 19)    ,//jump offset
        binatr::detail(binatr::detail::D_STATIC, 0xA,5)  //01010
    }));
    instrucion_set[A64_RET] = arg_indexed_array<binatr>(binatr({ //TODO(ch): It's good for macos, but i'm not sure, it's not defined by calling convention. Must check.
        binatr::detail(binatr::detail::D_STATIC, 0x3597C0,22),//1101011001011111000000
        binatr::detail(binatr::detail::D_REG, 5)             ,//reg register
        binatr::detail(binatr::detail::D_STATIC, 0x0,5)       //00000
    }));
};

inline m2b_map& get_instrucion_set()
{
    if(instrucion_set.empty())
        init_instrucion_set();
    return instrucion_set;
}

m2m_map target_mnemonics;
void init_target_mnemonics()
{
    {//OP_LOAD
        mnemotr loadbase(A64_LDRSW, {
            mnemotr::argutr(Arg(0)),
            mnemotr::argutr(size_t(2)),
            mnemotr::argutr(size_t(0))
            });
        mnemotr loadoffset(A64_LDRSW, {
            mnemotr::argutr(size_t(3)),
            mnemotr::argutr(size_t(2)),
            mnemotr::argutr(size_t(0))
            });
        arg_indexed_array<mnemotr> load_vars;
        load_vars.m_condition = arg_indexed_array<binatr>::C_ARRSIZE;
        load_vars.m_branches[3] = arg_indexed_array<mnemotr>(loadbase);
        load_vars.m_branches[4] = arg_indexed_array<mnemotr>(loadoffset);
        target_mnemonics[OP_LOAD] = load_vars;
    }

    //TODO(ch): Add branch check: op.args[0].value must be equal to TYPE_I32. Same for many and many and many operations.
    target_mnemonics[OP_STORE] = arg_indexed_array<mnemotr>( mnemotr(A64_STR32_I, {
        mnemotr::argutr(Arg(0)),
        mnemotr::argutr(size_t(1)),
        mnemotr::argutr(size_t(2))
    }));

    target_mnemonics[OP_MOV] = arg_indexed_array<mnemotr>( mnemotr(A64_MOV, {
        mnemotr::argutr(size_t(1)),
        mnemotr::argutr(size_t(0))
    }) );

    target_mnemonics[OP_AUG_ADD] = arg_indexed_array<mnemotr>( mnemotr(A64_ADD_R, {
        mnemotr::argutr(size_t(1)),
        mnemotr::argutr(size_t(0)),
        mnemotr::argutr(size_t(0))
    }) );

    target_mnemonics[OP_AUG_MUL] = arg_indexed_array<mnemotr>( mnemotr(A64_MUL, {
        mnemotr::argutr(size_t(1)),
        mnemotr::argutr(size_t(0)),
        mnemotr::argutr(size_t(0))
    }) );

    target_mnemonics[OP_AUG_DIV] = arg_indexed_array<mnemotr>( mnemotr(A64_SDIV, {
        mnemotr::argutr(size_t(1)),
        mnemotr::argutr(size_t(0)),
        mnemotr::argutr(size_t(0))
    }) );

    target_mnemonics[OP_RET] = arg_indexed_array<mnemotr>( mnemotr(A64_RET, {
        mnemotr::argutr(arg_hidden_reg_constructor(30)),
    }) );

};

inline m2m_map& get_target_mnemonics()
{
    if(target_mnemonics.empty())
        init_target_mnemonics();
    return target_mnemonics;
}

aarch64_backend::aarch64_backend()
{
    m_2binary = get_instrucion_set();
    m_2tararch = get_target_mnemonics();
}

bool aarch64_backend::handle_bytecodeop(const syntop& a_btop, syntfunc& a_formingtarget) const
{
    switch (a_btop.opcode)
    {
        case (OP_CMP_LT):
        case (OP_CMP_LE):
        case (OP_CMP_GT):
        {
            //TODO(ch): IMPORTANT. We don't have mechanism for handling complex conditions at all. Behavior difference is defined in B.cond. That's why we are ommiting target register and almost ommiting type of comparisson. For now I don't know solution and pipeline stage to be transformed(I think, it's possible to do in bytecode, at stage of initial program forming).
            if (a_btop.args.size() != 3 || a_btop.args[1].tag != Arg::IREG)
                throw std::string("Wrong CMP format.");
            if (a_btop.args[2].tag != Arg::IREG)
                throw std::string("Unsupported CMP format.");
            if(a_btop.opcode != OP_CMP_LT && a_btop.opcode != OP_CMP_GT)
                throw std::string("Unsupported CMP type");
            m_lastcondition = a_btop.opcode;
            Arg cmp1 = translate_reg(a_btop.args[1].idx); //TODO(ch): Must exist.
            Arg cmp2 = translate_reg(a_btop.args[2].idx); //TODO(ch): Must exist.
            a_formingtarget.m_program.emplace_back(A64_CMP_R, std::initializer_list<Arg>({cmp2, cmp1})); //TODO(ch): I'm not sure about inversed argument order.
            return true;
        }
        case (OP_JNE):
        case (OP_JZ):
        {
            if (a_btop.args.size() != 2 || a_btop.args[0].tag != Arg::IREG || a_btop.args[1].tag != Arg::ICONST)
                throw std::string("Wrong CMP format.");
            if (a_btop.opcode == OP_JNE)
            {
                if(m_lastcondition == OP_CMP_LT)
                    m_lastcondition = OP_CMP_GE;
                else if (m_lastcondition == OP_CMP_GT)
                    m_lastcondition = OP_CMP_LE;
            }
            if (m_lastcondition != OP_CMP_LT && m_lastcondition != OP_CMP_GT && m_lastcondition != OP_CMP_LE && m_lastcondition != OP_CMP_GE)
                throw std::string("Unsupported B. condition");
            m_labelrefmap[a_btop.args[1].value].emplace_back(a_formingtarget.m_program.size(), 0, get_m2m_currentoffset());
            int targetop = (m_lastcondition == OP_CMP_LT) ? A64_B_LT : (
                           (m_lastcondition == OP_CMP_GT) ? A64_B_GT : (
                           (m_lastcondition == OP_CMP_GE) ? A64_B_GE :
                                                            A64_B_LE ));
            m_lastcondition = 0; //Avoiding ability to use it twice.
            a_formingtarget.m_program.emplace_back(targetop, std::initializer_list<Arg>({get_m2m_currentoffset()}));
            return true;
        }
        case (OP_LABEL):
        {
            if (a_btop.args.size() != 1 || a_btop.args[0].tag != Arg::ICONST)
                throw std::string("Wrong LABEL format.");
            if(m_labelmap.count(a_btop.args[0].value) != 0)
                throw std::string("Label redefinition");
            m_labelmap[a_btop.args[0].value] = get_m2m_currentoffset();
            return true;
        }
        default:
            return false;
    };
}

void* aarch64_backend::compile(Context* a_ctx, Func* a_func) const
{
    FuncImpl* func = static_cast<FuncImpl*>(a_func);
    syntfunc aarch64code = bytecode2target(func->getData());
    p_canvas canvas2copy;
    canvas2copy.fromsynt(m_2binary, aarch64code.m_program);
    
    allocator* alloc = arm64_alloc::get_instance();
    uint8_t* exebuf = alloc->allocate(canvas2copy.m_buffer.size());
    
    memcpy(exebuf,(void*)&*canvas2copy.m_buffer.begin(), canvas2copy.m_buffer.size()); //TODO(ch): You have to change used adresses before.
    alloc->protect2execution(exebuf);
    return exebuf;
}

syntfunc aarch64_backend::bytecode2target(const syntfunc& a_bcfunc) const
{
    m_nextidx = 0;
    m_regmap.clear();
    m_retreg = a_bcfunc.m_retreg;
    m_labelmap.clear();//labels offsets from start.
    m_labelrefmap.clear(); // label referenes map is needed to calculate and put in relative offsets after
    m_lastcondition = 0;
    syntfunc result = backend_impl::bytecode2target(a_bcfunc);
    for(auto label: m_labelrefmap)
    {
        if(m_labelmap.count(label.first) == 0)
            throw std::string("Reference to unknown label");
        const int64_t loff = static_cast<int64_t>(m_labelmap[label.first]);
        for(label_ref_info& lref : label.second)
        {
            if (lref.m_opnum >= result.m_program.size())
                throw std::string("Internal error: operation number is too big");
            if (lref.m_argnum >= result.m_program[lref.m_opnum].args.size())
                throw std::string("Internal error: operation don't have so much arguments");
            if (result.m_program[lref.m_opnum].args[lref.m_argnum].tag != Arg::ICONST)
                throw std::string("Internal error: operation don't have so much arguments");
            int64_t& opoff = result.m_program[lref.m_opnum].args[lref.m_argnum].value;
            opoff = (loff - opoff) / 4; //AArch64 supports only multiply-4 offsets, so, for compactification, they are divided by 4. //TODO(ch): shift?
        }
    }
    return result;
}

Arg aarch64_backend::translate_reg(IRegInternal tofind) const
{
    if(tofind == m_retreg)
        return arg_hidden_reg_constructor(0); //TODO(ch): It's sad, but even on this very initial stage we have to analyse definition chains to ensure, that we are not rewriting return register(x0) earlier, than we have to read from it. Now we don't have this analysis. Ts bad and risky(simple so-so alternative is automatically copy x0 registrer to virtual argument and reassign it everywhere).
    size_t idx;
    if(m_regmap.count(tofind) != 0)
    {
        idx = m_regmap.at(tofind);
    }
    else
    {
        idx = m_nextidx++;
        m_regmap[tofind] = idx;
    }
    if (idx >= 30)
       throw std::string("Insufficient registers");
    return arg_hidden_reg_constructor(idx);
}
std::unordered_map<int, std::string> aarch64_backend::get_op_strings() const
{
    return std::unordered_map<int, std::string>({{A64_LDRSW, "ldrsw"}, {A64_STR32_I, "str.32.i"}, {A64_MOV, "mov"}, {A64_ADD_R, "add.r"}, {A64_MUL, "mul"}, {A64_SDIV, "sdiv"}, {A64_CMP_R, "cmp.r"}, {A64_B_LT, "b.lt"}, {A64_B_GT, "b.gt"}, {A64_B_GE, "b.ge"}, {A64_B_LE, "b.le"}, {A64_RET, "ret"}});

}

};
