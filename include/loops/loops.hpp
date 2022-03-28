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

namespace loops
{

enum {
    TYPE_U8=0, TYPE_I8=1, TYPE_U16=2, TYPE_I16=3,
    TYPE_U32=4, TYPE_I32=5, TYPE_U64=6, TYPE_I64=7,
    TYPE_FP16=8, TYPE_BF16=9, TYPE_FP32=10, TYPE_FP64=11
};

enum {
    OP_LOAD=0,
    OP_LOADX,
    OP_LOAD_SCALAR,
    OP_LOAD2X,
    OP_LOAD_DEINTERLEAVE,
    OP_STORE,
    OP_STOREX,
    OP_STORE_SCALAR,
    OP_STORE2X,
    OP_STORE_INTERLEAVE,
    OP_CONST,

    OP_CAST,
    OP_REINTERPRET,
    OP_ROUND,
    OP_FLOOR,
    OP_CEIL,

    OP_MOV,

    OP_ADD,
    OP_ADD_WRAP,
    OP_SUB,
    OP_SUB_WRAP,
    OP_MUL,
    OP_MUL_HIGH,
    OP_MUL_EXPAND,
    OP_DIV,
    OP_MOD,
    OP_SHL,
    OP_SHR,
    OP_AND,
    OP_OR,
    OP_XOR,
    OP_NOT,
    OP_NEG,
    OP_CMP_EQ,
    OP_CMP_NE,
    OP_CMP_LT,
    OP_CMP_LE,
    OP_CMP_GE,
    OP_CMP_GT,
    OP_SELECT,
    OP_MIN,
    OP_MAX,
    OP_ABS,
    OP_SIGN,
    OP_ALL,
    OP_ANY,

    OP_JMP,
    OP_JNE, //TODO(ch): I mean jump-if-not-equal, or, let's better say jump-if-true.  There is not such an instruction in real processors. It's 100% virtual, in reality we must analyze closest context.
    OP_JZ,  //TODO(ch): I mean jump-if-zero     , or, let's better say jump-if-false. There is not such an instruction in real processors. It's 100% virtual, in reality we must analyze closest context.
    OP_RET,  //TODO(ch): At virtual code stage we don't know call convention and cannot understand what is the target for where we have move result. In final code it will be mov/push + ret.
    OP_LABEL,
    
    OP_NOINIT
};

template<typename _Tp> struct ElemTraits {};
template<> struct ElemTraits<uint8_t> {
    typedef uint8_t elemtype;
    enum { depth = TYPE_U8, elemsize=1 };
};
template<> struct ElemTraits<int8_t> {
    typedef int8_t elemtype;
    enum { depth = TYPE_I8, elemsize=1 };
};
template<> struct ElemTraits<uint16_t> {
    typedef uint16_t elemtype;
    enum { depth = TYPE_U16, elemsize=2 };
};
template<> struct ElemTraits<int16_t> {
    typedef int16_t elemtype;
    enum { depth = TYPE_I16, elemsize=2 };
};
template<> struct ElemTraits<uint32_t> {
    typedef uint32_t elemtype;
    enum { depth = TYPE_U32, elemsize=4 };
};
template<> struct ElemTraits<int32_t> {
    typedef int32_t elemtype;
    enum { depth = TYPE_I32, elemsize=4 };
};
template<> struct ElemTraits<uint64_t> {
    typedef uint64_t elemtype;
    enum { depth = TYPE_U64, elemsize=8 };
};
template<> struct ElemTraits<int64_t> {
    typedef int64_t elemtype;
    enum { depth = TYPE_I64, elemsize=8 };
};
template<> struct ElemTraits<float> {
    typedef float elemtype;
    enum { depth = TYPE_FP32, elemsize=4 };
};
template<> struct ElemTraits<double> {
    typedef double elemtype;
    enum { depth = TYPE_FP64, elemsize=8 };
};

class Func;
struct IReg
{
    IReg();

    IReg(const IReg& r); //Must generate copy(mov) code 
    IReg(IReg&& a);
    IReg& operator=(const IReg& r); // may generate real code
                                      // if 'this' is already initialized

    size_t idx;
    Func* func;
    static const size_t NOIDX;
};

class Context;
template<typename _Tp> struct VReg
{
    typedef _Tp elemtype;

    VReg();
    VReg(int idx, const Context& ctx);

    VReg(const VReg<_Tp>& r);
    VReg<_Tp>& operator = (const VReg<_Tp>& r);

    int idx; // 0 means uninitialized
    Context* ctx;
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
    enum { EMPTY = 0, IREG = 1, ICONST = 2 };//, VREG = 3, VCONST = 4 //TODO(ch): Uncomment.

    Arg();
    Arg(const IReg& r);
    Arg(int64_t a_value);
    template<typename _Tp> Arg(const VReg<_Tp>& vr);

    IReg ireg() const; //TODO(ch): what for?
    template<typename _Tp> VReg<_Tp> vreg() const;

    size_t idx; //TODO(ch): IRegInternal??? Or, it's better to create ArgInternal?
    Func* func;
    size_t tag;
    int64_t value;
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
    void* ptr(); // returns function pointer
    void printBytecode(std::ostream& out) const;
    void printAssembly(std::ostream& out) const;
    void printTargetHex(std::ostream& out) const; //IMPORTANT: delete this one and implement table-printer in printAssembly.

protected:
    static Func make(Func* a_wrapped);
    Func* impl;
};

class Backend
{
    friend Backend* _getImpl(Backend* wrapper);
public:
    Backend(const Backend& f);
    Backend& operator=(const Backend& f);
    virtual ~Backend();
    virtual void* compile(Context* a_ctx, Func* a_func) const;
    static Backend makeAarch64Compiler();
protected:
    Backend();
private:
    Backend(Backend* a_p);
    Backend* impl;
};

class Context
{
    friend Context* _getImpl(Context* wrapper);
public:
    Context();
    Context(Backend cmpl);
    Context(const Context& ctx);
    virtual ~Context();
    Context& operator=(const Context& ctx);

    void startFunc(const std::string& name, std::initializer_list<IReg*> params);
    void endFunc();
    void getFuncs(std::vector<Func>& funcs);
    Func getFunc(const std::string& name);

    IReg const_(int64_t value);
    template<typename _Tp> VReg<_Tp> vconst_(_Tp value);
    IReg ireg_();
    template<typename _Tp> VReg<_Tp> vreg_();

    // control flow
    void do_();
    void do_if_(const IReg& r); // start loop if r == true
    void while_(const IReg& r); // continue loop if r == true
    void break_();
    void continue_();
    void if_(const IReg& r);
    void elif_(const IReg& r);
    void else_();
    void endif_();
    void return_(const IReg& retval);
    void return_();
    // direct call
    IReg call_(const IReg& addr, std::initializer_list<IReg> args);
    // indirect call
    IReg call_(const IReg& addr, const IReg& offset, std::initializer_list<IReg> args);
protected:
    Context* impl;
};

IReg newiop(int opcode, ::std::initializer_list<Arg> args);
IReg newiop(int opcode, int depth, std::initializer_list<Arg> args);
void newiopNoret(int opcode, ::std::initializer_list<Arg> args);
void newiopNoret(int opcode, int depth, std::initializer_list<Arg> args);
void newiopAug(int opcode, ::std::initializer_list<Arg> args);

///////////////////////////// integer operations ///////////////////////

// load with zero/sign extension
static inline IReg loadx(const IReg& base, int depth)
{ return newiop(OP_LOAD, depth, {base}); }
static inline IReg loadx(const IReg& base, const IReg& offset, int depth)
{ return newiop(OP_LOAD, depth, {base, offset}); }

static inline IReg load(const IReg& base)
{ return static_cast<IReg&&>(newiop(OP_LOAD, TYPE_I64, {base})); }
static inline IReg load(const IReg& base, const IReg& offset)
{ return newiop(OP_LOAD, TYPE_I64, {base, offset}); }

template<typename _Tp> static inline IReg load_(const IReg& base)
{ return loadx(base, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
IReg load_(const IReg& base, const IReg& offset)
{ return static_cast<IReg&&>(loadx(base, offset, ElemTraits<_Tp>::depth)); }

// store part of register
static inline void storex(const IReg& base, const IReg& r, int depth)
{ newiopNoret(OP_STORE, depth, {base, r}); }
static inline void storex(const IReg& base, const IReg& offset, const IReg& r, int depth)
{ newiopNoret(OP_STORE, depth, {base, offset, r}); }
static inline void store(const IReg& base, const IReg& r)
{ newiopNoret(OP_STORE, TYPE_I64, {base, r}); }
static inline void store(const IReg& base, const IReg& offset, const IReg& r)
{ newiopNoret(OP_STORE, TYPE_I64, {base, offset, r}); }
template<typename _Tp> static inline
void store_(const IReg& base, const IReg& r)
{ storex(base, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
void store_(const IReg& base, const IReg& offset, const IReg& r)
{ storex(base, offset, r, ElemTraits<_Tp>::depth); }

static inline IReg operator + (const IReg& a, const IReg& b)
{ return newiop(OP_ADD, {a, b}); }
static inline IReg operator - (const IReg& a, const IReg& b)
{ return newiop(OP_SUB, {a, b}); }
static inline IReg operator * (const IReg& a, const IReg& b)
{ return newiop(OP_MUL, {a, b}); }
static inline IReg operator / (const IReg& a, const IReg& b)
{ return newiop(OP_DIV, {a, b}); }
static inline IReg operator % (const IReg& a, const IReg& b)
{ return newiop(OP_MOD, {a, b}); }
static inline IReg operator >> (const IReg& a, const IReg& b)
{ return newiop(OP_SHR, {a, b}); }
static inline IReg operator << (const IReg& a, const IReg& b)
{ return newiop(OP_SHL, {a, b}); }
static inline IReg operator & (const IReg& a, const IReg& b)
{ return newiop(OP_AND, {a, b}); }
static inline IReg operator | (const IReg& a, const IReg& b)
{ return newiop(OP_OR, {a, b}); }
static inline IReg operator ^ (const IReg& a, const IReg& b)
{ return newiop(OP_XOR, {a, b}); }
static inline IReg operator ~ (const IReg& a)
{ return newiop(OP_NOT, {a}); }
static inline IReg operator - (const IReg& a)
{ return newiop(OP_NEG, {a}); }
static inline IReg operator == (const IReg& a, const IReg& b)
{ return newiop(OP_CMP_EQ, {a, b}); }
static inline IReg operator != (const IReg& a, const IReg& b)
{ return newiop(OP_CMP_NE, {a, b}); }
static inline IReg operator <= (const IReg& a, const IReg& b)
{ return newiop(OP_CMP_LE, {a, b}); }
static inline IReg operator >= (const IReg& a, const IReg& b)
{ return newiop(OP_CMP_GE, {a, b}); }
static inline IReg operator > (const IReg& a, const IReg& b)
{ return newiop(OP_CMP_GT, {a, b}); }
static inline IReg operator < (const IReg& a, const IReg& b)
{ return newiop(OP_CMP_LT, {a, b}); }
static inline IReg select(const IReg& flag, const IReg& iftrue, const IReg& iffalse)
{ return newiop(OP_SELECT, {flag, iftrue, iffalse}); }
static inline IReg max(const IReg& a, const IReg& b)
{ return newiop(OP_MAX, {a, b}); }
static inline IReg min(const IReg& a, const IReg& b)
{ return newiop(OP_MIN, {a, b}); }
static inline IReg abs(const IReg& a)
{ return newiop(OP_ABS, {a}); }
static inline IReg sign(const IReg& a)
{ return newiop(OP_SIGN, {a}); }

static inline IReg& operator += (IReg& a, const IReg& b)
{ newiopAug(OP_ADD, {a, a, b}); return a; }
static inline IReg& operator -= (IReg& a, const IReg& b)
{ newiopAug(OP_SUB, {a, a, b}); return a; }
static inline IReg& operator *= (IReg& a, const IReg& b)
{ newiopAug(OP_MUL, {a, a, b}); return a; }
static inline IReg& operator /= (IReg& a, const IReg& b)
{ newiopAug(OP_DIV, {a, a, b}); return a; }
static inline IReg& operator %= (IReg& a, const IReg& b)
{ newiopAug(OP_MOD, {a, a, b}); return a; }
static inline IReg& operator >>= (IReg& a, const IReg& b)
{ newiopAug(OP_SHR, {a, a, b}); return a; }
static inline IReg& operator <<= (IReg& a, const IReg& b)
{ newiopAug(OP_SHL, {a, a, b}); return a; }
static inline IReg& operator &= (IReg& a, const IReg& b)
{ newiopAug(OP_AND, {a, a, b}); return a; }
static inline IReg& operator |= (IReg& a, const IReg& b)
{ newiopAug(OP_OR, {a, a, b}); return a; }
static inline IReg& operator ^= (IReg& a, const IReg& b)
{ newiopAug(OP_XOR, {a, a, b}); return a; }

///////////////////////////// vector operations ///////////////////////

//template<typename _Tp> VReg<_Tp> loadv(const IReg& base);
//template<typename _Tp> VReg<_Tp> loadv(const IReg& base, const IReg& offset);
//// load with zero/sign extension
//template<typename _Tp> VReg<_Tp> loadvx(const IReg& base, int depth);
//template<typename _Tp> VReg<_Tp> loadvx(const IReg& base, const IReg& offset, int depth);
//
//template<typename _Tp> void storev(const IReg& base, const VReg<_Tp>& r);
//template<typename _Tp> void storev(const IReg& base, const IReg& offset, const IReg& r);
//// cast and store
//template<typename _Tp> void storevx(const IReg& base, const VReg<_Tp>& r, int depth);
//template<typename _Tp> void storevx(const IReg& base, const IReg& offset, const VReg<_Tp>& r, int depth);
//
//template<typename _Tp> VReg<_Tp> operator + (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator - (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> add_wrap(const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> sub_wrap(const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator * (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator / (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator % (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator >> (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator << (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator & (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator | (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator ^ (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator ~ (const VReg<_Tp>& a);
//template<typename _Tp> VReg<_Tp> operator - (const VReg<_Tp>& a);
//// SSE, NEON etc. comparison operations on vectors produce vectors of the same type as the compared vectors.
//template<typename _Tp> VReg<_Tp> operator == (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator != (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator <= (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator >= (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator > (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> operator < (const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> select(const VReg<_Tp>& flag, const VReg<_Tp>& iftrue, const VReg<_Tp>& iffalse);
//template<typename _Tp> VReg<_Tp> max(const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> min(const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> abs(const VReg<_Tp>& a);
//template<typename _Tp> VReg<_Tp> sign(const VReg<_Tp>& a);
//
//template<typename _Tp> VReg<_Tp>& operator += (VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator -= (VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator *= (VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator /= (VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator %= (VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator &= (VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator |= (VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator ^= (VReg<_Tp>& a, const VReg<_Tp>& b);
//
//// if all/any of the elements is true
//template<typename _Tp> IReg all(VReg<_Tp>& a);
//template<typename _Tp> IReg any(VReg<_Tp>& a);

// [TODO] need to add type conversion (including expansion etc.), type reinterpretation

}

#endif
