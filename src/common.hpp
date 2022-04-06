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
    typedef size_t IRegInternal;

    inline IReg iregHid(IRegInternal a_idx, Func* a_func)
    {
        IReg ret;
        ret.func = a_func;
        ret.idx = a_idx;
        return static_cast<IReg&&>(ret);
    }

    inline Arg argIReg(IRegInternal idx, Func* impl = nullptr)
    {
        Arg res;
        res.tag = Arg::IREG;
        res.idx = idx;
        res.func = impl;
        return res;
    }
    
    inline Arg argIConst(int64_t val, Func* impl = nullptr)
    {
        Arg res;
        res.tag = Arg::ICONST;
        res.value = val;
        res.func = impl;
        return res;
    }

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
    
    struct Syntfunc
    {
        std::vector<Syntop> program;
        std::vector<IRegInternal> params;
        std::string name;
        enum {RETREG = size_t(-2)};
        Syntfunc() {}
    };


    template<typename T>
    struct SyntopIndexedArray
    {
        struct ArgIndA
        {
            enum {C_ARRSIZE, C_ARGTYPE, C_ARGVALUE, C_LEAF};
            int m_condition;
            size_t m_argnum;
            T m_leaf;
            std::unordered_map<int, ArgIndA> m_branches;
            ArgIndA(): m_condition(C_LEAF) {}
            ArgIndA(const T& a_leaf): m_condition(C_LEAF), m_leaf(a_leaf) {}
        };

        std::unordered_map<int, ArgIndA > containment;
        void add(int opcode, const ArgIndA& toAdd) {containment[opcode] = toAdd;};
        const T& operator[](const Syntop& index) const
        {
            auto opcoderator = containment.find(index.opcode);
            if (opcoderator == containment.end())
                throw std::string("Index error: nothing is registered with given opcode.");
            const ArgIndA* branch = &(opcoderator->second);
            while(branch->m_condition != ArgIndA::C_LEAF)
            {
                int condition_val;
                switch(branch->m_condition)
                {
                    case(ArgIndA::C_ARRSIZE):
                    {
                        condition_val = index.size();
                        break;
                    }
                    case(ArgIndA::C_ARGTYPE):
                    {
                        if(branch->m_argnum >= index.size())
                            throw std::string("Argument-indexed Array: too big argument indexed used in condition .");
                        condition_val = index[branch->m_argnum].tag;
                        break;
                    }
                    case(ArgIndA::C_ARGVALUE):
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
        inline bool empty() const { return containment.empty();}
        inline bool has(int opcode) const { return containment.count(opcode) != 0;} //Check only existence of command. For checking existance of variant correspondent to given arguments use has(Syntop& index)
    };

    namespace BackendTableConstructor
    {
        //Sl is for "SyntopIndexedArray leaf"
        template<typename T>
        inline typename SyntopIndexedArray<T>::ArgIndA Sl(const T& towrap) { return typename SyntopIndexedArray<T>::ArgIndA(towrap); }
        
        template<typename T>
        struct SyntopTreeTempBranch
        {
            int condval;
            typename SyntopIndexedArray<T>::ArgIndA val;
        };
    
        //Sb is for "SyntopIndexedArray branch"
        template<typename T>
        inline SyntopTreeTempBranch<T> _Sb(int cval, const typename SyntopIndexedArray<T>::ArgIndA& val)
        {
            SyntopTreeTempBranch<T> result;
            result.condval = cval;
            result.val = val;
            return result;
        }
    
        template<typename T>
        inline typename SyntopIndexedArray<T>::ArgIndA Sf(int condtype, size_t argnum, std::initializer_list<SyntopTreeTempBranch<T> > branches)
        {
            typename SyntopIndexedArray<T>::ArgIndA result;
            result.m_condition = condtype;
            result.m_argnum = argnum;
            for(const SyntopTreeTempBranch<T>& branch : branches)
                result.m_branches[branch.condval] = branch.val;
            return result;
        }

        //SFsiz is for "SyntopIndexedArray fork by array size"
        template<typename T>
        inline typename SyntopIndexedArray<T>::ArgIndA SFsiz(std::initializer_list<SyntopTreeTempBranch<T> > branches)
        {
            return Sf(SyntopIndexedArray<T>::ArgIndA::C_ARRSIZE, 0, branches);
        }

        //SFtyp is for "SyntopIndexedArray fork by argument type"
        template<typename T>
        inline typename SyntopIndexedArray<T>::ArgIndA SFtyp(size_t argnum, std::initializer_list<SyntopTreeTempBranch<T> > branches)
        {
            return Sf(SyntopIndexedArray<T>::ArgIndA::C_ARGTYPE, argnum, branches);
        }

        //SFval is for "SyntopIndexedArray fork by argument value"
        template<typename T>
        inline typename SyntopIndexedArray<T>::ArgIndA SFval(size_t argnum, std::initializer_list<SyntopTreeTempBranch<T> > branches)
        {
            return Sf(SyntopIndexedArray<T>::ArgIndA::C_ARGVALUE, argnum, branches);
        }
    };

    class ContextImpl : public Context
    {
    public:
        ContextImpl(Context* owner, Backend cmpl) : m_owner(owner), m_cmpl(cmpl) {}
        void startFunc(const std::string& name, std::initializer_list<IReg*> params);
        void endFunc();
        Func getFunc(const std::string& name);

        int m_refcount;
        inline Func* getCurrentFunc() { return &m_currentFunc; }
        inline Backend* getBackend() { return &m_cmpl; }
        inline Context* getOwner() const { return m_owner; }
    private:
        std::unordered_map<std::string, Func> m_functionsStorage;
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
