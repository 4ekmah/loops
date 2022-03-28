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

namespace loops {

class ContextImpl;
class FuncImpl : public Func
{
public:
    FuncImpl(const std::string& name, Context* ctx, std::initializer_list<IReg*> params);
    static Func makeWrapper(const std::string& name, Context* ctx, std::initializer_list<IReg*> params);

    std::string name() const { return m_name; }; //TODO(ch): what for we need name here?
    void call(std::initializer_list<int64_t> args) const;
    void* ptr();

    void printBytecode(std::ostream& out) const;
    enum { PC_OPNUM = 1 , PC_OP = 2, PC_HEX = 4 };
    void printAssembly(std::ostream& out, int colums = PC_OPNUM | PC_OP) const;//TODO(ch): Implement columns!
    void printTargetHex(std::ostream& out) const;

    size_t m_refcount; //TODO: I must check if refcounting and impl logic is threadsafe.
    inline size_t provideIdx() { return m_nextIdx++; }
    size_t appendLabel();
    static const size_t EMPTYLABEL;

    inline IReg newiop(int opcode, ::std::initializer_list<Arg> args);
    inline IReg newiop(int opcode, int depth, ::std::initializer_list<Arg> args);
    inline IReg newiop(int opcode, ::std::initializer_list<Arg> args, IRegInternal retreg);
    inline void newiopNoret(int opcode, ::std::initializer_list<Arg> args);
    inline void newiopNoret(int opcode, int depth, std::initializer_list<Arg> args);

    void endfunc();

    IReg const_(int64_t value);

    void do_();
    //void do_if_(const IReg& r);
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
private:
    void printFunctionHeader(std::ostream& out) const;

    struct cflowbracket
    {
        enum { DO, DOIF, IF, ELSE };
        size_t tag;
        size_t labelOrPos;
        cflowbracket(size_t a_tag, size_t a_labelOrPos) : tag(a_tag), labelOrPos(a_labelOrPos) {}
    };
    std::stack<cflowbracket> m_cflowStack;
    Syntfunc m_data;
    std::vector<size_t> m_returnPositions;
    std::string m_name;
    ContextImpl* m_context;
    size_t m_nextIdx;
    size_t m_nextLabelIdx;
};

IReg iregHiddenConstructor(Func* a_func, size_t a_idx); //Don't use this function directly!

inline IReg FuncImpl::newiop(int opcode, ::std::initializer_list<Arg> args)
{
    size_t retidx = provideIdx();
    m_data.m_program.emplace_back(opcode, std::initializer_list<Arg>({iregHiddenConstructor(this, retidx)}), args);
    return iregHiddenConstructor(this, retidx);
}

inline IReg FuncImpl::newiop(int opcode, int depth, ::std::initializer_list<Arg> args)
{
    size_t retidx = provideIdx();
    m_data.m_program.emplace_back(opcode, std::initializer_list<Arg>({iregHiddenConstructor(this, retidx), depth}), args);
    return iregHiddenConstructor(this, retidx);
}

inline IReg FuncImpl::newiop(int opcode, ::std::initializer_list<Arg> args, IRegInternal retreg)
{
    Arg retarg;
    retarg.tag = Arg::IREG;
    retarg.idx = retreg;
    m_data.m_program.emplace_back(opcode, std::initializer_list<Arg>({retarg}), args);
    return iregHiddenConstructor(this, retreg);
}

inline void FuncImpl::newiopNoret(int opcode, ::std::initializer_list<Arg> args)
{
    m_data.m_program.emplace_back(opcode, args);
}

inline void FuncImpl::newiopNoret(int opcode, int depth, std::initializer_list<Arg> args)
{
    m_data.m_program.emplace_back(opcode, std::initializer_list<Arg>({Arg(depth)}), args);
}

inline FuncImpl* getImpl(Func* wrapper)
{
    if (!wrapper)
        throw std::string("Null context pointer.");
    return static_cast<FuncImpl*>(_getImpl(wrapper));
}

};
#endif // __LOOPS_FUNC_IMPL_HPP__
