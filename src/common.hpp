/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/
#ifndef __LOOPS_COMMON_HPP__
#define __LOOPS_COMMON_HPP__

#include "loops/loops.hpp"
#include <functional>
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

    inline Arg argIConst(int64_t val, Func* impl = nullptr)
    {
        Arg res;
        res.tag = Arg::ICONST;
        res.value = val;
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

    enum ArgFlags
    {
        AF_ADDRESS = 1,
        AF_LOWER32 = 2,
        AF_NOPRINT = 4,
        AF_PRINTOFFSET = 8,
    };

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
                throw std::runtime_error("Syntaxic operation: too much args!");
            return args + args_size;
        }
        inline const Arg* begin() const { return args; }
        inline const Arg* end() const
        {
            if(args_size > SYNTOP_ARGS_MAX)
                throw std::runtime_error("Syntaxic operation: too much args!");
            return args + args_size;
        }

        inline size_t size() const { return args_size; }

        inline Arg& operator[](size_t anum)
        {
            if(anum >= args_size)
                throw std::runtime_error("Syntaxic operation: too big argument index!");
            return args[anum];
        }
        inline const Arg& operator[](size_t anum) const
        {
            if(anum >= args_size)
                throw std::runtime_error("Syntaxic operation: too big argument index!");
            return args[anum];
        }

        inline Arg& back()
        {
            if(args_size == 0)
                throw std::runtime_error("Syntaxic operation: taking argument from non-parameterized operation!");
            return args[args_size - 1];
        }
        inline const Arg& back() const
        {
            if(args_size == 0)
                throw std::runtime_error("Syntaxic operation: taking argument from non-parameterized operation!");
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

    inline uint64_t makeRegBasket(std::initializer_list<IRegInternal> regNumbers)
    {
        uint64_t res = 0;
        for (IRegInternal bitnum : regNumbers)
            res |= (static_cast<uint64_t>(1) << bitnum);
        return res;
    }

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
        inline Backend* getBackend() { return m_backend.get(); }
        inline RegisterAllocator* getRegisterAllocator() { return m_registerAllocator.get(); }
        inline Context* getOwner() const { return m_owner; }
    private:
        std::unordered_map<std::string, Func> m_functionsStorage;
        Func m_currentFunc;
        Context* m_owner;
        std::shared_ptr<Backend> m_backend;
        std::shared_ptr<RegisterAllocator> m_registerAllocator;
    };

    inline Func* _getImpl(Func* wrapper) { return wrapper->impl; };
    inline Context* _getImpl(Context* wrapper) { return wrapper->impl; };
    inline ContextImpl* getImpl(Context* wrapper)
    {
        Assert(wrapper);
        return static_cast<ContextImpl*>(_getImpl(wrapper));
    }
};

#endif //__LOOPS_COMMON_HPP__
