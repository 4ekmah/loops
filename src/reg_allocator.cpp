/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include <algorithm>
#include <list>
#include <cstring>
#include <unordered_map>
#include "reg_allocator.hpp"
#include "common.hpp"
#include "func_impl.hpp"
/*
This file contains two consequently applied algorithms: liveness analysis and register
allocation.

I.) Liveness analysis considers all references of virtual register and detect start and end
operation numbers of register usage interval. In most sophisticated versions of algorithm this
interval can be discontinious, in some sublintervals register can be inactive. There it's
assumed, that register is alive from start to end. On the other hand, there is auxillary effect
of this algorithm: separating liveinterval to many subintervals with reappointing new register
number to each subinterval. In some degree, it can substitute effectiveness of more sophisticated
scheme.

Algorithm have three stages:
1.) Lookup for elementary subintervals and initialization of event queues.
    It's known amount of virtual registers, used in code of the function. First step is
    initialization of vector of subintervals, each register will have subintervals breakdown -
    vector of starts and ends of subinterval(for simplicity it's used LiveInterval struct
    everywhere in both algorithms). For each instruction, by making getInRegisters,
    getOutRegisters requests to Backend, algorithm understands, which registers was used in
    instruction and which was redefined. Each elementary subinterval is the chain like def-use-
    use-...-use(use is optional). Algorithm stores info only about first and last link of chain.

    In the same time it's initialized control flow event queue, kkeping loops and embranchments.
    It's stored info about each loop or menbranchment into CFqueue with help of control flow 
    stack(flowstack).
    CFqueue[<startPosition>].endNesting is end of loop/embranchemnt.
    CFqueue[<endPosition>].endNesting is start of loop/embranchemnt.
    CFqueue[<startEmbranchementPosition>].elsePos is else position. At this stage elseif is
    already deconstructed.
    Time complexity: O(N) - where N is amount of instructions.
    Space complexity: O(M), M - amount of elementary subintervals.

2.) Splicing subintervals, which intersects loops/embranchment borders.
    Main idea: if subinterval was alive at start of the loop, it must stay alive until the
    end of loop, that's why in this case all subintervals, intersects loop's interval must be
    spliced in one. Much more sophisticated, but, basically, almost the same logic is used 
    for embranchment. Difficulty is splicing decision logic - in some cases they can stood
    separated.
    
    For making this splices there will be used CFqueue. It's obvious, that there
    is no need to work with all registers, but only with active. For effective keeping "active"
    container actual it's used same event queue with locating here information about starts end
    ends of subintervals activity.

    Container of active intervals uses different definition of interval ends. Subinterval uses
    line of last use, there it's used new definition, because splicing condition needs last known
    active value.
        Subintervals    Active Intervals
    1:        ^                ^
    2:        |                |
    3:        v                |
    4:                         |
    5:                         v
    6:        ^                ^
    7:        |                |
    8:        |                |
    9:        v                v

    Algorithm simulteneously iterates over subintervals for every known register. In start, for
    each register it's zero number of of current subinterval with help of function 
    initSubintervalHeaders(). Also all the registers interval starts are added to events queue
    as LAE_SWITCHSUBINT events. Also, paramater registers are added to "active" multiset, which
    is ordered by ends of intervals. This order accelerates find operations(because element to
    erase will always be in start of "active" container).
 
    For basic program, don't contain nested blocks, algorithm iterates subintervals, active and
    events only forward, each subinterval will is considered only once(don't be confused
    by matix view of data). If there are nested blocks, container of active is recursively stored
    at start of block and droped at end as a list of registers to be checked for splicing.
    
    Example, which demonstrate, why the most nested blocks must splice registers first we 
    encountered in real practice:

    IReg v = CTX.const_(val1);
    CTX.if_(cond1)
        v = CTX.const_(val2);
    CTX.elif(cond2)
        v = CTX.const_(val3);
    CTX.elif(cond3)
        v = CTX.const_(val3);
    CTX.endif();
 
    Without splicing in most inner branch first, algorithm inadequately decided to keep register's
    subintervals separated in outer embranchement.

    When it's meet loop/embranchment start, active subintervals container is stored to stack of actives
    (m_active_headers_stack), keeping registers idx's and the number of currently considered subinterval.

    When it's meet loop end, each active subinterval where active at loop start will be spliced with all
    subintervals, intersected with loop body. If loop is finished further, than union got, subinterval
    will be prolongated to end end of loop.

    When it's meet embranchment end, happens bascally almost same, but decision about splicing have
    sophisticated logic, depends on amount of branches, position of first "use" and fisrt "def" in
    each branch and after-embranchment usage(afterlife). In some cases list of subintervals to splice can
    be shorted.

    Time complexity: O(B*M) - where B - number of basic blocks, M - amount of elementary
    subintervals, roughly equal to amount of instructions.
    Space complexity: O(M + B*I), where I - amount of registers.

3.) Renaming.
    Finally each subinterval can be considered as separate register.

    Two linear loops:
    First one is over the subintervals breakdown with appointing new register indexes. One detail
    is indexes of parameter registers cannot be changed(they are first). Thus, first subintervals
    of parameter registers are reappointed first.

    Second one is over program with register index substitution. Detail: in some case one
    register can be used along instruction as input and as output both. In this case liveinterval
    can be breaked in this certain instruction and input register will took appointment different
    with output. This logic is accurately handled with help about Backend's data about output
    registers and subintervals ends coincendence.

    Time complexity: O(N+M) - where N is amount of instructions, M - amount of subintervals after
    splicing.
    Space complexity: O(M), M - amount of elementary subintervals.

Changes in code of function is auxillary result of algorithm. Main result is actual LiveInterval
vector for all registers. It's used as input data for register allocator.

II.) Register allocator fits unlimited amount of virtual register to fixed set of registers of
target CPU. Other purpose of algorithm is to collect some data needed to write function's
prologue and epilogue. It's a lot of data, thus prologue and epilogue are also written there.

Understanding RegisterPool have big value in context of register allocation. Scalar registers
can lie in four functional vessels: parameter, return, caller-saved(scratch), callee-saved. Of
course these vessels are defined differently for different architectures. Basksets can
intersects, there is order of taking free registers from them: parameter, return, caller-saved,
callee-saved. Since amount of registers cannot have too big value(32 is maximum of scalars), the
best container for such scheme is dynamical bitfield. If there is need to take register from
certain vessel, it's possible to mask bitfield with static vessel's mask. There is ability to
override this masks for supporting SpillStress mode.

Linear scan(algorithm description is given in paper: Poletto, Massimiliano; Sarkar, Vivek (1999).
"Linear scan register allocation". ACM Transactions on Programming Languages and Systems. 21 (5):
895�913.)
1.) Containers initialization.
    LiveIntervals got from liveness analysis are separated in two groups: parameters are
    immediately added to "active" multiset, which is ordered by ends of intervals, others are
    added to "liveintervals" multiset, ordered by starts. Real registers for parameters are
    taken with provideParamFromPool, than, if it's not enough, with provideRegFromPool, and
    finally they are spilled(in this case there is no stack increment - they was passed through
    the stack). All appointed and pilled registers will be stored in regReassignment vector.
2.) Matching target-mchine and virtual registers.
    Let's consider consequent LiveInterval from liveintervals. First, all expired active
    intervals(which end will be lesser than start of current interval) must be droped. If
    interval is dropped, it will return used register.

    Next is attempt to appoint the real register to current virtual register(interval). If there
    is free registers, it's just added to regReassignment and active. Otherwise there needed
    decision: what register must be spilled. There used heuristic: must be spilled register,
    which ends last. It's enough to compare ends of current register and last of active to
    determine register to spill.

    Also there implemented optimization, simplifing ternary instruction to binary instruction
    convesrion: if there is was a spectre of free registers for current interval, priority is
    given to registers just freed in start position of interval(i.e. input registers in
    instruction, where interval was defined). There used hints given by Backend's
    reusingPreferences.

    Time complexity: O(M * log R) - where M - amount of subintervals, R - amount of real
    registers of target machine(this is restriction of "active" size).
    Space complexity: O(M), M - amount of subintervals.
3.) Renaming and adding spill/unspill instructions.
    In loop over instructions, Backend gives numbers of input and output arguments, and choosed
    spilled one of them.

    Next(on Intel64) is attempt to match instruction variation which support memory-placed
    operands. For this used Backend's filterStackPlaceable method. In IR it looks like
    substitution of IREG/VREG with ISPILLED/VSPILLED. Aarch64 don't have this optimization.

    All spilled input parameters are extracted from stack into one of three register are provided
    by provideSpillPlaceholder(it's added UNSPILL instructions). In the same manner there found
    synonims to output spilled registers. All instruction's argument virtual registers are
    substituted: to real registers, to ISPILLED, to spill placeholders. Finally, there added
    SPILL instructions for output stack parameters.

    This stage must be modified to track location of variable value: in register/in stack. That's
    the way to decrease amount of SPILL/UNSPILL operations around every instruction and decrease
    stack memory usage with reusing space of droped variables. Putting into operation this
    mechanics is the highest priority task for Register allocator.

    Time complexity: O(N) - where N is amount of instructions.
    Space complexity: O(M), M - amount of subintervals.
4.) Prologue/Epilogue
    There was collected data, needed for writing prologue and epilogue of function: stack
    increment, indexes of stack-passed parameters, which are NOT extracted from it, indexes of
    parameters, which have to be spilled at start, indexes of used callee-saved registers.

    Time complexity: O(P+E+N) - where P is amount of parameters, and E - amount of used
    callee-saved registers, N is amount of instructions.
    Space complexity: O(P+E+N).
*/

namespace loops
{
    inline RegIdx pickFirstBit64(uint64_t& bigNum)
    {
        Assert(bigNum != 0);
        RegIdx ret = lsb64(bigNum);
        bigNum = (bigNum | (uint64_t(1) << ret)) ^ (uint64_t(1) << ret);
        return ret;
    }

    RegisterPool::RegisterPool(Backend* a_backend): m_backend(a_backend)
        , m_spillPlaceholdersAvailable{0,0}

    {}

    void RegisterPool::initRegisterPool()
    {
        memset(&(m_reorderInner2Arch[0][0][0]), REG_UNDEF, sizeof(m_reorderInner2Arch));
        memset(&(m_reorderArch2Inner[0][0][0]), REG_UNDEF, sizeof(m_reorderArch2Inner));
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        {
            std::vector<int> origRegistersV[VESS_AMOUNT];

            origRegistersV[PARAMS_VESS] = m_backend->parameterRegisters(basketNum),
            origRegistersV[RETURN_VESS] = m_backend->returnRegisters(basketNum),
            origRegistersV[CALLER_VESS] = m_backend->callerSavedRegisters(basketNum),
            origRegistersV[CALLEE_VESS] = m_backend->calleeSavedRegisters(basketNum);
        
            if (m_registersO[basketNum][PARAMS_VESS].size() != 0 || m_registersO[basketNum][RETURN_VESS].size() != 0 ||
                m_registersO[basketNum][CALLER_VESS].size() != 0 || m_registersO[basketNum][CALLEE_VESS].size() != 0)
                for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
                    origRegistersV[vessNum] = m_registersO[basketNum][vessNum];
            
            m_pool[basketNum] = 0;
            for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
            {
                uint8_t regAmount = static_cast<uint8_t>(origRegistersV[vessNum].size());
                m_vessel[basketNum][vessNum] = (((uint64_t)(1)) << regAmount) - 1;
                for(uint8_t inRegNum = 0; inRegNum < regAmount; inRegNum++)
                {
                    const uint8_t argregNum = (uint8_t)origRegistersV[vessNum][inRegNum];
                    m_reorderInner2Arch[basketNum][vessNum][inRegNum] = argregNum;
                    m_reorderArch2Inner[basketNum][vessNum][argregNum] = inRegNum;
                    m_pool[basketNum] |= (((uint64_t)(1)) << argregNum);
                }
            }
            m_spillPlaceholdersAvailable[basketNum] = m_pool[basketNum];
            m_usedCallee[basketNum].clear();
        }
    }

    size_t RegisterPool::freeRegsAmount(int basketNum) const
    {
        return amountOfBits64(m_pool[basketNum]) - MAXIMUM_SPILLS; //TODO(ch): MAXIMUM_SPILLS must become basket-dependend variable.
    }

    RegIdx RegisterPool::provideParamFromPool(int basketNum)
    {
        if(m_vessel[basketNum][PARAMS_VESS] == 0)
            return IReg::NOIDX;
        RegIdx res = lsb64(m_vessel[basketNum][PARAMS_VESS]);
        res = m_reorderInner2Arch[basketNum][PARAMS_VESS][res];
        removeFromAllVessels(basketNum, res);
        return res;
    }

    RegIdx RegisterPool::provideRegFromPool(int basketNum, RegIdx a_hint)
    {
        RegIdx res = NOREGISTER;
        if (a_hint != IReg::NOIDX)
        {
            for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
            {
                uint8_t inRegNum = m_reorderArch2Inner[basketNum][vessNum][a_hint];
                if(inRegNum == REG_UNDEF)
                    continue;
                if(m_vessel[basketNum][vessNum] & (((uint64_t)(1)) << inRegNum))
                {
                    res = a_hint;
                    break;
                }
            }
            Assert(res!=NOREGISTER);
        }
        else if (havefreeRegs(basketNum))
        {
            for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
                if(m_vessel[basketNum][vessNum])
                {
                    res = lsb64(m_vessel[basketNum][vessNum]);
                    res = m_reorderInner2Arch[basketNum][vessNum][res];
                    break;
                }
            Assert(res != NOREGISTER);
            if (m_reorderArch2Inner[basketNum][CALLEE_VESS][res] != REG_UNDEF)
                m_usedCallee[basketNum].insert(res);
        }
        removeFromAllVessels(basketNum, res);
        res = (res == NOREGISTER) ? IReg::NOIDX : res;
        return res;
    }

    //TODO(ch): this function is just a workaround for ld2/ld3/ld4 instructions
    std::vector<RegIdx> RegisterPool::provideConsecutiveRegs(int basketNum, int amount)
    {
        int amount_temp = amount; 
        std::vector<RegIdx> res;
        std::vector<RegIdx> fragmented;
        res.reserve(amount);
        fragmented.reserve(32);
        while(amount_temp > 0)
        {
            RegIdx next = provideRegFromPool(basketNum);
            if(next == IReg::NOIDX)
                throw std::runtime_error("Register allocator: register space is too fragmented for ld2/ld3/ld4 workaround.");
            if(res.empty() || next == res.back() + 1)
            {
                amount_temp--;
            }
            else
            {
                fragmented.insert(fragmented.end(), res.begin(), res.end());
                res.clear();
                amount_temp = amount - 1;
            }
            res.push_back(next);
        }
        for(auto t_rel : fragmented)
            releaseReg(basketNum, t_rel);
        return res;
    }

    RegIdx RegisterPool::provideReturnFromPool(int basketNum)
    {
        Assert(m_reorderInner2Arch[basketNum][RETURN_VESS][0] != REG_UNDEF);
        RegIdx res = m_reorderInner2Arch[basketNum][RETURN_VESS][0];
        removeFromAllVessels(basketNum, res);
        return res;
    }

    void RegisterPool::releaseReg(int basketNum, RegIdx freeReg)
    {
        Assert(freeReg != IReg::NOIDX);
        m_pool[basketNum] |= (((uint64_t)(1)) << freeReg);
        for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
        {
            uint8_t inRegNum = m_reorderArch2Inner[basketNum][vessNum][freeReg];
            if(inRegNum != REG_UNDEF)
                m_vessel[basketNum][vessNum] |= (((uint64_t)(1)) << inRegNum);
        }
    }

    RegIdx RegisterPool::provideSpillPlaceholder(int basketNum)
    {
        if (m_spillPlaceholders[basketNum] == 0)
            return IReg::NOIDX;
        int res = lsb64(m_spillPlaceholders[basketNum]);
        uint64_t pos = 1;
        pos <<= res;
        m_spillPlaceholders[basketNum] = (m_spillPlaceholders[basketNum] | pos) ^ (pos);
        if(m_reorderArch2Inner[basketNum][CALLEE_VESS][res] != REG_UNDEF)
            m_usedCallee[basketNum].insert(res);
        return res;
    }

    void RegisterPool::clearSpillPlaceholders(int basketNum)
    {
        m_spillPlaceholders[basketNum] = m_spillPlaceholdersAvailable[basketNum];
    }

    void RegisterPool::overrideRegisterSet(int basketNum, const std::vector<int>&  a_parameterRegisters,
                                                          const std::vector<int>&  a_returnRegisters,
                                                          const std::vector<int>&  a_callerSavedRegisters,
                                                          const std::vector<int>&  a_calleeSavedRegisters)
    {
        m_registersO[basketNum][PARAMS_VESS] = a_parameterRegisters;
        m_registersO[basketNum][RETURN_VESS] = a_returnRegisters;
        m_registersO[basketNum][CALLER_VESS] = a_callerSavedRegisters;
        m_registersO[basketNum][CALLEE_VESS] = a_calleeSavedRegisters;
    }

    void RegisterPool::getOverridenParams(std::vector<int> (&regParsOverride)[RB_AMOUNT]) const
    {
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            regParsOverride[basketNum] = m_registersO[basketNum][PARAMS_VESS]; 
    }

    void RegisterPool::removeFromAllVessels(int basketNum, int reg)
    {
        if (reg == NOREGISTER)
            return;
        m_pool[basketNum] |= (((uint64_t)1) << reg);
        m_pool[basketNum] ^= (((uint64_t)1) << reg);
        if (lsb64(m_pool[basketNum]) > lsb64(m_spillPlaceholdersAvailable[basketNum]))
            m_spillPlaceholdersAvailable[basketNum] = m_pool[basketNum];
        for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
        {
            uint8_t innerReg = m_reorderArch2Inner[basketNum][vessNum][reg];
            m_vessel[basketNum][vessNum] |= (((uint64_t)1) << innerReg);
            m_vessel[basketNum][vessNum] ^= (((uint64_t)1) << innerReg);
        }
    }

    RegisterAllocator::RegisterAllocator(Backend* a_backend, const std::vector<LiveInterval>* a_live_intervals, int a_snippet_caused_spills, bool a_have_function_calls) : CompilerPass(a_backend)
        , m_pool(a_backend)
        , m_live_intervals(a_live_intervals)
        , m_snippet_caused_spills(a_snippet_caused_spills)
        , m_have_function_calls(a_have_function_calls)
        , m_epilogueSize(0)
    {}

    //TODO(ch): It's good idea on intel64 + windows to use "shadow space", which is 32 bytes in stack just before
    //5-th parameter(other words - 1-st stack-passsed parameter). It's default and consistent place for spilling
    //register parameters.
    void RegisterAllocator::process(Syntfunc& a_dest, const Syntfunc& a_source)
    {
        m_pool.initRegisterPool();
        std::multiset<LiveInterval, startordering> liveintervals[RB_AMOUNT];
        std::vector<LiveInterval> parintervals[RB_AMOUNT];
        std::vector<RegIdx> paramsSorted[RB_AMOUNT];
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            paramsSorted[basketNum].reserve(a_source.params.size());
        for (const Arg& par : a_source.params)
        {
            Assert(par.tag == Arg::IREG || par.tag == Arg::VREG);
            int basketNum = (par.tag == Arg::IREG ? RB_INT : RB_VEC);
            paramsSorted[basketNum].push_back(par.idx);
        }
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        {
            //After liveness analysis we are sorting given liveintervals into two heaps:
            //parameters, which are immediately marked as active and other intervals,
            //which are reordered by start positions to work with Linear scan algorithm.
            size_t idx = 0;
            size_t idxParMax = std::min(m_live_intervals[basketNum].size(), paramsSorted[basketNum].size());
            parintervals[basketNum].reserve(idxParMax);
            for (; idx < idxParMax; ++idx)
                parintervals[basketNum].push_back(m_live_intervals[basketNum][idx]);
            idxParMax = m_live_intervals[basketNum].size();
            for (; idx < idxParMax; ++idx)
                liveintervals[basketNum].insert(m_live_intervals[basketNum][idx]);
        }
        //TODO(ch):This ugly workaround must be eliminated after introducing register allocation with restrictions.
        std::unordered_map<RegIdx, std::pair<RegIdx, RegIdx> > unspillableLd2[RB_AMOUNT];
        std::unordered_map<RegIdx, RegIdx> already_allocatedLd2[RB_AMOUNT];
        for(const Syntop& op: a_source.program)
            if(op.opcode == VOP_ARM_LD2)
            {
                std::pair<RegIdx, RegIdx> order = std::make_pair(op[0].idx, op[1].idx); 
                unspillableLd2[RB_VEC].insert(std::make_pair(op[0].idx, order));
                unspillableLd2[RB_VEC].insert(std::make_pair(op[1].idx, order));
            }

        //Space in stack used by snippets will be located in the bottom,
        //so spilled variables will be located higher.
        int64_t spoffset[RB_AMOUNT] = {0, 0};

        std::map<RegIdx, int> stackParamLayout[RB_AMOUNT];
        {
            std::vector<int> regParsOverride[RB_AMOUNT];
            m_pool.getOverridenParams(regParsOverride);
            m_backend->getStackParameterLayout(a_source, regParsOverride, stackParamLayout);
        }

        //Looking spilled registers and reusing registers with renaming map.
        std::vector<Arg> regReassignment[RB_AMOUNT];
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++) 
        {
            const int REGtag = ((basketNum == RB_INT) ? Arg::IREG : Arg::VREG);
            std::multiset<LiveInterval, endordering> active;
            regReassignment[basketNum].resize(a_source.regAmount[basketNum], Arg());
            {//Get pseudonames for parameters.
                RegIdx parreg = 0;
                for (; parreg < (int)parintervals[basketNum].size(); parreg++)
                {
                    RegIdx idx = parintervals[basketNum][parreg].idx;
                    RegIdx attempt = m_pool.provideParamFromPool(basketNum);
                    if (attempt == IReg::NOIDX)
                        break;
                    regReassignment[basketNum][idx] = argReg(basketNum, attempt);
                    active.insert(parintervals[basketNum][parreg]);
                }
                for (; parreg < (int)parintervals[basketNum].size(); parreg++)
                {
                    RegIdx idx = parintervals[basketNum][parreg].idx;
                    RegIdx attempt = m_pool.provideRegFromPool(basketNum);
                    if (attempt == IReg::NOIDX)
                        break;
                    regReassignment[basketNum][idx] = argReg(basketNum, attempt);
                    active.insert(parintervals[basketNum][parreg]);
                }
                for (; parreg < (int)parintervals[basketNum].size(); parreg++)
                {
                    RegIdx idx = parintervals[basketNum][parreg].idx;
                    regReassignment[basketNum][idx] = argSpilled(basketNum, 0);
                }
            }
            for (auto interval = liveintervals[basketNum].begin(); interval != liveintervals[basketNum].end(); ++interval)
            {
                std::unordered_map<RegIdx, RegIdx> opUndefs; //TODO(ch): You also have to consider spilled undefs.
                { //Dropping expired registers.
                    auto removerator = active.begin();
                    for (; removerator != active.end(); ++removerator)
                        if (removerator->end <= interval->start)
                        {
                            Assert(regReassignment[basketNum][removerator->idx].tag == REGtag);
                            m_pool.releaseReg(basketNum, regReassignment[basketNum][removerator->idx].idx);
                            if (removerator->end == interval->start) //Current line, line of definition of considered register
                                opUndefs.insert(std::pair<RegIdx,RegIdx>(removerator->idx, regReassignment[basketNum][removerator->idx].idx));
                        }
                        else
                            break;
                    active.erase(active.begin(), removerator);
                }
                if (!m_pool.havefreeRegs(basketNum))
                {
                    if(unspillableLd2[basketNum].find(interval->idx) != unspillableLd2[basketNum].end())
                        throw std::runtime_error("Register allocator: not enough free registers for ld2 workaround.");
                    bool stackParameterSpilled = false;
                    std::multiset<LiveInterval, endordering>::reverse_iterator lastactive = active.rbegin();
                    while(lastactive!=active.rend() && unspillableLd2[basketNum].find(lastactive->idx) != unspillableLd2[basketNum].end())
                        lastactive++;
                    if (lastactive != active.rend() && lastactive->end > interval->end)
                    {
                        regReassignment[basketNum][interval->idx] = regReassignment[basketNum][lastactive->idx];
                        RegIdx keepidx = regReassignment[basketNum][lastactive->idx].idx; //We need to know appointed target architecture register for spilled parameters.
                        stackParameterSpilled = stackParamLayout->count(lastactive->idx);
                        regReassignment[basketNum][lastactive->idx] = argSpilled(basketNum, stackParameterSpilled ? 0 : spoffset[basketNum]);
                        regReassignment[basketNum][lastactive->idx].idx = keepidx;
                        active.erase(--(active.end()));
                        active.insert(*interval);
                    }
                    else
                    {
                        RegIdx keepidx = regReassignment[basketNum][interval->idx].idx; //We need to know appointed target architecture register for spilled parameters.
                        stackParameterSpilled = stackParamLayout->count(interval->idx);
                        regReassignment[basketNum][interval->idx] = argSpilled(basketNum, stackParameterSpilled ? 0 : spoffset[basketNum]);
                        regReassignment[basketNum][interval->idx].idx = keepidx;
                    }
                    if(!stackParameterSpilled) 
                        spoffset[basketNum]++;
                }
                else
                {
                    RegIdx hwReg;
                    active.insert(*interval);
                    auto unsprator = unspillableLd2[basketNum].find(interval->idx);
                    if(basketNum == RB_VEC && ( unsprator != unspillableLd2[basketNum].end()))
                    {
                        auto alrator = already_allocatedLd2[basketNum].find(interval->idx);
                        if(alrator != already_allocatedLd2[basketNum].end())
                        {
                            hwReg = alrator->second;
                            already_allocatedLd2[basketNum].erase(interval->idx);
                        }
                        else
                        {
                            std::vector<RegIdx> consecutive_regs = m_pool.provideConsecutiveRegs(RB_VEC, 2);
                            RegIdx oppositeSrc = unsprator->second.second;
                            RegIdx oppositeDst = consecutive_regs[1];
                            RegIdx dst = consecutive_regs[0];
                            if (oppositeSrc == interval->idx)
                            {
                                oppositeSrc = unsprator->second.first;
                                oppositeDst = consecutive_regs[0];
                                dst = consecutive_regs[1];
                            }
                            hwReg = dst;
                            already_allocatedLd2[basketNum].insert(std::make_pair(oppositeSrc, oppositeDst));
                        }
                    } 
                    else
                    {
                        //There we are looking around last used input registers and trying to reuse them as
                        //out. This optimization is important, e.g., for add, sub and mul operation on Intel, where
                        //this operation are binary, not ternary.
                        RegIdx poolHint = IReg::NOIDX;
                        if (opUndefs.size())
                        {
                            std::unordered_map<size_t, RegIdx> opUndefsIdxMap;
                            std::set<int> opUndefsIdx;
                            const Syntop& op = a_source.program[interval->start];
                            std::set<int> iNs = m_backend->getInRegistersIdxs(op, basketNum);
                            for (int in : iNs)
                                if (opUndefs.count(op[in].idx))
                                {
                                    opUndefsIdxMap[in] = opUndefs.at(op[in].idx);
                                    opUndefsIdx.insert(in);
                                }
                            int argnumHint = m_backend->reusingPreferences(op, opUndefsIdx);
                            if (argnumHint != UNDEFINED_ARGUMENT_NUMBER)
                                poolHint = opUndefsIdxMap.at(argnumHint);
                        }
                        hwReg = m_pool.provideRegFromPool(basketNum, poolHint);
                    }
                    regReassignment[basketNum][interval->idx] = argReg(basketNum, hwReg);
                }
            }
        }

        const Arg retreg = argReg(RB_INT, m_pool.provideReturnFromPool(RB_INT));
        auto getReassigned = [retreg, &regReassignment](int basketNum, int old)
        {
            return (old == Syntfunc::RETREG && basketNum == RB_INT ? retreg : regReassignment[basketNum][old]);
        };

        // TODO(ch):
        // 1.) Let's consider sequence of instructions, where it's used one register. Obviously, it can be unspilled only once at start of sequence and
        // spilled only once at end. But for now it will spill/unspill at each instruction. I think, this unefficiency can be easily avoided by using some
        // variable-spill map.
        // 2.) Also, we have to take into account live intervals of spilled variables. At some moment place in memory for one variable can be used for
        // another variable.
        // 3.) By the way, when we are using only least spill placeholder, instead of using as much of them, as possible - it's bad practice. 
        // minimizing prologue/epilogue overhead isn't so important.

        int basketElemX[RB_AMOUNT] = {1,1};
        basketElemX[RB_VEC] = m_backend->getVectorRegisterBits() / 64;
        
        std::vector<std::map<RegIdx, Arg> > unspilledRenaming[RB_AMOUNT];
        std::vector<std::map<RegIdx, Arg> > spilledRenaming[RB_AMOUNT];
        std::vector<std::set<int> > stackPlaceable[RB_AMOUNT];
        int nettoSpills[RB_AMOUNT];
        int spAddAligned = m_snippet_caused_spills;
        //Collecting instructionwise spills properties
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        {
            unspilledRenaming[basketNum].resize(a_source.program.size());
            spilledRenaming[basketNum].resize(a_source.program.size());
            stackPlaceable[basketNum].resize(a_source.program.size());
            for (size_t opnum = 0; opnum < a_source.program.size(); ++opnum)
            {
                const Syntop& op = a_source.program[opnum];
                std::set<int> unspilledIdxs;
                std::set<int> spilledIdxs;
                const int REGtag = ((basketNum == RB_INT) ? Arg::IREG : Arg::VREG);
                const int SPLtag = ((basketNum == RB_INT) ? Arg::ISPILLED : Arg::VSPILLED);
                unspilledIdxs = m_backend->getInRegistersIdxs(op, basketNum);
                spilledIdxs = m_backend->getOutRegistersIdxs(op, basketNum);
                for (std::set<int>::iterator removerator = unspilledIdxs.begin(); removerator != unspilledIdxs.end();)
                {
                    int argNum = (*removerator);
                    Assert(argNum < op.size() && op.args[argNum].tag == REGtag);
                    if (getReassigned(basketNum, op.args[argNum].idx).tag == SPLtag)
                        removerator++;
                    else
                        removerator = unspilledIdxs.erase(removerator);
                }
                for (std::set<int>::iterator removerator = spilledIdxs.begin(); removerator != spilledIdxs.end();)
                {
                    int argNum = (*removerator);
                    Assert(argNum < op.size() && op.args[argNum].tag == REGtag);
                    if (getReassigned(basketNum, op.args[argNum].idx).tag == SPLtag)
                        removerator++;
                    else
                        removerator = spilledIdxs.erase(removerator);
                }
                stackPlaceable[basketNum][opnum] = spilledIdxs;
                stackPlaceable[basketNum][opnum].insert(unspilledIdxs.begin(), unspilledIdxs.end());
                stackPlaceable[basketNum][opnum] = m_backend->filterStackPlaceable(op, stackPlaceable[basketNum][opnum]);
                for (std::set<int>::iterator removerator = unspilledIdxs.begin(); removerator != unspilledIdxs.end();)
                    if (stackPlaceable[basketNum][opnum].count(*removerator) != 0)
                        removerator = unspilledIdxs.erase(removerator);
                    else
                        removerator++;
                for (std::set<int>::iterator removerator = spilledIdxs.begin(); removerator != spilledIdxs.end();)
                    if (stackPlaceable[basketNum][opnum].count(*removerator) != 0)
                        removerator = spilledIdxs.erase(removerator);
                    else
                        removerator++;

                m_pool.clearSpillPlaceholders(basketNum);
                for (int argNum : unspilledIdxs)
                {
                    RegIdx idx = op.args[argNum].idx;
                    Assert(argNum < op.size() && op.args[argNum].tag == REGtag);
                    if(unspilledRenaming[basketNum][opnum].count(idx) == 0) 
                    {
                        RegIdx pseudoname = m_pool.provideSpillPlaceholder(basketNum);
                        if (pseudoname == IReg::NOIDX)
                            throw std::runtime_error("Register allocator : not enough free registers.");
                        Arg newArg = op[argNum];
                        newArg.idx = pseudoname;
                        unspilledRenaming[basketNum][opnum][idx] = newArg;
                    }
                }

                for (int argNum : spilledIdxs)
                {
                    RegIdx idx = op.args[argNum].idx;
                    Assert(argNum < op.size() && op.args[argNum].tag == REGtag);
                    if(unspilledRenaming[basketNum][opnum].count(idx) != 0)
                    {
                        spilledRenaming[basketNum][opnum][idx] = unspilledRenaming[basketNum][opnum][idx];
                        continue;
                    }
                    if(spilledRenaming[basketNum][opnum].count(idx) == 0) 
                    {
                        RegIdx pseudoname = m_pool.provideSpillPlaceholder(basketNum);
                        if (pseudoname == IReg::NOIDX)
                            throw std::runtime_error("Register allocator : not enough free registers.");
                        Arg newArg = op[argNum];
                        newArg.idx = pseudoname;
                        spilledRenaming[basketNum][opnum][idx] = newArg;
                    }
                }
            }
            const int SPLtag = ((basketNum == RB_INT) ? Arg::ISPILLED : Arg::VSPILLED);
            int parametersStoodSpilled = 0; 
            for(auto p : stackParamLayout[basketNum]) 
                if (regReassignment[basketNum][p.first].tag == SPLtag)
                    parametersStoodSpilled++;
            nettoSpills[basketNum] = (int)std::count_if(regReassignment[basketNum].begin(), regReassignment[basketNum].end(), [SPLtag](const Arg& arg) {return arg.tag == SPLtag; }) - parametersStoodSpilled;
            nettoSpills[basketNum] += (int)m_pool.usedCallee(basketNum).size();
            spAddAligned += nettoSpills[basketNum] * basketElemX[basketNum];
        }
        if(m_have_function_calls)
            spAddAligned += m_backend->callerStackIncrement();
        spAddAligned = m_backend->stackGrowthAlignment(spAddAligned);

        size_t basketOffset[RB_AMOUNT] = {0,0};
        {
            std::vector<int> stackBasketOrder = m_backend->getStackBasketOrder();
            basketOffset[stackBasketOrder[0]] = m_snippet_caused_spills;
            for(int stackBasketNum = 1; stackBasketNum < (int)stackBasketOrder.size(); stackBasketNum++)
            {
                const int currBN = stackBasketOrder[stackBasketNum];
                const int prevBN = stackBasketOrder[stackBasketNum - 1];
                basketOffset[currBN] = basketOffset[prevBN] + basketElemX[prevBN] * nettoSpills[prevBN];
            }
        }
        
        auto getSpillOffset = [&stackParamLayout, &getReassigned, &spAddAligned, &basketOffset, &basketElemX](int basketNum, RegIdx reg)
        {
            const int SPLtag = ((basketNum == RB_INT) ? Arg::ISPILLED : Arg::VSPILLED);
            Arg reassigned = getReassigned(basketNum, reg);
            Assert(reassigned.tag == SPLtag);
            int64_t spillOffset = reassigned.value * basketElemX[basketNum] + basketOffset[basketNum];
            if (stackParamLayout[basketNum].count(reg))
                spillOffset = spAddAligned + stackParamLayout[basketNum][reg];
            return spillOffset;
        };
        
        //Renaming registers and adding spill operations
        std::vector<Syntop> newProgUnbracketed;
        newProgUnbracketed.reserve(a_source.program.size() * 3);
        for (size_t opnum = 0; opnum < a_source.program.size(); ++opnum)
        {
            Syntop op = a_source.program[opnum];
            for (int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                for (auto ar : unspilledRenaming[basketNum][opnum])
                    newProgUnbracketed.push_back(Syntop(OP_UNSPILL, { ar.second, argIImm(getSpillOffset(basketNum, ar.first)) }));
            for (int arnum = 0; arnum < op.size(); arnum++)
            {
                Arg& ar = op[arnum];
                if (ar.tag == Arg::IREG || ar.tag == Arg::VREG)
                {
                    int basketNum = (ar.tag == Arg::IREG ? RB_INT : RB_VEC);
                    if (stackPlaceable[basketNum][opnum].count(arnum) != 0)
                        ar = argSpilled(basketNum, getSpillOffset(basketNum, ar.idx));
                    else if(spilledRenaming[basketNum][opnum].count(ar.idx)) 
                        ar = spilledRenaming[basketNum][opnum][ar.idx];
                    else if(unspilledRenaming[basketNum][opnum].count(ar.idx)) 
                        ar = unspilledRenaming[basketNum][opnum][ar.idx];
                    else
                        ar.idx = getReassigned(basketNum, ar.idx).idx;
                }
            }
            newProgUnbracketed.push_back(op);
            for(int basketNum = 0; basketNum<RB_AMOUNT; basketNum++)
                for (auto ar : spilledRenaming[basketNum][opnum])
                    newProgUnbracketed.push_back(Syntop(OP_SPILL, { argIImm(getSpillOffset(basketNum, ar.first)), ar.second }));
        }
        
        a_dest.program.reserve(newProgUnbracketed.size() + 64);
        a_dest.params = a_source.params;
        a_dest.name = a_source.name;
        for (int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            a_dest.regAmount[basketNum] = a_source.regAmount[basketNum];
        { //Write prologue
            if (spAddAligned)
            {
                a_dest.program.push_back(Syntop(OP_SUB, { m_backend->getSParg(), m_backend->getSParg(), argIImm(spAddAligned * 8) }));
                for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                {
                    for (RegIdx par : paramsSorted[basketNum])
                        if (regReassignment[basketNum][par].tag == Arg::ISPILLED && (stackParamLayout[basketNum].count(par) == 0))
                        {
                            Arg spilled = regReassignment[basketNum][par];
                            spilled.tag = basketNum == RB_INT ? Arg::IREG : Arg::VREG;
                            a_dest.program.push_back(Syntop(OP_SPILL, { argIImm(getSpillOffset(basketNum, par)), spilled }));
                        }
                    size_t savNum = (nettoSpills[basketNum] - m_pool.usedCallee(basketNum).size()) * basketElemX[basketNum];
                    for (RegIdx toSav : m_pool.usedCallee(basketNum))
                    {
                        Arg spilled = argReg(basketNum, toSav);
                        if(basketNum == RB_VEC)
                            spilled.elemtype = TYPE_U8; // We actually don't care, just taking simplest type.
                        a_dest.program.push_back(Syntop(OP_SPILL, { argIImm(basketOffset[basketNum] + savNum), spilled }));
                        savNum += basketElemX[basketNum];
                    }
                }
            }
            for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                for(auto param : stackParamLayout[basketNum])
                {
                    RegIdx idx = param.first;
                    if (regReassignment[basketNum][idx].tag == Arg::IREG)
                    {
                        Assert(stackParamLayout[basketNum].find(idx) != stackParamLayout[basketNum].end());
                        a_dest.program.push_back(Syntop(OP_UNSPILL, { regReassignment[basketNum][idx], argIImm(spAddAligned + param.second) }));
                    }
                }
            if(m_have_function_calls)
                m_backend->writeCallerPrologue(a_dest, spAddAligned);
        }
        a_dest.program.insert(a_dest.program.end(), newProgUnbracketed.begin(), newProgUnbracketed.end());
        m_epilogueSize = (int)a_dest.program.size();
        { //Write epilogue
            if(m_have_function_calls)
                m_backend->writeCallerEpilogue(a_dest, spAddAligned);
            if (spAddAligned)
            {
                for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                {
                    size_t savNum = (nettoSpills[basketNum] - m_pool.usedCallee(basketNum).size()) * basketElemX[basketNum];
                    for (RegIdx toSav : m_pool.usedCallee(basketNum))
                    {
                        Arg spilled = argReg(basketNum, toSav);
                        if(basketNum == RB_VEC)
                            spilled.elemtype = TYPE_U8; // We actually don't care, just taking simplest type.
                        a_dest.program.push_back(Syntop(OP_UNSPILL, { spilled, argIImm(basketOffset[basketNum] + savNum) }));
                        savNum += basketElemX[basketNum];
                    }
                }
                a_dest.program.push_back(Syntop(OP_ADD, { m_backend->getSParg(), m_backend->getSParg(), argIImm(spAddAligned * 8) }));
            }
        }
        m_epilogueSize = (int)a_dest.program.size() - m_epilogueSize;
    }

    class LivenessAnalysisAlgoImpl;
    class LivenessAnalysisAlgoImpl : public LivenessAnalysisAlgo
    {
    public:
        LivenessAnalysisAlgoImpl(const Backend* a_owner);
        virtual ~LivenessAnalysisAlgoImpl() override {}
        virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;

        virtual std::vector<LiveInterval>* live_intervals() override final { return m_liveintervals; }
        virtual int getSnippetCausedSpills() const override final { return m_snippetCausedSpills; }
        virtual bool haveFunctionCalls() const override final { return m_haveFunctionCalls; }
    private:
        struct LAEvent //Liveness Analysis Event
        {
            enum { LAE_STARTLOOP, LAE_ENDLOOP, LAE_STARTBRANCH, LAE_ENDBRANCH, LAE_SWITCHSUBINT, NONDEF = -1 };
            int eventType;
            RegIdx idx;
            int elsePos;
            int oppositeNestingSide;
            int basketNum;
            LAEvent() : eventType(NONDEF), idx(IReg::NOIDX), elsePos(UNDEFINED_OPERATION_NUMBER), oppositeNestingSide(UNDEFINED_OPERATION_NUMBER) {}
            LAEvent(int a_eventType) : eventType(a_eventType), idx(IReg::NOIDX), elsePos(UNDEFINED_OPERATION_NUMBER), oppositeNestingSide(UNDEFINED_OPERATION_NUMBER), basketNum(RB_AMOUNT) {}
            LAEvent(int a_eventType, RegIdx a_idx, int basketNum_) : eventType(a_eventType), idx(a_idx), elsePos(UNDEFINED_OPERATION_NUMBER), oppositeNestingSide(UNDEFINED_OPERATION_NUMBER), basketNum(basketNum_) {}
        };
        std::vector<std::vector<LiveInterval> > m_subintervals[RB_AMOUNT]; //TODO(ch): std::vector<std::list<LiveInterval> > will avoid moves and allocations.
                                                                            //but in this case m_subintervalHeaders must be std::vector<std::list<LiveInterval>::iterator>
                                                                            //Header is number of subinterval in process of iteration over subintervals(keeping every interval in program).
        std::vector<int> m_subintervalHeaders[RB_AMOUNT];
        std::deque<std::map<RegIdx, int> > m_active_headers_stack[RB_AMOUNT];
        void push_active_state(const std::multiset<LiveInterval, endordering> (&a_lastActive) [RB_AMOUNT]
            , int a_endif);
        void pop_active_state();
        std::map<RegIdx, int>::const_iterator acs_begin(int basketNum) const;
        std::map<RegIdx, int>::const_iterator acs_end(int basketNum) const;
        std::vector<LiveInterval> m_liveintervals[RB_AMOUNT];
        int m_snippetCausedSpills;
        bool m_haveFunctionCalls;
        inline int regAmount(int basketNum) const { return (int)m_subintervals[basketNum].size(); }
        inline int siAmount(int basketNum, RegIdx regNum) const;
        inline bool defined(int basketNum, RegIdx regNum) const { return siAmount(basketNum, regNum) > 0; }
        inline void def(int basketNum, RegIdx regNum, int opnum);
        inline void use(int basketNum, RegIdx regNum, int opnum);
        inline void spliceUntilSinum(int basketNum, RegIdx regNum, int siEnd, int siStart = UNDEFINED_OPERATION_NUMBER);
        inline int expandUntilOpnum(int basketNum, RegIdx regNum, int opnum, int siStart);
        inline int deactivationOpnum(int basketNum, RegIdx regNum);
        inline void initSubintervalHeaders(int initval = 0);
        inline int getCurrentSinum(int basketNum, RegIdx regNum);
        inline LiveInterval& getCurrentSubinterval(int basketNum, RegIdx regNum);
        inline LiveInterval& getNextSubinterval(int basketNum, RegIdx regNum);
        inline bool isIterateable(int basketNum, RegIdx regNum) const; //Well, unfotunately, we don't have after-end-state, only last-one state.
        inline void iterateSubinterval(int basketNum, RegIdx regNum);
        inline void moveEventLater(std::multimap<int, LAEvent>& queue, RegIdx regNum, int eventType, int oldOpnum, int newOpnum);
    };

    void LivenessAnalysisAlgoImpl::process(Syntfunc& a_dest, const Syntfunc& a_source)
    {
        //TODO(ch): Introduce inplace passes. 
        Assert(&a_dest == &a_source); 
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            m_subintervals[basketNum].resize(a_source.regAmount[basketNum], std::vector<LiveInterval>());
        
        //This function accomplishes four goals:
        //1.) Separates all register live intervals to small def-use subintervals. There can be a lot of usages in one subinterval, but only one definition.
        //2.) Expand subinteravls, which intersects with loop's body, or branches in some certain cases.
        //3.) Rename subintervals got into new variables.
        //4.) Also, find the biggest number of spilled variables needed for deployment of some instructions into snippets(e.g., DIV on intel).
        
        //IMPORTANT: Think around situation 1-0-1, when register is defined inside of block and redefined in another of same depth.(0-1-0, obviously doesn't matter).
        std::multimap<int, LAEvent> CFqueue;
        RegIdx paramsAmount[RB_AMOUNT] = {0, 0};
        { //1.) Calculation of simplest [def-use] subintervals and collect precise info about borders of loops and branches.
            std::deque<ControlFlowBracket> flowstack;
            for (const Arg& par : a_source.params)
            {
                Assert(par.tag == Arg::IREG || par.tag == Arg::VREG);
                int basketNum = (par.tag == Arg::IREG ? RB_INT : RB_VEC);
                def(basketNum, par.idx, 0);
                paramsAmount[basketNum]++;
            }
            int basketElemX[RB_AMOUNT] = {1,1};
            basketElemX[RB_VEC] = m_backend->getVectorRegisterBits() / 64;
            for (int opnum = 0; opnum < (int)a_source.program.size(); opnum++)
            {
                const Syntop& op = a_source.program[opnum];
                int opSnippetSpills = 0;
                for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                    opSnippetSpills += m_backend->spillSpaceNeeded(op, basketNum) * basketElemX[basketNum];
                m_snippetCausedSpills = std::max(m_snippetCausedSpills, opSnippetSpills);
                m_haveFunctionCalls = m_haveFunctionCalls || (op.opcode == OP_CALL_NORET || op.opcode == OP_CALL);
                switch (op.opcode)
                {
                case (OP_IF_CEND):
                {
                    Assert(op.size() == 0);
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::IF, opnum));
                    CFqueue.insert(std::make_pair(opnum, LAEvent(LAEvent::LAE_STARTBRANCH)));
                    continue;
                }
                case (OP_ELSE):
                {
                    Assert(op.size() == 2 && op.args[0].tag == Arg::IIMMEDIATE && op.args[1].tag == Arg::IIMMEDIATE);
                    Assert(flowstack.size() && flowstack.back().tag == ControlFlowBracket::IF);
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::ELSE, opnum));
                    continue;
                }
                case (OP_ENDIF):
                {
                    Assert(op.size() == 1 && op.args[0].tag == Arg::IIMMEDIATE);
                    Assert(flowstack.size());
                    ControlFlowBracket bracket = flowstack.back();
                    flowstack.pop_back();
                    int elsePos = LAEvent::NONDEF;
                    if (bracket.tag == ControlFlowBracket::ELSE)
                    {
                        elsePos = bracket.label_or_pos;
                        Assert(flowstack.size());
                        bracket = flowstack.back();
                        flowstack.pop_back();
                    }
                    Assert(bracket.tag == ControlFlowBracket::IF);
                    auto rator = CFqueue.find(bracket.label_or_pos);
                    Assert(rator != CFqueue.end());
                    int ifStart = rator->first;
                    rator->second.oppositeNestingSide = opnum;
                    rator = CFqueue.insert(std::make_pair(opnum, LAEvent(LAEvent::LAE_ENDBRANCH)));
                    rator->second.elsePos = elsePos;
                    rator->second.oppositeNestingSide = ifStart;
                    continue;
                }
                case (OP_WHILE_CSTART):
                {
                    Assert(op.size() == 1 && op.args[0].tag == Arg::IIMMEDIATE);
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::WHILE, opnum));
                    CFqueue.insert(std::make_pair(opnum, LAEvent(LAEvent::LAE_STARTLOOP)));
                    continue;
                }
                case (OP_ENDWHILE):
                {
                    Assert(op.size() == 2 && op.args[0].tag == Arg::IIMMEDIATE && op.args[1].tag == Arg::IIMMEDIATE);
                    Assert(flowstack.size() && flowstack.back().tag == ControlFlowBracket::WHILE);
                    const ControlFlowBracket& bracket = flowstack.back();
                    flowstack.pop_back();
                    auto rator = CFqueue.find(bracket.label_or_pos);
                    Assert(rator != CFqueue.end());
                    int whilePos = rator->first;
                    rator->second.oppositeNestingSide = opnum;
                    rator = CFqueue.insert(std::make_pair(opnum, LAEvent(LAEvent::LAE_ENDLOOP)));
                    rator->second.oppositeNestingSide = whilePos;
                    continue;
                }
                default:
                    for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                    {
                        std::set<RegIdx> IORegs = m_backend->getUsedRegisters(op, basketNum, AF_INPUT | AF_OUTPUT);
                        std::set<RegIdx> inRegs = m_backend->getInRegisters(op, basketNum);
                        std::set<RegIdx> outRegs = m_backend->getOutRegisters(op, basketNum);
                        for(auto IO : IORegs)  //Register used in one instruction position as Input and Output simultaneously
                        {                      //must not be redefined. First of all, it's usage, and it must be used
                            inRegs.insert(IO); //via already known name.
                            outRegs.erase(IO);
                        }
                        for (RegIdx inreg : inRegs)
                            use(basketNum, inreg, opnum);
                        for (RegIdx outreg : outRegs)
                            def(basketNum, outreg, opnum);
                    }
                    break;
                }
            }
            int alignx = 1;
            for(int bex : basketElemX) 
                alignx = std::max(alignx, bex);
            m_snippetCausedSpills += m_snippetCausedSpills % alignx ? alignx - m_snippetCausedSpills % alignx : 0; 
        }

        { //2.) Calculating intervals crossing loops and embranchments.
            initSubintervalHeaders(-1);
            std::multiset<LiveInterval, endordering> lastActive[RB_AMOUNT]; // NOTE: In this part of code LiveInterval::end means not end position of subinterval, but deactivation position, position, when starts new subinterval or ends final one.
            for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            {
                for (RegIdx idx = 0; idx < regAmount(basketNum); idx++)
                {
                    if (siAmount(basketNum, idx) == 0)
                        continue;
                    const int sintStart = m_subintervals[basketNum][idx][0].start;
                    int eventPos;
                    if (sintStart == 0)
                    {
                        iterateSubinterval(basketNum, idx);
                        eventPos = deactivationOpnum(basketNum, idx);
                        LiveInterval toActive = getCurrentSubinterval(basketNum, idx);
                        toActive.end = eventPos;
                        lastActive[basketNum].insert(toActive);
                        deactivationOpnum(basketNum, idx);
                    }
                    else
                        eventPos = sintStart;
                    CFqueue.insert(std::make_pair(eventPos, LAEvent(LAEvent::LAE_SWITCHSUBINT, idx, basketNum)));
                }
            }

            while (!CFqueue.empty())
            {
                int opnum = CFqueue.begin()->first;
                LAEvent event = CFqueue.begin()->second;
                CFqueue.erase(CFqueue.begin());
                switch (event.eventType)
                {
                case (LAEvent::LAE_SWITCHSUBINT):
                {
                    std::multiset<LiveInterval, endordering>& b_lastActive = lastActive[event.basketNum];
                    if ((getCurrentSinum(event.basketNum, event.idx) + 1) > 0)
                    {
                        auto removerator = b_lastActive.begin();
                        while (removerator != b_lastActive.end() && removerator->idx != event.idx && removerator->end == opnum)
                            ++removerator;
                        if (removerator != b_lastActive.end() && removerator->idx == event.idx)
                            b_lastActive.erase(removerator);
                    }
                    if (isIterateable(event.basketNum, event.idx))
                    {
                        LAEvent toAdd = event;
                        iterateSubinterval(event.basketNum, event.idx);
                        int eventPos = deactivationOpnum(event.basketNum, event.idx);
                        LiveInterval toActive = getCurrentSubinterval(event.basketNum, event.idx);
                        toActive.end = eventPos;
                        b_lastActive.insert(toActive);
                        CFqueue.insert(std::make_pair(eventPos, toAdd));
                    }
                    break;
                };
                case (LAEvent::LAE_STARTBRANCH):
                case (LAEvent::LAE_STARTLOOP):
                {
                    push_active_state(lastActive, event.oppositeNestingSide);
                    break;
                }
                case (LAEvent::LAE_ENDBRANCH):
                {
                    for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                    {
                        const int ifPos = event.oppositeNestingSide;
                        const int endifPos = opnum;
                        const bool haveElse = (event.elsePos != UNDEFINED_OPERATION_NUMBER);
                        const int elsePos = haveElse ? event.elsePos : endifPos;
                        std::multiset<LiveInterval, endordering> lastActiveChanged;
                        for(auto ifidrator = acs_begin(basketNum); ifidrator != acs_end(basketNum); ifidrator++)
                        {
                            int firstUseMain = UNDEFINED_OPERATION_NUMBER, firstDefMain = UNDEFINED_OPERATION_NUMBER;
                            int firstUseElse = UNDEFINED_OPERATION_NUMBER, firstDefElse = UNDEFINED_OPERATION_NUMBER;
                            const RegIdx idx = ifidrator->first;
                            bool afterlife = m_subintervals[basketNum][idx].back().end > endifPos;
                            if (!haveElse && !afterlife)
                                continue;
                            const int initSinum = ifidrator->second;
                            // In next loop we are finding first redefinition of register and
                            // first usage in each branch (firstUseMain, firstDefMain, firstUseElse, firstDefElse)
                            // Also sinum will be number of last subinterval intersected with embranchment.
                            int sinum = initSinum;
                            for (; sinum < siAmount(basketNum, idx); ++sinum)
                            {
                                const int sistart = m_subintervals[basketNum][idx][sinum].start;
                                const int siend = m_subintervals[basketNum][idx][sinum].end;
                                if (sistart > endifPos)
                                {
                                    Assert(sinum > 0);
                                    sinum--;
                                    break;
                                }
                                if (sistart > ifPos)
                                {
                                    if (haveElse && sistart > elsePos && firstDefElse == UNDEFINED_OPERATION_NUMBER)
                                        firstDefElse = sinum;
                                    else if (firstDefMain == UNDEFINED_OPERATION_NUMBER)
                                        firstDefMain = sinum;
                                }
                                if (siend > endifPos)
                                    break;
                                if (siend > ifPos)
                                {
                                    if (haveElse && siend > elsePos && firstUseElse == UNDEFINED_OPERATION_NUMBER)
                                        firstUseElse = sinum;
                                    else if (firstUseMain == UNDEFINED_OPERATION_NUMBER)
                                        firstUseMain = sinum;
                                }
                            }
                            if (sinum == siAmount(basketNum, idx))
                                sinum = siAmount(basketNum, idx) - 1;

                            //Usages after redefinition can be ommited, they are not connected to pre-embranchment register value.
                            if (firstUseMain != UNDEFINED_OPERATION_NUMBER && firstUseMain >= firstDefMain)
                                firstUseMain = UNDEFINED_OPERATION_NUMBER;
                            if (firstUseElse != UNDEFINED_OPERATION_NUMBER && firstUseElse >= firstDefElse)
                                firstUseElse = UNDEFINED_OPERATION_NUMBER;
                            bool splice = false;

                            if(afterlife && (firstUseMain != UNDEFINED_OPERATION_NUMBER || firstDefMain != UNDEFINED_OPERATION_NUMBER || firstUseElse != UNDEFINED_OPERATION_NUMBER || firstDefElse != UNDEFINED_OPERATION_NUMBER))
                            {
                                splice = true;
                            }
                            else if (firstDefMain != UNDEFINED_OPERATION_NUMBER && firstUseElse != UNDEFINED_OPERATION_NUMBER) // Abscence of linear separability.
                            {
                                if (!afterlife && firstDefElse != UNDEFINED_OPERATION_NUMBER)//Tail from firstDefElse can be separated
                                    sinum = firstUseElse;
                                splice = true;
                            }
                            else if (firstDefElse == UNDEFINED_OPERATION_NUMBER && firstUseElse == UNDEFINED_OPERATION_NUMBER)
                            {
                                if (firstDefMain == UNDEFINED_OPERATION_NUMBER)
                                    splice = false;
                                else
                                    splice = true;
                            }
                            else if ((firstDefMain == UNDEFINED_OPERATION_NUMBER && firstDefElse != UNDEFINED_OPERATION_NUMBER) || //One-of-branch redefinition with
                                (firstDefMain != UNDEFINED_OPERATION_NUMBER && firstDefElse == UNDEFINED_OPERATION_NUMBER))        //afterusage means splicing.
                            {
                                splice = true;
                            }
                            else
                                splice = false;
                            if (splice)
                            {
                                const int switchIpos = isIterateable(basketNum, idx) ? getNextSubinterval(basketNum, idx).start : getCurrentSubinterval(basketNum, idx).end;
                                spliceUntilSinum(basketNum, idx, sinum, initSinum);
                                if(switchIpos > opnum)
                                {
                                    LiveInterval changedOne = m_subintervals[basketNum][idx][initSinum];
                                    changedOne.end = deactivationOpnum(basketNum, idx);
                                    lastActiveChanged.insert(changedOne);
                                    auto removerator = lastActive[basketNum].find(LiveInterval(idx, switchIpos));;
                                    while(removerator != lastActive[basketNum].end() && removerator->end == switchIpos && removerator->idx != idx) ++removerator;
                                    Assert(removerator != lastActive[basketNum].end());
                                    lastActive[basketNum].erase(removerator);
                                    moveEventLater(CFqueue, idx, LAEvent::LAE_SWITCHSUBINT, switchIpos, changedOne.end);
                                }
                            }
                        }
                        lastActive[basketNum].insert(lastActiveChanged.begin(), lastActiveChanged.end());
                    };
                    pop_active_state();
                    break;
                }
                case (LAEvent::LAE_ENDLOOP):
                {
                    const int endwhilePos = opnum;
                    for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                    {
                        std::multiset<LiveInterval, endordering> lastActiveChanged;
                        for(auto ifidrator = acs_begin(basketNum); ifidrator != acs_end(basketNum); ifidrator++)
                        {
                            const RegIdx idx = ifidrator->first;
                            const int si_start = ifidrator->second;
                            const int switchIpos = isIterateable(basketNum, idx) ? getNextSubinterval(basketNum, idx).start : getCurrentSubinterval(basketNum, idx).end;
                            if(m_subintervals[basketNum][idx][si_start].end < endwhilePos)
                            {
                                expandUntilOpnum(basketNum, idx, endwhilePos, si_start);
                                auto removerator = lastActive[basketNum].find(LiveInterval(idx, switchIpos));
                                while(removerator != lastActive[basketNum].end() && removerator->end == switchIpos && removerator->idx != idx) ++removerator;
                                if(removerator != lastActive[basketNum].end())
                                {
                                    lastActive[basketNum].erase(removerator);
                                    LiveInterval changedOne = m_subintervals[basketNum][idx][si_start];
                                    changedOne.end = deactivationOpnum(basketNum, idx);
                                    lastActiveChanged.insert(changedOne);
                                    moveEventLater(CFqueue, idx, LAEvent::LAE_SWITCHSUBINT, switchIpos, changedOne.end);
                                }
                            }
                        }
                        lastActive[basketNum].insert(lastActiveChanged.begin(), lastActiveChanged.end());
                    }
                    pop_active_state();
                    break;
                }
                default:
                    throw std::runtime_error("Internal error: unexpected event in branch queue.");
                }
            }
        }

        size_t resSize[RB_AMOUNT];
        { //3.) Renaming splitted registers.
            initSubintervalHeaders();
            for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++ )
            {
                RegIdx pseudIdx = paramsAmount[basketNum];
                for (RegIdx idx = 0; idx < paramsAmount[basketNum]; idx++)
                    for (int si = 1; si < (int)siAmount(basketNum, idx); si++)
                        m_subintervals[basketNum][idx][si].idx = pseudIdx++;
                for (RegIdx idx = paramsAmount[basketNum]; idx < regAmount(basketNum); idx++)
                    for (LiveInterval& li : m_subintervals[basketNum][idx])
                        li.idx = pseudIdx++;
                resSize[basketNum] = pseudIdx;
            }

            for (int opnum = 0; opnum < (int)a_dest.program.size(); opnum++)
            {
                Syntop& op = a_dest.program[opnum];
                std::set<int> outRegArnums[RB_AMOUNT];
                for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++ )
                    outRegArnums[basketNum] = m_backend->getOutRegistersIdxs(op, basketNum);
                for (int arnum = 0; arnum < (int)op.size(); arnum++)
                {
                    Arg& arg = op.args[arnum];
                    int basketNum = (arg.tag == Arg::IREG ? RB_INT: (arg.tag == Arg::VREG ? RB_VEC : RB_AMOUNT));
                    if(basketNum < RB_AMOUNT)
                    {
                        if (arg.idx == Syntfunc::RETREG)
                            continue;
                        bool isOut = (outRegArnums[basketNum].count(arnum) > 0);
                        Assert(siAmount(basketNum, arg.idx) > 0);
                        while (isIterateable(basketNum, arg.idx))
                        {
                            LiveInterval& li = getCurrentSubinterval(basketNum, arg.idx);
                            if (opnum >= li.start && opnum <= li.end)
                                break;
                            iterateSubinterval(basketNum, arg.idx);
                        }
                        size_t sinum = getCurrentSinum(basketNum, arg.idx);
                        if (isIterateable(basketNum, arg.idx) && isOut && (getNextSubinterval(basketNum, arg.idx).start <= opnum))
                            sinum++;
                        arg.idx = m_subintervals[basketNum][arg.idx][sinum].idx;
                    }
                }
            }
        }
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        {
            m_liveintervals[basketNum] = std::vector<LiveInterval>(resSize[basketNum], LiveInterval(0,0));
            for (auto res : m_subintervals[basketNum])
                for (auto pseud : res)
                    m_liveintervals[basketNum][pseud.idx] = pseud;
            a_dest.regAmount[basketNum] = (int)m_liveintervals[basketNum].size();
        }
    }

    LivenessAnalysisAlgoImpl::LivenessAnalysisAlgoImpl(const Backend* a_owner) : LivenessAnalysisAlgo(a_owner, 0)
        , m_snippetCausedSpills(0)
        , m_haveFunctionCalls(false)
    {}

    void LivenessAnalysisAlgoImpl::push_active_state(const std::multiset<LiveInterval, endordering> (&a_lastActive) [RB_AMOUNT]
                    , int a_endif)
    {
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        {
            m_active_headers_stack[basketNum].push_back(std::map<RegIdx, int>());
            auto filled = m_active_headers_stack[basketNum].rbegin();
            for(auto lastactive: a_lastActive[basketNum])
            {
                if(lastactive.end > a_endif) //Will not consider subintervals contains whole embranchement.
                    break;
                RegIdx idx = lastactive.idx;
                filled->insert(std::pair<RegIdx, int>(idx, m_subintervalHeaders[basketNum][idx]));
            }
        }
    }

    void LivenessAnalysisAlgoImpl::pop_active_state()
    {
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        {
            Assert(!m_active_headers_stack[basketNum].empty());
            m_active_headers_stack[basketNum].pop_back();
        }
    }

    std::map<RegIdx, int>::const_iterator LivenessAnalysisAlgoImpl::acs_begin(int basketNum) const
    {
        const std::deque<std::map<RegIdx, int> >& b_states = m_active_headers_stack[basketNum];
        Assert(!b_states.empty());
        return b_states.back().cbegin();
    }

    std::map<RegIdx, int>::const_iterator LivenessAnalysisAlgoImpl::acs_end(int basketNum) const
    {
        const std::deque<std::map<RegIdx, int> >& b_states = m_active_headers_stack[basketNum];
        Assert(!b_states.empty());
        return b_states.back().cend();
    }

    int LivenessAnalysisAlgoImpl::siAmount(int basketNum, RegIdx regNum) const
    {
        Assert(regNum!= IReg::NOIDX && regNum < regAmount(basketNum));
        return (int)m_subintervals[basketNum][regNum].size();
    }

    void LivenessAnalysisAlgoImpl::def(int basketNum, RegIdx regNum, int opnum)
    {
        if (regNum != Syntfunc::RETREG)
        {
            Assert(regNum != IReg::NOIDX && regNum < regAmount(basketNum));
            m_subintervals[basketNum][regNum].push_back(LiveInterval(regNum, opnum));
        }
    }

    void LivenessAnalysisAlgoImpl::use(int basketNum, RegIdx regNum, int opnum)
    {
        if (regNum != Syntfunc::RETREG)
        {
            if (regNum != IReg::NOIDX && !defined(basketNum, regNum))
                throw std::runtime_error("Compile error: using uninitialized register");
            m_subintervals[basketNum][regNum].back().end = opnum;
        }
    }

    void LivenessAnalysisAlgoImpl::spliceUntilSinum(int basketNum, RegIdx regNum, int siEnd, int a_siStart)
    {
        m_subintervalHeaders[basketNum][regNum] = a_siStart == UNDEFINED_OPERATION_NUMBER ? m_subintervalHeaders[basketNum][regNum] : a_siStart;
        int siStart = m_subintervalHeaders[basketNum][regNum];
        Assert(siStart <= siEnd);
        Assert(siEnd != UNDEFINED_OPERATION_NUMBER && siEnd < siAmount(basketNum, regNum));
        m_subintervals[basketNum][regNum][siStart].end = m_subintervals[basketNum][regNum][siEnd].end;
        m_subintervals[basketNum][regNum].erase(m_subintervals[basketNum][regNum].begin() + siStart + 1, m_subintervals[basketNum][regNum].begin() + siEnd + 1);
    }

    int LivenessAnalysisAlgoImpl::expandUntilOpnum(int basketNum, RegIdx regNum, int opnum, int a_siStart)
    {
        m_subintervalHeaders[basketNum][regNum] = a_siStart == UNDEFINED_OPERATION_NUMBER ? m_subintervalHeaders[basketNum][regNum] : a_siStart;
        int si_start = m_subintervalHeaders[basketNum][regNum];
        int subinterval2erase = si_start + 1;
        for (; subinterval2erase < siAmount(basketNum, regNum); subinterval2erase++)
            if (m_subintervals[basketNum][regNum][subinterval2erase].start >= opnum)
            {
                subinterval2erase--;
                break;
            }
            else
                opnum = std::max(m_subintervals[basketNum][regNum][subinterval2erase].end, opnum);
        subinterval2erase = std::min(subinterval2erase, siAmount(basketNum, regNum) - 1);
        spliceUntilSinum(basketNum, regNum, subinterval2erase);
        m_subintervals[basketNum][regNum][si_start].end = opnum;
        return opnum;
    }

    int LivenessAnalysisAlgoImpl::deactivationOpnum(int basketNum, RegIdx regNum)
    {
        int sinum = m_subintervalHeaders[basketNum][regNum];
        return (sinum + 1 < siAmount(basketNum, regNum)) ? m_subintervals[basketNum][regNum][sinum + 1].start : m_subintervals[basketNum][regNum][sinum].end;
    }

    void LivenessAnalysisAlgoImpl::initSubintervalHeaders(int initval)
    {
        for(size_t sibNum = 0; sibNum < RB_AMOUNT; sibNum++)
        {
            m_subintervalHeaders[sibNum].clear();
            m_subintervalHeaders[sibNum].resize(m_subintervals[sibNum].size(), initval);
        }
    }

    int LivenessAnalysisAlgoImpl::getCurrentSinum(int basketNum, RegIdx regNum) //return -> int
    {
        return m_subintervalHeaders[basketNum][regNum];
    }

    LiveInterval& LivenessAnalysisAlgoImpl::getCurrentSubinterval(int basketNum, RegIdx regNum)
    {
        Assert(m_subintervalHeaders[basketNum][regNum] != UNDEFINED_OPERATION_NUMBER && m_subintervalHeaders[basketNum][regNum] < siAmount(basketNum, regNum));
        return m_subintervals[basketNum][regNum][m_subintervalHeaders[basketNum][regNum]];
    }

    LiveInterval& LivenessAnalysisAlgoImpl::getNextSubinterval(int basketNum, RegIdx regNum)
    {
        Assert(m_subintervalHeaders[basketNum][regNum] != UNDEFINED_OPERATION_NUMBER && m_subintervalHeaders[basketNum][regNum] + 1 < siAmount(basketNum, regNum));
        return m_subintervals[basketNum][regNum][m_subintervalHeaders[basketNum][regNum] + 1];
    }

    bool LivenessAnalysisAlgoImpl::isIterateable(int basketNum, RegIdx regNum) const
    {
        return (m_subintervalHeaders[basketNum][regNum] + 1) < siAmount(basketNum, regNum);
    }

    void LivenessAnalysisAlgoImpl::iterateSubinterval(int basketNum, RegIdx regNum)
    {
        if (isIterateable(basketNum, regNum))
            m_subintervalHeaders[basketNum][regNum]++;
    }

    void LivenessAnalysisAlgoImpl::moveEventLater(std::multimap<int, LAEvent>& queue, RegIdx regNum, int eventType, int oldOpnum, int newOpnum)
    {
        auto qremrator = queue.find(oldOpnum);
        while (qremrator != queue.end() && qremrator->first == oldOpnum)
            if (qremrator->second.eventType == eventType && qremrator->second.idx == regNum)
                break;
            else
                qremrator++;
        Assert(qremrator != queue.end() && qremrator->first == oldOpnum);
        LAEvent toRead = qremrator->second;
        queue.erase(qremrator);
        queue.insert(std::make_pair(newOpnum, toRead));
    }

    LivenessAnalysisAlgo::LivenessAnalysisAlgo(const Backend* a_owner) : CompilerPass(a_owner), impl(new LivenessAnalysisAlgoImpl(a_owner))
    {}

    LivenessAnalysisAlgo::~LivenessAnalysisAlgo()
    {
        delete impl;
    }

    void LivenessAnalysisAlgo::process(Syntfunc& a_dest, const Syntfunc& a_source)
    {
        impl->process(a_dest, a_source);
    }

    std::vector<LiveInterval>* LivenessAnalysisAlgo::live_intervals()
    {
        return impl->live_intervals();
    }

    int LivenessAnalysisAlgo::getSnippetCausedSpills() const
    {
        return impl->getSnippetCausedSpills();
    }

    bool LivenessAnalysisAlgo::haveFunctionCalls() const
    {
        return impl->haveFunctionCalls();
    }

    LivenessAnalysisAlgo::LivenessAnalysisAlgo(const Backend* a_owner, int): CompilerPass(a_owner), impl(nullptr) {}
}
