/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include <algorithm>
#include "reg_allocator.hpp"
#include "common.hpp"
#include "func_impl.hpp"

namespace loops
{
    inline size_t firstOneBit64(uint64_t bigNum)
    {
        uint64_t firstReg = (bigNum & ~(bigNum - 1));
        static const uint8_t bnBase[8] = { 0, 8, 16, 24, 32, 40, 48, 56 };
        static const uint8_t bnAdd[129] = { 0,1,2,0,3,0,0,0,4,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                            6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                            7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                            8 };
        uint32_t first32 = (uint32_t)(firstReg & 0xFFFFFFFF);
        uint32_t second32 = (uint32_t)(firstReg >> 32);
        uint8_t bytenum = (second32 != 0) << 2;
        uint32_t bnHalf = first32 + ((~(uint32_t)(second32 == 0)) & second32);

        uint16_t first16 = (uint16_t)(bnHalf & 0xFFFF);
        uint16_t second16 = (uint16_t)(bnHalf >> 16);
        bytenum += (second16 != 0) << 1;
        uint16_t bnHalfHalf = first16 + ((~(uint32_t)(second16 == 0)) & second16);

        uint8_t first8 = (uint8_t)(bnHalfHalf & 0xFF);
        uint8_t second8 = (uint8_t)(bnHalfHalf >> 8);
        bytenum += (second8!= 0);
        uint8_t bytecontent = first8 + ((~(uint32_t)(second8 == 0)) & second8);

        return static_cast<size_t>(bnBase[bytenum]) + static_cast<size_t>(bnAdd[bytecontent]) - 1;
    }

    inline size_t pickFirstBit64(uint64_t& bigNum)
    {
        Assert(bigNum != 0);
        size_t ret = firstOneBit64(bigNum);
        bigNum = (bigNum | (uint64_t(1) << ret)) ^ (uint64_t(1) << ret);
        return ret;
    }

    inline size_t amountOfBits64(uint64_t bigNum)
    {
        size_t res = 0;
        static const uint8_t amountInByte[256] = 
            { 0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
              1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
              1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
              2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8 };
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        return res;
    }

    const size_t RegisterPool::m_maximumSpills = 3;    //TODO(ch):need more detailed scheme
    const size_t RegisterPool::NOREGISTER = static_cast<size_t>(-1);

    RegisterPool::RegisterPool(ContextImpl* a_owner): m_owner(a_owner)
        , m_pool(0)
        , m_parameterRegisters(0)
        , m_returnRegisters(0)
        , m_callerSavedRegisters(0)
        , m_calleeSavedRegisters(0)
        , m_spillPlaceholders(0)
        , m_spillPlaceholdersAvailable(0)
        , m_parameterRegistersO(0)
        , m_returnRegistersO(0)
        , m_callerSavedRegistersO(0)
        , m_calleeSavedRegistersO(0)
    {}

    void RegisterPool::initRegisterPool()
    {
        Backend* backend = m_owner->getBackend();
        uint64_t origParameterRegisters = backend->parameterRegisters();
        uint64_t origReturnRegisters = backend->returnRegisters();
        uint64_t origCallerSavedRegisters = backend->callerSavedRegisters();
        uint64_t origCalleeSavedRegisters = backend->calleeSavedRegisters();
        if (m_parameterRegistersO != 0 || m_returnRegistersO != 0 ||
            m_callerSavedRegistersO != 0 || m_calleeSavedRegistersO != 0)
        {
            origParameterRegisters = m_parameterRegistersO;
            origReturnRegisters = m_returnRegistersO;
            origCallerSavedRegisters = m_callerSavedRegistersO;
            origCalleeSavedRegisters = m_calleeSavedRegistersO;
        }
        std::unordered_map<IRegInternal, size_t> invertOrderMapping;
        size_t regMax = 0;
        {
            invertOrderMapping.clear();
            m_regOrder.clear();
            m_regOrder.reserve(amountOfBits64(origParameterRegisters) + amountOfBits64(origReturnRegisters) + amountOfBits64(origCallerSavedRegisters) + amountOfBits64(origCalleeSavedRegisters));
            auto append2maps = [this, &regMax, &invertOrderMapping](uint64_t basket)
            {
                while (basket)
                {
                    size_t reg = pickFirstBit64(basket);
                    auto irator = invertOrderMapping.find(reg);
                    if (irator == invertOrderMapping.end())
                    {
                        regMax = std::max(regMax, reg);
                        m_regOrder.push_back(reg);
                        invertOrderMapping[reg] = m_regOrder.size() - 1;
                    }
                }
            };
            append2maps(origParameterRegisters);
            append2maps(origReturnRegisters);
            append2maps(origCallerSavedRegisters);
            append2maps(origCalleeSavedRegisters);
            
            m_invertOrderMapping.clear();
            m_invertOrderMapping.resize(regMax + 1, IReg::NOIDX);
            for (auto regPair : invertOrderMapping)
            {
                m_invertOrderMapping[regPair.first] = regPair.second;
            }

            m_spillPlaceholdersAvailable = m_pool = (((uint64_t)(1)) << m_regOrder.size()) - 1;
            auto translateBasket = [this](uint64_t basket)
            {
                uint64_t res = 0 ;
                while (basket)
                {
                    size_t regSrc = pickFirstBit64(basket);
                    size_t regDest = m_invertOrderMapping[regSrc];
                    Assert(regDest != IReg::NOIDX);
                    res |= (uint64_t(1) << regDest);
                }
                return res;
            };
            m_parameterRegisters = translateBasket(origParameterRegisters);
            m_returnRegisters = translateBasket(origReturnRegisters);
            m_callerSavedRegisters = translateBasket(origCallerSavedRegisters);
            m_calleeSavedRegisters = translateBasket(origCalleeSavedRegisters);
        }
        m_usedCallee.clear();
    }

    size_t RegisterPool::freeRegsAmount() const
    {
        return amountOfBits64(m_pool) - m_maximumSpills;
    }

    IRegInternal RegisterPool::provideParamFromPool()
    {
        IRegInternal res = firstOneBit64(m_parameterRegisters & m_pool);
        if (res == size_t(NOREGISTER))
            return IReg::NOIDX;
        removeFromAllBaskets(res);
        res = m_regOrder[res];
        return res;
    }

    IRegInternal RegisterPool::provideRegFromPool(IRegInternal a_hint)
    {
        static const size_t maximumSpills = 3;
        IRegInternal res = NOREGISTER;
        if (a_hint != IReg::NOIDX)
        {
            Assert(m_invertOrderMapping[a_hint] != IReg::NOIDX);
            res = m_invertOrderMapping[a_hint];
            Assert(((uint64_t(1)) << res) & m_pool);
        }
        else if (havefreeRegs())
        {
            res = firstOneBit64(m_pool);
            if (((uint64_t(1)) << res) & m_calleeSavedRegisters)
                m_usedCallee.insert(m_regOrder[res]);
        }
        removeFromAllBaskets(res);
        res = (res != NOREGISTER) ? m_regOrder[res] : IReg::NOIDX;
        return res;
    }

    IRegInternal RegisterPool::provideReturnFromPool()
    {
        IRegInternal res = firstOneBit64(m_returnRegisters); //TODO(ch): It will works only for one return register.
        Assert(res != NOREGISTER);
        removeFromAllBaskets(res);
        res = m_regOrder[res];
        return res;
    }

    void RegisterPool::releaseRegister(IRegInternal freeReg)
    {
        Assert(freeReg != IReg::NOIDX);
        Assert(m_invertOrderMapping[freeReg] != IReg::NOIDX);
        m_pool |= uint64_t(1) << m_invertOrderMapping[freeReg];
    }

    IRegInternal RegisterPool::provideSpillPlaceholder()
    {
        if (m_spillPlaceholders == 0)
            return IReg::NOIDX;
        size_t res = firstOneBit64(m_spillPlaceholders);
        uint64_t pos = 1;
        pos <<= res;
        m_spillPlaceholders = (m_spillPlaceholders | pos) ^ (pos);
        if (pos & m_calleeSavedRegisters)
            m_usedCallee.insert(m_regOrder[res]);
        return m_regOrder[res];
    }

    void RegisterPool::clearSpillPlaceholders()
    {
        m_spillPlaceholders = m_spillPlaceholdersAvailable;
    }

    void RegisterPool::overrideRegisterSet(uint64_t a_parameterRegisters, uint64_t a_returnRegisters,
        uint64_t a_callerSavedRegisters, uint64_t a_calleeSavedRegisters)
    {
        m_parameterRegistersO = a_parameterRegisters;
        m_returnRegistersO = a_returnRegisters;
        m_callerSavedRegistersO = a_callerSavedRegisters;
        m_calleeSavedRegistersO = a_calleeSavedRegisters;
    }

    void RegisterPool::removeFromAllBaskets(size_t reg)
    {
        if (reg == NOREGISTER)
            return;
        uint64_t pos = 1;
        pos <<= reg;
        m_pool = (m_pool | pos) ^ (pos);
        if (firstOneBit64(m_pool) > firstOneBit64(m_spillPlaceholdersAvailable))
            m_spillPlaceholdersAvailable = m_pool;
    }

    struct LiveInterval
    {
        size_t start, end;
        IRegInternal idx;
        LiveInterval(IRegInternal a_idx, size_t a_start) : start(a_start), end(a_start), idx(a_idx) {}
    };

    struct startordering
    {
        bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.start < b.start; }
    };

    struct endordering
    {
        bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.end < b.end; }
    };

    struct idxordering
    {
        bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.idx < b.idx; }
    };

    class LivenessAnalysisAlgo
    {
    public:
        LivenessAnalysisAlgo(size_t virtualRegsAmount, ContextImpl* a_owner);
        std::set<LiveInterval, idxordering> process(Syntfunc& a_processed);
        size_t getSnippetCausedSpills() const { return m_snippetCausedSpills; }
    private:
        struct LAEvent //Liveness Analysis Event
        {
            enum { LAE_STARTLOOP, LAE_STARTBRANCH, LAE_STARTSUBINT, LAE_SWITCHSUBINT, LAE_ENDSUBINT, NONDEF = -1 };
            int eventType;
            IRegInternal idx;
            size_t elsePos;
            size_t endNesting;
            LAEvent() : eventType(NONDEF), idx(IReg::NOIDX), elsePos(NONDEF), endNesting(NONDEF) {}
            LAEvent(int a_eventType) : eventType(a_eventType), idx(IReg::NOIDX), elsePos(NONDEF), endNesting(NONDEF) {}
            LAEvent(int a_eventType, IRegInternal a_idx) : eventType(a_eventType), idx(a_idx), elsePos(NONDEF), endNesting(NONDEF) {}
        };
        std::vector<std::vector<LiveInterval> > m_subintervals;//TODO(ch): std::vector<std::list<LiveInterval> > will avoid moves and allocations.
        std::vector<size_t> m_subintervalHeaders;              //but in this case m_subintervalHeaders must be std::vector<std::list<LiveInterval>::iterator>
        size_t m_virtualRegsAmount;
        ContextImpl* m_owner;
        size_t m_snippetCausedSpills;
        inline size_t size() const { return m_subintervals.size(); };
        inline size_t size(size_t regNum) const;
        inline bool defined(size_t regNum) const { return size(regNum) > 0; }
        inline void def(size_t regNum, size_t opnum);
        inline void use(size_t regNum, size_t opnum);
        inline void spliceUntilSinum(size_t regNum, size_t siEnd);
        inline size_t expandUntilOpnum(size_t regNum, size_t opnum);
        inline size_t deactivationOpnum(size_t regNum);
        inline void initSubintervalHeaders(size_t initval = 0);
        inline size_t getCurrentSinum(size_t regNum);
        inline LiveInterval& getCurrentSubinterval(size_t regNum);
        inline LiveInterval& getNextSubinterval(size_t regNum);
        inline bool isIterateable(size_t regNum) const; //Well, unfotunately, we don't have after-end-state, only last-one state. 
        inline void iterateSubinterval(size_t regNum);
        inline void moveEventLater(std::multimap<size_t, LAEvent>& queue, size_t regNum, int eventType, size_t oldOpnum, size_t newOpnum);
    };

    RegisterAllocator::RegisterAllocator(ContextImpl* a_owner): m_owner(a_owner)
        , m_pool(a_owner)
        , m_epilogueSize(0)
    {
        Assert(m_owner != nullptr);
    }

    void RegisterAllocator::process(FuncImpl* a_func, Syntfunc& a_processed, size_t a_virtualRegsAmount)
    {
        Backend* backend = m_owner->getBackend();
        m_pool.initRegisterPool();
        size_t snippetCausedSpills = 0;
        std::multiset<LiveInterval, startordering> liveintervals;
        std::multiset<LiveInterval, endordering> active;
        {
            LivenessAnalysisAlgo LAalgo(a_virtualRegsAmount, m_owner);
            std::set<LiveInterval, idxordering> analysisResult = LAalgo.process(a_processed);
            a_virtualRegsAmount = analysisResult.size();
            auto interval = analysisResult.begin();

            //After liveness analysis we are sorting given liveintervals into two heaps: 
            //parameters, which are immediately marked as active and other intervals,
            //which are reordered by start positions to work with Linear scan algorithm. 
            for (; interval != analysisResult.end() && interval->idx < a_processed.params.size(); ++interval)
                active.insert(*interval);
            for (; interval != analysisResult.end(); ++interval)
                liveintervals.insert(*interval);
            snippetCausedSpills = LAalgo.getSnippetCausedSpills();
        }

        //Space in stack used by snippets will be located in the bottom, 
        //so spilled variables will be located higher. 
        int64_t spoffset = snippetCausedSpills;

        std::vector<Arg> regReassignment;
        regReassignment.resize(a_virtualRegsAmount, Arg());
        std::vector<IRegInternal> paramsFromStack;
        std::map<IRegInternal, size_t>  stackParametersIndex;
        { // Looking spilled registers and reusing registers with renaming map.
            {//Get pseudonames for parameters.
                IRegInternal parreg = 0;
                for (; parreg < a_processed.params.size(); parreg++)
                {
                    size_t attempt = m_pool.provideParamFromPool();
                    if (attempt == IReg::NOIDX)
                        break;
                    regReassignment[parreg] = argIReg(attempt);
                }
                paramsFromStack.resize(a_processed.params.size() - parreg);
                std::copy(a_processed.params.begin() + parreg, a_processed.params.end(), paramsFromStack.begin());
                for (; parreg < a_processed.params.size(); parreg++)
                {
                    size_t attempt = m_pool.provideRegFromPool();
                    if (attempt == IReg::NOIDX)
                        break;
                    regReassignment[parreg] = argIReg(attempt);
                    stackParametersIndex.insert(std::pair<IRegInternal, size_t>(parreg, 0));
                }
                for (; parreg < a_processed.params.size(); parreg++)
                {
                    regReassignment[parreg] = argISpilled(0);
                    stackParametersIndex.insert(std::pair<IRegInternal, size_t>(parreg, 0));
                }
            }
            for (auto interval = liveintervals.begin(); interval != liveintervals.end(); ++interval)
            {
                std::unordered_map<IRegInternal, IRegInternal> opUndefs; //TODO(ch): You also have to consider spilled undefs.
                { //Dropping expired registers.
                    auto removerator = active.begin();
                    for (; removerator != active.end(); ++removerator)
                        if (removerator->end <= interval->start)
                        {
                            Assert(regReassignment[removerator->idx].tag == Arg::IREG);
                            m_pool.releaseRegister(regReassignment[removerator->idx].idx);
                            if (removerator->end == interval->start) //Current line, line of definition of considered register
                                opUndefs.insert(std::pair<IRegInternal,IRegInternal>(removerator->idx, regReassignment[removerator->idx].idx));
                        }
                        else
                            break;
                    active.erase(active.begin(), removerator);
                }
                if (!m_pool.havefreeRegs())
                {
                    bool stackParameterSpilled = false;
                    if (!active.empty() && active.rbegin()->end > interval->end)
                    {
                        regReassignment[interval->idx] = regReassignment[active.rbegin()->idx];
                        IRegInternal keepidx = regReassignment[active.rbegin()->idx].idx; //We need to know appointed target architecture register for spilled parameters.
                        stackParameterSpilled = stackParametersIndex.count(active.rbegin()->idx);
                        regReassignment[active.rbegin()->idx] = argISpilled(stackParameterSpilled ? 0 : spoffset);
                        regReassignment[active.rbegin()->idx].idx = keepidx;
                        active.erase(--(active.end()));
                        active.insert(*interval);
                    }
                    else
                    {
                        IRegInternal keepidx = regReassignment[interval->idx].idx; //We need to know appointed target architecture register for spilled parameters.
                        stackParameterSpilled = stackParametersIndex.count(interval->idx);
                        regReassignment[interval->idx] = argISpilled(stackParameterSpilled ? 0 : spoffset);
                        regReassignment[interval->idx].idx = keepidx;
                    }
                    if(!stackParameterSpilled) 
                        spoffset++;
                }
                else
                {
                    IRegInternal poolHint = IReg::NOIDX;
                    active.insert(*interval);
                    //There we looking around last used input registers and trying to reuse them as
                    //out. This optimization is important, e.g., for add, sub and mul operation on Intel, where
                    //this operation are binary, not ternary.
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
                        if (idxHint != IReg::NOIDX)
                            poolHint = opUndefsIdxMap.at(idxHint);
                    }
                    regReassignment[interval->idx] = argIReg(m_pool.provideRegFromPool(poolHint));
                }
            }
        }

        const Arg retreg = argIReg(m_pool.provideReturnFromPool());
        auto getReassigned = [retreg, &regReassignment](size_t old)
        {
            return (old == Syntfunc::RETREG ? retreg : regReassignment[old]);
        };

        // TODO(ch):
        // 1.) Let's consider sequence of instructions, where it's used one register. Obviously, it can be unspilled only once at start of sequence and
        // spilled only once at end. But for now it will spill/unspill at each instruction. I think, this unefficiency can be easily avoided by using some
        // variable-spill map.
        // 2.) Also, we have to take into account live intervals of spilled variables. At some moment place in memory for one variable can be used for
        // another variable.
        // 3.) By the way, when we are using only least spill placeholder, instead of using as much of them, as possible - it's bad practice. 
        // minimizing prologue/epilogue overhead isn't so important.
        std::list<std::tuple<size_t, size_t, IRegInternal> > stackParamsStoodSpilled; //<opnum, argnum, oldidx>
        stackParametersIndex.clear();
        for (size_t spnum = 0; spnum < paramsFromStack.size(); spnum++)
        {
            IRegInternal reg = paramsFromStack[spnum];
            if (regReassignment[reg].tag == Arg::ISPILLED)
                stackParametersIndex.insert(std::pair<IRegInternal, size_t>(reg, spnum));
        }

        std::vector<Syntop> newProgUnbracketed;
        newProgUnbracketed.reserve(a_processed.program.size() * 3);
        {//Renaming registers and adding spill operations
            for (size_t opnum = 0; opnum < a_processed.program.size(); ++opnum)
            {
                Syntop op = a_processed.program[opnum];
                std::set<size_t> unspilledIdxs = backend->getInRegistersIdxs(op);
                std::set<size_t> spilledIdxs = backend->getOutRegistersIdxs(op);
                for (std::set<size_t>::iterator removerator = unspilledIdxs.begin(); removerator != unspilledIdxs.end();)
                {
                    size_t argNum = (*removerator);
                    Assert(argNum < op.size() && op.args[argNum].tag == Arg::IREG);
                    if (getReassigned(op.args[argNum].idx).tag == Arg::ISPILLED)
                        removerator++;
                    else
                        removerator = unspilledIdxs.erase(removerator);
                }
                for (std::set<size_t>::iterator removerator = spilledIdxs.begin(); removerator != spilledIdxs.end();)
                {
                    size_t argNum = (*removerator);
                    Assert(argNum < op.size() && op.args[argNum].tag == Arg::IREG);
                    if (getReassigned(op.args[argNum].idx).tag == Arg::ISPILLED)
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
                {
                    Assert(argNum < op.size() && op.args[argNum].tag == Arg::IREG);
                    unspilledArgs.insert(op.args[argNum].idx);
                }
                std::set<IRegInternal> spilledArgs;
                for (size_t argNum : spilledIdxs)
                {
                    Assert(argNum < op.size() && op.args[argNum].tag == Arg::IREG);
                    spilledArgs.insert(op.args[argNum].idx);
                }

                std::map<IRegInternal, IRegInternal> argRenaming;

                m_pool.clearSpillPlaceholders();
                for (IRegInternal regAr : unspilledArgs)
                {
                    IRegInternal pseudoname = m_pool.provideSpillPlaceholder();
                    if (pseudoname == IReg::NOIDX)
                        throw std::string("Register allocator : not enough free registers.");
                    argRenaming[regAr] = pseudoname;
                    newProgUnbracketed.push_back(Syntop(OP_UNSPILL, { argIReg(pseudoname, a_func), getReassigned(regAr).value }));
                    if (stackParametersIndex.count(regAr))
                        stackParamsStoodSpilled.push_back(std::tuple<size_t, size_t, IRegInternal>(newProgUnbracketed.size() - 1, 1, regAr));
                }
                for (IRegInternal regAr : spilledArgs)
                    if(argRenaming.count(regAr) == 0)
                    {
                        IRegInternal pseudoname = m_pool.provideSpillPlaceholder();
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
                        {
                            IRegInternal oldidx = ar.idx;
                            ar = argISpilled(getReassigned(ar.idx).value, a_func);
                            if (stackParametersIndex.count(oldidx))
                                stackParamsStoodSpilled.push_back(std::tuple<size_t, size_t, IRegInternal>(newProgUnbracketed.size(), arnum, oldidx));
                        }
                        else
                            ar.idx = (argRenaming.count(ar.idx) == 0) ? getReassigned(ar.idx).idx : argRenaming[ar.idx];
                    }
                }
                newProgUnbracketed.push_back(op);
                for (IRegInternal regAr : spilledArgs)
                {
                    newProgUnbracketed.push_back(Syntop(OP_SPILL, { getReassigned(regAr).value, argIReg(argRenaming[regAr], a_func) }));
                    if (stackParametersIndex.count(regAr))
                        stackParamsStoodSpilled.push_back(std::tuple<size_t, size_t, IRegInternal>(newProgUnbracketed.size() - 1, 0, regAr));
                }
            }
        }

        size_t nettoSpills = std::count_if(regReassignment.begin(), regReassignment.end(), [](const Arg& arg) {return arg.tag == Arg::ISPILLED; }) - stackParametersIndex.size();
        nettoSpills += m_pool.usedCallee().size();
        size_t spAddAligned = backend->stackGrowthAlignment(nettoSpills + snippetCausedSpills);

        // Fixing stack-passed spill numbers, which was uncalculatble until now
        for (std::tuple<size_t, size_t, IRegInternal> spss : stackParamsStoodSpilled)
        {
            size_t opnum = std::get<0>(spss);
            size_t arnum = std::get<1>(spss);
            size_t oldid = std::get<2>(spss);
            newProgUnbracketed[opnum][arnum].value = backend->stackParamOffset(nettoSpills, snippetCausedSpills) + stackParametersIndex[oldid];
        }

        std::vector<Syntop> newProg;
        newProg.reserve(a_processed.program.size() * 3);
        { //Write prologue
            if (spAddAligned)
            {
                newProg.push_back(Syntop(OP_SUB, { backend->getSParg(a_func), backend->getSParg(a_func), argIConst(spAddAligned * 8) }));
                for (IRegInternal par : a_processed.params)
                    if (regReassignment[par].tag == Arg::ISPILLED && (std::find(paramsFromStack.begin(), paramsFromStack.end(), par) == paramsFromStack.end()))
                        newProg.push_back(Syntop(OP_SPILL, { argIConst(regReassignment[par].value), argIReg(regReassignment[par].idx) }));
                size_t savNum = nettoSpills + snippetCausedSpills - m_pool.usedCallee().size();
                for (IRegInternal toSav : m_pool.usedCallee())
                    newProg.push_back(Syntop(OP_SPILL, { argIConst(savNum++), argIReg(toSav) }));
            }
            for (size_t stackParamNum = 0; stackParamNum < paramsFromStack.size(); stackParamNum++)
            {
                IRegInternal idx = paramsFromStack[stackParamNum];
                if (regReassignment[idx].tag == Arg::IREG)
                    newProg.push_back(Syntop(OP_UNSPILL, { argIReg(regReassignment[idx].idx), argIConst(backend->stackParamOffset(nettoSpills, snippetCausedSpills) + stackParamNum) }));
            }
        }
        newProg.insert(newProg.end(), newProgUnbracketed.begin(), newProgUnbracketed.end());
        m_epilogueSize = newProg.size();
        { //Write epilogue
            if (spAddAligned)
            {
                size_t savNum = nettoSpills + snippetCausedSpills - m_pool.usedCallee().size();
                for (IRegInternal toSav : m_pool.usedCallee())
                    newProg.push_back(Syntop(OP_UNSPILL, { argIReg(toSav), argIConst(savNum++) }));
                newProg.push_back(Syntop(OP_ADD, { backend->getSParg(a_func), backend->getSParg(a_func), argIConst(spAddAligned * 8) }));
            }
        }
        m_epilogueSize = newProg.size() - m_epilogueSize;
        a_processed.program = newProg;
    }

    std::set<LiveInterval, idxordering> LivenessAnalysisAlgo::process(Syntfunc& a_processed)
    {
        //This function accomplishes four goals:
        //1.) Separates all register live intervals to small def-use subintervals. There can be a lot of usages in one subinterval, but only one definition.
        //2.) Expand subinteravls, which intersects with loop's body, or branches in some certain cases.
        //3.) Rename subintervals got into new variables.
        //4.) Also, find the biggest number of spilled variables needed for deployment of some instructions into snippets(e.g., DIV on intel).

        m_snippetCausedSpills = 0;
        Backend* backend = m_owner->getBackend();
        //IMPORTANT: Think around situation 1-0-1, when register is defined inside of block and redefined in another of same depth.(0-1-0, obviously doesn't matter).
        std::multimap<size_t, LAEvent> loopQueue;
        std::multimap<size_t, LAEvent> branchQueue;
        { //1.) Calculation of simplest [def-use] subintervals and collect precise info about borders of loops and branches.
            std::deque<ControlFlowBracket> flowstack;
            for (IRegInternal par = 0; par < a_processed.params.size(); par++)
                def(par, 0);
            for (size_t opnum = 0; opnum < a_processed.program.size(); opnum++)
            {
                const Syntop& op = a_processed.program[opnum];
                m_snippetCausedSpills = std::max(m_snippetCausedSpills, backend->spillSpaceNeeded(op));
                switch (op.opcode)
                {
                case (OP_IF):
                {
                    Assert(op.size() == 2 && op.args[0].tag == Arg::ICONST && op.args[1].tag == Arg::ICONST);
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::IF, opnum));
                    branchQueue.insert(std::make_pair(opnum, LAEvent(LAEvent::LAE_STARTBRANCH)));
                    continue;
                }
                case (OP_ELSE):
                {
                    Assert(op.size() == 2 && op.args[0].tag == Arg::ICONST && op.args[1].tag == Arg::ICONST);
                    Assert(flowstack.size() && flowstack.back().tag == ControlFlowBracket::IF);
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::ELSE, opnum));
                    continue;
                }
                case (OP_ENDIF):
                {
                    Assert(op.size() == 1 && op.args[0].tag == Arg::ICONST);
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
                    rator->second.endNesting = opnum;
                    rator->second.elsePos = elsePos;
                    continue;
                }
                case (OP_DO):
                {
                    Assert(op.size() == 1 && op.args[0].tag == Arg::ICONST);
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::DO, opnum));
                    loopQueue.insert(std::make_pair(opnum, LAEvent(LAEvent::LAE_STARTLOOP)));
                    continue;
                }
                case (OP_WHILE):
                {
                    Assert(op.size() == 4 && op.args[0].tag == Arg::ICONST && op.args[1].tag == Arg::ICONST && op.args[2].tag == Arg::ICONST && op.args[3].tag == Arg::ICONST);
                    Assert(flowstack.size() && flowstack.back().tag == ControlFlowBracket::DO);
                    const ControlFlowBracket& bracket = flowstack.back();
                    flowstack.pop_back();
                    auto rator = loopQueue.find(bracket.labelOrPos);
                    Assert(rator != loopQueue.end());
                    rator->second.endNesting = opnum;
                    continue;
                }
                case (OP_DOIF):
                {
                    Assert(op.size() == 3 && op.args[0].tag == Arg::ICONST && op.args[1].tag == Arg::ICONST && op.args[2].tag == Arg::ICONST);
                    if (opnum < 2)
                        throw std::string("Temporary condition solution needs one instruction before DOIF cycle.");
                    flowstack.push_back(ControlFlowBracket(ControlFlowBracket::DOIF, opnum - 2));
                    loopQueue.insert(std::make_pair(opnum - 2, LAEvent(LAEvent::LAE_STARTLOOP))); //TODO(ch): IMPORTANT(CMPLCOND): This(opnum - 2) mean that condition can be one-instruction only.
                    continue;
                }
                case (OP_ENDDO):
                {
                    Assert(op.size() == 2 && op.args[0].tag == Arg::ICONST && op.args[1].tag == Arg::ICONST);
                    Assert(flowstack.size() && flowstack.back().tag == ControlFlowBracket::DOIF);
                    const ControlFlowBracket& bracket = flowstack.back();
                    flowstack.pop_back();
                    auto rator = loopQueue.find(bracket.labelOrPos);
                    Assert(rator != loopQueue.end());
                    rator->second.endNesting = opnum;
                    continue;
                }
                default:
                {
                    std::set<IRegInternal> inRegs = backend->getInRegisters(op);
                    std::set<IRegInternal> outRegs = backend->getOutRegisters(op);
                    //std::set<IRegInternal> inOutRegs = backend->getUsedRegisters(op, Binatr::Detail::D_INPUT | Binatr::Detail::D_OUTPUT);  //TODO(ch): check on ARM, that it works fine without these commented lines.
                     //for (IRegInternal reg : inOutRegs)
                     //    outRegs.erase(reg);
                    for (IRegInternal inreg : inRegs)
                        use(inreg, opnum);
                    for (IRegInternal outreg : outRegs)
                        def(outreg, opnum);
                    break;
                }
                }
            }
        }

        { //2.) Expanding loop intervals, which are crossing loops borders.
            initSubintervalHeaders();
            std::multiset<LiveInterval, endordering> active;
            for (size_t idx = 0; idx < m_virtualRegsAmount; idx++)
            {
                const size_t sintStart = m_subintervals[idx][0].start;
                if (sintStart == 0)
                {
                    LiveInterval toActive = getCurrentSubinterval(idx);
                    active.insert(toActive);
                    loopQueue.insert(std::make_pair(toActive.end, LAEvent(LAEvent::LAE_ENDSUBINT, idx)));
                }
                else
                    loopQueue.insert(std::make_pair(sintStart, LAEvent(LAEvent::LAE_STARTSUBINT, idx)));
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
                    LiveInterval li = getCurrentSubinterval(event.idx);
                    active.insert(li);
                    const size_t sintEnd = li.end;
                    loopQueue.insert(std::make_pair(sintEnd, LAEvent(LAEvent::LAE_ENDSUBINT, event.idx)));
                    break;
                };
                case (LAEvent::LAE_ENDSUBINT):
                {
                    auto removerator = active.begin();
                    while (removerator != active.end() && removerator->idx != event.idx && removerator->end == opnum)
                        ++removerator;
                    Assert(removerator != active.end() && removerator->idx == event.idx);
                    active.erase(removerator);
                    if (isIterateable(event.idx))
                    {
                        const LiveInterval& newli = getNextSubinterval(event.idx);
                        iterateSubinterval(event.idx);
                        loopQueue.insert(std::make_pair(newli.start, LAEvent(LAEvent::LAE_STARTSUBINT, event.idx)));
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
                            const IRegInternal idx = arator->idx;
                            size_t newEnd = expandUntilOpnum(idx, event.endNesting);
                            changed_active.insert(getCurrentSubinterval(idx));
                            moveEventLater(loopQueue, arator->idx, LAEvent::LAE_ENDSUBINT, arator->end, newEnd);
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
            initSubintervalHeaders(-1);
            std::multiset<LiveInterval, endordering> lastActive; // NOTE: In this part of code LiveInterval::end means not end position of subinterval, but deactivation position, position, when starts new subinterval or ends final one.
            for (size_t idx = 0; idx < m_virtualRegsAmount; idx++)
            {
                if (size(idx) == 0)
                    continue;
                const size_t sintStart = m_subintervals[idx][0].start;
                size_t eventPos;
                if (sintStart == 0)
                {
                    iterateSubinterval(idx);
                    eventPos = deactivationOpnum(idx);
                    LiveInterval toActive = getCurrentSubinterval(idx);
                    toActive.end = eventPos;
                    lastActive.insert(toActive);
                    deactivationOpnum(idx);
                }
                else
                    eventPos = m_subintervals[idx][0].start;
                branchQueue.insert(std::make_pair(eventPos, LAEvent(LAEvent::LAE_SWITCHSUBINT, idx)));
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
                    if ((getCurrentSinum(event.idx) + 1) > 0)
                    {
                        auto removerator = lastActive.begin();
                        while (removerator != lastActive.end() && removerator->idx != event.idx && removerator->end == opnum)
                            ++removerator;
                        if (removerator != lastActive.end() && removerator->idx == event.idx)
                            lastActive.erase(removerator);
                    }
                    if (isIterateable(event.idx))
                    {
                        LAEvent toAdd = event;
                        iterateSubinterval(event.idx);
                        size_t eventPos = deactivationOpnum(event.idx);
                        LiveInterval toActive = getCurrentSubinterval(event.idx);
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
                        static const size_t NOTFOUND = -1;
                        size_t firstUseMain = NOTFOUND, firstDefMain = NOTFOUND;
                        size_t firstUseElse = NOTFOUND, firstDefElse = NOTFOUND;
                        const IRegInternal idx = larator->idx;
                        bool afterlife = (m_subintervals[idx].back().end > endifPos);
                        if (!haveElse && !afterlife)
                        {
                            larator++;
                            continue;
                        }
                        const size_t initSinum = getCurrentSinum(idx);
                        // In next loop we are finding first redefinition of register and 
                        // first usage in each branch (firstUseMain, firstDefMain, firstUseElse, firstDefElse)
                        // Also sinum will be number of last subinterval intersected with embranchment.
                        size_t sinum = initSinum;
                        for (; sinum < size(idx); ++sinum)
                        {
                            const size_t sistart = m_subintervals[idx][sinum].start;
                            const size_t siend = m_subintervals[idx][sinum].end;
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
                        if (sinum == size(idx))
                            sinum = size(idx) - 1;

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
                            const size_t switchIpos = larator->end;
                            spliceUntilSinum(idx, sinum);
                            LiveInterval changedOne = m_subintervals[idx][initSinum];
                            changedOne.end = deactivationOpnum(idx);
                            larator = lastActive.erase(larator);
                            lastActiveChanged.insert(changedOne);
                            moveEventLater(branchQueue, idx, LAEvent::LAE_SWITCHSUBINT, switchIpos, changedOne.end);
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
                for (size_t si = 1; si < size(idx); si++)
                    m_subintervals[idx][si].idx = pseudIdx++;
            for (IRegInternal idx = a_processed.params.size(); idx < size(); idx++)
                for (LiveInterval& li : m_subintervals[idx])
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
                        Assert(size(arg.idx) > 0);
                        size_t linum = 0;
                        if (size(arg.idx) > 1)
                            for (; linum < size(arg.idx); linum++)
                            {
                                LiveInterval& li = m_subintervals[arg.idx][linum];
                                if (opnum >= li.start && opnum <= li.end &&
                                    (!isOut || ((linum + 1) >= size(arg.idx)) || m_subintervals[arg.idx][linum + 1].start > opnum))
                                    break;
                            }
                        Assert(linum < size(arg.idx));
                        arg.idx = m_subintervals[arg.idx][linum].idx;
                    }
                }
            }
        }

        std::set<LiveInterval, idxordering> result;
        for (auto res : m_subintervals)
            for (auto pseud : res)
                result.insert(pseud);

        return result;
    }

    LivenessAnalysisAlgo::LivenessAnalysisAlgo(size_t virtualRegsAmount, ContextImpl* a_owner) :
        m_subintervals(virtualRegsAmount, std::vector<LiveInterval>())
        , m_owner(a_owner)
        , m_virtualRegsAmount(virtualRegsAmount)
    {}

    size_t LivenessAnalysisAlgo::size(size_t regNum) const
    {
        Assert(regNum < size());
        return m_subintervals[regNum].size();
    };

    void LivenessAnalysisAlgo::def(size_t regNum, size_t opnum)
    {
        if (regNum != Syntfunc::RETREG)
        {
            Assert(regNum < size());
            m_subintervals[regNum].push_back(LiveInterval(regNum, opnum));
        }
    }

    void LivenessAnalysisAlgo::use(size_t regNum, size_t opnum)
    {
        if (regNum != Syntfunc::RETREG) //TODO(ch): At some day we will need to work with different types of return.
        {
            if (!defined(regNum)) //TODO(ch): Isn't it too strict?
                throw std::string("Compile error: using uninitialized register");
            m_subintervals[regNum].back().end = opnum;
        }
    }

    void LivenessAnalysisAlgo::spliceUntilSinum(size_t regNum, size_t siEnd)
    {
        size_t siStart = m_subintervalHeaders[regNum];
        Assert(siStart <= siEnd);
        Assert(siEnd < size(regNum));
        m_subintervals[regNum][siStart].end = m_subintervals[regNum][siEnd].end;
        m_subintervals[regNum].erase(m_subintervals[regNum].begin() + siStart + 1, m_subintervals[regNum].begin() + siEnd + 1);
    }

    size_t LivenessAnalysisAlgo::expandUntilOpnum(size_t regNum, size_t opnum)
    {
        size_t sinum = m_subintervalHeaders[regNum];
        size_t subinterval2erase = sinum + 1;
        for (; subinterval2erase < size(regNum); subinterval2erase++)
            if (m_subintervals[regNum][subinterval2erase].start >= opnum)
            {
                subinterval2erase--;
                break;
            }
            else
                opnum = std::max(m_subintervals[regNum][subinterval2erase].end, opnum);
        subinterval2erase = std::min(subinterval2erase, size(regNum) - 1);
        spliceUntilSinum(regNum, subinterval2erase);
        m_subintervals[regNum][sinum].end = opnum;
        return opnum;
    }

    size_t LivenessAnalysisAlgo::deactivationOpnum(size_t regNum)
    {
        size_t sinum = m_subintervalHeaders[regNum];
        return (sinum + 1 < size(regNum)) ? m_subintervals[regNum][sinum + 1].start : m_subintervals[regNum][sinum].end;
    }

    void LivenessAnalysisAlgo::initSubintervalHeaders(size_t initval)
    {
        m_subintervalHeaders.clear();
        m_subintervalHeaders.resize(m_subintervals.size(), initval);
    }

    size_t LivenessAnalysisAlgo::getCurrentSinum(size_t regNum)
    {
        return m_subintervalHeaders[regNum];
    }

    LiveInterval& LivenessAnalysisAlgo::getCurrentSubinterval(size_t regNum)
    {
        Assert(m_subintervalHeaders[regNum] < size(regNum));
        return m_subintervals[regNum][m_subintervalHeaders[regNum]];
    }

    LiveInterval& LivenessAnalysisAlgo::getNextSubinterval(size_t regNum)
    {
        Assert(m_subintervalHeaders[regNum] + 1 < size(regNum));
        return m_subintervals[regNum][m_subintervalHeaders[regNum] + 1];
    }

    bool LivenessAnalysisAlgo::isIterateable(size_t regNum) const //Well, unfotunately, we don't have after-end-state, only last-one state. 
    {
        return (m_subintervalHeaders[regNum] + 1) < size(regNum);
    }

    void LivenessAnalysisAlgo::iterateSubinterval(size_t regNum)
    {
        if (isIterateable(regNum))
            m_subintervalHeaders[regNum]++;
    }

    void LivenessAnalysisAlgo::moveEventLater(std::multimap<size_t, LAEvent>& queue, size_t regNum, int eventType, size_t oldOpnum, size_t newOpnum)
    {
        auto qremrator = queue.find(oldOpnum);
        while (qremrator != queue.end() && qremrator->first == oldOpnum)
            if (qremrator->second.eventType == eventType && qremrator->second.idx == regNum)
                break;
        Assert(qremrator != queue.end() && qremrator->first == oldOpnum);
        LAEvent toRead = qremrator->second;
        queue.erase(qremrator);
        queue.insert(std::make_pair(newOpnum, toRead));
    }
};