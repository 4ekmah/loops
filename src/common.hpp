/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
#ifndef __LOOPS_COMMON_HPP__
#define __LOOPS_COMMON_HPP__

#include "loops/loops.hpp"
#include <unordered_map>
#include <stack>
namespace loops
{
    struct syntop
    {
        int opcode;
        std::vector<Arg> args; //TODO(ch): recreate as Arg[10] instead of vector.
        syntop(const syntop& fwho);
        syntop(int a_opcode, std::initializer_list<Arg> a_args);
        syntop(int a_opcode, std::initializer_list<Arg> a_prefix,                                             std::initializer_list<Arg> a_args);
    };
    typedef std::vector<syntop> syntprogram;
    
    //TODO(ch): create normal printer class with ability to set overrules.
    void print_program(::std::ostream& str, const syntprogram& prog,
                       const std::unordered_map<int, std::string>& opstrings,
                       const std::unordered_map<int, std::function<void(::std::ostream&, const syntop&)> >& p_overrules, size_t firstop = -1, size_t lastop = -1);
    
    void print_program(::std::ostream& str, const syntprogram& prog,
                       const std::unordered_map<int, std::string>& opstrings, size_t firstop = -1, size_t lastop = -1);

    typedef size_t IRegInternal;
    class ContextImpl;

    class FuncImpl : public Func
    {
    public:
        FuncImpl(const std::string& name, Context* ctx, std::initializer_list<IReg*> params);
        static Func makeWrapper(const std::string& name, Context* ctx, std::initializer_list<IReg*> params);

        std::string name() const { return m_name; }; //TODO(ch): what for we need name here?
        void call(std::initializer_list<int64_t> args) const;
        void* ptr();

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

        inline const std::vector<IRegInternal>& getParams() const { return m_params_idx; }
        inline const std::vector<syntop>& getProgram() const { return m_program; }
        inline const IRegInternal getRetReg() const { return m_return_idx; }
    private:

        struct cflowbracket
        {
            enum { DO, DOIF, IF, ELSE };
            size_t m_tag;
            size_t m_label_or_pos;
            cflowbracket(size_t a_tag, size_t a_label_or_pos) : m_tag(a_tag), m_label_or_pos(a_label_or_pos) {}
        };
        std::stack<cflowbracket> m_cflowstack;

        syntprogram m_program;
        std::vector<IRegInternal> m_params_idx;
        IRegInternal m_return_idx;
        std::vector<size_t> m_return_positions;
        std::string m_name;
        ContextImpl* m_context;
        size_t m_nextidx;
        size_t m_nextlabelidx;

        inline std::vector<Arg> depthed_args(int depth, std::initializer_list<Arg> args);
    };

    class RCCompiler : public Compiler
    {
    public:
        size_t refcount;
        RCCompiler() : Compiler(), refcount(0) {}
    };
};

#endif //__LOOPS_COMMON_HPP__
