/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_FUNC_IMPL_HPP__
#define __LOOPS_FUNC_IMPL_HPP__

#include "loops/loops.hpp"
#include "common.hpp"
#include <deque>
#include <map>
#include <set>

namespace loops {

class ContextImpl;

struct ControlFlowBracket
{
    enum { WHILE, IF, ELSE };
    size_t tag;
    size_t labelOrPos;
    size_t elifRepeats;
    std::vector<size_t> breaks;
    std::vector<size_t> continues;
    ControlFlowBracket(size_t a_tag, size_t a_labelOrPos, size_t a_elifRep = 0) : tag(a_tag), labelOrPos(a_labelOrPos), elifRepeats(a_elifRep) {}
};

class FuncImpl : public Func
{
public:
    FuncImpl(const std::string& name, ContextImpl* ctx, std::initializer_list<IReg*> params);
    static Func makeWrapper(const std::string& name, ContextImpl* ctx, std::initializer_list<IReg*> params);

    void call(std::initializer_list<int64_t> args) const;
    void* ptr();
    void applySyntopStages();
    //directTranslation == true avoids most part of stages, like register allocation or controlBlocks2Jumps.
    //It's assumed, that code is already written in manner of being projected to target architecture. It's used for tests only(even for listing-only tests).
    inline void setDirectTranslation(bool directTranslation) { m_directTranslation = directTranslation; }
    void overrideRegisterSet(int basketNum, const std::vector<size_t>&  a_parameterRegisters,
                                            const std::vector<size_t>&  a_returnRegisters,
                                            const std::vector<size_t>&  a_callerSavedRegisters,
                                            const std::vector<size_t>&  a_calleeSavedRegisters);
    
    void setCompiledPtr(void* ptr) {m_compiled = ptr;}  //TODO(ch): I don't like this scheme. it's better to separate "compile" stage to "compile2buf" "writeBuf2exe"
                                                        //Generally, we have to implement FuncImpl as a pipeline, where each creating output ofeach stage deletes input(doesn't keep it).
    void printBytecode(std::ostream& out);
    void printAssembly(std::ostream& out, int columns);
    std::string name() const {return m_data.name;}

    size_t m_refcount; //TODO: I must check if refcounting and impl logic is threadsafe.
    inline RegIdx provideIdx(int basketNum) { return m_data.provideIdx(basketNum); }
    size_t provideLabel();
    enum {NOLABEL = -1};

    inline Context GetContext() { return m_context->getOwner(); }

    inline IReg newiop(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
    inline IReg newiop(int opcode, int depth, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
    inline IReg newiopPreret(int opcode, ::std::initializer_list<Arg> args, RegIdx retreg, ::std::initializer_list<size_t> tryImmList = {});
    inline void newiopNoret(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
    inline std::vector<int> newiopNoret_initregs(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> regsn_to_init);
    inline void newiopNoret(int opcode, int depth, std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
    static FuncImpl* verifyArgs(std::initializer_list<Arg> args);
    template<typename _Tp>
    inline VReg<_Tp> newiopV(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});

    IReg const_(int64_t value);
    
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

    void while_(const IReg& r);
    void endwhile_();
    void break_();
    void continue_();
    void if_(const IReg& r);
    void elif_(const IReg& r);
    void else_();
    void subst_elif(const IReg& r);
    void subst_else();
    void endif_();
    void return_();
    void return_(int64_t retval);
    void return_(const IReg& retval);

    void markConditionStart();

    IReg select(const IReg& cond, const IReg& truev, const IReg& falsev);
    inline IReg select(const IReg& cond, int64_t truev, const IReg& falsev)
    {
        return select(cond, const_(truev), falsev);
    }
    inline IReg select(const IReg& cond, const IReg& truev, int64_t falsev)
    {
        return select(cond, truev, const_(falsev));
    }

    const Syntfunc& getData() const { return m_data; }
    
    int m_cmpopcode; // TODO(ch): IMPORTANT(CMPLCOND) delete this trivial workaround ASAP;
private:
    int m_conditionStart;
    bool m_substConditionBypass;
    std::deque<ControlFlowBracket> m_cflowStack;
    Syntfunc m_data;
    std::unordered_map<size_t, std::pair<size_t, size_t> > m_ifLabelMap; //[label]=(ifpos, elifrep)
    ContextImpl* m_context;
    size_t m_nextLabelIdx;
    bool m_directTranslation;
    
    std::vector<size_t> m_parameterRegistersO[RB_AMOUNT];
    std::vector<size_t> m_returnRegistersO[RB_AMOUNT];
    std::vector<size_t> m_callerSavedRegistersO[RB_AMOUNT];
    std::vector<size_t> m_calleeSavedRegistersO[RB_AMOUNT];

    enum {RT_NOTDEFINED, RT_REGISTER, RT_VOID};
    int m_returnType;
    bool m_syntopStagesApplied;

    int condition2jumptype(int cond);

    void controlBlocks2Jumps();
    void printSyntopBC(const Syntop& op) const; //Debug purposes only

    void immediateImplantationAttempt(Syntop& op, size_t anumAdd, ::std::initializer_list<size_t> tryImmList);
    
    void* m_compiled;
};

inline IReg FuncImpl::newiop(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{
    RegIdx retidx = provideIdx(RB_INT);
    Syntop toAdd(opcode, std::initializer_list<Arg>({ iregHid(retidx, this) }), args);
    if (tryImmList.size()) immediateImplantationAttempt(toAdd, 1, tryImmList);
    m_data.program.emplace_back(toAdd);
    return iregHid(retidx, this);
}

inline IReg FuncImpl::newiop(int opcode, int depth, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{
    RegIdx retidx = provideIdx(RB_INT);
    Syntop toAdd(opcode, std::initializer_list<Arg>({ iregHid(retidx, this), depth }), args);
    if (tryImmList.size()) immediateImplantationAttempt(toAdd, 2, tryImmList);
    m_data.program.emplace_back(toAdd);
    return iregHid(retidx, this);
}

inline IReg FuncImpl::newiopPreret(int opcode, ::std::initializer_list<Arg> args, RegIdx retreg, ::std::initializer_list<size_t> tryImmList)
{
    Syntop toAdd(opcode, std::initializer_list<Arg>({ argReg(RB_INT, retreg, this) }), args);
    if (tryImmList.size()) immediateImplantationAttempt(toAdd, 1, tryImmList);
    m_data.program.emplace_back(toAdd);
    return iregHid(retreg, this);
}

inline void FuncImpl::newiopNoret(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{
    Syntop toAdd(opcode, args);
    if (tryImmList.size()) immediateImplantationAttempt(toAdd, 0, tryImmList);
    m_data.program.emplace_back(toAdd);
}

inline std::vector<int> FuncImpl::newiopNoret_initregs(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> regsn_to_init)
{
    std::vector<int> res;
    res.reserve(regsn_to_init.size());
    std::vector<Arg> args_vec = args;
    for(size_t argn : regsn_to_init)
    {
        Assert(argn<args_vec.size() && argn >=0 && (args_vec[argn].tag == Arg::IREG || args_vec[argn].tag == Arg::VREG));
        RegIdx aidx = provideIdx((args_vec[argn].tag == Arg::IREG) ? RB_INT : RB_VEC);
        args_vec[argn].idx = aidx;
        res.push_back(aidx);
    }
    Syntop toAdd(opcode, args_vec);
    m_data.program.emplace_back(toAdd);
    return res;
}

inline void FuncImpl::newiopNoret(int opcode, int depth, std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{
    Syntop toAdd(opcode, std::initializer_list<Arg>({ Arg(depth) }), args);
    if (tryImmList.size()) immediateImplantationAttempt(toAdd, 1, tryImmList);
    m_data.program.emplace_back(toAdd);
}

template<typename _Tp>
VReg<_Tp> FuncImpl::newiopV(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{
    RegIdx retidx = provideIdx(RB_VEC);
    Syntop toAdd(opcode, std::initializer_list<Arg>({ vregHid<_Tp>(retidx, this) }), args);
    if (tryImmList.size()) immediateImplantationAttempt(toAdd, 1, tryImmList);
    m_data.program.emplace_back(toAdd);
    return vregHid<_Tp>(retidx, this);
}

inline FuncImpl* getImpl(Func* wrapper)
{
    if (!wrapper)
        throw std::runtime_error("Null context pointer.");
    return static_cast<FuncImpl*>(_getImpl(wrapper));
}

};
#endif // __LOOPS_FUNC_IMPL_HPP__
