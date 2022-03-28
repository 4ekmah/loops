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
    struct Syntop
    {
    private:
        enum {SYNTOP_ARGS_MAX = 10};
    public:
        int opcode;
        Arg args[SYNTOP_ARGS_MAX];
        size_t args_size;
        Syntop();
        Syntop(const Syntop& fwho);
        Syntop(int a_opcode, const std::vector<Arg>& a_args);
        Syntop(int a_opcode, std::initializer_list<Arg> a_args);
        Syntop(int a_opcode, std::initializer_list<Arg> a_prefix, std::initializer_list<Arg> a_args);
        inline Arg* begin(){ return args; }
        inline Arg* end()
        {
            if(args_size > SYNTOP_ARGS_MAX)
                throw std::string("Syntaxic operation: too much args!");
            return args + args_size;
        }
        inline const Arg* begin() const { return args; }
        inline const Arg* end() const
        {
            if(args_size > SYNTOP_ARGS_MAX)
                throw std::string("Syntaxic operation: too much args!");
            return args + args_size;
        }

        inline size_t size() const { return args_size; }

        inline Arg& operator[](size_t anum)
        {
            if(anum >= args_size)
                throw std::string("Syntaxic operation: too big argument index!");
            return args[anum];
        }
        inline const Arg& operator[](size_t anum) const
        {
            if(anum >= args_size)
                throw std::string("Syntaxic operation: too big argument index!");
            return args[anum];
        }

        inline Arg& back()
        {
            if(args_size == 0)
                throw std::string("Syntaxic operation: taking argument from non-parameterized operation!");
            return args[args_size - 1];
        }
        inline const Arg& back() const
        {
            if(args_size == 0)
                throw std::string("Syntaxic operation: taking argument from non-parameterized operation!");
            return args[args_size - 1];
        }
    };
    
    typedef size_t IRegInternal;

    struct Syntfunc
    {
        std::vector<Syntop> m_program;
        std::vector<IRegInternal> m_params;
        IRegInternal m_retreg;
        Syntfunc() : m_retreg(IReg::NOIDX) {}
    };

    template<typename T>
    struct ArgIndexedArray //TODO(ch): can't we implement syntop_indexed_array at last???
    {
        enum {C_ARRSIZE, C_ARGTYPE, C_ARGVALUE, C_LEAF};
        int m_condition;
        size_t m_argnum;
        T m_leaf;
        std::unordered_map<int, ArgIndexedArray<T> > m_branches;
        ArgIndexedArray(): m_condition(C_LEAF) {}
        ArgIndexedArray(const T& a_leaf): m_condition(C_LEAF), m_leaf(a_leaf) {}
        const T& operator[](const Syntop& index) const
        {
            const ArgIndexedArray* branch = this;
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
    void printProgram(::std::ostream& str, const Syntfunc& func,
                       const std::unordered_map<int, std::string>& opstrings,
                       const std::unordered_map<int, std::function<void(::std::ostream&, const Syntop&)> >& p_overrules, size_t firstop = -1, size_t lastop = -1);
    
    void printProgram(::std::ostream& str, const Syntfunc& func,
                       const std::unordered_map<int, std::string>& opstrings, size_t firstop = -1, size_t lastop = -1);

    class ContextImpl : public Context
    {
    public:
        ContextImpl(Context* owner, Backend cmpl) : m_owner(owner), m_cmpl(cmpl) {}
        void startFunc(const std::string& name, std::initializer_list<IReg*> params);
        void endFunc();
        Func getFunc(const std::string& name);

        int m_refcount;
        inline Func* getCurrentFunc() { return &m_currentFunc; }
        inline Backend* getCompiler() { return &m_cmpl; }
        inline Context* getOwner() const { return m_owner; }
    private:
        std::unordered_map<std::string, Func> m_functionsStorage; //TODO(ch): unordered map?
        Func m_currentFunc;
        Context* m_owner;
        Backend m_cmpl;
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
