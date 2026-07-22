/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_LIVENESS_ANALYSIS_HPP__
#define __LOOPS_LIVENESS_ANALYSIS_HPP__

#include "loops/loops.hpp"
#include "backend.hpp"
#include "common.hpp"
#include "pipeline.hpp"
#include <map>
#include <set>
#include <unordered_set>

namespace loops {
/*
TODO(ch): Implement with RISC-V RVV
There will be needed modification for support of connected(nested) vectors:
Nested vector register must be redefined on redefinition of container vector,
but only if this nested register will be used after this redefinition.
*/
struct LiveInterval
{
    int start, end;
    RegIdx idx;
    //Priority is measured like sum of occurrences of usage, multilplied by 4^k, where k is hierarchical depth of loop.
    uint64_t priority; //DUBUG: we need here saturation sums, if it max(uint64_t), let it be unchangeable.
    //Sometimes LiveInterval struct is used as split(part of big interval). In this situation it's important to 
    //distinguish last split.
    bool is_last_split;
    LiveInterval(RegIdx a_idx, int a_start) : start(a_start), end(a_start), idx(a_idx), priority(0), is_last_split(true) {}
};

struct startordering
{
    bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.start < b.start; }
};

struct endordering
{
    bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.end < b.end; }
};

struct BasicBlocksTree
{
    std::vector<std::shared_ptr<BasicBlocksTree> > children;
    enum {BBT_IF, BBT_WHILE, BBT_FUNC};
    int type;
    int start_pos;
    int end_pos;
    int else_pos;
    BasicBlocksTree() {}
    BasicBlocksTree(int a_type, int a_start_pos) : type(a_type), start_pos(a_start_pos) {}
};

typedef std::shared_ptr<BasicBlocksTree> BasicBlocksTreePtr;
class LivenessAnalysisAlgo : public CompilerPass
{
public:
    LivenessAnalysisAlgo(const Backend* a_owner);
    virtual ~LivenessAnalysisAlgo();
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override;
        virtual bool is_inplace() const override final { return true; }
        virtual std::string pass_id() const override final { return "CP_LIVENESS_ANALYSIS"; }

    virtual std::array<std::vector<LiveInterval>, RB_AMOUNT>* live_intervals();
    virtual int getSnippetCausedSpills() const;
    virtual BasicBlocksTreePtr getBasicBlocksTree() const;
    virtual bool haveFunctionCalls() const;
protected:
    LivenessAnalysisAlgo(const Backend* a_owner, int);
private:
    LivenessAnalysisAlgo* impl;
};
}
#endif // __LOOPS_LIVENESS_ANALYSIS_HPP__
