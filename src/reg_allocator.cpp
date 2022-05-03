/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "reg_allocator.hpp"
#include "common.hpp"
#include "func_impl.hpp"

namespace loops
{
    struct LiveInterval
    {
        size_t start, end;
        IRegInternal idx;
        size_t subinterval;
        LiveInterval(IRegInternal a_idx, size_t a_start) : start(a_start), end(a_start), idx(a_idx), subinterval(-1) {}
        LiveInterval(std::pair<IRegInternal, std::pair<size_t, size_t> > a_tuple) : start(a_tuple.second.first), end(a_tuple.second.second), idx(a_tuple.first), subinterval(-1) {}
    };

    struct startordering
    {
        bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.start < b.start; }
    };

    struct endordering
    {
        bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.end < b.end; }
    };

    struct LAEvent //Liveness Analysis Event
    {
        enum { LAE_STARTLOOP, LAE_STARTBRANCH, LAE_STARTSUBINT, LAE_SWITCHSUBINT, LAE_ENDSUBINT, NONDEF = -1 };
        int eventType;
        IRegInternal idx;
        size_t subInterval;
        size_t elsePos;
        size_t endNesting;
        LAEvent() : eventType(NONDEF), idx(IReg::NOIDX), subInterval(NONDEF), elsePos(NONDEF), endNesting(NONDEF) {}
    };

    typedef std::vector<LiveInterval> LALayout; //TODO(ch): find better solution for detecting pseudonames per line number.


    RegisterAllocator::RegisterAllocator(ContextImpl* a_owner): m_owner(a_owner)
        , m_epilogueSize(0)
        , m_knownRegsAmount(0)
        , m_spillPlaceholdersTop(0)
        , m_snippetCausedSpills(0)
    {
        Assert(m_owner != nullptr);
    }

    void RegisterAllocator::process(FuncImpl* a_func, Syntfunc& a_processed, size_t a_knownRegsAmount)
    {
        Backend* backend = m_owner->getBackend();
        m_knownRegsAmount = a_knownRegsAmount;
        if (backend->registersAmount() < a_processed.params.size())
            throw std::string("Register allocator: not enough registers for passing function parameters.");

        std::multiset<LiveInterval, startordering> liveintervals;
        std::multiset<LiveInterval, endordering> parActive;
        {
            std::map<IRegInternal, std::pair<size_t, size_t> > analysisResult = livenessAnalysis(a_processed);
            std::map<IRegInternal, LiveInterval> _liveintervals;
            std::map<IRegInternal, LiveInterval> _liveparams;
            for (auto interval = analysisResult.begin(); interval != analysisResult.end(); ++interval)
                if (interval->first < a_processed.params.size())
                    _liveparams.insert(std::make_pair(interval->first, LiveInterval(*interval)));
                else
                    _liveintervals.insert(std::make_pair(interval->first, LiveInterval(*interval)));
            for (auto interval = _liveintervals.begin(); interval != _liveintervals.end(); ++interval)
                liveintervals.insert(interval->second);
            for (auto interval = _liveparams.begin(); interval != _liveparams.end(); ++interval)
                parActive.insert(interval->second);
        }

        int64_t spoffset = m_snippetCausedSpills;
        std::map<IRegInternal, int64_t> spilled;
        std::map<IRegInternal, IRegInternal> renamingMap;
        std::vector<IRegInternal> paramsInStack;
        paramsInStack.reserve(a_processed.params.size());
        initRegisterPool();
        { // Looking spilled registers and reusing registers with renaming map.
            {//Get pseudonames for parameters.
                IRegInternal parreg = 0;
                for (; parreg < a_processed.params.size(); parreg++)
                {
                    size_t attempt = provideParamFromPool();
                    if (attempt == IReg::NOIDX)
                        break;
                    renamingMap[parreg] = attempt;
                }
                for (; parreg < a_processed.params.size(); parreg++)
                {
                    size_t attempt = provideRegFromPool();
                    if (attempt == IReg::NOIDX)
                        throw std::runtime_error("Register allocator : so much arguments is not supported for now.");
                    renamingMap[parreg] = attempt;
                    paramsInStack.push_back(attempt);
                }
            }
            std::multiset<LiveInterval, endordering> active;
            for (auto interval = liveintervals.begin(); interval != liveintervals.end(); ++interval)
            {
                std::unordered_map<IRegInternal, IRegInternal> opUndefs; //TODO(ch): You also have to consider spilled undefs.
                { //Dropping expired registers.
                    {
                        auto removerator = active.begin();
                        for (; removerator != active.end(); ++removerator)
                            if (removerator->end <= interval->start)
                            {
                                auto renrator = renamingMap.find(removerator->idx);
                                if (renrator == renamingMap.end())
                                    throw std::runtime_error("Register allocator : internal renaming error.");
                                releaseRegister(renrator->second);
                                if (removerator->end == interval->start) //Current line, line of definition of considered register
                                    opUndefs.insert(*renrator);
                            }
                            else
                                break;
                        active.erase(active.begin(), removerator);
                    }
                    {
                        auto removerator = parActive.begin();
                        for (; removerator != parActive.end(); ++removerator)
                            if (removerator->end <= interval->start)
                            {
                                auto renrator = renamingMap.find(removerator->idx);
                                if (renrator == renamingMap.end())
                                    throw std::string("Register allocator : internal renaming error.");
                                releaseRegister(renrator->second);
                                if (removerator->end == interval->start) //Current line, line of definition of considered register
                                    opUndefs.insert(*renrator);
                            }
                            else
                                break;
                        parActive.erase(parActive.begin(), removerator);
                    }
                }
                if (!havefreeRegs())
                {
                    if (!active.empty() && active.rbegin()->end > interval->end)
                    {
                        renamingMap[interval->idx] = renamingMap[active.rbegin()->idx];
                        renamingMap.erase(active.rbegin()->idx);
                        spilled[active.rbegin()->idx] = spoffset;
                        active.erase(--(active.end()));
                        active.insert(*interval);
                    }
                    else
                        spilled[interval->idx] = spoffset;
                    spoffset++;
                }
                else
                {
                    IRegInternal poolHint = IReg::NOIDX;
                    active.insert(*interval);
                    if (opUndefs.size())
                    {
                        std::unordered_map<size_t, IRegInternal> opUndefsIdxMap;
                        std::set<size_t> opUndefsIdx;
                        const Syntop& op = a_processed.program[interval->start];
                        std::set<size_t> iNs = backend->getInRegistersIdxs(op);
                        for (size_t in : iNs)
                            if (opUndefs.count(op[in].idx))
                            {
                                opUndefsIdxMap[in] = opUndefs.at(op[in].idx);
                                opUndefsIdx.insert(in);
                            }
                        size_t idxHint = backend->reusingPreferences(op, opUndefsIdx);
                        if (idxHint != -1)
                            poolHint = opUndefsIdxMap.at(idxHint);
                    }
                    renamingMap[interval->idx] = provideRegFromPool(poolHint);
                }
            }
        }
        renamingMap[Syntfunc::RETREG] = provideReturnFromPool();

        std::vector<Syntop> newProg;
        newProg.reserve(a_processed.program.size() * 3);
        backend->writePrologue(a_processed, newProg, spilled.size() + m_snippetCausedSpills, m_usedCallee, paramsInStack);

        // TODO(ch):
        // 1.) Let's consider sequence of instructions, where it's used one register. Obviously, it can be unspilled only once at start of sequence and
        // spilled only once at end. But for now it will spilled/unpilled at each instruction. I think, this unefficiency can be easily avoided by using some
        // variable-spill map.
        // 2.) Also, we have to take into account live intervals of spilled variables. At some moment place in memory for one variable can be used for
        // another variable.

        {//Renaming registers and adding spill operations
            for (size_t opnum = 0; opnum < a_processed.program.size(); ++opnum)
            {
                Syntop op = a_processed.program[opnum];
                std::set<size_t> unspilledIdxs = backend->getInRegistersIdxs(op);
                std::set<size_t> spilledIdxs = backend->getOutRegistersIdxs(op);
                for (std::set<size_t>::iterator removerator = unspilledIdxs.begin(); removerator != unspilledIdxs.end();)
                {
                    size_t argNum = (*removerator);
                    if (argNum >= op.size() || op.args[argNum].tag != Arg::IREG)
                        throw std::string("Register allocator : internal error, invalid argument number.");
                    if (spilled.count(op.args[argNum].idx) != 0)
                        removerator++;
                    else
                        removerator = unspilledIdxs.erase(removerator);
                }
                for (std::set<size_t>::iterator removerator = spilledIdxs.begin(); removerator != spilledIdxs.end();)
                {
                    size_t argNum = (*removerator);
                    if (argNum >= op.size() || op.args[argNum].tag != Arg::IREG)
                        throw std::string("Register allocator : internal error, invalid argument number.");
                    if (spilled.count(op.args[argNum].idx) != 0)
                        removerator++;
                    else
                        removerator = spilledIdxs.erase(removerator);
                }
                std::set<size_t> stackPlaceable = spilledIdxs;
                stackPlaceable.insert(unspilledIdxs.begin(), unspilledIdxs.end());
                stackPlaceable = backend->filterStackPlaceable(op, stackPlaceable);
                for (std::set<size_t>::iterator removerator = unspilledIdxs.begin(); removerator != unspilledIdxs.end();)
                    if (stackPlaceable.count(*removerator) != 0)
                        removerator = unspilledIdxs.erase(removerator);
                    else
                        removerator++;
                for (std::set<size_t>::iterator removerator = spilledIdxs.begin(); removerator != spilledIdxs.end();)
                    if (stackPlaceable.count(*removerator) != 0)
                        removerator = spilledIdxs.erase(removerator);
                    else
                        removerator++;
                std::set<IRegInternal> unspilledArgs;
                for (size_t argNum : unspilledIdxs)
                    if (argNum >= op.size() || op.args[argNum].tag != Arg::IREG)
                        throw std::string("Register allocator : internal error, invalid argument number.");
                    else
                        unspilledArgs.insert(op.args[argNum].idx);
                std::set<IRegInternal> spilledArgs;
                for (size_t argNum : spilledIdxs)
                    if (argNum >= op.size() || op.args[argNum].tag != Arg::IREG)
                        throw std::string("Register allocator : internal error, invalid argument number.");
                    else
                        spilledArgs.insert(op.args[argNum].idx);

                std::map<IRegInternal, IRegInternal> argRenaming;

                clearSpillPlaceholders();
                for (IRegInternal regAr : unspilledArgs)
                {
                    IRegInternal pseudoname = provideSpillPlaceholder();
                    if (pseudoname == IReg::NOIDX)
                        throw std::string("Register allocator : not enough free registers.");
                    argRenaming[regAr] = pseudoname;
                    newProg.push_back(Syntop(OP_UNSPILL, { argIReg(pseudoname, a_func), spilled[regAr] }));
                }
                for (IRegInternal regAr : spilledArgs)
                {
                    IRegInternal pseudoname = provideSpillPlaceholder();
                    if (pseudoname == IReg::NOIDX)
                        throw std::string("Register allocator : not enough free registers.");
                    argRenaming[regAr] = pseudoname;
                }
                for (size_t arnum = 0; arnum < op.size(); arnum++)
                {
                    Arg& ar = op[arnum];
                    if (ar.tag == Arg::IREG)
                    {
                        if (stackPlaceable.count(arnum) != 0)
                            ar = argISpilled(spilled[ar.idx], a_func);
                        else
                            ar.idx = (argRenaming.count(ar.idx) == 0) ? renamingMap[ar.idx] : argRenaming[ar.idx];
                    }
                }
                newProg.push_back(op);
                for (IRegInternal regAr : spilledArgs)
                    newProg.push_back(Syntop(OP_SPILL, { spilled[regAr], argIReg(argRenaming[regAr], a_func) }));
            }
        }
        m_epilogueSize = newProg.size();
        backend->writeEpilogue(a_processed, newProg, spilled.size() + m_snippetCausedSpills, m_usedCallee);
        m_epilogueSize = newProg.size() - m_epilogueSize;
        a_processed.program = newProg;
    }

    enum { RT_PARAMETER = 1, RT_RETURN = 2, RT_CALLERSAVED = 4, RT_CALLEESAVED = 8 };
    void RegisterAllocator::initRegisterPool()
    {
        Backend* backend = m_owner->getBackend();
        m_parameterRegisters = backend->parameterRegisters();
        m_returnRegisters = backend->returnRegisters();
        m_callerSavedRegisters = backend->callerSavedRegisters();
        m_calleeSavedRegisters = backend->calleeSavedRegisters();
        m_usedCallee.clear();
        for (IRegInternal par : m_parameterRegisters)
            m_registersDistr[par] |= RT_PARAMETER;
        for (IRegInternal par : m_returnRegisters)
            m_registersDistr[par] |= RT_RETURN;
        for (IRegInternal par : m_callerSavedRegisters)
            m_registersDistr[par] |= RT_CALLERSAVED;
        for (IRegInternal par : m_calleeSavedRegisters)
            m_registersDistr[par] |= RT_CALLEESAVED;
    }

    size_t RegisterAllocator::freeRegsAmount() const
    {
        //TODO(ch): Need more efficient implementation.
        static const size_t maximumSpills = 3; //TODO(ch):need more detailed scheme
        std::set<IRegInternal> allRegisters;
        allRegisters.insert(m_parameterRegisters.begin(), m_parameterRegisters.end());
        allRegisters.insert(m_returnRegisters.begin(), m_returnRegisters.end());
        allRegisters.insert(m_calleeSavedRegisters.begin(), m_calleeSavedRegisters.end());
        allRegisters.insert(m_callerSavedRegisters.begin(), m_callerSavedRegisters.end());
        return allRegisters.size() - maximumSpills;
    }

    bool RegisterAllocator::havefreeRegs() const
    {
        //TODO(ch): Need more efficient implementation.
        return freeRegsAmount() > 0;
    }

    IRegInternal RegisterAllocator::provideParamFromPool()
    {
        IRegInternal res = IReg::NOIDX;
        if (m_parameterRegisters.size() != 0)
            res = m_parameterRegisters[0];
        removeFromAllBaskets(res);
        return res;
    }

    IRegInternal RegisterAllocator::provideRegFromPool(IRegInternal a_hint)
    {
        static const size_t maximumSpills = 3; //TODO(ch):need more detailed scheme
        IRegInternal res = IReg::NOIDX;
        if (a_hint != IReg::NOIDX)
        {
            bool hintfound = std::find(m_parameterRegisters.begin(), m_parameterRegisters.end(), a_hint) != m_parameterRegisters.end();
            hintfound = hintfound || std::find(m_returnRegisters.begin(), m_returnRegisters.end(), a_hint) != m_returnRegisters.end();
            hintfound = hintfound || std::find(m_callerSavedRegisters.begin(), m_callerSavedRegisters.end(), a_hint) != m_callerSavedRegisters.end();
            hintfound = hintfound || std::find(m_calleeSavedRegisters.begin(), m_calleeSavedRegisters.end(), a_hint) != m_calleeSavedRegisters.end();
            Assert(hintfound);
            res = a_hint;
        }
        else if (m_parameterRegisters.size() != 0) //We are sure, that all real parameters are already taken.
            res = m_parameterRegisters[0];
        else if (m_returnRegisters.size() != 0) //We are sure, that all real return registers will be needed only after usage them in general purposes.
            res = m_returnRegisters[0];
        else if (m_callerSavedRegisters.size() != 0)
            res = m_callerSavedRegisters[0];
        else if (m_calleeSavedRegisters.size() > maximumSpills)
        {
            res = m_calleeSavedRegisters[0];
            m_usedCallee.insert(res);
        }
        removeFromAllBaskets(res);
        return res;
    }

    IRegInternal RegisterAllocator::provideSpillPlaceholder()
    {
        if (m_calleeSavedRegisters.size() <= m_spillPlaceholdersTop)
            return IReg::NOIDX;
        m_usedCallee.insert(m_calleeSavedRegisters[m_spillPlaceholdersTop]);
        return m_calleeSavedRegisters[m_spillPlaceholdersTop++];
    }

    IRegInternal RegisterAllocator::provideReturnFromPool()
    {
        Backend* backend = m_owner->getBackend();
        m_returnRegisters = backend->returnRegisters(); //TODO(ch): It will works only for one return register.
        IRegInternal res = IReg::NOIDX;
        if (m_returnRegisters.size() != 0)
            res = m_returnRegisters[0];
        removeFromAllBaskets(res);
        return res;
    }

    void RegisterAllocator::clearSpillPlaceholders()
    {
        m_spillPlaceholdersTop = 0;
    }

    void RegisterAllocator::removeFromAllBaskets(IRegInternal reg)
    {
        if (reg == IReg::NOIDX)
            return;
        //TODO(ch): In truth, only two baskets can intersects: PARAMETERS and RETURN. Other can be precisely divided. 
        if (m_registersDistr[reg] & RT_PARAMETER)
        {
            std::vector<IRegInternal>::iterator rator = std::find(m_parameterRegisters.begin(), m_parameterRegisters.end(), reg);
            if (rator != m_parameterRegisters.end())
                m_parameterRegisters.erase(rator); //TODO(ch): It's better to keep registers in basket in reverse order.
        }
        if (m_registersDistr[reg] & RT_RETURN)
        {
            std::vector<IRegInternal>::iterator rator = std::find(m_returnRegisters.begin(), m_returnRegisters.end(), reg);
            if (rator != m_returnRegisters.end())
                m_returnRegisters.erase(rator);
        }
        if (m_registersDistr[reg] & RT_CALLERSAVED)
        {
            std::vector<IRegInternal>::iterator rator = std::find(m_callerSavedRegisters.begin(), m_callerSavedRegisters.end(), reg);
            if (rator != m_callerSavedRegisters.end())
                m_callerSavedRegisters.erase(rator);
        }
        if (m_registersDistr[reg] & RT_CALLEESAVED)
        {
            std::vector<IRegInternal>::iterator rator = std::find(m_calleeSavedRegisters.begin(), m_calleeSavedRegisters.end(), reg);
            if (rator != m_calleeSavedRegisters.end())
                m_calleeSavedRegisters.erase(rator);
        }
    }

    void RegisterAllocator::releaseRegister(IRegInternal freeReg)
    {
        if (m_registersDistr[freeReg] & RT_PARAMETER)
            m_parameterRegisters.insert(m_parameterRegisters.begin(), freeReg); //TODO(ch): It's better to keep registers in basket in reverse order.
        if (m_registersDistr[freeReg] & RT_RETURN)
            m_returnRegisters.insert(m_returnRegisters.begin(), freeReg);
        if (m_registersDistr[freeReg] & RT_CALLERSAVED)
            m_callerSavedRegisters.insert(m_callerSavedRegisters.begin(), freeReg);
        if (m_registersDistr[freeReg] & RT_CALLEESAVED)
            m_calleeSavedRegisters.insert(m_calleeSavedRegisters.begin(), freeReg);
    }

    std::map<IRegInternal, std::pair<size_t, size_t> > RegisterAllocator::livenessAnalysis(Syntfunc& a_processed)
    {
        //This function accomplishes four goals:
        //1.) Separates all intervals of registers(variables) usage to small def-use subintervals. There can be a lot of usages in one subinterval, but only one definition.
        //2.) Expand subinteravls, which intersects with loop's body, or branches in some certain cases.
        //3.) Rename subintervals got into new variables.
        //4.) Also, find the biggest number of spilled variables needed for deployment of some instructions into snippets(e.g., DIV on intel).
        
        m_snippetCausedSpills = 0;
        Backend* backend = m_owner->getBackend();
        //IMPORTANT: Think around situation 1-0-1, when register is defined inside of block and redefined in another of same depth.(0-1-0, obviously doesn't matter).
        std::multimap<size_t, LAEvent> loopQueue;
        std::multimap<size_t, LAEvent> branchQueue;
        std::vector<LALayout> subintervals(m_knownRegsAmount, LALayout());
        { //1.) Calculation of simplest [def-use] subintervals and collect precise info about borders of loops and branches.
            std::deque<ControlFlowBracket> flowstack;
            for (IRegInternal par = 0; par < a_processed.params.size(); par++)
                subintervals[par].push_back(LiveInterval(par, 0));
            for (size_t opnum = 0; opnum < a_processed.program.size(); opnum++)
            {
                const Syntop& op = a_processed.program[opnum];
                m_snippetCausedSpills = std::max(m_snippetCausedSpills, backend->spillSpaceNeeded(op));
                switch (op.opcode)
                {
                case (OP_IF):
                {
                    if (op.size() != 2 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST)
                        throw std::string("Internal error: wrong IF command format");
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::IF, opnum));
                    LAEvent toAdd;
                    toAdd.eventType = LAEvent::LAE_STARTBRANCH;
                    branchQueue.insert(std::make_pair(opnum, toAdd));
                    continue;
                }
                case (OP_ELSE):
                {
                    if (op.size() != 2 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST)
                        throw std::string("Internal error: wrong \"endif\" command format");
                    if (!flowstack.size() || flowstack.back().tag != ControlFlowBracket::IF)
                        throw std::string("Internal error: control brackets error.");
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::ELSE, opnum));
                    continue;
                }
                case (OP_ENDIF):
                {
                    if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                        throw std::string("Internal error: wrong \"endif\" command format");
                    if (!flowstack.size())
                        throw std::string("Internal error: control brackets error.");
                    ControlFlowBracket bracket = flowstack.back();
                    flowstack.pop_back();
                    size_t elsePos = LAEvent::NONDEF;
                    if (bracket.tag == ControlFlowBracket::ELSE)
                    {
                        elsePos = bracket.labelOrPos;
                        if (!flowstack.size())
                            throw std::string("Internal error: control brackets error.");
                        bracket = flowstack.back();
                        flowstack.pop_back();
                    }
                    if (bracket.tag != ControlFlowBracket::IF)
                        throw std::string("Internal error: control brackets error.");
                    auto rator = branchQueue.find(bracket.labelOrPos);
                    if (rator == branchQueue.end())
                        throw std::string("Internal error: control flow queue doesn't contain corresponding IF for given ENDIF.");
                    rator->second.endNesting = opnum;
                    rator->second.elsePos = elsePos;
                    continue;
                }
                case (OP_DO):
                {
                    if (op.size() != 1 || op.args[0].tag != Arg::ICONST)
                        throw std::string("Internal error: wrong DO command format");
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::DO, opnum));
                    LAEvent toAdd;
                    toAdd.eventType = LAEvent::LAE_STARTLOOP;
                    loopQueue.insert(std::make_pair(opnum, toAdd));
                    continue;
                }
                case (OP_WHILE):
                {
                    if (op.size() != 4 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST || op.args[2].tag != Arg::ICONST || op.args[3].tag != Arg::ICONST)
                        throw std::string("Internal error: wrong WHILE command format");
                    if (!flowstack.size() || flowstack.back().tag != ControlFlowBracket::DO)
                        throw std::string("Internal error: control brackets error.");
                    const ControlFlowBracket& bracket = flowstack.back();
                    flowstack.pop_back();
                    auto rator = loopQueue.find(bracket.labelOrPos);
                    if (rator == loopQueue.end())
                        throw std::string("Internal error: control flow queue doesn't contain corresponding DO for given WHILE.");
                    rator->second.endNesting = opnum;
                    continue;
                }
                case (OP_DOIF):
                {
                    if (op.size() != 3 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST || op.args[2].tag != Arg::ICONST)
                        throw std::string("Internal error: wrong DOIF command format");
                    if (opnum < 2)
                        throw std::string("Temporary condition solution needs one instruction before DOIF cycle.");
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::DOIF, opnum - 2));
                    LAEvent toAdd;
                    toAdd.eventType = LAEvent::LAE_STARTLOOP;
                    loopQueue.insert(std::make_pair(opnum - 2, toAdd)); //TODO(ch): IMPORTANT(CMPLCOND): This mean that condition can be one-instruction only.
                    continue;
                }
                case (OP_ENDDO):
                {
                    if (op.size() != 2 || op.args[0].tag != Arg::ICONST || op.args[1].tag != Arg::ICONST)
                        throw std::string("Internal error: wrong ENDDO command format");
                    if (!flowstack.size() || flowstack.back().tag != ControlFlowBracket::DOIF)
                        throw std::string("Internal error: control brackets error.");
                    const ControlFlowBracket& bracket = flowstack.back();
                    flowstack.pop_back();
                    auto rator = loopQueue.find(bracket.labelOrPos);
                    if (rator == loopQueue.end())
                        throw std::string("Internal error: control flow queue doesn't contain corresponding DOIF for given ENDDO.");
                    rator->second.endNesting = opnum;
                    continue;
                }
                default:
                {
                    
                    std::set<IRegInternal> inRegs = backend->getInRegisters(op);
                    std::set<IRegInternal> outRegs = backend->getOutRegisters(op);
                    std::set<IRegInternal> inOutRegs = backend->getUsedRegisters(op, Binatr::Detail::D_INPUT | Binatr::Detail::D_OUTPUT);
                    for (IRegInternal inreg : inRegs)
                    {
                        if (inreg == Syntfunc::RETREG) //TODO(ch): At some day we will need to work with different types of return.
                            continue;
                        if (subintervals[inreg].size() == 0) //TODO(ch): Isn't it too strict?
                            throw std::string("Compile error: using uninitialized register");
                        LiveInterval& intrvl = subintervals[inreg].back();
                        intrvl.end = opnum;
                    }
                    for (IRegInternal outreg : outRegs)
                    {
                        if (outreg == Syntfunc::RETREG)
                            continue;
                        if (inOutRegs.count(outreg) != 0)
                            continue;
                        subintervals[outreg].push_back(LiveInterval(outreg, opnum));
                    }
                    break;
                }
                }
            }
        }

        { //2.) Expanding loop intervals, which are crossing loops borders.
            std::multiset<LiveInterval, endordering> active;
            for (size_t idx = 0; idx < m_knownRegsAmount; idx++)
            {
                const size_t sintStart = subintervals[idx][0].start;
                if (sintStart == 0)
                {
                    LiveInterval toActive = subintervals[idx][0];
                    toActive.subinterval = 0;
                    active.insert(toActive);
                    const size_t sintEnd = subintervals[idx][0].end;
                    LAEvent toAdd;
                    toAdd.eventType = LAEvent::LAE_ENDSUBINT;
                    toAdd.idx = idx;
                    toAdd.subInterval = 0;
                    loopQueue.insert(std::make_pair(sintEnd, toAdd));
                }
                else
                {
                    LAEvent toAdd;
                    toAdd.eventType = LAEvent::LAE_STARTSUBINT;
                    toAdd.idx = idx;
                    toAdd.subInterval = 0;
                    loopQueue.insert(std::make_pair(sintStart, toAdd));
                }
            }
            while (!loopQueue.empty())
            {
                auto loopRator = loopQueue.begin();
                size_t opnum = loopQueue.begin()->first;
                LAEvent event = loopQueue.begin()->second;
                loopQueue.erase(loopQueue.begin());
                switch (event.eventType)
                {
                case (LAEvent::LAE_STARTSUBINT):
                {
                    LiveInterval li = subintervals[event.idx][event.subInterval];
                    li.subinterval = event.subInterval;
                    active.insert(li);
                    const size_t sintEnd = li.end;
                    LAEvent toAdd;
                    toAdd.eventType = LAEvent::LAE_ENDSUBINT;
                    toAdd.idx = event.idx;
                    toAdd.subInterval = event.subInterval;
                    loopQueue.insert(std::make_pair(sintEnd, toAdd));
                    break;
                };
                case (LAEvent::LAE_ENDSUBINT):
                {
                    auto removerator = active.begin();
                    while (removerator != active.end() && removerator->idx != event.idx && removerator->end == opnum)
                        ++removerator;
                    if (removerator == active.end() || removerator->idx != event.idx)
                        throw std::string("Internal error: finishing non-active interval.");
                    active.erase(removerator);
                    if (subintervals[event.idx].size() < event.subInterval + 1)
                    {
                        const LiveInterval& newli = subintervals[event.idx][event.subInterval + 1];
                        LAEvent toAdd;
                        toAdd.eventType = LAEvent::LAE_STARTSUBINT;
                        toAdd.idx = event.idx;
                        toAdd.subInterval = event.subInterval + 1;
                        loopQueue.insert(std::make_pair(newli.start, toAdd));
                    }
                    break;
                };
                case (LAEvent::LAE_STARTLOOP):
                {
                    std::multiset<LiveInterval, endordering> changed_active;
                    auto arator = active.begin();
                    for (; arator != active.end(); ++arator)
                        if (arator->end < event.endNesting)
                        {
                            IRegInternal idx = arator->idx;
                            size_t sinum = arator->subinterval;
                            size_t si2ers = sinum + 1;
                            size_t newEnd = event.endNesting;
                            for (; si2ers < subintervals[idx].size(); si2ers++)
                                if (subintervals[idx][si2ers].start > event.endNesting)
                                    break;
                                else
                                    newEnd = std::max(subintervals[idx][si2ers].end, newEnd);
                            subintervals[idx].erase(subintervals[idx].begin() + sinum + 1, subintervals[idx].begin() + si2ers);
                            subintervals[idx][sinum].end = newEnd;
                            subintervals[idx][sinum].subinterval = sinum;
                            changed_active.insert(subintervals[idx][sinum]);
                            auto qremrator = loopQueue.find(arator->end);
                            while (qremrator != loopQueue.end() && qremrator->first == arator->end)
                                if (qremrator->second.eventType == LAEvent::LAE_ENDSUBINT && qremrator->second.idx == arator->idx)
                                    break;
                            if (qremrator == loopQueue.end() || qremrator->first != arator->end)
                                throw std::string("Internal error: end interval event not found in queue.");
                            LAEvent toReadd = qremrator->second;
                            loopQueue.erase(qremrator);
                            loopQueue.insert(std::make_pair(newEnd, toReadd));
                        }
                        else
                            break;
                    active.erase(active.begin(), arator);
                    active.insert(changed_active.begin(), changed_active.end());
                    break;
                }
                default:
                    throw std::string("Internal error: unexpected event in loop queue.");
                }
            }
        }

        { //3.) Calculating intervals crossing if branches.
            std::multiset<LiveInterval, endordering> lastActive; // NOTE: In this part of code LiveInterval::end means not end position of subinterval, but deactivation position, position, when starts new subinterval or ends final one.
            for (size_t idx = 0; idx < m_knownRegsAmount; idx++)
            {
                if (subintervals[idx].size() == 0)
                    continue;
                const size_t sintStart = subintervals[idx][0].start;
                LAEvent toAdd;
                toAdd.eventType = LAEvent::LAE_SWITCHSUBINT;
                toAdd.idx = idx;
                size_t eventPos;
                if (sintStart == 0)
                {
                    eventPos = subintervals[idx].size() == 1 ? subintervals[idx][0].end : subintervals[idx][1].start;
                    LiveInterval toActive = subintervals[idx][0];
                    toActive.subinterval = 0;
                    toActive.end = eventPos;
                    lastActive.insert(toActive);
                    toAdd.subInterval = 1; //Destination interval
                }
                else
                {
                    eventPos = subintervals[idx][0].start;
                    toAdd.subInterval = 0; //Destination interval
                }
                branchQueue.insert(std::make_pair(eventPos, toAdd));
            }

            while (!branchQueue.empty())
            {
                size_t opnum = branchQueue.begin()->first;
                LAEvent event = branchQueue.begin()->second;
                branchQueue.erase(branchQueue.begin());
                switch (event.eventType)
                {
                case (LAEvent::LAE_SWITCHSUBINT):
                {
                    if (event.subInterval > 0)
                    {
                        auto removerator = lastActive.begin();
                        while (removerator != lastActive.end() && removerator->idx != event.idx && removerator->end == opnum)
                            ++removerator;
                        if (removerator != lastActive.end() && removerator->idx == event.idx)
                            lastActive.erase(removerator);
                    }
                    if (event.subInterval < subintervals[event.idx].size())
                    {
                        LAEvent toAdd;
                        toAdd.eventType = LAEvent::LAE_SWITCHSUBINT;
                        toAdd.idx = event.idx;
                        toAdd.subInterval = event.subInterval + 1;
                        size_t eventPos = (event.subInterval + 1 < subintervals[event.idx].size()) ?
                            subintervals[event.idx][event.subInterval + 1].start :
                            subintervals[event.idx][event.subInterval].end;
                        LiveInterval toActive = subintervals[event.idx][event.subInterval];
                        toActive.subinterval = event.subInterval;
                        toActive.end = eventPos;
                        lastActive.insert(toActive);
                        branchQueue.insert(std::make_pair(eventPos, toAdd));
                    }
                    break;
                };
                case (LAEvent::LAE_STARTBRANCH):
                {
                    const size_t ifPos = opnum;
                    const size_t endifPos = event.endNesting;
                    const bool haveElse = (event.elsePos != LAEvent::NONDEF);
                    const size_t elsePos = haveElse ? event.elsePos : endifPos;
                    std::multiset<LiveInterval, endordering> lastActiveChanged;
                    std::multiset<LiveInterval, endordering>::iterator larator = lastActive.begin();
                    for (; larator != lastActive.end() && larator->end < endifPos;)
                    {
                        size_t firstUseMain = -1;
                        size_t firstDefMain = -1;
                        size_t firstUseElse = -1;
                        size_t firstDefElse = -1;
                        const IRegInternal idx = larator->idx;
                        bool afterlife = (subintervals[idx].back().end > endifPos);
                        if (!haveElse && !afterlife)
                        {
                            larator++;
                            continue;
                        }
                        const size_t initSinum = larator->subinterval;
                        size_t sinum = initSinum;
                        for (; sinum < subintervals[idx].size(); ++sinum)
                        {
                            const size_t sistart = subintervals[idx][sinum].start;
                            const size_t siend = subintervals[idx][sinum].end;
                            if (sistart > endifPos)
                                break;
                            if (sistart > ifPos)
                            {
                                if (haveElse && sistart > elsePos && firstDefElse == -1)
                                    firstDefElse = sistart;
                                else if (firstDefMain == -1)
                                    firstDefMain = sistart;
                            }
                            if (siend > endifPos)
                                break;
                            if (siend > ifPos)
                            {
                                if (haveElse && siend > elsePos && firstUseElse == -1)
                                    firstUseElse = siend;
                                else if (firstUseMain == -1)
                                    firstUseMain = siend;
                            }
                        }
                        if (firstUseMain != -1 && firstUseMain > firstDefMain)
                            firstUseMain = -1;
                        if (firstUseElse != -1 && firstUseElse > firstDefElse)
                            firstUseElse = -1;
                        bool splice = false;
                        if (firstDefMain != -1 && firstUseElse != -1) // Abscence of linear separability.
                        {
                            //if(!afterlife && firstDefElse != -1)//TODO(ch): than you can splice just upto firstUseElse by changing sinum.
                            splice = true;
                        }
                        else if (!afterlife)
                        {
                            splice = false;
                        }
                        else if (firstDefElse == -1 && firstUseElse == -1)
                        {
                            if (firstDefMain == -1)
                                splice = false;
                            splice = true;
                        }
                        else if ((firstDefMain == -1 && firstDefElse != -1) || //One-of-branch redefinition with
                            (firstDefMain != -1 && firstDefElse == -1))  //afterusage means splicing.
                        {
                            splice = true;
                        }
                        else
                            splice = false;
                        if (splice)
                        {
                            const size_t switchIpos = larator->end;
                            if (sinum == subintervals[idx].size())
                                sinum = subintervals[idx].size() - 1;
                            else if (subintervals[idx][sinum].start > endifPos)
                                sinum--;
                            subintervals[idx][initSinum].end = subintervals[idx][sinum].end;
                            subintervals[idx].erase(subintervals[idx].begin() + initSinum + 1, subintervals[idx].begin() + sinum + 1);
                            LiveInterval changedOne = subintervals[idx][initSinum];
                            changedOne.end = (initSinum + 1 < subintervals[idx].size()) ? subintervals[idx][initSinum + 1].start : changedOne.end;
                            larator = lastActive.erase(larator);
                            lastActiveChanged.insert(changedOne);

                            auto qremrator = branchQueue.find(switchIpos);
                            while (qremrator != branchQueue.end() && qremrator->first == switchIpos)
                                if (qremrator->second.eventType == LAEvent::LAE_SWITCHSUBINT && qremrator->second.idx == idx)
                                    break;
                            if (qremrator == branchQueue.end() || qremrator->first != switchIpos)
                                throw std::string("Internal error: end interval event not found in queue.");
                            LAEvent toReadd = qremrator->second;
                            branchQueue.erase(qremrator);
                            branchQueue.insert(std::make_pair(changedOne.end, toReadd));
                        }
                        else
                            larator++;
                    }
                    lastActive.insert(lastActiveChanged.begin(), lastActiveChanged.end());
                    break;
                }
                default:
                    throw std::string("Internal error: unexpected event in branch queue.");
                }
            }
        }

        { //4.) Renaming splitted registers.
            size_t pseudIdx = a_processed.params.size();
            for (IRegInternal idx = 0; idx < a_processed.params.size(); idx++)
                for (size_t si = 1; si < subintervals[idx].size(); si++)
                    subintervals[idx][si].idx = pseudIdx++;
            for (IRegInternal idx = a_processed.params.size(); idx < subintervals.size(); idx++)
                for (LiveInterval& li : subintervals[idx])
                    li.idx = pseudIdx++;
            for (size_t opnum = 0; opnum < a_processed.program.size(); opnum++)
            {
                Syntop& op = a_processed.program[opnum];
                std::set<size_t> outRegArnums = backend->getOutRegistersIdxs(op);
                for (size_t arnum = 0; arnum < op.size(); arnum++)
                {
                    Arg& arg = op.args[arnum];
                    if (arg.tag == Arg::IREG)
                    {
                        if (arg.idx == Syntfunc::RETREG)
                            continue;
                        bool isOut = (outRegArnums.count(arnum) > 0);
                        if (subintervals.size() == 0)
                            throw std::string("Liveness analysis: unknown register is referenced on renaming.");
                        size_t linum = 0;
                        if (subintervals[arg.idx].size() > 1)
                            for (; linum < subintervals[arg.idx].size(); linum++)
                            {
                                LiveInterval& li = subintervals[arg.idx][linum];
                                if (opnum >= li.start && opnum <= li.end &&
                                    (!isOut || ((linum + 1) >= subintervals[arg.idx].size()) || subintervals[arg.idx][linum + 1].start > opnum))
                                    break;
                            }
                        if (linum == subintervals[arg.idx].size())
                            throw std::string("Liveness analysis: register isn't active at given line number.");
                        arg.idx = subintervals[arg.idx][linum].idx;
                    }
                }
            }
        }

        std::map<IRegInternal, std::pair<size_t, size_t> > result;
        for (auto res : subintervals)
            for (auto pseud : res)
            {
                LiveInterval toAdd(pseud.idx, pseud.start);
                toAdd.end = pseud.end;
                result.insert(std::make_pair(pseud.idx, std::make_pair(toAdd.start, toAdd.end)));
            }

        return result;
    }
};
