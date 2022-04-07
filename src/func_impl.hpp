/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_FUNC_IMPL_HPP__
#define __LOOPS_FUNC_IMPL_HPP__

#include "loops/loops.hpp"
#include "common.hpp"
#include <stack>
#include <map>
#include <set>

namespace loops {

class ContextImpl;
class FuncImpl : public Func
{
public:
    FuncImpl(const std::string& name, Context* ctx, std::initializer_list<IReg*> params);
    static Func makeWrapper(const std::string& name, Context* ctx, std::initializer_list<IReg*> params);

    void call(std::initializer_list<int64_t> args) const;
    void* ptr();

    void printBytecode(std::ostream& out) const;
    void printAssembly(std::ostream& out, int columns) const;

    size_t m_refcount; //TODO: I must check if refcounting and impl logic is threadsafe.
    inline size_t provideIdx() { return m_nextIdx++; }
    size_t provideLabel();
    static const size_t EMPTYLABEL;

    inline IReg newiop(int opcode, ::std::initializer_list<Arg> args);
    inline IReg newiop(int opcode, int depth, ::std::initializer_list<Arg> args);
    inline IReg newiop(int opcode, ::std::initializer_list<Arg> args, IRegInternal retreg);
    inline void newiopNoret(int opcode, ::std::initializer_list<Arg> args);
    inline void newiopNoret(int opcode, int depth, std::initializer_list<Arg> args);
    static FuncImpl* verifyArgs(std::initializer_list<Arg> args);

    void endfunc();

    IReg const_(int64_t value);

    void do_();
    //void doif_(const IReg& r);
    void while_(const IReg& r);
    //void break_();
    //void continue_();
    void if_(const IReg& r);
    //void elif_(const IReg& r);
    //void else_();
    void endif_();
    void return_(const IReg& retval);
    void return_();

    const Syntfunc& getData() const { return m_data; }
    
    int m_cmpopcode; // TODO(ch): IMPORTANT delete this trivial workaround ASAP;
private:
    struct cflowbracket
    {
        enum { DO, DOIF, IF, ELSE };
        size_t tag;
        size_t labelOrPos;
        cflowbracket(size_t a_tag, size_t a_labelOrPos) : tag(a_tag), labelOrPos(a_labelOrPos) {}
    };
    std::stack<cflowbracket> m_cflowStack;
    Syntfunc m_data;
    ContextImpl* m_context;
    size_t m_nextIdx;
    size_t m_nextLabelIdx;
    size_t m_epilogueSize;

    enum {RT_NOTDEFINED, RT_REGISTER, RT_VOID};
    int m_returnType;

    void allocateRegisters();
    void jumpificate();
    std::map<IRegInternal, std::pair<size_t, size_t> > livenessAnalysis();
};

inline IReg FuncImpl::newiop(int opcode, ::std::initializer_list<Arg> args)
{
    size_t retidx = provideIdx();
    m_data.program.emplace_back(opcode, std::initializer_list<Arg>({iregHid(retidx, this)}), args);
    return iregHid(retidx, this);
}

inline IReg FuncImpl::newiop(int opcode, int depth, ::std::initializer_list<Arg> args)
{
    size_t retidx = provideIdx();
    m_data.program.emplace_back(opcode, std::initializer_list<Arg>({iregHid(retidx, this), depth}), args);
    return iregHid(retidx, this);
}

inline IReg FuncImpl::newiop(int opcode, ::std::initializer_list<Arg> args, IRegInternal retreg)
{
    m_data.program.emplace_back(opcode, std::initializer_list<Arg>({argIReg(retreg, this)}), args);
    return iregHid(retreg, this);
}

inline void FuncImpl::newiopNoret(int opcode, ::std::initializer_list<Arg> args)
{
    m_data.program.emplace_back(opcode, args);
}

inline void FuncImpl::newiopNoret(int opcode, int depth, std::initializer_list<Arg> args)
{
    m_data.program.emplace_back(opcode, std::initializer_list<Arg>({Arg(depth)}), args);
}

inline FuncImpl* getImpl(Func* wrapper)
{
    if (!wrapper)
        throw std::string("Null context pointer.");
    return static_cast<FuncImpl*>(_getImpl(wrapper));
}

};
#endif // __LOOPS_FUNC_IMPL_HPP__
