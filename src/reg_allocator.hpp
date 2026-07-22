/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_REG_ALLOCATOR_HPP__
#define __LOOPS_REG_ALLOCATOR_HPP__

#include "loops/loops.hpp"
#include "liveness_analysis.hpp"
#include "backend.hpp"
#include "common.hpp"
#include "pipeline.hpp"
#include <map>
#include <set>
#include <unordered_set>

namespace loops {

/*
TODO(ch): Implement with RISC-V RVV
For RVV support Register pool must be modified to support connected vectors(fractions and splices):
1.) There will be hierarchy of nested vectors: biggest vector can be separated into 1/2, 1/4, 1/8,
1/16, 1/32, 1/64. Connected vectors are allocated in one space and simulteneously: container one \
and nested one.
2.) Since vectors can have different sizes, allocation of nested vector blocks container allocation,
excepts connected vectors.
3.) And vice versa, container vector can be released only after releasing all components.
 */
class RegisterPool
{
public:
    RegisterPool(Backend* m_owner);

    void initRegisterPool();
    size_t freeRegsAmount(int basket_num) const;
    inline bool havefreeRegs(int basket_num) const { return freeRegsAmount(basket_num) > 0; }
    RegIdx provideParamFromPool(int basket_num);  //Must be called first.
    // (There must be provided first registers from parameter vessel, further: return - callerSaved - calleeSaved).
    RegIdx provideRegFromPool(int basket_num, RegIdx a_hint = IReg::NOIDX);
    std::vector<RegIdx> provideConsecutiveRegs(int basket_num, int amount);
    RegIdx provideReturnFromPool(int basket_num); //Must be called last.
    void releaseReg(int basket_num, RegIdx freeReg);

    RegIdx provideSpillPlaceholder(int basket_num);
    void clearSpillPlaceholders(int basket_num);

    inline std::set<RegIdx> usedCallee(int basket_num) const { return m_usedCallee[basket_num]; }

    //Functions for summarizing information about separate register allocations.
    inline void mergeSpillPlaceholders(const RegisterPool& other);
    inline void mergeUsedCallee(const RegisterPool& other);

    void overrideRegisterSet(int basket_num, const std::vector<int>&  a_parameterRegisters,
                                            const std::vector<int>&  a_returnRegisters,
                                            const std::vector<int>&  a_callerSavedRegisters,
                                            const std::vector<int>&  a_calleeSavedRegisters);
        
    std::array<std::vector<int>, RB_AMOUNT> getOverridenParams() const;
private:
    Backend* m_backend;
    // Sometimes register can exist in more than one vessel(like parameter and return), so we have to trace
    // register to be erased from all of them.
    void removeFromAllVessels(int basket_num, int reg);

    enum { PARAMS_VESS = 0, RETURN_VESS = 1, CALLER_VESS = 2, CALLEE_VESS = 3, VESS_AMOUNT = 4, REG_MAX = 64, REG_UNDEF = 255 };
    enum { NOREGISTER = -1, MAXIMUM_SPILLS = 3}; //TODO(ch):need more detailed spill scheme, than just 3 spills.
    // Register pool have internal ordering of registers for supporting correct providing sequence.
    uint8_t m_reorderArch2Inner[RB_AMOUNT][VESS_AMOUNT][REG_MAX];
    uint8_t m_reorderInner2Arch[RB_AMOUNT][VESS_AMOUNT][REG_MAX];
    uint64_t m_vessel[RB_AMOUNT][VESS_AMOUNT];
    uint64_t m_pool[RB_AMOUNT]; // Pool is used for easy registers amount calculation and spill placeholders management.
    uint64_t m_spillPlaceholders[RB_AMOUNT]; //There used outer register ordering
    uint64_t m_spillPlaceholdersAvailable[RB_AMOUNT];
    std::set<RegIdx> m_usedCallee[RB_AMOUNT];
    std::vector<int> m_registersO[RB_AMOUNT][VESS_AMOUNT];
};

/*
TODO(ch): Implement with RISC-V RVV
There can be useful next approach:
Do not choose upper-level multiplier(M1, M2, ..M8) until calculation amount of needed
vector registers(which can be done only on register allocation). When amount is known,
it's simple to find appropriate multiplier, defining vector register set able to fit
this amount with maximal length of vector.
E.g:
M8 provide 4 highest-level registers of hierarchy depth = 6.
M4 provide 8 highest-level registers of hierarchy depth = 5.
M1 provide 32 highest-level registers of hierarchy depth = 3.
MF8 provide 256 highest-level registers of hierarchy depth = 0.
*/

class FuncImpl;
class RegisterAllocator : public CompilerPass
{
public:
    RegisterAllocator(Backend* a_backend, const std::array<std::vector<LiveInterval>, RB_AMOUNT>* a_live_intervals, BasicBlocksTreePtr a_bbt, int a_snippet_caused_spills, bool a_have_function_calls);
    virtual ~RegisterAllocator() override {}
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual bool is_inplace() const override final { return false; } 
    virtual std::string pass_id() const override final { return "CP_REGISTER_ALLOCATION"; }

    inline int epilogueSize() const { return m_epilogueSize; }
    RegisterPool& getRegisterPool() { return m_pool; }
private:
    const std::array<std::vector<LiveInterval>, RB_AMOUNT>* m_liveintervals_raw;
    BasicBlocksTreePtr m_bbt;
    void layOutLiveIntervals(const Syntfunc& a_source, 
                             std::array<std::vector<LiveInterval>, RB_AMOUNT>& parintervals,
                             std::array<std::multiset<LiveInterval, startordering>, RB_AMOUNT>& liveintervals,
                             std::array<std::vector<RegIdx>, RB_AMOUNT>& params_sorted);
    struct RegisterReassignment
    {
        std::vector<int> bounds; 
        std::vector<Arg> args;
        inline Arg getAt(int opnum);
        RegisterReassignment() {}
        RegisterReassignment(int start, int end, const Arg& base_replace) :
            bounds({start, end}), args({base_replace}) {}
    };
    RegisterPool m_pool;
    RegisterPool m_poolBase; //Pool just after allocation parameters, cloned for every block allocation.

    //One allocated subinterval of a register: on [start_pos, end_pos) the register lives in "assignment".
    struct SplitAssignment
    {
        int start_pos;
        int end_pos;
        Arg assignment;
    };
    //Positionally-mixed allocations of liveinterval cuts, made by boundaries of WHILE_ blocks.
    std::array<std::vector<std::vector<SplitAssignment> >, RB_AMOUNT> m_split_assignments;
    
    //We do not consider IF_ nodes as blocks in our hierarhcial allocation approach, because only WHILE_ nodes 
    //affects performance enough. This function erases IF_ nodes from basic blocks tree.
    void removeBranchesFromBBT(BasicBlocksTree& node);

    //Split a live interval by boundaries of WHILE_ blocks. Parameters are not cut(they keep a whole reassignment).
    std::array<std::vector<std::vector<LiveInterval> >, RB_AMOUNT> makeBlockSplits(const std::array<std::multiset<LiveInterval, startordering>, RB_AMOUNT>& liveintervals);

    //Pinned spill slot per register index(see linearScanBlock).
    std::array<std::map<RegIdx, int>, RB_AMOUNT> m_spill_slot_of;
    //Provide place for new spill or get already provided 
    int64_t getSpillSlot(RegIdx idx, int basket_num);

    //Plain linear scan over a single block's splits(one basket): the flat per-block allocation primitive,
    //driven by allocateBlock's recursion. It doesn't consider function parameters. Works with fresh register 
    //pool, negotiatian of different allocation is made after.
    void linearScanBlock(int basket_num, const Syntfunc& a_source,
        const std::multiset<LiveInterval, startordering>& liveintervals,
        std::multiset<LiveInterval, endordering>& active,
        std::vector<RegisterReassignment>& result,
        const std::unordered_map<RegIdx, std::pair<RegIdx, RegIdx> >& unspillableLd2,
        std::unordered_map<RegIdx, RegIdx>& already_allocatedLd2);

    //Recursively allocates registers for a block(loop) of the BasicBlocksTree and its subtree(one basket),
    //inner-loops-first: each child WHILE is scanned on a fresh pool, then this block scans its own intervals(the
    //splits made by boundaries of children. Boundaries between a split and its neighbours are reconciled afterwards 
    //by the per-split transfers in insertSpillInstructions.
    void allocateBlock(const BasicBlocksTree& node, int basket_num, const Syntfunc& a_source,
        const std::vector<std::vector<LiveInterval> >& block_splits,
        std::multiset<LiveInterval, endordering>& active,
        std::vector<RegisterReassignment>& result,
        const std::unordered_map<RegIdx, std::pair<RegIdx, RegIdx> >& unspillableLd2,
        std::unordered_map<RegIdx, RegIdx>& already_allocatedLd2);

    std::array<std::vector<RegisterReassignment>, RB_AMOUNT> assignRegisters(const Syntfunc& a_source,
        const std::array<std::multiset<LiveInterval, startordering>, RB_AMOUNT>& liveintervals,
        const std::array<std::vector<LiveInterval>, RB_AMOUNT>& parintervals);

    struct SpillInfo
    {
        std::array<std::vector<std::map<RegIdx, Arg> >, RB_AMOUNT> unspilledRenaming;
        std::array<std::vector<std::map<RegIdx, Arg> >, RB_AMOUNT> spilledRenaming;
        std::array<std::vector<std::set<int> >, RB_AMOUNT> stackPlaceable;
        int nettoSpills[RB_AMOUNT] = {0, 0};
        size_t basket_offset[RB_AMOUNT]; //Start postions of scalar and vector baskets in stack
        int64_t m_spoffset[RB_AMOUNT] = {0, 0}; 
        int spAddAligned;
    };

    SpillInfo modelSpills(const Syntfunc& a_source);

    inline int64_t getSpillOffset(int basket_num, RegIdx reg, Arg spilled);
    inline int64_t getSpillOffset(int basket_num, int opnum, RegIdx reg);

    struct SplitTransfers
    {
        RegIdx vidx;
        Arg src;
        Arg dst;
        int basket_num;
        bool src_scratch;
    };

    void emitParallelCopy(Syntfunc& a_destination, const std::vector<SplitTransfers>& transfersHere);

    void insertSpillInstructions(const Syntfunc& a_source,
                                 Syntfunc& a_destination);
    void writePrologue(Syntfunc& a_destination);
    void writeEpilogue(Syntfunc& a_destination);

    inline Arg getReassigned(int basket_num, int opnum, int old);

    int m_snippet_caused_spills;
    bool m_have_function_calls;
    int m_epilogueSize;

    //Algorithm constants:
    int m_basketElemX[RB_AMOUNT];
    Arg m_retreg;
    
    //Widely used algorithm variables:
    std::array<std::vector<RegisterReassignment>, RB_AMOUNT> m_reg_reassignment;
    std::array<std::map<RegIdx, int>, RB_AMOUNT> m_stackParamLayout;
    std::array<std::vector<RegIdx>, RB_AMOUNT> m_params_sorted;
    SpillInfo m_spill_info;

    inline bool isParam(int basket_num, int idx);
    inline bool isRegisterPassedParam(int basket_num, int idx);
    inline bool isStackPassedParam(int basket_num, int idx);
    friend void DUBUGprint_allocation(const std::array<std::vector<RegisterReassignment>, RB_AMOUNT>& m_reg_reassignment);
};
}
#endif // __LOOPS_REG_ALLOCATOR_HPP__
