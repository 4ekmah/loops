/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#ifndef __LOOPS_COMMON_HPP__
#define __LOOPS_COMMON_HPP__

#include "loops/loops.hpp"
#include <functional>
#include <set>
#include <stack>
#include <unordered_map>
#include <typeindex>
#include <memory>

#define LOOPS_ASSERT_LINE_(x) #x
#define LOOPS_ASSERT_LINE(x) LOOPS_ASSERT_LINE_(x)

#undef Assert
#define Assert(expr) if(expr) ; else throw std::runtime_error("Assertion '" #expr "' failed at " __FILE__ ":" LOOPS_ASSERT_LINE(__LINE__))

namespace loops
{
    typedef int RegIdx;
    enum RegisterBasket { RB_INT = 0, RB_VEC = 1, RB_AMOUNT };

    inline IReg iregHid(RegIdx a_idx, Func* a_func)
    {
        IReg ret;
        ret.func = a_func;
        ret.idx = a_idx;
        return static_cast<IReg&&>(ret);
    }
    
    template<typename _Tp>
    inline VReg<_Tp> vregHid(RegIdx a_idx, Func* a_func)
    {
        VReg<_Tp> ret;
        ret.func = a_func;
        ret.idx = a_idx;
        return static_cast<VReg<_Tp>&&>(ret);
    }

    template<typename _Tp>
    inline void vregHidCopy(VReg<_Tp>& dest, const VReg<_Tp>& src) //TODO(ch): HidCopy isn't needed anymore, there is rawcopy.
    {
        dest.func = src.func;
        dest.idx = src.idx;
    }

    inline Arg argReg(int basketNum, RegIdx idx, Func* impl = nullptr)
    {
        Arg res;
        res.tag = basketNum == RB_INT ? Arg::IREG : Arg::VREG;
        res.idx = idx;
        res.func = impl;
        return res;
    }

    inline Arg argSpilled(int basketNum, size_t spOffset, Func* impl = nullptr)
    {
        Arg res;
        res.tag = basketNum == RB_INT ? Arg::ISPILLED : Arg::VSPILLED;
        res.value = spOffset;
        res.func = impl;
        return res;
    }

    inline Arg argIImm(int64_t val, Func* impl = nullptr)
    {
        Arg res;
        res.tag = Arg::IIMMEDIATE;
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

    static inline bool isInteger(int elemTyp)
    {
        return elemTyp == TYPE_U8 || elemTyp == TYPE_U16 ||elemTyp == TYPE_U32 ||elemTyp == TYPE_U64 ||
               elemTyp == TYPE_I8 || elemTyp == TYPE_I16 ||elemTyp == TYPE_I32 ||elemTyp == TYPE_I64;
    }

    static inline bool isFloat(int elemTyp)
    { return elemTyp == TYPE_FP16 || elemTyp == TYPE_FP32 ||elemTyp == TYPE_FP64; }

    static inline bool isUnsignedInteger(int elemType)
    { return elemType == TYPE_U8 || elemType == TYPE_U16 ||elemType == TYPE_U32 ||elemType == TYPE_U64; }

    static inline bool isSignedInteger(int elemType)
    { return elemType == TYPE_I8 || elemType == TYPE_I16 ||elemType == TYPE_I32 ||elemType == TYPE_I64; }

    static inline size_t elemSize(int typ)
    {
        switch (typ) {
            case TYPE_I8:
            case TYPE_U8:
                return 1;
            case TYPE_I16:
            case TYPE_U16:
            case TYPE_FP16:
            case TYPE_BF16:
                return 2;
            case TYPE_I32:
            case TYPE_U32:
            case TYPE_FP32:
                return 4;
            case TYPE_I64:
            case TYPE_U64:
            case TYPE_FP64:
                return 8;
            default:
                throw std::runtime_error("Unknown data type.");
        }
    }

    enum ArgFlags
    {
        AF_ADDRESS  = 1,
        AF_LOWER32  = 2, //010
        AF_LOWER16  = 4, //100
        AF_LOWER8   = 6, //110
        AF_NOPRINT  = 8,
        AF_PRINTOFFSET = 16,
    };

    enum InstructionConditions
    {
        IC_EQ = 0,
        IC_NE,
        IC_LT,
        IC_GT,
        IC_UGT,
        IC_LE,
        IC_ULE,
        IC_GE,
        IC_S,
        IC_NS,
        IC_UNKNOWN
    };

    inline int invertCondition(int condition)
    {
        return condition == IC_EQ  ? IC_NE : (
               condition == IC_NE  ? IC_EQ : (
               condition == IC_LT  ? IC_GE : (
               condition == IC_GT  ? IC_LE : (
               condition == IC_UGT ? IC_ULE : (
               condition == IC_LE  ? IC_GT : (
               condition == IC_ULE ? IC_UGT : (
               condition == IC_GE  ? IC_LT : (
               condition == IC_S   ? IC_NS : (
               condition == IC_NS  ? IC_S  : IC_UNKNOWN)))))))));
    }

    static inline uint64_t makeBitmask64(std::initializer_list<size_t> regNumbers)
    {
        uint64_t res = 0;
        for (size_t bitnum : regNumbers)
            res |= (static_cast<uint64_t>(1) << bitnum);
        return res;
    }

    inline RegIdx onlyBitPos64(uint64_t bigNum)
    {
        static const uint8_t bnBase[8] = { 0, 8, 16, 24, 32, 40, 48, 56 };
        static const uint8_t bnAdd[129] = { 0,1,2,0,3,0,0,0,4,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                            6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                            7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                            8 };
        uint32_t first32 = (uint32_t)(bigNum & 0xFFFFFFFF);
        uint32_t second32 = (uint32_t)(bigNum >> 32);
        uint8_t bytenum = (second32 != 0) << 2;
        uint32_t bnHalf = first32 + ((~(uint32_t)(second32 == 0)) & second32);

        uint16_t first16 = (uint16_t)(bnHalf & 0xFFFF);
        uint16_t second16 = (uint16_t)(bnHalf >> 16);
        bytenum += (second16 != 0) << 1;
        uint16_t bnHalfHalf = first16 + ((~(uint32_t)(second16 == 0)) & second16);

        uint8_t first8 = (uint8_t)(bnHalfHalf & 0xFF);
        uint8_t second8 = (uint8_t)(bnHalfHalf >> 8);
        bytenum += (second8 != 0);
        uint8_t bytecontent = first8 + ((~(uint32_t)(second8 == 0)) & second8);

        return static_cast<RegIdx>(bnBase[bytenum]) + static_cast<RegIdx>(bnAdd[bytecontent]) - 1;
    }


    inline RegIdx lsb64(uint64_t bigNum)
    {
        uint64_t firstReg = (bigNum & ~(bigNum - 1));
        return onlyBitPos64(firstReg);
    }

    inline RegIdx msb64(uint64_t bigNum)
    {
        if (bigNum == 0)
            return 0;
        bigNum |= bigNum >> 1;
        bigNum |= bigNum >> 2;
        bigNum |= bigNum >> 4;
        bigNum |= bigNum >> 8;
        bigNum |= bigNum >> 16;
        bigNum |= bigNum >> 32;
        if (bigNum == 0xFFFFFFFFFFFFFFFF)
            return 63;
        bigNum += 1;
        return onlyBitPos64(bigNum) - 1;
    }

    inline size_t amountOfBits64(uint64_t bigNum)
    {
        size_t res = 0;
        static const uint8_t amountInByte[256] =
        { 0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
          1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
          1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
          2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8 };
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
        bigNum >>= 8;
        res += amountInByte[bigNum & 0xFF];
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
        int spillPrefix;  //TODO(ch): IMPORTANT(CMPLCOND) : delete
        int spillPostfix; //TODO(ch): IMPORTANT(CMPLCOND) : delete
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
        std::vector<Arg> params;
        std::string name;
        int regAmount[RB_AMOUNT];
        enum {RETREG = size_t(-2)};
        Syntfunc() : regAmount{0,0} {}
        inline RegIdx provideIdx(int basketNum) { return regAmount[basketNum]++; }
    };

    class CompilerStage
    {
    public:
        virtual void process(Syntfunc& a_processed) const = 0;
        virtual ~CompilerStage() {}
    };
    typedef std::shared_ptr<CompilerStage> CompilerStagePtr;

    class Backend;
    class RegisterAllocator;
    class ContextImpl : public Context
    {
    public:
        ContextImpl(Context* owner);
        void startFunc(const std::string& name, std::initializer_list<IReg*> params);
        void endFunc();
        Func getFunc(const std::string& name);
        bool hasFunc(const std::string& name);
        std::string getPlatformName() const;
        size_t vbytes() const;
        void compileAll();

        int m_refcount;
        inline Func* getCurrentFunc() { return &m_currentFunc; }
        inline Backend* getBackend() { return m_backend.get(); }
        inline RegisterAllocator* getRegisterAllocator() { return m_registerAllocator.get(); }
        Context getOwner();
    private:
        std::unordered_map<std::string, Func> m_functionsStorage;
        Func m_currentFunc;
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
