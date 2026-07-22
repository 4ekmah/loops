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
#include "common.hpp"
#include "func_impl.hpp"

/*
Liveness analysis considers all references of virtual register and detect start and end
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
*/

namespace loops
{

class LivenessAnalysisAlgoImpl;
class LivenessAnalysisAlgoImpl : public LivenessAnalysisAlgo
{
public:
    LivenessAnalysisAlgoImpl(const Backend* a_owner);
    virtual ~LivenessAnalysisAlgoImpl() override {}
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual std::array<std::vector<LiveInterval>, RB_AMOUNT>* live_intervals() override final { return &m_liveintervals; }
    virtual int getSnippetCausedSpills() const override final { return m_snippetCausedSpills; }
    virtual BasicBlocksTreePtr getBasicBlocksTree() const { return m_basic_blocks_tree; }
    virtual bool haveFunctionCalls() const override final { return m_haveFunctionCalls; }
private:
    struct SIEvent //Subinterval Event
    {
        RegIdx idx;
        int basket_num;
        SIEvent() : idx(IReg::NOIDX) {}
        SIEvent(RegIdx a_idx, int a_basket_num) : idx(a_idx), basket_num(a_basket_num) {}
    };
    friend struct LAEventIterator;
    std::array<std::vector<std::vector<LiveInterval> >, RB_AMOUNT> m_subintervals; //TODO(ch): std::vector<std::list<LiveInterval> > will avoid moves and allocations.
                                                                                    //but in this case m_subintervalHeaders must be std::vector<std::list<LiveInterval>::iterator>
                                                                                    //Header is number of subinterval in process of iteration over subintervals(keeping every interval in program).
    std::array<std::vector<int>, RB_AMOUNT> m_subintervalHeaders;
    std::array<std::deque<std::map<RegIdx, int> >, RB_AMOUNT> m_active_headers_stack;
    void push_active_state(const std::array<std::multiset<LiveInterval, endordering>, RB_AMOUNT>& a_lastActive, int a_endif);
    void pop_active_state();
    std::map<RegIdx, int>::const_iterator acs_begin(int basket_num) const;
    std::map<RegIdx, int>::const_iterator acs_end(int basket_num) const;
    std::array<std::vector<LiveInterval>, RB_AMOUNT> m_liveintervals;
    int m_snippetCausedSpills;
    BasicBlocksTreePtr m_basic_blocks_tree;
    bool m_haveFunctionCalls;
    inline int regAmount(int basket_num) const { return (int)m_subintervals[basket_num].size(); }
    inline int siAmount(int basket_num, RegIdx regNum) const;
    inline bool defined(int basket_num, RegIdx regNum) const { return siAmount(basket_num, regNum) > 0; }
    inline void def(int basket_num, RegIdx regNum, int opnum, uint64_t priority_scale = 0);
    inline void use(int basket_num, RegIdx regNum, int opnum, uint64_t priority_scale = 0);
    inline void spliceUntilSinum(int basket_num, RegIdx regNum, int siEnd, int siStart = UNDEFINED_OPERATION_NUMBER);
    inline int expandUntilOpnum(int basket_num, RegIdx regNum, int opnum, int siStart);
    inline int deactivationOpnum(int basket_num, RegIdx regNum);
    inline void initSubintervalHeaders(int initval = 0);
    inline int getCurrentSinum(int basket_num, RegIdx regNum);
    inline LiveInterval& getCurrentSubinterval(int basket_num, RegIdx regNum);
    inline LiveInterval& getNextSubinterval(int basket_num, RegIdx regNum);
    inline bool isIterateable(int basket_num, RegIdx regNum) const; //Well, unfotunately, we don't have after-end-state, only last-one state.
    inline void iterateSubinterval(int basket_num, RegIdx regNum);
    inline void moveEventLater(std::multimap<int, SIEvent>& queue, RegIdx regNum, int oldOpnum, int newOpnum);
};

//It is destructive for subint_queue
struct LAEventIterator //Liveness analysis event
{
    enum { LAE_STARTLOOP, LAE_ENDLOOP, LAE_STARTBRANCH, LAE_ENDBRANCH, LAE_SWITCHSUBINT};
    int event_type;
    int opnum;
    int else_pos;
    int idx;
    int basket_num;
    int opposite_nesting_side;
    LAEventIterator(const BasicBlocksTreePtr a_bbt, std::multimap<int, LivenessAnalysisAlgoImpl::SIEvent>& a_subint_queue);
    bool done() const;
    void next();
private:
    struct CFEvent //Control flow Event
    {
        int event_type;
        int else_pos;
        int opposite_nesting_side;
        CFEvent(int a_event_type) : event_type(a_event_type), else_pos(UNDEFINED_OPERATION_NUMBER), opposite_nesting_side(UNDEFINED_OPERATION_NUMBER) {}
    };
    const BasicBlocksTreePtr bbt;
    std::multimap<int, LivenessAnalysisAlgoImpl::SIEvent>& subint_queue;
    std::map<int, CFEvent> bbt_queue;
    std::map<int, CFEvent>::iterator bbt_iterator;
    bool isdone;
};

LAEventIterator::LAEventIterator(const BasicBlocksTreePtr a_bbt, std::multimap<int, LivenessAnalysisAlgoImpl::SIEvent>& a_subint_queue):
    opnum(0)
    , bbt(a_bbt)
    , subint_queue(a_subint_queue)
    , isdone(false)
{
    std::stack<const BasicBlocksTree*> bbt_stack;
    bbt_stack.push(bbt.get());
    std::stack<int> child_idx_stack;
    child_idx_stack.push(0);
    while(bbt_stack.size())
    {
        const BasicBlocksTree* curr_block = bbt_stack.top();
        if(child_idx_stack.top() < (int)curr_block->children.size())
        {
            int child_idx = child_idx_stack.top();
            child_idx_stack.top()++;
            child_idx_stack.push(0);
            bbt_stack.push(curr_block->children[child_idx].get());
            curr_block = bbt_stack.top();
            CFEvent evnt(curr_block->type == BasicBlocksTree::BBT_IF ?
                                                    LAE_STARTBRANCH : 
                                                    LAE_STARTLOOP);
            evnt.opposite_nesting_side = curr_block->end_pos;
            bbt_queue.insert(std::make_pair(curr_block->start_pos, evnt)); 
        }
        else
        {
            if(bbt_stack.size() > 1)
            {
                CFEvent evnt(LAE_ENDLOOP);
                if(curr_block->type == BasicBlocksTree::BBT_IF)
                {
                    evnt = CFEvent(LAE_ENDBRANCH);
                    evnt.else_pos = curr_block->else_pos;
                }
                evnt.opposite_nesting_side = curr_block->start_pos;
                bbt_queue.insert(std::make_pair(curr_block->end_pos, evnt)); 
            }
            bbt_stack.pop();
            child_idx_stack.pop();
        }
    }
    bbt_iterator = bbt_queue.begin();
    next();
}
    
bool LAEventIterator::done() const 
{
    return isdone;
}

void LAEventIterator::next()
{
    const int op_end = bbt->end_pos;
    int subint_next_opnum = op_end;
    int bbt_next_opnum = op_end;
    if(!subint_queue.empty())
        subint_next_opnum = subint_queue.begin()->first;
    if(bbt_iterator != bbt_queue.end())
        bbt_next_opnum = bbt_iterator->first;
    if(subint_next_opnum == op_end && bbt_next_opnum == op_end)
    {
        isdone = true;
    }
    else if(subint_next_opnum < bbt_next_opnum)
    {
        auto subint_queue_iterator = subint_queue.begin();
        opnum = subint_next_opnum;
        event_type = LAE_SWITCHSUBINT;
        idx = subint_queue_iterator->second.idx;
        basket_num = subint_queue_iterator->second.basket_num;
        subint_queue.erase(subint_queue_iterator);
    }
    else // if(bbt_next_opnum < subint_next_opnum)
    {
        opnum = bbt_next_opnum;
        event_type = bbt_iterator->second.event_type;
        else_pos = bbt_iterator->second.else_pos;
        opposite_nesting_side = bbt_iterator->second.opposite_nesting_side;
        bbt_iterator++;
    }
}

void LivenessAnalysisAlgoImpl::process(Syntfunc& a_dest, const Syntfunc& a_source)
{
    m_basic_blocks_tree = std::make_shared<BasicBlocksTree>(BasicBlocksTree::BBT_FUNC, 0);
    m_basic_blocks_tree->end_pos = (int)a_dest.program.size();
    std::stack<BasicBlocksTree*> bbtstack;
    bbtstack.push(m_basic_blocks_tree.get());
    
    //TODO(ch): Introduce inplace passes. 
    LOOPS_ASSERT(&a_dest == &a_source); 
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
        m_subintervals[basket_num].resize(a_source.regAmount[basket_num], std::vector<LiveInterval>());
    
    //This function accomplishes four goals:
    //1.) Separates all register live intervals to small def-use subintervals. There can be a lot of usages in one subinterval, but only one definition.
    //2.) Expand subinteravls, which intersects with loop's body, or branches in some certain cases.
    //3.) Rename subintervals got into new variables.
    //4.) Also, find the biggest number of spilled variables needed for deployment of some instructions into snippets(e.g., DIV on intel).
    
    //IMPORTANT: Think around situation 1-0-1, when register is defined inside of block and redefined in another of same depth.(0-1-0, obviously doesn't matter).
    RegIdx paramsAmount[RB_AMOUNT] = {0, 0};
    int64_t priority_scale = 1;
    { //1.) Calculation of simplest [def-use] subintervals and collect precise info about borders of loops and branches.
        // std::deque<ControlFlowBracket> flowstack;
        for (const Arg& par : a_source.params)
        {
            LOOPS_ASSERT(par.tag == Arg::IREG || par.tag == Arg::VREG);
            int basket_num = (par.tag == Arg::IREG ? RB_INT : RB_VEC);
            def(basket_num, par.idx, 0);
            paramsAmount[basket_num]++;
        }
        int basketElemX[RB_AMOUNT] = {1,1};
        basketElemX[RB_VEC] = m_backend->getVectorRegisterBits() / 64;
        for (int opnum = 0; opnum < (int)a_source.program.size(); opnum++)
        {
            const Syntop& op = a_source.program[opnum];
            int opSnippetSpills = 0;
            for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
                opSnippetSpills += m_backend->spillSpaceNeeded(op, basket_num) * basketElemX[basket_num];
            m_snippetCausedSpills = std::max(m_snippetCausedSpills, opSnippetSpills);
            m_haveFunctionCalls = m_haveFunctionCalls || (op.opcode == OP_CALL_NORET || op.opcode == OP_CALL);
            switch (op.opcode)
            {
            case (OP_IF_CEND):
            {
                LOOPS_ASSERT(op.size() == 0);
                BasicBlocksTreePtr bbttoadd = std::make_shared<BasicBlocksTree>(BasicBlocksTree::BBT_IF, opnum); 
                bbtstack.top()->children.push_back(bbttoadd);
                bbtstack.push(bbttoadd.get());
                continue;
            }
            case (OP_ELSE):
            {
                LOOPS_ASSERT(op.size() == 2 && op.args[0].tag == Arg::IIMMEDIATE && op.args[1].tag == Arg::IIMMEDIATE);
                LOOPS_ASSERT(bbtstack.size() && bbtstack.top()->type == BasicBlocksTree::BBT_IF);
                bbtstack.top()->else_pos = opnum;
                continue;
            }
            case (OP_ENDIF):
            {
                LOOPS_ASSERT(op.size() == 1 && op.args[0].tag == Arg::IIMMEDIATE);
                LOOPS_ASSERT(bbtstack.size() && bbtstack.top()->type == BasicBlocksTree::BBT_IF);
                bbtstack.top()->end_pos = opnum;
                bbtstack.pop();
                continue;
            }
            case (OP_WHILE_CSTART):
            {                
                LOOPS_ASSERT(op.size() == 1 && op.args[0].tag == Arg::IIMMEDIATE);
                priority_scale <<= 2;
                BasicBlocksTreePtr bbttoadd = std::make_shared<BasicBlocksTree>(BasicBlocksTree::BBT_WHILE, opnum); 
                bbtstack.top()->children.push_back(bbttoadd);
                bbtstack.push(bbttoadd.get());
                continue;
            }
            case (OP_ENDWHILE):
            {
                LOOPS_ASSERT(op.size() == 2 && op.args[0].tag == Arg::IIMMEDIATE && op.args[1].tag == Arg::IIMMEDIATE);
                LOOPS_ASSERT(bbtstack.size() && bbtstack.top()->type == BasicBlocksTree::BBT_WHILE);
                bbtstack.top()->end_pos = opnum;
                bbtstack.pop();
                priority_scale >>= 2;
                continue;
            }
            default:
                for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
                {
                    std::set<RegIdx> IORegs = m_backend->getUsedRegisters(op, basket_num, AF_INPUT | AF_OUTPUT);
                    std::set<RegIdx> inRegs = m_backend->getInRegisters(op, basket_num);
                    std::set<RegIdx> outRegs = m_backend->getOutRegisters(op, basket_num);
                    for(auto IO : IORegs)  //Register used in one instruction position as Input and Output simultaneously
                    {                      //must not be redefined. First of all, it's usage, and it must be used
                        inRegs.insert(IO); //via already known name.
                        outRegs.erase(IO);
                    }
                    for (RegIdx inreg : inRegs)
                        use(basket_num, inreg, opnum, priority_scale);
                    for (RegIdx outreg : outRegs)
                        def(basket_num, outreg, opnum, priority_scale);
                }
                break;
            }
        }
        int alignx = 1;
        for(int bex : basketElemX) 
            alignx = std::max(alignx, bex);
        m_snippetCausedSpills += m_snippetCausedSpills % alignx ? alignx - m_snippetCausedSpills % alignx : 0; 
    }
    LOOPS_ASSERT(bbtstack.size() == 1);
    { //2.) Calculating intervals crossing loops and embranchments.
        std::multimap<int, SIEvent> SIqueue;
        initSubintervalHeaders(-1);
        std::array<std::multiset<LiveInterval, endordering>, RB_AMOUNT> lastActive; // NOTE: In this part of code LiveInterval::end means not end position of subinterval, but deactivation position, position, when starts new subinterval or ends final one.
        for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
        {
            for (RegIdx idx = 0; idx < regAmount(basket_num); idx++)
            {
                if (siAmount(basket_num, idx) == 0)
                    continue;
                const int sintStart = m_subintervals[basket_num][idx][0].start;
                int eventPos;
                if (sintStart == 0)
                {
                    iterateSubinterval(basket_num, idx);
                    eventPos = deactivationOpnum(basket_num, idx);
                    LiveInterval toActive = getCurrentSubinterval(basket_num, idx);
                    toActive.end = eventPos;
                    lastActive[basket_num].insert(toActive);
                    deactivationOpnum(basket_num, idx);
                }
                else
                    eventPos = sintStart;
                SIqueue.insert(std::make_pair(eventPos, SIEvent(idx, basket_num)));
            }
        }

        LAEventIterator event(m_basic_blocks_tree, SIqueue);

        while (!event.done())
        {
            int opnum = event.opnum;
            switch (event.event_type)
            {
            case (LAEventIterator::LAE_SWITCHSUBINT):
            {
                std::multiset<LiveInterval, endordering>& b_lastActive = lastActive[event.basket_num];
                if ((getCurrentSinum(event.basket_num, event.idx) + 1) > 0)
                {
                    auto removerator = b_lastActive.begin();
                    while (removerator != b_lastActive.end() && removerator->idx != event.idx && removerator->end == opnum)
                        ++removerator;
                    if (removerator != b_lastActive.end() && removerator->idx == event.idx)
                        b_lastActive.erase(removerator);
                }
                if (isIterateable(event.basket_num, event.idx))
                {
                    SIEvent toAdd;
                    toAdd.basket_num = event.basket_num;
                    toAdd.idx = event.idx;
                    iterateSubinterval(event.basket_num, event.idx);
                    int eventPos = deactivationOpnum(event.basket_num, event.idx);
                    LiveInterval toActive = getCurrentSubinterval(event.basket_num, event.idx);
                    toActive.end = eventPos;
                    b_lastActive.insert(toActive);
                    SIqueue.insert(std::make_pair(eventPos, toAdd));
                }
                break;
            };
            case (LAEventIterator::LAE_STARTBRANCH):
            case (LAEventIterator::LAE_STARTLOOP):
            {
                push_active_state(lastActive, event.opposite_nesting_side);
                break;
            }
            case (LAEventIterator::LAE_ENDBRANCH):
            {
                for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
                {
                    const int ifPos = event.opposite_nesting_side;
                    const int endifPos = opnum;
                    const bool haveElse = (event.else_pos != UNDEFINED_OPERATION_NUMBER);
                    const int else_pos = haveElse ? event.else_pos : endifPos;
                    std::multiset<LiveInterval, endordering> lastActiveChanged;
                    for(auto ifidrator = acs_begin(basket_num); ifidrator != acs_end(basket_num); ifidrator++)
                    {
                        int firstUseMain = UNDEFINED_OPERATION_NUMBER, firstDefMain = UNDEFINED_OPERATION_NUMBER;
                        int firstUseElse = UNDEFINED_OPERATION_NUMBER, firstDefElse = UNDEFINED_OPERATION_NUMBER;
                        const RegIdx idx = ifidrator->first;
                        bool afterlife = m_subintervals[basket_num][idx].back().end > endifPos;
                        if (!haveElse && !afterlife)
                            continue;
                        const int initSinum = ifidrator->second;
                        // In next loop we are finding first redefinition of register and
                        // first usage in each branch (firstUseMain, firstDefMain, firstUseElse, firstDefElse)
                        // Also sinum will be number of last subinterval intersected with embranchment.
                        int sinum = initSinum;
                        for (; sinum < siAmount(basket_num, idx); ++sinum)
                        {
                            const int sistart = m_subintervals[basket_num][idx][sinum].start;
                            const int siend = m_subintervals[basket_num][idx][sinum].end;
                            if (sistart > endifPos)
                            {
                                LOOPS_ASSERT(sinum > 0);
                                sinum--;
                                break;
                            }
                            if (sistart > ifPos)
                            {
                                if (haveElse && sistart > else_pos && firstDefElse == UNDEFINED_OPERATION_NUMBER)
                                    firstDefElse = sinum;
                                else if (firstDefMain == UNDEFINED_OPERATION_NUMBER)
                                    firstDefMain = sinum;
                            }
                            if (siend > endifPos)
                                break;
                            if (siend > ifPos)
                            {
                                if (haveElse && siend > else_pos && firstUseElse == UNDEFINED_OPERATION_NUMBER)
                                    firstUseElse = sinum;
                                else if (firstUseMain == UNDEFINED_OPERATION_NUMBER)
                                    firstUseMain = sinum;
                            }
                        }
                        if (sinum == siAmount(basket_num, idx))
                            sinum = siAmount(basket_num, idx) - 1;

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
                            const int switchIpos = isIterateable(basket_num, idx) ? getNextSubinterval(basket_num, idx).start : getCurrentSubinterval(basket_num, idx).end;
                            spliceUntilSinum(basket_num, idx, sinum, initSinum);
                            if(switchIpos > opnum)
                            {
                                LiveInterval changedOne = m_subintervals[basket_num][idx][initSinum];
                                changedOne.end = deactivationOpnum(basket_num, idx);
                                lastActiveChanged.insert(changedOne);
                                auto removerator = lastActive[basket_num].find(LiveInterval(idx, switchIpos));;
                                while(removerator != lastActive[basket_num].end() && removerator->end == switchIpos && removerator->idx != idx) ++removerator;
                                LOOPS_ASSERT(removerator != lastActive[basket_num].end());
                                lastActive[basket_num].erase(removerator);
                                moveEventLater(SIqueue, idx, switchIpos, changedOne.end);
                            }
                        }
                    }
                    lastActive[basket_num].insert(lastActiveChanged.begin(), lastActiveChanged.end());
                };
                pop_active_state();
                break;
            }
            case (LAEventIterator::LAE_ENDLOOP):
            {
                const int endwhilePos = opnum;
                for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
                {
                    std::multiset<LiveInterval, endordering> lastActiveChanged;
                    for(auto ifidrator = acs_begin(basket_num); ifidrator != acs_end(basket_num); ifidrator++)
                    {
                        const RegIdx idx = ifidrator->first;
                        const int si_start = ifidrator->second;
                        const int switchIpos = isIterateable(basket_num, idx) ? getNextSubinterval(basket_num, idx).start : getCurrentSubinterval(basket_num, idx).end;
                        if(m_subintervals[basket_num][idx][si_start].end < endwhilePos)
                        {
                            expandUntilOpnum(basket_num, idx, endwhilePos, si_start);
                            auto removerator = lastActive[basket_num].find(LiveInterval(idx, switchIpos));
                            while(removerator != lastActive[basket_num].end() && removerator->end == switchIpos && removerator->idx != idx) ++removerator;
                            if(removerator != lastActive[basket_num].end())
                            {
                                lastActive[basket_num].erase(removerator);
                                LiveInterval changedOne = m_subintervals[basket_num][idx][si_start];
                                changedOne.end = deactivationOpnum(basket_num, idx);
                                lastActiveChanged.insert(changedOne);
                                moveEventLater(SIqueue, idx, switchIpos, changedOne.end);
                            }
                        }
                    }
                    lastActive[basket_num].insert(lastActiveChanged.begin(), lastActiveChanged.end());
                }
                pop_active_state();
                break;
            }
            default:
                throw loops::exception("Internal error: unexpected event in branch queue.");
            }
            event.next();
        }
    }

    std::stack<int> child_idx_stack;
    child_idx_stack.push(0);
    size_t resSize[RB_AMOUNT];
    { //3.) Renaming splitted registers.
        initSubintervalHeaders();
        for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++ )
        {
            RegIdx pseudIdx = paramsAmount[basket_num];
            for (RegIdx idx = 0; idx < paramsAmount[basket_num]; idx++)
                for (int si = 1; si < (int)siAmount(basket_num, idx); si++)
                    m_subintervals[basket_num][idx][si].idx = pseudIdx++;
            for (RegIdx idx = paramsAmount[basket_num]; idx < regAmount(basket_num); idx++)
                for (LiveInterval& li : m_subintervals[basket_num][idx])
                    li.idx = pseudIdx++;
            resSize[basket_num] = pseudIdx;
        }

        for (int opnum = 0; opnum < (int)a_dest.program.size(); opnum++)
        {
            //Keeping bbtstack actual
            {
                int child_idx = child_idx_stack.top();
                if(child_idx < (int)bbtstack.top()->children.size())
                {
                    if(bbtstack.top()->children[child_idx]->start_pos == opnum)
                    {
                        bbtstack.push(bbtstack.top()->children[child_idx].get());
                        child_idx_stack.top()++;
                        child_idx_stack.push(0);
                    }
                }
                else if(bbtstack.top()->end_pos == opnum)
                {
                    bbtstack.pop();
                    child_idx_stack.pop();
                }
            }
            Syntop& op = a_dest.program[opnum];
            std::array<std::set<int>, RB_AMOUNT> outRegArnums;
            for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++ )
                outRegArnums[basket_num] = m_backend->getOutRegistersIdxs(op, basket_num);
            for (int arnum = 0; arnum < (int)op.size(); arnum++)
            {
                Arg& arg = op.args[arnum];
                int basket_num = (arg.tag == Arg::IREG ? RB_INT: (arg.tag == Arg::VREG ? RB_VEC : RB_AMOUNT));
                if(basket_num < RB_AMOUNT)
                {
                    if (arg.idx == Syntfunc::RETREG)
                        continue;
                    bool isOut = (outRegArnums[basket_num].count(arnum) > 0);
                    LOOPS_ASSERT(siAmount(basket_num, arg.idx) > 0);
                    while (isIterateable(basket_num, arg.idx))
                    {
                        LiveInterval& li = getCurrentSubinterval(basket_num, arg.idx);
                        if (opnum >= li.start && opnum <= li.end)
                            break;
                        iterateSubinterval(basket_num, arg.idx);
                    }
                    size_t sinum = getCurrentSinum(basket_num, arg.idx);
                    if (isIterateable(basket_num, arg.idx) && isOut && (getNextSubinterval(basket_num, arg.idx).start <= opnum))
                        sinum++;
                    arg.idx = m_subintervals[basket_num][arg.idx][sinum].idx;
                }
            }
        }
    }
    LOOPS_ASSERT(bbtstack.size() == 1);

    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
    {
        m_liveintervals[basket_num] = std::vector<LiveInterval>(resSize[basket_num], LiveInterval(0,0));
        for (auto res : m_subintervals[basket_num])
            for (auto pseud : res)
                m_liveintervals[basket_num][pseud.idx] = pseud;
        a_dest.regAmount[basket_num] = (int)m_liveintervals[basket_num].size();
    }
}

LivenessAnalysisAlgoImpl::LivenessAnalysisAlgoImpl(const Backend* a_owner) : LivenessAnalysisAlgo(a_owner, 0)
    , m_snippetCausedSpills(0)
    , m_haveFunctionCalls(false)
{}
void LivenessAnalysisAlgoImpl::push_active_state(const std::array<std::multiset<LiveInterval, endordering>, RB_AMOUNT>& a_lastActive, int a_endif)
{
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
    {
        m_active_headers_stack[basket_num].push_back(std::map<RegIdx, int>());
        auto filled = m_active_headers_stack[basket_num].rbegin();
        for(auto lastactive: a_lastActive[basket_num])
        {
            if(lastactive.end > a_endif) //Will not consider subintervals contains whole embranchement.
                break;
            RegIdx idx = lastactive.idx;
            filled->insert(std::pair<RegIdx, int>(idx, m_subintervalHeaders[basket_num][idx]));
        }
    }
}

void LivenessAnalysisAlgoImpl::pop_active_state()
{
    for(int basket_num = 0; basket_num < RB_AMOUNT; basket_num++)
    {
        LOOPS_ASSERT(!m_active_headers_stack[basket_num].empty());
        m_active_headers_stack[basket_num].pop_back();
    }
}

std::map<RegIdx, int>::const_iterator LivenessAnalysisAlgoImpl::acs_begin(int basket_num) const
{
    const std::deque<std::map<RegIdx, int> >& b_states = m_active_headers_stack[basket_num];
    LOOPS_ASSERT(!b_states.empty());
    return b_states.back().cbegin();
}

std::map<RegIdx, int>::const_iterator LivenessAnalysisAlgoImpl::acs_end(int basket_num) const
{
    const std::deque<std::map<RegIdx, int> >& b_states = m_active_headers_stack[basket_num];
    LOOPS_ASSERT(!b_states.empty());
    return b_states.back().cend();
}

int LivenessAnalysisAlgoImpl::siAmount(int basket_num, RegIdx regNum) const
{
    LOOPS_ASSERT(regNum!= IReg::NOIDX && regNum < regAmount(basket_num));
    return (int)m_subintervals[basket_num][regNum].size();
}

void LivenessAnalysisAlgoImpl::def(int basket_num, RegIdx regNum, int opnum, uint64_t priority_scale)
{
    if (regNum != Syntfunc::RETREG)
    {
        LOOPS_ASSERT(regNum != IReg::NOIDX && regNum < regAmount(basket_num));
        m_subintervals[basket_num][regNum].push_back(LiveInterval(regNum, opnum));
        m_subintervals[basket_num][regNum].back().priority = priority_scale;
    }
}

void LivenessAnalysisAlgoImpl::use(int basket_num, RegIdx regNum, int opnum, uint64_t priority_scale)
{
    if (regNum != Syntfunc::RETREG)
    {
        if (regNum != IReg::NOIDX && !defined(basket_num, regNum))
            throw loops::exception("Compile error: using uninitialized register");
        m_subintervals[basket_num][regNum].back().end = opnum;
        m_subintervals[basket_num][regNum].back().priority += priority_scale;
    }
}

void LivenessAnalysisAlgoImpl::spliceUntilSinum(int basket_num, RegIdx regNum, int siEnd, int a_siStart)
{
    m_subintervalHeaders[basket_num][regNum] = a_siStart == UNDEFINED_OPERATION_NUMBER ? m_subintervalHeaders[basket_num][regNum] : a_siStart;
    int siStart = m_subintervalHeaders[basket_num][regNum];
    LOOPS_ASSERT(siStart <= siEnd);
    LOOPS_ASSERT(siEnd != UNDEFINED_OPERATION_NUMBER && siEnd < siAmount(basket_num, regNum));
    m_subintervals[basket_num][regNum][siStart].end = m_subintervals[basket_num][regNum][siEnd].end;
    std::vector<LiveInterval>::iterator remove_start = m_subintervals[basket_num][regNum].begin() + siStart + 1;
    std::vector<LiveInterval>::iterator remove_end = m_subintervals[basket_num][regNum].begin() + siEnd + 1;
    for(std::vector<LiveInterval>::iterator sum_iterator = remove_start; sum_iterator < remove_end; sum_iterator++)
        m_subintervals[basket_num][regNum][siStart].priority += sum_iterator->priority;
    m_subintervals[basket_num][regNum].erase(remove_start, remove_end);
}

int LivenessAnalysisAlgoImpl::expandUntilOpnum(int basket_num, RegIdx regNum, int opnum, int a_siStart)
{
    m_subintervalHeaders[basket_num][regNum] = a_siStart == UNDEFINED_OPERATION_NUMBER ? m_subintervalHeaders[basket_num][regNum] : a_siStart;
    int si_start = m_subintervalHeaders[basket_num][regNum];
    int subinterval2erase = si_start + 1;
    for (; subinterval2erase < siAmount(basket_num, regNum); subinterval2erase++)
        if (m_subintervals[basket_num][regNum][subinterval2erase].start >= opnum)
        {
            subinterval2erase--;
            break;
        }
        else
            opnum = std::max(m_subintervals[basket_num][regNum][subinterval2erase].end, opnum);
    subinterval2erase = std::min(subinterval2erase, siAmount(basket_num, regNum) - 1);
    spliceUntilSinum(basket_num, regNum, subinterval2erase);
    m_subintervals[basket_num][regNum][si_start].end = opnum;
    return opnum;
}

int LivenessAnalysisAlgoImpl::deactivationOpnum(int basket_num, RegIdx regNum)
{
    int sinum = m_subintervalHeaders[basket_num][regNum];
    return (sinum + 1 < siAmount(basket_num, regNum)) ? m_subintervals[basket_num][regNum][sinum + 1].start : m_subintervals[basket_num][regNum][sinum].end;
}

void LivenessAnalysisAlgoImpl::initSubintervalHeaders(int initval)
{
    for(size_t sibNum = 0; sibNum < RB_AMOUNT; sibNum++)
    {
        m_subintervalHeaders[sibNum].clear();
        m_subintervalHeaders[sibNum].resize(m_subintervals[sibNum].size(), initval);
    }
}

int LivenessAnalysisAlgoImpl::getCurrentSinum(int basket_num, RegIdx regNum) //return -> int
{
    return m_subintervalHeaders[basket_num][regNum];
}

LiveInterval& LivenessAnalysisAlgoImpl::getCurrentSubinterval(int basket_num, RegIdx regNum)
{
    LOOPS_ASSERT(m_subintervalHeaders[basket_num][regNum] != UNDEFINED_OPERATION_NUMBER && m_subintervalHeaders[basket_num][regNum] < siAmount(basket_num, regNum));
    return m_subintervals[basket_num][regNum][m_subintervalHeaders[basket_num][regNum]];
}

LiveInterval& LivenessAnalysisAlgoImpl::getNextSubinterval(int basket_num, RegIdx regNum)
{
    LOOPS_ASSERT(m_subintervalHeaders[basket_num][regNum] != UNDEFINED_OPERATION_NUMBER && m_subintervalHeaders[basket_num][regNum] + 1 < siAmount(basket_num, regNum));
    return m_subintervals[basket_num][regNum][m_subintervalHeaders[basket_num][regNum] + 1];
}

bool LivenessAnalysisAlgoImpl::isIterateable(int basket_num, RegIdx regNum) const
{
    return (m_subintervalHeaders[basket_num][regNum] + 1) < siAmount(basket_num, regNum);
}

void LivenessAnalysisAlgoImpl::iterateSubinterval(int basket_num, RegIdx regNum)
{
    if (isIterateable(basket_num, regNum))
        m_subintervalHeaders[basket_num][regNum]++;
}

void LivenessAnalysisAlgoImpl::moveEventLater(std::multimap<int, SIEvent>& queue, RegIdx regNum, int oldOpnum, int newOpnum)
{
    auto qremrator = queue.find(oldOpnum);
    while (qremrator != queue.end() && qremrator->first == oldOpnum)
        if (qremrator->second.idx == regNum)
            break;
        else
            qremrator++;
    LOOPS_ASSERT(qremrator != queue.end() && qremrator->first == oldOpnum);
    SIEvent toRead = qremrator->second;
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

std::array<std::vector<LiveInterval>, RB_AMOUNT>* LivenessAnalysisAlgo::live_intervals()
{
    return impl->live_intervals();
}

int LivenessAnalysisAlgo::getSnippetCausedSpills() const
{
    return impl->getSnippetCausedSpills();
}

BasicBlocksTreePtr LivenessAnalysisAlgo::getBasicBlocksTree() const
{
    return impl->getBasicBlocksTree();
}

bool LivenessAnalysisAlgo::haveFunctionCalls() const
{
    return impl->haveFunctionCalls();
}

LivenessAnalysisAlgo::LivenessAnalysisAlgo(const Backend* a_owner, int): CompilerPass(a_owner), impl(nullptr) {}
}
