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

    void print_bytecode(std::ostream& out) const;
    void print_target_mnemonics(std::ostream& out) const;
    void print_target_hex(std::ostream& out) const;

    size_t refcount; //TODO: I must check if refcounting and impl logic is threadsafe.
    inline size_t provide_idx() { return m_nextidx++; }
    size_t append_label();
    static const size_t EMPTYLABEL;

    inline IReg newiop(int opcode, ::std::initializer_list<Arg> args);
    inline IReg newiop(int opcode, int depth, ::std::initializer_list<Arg> args);
    inline IReg newiop(int opcode, ::std::initializer_list<Arg> args, IRegInternal retreg);
    inline void newiop_noret(int opcode, ::std::initializer_list<Arg> args);
    inline void newiop_noret(int opcode, int depth, std::initializer_list<Arg> args);

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

    const syntfunc& getData() const { return m_data; }
private:
    void print_function_header(std::ostream& out) const;

    struct cflowbracket
    {
        enum { DO, DOIF, IF, ELSE };
        size_t m_tag;
        size_t m_label_or_pos;
        cflowbracket(size_t a_tag, size_t a_label_or_pos) : m_tag(a_tag), m_label_or_pos(a_label_or_pos) {}
    };
    std::stack<cflowbracket> m_cflowstack;
    syntfunc m_data;
    std::vector<size_t> m_return_positions;
    std::string m_name;
    ContextImpl* m_context;
    size_t m_nextidx;
    size_t m_nextlabelidx;

    inline std::vector<Arg> depthed_args(int depth, std::initializer_list<Arg> args);
};

IReg ireg_hidden_constructor(Func* a_func, size_t a_idx); //Don't use this function directly!

inline IReg FuncImpl::newiop(int opcode, ::std::initializer_list<Arg> args)
{
    size_t retidx = provide_idx();
    m_data.m_program.emplace_back(opcode, std::initializer_list<Arg>({ireg_hidden_constructor(this, retidx)}), args);
    return ireg_hidden_constructor(this, retidx);
}

inline IReg FuncImpl::newiop(int opcode, int depth, ::std::initializer_list<Arg> args)
{
    size_t retidx = provide_idx();
    m_data.m_program.emplace_back(opcode, std::initializer_list<Arg>({ireg_hidden_constructor(this, retidx), depth}), args);
    return ireg_hidden_constructor(this, retidx);
}

inline IReg FuncImpl::newiop(int opcode, ::std::initializer_list<Arg> args, IRegInternal retreg)
{
    Arg retarg;
    retarg.tag = Arg::IREG;
    retarg.idx = retreg;
    m_data.m_program.emplace_back(opcode, std::initializer_list<Arg>({retarg}), args);
    return ireg_hidden_constructor(this, retreg);
}

inline void FuncImpl::newiop_noret(int opcode, ::std::initializer_list<Arg> args)
{
    m_data.m_program.emplace_back(opcode, args);
}

inline void FuncImpl::newiop_noret(int opcode, int depth, std::initializer_list<Arg> args)
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
