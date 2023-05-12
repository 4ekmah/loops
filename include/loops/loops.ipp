
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

void VReg_constr_(const Recipe& fromwho, int& idx, Func*& func, int restype);

template<typename _Tp>
VReg<_Tp>::VReg(const VReg<_Tp>& r)
{
    if(r.func != nullptr)
    {
        Recipe fromwho(r);
        VReg_constr_(fromwho, idx, func, ElemTraits<_Tp>::depth);
    }
    else
    {
        idx = NOIDX;
        func = nullptr;
    }
}

template<typename _Tp>
VReg<_Tp>::VReg(const VRecipe<_Tp>& fromwho)
{
    VReg_constr_(fromwho.notype(), idx, func, ElemTraits<_Tp>::depth);
}

template<typename _Tp>
VReg<_Tp>& VReg<_Tp>::operator=(const VReg<_Tp>& r)
{
    VRecipe<_Tp> fromwho = VRecipe<_Tp>::make(r);
    return operator=(fromwho);
}

void VReg_assign_(const Recipe& target, const Recipe& from);

template<typename _Tp>
VReg<_Tp>& VReg<_Tp>::operator=(const VRecipe<_Tp>& fromwho)
{
    VReg_assign_(VRecipe<_Tp>::make(*this).notype(), fromwho.notype());
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

void VReg_copyidx_(const Recipe& fromwho, int& idx, Func*& func);

template<typename _Tp>
void VReg<_Tp>::copyidx(const VRecipe<_Tp>& from)
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

struct __loops_RecipeStr_
{
    int opcode;
    bool is_vector;
    int type;
    Arg leaf;
    int refcounter;
    Func* func;
    std::vector<Recipe> children;
    __loops_RecipeStr_():refcounter(0), func(nullptr) {}
};

Recipe::Recipe(): pointee(NULL) {}
Recipe::Recipe(const Recipe& fromwho):pointee(fromwho.pointee) { if(pointee) pointee->refcounter++; }
Recipe& Recipe::operator=(const Recipe& fromwho)
{
    if(pointee!=fromwho.pointee)
    {
        if(pointee) { if((--(pointee->refcounter)) == 0) { delete pointee; } }
        pointee = fromwho.pointee;
        if(pointee) pointee->refcounter++;
    }
    return *this;
}

Recipe::Recipe(const Arg& a_leaf): pointee(new __loops_RecipeStr_)
{
    pointee->refcounter = 1;
    pointee->opcode = RECIPE_LEAF;
    pointee->type = a_leaf.elemtype;
    pointee->is_vector = a_leaf.tag == Arg::VREG;
    pointee->leaf = a_leaf;
}

Recipe::Recipe(int64_t a_leaf): pointee(new __loops_RecipeStr_)
{
    pointee->refcounter = 1;
    pointee->opcode = RECIPE_LEAF;
    pointee->is_vector = false;
    pointee->leaf = Arg(a_leaf);
}

Recipe::~Recipe() { if(pointee) { if(--(pointee->refcounter) == 0) delete pointee; } }
int& Recipe::opcode() { if(!pointee) throw std::runtime_error("Null pointer in Recipe."); return pointee->opcode; }
bool& Recipe::is_vector() { if(!pointee) throw std::runtime_error("Null pointer in Recipe."); return pointee->is_vector; }
bool Recipe::is_leaf() const {return opcode() == RECIPE_LEAF; }
int& Recipe::type() { if(!pointee) throw std::runtime_error("Null pointer in Recipe."); return pointee->type;}
Arg& Recipe::leaf()
{
    if(!pointee) throw std::runtime_error("Null pointer in Recipe.");
    if(pointee->opcode != RECIPE_LEAF) throw std::runtime_error("Interpretting leaf node as branch.");
    return pointee->leaf;
}

Func*& Recipe::func() { if(!pointee) throw std::runtime_error("Null pointer in Recipe."); return pointee->func;}

std::vector<Recipe>& Recipe::children()
{
    if(!pointee) throw std::runtime_error("Null pointer in Recipe.");
    if(pointee->opcode == RECIPE_LEAF) throw std::runtime_error("Interpretting leaf node as branch.");
    return pointee->children;
}

int Recipe::opcode() const { if(!pointee) throw std::runtime_error("Null pointer in Recipe."); return pointee->opcode; }
bool Recipe::is_vector() const { if(!pointee) throw std::runtime_error("Null pointer in Recipe."); return pointee->is_vector; }
int Recipe::type() const { if(!pointee) throw std::runtime_error("Null pointer in Recipe."); return pointee->type;}
const Arg& Recipe::leaf() const
{
    if(!pointee) throw std::runtime_error("Null pointer in Recipe.");
    if(pointee->opcode != RECIPE_LEAF) throw std::runtime_error("Interpretting leaf node as branch.");
    return pointee->leaf;
}
const std::vector<Recipe>& Recipe::children() const
{
    if(!pointee) throw std::runtime_error("Null pointer in Recipe.");
    if(pointee->opcode == RECIPE_LEAF) throw std::runtime_error("Interpretting leaf node as branch.");
    return pointee->children;
}

Func* Recipe::func() const { if(!pointee) throw std::runtime_error("Null pointer in Recipe."); return pointee->func;}

bool Recipe::empty() const { return pointee == nullptr; }

inline IRecipe::IRecipe(const IReg& a_leaf)
{
    super.pointee = new __loops_RecipeStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = RECIPE_LEAF;
    super.pointee->is_vector = false;
    super.pointee->type = TYPE_I64;
    super.pointee->leaf = Arg(a_leaf);
    super.pointee->func = a_leaf.func;
}

inline IRecipe::IRecipe(int a_opcode, int a_type, std::initializer_list<Recipe> a_children)
{
    super.pointee = new __loops_RecipeStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = a_opcode;
    super.pointee->is_vector = false;
    super.pointee->type = a_type;
    super.pointee->children = a_children;
    super.infer_owner();
}

inline IRecipe::IRecipe(int a_opcode, int a_type, std::vector<Recipe> a_children)
{
    super.pointee = new __loops_RecipeStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = a_opcode;
    super.pointee->is_vector = false;
    super.pointee->type = a_type;
    super.pointee->children = a_children;
    super.infer_owner();
}

template <typename _Tp>
inline VRecipe<_Tp> VRecipe<_Tp>::make(const VReg<_Tp>& a_leaf)
{
    VRecipe<_Tp> res;
    res.super.pointee = new __loops_RecipeStr_;
    res.super.pointee->refcounter = 1;
    res.super.pointee->opcode = RECIPE_LEAF;
    res.super.pointee->is_vector = true;
    res.super.pointee->type = ElemTraits<_Tp>::depth;
    res.super.pointee->leaf = Arg(a_leaf);
    res.super.pointee->func = a_leaf.func;
    return res;
}

template <typename _Tp>
inline VRecipe<_Tp>::VRecipe(int a_opcode, std::initializer_list<Recipe> a_children)
{
    super.pointee = new __loops_RecipeStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = a_opcode;
    super.pointee->is_vector = true;
    super.pointee->type = ElemTraits<_Tp>::depth;
    super.pointee->children = a_children;
    super.infer_owner();
}

template <typename _Tp>
inline VRecipe<_Tp>::VRecipe(int a_opcode, std::vector<Recipe> a_children)
{
    super.pointee = new __loops_RecipeStr_;
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
    explicit __Loops_CFScopeBracket_(const __Loops_CondPrefixMarker_& inh, CFType _cftype, const IRecipe& condition);
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
    void return_(const IRecipe& r);
    void return_(int64_t r);
};

void __setfunc_by_context_(Context* CTX, Recipe& recipe);

static inline IRecipe __loops_const_(Context* CTX, int64_t _val)
{
    Recipe val(_val);
    __setfunc_by_context_(CTX, val);
    return IRecipe(OP_MOV, TYPE_I64, {val});
}

static inline IRecipe __loops_def_(Context* CTX)
{
    IRecipe res(OP_DEF, TYPE_I64, {});
    __setfunc_by_context_(CTX, res.super);
    return res;
}

IRecipe __loops_const_(Context* CTX, int64_t _val);
IRecipe __loops_def_(Context* CTX);

template<typename _Tp>
VRecipe<_Tp> __loops_vconst_(Context* CTX, _Tp _val)
{
    int64_t val64 = 0;
    *(reinterpret_cast<_Tp*>(&val64)) = _val;
    Recipe val(val64);
    __setfunc_by_context_(CTX, val);
    return VRecipe<_Tp>(OP_MOV, {val});
}

template<typename _Tp>
VRecipe<_Tp> __loops_vdef_(Context* CTX)
{
    VRecipe<_Tp> res(VOP_DEF, {});
    __setfunc_by_context_(CTX, res.super);
    return res;
}

void newiopNoret(int opcode, ::std::initializer_list<Recipe> args);

// void newiopNoret(int opcode, ::std::initializer_list<Recipe> args);
///////////////////////////// integer operations ///////////////////////
// Load with zero/sign extension:
static inline IRecipe loadx(const IRecipe& base, int depth)
{ return IRecipe(OP_LOAD, depth, {base.notype()}); }
static inline IRecipe loadx(const IRecipe& base, const IRecipe& offset, int depth)
{ return IRecipe(OP_LOAD, depth, {base.notype(), offset.notype()}); }
static inline IRecipe loadx(const IRecipe& base, int64_t offset, int depth)
{ return IRecipe(OP_LOAD, depth, {base.notype(), Recipe(offset)}); }

template<typename _Tp> IRecipe load_(const IRecipe& base)
{ return loadx(base, ElemTraits<_Tp>::depth); }
template<typename _Tp> IRecipe load_(const IRecipe& base, const IRecipe& offset)
{ return loadx(base, offset, ElemTraits<_Tp>::depth); }
template<typename _Tp> IRecipe load_(const IRecipe& base, int64_t offset)
{ return loadx(base, offset, ElemTraits<_Tp>::depth); }

// store part of register
static inline void storex(const IRecipe& base, const IRecipe& r, int depth)
{
    Recipe r_(r.notype());
    r_.type() = depth;
    newiopNoret(OP_STORE, {base.notype(), r_});
}
static inline void storex(const IRecipe& base, int64_t a, int depth)
{
    Recipe r_(a);
    r_.type() = depth;
    newiopNoret(OP_STORE, {base.notype(), r_});
}
static inline void storex(const IRecipe& base, const IRecipe& offset, const IRecipe& r, int depth)
{
    Recipe r_(r.notype());
    r_.type() = depth;
    newiopNoret(OP_STORE, {base.notype(), offset.notype(), r_});
}
static inline void storex(const IRecipe& base, int64_t offset, const IRecipe& r, int depth)
{
    Recipe r_(r.notype());
    r_.type() = depth;
    newiopNoret(OP_STORE, {base.notype(), Recipe(offset), r_});
}
static inline void storex(const IRecipe& base, const IRecipe& offset, int64_t a, int depth)
{
    Recipe r_(a);
    r_.type() = depth;
    newiopNoret(OP_STORE, {base.notype(), offset.notype(), r_});
}
static inline void storex(const IRecipe& base, int64_t offset, int64_t a, int depth)
{
    Recipe r_(a);
    r_.type() = depth;
    newiopNoret(OP_STORE, {base.notype(), Recipe(offset), r_});
}
static inline void store(const IRecipe& base, const IRecipe& r)
{ storex(base, r, TYPE_I64); }
static inline void store(const IRecipe& base, const IRecipe& offset, const IRecipe& r)
{ storex(base, offset, r, TYPE_I64);}
template<typename _Tp> void store_(const IRecipe& base, const IRecipe& r)
{ storex(base, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> void store_(const IRecipe& base, int64_t a)
{ storex(base, a, ElemTraits<_Tp>::depth); }
template<typename _Tp> void store_(const IRecipe& base, const IRecipe& offset, const IRecipe& r)
{ storex(base, offset, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> void store_(const IRecipe& base, int64_t offset, const IRecipe& r)
{ storex(base, offset, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> void store_(const IRecipe& base, const IRecipe& offset, int64_t a)
{ storex(base, offset, a, ElemTraits<_Tp>::depth); }
template<typename _Tp> void store_(const IRecipe& base, int64_t offset, int64_t a)
{ storex(base, offset, a, ElemTraits<_Tp>::depth); }

// Integer arithmetic and bitwise operations:
static inline IRecipe operator + (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_ADD, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator + (const IRecipe& a, int64_t b)
{ return IRecipe(OP_ADD, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator + (int64_t a, const IRecipe& b)
{ return IRecipe(OP_ADD, b.type(), {b.notype(), Recipe(a)}); }
static inline IRecipe operator - (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_SUB, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator - (const IRecipe& a, int64_t b)
{ return IRecipe(OP_SUB, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator - (int64_t a, const IRecipe& b)
{ return IRecipe(OP_SUB, b.type(), {Recipe(a), b.notype()}); }
static inline IRecipe operator * (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_MUL, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator * (const IRecipe& a, int64_t b)
{ return IRecipe(OP_MUL, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator * (int64_t a, const IRecipe& b)
{ return IRecipe(OP_MUL, b.type(), {b.notype(), Recipe(a)}); }
static inline IRecipe operator / (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_DIV, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator / (const IRecipe& a, int64_t b)
{ return IRecipe(OP_DIV, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator / (int64_t a, const IRecipe& b)
{ return IRecipe(OP_DIV, b.type(), {Recipe(a), b.notype()}); }
static inline IRecipe operator % (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_MOD, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator % (const IRecipe& a, int64_t b)
{ return IRecipe(OP_MOD, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator % (int64_t a, const IRecipe& b)
{ return IRecipe(OP_MOD, b.type(), {Recipe(a), b.notype()}); }
static inline IRecipe operator - (const IRecipe& a)
{ return IRecipe(OP_NEG, a.type(), {a.notype()}); }
static inline IRecipe operator >> (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_SAR, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator >> (const IRecipe& a, int64_t b)
{ return IRecipe(OP_SAR, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator >> (int64_t a, const IRecipe& b)
{ return IRecipe(OP_SAR, b.type(), {Recipe(a), b.notype()}); }
static inline IRecipe ushift_right(const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_SHR, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe ushift_right(const IRecipe& a, int64_t b)
{ return IRecipe(OP_SHR, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe ushift_right(int64_t a, const IRecipe& b)
{ return IRecipe(OP_SHR, b.type(), {Recipe(a), b.notype()}); }
static inline IRecipe operator <<(const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_SHL, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator <<(const IRecipe& a, int64_t b)
{ return IRecipe(OP_SHL, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator <<(int64_t a, const IRecipe& b)
{ return IRecipe(OP_SHL, b.type(), {Recipe(a), b.notype()}); }
static inline IRecipe operator & (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_AND, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator & (const IRecipe& a, int64_t b)
{ return IRecipe(OP_AND, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator & (int64_t a, const IRecipe& b)
{ return IRecipe(OP_AND, b.type(), {b.notype(), Recipe(a)}); }
static inline IRecipe operator | (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_OR, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator | (const IRecipe& a, int64_t b)
{ return IRecipe(OP_OR, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator | (int64_t a, const IRecipe& b)
{ return IRecipe(OP_OR, b.type(), {b.notype(), Recipe(a)}); }
static inline IRecipe operator ^ (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_XOR, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator ^ (const IRecipe& a, int64_t b)
{ return IRecipe(OP_XOR, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator ^ (int64_t a, const IRecipe& b)
{ return IRecipe(OP_XOR, b.type(), {b.notype(), Recipe(a)}); }
static inline IRecipe operator ~ (const IRecipe& a)
{ return IRecipe(OP_NOT, a.type(), {a.notype()}); }

// Comparisson and logical operations:
static inline IRecipe operator == (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_EQ, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator == (const IRecipe& a, int64_t b)
{ return IRecipe(OP_EQ, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator != (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_NE, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator != (const IRecipe& a, int64_t b)
{ return IRecipe(OP_NE, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator <= (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_LE, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator <= (const IRecipe& a, int64_t b)
{ return IRecipe(OP_LE, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe ule(const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_ULE, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe ule(const IRecipe& a, int64_t b)
{ return IRecipe(OP_ULE, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator >= (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_GE, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator >= (const IRecipe& a, int64_t b)
{ return IRecipe(OP_GE, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe uge(const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_ULE, a.type(), {b.notype(), a.notype()}); } //TODO(ch): Uge -> ule implementation is formed by ARM. Check for better ideas on Intel.
static inline IRecipe uge(const IRecipe& a, int64_t b)
{ return IRecipe(OP_ULE, a.type(), {Recipe(b), a.notype()}); }
static inline IRecipe operator > (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_GT, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator > (const IRecipe& a, int64_t b)
{ return IRecipe(OP_GT, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe ugt(const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_UGT, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe ugt(const IRecipe& a, int64_t b)
{ return IRecipe(OP_UGT, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe operator < (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_LT, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe operator < (const IRecipe& a, int64_t b)
{ return IRecipe(OP_LT, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe ult(const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_UGT, a.type(), {b.notype(), a.notype()}); }
static inline IRecipe ult(const IRecipe& a, int64_t b)
{ return IRecipe(OP_UGT, a.type(), {Recipe(b), a.notype()}); }
static inline IRecipe operator == (int64_t a, const IRecipe& b) { return b == a; }
static inline IRecipe operator != (int64_t a, const IRecipe& b) { return b != a; }
static inline IRecipe operator <= (int64_t a, const IRecipe& b) { return b >= a; }
static inline IRecipe ule         (int64_t a, const IRecipe& b) { return uge(b,a); }
static inline IRecipe operator >= (int64_t a, const IRecipe& b) { return b <= a; }
static inline IRecipe uge         (int64_t a, const IRecipe& b) { return ule(b, a);}
static inline IRecipe operator >  (int64_t a, const IRecipe& b) { return b < a; }
static inline IRecipe ugt         (int64_t a, const IRecipe& b) { return ult(b,a);}
static inline IRecipe operator <  (int64_t a, const IRecipe& b) { return b > a; }
static inline IRecipe ult         (int64_t a, const IRecipe& b) {return ugt(b,a);}
static inline IRecipe operator && (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_LOGICAL_AND, TYPE_BOOLEAN, {a.notype(), b.notype()}); }
static inline IRecipe operator || (const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_LOGICAL_OR, TYPE_BOOLEAN, {a.notype(), b.notype()}); }
static inline IRecipe operator ! (const IRecipe& a)
{ return IRecipe(OP_LOGICAL_NOT, TYPE_BOOLEAN, {a.notype()}); }

//Augmenting operations:
static inline IReg& operator += (IReg& _a, const IRecipe& b)
{ IRecipe a(_a); newiopNoret(OP_ADD, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator += (IReg& _a, int64_t b)
{ IRecipe a(_a); newiopNoret(OP_ADD, {a.notype(), a.notype(), Recipe(b)}); return _a; }
static inline IReg& operator -= (IReg& _a, const IRecipe& b)
{ IRecipe a(_a); newiopNoret(OP_SUB, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator -= (IReg& _a, int64_t b)
{ IRecipe a(_a); newiopNoret(OP_SUB, {a.notype(), a.notype(), Recipe(b)}); return _a; }
static inline IReg& operator *= (IReg& _a, const IRecipe& b)
{ IRecipe a(_a); newiopNoret(OP_MUL, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator *= (IReg& _a, int64_t b)
{ IRecipe a(_a); newiopNoret(OP_MUL, {a.notype(), a.notype(), Recipe(b)}); return _a; }
static inline IReg& operator /= (IReg& _a, const IRecipe& b)
{ IRecipe a(_a); newiopNoret(OP_DIV, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator /= (IReg& _a, int64_t b)
{ IRecipe a(_a); newiopNoret(OP_DIV, {a.notype(), a.notype(), Recipe(b)}); return _a; }
static inline IReg& operator %= (IReg& _a, const IRecipe& b)
{ IRecipe a(_a); newiopNoret(OP_MOD, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator %= (IReg& _a, int64_t b)
{ IRecipe a(_a); newiopNoret(OP_MOD, {a.notype(), a.notype(), Recipe(b)}); return _a; }
static inline IReg& operator >>= (IReg& _a, const IRecipe& b)
{ IRecipe a(_a); newiopNoret(OP_SAR, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator >>= (IReg& _a, int64_t b)
{ IRecipe a(_a); newiopNoret(OP_SAR, {a.notype(), a.notype(), Recipe(b)}); return _a; }
static inline IReg& operator <<= (IReg& _a, const IRecipe& b)
{ IRecipe a(_a); newiopNoret(OP_SHL, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator <<= (IReg& _a, int64_t b)
{ IRecipe a(_a); newiopNoret(OP_SHL, {a.notype(), a.notype(), Recipe(b)}); return _a; }
static inline IReg& operator &= (IReg& _a, const IRecipe& b)
{ IRecipe a(_a); newiopNoret(OP_AND, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator &= (IReg& _a, int64_t b)
{ IRecipe a(_a); newiopNoret(OP_AND, {a.notype(), a.notype(), Recipe(b)}); return _a; }
static inline IReg& operator |= (IReg& _a, const IRecipe& b)
{ IRecipe a(_a); newiopNoret(OP_OR, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator |= (IReg& _a, int64_t b)
{ IRecipe a(_a); newiopNoret(OP_OR, {a.notype(), a.notype(), Recipe(b)}); return _a; }
static inline IReg& operator ^= (IReg& _a, const IRecipe& b)
{ IRecipe a(_a); newiopNoret(OP_XOR, {a.notype(), a.notype(), b.notype()}); return _a; }
static inline IReg& operator ^= (IReg& _a, int64_t b)
{ IRecipe a(_a); newiopNoret(OP_XOR, {a.notype(), a.notype(), Recipe(b)}); return _a; }

//Other integer operations:
static inline IRecipe select(const IRecipe& cond, const IRecipe& true_, const IRecipe& false_)
{ return IRecipe(OP_SELECT, true_.type(), {cond.notype(), true_.notype(), false_.notype()}); }
static inline IRecipe select(const IRecipe& cond, int64_t true_, const IRecipe& false_)
{ return IRecipe(OP_SELECT, false_.type(), {cond.notype(), Recipe(true_), false_.notype()}); }
static inline IRecipe select(const IRecipe& cond, const IRecipe& true_, int64_t false_)
{ return IRecipe(OP_SELECT, true_.type(), {cond.notype(), true_.notype(), Recipe(false_)}); }
static inline IRecipe max(const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_MAX, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe max(const IRecipe& a, int64_t b)
{ return IRecipe(OP_MAX, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe max(int64_t a, const IRecipe& b)
{ return IRecipe(OP_MAX, b.type(), {b.notype(), Recipe(a)}); }
static inline IRecipe min(const IRecipe& a, const IRecipe& b)
{ return IRecipe(OP_MIN, a.type(), {a.notype(), b.notype()}); }
static inline IRecipe min(const IRecipe& a, int64_t b)
{ return IRecipe(OP_MIN, a.type(), {a.notype(), Recipe(b)}); }
static inline IRecipe min(int64_t a, const IRecipe& b)
{ return IRecipe(OP_MIN, b.type(), {b.notype(), Recipe(a)}); }
static inline IRecipe abs(const IRecipe& a)
{ return IRecipe(OP_ABS, a.type(), {a.notype()}); }
static inline IRecipe sign(const IRecipe& a)
{ return IRecipe(OP_SIGN, a.type(), {a.notype()}); }

///////////////////////////// vector operations ///////////////////////
#define LOOPS_CONVERT_ARGS_TBINARY(bfunc) \
template<typename _Tp> VRecipe<_Tp> bfunc (const VReg<_Tp>& a, const VReg<_Tp>& b) { return bfunc(VRecipe<_Tp>::make(a), VRecipe<_Tp>::make(b)); } \
template<typename _Tp> VRecipe<_Tp> bfunc (const VReg<_Tp>& a, const VRecipe<_Tp>& b){ return bfunc(VRecipe<_Tp>::make(a), b); } \
template<typename _Tp> VRecipe<_Tp> bfunc (const VRecipe<_Tp>& a, const VReg<_Tp>& b){ return bfunc(a, VRecipe<_Tp>::make(b)); }

#define LOOPS_CONVERT_ARGS_TBIN_INTSUF(bfunc) \
template<typename _Tp> VRecipe<_Tp> bfunc(const VReg<_Tp>& a, int64_t suf) { return bfunc(VRecipe<_Tp>::make(a), suf); }

#define LOOPS_CONVERT_ARGS_TUNARY(ufunc) \
template<typename _Tp> VRecipe<_Tp> ufunc (const VReg<_Tp>& a) { return ufunc(VRecipe<_Tp>::make(a)); } \

#define LOOPS_CONVERT_ARGS_TUNARY_DOUT(otype, ufunc) template<typename _Tp> VRecipe<otype> ufunc (const VReg<_Tp>& a) { return ufunc(VRecipe<otype>::make(a)); }

#define LOOPS_CONVERT_ARGS_TUNARY_DIN(itype, ufunc) \
template<typename _Tp> VRecipe<_Tp> ufunc(const VReg<itype>& a) { return ufunc(VRecipe<_Tp>::make(a));}

#define LOOPS_CONVERT_ARGS_TBINARY_2TYPES(bfunc) \
template<typename _Tp, typename _Sp> VRecipe<_Tp> bfunc (const VReg<_Tp>& a, const VReg<_Sp>& b)   { return bfunc(VRecipe<_Tp>::make(a), VRecipe<_Sp>::make(b)); } \
template<typename _Tp, typename _Sp> VRecipe<_Tp> bfunc (const VReg<_Tp>& a, const VRecipe<_Sp>& b){ return bfunc(VRecipe<_Tp>::make(a), b); } \
template<typename _Tp, typename _Sp> VRecipe<_Tp> bfunc (const VRecipe<_Tp>& a, const VReg<_Sp>& b){ return bfunc(a, VRecipe<_Sp>::make(b)); }

#define LOOPS_CONVERT_ARGS_TBINARY_DOUT(otype, bfunc) \
template<typename _Tp> VRecipe<otype> bfunc (const VReg<_Tp>& a, const VReg<_Tp>& b) { return bfunc(VRecipe<_Tp>::make(a), VRecipe<_Tp>::make(b)); } \
template<typename _Tp> VRecipe<otype> bfunc (const VReg<_Tp>& a, const VRecipe<_Tp>& b){ return bfunc(VRecipe<_Tp>::make(a), b); } \
template<typename _Tp> VRecipe<otype> bfunc (const VRecipe<_Tp>& a, const VReg<_Tp>& b){ return bfunc(a, VRecipe<_Tp>::make(b)); }

#define LOOPS_CONVERT_ARGS_TBINARY_RMASK(bfunc) LOOPS_CONVERT_ARGS_TBINARY_DOUT(typename ElemTraits<_Tp>::masktype, bfunc)

#define LOOPS_CONVERT_ARGS_TAUG(afunc) \
template<typename _Tp> VReg<_Tp>& afunc (VReg<_Tp>& _a, const VReg<_Tp>& b) { return afunc(_a, VRecipe<_Tp>::make(b)); }

// Load with zero/sign extension:
template<typename _Tp> VRecipe<_Tp> loadvec(const IRecipe& base)
{ return VRecipe<_Tp>(VOP_LOAD, {base.notype()}); }
template<typename _Tp> VRecipe<_Tp> loadvec(const IRecipe& base, const IRecipe& offset)
{ return VRecipe<_Tp>(VOP_LOAD, {base.notype(), offset.notype()}); }
template<typename _Tp> VRecipe<_Tp> loadvec(const IRecipe& base, int64_t offset)
{ return VRecipe<_Tp>(VOP_LOAD, {base.notype(), Recipe(offset)}); }
template<typename _Tp> VRecipe<_Tp> loadlane(const IRecipe& base, int64_t lane_index)
{ return VRecipe<_Tp>(VOP_ARM_LD1, {Recipe(lane_index), base.notype()}); }
//TODO(ch): find a way to delete next warning:
//WARNING! It's assumed here, that res1 and res2 are not initialized yet.
//template<typename _Tp> std::pair<Recipe, Recipe> loadvec_deinterleave2(const Recipe& base); //TODO(ch): optimal form of signature
void loadvec_deinterleave2_(Arg& res1, Arg& res2, const IRecipe& base);
template<typename _Tp> void loadvec_deinterleave2(VReg<_Tp>& res1, VReg<_Tp>& res2, const IRecipe& base)
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
template<typename _Tp> void storevec(const IRecipe& base, const VRecipe<_Tp>& r)
{ newiopNoret(VOP_STORE, {base.notype(), r.notype()}); }
template<typename _Tp> void storevec(const IRecipe& base, const IRecipe& offset, const VRecipe<_Tp>& r)
{ newiopNoret(VOP_STORE, {base.notype(), offset.notype(), r.notype()}); }
template<typename _Tp> void storelane(const IRecipe& base, const VRecipe<_Tp>& r, int64_t lane_index)
{ newiopNoret(VOP_ARM_ST1, {base.notype(), r.notype(), Recipe(lane_index)}); }

// Casts:
template<typename _Dp, typename _Tp> VRecipe<_Dp> cast(const VRecipe<_Tp>& a)
{ return VRecipe<_Dp>(VOP_CAST, {a.notype()}); }
template<typename _Dp, typename _Tp> VRecipe<_Dp> reinterpret(const VRecipe<_Tp>& a)
{ return VRecipe<_Dp>(VOP_REINTERPRET, {a.notype()});}
template<typename _Dp> VRecipe<_Dp> trunc(const VRecipe<f16_t>& a)  //Convert with rounding to zero
{
    static_assert(ElemTraits<_Dp>::depth == TYPE_I16 || ElemTraits<_Dp>::depth == TYPE_U16, "Attempt to convert real number to integer of different size or not integer.");
    return VRecipe<_Dp>(VOP_TRUNC, {a.notype()});
}
template<typename _Dp> VRecipe<_Dp> trunc(const VRecipe<float>& a)  //Convert with rounding to zero
{
    static_assert(ElemTraits<_Dp>::depth == TYPE_I32 || ElemTraits<_Dp>::depth == TYPE_U32, "Attempt to convert real number to integer of different size or not integer.");
    return VRecipe<_Dp>(VOP_TRUNC, {a.notype()});
}
template<typename _Dp> VRecipe<_Dp> trunc(const VRecipe<double>& a)  //Convert with rounding to zero
{
    static_assert(ElemTraits<_Dp>::depth == TYPE_I64 || ElemTraits<_Dp>::depth == TYPE_U64, "Attempt to convert real number to integer of different size or not integer.");
    return VRecipe<_Dp>(VOP_TRUNC, {a.notype()});
}
template<typename _Dp> VRecipe<_Dp> floor(const VRecipe<f16_t>& a)  //Convert with rounding to minus infinity
{
    static_assert(ElemTraits<_Dp>::depth == TYPE_I16 || ElemTraits<_Dp>::depth == TYPE_U16, "Attempt to convert real number to integer of different size or not integer.");
    return VRecipe<_Dp>(VOP_FLOOR, {a.notype()});
}
template<typename _Dp> VRecipe<_Dp> floor(const VRecipe<float>& a)  //Convert with rounding to minus infinity
{
    static_assert(ElemTraits<_Dp>::depth == TYPE_I32 || ElemTraits<_Dp>::depth == TYPE_U32, "Attempt to convert real number to integer of different size or not integer.");
    return VRecipe<_Dp>(VOP_FLOOR, {a.notype()});
}
template<typename _Dp> VRecipe<_Dp> floor(const VRecipe<double>& a)  //Convert with rounding to minus infinity
{
    static_assert(ElemTraits<_Dp>::depth == TYPE_I64 || ElemTraits<_Dp>::depth == TYPE_U64, "Attempt to convert real number to integer of different size or not integer.");
    return VRecipe<_Dp>(VOP_FLOOR, {a.notype()});
}

//TODO(ch): cvtTp -> ceil, cvtTe -> round, also cast(double <=> float, float <=> f16_t)
template<typename _Tp> VRecipe<_Tp> broadcast(const IRecipe& scalar)
{ return VRecipe<_Tp>(VOP_BROADCAST, { scalar.notype() }); }
template<typename _Tp> VRecipe<_Tp> broadcast(const VRecipe<_Tp>& inp, int64_t ilane_index)
{ return VRecipe<_Tp>(VOP_BROADCAST, { inp.notype(), Recipe(ilane_index) }); }
template<typename _Tp> VRecipe<typename ElemTraits<_Tp>::duplicatetype> cast_low(const VRecipe<_Tp>& r)
{ return VRecipe<typename ElemTraits<_Tp>::duplicatetype>(VOP_CAST_LOW, { r.notype() }); }
template<typename _Tp> VRecipe<typename ElemTraits<_Tp>::duplicatetype> cast_high(const VRecipe<_Tp>& r)
{ return VRecipe<typename ElemTraits<_Tp>::duplicatetype>(VOP_CAST_HIGH, { r.notype() }); }
template<typename _Tp> VRecipe<typename ElemTraits<_Tp>::halftype> shrink(const VRecipe<_Tp>& r0, const VRecipe<_Tp>& r1)
{ return VRecipe<typename ElemTraits<_Tp>::half_type>(VOP_SHRINK, {r0.notype(),r1.notype()}); }

//Lane manipulations:
template<typename _Tp> IRecipe getlane(const VRecipe<_Tp>& r, int64_t lane_index)
{ return IRecipe(VOP_GETLANE, ElemTraits<_Tp>::depth, {r.notype(), Recipe(lane_index)}); }
template<typename _Tp> void setlane(const VRecipe<_Tp>& v, int64_t lane_index, const IRecipe& i)
{ newiopNoret(VOP_SETLANE, {v.notype(), Recipe(lane_index), i.notype()}); }
template<typename _Tp> void setlane(const VRecipe<_Tp>& v, int64_t lane_index, const VRecipe<_Tp>& inp, int64_t ilane_index)
{ newiopNoret(VOP_SETLANE, {v.notype(), Recipe(lane_index), inp.notype(), Recipe(ilane_index)}); }
template<typename _Tp> VRecipe<_Tp> reduce_max(const VRecipe<_Tp>& r)
{ return VRecipe<_Tp>(VOP_REDUCE_MAX, { r.notype() }); }
template<typename _Tp> VRecipe<_Tp> reduce_min(const VRecipe<_Tp>& r)
{ return VRecipe<_Tp>(VOP_REDUCE_MIN, { r.notype() }); }
template<typename _Tp> VRecipe<_Tp> ext(const VRecipe<_Tp>& n, const VRecipe<_Tp>& m, int64_t index)
{ return VRecipe<_Tp>(VOP_ARM_EXT, {n.notype(), m.notype(), Recipe(index)}); }

// Arithmetic and bitwise operations:
template<typename _Tp> VRecipe<_Tp> operator + (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<_Tp>(VOP_ADD, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<_Tp> operator - (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<_Tp>(VOP_SUB, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<_Tp> operator * (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<_Tp>(VOP_MUL, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<_Tp> operator / (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<_Tp>(VOP_DIV, {a.notype(), b.notype()}); }
//template<typename _Tp> VRecipe<_Tp> operator % (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b);
template<typename _Tp> VRecipe<_Tp> operator - (const VRecipe<_Tp>& a)
{ return VRecipe<_Tp>(VOP_NEG, {a.notype()}); }

template<typename _Tp> VRecipe<_Tp> fma(const VRecipe<_Tp>& a, const VRecipe<_Tp>& b, const VRecipe<_Tp>& c)
{ return VRecipe<_Tp>(VOP_FMA, {a.notype(), b.notype(), c.notype()}); }
template<typename _Tp> VRecipe<_Tp> fma(const VRecipe<_Tp>& a, const VRecipe<_Tp>& b, const VRecipe<_Tp>& c, int64_t index)
{ return VRecipe<_Tp>(VOP_FMA, {a.notype(), b.notype(), c.notype(), Recipe(index)}); }

template<typename _Tp>
VRecipe<_Tp> pow(const VRecipe<_Tp>& a, int p)
{
    Context CTX = ExtractContext(a.notype());
    USE_CONTEXT_(CTX);
    if(p == 0)
        return VCONST_(_Tp, 1);
    VRecipe<_Tp> _a = a;
    VRecipe<_Tp> res;
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
template<typename _Tp> VRecipe<_Tp> pow(const VReg<_Tp>& a, int p) { return pow(VRecipe<_Tp>::make(a),p); }

struct exp_consts
{
    VReg<float> lo, hi, half, one, LOG2EF, C1, C2, p0, p1, p2, p3, p4, p5;
    VReg<int32_t> _7f;
    exp_consts(Context CTX);
};

template<typename _Tp> VRecipe<_Tp> operator >> (const VRecipe<_Tp>& a, int64_t b)
{ return VRecipe<_Tp>(VOP_SAR, {a.notype(), Recipe(b)}); }
template<typename _Tp> VRecipe<_Tp> ushift_right (const VRecipe<_Tp>& a, int64_t b)
{ return VRecipe<_Tp>(VOP_SHR, {a.notype(), Recipe(b)}); }
template<typename _Tp, typename _Sp> VRecipe<_Tp> operator << (const VRecipe<_Tp>& a, const VRecipe<_Sp>& b)
{
    static_assert(sizeof(_Tp) == sizeof(_Sp), "the # of lanes in the 1st and 2nd argument must be the same");
    return VRecipe<_Tp>(VOP_SAL, {a.notype(), b.notype()});
}
template<typename _Tp> VRecipe<_Tp> operator << (const VRecipe<_Tp>& a, int64_t b)
{ return VRecipe<_Tp>(VOP_SAL, {a.notype(), Recipe(b)}); }

template<typename _Tp, typename _Sp> VRecipe<_Tp> ushift_left(const VRecipe<_Tp>& a, const VRecipe<_Sp>& b)
{
    static_assert(sizeof(_Tp) == sizeof(_Sp), "the # of lanes in the 1st and 2nd argument must be the same");
    return VRecipe<_Tp>(VOP_SHL, {a.notype(), Recipe(b)});
}
template<typename _Tp> VRecipe<_Tp> operator & (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<_Tp>(VOP_AND, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<_Tp> operator | (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<_Tp>(VOP_OR, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<_Tp> operator ^ (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<_Tp>(VOP_XOR, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<_Tp> operator ~ (const VRecipe<_Tp>& a)
{ return VRecipe<_Tp>(VOP_NOT, {a.notype()}); }

// Vector comparisson and masking:
template<typename _Tp> VRecipe<typename ElemTraits<_Tp>::masktype> operator == (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<typename ElemTraits<_Tp>::masktype>(VOP_EQ, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<typename ElemTraits<_Tp>::masktype> operator != (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<typename ElemTraits<_Tp>::masktype>(VOP_NE, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<typename ElemTraits<_Tp>::masktype> operator >= (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<typename ElemTraits<_Tp>::masktype>(VOP_GE, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<typename ElemTraits<_Tp>::masktype> operator <= (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<typename ElemTraits<_Tp>::masktype>(VOP_LE, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<typename ElemTraits<_Tp>::masktype> operator > (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<typename ElemTraits<_Tp>::masktype>(VOP_GT, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<typename ElemTraits<_Tp>::masktype> operator < (const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<typename ElemTraits<_Tp>::masktype>(VOP_LT, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<_Tp> select(const VRecipe<typename ElemTraits<_Tp>::masktype>& flag, const VRecipe<_Tp>& iftrue, const VRecipe<_Tp>& iffalse)
{ return VRecipe<_Tp>(VOP_SELECT, {flag.notype(), iftrue.notype(), iffalse.notype()}); }
template<typename _Tp> VRecipe<_Tp> max(const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<_Tp>(VOP_MAX, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<_Tp> min(const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<_Tp>(VOP_MIN, {a.notype(), b.notype()}); }

//Augmenting operations:
template<typename _Tp> VReg<_Tp>& operator += (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a = VRecipe<_Tp>::make(_a); newiopNoret(VOP_ADD, {a.notype(), a.notype(), b.notype()}); return _a; }
template<typename _Tp> VReg<_Tp>& operator -= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a = VRecipe<_Tp>::make(_a); newiopNoret(VOP_SUB, {a.notype(), a.notype(), b.notype()}); return _a; }
template<typename _Tp> VReg<_Tp>& operator *= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a = VRecipe<_Tp>::make(_a); newiopNoret(VOP_MUL, {a.notype(), a.notype(), b.notype()}); return _a; }
template<typename _Tp> VReg<_Tp>& operator /= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a = VRecipe<_Tp>::make(_a); newiopNoret(VOP_DIV, {a.notype(), a.notype(), b.notype()}); return _a; }
//template<typename _Tp> VReg<_Tp>& operator %= (VReg<_Tp>& _a, const VRecipe<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& _a, int64_t _b)
{ VRecipe<_Tp> a = VRecipe<_Tp>::make(_a);  Recipe b(_b); newiopNoret(VOP_SAR, {a.notype(), a.notype(), b}); return _a; }
template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& _a, VRecipe<_Tp> b)
{ VRecipe<_Tp> a = VRecipe<_Tp>::make(_a); newiopNoret(VOP_SAL, {a.notype(), a.notype(), b.notype()}); return _a; }
template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& _a, int64_t _b)
{ VRecipe<_Tp> a = VRecipe<_Tp>::make(_a);  Recipe b(_b); newiopNoret(VOP_SAL, {a.notype(), a.notype(), b}); return _a; }
template<typename _Tp> VReg<_Tp>& operator &= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a = VRecipe<_Tp>::make(_a); newiopNoret(VOP_AND, {a.notype(), a.notype(), b.notype()} ); return _a; }
template<typename _Tp> VReg<_Tp>& operator |= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a = VRecipe<_Tp>::make(_a); newiopNoret(VOP_OR, {a.notype(), a.notype(), b.notype()} ); return _a; }
template<typename _Tp> VReg<_Tp>& operator ^= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a = VRecipe<_Tp>::make(_a); newiopNoret(VOP_XOR, {a.notype(), a.notype(), b.notype()} ); return _a; }

//Argument conversions:
template<typename _Tp> void storevec(const IRecipe& base, const VReg<_Tp>& r){ storevec(base, VRecipe<_Tp>::make(r)); }
template<typename _Tp> void storevec(const IRecipe& base, const IRecipe& offset, const VReg<_Tp>& r) { storevec(base,offset, VRecipe<_Tp>::make(r));}
template<typename _Tp> void storelane(const IRecipe& base, const VReg<_Tp>& r, int64_t lane_index) {storelane(base, VRecipe<_Tp>::make(r), lane_index);}

template<typename _Dp, typename _Tp> VRecipe<_Dp> cast(const VReg<_Tp>& a) { return cast<_Dp>(VRecipe<_Tp>::make(a));}
template<typename _Dp, typename _Tp> VRecipe<_Dp> reinterpret(const VReg<_Tp>& a) { return reinterpret<_Dp>(VRecipe<_Tp>::make(a));}

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

template<typename _Tp> IRecipe getlane(const VReg<_Tp>& r, int64_t lane_index) { return getlane(VRecipe<_Tp>::make(r), lane_index); }
template<typename _Tp> void setlane(const VReg<_Tp>& v, int64_t lane_index, const IRecipe& i) { setlane(VRecipe<_Tp>::make(v), lane_index, i); }
template<typename _Tp> void setlane(const VRecipe<_Tp>& v, int64_t lane_index, const    VReg<_Tp>& inp, int64_t ilane_index) { setlane(v, lane_index, VRecipe<_Tp>::make(inp), ilane_index); }
template<typename _Tp> void setlane(const    VReg<_Tp>& v, int64_t lane_index, const VRecipe<_Tp>& inp, int64_t ilane_index) { setlane(VRecipe<_Tp>::make(v), lane_index, inp, ilane_index); }
template<typename _Tp> void setlane(const    VReg<_Tp>& v, int64_t lane_index, const    VReg<_Tp>& inp, int64_t ilane_index) { setlane(VRecipe<_Tp>::make(v), lane_index, VRecipe<_Tp>::make(inp), ilane_index); }
LOOPS_CONVERT_ARGS_TUNARY(reduce_max)
LOOPS_CONVERT_ARGS_TUNARY(reduce_min)
template<typename _Tp> VRecipe<_Tp> ext(const VRecipe<_Tp>& n, const    VReg<_Tp>& m, int64_t index) { return ext(n, VRecipe<_Tp>::make(m), index); }
template<typename _Tp> VRecipe<_Tp> ext(const    VReg<_Tp>& n, const VRecipe<_Tp>& m, int64_t index) { return ext(VRecipe<_Tp>::make(n), m, index); }
template<typename _Tp> VRecipe<_Tp> ext(const    VReg<_Tp>& n, const    VReg<_Tp>& m, int64_t index) { return ext(VRecipe<_Tp>::make(n), VRecipe<_Tp>::make(m), index); }

template<typename _Tp> VRecipe<_Tp> fma(const VRecipe<_Tp>& a, const VRecipe<_Tp>& b, const VReg<_Tp>& c) { return fma(a, b, VRecipe<_Tp>::make(c)); }
template<typename _Tp> VRecipe<_Tp> fma(const VRecipe<_Tp>& a, const VReg<_Tp>& b, const VRecipe<_Tp>& c) { return fma(a, VRecipe<_Tp>::make(b), c); }
template<typename _Tp> VRecipe<_Tp> fma(const VRecipe<_Tp>& a, const VReg<_Tp>& b, const VReg<_Tp>& c) { return fma(a, VRecipe<_Tp>::make(b), VRecipe<_Tp>::make(c)); }
template<typename _Tp> VRecipe<_Tp> fma(const VReg<_Tp>& a, const VRecipe<_Tp>& b, const VRecipe<_Tp>& c) { return fma(VRecipe<_Tp>::make(a), b, c); }
template<typename _Tp> VRecipe<_Tp> fma(const VReg<_Tp>& a, const VRecipe<_Tp>& b, const VReg<_Tp>& c) { return fma(VRecipe<_Tp>::make(a), b, VRecipe<_Tp>::make(c)); }
template<typename _Tp> VRecipe<_Tp> fma(const VReg<_Tp>& a, const VReg<_Tp>& b, const VRecipe<_Tp>& c) { return fma(VRecipe<_Tp>::make(a), VRecipe<_Tp>::make(b), c); }
template<typename _Tp> VRecipe<_Tp> fma(const VReg<_Tp>& a, const VReg<_Tp>& b, const VReg<_Tp>& c) { return fma(VRecipe<_Tp>::make(a), VRecipe<_Tp>::make(b), VRecipe<_Tp>::make(c));}
template<typename _Tp> VRecipe<_Tp> fma(const VRecipe<_Tp>& a, const VRecipe<_Tp>& b, const VReg<_Tp>& c, int64_t index) { return fma(a, b, VRecipe<_Tp>::make(c), index); }
template<typename _Tp> VRecipe<_Tp> fma(const VRecipe<_Tp>& a, const VReg<_Tp>& b, const VRecipe<_Tp>& c, int64_t index) { return fma(a, VRecipe<_Tp>::make(b), c, index); }
template<typename _Tp> VRecipe<_Tp> fma(const VRecipe<_Tp>& a, const VReg<_Tp>& b, const VReg<_Tp>& c, int64_t index) { return fma(a, VRecipe<_Tp>::make(b), VRecipe<_Tp>::make(c), index); }
template<typename _Tp> VRecipe<_Tp> fma(const VReg<_Tp>& a, const VRecipe<_Tp>& b, const VRecipe<_Tp>& c, int64_t index) { return fma(VRecipe<_Tp>::make(a), b, c, index); }
template<typename _Tp> VRecipe<_Tp> fma(const VReg<_Tp>& a, const VRecipe<_Tp>& b, const VReg<_Tp>& c, int64_t index) { return fma(VRecipe<_Tp>::make(a), b, VRecipe<_Tp>::make(c), index); }
template<typename _Tp> VRecipe<_Tp> fma(const VReg<_Tp>& a, const VReg<_Tp>& b, const VRecipe<_Tp>& c, int64_t index) { return fma(VRecipe<_Tp>::make(a), VRecipe<_Tp>::make(b), c, index); }
template<typename _Tp> VRecipe<_Tp> fma(const VReg<_Tp>& a, const VReg<_Tp>& b, const VReg<_Tp>& c, int64_t index) { return fma(VRecipe<_Tp>::make(a), VRecipe<_Tp>::make(b), VRecipe<_Tp>::make(c), index);}

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

template<typename _Tp> VRecipe<_Tp> select(const VRecipe<typename ElemTraits<_Tp>::masktype>& flag, const VRecipe<_Tp>& iftrue, const    VReg<_Tp>& iffalse)
{ return select(flag, iftrue, VRecipe<_Tp>::make(iffalse)); }
template<typename _Tp> VRecipe<_Tp> select(const VRecipe<typename ElemTraits<_Tp>::masktype>& flag, const    VReg<_Tp>& iftrue, const VRecipe<_Tp>& iffalse)
{ return select(flag, VRecipe<_Tp>::make(iftrue), iffalse); }
template<typename _Tp> VRecipe<_Tp> select(const VRecipe<typename ElemTraits<_Tp>::masktype>& flag, const    VReg<_Tp>& iftrue, const    VReg<_Tp>& iffalse)
{ return select(flag, VRecipe<_Tp>::make(iftrue), VRecipe<_Tp>::make(iffalse)); }
template<typename _Tp> VRecipe<_Tp> select(const    VReg<typename ElemTraits<_Tp>::masktype>& flag, const VRecipe<_Tp>& iftrue, const VRecipe<_Tp>& iffalse)
{ return select(VRecipe<typename ElemTraits<_Tp>::masktype>::make(flag), iftrue, iffalse); }
template<typename _Tp> VRecipe<_Tp> select(const    VReg<typename ElemTraits<_Tp>::masktype>& flag, const VRecipe<_Tp>& iftrue, const    VReg<_Tp>& iffalse)
{ return select(VRecipe<typename ElemTraits<_Tp>::masktype>::make(flag), iftrue, VRecipe<_Tp>::make(iffalse)); }
template<typename _Tp> VRecipe<_Tp> select(const    VReg<typename ElemTraits<_Tp>::masktype>& flag, const    VReg<_Tp>& iftrue, const VRecipe<_Tp>& iffalse)
{ return select(VRecipe<typename ElemTraits<_Tp>::masktype>::make(flag), VRecipe<_Tp>::make(iftrue), iffalse); }
template<typename _Tp> VRecipe<_Tp> select(const    VReg<typename ElemTraits<_Tp>::masktype>& flag, const    VReg<_Tp>& iftrue, const    VReg<_Tp>& iffalse)
{ return select(VRecipe<typename ElemTraits<_Tp>::masktype>::make(flag), VRecipe<_Tp>::make(iftrue), VRecipe<_Tp>::make(iffalse)); }

static inline VRecipe<float> exp(const VReg<float>& x, const exp_consts& expt) { return exp(VRecipe<float>::make(x), expt); }

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
