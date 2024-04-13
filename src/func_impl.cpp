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

static std::unordered_map<int, std::string> cond_suffixes = {
    {OP_EQ,      "eq"},
    {OP_NE,      "ne"},
    {OP_GE,      "ge"},
    {OP_LE,      "le"},
    {OP_ULE,     "ule"},
    {OP_GT,      "gt"},
    {OP_UGT,     "ugt"},
    {OP_LT,      "gt"},
    {OP_S,       "s"},
    {OP_NS,      "ns"},
};

std::unordered_map<int, Printer::ColPrinter > opnameoverrules = {
    {OP_LOAD, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "load." << type_suffixes[op.args[0].elemtype];
    }},
    {OP_STORE, [](::std::ostream& str, const Syntop& op, int, Backend*){
        int _Tp = op.size() == 3 ? op[2].elemtype : op[1].elemtype;
        str << "store." << type_suffixes[_Tp];
    }},
    {OP_JCC, [](::std::ostream& str, const Syntop& op, int, Backend*){
        Assert(op.size() == 2 && op[0].tag == Arg::IIMMEDIATE && op[1].tag == Arg::IIMMEDIATE && cond_suffixes.find((int)(op[0].value)) != cond_suffixes.end());
        str << "jmp_" << cond_suffixes[(int)op[0].value] << " "<< op[1].value;
    }},
    {OP_LABEL, [](::std::ostream& str, const Syntop& op, int, Backend*){
        if (op.size() != 1 || op.args[0].tag != Arg::IIMMEDIATE)
            throw std::runtime_error("Wrong LABEL format");
        str << "label " << op.args[0] << ":";
    }},
    {OP_SELECT, [](::std::ostream& str, const Syntop& op, int, Backend*){
        Assert(op.size() == 4 && op[1].tag == Arg::IIMMEDIATE && cond_suffixes.find((int)op[1].value) != cond_suffixes.end());
        str << "select_" << cond_suffixes[(int)op[1].value];
    }},
    {OP_IVERSON, [](::std::ostream& str, const Syntop& op, int, Backend*){
        Assert(op.size() == 2 && op[1].tag == Arg::IIMMEDIATE && cond_suffixes.find((int)op[1].value) != cond_suffixes.end());
        str << "iverson_" << cond_suffixes[(int)op[1].value];
    }},
    {VOP_LOAD, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "vld." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_ARM_LD1, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "vld_lane." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_ARM_LD2, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "vld_deinterleave2." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_STORE, [](::std::ostream& str, const Syntop& op, int, Backend*){
        int _Tp = op.size() == 3 ? op[2].elemtype : op[1].elemtype;
        str << "vst." << type_suffixes[_Tp];
    }},
    {VOP_ARM_ST1, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "vst_lane." << type_suffixes[op.args[1].elemtype];
    }},
    {VOP_GETLANE, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "getlane." << type_suffixes[op.args[1].elemtype];
    }},
    {VOP_SETLANE, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "setlane." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_ADD, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "add." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_SUB, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "sub." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_MUL, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "mul." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_DIV, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "mul." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_FMA, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "fma." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_ARM_EXT, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "ext." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_SAL, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "sal." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_SHL, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "shl." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_SAR, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "sar." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_SHR, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "shr." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_NEG, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "neg." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_MIN, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "min." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_MAX, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "max." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_GT, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "gt." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_GE, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "ge." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_LT, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "lt." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_LE, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "le." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_NE, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "ne." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_EQ, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "eq." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_TRUNC, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "trunc." << type_suffixes[op.args[1].elemtype] << "_" << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_FLOOR, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "floor." << type_suffixes[op.args[1].elemtype] << "_" << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_CAST, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "cast." << type_suffixes[op.args[1].elemtype] << "_" << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_BROADCAST, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "broadcast." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_CAST_LOW, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "cast." << type_suffixes[op.args[0].elemtype]<<".from."<<type_suffixes[op.args[1].elemtype]<<".low";
    }},
    {VOP_CAST_HIGH, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "cast." << type_suffixes[op.args[0].elemtype]<<".from."<<type_suffixes[op.args[1].elemtype]<<".high";
    }},
    {VOP_SHRINK, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "shrink." << type_suffixes[op.args[0].elemtype]<<".from."<<type_suffixes[op.args[1].elemtype];
    }},
    {VOP_POPCOUNT, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "popcount." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_ARM_SHRINK_LOW, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "arm_shrink." << type_suffixes[op.args[0].elemtype]<<".from."<<type_suffixes[op.args[1].elemtype]<<".low";
    }},
    {VOP_ARM_SHRINK_HIGH, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "arm_shrink." << type_suffixes[op.args[0].elemtype]<<".from."<<type_suffixes[op.args[1].elemtype]<<".high";
    }},
    {VOP_REDUCE_MAX, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "reduce.max." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_REDUCE_MIN, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "reduce.max." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_REDUCE_SUM, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "reduce.sum." << type_suffixes[op.args[0].elemtype];
    }},
    {VOP_REDUCE_WSUM, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "reduce.wmax" << type_suffixes[op.args[0].elemtype]<<".from."<<type_suffixes[op.args[1].elemtype];
    }},
    {VOP_DEF, [](::std::ostream& str, const Syntop& op, int, Backend*){
        str << "vdef." << type_suffixes[op.args[0].elemtype];
    }},
};

std::unordered_map<int, Printer::ColPrinter > argoverrules = {
    {OP_LABEL, [](::std::ostream& /*str*/, const Syntop& /*op*/, int, Backend*){}},
    {OP_JCC, [](::std::ostream& /*str*/, const Syntop& /*op*/, int, Backend*){}},
    {VOP_DEF, [](::std::ostream& str, const Syntop& op, int, Backend*){ str<<op[0]; }}, //TODO(ch): this is a workaround for providing context to newiop<...> with no arguments.
    {OP_CALL, [](::std::ostream& str, const Syntop& op, int, Backend*){
        if (op.size() < 2 || op.args[0].tag == Arg::VREG)
            throw std::runtime_error("Wrong CALL format");
        str << "["; if(op.args[1].tag == Arg::IIMMEDIATE) print_address(str, op.args[1].value); else str << op.args[1]; str << "](" << op.args[0];
        for(int anum = 2; anum < op.size(); anum++) str<<", "<<op[anum];
        str << ")";
    }},
    {OP_CALL_NORET, [](::std::ostream& str, const Syntop& op, int, Backend*){
        if (op.size() < 1 || op.args[0].tag == Arg::VREG)
            throw std::runtime_error("Wrong CALL_NORET format");
        str << "["; if(op.args[0].tag == Arg::IIMMEDIATE) print_address(str, op.args[0].value); else str << op.args[0]; str << "](";
        for(int anum = 1; anum + 1 < op.size(); anum++) str<<op[anum]<<", ";
        if(op.size() > 1) str<<op[op.size() - 1];
        str << ")";
    }},
    {OP_SELECT, [](::std::ostream& str, const Syntop& op, int, Backend*){ str<<op[0]<<", "<< op[2]<<", "<<op[3];}},
    {OP_IVERSON, [](::std::ostream& str, const Syntop& op, int, Backend*){ str<<op[0];}},
};

std::unordered_map<int, std::string> opstrings = { //TODO(ch): will you create at every print?
    {OP_MOV,         "mov"},
    {OP_XCHG,        "xchg"},
    {OP_ADD,         "add"},
    {OP_MUL,         "mul"},
    {OP_SUB,         "sub"},
    {OP_DIV,         "div"},
    {OP_MOD,         "mod"},
    {OP_SHL,         "shl"},
    {OP_SHR,         "shr"},
    {OP_SAR,         "sar"},
    {OP_AND,         "and"},
    {OP_OR,          "or"},
    {OP_XOR,         "xor"},
    {OP_NOT,         "not"},
    {OP_NEG,         "neg"},
    {OP_CMP,         "cmp"},
    {OP_MIN,         "min"},
    {OP_MAX,         "max"},
    {OP_ABS,         "abs"},
    {OP_SIGN,        "sign"},
    {OP_GT,          "gt"},
    {OP_UGT,         "ugt"},
    {OP_GE,          "ge"},
    {OP_LT,          "lt"},
    {OP_LE,          "le"},
    {OP_ULE,         "ule"},
    {OP_NE,          "ne"},
    {OP_EQ,          "eq"},
    {OP_S,           "s"},
    {OP_NS,          "ns"},
    {OP_LOGICAL_AND, "log_and"},
    {OP_LOGICAL_OR,  "log_or"},
    {OP_LOGICAL_NOT, "log_not"},
    {OP_JMP,         "jmp"},
    {OP_SPILL,       "spill"},
    {OP_UNSPILL,     "unspill"},
    {OP_STEM_CSTART, "annotation:stemcstart"},
    {OP_IF_CSTART,   "annotation:ifcstart"},
    {OP_ELIF_CSTART, "annotation:elif"},
    {OP_IF_CEND,     "annotation:ifcend"},
    {OP_ELSE,        "annotation:else"},
    {OP_ENDIF,       "annotation:endif"},
    {OP_WHILE_CSTART,"annotation:whilecstart"},
    {OP_WHILE_CEND,  "annotation:whilecend"},
    {OP_ENDWHILE,    "annotation:endwhile"},
    {OP_BREAK,       "annotation:break"},
    {OP_CONTINUE,    "annotation:continue"},
    {VOP_AND,        "and"},
    {VOP_OR,         "or"},
    {VOP_XOR,        "xor"},
    {VOP_NOT,        "not"},
    {VOP_SELECT,     "select"},
    {OP_RET,         "ret"},
    {OP_CALL,        "call"},
    {OP_CALL_NORET,  "call_noret"},
    {OP_X86_ADC,     "x86_adc"},
    {OP_X86_CQO,     "x86_cqo"},
    {OP_ARM_CINC,    "arm_cinc"},
    {OP_ARM_CNEG,    "arm_cneg"},
    {OP_ARM_MOVK,    "arm_movk"},
    {OP_ARM_LDP,     "arm_ldp"},
    {OP_ARM_STP,     "arm_stp"},
    {OP_DEF,         "def"},
};

FuncImpl::FuncImpl(const std::string& name, ContextImpl* ctx, std::initializer_list<IReg*> params) : m_refcount(0) //TODO(ch): support vector parameters
    , m_context(ctx) 
    , m_pipeline(std::make_shared<Pipeline>(ctx->getBackend(), this, name, params))
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

void FuncImpl::overrideRegisterSet(int basketNum, const std::vector<int>& a_parameterRegisters,
    const std::vector<int>& a_returnRegisters,
    const std::vector<int>& a_callerSavedRegisters,
    const std::vector<int>& a_calleeSavedRegisters)
{
    m_pipeline->overrideRegisterSet(basketNum, a_parameterRegisters, a_returnRegisters, a_callerSavedRegisters, a_calleeSavedRegisters);
}

void FuncImpl::printIR(std::ostream& /*out*/, const std::string& uptoPass_)
{
    //DUBUG: deexpreremntize
    Pipeline l_pipeline(*(m_context->debug_mode() ? m_debug_pipeline.get(): m_pipeline.get()));
    std::string uptoPass = uptoPass_;
    if (uptoPass == "")
    {
        std::vector<std::string>  allpasses = l_pipeline.get_all_passes();
        auto found = std::find(allpasses.begin(), allpasses.end(), "CP_IR_TO_ASSEMBLY");
        Assert(found != allpasses.end() && found != allpasses.begin());
        uptoPass = *(found - 1);
    }
    l_pipeline.run_until(uptoPass);
    printer_new* _printer;
    Assert(create_ir_printer(Func::PC_OPNUM | Func::PC_OP, &_printer) == 0);
    syntfunc2print s2p;
    s2p.name = (char*)(l_pipeline.get_data().name.c_str());
    s2p.params = (Arg*)l_pipeline.get_data().params.data();
    s2p.params_size = (int)l_pipeline.get_data().params.size();
    s2p.program = (Syntop*)l_pipeline.get_data().program.data();
    s2p.program_size = (int)l_pipeline.get_data().program.size();

    Assert(print_syntfunc(_printer, stderr/*DUBUG: extract from out */, &s2p) == 0);
    free_printer(_printer);

    // Printer printer({Printer::colNumPrinter(0), Printer::colOpnamePrinter(opstrings, opnameoverrules), Printer::colArgListPrinter(l_pipeline.get_data(), argoverrules)});
    // printer.print(out, l_pipeline.get_data());
}

void FuncImpl::printAssembly(std::ostream& out, int columns)
{
    Pipeline l_pipeline(*(m_context->debug_mode() ? m_debug_pipeline.get(): m_pipeline.get()));
    l_pipeline.run_until("CP_IR_TO_ASSEMBLY");
    
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
    m_pipeline->run_until("CP_COLLECTING");
    if (m_directTranslation)
        m_pipeline->skip_until("CP_IR_TO_ASSEMBLY");
    if (m_context->debug_mode())
        m_debug_pipeline = std::make_shared<Pipeline>(*(m_pipeline.get()));
}
}
