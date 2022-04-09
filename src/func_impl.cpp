/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "func_impl.hpp"
#include "backend.hpp"
#include "printer.hpp"
#include <algorithm>
#include <iomanip>
#include <deque>

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

FuncImpl::FuncImpl(const std::string& name, Context* ctx, std::initializer_list<IReg*> params) : m_refcount(0)
    , m_nextIdx(0)
    , m_nextLabelIdx(0)
    , m_context(getImpl(ctx))
    , m_returnType(RT_NOTDEFINED)
    , m_compiled(nullptr)
{
    m_data.name = name;
    m_data.params.reserve(params.size());
    for (IReg* parreg : params)
    {
        if(parreg->func != nullptr || parreg->idx != IReg::NOIDX)
            throw std::string("Parameter index is already initilized in some other function");
        parreg->func = this;
        parreg->idx = provideIdx();
        m_data.params.emplace_back(parreg->idx);
    }
}

Func FuncImpl::makeWrapper(const std::string& name, Context* ctx, std::initializer_list<IReg*> params)
{
    return Func::make(new FuncImpl(name, ctx, params));
}

void* FuncImpl::ptr()
{
    if(!m_compiled)
        m_compiled = m_context->getBackend()->compile(m_context, this);
    return m_compiled;
}

void FuncImpl::printBytecode(std::ostream& out) const
{
    std::unordered_map<int, Printer::ColPrinter > opnameoverrules = {
        {OP_LOAD, [this](::std::ostream& str, const Syntop& op, size_t, BackendImpl*){
            str << "load." << type_suffixes[op.args[1].value];
        }},
        {OP_STORE, [this](::std::ostream& str, const Syntop& op, size_t, BackendImpl*){
            str << "store." << type_suffixes[op.args[0].value];
        }},
        {OP_LABEL, [this](::std::ostream& str, const Syntop& op, size_t, BackendImpl*){
            if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                throw std::string("Wrong LABEL format");
            str << "label " << op.args[0] << ":";
        }}
    };
    
    std::unordered_map<int, Printer::ColPrinter > argoverrules = {
        {OP_LABEL, [this](::std::ostream& str, const Syntop& op, size_t, BackendImpl*){}}
    };

    std::unordered_map<int, std::string> opstrings = { //TODO(ch): will you create at every print?
        {OP_MOV, "mov"},
        {OP_CMP, "cmp"},
        {OP_ADD, "add"},
        {OP_MUL, "mul"},
        {OP_DIV, "div"},
        {OP_SUB, "sub"},
        {OP_JMP, "jmp"},
        {OP_JMP_EQ, "jmp_eq"},
        {OP_JMP_NE, "jmp_ne"},
        {OP_JMP_GE, "jmp_ge"},
        {OP_JMP_LE, "jmp_le"},
        {OP_JMP_GT, "jmp_gt"},
        {OP_JMP_LT, "jmp_gt"},
        {OP_SPILL, "spill"},
        {OP_UNSPILL, "unspill"},
        {OP_IF, "annotation:if"},
        {OP_ELIF, "annotation:elif"},
        {OP_ELSE, "annotation:else"},
        {OP_ENDIF,"annotation:endif"},
        {OP_DO, "annotation:do"},
        {OP_WHILE, "annotation:while"},
        {OP_RET, "ret"},
    };
    Printer printer({Printer::colNumPrinter(0), Printer::colOpnamePrinter(opstrings, opnameoverrules), Printer::colArgListPrinter(argoverrules)});
    printer.print(out, m_data);
}

void FuncImpl::printAssembly(std::ostream& out, int columns) const
{
    BackendImpl* be = getImpl(m_context->getBackend());
    Syntfunc tarcode = be->bytecode2Target(m_data);
    std::vector<Printer::ColPrinter> columnPrs;
    columnPrs.reserve(3);
    if(columns&PC_OPNUM)
        columnPrs.push_back(Printer::colNumPrinter(0));
    if(columns&PC_OP)
    {
        columnPrs.push_back(Printer::colOpnamePrinter(be->getOpStrings()));
        columnPrs.push_back(Printer::colArgListPrinter());
    }
    if(columns&PC_HEX)
    {
        if(columns&PC_OP)
            columnPrs.push_back(Printer::colDelimeterPrinter());
        columnPrs.push_back(be->colHexPrinter(tarcode));
    }
    Printer printer(columnPrs);
    printer.setBackend(be);
    printer.print(out, tarcode);
}

size_t FuncImpl::provideLabel()
{
    return m_nextLabelIdx++;
}

FuncImpl* FuncImpl::verifyArgs(std::initializer_list<Arg> args)
{
    FuncImpl* func = nullptr;
    for (const Arg& arg : args)
        if (arg.tag == Arg::IREG)
        {
            if (func == nullptr)
                func = static_cast<FuncImpl*>(arg.func);
            else if(func != static_cast<FuncImpl*>(arg.func))
                throw std::string("Registers of different functions as arguments of one instruction.");
        }
    if (func == nullptr)
        throw std::string("Cannot find mother function in registers.");
    return func;
}

const size_t FuncImpl::EMPTYLABEL = static_cast<size_t>(-1);

void FuncImpl::endfunc()
{
    //TODO(ch): block somehow adding new instruction after this call.
    allocateRegisters();
    jumpificate();
}

IReg FuncImpl::const_(int64_t value)
{
    return static_cast<IReg&&>(newiop(OP_MOV, { argIConst(value, this) }));
}

void FuncImpl::do_()
{
    size_t label = provideLabel();
    m_cflowStack.push(FuncImpl::cflowbracket(FuncImpl::cflowbracket::DO, label));
    newiopNoret(OP_DO, {argIConst(label,this)});
}

void FuncImpl::while_(const IReg& r)
{
    if (m_cflowStack.size() == 0)
        throw std::string("Unclosed control flow bracket: there is no \"do\" for \"while\".");
    auto bracket = m_cflowStack.top();
    if (bracket.tag != FuncImpl::cflowbracket::DO)
        throw std::string("Control flow bracket error: expected corresponding \"do\" for \"while\".");
    m_cflowStack.pop();
    newiopNoret(OP_WHILE, {argIConst(m_cmpopcode, this), argIConst(bracket.labelOrPos, this)});
}

void FuncImpl::if_(const IReg& r)
{
    m_cflowStack.push(FuncImpl::cflowbracket(FuncImpl::cflowbracket::IF, m_data.program.size()));
    m_cmpopcode = m_cmpopcode == OP_JMP_EQ ? OP_JMP_NE : (
                  m_cmpopcode == OP_JMP_NE ? OP_JMP_EQ : (
                  m_cmpopcode == OP_JMP_LT ? OP_JMP_GE : (
                  m_cmpopcode == OP_JMP_GT ? OP_JMP_LE : (
                  m_cmpopcode == OP_JMP_LE ? OP_JMP_GT : (
                  m_cmpopcode == OP_JMP_GE ? OP_JMP_LT : OP_JMP)))));
    if(m_cmpopcode == OP_JMP)
        throw std::string("Temporary condition solution failed.");
    newiopNoret(OP_IF, {argIConst(m_cmpopcode, this), argIConst(0, this)});
}

void FuncImpl::endif_() //TODO(ch): Corresponding tag also can be ELSE or ELIF.
{
    if (m_cflowStack.size() == 0)
        throw std::string("Unclosed control flow bracket: there is no \"if\", \"elif\" or \"else\", for \"endif\".");
    auto bracket = m_cflowStack.top();
    if (bracket.tag != FuncImpl::cflowbracket::IF)
        throw std::string("Control flow bracket error: expected corresponding \"if\", \"elif\" or \"else\" for \"endif\".");
    m_cflowStack.pop();
    size_t posnext = m_data.program.size();
    if (bracket.labelOrPos >= posnext)
        throw std::string("\"If\" internal error: wrong branch start address");
    const Syntop& ifop = m_data.program[bracket.labelOrPos];
    if (ifop.opcode != OP_IF || ifop.size() != 2 || ifop.args[1].tag != Arg::ICONST)
        throw std::string("\"If\" internal error: wrong IF command format");
    size_t label = provideLabel();
    m_data.program[bracket.labelOrPos].args[1].value = label;
    newiopNoret(OP_ENDIF, {argIConst(label, this)});
    m_cmpopcode = OP_JMP;
}

void FuncImpl::return_(const IReg& retval)
{
    if (m_returnType == RT_VOID)
        throw std::string("Mixed return types");
    newiopNoret(OP_MOV, {argIReg(Syntfunc::RETREG, this), retval});
    newiopNoret(OP_RET, {});
}

void FuncImpl::return_()
{
    if (m_returnType == RT_REGISTER)
        throw std::string("Mixed return types");
    newiopNoret(OP_RET, {});
}

struct LiveInterval
{
    size_t start, end;
    size_t startDepth, endDepth;
    IRegInternal idx;
    LiveInterval(IRegInternal a_idx, size_t a_start, size_t a_startDepth = 0) : start(a_start), end(a_start), startDepth(a_startDepth), endDepth(a_startDepth), idx(a_idx){}
    LiveInterval(std::pair<IRegInternal,std::pair<size_t, size_t> > a_tuple) : start(a_tuple.second.first), end(a_tuple.second.second), idx(a_tuple.first) {}
};

struct startordering
{
    bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.start < b.start; }
};

struct endordering
{
    bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.end < b.end; }
};

typedef std::vector<LiveInterval> LiveAnStruct; //TODO(ch): find better solution for detecting pseudonames per line number.

std::map<IRegInternal, std::pair<size_t, size_t> > FuncImpl::livenessAnalysis()
{
    const BackendImpl* backend = getImpl(m_context->getBackend());
    std::vector<LiveAnStruct> result(m_nextIdx,LiveAnStruct());
    size_t pseudIdx = 0;
    for(IRegInternal par = 0; par < m_data.params.size(); par++)
        result[par].push_back(LiveInterval(pseudIdx++, 0, 0));
    std::deque<cflowbracket> flowstack;
    
    //             depth               idx,subintr
    std::multimap<size_t, std::pair<size_t, size_t> > bracketOutHooks;
    
    //Finding Liveintervals and splitting them if it's needed.
    for(size_t opnum = 0; opnum < m_data.program.size(); opnum++) //Looks like template for walk. Hmmm...
    {
        const Syntop& op = m_data.program[opnum];
        switch (op.opcode)
        {
            case (OP_IF):
            {
                if (op.size() != 2 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong IF command format");
                flowstack.push_back(FuncImpl::cflowbracket(FuncImpl::cflowbracket::IF, opnum));
                continue;
            }
            case (OP_ENDIF):
            {
                if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong ENDIF command format");
                if(!flowstack.size() || flowstack.back().tag != FuncImpl::cflowbracket::IF)
                    throw std::string("Internal error: control brackets error.");
                flowstack.pop_back();
                continue;
            }
            case (OP_DO):
            {
                if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong DO command format");
                flowstack.push_back(FuncImpl::cflowbracket(FuncImpl::cflowbracket::DO, opnum));
                continue;
            }
            case (OP_WHILE):
            {
                if (op.size() != 2 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong WHILE command format");
                if(!flowstack.size() || flowstack.back().tag != FuncImpl::cflowbracket::DO)
                    throw std::string("Internal error: control brackets error.");
                const cflowbracket& bracket = flowstack.back();
                size_t lookupDepth = flowstack.size() - 1;
                for(auto hrator = bracketOutHooks.begin(); hrator!=bracketOutHooks.end();) //TODO(ch): use lowerbound or something.
                    if(hrator->first == lookupDepth)
                    {
                        size_t idx = hrator->second.first;
                        size_t subinterval = hrator->second.second;
                        result[idx].erase(result[idx].begin() + subinterval + 1, result[idx].end());
                        result[idx][subinterval].end = opnum;
                        result[idx][subinterval].endDepth = lookupDepth;
                        hrator = bracketOutHooks.erase(hrator);
                    }
                    else
                        hrator++;
                flowstack.pop_back();
                continue;
            }
            default:
            {
                std::set<IRegInternal> inRegs = backend->getInRegisters(op);
                std::set<IRegInternal> outRegs = backend->getOutRegisters(op);
                for(IRegInternal inreg : inRegs)
                {
                    if(inreg == Syntfunc::RETREG) //TODO(ch): At some day we will need to work with different types of return.
                        continue;
                    if (result[inreg].size() == 0) //TODO(ch): Isn't it too strict?
                        throw std::string("Compile error: using uninitialized register");
                    size_t siEndDepth = result[inreg].back().endDepth;
                    if (siEndDepth < flowstack.size())
                    {
                        int foundLoopDepth = -1;
                        int rvsdDepth = flowstack.size();
                        for(auto rator = flowstack.rbegin(); rator!=flowstack.rend() && siEndDepth </*(!!!)<?*/ rvsdDepth; rator++, rvsdDepth--)
                            if(rator->tag == FuncImpl::cflowbracket::DO) //TODO(ch): Or any other loop.
                                foundLoopDepth = rvsdDepth - 1;
                        if(foundLoopDepth!= -1)
                        {
                            bracketOutHooks.insert(std::make_pair(size_t(foundLoopDepth), std::make_pair(inreg, result[inreg].size() - 1)));
                            continue;
                        }
                    }
                    LiveInterval& intrvl = result[inreg].back();
                    intrvl.end = opnum;
                    intrvl.endDepth = flowstack.size();
                }
                for(IRegInternal outreg : outRegs)
                {
                    if(outreg == Syntfunc::RETREG)
                        continue;
                    if(result[outreg].size() > 0)
                    {
                        LiveInterval& intrvl = result[outreg].back();
                        size_t currentDepth = flowstack.size();
                        if (intrvl.startDepth != currentDepth) //IMPORTANT: Think around situation 1-0-1, when register is defined inside of block and redefined in another of same depth.(0-1-0, obviously doesn't matter). IMPORTANT: In situation {def-IF(def)-use} inner def must NOT break the interval.
                        {
                            intrvl.end = opnum;
                            intrvl.endDepth = currentDepth;
                        }
                        else
                            result[outreg].push_back(LiveInterval(pseudIdx++, opnum, flowstack.size()));
                    }
                    else
                        result[outreg].push_back(LiveInterval(pseudIdx++, opnum, flowstack.size()));
                }
                break;
            }
        }
    }

    //Renaming splitted registers
    for(size_t opnum = 0; opnum < m_data.program.size(); opnum++)
    {
        Syntop& op = m_data.program[opnum];
        std::set<size_t> inRegs = backend->getInRegistersIdxs(op);
        for(size_t arnum = 0; arnum < op.size(); arnum++)
        {
            Arg& arg = op.args[arnum];
            if(arg.tag == Arg::IREG)
            {
                if(arg.idx == Syntfunc::RETREG)
                    continue;
                bool isIn = inRegs.count(arnum);
                if (result.size() == 0)
                    throw std::string("Liveness analysis: unknown register is referenced on renaming.");
                size_t linum = 0;
                if (result[arg.idx].size() > 1)
                    for(; linum < result[arg.idx].size(); linum++)
                    {
                        LiveInterval& li = result[arg.idx][linum];
                        if (opnum >= li.start && opnum <= li.end && (isIn || opnum != li.end))
                            break;
                    }
                if (linum == result[arg.idx].size())
                    throw std::string("Liveness analysis: register isn't active at given line number.");
                arg.idx = result[arg.idx][linum].idx;
            }
        }
    }

    std::map<IRegInternal, std::pair<size_t, size_t> > afterresult;
    for(auto res:result)
        for(auto pseud:res)
        {
            LiveInterval toAdd(pseud.idx, pseud.start);
            toAdd.end = pseud.end;
            afterresult.insert(std::make_pair(pseud.idx, std::make_pair(toAdd.start,toAdd.end)));
        }
    
    return afterresult;
}

void FuncImpl::allocateRegisters()
{
    const BackendImpl* backend = getImpl(m_context->getBackend());
    const size_t maximumSpills = 3; //TODO(ch): At this very initial stage I just consider last three available registers as spill-used registers.
    if(backend->registersAmount() < m_data.params.size())
        throw std::string("Register allocator: not enough registers for passing function parameters.");
    const size_t availableSpills = backend->registersAmount() - m_data.params.size(); //TODO(ch): This condition will become softer when we will trace livetime of input registers.
    const size_t R = backend->registersAmount() - maximumSpills;
    size_t registersUsed = backend->registersAmount();
    std::multiset<LiveInterval, startordering> liveintervals;
    std::multiset<LiveInterval, endordering> parActive;
    {
        std::map<IRegInternal, std::pair<size_t, size_t> > analysisResult = livenessAnalysis();
        registersUsed = std::min(registersUsed, analysisResult.size());
        std::map<IRegInternal, LiveInterval> _liveintervals;
        std::map<IRegInternal, LiveInterval> _liveparams;
        for(auto interval:analysisResult)
            if(interval.first < m_data.params.size())
                _liveparams.insert(std::make_pair(interval.first, LiveInterval(interval)));
            else
                _liveintervals.insert(std::make_pair(interval.first, LiveInterval(interval)));
        for(auto interval : _liveintervals)
            liveintervals.insert(interval.second);
        for(auto interval : _liveparams)
            parActive.insert(interval.second);
    }
    int64_t spoffset = 0;
    std::map<IRegInternal, int64_t> spilled;
    std::map<IRegInternal, IRegInternal> renamingMap;
    { // Looking spilled registers and reusing registers with renaming map.
        for(IRegInternal parreg = 0; parreg < m_data.params.size(); parreg ++)
            renamingMap[parreg] = parreg;
        renamingMap[Syntfunc::RETREG] = Syntfunc::RETREG;
        std::set<IRegInternal> registerPool;
        for(IRegInternal freeReg = m_data.params.size(); freeReg < R; ++freeReg) registerPool.insert(freeReg);
        std::multiset<LiveInterval, endordering> active;
        for(auto interval : liveintervals)
        {
            { //Dropping expired registers.
                {
                    auto removerator = active.begin();
                    for(;removerator != active.end(); ++removerator)
                        if (removerator->end <= interval.start)
                        {
                            if(renamingMap.count(removerator->idx) == 1)
                                registerPool.insert(renamingMap.at(removerator->idx));
                            else
                                registerPool.insert(removerator->idx);
                        }
                        else
                            break;
                    active.erase(active.begin(), removerator);
                }
                {
                    auto removerator = parActive.begin();
                    for(;removerator != parActive.end(); ++removerator)
                        if (removerator->end <= interval.start)
                            registerPool.insert(removerator->idx);
                        else
                            break;
                    parActive.erase(parActive.begin(), removerator);
                }
            }
            if(registerPool.empty())
            {
                if(!active.empty() && active.rbegin()->end > interval.end)
                {
                    renamingMap[interval.idx] = renamingMap[active.rbegin()->idx];
                    renamingMap.erase(active.rbegin()->idx);
                    spilled[active.rbegin()->idx] = spoffset;
                    active.erase(--(active.end()));
                    active.insert(interval);
                }
                else
                    spilled[interval.idx] = spoffset;
                spoffset++;
            }
            else
            {
                active.insert(interval);
                renamingMap[interval.idx] = *registerPool.begin();
                registerPool.erase(registerPool.begin());
            }
        }
    }

    std::vector<Syntop> newProg;
    newProg.reserve(m_data.program.size() * 3);
    backend->writePrologue(m_data, newProg, registersUsed, spilled.size());

    // TODO(ch):
    // 1.) Let's consider sequence of instructions, where it's used one register. Obviously, it can be unspilled only once at start of sequence and
    // spilled only once at end. But for now it will spilled/unpilled at each instruction. I think, this unefficiency can be easily avoided by using some
    // variable-spill map.
    // 2.) Also, we have to take into account live intervals of spilled variables. At some moment place in memory for one variable can be used for
    // another variable.
    {//Renaming registers and adding spill operations
        for(size_t opnum = 0; opnum < m_data.program.size(); ++opnum)
        {
            Syntop op = m_data.program[opnum];
            std::set<IRegInternal> unspilledArgs = backend->getInRegisters(op);
            std::set<IRegInternal> spilledArgs = backend->getOutRegisters(op);
            std::map<IRegInternal,IRegInternal> argRenaming;
            IRegInternal currSpRegIndex = R;
            for(IRegInternal regAr: unspilledArgs)
                if(spilled.count(regAr))
                {
                    argRenaming[regAr] = currSpRegIndex;
                    newProg.push_back(Syntop(OP_UNSPILL, {argIReg(currSpRegIndex, this), spilled[regAr]}));
                    ++currSpRegIndex;
                }
            for(IRegInternal regAr: spilledArgs)
                if(spilled.count(regAr) != 0 && unspilledArgs.count(regAr) == 0)
                {
                    argRenaming[regAr] = currSpRegIndex;
                    ++currSpRegIndex;
                }

            if(availableSpills < (currSpRegIndex - R))
                throw std::string("Register allocator : not enough free registers.");
            for(Arg& ar: op)
                if (ar.tag == Arg::IREG)
                    ar.idx = (argRenaming.count(ar.idx) == 0) ? renamingMap[ar.idx] : argRenaming[ar.idx];
            newProg.push_back(op);
            for(IRegInternal regAr: spilledArgs)
                if(spilled.count(regAr))
                    newProg.push_back(Syntop(OP_SPILL, {spilled[regAr], argIReg(argRenaming[regAr], this)}));
        }
    }
    m_epilogueSize = newProg.size();
    backend->writeEpilogue(m_data, newProg, registersUsed, spilled.size());
    m_epilogueSize = newProg.size() - m_epilogueSize;
    m_data.program = newProg;
}

void FuncImpl::jumpificate()
{
    //TODO(ch): what if return are not on all control pathes(this problem exists for register returns only)??? Think out.
    //          I think, it can be effectively soluted only after deletition of after-jump-silent tails a-la jmp end; mov ..code-without-jumps...; end:
    //          After this we just need to check last operation before end mark. it must be mov to return register.
    //Handle situation, when return is just before end of function (reasonable)
    std::vector<Syntop> newProg;
    newProg.reserve(newProg.size()*2);
    size_t returnLabel = provideLabel();
    bool returnJumps = false;
    size_t bodySize = m_data.program.size() - m_epilogueSize;
    for(size_t opnum = 0; opnum < bodySize; opnum++)
    {
        const Syntop& op = m_data.program[opnum];
        switch (op.opcode) {
            case (OP_IF):
            {
                if (op.size() != 2 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong IF command format");
                newProg.push_back(Syntop(op.args[0].value, {op.args[1].value}));
                break;
            }
            case (OP_ENDIF):
            {
                if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong ENDIF command format");
                newProg.push_back(Syntop(OP_LABEL, {op.args[0].value}));
                break;
            }
            case (OP_DO):
            {
                if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong DO command format");
                newProg.push_back(Syntop(OP_LABEL, {op.args[0].value}));
                break;
            }
            case (OP_WHILE):
            {
                if (op.size() != 2 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST)
                   throw std::string("Internal error: wrong WHILE command format");
                newProg.push_back(Syntop(op.args[0].value, {op.args[1].value}));
                break;
            }
            case (OP_RET):
            {
                if(opnum + 1 != (m_data.program.size() - m_epilogueSize))
                {
                    newProg.push_back(Syntop(OP_JMP, {argIConst(returnLabel, this)}));
                    returnJumps = true;
                }
                break;
            }
            default:
                newProg.push_back(op);
                break;
        }
    }
    if(returnJumps)
        newProg.push_back(Syntop(OP_LABEL, {argIConst(returnLabel, this)}));
    //Write epilogue
    for(size_t opnum = bodySize; opnum < m_data.program.size(); opnum++)
        newProg.push_back(m_data.program[opnum]);
    newProg.push_back(Syntop(OP_RET, {}));
    m_data.program = newProg;
}

};
