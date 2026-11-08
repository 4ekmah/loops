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

namespace loops {

struct startordering
{
    bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.start < b.start; }
};

struct endordering
{
    bool operator() (const LiveInterval& a, const LiveInterval& b) const { return a.end < b.end; }
};

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
