
/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_LOOPS_IPP__
#define __LOOPS_LOOPS_IPP__

#include <inttypes.h>
#include <ostream>
#include <string>
#include <cstring>
#include <vector>
#include <stdexcept>
#include "defines.hpp"

namespace loops
{
//template<typename _Tp> struct ElemTraits {};
template<> struct ElemTraits<uint8_t> {
    typedef uint8_t elemtype;
    typedef uint8_t masktype;
    typedef int8_t countertype;
    typedef uint16_t duplicatetype;
    enum { depth = TYPE_U8, elemsize=1 };
};
template<> struct ElemTraits<int8_t> {
    typedef int8_t elemtype;
    typedef uint8_t masktype;
    typedef int8_t countertype;
    typedef int16_t duplicatetype;
    enum { depth = TYPE_I8, elemsize=1 };
};
template<> struct ElemTraits<uint16_t> {
    typedef uint16_t elemtype;
    typedef uint16_t masktype;
    typedef int16_t countertype;
    typedef uint32_t duplicatetype;
    typedef uint8_t halftype;
    enum { depth = TYPE_U16, elemsize=2 };
};
template<> struct ElemTraits<int16_t> {
    typedef int16_t elemtype;
    typedef uint16_t masktype;
    typedef int16_t countertype;
    typedef int32_t duplicatetype;
    typedef int8_t halftype;
    enum { depth = TYPE_I16, elemsize=2 };
};
template<> struct ElemTraits<uint32_t> {
    typedef uint32_t elemtype;
    typedef uint32_t masktype;
    typedef int32_t countertype;
    typedef uint64_t duplicatetype;
    typedef uint16_t halftype;
    enum { depth = TYPE_U32, elemsize=4 };
};
template<> struct ElemTraits<int32_t> {
    typedef int32_t elemtype;
    typedef uint32_t masktype;
    typedef int32_t countertype;
    typedef int64_t duplicatetype;
    typedef int16_t halftype;
    enum { depth = TYPE_I32, elemsize=4 };
};
template<> struct ElemTraits<uint64_t> {
    typedef uint64_t elemtype;
    typedef uint64_t masktype;
    typedef int64_t countertype;
    typedef uint32_t halftype;
    enum { depth = TYPE_U64, elemsize=8 };
};
template<> struct ElemTraits<int64_t> {
    typedef int64_t elemtype;
    typedef uint64_t masktype;
    typedef int64_t countertype;
    typedef int32_t halftype;
    enum { depth = TYPE_I64, elemsize=8 };
};
template<> struct ElemTraits<f16_t> {
    typedef float elemtype;
    typedef uint16_t masktype;
    typedef int16_t countertype;
    enum { depth = TYPE_FP16, elemsize=2 };
};
template<> struct ElemTraits<float> {
    typedef float elemtype;
    typedef uint32_t masktype;
    typedef int32_t countertype;
    enum { depth = TYPE_FP32, elemsize=4 };
};
template<> struct ElemTraits<double> {
    typedef double elemtype;
    typedef uint64_t masktype;
    typedef int64_t countertype;
    enum { depth = TYPE_FP64, elemsize=8 };
};

void VReg_constr_(const Expr& fromwho, int& idx, Func*& func, int restype);

template<typename _Tp>
VReg<_Tp>::VReg(const VReg<_Tp>& r)
{
    if(r.func != nullptr)
    {
        Expr fromwho(r);
        fromwho.func() = r.func;
        VReg_constr_(fromwho, idx, func, ElemTraits<_Tp>::depth);
    }
    else
    {
        idx = NOIDX;
        func = nullptr;
    }
}

template<typename _Tp>
VReg<_Tp>::VReg(const VExpr<_Tp>& fromwho)
{
    VReg_constr_(fromwho.notype(), idx, func, ElemTraits<_Tp>::depth);
}

template<typename _Tp>
VReg<_Tp>& VReg<_Tp>::operator=(const VReg<_Tp>& r)
{
    VExpr<_Tp> fromwho = VExpr<_Tp>(r);
    return operator=(fromwho);
}

void VReg_assign_(const Expr& target, const Expr& from);

template<typename _Tp>
VReg<_Tp>& VReg<_Tp>::operator=(const VExpr<_Tp>& fromwho)
{
    VReg_assign_(VExpr<_Tp>(*this).notype(), fromwho.notype());
    return (*this);
}

template<typename _Tp>
void VReg<_Tp>::copyidx(const VReg<_Tp>& from)
{
    if(func != nullptr && func != from.func)
        throw std::runtime_error("Registers of different functions in idx assignment.");
    func = from.func;
    idx = from.idx;
}

void VReg_copyidx_(const Expr& fromwho, int& idx, Func*& func);

template<typename _Tp>
void VReg<_Tp>::copyidx(const VExpr<_Tp>& from)
{
    if(from.is_leaf() && from.leaf().tag == Arg::VREG)
        VReg_copyidx_(from.notype(),idx,func); 
    else
        copyidx(VReg<_Tp>(from));
}

Arg::Arg() : idx(IReg::NOIDX), tag(EMPTY), value(0), flags(0), elemtype(-1) {}

Arg::Arg(const IReg& r) : idx(r.idx), tag(r.func ? Arg::IREG : Arg::EMPTY), value(0), flags(0) {}

template<typename _Tp>
Arg::Arg(const VReg<_Tp>& vr): idx(vr.idx)
    , tag(VREG)
    , elemtype(ElemTraits<_Tp>::depth)
    , flags(0){}

struct __loops_ExprStr_
{
    int opcode;
    bool is_vector;
    int type;
    Arg leaf;
    int refcounter;
    Func* func;
    std::vector<Expr> children;
    __loops_ExprStr_():refcounter(0), func(nullptr) {}
};

Expr::Expr(): pointee(NULL) {}
Expr::Expr(const Expr& fromwho):pointee(fromwho.pointee) { if(pointee) pointee->refcounter++; }
Expr& Expr::operator=(const Expr& fromwho)
{
    if(pointee!=fromwho.pointee)
    {
        if(pointee) { if((--(pointee->refcounter)) == 0) { delete pointee; } }
        pointee = fromwho.pointee;
        if(pointee) pointee->refcounter++;
    }
    return *this;
}

Expr::Expr(const Arg& a_leaf): pointee(new __loops_ExprStr_)
{
    pointee->refcounter = 1;
    pointee->opcode = EXPR_LEAF;
    pointee->type = a_leaf.elemtype;
    pointee->is_vector = a_leaf.tag == Arg::VREG;
    pointee->leaf = a_leaf;
}

Expr::Expr(int64_t a_leaf): pointee(new __loops_ExprStr_)
{
    pointee->refcounter = 1;
    pointee->opcode = EXPR_LEAF;
    pointee->is_vector = false;
    pointee->leaf = Arg(a_leaf);
}

Expr::~Expr() { if(pointee) { if(--(pointee->refcounter) == 0) delete pointee; } }
int& Expr::opcode() { if(!pointee) throw std::runtime_error("Null pointer in Expr."); return pointee->opcode; }
bool& Expr::is_vector() { if(!pointee) throw std::runtime_error("Null pointer in Expr."); return pointee->is_vector; }
bool Expr::is_leaf() const {return opcode() == EXPR_LEAF; }
int& Expr::type() { if(!pointee) throw std::runtime_error("Null pointer in Expr."); return pointee->type;}
Arg& Expr::leaf()
{
    if(!pointee) throw std::runtime_error("Null pointer in Expr.");
    if(pointee->opcode != EXPR_LEAF) throw std::runtime_error("Interpretting leaf node as branch.");
    return pointee->leaf;
}

Func*& Expr::func() { if(!pointee) throw std::runtime_error("Null pointer in Expr."); return pointee->func;}

std::vector<Expr>& Expr::children()
{
    if(!pointee) throw std::runtime_error("Null pointer in Expr.");
    if(pointee->opcode == EXPR_LEAF) throw std::runtime_error("Interpretting leaf node as branch.");
    return pointee->children;
}

int Expr::opcode() const { if(!pointee) throw std::runtime_error("Null pointer in Expr."); return pointee->opcode; }
bool Expr::is_vector() const { if(!pointee) throw std::runtime_error("Null pointer in Expr."); return pointee->is_vector; }
int Expr::type() const { if(!pointee) throw std::runtime_error("Null pointer in Expr."); return pointee->type;}
const Arg& Expr::leaf() const
{
    if(!pointee) throw std::runtime_error("Null pointer in Expr.");
    if(pointee->opcode != EXPR_LEAF) throw std::runtime_error("Interpretting leaf node as branch.");
    return pointee->leaf;
}
const std::vector<Expr>& Expr::children() const
{
    if(!pointee) throw std::runtime_error("Null pointer in Expr.");
    if(pointee->opcode == EXPR_LEAF) throw std::runtime_error("Interpretting leaf node as branch.");
    return pointee->children;
}

Func* Expr::func() const { if(!pointee) throw std::runtime_error("Null pointer in Expr."); return pointee->func;}

bool Expr::empty() const { return pointee == nullptr; }

inline IExpr::IExpr(const IReg& a_leaf)
{
    super.pointee = new __loops_ExprStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = EXPR_LEAF;
    super.pointee->is_vector = false;
    super.pointee->type = TYPE_I64;
    super.pointee->leaf = Arg(a_leaf);
    super.pointee->func = a_leaf.func;
}

inline IExpr::IExpr(int a_opcode, int a_type, std::initializer_list<Expr> a_children)
{
    super.pointee = new __loops_ExprStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = a_opcode;
    super.pointee->is_vector = false;
    super.pointee->type = a_type;
    super.pointee->children = a_children;
    super.infer_owner();
}

inline IExpr::IExpr(int a_opcode, int a_type, std::vector<Expr> a_children)
{
    super.pointee = new __loops_ExprStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = a_opcode;
    super.pointee->is_vector = false;
    super.pointee->type = a_type;
    super.pointee->children = a_children;
    super.infer_owner();
}

template <typename _Tp>
inline VExpr<_Tp>::VExpr(const VReg<_Tp>& a_leaf)
{
    super.pointee = new __loops_ExprStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = EXPR_LEAF;
    super.pointee->is_vector = true;
    super.pointee->type = ElemTraits<_Tp>::depth;
    super.pointee->leaf = Arg(a_leaf);
    super.pointee->func = a_leaf.func;
}

template <typename _Tp>
inline VExpr<_Tp>::VExpr(int a_opcode, std::initializer_list<Expr> a_children)
{
    super.pointee = new __loops_ExprStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = a_opcode;
    super.pointee->is_vector = true;
    super.pointee->type = ElemTraits<_Tp>::depth;
    super.pointee->children = a_children;
    super.infer_owner();
}

template <typename _Tp>
inline VExpr<_Tp>::VExpr(int a_opcode, std::vector<Expr> a_children)
{
    super.pointee = new __loops_ExprStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = a_opcode;
    super.pointee->is_vector = true;
    super.pointee->type = ElemTraits<_Tp>::depth;
    super.pointee->children = a_children;
    super.infer_owner();
}

struct __Loops_CondPrefixMarker_
{
    __Loops_CondPrefixMarker_(Context& CTX_);
    Context* CTX;
};

struct __Loops_CFScopeBracket_
{
    enum CFType {IF, ELIF, ELSE, WHILE };
    explicit __Loops_CFScopeBracket_(const __Loops_CondPrefixMarker_& inh, CFType _cftype, const IExpr& condition);
    explicit __Loops_CFScopeBracket_(Context& inh);
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

struct __Loops_CF_rvalue_
{
    __Loops_CF_rvalue_(Context* _CTX);
    Context* CTX;
    void break_();
    void continue_();
    void return_();
    void return_(const IExpr& r);
    void return_(int64_t r);
    void call_(funcptr0_noret_t fptr);
    void call_(funcptr1_noret_t fptr, const IExpr& arg0);
    void call_(funcptr2_noret_t fptr, const IExpr& arg0, const IExpr& arg1);
    void call_(funcptr3_noret_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2);
    void call_(funcptr4_noret_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3);
    void call_(funcptr5_noret_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4);
    void call_(funcptr6_noret_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5);
    void call_(funcptr7_noret_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5, const IExpr& arg6);
    void call_(funcptr8_noret_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5, const IExpr& arg6, const IExpr& arg7);
    IReg call_(funcptr0_t fptr);
    IReg call_(funcptr1_t fptr, const IExpr& arg0);
    IReg call_(funcptr2_t fptr, const IExpr& arg0, const IExpr& arg1);
    IReg call_(funcptr3_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2);
    IReg call_(funcptr4_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3);
    IReg call_(funcptr5_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4);
    IReg call_(funcptr6_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5);
    IReg call_(funcptr7_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5, const IExpr& arg6);
    IReg call_(funcptr8_t fptr, const IExpr& arg0, const IExpr& arg1, const IExpr& arg2, const IExpr& arg3, const IExpr& arg4, const IExpr& arg5, const IExpr& arg6, const IExpr& arg7);
};

void __setfunc_by_context_(Context* CTX, Expr& expr);

static inline IExpr __loops_const_(Context* CTX, int64_t _val)
{
    Expr val(_val);
    __setfunc_by_context_(CTX, val);
    return IExpr(OP_MOV, TYPE_I64, {val});
}

static inline IExpr __loops_def_(Context* CTX)
{
    IExpr res(OP_DEF, TYPE_I64, {});
    __setfunc_by_context_(CTX, res.super);
    return res;
}

IExpr __loops_const_(Context* CTX, int64_t _val);
IExpr __loops_def_(Context* CTX);

template<typename _Tp>
VExpr<_Tp> __loops_vconst_(Context* CTX, _Tp _val)
{
    int64_t val64 = 0;
    *(reinterpret_cast<_Tp*>(&val64)) = _val;
    Expr val(val64);
    __setfunc_by_context_(CTX, val);
    return VExpr<_Tp>(OP_MOV, {val});
}

template<typename _Tp>
VExpr<_Tp> __loops_vdef_(Context* CTX)
{
    VExpr<_Tp> res(VOP_DEF, {});
    __setfunc_by_context_(CTX, res.super);
    return res;
}

void newiopNoret(int opcode, ::std::initializer_list<Expr> args);

// void newiopNoret(int opcode, ::std::initializer_list<Expr> args);
///////////////////////////// integer operations ///////////////////////
// Load with zero/sign extension:
static inline IExpr loadx(const IExpr& base, int depth)
{ return IExpr(OP_LOAD, depth, {base.notype()}); }
static inline IExpr loadx(const IExpr& base, const IExpr& offset, int depth)
{ return IExpr(OP_LOAD, depth, {base.notype(), offset.notype()}); }
static inline IExpr loadx(const IExpr& base, int64_t offset, int depth)
{ return IExpr(OP_LOAD, depth, {base.notype(), Expr(offset)}); }

template<typename _Tp> IExpr load_(const IExpr& base)
{ return loadx(base, ElemTraits<_Tp>::depth); }
template<typename _Tp> IExpr load_(const IExpr& base, const IExpr& offset)
{ return loadx(base, offset, ElemTraits<_Tp>::depth); }
template<typename _Tp> IExpr load_(const IExpr& base, int64_t offset)
{ return loadx(base, offset, ElemTraits<_Tp>::depth); }

// store part of register
static inline void storex(const IExpr& base, const IExpr& r, int depth)
{
    Expr r_(r.notype());
    r_.type() = depth;
    newiopNoret(OP_STORE, {base.notype(), r_});
}
static inline void storex(const IExpr& base, int64_t a, int depth)
{
    Expr r_(a);
    r_.type() = depth;
    newiopNoret(OP_STORE, {base.notype(), r_});
}
static inline void storex(const IExpr& base, const IExpr& offset, const IExpr& r, int depth)
{
    Expr r_(r.notype());
    r_.type() = depth;
    newiopNoret(OP_STORE, {base.notype(), offset.notype(), r_});
}
static inline void storex(const IExpr& base, int64_t offset, const IExpr& r, int depth)
{
    Expr r_(r.notype());
    r_.type() = depth;
    newiopNoret(OP_STORE, {base.notype(), Expr(offset), r_});
}
static inline void storex(const IExpr& base, const IExpr& offset, int64_t a, int depth)
{
    Expr r_(a);
    r_.type() = depth;
    newiopNoret(OP_STORE, {base.notype(), offset.notype(), r_});
}
static inline void storex(const IExpr& base, int64_t offset, int64_t a, int depth)
{
    Expr r_(a);
    r_.type() = depth;
    newiopNoret(OP_STORE, {base.notype(), Expr(offset), r_});
}
static inline void store(const IExpr& base, const IExpr& r)
{ storex(base, r, TYPE_I64); }
static inline void store(const IExpr& base, const IExpr& offset, const IExpr& r)
{ storex(base, offset, r, TYPE_I64);}
template<typename _Tp> void store_(const IExpr& base, const IExpr& r)
{ storex(base, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> void store_(const IExpr& base, int64_t a)
{ storex(base, a, ElemTraits<_Tp>::depth); }
template<typename _Tp> void store_(const IExpr& base, const IExpr& offset, const IExpr& r)
{ storex(base, offset, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> void store_(const IExpr& base, int64_t offset, const IExpr& r)
{ storex(base, offset, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> void store_(const IExpr& base, const IExpr& offset, int64_t a)
{ storex(base, offset, a, ElemTraits<_Tp>::depth); }
template<typename _Tp> void store_(const IExpr& base, int64_t offset, int64_t a)
{ storex(base, offset, a, ElemTraits<_Tp>::depth); }

// Integer arithmetic and bitwise operations:
static inline IExpr operator + (const IExpr& a, const IExpr& b)
{ return IExpr(OP_ADD, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator + (const IExpr& a, int64_t b)
{ return IExpr(OP_ADD, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator + (int64_t a, const IExpr& b)
{ return IExpr(OP_ADD, b.type(), {b.notype(), Expr(a)}); }
static inline IExpr operator - (const IExpr& a, const IExpr& b)
{ return IExpr(OP_SUB, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator - (const IExpr& a, int64_t b)
{ return IExpr(OP_SUB, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator - (int64_t a, const IExpr& b)
{ return IExpr(OP_SUB, b.type(), {Expr(a), b.notype()}); }
static inline IExpr operator * (const IExpr& a, const IExpr& b)
{ return IExpr(OP_MUL, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator * (const IExpr& a, int64_t b)
{ return IExpr(OP_MUL, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator * (int64_t a, const IExpr& b)
{ return IExpr(OP_MUL, b.type(), {b.notype(), Expr(a)}); }
static inline IExpr operator / (const IExpr& a, const IExpr& b)
{ return IExpr(OP_DIV, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator / (const IExpr& a, int64_t b)
{ return IExpr(OP_DIV, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator / (int64_t a, const IExpr& b)
{ return IExpr(OP_DIV, b.type(), {Expr(a), b.notype()}); }
static inline IExpr operator % (const IExpr& a, const IExpr& b)
{ return IExpr(OP_MOD, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator % (const IExpr& a, int64_t b)
{ return IExpr(OP_MOD, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator % (int64_t a, const IExpr& b)
{ return IExpr(OP_MOD, b.type(), {Expr(a), b.notype()}); }
static inline IExpr operator - (const IExpr& a)
{ return IExpr(OP_NEG, a.type(), {a.notype()}); }
static inline IExpr operator >> (const IExpr& a, const IExpr& b)
{ return IExpr(OP_SAR, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator >> (const IExpr& a, int64_t b)
{ return IExpr(OP_SAR, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator >> (int64_t a, const IExpr& b)
{ return IExpr(OP_SAR, b.type(), {Expr(a), b.notype()}); }
static inline IExpr ushift_right(const IExpr& a, const IExpr& b)
{ return IExpr(OP_SHR, a.type(), {a.notype(), b.notype()}); }
static inline IExpr ushift_right(const IExpr& a, int64_t b)
{ return IExpr(OP_SHR, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr ushift_right(int64_t a, const IExpr& b)
{ return IExpr(OP_SHR, b.type(), {Expr(a), b.notype()}); }
static inline IExpr operator <<(const IExpr& a, const IExpr& b)
{ return IExpr(OP_SHL, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator <<(const IExpr& a, int64_t b)
{ return IExpr(OP_SHL, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator <<(int64_t a, const IExpr& b)
{ return IExpr(OP_SHL, b.type(), {Expr(a), b.notype()}); }
static inline IExpr operator & (const IExpr& a, const IExpr& b)
{ return IExpr(OP_AND, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator & (const IExpr& a, int64_t b)
{ return IExpr(OP_AND, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator & (int64_t a, const IExpr& b)
{ return IExpr(OP_AND, b.type(), {b.notype(), Expr(a)}); }
static inline IExpr operator | (const IExpr& a, const IExpr& b)
{ return IExpr(OP_OR, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator | (const IExpr& a, int64_t b)
{ return IExpr(OP_OR, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator | (int64_t a, const IExpr& b)
{ return IExpr(OP_OR, b.type(), {b.notype(), Expr(a)}); }
static inline IExpr operator ^ (const IExpr& a, const IExpr& b)
{ return IExpr(OP_XOR, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator ^ (const IExpr& a, int64_t b)
{ return IExpr(OP_XOR, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator ^ (int64_t a, const IExpr& b)
{ return IExpr(OP_XOR, b.type(), {b.notype(), Expr(a)}); }
static inline IExpr operator ~ (const IExpr& a)
{ return IExpr(OP_NOT, a.type(), {a.notype()}); }

// Comparisson and logical operations:
static inline IExpr operator == (const IExpr& a, const IExpr& b)
{ return IExpr(OP_EQ, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator == (const IExpr& a, int64_t b)
{ return IExpr(OP_EQ, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator != (const IExpr& a, const IExpr& b)
{ return IExpr(OP_NE, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator != (const IExpr& a, int64_t b)
{ return IExpr(OP_NE, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator <= (const IExpr& a, const IExpr& b)
{ return IExpr(OP_LE, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator <= (const IExpr& a, int64_t b)
{ return IExpr(OP_LE, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr ule(const IExpr& a, const IExpr& b)
{ return IExpr(OP_ULE, a.type(), {a.notype(), b.notype()}); }
static inline IExpr ule(const IExpr& a, int64_t b)
{ return IExpr(OP_ULE, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator >= (const IExpr& a, const IExpr& b)
{ return IExpr(OP_GE, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator >= (const IExpr& a, int64_t b)
{ return IExpr(OP_GE, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr uge(const IExpr& a, const IExpr& b)
{ return IExpr(OP_ULE, a.type(), {b.notype(), a.notype()}); } //TODO(ch): Uge -> ule implementation is formed by ARM. Check for better ideas on Intel.
static inline IExpr uge(const IExpr& a, int64_t b)
{ return IExpr(OP_ULE, a.type(), {Expr(b), a.notype()}); }
static inline IExpr operator > (const IExpr& a, const IExpr& b)
{ return IExpr(OP_GT, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator > (const IExpr& a, int64_t b)
{ return IExpr(OP_GT, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr ugt(const IExpr& a, const IExpr& b)
{ return IExpr(OP_UGT, a.type(), {a.notype(), b.notype()}); }
static inline IExpr ugt(const IExpr& a, int64_t b)
{ return IExpr(OP_UGT, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr operator < (const IExpr& a, const IExpr& b)
{ return IExpr(OP_LT, a.type(), {a.notype(), b.notype()}); }
static inline IExpr operator < (const IExpr& a, int64_t b)
{ return IExpr(OP_LT, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr ult(const IExpr& a, const IExpr& b)
{ return IExpr(OP_UGT, a.type(), {b.notype(), a.notype()}); }
static inline IExpr ult(const IExpr& a, int64_t b)
{ return IExpr(OP_UGT, a.type(), {Expr(b), a.notype()}); }
static inline IExpr operator == (int64_t a, const IExpr& b) { return b == a; }
static inline IExpr operator != (int64_t a, const IExpr& b) { return b != a; }
static inline IExpr operator <= (int64_t a, const IExpr& b) { return b >= a; }
static inline IExpr ule         (int64_t a, const IExpr& b) { return uge(b,a); }
static inline IExpr operator >= (int64_t a, const IExpr& b) { return b <= a; }
static inline IExpr uge         (int64_t a, const IExpr& b) { return ule(b, a);}
static inline IExpr operator >  (int64_t a, const IExpr& b) { return b < a; }
static inline IExpr ugt         (int64_t a, const IExpr& b) { return ult(b,a);}
static inline IExpr operator <  (int64_t a, const IExpr& b) { return b > a; }
static inline IExpr ult         (int64_t a, const IExpr& b) {return ugt(b,a);}
static inline IExpr operator && (const IExpr& a, const IExpr& b)
{ return IExpr(OP_LOGICAL_AND, TYPE_BOOLEAN, {a.notype(), b.notype()}); }
static inline IExpr operator || (const IExpr& a, const IExpr& b)
{ return IExpr(OP_LOGICAL_OR, TYPE_BOOLEAN, {a.notype(), b.notype()}); }
static inline IExpr operator ! (const IExpr& a)
{ return IExpr(OP_LOGICAL_NOT, TYPE_BOOLEAN, {a.notype()}); }

//Augmenting operations:
static inline IReg& operator += (IReg& _a, const IExpr& b)
{ IExpr a(_a); newiopNoret(OP_ADD, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator += (IReg& _a, int64_t b)
{ IExpr a(_a); newiopNoret(OP_ADD, {a.notype(), a.notype(), Expr(b)}); return _a; }
static inline IReg& operator -= (IReg& _a, const IExpr& b)
{ IExpr a(_a); newiopNoret(OP_SUB, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator -= (IReg& _a, int64_t b)
{ IExpr a(_a); newiopNoret(OP_SUB, {a.notype(), a.notype(), Expr(b)}); return _a; }
static inline IReg& operator *= (IReg& _a, const IExpr& b)
{ IExpr a(_a); newiopNoret(OP_MUL, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator *= (IReg& _a, int64_t b)
{ IExpr a(_a); newiopNoret(OP_MUL, {a.notype(), a.notype(), Expr(b)}); return _a; }
static inline IReg& operator /= (IReg& _a, const IExpr& b)
{ IExpr a(_a); newiopNoret(OP_DIV, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator /= (IReg& _a, int64_t b)
{ IExpr a(_a); newiopNoret(OP_DIV, {a.notype(), a.notype(), Expr(b)}); return _a; }
static inline IReg& operator %= (IReg& _a, const IExpr& b)
{ IExpr a(_a); newiopNoret(OP_MOD, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator %= (IReg& _a, int64_t b)
{ IExpr a(_a); newiopNoret(OP_MOD, {a.notype(), a.notype(), Expr(b)}); return _a; }
static inline IReg& operator >>= (IReg& _a, const IExpr& b)
{ IExpr a(_a); newiopNoret(OP_SAR, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator >>= (IReg& _a, int64_t b)
{ IExpr a(_a); newiopNoret(OP_SAR, {a.notype(), a.notype(), Expr(b)}); return _a; }
static inline IReg& operator <<= (IReg& _a, const IExpr& b)
{ IExpr a(_a); newiopNoret(OP_SHL, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator <<= (IReg& _a, int64_t b)
{ IExpr a(_a); newiopNoret(OP_SHL, {a.notype(), a.notype(), Expr(b)}); return _a; }
static inline IReg& operator &= (IReg& _a, const IExpr& b)
{ IExpr a(_a); newiopNoret(OP_AND, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator &= (IReg& _a, int64_t b)
{ IExpr a(_a); newiopNoret(OP_AND, {a.notype(), a.notype(), Expr(b)}); return _a; }
static inline IReg& operator |= (IReg& _a, const IExpr& b)
{ IExpr a(_a); newiopNoret(OP_OR, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator |= (IReg& _a, int64_t b)
{ IExpr a(_a); newiopNoret(OP_OR, {a.notype(), a.notype(), Expr(b)}); return _a; }
static inline IReg& operator ^= (IReg& _a, const IExpr& b)
{ IExpr a(_a); newiopNoret(OP_XOR, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator ^= (IReg& _a, int64_t b)
{ IExpr a(_a); newiopNoret(OP_XOR, {a.notype(), a.notype(), Expr(b)}); return _a; }

//Other integer operations:
static inline IExpr select(const IExpr& cond, const IExpr& true_, const IExpr& false_)
{ return IExpr(OP_SELECT, true_.type(), {cond.notype(), true_.notype(), false_.notype()}); }
static inline IExpr select(const IExpr& cond, int64_t true_, const IExpr& false_)
{ return IExpr(OP_SELECT, false_.type(), {cond.notype(), Expr(true_), false_.notype()}); }
static inline IExpr select(const IExpr& cond, const IExpr& true_, int64_t false_)
{ return IExpr(OP_SELECT, true_.type(), {cond.notype(), true_.notype(), Expr(false_)}); }
static inline IExpr max(const IExpr& a, const IExpr& b)
{ return IExpr(OP_MAX, a.type(), {a.notype(), b.notype()}); }
static inline IExpr max(const IExpr& a, int64_t b)
{ return IExpr(OP_MAX, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr max(int64_t a, const IExpr& b)
{ return IExpr(OP_MAX, b.type(), {b.notype(), Expr(a)}); }
static inline IExpr min(const IExpr& a, const IExpr& b)
{ return IExpr(OP_MIN, a.type(), {a.notype(), b.notype()}); }
static inline IExpr min(const IExpr& a, int64_t b)
{ return IExpr(OP_MIN, a.type(), {a.notype(), Expr(b)}); }
static inline IExpr min(int64_t a, const IExpr& b)
{ return IExpr(OP_MIN, b.type(), {b.notype(), Expr(a)}); }
static inline IExpr abs(const IExpr& a)
{ return IExpr(OP_ABS, a.type(), {a.notype()}); }
static inline IExpr sign(const IExpr& a)
{ return IExpr(OP_SIGN, a.type(), {a.notype()}); }

///////////////////////////// vector operations ///////////////////////
#define LOOPS_CONVERT_ARGS_TBINARY(bfunc) \
template<typename _Tp> VExpr<_Tp> bfunc (const VReg<_Tp>& a, const VReg<_Tp>& b) { return bfunc(VExpr<_Tp>(a), VExpr<_Tp>(b)); } \
template<typename _Tp> VExpr<_Tp> bfunc (const VReg<_Tp>& a, const VExpr<_Tp>& b){ return bfunc(VExpr<_Tp>(a), b); } \
template<typename _Tp> VExpr<_Tp> bfunc (const VExpr<_Tp>& a, const VReg<_Tp>& b){ return bfunc(a, VExpr<_Tp>(b)); }

#define LOOPS_CONVERT_ARGS_TBIN_INTSUF(bfunc) \
template<typename _Tp> VExpr<_Tp> bfunc(const VReg<_Tp>& a, int64_t suf) { return bfunc(VExpr<_Tp>(a), suf); }

#define LOOPS_CONVERT_ARGS_TUNARY(ufunc) \
template<typename _Tp> VExpr<_Tp> ufunc (const VReg<_Tp>& a) { return ufunc(VExpr<_Tp>(a)); } \

#define LOOPS_CONVERT_ARGS_TUNARY_DOUT(otype, ufunc) template<typename _Tp> VExpr<otype> ufunc (const VReg<_Tp>& a) { return ufunc(VExpr<otype>(a)); }

#define LOOPS_CONVERT_ARGS_TUNARY_DIN(itype, ufunc) \
template<typename _Tp> VExpr<_Tp> ufunc(const VReg<itype>& a) { return ufunc(VExpr<_Tp>(a));}

#define LOOPS_CONVERT_ARGS_TBINARY_2TYPES(bfunc) \
template<typename _Tp, typename _Sp> VExpr<_Tp> bfunc (const VReg<_Tp>& a, const VReg<_Sp>& b)   { return bfunc(VExpr<_Tp>(a), VExpr<_Sp>(b)); } \
template<typename _Tp, typename _Sp> VExpr<_Tp> bfunc (const VReg<_Tp>& a, const VExpr<_Sp>& b){ return bfunc(VExpr<_Tp>(a), b); } \
template<typename _Tp, typename _Sp> VExpr<_Tp> bfunc (const VExpr<_Tp>& a, const VReg<_Sp>& b){ return bfunc(a, VExpr<_Sp>(b)); }

#define LOOPS_CONVERT_ARGS_TBINARY_DOUT(otype, bfunc) \
template<typename _Tp> VExpr<otype> bfunc (const VReg<_Tp>& a, const VReg<_Tp>& b) { return bfunc(VExpr<_Tp>(a), VExpr<_Tp>(b)); } \
template<typename _Tp> VExpr<otype> bfunc (const VReg<_Tp>& a, const VExpr<_Tp>& b){ return bfunc(VExpr<_Tp>(a), b); } \
template<typename _Tp> VExpr<otype> bfunc (const VExpr<_Tp>& a, const VReg<_Tp>& b){ return bfunc(a, VExpr<_Tp>(b)); }

#define LOOPS_CONVERT_ARGS_TBINARY_RMASK(bfunc) LOOPS_CONVERT_ARGS_TBINARY_DOUT(typename ElemTraits<_Tp>::masktype, bfunc)

#define LOOPS_CONVERT_ARGS_TAUG(afunc) \
template<typename _Tp> VReg<_Tp>& afunc (VReg<_Tp>& _a, const VReg<_Tp>& b) { return afunc(_a, VExpr<_Tp>(b)); }

// Load with zero/sign extension:
template<typename _Tp> VExpr<_Tp> loadvec(const IExpr& base)
{ return VExpr<_Tp>(VOP_LOAD, {base.notype()}); }
template<typename _Tp> VExpr<_Tp> loadvec(const IExpr& base, const IExpr& offset)
{ return VExpr<_Tp>(VOP_LOAD, {base.notype(), offset.notype()}); }
template<typename _Tp> VExpr<_Tp> loadvec(const IExpr& base, int64_t offset)
{ return VExpr<_Tp>(VOP_LOAD, {base.notype(), Expr(offset)}); }
template<typename _Tp> VExpr<_Tp> loadlane(const IExpr& base, int64_t lane_index)
{ return VExpr<_Tp>(VOP_ARM_LD1, {Expr(lane_index), base.notype()}); }
//TODO(ch): find a way to delete next warning:
//WARNING! It's assumed here, that res1 and res2 are not initialized yet.
//template<typename _Tp> std::pair<Expr, Expr> loadvec_deinterleave2(const Expr& base); //TODO(ch): optimal form of signature
void loadvec_deinterleave2_(Arg& res1, Arg& res2, const IExpr& base);
template<typename _Tp> void loadvec_deinterleave2(VReg<_Tp>& res1, VReg<_Tp>& res2, const IExpr& base)
{
    if(res1.func || res2.func)
        throw std::runtime_error("Load deinterleave doesn't support initilized results registers yet.");
    Arg r1(res1);
    Arg r2(res2);
    loadvec_deinterleave2_(r1, r2, base);
    res1.func = res2.func = base.super.func();
    res1.idx = r1.idx;
    res2.idx = r2.idx;
}

// Store:
template<typename _Tp> void storevec(const IExpr& base, const VExpr<_Tp>& r)
{ newiopNoret(VOP_STORE, {base.notype(), r.notype()}); }
template<typename _Tp> void storevec(const IExpr& base, const IExpr& offset, const VExpr<_Tp>& r)
{ newiopNoret(VOP_STORE, {base.notype(), offset.notype(), r.notype()}); }
template<typename _Tp> void storelane(const IExpr& base, const VExpr<_Tp>& r, int64_t lane_index)
{ newiopNoret(VOP_ARM_ST1, {base.notype(), r.notype(), Expr(lane_index)}); }

// Casts:
template<typename _Dp, typename _Tp> VExpr<_Dp> cast(const VExpr<_Tp>& a)
{ return VExpr<_Dp>(VOP_CAST, {a.notype()}); }
template<typename _Dp, typename _Tp> VExpr<_Dp> reinterpret(const VExpr<_Tp>& a)
{ return VExpr<_Dp>(VOP_REINTERPRET, {a.notype()});}
template<typename _Dp> VExpr<_Dp> trunc(const VExpr<f16_t>& a)  //Convert with rounding to zero
{
    static_assert(ElemTraits<_Dp>::depth == TYPE_I16 || ElemTraits<_Dp>::depth == TYPE_U16, "Attempt to convert real number to integer of different size or not integer.");
    return VExpr<_Dp>(VOP_TRUNC, {a.notype()});
}
template<typename _Dp> VExpr<_Dp> trunc(const VExpr<float>& a)  //Convert with rounding to zero
{
    static_assert(ElemTraits<_Dp>::depth == TYPE_I32 || ElemTraits<_Dp>::depth == TYPE_U32, "Attempt to convert real number to integer of different size or not integer.");
    return VExpr<_Dp>(VOP_TRUNC, {a.notype()});
}
template<typename _Dp> VExpr<_Dp> trunc(const VExpr<double>& a)  //Convert with rounding to zero
{
    static_assert(ElemTraits<_Dp>::depth == TYPE_I64 || ElemTraits<_Dp>::depth == TYPE_U64, "Attempt to convert real number to integer of different size or not integer.");
    return VExpr<_Dp>(VOP_TRUNC, {a.notype()});
}
template<typename _Dp> VExpr<_Dp> floor(const VExpr<f16_t>& a)  //Convert with rounding to minus infinity
{
    static_assert(ElemTraits<_Dp>::depth == TYPE_I16 || ElemTraits<_Dp>::depth == TYPE_U16, "Attempt to convert real number to integer of different size or not integer.");
    return VExpr<_Dp>(VOP_FLOOR, {a.notype()});
}
template<typename _Dp> VExpr<_Dp> floor(const VExpr<float>& a)  //Convert with rounding to minus infinity
{
    static_assert(ElemTraits<_Dp>::depth == TYPE_I32 || ElemTraits<_Dp>::depth == TYPE_U32, "Attempt to convert real number to integer of different size or not integer.");
    return VExpr<_Dp>(VOP_FLOOR, {a.notype()});
}
template<typename _Dp> VExpr<_Dp> floor(const VExpr<double>& a)  //Convert with rounding to minus infinity
{
    static_assert(ElemTraits<_Dp>::depth == TYPE_I64 || ElemTraits<_Dp>::depth == TYPE_U64, "Attempt to convert real number to integer of different size or not integer.");
    return VExpr<_Dp>(VOP_FLOOR, {a.notype()});
}

//TODO(ch): cvtTp -> ceil, cvtTe -> round, also cast(double <=> float, float <=> f16_t)
template<typename _Tp> VExpr<_Tp> broadcast(const IExpr& scalar)
{ return VExpr<_Tp>(VOP_BROADCAST, { scalar.notype() }); }
template<typename _Tp> VExpr<_Tp> broadcast(const VExpr<_Tp>& inp, int64_t ilane_index)
{ return VExpr<_Tp>(VOP_BROADCAST, { inp.notype(), Expr(ilane_index) }); }
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::duplicatetype> cast_low(const VExpr<_Tp>& r)
{ return VExpr<typename ElemTraits<_Tp>::duplicatetype>(VOP_CAST_LOW, { r.notype() }); }
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::duplicatetype> cast_high(const VExpr<_Tp>& r)
{ return VExpr<typename ElemTraits<_Tp>::duplicatetype>(VOP_CAST_HIGH, { r.notype() }); }
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::halftype> shrink(const VExpr<_Tp>& r0, const VExpr<_Tp>& r1)
{ return VExpr<typename ElemTraits<_Tp>::half_type>(VOP_SHRINK, {r0.notype(),r1.notype()}); }

//Lane manipulations:
template<typename _Tp> IExpr getlane(const VExpr<_Tp>& r, int64_t lane_index)
{ return IExpr(VOP_GETLANE, ElemTraits<_Tp>::depth, {r.notype(), Expr(lane_index)}); }
template<typename _Tp> void setlane(const VExpr<_Tp>& v, int64_t lane_index, const IExpr& i)
{ newiopNoret(VOP_SETLANE, {v.notype(), Expr(lane_index), i.notype()}); }
template<typename _Tp> void setlane(const VExpr<_Tp>& v, int64_t lane_index, const VExpr<_Tp>& inp, int64_t ilane_index)
{ newiopNoret(VOP_SETLANE, {v.notype(), Expr(lane_index), inp.notype(), Expr(ilane_index)}); }
template<typename _Tp> VExpr<_Tp> reduce_max(const VExpr<_Tp>& r)
{ return VExpr<_Tp>(VOP_REDUCE_MAX, { r.notype() }); }
template<typename _Tp> VExpr<_Tp> reduce_min(const VExpr<_Tp>& r)
{ return VExpr<_Tp>(VOP_REDUCE_MIN, { r.notype() }); }
template<typename _Tp> VExpr<_Tp> ext(const VExpr<_Tp>& n, const VExpr<_Tp>& m, int64_t index)
{ return VExpr<_Tp>(VOP_ARM_EXT, {n.notype(), m.notype(), Expr(index)}); }

// Arithmetic and bitwise operations:
template<typename _Tp> VExpr<_Tp> operator + (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<_Tp>(VOP_ADD, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<_Tp> operator - (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<_Tp>(VOP_SUB, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<_Tp> operator * (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<_Tp>(VOP_MUL, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<_Tp> operator / (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<_Tp>(VOP_DIV, {a.notype(), b.notype()}); }
//template<typename _Tp> VExpr<_Tp> operator % (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator - (const VExpr<_Tp>& a)
{ return VExpr<_Tp>(VOP_NEG, {a.notype()}); }

template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const VExpr<_Tp>& b, const VExpr<_Tp>& c)
{ return VExpr<_Tp>(VOP_FMA, {a.notype(), b.notype(), c.notype()}); }
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const VExpr<_Tp>& b, const VExpr<_Tp>& c, int64_t index)
{ return VExpr<_Tp>(VOP_FMA, {a.notype(), b.notype(), c.notype(), Expr(index)}); }

template<typename _Tp>
VExpr<_Tp> pow(const VExpr<_Tp>& a, int p)
{
    Context CTX = ExtractContext(a.notype());
    USE_CONTEXT_(CTX);
    if(p == 0)
        return VCONST_(_Tp, 1);
    VExpr<_Tp> _a = a;
    VExpr<_Tp> res;
    while (p)
        if (p & 1)
        {
            res = (res.empty()) ? _a : res * _a;
            --p;
        }
        else
        {
            _a = _a * _a;
            p >>= 1;
        }
    return res;
}
template<typename _Tp> VExpr<_Tp> pow(const VReg<_Tp>& a, int p) { return pow(VExpr<_Tp>(a),p); }

struct exp_consts
{
    VReg<float> lo, hi, half, one, LOG2EF, C1, C2, p0, p1, p2, p3, p4, p5;
    VReg<int32_t> _7f;
    exp_consts(Context CTX);
};

template<typename _Tp> VExpr<_Tp> operator >> (const VExpr<_Tp>& a, int64_t b)
{ return VExpr<_Tp>(VOP_SAR, {a.notype(), Expr(b)}); }
template<typename _Tp> VExpr<_Tp> ushift_right (const VExpr<_Tp>& a, int64_t b)
{ return VExpr<_Tp>(VOP_SHR, {a.notype(), Expr(b)}); }
template<typename _Tp, typename _Sp> VExpr<_Tp> operator << (const VExpr<_Tp>& a, const VExpr<_Sp>& b)
{
    static_assert(sizeof(_Tp) == sizeof(_Sp), "the # of lanes in the 1st and 2nd argument must be the same");
    return VExpr<_Tp>(VOP_SAL, {a.notype(), b.notype()});
}
template<typename _Tp> VExpr<_Tp> operator << (const VExpr<_Tp>& a, int64_t b)
{ return VExpr<_Tp>(VOP_SAL, {a.notype(), Expr(b)}); }

template<typename _Tp, typename _Sp> VExpr<_Tp> ushift_left(const VExpr<_Tp>& a, const VExpr<_Sp>& b)
{
    static_assert(sizeof(_Tp) == sizeof(_Sp), "the # of lanes in the 1st and 2nd argument must be the same");
    return VExpr<_Tp>(VOP_SHL, {a.notype(), Expr(b)});
}
template<typename _Tp> VExpr<_Tp> operator & (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<_Tp>(VOP_AND, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<_Tp> operator | (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<_Tp>(VOP_OR, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<_Tp> operator ^ (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<_Tp>(VOP_XOR, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<_Tp> operator ~ (const VExpr<_Tp>& a)
{ return VExpr<_Tp>(VOP_NOT, {a.notype()}); }

// Vector comparisson and masking:
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator == (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<typename ElemTraits<_Tp>::masktype>(VOP_EQ, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator != (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<typename ElemTraits<_Tp>::masktype>(VOP_NE, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator >= (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<typename ElemTraits<_Tp>::masktype>(VOP_GE, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator <= (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<typename ElemTraits<_Tp>::masktype>(VOP_LE, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator > (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<typename ElemTraits<_Tp>::masktype>(VOP_GT, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator < (const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<typename ElemTraits<_Tp>::masktype>(VOP_LT, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<_Tp> select(const VExpr<typename ElemTraits<_Tp>::masktype>& flag, const VExpr<_Tp>& iftrue, const VExpr<_Tp>& iffalse)
{ return VExpr<_Tp>(VOP_SELECT, {flag.notype(), iftrue.notype(), iffalse.notype()}); }
template<typename _Tp> VExpr<_Tp> max(const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<_Tp>(VOP_MAX, {a.notype(), b.notype()}); }
template<typename _Tp> VExpr<_Tp> min(const VExpr<_Tp>& a, const VExpr<_Tp>& b)
{ return VExpr<_Tp>(VOP_MIN, {a.notype(), b.notype()}); }

//Augmenting operations:
template<typename _Tp> VReg<_Tp>& operator += (VReg<_Tp>& _a, const VExpr<_Tp>& b)
{ VExpr<_Tp> a = VExpr<_Tp>(_a); newiopNoret(VOP_ADD, {a.notype(), a.notype(), b.notype()}); return _a; }
template<typename _Tp> VReg<_Tp>& operator -= (VReg<_Tp>& _a, const VExpr<_Tp>& b)
{ VExpr<_Tp> a = VExpr<_Tp>(_a); newiopNoret(VOP_SUB, {a.notype(), a.notype(), b.notype()}); return _a; }
template<typename _Tp> VReg<_Tp>& operator *= (VReg<_Tp>& _a, const VExpr<_Tp>& b)
{ VExpr<_Tp> a = VExpr<_Tp>(_a); newiopNoret(VOP_MUL, {a.notype(), a.notype(), b.notype()}); return _a; }
template<typename _Tp> VReg<_Tp>& operator /= (VReg<_Tp>& _a, const VExpr<_Tp>& b)
{ VExpr<_Tp> a = VExpr<_Tp>(_a); newiopNoret(VOP_DIV, {a.notype(), a.notype(), b.notype()}); return _a; }
//template<typename _Tp> VReg<_Tp>& operator %= (VReg<_Tp>& _a, const VExpr<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& _a, const VExpr<_Tp>& b)
template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& _a, int64_t _b)
{ VExpr<_Tp> a = VExpr<_Tp>(_a);  Expr b(_b); newiopNoret(VOP_SAR, {a.notype(), a.notype(), b}); return _a; }
template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& _a, VExpr<_Tp> b)
{ VExpr<_Tp> a = VExpr<_Tp>(_a); newiopNoret(VOP_SAL, {a.notype(), a.notype(), b.notype()}); return _a; }
template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& _a, int64_t _b)
{ VExpr<_Tp> a = VExpr<_Tp>(_a);  Expr b(_b); newiopNoret(VOP_SAL, {a.notype(), a.notype(), b}); return _a; }
template<typename _Tp> VReg<_Tp>& operator &= (VReg<_Tp>& _a, const VExpr<_Tp>& b)
{ VExpr<_Tp> a = VExpr<_Tp>(_a); newiopNoret(VOP_AND, {a.notype(), a.notype(), b.notype()} ); return _a; }
template<typename _Tp> VReg<_Tp>& operator |= (VReg<_Tp>& _a, const VExpr<_Tp>& b)
{ VExpr<_Tp> a = VExpr<_Tp>(_a); newiopNoret(VOP_OR, {a.notype(), a.notype(), b.notype()} ); return _a; }
template<typename _Tp> VReg<_Tp>& operator ^= (VReg<_Tp>& _a, const VExpr<_Tp>& b)
{ VExpr<_Tp> a = VExpr<_Tp>(_a); newiopNoret(VOP_XOR, {a.notype(), a.notype(), b.notype()} ); return _a; }

//Argument conversions:
template<typename _Tp> void storevec(const IExpr& base, const VReg<_Tp>& r){ storevec(base, VExpr<_Tp>(r)); }
template<typename _Tp> void storevec(const IExpr& base, const IExpr& offset, const VReg<_Tp>& r) { storevec(base,offset, VExpr<_Tp>(r));}
template<typename _Tp> void storelane(const IExpr& base, const VReg<_Tp>& r, int64_t lane_index) {storelane(base, VExpr<_Tp>(r), lane_index);}

template<typename _Dp, typename _Tp> VExpr<_Dp> cast(const VReg<_Tp>& a) { return cast<_Dp>(VExpr<_Tp>(a));}
template<typename _Dp, typename _Tp> VExpr<_Dp> reinterpret(const VReg<_Tp>& a) { return reinterpret<_Dp>(VExpr<_Tp>(a));}

LOOPS_CONVERT_ARGS_TUNARY_DIN(f16_t, trunc)
LOOPS_CONVERT_ARGS_TUNARY_DIN(float, trunc)
LOOPS_CONVERT_ARGS_TUNARY_DIN(double, trunc)
LOOPS_CONVERT_ARGS_TUNARY_DIN(f16_t, floor)
LOOPS_CONVERT_ARGS_TUNARY_DIN(float, floor)
LOOPS_CONVERT_ARGS_TUNARY_DIN(double, floor)

LOOPS_CONVERT_ARGS_TBIN_INTSUF(broadcast)
LOOPS_CONVERT_ARGS_TUNARY_DOUT(typename ElemTraits<_Tp>::duplicatetype, cast_low)
LOOPS_CONVERT_ARGS_TUNARY_DOUT(typename ElemTraits<_Tp>::duplicatetype, cast_high)
LOOPS_CONVERT_ARGS_TBINARY_DOUT(typename ElemTraits<_Tp>::halftype, shrink)

template<typename _Tp> IExpr getlane(const VReg<_Tp>& r, int64_t lane_index) { return getlane(VExpr<_Tp>(r), lane_index); }
template<typename _Tp> void setlane(const VReg<_Tp>& v, int64_t lane_index, const IExpr& i) { setlane(VExpr<_Tp>(v), lane_index, i); }
template<typename _Tp> void setlane(const VExpr<_Tp>& v, int64_t lane_index, const    VReg<_Tp>& inp, int64_t ilane_index) { setlane(v, lane_index, VExpr<_Tp>(inp), ilane_index); }
template<typename _Tp> void setlane(const    VReg<_Tp>& v, int64_t lane_index, const VExpr<_Tp>& inp, int64_t ilane_index) { setlane(VExpr<_Tp>(v), lane_index, inp, ilane_index); }
template<typename _Tp> void setlane(const    VReg<_Tp>& v, int64_t lane_index, const    VReg<_Tp>& inp, int64_t ilane_index) { setlane(VExpr<_Tp>(v), lane_index, VExpr<_Tp>(inp), ilane_index); }
LOOPS_CONVERT_ARGS_TUNARY(reduce_max)
LOOPS_CONVERT_ARGS_TUNARY(reduce_min)
template<typename _Tp> VExpr<_Tp> ext(const VExpr<_Tp>& n, const    VReg<_Tp>& m, int64_t index) { return ext(n, VExpr<_Tp>(m), index); }
template<typename _Tp> VExpr<_Tp> ext(const    VReg<_Tp>& n, const VExpr<_Tp>& m, int64_t index) { return ext(VExpr<_Tp>(n), m, index); }
template<typename _Tp> VExpr<_Tp> ext(const    VReg<_Tp>& n, const    VReg<_Tp>& m, int64_t index) { return ext(VExpr<_Tp>(n), VExpr<_Tp>(m), index); }

template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const VExpr<_Tp>& b, const VReg<_Tp>& c) { return fma(a, b, VExpr<_Tp>(c)); }
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const VReg<_Tp>& b, const VExpr<_Tp>& c) { return fma(a, VExpr<_Tp>(b), c); }
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const VReg<_Tp>& b, const VReg<_Tp>& c) { return fma(a, VExpr<_Tp>(b), VExpr<_Tp>(c)); }
template<typename _Tp> VExpr<_Tp> fma(const VReg<_Tp>& a, const VExpr<_Tp>& b, const VExpr<_Tp>& c) { return fma(VExpr<_Tp>(a), b, c); }
template<typename _Tp> VExpr<_Tp> fma(const VReg<_Tp>& a, const VExpr<_Tp>& b, const VReg<_Tp>& c) { return fma(VExpr<_Tp>(a), b, VExpr<_Tp>(c)); }
template<typename _Tp> VExpr<_Tp> fma(const VReg<_Tp>& a, const VReg<_Tp>& b, const VExpr<_Tp>& c) { return fma(VExpr<_Tp>(a), VExpr<_Tp>(b), c); }
template<typename _Tp> VExpr<_Tp> fma(const VReg<_Tp>& a, const VReg<_Tp>& b, const VReg<_Tp>& c) { return fma(VExpr<_Tp>(a), VExpr<_Tp>(b), VExpr<_Tp>(c));}
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const VExpr<_Tp>& b, const VReg<_Tp>& c, int64_t index) { return fma(a, b, VExpr<_Tp>(c), index); }
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const VReg<_Tp>& b, const VExpr<_Tp>& c, int64_t index) { return fma(a, VExpr<_Tp>(b), c, index); }
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const VReg<_Tp>& b, const VReg<_Tp>& c, int64_t index) { return fma(a, VExpr<_Tp>(b), VExpr<_Tp>(c), index); }
template<typename _Tp> VExpr<_Tp> fma(const VReg<_Tp>& a, const VExpr<_Tp>& b, const VExpr<_Tp>& c, int64_t index) { return fma(VExpr<_Tp>(a), b, c, index); }
template<typename _Tp> VExpr<_Tp> fma(const VReg<_Tp>& a, const VExpr<_Tp>& b, const VReg<_Tp>& c, int64_t index) { return fma(VExpr<_Tp>(a), b, VExpr<_Tp>(c), index); }
template<typename _Tp> VExpr<_Tp> fma(const VReg<_Tp>& a, const VReg<_Tp>& b, const VExpr<_Tp>& c, int64_t index) { return fma(VExpr<_Tp>(a), VExpr<_Tp>(b), c, index); }
template<typename _Tp> VExpr<_Tp> fma(const VReg<_Tp>& a, const VReg<_Tp>& b, const VReg<_Tp>& c, int64_t index) { return fma(VExpr<_Tp>(a), VExpr<_Tp>(b), VExpr<_Tp>(c), index);}

LOOPS_CONVERT_ARGS_TBINARY(operator+)
LOOPS_CONVERT_ARGS_TBINARY(operator-)
LOOPS_CONVERT_ARGS_TBINARY(operator*)
LOOPS_CONVERT_ARGS_TBINARY(operator/)
LOOPS_CONVERT_ARGS_TUNARY(operator-)

LOOPS_CONVERT_ARGS_TBIN_INTSUF(operator>>)
LOOPS_CONVERT_ARGS_TBIN_INTSUF(ushift_right)
LOOPS_CONVERT_ARGS_TBIN_INTSUF(operator<<)
LOOPS_CONVERT_ARGS_TBINARY_2TYPES(operator<<)
LOOPS_CONVERT_ARGS_TBINARY_2TYPES(ushift_left)
LOOPS_CONVERT_ARGS_TBINARY(operator&)
LOOPS_CONVERT_ARGS_TBINARY(operator|)
LOOPS_CONVERT_ARGS_TBINARY(operator^)
LOOPS_CONVERT_ARGS_TUNARY(operator~)

LOOPS_CONVERT_ARGS_TBINARY_RMASK(operator ==)
LOOPS_CONVERT_ARGS_TBINARY_RMASK(operator !=)
LOOPS_CONVERT_ARGS_TBINARY_RMASK(operator >=)
LOOPS_CONVERT_ARGS_TBINARY_RMASK(operator <=)
LOOPS_CONVERT_ARGS_TBINARY_RMASK(operator >)
LOOPS_CONVERT_ARGS_TBINARY_RMASK(operator <)
LOOPS_CONVERT_ARGS_TBINARY(max)
LOOPS_CONVERT_ARGS_TBINARY(min)

template<typename _Tp> VExpr<_Tp> select(const VExpr<typename ElemTraits<_Tp>::masktype>& flag, const VExpr<_Tp>& iftrue, const    VReg<_Tp>& iffalse)
{ return select(flag, iftrue, VExpr<_Tp>(iffalse)); }
template<typename _Tp> VExpr<_Tp> select(const VExpr<typename ElemTraits<_Tp>::masktype>& flag, const    VReg<_Tp>& iftrue, const VExpr<_Tp>& iffalse)
{ return select(flag, VExpr<_Tp>(iftrue), iffalse); }
template<typename _Tp> VExpr<_Tp> select(const VExpr<typename ElemTraits<_Tp>::masktype>& flag, const    VReg<_Tp>& iftrue, const    VReg<_Tp>& iffalse)
{ return select(flag, VExpr<_Tp>(iftrue), VExpr<_Tp>(iffalse)); }
template<typename _Tp> VExpr<_Tp> select(const    VReg<typename ElemTraits<_Tp>::masktype>& flag, const VExpr<_Tp>& iftrue, const VExpr<_Tp>& iffalse)
{ return select(VExpr<typename ElemTraits<_Tp>::masktype>(flag), iftrue, iffalse); }
template<typename _Tp> VExpr<_Tp> select(const    VReg<typename ElemTraits<_Tp>::masktype>& flag, const VExpr<_Tp>& iftrue, const    VReg<_Tp>& iffalse)
{ return select(VExpr<typename ElemTraits<_Tp>::masktype>(flag), iftrue, VExpr<_Tp>(iffalse)); }
template<typename _Tp> VExpr<_Tp> select(const    VReg<typename ElemTraits<_Tp>::masktype>& flag, const    VReg<_Tp>& iftrue, const VExpr<_Tp>& iffalse)
{ return select(VExpr<typename ElemTraits<_Tp>::masktype>(flag), VExpr<_Tp>(iftrue), iffalse); }
template<typename _Tp> VExpr<_Tp> select(const    VReg<typename ElemTraits<_Tp>::masktype>& flag, const    VReg<_Tp>& iftrue, const    VReg<_Tp>& iffalse)
{ return select(VExpr<typename ElemTraits<_Tp>::masktype>(flag), VExpr<_Tp>(iftrue), VExpr<_Tp>(iffalse)); }

static inline VExpr<float> exp(const VReg<float>& x, const exp_consts& expt) { return exp(VExpr<float>(x), expt); }

LOOPS_CONVERT_ARGS_TAUG(operator+=)
LOOPS_CONVERT_ARGS_TAUG(operator-=)
LOOPS_CONVERT_ARGS_TAUG(operator*=)
LOOPS_CONVERT_ARGS_TAUG(operator/=)
LOOPS_CONVERT_ARGS_TAUG(operator<<=)
LOOPS_CONVERT_ARGS_TAUG(operator&=)
LOOPS_CONVERT_ARGS_TAUG(operator|=)
LOOPS_CONVERT_ARGS_TAUG(operator^=)

#undef LOOPS_CONVERT_ARGS_TBINARY
#undef LOOPS_CONVERT_ARGS_TUNARY
#undef LOOPS_CONVERT_ARGS_TUNARY_DOUT
#undef LOOPS_CONVERT_ARGS_TBINARY_2TYPES
#undef LOOPS_CONVERT_ARGS_TBINARY_DOUT
#undef LOOPS_CONVERT_ARGS_TBINARY_RMASK
#undef LOOPS_CONVERT_ARGS_TAUG
}
#endif
