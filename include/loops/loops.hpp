/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_LOOPS_HPP__
#define __LOOPS_LOOPS_HPP__

#include <inttypes.h>
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

    OP_AUG_ADD,
    OP_AUG_SUB,
    OP_AUG_MUL,
    OP_AUG_MUL_HIGH,
    OP_AUG_DIV,
    OP_AUG_MOD,
    OP_AUG_SHL,
    OP_AUG_SHR,
    OP_AUG_AND,
    OP_AUG_OR,
    OP_AUG_XOR,
    OP_AUG_MIN,
    OP_AUG_MAX,

    OP_JNE, //TODO(ch): I mean jump-if-not-equal, or, let's better say jump-if-true.  There is not such an instruction in real processors. It's 100% virtual, in reality we must analyze closest context.
    OP_JZ,  //TODO(ch): I mean jump-if-zero     , or, let's better say jump-if-false. There is not such an instruction in real processors. It's 100% virtual, in reality we must analyze closest context.
    OP_RET  //TODO(ch): At virtual code stage we don't know call convention and cannot understand what is the target for where we have move result. In final code it will be mov/push + ret.
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

class Context;

struct IReg
{
    IReg();
    IReg(Context* a_ctx); 

    IReg(const IReg& r); //Must generate copy(mov) code 
    IReg(IReg&& a);      //TODO(ch): Very important(!!!): this constructor must be called on every return! Must just copy fields.
    IReg& operator=(const IReg& r); // may generate real code
                                      // if 'this' is already initialized

    size_t idx; // 0 means uninitialized
    Context* ctx;
};

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
    enum { EMPTY = 0, IREG = 1, ICONST = 2 };//, VREG = 3, VCONST = 4 //TODO(ch): Uncomment. BUT!!! Before ask VP about consts!

    Arg();
    Arg(const IReg& r);
    template<typename _Tp> Arg(const VReg<_Tp>& vr);

    IReg ireg() const;
    template<typename _Tp> VReg<_Tp> vreg() const;

    int idx;
    Context* ctx;
    int tag;
    int64_t value;
};

class Func
{
    friend Func* _getImpl(const Func& wrapper);
public:
    Func();
    Func(const std::string& name, Context* ctx, std::initializer_list<IReg*> params); //TODO(ch): params or arguments? Unify names. TODO(ch): I think, this constructor must be hidden
    Func(const Func& f);
    Func& operator=(const Func& f);
    virtual ~Func();

    std::string name() const; //TODO(ch): what for we need name here? 
    void call(std::initializer_list<int64_t> args) const;
    void* ptr(); // returns function pointer
protected:
    Func* impl;
};

class Compiler
{
public:
    Compiler(const Compiler& f);
    Compiler& operator=(const Compiler& f);
    virtual ~Compiler();
    virtual void* compile(Context* ctx, Func a_func) const;
    static Compiler make_virtual_dump();
protected:
    Compiler();
private: 
    Compiler* p;
};

class Context
{
    friend Context* _getImpl(const Context& wrapper);
public:
    Context();
    Context(Compiler cmpl);
    Context(const Context& ctx);
    virtual ~Context();
    Context& operator = (const Context& ctx);

    void startfunc(const std::string& name, std::initializer_list<IReg*> params);
    void endfunc(const IReg& retval); //TODO(ch): I'm not sure about retval. There must be return_(retval) better. 
    void getfuncs(std::vector<Func>& funcs);
    Func getfunc(const std::string& name);

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
    // direct call
    IReg call_(const IReg& addr, std::initializer_list<IReg> args);
    // indirect call
    IReg call_(const IReg& addr, const IReg& offset, std::initializer_list<IReg> args);

    size_t provideIdx(); //TODO(ch) : This one must be hidden.

protected:
    Context* impl;
};

IReg newiop(int opcode, ::std::initializer_list<Arg> args);
IReg newiop(int opcode, int depth, std::initializer_list<Arg> args);

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
{ newiop(OP_STORE, depth, {base, r}); }
static inline void storex(const IReg& base, const IReg& offset, const IReg& r, int depth)
{ newiop(OP_STORE, depth, {base, offset, r}); } //TODO(ch): This code will create parasite idx, since newiop always creates new operation.
static inline void store(const IReg& base, const IReg& r)
{ newiop(OP_STORE, TYPE_I64, {base, r}); }
static inline void store(const IReg& base, const IReg& offset, const IReg& r)
{ newiop(OP_STORE, TYPE_I64, {base, offset, r}); }
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
{ newiop(OP_AUG_ADD, {a, b}); return a; } //TODO(ch): This code will create parasite idx, since newiop always creates new operation.
static inline IReg& operator -= (IReg& a, const IReg& b)
{ newiop(OP_AUG_SUB, {a, b}); return a; }
static inline IReg& operator *= (IReg& a, const IReg& b)
{ newiop(OP_AUG_MUL, {a, b}); return a; }
static inline IReg& operator /= (IReg& a, const IReg& b)
{ newiop(OP_AUG_DIV, {a, b}); return a; }
static inline IReg& operator %= (IReg& a, const IReg& b)
{ newiop(OP_AUG_MOD, {a, b}); return a; }
static inline IReg& operator >>= (IReg& a, const IReg& b)
{ newiop(OP_AUG_SHR, {a, b}); return a; }
static inline IReg& operator <<= (IReg& a, const IReg& b)
{ newiop(OP_AUG_SHL, {a, b}); return a; }
static inline IReg& operator &= (IReg& a, const IReg& b)
{ newiop(OP_AUG_AND, {a, b}); return a; }
static inline IReg& operator |= (IReg& a, const IReg& b)
{ newiop(OP_AUG_OR, {a, b}); return a; }
static inline IReg& operator ^= (IReg& a, const IReg& b)
{ newiop(OP_AUG_XOR, {a, b}); return a; }

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
