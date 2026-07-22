/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include <algorithm>
#include <array>
#include <list>
#include <cstring>
#include <unordered_map>
#include "liveness_analysis.hpp"
#include "reg_allocator.hpp"
#include "common.hpp"
#include "func_impl.hpp"

//DUBUG list:
//1.) I know certainly, that loops cannot call functions with more arguments, than calling convention grants to copy in registers. 
/*
Register allocator fits unlimited amount of virtual register to fixed set of registers of
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
    the stack). All appointed and pilled registers will be stored in reg_reassignment vector.
2.) Matching target-mchine and virtual registers.
    Let's consider consequent LiveInterval from liveintervals. First, all expired active
    intervals(which end will be lesser than start of current interval) must be droped. If
    interval is dropped, it will return used register.

    Next is attempt to appoint the real register to current virtual register(interval). If there
    is free registers, it's just added to reg_reassignment and active. Otherwise there needed
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

void DUBUGprint_allocation(const std::array<std::vector<RegisterAllocator::RegisterReassignment>, RB_AMOUNT>& m_reg_reassignment)
{
    for(int basket_num = 0; basket_num < (int)m_reg_reassignment.size(); basket_num++)
    {
        if (basket_num == RB_INT)
            printf("Scalar registers reassignment:\n");
        else 
            printf("Vector registers reassignment:\n");
        for(int idx = 0; idx < (int)m_reg_reassignment[basket_num].size(); idx++)
        {
            const RegisterAllocator::RegisterReassignment& ra = m_reg_reassignment[basket_num][idx];
            printf("    %d ->", idx);
            for(int sinum = 0; sinum < (int)ra.args.size(); sinum++)
            {
                bool spilled = ra.args[sinum].tag == Arg::ISPILLED || ra.args[sinum].tag == Arg::VSPILLED;
                printf("(%d-%d:", ra.bounds[sinum], ra.bounds[sinum + 1]);
                printf("%s%d)|", spilled ? "s" : "r", (int)(spilled ? ra.args[sinum].value : ra.args[sinum].idx));
            }
            printf("\n");
        }
            
    } 
}

inline RegIdx pickFirstBit64(uint64_t& bigNum)
{
    LOOPS_ASSERT(bigNum != 0);
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
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
    {
        std::vector<int> origRegistersV[VESS_AMOUNT];

        origRegistersV[PARAMS_VESS] = m_backend->parameterRegisters(basket_num),
        origRegistersV[RETURN_VESS] = m_backend->returnRegisters(basket_num),
        origRegistersV[CALLER_VESS] = m_backend->callerSavedRegisters(basket_num),
        origRegistersV[CALLEE_VESS] = m_backend->calleeSavedRegisters(basket_num);
    
        if (m_registersO[basket_num][PARAMS_VESS].size() != 0 || m_registersO[basket_num][RETURN_VESS].size() != 0 ||
            m_registersO[basket_num][CALLER_VESS].size() != 0 || m_registersO[basket_num][CALLEE_VESS].size() != 0)
            for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
                origRegistersV[vessNum] = m_registersO[basket_num][vessNum];
        
        m_pool[basket_num] = 0;
        for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
        {
            uint8_t regAmount = static_cast<uint8_t>(origRegistersV[vessNum].size());
            m_vessel[basket_num][vessNum] = (((uint64_t)(1)) << regAmount) - 1;
            for(uint8_t inRegNum = 0; inRegNum < regAmount; inRegNum++)
            {
                const uint8_t argregNum = (uint8_t)origRegistersV[vessNum][inRegNum];
                m_reorderInner2Arch[basket_num][vessNum][inRegNum] = argregNum;
                m_reorderArch2Inner[basket_num][vessNum][argregNum] = inRegNum;
                m_pool[basket_num] |= (((uint64_t)(1)) << argregNum);
            }
        }
        m_spillPlaceholdersAvailable[basket_num] = m_pool[basket_num];
        m_usedCallee[basket_num].clear();
    }
}

size_t RegisterPool::freeRegsAmount(int basket_num) const
{
    return amountOfBits64(m_pool[basket_num]) - MAXIMUM_SPILLS; //TODO(ch): MAXIMUM_SPILLS must become basket-dependend variable.
}

RegIdx RegisterPool::provideParamFromPool(int basket_num)
{
    if(m_vessel[basket_num][PARAMS_VESS] == 0)
        return IReg::NOIDX;
    RegIdx res = lsb64(m_vessel[basket_num][PARAMS_VESS]);
    res = m_reorderInner2Arch[basket_num][PARAMS_VESS][res];
    removeFromAllVessels(basket_num, res);
    return res;
}

RegIdx RegisterPool::provideRegFromPool(int basket_num, RegIdx a_hint)
{
    RegIdx res = NOREGISTER;
    if (a_hint != IReg::NOIDX)
    {
        for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
        {
            uint8_t inRegNum = m_reorderArch2Inner[basket_num][vessNum][a_hint];
            if(inRegNum == REG_UNDEF)
                continue;
            if(m_vessel[basket_num][vessNum] & (((uint64_t)(1)) << inRegNum))
            {
                res = a_hint;
                break;
            }
        }
    }
    if (res == NOREGISTER && havefreeRegs(basket_num))
    {
        for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
            if(m_vessel[basket_num][vessNum])
            {
                res = lsb64(m_vessel[basket_num][vessNum]);
                res = m_reorderInner2Arch[basket_num][vessNum][res];
                break;
            }
        LOOPS_ASSERT(res != NOREGISTER);
    }
    if (res != NOREGISTER && m_reorderArch2Inner[basket_num][CALLEE_VESS][res] != REG_UNDEF)
        m_usedCallee[basket_num].insert(res);
    removeFromAllVessels(basket_num, res);
    res = (res == NOREGISTER) ? IReg::NOIDX : res;
    return res;
}

//TODO(ch): this function is just a workaround for ld2/ld3/ld4 instructions
std::vector<RegIdx> RegisterPool::provideConsecutiveRegs(int basket_num, int amount)
{
    int amount_temp = amount; 
    std::vector<RegIdx> res;
    std::vector<RegIdx> fragmented;
    res.reserve(amount);
    fragmented.reserve(32);
    while(amount_temp > 0)
    {
        RegIdx next = provideRegFromPool(basket_num);
        if(next == IReg::NOIDX)
            throw loops::exception("Register allocator: register space is too fragmented for ld2/ld3/ld4 workaround.");
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
        releaseReg(basket_num, t_rel);
    return res;
}

RegIdx RegisterPool::provideReturnFromPool(int basket_num)
{
    LOOPS_ASSERT(m_reorderInner2Arch[basket_num][RETURN_VESS][0] != REG_UNDEF);
    RegIdx res = m_reorderInner2Arch[basket_num][RETURN_VESS][0];
    removeFromAllVessels(basket_num, res);
    return res;
}

void RegisterPool::releaseReg(int basket_num, RegIdx freeReg)
{
    LOOPS_ASSERT(freeReg != IReg::NOIDX);
    m_pool[basket_num] |= (((uint64_t)(1)) << freeReg);
    for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
    {
        uint8_t inRegNum = m_reorderArch2Inner[basket_num][vessNum][freeReg];
        if(inRegNum != REG_UNDEF)
            m_vessel[basket_num][vessNum] |= (((uint64_t)(1)) << inRegNum);
    }
}

RegIdx RegisterPool::provideSpillPlaceholder(int basket_num)
{
    if (m_spillPlaceholders[basket_num] == 0)
        return IReg::NOIDX;
    int res = lsb64(m_spillPlaceholders[basket_num]);
    m_spillPlaceholders[basket_num] &= ~(((uint64_t)1) << res);
    if(m_reorderArch2Inner[basket_num][CALLEE_VESS][res] != REG_UNDEF)
        m_usedCallee[basket_num].insert(res);
    return res;
}

void RegisterPool::clearSpillPlaceholders(int basket_num)
{
    m_spillPlaceholders[basket_num] = m_spillPlaceholdersAvailable[basket_num];
}

inline void RegisterPool::mergeUsedCallee(const RegisterPool& other)
{
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
        m_usedCallee[basket_num].insert(other.m_usedCallee[basket_num].begin(), other.m_usedCallee[basket_num].end());
}

inline void RegisterPool::mergeSpillPlaceholders(const RegisterPool& other)
{
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
        m_spillPlaceholdersAvailable[basket_num] &= other.m_spillPlaceholdersAvailable[basket_num];
}

void RegisterPool::overrideRegisterSet(int basket_num, const std::vector<int>&  a_parameterRegisters,
                                                        const std::vector<int>&  a_returnRegisters,
                                                        const std::vector<int>&  a_callerSavedRegisters,
                                                        const std::vector<int>&  a_calleeSavedRegisters)
{
    m_registersO[basket_num][PARAMS_VESS] = a_parameterRegisters;
    m_registersO[basket_num][RETURN_VESS] = a_returnRegisters;
    m_registersO[basket_num][CALLER_VESS] = a_callerSavedRegisters;
    m_registersO[basket_num][CALLEE_VESS] = a_calleeSavedRegisters;
}

std::array<std::vector<int>, RB_AMOUNT> RegisterPool::getOverridenParams() const
{
    std::array<std::vector<int>, RB_AMOUNT> res;
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
        res[basket_num] = m_registersO[basket_num][PARAMS_VESS]; 
    return res;
}

void RegisterPool::removeFromAllVessels(int basket_num, int reg)
{
    if (reg == NOREGISTER)
        return;
    m_pool[basket_num] |= (((uint64_t)1) << reg);
    m_pool[basket_num] ^= (((uint64_t)1) << reg);
    m_spillPlaceholdersAvailable[basket_num] &= ~(((uint64_t)1) << reg);
    for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
    {
        uint8_t innerReg = m_reorderArch2Inner[basket_num][vessNum][reg];
        if (innerReg == REG_UNDEF) //reg is absent from this vessel; shifting by REG_UNDEF(255) would be UB.
            continue;
        m_vessel[basket_num][vessNum] |= (((uint64_t)1) << innerReg);
        m_vessel[basket_num][vessNum] ^= (((uint64_t)1) << innerReg);
    }
}

RegisterAllocator::RegisterAllocator(Backend* a_backend, const std::array<std::vector<LiveInterval>, RB_AMOUNT>* a_live_intervals, BasicBlocksTreePtr a_bbt, int a_snippet_caused_spills, bool a_have_function_calls) : CompilerPass(a_backend)
    , m_liveintervals_raw(a_live_intervals)
    , m_bbt(a_bbt)
    , m_pool(a_backend)
    , m_poolBase(a_backend)
    , m_snippet_caused_spills(a_snippet_caused_spills)
    , m_have_function_calls(a_have_function_calls)
    , m_epilogueSize(0)
{
    m_basketElemX[RB_INT] = 1;
    m_basketElemX[RB_VEC] = m_backend->getVectorRegisterBits() / 64;
}

void RegisterAllocator::layOutLiveIntervals(const Syntfunc& a_source, 
                            std::array<std::vector<LiveInterval>, RB_AMOUNT>& parintervals,
                            std::array<std::multiset<LiveInterval, startordering>, RB_AMOUNT>& liveintervals,
                            std::array<std::vector<RegIdx>, RB_AMOUNT>& params_sorted)
{
    //After liveness analysis we are sorting given liveintervals into two heaps:
    //parameters(parintervals), which are immediately marked as active and other intervals(liveintervals),
    //which are reordered by start positions to work with Linear scan algorithm.
    //Also, this function sorts parameters into two baskets(params_sorted): scalars and vectors.
    
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
    {
        params_sorted[basket_num].clear();
        params_sorted[basket_num].reserve(a_source.params.size());
    }
    for (const Arg& par : a_source.params)
    {
        LOOPS_ASSERT(par.tag == Arg::IREG || par.tag == Arg::VREG);
        int basket_num = (par.tag == Arg::IREG ? RB_INT : RB_VEC);
        params_sorted[basket_num].push_back(par.idx);
    }
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
    {
        liveintervals[basket_num].clear();
        parintervals[basket_num].clear();
        size_t idx = 0;
        //Function can have more arguments, than it use, so:  
        size_t idxParMax = std::min((*m_liveintervals_raw)[basket_num].size(), params_sorted[basket_num].size());
        parintervals[basket_num].reserve(idxParMax);
        for (; idx < idxParMax; ++idx)
            parintervals[basket_num].push_back((*m_liveintervals_raw)[basket_num][idx]);
        idxParMax = (*m_liveintervals_raw)[basket_num].size();
        for (; idx < idxParMax; ++idx)
            liveintervals[basket_num].insert((*m_liveintervals_raw)[basket_num][idx]);
    }
}

inline Arg RegisterAllocator::RegisterReassignment::getAt(int opnum)
{
    int bnum = (int)std::distance(bounds.begin(), std::lower_bound(bounds.begin(), bounds.end(), opnum));
    bnum = bounds[bnum] == opnum ? bnum : bnum - 1;
    while(bnum < ((int)args.size()) - 1 && bounds[bnum] == bounds[bnum + 1])
        bnum++;
    return args[bnum];
}

void RegisterAllocator::removeBranchesFromBBT(BasicBlocksTree& node)
{
    std::vector<std::shared_ptr<BasicBlocksTree> > newChildren;
    newChildren.reserve(node.children.size());
    for(std::shared_ptr<BasicBlocksTree>& child : node.children)
    {
        removeBranchesFromBBT(*child);
        if(child->type == BasicBlocksTree::BBT_IF)
            for(std::shared_ptr<BasicBlocksTree>& grandchild : child->children)
                newChildren.push_back(grandchild);
        else
            newChildren.push_back(child);
    }
    node.children = std::move(newChildren);
}

static void collectLoops(const BasicBlocksTree& node, std::vector<const BasicBlocksTree*>& out)
{
    for(const BasicBlocksTreePtr& child : node.children)
    {
        if(child->type == BasicBlocksTree::BBT_WHILE)
            out.push_back(child.get());
        collectLoops(*child, out);
    }
}

std::array<std::vector<std::vector<LiveInterval> >, RB_AMOUNT> RegisterAllocator::makeBlockSplits(const std::array<std::multiset<LiveInterval, startordering>, RB_AMOUNT>& liveintervals)
{
    //Boundaries have to cut live intervals, sorted ascending and deduplicated.
    std::vector<int> loops_boundaries;
    {
        std::vector<const BasicBlocksTree*> loops;
        collectLoops(*m_bbt, loops);
        loops_boundaries.reserve(2 * loops.size());
        for(const BasicBlocksTree* m : loops)
        {
            loops_boundaries.push_back(m->start_pos);
            loops_boundaries.push_back(m->end_pos + 1);
        }
        std::sort(loops_boundaries.begin(), loops_boundaries.end());
        loops_boundaries.erase(std::unique(loops_boundaries.begin(), loops_boundaries.end()), loops_boundaries.end());
    }    
    std::array<std::vector<std::vector<LiveInterval> >, RB_AMOUNT> res; 
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
    {
        const std::multiset<LiveInterval, startordering>& baskets_intervals = liveintervals[basket_num];
        std::vector<std::vector<LiveInterval> >& block_splits = res[basket_num];
        block_splits.resize((*m_liveintervals_raw)[basket_num].size());
        //TODO(ch): Probably, we can speed up by using ordering of liveintervals, but I don't really think it worth it.
        for(const LiveInterval& li:baskets_intervals)
        {
            int idx = li.idx;
            std::vector<int>::iterator cutsBegin = std::upper_bound(loops_boundaries.begin(), loops_boundaries.end(), li.start);
            std::vector<int>::iterator cutsEnd = std::upper_bound(cutsBegin, loops_boundaries.end(), li.end);
            block_splits[idx].reserve((size_t)(cutsEnd - cutsBegin) + 1);
            int prev = li.start;
            for(std::vector<int>::iterator cut = cutsBegin; cut != cutsEnd; ++cut)
            {
                const int b = *cut;
                //Non-final piece: its reassignment covers [prev, b), but it keeps its register through the boundary
                //op `b`(end = b, !is_last_split) so the boundary copy at `b` can read it and a value defined at b-1
                //cannot steal it early.
                LiveInterval split(li.idx, prev);
                split.end = b;
                split.is_last_split = false;
                split.priority = li.priority;
                block_splits[idx].push_back(split);
                prev = b;
            }
            LiveInterval last(li.idx, prev);
            last.end = li.end;
            last.priority = li.priority;
            block_splits[idx].push_back(last);
        }
    }
    return res;
}

std::array<std::vector<RegisterAllocator::RegisterReassignment>, RB_AMOUNT> RegisterAllocator::assignRegisters(const Syntfunc& a_source,
    const std::array<std::multiset<LiveInterval, startordering>, RB_AMOUNT>& liveintervals,
    const std::array<std::vector<LiveInterval>, RB_AMOUNT>& parintervals)
{
    //Function takes live intervals, program and create a mapping from abstract old register
    //indexes to new real machine registers. If it's not enough machine registers, some of
    //them are spilled, so mapping for this register will point to spill position in stack.

    //Remove IF statements, because they doesn't affect performance on register allocation.
    removeBranchesFromBBT(*m_bbt);

    //Positions(loop entries/exits, while-only after branch removal) at which registers crossing a loop are cut.
    std::array<std::vector<std::vector<LiveInterval> >, RB_AMOUNT> block_splits = RegisterAllocator::makeBlockSplits(liveintervals);

    //TODO(ch):This ugly workaround must be eliminated after introducing register allocation with restrictions.
    std::array<std::unordered_map<RegIdx, std::pair<RegIdx, RegIdx> >, RB_AMOUNT> unspillableLd2;
    std::array<std::unordered_map<RegIdx, RegIdx>, RB_AMOUNT> already_allocatedLd2;
    for(const Syntop& op: a_source.program)
        if(op.opcode == VOP_ARM_LD2)
        {
            std::pair<RegIdx, RegIdx> order = std::make_pair(op[0].idx, op[1].idx); 
            unspillableLd2[RB_VEC].insert(std::make_pair(op[0].idx, order));
            unspillableLd2[RB_VEC].insert(std::make_pair(op[1].idx, order));
        }

    //Space in stack used by snippets will be located in the bottom,
    //so spilled variables will be located higher.
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
        m_spill_slot_of[basket_num].clear();

    std::array<std::vector<RegisterReassignment>, RB_AMOUNT> result;
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
    {
        std::multiset<LiveInterval, endordering> active;
        result[basket_num].resize(a_source.regAmount[basket_num]);
        m_split_assignments[basket_num].assign(a_source.regAmount[basket_num], {});
        {//Get pseudonames for parameters.
            RegIdx parreg = 0;
            for (; parreg < (int)parintervals[basket_num].size(); parreg++)
            {
                const LiveInterval& interval = parintervals[basket_num][parreg];
                RegIdx idx = interval.idx;
                RegIdx attempt = m_pool.provideParamFromPool(basket_num);
                if (attempt == IReg::NOIDX)
                    break;
                result[basket_num][idx] = RegisterReassignment(interval.start, interval.end + 1, argReg(basket_num, attempt));
                if(isStackPassedParam(basket_num, idx))
                {
                    RegisterReassignment& param_r = result[basket_num][idx];
                    param_r.bounds.insert(param_r.bounds.begin(), 0);   //DUBUG: there needed some kind of function, which append subinterval.
                    param_r.args.insert(param_r.args.begin(), argSpilled(basket_num, 0));
                } 
                active.insert(parintervals[basket_num][parreg]);
            }
            for (; parreg < (int)parintervals[basket_num].size(); parreg++)
            {
                const LiveInterval& interval = parintervals[basket_num][parreg];
                RegIdx idx = interval.idx;
                RegIdx attempt = m_pool.provideRegFromPool(basket_num);
                if (attempt == IReg::NOIDX)
                    break;
                result[basket_num][idx] = RegisterReassignment(interval.start, interval.end + 1, argReg(basket_num, attempt));
                if(isStackPassedParam(basket_num, idx))
                {
                    RegisterReassignment& param_r = result[basket_num][idx];
                    param_r.bounds.insert(param_r.bounds.begin(), 0);   //DUBUG: there needed some kind of function, which append subinterval.
                    param_r.args.insert(param_r.args.begin(), argSpilled(basket_num, 0));
                }
                active.insert(parintervals[basket_num][parreg]);
            }
            //DUBUG: At least here we can use space provided by calling convention. But, probably, in previous case, 
            //when we have enough registers, but variable is already allocated in stack we can do it too.
            for (; parreg < (int)parintervals[basket_num].size(); parreg++)
            {
                const LiveInterval& interval = parintervals[basket_num][parreg];
                RegIdx idx = interval.idx;
                result[basket_num][idx] = RegisterReassignment(interval.start, interval.end + 1, argSpilled(basket_num, 0));
            }
        }
        m_poolBase = m_pool; //post-seeding snapshot: parameters(and callee) reserved; base for fresh per-block pools.
        allocateBlock(*m_bbt, basket_num, a_source, block_splits[basket_num], active, result[basket_num], unspillableLd2[basket_num], already_allocatedLd2[basket_num]);

        for(RegIdx idx = 0; idx < (RegIdx)result[basket_num].size(); idx++)
        {
            std::vector<SplitAssignment>& sas = m_split_assignments[basket_num][idx];
            if(sas.empty()) //parameter(kept in result) or unused register.
                continue;
            std::sort(sas.begin(), sas.end(),
                      [](const SplitAssignment& a, const SplitAssignment& b){ return a.start_pos < b.start_pos; });
            RegisterReassignment rr(sas[0].start_pos, sas[0].end_pos, sas[0].assignment);
            for(size_t p = 1; p < sas.size(); p++)
            {
                LOOPS_ASSERT(rr.bounds.back() == sas[p].start_pos); //contiguous tiling, no gaps/overlaps.
                rr.bounds.push_back(sas[p].end_pos);
                rr.args.push_back(sas[p].assignment);
            }
            result[basket_num][idx] = rr;
        }
    }
    return result;
}

void RegisterAllocator::allocateBlock(const BasicBlocksTree& node, int basket_num, const Syntfunc& a_source,
    const std::vector<std::vector<LiveInterval> >& block_splits,
    std::multiset<LiveInterval, endordering>& active,
    std::vector<RegisterReassignment>& result,
    const std::unordered_map<RegIdx, std::pair<RegIdx, RegIdx> >& unspillableLd2,
    std::unordered_map<RegIdx, RegIdx>& already_allocatedLd2)
{
    //1. Allocate child loops first(innermost-first). Each child scans on a fresh pool(copy of m_poolBase, so
    //parameters/callee stay reserved) and a fresh active set: it is an independent allocation.
    for(const BasicBlocksTreePtr& child : node.children)
    {
        RegisterPool savedPool = m_pool;
        m_pool = m_poolBase;
        std::multiset<LiveInterval, endordering> childActive;
        allocateBlock(*child, basket_num, a_source, block_splits, childActive, result, unspillableLd2, already_allocatedLd2);
        savedPool.mergeUsedCallee(m_pool);
        savedPool.mergeSpillPlaceholders(m_pool);
        m_pool = savedPool;
    }
    //2. Claim this block's OWN splits out of the(block-independent) decomposition: those lying in [nlo, nhi) but
    //not inside any direct child(a child owns its interior). Splitting runs at every loop boundary, so every
    //split falls wholly inside or wholly outside each child; ownership is the deepest block containing it.
    const bool isRoot = (node.type == BasicBlocksTree::BBT_FUNC);
    const int nlo = isRoot ? 0 : node.start_pos;
    const int nhi = isRoot ? (int)node.end_pos : node.end_pos + 1;
    std::multiset<LiveInterval, startordering> blockIntervals;
    for(RegIdx idx = 0; idx < (int)block_splits.size(); idx++)
    {
        if(isParam(basket_num, idx))
            continue;
        for(const LiveInterval& piece : block_splits[idx])
        {
            const int lo = piece.start;
            const int hi = piece.is_last_split ? piece.end + 1: piece.end;
            if(!(lo >= nlo && hi <= nhi)) //not in this block's extent
                continue;
            bool inChild = false;
            for(const BasicBlocksTreePtr& child : node.children)
                if(lo >= child->start_pos && hi <= child->end_pos + 1) //+1 is because loop boundary is defined like +1.
                {
                    inChild = true;
                    break;
                }
            if(inChild) //owned by a child(already allocated above)
                continue;
            blockIntervals.insert(piece);
        }
    }
    linearScanBlock(basket_num, a_source, blockIntervals, active, result, unspillableLd2, already_allocatedLd2);
}

void RegisterAllocator::linearScanBlock(int basket_num, const Syntfunc& a_source,
    const std::multiset<LiveInterval, startordering>& liveintervals,
    std::multiset<LiveInterval, endordering>& active,
    std::vector<RegisterReassignment>& result,
    const std::unordered_map<RegIdx, std::pair<RegIdx, RegIdx> >& unspillableLd2,
    std::unordered_map<RegIdx, RegIdx>& already_allocatedLd2)
{
    const int REGtag = ((basket_num == RB_INT) ? Arg::IREG : Arg::VREG);
    const int SPLtag = ((basket_num == RB_INT) ? Arg::ISPILLED : Arg::VSPILLED);
    std::vector<std::vector<SplitAssignment> >& split_assignments = m_split_assignments[basket_num];
    for (auto interval = liveintervals.begin(); interval != liveintervals.end(); ++interval)
    {
        std::unordered_map<RegIdx, RegIdx> opUndefs; //TODO(ch): You also have to consider spilled undefs.
        //Upper bound of this split's reassignment subinterval. A non-final split(!is_last_split) keeps its
        //register live through the boundary op `end`(see expire logic), but its subinterval is only [start, end).
        const int hi = interval->is_last_split ? interval->end + 1: interval->end;
        { //Dropping expired registers.
            auto removerator = active.begin();
            for (; removerator != active.end(); ++removerator)
                if (removerator->end <= interval->start)
                {
                    Arg curLoc = isParam(basket_num, removerator->idx) ? result[removerator->idx].args.back(): split_assignments[removerator->idx].back().assignment;
                    LOOPS_ASSERT(curLoc.tag == REGtag);
                    int assigned_idx = curLoc.idx;
                    m_pool.releaseReg(basket_num, assigned_idx);
                    if (removerator->end == interval->start) //Current line, line of definition of considered register
                        opUndefs.insert(std::pair<RegIdx,RegIdx>(removerator->idx, assigned_idx));
                }
                else
                    break;
            active.erase(active.begin(), removerator);
        }
        //If this virtual register was spilled once, we can use it's slot for every spilled split.
        {
            bool adjacentSpilled = false;
            for(const SplitAssignment& sa : split_assignments[interval->idx])
            {
                const Arg& a = sa.assignment;
                if(a.tag == SPLtag && (sa.end_pos == interval->start || sa.start_pos == hi))
                {
                    adjacentSpilled = true;
                    break;
                }
            }
            if(adjacentSpilled)
            {
                const Arg sp = argSpilled(basket_num, isStackPassedParam(basket_num, interval->idx) ? 0 : getSpillSlot(interval->idx, basket_num));
                split_assignments[interval->idx].push_back(SplitAssignment{interval->start, hi, sp});
                continue;
            }
        }
        if (!m_pool.havefreeRegs(basket_num))
        {
            if(unspillableLd2.find(interval->idx) != unspillableLd2.end())
                throw loops::exception("Register allocator: not enough free registers for ld2 workaround.");
            bool stackParameterSpilled = false;
            std::multiset<LiveInterval, endordering>::reverse_iterator lastactive = active.rbegin();
            while(lastactive!=active.rend() && unspillableLd2.find(lastactive->idx) != unspillableLd2.end())
                lastactive++;
            if (lastactive != active.rend() && lastactive->end > interval->end)
            {
                //Victim is a register considered to be moved to stack.
                const RegIdx vict_idx = lastactive->idx;
                Arg curLoc = isParam(basket_num, vict_idx) ? result[vict_idx].args.back(): split_assignments[vict_idx].back().assignment;
                LOOPS_ASSERT(curLoc.tag == REGtag);
                const RegIdx stolenReg = curLoc.idx; //Victim's active register, read before changes.
                stackParameterSpilled = isStackPassedParam(basket_num, vict_idx);
                const Arg victSpilled = argSpilled(basket_num, stackParameterSpilled ? 0 : getSpillSlot(vict_idx, basket_num));
                if(isParam(basket_num, vict_idx))
                {
                    RegisterReassignment keeped = result[vict_idx];
                    int lastsn = ((int)keeped.args.size()) - 1;
                    result[vict_idx] = RegisterReassignment(keeped.bounds[lastsn], keeped.bounds[lastsn + 1], victSpilled);
                    if(isRegisterPassedParam(basket_num, vict_idx))
                    {
                        RegisterReassignment& param = result[vict_idx];
                        param.bounds.insert(param.bounds.begin(), 0);   //DUBUG: there needed some kind of function, which append subinterval.
                        param.args.insert(param.args.begin(), keeped.args[0]);
                    }
                }
                else
                {
                    LOOPS_ASSERT(!split_assignments[vict_idx].empty());
                    split_assignments[vict_idx].back().assignment = victSpilled;
                }
                split_assignments[interval->idx].push_back(SplitAssignment{interval->start, hi, argReg(basket_num, stolenReg)});
                active.erase(--(active.end()));
                active.insert(*interval);
            }
            else
            {
                stackParameterSpilled = isStackPassedParam(basket_num, interval->idx);
                const Arg sp = argSpilled(basket_num, stackParameterSpilled ? 0 : getSpillSlot(interval->idx, basket_num));
                split_assignments[interval->idx].push_back(SplitAssignment{interval->start, hi, sp});
            }
        }
        else
        {
            RegIdx hwReg;
            active.insert(*interval);
            auto unsprator = unspillableLd2.find(interval->idx);
            if(basket_num == RB_VEC && ( unsprator != unspillableLd2.end()))
            {
                auto alrator = already_allocatedLd2.find(interval->idx);
                if(alrator != already_allocatedLd2.end())
                {
                    hwReg = alrator->second;
                    already_allocatedLd2.erase(interval->idx);
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
                    already_allocatedLd2.insert(std::make_pair(oppositeSrc, oppositeDst));
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
                    std::set<int> iNs = m_backend->getInRegistersIdxs(op, basket_num);
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
                //We are trying to keep same assignment for heighbouring splits.
                if(poolHint == IReg::NOIDX)
                {
                    const std::vector<SplitAssignment >& own = split_assignments[interval->idx];
                    RegIdx anyHint = IReg::NOIDX;
                    for(const SplitAssignment& sa : own)
                    {
                        if(sa.assignment.tag != REGtag)
                            continue;
                        if(sa.end_pos == interval->start || sa.start_pos == hi) //immediately adjacent
                        {
                            poolHint = sa.assignment.idx;
                            break;
                        }
                        anyHint = sa.assignment.idx;
                    }
                    if(poolHint == IReg::NOIDX)
                        poolHint = anyHint;
                }
                hwReg = m_pool.provideRegFromPool(basket_num, poolHint);
            }
            split_assignments[interval->idx].push_back(SplitAssignment{interval->start, hi, argReg(basket_num, hwReg)});
        }
    }
}

RegisterAllocator::SpillInfo RegisterAllocator::modelSpills(const Syntfunc& a_source)
{
    SpillInfo to_fill;
    // TODO(ch):
    // 1.) Let's consider sequence of instructions, where it's used one register. Obviously, it can be unspilled only once at start of sequence and
    // spilled only once at end. But for now it will spill/unspill at each instruction. I think, this unefficiency can be easily avoided by using some
    // variable-spill map.
    // 2.) Also, we have to take into account live intervals of spilled variables. At some moment place in memory for one variable can be used for
    // another variable.
    // 3.) By the way, when we are using only least spill placeholder, instead of using as much of them, as possible - it's bad practice. 
    // minimizing prologue/epilogue overhead isn't so important.

    //There are two types of stack scratch tasks: for register transfers(needs 1 register per basket) and
    //snippets scratch(depends on instructions, used in program). They are located in one space.
    int largest_register = 1;
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
        largest_register = std::max(largest_register, m_basketElemX[basket_num]);
    const int first_non_scratch_stack_position = std::max(m_snippet_caused_spills, largest_register);
    to_fill.spAddAligned = first_non_scratch_stack_position;

    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
    {
        to_fill.nettoSpills[basket_num] = 0;
        to_fill.unspilledRenaming[basket_num].clear();
        to_fill.unspilledRenaming[basket_num].resize(a_source.program.size());
        to_fill.spilledRenaming[basket_num].clear();
        to_fill.spilledRenaming[basket_num].resize(a_source.program.size());
        to_fill.stackPlaceable[basket_num].clear();
        to_fill.stackPlaceable[basket_num].resize(a_source.program.size());
        for (size_t opnum = 0; opnum < a_source.program.size(); ++opnum)
        {
            const Syntop& op = a_source.program[opnum];
            std::set<int> unspilledIdxs;
            std::set<int> spilledIdxs;
            const int REGtag = ((basket_num == RB_INT) ? Arg::IREG : Arg::VREG);
            const int SPLtag = ((basket_num == RB_INT) ? Arg::ISPILLED : Arg::VSPILLED);
            unspilledIdxs = m_backend->getInRegistersIdxs(op, basket_num);
            spilledIdxs = m_backend->getOutRegistersIdxs(op, basket_num);
            for (std::set<int>::iterator removerator = unspilledIdxs.begin(); removerator != unspilledIdxs.end();)
            {
                int argNum = (*removerator);
                LOOPS_ASSERT(argNum < op.size() && op.args[argNum].tag == REGtag);
                if (getReassigned(basket_num, (int)opnum, op.args[argNum].idx).tag == SPLtag)
                    removerator++;
                else
                    removerator = unspilledIdxs.erase(removerator);
            }
            for (std::set<int>::iterator removerator = spilledIdxs.begin(); removerator != spilledIdxs.end();)
            {
                int argNum = (*removerator);
                LOOPS_ASSERT(argNum < op.size() && op.args[argNum].tag == REGtag);
                if (getReassigned(basket_num, (int)opnum, op.args[argNum].idx).tag == SPLtag)
                    removerator++;
                else
                    removerator = spilledIdxs.erase(removerator);
            }
            to_fill.stackPlaceable[basket_num][opnum] = spilledIdxs;
            to_fill.stackPlaceable[basket_num][opnum].insert(unspilledIdxs.begin(), unspilledIdxs.end());
            to_fill.stackPlaceable[basket_num][opnum] = m_backend->filterStackPlaceable(op, to_fill.stackPlaceable[basket_num][opnum]);
            for (std::set<int>::iterator removerator = unspilledIdxs.begin(); removerator != unspilledIdxs.end();)
                if (to_fill.stackPlaceable[basket_num][opnum].count(*removerator) != 0)
                    removerator = unspilledIdxs.erase(removerator);
                else
                    removerator++;
            for (std::set<int>::iterator removerator = spilledIdxs.begin(); removerator != spilledIdxs.end();)
                if (to_fill.stackPlaceable[basket_num][opnum].count(*removerator) != 0)
                    removerator = spilledIdxs.erase(removerator);
                else
                    removerator++;

            m_pool.clearSpillPlaceholders(basket_num);
            for (int argNum : unspilledIdxs)
            {
                RegIdx idx = op.args[argNum].idx;
                LOOPS_ASSERT(argNum < op.size() && op.args[argNum].tag == REGtag);
                if(to_fill.unspilledRenaming[basket_num][opnum].count(idx) == 0) 
                {
                    RegIdx pseudoname = m_pool.provideSpillPlaceholder(basket_num);
                    if (pseudoname == IReg::NOIDX)
                        throw loops::exception("Register allocator : not enough free registers.");
                    Arg newArg = op[argNum];
                    newArg.idx = pseudoname;
                    to_fill.unspilledRenaming[basket_num][opnum][idx] = newArg;
                }
            }

            for (int argNum : spilledIdxs)
            {
                RegIdx idx = op.args[argNum].idx;
                LOOPS_ASSERT(argNum < op.size() && op.args[argNum].tag == REGtag);
                if(to_fill.unspilledRenaming[basket_num][opnum].count(idx) != 0)
                {
                    to_fill.spilledRenaming[basket_num][opnum][idx] = to_fill.unspilledRenaming[basket_num][opnum][idx];
                    continue;
                }
                if(to_fill.spilledRenaming[basket_num][opnum].count(idx) == 0) 
                {
                    RegIdx pseudoname = m_pool.provideSpillPlaceholder(basket_num);
                    if (pseudoname == IReg::NOIDX)
                        throw loops::exception("Register allocator : not enough free registers.");
                    Arg newArg = op[argNum];
                    newArg.idx = pseudoname;
                    to_fill.spilledRenaming[basket_num][opnum][idx] = newArg;
                }
            }
        }
        const int SPLtag = ((basket_num == RB_INT) ? Arg::ISPILLED : Arg::VSPILLED);
        int parametersStoodSpilled = 0; 
        for(auto p : m_stackParamLayout[basket_num])
            if (m_reg_reassignment[basket_num][p.first].args[0].tag == SPLtag)
                parametersStoodSpilled++;
        for(int idx = 0; idx < (int)m_reg_reassignment[basket_num].size(); idx++)
            for(int intnum = 0; intnum < (int)m_reg_reassignment[basket_num][idx].args.size(); intnum++)
                if(m_reg_reassignment[basket_num][idx].args[intnum].tag == SPLtag)
                    to_fill.nettoSpills[basket_num]++;
        to_fill.nettoSpills[basket_num] -= parametersStoodSpilled;
        to_fill.nettoSpills[basket_num] += (int)m_pool.usedCallee(basket_num).size();
        to_fill.spAddAligned += to_fill.nettoSpills[basket_num] * m_basketElemX[basket_num];
    }
    if(m_have_function_calls)
        to_fill.spAddAligned += m_backend->callerStackIncrement();
    to_fill.spAddAligned = m_backend->stackGrowthAlignment(to_fill.spAddAligned);
    {
        to_fill.basket_offset[RB_INT] = 0;
        to_fill.basket_offset[RB_VEC] = 0;
        std::vector<int> stackBasketOrder = m_backend->getStackBasketOrder();
        to_fill.basket_offset[stackBasketOrder[0]] = first_non_scratch_stack_position;
        for(int stackBasketNum = 1; stackBasketNum < (int)stackBasketOrder.size(); stackBasketNum++)
        {
            const int currBN = stackBasketOrder[stackBasketNum];
            const int prevBN = stackBasketOrder[stackBasketNum - 1];
            to_fill.basket_offset[currBN] = to_fill.basket_offset[prevBN] + m_basketElemX[prevBN] * to_fill.nettoSpills[prevBN];
        }
    }
    return to_fill;
}

struct CFGInfo //Control flow graph info.
{
    std::vector<std::vector<int> > succ;
    std::vector<int> predCount;
};

static CFGInfo buildCFG(const Syntfunc& a_source)
{
    const int N = (int)a_source.program.size();
    std::unordered_map<int64_t, int> labelPos;
    for(int q = 0; q < N; q++)
    {
        const Syntop& op = a_source.program[q];
        switch (op.opcode)
        {
        case (OP_LABEL):        labelPos[op.args[0].value] = q;     break; //explicit label(IF branch targets)
        case (OP_WHILE_CSTART): labelPos[op.args[0].value] = q;     break; //continue label -> loop head
        case (OP_ENDWHILE):     labelPos[op.args[1].value] = q + 1; break; //break label -> loop exit
        case (OP_ENDIF):        labelPos[op.args[0].value] = q + 1; break; //out label -> after the IF
        case (OP_ELSE):         labelPos[op.args[0].value] = q + 1; break; //else label -> else body
        }
    }
    CFGInfo cfg;
    cfg.succ.resize(N);
    cfg.predCount.assign(N, 0);
    auto target = [&](int64_t label, int def){ std::unordered_map<int64_t,int>::iterator it = labelPos.find(label); return it == labelPos.end() ? def : it->second; };
    for(int q = 0; q < N; q++)
    {
        const Syntop& op = a_source.program[q];
        switch (op.opcode)
        {
        case (OP_JMP):      cfg.succ[q].push_back(target(op.args[0].value, q + 1)); break;
        case (OP_JCC):      cfg.succ[q].push_back(q + 1); cfg.succ[q].push_back(target(op.args[1].value, q + 1)); break;
        case (OP_BREAK):    cfg.succ[q].push_back(target(op.args[0].value, q + 1)); break;
        case (OP_ENDWHILE): cfg.succ[q].push_back(target(op.args[0].value, q + 1)); break; //back-edge to whilecstart 
        case (OP_ELSE):     cfg.succ[q].push_back(target(op.args[1].value, q + 1)); break; //jump over else body 
        case (OP_RET):      break;
        default: if(q + 1 < N) cfg.succ[q].push_back(q + 1);
        }
    }
    for(int q = 0; q < N; q++)
        for(int v : cfg.succ[q])
            if(v >= 0 && v < N) cfg.predCount[v]++;
    return cfg;
}


//There are need in transfers between splits at every position. This function generate optimal movement
//sequence for satisfaction of known permutation. One scratch spill position is used for optimal solution.
void RegisterAllocator::emitParallelCopy(Syntfunc& a_destination, const std::vector<SplitTransfers>& transfersHere)
{
    auto sameLoc = [](const Arg& a, const Arg& b){ return a.tag == b.tag && a.idx == b.idx && a.value == b.value; };
    std::vector<SplitTransfers> pending;
    pending.reserve(transfersHere.size());
    for(const auto& tr : transfersHere)
    {
        const Arg& src = tr.src;
        const Arg& dst = tr.dst;
        if(sameLoc(src, dst)) //no-op: both sides ended up on the same register/slot.
            continue;
        // int basket = (src.tag == Arg::IREG || src.tag == Arg::ISPILLED) ? RB_INT : RB_VEC;
        pending.push_back(tr);
    }
    {//Collapse per-register chains.
        std::vector<SplitTransfers> collapsed;
        std::vector<bool> taken(pending.size(), false);
        for(size_t i = 0; i < pending.size(); i++)
        {
            if(taken[i])
                continue;
            std::vector<size_t> grp;
            for(size_t j = i; j < pending.size(); j++)
                if(pending[j].vidx == pending[i].vidx && pending[j].basket_num == pending[i].basket_num) //idx is per-basket
                {
                    grp.push_back(j);
                    taken[j] = true;
                }
            if(grp.size() == 1)
            {
                collapsed.push_back(pending[grp[0]]);
                continue;
            }
            size_t startk = grp[0]; //chain start: the source that is not any transfer's destination.
            for(size_t a = 0; a < grp.size(); a++)
            {
                bool isDst = false;
                for(size_t b = 0; b < grp.size() && !isDst; b++)
                    isDst = sameLoc(pending[grp[a]].src, pending[grp[b]].dst);
                if(!isDst)
                {
                    startk = grp[a];
                    break;
                }
            }
            SplitTransfers t = pending[startk];
            Arg cur = t.dst;
            bool cyclic = false;
            for(size_t step = 0; step < grp.size(); step++) //follow src==cur links to the chain end.
            {
                if(sameLoc(cur, t.src)) //chain returned to its start: a single register's transfers cancel
                { cyclic = true; break; } //(e.g. an inner loop's exit transfer and the enclosing loop's
                bool advanced = false;    //back-edge copy coincide at one op). Net is a no-op -> drop it.
                for(size_t a = 0; a < grp.size() && !advanced; a++)
                    if(sameLoc(pending[grp[a]].src, cur))
                    {
                        cur = pending[grp[a]].dst;
                        advanced = true;
                    }
                if(!advanced)
                    break;
            }
            t.dst = cur;
            if(!cyclic && !sameLoc(t.src, t.dst))
                collapsed.push_back(t);
        }
        pending.swap(collapsed);
    }
    while(!pending.empty())
    {
        int pick = -1;
        for(size_t i = 0; i < pending.size() && pick < 0; i++)
        {
            bool needed = false; //is pending[i].dst still read as a source by another pending transfer?
            for(size_t j = 0; j < pending.size() && !needed; j++)
                if(j != i && !pending[j].src_scratch && sameLoc(pending[j].src, pending[i].dst))
                    needed = true;
            if(!needed)
                pick = (int)i;
        }
        if(pick < 0)
        {   //Register-only cycle.
            int vic = -1;
            for(size_t i = 0; i < pending.size() && vic < 0; i++)
                if(!pending[i].src_scratch)
                    vic = (int)i;
            LOOPS_ASSERT(vic >= 0);
            SplitTransfers& t = pending[(size_t)vic];
            LOOPS_ASSERT(t.src.tag == Arg::IREG || t.src.tag == Arg::VREG);
            Arg sp = t.src;
            if(t.basket_num == RB_VEC)
                sp.elemtype = TYPE_U8;
            a_destination.program.push_back(Syntop(OP_SPILL, { argIImm(0), sp })); //Spill to scratch-space
            t.src_scratch = true; //will be loaded back from scratch into its destination register later.
            continue;
        }
        SplitTransfers t = pending[(size_t)pick];
        pending.erase(pending.begin() + (size_t)pick);
        const int REGtag = (t.basket_num == RB_INT ? Arg::IREG : Arg::VREG);
        const int SPLtag = (t.basket_num == RB_INT ? Arg::ISPILLED : Arg::VSPILLED);
        Arg first = t.src, second = t.dst;
        //A register copy(or full-register spill/unspill) doesn't care about vector lane type.
        if(t.basket_num == RB_VEC)
        {
            if(first.tag == Arg::VREG) first.elemtype = TYPE_U8;
            if(second.tag == Arg::VREG) second.elemtype = TYPE_U8;
        }
        if(t.src_scratch)
            a_destination.program.push_back(Syntop(OP_UNSPILL, { second, argIImm(0) })); //Unspill from scratch-space
        else if(first.tag == REGtag && second.tag == SPLtag)
            a_destination.program.push_back(Syntop(OP_SPILL, { argIImm(getSpillOffset(t.basket_num, t.vidx, second)), first }));
        else if(first.tag == SPLtag && second.tag == REGtag)
            a_destination.program.push_back(Syntop(OP_UNSPILL, { second, getSpillOffset(t.basket_num, t.vidx, first) }));
        else if(first.tag == REGtag && second.tag == REGtag)
            a_destination.program.push_back(Syntop(OP_MOV, { second, first }));
        else
        {
            throw loops::exception("Register transfer type is not supported!");
        }
    }
}

int64_t RegisterAllocator::getSpillSlot(RegIdx idx, int basket_num)
{
    auto it = m_spill_slot_of[basket_num].find(idx);
    if(it != m_spill_slot_of[basket_num].end())
        return it->second;
    int s = (int)m_spill_info.m_spoffset[basket_num]++;
    m_spill_slot_of[basket_num][idx] = s;
    return s;
}

void RegisterAllocator::insertSpillInstructions(const Syntfunc& a_source, Syntfunc& a_destination)
{
    std::vector<std::vector<SplitTransfers> > split_transfers;
    split_transfers.resize(a_source.program.size());
    //Edge-based reconciliation. For every structured-CFG edge u->v and every register live across it, if its
    //location differs(getAt(u) != getAt(v)) place a copy on that edge.
    {
        const CFGInfo cfg = buildCFG(a_source);
        const int N = (int)a_source.program.size();
        for(int u = 0; u < N; u++)
            for(int v : cfg.succ[u])
            {
                if(v < 0 || v >= N)
                    continue;
                const bool backEdge = (a_source.program[u].opcode == OP_ENDWHILE && a_source.program[v].opcode == OP_WHILE_CSTART);
                const int pos = backEdge ? u : v;
                for(int bn = 0; bn < RB_AMOUNT; bn++)
                    for(int idx = 0; idx < (int)m_reg_reassignment[bn].size(); idx++)
                    {
                        RegisterReassignment& ra = m_reg_reassignment[bn][idx];
                        if(ra.args.empty() || u < ra.bounds.front() || u >= ra.bounds.back() || v < ra.bounds.front() || v >= ra.bounds.back())
                            continue; //register not live on both ends of the edge
                        const Arg su = ra.getAt(u), sv = ra.getAt(v);
                        if(su.tag == sv.tag && su.idx == sv.idx && su.value == sv.value)
                            continue;
                        split_transfers[pos].push_back(SplitTransfers{(RegIdx)idx, su, sv, bn, false});
                    }
            }
        for(int bn = 0; bn < RB_AMOUNT; bn++)
            for(int idx = 0; idx < (int)m_reg_reassignment[bn].size(); idx++)
            {
                const RegisterReassignment& ra = m_reg_reassignment[bn][idx];
                if(ra.args.size() >= 2 && ra.bounds[1] == 0)
                    split_transfers[0].push_back(SplitTransfers{(RegIdx)idx, ra.args[0], ra.args[1], bn, false});
            }
    }

    //Renaming registers and adding spill operations
    for (size_t opnum = 0; opnum < a_source.program.size(); ++opnum)
    {
        emitParallelCopy(a_destination, split_transfers[opnum]);
        Syntop op = a_source.program[opnum];
        for (int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
            for (auto ar : m_spill_info.unspilledRenaming[basket_num].at(opnum))
                a_destination.program.push_back(Syntop(OP_UNSPILL, { ar.second, argIImm(getSpillOffset(basket_num, (int)opnum, ar.first)) }));
        for (int arnum = 0; arnum < op.size(); arnum++)
        {
            Arg& ar = op[arnum];
            if (ar.tag == Arg::IREG || ar.tag == Arg::VREG)
            {
                int basket_num = (ar.tag == Arg::IREG ? RB_INT : RB_VEC);
                if (m_spill_info.stackPlaceable[basket_num][opnum].count(arnum) != 0)
                    ar = argSpilled(basket_num, getSpillOffset(basket_num, (int)opnum, ar.idx));
                else if(m_spill_info.spilledRenaming[basket_num][opnum].count(ar.idx)) 
                    ar = m_spill_info.spilledRenaming[basket_num][opnum].at(ar.idx);
                else if(m_spill_info.unspilledRenaming[basket_num][opnum].count(ar.idx)) 
                    ar = m_spill_info.unspilledRenaming[basket_num][opnum].at(ar.idx);
                else
                    ar.idx = getReassigned(basket_num, (int)opnum, ar.idx).idx;
            }
        }
        a_destination.program.push_back(op);
        for(int basket_num = 0; basket_num<RB_AMOUNT; basket_num++)
            for (auto ar : m_spill_info.spilledRenaming[basket_num][opnum])
                a_destination.program.push_back(Syntop(OP_SPILL, { argIImm(getSpillOffset(basket_num, (int)opnum, ar.first)), ar.second }));
    }
}

void RegisterAllocator::writePrologue(Syntfunc& a_destination)
{
    if (m_spill_info.spAddAligned)
    {
        a_destination.program.push_back(Syntop(OP_SUB, { m_backend->getSParg(), m_backend->getSParg(), argIImm(m_spill_info.spAddAligned * 8) }));
        for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
        {
            size_t savNum = (m_spill_info.nettoSpills[basket_num] - m_pool.usedCallee(basket_num).size()) * m_basketElemX[basket_num];
            for (RegIdx toSav : m_pool.usedCallee(basket_num))
            {
                Arg spilled = argReg(basket_num, toSav);
                if(basket_num == RB_VEC)
                    spilled.elemtype = TYPE_U8; // We actually don't care, just taking simplest type.
                a_destination.program.push_back(Syntop(OP_SPILL, { argIImm(m_spill_info.basket_offset[basket_num] + savNum), spilled }));
                savNum += m_basketElemX[basket_num];
            }
        }
    }
    if(m_have_function_calls)
        m_backend->writeCallerPrologue(a_destination, m_spill_info.spAddAligned);
}

void RegisterAllocator::writeEpilogue(Syntfunc& a_destination)
{
    m_epilogueSize = (int)a_destination.program.size();
    { //Write epilogue
        if(m_have_function_calls)
            m_backend->writeCallerEpilogue(a_destination, m_spill_info.spAddAligned);
        if (m_spill_info.spAddAligned)
        {
            for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
            {
                size_t savNum = (m_spill_info.nettoSpills[basket_num] - m_pool.usedCallee(basket_num).size()) * m_basketElemX[basket_num];
                for (RegIdx toSav : m_pool.usedCallee(basket_num))
                {
                    Arg spilled = argReg(basket_num, toSav);
                    if(basket_num == RB_VEC)
                        spilled.elemtype = TYPE_U8; // We actually don't care, just taking simplest type.
                    a_destination.program.push_back(Syntop(OP_UNSPILL, { spilled, argIImm(m_spill_info.basket_offset[basket_num] + savNum) }));
                    savNum += m_basketElemX[basket_num];
                }
            }
            a_destination.program.push_back(Syntop(OP_ADD, { m_backend->getSParg(), m_backend->getSParg(), argIImm(m_spill_info.spAddAligned * 8) }));
        }
    }
    m_epilogueSize = (int)a_destination.program.size() - m_epilogueSize;
}

//TODO(ch): It's good idea on intel64 + windows to use "shadow space", which is 32 bytes in stack just before
//5-th parameter(other words - 1-st stack-passsed parameter). It's default and consistent place for spilling
//register parameters.
void RegisterAllocator::process(Syntfunc& a_dest, const Syntfunc& a_source)
{
    m_pool.initRegisterPool();
    m_stackParamLayout = m_backend->getStackParameterLayout(a_source, m_pool.getOverridenParams());

    std::array<std::vector<LiveInterval>, RB_AMOUNT> parintervals;
    std::array<std::multiset<LiveInterval, startordering>, RB_AMOUNT> liveintervals;
    layOutLiveIntervals(a_source, parintervals, liveintervals, m_params_sorted);

    m_reg_reassignment = assignRegisters(a_source, liveintervals, parintervals);
    m_retreg = argReg(RB_INT, m_pool.provideReturnFromPool(RB_INT));

    m_spill_info = modelSpills(a_source);

    a_dest.program.reserve(a_source.program.size() * 3 + 128);
    a_dest.params = a_source.params;
    a_dest.name = a_source.name;
    for (int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
        a_dest.regAmount[basket_num] = a_source.regAmount[basket_num];

    writePrologue(a_dest);
    insertSpillInstructions(a_source, a_dest);
    writeEpilogue(a_dest);
}

inline Arg RegisterAllocator::getReassigned(int basket_num, int opnum, int old_idx)
{
    return (old_idx == Syntfunc::RETREG && basket_num == RB_INT ? m_retreg : m_reg_reassignment[basket_num][old_idx].getAt(opnum));
}

inline int64_t RegisterAllocator::getSpillOffset(int basket_num, RegIdx reg, Arg spilled)
{
    const int SPLtag = ((basket_num == RB_INT) ? Arg::ISPILLED : Arg::VSPILLED);
    LOOPS_ASSERT(spilled.tag == SPLtag);
    int64_t spillOffset = spilled.value * m_basketElemX[basket_num] + m_spill_info.basket_offset[basket_num];
    if (isStackPassedParam(basket_num, reg))
        spillOffset = m_spill_info.spAddAligned + m_stackParamLayout[basket_num][reg];
    return spillOffset;
}

inline int64_t RegisterAllocator::getSpillOffset(int basket_num, int opnum, RegIdx reg)
{
    const int SPLtag = ((basket_num == RB_INT) ? Arg::ISPILLED : Arg::VSPILLED);
    Arg reassigned = getReassigned(basket_num, opnum, reg);
    LOOPS_ASSERT(reassigned.tag == SPLtag);
    int64_t spillOffset = reassigned.value * m_basketElemX[basket_num] + m_spill_info.basket_offset[basket_num];
    if (isStackPassedParam(basket_num, reg))
        spillOffset = m_spill_info.spAddAligned + m_stackParamLayout[basket_num][reg];
    return spillOffset;
}

inline bool RegisterAllocator::isParam(int basket_num, int idx)
{
    return idx < (int)m_params_sorted[basket_num].size();
}

inline bool RegisterAllocator::isRegisterPassedParam(int basket_num, int idx)
{
    int registerParams = (int)m_params_sorted[basket_num].size() - (int)m_stackParamLayout[basket_num].size();
    return idx < registerParams;
}

inline bool RegisterAllocator::isStackPassedParam(int basket_num, int idx)
{
    int registerParams = (int)m_params_sorted[basket_num].size() - (int)m_stackParamLayout[basket_num].size();
    return isParam(basket_num, idx) && idx >= registerParams;
}
}
