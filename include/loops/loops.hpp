/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_LOOPS_HPP__
#define __LOOPS_LOOPS_HPP__

#include <inttypes.h>
#include <ostream>
#include <string>
#include <cstring>
#include <vector>
#include <stdexcept>
#include "defines.hpp"

namespace loops
{

struct f16_t {
    f16_t();
    explicit f16_t(float x);
    operator float();
    uint16_t bits;
};

enum {
    TYPE_U8=0, TYPE_I8=1, TYPE_U16=2, TYPE_I16=3,
    TYPE_U32=4, TYPE_I32=5, TYPE_U64=6, TYPE_I64=7,
    TYPE_FP16=8, TYPE_BF16=9, TYPE_FP32=10, TYPE_FP64=11, 
    TYPE_BOOLEAN};

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

    OP_GT,
    OP_UGT,
    OP_GE,
    OP_LT,
    OP_LE,
    OP_ULE,
    OP_NE,
    OP_EQ,
    OP_S,
    OP_NS,

    OP_LOGICAL_AND,
    OP_LOGICAL_OR,
    OP_LOGICAL_NOT,

    OP_JMP,         //OP_JMP <target_label>             //TODO(ch): keep there more annotations
    OP_JCC,         //OP_JCC <cmpcode>, <target_label>
    OP_RET,
    OP_LABEL,

    OP_STEM_CSTART,

    OP_IF_CSTART,   
    OP_ELIF_CSTART, //OP_ELIF_CSTART <elselabel, outlabel>
    OP_IF_CEND,     //OP_IF_CEND
    OP_ELSE,        //OP_ELSE <elselabel, outlabel>
    OP_ENDIF,       //OP_ENDIF <outlabel> 

    OP_WHILE_CSTART,//OP_WHILE_CSTART <continuelabel>
    OP_WHILE_CEND,  //OP_WHILE_CEND
    OP_ENDWHILE,    //OP_ENDWHILE <continuelabel>, <breaklabel>
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
    VOP_SELECT,
    
    VOP_ALL,
    VOP_ANY,
    VOP_TRUNC,
    VOP_FLOOR,
    VOP_CAST,
    VOP_REINTERPRET,
    VOP_BROADCAST,
    VOP_CAST_LOW,
    VOP_CAST_HIGH,
    VOP_SHRINK_LOW,
    VOP_SHRINK_HIGH,
    VOP_SHRINK,
    VOP_REDUCE_MAX,
    VOP_REDUCE_MIN,

//Intel-only operations:
    OP_X86_ADC, //Add with carry flag.
    OP_X86_CQO,
//Aarch64-only operations:
    OP_ARM_CINC,
    OP_ARM_CNEG,
    OP_ARM_MOVK,   //Move bytes to shifted byte position of register and keep other bits unchanged.
    VOP_ARM_LD1,   //TODO(ch) : check if there exists analogues on Intel and try to move it to common block.
    VOP_ARM_ST1,
    VOP_ARM_LD2,
    VOP_ARM_EXT,
    VOP_GETLANE,
    VOP_SETLANE,

    OP_DEF,
    VOP_DEF,

    OP_NOINIT,
};

enum
{
    RECIPE_LEAF = OP_NOINIT + 1
};

template<typename _Tp> struct ElemTraits {};
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

static inline size_t elem_size(int typ)
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
static inline int mask_type(int typ)
{
    switch (typ) {
        case TYPE_I8:
        case TYPE_U8:
            return TYPE_U8;
        case TYPE_I16:
        case TYPE_U16:
        case TYPE_FP16:
        case TYPE_BF16:
            return TYPE_U16;
        case TYPE_I32:
        case TYPE_U32:
        case TYPE_FP32:
            return TYPE_U32;
        case TYPE_I64:
        case TYPE_U64:
        case TYPE_FP64:
            return TYPE_U64;
        default:
            throw std::runtime_error("Unknown data type.");
    }
}

static inline int half_type(int typ)
{
    switch (typ) {
        case TYPE_I16:
            return TYPE_I8;
        case TYPE_U16:
            return TYPE_U8;
        case TYPE_I32:
            return TYPE_I16;
        case TYPE_U32:
            return TYPE_U16;
        case TYPE_I64:
            return TYPE_I32;
        case TYPE_U64:
            return TYPE_U32;
        default:
            throw std::runtime_error("Half-type is not supported for types, lesser, than 16 bits and floats.");
    }
}

static inline int duplicate_type(int typ)
{
    switch (typ) {
        case TYPE_I8:
            return TYPE_I16;
        case TYPE_U8:
            return TYPE_U16;
        case TYPE_I16:
            return TYPE_I32;
        case TYPE_U16:
            return TYPE_U32;
        case TYPE_I32:
            return TYPE_I64;
        case TYPE_U32:
            return TYPE_U64;
        default:
            throw std::runtime_error("Duplicate-type is not supported for types, bigger, than 32 bits and floats.");
    }
}

class Func;
class Recipe;
class IRecipe;
struct IReg
{
    IReg();
    IReg(const IReg& r); //Must generate copy(mov) code
    IReg(const IRecipe& fromwho);
    IReg& operator=(const IReg& r); // may generate real code if 'this' is already initialized
    IReg& operator=(const IRecipe& fromwho);
    /*
    copyidx is a way to work with IReg/VReg like with regular objects, like it needed for sophisticated generation logic.
    Unlike usual copy, this function doesn't have any effects, it doesn't change current buffer of function.
    Two most obvious usages are late initializtion for registers are elements of dynamic arrays and register 
    aliases(sometimes it's convinient to use different variables in one expression accordingly to current
    generation situation.) 
    */   
    void copyidx(const IReg& from);

    int idx;
    Func* func;
    enum {NOIDX = -1};
};

class Context;
template <typename _Tp> class VRecipe;
template<typename _Tp> struct VReg
{
    typedef _Tp elemtype;

    VReg() : idx(NOIDX), func(nullptr) {}
    VReg(const VReg<_Tp>& r);
    VReg(const VRecipe<_Tp>& fromwho);
    VReg<_Tp>& operator=(const VReg<_Tp>& r);
    VReg<_Tp>& operator=(const VRecipe<_Tp>& fromwho);
     /*
    copyidx is a way to work with IReg/VReg like with regular objects, like it needed for sophisticated generation logic.
    Unlike usual copy, this function doesn't have any effects, it doesn't change current buffer of function.
    Two most obvious usages are late initializtion for registers are elements of dynamic arrays and register 
    aliases(sometimes it's convinient to use different variables in one expression accordingly to current
    generation situation.) 
    */   
    void copyidx(const VReg<_Tp>& from);

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

struct __loops_RecipeStr_;
class Recipe
{
public:
    inline Recipe();
    inline Recipe(const Recipe& fromwho);
    inline Recipe& operator=(const Recipe& fromwho);
    inline Recipe(const Arg& a_leaf);
    inline Recipe(int64_t a_leaf);
    inline ~Recipe();
    inline int& opcode();
    inline bool& is_vector();
    inline bool is_leaf() const;
    inline int& type();
    inline Arg& leaf();
    inline std::vector<Recipe>& children();
    inline int opcode() const;
    inline bool is_vector() const ;
    inline int type() const;
    inline const Arg& leaf() const;
    inline const std::vector<Recipe>& children() const;
    inline bool empty() const ;
    //+add assertions on class construction.
    __loops_RecipeStr_* pointee;
};

class IRecipe
{
public:
    Recipe super;
    IRecipe() {}
    // inline IRecipe(const IRecipe& fromwho) : Recipe(fromwho) {}
    inline IRecipe(const IReg& a_leaf);
    inline IRecipe(int a_opcode, int a_type, std::initializer_list<Recipe> a_children);
    Recipe notype() const { return super; }

    inline int& opcode() {return super.opcode();} 
    inline bool& is_vector() {return super.is_vector();} 
    inline bool is_leaf() const {return super.is_leaf();} 
    inline int& type() {return super.type();} 
    inline Arg& leaf() {return super.leaf();} 
    inline std::vector<Recipe>& children() {return super.children();} 
    inline int opcode() const {return super.opcode();} 
    inline bool is_vector() const {return super.is_vector();} 
    inline int type() const {return super.type();} 
    inline const Arg& leaf() const {return super.leaf();} 
    inline const std::vector<Recipe>& children() const {return super.children();} 
    inline bool empty() const {return super.empty();} 

};

template <typename _Tp>
class VRecipe
{
public:
    Recipe super;

    VRecipe() {}
    inline VRecipe(const VReg<_Tp>& a_leaf);
    inline VRecipe(int a_opcode, std::initializer_list<Recipe> a_children);
    Recipe notype() const {return super;}

    inline int& opcode() {return super.opcode();} 
    inline bool& is_vector() {return super.is_vector();} 
    inline bool is_leaf() const {return super.is_leaf();} 
    inline int& type() {return super.type();} 
    inline Arg& leaf() {return super.leaf();} 
    inline std::vector<Recipe>& children() {return super.children();} 
    inline int opcode() const {return super.opcode();} 
    inline bool is_vector() const {return super.is_vector();} 
    inline int type() const {return super.type();} 
    inline const Arg& leaf() const {return super.leaf();} 
    inline const std::vector<Recipe>& children() const {return super.children();} 
    inline bool empty() const {return super.empty();}     
};

struct __loops_RecipeStr_
{
    int opcode;
    bool is_vector;
    int type;
    Arg leaf;
    int refcounter;
    std::vector<Recipe> children;
    __loops_RecipeStr_():refcounter(0){}
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
    pointee->is_vector = a_leaf.tag != Arg::IREG && a_leaf.tag == Arg::IIMMEDIATE;
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
bool Recipe::empty() const { return pointee == nullptr; }

inline IRecipe::IRecipe(const IReg& a_leaf)
{
    super.pointee = new __loops_RecipeStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = RECIPE_LEAF;
    super.pointee->is_vector = false;
    super.pointee->type = TYPE_I64;
    super.pointee->leaf = Arg(a_leaf);
}
inline IRecipe::IRecipe(int a_opcode, int a_type, std::initializer_list<Recipe> a_children)
{
    super.pointee = new __loops_RecipeStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = a_opcode;
    super.pointee->is_vector = false;
    super.pointee->type = a_type;
    super.pointee->children.reserve(a_children.size());
    for(const Recipe& child : a_children)
        super.pointee->children.emplace_back(child);
}

template <typename _Tp>
inline VRecipe<_Tp>::VRecipe(const VReg<_Tp>& a_leaf)
{
    super.pointee = new __loops_RecipeStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = RECIPE_LEAF;
    super.pointee->is_vector = true;
    super.pointee->type = ElemTraits<_Tp>::depth;
    super.pointee->leaf = Arg(a_leaf);
}

template <typename _Tp>
inline VRecipe<_Tp>::VRecipe(int a_opcode, std::initializer_list<Recipe> a_children)
{
    super.pointee = new __loops_RecipeStr_;
    super.pointee->refcounter = 1;
    super.pointee->opcode = a_opcode;
    super.pointee->is_vector = true;
    super.pointee->type = ElemTraits<_Tp>::depth;
    super.pointee->children.reserve(a_children.size());
    for(const Recipe& child : a_children)
        super.pointee->children.emplace_back(child);
}


class Func
{
    friend Func* _getImpl(Func* wrapper);
public:
    Func();
    Func(const Func& f);
    Func& operator=(const Func& f);
    virtual ~Func();

    std::string name() const; //TODO(ch): what for we need name here? 
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
    bool hasFunc(const std::string& name);

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

    //TODO(ch): make next methods static:
    std::string getPlatformName() const;
    size_t vbytes() const; //size of vector register in bytes
    template<typename _Tp> inline size_t vlanes() const { return vbytes() / sizeof(_Tp); }
    void compileAll();
    void debugModeOn();//Gives ability to print listings even after compilation
protected:
    Context(Context* a_impl): impl(a_impl) {}
    Context* impl;
};

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

static inline IRecipe __loops_const_(Context* CTX, int64_t _val)
{
    Recipe val(Arg(_val, CTX));
    return IRecipe(OP_MOV, TYPE_I64, {val});
}

static inline IRecipe __loops_def_(Context* CTX)
{
    Recipe dummy(Arg(0, CTX));//TODO(ch): this Arg(0) is a workaround for providing context to Recipe. 
    return IRecipe(OP_DEF, TYPE_I64, {dummy});
}

template<typename _Tp>
VRecipe<_Tp> __loops_vconst_(Context* CTX, _Tp _val)
{
    int64_t val64 = 0;
    *(reinterpret_cast<_Tp*>(&val64)) = _val;
    Recipe val(Arg(val64, CTX));
    return VRecipe<_Tp>(OP_MOV, {val});
}
template<typename _Tp>
VRecipe<_Tp> __loops_vdef_(Context* CTX)
{
    Recipe dummy(Arg(0, CTX));//TODO(ch): this Arg(0) is a workaround for providing context to Recipe. 
    return VRecipe<_Tp>(VOP_DEF, ElemTraits<_Tp>::depth, {dummy});
}

//TODO(ch): Unfortunately, execution of condtions cannot be considered as lazy. Code with effects(assignments or function calls, in future) will be 
//done independently of status of already evaluated conditions. It's result of code collection procedure traits. Probably, it should be fixed, but it's not easy.

#define USE_CONTEXT_(ctx) loops::Context __loops_ctx__(ctx);
#define STARTFUNC_(funcname, ...) if(__Loops_FuncScopeBracket_ __loops_func_{&__loops_ctx__, (funcname), {__VA_ARGS__}}) ; else
#define CONST_(x) __loops_const_(&__loops_ctx__, x)
#define DEF_(x) __loops_def_<eltyp>(&__loops_ctx__)
#define VCONST_(eltyp, x) __loops_vconst_<eltyp>(&__loops_ctx__, x)
#define VDEF_(eltyp) __loops_vdef_<eltyp>(&__loops_ctx__)
#define IF_(expr) if(__Loops_CFScopeBracket_ __loops_cf_{__Loops_CondPrefixMarker_(__loops_ctx__), __Loops_CFScopeBracket_::IF, (expr)}) ; else
#define ELIF_(expr) if(__Loops_CFScopeBracket_ __loops_cf_{__Loops_CondPrefixMarker_(__loops_ctx__), __Loops_CFScopeBracket_::ELIF, (expr)}) ; else
#define ELSE_ if(__Loops_CFScopeBracket_ __loops_cf_{__loops_ctx__}) ; else
#define WHILE_(expr) if(__Loops_CFScopeBracket_ __loops_cf_{__Loops_CondPrefixMarker_(__loops_ctx__), __Loops_CFScopeBracket_::WHILE, (expr)}) ; else
#define BREAK_ loops::__Loops_CF_rvalue_(&__loops_ctx__).break_()
#define CONTINUE_ loops::__Loops_CF_rvalue_(&__loops_ctx__).continue_()
#define RETURN_(x) loops::__Loops_CF_rvalue_(&__loops_ctx__).return_(x)

//DUBUGGG: Other interesting idea is to keep func not in Args, but only in IReg/Vreg and Recipes. Args is used mostly beyond code collection pass,
//where func is always fully-determined.

//DUBUGGG: Create template implementation file

void newiopNoret(int opcode, ::std::initializer_list<Recipe> args);
///////////////////////////// integer operations ///////////////////////
// Load with zero/sign extension:
static inline IRecipe loadx(const IRecipe& base, int depth)
{ return IRecipe(OP_LOAD, depth, {base.notype()}); }
static inline IRecipe loadx(const IRecipe& base, const IRecipe& offset, int depth)
{ return IRecipe(OP_LOAD, depth, {base.notype(), offset.notype()}); }
static inline IRecipe loadx(const IRecipe& base, int64_t offset, int depth)
{ return IRecipe(OP_LOAD, depth, {base.notype(), Recipe(offset)}); }

template<typename _Tp> static inline IRecipe load_(const IRecipe& base)
{ return loadx(base, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
IRecipe load_(const IRecipe& base, const IRecipe& offset)
{ return loadx(base, offset, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
IRecipe load_(const IRecipe& base, int64_t offset)
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
template<typename _Tp> static inline
void store_(const IRecipe& base, const IRecipe& r)
{ storex(base, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
void store_(const IRecipe& base, int64_t a)
{ storex(base, a, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
void store_(const IRecipe& base, const IRecipe& offset, const IRecipe& r)
{ storex(base, offset, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
void store_(const IRecipe& base, int64_t offset, const IRecipe& r)
{ storex(base, offset, r, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
void store_(const IRecipe& base, const IRecipe& offset, int64_t a)
{ storex(base, offset, a, ElemTraits<_Tp>::depth); }
template<typename _Tp> static inline
void store_(const IRecipe& base, int64_t offset, int64_t a)
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
static inline IRecipe ule(int64_t a, const IRecipe& b) { return uge(b,a); }
static inline IRecipe operator >= (int64_t a, const IRecipe& b) { return b <= a; }
static inline IRecipe uge(int64_t a, const IRecipe& b) { return ule(b, a);}
static inline IRecipe operator > (int64_t a, const IRecipe& b) { return b < a; }
static inline IRecipe ugt(int64_t a, const IRecipe& b) { return ult(b,a);}
static inline IRecipe operator < (int64_t a, const IRecipe& b) { return b > a; }
static inline IRecipe ult(int64_t a, const IRecipe& b) {return ugt(b,a);}
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
IRecipe pow(const IRecipe& a, int p);

///////////////////////////// vector operations ///////////////////////
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
    res1.func = res2.func = r1.func;
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
//template<typename _Tp> VRecipe<_Tp> add_wrap(const VRecipe<_Tp>& a, const VRecipe<_Tp>& b);
//template<typename _Tp> VRecipe<_Tp> sub_wrap(const VRecipe<_Tp>& a, const VRecipe<_Tp>& b);
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
//template<typename _Tp> IRecipe all(VRecipe<_Tp>& a);
//template<typename _Tp> IRecipe any(VRecipe<_Tp>& a);
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
template<typename _Tp> VRecipe<_Tp> operator + (const VReg<_Tp>& a, const VReg<_Tp>& b) { return VRecipe<_Tp>(a) + VRecipe<_Tp>(b); }
template<typename _Tp> VRecipe<_Tp> operator + (const VReg<_Tp>& a, const VRecipe<_Tp>& b) { return VRecipe<_Tp>(a) + b; }
template<typename _Tp> VRecipe<_Tp> operator + (const VRecipe<_Tp>& a, const VReg<_Tp>& b) { return a + VRecipe<_Tp>(b); }
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
template<typename _Tp> VRecipe<_Tp> fma(const VRecipe<_Tp>& a, const VRecipe<_Tp>& b, const VReg<_Tp>& c) { return fma(a, b, VRecipe<_Tp>(c)); }
template<typename _Tp> VRecipe<_Tp> fma(const VRecipe<_Tp>& a, const VReg<_Tp>& b, const VRecipe<_Tp>& c) { return fma(a, VRecipe<_Tp>(b), c); } 
template<typename _Tp> VRecipe<_Tp> fma(const VRecipe<_Tp>& a, const VReg<_Tp>& b, const VReg<_Tp>& c) { return fma(a, VRecipe<_Tp>(b), VRecipe<_Tp>(c)); } 
template<typename _Tp> VRecipe<_Tp> fma(const VReg<_Tp>& a, const VRecipe<_Tp>& b, const VRecipe<_Tp>& c) { return fma(VRecipe<_Tp>(a), b, c); } 
template<typename _Tp> VRecipe<_Tp> fma(const VReg<_Tp>& a, const VRecipe<_Tp>& b, const VReg<_Tp>& c) { return fma(VRecipe<_Tp>(a), b, VRecipe<_Tp>(c)); } 
template<typename _Tp> VRecipe<_Tp> fma(const VReg<_Tp>& a, const VReg<_Tp>& b, const VRecipe<_Tp>& c) { return fma(VRecipe<_Tp>(a), VRecipe<_Tp>(b), c); } 
template<typename _Tp> VRecipe<_Tp> fma(const VReg<_Tp>& a, const VReg<_Tp>& b, const VReg<_Tp>& c) { return fma(VRecipe<_Tp>(a), VRecipe<_Tp>(b), VRecipe<_Tp>(c)); } 
template<typename _Tp> VRecipe<_Tp> fma(const VRecipe<_Tp>& a, const VRecipe<_Tp>& b, const VRecipe<_Tp>& c, int64_t index)
{ return VRecipe<_Tp>(VOP_FMA, {a.notype(), b.notype(), c.notype(), Recipe(index)}); }
template<typename _Tp> VRecipe<_Tp> pow(const VRecipe<_Tp>& a, int p);
struct exp_consts;
exp_consts expInit(Context CTX);
VRecipe<float> exp(const VRecipe<float>& x, const exp_consts& expt);

//template<typename _Tp, typename _Sp> VRecipe<_Tp> operator >> (const VRecipe<_Tp>& a, const VRecipe<_Sp>& b)
template<typename _Tp> VRecipe<_Tp> operator >> (const VRecipe<_Tp>& a, int64_t b)
{ return VRecipe<_Tp>(VOP_SAR, {a.notype(), Recipe(b)}); }
//template<typename _Tp, typename _Sp> VRecipe<_Tp> ushift_right(const VRecipe<_Tp>& a, const VRecipe<_Sp>& b)
template<typename _Tp> VRecipe<_Tp> ushift_right (const VRecipe<_Tp>& a, int64_t b)
{ return VRecipe<_Tp>(VOP_SHR, {a.notype(), Recipe(b)}); }
template<typename _Tp, typename _Sp> VRecipe<_Tp> operator << (const VRecipe<_Tp>& a, const VRecipe<_Sp>& b)
{
    static_assert(sizeof(_Tp) == sizeof(_Sp), "the # of lanes in the 1st and 2nd argument must be the same");
    return VRecipe<_Tp>(VOP_SAL, {a.notype(), b.notype()});
}
template<typename _Tp> VRecipe<_Tp> operator << (const VRecipe<_Tp>& a, int64_t b)
{ return VRecipe<_Tp>(VOP_SAL, {a.notype(), Recipe(b)}); }
//template<typename _Tp, typename _Sp> VRecipe<_Tp> ushift_left(const VRecipe<_Tp>& a, int64_t b)
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
template<typename _Tp> VRecipe<_Tp> max(const VReg<_Tp>& a, const VReg<_Tp>& b) { return max(VRecipe<_Tp>(a), VRecipe<_Tp>(b));}
template<typename _Tp> VRecipe<_Tp> max(const VRecipe<_Tp>& a, const VReg<_Tp>& b) { return max(a, VRecipe<_Tp>(b));}
template<typename _Tp> VRecipe<_Tp> max(const VReg<_Tp>& a, const VRecipe<_Tp>& b) { return max(VRecipe<_Tp>(a), b);}
template<typename _Tp> VRecipe<_Tp> min(const VRecipe<_Tp>& a, const VRecipe<_Tp>& b)
{ return VRecipe<_Tp>(VOP_MIN, {a.notype(), b.notype()}); }
template<typename _Tp> VRecipe<_Tp> min(const VReg<_Tp>& a, const VReg<_Tp>& b) { return min(VRecipe<_Tp>(a), VRecipe<_Tp>(b));}
template<typename _Tp> VRecipe<_Tp> min(const VRecipe<_Tp>& a, const VReg<_Tp>& b) { return min(a, VRecipe<_Tp>(b));}
template<typename _Tp> VRecipe<_Tp> min(const VReg<_Tp>& a, const VRecipe<_Tp>& b) { return min(VRecipe<_Tp>(a), b);}
//template<typename _Tp> VRecipe<_Tp> abs(const VRecipe<_Tp>& a);
//template<typename _Tp> VRecipe<_Tp> sign(const VRecipe<_Tp>& a);


//Augmenting operations:
template<typename _Tp> VReg<_Tp>& operator += (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a(_a); newiopNoret(VOP_ADD, {a.notype(), a.notype(), b.notype()}); return _a; }
template<typename _Tp> VReg<_Tp>& operator -= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a(_a); newiopNoret(VOP_SUB, {a.notype(), a.notype(), b.notype()}); return _a; }
template<typename _Tp> VReg<_Tp>& operator *= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a(_a); newiopNoret(VOP_MUL, {a.notype(), a.notype(), b.notype()}); return _a; }
template<typename _Tp> VReg<_Tp>& operator /= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a(_a); newiopNoret(VOP_DIV, {a.notype(), a.notype(), b.notype()}); return _a; }
//template<typename _Tp> VReg<_Tp>& operator %= (VReg<_Tp>& _a, const VRecipe<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& _a, int64_t _b)
{ VRecipe<_Tp> a(_a);  Recipe b(_b); newiopNoret(VOP_SAR, {a.notype(), a.notype(), b}); return _a; }
template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& _a, VRecipe<_Tp> b)
{ VRecipe<_Tp> a(_a); newiopNoret(VOP_SAL, {a.notype(), a.notype(), b.notype()}); return _a; }
template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& _a, int64_t _b)
{ VRecipe<_Tp> a(_a);  Recipe b(_b); newiopNoret(VOP_SAL, {a.notype(), a.notype(), b}); return _a; }
template<typename _Tp> VReg<_Tp>& operator &= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a(_a); newiopNoret(VOP_AND, {a.notype(), a.notype(), b.notype()} ); return _a; }
template<typename _Tp> VReg<_Tp>& operator |= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a(_a); newiopNoret(VOP_OR, {a.notype(), a.notype(), b.notype()} ); return _a; }
template<typename _Tp> VReg<_Tp>& operator ^= (VReg<_Tp>& _a, const VRecipe<_Tp>& b)
{ VRecipe<_Tp> a(_a); newiopNoret(VOP_XOR, {a.notype(), a.notype(), b.notype()} ); return _a; }

//// if all/any of the elements is true

//TODO(ch): These template implementations can be obviously moved to auxilary header:

Context ExtractContext(const Recipe& arg);

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
    Recipe fromwho(r);
    return operator=(fromwho);
}

void VReg_assign_(const Arg& target, const Recipe& fromwho);

template<typename _Tp>
VReg<_Tp>& VReg<_Tp>::operator=(const VRecipe<_Tp>& fromwho)
{
    VReg_assign_(Arg(*this), fromwho.notype());
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

template<typename _Tp>
Arg::Arg(const VReg<_Tp>& vr): idx(vr.idx)
    , func(vr.func)
    , tag(VREG)
    , elemtype(ElemTraits<_Tp>::depth)
    , flags(0){}

template<typename _Tp>
VRecipe<_Tp> pow(const VRecipe<_Tp>& a, int p)
{
    Context CTX = ExtractContext(a);
    USE_CONTEXT_(CTX);
    if(p == 0)
        switch(a.type())
        {
            case (TYPE_U8 ): return VCONST_(uint8_t, 1);
            case (TYPE_I8 ): return VCONST_(int8_t, 1);
            case (TYPE_U16): return VCONST_(uint16_t, 1);
            case (TYPE_I16): return VCONST_(int16_t, 1);
            case (TYPE_U32): return VCONST_(uint32_t, 1);
            case (TYPE_I32): return VCONST_(int32_t, 1);
            case (TYPE_U64): return VCONST_(uint64_t, 1);
            case (TYPE_I64): return VCONST_(int64_t, 1);
            case (TYPE_FP16): return VCONST_(f16_t, f16_t(1.0));
            case (TYPE_BF16): throw std::runtime_error("BF16 type isn't supported yet.");
            case (TYPE_FP32): return VCONST_(float, 1.0);
            case (TYPE_FP64): return VCONST_(double, 1.0);
            default:
                throw std::runtime_error("Unknown data type.");
        }
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

struct exp_consts
{
    VReg<float> lo, hi, half, one, LOG2EF, C1, C2, p0, p1, p2, p3, p4, p5;
    VReg<int32_t> _7f;
    exp_consts(Context CTX);
};


}
#endif