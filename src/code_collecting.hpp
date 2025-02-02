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
    int label_or_pos; //Used as operation num in register allocator, as label id otherwise.
    int auxfield;     //Used as additional label id or as elif counter.
    ControlFlowBracket(size_t a_tag, int a_labelOrPos, int auxfield_ = 0) : tag(a_tag), label_or_pos(a_labelOrPos), auxfield(auxfield_) {}
};

class CodeCollecting : public CompilerPass
{
public:
    CodeCollecting(Syntfunc& a_data, Func* a_func);
    virtual void process(Syntfunc& a_dest, const Syntfunc& a_source) override final;
    virtual bool is_inplace() const override final { return true; }
    virtual std::string pass_id() const override final;

    inline void newiopNoret(int opcode, ::std::initializer_list<Expr> args);
    void loadvec_deinterleave2_(Arg& res1, Arg& res2, const Expr& base);
    
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
    Arg reg_constr(Expr& fromwho);
    void reg_assign(Expr& target, Expr& fromwho);
    void while_(Expr& r);
    void endwhile_();
    void break_(int depth);
    void continue_(int depth);
    void if_(Expr& r);
    void elif_(Expr& r);
    void else_();
    void subst_elif(Expr& r);
    void subst_else();
    void endif_();
    void return_();
    void return_(Expr& r);
private:
    enum {UR_WRAPIIMM = 1, UR_NONEWIDX = 2, UR_LNOT_ELIMINATED = 4};
    Arg unpack_expr(Expr& expr, int flag = 0, Syntfunc* buffer = nullptr);
    Expr eliminate_not(Expr& expr, bool inverseflag = false);
    void unpack_condition(Expr& expr, int true_jmp, int false_jmp);
    enum {UC_CORRECT_PREFFERED = 1};
    void unpack_condition_(Syntfunc& condition_buffer, Expr& expr, int labeltrue, int labelfalse, int flags = 0);
    void reopen_endif(bool cond_prefix_allowed = false);
    std::deque<ControlFlowBracket> m_cflowStack;
    Syntfunc& m_data;
    enum {RT_NOTDEFINED, RT_REGISTER, RT_VOID};
    int m_returnType;
    
};

inline void CodeCollecting::newiopNoret(int opcode, ::std::initializer_list<Expr> args_)
{
    std::vector<Arg> args;
    args.reserve(args_.size());
    for(Expr arg : args_) 
    {
        args.push_back(unpack_expr(arg));
    }
    Syntop toAdd(opcode, args);
    m_data.program.push_back(toAdd);
}
}
#endif // __LOOPS_CODE_COLLECTING_HPP__
