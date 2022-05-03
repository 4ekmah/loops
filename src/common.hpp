/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
#ifndef __LOOPS_COMMON_HPP__
#define __LOOPS_COMMON_HPP__

#include "loops/loops.hpp"
#include <set>
#include <stack>
#include <unordered_map>
#include <typeindex>

#define LOOPS_ASSERT_LINE_(x) #x
#define LOOPS_ASSERT_LINE(x) LOOPS_ASSERT_LINE_(x)

#undef Assert
#define Assert(expr) if(expr) ; else throw std::runtime_error("Assertion '" #expr "' failed at " __FILE__ ":" LOOPS_ASSERT_LINE(__LINE__))

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

    inline Arg argISpilled(size_t spOffset, Func* impl = nullptr)
    {
        Arg res;
        res.tag = Arg::ISPILLED;
        res.value = spOffset;
        res.func = impl;
        return res;
    }

    inline bool regOrSpi(const Arg& toCheck)
    {
        return toCheck.tag == Arg::IREG || toCheck.tag == Arg::ISPILLED;
    }

    inline bool regOrSpiEq(const Arg& toCmp1, const Arg& toCmp2)
    {
        Assert(regOrSpi(toCmp1) && regOrSpi(toCmp2));
        if (toCmp1.tag != toCmp2.tag)
            return false;
        if (toCmp1.tag == Arg::IREG)
            return toCmp1.idx == toCmp2.idx;
        if (toCmp1.tag == Arg::ISPILLED)
            return toCmp1.value == toCmp2.value;
        return false;
    }

    inline Arg argIConst(int64_t val, Func* impl = nullptr)
    {
        Arg res;
        res.tag = Arg::ICONST;
        res.value = val;
        res.func = impl;
        return res;
    }

    struct ExpandableClass
    {
    public:
        template<typename T>
        void addExtension(const T& toAdd)
        {
            if(m_dictionary.get() == nullptr)
                m_dictionary = std::make_shared<std::unordered_map<std::type_index, std::shared_ptr<voidshrd> > >();
            std::unordered_map<std::type_index, std::shared_ptr<voidshrd> >& dictionary = *m_dictionary;
            if(dictionary.count(typeid(T)) != 0)
                throw std::string("Internal error: trying to add extension already defined.");
            std::shared_ptr< shrd<T> > extension = std::make_shared< shrd<T> >(toAdd);
            std::shared_ptr<voidshrd> extcasted = std::static_pointer_cast<voidshrd>(extension);
            std::pair<std::type_index, std::shared_ptr<voidshrd> > partoadd(typeid(T), extcasted);
            dictionary.insert(partoadd);
        }

        template<typename T>
        void addExtension()
        {
            if(m_dictionary.get() == nullptr)
                m_dictionary = std::make_shared<std::unordered_map<std::type_index, std::shared_ptr<voidshrd> > >();
            std::unordered_map<std::type_index, std::shared_ptr<voidshrd> >& dictionary = *m_dictionary;
            if(dictionary.count(typeid(T)) != 0)
                throw std::string("Internal error: trying to add extension already defined.");
            std::shared_ptr< shrd<T> > extension = std::make_shared< shrd<T> >();
            std::shared_ptr<voidshrd> extcasted = std::static_pointer_cast<voidshrd>(extension);
            std::pair<std::type_index, std::shared_ptr<voidshrd> > partoadd(typeid(T), extcasted);
            dictionary.insert(partoadd);
        }

        template<typename T>
        T& getExtension()
        {
            if(m_dictionary.get() == nullptr)
                throw std::string("Internal error: asking for nonexistent expansion.");
            std::unordered_map<std::type_index, std::shared_ptr<voidshrd> >& dictionary = *m_dictionary;
            if(dictionary.count(typeid(T)) == 0)
                throw std::string("Internal error: asking for nonexistent expansion.");
            std::shared_ptr<shrd<T> > extptr = std::static_pointer_cast<shrd<T> >(dictionary.at(typeid(T)));
            return extptr->get();
        }

        template<typename T>
        const T& getExtension() const
        {
            if(m_dictionary.get() == nullptr)
                throw std::string("Internal error: asking for nonexistent expansion.");
            std::unordered_map<std::type_index, std::shared_ptr<voidshrd> >& dictionary = *m_dictionary;
            if(dictionary.count(typeid(T)) == 0)
                throw std::string("Internal error: asking for nonexistent expansion.");
            std::shared_ptr<shrd<T> > extptr = std::static_pointer_cast<shrd<T> >(dictionary.at(typeid(T)));
            return extptr->get();
        }
        
        template<typename T>
        bool haveExtension() const
        {
            if(m_dictionary.get() == nullptr)
                return false;
            std::unordered_map<std::type_index, std::shared_ptr<voidshrd> >& dictionary = *m_dictionary;
            if(dictionary.count(typeid(T)) == 0)
                return false;
            return true;
        }
    private:
        struct voidshrd
        {
            public:
            virtual ~voidshrd() {}
        };
        template <typename T>
        struct shrd : public voidshrd
        {
            std::shared_ptr<T> content;
            inline shrd() : content(std::make_shared<T>()) {}
            inline shrd(const T& cp) : content(std::make_shared<T>(cp)) {}
            virtual ~shrd() {}
            inline T& get() { return *content; }
            inline const T& get() const { return *content; }
        };
        std::shared_ptr< std::unordered_map<std::type_index, std::shared_ptr<voidshrd> > > m_dictionary;
    };

    struct Syntop: public ExpandableClass
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

    class CompilerStage
    {
    public:
        virtual void process(Syntfunc& a_processed) const = 0;
        virtual ~CompilerStage() {}
    };
    typedef std::shared_ptr<CompilerStage> CompilerStagePtr;

    template<typename T>
    struct SyntopIndexedArray
    {
        struct ArgIndA
        {
            enum {C_ARRSIZE, C_ARGTYPE, C_ARGVALUE, C_LEAF, C_FLAGMASK};
            int m_condition;
            size_t m_argnum;
            uint64_t m_flagmask;
            T m_leaf;
            std::unordered_map<int64_t, ArgIndA> m_branches;
            ArgIndA(): m_condition(C_LEAF), m_argnum(-1), m_flagmask(0) {}
            ArgIndA(const T& a_leaf): m_condition(C_LEAF), m_leaf(a_leaf), m_argnum(-1), m_flagmask(0) {}
        };

        std::unordered_map<int, ArgIndA > containment;
        void add(int opcode, const ArgIndA& toAdd) {containment[opcode] = toAdd;};
        const T& operator[](const Syntop& index) const;
        std::set<size_t> filterStackPlaceable(const Syntop& index, const std::set<size_t>& toFilter) const;
        inline bool empty() const { return containment.empty();}
        inline bool has(int opcode) const { return containment.count(opcode) != 0;} //Check only existence of command. For checking existance of variant correspondent to given arguments use has(Syntop& index)
    private:
        static std::set<size_t> filterStackPlaceable_(const Syntop& index, const std::set<size_t>& toFilter, const ArgIndA& argind, bool& exists)
        {
            if (argind.m_condition == ArgIndA::C_LEAF)
            {
                exists = true;
                return std::set<size_t>();
            }
            int64_t condition_val;
            switch (argind.m_condition)
            {
            case(ArgIndA::C_ARRSIZE):
            {
                condition_val = index.size();
                break;
            }
            case(ArgIndA::C_ARGTYPE):
            {
                Assert(argind.m_argnum < index.size());
                condition_val = index[argind.m_argnum].tag;
                if (condition_val == Arg::IREG && toFilter.count(argind.m_argnum) && argind.m_branches.count(Arg::ISPILLED))
                {
                    Assert(argind.m_branches.count(Arg::IREG));
                    bool exNsp;
                    std::set<size_t> resNsp = filterStackPlaceable_(index, toFilter, argind.m_branches.at(Arg::IREG), exNsp);
                    std::set<size_t> toFilter2 = toFilter;
                    toFilter2.erase(argind.m_argnum);
                    bool exSp;
                    std::set<size_t> resSp = filterStackPlaceable_(index, toFilter2, argind.m_branches.at(Arg::ISPILLED), exSp);
                    resSp.insert(argind.m_argnum);
                    exists = (exSp || exNsp);
                    if ((exSp && exNsp && resSp.size() > resNsp.size()) || (exSp && !exNsp))
                        return resSp;
                    else if (exNsp)
                        return resNsp;
                    else
                        return std::set<size_t>();
                }
                break;
            }
            case(ArgIndA::C_ARGVALUE):
            {
                Assert(argind.m_argnum < index.size());
                condition_val = index[argind.m_argnum].value;
                break;
            }
            case(ArgIndA::C_FLAGMASK):
            {
                Assert(argind.m_argnum < index.size());
                condition_val = argind.m_flagmask & index[argind.m_argnum].flags;
                break;
            }
            default:
                throw std::string("Argument-indexed Array: Unknown condition type.");
            }
            if (argind.m_branches.count(condition_val) == 0)
            {
                exists = false;
                return std::set<size_t>();
            }
            return filterStackPlaceable_(index, toFilter, argind.m_branches.at(condition_val), exists);
        }
    };

    template<typename T>
    const T& SyntopIndexedArray<T>::operator[](const Syntop& index) const
    {
        auto opcoderator = containment.find(index.opcode);
        if (opcoderator == containment.end())
            throw std::string("Index error: nothing is registered with given opcode.");
        const ArgIndA* branch = &(opcoderator->second);
        while (branch->m_condition != ArgIndA::C_LEAF)
        {
            int64_t condition_val;
            switch (branch->m_condition)
            {
            case(ArgIndA::C_ARRSIZE):
            {
                condition_val = index.size();
                break;
            }
            case(ArgIndA::C_ARGTYPE):
            {
                if (branch->m_argnum >= index.size())
                    throw std::string("Argument-indexed Array: too big argument indexed used in condition .");
                condition_val = index[branch->m_argnum].tag;
                break;
            }
            case(ArgIndA::C_ARGVALUE):
            {
                if (branch->m_argnum >= index.size())
                    throw std::string("Argument-indexed Array: too big argument indexed used in condition .");
                condition_val = index[branch->m_argnum].value;
                break;
            }
            case(ArgIndA::C_FLAGMASK):
            {
                Assert(branch->m_argnum < index.size());
                condition_val = branch->m_flagmask & index[branch->m_argnum].flags;
                break;
            }
            default:
                throw std::string("Argument-indexed Array: Unknown condition type.");
            }
            if (branch->m_branches.count(condition_val) == 0)
                throw std::string("Argument-indexed Array: don't have such an element.");
            branch = &(branch->m_branches.at(condition_val));
        }
        return branch->m_leaf;
    }

    template<typename T>
    std::set<size_t> SyntopIndexedArray<T>::filterStackPlaceable(const Syntop& index, const std::set<size_t>& toFilter) const
    {
        auto opcoderator = containment.find(index.opcode);
        Assert(opcoderator != containment.end());
        bool exists;
        return filterStackPlaceable_(index, toFilter, *(&(opcoderator->second)), exists);
    }

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
            SyntopTreeTempBranch() : condval(-1) {}
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

        //SFtyp is for "SyntopIndexedArray fork by argument type"
        template<typename T>
        inline typename SyntopIndexedArray<T>::ArgIndA SFflg(size_t argnum, uint64_t flagmask, std::initializer_list<SyntopTreeTempBranch<T> > branches)
        {
            SyntopIndexedArray<T>::ArgIndA ret = Sf(SyntopIndexedArray<T>::ArgIndA::C_FLAGMASK, argnum, branches);
            ret.m_flagmask = flagmask;
            return ret;
        }

        //SFval is for "SyntopIndexedArray fork by argument value"
        template<typename T>
        inline typename SyntopIndexedArray<T>::ArgIndA SFval(size_t argnum, std::initializer_list<SyntopTreeTempBranch<T> > branches)
        {
            return Sf(SyntopIndexedArray<T>::ArgIndA::C_ARGVALUE, argnum, branches);
        }
    };
    class Backend;
    class RegisterAllocator;
    class ContextImpl : public Context
    {
    public:
        ContextImpl(Context* owner);
        void startFunc(const std::string& name, std::initializer_list<IReg*> params);
        void endFunc();
        Func getFunc(const std::string& name);
        std::string getPlatformName() const;
        void compileAll();

        int m_refcount;
        inline Func* getCurrentFunc() { return &m_currentFunc; }
        inline Backend* getBackend() { return m_bcknd.get(); }
        inline RegisterAllocator* getRegisterAllocator() { return m_registerAllocator.get(); }
        inline Context* getOwner() const { return m_owner; }
    private:
        std::unordered_map<std::string, Func> m_functionsStorage;
        Func m_currentFunc;
        Context* m_owner;
        std::shared_ptr<Backend> m_bcknd;
        std::shared_ptr<RegisterAllocator> m_registerAllocator;
    };

    inline Func* _getImpl(Func* wrapper) { return wrapper->impl; };
    inline Context* _getImpl(Context* wrapper) { return wrapper->impl; };
    inline ContextImpl* getImpl(Context* wrapper)
    {
        if(!wrapper)
            throw std::string("Null context pointer.");
        return static_cast<ContextImpl*>(_getImpl(wrapper));
    }
};

#endif //__LOOPS_COMMON_HPP__
