/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include <algorithm>
#include <list>
#include <cstring>
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

Algorithm have four stages:
1.) Lookup for elementary subintervals and initialization of event queues.
    It's known amount of virtual registers, used in code of the function. First step is
    initialization of vector of subintervals, each register will have subintervals breakdown -
    vector of starts and ends of subinterval(for simplicity it's used LiveInterval struct
    everywhere in both algorithms). For each instruction, by making getInRegisters,
    getOutRegisters requests to Backend, algorithm understands, which registers was used in
    instruction and which was redefined. Each elementary subinterval is the chain like def-use-
    use-...-use(use is optional). Algorithm stores info only about first and last link of chain.

    In the same time it's initialized two event queues: for loops and for embranchments. It's
    stored info about each loop into loopQueue with help of control flow stack(flowstack).
    loopQueue[<startLoopPosition>].endNesting is end of loop. Same for branchQueue:
    branchQueue[<startEmbranchementPosition>].endNesting is endif position and
    branchQueue[<startEmbranchementPosition>].elsePos is else position. At this stage elseif are
    already deconstructed.
    Time complexity: O(N) - where N is amount of instructions.
    Space complexity: O(M), M - amount of elementary subintervals.

2.) Splicing subintervals, which intersects loops borders.
    Main idea: if subinterval was alive at start of the loop, it must stay alive until the
    end of loop, that's why in this case all subintervals, intersects loop's interval must be
    spliced in one. For making this splice there will be used loopQueue. It's obvious, that there
    is no need to work with all registers, but only with active. For effective keeping "active"
    container actual it's used same event queue with locating here information about starts end
    ends of subintervals activity.

    It's important, that on algorithm's work there is only forward movement along vector of
    certain register. That's why each subinterval will be considered only once(don't be confused
    by matix view of data).

    In start, for each register it's zeroed number of of current subinterval with help of
    function initSubintervalHeaders(). Registers are activate from 0 instruction(parameters) must
    be added to the event queue with LAE_ENDSUBINT tag, other with LAE_STARTSUBINT. Also
    parameters are added to "active" multiset, which is ordered by ends of intervals. This order
    increases find operations(because element to erase will always be in start of "active"
    container).

    In iterations over instruction  it's keeped "active" and numbers of current subintervals
    actual with help of iterateSubinterval, getNextSubinterval, isIterateable methods.

    When it's meet loop start (there is loop end in event description), each active subinterval
    will spliced all subintervals, intersected with loop body. If loop is finished further, than
    union got, subinterval will be prolongated to end end of loop.

    Time complexity: O(B*M) - where B - number of basic blocks, M - amount of elementary
    subintervals, roughly equal to amount of instructions.
    Space complexity: O(M), M - amount of elementary subintervals.

3.) Splicing subintervals, which intersects embrachements borders.
    Algorithm is similar to previous one, but there is three serious differences:

    First, unfortunately, it cannot be done in loop's linear manner, it need some
    recursion, because splicing in inner embranchments can affect on outer splicing decisions.
 
    The example we have encountered:
    IReg v = CTX.const_(val1);
    CTX.if_(cond1)
        v = CTX.const_(val2);
    CTX.elif(cond2)
        v = CTX.const_(val3);
    CTX.elif(cond3)
        v = CTX.const_(val3);
    CTX.endif();
 
    Without splicing in most inner branch, algorithm inadequately decided to keep register's
    subintervals separated in outer embranchement.
 
    Therefore, embranchment handling must be done after of embranchement. On other hand, algorithm
    needs the list of registers was active in start of block. This lists are stacked in
    BranchStateStack class(pushed at start of block and popped at an end).
  
    Second, on intersection condtions it's used last active intervals instead of active one(that
    is the reason why stage 2 and 3 cannot be united into one stage). E.g:
        Active intervals    LastActive Intervals
    1:        ^                      ^
    2:        |                      |
    3:        v                      |
    4:                               |
    5:                               v
    6:        ^                      ^
    7:        |                      |
    8:        |                      |
    9:        v                      v

    Third, this stage have much more complicated splicing decision logic. It depends on amount
    of branches, position of first "use" and fisrt "def" in each branch and after-embranchment
    usage(afterlife). In some cases it's list of subintervals to splice can be shorted.

    Time complexity same as for previous: O(B*M), where B - number of basic blocks, M - amount
    of elementary subintervals, roughly equal to amount of instructions.
    Space complexity: O(B*M), B - number of basic blocks, M - amount of elementary subintervals.

4.) Renaming.
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
    operands. For this used Backend's filterStackPlaceable method. In bytecode it looks like
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
            std::vector<size_t> origRegistersV[VESS_AMOUNT];

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
                    const uint8_t arRegNum = origRegistersV[vessNum][inRegNum];
                    m_reorderInner2Arch[basketNum][vessNum][inRegNum] = arRegNum;
                    m_reorderArch2Inner[basketNum][vessNum][arRegNum] = inRegNum;
                    m_pool[basketNum] |= (((uint64_t)(1)) << arRegNum);
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
        static const size_t maximumSpills = 3;
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
        size_t res = lsb64(m_spillPlaceholders[basketNum]);
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

    void RegisterPool::overrideRegisterSet(int basketNum, const std::vector<size_t>&  a_parameterRegisters,
                                                          const std::vector<size_t>&  a_returnRegisters,
                                                          const std::vector<size_t>&  a_callerSavedRegisters,
                                                          const std::vector<size_t>&  a_calleeSavedRegisters)
    {
        m_registersO[basketNum][PARAMS_VESS] = a_parameterRegisters;
        m_registersO[basketNum][RETURN_VESS] = a_returnRegisters;
        m_registersO[basketNum][CALLER_VESS] = a_callerSavedRegisters;
        m_registersO[basketNum][CALLEE_VESS] = a_calleeSavedRegisters;
    }

    void RegisterPool::getOverridenParams(std::vector<size_t> (&regParsOverride)[RB_AMOUNT]) const
    {
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            regParsOverride[basketNum] = m_registersO[basketNum][PARAMS_VESS]; 
    }

    void RegisterPool::removeFromAllVessels(int basketNum, size_t reg)
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

    struct LiveInterval
    {
        size_t start, end;
        RegIdx idx;
        LiveInterval(RegIdx a_idx, size_t a_start) : start(a_start), end(a_start), idx(a_idx) {}
    };

    struct startordering
    {
        bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.start < b.start; }
    };

    struct endordering
    {
        bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.end < b.end; }
    };

/*
TODO(ch): Implement with RISC-V RVV
There will be needed modification for support of connected(nested) vectors:
Nested vector register must be redefined on redefinition of container vector,
but only if this nested register will be used after this redefinition.
*/
    class LivenessAnalysisAlgo
    {
    public:
        LivenessAnalysisAlgo(ContextImpl* a_owner);
        void process(Syntfunc& a_processed, std::vector<LiveInterval> (&a_liveintervals)[RB_AMOUNT]);
        size_t getSnippetCausedSpills() const
        {
            return m_snippetCausedSpills;
        }
    private:
        struct LAEvent //Liveness Analysis Event
        {
            enum { LAE_STARTLOOP, LAE_STARTBRANCH, LAE_ENDBRANCH, LAE_STARTSUBINT, LAE_SWITCHSUBINT, LAE_ENDSUBINT, NONDEF = -1 };
            int eventType;
            RegIdx idx;
            size_t elsePos;
            size_t oppositeNestingSide;
            int basketNum;
            LAEvent() : eventType(NONDEF), idx(IReg::NOIDX), elsePos(NONDEF), oppositeNestingSide(NONDEF) {}
            LAEvent(int a_eventType) : eventType(a_eventType), idx(IReg::NOIDX), elsePos(NONDEF), oppositeNestingSide(NONDEF), basketNum(RB_AMOUNT) {}
            LAEvent(int a_eventType, RegIdx a_idx, int basketNum_) : eventType(a_eventType), idx(a_idx), elsePos(NONDEF), oppositeNestingSide(NONDEF), basketNum(basketNum_) {}
        };
        std::vector<std::vector<LiveInterval> > m_subintervals[RB_AMOUNT]; //TODO(ch): std::vector<std::list<LiveInterval> > will avoid moves and allocations.
                                                                   //but in this case m_subintervalHeaders must be std::vector<std::list<LiveInterval>::iterator>
        std::vector<size_t> m_subintervalHeaders[RB_AMOUNT];
        ContextImpl* m_owner;
        size_t m_snippetCausedSpills;
        inline size_t regAmount(int basketNum) const { return m_subintervals[basketNum].size(); }
        inline size_t siAmount(int basketNum, RegIdx regNum) const;
        inline bool defined(int basketNum, RegIdx regNum) const { return siAmount(basketNum, regNum) > 0; }
        inline void def(int basketNum, RegIdx regNum, size_t opnum);
        inline void use(int basketNum, RegIdx regNum, size_t opnum);
        inline void spliceUntilSinum(int basketNum, RegIdx regNum, size_t siEnd, size_t siStart = -1);
        inline size_t expandUntilOpnum(int basketNum, RegIdx regNum, size_t opnum);
        inline size_t deactivationOpnum(int basketNum, RegIdx regNum);
        inline void initSubintervalHeaders(size_t initval = 0);
        inline size_t getCurrentSinum(int basketNum, RegIdx regNum);
        inline LiveInterval& getCurrentSubinterval(int basketNum, RegIdx regNum);
        inline LiveInterval& getNextSubinterval(int basketNum, RegIdx regNum);
        inline bool isIterateable(int basketNum, RegIdx regNum) const; //Well, unfotunately, we don't have after-end-state, only last-one state.
        inline void iterateSubinterval(int basketNum, RegIdx regNum);
        inline void moveEventLater(std::multimap<size_t, LAEvent>& queue, RegIdx regNum, int eventType, size_t oldOpnum, size_t newOpnum);
    };

    RegisterAllocator::RegisterAllocator(ContextImpl* a_owner): m_owner(a_owner)
        , m_pool(a_owner->getBackend())
        , m_epilogueSize(0)
    {
        Assert(m_owner != nullptr);
    }

    //TODO(ch): It's good idea on intel64 + windows to use "shadow space", which is 32 bytes in stack just before
    //5-th parameter(other words - 1-st stack-passsed parameter). It's default and consistent place for spilling
    //register parameters.
    void RegisterAllocator::process(FuncImpl* a_func, Syntfunc& a_processed)
    {
        Backend* backend = m_owner->getBackend();
        m_pool.initRegisterPool();
        size_t snippetCausedSpills;
        std::multiset<LiveInterval, startordering> liveintervals[RB_AMOUNT];
        std::vector<LiveInterval> parintervals[RB_AMOUNT];
        std::vector<RegIdx> paramsSorted[RB_AMOUNT];
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            paramsSorted[basketNum].reserve(a_processed.params.size());
        for (Arg& par : a_processed.params)
        {
            Assert(par.tag == Arg::IREG || par.tag == Arg::VREG);
            int basketNum = (par.tag == Arg::IREG ? RB_INT : RB_VEC);
            paramsSorted[basketNum].push_back(par.idx);
        }
        //Liveness analysis call
        {
            LivenessAnalysisAlgo LAalgo(m_owner);
            std::vector<LiveInterval> analysisResult[RB_AMOUNT];
            LAalgo.process(a_processed, analysisResult);
            for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            {
                a_processed.regAmount[basketNum] = analysisResult[basketNum].size();
                //After liveness analysis we are sorting given liveintervals into two heaps:
                //parameters, which are immediately marked as active and other intervals,
                //which are reordered by start positions to work with Linear scan algorithm.
                size_t idx = 0;
                size_t idxParMax = std::min(analysisResult[basketNum].size(), paramsSorted[basketNum].size());
                parintervals[basketNum].reserve(idxParMax);
                for (; idx < idxParMax; ++idx)
                    parintervals[basketNum].push_back(analysisResult[basketNum][idx]);
                idxParMax = analysisResult[basketNum].size();
                for (; idx < idxParMax; ++idx)
                    liveintervals[basketNum].insert(analysisResult[basketNum][idx]);
                snippetCausedSpills = LAalgo.getSnippetCausedSpills();
            }
        }

        //Space in stack used by snippets will be located in the bottom,
        //so spilled variables will be located higher.
        int64_t spoffset[RB_AMOUNT] = {0, 0};

        std::map<RegIdx, size_t> stackParamLayout[RB_AMOUNT];
        {
            std::vector<size_t> regParsOverride[RB_AMOUNT];
            m_pool.getOverridenParams(regParsOverride);
            backend->getStackParameterLayout(a_processed, regParsOverride, stackParamLayout);
        }

        //Looking spilled registers and reusing registers with renaming map.
        std::vector<Arg> regReassignment[RB_AMOUNT];
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++) 
        {
            const int REGtag = ((basketNum == RB_INT) ? Arg::IREG : Arg::VREG);
            std::multiset<LiveInterval, endordering> active;
            regReassignment[basketNum].resize(a_processed.regAmount[basketNum], Arg());
            {//Get pseudonames for parameters.
                RegIdx parreg = 0;
                for (; parreg < parintervals[basketNum].size(); parreg++)
                {
                    RegIdx idx = parintervals[basketNum][parreg].idx;
                    RegIdx attempt = m_pool.provideParamFromPool(basketNum);
                    if (attempt == IReg::NOIDX)
                        break;
                    regReassignment[basketNum][idx] = argReg(basketNum, attempt);
                    active.insert(parintervals[basketNum][parreg]);
                }
                for (; parreg < parintervals[basketNum].size(); parreg++)
                {
                    RegIdx idx = parintervals[basketNum][parreg].idx;
                    RegIdx attempt = m_pool.provideRegFromPool(basketNum);
                    if (attempt == IReg::NOIDX)
                        break;
                    regReassignment[basketNum][idx] = argReg(basketNum, attempt);
                    active.insert(parintervals[basketNum][parreg]);
                }
                for (; parreg < parintervals[basketNum].size(); parreg++)
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
                    bool stackParameterSpilled = false;
                    if (!active.empty() && active.rbegin()->end > interval->end)
                    {
                        regReassignment[basketNum][interval->idx] = regReassignment[basketNum][active.rbegin()->idx];
                        RegIdx keepidx = regReassignment[basketNum][active.rbegin()->idx].idx; //We need to know appointed target architecture register for spilled parameters.
                        stackParameterSpilled = stackParamLayout->count(active.rbegin()->idx);
                        regReassignment[basketNum][active.rbegin()->idx] = argSpilled(basketNum, stackParameterSpilled ? 0 : spoffset[basketNum]);
                        regReassignment[basketNum][active.rbegin()->idx].idx = keepidx;
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
                    RegIdx poolHint = IReg::NOIDX;
                    active.insert(*interval);
                    //There we looking around last used input registers and trying to reuse them as
                    //out. This optimization is important, e.g., for add, sub and mul operation on Intel, where
                    //this operation are binary, not ternary.
                    if (opUndefs.size())
                    {
                        std::unordered_map<size_t, RegIdx> opUndefsIdxMap;
                        std::set<size_t> opUndefsIdx;
                        const Syntop& op = a_processed.program[interval->start];
                        std::set<size_t> iNs = backend->getInRegistersIdxs(op, basketNum);
                        for (size_t in : iNs)
                            if (opUndefs.count(op[in].idx))
                            {
                                opUndefsIdxMap[in] = opUndefs.at(op[in].idx);
                                opUndefsIdx.insert(in);
                            }
                        size_t idxHint = backend->reusingPreferences(op, opUndefsIdx);
                        if (idxHint != IReg::NOIDX)
                            poolHint = opUndefsIdxMap.at(idxHint);
                    }
                    regReassignment[basketNum][interval->idx] = argReg(basketNum, m_pool.provideRegFromPool(basketNum, poolHint));
                }
            }
        }

        const Arg retreg = argReg(RB_INT, m_pool.provideReturnFromPool(RB_INT));
        auto getReassigned = [retreg, &regReassignment](int basketNum, size_t old)
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

        size_t basketElemX[RB_AMOUNT] = {1,1};
        basketElemX[RB_VEC] = backend->getVectorRegisterBits() / 64;
        
        std::vector<std::map<RegIdx, Arg> > unspilledRenaming[RB_AMOUNT];
        std::vector<std::map<RegIdx, Arg> > spilledRenaming[RB_AMOUNT];
        std::vector<std::set<size_t> > stackPlaceable[RB_AMOUNT];
        size_t nettoSpills[RB_AMOUNT];
        size_t spAddAligned = snippetCausedSpills;
        //Collecting instructionwise spills properties
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
        {
            unspilledRenaming[basketNum].resize(a_processed.program.size());
            spilledRenaming[basketNum].resize(a_processed.program.size());
            stackPlaceable[basketNum].resize(a_processed.program.size());
            for (size_t opnum = 0; opnum < a_processed.program.size(); ++opnum)
            {
                Syntop& op = a_processed.program[opnum];
                std::set<size_t> unspilledIdxs;
                std::set<size_t> spilledIdxs;
                const int REGtag = ((basketNum == RB_INT) ? Arg::IREG : Arg::VREG);
                const int SPLtag = ((basketNum == RB_INT) ? Arg::ISPILLED : Arg::VSPILLED);
                unspilledIdxs = backend->getInRegistersIdxs(op, basketNum);
                spilledIdxs = backend->getOutRegistersIdxs(op, basketNum);
                for (std::set<size_t>::iterator removerator = unspilledIdxs.begin(); removerator != unspilledIdxs.end();)
                {
                    size_t argNum = (*removerator);
                    Assert(argNum < op.size() && op.args[argNum].tag == REGtag);
                    if (getReassigned(basketNum, op.args[argNum].idx).tag == SPLtag)
                        removerator++;
                    else
                        removerator = unspilledIdxs.erase(removerator);
                }
                for (std::set<size_t>::iterator removerator = spilledIdxs.begin(); removerator != spilledIdxs.end();)
                {
                    size_t argNum = (*removerator);
                    Assert(argNum < op.size() && op.args[argNum].tag == REGtag);
                    if (getReassigned(basketNum, op.args[argNum].idx).tag == SPLtag)
                        removerator++;
                    else
                        removerator = spilledIdxs.erase(removerator);
                }
                stackPlaceable[basketNum][opnum] = spilledIdxs;
                stackPlaceable[basketNum][opnum].insert(unspilledIdxs.begin(), unspilledIdxs.end());
                stackPlaceable[basketNum][opnum] = backend->filterStackPlaceable(op, stackPlaceable[basketNum][opnum]);
                for (std::set<size_t>::iterator removerator = unspilledIdxs.begin(); removerator != unspilledIdxs.end();)
                    if (stackPlaceable[basketNum][opnum].count(*removerator) != 0)
                        removerator = unspilledIdxs.erase(removerator);
                    else
                        removerator++;
                for (std::set<size_t>::iterator removerator = spilledIdxs.begin(); removerator != spilledIdxs.end();)
                    if (stackPlaceable[basketNum][opnum].count(*removerator) != 0)
                        removerator = spilledIdxs.erase(removerator);
                    else
                        removerator++;

                m_pool.clearSpillPlaceholders(basketNum);
                for (size_t argNum : unspilledIdxs)
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

                for (size_t argNum : spilledIdxs)
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
                op.spillPrefix += unspilledRenaming[basketNum][opnum].size();
                op.spillPostfix += spilledRenaming[basketNum][opnum].size();
            }
            const int SPLtag = ((basketNum == RB_INT) ? Arg::ISPILLED : Arg::VSPILLED);
            size_t parametersStoodSpilled = 0; 
            for(auto p : stackParamLayout[basketNum]) 
                if (regReassignment[basketNum][p.first].tag == SPLtag)
                    parametersStoodSpilled++;
            nettoSpills[basketNum] = std::count_if(regReassignment[basketNum].begin(), regReassignment[basketNum].end(), [SPLtag](const Arg& arg) {return arg.tag == SPLtag; }) - parametersStoodSpilled;
            nettoSpills[basketNum] += m_pool.usedCallee(basketNum).size();
            spAddAligned += nettoSpills[basketNum] * basketElemX[basketNum];
        }
        spAddAligned = backend->stackGrowthAlignment(spAddAligned);

        size_t basketOffset[RB_AMOUNT] = {0,0};
        {
            std::vector<int> stackBasketOrder = backend->getStackBasketOrder();
            basketOffset[stackBasketOrder[0]] = snippetCausedSpills;
            for(int stackBasketNum = 1; stackBasketNum < stackBasketOrder.size(); stackBasketNum++)
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
        newProgUnbracketed.reserve(a_processed.program.size() * 3);
        for (size_t opnum = 0; opnum < a_processed.program.size(); ++opnum)
        {
            Syntop& op = a_processed.program[opnum];
            for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                for (auto ar : unspilledRenaming[basketNum][opnum])
                    newProgUnbracketed.push_back(Syntop(OP_UNSPILL, { ar.second, argIImm(getSpillOffset(basketNum, ar.first )) })); 
            
            for (size_t arnum = 0; arnum < op.size(); arnum++)
            {
                Arg& ar = op[arnum];
                if (ar.tag == Arg::IREG || ar.tag == Arg::VREG)
                {
                    int basketNum = (ar.tag == Arg::IREG ? RB_INT : RB_VEC);
                    if (stackPlaceable[basketNum][opnum].count(arnum) != 0)
                        ar = argSpilled(basketNum, getSpillOffset(basketNum, ar.idx), a_func);
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
        
        std::vector<Syntop> newProg;
        newProg.reserve(a_processed.program.size() * 3);
        { //Write prologue
            if (spAddAligned)
            {
                newProg.push_back(Syntop(OP_SUB, { backend->getSParg(a_func), backend->getSParg(a_func), argIImm(spAddAligned * 8) }));
                for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                {
                    for (RegIdx par : paramsSorted[basketNum])
                        if (regReassignment[basketNum][par].tag == Arg::ISPILLED && (stackParamLayout[basketNum].count(par) == 0))
                        {
                            Arg spilled = regReassignment[basketNum][par];
                            spilled.tag = basketNum == RB_INT ? Arg::IREG : Arg::VREG;
                            newProg.push_back(Syntop(OP_SPILL, { argIImm(getSpillOffset(basketNum, par)), spilled }));
                        }
                    size_t savNum = (nettoSpills[basketNum] - m_pool.usedCallee(basketNum).size()) * basketElemX[basketNum];
                    for (RegIdx toSav : m_pool.usedCallee(basketNum))
                    {
                        Arg spilled = argReg(basketNum, toSav);
                        if(basketNum == RB_VEC)
                            spilled.elemtype = TYPE_U8; // We actually don't care, just taking simplest type.
                        newProg.push_back(Syntop(OP_SPILL, { argIImm(basketOffset[basketNum] + savNum), spilled }));
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
                        newProg.push_back(Syntop(OP_UNSPILL, { regReassignment[basketNum][idx], argIImm(spAddAligned + param.second) }));
                    }
                }
        }
        newProg.insert(newProg.end(), newProgUnbracketed.begin(), newProgUnbracketed.end());
        m_epilogueSize = newProg.size();
        { //Write epilogue
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
                        newProg.push_back(Syntop(OP_UNSPILL, { spilled, argIImm(basketOffset[basketNum] + savNum) }));
                        savNum += basketElemX[basketNum];
                    }
                }
                newProg.push_back(Syntop(OP_ADD, { backend->getSParg(a_func), backend->getSParg(a_func), argIImm(spAddAligned * 8) }));
            }
        }
        m_epilogueSize = newProg.size() - m_epilogueSize;
        a_processed.program = newProg;
    }

    class BranchStateStack
    {
    private:
        std::deque<std::map<RegIdx, size_t> > m_states[RB_AMOUNT];
    public:
        void pushState(const std::multiset<LiveInterval, endordering> (&a_lastActive) [RB_AMOUNT]
                       , const std::vector<size_t> (&a_subintervalHeaders) [RB_AMOUNT]
                       , size_t a_endif)
        {
            for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            {
                m_states[basketNum].push_back(std::map<RegIdx, size_t>());
                auto filled = m_states[basketNum].rbegin();
                for(auto lastactive: a_lastActive[basketNum])
                {
                    if(lastactive.end > a_endif) //Will not consider subintervals contains whole embranchement.
                        break;
                    RegIdx idx = lastactive.idx;
                    filled->insert(std::pair<RegIdx, size_t>(idx, a_subintervalHeaders[basketNum][idx]));
                }
            }
        }

        void popState()
        {
            for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            {
                Assert(!m_states[basketNum].empty());
                m_states[basketNum].pop_back();
            }
        }

        std::map<RegIdx, size_t>::const_iterator begin(int basketNum) const
        {
            const std::deque<std::map<RegIdx, size_t> >& b_states = m_states[basketNum];
            Assert(!b_states.empty());
            return b_states.back().cbegin();
        }

        std::map<RegIdx, size_t>::const_iterator end(int basketNum) const
        {
            const std::deque<std::map<RegIdx, size_t> >& b_states = m_states[basketNum];
            Assert(!b_states.empty());
            return b_states.back().cend();
        }
    };

    void LivenessAnalysisAlgo::process(Syntfunc& a_processed, std::vector<LiveInterval> (&a_liveintervals)[RB_AMOUNT])
    {
        for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            m_subintervals[basketNum].resize(a_processed.regAmount[basketNum], std::vector<LiveInterval>());
        
        //This function accomplishes four goals:
        //1.) Separates all register live intervals to small def-use subintervals. There can be a lot of usages in one subinterval, but only one definition.
        //2.) Expand subinteravls, which intersects with loop's body, or branches in some certain cases.
        //3.) Rename subintervals got into new variables.
        //4.) Also, find the biggest number of spilled variables needed for deployment of some instructions into snippets(e.g., DIV on intel).
        
        Backend* backend = m_owner->getBackend();
        //IMPORTANT: Think around situation 1-0-1, when register is defined inside of block and redefined in another of same depth.(0-1-0, obviously doesn't matter).
        std::multimap<size_t, LAEvent> loopQueue;
        std::multimap<size_t, LAEvent> branchQueue;
        RegIdx paramsAmount[RB_AMOUNT] = {0, 0};
        { //1.) Calculation of simplest [def-use] subintervals and collect precise info about borders of loops and branches.
            std::deque<ControlFlowBracket> flowstack;
            for (Arg& par : a_processed.params)
            {
                Assert(par.tag == Arg::IREG || par.tag == Arg::VREG);
                int basketNum = (par.tag == Arg::IREG ? RB_INT : RB_VEC);
                def(basketNum, par.idx, 0);
                paramsAmount[basketNum]++;
            }
            size_t basketElemX[RB_AMOUNT] = {1,1};
            basketElemX[RB_VEC] = backend->getVectorRegisterBits() / 64;
            for (size_t opnum = 0; opnum < a_processed.program.size(); opnum++)
            {
                const Syntop& op = a_processed.program[opnum];
                size_t opSnippetSpills = 0;
                for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                    opSnippetSpills += backend->spillSpaceNeeded(op, basketNum) * basketElemX[basketNum];
                m_snippetCausedSpills = std::max(m_snippetCausedSpills, opSnippetSpills);
                switch (op.opcode)
                {
                case (OP_IF):
                {
                    Assert(op.size() == 2 && op.args[0].tag == Arg::IIMMEDIATE && op.args[1].tag == Arg::IIMMEDIATE);
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::IF, opnum));
                    branchQueue.insert(std::make_pair(opnum, LAEvent(LAEvent::LAE_STARTBRANCH)));
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
                    size_t elsePos = LAEvent::NONDEF;
                    if (bracket.tag == ControlFlowBracket::ELSE)
                    {
                        elsePos = bracket.labelOrPos;
                        Assert(flowstack.size());
                        bracket = flowstack.back();
                        flowstack.pop_back();
                    }
                    Assert(bracket.tag == ControlFlowBracket::IF);
                    auto rator = branchQueue.find(bracket.labelOrPos);
                    Assert(rator != branchQueue.end());
                    size_t ifStart = rator->first;
                    rator->second.oppositeNestingSide = opnum;
                    rator = branchQueue.insert(std::make_pair(opnum, LAEvent(LAEvent::LAE_ENDBRANCH)));
                    rator->second.elsePos = elsePos;
                    rator->second.oppositeNestingSide = ifStart;
                    continue;
                }
                case (OP_WHILE):
                {
                    Assert(op.size() == 3 && op.args[0].tag == Arg::IIMMEDIATE && op.args[1].tag == Arg::IIMMEDIATE && op.args[2].tag == Arg::IIMMEDIATE);
                    if (opnum < 2)
                        throw std::runtime_error("Temporary condition solution needs one instruction before WHILE cycle.");
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::WHILE, opnum - 1));
                    loopQueue.insert(std::make_pair(opnum - 1, LAEvent(LAEvent::LAE_STARTLOOP))); //TODO(ch): IMPORTANT(CMPLCOND): This(opnum - 1) mean that condition can be one-instruction only.
                    continue;
                }
                case (OP_ENDWHILE):
                {
                    Assert(op.size() == 2 && op.args[0].tag == Arg::IIMMEDIATE && op.args[1].tag == Arg::IIMMEDIATE);
                    Assert(flowstack.size() && flowstack.back().tag == ControlFlowBracket::WHILE);
                    const ControlFlowBracket& bracket = flowstack.back();
                    flowstack.pop_back();
                    auto rator = loopQueue.find(bracket.labelOrPos);
                    Assert(rator != loopQueue.end());
                    rator->second.oppositeNestingSide = opnum;
                    continue;
                }
                default:
                    for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                    {
                        std::set<RegIdx> IORegs = backend->getUsedRegisters(op, basketNum, BinTranslation::Token::T_INPUT | BinTranslation::Token::T_OUTPUT);
                        std::set<RegIdx> inRegs = backend->getInRegisters(op, basketNum);
                        std::set<RegIdx> outRegs = backend->getOutRegisters(op, basketNum);
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
        }

        { //2.) Expanding intervals, which are crossing loops borders.
            initSubintervalHeaders();
            std::multiset<LiveInterval, endordering> active[RB_AMOUNT];
            for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            {
                for (RegIdx idx = 0; idx < regAmount(basketNum); idx++)
                {
                    const size_t sintStart = getCurrentSubinterval(basketNum, idx).start;
                    if (sintStart == 0)
                    {
                        LiveInterval toActive = getCurrentSubinterval(basketNum, idx);
                        active[basketNum].insert(toActive);
                        loopQueue.insert(std::make_pair(toActive.end, LAEvent(LAEvent::LAE_ENDSUBINT, idx, basketNum)));
                    }
                    else
                        loopQueue.insert(std::make_pair(sintStart, LAEvent(LAEvent::LAE_STARTSUBINT, idx, basketNum)));
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
                    std::multiset<LiveInterval, endordering>& b_active = active[event.basketNum];
                    LiveInterval li = getCurrentSubinterval(event.basketNum, event.idx);
                    b_active.insert(li);
                    const size_t sintEnd = li.end;
                    loopQueue.insert(std::make_pair(sintEnd, LAEvent(LAEvent::LAE_ENDSUBINT, event.idx, event.basketNum)));
                    break;
                };
                case (LAEvent::LAE_ENDSUBINT):
                {
                    std::multiset<LiveInterval, endordering>& b_active = active[event.basketNum];
                    auto removerator = b_active.begin();
                    while (removerator != b_active.end() && removerator->idx != event.idx && removerator->end == opnum)
                        ++removerator;
                    Assert(removerator != b_active.end() && removerator->idx == event.idx);
                    b_active.erase(removerator);
                    if (isIterateable(event.basketNum, event.idx))
                    {
                        const LiveInterval& newli = getNextSubinterval(event.basketNum, event.idx);
                        iterateSubinterval(event.basketNum, event.idx);
                        loopQueue.insert(std::make_pair(newli.start, LAEvent(LAEvent::LAE_STARTSUBINT, event.idx, event.basketNum)));
                    }
                    break;
                };
                case (LAEvent::LAE_STARTLOOP):
                {
                    for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                    {
                        std::multiset<LiveInterval, endordering> changed_active;
                        auto arator = active[basketNum].begin();
                        for (; arator != active[basketNum].end(); ++arator)
                            if (arator->end < event.oppositeNestingSide)
                            {
                                const RegIdx idx = arator->idx;
                                size_t newEnd = expandUntilOpnum(basketNum, idx, event.oppositeNestingSide);
                                changed_active.insert(getCurrentSubinterval(basketNum, idx));
                                moveEventLater(loopQueue, arator->idx, LAEvent::LAE_ENDSUBINT, arator->end, newEnd);
                            }
                            else
                                break;
                        active[basketNum].erase(active[basketNum].begin(), arator);
                        active[basketNum].insert(changed_active.begin(), changed_active.end());
                    };
                    break;
                }
                default:
                    throw std::runtime_error("Internal error: unexpected event in loop queue.");
                }
            }
        }

        { //3.) Calculating intervals crossing if branches.
            initSubintervalHeaders(-1);
            std::multiset<LiveInterval, endordering> lastActive[RB_AMOUNT]; // NOTE: In this part of code LiveInterval::end means not end position of subinterval, but deactivation position, position, when starts new subinterval or ends final one.
            BranchStateStack branchStack;
            for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
            {
                for (RegIdx idx = 0; idx < regAmount(basketNum); idx++)
                {
                    if (siAmount(basketNum, idx) == 0)
                        continue;
                    const size_t sintStart = m_subintervals[basketNum][idx][0].start;
                    size_t eventPos;
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
                    branchQueue.insert(std::make_pair(eventPos, LAEvent(LAEvent::LAE_SWITCHSUBINT, idx, basketNum)));
                }
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
                        size_t eventPos = deactivationOpnum(event.basketNum, event.idx);
                        LiveInterval toActive = getCurrentSubinterval(event.basketNum, event.idx);
                        toActive.end = eventPos;
                        b_lastActive.insert(toActive);
                        branchQueue.insert(std::make_pair(eventPos, toAdd));
                    }
                    break;
                };
                case (LAEvent::LAE_STARTBRANCH):
                {
                    branchStack.pushState(lastActive, m_subintervalHeaders, event.oppositeNestingSide);
                    break;
                }
                case (LAEvent::LAE_ENDBRANCH):
                {
                    for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                    {
                        const size_t ifPos = event.oppositeNestingSide;
                        const size_t endifPos = opnum;
                        const bool haveElse = (event.elsePos != LAEvent::NONDEF);
                        const size_t elsePos = haveElse ? event.elsePos : endifPos;
                        std::multiset<LiveInterval, endordering> lastActiveChanged;
                        for(auto ifidrator = branchStack.begin(basketNum); ifidrator != branchStack.end(basketNum); ifidrator++)
                        {
                            static const size_t NOTFOUND = -1;
                            size_t firstUseMain = NOTFOUND, firstDefMain = NOTFOUND;
                            size_t firstUseElse = NOTFOUND, firstDefElse = NOTFOUND;
                            const RegIdx idx = ifidrator->first;
                            bool afterlife = (m_subintervals[basketNum][idx].back().end > endifPos);
                            if (!haveElse && !afterlife)
                                continue;
                            const size_t initSinum = ifidrator->second;
                            // In next loop we are finding first redefinition of register and
                            // first usage in each branch (firstUseMain, firstDefMain, firstUseElse, firstDefElse)
                            // Also sinum will be number of last subinterval intersected with embranchment.
                            size_t sinum = initSinum;
                            for (; sinum < siAmount(basketNum, idx); ++sinum)
                            {
                                const size_t sistart = m_subintervals[basketNum][idx][sinum].start;
                                const size_t siend = m_subintervals[basketNum][idx][sinum].end;
                                if (sistart > endifPos)
                                {
                                    Assert(sinum > 0);
                                    sinum--;
                                    break;
                                }
                                if (sistart > ifPos)
                                {
                                    if (haveElse && sistart > elsePos && firstDefElse == NOTFOUND)
                                        firstDefElse = sinum;
                                    else if (firstDefMain == NOTFOUND)
                                        firstDefMain = sinum;
                                }
                                if (siend > endifPos)
                                    break;
                                if (siend > ifPos)
                                {
                                    if (haveElse && siend > elsePos && firstUseElse == NOTFOUND)
                                        firstUseElse = sinum;
                                    else if (firstUseMain == NOTFOUND)
                                        firstUseMain = sinum;
                                }
                            }
                            if (sinum == siAmount(basketNum, idx))
                                sinum = siAmount(basketNum, idx) - 1;

                            //Usages after redefinition can be ommited, they are not connected to pre-embranchment register value.
                            if (firstUseMain != NOTFOUND && firstUseMain >= firstDefMain)
                                firstUseMain = NOTFOUND;
                            if (firstUseElse != NOTFOUND && firstUseElse >= firstDefElse)
                                firstUseElse = NOTFOUND;
                            bool splice = false;

                            if (firstDefMain != NOTFOUND && firstUseElse != NOTFOUND) // Abscence of linear separability.
                            {
                                if (!afterlife && firstDefElse != NOTFOUND)//Tail from firstDefElse can be separated
                                    sinum = firstUseElse;
                                splice = true;
                            }
                            else if (!afterlife)
                            {
                                splice = false;
                            }
                            else if (firstDefElse == NOTFOUND && firstUseElse == NOTFOUND)
                            {
                                if (firstDefMain == NOTFOUND)
                                    splice = false;
                                else
                                    splice = true;
                            }
                            else if ((firstDefMain == NOTFOUND && firstDefElse != NOTFOUND) || //One-of-branch redefinition with
                                (firstDefMain != NOTFOUND && firstDefElse == NOTFOUND))        //afterusage means splicing.
                            {
                                splice = true;
                            }
                            else
                                splice = false;
                            if (splice)
                            {
                                const size_t switchIpos = isIterateable(basketNum, idx) ? getNextSubinterval(basketNum, idx).start : getCurrentSubinterval(basketNum, idx).end;
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
                                    moveEventLater(branchQueue, idx, LAEvent::LAE_SWITCHSUBINT, switchIpos, changedOne.end);
                                }
                            }
                        }
                        lastActive[basketNum].insert(lastActiveChanged.begin(), lastActiveChanged.end());
                    };
                    branchStack.popState();
                    break;
                }
                default:
                    throw std::runtime_error("Internal error: unexpected event in branch queue.");
                }
            }
        }

        size_t resSize[RB_AMOUNT];
        { //4.) Renaming splitted registers.
            initSubintervalHeaders();
            for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++ )
            {
                RegIdx pseudIdx = paramsAmount[basketNum];
                for (RegIdx idx = 0; idx < paramsAmount[basketNum]; idx++)
                    for (size_t si = 1; si < siAmount(basketNum, idx); si++)
                        m_subintervals[basketNum][idx][si].idx = pseudIdx++;
                for (RegIdx idx = paramsAmount[basketNum]; idx < regAmount(basketNum); idx++)
                    for (LiveInterval& li : m_subintervals[basketNum][idx])
                        li.idx = pseudIdx++;
                resSize[basketNum] = pseudIdx;
            }

            for (size_t opnum = 0; opnum < a_processed.program.size(); opnum++)
            {
                Syntop& op = a_processed.program[opnum];
                std::set<size_t> outRegArnums[RB_AMOUNT];
                for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++ )
                    outRegArnums[basketNum] = backend->getOutRegistersIdxs(op, basketNum);
                for (size_t arnum = 0; arnum < op.size(); arnum++)
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
            a_liveintervals[basketNum] = std::vector<LiveInterval>(resSize[basketNum], LiveInterval(0,0));
            for (auto res : m_subintervals[basketNum])
                for (auto pseud : res)
                    a_liveintervals[basketNum][pseud.idx] = pseud;
        }
    }

    LivenessAnalysisAlgo::LivenessAnalysisAlgo(ContextImpl* a_owner) : m_snippetCausedSpills(0)
        , m_owner(a_owner)
    {}

    size_t LivenessAnalysisAlgo::siAmount(int basketNum, RegIdx regNum) const
    {
        Assert(regNum < regAmount(basketNum));
        return m_subintervals[basketNum][regNum].size();
        
    };

    void LivenessAnalysisAlgo::def(int basketNum, RegIdx regNum, size_t opnum)
    {
        if (regNum != Syntfunc::RETREG)
        {
            Assert(regNum < regAmount(basketNum));
            m_subintervals[basketNum][regNum].push_back(LiveInterval(regNum, opnum));
        }
    }

    void LivenessAnalysisAlgo::use(int basketNum, RegIdx regNum, size_t opnum)
    {
        if (regNum != Syntfunc::RETREG)
        {
            if (!defined(basketNum, regNum))
                throw std::runtime_error("Compile error: using uninitialized register");
            m_subintervals[basketNum][regNum].back().end = opnum;
        }
    }

    void LivenessAnalysisAlgo::spliceUntilSinum(int basketNum, RegIdx regNum, size_t siEnd, size_t a_siStart)
    {
        m_subintervalHeaders[basketNum][regNum] = a_siStart == -1 ? m_subintervalHeaders[basketNum][regNum] : a_siStart;
        size_t siStart = m_subintervalHeaders[basketNum][regNum];
        Assert(siStart <= siEnd);
        Assert(siEnd < siAmount(basketNum, regNum));
        m_subintervals[basketNum][regNum][siStart].end = m_subintervals[basketNum][regNum][siEnd].end;
        m_subintervals[basketNum][regNum].erase(m_subintervals[basketNum][regNum].begin() + siStart + 1, m_subintervals[basketNum][regNum].begin() + siEnd + 1);
    }

    size_t LivenessAnalysisAlgo::expandUntilOpnum(int basketNum, RegIdx regNum, size_t opnum)
    {
        size_t sinum = m_subintervalHeaders[basketNum][regNum];
        size_t subinterval2erase = sinum + 1;
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
        m_subintervals[basketNum][regNum][sinum].end = opnum;
        return opnum;
    }

    size_t LivenessAnalysisAlgo::deactivationOpnum(int basketNum, RegIdx regNum)
    {
        size_t sinum = m_subintervalHeaders[basketNum][regNum];
        return (sinum + 1 < siAmount(basketNum, regNum)) ? m_subintervals[basketNum][regNum][sinum + 1].start : m_subintervals[basketNum][regNum][sinum].end;
    }

    void LivenessAnalysisAlgo::initSubintervalHeaders(size_t initval)
    {
        for(size_t sibNum = 0; sibNum < RB_AMOUNT; sibNum++)
        {
            m_subintervalHeaders[sibNum].clear();
            m_subintervalHeaders[sibNum].resize(m_subintervals[sibNum].size(), initval);
        }
    }

    size_t LivenessAnalysisAlgo::getCurrentSinum(int basketNum, RegIdx regNum)
    {
        return m_subintervalHeaders[basketNum][regNum];
    }

    LiveInterval& LivenessAnalysisAlgo::getCurrentSubinterval(int basketNum, RegIdx regNum)
    {
        Assert(m_subintervalHeaders[basketNum][regNum] < siAmount(basketNum, regNum));
        return m_subintervals[basketNum][regNum][m_subintervalHeaders[basketNum][regNum]];
    }

    LiveInterval& LivenessAnalysisAlgo::getNextSubinterval(int basketNum, RegIdx regNum)
    {
        Assert(m_subintervalHeaders[basketNum][regNum] + 1 < siAmount(basketNum, regNum));
        return m_subintervals[basketNum][regNum][m_subintervalHeaders[basketNum][regNum] + 1];
    }

    bool LivenessAnalysisAlgo::isIterateable(int basketNum, RegIdx regNum) const
    {
        return (m_subintervalHeaders[basketNum][regNum] + 1) < siAmount(basketNum, regNum);
    }

    void LivenessAnalysisAlgo::iterateSubinterval(int basketNum, RegIdx regNum)
    {
        if (isIterateable(basketNum, regNum))
            m_subintervalHeaders[basketNum][regNum]++;
    }

    void LivenessAnalysisAlgo::moveEventLater(std::multimap<size_t, LAEvent>& queue, RegIdx regNum, int eventType, size_t oldOpnum, size_t newOpnum)
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
};
