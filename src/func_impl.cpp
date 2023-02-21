/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "func_impl.hpp"
#include "backend.hpp"
#include "composer.hpp"
#include "printer.hpp"
#include "reg_allocator.hpp"
#include <algorithm>
#include <cstring>
#include <iomanip>
#include <deque>
#include <iostream>

namespace loops
{
std::string type_suffixes[] = { //TODO(ch): find a better place for this
    "u8",
    "i8",
    "u16",
    "i16",
    "u32",
    "i32",
    "u64",
    "i64",
    "fp16",
    "bf16",
    "fp32",
    "fp64",
};

std::unordered_map<int, Printer::ColPrinter > opnameoverrules = {
    {OP_LOAD, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "load." << type_suffixes[op.args[0].elemtype];
    }},
    {OP_STORE, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        int _Tp = op.size() == 3 ? op[2].elemtype : op[1].elemtype;
        str << "store." << type_suffixes[_Tp];
    }},
    {VOP_LOAD, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "vld." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_ARM_LD2, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "vld_deinterleave2." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_ARM_LD1, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "vld_lane." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_STORE, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        int _Tp = op.size() == 3 ? op[2].elemtype : op[1].elemtype;
        str << "vst." << type_suffixes[_Tp];
    }},
    {VOP_ARM_ST1, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "vst_lane." << type_suffixes[op.args[1].elemtype];
    }},
    {VOP_GETLANE, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "getlane." << type_suffixes[op.args[1].elemtype];
    }},
    {VOP_SETLANE, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "setlane." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_ADD, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "add." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_SUB, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "sub." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_MUL, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "mul." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_DIV, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "mul." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_FMA, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "fma." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_ARM_EXT, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "ext." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_SAL, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "sal." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_SHL, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "shl." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_SAR, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "sar." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_SHR, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "shr." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_NEG, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "neg." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_MIN, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "min." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_MAX, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "max." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_GT, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "gt." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_GE, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "ge." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_LT, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "lt." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_LE, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "le." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_NE, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "ne." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_EQ, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "eq." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_TRUNC, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "trunc." << type_suffixes[op.args[1].elemtype] << "_" << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_FLOOR, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "floor." << type_suffixes[op.args[1].elemtype] << "_" << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_CAST, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "cast." << type_suffixes[op.args[1].elemtype] << "_" << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_BROADCAST, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "broadcast." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_CAST_LOW, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "cast." << type_suffixes[op.args[0].elemtype]<<".from."<<type_suffixes[op.args[1].elemtype]<<".low";
    }},
    {VOP_CAST_HIGH, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "cast." << type_suffixes[op.args[0].elemtype]<<".from."<<type_suffixes[op.args[1].elemtype]<<".high";
    }},
    {VOP_SHRINK_LOW, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "shrink." << type_suffixes[op.args[0].elemtype]<<".from."<<type_suffixes[op.args[1].elemtype]<<".low";
    }},
    {VOP_SHRINK_HIGH, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "shrink." << type_suffixes[op.args[0].elemtype]<<".from."<<type_suffixes[op.args[1].elemtype]<<".high";
    }},
    {VOP_REDUCE_MAX, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "reduce.max." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_REDUCE_MIN, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "reduce.max." << type_suffixes[op.args[0].elemtype];
    }},
    {OP_LABEL, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        if (op.size() != 1 || op.args[0].tag != Arg::IIMMEDIATE)
            throw std::runtime_error("Wrong LABEL format");
        str << "label " << op.args[0] << ":";
    }},
    {VOP_DEF, [](::std::ostream& str, const Syntop& op, size_t, Backend*){
        str << "vdef." << type_suffixes[op.args[0].elemtype];
    }},
};

std::unordered_map<int, Printer::ColPrinter > argoverrules = {
    {OP_LABEL, [](::std::ostream& str, const Syntop& op, size_t, Backend*){}},
    {VOP_DEF, [](::std::ostream& str, const Syntop& op, size_t, Backend*){ str<<op[0];}}, //TODO(ch): this is a workaround for providing context to newiop<...> with no arguments.
};

std::unordered_map<int, std::string> opstrings = { //TODO(ch): will you create at every print?
    {OP_MOV,      "mov"},
    {OP_XCHG,     "xchg"},
    {OP_ADD,      "add"},
    {OP_MUL,      "mul"},
    {OP_SUB,      "sub"},
    {OP_DIV,      "div"},
    {OP_MOD,      "mod"},
    {OP_SHL,      "shl"},
    {OP_SHR,      "shr"},
    {OP_SAR,      "sar"},
    {OP_AND,      "and"},
    {OP_OR,       "or"},
    {OP_XOR,      "xor"},
    {OP_NOT,      "not"},
    {OP_NEG,      "neg"},
    {OP_CMP,      "cmp"},
    {OP_SELECT,   "select"},
    {OP_MIN,      "min"},
    {OP_MAX,      "max"},
    {OP_ABS,      "abs"},
    {OP_SIGN,     "sign"},
    {OP_JMP,      "jmp"},
    {OP_JMP_EQ,   "jmp_eq"},
    {OP_JMP_NE,   "jmp_ne"},
    {OP_JMP_GE,   "jmp_ge"},
    {OP_JMP_LE,   "jmp_le"},
    {OP_JMP_ULE,  "jmp_ule"},
    {OP_JMP_GT,   "jmp_gt"},
    {OP_JMP_UGT,  "jmp_ugt"},
    {OP_JMP_LT,   "jmp_gt"},
    {OP_SPILL,    "spill"},
    {OP_UNSPILL,  "unspill"},
    {OP_IF,       "annotation:if"},
    {OP_ELSE,     "annotation:else"},
    {OP_ENDIF,    "annotation:endif"},
    {OP_WHILE,    "annotation:while"},
    {OP_ENDWHILE, "annotation:endwhile"},
    {OP_BREAK,    "annotation:break"},
    {OP_CONTINUE, "annotation:continue"},
    {VOP_AND,     "and"},
    {VOP_OR,      "or"},
    {VOP_XOR,     "xor"},
    {VOP_NOT,     "not"},
    {VOP_SELECT,  "select"},
    {OP_RET,      "ret"},
    {OP_X86_ADC,  "x86_adc"},
    {OP_X86_CQO,  "x86_cqo"},
    {OP_ARM_CINC, "arm_cinc"},
    {OP_ARM_CNEG, "arm_cneg"},
    {OP_ARM_MOVK, "arm_movk"},
    {OP_DEF,      "def"},
};

FuncImpl::FuncImpl(const std::string& name, ContextImpl* ctx, std::initializer_list<IReg*> params) : m_refcount(0) //TODO(ch): support vector parameters
    , m_pipeline(std::make_shared<Pipeline>(ctx->getBackend(), this, name, params))
    , m_context(ctx)
    , m_compiled(nullptr)
    , m_directTranslation(false)
{
}

Func FuncImpl::makeWrapper(const std::string& name, ContextImpl* ctx, std::initializer_list<IReg*> params)
{
    return Func::make(new FuncImpl(name, ctx, params));
}

void* FuncImpl::ptr()
{
    if(m_compiled == nullptr)
    {
        Assert(m_pipeline.get());
        FuncBodyBuf body = get_hex_body();
        Backend* backend = m_context->getBackend();
        Allocator* alloc = backend->getAllocator();
        m_compiled = alloc->allocate(body->size());
        memcpy(m_compiled, (void*)&(body->operator[](0)), body->size()); //TODO(ch): You have to change used adressess before.
        alloc->protect2Execution((uint8_t*)m_compiled);
    }
    return m_compiled;
}

void FuncImpl::overrideRegisterSet(int basketNum, const std::vector<size_t>& a_parameterRegisters,
    const std::vector<size_t>& a_returnRegisters,
    const std::vector<size_t>& a_callerSavedRegisters,
    const std::vector<size_t>& a_calleeSavedRegisters)
{
    m_pipeline->overrideRegisterSet(basketNum, a_parameterRegisters, a_returnRegisters, a_callerSavedRegisters, a_calleeSavedRegisters);
}

FuncImpl* FuncImpl::verifyArgs_(std::initializer_list<Recipe> args)
{
    FuncImpl* res = nullptr;
    for(Recipe rcp : args)
    {
        if(rcp.opcode() == RECIPE_LEAF)
        {
            FuncImpl* pretender = (FuncImpl*)(rcp.leaf().func);
            if(pretender != nullptr)
            {
                if(res == nullptr)
                    res = pretender;
                else if(res != pretender)
                    throw std::runtime_error("Registers of different functions as arguments of one expression.");
            }        
        }
        else for(int child_num = 0; child_num < rcp.children().size(); child_num++)
        {
            FuncImpl* pretender = verifyArgs_({rcp.children()[child_num]});
            if(pretender != nullptr)
            {
                if(res == nullptr)
                    res = pretender;
                else if(res != pretender)
                    throw std::runtime_error("Registers of different functions as arguments of one expression.");
            }
        }
    }
    return res;
}

void FuncImpl::printBytecode(std::ostream& out, int uptoStage)
{
    Pipeline l_pipeline(*(m_context->debug_mode() ? m_debug_pipeline.get(): m_pipeline.get()));
    l_pipeline.run_until(uptoStage);
    Printer printer({Printer::colNumPrinter(0), Printer::colOpnamePrinter(opstrings, opnameoverrules), Printer::colArgListPrinter(l_pipeline.get_data(), argoverrules)});
    printer.print(out, l_pipeline.get_data());
}

void FuncImpl::printAssembly(std::ostream& out, int columns)
{
    Pipeline l_pipeline(*(m_context->debug_mode() ? m_debug_pipeline.get(): m_pipeline.get()));
    l_pipeline.run_until_including(CS_BYTECODE_TO_ASSEMBLY);
    
    Backend* backend = m_context->getBackend();
    std::vector<Printer::ColPrinter> columnPrs;
    columnPrs.reserve(3);
    if(columns&PC_OPNUM)
        columnPrs.push_back(Printer::colNumPrinter(0));
    if(columns&PC_OP)
    {
        columnPrs.push_back(Printer::colOpnamePrinter(backend->getOpStrings()));
        columnPrs.push_back(Printer::colArgListPrinter(l_pipeline.get_data()));
    }
    if(columns&PC_HEX)
    {
        if(columns&PC_OP)
            columnPrs.push_back(Printer::colDelimeterPrinter());
        columnPrs.push_back(backend->colHexPrinter(l_pipeline.get_data()));
    }
    Printer printer(columnPrs);
    printer.setBackend(backend);
    printer.print(out, l_pipeline.get_data());
}

FuncImpl* FuncImpl::verifyArgs(std::initializer_list<Recipe> args)
{
    FuncImpl* res = verifyArgs_(args);
    if (res == nullptr)
        throw std::runtime_error("Cannot find mother function in expression arguments.");
    return res;
}

const Syntfunc& FuncImpl::get_data() const
{
    if(m_pipeline.get())
        return m_pipeline->get_data(); 
    AssertMsg(m_context->debug_mode(), "Function is already compiled.");
    return m_debug_pipeline->get_data();
}

const FuncBodyBuf FuncImpl::get_hex_body()
{
    m_pipeline->full_run();
    const FuncBodyBuf res = m_pipeline->result_buffer();
    m_pipeline.reset();
    return res;
}

void FuncImpl::endFunc()
{
    m_pipeline->run_until_including(CS_COLLECTING);
    if (m_directTranslation)
        m_pipeline->pass_until(CS_BYTECODE_TO_ASSEMBLY);
    if (m_context->debug_mode())
        m_debug_pipeline = std::make_shared<Pipeline>(*(m_pipeline.get()));
}
};
