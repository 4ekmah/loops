/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
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
    enum { DO, DOIF, IF, ELSE };
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
    FuncImpl(const std::string& name, Context* ctx, std::initializer_list<IReg*> params);
    static Func makeWrapper(const std::string& name, Context* ctx, std::initializer_list<IReg*> params);

    void call(std::initializer_list<int64_t> args) const;
    void* ptr();
    void setCompiledPtr(void* ptr) {m_compiled = ptr;}  //TODO(ch): I don't like this scheme. it's better to separate "compile" stage to "compile2buf" "writeBuf2exe"

    void printBytecode(std::ostream& out) const;
    void printAssembly(std::ostream& out, int columns) const;
    std::string name() const {return m_data.name;}

    size_t m_refcount; //TODO: I must check if refcounting and impl logic is threadsafe.
    inline IRegInternal provideIdx() { return m_nextIdx++; }
    size_t provideLabel();
    enum {NOLABEL = -1};

    inline IReg newiop(int opcode, ::std::initializer_list<Arg> args, uint64_t tryImmMask = 0);
    inline IReg newiop(int opcode, int depth, ::std::initializer_list<Arg> args, uint64_t tryImmMask = 0);
    inline IReg newiopPreret(int opcode, ::std::initializer_list<Arg> args, IRegInternal retreg, uint64_t tryImmMask = 0);
    inline void newiopNoret(int opcode, ::std::initializer_list<Arg> args, uint64_t tryImmMask = 0);
    inline void newiopNoret(int opcode, int depth, std::initializer_list<Arg> args, uint64_t tryImmMask = 0);
    static FuncImpl* verifyArgs(std::initializer_list<Arg> args);

    void endfunc();

    IReg const_(int64_t value);

    void do_();
    void while_(const IReg& r);
    void doif_(const IReg& r);
    void enddo_();
    void break_();
    void continue_();
    void if_(const IReg& r);
    void elif_(const IReg& r);
    void else_();
    void endif_();
    void return_();
    void return_(int64_t retval);
    void return_(const IReg& retval);


    const Syntfunc& getData() const { return m_data; }
    
    int m_cmpopcode; // TODO(ch): IMPORTANT(CMPLCOND) delete this trivial workaround ASAP;
private:
    std::deque<ControlFlowBracket> m_cflowStack;
    Syntfunc m_data;
    ContextImpl* m_context;
    int m_nextIdx;
    size_t m_nextLabelIdx;

    enum {RT_NOTDEFINED, RT_REGISTER, RT_VOID};
    int m_returnType;

    void controlBlocks2Jumps();
    int invertCondition(int condition) const;
    void printSyntopBC(const Syntop& op) const; //Debug purposes only

    void immediateImplantationAttempt(Syntop& op, uint64_t tryImmMask, size_t anumAdd);
    
    void* m_compiled;
};

inline IReg FuncImpl::newiop(int opcode, ::std::initializer_list<Arg> args, uint64_t tryImmMask)
{
    IRegInternal retidx = provideIdx();
    Syntop toAdd(opcode, std::initializer_list<Arg>({ iregHid(retidx, this) }), args);
    if (tryImmMask) immediateImplantationAttempt(toAdd, tryImmMask, 1);
    m_data.program.emplace_back(toAdd);
    return iregHid(retidx, this);
}

inline IReg FuncImpl::newiop(int opcode, int depth, ::std::initializer_list<Arg> args, uint64_t tryImmMask)
{
    IRegInternal retidx = provideIdx();
    Syntop toAdd(opcode, std::initializer_list<Arg>({ iregHid(retidx, this), depth }), args);
    if (tryImmMask) immediateImplantationAttempt(toAdd, tryImmMask, 2);
    m_data.program.emplace_back(toAdd);
    return iregHid(retidx, this);
}

inline IReg FuncImpl::newiopPreret(int opcode, ::std::initializer_list<Arg> args, IRegInternal retreg, uint64_t tryImmMask)
{
    Syntop toAdd(opcode, std::initializer_list<Arg>({ argIReg(retreg, this) }), args);
    if (tryImmMask) immediateImplantationAttempt(toAdd, tryImmMask, 1);
    m_data.program.emplace_back(toAdd);
    return iregHid(retreg, this);
}

inline void FuncImpl::newiopNoret(int opcode, ::std::initializer_list<Arg> args, uint64_t tryImmMask)
{
    Syntop toAdd(opcode, args);
    if (tryImmMask) immediateImplantationAttempt(toAdd, tryImmMask, 0);
    m_data.program.emplace_back(toAdd);
}

inline void FuncImpl::newiopNoret(int opcode, int depth, std::initializer_list<Arg> args, uint64_t tryImmMask)
{
    Syntop toAdd(opcode, std::initializer_list<Arg>({ Arg(depth) }), args);
    if (tryImmMask) immediateImplantationAttempt(toAdd, tryImmMask, 1);
    m_data.program.emplace_back(toAdd);
}

inline FuncImpl* getImpl(Func* wrapper)
{
    if (!wrapper)
        throw std::runtime_error("Null context pointer.");
    return static_cast<FuncImpl*>(_getImpl(wrapper));
}

};
#endif // __LOOPS_FUNC_IMPL_HPP__
