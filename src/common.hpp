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
        syntop();
        syntop(const syntop& fwho);
        syntop(int a_opcode, const std::vector<Arg>& a_args);
        syntop(int a_opcode, std::initializer_list<Arg> a_args);
        syntop(int a_opcode, std::initializer_list<Arg> a_prefix,                                             std::initializer_list<Arg> a_args);
    };
    
    typedef size_t IRegInternal;

    struct syntfunc
    {
        std::vector<syntop> m_program;
        std::vector<IRegInternal> m_params;
        IRegInternal m_retreg;
        syntfunc() : m_retreg(IReg::NOIDX) {}
    };

    template<typename T>
    struct arg_indexed_array //TODO(ch): can't we implement syntop_indexed_array at last???
    {
        enum {C_ARRSIZE, C_ARGTYPE, C_ARGVALUE, C_LEAF};
        int m_condition;
        size_t m_argnum;
        T m_leaf;
        std::unordered_map<int, arg_indexed_array<T> > m_branches;
        arg_indexed_array(): m_condition(C_LEAF) {}
        arg_indexed_array(const T& a_leaf): m_condition(C_LEAF), m_leaf(a_leaf) {}
        const T& operator[](const std::vector<Arg>& index) const
        {
            const arg_indexed_array* branch = this;
            while(branch->m_condition != C_LEAF)
            {
                int condition_val;
                switch(branch->m_condition)
                {
                    case(C_ARRSIZE):
                    {
                        condition_val = index.size();
                        break;
                    }
                    case(C_ARGTYPE):
                    {
                        if(branch->m_argnum >= index.size())
                            throw std::string("Argument-indexed Array: too big argument indexed used in condition .");
                        condition_val = index[branch->m_argnum].tag;
                        break;
                    }
                    case(C_ARGVALUE):
                    {
                        if(branch->m_argnum >= index.size())
                            throw std::string("Argument-indexed Array: too big argument indexed used in condition .");
                        condition_val = index[branch->m_argnum].value;
                        break;
                    }
                    default:
                        throw std::string("Argument-indexed Array: Unknown condition type.");
                }
                if(branch->m_branches.count(condition_val) == 0)
                    throw std::string("Argument-indexed Array: don't have such an element.");
                branch = &(branch->m_branches.at(condition_val));
            }
            return branch->m_leaf;
        }
    };

    
    //TODO(ch): create normal printer class with ability to set overrules.
    void print_program(::std::ostream& str, const syntfunc& func,
                       const std::unordered_map<int, std::string>& opstrings,
                       const std::unordered_map<int, std::function<void(::std::ostream&, const syntop&)> >& p_overrules, size_t firstop = -1, size_t lastop = -1);
    
    void print_program(::std::ostream& str, const syntfunc& func,
                       const std::unordered_map<int, std::string>& opstrings, size_t firstop = -1, size_t lastop = -1);

    class ContextImpl : public Context
    {
    private:
        std::unordered_map<std::string, Func> m_functionsStorage; //TODO(ch): unordered map?
        Func m_currentFunc;
        Context* m_owner;
        Backend m_cmpl;
    public:
        ContextImpl(Context* owner, Backend cmpl) : m_owner(owner), m_cmpl(cmpl) {}
        void startfunc(const std::string& name, std::initializer_list<IReg*> params);
        void endfunc();
        Func getfunc(const std::string& name);

        int refcount;
        inline Func* get_current_func() { return &m_currentFunc; }
        inline Backend* get_compiler() { return &m_cmpl; }
        inline Context* get_owner() const { return m_owner; }
    };


    inline Func* _getImpl(Func* wrapper) { return wrapper->impl; };
    inline Context* _getImpl(Context* wrapper) { return wrapper->impl; };
    inline Backend* _getImpl(Backend* wrapper) { return wrapper->impl; };
    inline ContextImpl* getImpl(Context* wrapper)
    {
        if(!wrapper)
            throw std::string("Null context pointer.");
        return static_cast<ContextImpl*>(_getImpl(wrapper));
    }
};

#endif //__LOOPS_COMMON_HPP__
