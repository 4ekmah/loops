/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "aarch64.hpp"
#include "composer.hpp"
#include <unordered_map>
#include <unistd.h>
#include <sys/mman.h>
#include <libkern/OSCacheControl.h>

namespace loops
{

std::unordered_map<int, i_snippet> snippets =
{
    {A64_LDRSW_R, i_snippet({ //TODO(ch): there is no LDRSW_R instruction in ARM processors, it's prespecialized version of LDRSW(register). We must make switchers much more flexible and functional to support real LDRSW. Specialization is: no scaling
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x5C5,11),//10111000100
        i_snippet::detail(i_snippet::detail::D_REG,5)           ,//offset register
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x1A,6) ,//options field (presetted to 011010)
        i_snippet::detail(i_snippet::detail::D_REG,5)           ,//base register
        i_snippet::detail(i_snippet::detail::D_REG,5)            //target register
    })},
    {A64_LDRSW_I, i_snippet({ //TODO(ch): this operation is referenced as LDRSW(immediate).
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x5C4,11),//10111000011
        i_snippet::detail(i_snippet::detail::D_CONST, 9)        ,//offset constant
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x1,2)   ,//01
        i_snippet::detail(i_snippet::detail::D_REG,5)           ,//base register
        i_snippet::detail(i_snippet::detail::D_REG,5)            //target register
    })},
    {A64_STR32_I, i_snippet({ //TODO(ch): there is no STR32_I instruction in ARM processors, it's prespecialized version of STR(immediate). We must make switchers much more flexible and functional to support real STR. Specialization is: 32 register
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x5C0,11),//10111000000
                                                                   
        i_snippet::detail(i_snippet::detail::D_CONST, 9)        ,//offset constant
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x3,2)   ,//options field (presetted to 011010)
        i_snippet::detail(i_snippet::detail::D_REG,5)           ,//target-address register
        i_snippet::detail(i_snippet::detail::D_REG,5)            //unloaded register
    })},
    {A64_MOV_I, i_snippet({ //TODO(ch): there is no MOV_I instruction in ARM processors, it's prespecialized version of MOV(wide immediate). We must make switchers much more flexible and functional to support real MOV. Specialization is: 64 register
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x694,11),//11010010100
        i_snippet::detail(i_snippet::detail::D_CONST, 16)       ,//constant fill reg
        i_snippet::detail(i_snippet::detail::D_REG, 5)           //target register
    })},
    {A64_MOV_R, i_snippet({ //TODO(ch): there is no MOV_I instruction in ARM processors, it's prespecialized version of MOV. We must make switchers much more flexible and functional to support real MOV. Specialization is: 64 register
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x550,11),//10101010000
        i_snippet::detail(i_snippet::detail::D_REG, 5)          ,//source register
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x1F ,11),//00000011111
        i_snippet::detail(i_snippet::detail::D_REG, 5)           //target register
    })},
    {A64_ADD_R, i_snippet({ //TODO(ch): there is no ADD_R instruction in ARM processors, it's prespecialized version of ADD(shifted register). We must make switchers much more flexible and functional to support real ADD. Specialization is: 64 registers, noshift
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x458,11),//1000000100
        i_snippet::detail(i_snippet::detail::D_REG, 5)          ,//addendum1 register
        i_snippet::detail(i_snippet::detail::D_STATIC, 0, 6)    ,//increment register
        i_snippet::detail(i_snippet::detail::D_REG, 5)          ,//addendum2 register
        i_snippet::detail(i_snippet::detail::D_REG, 5)           //target register
    })},
    {A64_MUL, i_snippet({ //TODO(ch): this is specialized version of MUL instruction. We must make switchers much more flexible and functional to support real MUL. Specialization is: 64 registers.
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x4D8,11),//10011011000
        i_snippet::detail(i_snippet::detail::D_REG, 5)          ,//multiplier register
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x1F, 6) ,//
        i_snippet::detail(i_snippet::detail::D_REG, 5)          ,//multiplied register
        i_snippet::detail(i_snippet::detail::D_REG, 5)           //target register
    })},
    {A64_SDIV, i_snippet({ //TODO(ch): this is specialized version of SDIV instruction. We must make switchers much more flexible and functional to support real SDIV. Specialization is: 64 registers.
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x4D6,11),//10011010110
        i_snippet::detail(i_snippet::detail::D_REG, 5)          ,//divisor register
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x3, 6) ,//
        i_snippet::detail(i_snippet::detail::D_REG, 5)          ,//divided register
        i_snippet::detail(i_snippet::detail::D_REG, 5)           //target register
    })},
    {A64_CMP_R, i_snippet({ //TODO(ch): there is no CMP_R instruction in ARM processors, it's prespecialized version of CMP(shifted register). We must make switchers much more flexible and functional to support real CMP. Specialization is: 64 register, zero shift.
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x758,11),//11101011000
        i_snippet::detail(i_snippet::detail::D_REG, 5)          ,//cmp1 register
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x0,6)   ,//000000
        i_snippet::detail(i_snippet::detail::D_REG, 5)          ,//cmp2 register
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x1F,5)  //11111
    })},
    {A64_B_LT, i_snippet({ //TODO(ch): there is no B_LT instruction in ARM processors, it's prespecialized version of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x54,8),//01010100
        i_snippet::detail(i_snippet::detail::D_OFFSET, 19)    ,//jump offset
        i_snippet::detail(i_snippet::detail::D_STATIC, 0xB,5)  //01011
    })},
    {A64_B_LE, i_snippet({ //TODO(ch): there is no B_LE instruction in ARM processors, it's prespecialized version of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x54,8),//01010100
        i_snippet::detail(i_snippet::detail::D_OFFSET, 19)    ,//jump offset
        i_snippet::detail(i_snippet::detail::D_STATIC, 0xD,5)  //01101
    })},
    {A64_B_GT, i_snippet({ //TODO(ch): there is no B_GT instruction in ARM processors, it's prespecialized version of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x54,8),//01010100
        i_snippet::detail(i_snippet::detail::D_OFFSET, 19)    ,//jump offset
        i_snippet::detail(i_snippet::detail::D_STATIC, 0xC,5)  //01100
    })},
    {A64_B_GE, i_snippet({ //TODO(ch): there is no B_GE instruction in ARM processors, it's prespecialized version of B.cond. We must make switchers much more flexible and functional to support real B.cond. Specialization is: fixed condition.
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x54,8),//01010100
        i_snippet::detail(i_snippet::detail::D_OFFSET, 19)    ,//jump offset
        i_snippet::detail(i_snippet::detail::D_STATIC, 0xA,5)  //01010
    })},
    {A64_RET, i_snippet({ //TODO(ch): It's good for macos, but i'm not sure, it's not defined by calling convention. Must check.
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x3597C0,22),//1101011001011111000000
        i_snippet::detail(i_snippet::detail::D_REG, 5)             ,//reg register
        i_snippet::detail(i_snippet::detail::D_STATIC, 0x0,5)       //00000
    })}
};

inline Arg arg_hidden_reg_constructor(size_t reg) //TODO(ch): What the hell? Create normal walk class and make these functions method of class.
{
    Arg res;
    res.tag = Arg::IREG;
    res.idx = reg;
    return res;
}

Arg arg_find_reg(size_t& next, std::unordered_map<size_t, size_t>& regmap, size_t tofind, IRegInternal a_retreg)
{
    if(tofind == a_retreg)
        return arg_hidden_reg_constructor(0); //TODO(ch): It's sad, but even on this stage we have to analyse definition chains to ensure, that we are not rewriting return register(x0) earlier, than we have to read from it. Now we don't have this analysis. Ts bad and risky(simple so-so alternative is automatically copy x0 registrer to virtual argument and reassign it everywhere).
    size_t idx;
    if(regmap.count(tofind) != 0)
    {
        idx = regmap.at(tofind);
    }
    else
    {
        idx = next++;
        regmap[tofind] = idx;
    }
    if (idx >= 30)
       throw std::string("Insufficient registers");
    return arg_hidden_reg_constructor(idx);
}

struct label_ref_info
{
    size_t m_opnum;
    size_t m_argnum;
    size_t m_opoffset;
    label_ref_info(): m_opnum(0), m_argnum(0), m_opoffset(0) {}
    label_ref_info(size_t a_opnum, size_t a_argnum, size_t a_opoffset): m_opnum(a_opnum), m_argnum(a_argnum), m_opoffset(a_opoffset) {}
};

std::vector<syntop> bytecode2arm64(const std::vector<syntop>& bytecode, const std::vector<IRegInternal>& parameters, IRegInternal a_retreg)
{
    std::vector<syntop> result;
    result.reserve(bytecode.size());
    size_t nextidx = 0;
    std::unordered_map<size_t, size_t> regmap;//Translation map from bytecode indexes
    std::unordered_map<size_t, size_t> labelmap;//labels offsets from start.
    std::unordered_map<size_t, std::vector<label_ref_info> > labelrefmap; // label referenes map is needed to calculate and put in relative offsets after
    for(size_t parreg = 0; parreg<parameters.size(); parreg++ ) //TODO(ch): You need to analyze arguments better.
        arg_find_reg(nextidx, regmap, parreg, a_retreg);

    //TODO(ch): there must be function header definition. We must
    int lastcondition = 0; //TODO(ch): Superkolkhoz
    size_t currentoffset = 0;
    for(const syntop& op: bytecode)
    {
        size_t curr_tar_op = result.size();
        switch (op.opcode)
        {
            case (OP_LOAD):
            {
                if((op.args.size() != 3 && op.args.size() != 4) ||
                    op.args[0].tag != Arg::IREG ||
                    op.args[1].tag != Arg::ICONST)
                    throw std::string("Wrong LOAD format.");
                if (op.args[1].value != TYPE_I32 ||
                    op.args[2].tag != Arg::IREG ||
                    (op.args.size() == 4 && op.args[3].tag != Arg::IREG))
                    throw std::string("AArch64 unsupported LOAD format.");
                Arg target = arg_find_reg(nextidx, regmap, op.args[0].idx, a_retreg);
                Arg address = arg_find_reg(nextidx, regmap, op.args[2].idx, a_retreg); //TODO(ch): must already exist
                if(op.args.size() == 4)
                {
                    Arg offset = arg_find_reg(nextidx, regmap, op.args[3].idx, a_retreg);//TODO(ch): must already exist
                    result.emplace_back(A64_LDRSW_R, std::initializer_list<Arg>({offset, address, target}));
                }
                else
                    result.emplace_back(A64_LDRSW_I, std::initializer_list<Arg>({0, address,target}));
                break;
            }
            case (OP_STORE):
            {
                if (op.args.size() != 3 || op.args[0].tag != Arg::ICONST
                    || op.args[2].tag != Arg::IREG)
                    throw std::string("Wrong STORE format.");
                if (op.args[0].value != TYPE_I32 ||
                    op.args[1].tag != Arg::IREG)
                    throw std::string("AArch64 unsupported STORE format.");
                Arg address = arg_find_reg(nextidx, regmap, op.args[2].idx, a_retreg);
                Arg valreg = arg_find_reg(nextidx, regmap, op.args[1].idx, a_retreg);
                result.emplace_back(A64_STR32_I, std::initializer_list<Arg>({0, valreg, address}));
               break;
            }
            case (OP_MOV):
            {
                if (op.args.size() != 2 || op.args[0].tag != Arg::IREG)
                    throw std::string("Wrong MOV format.");
                int targetop = (op.args[1].tag == Arg::IREG) ? A64_MOV_R : A64_MOV_I;
                Arg target = arg_find_reg(nextidx, regmap, op.args[0].idx, a_retreg);
                Arg source = op.args[1];
                if(source.tag == Arg::IREG)
                    source = arg_find_reg(nextidx, regmap, source.idx, a_retreg); //TODO(ch): There needed version non-tolerate to absence of register in map.
                result.emplace_back(targetop, std::initializer_list<Arg>({source, target})); //TODO:What an argument transition?!?!? Add normal array-based copy constructor.
                break;
            }
            case (OP_CMP_LT):
            case (OP_CMP_LE):
            case (OP_CMP_GT):
            {
                //TODO(ch): IMPORTANT. We don't have mechanism for handling complex conditions at all. Behavior difference is defined in B.cond. That's why we are ommiting target register and almost ommiting type of comparisson. For now I don't know solution and pipeline stage to be transformed(I think, it's possible to do in bytecode, at stage of initial program forming).
                if (op.args.size() != 3 || op.args[1].tag != Arg::IREG)
                    throw std::string("Wrong CMP format.");
                if (op.args[2].tag != Arg::IREG)
                    throw std::string("Unsupported CMP format.");
                if(op.opcode != OP_CMP_LT && op.opcode != OP_CMP_GT)
                    throw std::string("Unsupported CMP type");
                lastcondition = op.opcode;
                Arg cmp1 = arg_find_reg(nextidx, regmap, op.args[1].idx, a_retreg); //TODO(ch): Must exist.
                Arg cmp2 = arg_find_reg(nextidx, regmap, op.args[2].idx, a_retreg); //TODO(ch): Must exist.
                result.emplace_back(A64_CMP_R, std::initializer_list<Arg>({cmp2, cmp1})); //TODO(ch): I'm not sure about inversed argument order.
                break;
            }
            case (OP_AUG_ADD):
            case (OP_AUG_MUL):
            case (OP_AUG_DIV):
            {
                if (op.args.size() != 2 || op.args[0].tag != Arg::IREG || op.args[1].tag != Arg::IREG)
                    throw std::string("Wrong AUG_ADD format.");
                Arg target   = arg_find_reg(nextidx, regmap, op.args[0].idx, a_retreg);
                Arg operand2 = arg_find_reg(nextidx, regmap, op.args[1].idx, a_retreg);
                int targetop = (op.opcode == OP_AUG_ADD) ? A64_ADD_R : (
                               (op.opcode == OP_AUG_MUL) ? A64_MUL   : (
                                                                        A64_SDIV));
                result.emplace_back(targetop, std::initializer_list<Arg>({operand2, target, target}));
                break;
            }
            case (OP_JNE):
            case (OP_JZ):
            {
                if (op.args.size() != 2 || op.args[0].tag != Arg::IREG || op.args[1].tag != Arg::ICONST)
                    throw std::string("Wrong CMP format.");
                if (op.opcode == OP_JNE)
                {
                    if(lastcondition == OP_CMP_LT)
                        lastcondition = OP_CMP_GE;
                    else if (lastcondition == OP_CMP_GT)
                        lastcondition = OP_CMP_LE;
                }
                if (lastcondition != OP_CMP_LT && lastcondition != OP_CMP_GT && lastcondition != OP_CMP_LE && lastcondition != OP_CMP_GE)
                    throw std::string("Unsupported B. condition");
                labelrefmap[op.args[1].value].emplace_back(result.size(), 0, currentoffset);
                int targetop = (lastcondition == OP_CMP_LT) ? A64_B_LT : (
                               (lastcondition == OP_CMP_GT) ? A64_B_GT : (
                               (lastcondition == OP_CMP_GE) ? A64_B_GE :
                                                                A64_B_LE ));
                lastcondition = 0; //Avoiding ability to use it twice.
                result.emplace_back(targetop, std::initializer_list<Arg>({currentoffset}));
                break;
            }
            case (OP_RET):
            {
                result.emplace_back(A64_RET, std::initializer_list<Arg>({arg_hidden_reg_constructor(30)}));
                break;
            }
            case (OP_LABEL):
            {
                if (op.args.size() != 1 || op.args[0].tag != Arg::ICONST)
                    throw std::string("Wrong LABEL format.");
                if(labelmap.count(op.args[0].value) != 0)
                    throw std::string("Label redefinition");
                labelmap[op.args[0].value] = currentoffset;
                break;
            }
            default:
                throw std::string("AArch64: unsupported bytecode instruction.");
        };
        for(size_t addedop = curr_tar_op; addedop<result.size(); addedop++)
            currentoffset += snippets.at(result[addedop].opcode).size();
        
    }
    for(auto label: labelrefmap)
    {
        if(labelmap.count(label.first) == 0)
            throw std::string("Reference to unknown label");
        const int64_t loff = static_cast<int64_t>(labelmap[label.first]);
        for(label_ref_info& lref : label.second)
        {
            if (lref.m_opnum >= result.size())
                throw std::string("Internal error: operation number is too big");
            if (lref.m_argnum >= result[lref.m_opnum].args.size())
                throw std::string("Internal error: operation don't have so much arguments");
            if (result[lref.m_opnum].args[lref.m_argnum].tag != Arg::ICONST)
                throw std::string("Internal error: operation don't have so much arguments");
            int64_t& opoff = result[lref.m_opnum].args[lref.m_argnum].value;
            opoff = (loff - opoff) / 4; //AArch64 supports only multiply-4 offsets, so, for compactification, they are divided by 4. //TODO(ch): shift?
        }
    }
    return result;
}

const instruction_set& aarch64instructionset() {return snippets;}
void* AArch64Compiler::compile(Context* a_ctx, Func* a_func) const
{
    FuncImpl* func = static_cast<FuncImpl*>(a_func);
    std::vector<syntop> aarch64code = bytecode2arm64(func->getProgram(), func->getParams(), func->getRetReg());
    p_canvas canvas2copy;
    canvas2copy.fromsynt(snippets, aarch64code);

    //Allocate memory:
    size_t size = canvas2copy.m_buffer.size();
    size_t pageSize = sysconf(_SC_PAGESIZE);
    const size_t alignedSizeM1 = pageSize - 1;
    size = (size + alignedSizeM1) & ~alignedSizeM1;
#ifdef MAP_ANONYMOUS
    int mode = MAP_PRIVATE | MAP_ANONYMOUS;
#elif defined(MAP_ANON)
    int mode = MAP_PRIVATE | MAP_ANON;
#else
#error "not supported"
#endif
//#ifdef XBYAK_USE_MAP_JIT
    mode |= MAP_JIT;
//#endif
    void *p = mmap(NULL, size, PROT_READ | PROT_WRITE, mode, -1, 0); //TODO(ch): opposite is munmap.
    if (p == MAP_FAILED)
      throw std::string("Memory allocation failure");
    assert(p);
    
    memcpy(p,(void*)&*canvas2copy.m_buffer.begin(), canvas2copy.m_buffer.size()); //TODO(ch): You have to change used adresses before.
    
    size_t iaddr = reinterpret_cast<size_t>(p);
    size_t roundAddr = iaddr & ~(pageSize - static_cast<size_t>(1));

    auto proret = mprotect(reinterpret_cast<void *>(roundAddr), 4096/*size*/ + (iaddr- roundAddr), PROT_READ | PROT_EXEC);
    if(proret != 0)
        throw std::string("Memory protection fail");
    sys_icache_invalidate(p, canvas2copy.m_buffer.size());

    return p;
}

};
