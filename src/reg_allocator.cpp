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

//1: block-hierarchical allocation(loops are allocated innermost-first and negotiated with the enclosing block).
//0: the whole function is one block, which turns the scheme into the plain linear scan; kept for A/B comparison.
#define HIERARCHICAL_LINEAR_SCAN 1

//DUBUG list:
//1.) I know certainly, that loops cannot call functions with more arguments, than calling convention grants to copy in registers. 
//2.) Restore LD2 solution.
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
                bool spilled = ra.args[sinum]->tag == Arg::ISPILLED || ra.args[sinum]->tag == Arg::VSPILLED;
                printf("(%d-%d:", ra.bounds[sinum], ra.bounds[sinum + 1]);
                printf("%s%d)|", spilled ? "s" : "r", (int)(spilled ? ra.args[sinum]->value : ra.args[sinum]->idx));
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

RegisterPool::RegisterPool(const Backend* a_backend): m_backend(a_backend)
    , m_spillPlaceholdersAvailable{0,0}
    , m_reservedRegs{0,0}
{
    memset(&(m_reservationCursor[0][0]), 0, sizeof(m_reservationCursor));
}

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
        m_maxRegisterNumber[basket_num] = origRegistersV[0][0];
        for(int vessNum = 0; vessNum < VESS_AMOUNT; vessNum++)
        {
            LOOPS_ASSERT(origRegistersV[vessNum].size() < REG_MAX);
            uint8_t regAmount = static_cast<uint8_t>(origRegistersV[vessNum].size());
            m_vessel[basket_num][vessNum] = (((uint64_t)(1)) << regAmount) - 1;
            for(uint8_t inRegNum = 0; inRegNum < regAmount; inRegNum++)
            {
                const uint8_t argregNum = (uint8_t)origRegistersV[vessNum][inRegNum];
                m_reorderInner2Arch[basket_num][vessNum][inRegNum] = argregNum;
                m_reorderArch2Inner[basket_num][vessNum][argregNum] = inRegNum;
                m_pool[basket_num] |= (((uint64_t)(1)) << argregNum);
                m_maxRegisterNumber[basket_num] = std::max(m_maxRegisterNumber[basket_num], (int)argregNum);
            }
        }
        m_spillPlaceholdersAvailable[basket_num] = m_pool[basket_num];
        m_usedCallee[basket_num].clear();
    }
}

RegIdx RegisterPool::provideParamFromPool(int basket_num, int needed_until)
{
    if(m_vessel[basket_num][PARAMS_VESS] == 0)
        return IReg::NOIDX;
    RegIdx res = lsb64(m_vessel[basket_num][PARAMS_VESS]);
    res = m_reorderInner2Arch[basket_num][PARAMS_VESS][res];
    LOOPS_ASSERT(!(reservedMask(basket_num, needed_until) & (((uint64_t)1) << res)));
    removeFromAllVessels(basket_num, res);
    return res;
}

RegIdx RegisterPool::provideRegFromPool(int basket_num, RegIdx a_hint, int needed_until)
{
    return provideRegFromPool(basket_num, a_hint, reservedMask(basket_num, needed_until));
}

RegIdx RegisterPool::provideRegFromPool(int basket_num, RegIdx a_hint, uint64_t excluded)
{
    RegIdx res = NOREGISTER;
    if (a_hint != IReg::NOIDX && !(excluded & (((uint64_t)1) << a_hint)))
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
    if (res == NOREGISTER && amountOfBits64(m_pool[basket_num] & ~excluded) > MAXIMUM_SPILLS)
    {
        for(int vessNum = 0; vessNum < VESS_AMOUNT && res == NOREGISTER; vessNum++)
        {
            uint64_t vessel = m_vessel[basket_num][vessNum];
            while(vessel)
            {
                RegIdx candidate = m_reorderInner2Arch[basket_num][vessNum][pickFirstBit64(vessel)];
                if(!(excluded & (((uint64_t)1) << candidate)))
                {
                    res = candidate;
                    break;
                }
            }
        }
        LOOPS_ASSERT(res != NOREGISTER);
    }
    if (res != NOREGISTER && m_reorderArch2Inner[basket_num][CALLEE_VESS][res] != REG_UNDEF)
        m_usedCallee[basket_num].insert(res);
    removeFromAllVessels(basket_num, res);
    res = (res == NOREGISTER) ? IReg::NOIDX : res;
    return res;
}

void RegisterPool::reserveReg(int basket_num, RegIdx reg, int from)
{
    LOOPS_ASSERT(reg != IReg::NOIDX && reg < REG_MAX);
    std::vector<int>& starts = m_reservations[basket_num][reg];
    LOOPS_ASSERT(starts.empty() || starts.back() <= from);
    starts.push_back(from);
    m_reservedRegs[basket_num] |= ((uint64_t)1) << reg;
}

void RegisterPool::clearReservations(int basket_num)
{   //Consumed registers are already out of m_reservedRegs, so every list is walked.
    for(int reg = 0; reg < REG_MAX; reg++)
    {
        m_reservations[basket_num][reg].clear();
        m_reservationCursor[basket_num][reg] = 0;
    }
    m_reservedRegs[basket_num] = 0;
}

void RegisterPool::dropReservation(int basket_num, RegIdx reg, int from)
{
    const std::vector<int>& starts = m_reservations[basket_num][reg];
    int& cursor = m_reservationCursor[basket_num][reg];
    LOOPS_ASSERT(cursor < (int)starts.size() && starts[cursor] == from);
    cursor++;
    if(cursor == (int)starts.size()) //nothing left: the register is out of the picture until clearReservations
        m_reservedRegs[basket_num] &= ~(((uint64_t)1) << reg);
}

RegIdx RegisterPool::provideReservedReg(int basket_num, RegIdx reg, int from, int needed_until)
{
    dropReservation(basket_num, reg, from);
    RegIdx res = provideRegFromPool(basket_num, reg, needed_until);
    LOOPS_ASSERT(res == reg);
    return res;
}

uint64_t RegisterPool::reservedMask(int basket_num, int needed_until) const
{
    uint64_t res = 0;
    uint64_t reserved = m_reservedRegs[basket_num];
    while(reserved)
    {
        const RegIdx reg = pickFirstBit64(reserved);
        if(m_reservations[basket_num][reg][m_reservationCursor[basket_num][reg]] < needed_until)
            res |= ((uint64_t)1) << reg;
    }
    return res;
}

size_t RegisterPool::freeRegsAmount(int basket_num, int needed_until) const
{   //TODO(ch): MAXIMUM_SPILLS must become basket-dependend variable.
    const int free_regs = amountOfBits64(m_pool[basket_num] & ~reservedMask(basket_num, needed_until));
    return free_regs > MAXIMUM_SPILLS ? free_regs - MAXIMUM_SPILLS : 0;
}

//TODO(ch): this function is just a workaround for ld2/ld3/ld4 instructions
std::vector<RegIdx> RegisterPool::provideConsecutiveRegs(int basket_num, int amount, int needed_until)
{
    int amount_temp = amount; 
    std::vector<RegIdx> res;
    std::vector<RegIdx> fragmented;
    res.reserve(amount);
    fragmented.reserve(32);
    while(amount_temp > 0)
    {
        RegIdx next = provideRegFromPool(basket_num, IReg::NOIDX, needed_until);
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
                            std::array<std::vector<RegIdx>, RB_AMOUNT>& params_sorted)
{
    //After liveness analysis we are collecting parameters' intervals(parintervals), which are immediately
    //marked as active on the root scan; non-parameter intervals are taken by blocks directly from
    //m_liveintervals_raw in allocateBlock.
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
        parintervals[basket_num].clear();
        //Function can have more arguments, than it use, so:
        size_t idxParMax = std::min((*m_liveintervals_raw)[basket_num].size(), params_sorted[basket_num].size());
        parintervals[basket_num].reserve(idxParMax);
        for (size_t idx = 0; idx < idxParMax; ++idx)
            parintervals[basket_num].push_back((*m_liveintervals_raw)[basket_num][idx]);
    }
}

int RegisterAllocator::RegisterReassignment::splitNumAt(int opnum) const
{
    LOOPS_ASSERT(!bounds.empty() && opnum >= bounds.front() && opnum < bounds.back());
    int bnum = (int)std::distance(bounds.begin(), std::lower_bound(bounds.begin(), bounds.end(), opnum));
    bnum = bounds[bnum] == opnum ? bnum : bnum - 1;
    while(bnum < ((int)args.size()) - 1 && bounds[bnum] == bounds[bnum + 1])
        bnum++;
    return bnum;
}

Arg& RegisterAllocator::RegisterReassignment::getAt(int opnum)
{
    Arg& res = *args[splitNumAt(opnum)];
    LOOPS_ASSERT(res.tag != Arg::EMPTY);
    return res;
}

const Arg& RegisterAllocator::RegisterReassignment::getAt(int opnum) const
{
    const Arg& res = *args[splitNumAt(opnum)];
    LOOPS_ASSERT(res.tag != Arg::EMPTY);
    return res;
}

void RegisterAllocator::RegisterReassignment::overlaySplit(int start, int end, AssignedArg arg)
{
    if(bounds.empty())
    {
        bounds = {start, end};
        args = {arg};
        return;
    }
    if(start >= bounds.back())
    {
        if(start > bounds.back()) //hole between existing splits and the new one
        {
            args.push_back(AssignedArg(Arg()));
            bounds.push_back(start);
        }
        args.push_back(arg);
        bounds.push_back(end);
        return;
    }
    if(end <= bounds.front())
    {
        if(end < bounds.front()) //hole between the new split and existing ones
        {
            args.insert(args.begin(), AssignedArg(Arg()));
            bounds.insert(bounds.begin(), end);
        }
        args.insert(args.begin(), arg);
        bounds.insert(bounds.begin(), start);
        return;
    }
    //Overlay in the interior: the region must lie wholly inside a single split - a hole or a covering
    //assignment of this block, which is cut around the newcomer.
    int snum = (int)std::distance(bounds.begin(), std::upper_bound(bounds.begin(), bounds.end(), start)) - 1;
    LOOPS_ASSERT(snum >= 0 && snum < (int)args.size());
    LOOPS_ASSERT(bounds[snum] <= start && end <= bounds[snum + 1]);
    if(end < bounds[snum + 1]) //keep the right part of the covering split: both halves are one location,
    {                          //so they share the object; a cut hole gets a fresh one - holes are not locations.
        AssignedArg right_part = args[snum]->tag == Arg::EMPTY ? AssignedArg(Arg()) : args[snum];
        bounds.insert(bounds.begin() + snum + 1, end);
        args.insert(args.begin() + snum + 1, right_part);
    }
    if(bounds[snum] < start) //keep the left part of the covering split
    {
        bounds.insert(bounds.begin() + snum + 1, start);
        args.insert(args.begin() + snum + 1, arg);
    }
    else
        args[snum] = arg;
}

void RegisterAllocator::RegisterReassignment::appendSplits(const RegisterReassignment& tail)
{
    if(tail.bounds.empty())
        return;
    if(bounds.empty())
    {
        *this = tail;
        return;
    }
    LOOPS_ASSERT(tail.bounds.front() >= bounds.back());
    if(tail.bounds.front() > bounds.back())
    {
        bounds.push_back(tail.bounds.front());
        args.push_back(AssignedArg(Arg()));
    }
    bounds.insert(bounds.end(), tail.bounds.begin() + 1, tail.bounds.end());
    args.insert(args.end(), tail.args.begin(), tail.args.end());
}

void RegisterAllocator::removeBranchesFromBBT(BasicBlocksTree& node)
{
    std::vector<std::shared_ptr<BasicBlocksTree> > newChildren;
    newChildren.reserve(node.children.size());
    for(std::shared_ptr<BasicBlocksTree>& child : node.children)
    {
        removeBranchesFromBBT(*child);
#if HIERARCHICAL_LINEAR_SCAN
        const bool dissolve = (child->type == BasicBlocksTree::BBT_IF);
#else
        const bool dissolve = true;
#endif
        if(dissolve)
        {
            for(int basketNum = 0; basketNum < RB_AMOUNT; basketNum++)
                node.reg_occurencies[basketNum].insert(child->reg_occurencies[basketNum].begin(),
                                                        child->reg_occurencies[basketNum].end());
            for(std::shared_ptr<BasicBlocksTree>& grandchild : child->children)
                newChildren.push_back(grandchild);
        }
        else
            newChildren.push_back(child);
    }
    node.children = std::move(newChildren);
}

std::array<std::vector<RegisterAllocator::RegisterReassignment>, RB_AMOUNT> RegisterAllocator::assignRegisters(const Syntfunc& a_source,
    const std::array<std::vector<LiveInterval>, RB_AMOUNT>& parintervals)
{
    //Function takes live intervals, program and create a mapping from abstract old register
    //indexes to new real machine registers. If it's not enough machine registers, some of
    //them are spilled, so mapping for this register will point to spill position in stack.

    //Remove IF statements, because they doesn't affect performance on register allocation.
    removeBranchesFromBBT(*m_bbt);

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
    {
        m_spill_slot_of[basket_num].clear();
        m_spill_info.m_spoffset[basket_num] = 0;
    }

    std::array<std::vector<RegisterReassignment>, RB_AMOUNT> result;
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
    {
        std::multiset<LiveInterval, endordering> active;
        result[basket_num].resize(a_source.regAmount[basket_num]);
        {//Get pseudonames for parameters.
            RegIdx parreg = 0;
            for (; parreg < (int)parintervals[basket_num].size(); parreg++)
            {
                const LiveInterval& interval = parintervals[basket_num][parreg];
                RegIdx idx = interval.idx;
                RegIdx attempt = m_pool.provideParamFromPool(basket_num, interval.end + 1);
                if (attempt == IReg::NOIDX)
                    break;
                result[basket_num][idx] = RegisterReassignment(interval.start, interval.end + 1, argReg(basket_num, attempt));
                if(isStackPassedParam(basket_num, idx))
                    result[basket_num][idx].overlaySplit(0, interval.start, argSpilled(basket_num, 0));
                active.insert(parintervals[basket_num][parreg]);
            }
            for (; parreg < (int)parintervals[basket_num].size(); parreg++)
            {
                const LiveInterval& interval = parintervals[basket_num][parreg];
                RegIdx idx = interval.idx;
                RegIdx attempt = m_pool.provideRegFromPool(basket_num, IReg::NOIDX, interval.end + 1);
                if (attempt == IReg::NOIDX)
                    break;
                result[basket_num][idx] = RegisterReassignment(interval.start, interval.end + 1, argReg(basket_num, attempt));
                if(isStackPassedParam(basket_num, idx))
                    result[basket_num][idx].overlaySplit(0, interval.start, argSpilled(basket_num, 0));
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
        allocateBlock(basket_num, *m_bbt, a_source, active, result[basket_num], m_pool);
        for(RegIdx idx = 0; idx < (RegIdx)result[basket_num].size(); idx++)
            for(const AssignedArg& arg : result[basket_num][idx].args)
                LOOPS_ASSERT(arg->tag != Arg::EMPTY); //every hole must be filled: splits tile the whole live interval.
    }
    return result;
}

static inline bool isLiveInBlock(const LiveInterval& li, int start, int end)
{
    const int lo = std::max(li.start, start);
    const int hi = std::min(li.end + 1, end);
    return lo < hi;
}

void RegisterAllocator::allocateBlock(int basket_num, const BasicBlocksTree& node, const Syntfunc& a_source,
    std::multiset<LiveInterval, endordering>& active,
    std::vector<RegisterReassignment>& block_reassignment,
    RegisterPool& result_pool)
{
    //1. Collect this block's own intervals.
    const bool isRoot = (node.type == BasicBlocksTree::BBT_FUNC);
    const int nlo = isRoot ? 0 : node.start_pos;
    const int nhi = node.end_pos;
    //ULTRADUBUG: Well, I really don't like, that we are constructing blockIntervals many times by full passage over m_liveintervals_raw.
    //It have to be done only once and this initial container have to be cut locally. It will be much faster.
    std::multiset<LiveInterval, startordering> blockIntervals;
    const std::vector<LiveInterval>& raw_intervals = (*m_liveintervals_raw)[basket_num];
    for(RegIdx idx = 0; idx < (RegIdx)raw_intervals.size(); idx++)
    {
        const LiveInterval& li = raw_intervals[idx];
        if(isParam(basket_num, idx) || !isLiveInBlock(li, nlo, nhi))
            continue;
        const int lo = std::max(li.start, nlo);
        const int hi = std::min(li.end + 1, nhi);
        LiveInterval clip(idx, lo);
        clip.end = hi - 1;
        clip.priority = li.priority;
        blockIntervals.insert(clip);
    }
    //2. Allocate child loops first(innermost-first). Each child scans on a fresh pool(copy of m_poolBase, so
    //parameters/callee stay reserved) and a fresh active set into its own table: it is an independent allocation.
    std::vector<std::vector<RegisterReassignment> > children_reassignments(node.children.size());
    std::vector<RegisterPool> children_pools(node.children.size(), m_poolBase);
    std::vector<std::unordered_set<RegIdx>> children_live(node.children.size(), std::unordered_set<RegIdx>());
    for(size_t cnum = 0; cnum < node.children.size(); cnum++)
    {
        children_reassignments[cnum].resize(block_reassignment.size());
        std::multiset<LiveInterval, endordering> childActive;
        allocateBlock(basket_num, *node.children[cnum], a_source, childActive, children_reassignments[cnum], children_pools[cnum]);
        for(const LiveInterval& li : blockIntervals)
            if(isLiveInBlock(li, node.children[cnum]->start_pos, node.children[cnum]->end_pos))
                children_live[cnum].insert(li.idx);
    }
    std::vector<RegisterReassignment> negotiated_assignments = negotiateAndMergeBlockAssignments(basket_num, children_reassignments,
                                                                                          node.children, children_live, children_pools);
    //3. Linear scan of block itself with geven hints from children and final merging.
    std::unordered_map<RegIdx, RegIdx> hints = makeBlocksHints(negotiated_assignments, node.children);
    if(node.type == BasicBlocksTree::BBT_ALLOCATION_RESTRICTION)
    {
        LOOPS_ASSERT((node.end_pos - node.start_pos) == 1);
        int opnum = node.start_pos;
        AllocationRestriction restriction = m_backend->getRestriction(a_source.program[opnum]);
        if(restriction.type & AllocationRestriction::AR_FIXED)
        {
            //ULTRADUBUG:

        }
    }
    else
        linearScan(basket_num, a_source, blockIntervals, active, block_reassignment, result_pool, hints, node.reg_occurencies[basket_num]);
    //4. Overlay children splits over the block's own ones. A child's split that landed on the same register
    //as the block's is the same location: the objects are united, so a later renaming moves them together
    //and the boundary needs no transfer.
    const int REGtag = ((basket_num == RB_INT) ? Arg::IREG : Arg::VREG);
    for(RegIdx idx = 0; idx < (RegIdx)negotiated_assignments.size(); idx++)
    {
        const RegisterReassignment& negotiated = negotiated_assignments[idx];
        RegisterReassignment& own = block_reassignment[idx];
        for(int snum = 0; snum < (int)negotiated.args.size(); snum++)
        {
            AssignedArg child_arg = negotiated.args[snum];
            if(child_arg->tag == Arg::EMPTY)
                continue;
            const int start = negotiated.bounds[snum];
            const int end = negotiated.bounds[snum + 1];
            if(!own.bounds.empty())
            {
                AssignedArg own_arg = own.args[own.splitNumAt(start)];
                if(own_arg->tag == REGtag && child_arg->tag == REGtag && own_arg->idx == child_arg->idx)
                    own_arg.merge(child_arg);
            }
            own.overlaySplit(start, end, child_arg);
        }
    }
    for(size_t cnum = 0; cnum < node.children.size(); cnum++)
    {
        result_pool.mergeUsedCallee(children_pools[cnum]);
        result_pool.mergeSpillPlaceholders(children_pools[cnum]);
    }
}

static bool sameLocation(const Arg& a, const Arg& b)
{
    if(a.tag != b.tag)
        return false;
    if(a.tag == Arg::IREG || a.tag == Arg::VREG)
        return a.idx == b.idx;
    if(a.tag == Arg::ISPILLED || a.tag == Arg::VSPILLED)
        return a.value == b.value;
    return a.tag == Arg::EMPTY; //holes are indistinguishable; other tags are not locations at all
}

bool RegisterAllocator::RegisterReassignment::isSwappableInBlock(const BasicBlocksTreePtr& block) const
{
    //The value can be born or die inside the block: probed is its own range there.
    int startSplitNum = splitNumAt(std::max(block->start_pos, bounds.front()));
    int endSplitNum = splitNumAt(std::min(block->end_pos, bounds.back()) - 1);
    AssignedArg startSplit = args[startSplitNum];
    if(startSplit->tag != Arg::IREG && startSplit->tag != Arg::VREG) 
        return false;
    if(startSplitNum == endSplitNum)
        return true;
    AssignedArg nextSplit;
    for(int snum = startSplitNum + 1; snum < endSplitNum; snum++)
        if(!startSplit.isConnected(args[snum]))
        {   
            nextSplit = args[snum];
            break;
        }
    if(nextSplit.isEmpty())
        return true;
    return startSplit->idx != nextSplit->idx;
}

void RegisterAllocator::formNegotiationHeader(int basket_num,
                                              std::vector<RegisterReassignment>& assignment,
                                              const BasicBlocksTreePtr& block,
                                              const std::unordered_set<RegIdx>& live,
                                              std::vector<int>& weights,
                                              std::vector<RegIdx>& hw2reg)
{
    weights.clear();
    hw2reg.clear();
    weights.resize(assignment.size(), 0);
    hw2reg.resize(m_poolBase.maxRegisterNumber(basket_num) + 1, NOASSIGNED);
    for(RegIdx rnum: live)
        for(const AssignedArg& arg : assignment[rnum].args)
            if(arg->tag == Arg::IREG || arg->tag == Arg::VREG)
            {
                RegIdx& owner = hw2reg[arg->idx];
                owner = (owner == NOASSIGNED || owner == rnum) ? rnum : (RegIdx)SHARED;
            }
    for(RegIdx rnum: live)
    {
        const Arg& assigned = assignment[rnum].getAtEntry(block->start_pos);
        if((assigned.tag == Arg::IREG || assigned.tag == Arg::VREG) && hw2reg[assigned.idx] == rnum && assignment[rnum].isSwappableInBlock(block))
            weights[rnum] = 1;
    }
}

std::vector<RegisterAllocator::RegisterReassignment> RegisterAllocator::negotiateAndMergeBlockAssignments(int basket_num,
                                                             std::vector<std::vector<RegisterReassignment>>& assignments,
                                                             const std::vector<BasicBlocksTreePtr>& blocks,
                                                             const std::vector<std::unordered_set<RegIdx>>& live, //DUBUG: probably, it's better to make via blocks ranges and access to raw_intervals?
                                                             std::vector<RegisterPool>& pools)
{
    LOOPS_ASSERT(assignments.size() == blocks.size() &&
                 assignments.size() == live.size() &&
                 assignments.size() == pools.size());
    if(assignments.empty())
        return std::vector<RegisterReassignment>();
    if(assignments.size() == 1)
        return assignments[0];
    std::unordered_set<RegIdx> upper_live = live[0];
    //Here weight of register means amount of blocks, where it is connected in one component
    //Since we are negotiating it from up to down, appending blocks one by one, measure LAST 
    //connectivity component. This is important, because we cannot swap components with different
    //weight. Just pretend, that we have to move swap register, used in two blocks with register, 
    //used in one block. It will unpredictably break situation(or it will become too hard to implement
    //it correctly). Thus, we are consider only swaps for equally weighted registers.
    std::vector<std::vector<RegIdx>> hw2reg(assignments.size(), std::vector<RegIdx>());
    std::vector<int> upper_weights, lower_weights;
    formNegotiationHeader(basket_num, assignments[0], blocks[0], upper_live, upper_weights, hw2reg[0]);

    auto deeplyNoAssigned = [&hw2reg](int lowest_block, int weight, RegIdx hwidx)
    {
        for(int bnum = lowest_block; lowest_block - bnum < weight; bnum--) 
            if(hw2reg[bnum][hwidx] != NOASSIGNED)
                return false;
        return true;
    };

    for(int bnum = 1; bnum < (int)assignments.size(); bnum++)
    {
        std::vector<RegisterReassignment>& upper_assignment = assignments[bnum-1];
        std::vector<RegisterReassignment>& lower_assignment = assignments[bnum];
        const BasicBlocksTree& upper_block = *(blocks[bnum-1]);
        const BasicBlocksTree& lower_block = *(blocks[bnum]);
        const std::unordered_set<RegIdx>& lower_live = live[bnum];
        RegisterPool& lower_pool = pools[bnum];
        std::vector<RegIdx>& upper_hw2reg = hw2reg[bnum-1];
        std::vector<RegIdx>& lower_hw2reg = hw2reg[bnum];

        LOOPS_ASSERT(lower_assignment.size() == assignments[0].size());
        formNegotiationHeader(basket_num, lower_assignment, blocks[bnum], lower_live, lower_weights, lower_hw2reg);
        for(RegIdx vidx: lower_live)
        {
            if(lower_weights[vidx] > 0 && upper_weights[vidx] > 0) 
            {
                Arg& upper_reg_assign = upper_assignment[vidx].getAt(upper_block.start_pos);
                Arg& lower_reg_assign = lower_assignment[vidx].getAt(lower_block.start_pos);
                RegIdx upper_hwidx = upper_reg_assign.idx;
                RegIdx lower_hwidx = lower_reg_assign.idx;
                if(upper_hwidx != lower_hwidx)
                {
                    if(lower_hw2reg[upper_hwidx] == NOASSIGNED)
                    {
                        int splitnum = lower_assignment[vidx].entrySplitNum(lower_block.start_pos);
                        lower_assignment[vidx].args[splitnum]->idx = upper_hwidx;
                        lower_hw2reg[upper_hwidx] = vidx;
                        lower_hw2reg[lower_hwidx] = NOASSIGNED;
                        lower_pool.releaseReg(basket_num, lower_hwidx);
                        lower_pool.provideRegFromPool(basket_num, upper_hwidx, lower_block.end_pos);
                    }
                    else if(upper_hw2reg[lower_hwidx] == NOASSIGNED && deeplyNoAssigned(bnum - 1, upper_weights[vidx], lower_hwidx))
                    {
                        for(int ubnum = bnum - 1; (bnum - 1) - ubnum < upper_weights[vidx]; ubnum--)
                        {
                            int splitnum = assignments[ubnum][vidx].entrySplitNum(blocks[ubnum]->start_pos);
                            assignments[ubnum][vidx].args[splitnum]->idx = lower_hwidx;
                            hw2reg[ubnum][lower_hwidx] = vidx;
                            hw2reg[ubnum][upper_hwidx] = NOASSIGNED;
                            pools[ubnum].releaseReg(basket_num, upper_hwidx);
                            pools[ubnum].provideRegFromPool(basket_num, lower_hwidx, blocks[ubnum]->end_pos);
                        }
                    }
                    else if(lower_hw2reg[upper_hwidx] >= 0 && lower_weights[lower_hw2reg[upper_hwidx]] > 0)
                    {
                        int vidx_alt = lower_hw2reg[upper_hwidx];
                        int splitnum = lower_assignment[vidx].entrySplitNum(lower_block.start_pos);
                        int splitnum_alt = lower_assignment[vidx_alt].entrySplitNum(lower_block.start_pos);
                        std::swap(lower_assignment[vidx].args[splitnum]->idx, lower_assignment[vidx_alt].args[splitnum_alt]->idx);
                        std::swap(lower_hw2reg[upper_hwidx], lower_hw2reg[lower_hwidx]);
                    }
                    else if(upper_hw2reg[lower_hwidx] >= 0 && upper_weights[upper_hw2reg[lower_hwidx]] > 0 && upper_weights[upper_hw2reg[lower_hwidx]] == upper_weights[vidx])
                    {
                        int vidx_alt = upper_hw2reg[lower_hwidx];
                        int splitnum = upper_assignment[vidx].entrySplitNum(upper_block.start_pos);
                        int splitnum_alt = upper_assignment[vidx_alt].entrySplitNum(upper_block.start_pos);
                        std::swap(upper_assignment[vidx].args[splitnum]->idx, upper_assignment[vidx_alt].args[splitnum_alt]->idx);
                        std::swap(upper_hw2reg[upper_hwidx], upper_hw2reg[lower_hwidx]);
                    }
                }
                upper_hwidx = upper_reg_assign.idx;
                lower_hwidx = lower_reg_assign.idx;
                if(upper_hwidx == lower_hwidx)
                {
                    int upper_splitnum = upper_assignment[vidx].splitNumAt(upper_block.start_pos);
                    int lower_splitnum = lower_assignment[vidx].splitNumAt(lower_block.start_pos);
                    upper_assignment[vidx].args[upper_splitnum].merge(lower_assignment[vidx].args[lower_splitnum]);
                    lower_weights[vidx] = upper_weights[vidx] = upper_weights[vidx] + lower_weights[vidx];
                }
            }
        }
        //Cheapeast way to write upper_weights = lower_weights.
        upper_weights.swap(lower_weights);
    }
    //Merging assignemnts
    std::vector<RegisterReassignment> result(assignments[0].size());
    for(size_t bnum = 0; bnum < assignments.size(); bnum++)
        for(RegIdx vidx = 0; vidx < (RegIdx)result.size(); vidx++)
            result[vidx].appendSplits(assignments[bnum][vidx]);
    return result;
}

std::unordered_map<RegIdx, RegIdx> RegisterAllocator::makeBlocksHints(std::vector<RegisterReassignment>& assignment,
                                                                      const std::vector<BasicBlocksTreePtr>& blocks)
{
    std::unordered_map<RegIdx, std::unordered_map<RegIdx, int> > occur_amount;
    for(int bnum = 0; bnum < (int)blocks.size(); bnum++)
    {
        const int start_pos = blocks[bnum]->start_pos;
        const int end_pos = blocks[bnum]->end_pos;
        for(int anum = 0; anum < (int)assignment.size(); anum++)
        {
            const RegisterReassignment& re = assignment[anum];
            if(re.bounds.empty())
                continue;
            const int probe = std::max(start_pos, re.bounds.front());
            if(probe >= end_pos || probe >= re.bounds.back())
                continue;
            const AssignedArg& arg = re.args[re.splitNumAt(probe)];
            if(arg->tag == Arg::IREG || arg->tag == Arg::VREG)
                occur_amount[anum][arg->idx]++;
        }
    }
    std::unordered_map<RegIdx, RegIdx> result;
    for(std::pair<RegIdx, std::unordered_map<RegIdx, int> > opair: occur_amount)
    {
        if(opair.second.empty())
            continue;
        int maximizer = opair.second.begin()->first;
        int maximum = opair.second.begin()->second;
        for(std::pair<RegIdx, int> mpair: opair.second)
        {
            if(maximum <= mpair.second)
            {
                maximizer = mpair.first;
                maximum = mpair.second;
            }
        }
        result[opair.first] = maximizer;
    }
    return result;
}

void RegisterAllocator::linearScan(int basket_num, const Syntfunc& a_source,
                                   const std::multiset<LiveInterval, startordering>& liveintervals,
                                   std::multiset<LiveInterval, endordering>& active,
                                   std::vector<RegisterReassignment>& block_reassignment,
                                   RegisterPool& result_pool,
                                   const std::unordered_map<RegIdx, RegIdx>& hints,
                                   const std::unordered_set<RegIdx>& reg_occurencies)
{
    //ULTRADUBUG: Bad news: packs can break reusingHints, since just died interval can be located in later pack(And we have to fix it!) 
    //Sorting intervals into packs: used first, hinted first.
    std::vector<std::multiset<LiveInterval, startordering> > packs;
    if(reg_occurencies.size())
    {
        std::multiset<LiveInterval, startordering> used;
        std::multiset<LiveInterval, startordering> riders;
        for(const LiveInterval& interval : liveintervals)
            if(reg_occurencies.find(interval.idx) == reg_occurencies.end())
                riders.insert(riders.end(), interval);
            else
                used.insert(used.end(), interval);
        if(used.size() > 0)
        {
            packs.push_back(std::multiset<LiveInterval, startordering>());
            packs.back().swap(used);
        }
        if(riders.size() > 0)
        {
            packs.push_back(std::multiset<LiveInterval, startordering>());
            packs.back().swap(riders);
        }
    }
    else
        packs.push_back(liveintervals);
    if(hints.size())
        for(int pnum = packs.size() - 1; pnum >= 0; pnum--)
        {
            std::multiset<LiveInterval, startordering> hinted;
            std::multiset<LiveInterval, startordering> nonhinted;
            for(const LiveInterval& interval : packs[pnum])
                if(hints.find(interval.idx) == hints.end())
                    nonhinted.insert(nonhinted.end(), interval);
                else
                    hinted.insert(hinted.end(), interval);
            if(hinted.size() > 0 && nonhinted.size() > 0)
            {
                packs[pnum].swap(hinted);
                packs.insert(packs.begin() + pnum + 1, std::multiset<LiveInterval, startordering>());
                packs[pnum + 1].swap(nonhinted);
            }
        }

    const int REGtag = ((basket_num == RB_INT) ? Arg::IREG : Arg::VREG);
    std::multiset<LiveInterval, startordering> fixed;
    std::unordered_set<RegIdx> fixed_idxs;
    std::vector<LiveInterval> placed(active.begin(), active.end()); //Intervals scanned since the last pack change.
    for(int pnum = 0; pnum < (int)packs.size(); pnum++)
    {
        if(pnum > 0)
        {
            for(const LiveInterval& li : active)
                result_pool.releaseReg(basket_num, block_reassignment[li.idx].getAt(li.start).idx);
            active.clear();
            for(const LiveInterval& li : placed)
                if(block_reassignment[li.idx].getAt(li.start).tag == REGtag) //spilled ones reserve nothing
                {
                    fixed.insert(li);
                    fixed_idxs.insert(li.idx);
                }
            placed.clear();
            result_pool.clearReservations(basket_num);
            for(const LiveInterval& li : fixed)
                result_pool.reserveReg(basket_num, block_reassignment[li.idx].getAt(li.start).idx, li.start);
        }
        std::multiset<LiveInterval, startordering>::iterator fixed_it = fixed.begin();
        for (auto interval = packs[pnum].begin(); interval != packs[pnum].end(); ++interval)
        {
            std::unordered_map<RegIdx, RegIdx> opUndefs; //TODO(ch): You also have to consider spilled undefs.
            const int hi = interval->end + 1;
            { //Dropping expired registers.
                auto removerator = active.begin();
                for (; removerator != active.end(); ++removerator)
                    if (removerator->end <= interval->start)
                    {
                        Arg curLoc = block_reassignment[removerator->idx].getAt(removerator->start);
                        LOOPS_ASSERT(curLoc.tag == REGtag);
                        int assigned_idx = curLoc.idx;
                        result_pool.releaseReg(basket_num, assigned_idx);
                        if (removerator->end == interval->start) //Current line, line of definition of considered register
                            opUndefs.insert(std::pair<RegIdx,RegIdx>(removerator->idx, assigned_idx));
                    }
                    else
                        break;
                active.erase(active.begin(), removerator);
            }
            //Fixed intervals reached by the cursor take their reserved registers(ones already over are reuse hints at most).
            for(; fixed_it != fixed.end() && fixed_it->start <= interval->start; ++fixed_it)
            {
                const RegIdx fixed_reg = block_reassignment[fixed_it->idx].getAt(fixed_it->start).idx;
                if(fixed_it->end <= interval->start)
                {
                    result_pool.dropReservation(basket_num, fixed_reg, fixed_it->start);
                    if(fixed_it->end == interval->start)
                        opUndefs.insert(std::pair<RegIdx,RegIdx>(fixed_it->idx, fixed_reg));
                    continue;
                }
                result_pool.provideReservedReg(basket_num, fixed_reg, fixed_it->start, fixed_it->end);
                active.insert(*fixed_it);
            }
            placed.push_back(*interval);
            if (!result_pool.havefreeRegs(basket_num, hi))
            {
                bool stackParameterSpilled = false;
                std::multiset<LiveInterval, endordering>::reverse_iterator lastactive = active.rbegin();
                while(lastactive != active.rend() && fixed_idxs.count(lastactive->idx))
                    ++lastactive;
                if (lastactive != active.rend() && lastactive->end > interval->end)
                {
                    //Victim is a register considered to be moved to stack.
                    const RegIdx vict_idx = lastactive->idx;
                    Arg curLoc = block_reassignment[vict_idx].getAt(lastactive->start);
                    LOOPS_ASSERT(curLoc.tag == REGtag);
                    const RegIdx stolenReg = curLoc.idx; //Victim's active register, read before changes.
                    stackParameterSpilled = isStackPassedParam(basket_num, vict_idx);
                    const Arg victSpilled = argSpilled(basket_num, stackParameterSpilled ? 0 : getSpillSlot(vict_idx, basket_num));
                    if(isParam(basket_num, vict_idx))
                    {
                        RegisterReassignment keeped = block_reassignment[vict_idx];
                        int lastsn = ((int)keeped.args.size()) - 1;
                        block_reassignment[vict_idx] = RegisterReassignment(keeped.bounds[lastsn], keeped.bounds[lastsn + 1], victSpilled);
                        if(isRegisterPassedParam(basket_num, vict_idx))
                            block_reassignment[vict_idx].overlaySplit(0, keeped.bounds[lastsn], *(keeped.args[0]));
                    }
                    else
                        block_reassignment[vict_idx].getAt(lastactive->start) = victSpilled;
                    block_reassignment[interval->idx] = RegisterReassignment(interval->start, hi, argReg(basket_num, stolenReg));
                    active.erase(std::next(lastactive).base());
                    active.insert(*interval);
                }
                else
                {
                    stackParameterSpilled = isStackPassedParam(basket_num, interval->idx);
                    const Arg sp = argSpilled(basket_num, stackParameterSpilled ? 0 : getSpillSlot(interval->idx, basket_num));
                    block_reassignment[interval->idx] = RegisterReassignment(interval->start, hi, sp);
                }
            }
            else
            {
                RegIdx hwReg;
                active.insert(*interval);
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
                if(poolHint == IReg::NOIDX && hints.find(interval->idx) != hints.end())
                    poolHint = hints.at(interval->idx);
                hwReg = result_pool.provideRegFromPool(basket_num, poolHint, hi);
                block_reassignment[interval->idx] = RegisterReassignment(interval->start, hi, argReg(basket_num, hwReg));
            }
        }
    }
    result_pool.clearReservations(basket_num);
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
        //Spilled values area is sized by the amount of slots provided by getSpillSlot: a slot stays
        //reserved even if children assignments were overlaid over all spilled splits using it.
        //Stack-passed parameters don't consume slots(they live in the caller's frame).
        to_fill.nettoSpills[basket_num] = (int)m_spill_info.m_spoffset[basket_num] + (int)m_pool.usedCallee(basket_num).size();
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

void RegisterAllocator::insertMultiLevelJumpTransfers(const Syntfunc& a_source,
    std::vector<std::vector<SplitTransfers> >& a_split_transfers)
{
#if !HIERARCHICAL_LINEAR_SCAN
    return; //One location per value for the whole function: a jump has nothing to compensate.
#endif
    const int N = (int)a_source.program.size();
    std::unordered_map<int64_t, int> label_owner_pos;
    for(int opnum = 0; opnum < N; opnum++)
    {
        const Syntop& op = a_source.program[opnum];
        if(op.opcode == OP_WHILE_CSTART)
            label_owner_pos[op.args[0].value] = opnum;
        else if(op.opcode == OP_ENDWHILE)
            label_owner_pos[op.args[1].value] = opnum;
    }
    for(int opnum = 0; opnum < N; opnum++)
    {
        const int jump_opcode = a_source.program[opnum].opcode;
        if(jump_opcode != OP_BREAK && jump_opcode != OP_CONTINUE)
            continue;
        //Chain of loops containing the jump, innermost last(IF nodes are already removed from the tree).
        std::vector<const BasicBlocksTree*> enclosing;
        const BasicBlocksTree* node = m_bbt.get();
        while(true)
        {
            const BasicBlocksTree* deeper = nullptr;
            for(const BasicBlocksTreePtr& child : node->children)
                if(opnum >= child->start_pos && opnum < child->end_pos)
                {
                    deeper = child.get();
                    break;
                }
            if(deeper == nullptr)
                break;
            enclosing.push_back(deeper);
            node = deeper;
        }
        auto owner = label_owner_pos.find(a_source.program[opnum].args[0].value);
        LOOPS_ASSERT(!enclosing.empty() && owner != label_owner_pos.end());
        const int owner_pos = owner->second;
        const BasicBlocksTree* target = nullptr;
        for(int depth = (int)enclosing.size() - 1; depth >= 0; depth--)
            if(enclosing[depth]->start_pos == owner_pos || enclosing[depth]->end_pos - 1 == owner_pos)
            {
                target = enclosing[depth];
                break;
            }
        if(target == nullptr)
            throw loops::exception("Register allocator: jump target is not a boundary of an enclosing loop.");
        if(target == enclosing.back())
            continue; //single-level jump: reconciled by ordinary boundary transfers.
        a_split_transfers[opnum].clear();
        const bool to_head = (target->start_pos == owner_pos);
        LOOPS_ASSERT(to_head == (jump_opcode == OP_CONTINUE));
        for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
            for(int idx = 0; idx < (int)m_reg_reassignment[basket_num].size(); idx++)
            {
                RegisterReassignment& ra = m_reg_reassignment[basket_num][idx];
                if(ra.args.empty())
                    continue;
                //Value must be alive at the jump and at the landing point(intervals are contiguous, and
                //the jump op defines nothing, so front <= opnum - 1 still means alive at the jump).
                if(to_head ? (ra.bounds.front() > owner_pos || ra.bounds.back() <= opnum)
                           : (ra.bounds.front() > opnum - 1 || ra.bounds.back() <= target->end_pos))
                    continue;
                const Arg su = ra.getAt(opnum - 1), sv = ra.getAt(owner_pos);
                if(sameLocation(su, sv))
                    continue;
                a_split_transfers[opnum].push_back(SplitTransfers{(RegIdx)idx, su, sv, basket_num, false});
            }
    }
}

//There are need in transfers between splits at every position. This function generate optimal movement
//sequence for satisfaction of known permutation. One scratch spill position is used for optimal solution.
void RegisterAllocator::emitParallelCopy(Syntfunc& a_destination, const std::vector<SplitTransfers>& transfersHere)
{
    std::vector<SplitTransfers> pending;
    pending.reserve(transfersHere.size());
    for(const auto& tr : transfersHere)
    {
        const Arg& src = tr.src;
        const Arg& dst = tr.dst;
        if(sameLocation(src, dst)) //no-op: both sides ended up on the same register/slot.
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
                    isDst = sameLocation(pending[grp[a]].src, pending[grp[b]].dst);
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
                if(sameLocation(cur, t.src)) //chain returned to its start: a single register's transfers cancel
                { cyclic = true; break; } //(e.g. an inner loop's exit transfer and the enclosing loop's
                bool advanced = false;    //back-edge copy coincide at one op). Net is a no-op -> drop it.
                for(size_t a = 0; a < grp.size() && !advanced; a++)
                    if(sameLocation(pending[grp[a]].src, cur))
                    {
                        cur = pending[grp[a]].dst;
                        advanced = true;
                    }
                if(!advanced)
                    break;
            }
            t.dst = cur;
            if(!cyclic && !sameLocation(t.src, t.dst))
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
                if(j != i && !pending[j].src_scratch && sameLocation(pending[j].src, pending[i].dst))
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

static void collectBlockBoundaries(const BasicBlocksTree& node, std::vector<int>& boundaries)
{
    for(const BasicBlocksTreePtr& child : node.children)
    {
        boundaries.push_back(child->start_pos);
        boundaries.push_back(child->end_pos);
        collectBlockBoundaries(*child, boundaries);
    }
}

void RegisterAllocator::insertSpillInstructions(const Syntfunc& a_source, Syntfunc& a_destination)
{
    //1.) Forming transfers on edges of register's subintervals. 
    std::vector<std::vector<SplitTransfers> > split_transfers;
    split_transfers.resize(a_source.program.size());
    {
        const int N = (int)a_source.program.size();
        std::vector<int> boundaries;
        collectBlockBoundaries(*m_bbt, boundaries);
        std::sort(boundaries.begin(), boundaries.end());
        boundaries.erase(std::unique(boundaries.begin(), boundaries.end()), boundaries.end());
        //Self-check of the scheme above: a location change anywhere else would be silently lost.
        for(int bn = 0; bn < RB_AMOUNT; bn++)
            for(int idx = 0; idx < (int)m_reg_reassignment[bn].size(); idx++)
            {
                const RegisterReassignment& ra = m_reg_reassignment[bn][idx];
                for(int snum = 1; snum < (int)ra.args.size(); snum++)
                {
                    if(sameLocation(*ra.args[snum - 1], *ra.args[snum]))
                        continue;
                    LOOPS_ASSERT(ra.bounds[snum] == 0 || std::binary_search(boundaries.begin(), boundaries.end(), ra.bounds[snum]));
                }
            }
        for(int p : boundaries)
        {
            if(p <= 0 || p >= N)
                continue;
            for(int bn = 0; bn < RB_AMOUNT; bn++)
                for(int idx = 0; idx < (int)m_reg_reassignment[bn].size(); idx++)
                {
                    RegisterReassignment& ra = m_reg_reassignment[bn][idx];
                    if(ra.args.empty() || ra.bounds.front() >= p || ra.bounds.back() <= p)
                        continue; //value is not alive on both sides of the boundary
                    const Arg su = ra.getAt(p - 1), sv = ra.getAt(p);
                    if(sameLocation(su, sv))
                        continue;
                    split_transfers[p].push_back(SplitTransfers{(RegIdx)idx, su, sv, bn, false});
                }
        }
        for(int bn = 0; bn < RB_AMOUNT; bn++)
            for(int idx = 0; idx < (int)m_reg_reassignment[bn].size(); idx++)
            {
                const RegisterReassignment& ra = m_reg_reassignment[bn][idx];
                if(ra.args.size() >= 2 && ra.bounds[1] == 0)
                    split_transfers[0].push_back(SplitTransfers{(RegIdx)idx, *ra.args[0], *ra.args[1], bn, false});
            }
    }
    insertMultiLevelJumpTransfers(a_source, split_transfers);

    //2.) Renaming registers and adding spill operations
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
    layOutLiveIntervals(a_source, parintervals, m_params_sorted);

    m_reg_reassignment = assignRegisters(a_source, parintervals);
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
