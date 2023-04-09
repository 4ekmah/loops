/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#ifndef __LOOPS_CODE_COLLECTING_HPP__
#define __LOOPS_CODE_COLLECTING_HPP__

#include "loops/loops.hpp"
#include "common.hpp"
#include "backend.hpp"
#include <deque>
#include <map>
#include <set>

namespace loops {

struct ControlFlowBracket
{
    enum { WHILE, IF, ELSE };
    size_t tag;
    size_t label_or_pos; //Used as operation num in regeister allocator, as label id otherwise.
    size_t auxfield; //Used as additional label id or as elif counter.
    ControlFlowBracket(size_t a_tag, size_t a_labelOrPos, size_t auxfield_ = 0) : tag(a_tag), label_or_pos(a_labelOrPos), auxfield(auxfield_) {}
};

class CodeCollecting : public CompilerStage
{
public:
    CodeCollecting(Syntfunc& a_data, Func* a_func);
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual bool is_inplace() const override final { return true; }
    virtual StageID stage_id() const override final;

    inline void newiopNoret(int opcode, ::std::initializer_list<Recipe> args);
    void loadvec_deinterleave2_(Arg& res1, Arg& res2, const Recipe& base);
    
    /*
    TODO(ch): Implement with RISC-V RVV
    //Create register, connected to v1, v2, and contain them as halves. RVV-only.
    template<typename _Tp> VReg<_Tp> spliceVregs(const VReg<_Tp>& v1, const VReg<_Tp>& v2);
    //Create constrained register, which is pnum's part of wh. Divider is degree of two. RVV-only.
    template<typename _Tp> VReg<_Tp> fractionVreg(const VReg<_Tp>& wh, size_t divider, size_t pnum);
    
    These functions will fill next structure, keeping info about interregister connections:
             containerReg           divider              containedReg
    std::map<RegIdx, std::pair<size_t, std::vector<RegIdx> > > m_vregConstraints;
    Divider is degree of two from 2 to 64(mf8 of m8), because it's better to consider constraints
    like relational.
    */
    Arg reg_constr(Recipe& fromwho);
    void reg_assign(const Arg& target, Recipe& fromwho);
    void while_(Recipe& r);
    void endwhile_();
    void break_();
    void continue_();
    void if_(Recipe& r);
    void elif_(Recipe& r);
    void else_();
    void subst_elif(Recipe& r);
    void subst_else();
    void endif_();
    void return_();
    void return_(Recipe& r);
private:
    enum {UR_WRAPIIMM = 1, UR_NONEWIDX = 2};
    Arg unpack_recipe(Recipe& rcp, int flag = 0, Syntfunc* buffer = nullptr);
    Recipe eliminate_not(Recipe& rcp, bool inverseflag = false);
    Syntop unpack_condition_old(Recipe& rcp); //DUBUG:temporary
    void unpack_condition(Recipe& rcp, int true_jmp, int false_jmp);
    void unpack_ifcond_(Syntfunc& condition_buffer, Recipe& rcp, int labeltrue, int labelfalse, bool jmp2correct);
    void reopen_endif(bool cond_prefix_allowed = false);
    Func* m_func;
    std::deque<ControlFlowBracket> m_cflowStack;
    Syntfunc& m_data;
    enum {RT_NOTDEFINED, RT_REGISTER, RT_VOID};
    int m_returnType;
    
};

inline void CodeCollecting::newiopNoret(int opcode, ::std::initializer_list<Recipe> args_)
{
    std::vector<Arg> args;
    args.reserve(args_.size());
    for(Recipe arg : args_) 
    {
        args.push_back(unpack_recipe(arg));
    }
    Syntop toAdd(opcode, args);
    m_data.program.push_back(toAdd);
}
};
#endif // __LOOPS_CODE_COLLECTING_HPP__
