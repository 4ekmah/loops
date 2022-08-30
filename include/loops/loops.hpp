/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_LOOPS_HPP__
#define __LOOPS_LOOPS_HPP__

#include <inttypes.h>
#include <ostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "defines.hpp"

namespace loops
{

struct f16_t {
    explicit f16_t(float x);
    uint16_t bits;
};

enum {
    TYPE_U8=0, TYPE_I8=1, TYPE_U16=2, TYPE_I16=3,
    TYPE_U32=4, TYPE_I32=5, TYPE_U64=6, TYPE_I64=7,
    TYPE_FP16=8, TYPE_BF16=9, TYPE_FP32=10, TYPE_FP64=11
};

enum {
    OP_LOAD=0,
    OP_STORE,

    OP_MOV,
    OP_XCHG,

    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_SHL,
    OP_SHR,
    OP_SAR,
    OP_AND, //Note: It's bitwise operations.
    OP_OR,
    OP_XOR,
    OP_NOT,
    OP_NEG,
    OP_CMP,
    OP_SELECT,   //SELECT <dest>, <condition>, <value-for-true>, <value-for-false>
    OP_MIN,
    OP_MAX,
    OP_ABS,
    OP_SIGN,

    OP_ROUND,
    OP_FLOOR,
    OP_CEIL,

    //For service usage only
    OP_SPILL,   //(stackPos, reg), stackPos is positive distance from SP, measured in 8byte-long units
    OP_UNSPILL, //(reg, stackPos)

    OP_JMP,
    OP_JMP_GT, //TODO(ch): implement JCC operation instead of this endless variations.
    OP_JMP_GE,
    OP_JMP_LT,
    OP_JMP_LE,
    OP_JMP_NE,
    OP_JMP_EQ,
    OP_RET,
    OP_LABEL,

    OP_IF,
    OP_ELSE,
    OP_ENDIF,
    OP_WHILE,  //WHILE  <CMPcode>, <continuelabel>, <breaklabel>  //TODO(ch): keep there more annotations
    OP_ENDWHILE, //ENDWHILE <continuelabel>, <breaklabel>
    OP_BREAK,
    OP_CONTINUE,

    VOP_LOAD,
    VOP_STORE,
    
    VOP_ADD,
    VOP_SUB,
    VOP_MUL,
    VOP_DIV,
//    VOP_MOD,
    VOP_FMA,
    VOP_SAL,
    VOP_SHL,
    VOP_SAR,
    VOP_SHR,
    VOP_AND,
    VOP_OR,
    VOP_XOR,
    VOP_NOT,
    VOP_NEG,

    VOP_MIN,
    VOP_MAX,

    VOP_GT,
    VOP_GE,
    VOP_LT,
    VOP_LE,
    VOP_NE,
    VOP_EQ,
    
    VOP_ALL,
    VOP_ANY,
    VOP_TRUNC,
    VOP_FLOOR,
    VOP_CAST,
    VOP_REINTERPRET,
    VOP_BROADCAST,

//Intel-only operations:
    OP_X86_ADC, //Add with carry flag.
    OP_X86_CQO,
//Aarch64-only operations:
    OP_ARM_CINC,
    OP_ARM_CNEG,
    OP_ARM_MOVK,   //Move bytes to shifted byte position of register and keep other bits unchanged.
    VOP_ARM_LD1,
    VOP_ARM_ST1,

    OP_NOINIT
};

template<typename _Tp> struct ElemTraits {};
template<> struct ElemTraits<uint8_t> {
    typedef uint8_t elemtype;
    typedef uint8_t masktype;
    enum { depth = TYPE_U8, elemsize=1 };
};
template<> struct ElemTraits<int8_t> {
    typedef int8_t elemtype;
    typedef uint8_t masktype;
    enum { depth = TYPE_I8, elemsize=1 };
};
template<> struct ElemTraits<uint16_t> {
    typedef uint16_t elemtype;
    typedef uint16_t masktype;
    enum { depth = TYPE_U16, elemsize=2 };
};
template<> struct ElemTraits<int16_t> {
    typedef int16_t elemtype;
    typedef uint16_t masktype;
    enum { depth = TYPE_I16, elemsize=2 };
};
template<> struct ElemTraits<uint32_t> {
    typedef uint32_t elemtype;
    typedef uint32_t masktype;
    enum { depth = TYPE_U32, elemsize=4 };
};
template<> struct ElemTraits<int32_t> {
    typedef int32_t elemtype;
    typedef uint32_t masktype;
    enum { depth = TYPE_I32, elemsize=4 };
};
template<> struct ElemTraits<uint64_t> {
    typedef uint64_t elemtype;
    typedef uint64_t masktype;
    enum { depth = TYPE_U64, elemsize=8 };
};
template<> struct ElemTraits<int64_t> {
    typedef int64_t elemtype;
    typedef uint64_t masktype;
    enum { depth = TYPE_I64, elemsize=8 };
};
template<> struct ElemTraits<f16_t> {
    typedef float elemtype;
    typedef uint16_t masktype;
    enum { depth = TYPE_FP16, elemsize=2 };
};
template<> struct ElemTraits<float> {
    typedef float elemtype;
    typedef uint32_t masktype;
    enum { depth = TYPE_FP32, elemsize=4 };
};
template<> struct ElemTraits<double> {
    typedef double elemtype;
    typedef uint64_t masktype;
    enum { depth = TYPE_FP64, elemsize=8 };
};

class Func;
struct IReg
{
    IReg();
    IReg(const IReg& r); //Must generate copy(mov) code
    IReg(IReg&& a) noexcept;
    IReg& operator=(const IReg& r); // may generate real code if 'this' is already initialized

    int idx;
    Func* func;
    enum {NOIDX = -1};
};

class Context;
template<typename _Tp> struct VReg
{
    typedef _Tp elemtype;

    VReg() : idx(NOIDX), func(nullptr) {}
    VReg(const VReg<_Tp>& r);
    VReg(VReg<_Tp>&& a) noexcept : func(a.func), idx(a.idx) {}
    VReg<_Tp>& operator=(const VReg<_Tp>& r);

    int idx;
    Func* func;
    enum {NOIDX = -1};
};

typedef VReg<uint8_t> VReg8u;
typedef VReg<int8_t> VReg8i;
typedef VReg<uint16_t> VReg16u;
typedef VReg<int16_t> VReg16i;
typedef VReg<uint32_t> VReg32u;
typedef VReg<int32_t> VReg32i;
typedef VReg<uint64_t> VReg64u;
typedef VReg<int64_t> VReg64i;
typedef VReg<float> VReg32f;
typedef VReg<double> VReg64f;

struct Arg
{
    enum { EMPTY = 0, IREG = 1, IIMMEDIATE = 2, VREG = 3, ISPILLED = 4, VSPILLED = 5 };

    Arg();
    Arg(const IReg& r);
    Arg(int64_t a_value, Context* ctx = nullptr);
    template<typename _Tp>
    Arg(const VReg<_Tp>& vr);
    int idx;
    Func* func;
    size_t tag;
    int64_t value;
    uint64_t flags;
    size_t elemtype;
};

class Func
{
    friend Func* _getImpl(Func* wrapper);
public:
    Func();
    Func(const Func& f);
    Func& operator=(const Func& f);
    virtual ~Func();

    std::string name() const; //TODO(ch): what for we need name here? 
    void call(std::initializer_list<int64_t> args) const;
    void* ptr(); // returns function pointer. Ensure, that all passed parameters are 64-bit wide.
    void printBytecode(std::ostream& out) const;
    enum { PC_OPNUM = 1 , PC_OP = 2, PC_HEX = 4 };
    void printAssembly(std::ostream& out, int columns = PC_OPNUM | PC_OP | PC_HEX) const;

protected:
    static Func make(Func* a_wrapped);
    Func* impl;
};

class Context
{
    friend Context* _getImpl(Context* wrapper);
public:
    Context();
    Context(const Context& ctx);
    virtual ~Context();
    Context& operator=(const Context& ctx);

    void getFuncs(std::vector<Func>& funcs);
    Func getFunc(const std::string& name);

    /*
    //TODO(ch): Implement with RISC-V RVV
    //Create register, connected to v1, v2, and contain them as halves. RVV-only.
    template<typename _Tp> VReg<_Tp> spliceVregs(const VReg<_Tp>& v1, const VReg<_Tp>& v2);
    //Create constrained register, which is pnum's part of wh. Divider is degree of two. RVV-only.
    template<typename _Tp> VReg<_Tp> fractionVreg(const VReg<_Tp>& wh, size_t divider, size_t pnum);
    Notes:
    1.) Neon will support no splices and only one type of fraction:
    fractionVreg(wh, 2, 1);
    2.) SVE will support no splices and no fractions(it's look strange to create many predicates for
    this[the only way how connected vectors can be implemented on SVE]).
    */

    // control flow
    //TODO(ch): IMPORTANT(CMPLCOND) Obsolete interface. Delete after complex condition implementation.
    void startFunc(const std::string& name, std::initializer_list<IReg*> params);
    IReg const_(int64_t value);
    void endFunc();
    void while_(const IReg& r);
    void endwhile_();
    void break_();
    void continue_();
    void if_(const IReg& r);
    void elif_(const IReg& r);
    void else_();
    void endif_();
    void return_();
    void return_(int64_t retval);
    void return_(const IReg& retval);
    
    // direct call
    IReg call_(const IReg& addr, std::initializer_list<IReg> args);
    // indirect call
    IReg call_(const IReg& addr, const IReg& offset, std::initializer_list<IReg> args);

    //TODO(ch): make next methods static:
    
    std::string getPlatformName() const;
    size_t vbytes() const; //size of vector register in bytes
    template<typename _Tp> inline size_t vlanes() const { return vbytes() / sizeof(_Tp); }
    void compileAll();
protected:
    Context(Context* a_impl): impl(a_impl) {}
    Context* impl;
};

struct __Loops_ConditionMarker_
{
    explicit __Loops_ConditionMarker_(Context* _CTX);
    operator bool() { return false; }
};

struct __Loops_CFScopeBracket_
{
    enum CFType {IF, ELIF, ELSE, WHILE };
    explicit __Loops_CFScopeBracket_(Context* _CTX, CFType _cftype, const IReg& condition);
    ~__Loops_CFScopeBracket_();
    Context* CTX;
    CFType cftype;
    operator bool() { return false; }
};

struct __Loops_FuncScopeBracket_
{
    explicit __Loops_FuncScopeBracket_(Context* _CTX, const std::string& name, std::initializer_list<IReg*> params);
    ~__Loops_FuncScopeBracket_();
    Context* CTX;
    operator bool() { return false; }
};

template<typename _Tp>
inline int64_t __loops_pack_2_valtype_(_Tp tocast)
{
    int64_t ret = 0;
    *(reinterpret_cast<_Tp*>(&ret)) = tocast;
    return ret;
}

#define USE_CONTEXT_(ctx) loops::Context __loops_ctx__(ctx);
#define STARTFUNC_(funcname, ...) if(__Loops_FuncScopeBracket_ __loops_func_{&__loops_ctx__, (funcname), {__VA_ARGS__}}) ; else
#define CONST_(x) __loops_ctx__.const_(x)
#define VCONST_(eltyp, x) newiopV<eltyp>(OP_MOV, { Arg(__loops_pack_2_valtype_(eltyp(x)), &__loops_ctx__) })
#define IF_(expr) if(__Loops_ConditionMarker_ __loops_cm_{&__loops_ctx__}) ; else \
    if(__Loops_CFScopeBracket_ __loops_cf_{&__loops_ctx__, __Loops_CFScopeBracket_::IF, (expr)}) ; else
#define ELIF_(expr) if(__Loops_ConditionMarker_ __loops_cm_{&__loops_ctx__}) ; else \
    if(__Loops_CFScopeBracket_ __loops_cf_{&__loops_ctx__, __Loops_CFScopeBracket_::ELIF, (expr)}) ; else
#define ELSE_ if(__Loops_CFScopeBracket_ __loops_cf_{&__loops_ctx__, __Loops_CFScopeBracket_::ELSE, (IReg())}) ; else
#define WHILE_(expr) if(__Loops_ConditionMarker_ __loops_cm_{&__loops_ctx__}) ; else \
    if(__Loops_CFScopeBracket_ __loops_cf_{&__loops_ctx__, __Loops_CFScopeBracket_::WHILE, (expr)}) ; else
#define BREAK_ __loops_ctx__.break_()
#define CONTINUE_ __loops_ctx__.continue_()
#define RETURN_(x) __loops_ctx__.return_(x)

IReg newiop(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
IReg newiop(int opcode, int depth, std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
void newiopNoret(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
void newiopNoret(int opcode, int depth, std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
void newiopAug(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
template<typename _Tp> inline VReg<_Tp> newiopV(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});

///////////////////////////// integer operations ///////////////////////

// load with zero/sign extension
static inline IReg loadx(const IReg& base, int depth)
{ return newiop(OP_LOAD, depth, {base}); }
static inline IReg loadx(const IReg& base, const IReg& offset, int depth)
{ return newiop(OP_LOAD, depth, {base, offset}); }
static inline IReg loadx(const IReg& base, int64_t offset, int depth)
{ return newiop(OP_LOAD, depth, {base, Arg(offset)}, { 1 }); }

static inline IReg load(const IReg& base)
{ return newiop(OP_LOAD, TYPE_I64, {base}); }
static inline IReg load(const IReg& base, const IReg& offset)
{ return newiop(OP_LOAD, TYPE_I64, {base, offset}); }

template<typename _Tp> static inline IReg load_(const IReg& base)
{ return loadx(base, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
IReg load_(const IReg& base, const IReg& offset)
{ return static_cast<IReg&&>(loadx(base, offset, ElemTraits<_Tp>::depth)); }
template<typename _Tp> static inline
IReg load_(const IReg& base, int64_t offset)
{ return static_cast<IReg&&>(loadx(base, offset, ElemTraits<_Tp>::depth)); }

// store part of register
static inline void storex(const IReg& base, const IReg& r, int depth)
{ newiopNoret(OP_STORE, depth, {base, r}); }
static inline void storex(const IReg& base, int64_t a, int depth)
{ newiopNoret(OP_STORE, depth, {base, Arg(a)}, { 1 }); }
static inline void storex(const IReg& base, const IReg& offset, const IReg& r, int depth)
{ newiopNoret(OP_STORE, depth, {base, offset, r}); }
static inline void storex(const IReg& base, int64_t offset, const IReg& r, int depth)
{ newiopNoret(OP_STORE, depth, {base, Arg(offset), r}, { 1 }); }
static inline void storex(const IReg& base, const IReg& offset, int64_t a, int depth)
{ newiopNoret(OP_STORE, depth, {base, offset, Arg(a)}, { 2 }); }
static inline void storex(const IReg& base, int64_t offset, int64_t a, int depth)
{ newiopNoret(OP_STORE, depth, {base, Arg(offset), Arg(a)}, { 1, 2 }); }
static inline void store(const IReg& base, const IReg& r)
{ newiopNoret(OP_STORE, TYPE_I64, {base, r}); }
static inline void store(const IReg& base, const IReg& offset, const IReg& r)
{ newiopNoret(OP_STORE, TYPE_I64, {base, offset, r}); }
template<typename _Tp> static inline
void store_(const IReg& base, const IReg& r)
{ storex(base, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
void store_(const IReg& base, int64_t a)
{ storex(base, a, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
void store_(const IReg& base, const IReg& offset, const IReg& r)
{ storex(base, offset, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
void store_(const IReg& base, int64_t offset, const IReg& r)
{ storex(base, offset, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
void store_(const IReg& base, const IReg& offset, int64_t a)
{ storex(base, offset, a, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
void store_(const IReg& base, int64_t offset, int64_t a)
{ storex(base, offset, a, ElemTraits<_Tp>::depth); }
static inline IReg operator + (const IReg& a, const IReg& b)
{ return newiop(OP_ADD, {a, b}); }
static inline IReg operator + (const IReg& a, int64_t b)
{ return newiop(OP_ADD, {a, Arg(b)}, {1}); }
static inline IReg operator + (int64_t a, const IReg& b)
{ return newiop(OP_ADD, {b, Arg(a)}, {1}); }
static inline IReg operator - (const IReg& a, const IReg& b)
{ return newiop(OP_SUB, {a, b}); }
static inline IReg operator - (const IReg& a, int64_t b)
{ return newiop(OP_SUB, {a, Arg(b)}, {1}); }
static inline IReg operator - (int64_t a, const IReg& b)
{ return newiop(OP_SUB, {Arg(a), b}, {0}); }
static inline IReg operator * (const IReg& a, const IReg& b)
{ return newiop(OP_MUL, {a, b}); }
static inline IReg operator * (const IReg& a, int64_t b)
{ return newiop(OP_MUL, {a, Arg(b)}, {1}); }
static inline IReg operator * (int64_t a, const IReg& b)
{ return newiop(OP_MUL, {b, Arg(a)}, {1}); }
static inline IReg operator / (const IReg& a, const IReg& b)
{ return newiop(OP_DIV, {a, b}); }
static inline IReg operator / (const IReg& a, int64_t b)
{ return newiop(OP_DIV, {a, Arg(b)}, {1}); }
static inline IReg operator / (int64_t a, const IReg& b)
{ return newiop(OP_DIV, {Arg(a), b}, {0}); }
static inline IReg operator % (const IReg& a, const IReg& b)
{ return newiop(OP_MOD, {a, b}); }
static inline IReg operator % (const IReg& a, int64_t b)
{ return newiop(OP_MOD, {a, Arg(b)}, {1}); }
static inline IReg operator % (int64_t a, const IReg& b)
{ return newiop(OP_MOD, {Arg(a), b}, {0}); }
static inline IReg operator - (const IReg& a)
{ return newiop(OP_NEG, {a}); }
static inline IReg operator >> (const IReg& a, const IReg& b)
{ return newiop(OP_SAR, {a, b}); }
static inline IReg operator >> (const IReg& a, int64_t b)
{ return newiop(OP_SAR, {a, Arg(b)}, {1}); }
static inline IReg operator >> (int64_t a, const IReg& b)
{ return newiop(OP_SAR, {Arg(a), b}, {0}); }
static inline IReg ushift_right(const IReg& a, const IReg& b)
{ return newiop(OP_SHR, {a, b}); }
static inline IReg ushift_right(const IReg& a, int64_t b)
{ return newiop(OP_SHR, {a, Arg(b)}, {1}); }
static inline IReg ushift_right(int64_t a, const IReg& b)
{ return newiop(OP_SHR, {Arg(a), b}, {0}); }
static inline IReg operator << (const IReg& a, const IReg& b)
{ return newiop(OP_SHL, {a, b}); }
static inline IReg operator << (const IReg& a, int64_t b)
{ return newiop(OP_SHL, {a, Arg(b)}, {1}); }
static inline IReg operator << (int64_t a, const IReg& b)
{ return newiop(OP_SHL, {Arg(a), b}, {0}); }
static inline IReg operator & (const IReg& a, const IReg& b)
{ return newiop(OP_AND, {a, b}); }
static inline IReg operator & (const IReg& a, int64_t b)
{ return newiop(OP_AND, {a, Arg(b)}, {1}); }
static inline IReg operator & (int64_t a, const IReg& b)
{ return newiop(OP_AND, {b, Arg(a)}, {1}); }
static inline IReg operator | (const IReg& a, const IReg& b)
{ return newiop(OP_OR, {a, b}); }
static inline IReg operator | (const IReg& a, int64_t b)
{ return newiop(OP_OR, {a, Arg(b)}, {1}); }
static inline IReg operator | (int64_t a, const IReg& b)
{ return newiop(OP_OR, {b, Arg(a)}, {1}); }
static inline IReg operator ^ (const IReg& a, const IReg& b)
{ return newiop(OP_XOR, {a, b}); }
static inline IReg operator ^ (const IReg& a, int64_t b)
{ return newiop(OP_XOR, {a, Arg(b)}, {1}); }
static inline IReg operator ^ (int64_t a, const IReg& b)
{ return newiop(OP_XOR, {b, Arg(a)}, {1}); }
static inline IReg operator ~ (const IReg& a)
{ return newiop(OP_NOT, {a}); }

IReg operator == (const IReg& a, const IReg& b);
IReg operator == (const IReg& a, int64_t b);
static inline IReg operator == (int64_t a, const IReg& b) { return b == a; }
IReg operator != (const IReg& a, const IReg& b);
IReg operator != (const IReg& a, int64_t b);
static inline IReg operator != (int64_t a, const IReg& b) { return b != a; }
IReg operator <= (const IReg& a, const IReg& b);
IReg operator <= (const IReg& a, int64_t b);
IReg operator >= (const IReg& a, const IReg& b);
IReg operator >= (const IReg& a, int64_t b);
IReg operator > (const IReg& a, const IReg& b);
IReg operator > (const IReg& a, int64_t b);
IReg operator < (const IReg& a, const IReg& b);
IReg operator < (const IReg& a, int64_t b);
static inline IReg operator <= (int64_t a, const IReg& b) { return b >= a; }
static inline IReg operator >= (int64_t a, const IReg& b) { return b <= a; }
static inline IReg operator > (int64_t a, const IReg& b) { return b < a; }
static inline IReg operator < (int64_t a, const IReg& b) { return b > a; }
IReg select(const IReg& cond, const IReg& truev, const IReg& falsev);
IReg select(const IReg& cond, int64_t truev, const IReg& falsev);
IReg select(const IReg& cond, const IReg& truev, int64_t falsev);
static inline IReg max(const IReg& a, const IReg& b) //TODD(ch): Add imediate arguments version.
{ return newiop(OP_MAX, {a, b}); }
static inline IReg min(const IReg& a, const IReg& b)
{ return newiop(OP_MIN, {a, b}); }
static inline IReg abs(const IReg& a)
{ return newiop(OP_ABS, {a}); }
static inline IReg sign(const IReg& a)
{ return newiop(OP_SIGN, {a}); }
IReg pow(const IReg& a, int p);

static inline IReg& operator += (IReg& a, const IReg& b)
{ newiopAug(OP_ADD, {a, a, b}); return a; }
static inline IReg& operator += (IReg& a, int64_t b)
{ newiopAug(OP_ADD, {a, a, Arg(b)}, {2}); return a; }
static inline IReg& operator -= (IReg& a, const IReg& b)
{ newiopAug(OP_SUB, {a, a, b}); return a; }
static inline IReg& operator -= (IReg& a, int64_t b)
{ newiopAug(OP_SUB, {a, a, Arg(b)}, {2}); return a; }
static inline IReg& operator *= (IReg& a, const IReg& b)
{ newiopAug(OP_MUL, {a, a, b}); return a; }
static inline IReg& operator *= (IReg& a, int64_t b)
{ newiopAug(OP_MUL, {a, a, Arg(b)}, {2}); return a; }
static inline IReg& operator /= (IReg& a, const IReg& b)
{ newiopAug(OP_DIV, {a, a, b}); return a; }
static inline IReg& operator /= (IReg& a, int64_t b)
{ newiopAug(OP_DIV, {a, a, Arg(b)}, {2}); return a; }
static inline IReg& operator %= (IReg& a, const IReg& b)
{ newiopAug(OP_MOD, {a, a, b}); return a; }
static inline IReg& operator %= (IReg& a, int64_t b)
{ newiopAug(OP_MOD, {a, a, Arg(b)}, {2}); return a; }
static inline IReg& operator >>= (IReg& a, const IReg& b)
{ newiopAug(OP_SAR, {a, a, b}); return a; }
static inline IReg& operator >>= (IReg& a, int64_t b)
{ newiopAug(OP_SAR, {a, a, Arg(b)}, {2}); return a; }
static inline IReg& operator <<= (IReg& a, const IReg& b)
{ newiopAug(OP_SHL, {a, a, b}); return a; }
static inline IReg& operator <<= (IReg& a, int64_t b)
{ newiopAug(OP_SHL, {a, a, Arg(b)}, {2}); return a; }
static inline IReg& operator &= (IReg& a, const IReg& b)
{ newiopAug(OP_AND, {a, a, b}); return a; }
static inline IReg& operator &= (IReg& a, int64_t b)
{ newiopAug(OP_AND, {a, a, Arg(b)}, {2}); return a; }
static inline IReg& operator |= (IReg& a, const IReg& b)
{ newiopAug(OP_OR, {a, a, b}); return a; }
static inline IReg& operator |= (IReg& a, int64_t b)
{ newiopAug(OP_OR, {a, a, Arg(b)}, {2}); return a; }
static inline IReg& operator ^= (IReg& a, const IReg& b)
{ newiopAug(OP_XOR, {a, a, b}); return a; }
static inline IReg& operator ^= (IReg& a, int64_t b)
{ newiopAug(OP_XOR, {a, a, Arg(b)}, {2}); return a; }

///////////////////////////// vector operations ///////////////////////

// load with zero/sign extension
template<typename _Tp> VReg<_Tp> loadvec(const IReg& base)
{ return newiopV<_Tp>(VOP_LOAD, {base}); }
template<typename _Tp> VReg<_Tp> loadvec(const IReg& base, const IReg& offset)
{ return newiopV<_Tp>(VOP_LOAD, {base, offset}); }
template<typename _Tp> VReg<_Tp> loadvec(const IReg& base, int64_t offset)
{ return newiopV<_Tp>(VOP_LOAD, {base, offset}); }
template<typename _Tp> VReg<_Tp> loadlane(const IReg& base, int64_t lane_index)
{ return newiopV<_Tp>(VOP_ARM_LD1, {base, lane_index}); }

// cast and store
template<typename _Tp> void storevec(const IReg& base, const VReg<_Tp>& r)
{ newiopNoret(VOP_STORE, {base, r}); }
template<typename _Tp> void storevec(const IReg& base, const IReg& offset, const VReg<_Tp>& r)
{ newiopNoret(VOP_STORE, {base, offset, r}); }
template<typename _Tp> void storelane(const IReg& base, const VReg<_Tp>& r, int64_t lane_index)
{ newiopNoret(VOP_ARM_ST1, {base, r, lane_index}); }

template<typename _Tp> VReg<_Tp> operator + (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<_Tp>(VOP_ADD, {a, b}); }
template<typename _Tp> VReg<_Tp> operator - (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<_Tp>(VOP_SUB, {a, b}); }
template<typename _Tp> VReg<_Tp> operator * (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<_Tp>(VOP_MUL, {a, b}); }
template<typename _Tp> VReg<_Tp> operator / (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<_Tp>(VOP_DIV, {a, b}); }
template<typename _Tp> VReg<_Tp> operator - (const VReg<_Tp>& a)
{ return newiopV<_Tp>(VOP_NEG, {a}); }
template<typename _Tp> VReg<_Tp> fma(const VReg<_Tp>& a, const VReg<_Tp>& b, const VReg<_Tp>& c)
{ return newiopV<_Tp>(VOP_FMA, {a, b, c}); }

//template<typename _Tp> VReg<_Tp> add_wrap(const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> sub_wrap(const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator % (const VReg<_Tp>& a, const VReg<_Tp>& b);

//template<typename _Tp, typename _Sp> VReg<_Tp> operator >> (const VReg<_Tp>& a, const VReg<_Sp>& b)
//{
//    static_assert(sizeof(_Tp) == sizeof(_Sp), "the # of lanes in the 1st and 2nd argument must be the same");
//    return newiopV<_Tp>(VOP_SAR, {a, b});
//}
template<typename _Tp> VReg<_Tp> operator >> (const VReg<_Tp>& a, int64_t b)
{ return newiopV<_Tp>(VOP_SAR, {a, b}, {1}); }
//template<typename _Tp, typename _Sp> VReg<_Tp> ushift_right(const VReg<_Tp>& a, const VReg<_Sp>& b)
//{
//    static_assert(sizeof(_Tp) == sizeof(_Sp), "the # of lanes in the 1st and 2nd argument must be the same");
//    return newiopV<_Tp>(VOP_SHR, {a, b});
//}
template<typename _Tp> VReg<_Tp> ushift_right(const VReg<_Tp>& a, int64_t b)
{ return newiopV<_Tp>(VOP_SHR, {a, b}, {1}); }
template<typename _Tp, typename _Sp> VReg<_Tp> operator << (const VReg<_Tp>& a, const VReg<_Sp>& b)
{
    static_assert(sizeof(_Tp) == sizeof(_Sp), "the # of lanes in the 1st and 2nd argument must be the same");
    return newiopV<_Tp>(VOP_SAL, {a, b});
}
template<typename _Tp> VReg<_Tp> operator << (const VReg<_Tp>& a, int64_t b)
{ return newiopV<_Tp>(VOP_SAL, {a, b}, {1}); }
template<typename _Tp, typename _Sp> VReg<_Tp> ushift_left(const VReg<_Tp>& a, const VReg<_Sp>& b)
{
    static_assert(sizeof(_Tp) == sizeof(_Sp), "the # of lanes in the 1st and 2nd argument must be the same");
    return newiopV<_Tp>(VOP_SHL, {a, b});
}

template<typename _Tp> VReg<_Tp> operator & (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<_Tp>(VOP_AND, {a, b}); }
template<typename _Tp> VReg<_Tp> operator | (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<_Tp>(VOP_OR, {a, b}); }
template<typename _Tp> VReg<_Tp> operator ^ (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<_Tp>(VOP_XOR, {a, b}); }
template<typename _Tp> VReg<_Tp> operator ~ (const VReg<_Tp>& a)
{ return newiopV<_Tp>(VOP_NOT, {a}); }
// SSE, NEON etc. comparison operations on vectors produce vectors of the same type as the compared vectors.
template<typename _Tp> VReg<typename ElemTraits<_Tp>::masktype> operator == (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<typename ElemTraits<_Tp>::masktype>(VOP_EQ, {a, b});}
template<typename _Tp> VReg<typename ElemTraits<_Tp>::masktype> operator != (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<typename ElemTraits<_Tp>::masktype>(VOP_NE, {a, b});}
template<typename _Tp> VReg<typename ElemTraits<_Tp>::masktype> operator >= (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<typename ElemTraits<_Tp>::masktype>(VOP_GE, {a, b});}
template<typename _Tp> VReg<typename ElemTraits<_Tp>::masktype> operator <= (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<typename ElemTraits<_Tp>::masktype>(VOP_LE, {a, b});}
template<typename _Tp> VReg<typename ElemTraits<_Tp>::masktype> operator > (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<typename ElemTraits<_Tp>::masktype>(VOP_GT, {a, b});}
template<typename _Tp> VReg<typename ElemTraits<_Tp>::masktype> operator < (const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<typename ElemTraits<_Tp>::masktype>(VOP_LT, {a, b});}
//template<typename _Tp> VReg<_Tp> select(const VReg<_Tp>& flag, const VReg<_Tp>& iftrue, const VReg<_Tp>& iffalse);
template<typename _Tp> VReg<_Tp> max(const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<_Tp>(VOP_MAX, {a, b}); }
template<typename _Tp> VReg<_Tp> min(const VReg<_Tp>& a, const VReg<_Tp>& b)
{ return newiopV<_Tp>(VOP_MIN, {a, b}); }
//template<typename _Tp> VReg<_Tp> abs(const VReg<_Tp>& a);
//template<typename _Tp> VReg<_Tp> sign(const VReg<_Tp>& a);
//
template<typename _Tp> VReg<_Tp> pow(const VReg<_Tp>& a, int p);

struct exp_consts;
exp_consts expInit(Context CTX);
VReg<float> exp(const VReg<float>& x, const exp_consts& expt);

template<typename _Tp> VReg<_Tp>& operator += (VReg<_Tp>& a, const VReg<_Tp>& b)
{ newiopAug(VOP_ADD, {Arg(a), Arg(a), Arg(b)}); return a; }
template<typename _Tp> VReg<_Tp>& operator -= (VReg<_Tp>& a, const VReg<_Tp>& b)
{ newiopAug(VOP_SUB, {Arg(a), Arg(a), Arg(b)}); return a; }
template<typename _Tp> VReg<_Tp>& operator *= (VReg<_Tp>& a, const VReg<_Tp>& b)
{ newiopAug(VOP_MUL, {Arg(a), Arg(a), Arg(b)}); return a; }
template<typename _Tp> VReg<_Tp>& operator /= (VReg<_Tp>& a, const VReg<_Tp>& b)
{ newiopAug(VOP_DIV, {Arg(a), Arg(a), Arg(b)}); return a; }
//template<typename _Tp> VReg<_Tp>& operator %= (VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& a, const VReg<_Tp>& b)
//{ newiopAug(VOP_SAR, {a, a, b}); return a;}
template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& a, int64_t b)
{ newiopAug(VOP_SAR, {a, a, b}, {2}); return a; }
template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& a, const VReg<_Tp>& b)
{ newiopAug(VOP_SAL, {a, a, b}); return a;}
template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& a, int64_t b)
{ newiopAug(VOP_SAL, {a, a, b}, {2}); return a; }
template<typename _Tp> VReg<_Tp>& operator &= (VReg<_Tp>& a, const VReg<_Tp>& b)
{ newiopAug(VOP_AND, {a, a, b} ); return a; }
template<typename _Tp> VReg<_Tp>& operator |= (VReg<_Tp>& a, const VReg<_Tp>& b)
{ newiopAug(VOP_OR, {a, a, b} ); return a; }
template<typename _Tp> VReg<_Tp>& operator ^= (VReg<_Tp>& a, const VReg<_Tp>& b)
{ newiopAug(VOP_XOR, {a, a, b} ); return a; }

//// if all/any of the elements is true
//template<typename _Tp> IReg all(VReg<_Tp>& a);
//template<typename _Tp> IReg any(VReg<_Tp>& a);

// [TODO] need to add type conversion (including expansion etc.), type reinterpretation
//TODO(ch): cvtTp -> ceil, cvtTe -> round, also cast(double <=> float, float <=> f16_t)
template<typename _Dp> VReg<_Dp> trunc(const VReg<f16_t>& a)  //Convert with rounding to zero
{
    static_assert(sizeof(_Dp) == sizeof(f16_t), "Attempt to convert real number to integer of different size.");
    return newiopV<_Dp>(VOP_TRUNC, {a});
}
template<typename _Dp> VReg<_Dp> trunc(const VReg<float>& a)  //Convert with rounding to zero
{
    static_assert(sizeof(_Dp) == sizeof(float), "Attempt to convert real number to integer of different size.");
    return newiopV<_Dp>(VOP_TRUNC, {a});
}
template<typename _Dp> VReg<_Dp> trunc(const VReg<double>& a)
{
    static_assert(sizeof(_Dp) == sizeof(double), "Attempt to convert real number to integer of different size.");
    return newiopV<_Dp>(VOP_TRUNC, {a});
}
template<typename _Dp> VReg<_Dp> floor(const VReg<f16_t>& a) //Convert with rounding to minus infinity
{
    static_assert(sizeof(_Dp) == sizeof(f16_t), "Attempt to convert real number to integer of different size.");
    return newiopV<_Dp>(VOP_FLOOR, {a});
}
template<typename _Dp> VReg<_Dp> floor(const VReg<float>& a) //Convert with rounding to minus infinity
{
    static_assert(sizeof(_Dp) == sizeof(float), "Attempt to convert real number to integer of different size.");
    return newiopV<_Dp>(VOP_FLOOR, {a});
}
template<typename _Dp> VReg<_Dp> floor(const VReg<double>& a)
{
    static_assert(sizeof(_Dp) == sizeof(double), "Attempt to convert real number to integer of different size.");
    return newiopV<_Dp>(VOP_FLOOR, {a});
}

template<typename _Dp, typename _Tp> VReg<_Dp> cast(const VReg<_Tp>& a)
{ return newiopV<_Dp>(VOP_CAST, {a}); }

template<typename _Dp, typename _Tp> VReg<_Dp> reinterpret(const VReg<_Tp>& a);


//TODO(ch): These template implementations can be obviously moved to auxilary header:

Context ExtractContext(const Arg& arg);

template<typename _Tp>
VReg<_Tp>::VReg(const VReg<_Tp>& r)
{
    VReg<_Tp> selfval = newiopV<_Tp>(OP_MOV, { r });
    idx = selfval.idx;
    func = selfval.func;
}

template<typename _Tp>
VReg<_Tp>& VReg<_Tp>::operator=(const VReg<_Tp>& r)
{
    if (r.func != func)
        throw std::runtime_error("Registers of different functions as arguments of one instruction.");
    if (func == nullptr)
        throw std::runtime_error("Null motherfunction.");
    newiopNoret(OP_MOV, {*this, r});
    return (*this);
}

template<typename _Tp>
Arg::Arg(const VReg<_Tp>& vr): idx(vr.idx)
    , func(vr.func)
    , tag(VREG)
    , elemtype(ElemTraits<_Tp>::depth)
    , flags(0){}

VReg<uint8_t>  newiopV_U8  (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
VReg<int8_t>   newiopV_I8  (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
VReg<uint16_t> newiopV_U16 (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
VReg<int16_t>  newiopV_I16 (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
VReg<uint32_t> newiopV_U32 (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
VReg<int32_t>  newiopV_I32 (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
VReg<uint64_t> newiopV_U64 (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
VReg<int64_t>  newiopV_I64 (int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
VReg<f16_t>   newiopV_FP16(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
//VReg<...> newiopV_BF16(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
VReg<float>    newiopV_FP32(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});
VReg<double>   newiopV_FP64(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList = {});

template<> inline VReg<uint8_t> newiopV<uint8_t>(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{ return newiopV_U8(opcode, args, tryImmList); }
template<> inline VReg<int8_t> newiopV<int8_t>(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{ return newiopV_I8(opcode, args, tryImmList); }
template<> inline VReg<uint16_t> newiopV<uint16_t>(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{ return newiopV_U16(opcode, args, tryImmList); }
template<> inline VReg<int16_t> newiopV<int16_t>(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{ return newiopV_I16(opcode, args, tryImmList); }
template<> inline VReg<uint32_t> newiopV<uint32_t>(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{ return newiopV_U32(opcode, args, tryImmList); }
template<> inline VReg<int32_t> newiopV<int32_t>(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{ return newiopV_I32(opcode, args, tryImmList); }
template<> inline VReg<uint64_t> newiopV<uint64_t>(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{ return newiopV_U64(opcode, args, tryImmList); }
template<> inline VReg<int64_t> newiopV<int64_t>(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{ return newiopV_I64(opcode, args, tryImmList); }
template<> inline VReg<f16_t> newiopV<f16_t>(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{ return newiopV_FP16(opcode, args, tryImmList); }
template<> inline VReg<float> newiopV<float>(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{ return newiopV_FP32(opcode, args, tryImmList); }
template<> inline VReg<double> newiopV<double>(int opcode, ::std::initializer_list<Arg> args, ::std::initializer_list<size_t> tryImmList)
{ return newiopV_FP64(opcode, args, tryImmList); }

template<typename _Tp> VReg<_Tp> pow(const VReg<_Tp>& a, int p)
{
    USE_CONTEXT_(ExtractContext(a));
    if(p == 0)
        return VCONST_(_Tp, 1);
    VReg<_Tp> _a = a;
    VReg<_Tp>* pres;
    while (p)
        if (p & 1) {
            pres = new VReg<_Tp>(_a);
            --p;
            break;
        }
        else {
            _a *= _a;
            p >>= 1;
        }
    VReg<_Tp>& res = *pres;
    while (p)
        if (p & 1) {
            res *= _a;
            --p;
        }
        else {
            _a *= _a;
            p >>= 1;
        }
    VReg<_Tp> ret = static_cast<VReg<_Tp>&&>(res);
    delete pres;
    return ret;
}

struct exp_consts
{
    VReg<float> lo, hi, half, one, LOG2EF, C1, C2, p0, p1, p2, p3, p4, p5;
    VReg<int32_t> _7f;
    exp_consts(Context CTX);
};

template<typename _Dp, typename _Tp> VReg<_Dp> reinterpret(const VReg<_Tp>& a)
{
    VReg<_Dp> res;
    res.func = a.func;
    res.idx = a.idx;
    return res;
}

}
#endif
