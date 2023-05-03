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
    TYPE_FP16=8, TYPE_BF16=9, TYPE_FP32=10, TYPE_FP64=11 , 
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
struct IReg
{
    IReg();
    IReg(const IReg& r); //Must generate copy(mov) code
    IReg(const Recipe& fromwho);
    IReg& operator=(const IReg& r); // may generate real code if 'this' is already initialized
    IReg& operator=(const Recipe& fromwho);
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
template<typename _Tp> struct VReg
{
    typedef _Tp elemtype;

    VReg() : idx(NOIDX), func(nullptr) {}
    VReg(const VReg<_Tp>& r);
    VReg(const Recipe& fromwho);
    VReg<_Tp>& operator=(const VReg<_Tp>& r);
    VReg<_Tp>& operator=(const Recipe& fromwho);
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
struct Recipe
{
    inline Recipe();
    inline Recipe(const Recipe& fromwho);
    inline Recipe& operator=(const Recipe& fromwho);
    inline Recipe(const Arg& a_leaf);
    inline Recipe(const IReg& a_leaf);
    template<typename _Tp> 
    inline Recipe(const VReg<_Tp>& a_leaf);
    inline Recipe(int64_t a_leaf);
    inline Recipe(int a_opcode, bool a_is_vector, int a_type, std::initializer_list<Recipe> a_children);
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
Recipe::Recipe(const IReg& a_leaf): pointee(new __loops_RecipeStr_)
{
    pointee->refcounter = 1;
    pointee->opcode = RECIPE_LEAF;
    pointee->is_vector = false;
    pointee->leaf = Arg(a_leaf);
}
Recipe::Recipe(int64_t a_leaf): pointee(new __loops_RecipeStr_)
{
    pointee->refcounter = 1;
    pointee->opcode = RECIPE_LEAF;
    pointee->is_vector = false;
    pointee->leaf = Arg(a_leaf);
}
template<typename _Tp> 
Recipe::Recipe(const VReg<_Tp>& a_leaf): pointee(new __loops_RecipeStr_)
{
    pointee->refcounter = 1;
    pointee->opcode = RECIPE_LEAF;
    pointee->is_vector = true;
    pointee->type = ElemTraits<_Tp>::depth;
    pointee->leaf = Arg(a_leaf);
}
Recipe::Recipe(int a_opcode, bool a_is_vector, int a_type, std::initializer_list<Recipe> a_children): pointee(new __loops_RecipeStr_)
{
    pointee->refcounter = 1;
    pointee->opcode = a_opcode;
    pointee->is_vector = a_is_vector;
    pointee->type = a_type;
    pointee->children.reserve(a_children.size());
    for(const Recipe& child : a_children)
        pointee->children.emplace_back(child);
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
    explicit __Loops_CFScopeBracket_(const __Loops_CondPrefixMarker_& inh, CFType _cftype, const Recipe& condition);
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
    void return_(const Recipe& r);
};

static inline Recipe __loops_const_(Context* CTX, int64_t _val)
{
    Recipe val(Arg(_val, CTX));
    return Recipe(OP_MOV, false, TYPE_I64, {val});
}

static inline Recipe __loops_def_(Context* CTX)
{
    Recipe dummy(Arg(0, CTX));//TODO(ch): this Arg(0) is a workaround for providing context to Recipe. 
    return Recipe(OP_DEF, false, TYPE_I64, {dummy});
}

template<typename _Tp>
Recipe __loops_vconst_(Context* CTX, _Tp _val)
{
    int64_t val64 = 0;
    *(reinterpret_cast<_Tp*>(&val64)) = _val;
    Recipe val(Arg(val64, CTX));
    return Recipe(OP_MOV, true, ElemTraits<_Tp>::depth, {val});
}
template<typename _Tp>
Recipe __loops_vdef_(Context* CTX)
{
    Recipe dummy(Arg(0, CTX));//TODO(ch): this Arg(0) is a workaround for providing context to Recipe. 
    return Recipe(VOP_DEF, ElemTraits<_Tp>::depth, {dummy});
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

//DUBUGGG: One of further step is introducing typified Recipes: only root node will be typified in AST, other will keep type in runtime manner
//So, this is the way to keep compile-time typechecking as it was before introducing AST.

//DUBUGGG: Other interesting idea is to keep func not in Args, but only in IReg/Vreg and Recipes. Args is used mostly beyond code collection pass,
//where func is always fully-determined.

static inline void assert_scalars_(::std::initializer_list<Recipe> args)
{
    for(const Recipe& arg : args)
        if(arg.is_vector())
            throw std::runtime_error("Unexpected vector.");
}

static inline void assert_haveireg_(::std::initializer_list<Recipe> args)
{
    bool foundreg = false;
    bool allarescalars = true;
    for(const Recipe& arg : args)
        if(arg.is_vector())
        {
            allarescalars = false;
            break;
        }
        else if (!arg.is_leaf() || arg.leaf().tag == Arg::IREG)
            foundreg = true;
    if(!allarescalars)
        throw std::runtime_error("Unexpected vector.");
    if(!foundreg)
        throw std::runtime_error("Scalar register is expected.");
}

static inline void assert_unisize_vreg(::std::initializer_list<Recipe> args)
{
    if(args.size() == 0) 
        return;
    int type = (*(args.begin())).type();
    for(const Recipe& arg : args)
        if(!arg.is_vector())
            throw std::runtime_error("Unexpected scalar.");
        else if(elem_size(arg.type()) != elem_size(type))
            throw std::runtime_error("Number of lanes in the in arguments must be the same.");
}

static inline void assert_unitype_vreg(::std::initializer_list<Recipe> args)
{
    if(args.size() == 0) 
        return;
    int type = (*(args.begin())).type();
    for(const Recipe& arg : args)
        if(!arg.is_vector())
            throw std::runtime_error("Unexpected scalar.");
        else if(arg.type() != type)
            throw std::runtime_error("Type mismatch.");
}

void newiopNoret(int opcode, ::std::initializer_list<Recipe> args);
///////////////////////////// integer operations ///////////////////////
// load with zero/sign extension
template<typename _Tp> static inline Recipe load_(const Recipe& base)
{
    assert_haveireg_({base});
    return Recipe(OP_LOAD, false, ElemTraits<_Tp>::depth, {base}); 
}
template<typename _Tp> static inline Recipe load_(const Recipe& base, const Recipe& offset)
{ 
    assert_haveireg_({base});
    return Recipe(OP_LOAD, false, ElemTraits<_Tp>::depth, {base, offset});
}
static inline Recipe load(const Recipe& base)
{ return load_<int64_t>(base); }
static inline Recipe load(const Recipe& base, const Recipe& offset)
{ return load_<int64_t>(base, offset); }

// store part of register
template<typename _Tp>
static inline void store_(const Recipe& base, const Recipe& r)
{ 
    assert_haveireg_({base});
    Recipe r_(r);
    r_.type() = ElemTraits<_Tp>::depth;
    newiopNoret(OP_STORE, {base, r});
}
template<typename _Tp> static inline
void store_(const Recipe& base, const Recipe& offset, const Recipe& r)
{ 
    assert_haveireg_({base});
    Recipe r_(r);
    r_.type() = ElemTraits<_Tp>::depth;
    newiopNoret(OP_STORE, {base, offset, r});
}

static inline void store(const Recipe& base, const Recipe& r)
{ store_<uint64_t>(base, r); }
static inline void store(const Recipe& base, const Recipe& offset, const Recipe& r)
{ store_<uint64_t>(base, offset, r); }

//Integer arithmetic and bitwise operations
static inline Recipe operator + (const Recipe& a, const Recipe& b)
{ 
    if(!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_ADD, false, b.type(), {b, a});
        else
            return Recipe(OP_ADD, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_ADD, true, a.type(), {a, b});
    }
}

static inline Recipe operator - (const Recipe& a, const Recipe& b)
{
    if(!a.is_vector())
    {
        assert_haveireg_({a, b});
        return Recipe(OP_SUB, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_SUB, true, a.type(), {a, b});
    }
}
static inline Recipe operator * (const Recipe& a, const Recipe& b)
{
    if(!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_MUL, false, b.type(), {b, a});
        else
            return Recipe(OP_MUL, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_MUL, true, a.type(), {a, b});
    }
}
static inline Recipe operator / (const Recipe& a, const Recipe& b)
{ 
    if(!a.is_vector())
    {
        assert_haveireg_({a, b});
        return Recipe(OP_DIV, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_DIV, true, a.type(), {a, b});
    }
}
static inline Recipe operator % (const Recipe& a, const Recipe& b)
{ 
    assert_haveireg_({a, b});
    return Recipe(OP_MOD, false, a.type(), {a, b});
    //TODO(ch): Support VRegs?
}
static inline Recipe operator - (const Recipe& a)
{ 
    if(!a.is_vector())
    {
        assert_haveireg_({a});
        return Recipe(OP_NEG, false, a.type(), {a});
    }
    else
    {
        return Recipe(VOP_NEG, true, a.type(), {a});
    }
}

static inline Recipe operator >> (const Recipe& a, const Recipe& b)
{
    if(!a.is_vector())
    {
        assert_haveireg_({a, b});
        return Recipe(OP_SAR, false, a.type(), {a, b});
    }
    else
    {
        //TODO(ch): Support:
        //template<typename _Tp, typename _Sp> VReg<_Tp> operator >> (const VReg<_Tp>& a, const VReg<_Sp>& b)
        if(!b.is_leaf() || b.leaf().tag != Arg::IIMMEDIATE)
            throw std::runtime_error("Only immediate shifts are supported.");
        return Recipe(VOP_SAR, true, a.type(), {a, b});

    }
}
static inline Recipe ushift_right (const Recipe& a, const Recipe& b)
{ 
    if(!a.is_vector())
    {
        assert_haveireg_({a, b});
        return Recipe(OP_SHR, false, a.type(), {a, b});
    }
    else
    {
        //TODO(ch): Support:
        //template<typename _Tp, typename _Sp> VReg<_Tp> ushift_right(const VReg<_Tp>& a, const VReg<_Sp>& b)
        if(!b.is_leaf() || b.leaf().tag != Arg::IIMMEDIATE)
            throw std::runtime_error("Only immediate shifts are supported.");
        return Recipe(VOP_SHR, true, a.type(), {a, b});
    }
}

static inline Recipe operator << (const Recipe& a, const Recipe& b)
{ 
    if(!a.is_vector())
    {
        assert_haveireg_({a, b});
        return Recipe(OP_SHL, false, a.type(), {a, b});
    }
    else
    {
        if(!b.is_vector())
        {
            if(!b.is_leaf() || b.leaf().tag != Arg::IIMMEDIATE)
                throw std::runtime_error("Scalar shift must be immediate.");
        }
        else
            assert_unisize_vreg({a, b});
        return Recipe(VOP_SAL, true, a.type(), {a, b});
    }
}

static inline Recipe ushift_left(const Recipe& a, const Recipe& b)
{
    assert_unisize_vreg({a, b});
    return Recipe(VOP_SHL, true, a.type(), {a, b});
}

static inline Recipe operator & (const Recipe& a, const Recipe& b)
{ 
    if(!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_AND, false, b.type(), {b, a});
        else
            return Recipe(OP_AND, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_AND, true, a.type(), {b, a});
    }
}

static inline Recipe operator | (const Recipe& a, const Recipe& b)
{ 
    if(!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_OR, false, b.type(), {b, a});
        else
            return Recipe(OP_OR, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_OR, true, a.type(), {b, a});
    }
}
static inline Recipe operator ^ (const Recipe& a, const Recipe& b)
{ 
    if(!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_XOR, false, b.type(), {b, a});
        else
            return Recipe(OP_XOR, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_XOR, true, a.type(), {b, a});
    }
}

static inline Recipe operator ~ (const Recipe& a)
{ 
    if (!a.is_vector())
    {
        assert_haveireg_({a});
        return Recipe(OP_NOT, false, a.type(), {a});

    }
    else
        return Recipe(VOP_NOT, true, a.type(), {a});
}

static inline Recipe operator == (const Recipe& a, const Recipe& b)
{ 
    if (!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_EQ, false, b.type(), {b, a});
        else
            return Recipe(OP_EQ, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_EQ, true, mask_type(a.type()), {a, b});
    }
}
static inline Recipe operator != (const Recipe& a, const Recipe& b)
{ 
    if (!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_NE, false, b.type(), {b, a});
        else
            return Recipe(OP_NE, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_NE, true, mask_type(a.type()), {a, b});
    }
}

static inline Recipe operator <= (const Recipe& a, const Recipe& b)
{ 
    if (!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_GE, false, b.type(), {b, a});
        else
            return Recipe(OP_LE, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_LE, true, mask_type(a.type()), {a, b});
    }
}
static inline Recipe ule (const Recipe& a, const Recipe& b)
{ 
    assert_haveireg_({a, b});
    return Recipe(OP_ULE, false, a.type(), {a, b});
}

static inline Recipe operator >= (const Recipe& a, const Recipe& b)
{ 
    if (!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_LE, false, b.type(), {b, a});
        else
            return Recipe(OP_GE, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_GE, true, mask_type(a.type()), {a, b});
    }
}
static inline Recipe uge (const Recipe& a, const Recipe& b)
{ 
    assert_haveireg_({a, b});
    return Recipe(OP_ULE, false, b.type(), {b, a});//TODO(ch): Uge -> ule implementation is formed by ARM. Check for better ideas on Intel.
}
static inline Recipe operator > (const Recipe& a, const Recipe& b)
{ 
    if (!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_LT, false, b.type(), {b, a});
        else
            return Recipe(OP_GT, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_GT, true, mask_type(a.type()), {a, b});
    }
}
static inline Recipe ugt (const Recipe& a, const Recipe& b)
{ 
    assert_haveireg_({a, b});
    return Recipe(OP_UGT, false, a.type(), {a, b});
}
static inline Recipe operator < (const Recipe& a, const Recipe& b)
{ 
    if (!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_GT, false, b.type(), {b, a});
        else
            return Recipe(OP_LT, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_LT, true, mask_type(a.type()), {a, b});
    }
}
static inline Recipe ult (const Recipe& a, const Recipe& b)
{
    assert_haveireg_({a, b});
    return Recipe(OP_UGT, false, b.type(), {b, a});//TODO(ch): Ult -> ugt implementation is formed by ARM. Check for better ideas on Intel.
}

//DUBUGGG: After  Recipe typeification there will be needed Recipe<IReg> -> Recipe<Boolean>(a -> a!=0) transformation with making assertions stronger.
static inline Recipe operator && (const Recipe& a, const Recipe& b)
{
    assert_haveireg_({a});
    assert_haveireg_({b});
    return Recipe(OP_LOGICAL_AND, false, TYPE_BOOLEAN, {a, b});
}

static inline Recipe operator || (const Recipe& a, const Recipe& b)
{
    assert_haveireg_({a});
    assert_haveireg_({b});
    return Recipe(OP_LOGICAL_OR, false, TYPE_BOOLEAN, {a, b});
}

static inline Recipe operator ! (const Recipe& a)
{
    assert_haveireg_({a});
    return Recipe(OP_LOGICAL_NOT, false, TYPE_BOOLEAN, {a});
}

static inline Recipe select(const Recipe& cond, const Recipe& true_, const Recipe& false_)
{
    if(!cond.is_vector())
    {
        assert_haveireg_({cond});
        return Recipe(OP_SELECT, false, true_.type(), {cond, true_, false_});
    }
    else
    {
        assert_unitype_vreg({true_, false_});
        if(cond.type() != mask_type(true_.type()))
            throw std::runtime_error("Mask type must be unsigned integer with same element size, as vectors to choose from.");
        return Recipe(VOP_SELECT, true, true_.type(), {cond, true_, false_});
    }
}
static inline Recipe max(const Recipe& a, const Recipe& b)
{ 
    if (!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_MAX, false, b.type(), {b, a});
        else
            return Recipe(OP_MAX, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_MAX, true, a.type(), {a, b});
    }
}
static inline Recipe min(const Recipe& a, const Recipe& b)
{ 
    if (!a.is_vector())
    {
        assert_haveireg_({a, b});
        if(a.is_leaf() && a.leaf().tag == Arg::IIMMEDIATE)
            return Recipe(OP_MIN, false, b.type(), {b, a});
        else
            return Recipe(OP_MIN, false, a.type(), {a, b});
    }
    else
    {
        assert_unitype_vreg({a, b});
        return Recipe(VOP_MIN, true, a.type(), {a, b});
    }
}
static inline Recipe abs(const Recipe& a)
{ 
    //TODO(ch):Support:
    //template<typename _Tp> VReg<_Tp> abs(const VReg<_Tp>& a);
    assert_haveireg_({a});
    return Recipe(OP_ABS, false, a.type(), {a});
}
static inline Recipe sign(const Recipe& a)
{ 
    //TODO(ch):Support:
    //template<typename _Tp> VReg<_Tp> sign(const VReg<_Tp>& a);
    assert_haveireg_({a});
    return Recipe(OP_SIGN, false, a.type(), {a});
}
Recipe pow(const Recipe& a, int64_t p);

static inline IReg& operator += (IReg& _a, const Recipe& b)
{ assert_scalars_({b}); Recipe a(_a); newiopNoret(OP_ADD, {a, a, b}); return _a; }
static inline IReg& operator -= (IReg& _a, const Recipe& b)
{ assert_scalars_({b}); Recipe a(_a); newiopNoret(OP_SUB, {a, a, b}); return _a; }
static inline IReg& operator *= (IReg& _a, const Recipe& b)
{ assert_scalars_({b}); Recipe a(_a); newiopNoret(OP_MUL, {a, a, b}); return _a; }
static inline IReg& operator /= (IReg& _a, const Recipe& b)
{ assert_scalars_({b}); Recipe a(_a); newiopNoret(OP_DIV, {a, a, b}); return _a; }
static inline IReg& operator %= (IReg& _a, const Recipe& b)
{ assert_scalars_({b}); Recipe a(_a); newiopNoret(OP_MOD, {a, a, b}); return _a; }
static inline IReg& operator >>= (IReg& _a, const Recipe& b)
{ assert_scalars_({b}); Recipe a(_a); newiopNoret(OP_SAR, {a, a, b}); return _a; }
static inline IReg& operator <<= (IReg& _a, const Recipe& b)
{ assert_scalars_({b}); Recipe a(_a); newiopNoret(OP_SHL, {a, a, b}); return _a; }
static inline IReg& operator &= (IReg& _a, const Recipe& b)
{ assert_scalars_({b}); Recipe a(_a); newiopNoret(OP_AND, {a, a, b}); return _a; }
static inline IReg& operator |= (IReg& _a, const Recipe& b)
{ assert_scalars_({b}); Recipe a(_a); newiopNoret(OP_OR, {a, a, b}); return _a; }
static inline IReg& operator ^= (IReg& _a, const Recipe& b)
{ assert_scalars_({b}); Recipe a(_a); newiopNoret(OP_XOR, {a, a, b}); return _a; }

///////////////////////////// vector operations ///////////////////////

// Load and stores
template<typename _Tp> static inline Recipe loadvec(const Recipe& base)
{ assert_haveireg_({base}); return Recipe(VOP_LOAD, true, ElemTraits<_Tp>::depth, {&base}); }
template<typename _Tp> static inline Recipe loadvec(const Recipe& base, const Recipe& offset)
{ assert_haveireg_({base}); return Recipe(VOP_LOAD, true,  ElemTraits<_Tp>::depth, {base, offset}); }
//TODO(ch): find a way to delete next warning:
//WARNING! It's assumed here, that res1 and res2 are not initialized yet.
//template<typename _Tp> std::pair<Recipe, Recipe> loadvec_deinterleave2(const Recipe& base); //TODO(ch): optimal form of signature
void loadvec_deinterleave2_(Arg& res1, Arg& res2, const Recipe& base);
template<typename _Tp> void loadvec_deinterleave2(VReg<_Tp>& res1, VReg<_Tp>& res2, const Recipe& base)
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
static inline void storevec(const Recipe& base, const Recipe& r)
{ 
    assert_haveireg_({base});
    if (!r.is_vector())
        throw std::runtime_error("Vector to store is expected.");
    newiopNoret(VOP_STORE, {base, r});
}
static inline void storevec(const Recipe& base, const Recipe& offset, const Recipe& r)
{
    assert_haveireg_({base});
    if (offset.is_vector())
        throw std::runtime_error("Scalar is expected.");
    if (!r.is_vector())
        throw std::runtime_error("Vector is expected.");
    newiopNoret(VOP_STORE, {base, offset, r});
}
static inline void storelane(const Recipe& base, const Recipe& r, int64_t lane_index)
{ 
    assert_haveireg_({base});
    Recipe lanidx(lane_index);
    if (!r.is_vector())
        throw std::runtime_error("Vector to store is expected.");
    newiopNoret(VOP_ARM_ST1, {base, r, lanidx});
}

// Cast
template<typename _Tp> Recipe broadcast(const Recipe& scalar)
{ return Recipe(VOP_BROADCAST, true, ElemTraits<_Tp>::depth, {scalar}); }

static inline Recipe broadcast(const Recipe& r, int64_t lane_index)
{ 
    Recipe lanidx(lane_index);
    if (!r.is_vector()) throw std::runtime_error("Vector is expected.");
    return Recipe(VOP_BROADCAST, true, r.type(), {r, lanidx});
}

template<typename _Tp> Recipe cast(const Recipe& a)
{ 
    if (!a.is_vector()) throw std::runtime_error("Vector is expected.");
    return Recipe(VOP_CAST, true, ElemTraits<_Tp>::depth, {a});
}

static inline Recipe cast_low(const Recipe& r)
{ 
    if (!r.is_vector()) throw std::runtime_error("Vector is expected.");
    return Recipe(VOP_CAST_LOW, true, duplicate_type(r.type()), {r});
}

static inline Recipe cast_high(const Recipe& r)
{ 
    if (!r.is_vector()) throw std::runtime_error("Vector is expected.");
    return Recipe(VOP_CAST_HIGH, true, duplicate_type(r.type()), {r});
}

static inline Recipe shrink(const Recipe& r0, const Recipe& r1)
{ 
    assert_unitype_vreg({r0, r1});
    if (!r0.is_vector() || !r1.is_vector()) throw std::runtime_error("Vector is expected.");
    return Recipe(VOP_SHRINK, true, half_type(r0.type()), {r0,r1});
}

template<typename _Tp> Recipe reinterpret(const Recipe& a)
{ 
    if (!a.is_vector()) throw std::runtime_error("Vector is expected.");
    return Recipe(VOP_REINTERPRET, true, ElemTraits<_Tp>::depth, {a});
}

static inline Recipe getlane(const Recipe& r, int64_t lane_index)
{ 
    Recipe lanidx(lane_index);
    if (!r.is_vector()) throw std::runtime_error("Vector is expected.");
    return Recipe(VOP_GETLANE, false, r.type(), {r, lanidx});
}
static inline void setlane(const Recipe& v, int64_t lane_index, const Recipe& i)
{ 
    assert_haveireg_({i});
    Recipe lanidx(lane_index);
    if (!v.is_vector())
        throw std::runtime_error("Vector is expected.");
    newiopNoret(VOP_SETLANE, {v, lanidx, i});
}

static inline void setlane(const Recipe& v, int64_t olane_index, const Recipe& r, int64_t ilane_index)
{ 
    if (!v.is_vector() || !r.is_vector())
        throw std::runtime_error("Vector is expected.");
    Recipe olanidx(ilane_index);
    Recipe ilanidx(olane_index);
    newiopNoret(VOP_SETLANE, {v, olanidx, r, ilanidx});
}

static inline Recipe reduce_max(const Recipe& r)
{ 
    if (!r.is_vector()) throw std::runtime_error("Vector is expected.");
    return Recipe(VOP_REDUCE_MAX, true, r.type(), {r});
}

static inline Recipe reduce_min(const Recipe& r)
{ 
    if (!r.is_vector()) throw std::runtime_error("Vector is expected.");
    return Recipe(VOP_REDUCE_MIN, true, r.type(), {r});
}

static inline Recipe ext(const Recipe& n, const Recipe& m, int64_t lane_index)
{
    assert_unitype_vreg({n,m});
    Recipe lanidx(lane_index);
    return Recipe(VOP_ARM_EXT, true, n.type(), {n, m, lanidx});
}

static inline Recipe fma(const Recipe& a, const Recipe& b, const Recipe& c)
{
    assert_unitype_vreg({a,b,c});
    return Recipe(VOP_FMA, true, a.type(), {a, b, c});
}
static inline Recipe fma(const Recipe& a, const Recipe& b, const Recipe& c, int64_t lane_index)
{
    assert_unitype_vreg({a, b, c});
    Recipe lanidx(lane_index);
    return Recipe(VOP_FMA, true, a.type(), {a, b, c, lanidx});
}

//template<typename _Tp> VReg<_Tp> add_wrap(const VReg<_Tp>& a, const VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp> sub_wrap(const VReg<_Tp>& a, const VReg<_Tp>& b);

struct exp_consts;
exp_consts expInit(Context CTX);
Recipe exp(const VReg<float>& x, const exp_consts& expt);

template<typename _Tp> VReg<_Tp>& operator += (VReg<_Tp>& _a, const Recipe& b)
{ Recipe a(_a); assert_unitype_vreg({a, b}); newiopNoret(VOP_ADD, {a, a, b}); return _a; }
template<typename _Tp> VReg<_Tp>& operator -= (VReg<_Tp>& _a, const Recipe& b)
{ Recipe a(_a); assert_unitype_vreg({a, b}); newiopNoret(VOP_SUB, {a, a, b}); return _a; }
template<typename _Tp> VReg<_Tp>& operator *= (VReg<_Tp>& _a, const Recipe& b)
{ Recipe a(_a); assert_unitype_vreg({a, b}); newiopNoret(VOP_MUL, {a, a, b}); return _a; }
template<typename _Tp> VReg<_Tp>& operator /= (VReg<_Tp>& _a, const Recipe& b)
{ Recipe a(_a); assert_unitype_vreg({a, b}); newiopNoret(VOP_DIV, {a, a, b}); return _a; }
//template<typename _Tp> VReg<_Tp>& operator %= (VReg<_Tp>& _a, const Recipe& b);
//template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& _a, const Recipe& b)
//{ Recipe a(_a); assert_unitype_vreg({a, b}); newiopNoret(VOP_SAR, {&a, &a, b}); return _a;}
template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& _a, int64_t _b)
{ Recipe a(_a);  Recipe b(_b); newiopNoret(VOP_SAR, {a, a, b}); return _a; }

template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& _a, const Recipe& b)
{
    Recipe a(_a);
    if(!b.is_vector()) 
    {
        if(!b.is_leaf() || b.leaf().tag != Arg::IIMMEDIATE)
            throw std::runtime_error("Only immediate shifts are supported.");
    }
    else
        assert_unitype_vreg({a, b});
    newiopNoret(VOP_SAL, {a, a, b});
    return _a;
}
template<typename _Tp> VReg<_Tp>& operator &= (VReg<_Tp>& _a, const Recipe& b)
{ Recipe a(_a); assert_unitype_vreg({a, b}); newiopNoret(VOP_AND, {a, a, b} ); return _a; }
template<typename _Tp> VReg<_Tp>& operator |= (VReg<_Tp>& _a, const Recipe& b)
{ Recipe a(_a); assert_unitype_vreg({a, b}); newiopNoret(VOP_OR, {a, a, b} ); return _a; }
template<typename _Tp> VReg<_Tp>& operator ^= (VReg<_Tp>& _a, const Recipe& b)
{ Recipe a(_a); assert_unitype_vreg({a, b}); newiopNoret(VOP_XOR, {a, a, b} ); return _a; }

//// if all/any of the elements is true
//template<typename _Tp> IReg all(VReg<_Tp>& a);
//template<typename _Tp> IReg any(VReg<_Tp>& a);

//TODO(ch): cvtTp -> ceil, cvtTe -> round, also cast(double <=> float, float <=> f16_t)
template<typename _Tp> Recipe trunc(const Recipe& a)  //Convert with rounding to zero
{
    if (a.type() == TYPE_FP16 || a.type() == TYPE_FP32 || a.type() == TYPE_FP64)
        throw std::runtime_error("Only real number can be truncated.");
    if (ElemTraits<_Tp>::depth == TYPE_I16 || ElemTraits<_Tp>::depth == TYPE_U16 || ElemTraits<_Tp>::depth == TYPE_I32 || ElemTraits<_Tp>::depth == TYPE_U32 ||
        ElemTraits<_Tp>::depth == TYPE_I64 || ElemTraits<_Tp>::depth == TYPE_U64) 
        throw std::runtime_error("Trunc can be done only to integer type.");
    if (ElemTraits<_Tp>::elemsize != elem_size(a.type())) 
        throw std::runtime_error("Attempt to convert real number to integer of different size.");
    return Recipe(VOP_TRUNC, ElemTraits<_Tp>::depth, {&a});
}

template<typename _Tp> Recipe floor(const Recipe& a)  //Convert with rounding to zero
{
    if (a.type() != TYPE_FP16 && a.type() != TYPE_FP32 && a.type() != TYPE_FP64)
        throw std::runtime_error("Only real number can be floored.");
    if (ElemTraits<_Tp>::depth != TYPE_I16 && ElemTraits<_Tp>::depth != TYPE_U16 && ElemTraits<_Tp>::depth != TYPE_I32 && ElemTraits<_Tp>::depth != TYPE_U32 &&
        ElemTraits<_Tp>::depth != TYPE_I64 && ElemTraits<_Tp>::depth != TYPE_U64)
        throw std::runtime_error("Floor can be done only to integer type.");
    if (ElemTraits<_Tp>::elemsize != elem_size(a.type())) 
        throw std::runtime_error("Attempt to convert real number to integer of different size.");
    return Recipe(VOP_FLOOR, true, ElemTraits<_Tp>::depth, {a});
}

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
VReg<_Tp>::VReg(const Recipe& fromwho)
{
    VReg_constr_(fromwho, idx, func, ElemTraits<_Tp>::depth);
}

template<typename _Tp>
VReg<_Tp>& VReg<_Tp>::operator=(const VReg<_Tp>& r)
{
    Recipe fromwho(r);
    return operator=(fromwho);
}

void VReg_assign_(const Arg& target, const Recipe& fromwho);

template<typename _Tp>
VReg<_Tp>& VReg<_Tp>::operator=(const Recipe& fromwho)
{
    VReg_assign_(Arg(*this), fromwho);
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

struct exp_consts
{
    VReg<float> lo, hi, half, one, LOG2EF, C1, C2, p0, p1, p2, p3, p4, p5;
    VReg<int32_t> _7f;
    exp_consts(Context CTX);
};

}
#endif
