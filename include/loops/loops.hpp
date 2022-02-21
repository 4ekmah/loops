/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_LOOPS_HPP__
#define __LOOPS_LOOPS_HPP__

#include <inttypes.h>
#include <string>

namespace loops
{

enum {
    U8=0, I8=1, U16=2, I16=3, U32=4, I32=5, U64=6, I64=7,
    FP16=8, BF16=9, FP32=10, FP64=11
};

enum {
    OP_LOAD=0,
    OP_LOAD_SCALAR,
    OP_LOADX,
    OP_LOAD2X,
    OP_LOAD_DEINTERLEAVE,
    OP_STORE,
    OP_STORE_SCALAR,
    OP_STOREX,
    OP_STORE2X,
    OP_STORE_INTERLEAVE,
    OP_SET1,

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
    OP_AUG_MAX
};

template<typename _Tp> struct ElemTraits {};
template<> struct ElemTraits<uint8_t> {
    typedef uint8_t elemtype;
    enum { depth = U8, elemsize=1 };
};
template<> struct ElemTraits<int8_t> {
    typedef int8_t elemtype;
    enum { depth = I8, elemsize=1 };
};
template<> struct ElemTraits<uint16_t> {
    typedef uint16_t elemtype;
    enum { depth = U16, elemsize=2 };
};
template<> struct ElemTraits<int16_t> {
    typedef int16_t elemtype;
    enum { depth = I16, elemsize=2 };
};
template<> struct ElemTraits<uint32_t> {
    typedef uint32_t elemtype;
    enum { depth = U32, elemsize=4 };
};
template<> struct ElemTraits<int32_t> {
    typedef int32_t elemtype;
    enum { depth = I32, elemsize=4 };
};
template<> struct ElemTraits<uint64_t> {
    typedef uint64_t elemtype;
    enum { depth = U64, elemsize=8 };
};
template<> struct ElemTraits<int64_t> {
    typedef int64_t elemtype;
    enum { depth = I64, elemsize=8 };
};
template<> struct ElemTraits<float> {
    typedef float elemtype;
    enum { depth = FP32, elemsize=4 };
};
template<> struct ElemTraits<double> {
    typedef double elemtype;
    enum { depth = FP64, elemsize=8 };
};

class Context;

struct IReg
{
    IReg();
    IReg(int64_t val);
    explicit IReg(const Context& ctx);

    IReg(const IReg& r);
    IReg& operator = (const IReg& r); // may generate real code if 'this' is already initialized

    int idx; // <0 for constant-initialized registers, 0 means uninitialized
    int64_t value; // for constant-initialized registers
    Context* ctx;
};

struct Label
{
    Label();
    explicit Label(const Context& ctx);

    Label(const Label& lbl);
    Label& operator = (const Label& lbl);

    int idx;
    Context* ctx;
};

template<typename _Tp> struct VReg
{
    typedef _Tp elemtype;

    VReg();
    VReg(int idx, const Context& ctx);
    explicit VReg(const Context& ctx);
    explicit VReg(_Tp val);

    VReg(const VReg<_Tp>& r);
    VReg<_Tp>& operator = (const VReg<_Tp>& r);
    VReg<_Tp>& operator = (_Tp val);

    int idx; // <0 for constant-initialized registers, 0 means initialized
            // idx uses independent enumeration scheme from IReg::idx
    elemtype value; // for constant-initialized registers
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
    enum {EMPTY=0, IREG=1, ICONST=2, VREG=3, VCONST=4};

    Arg();
    Arg(const IReg& r);
    template<typename _Tp> Arg(const VReg<_Tp>& vr);
    template<typename _Tp> VReg<_Tp> vreg() const;
    IReg ireg() const;

    int tag;
    int depth;
    int64_t value;
};

class Func
{
public:
    Func();
    Func(const std::string& name, const Context& ctx,
         std::initializer_list<IReg&> params);
    Func(const Func& f);
    Func& operator = (const Func& f);
    virtual ~Func();

    std::string name() const;
    void call(std::initializer_list<int64_t> args) const;
    void* ptr(); // returns function pointer
protected:
    struct Impl;
    Impl* p;
};

class Context
{
public:
    Context();
    Context(const Context& ctx);
    virtual ~Context();
    Context& operator = (const Context& ctx);

    void startfunc(const std::string& name, std::initializer_list<IReg&> params);
    void endfunc(const IReg& retval);
    void getfuncs(std::vector<Func>& funcs);
    Func getfunc(const std::string& name);

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

protected:
    struct Impl;
    Impl* p;
};

Arg newop(int opcode, std::initializer_list<Arg> args);

///////////////////////////// integer operations ///////////////////////

static inline IReg load(const IReg& base)
{ return newop(OP_LOAD, {base}).ireg(); }

static inline IReg load(const IReg& base, const IReg& offset)
{ return newop(OP_LOAD, {base, offset}).ireg(); }

// load with zero/sign extension
static inline IReg loadx(const IReg& base, int depth);
static inline IReg loadx(const IReg& base, const IReg& offset, int depth);
template<typename _Tp> static inline IReg load_(const IReg& base)
{ return loadx(base, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
IReg load_(const IReg& base, const IReg& offset)
{ return loadx(base, offset, ElemTraits<_Tp>::depth); }

static inline void store(const IReg& base, const IReg& r);
static inline void store(const IReg& base, const IReg& offset, const IReg& r);
// store part of register
static inline void storex(const IReg& base, const IReg& r, int depth);
static inline void storex(const IReg& base, const IReg& offset, const IReg& r, int depth);
template<typename _Tp> static inline
void store_(const IReg& base, const IReg& r)
{ return storex(base, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
void store_(const IReg& base, const IReg& offset, const IReg& r)
{ return storex(base, offset, r, ElemTraits<_Tp>::depth); }

static inline IReg operator + (const IReg& a, const IReg& b);
static inline IReg operator - (const IReg& a, const IReg& b);
static inline IReg operator * (const IReg& a, const IReg& b);
static inline IReg operator / (const IReg& a, const IReg& b);
static inline IReg operator % (const IReg& a, const IReg& b);
static inline IReg operator >> (const IReg& a, const IReg& b);
IReg operator << (const IReg& a, const IReg& b);
IReg operator & (const IReg& a, const IReg& b);
IReg operator | (const IReg& a, const IReg& b);
IReg operator ^ (const IReg& a, const IReg& b);
IReg operator ~ (const IReg& a);
IReg operator - (const IReg& a);
IReg operator == (const IReg& a, const IReg& b);
IReg operator != (const IReg& a, const IReg& b);
IReg operator <= (const IReg& a, const IReg& b);
IReg operator >= (const IReg& a, const IReg& b);
IReg operator > (const IReg& a, const IReg& b);
IReg operator < (const IReg& a, const IReg& b);
IReg select(const IReg& flag, const IReg& iftrue, const IReg& iffalse);
IReg max(const IReg& a, const IReg& b);
IReg min(const IReg& a, const IReg& b);
IReg abs(const IReg& a);
IReg sign(const IReg& a);

IReg& operator += (IReg& a, const IReg& b);
IReg& operator -= (IReg& a, const IReg& b);
IReg& operator *= (IReg& a, const IReg& b);
IReg& operator /= (IReg& a, const IReg& b);
IReg& operator %= (IReg& a, const IReg& b);
IReg& operator >>= (IReg& a, const IReg& b);
IReg& operator <<= (IReg& a, const IReg& b);
IReg& operator &= (IReg& a, const IReg& b);
IReg& operator |= (IReg& a, const IReg& b);
IReg& operator ^= (IReg& a, const IReg& b);

///////////////////////////// vector operations ///////////////////////

template<typename _Tp> VReg<_Tp> loadv(const IReg& base);
template<typename _Tp> VReg<_Tp> loadv(const IReg& base, const IReg& offset);
// load with zero/sign extension
template<typename _Tp> VReg<_Tp> loadvx(const IReg& base, int depth);
template<typename _Tp> VReg<_Tp> loadvx(const IReg& base, const IReg& offset, int depth);

template<typename _Tp> void storev(const IReg& base, const VReg<_Tp>& r);
template<typename _Tp> void storev(const IReg& base, const IReg& offset, const IReg& r);
// cast and store
template<typename _Tp> void storevx(const IReg& base, const VReg<_Tp>& r, int depth);
template<typename _Tp> void storevx(const IReg& base, const IReg& offset, const VReg<_Tp>& r, int depth);

template<typename _Tp> VReg<_Tp> operator + (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator - (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> add_wrap(const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> sub_wrap(const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator * (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator / (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator % (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator >> (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator << (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator & (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator | (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator ^ (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator ~ (const VReg<_Tp>& a);
template<typename _Tp> VReg<_Tp> operator - (const VReg<_Tp>& a);
// SSE, NEON etc. comparison operations on vectors produce vectors of the same type as the compared vectors.
template<typename _Tp> VReg<_Tp> operator == (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator != (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator <= (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator >= (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator > (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> operator < (const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> select(const VReg<_Tp>& flag, const VReg<_Tp>& iftrue, const VReg<_Tp>& iffalse);
template<typename _Tp> VReg<_Tp> max(const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> min(const VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp> abs(const VReg<_Tp>& a);
template<typename _Tp> VReg<_Tp> sign(const VReg<_Tp>& a);

template<typename _Tp> VReg<_Tp>& operator += (VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator -= (VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator *= (VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator /= (VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator %= (VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator &= (VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator |= (VReg<_Tp>& a, const VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator ^= (VReg<_Tp>& a, const VReg<_Tp>& b);

// if all/any of the elements is true
template<typename _Tp> IReg all(VReg<_Tp>& a);
template<typename _Tp> IReg any(VReg<_Tp>& a);

// [TODO] need to add type conversion (including expansion etc.), type reinterpretation

}

#endif
