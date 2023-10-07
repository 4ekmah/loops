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
    OP_LOAD             =   0,
    OP_STORE            =   1,

    OP_MOV              =   2,
    OP_XCHG             =   3,

    OP_ADD              =   4,
    OP_SUB              =   5,
    OP_MUL              =   6,
    OP_DIV              =   7,
    OP_MOD              =   8,
    OP_SHL              =   9,
    OP_SHR              =  10,
    OP_SAR              =  11,
    OP_AND              =  12, //Note: It's bitwise operations.
    OP_OR               =  13,
    OP_XOR              =  14,
    OP_NOT              =  15,
    OP_NEG              =  16,
    OP_CMP              =  17,
    OP_SELECT           =  18, //SELECT <dest>, <condition>, <value-for-true>, <value-for-false>
    OP_IVERSON          =  19, //IVERSON <dest>, <condition> - set dest reg to 1 if cond is true and to 0 - otherwise
    OP_MIN              =  20,
    OP_MAX              =  21,
    OP_ABS              =  22,
    OP_SIGN             =  23,
   
    OP_ROUND            =  24,
    OP_FLOOR            =  25,
    OP_CEIL             =  26,

    //For service usage only
    OP_SPILL            =  27, //OP_SPILL <stack_pos>, <reg> - stack_pos is positive distance from SP, measured in 8byte-long units
    OP_UNSPILL          =  28, //OP_UNSPILL <reg>, <stack_pos>
    OP_GT               =  29,
    OP_UGT              =  30,
    OP_GE               =  31,
    OP_LT               =  32,
    OP_LE               =  33,
    OP_ULE              =  34,
    OP_NE               =  35,
    OP_EQ               =  36,
    OP_S                =  37,
    OP_NS               =  38,
    
    OP_LOGICAL_AND      =  39,
    OP_LOGICAL_OR       =  40,
    OP_LOGICAL_NOT      =  41,
    
    OP_JMP              =  42, //OP_JMP <target_label>             //TODO(ch): keep there more annotations
    OP_JCC              =  43, //OP_JCC <cmpcode>, <target_label>
    OP_RET              =  44,
    OP_CALL             =  45, //OP_CALL       <function_addr>, <retreg>, <arg0>, ..., <arg7> (args are optional)
    OP_CALL_NORET       =  46, //OP_CALL_NORET <function_addr>, <arg0>, ..., <arg8> (args are optional)
    OP_LABEL            =  47,
    
    OP_STEM_CSTART      =  48,
    
    OP_IF_CSTART        =  49,
    OP_ELIF_CSTART      =  50, //OP_ELIF_CSTART <elselabel>, <outlabel>
    OP_IF_CEND          =  51, //OP_IF_CEND
    OP_ELSE             =  52, //OP_ELSE <elselabel>, <outlabel>
    OP_ENDIF            =  53, //OP_ENDIF <outlabel>

    OP_WHILE_CSTART     =  54, //OP_WHILE_CSTART <continuelabel>
    OP_WHILE_CEND       =  55, //OP_WHILE_CEND
    OP_ENDWHILE         =  56, //OP_ENDWHILE <continuelabel>, <breaklabel>
    OP_BREAK            =  57,
    OP_CONTINUE         =  58,
    VOP_LOAD            =  59,
    VOP_STORE           =  60,
    VOP_ADD             =  61,
    VOP_SUB             =  62,
    VOP_MUL             =  63,
    VOP_DIV             =  64,
//    VOP_MOD,
    VOP_FMA             =  65,
    VOP_SAL             =  66,
    VOP_SHL             =  67,
    VOP_SAR             =  68,
    VOP_SHR             =  69,
    VOP_AND             =  70,
    VOP_OR              =  71,
    VOP_XOR             =  72,
    VOP_NOT             =  73,
    VOP_NEG             =  74,

    VOP_MIN             =  75,
    VOP_MAX             =  76,

    VOP_GT              =  77,
    VOP_GE              =  78,
    VOP_LT              =  79,
    VOP_LE              =  80,
    VOP_NE              =  81,
    VOP_EQ              =  82,

    VOP_SELECT          =  83,
    
    VOP_ALL             =  84,
    VOP_ANY             =  85,
    VOP_TRUNC           =  86,
    VOP_FLOOR           =  87,
    VOP_CAST            =  88,
    VOP_REINTERPRET     =  89,
    VOP_BROADCAST       =  90,
    VOP_CAST_LOW        =  91,
    VOP_CAST_HIGH       =  92,
    VOP_SHRINK          =  93, // VOP_SHRINK <target_with_halfsize_elements>, <source_packed_low_half>, <source_packed_high_half>
    VOP_POPCOUNT        =  94,
    VOP_REDUCE_MAX      =  95,
    VOP_REDUCE_MIN      =  96,
    VOP_REDUCE_SUM      =  97,
    VOP_REDUCE_WSUM     =  98,

//Intel-only operations:
    OP_X86_ADC          =  99, //Add with carry flag.
    OP_X86_CQO          = 100,
//Aarch64-only operations:
    OP_ARM_CINC         = 101, //TODO(ch) : check if there exists analogues on Intel and try to move it to common block.
    OP_ARM_CNEG         = 102,
    OP_ARM_MOVK         = 103, //Move bytes to shifted byte position of register and keep other bits unchanged.
    OP_ARM_LDP          = 104,
    OP_ARM_STP          = 105,
    VOP_ARM_LD1         = 106,
    VOP_ARM_ST1         = 107,
    VOP_ARM_LD2         = 108,
    VOP_ARM_EXT         = 109,
    VOP_ARM_SHRINK_LOW  = 110, //Note: don't use these two directly, use VOP_SHRINK instead.
    VOP_ARM_SHRINK_HIGH = 111,
    VOP_GETLANE         = 112,
    VOP_SETLANE         = 113,

    OP_DEF              = 114,
    VOP_DEF             = 115,

    OP_NOINIT           = 116,
};

enum
{
    EXPR_LEAF = OP_NOINIT + 1
};

template<typename _Tp> struct ElemTraits {};
class Func;
class Expr;
class IExpr;

struct IReg
{
    IReg();
    IReg(const IReg& r); //Must generate copy(mov) code
    IReg(const IExpr& fromwho);
    IReg& operator=(const IReg& r); // may generate real code if 'this' is already initialized
    IReg& operator=(const IExpr& fromwho);
    // copyidx is a way to work with IReg/VReg like with regular objects, like it needed for sophisticated generation logic.
    // Unlike usual copy, this function doesn't have any effects, it doesn't change current buffer of function.
    // Two most obvious usages are late initializtion for registers are elements of dynamic arrays and register
    // aliases(sometimes it's convinient to use different variables in one expression accordingly to current
    // generation situation.)
    void copyidx(const IReg& from);
    // This one checks if IExpr contains only register leaf and copies index. Otherwise it creates new register
    void copyidx(const IExpr& from);

    int idx;
    Func* func;
    enum {NOIDX = -1};
};

class Context;
template <typename _Tp> class VExpr;
template<typename _Tp> struct VReg
{
    typedef _Tp elemtype;

    VReg() : idx(NOIDX), func(nullptr) {}
    VReg(const VReg<_Tp>& r);
    VReg(const VExpr<_Tp>& fromwho);
    VReg<_Tp>& operator=(const VReg<_Tp>& r);
    VReg<_Tp>& operator=(const VExpr<_Tp>& fromwho);
     /*
    copyidx is a way to work with IReg/VReg like with regular objects, like it needed for sophisticated generation logic.
    Unlike usual copy, this function doesn't have any effects, it doesn't change current buffer of function.
    Two most obvious usages are late initializtion for registers are elements of dynamic arrays and register 
    aliases(sometimes it's convinient to use different variables in one expression accordingly to current
    generation situation.) 
    */   
    void copyidx(const VReg<_Tp>& from);
    void copyidx(const VExpr<_Tp>& from);

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
    inline Arg();
    inline Arg(const IReg& r);
    Arg(int64_t a_value);
    template<typename _Tp>
    Arg(const VReg<_Tp>& vr);
    int idx;
    size_t tag;
    int64_t value;
    uint64_t flags;
    int elemtype;
};

struct __loops_ExprStr_;
class Expr
{
public:
    inline Expr();
    inline Expr(const Expr& fromwho);
    inline Expr& operator=(const Expr& fromwho);
    inline Expr(const Arg& a_leaf);
    inline Expr(int64_t a_leaf);
    inline ~Expr();
    inline int& opcode();
    inline bool& is_vector();
    inline bool is_leaf() const;
    inline int& type();
    inline Arg& leaf();
    inline std::vector<Expr>& children();
    inline Func*& func();
    inline int opcode() const;
    inline bool is_vector() const ;
    inline int type() const;
    inline const Arg& leaf() const;
    inline const std::vector<Expr>& children() const;
    inline Func* func() const;
    inline bool empty() const;
    Func* infer_owner(Func* preinfered = nullptr); 
    //+add assertions on class construction.
    __loops_ExprStr_* pointee;
};

class IExpr
{
public:
    Expr super;
    IExpr() {}
    // inline IExpr(const IExpr& fromwho) : Expr(fromwho) {}
    inline IExpr(const IReg& a_leaf);
    inline IExpr(int a_opcode, int a_type, std::initializer_list<Expr> a_children);
    inline IExpr(int a_opcode, int a_type, std::vector<Expr> a_children);
    Expr notype() const { return super; }

    inline int& opcode() {return super.opcode();} 
    inline bool& is_vector() {return super.is_vector();} 
    inline bool is_leaf() const {return super.is_leaf();} 
    inline int& type() {return super.type();} 
    inline Arg& leaf() {return super.leaf();} 
    inline std::vector<Expr>& children() {return super.children();} 
    inline int opcode() const {return super.opcode();} 
    inline bool is_vector() const {return super.is_vector();} 
    inline int type() const {return super.type();} 
    inline const Arg& leaf() const {return super.leaf();} 
    inline const std::vector<Expr>& children() const {return super.children();} 
    inline bool empty() const {return super.empty();} 

};

template <typename _Tp>
class VExpr
{
public:
    Expr super;

    VExpr() {}
    inline VExpr<_Tp>(const VReg<_Tp>& a_leaf);
    inline VExpr(int a_opcode, std::initializer_list<Expr> a_children);
    inline VExpr(int a_opcode, std::vector<Expr> a_children);
    Expr notype() const {return super;}

    inline int& opcode() {return super.opcode();} 
    inline bool& is_vector() {return super.is_vector();} 
    inline bool is_leaf() const {return super.is_leaf();} 
    inline int& type() {return super.type();} 
    inline Arg& leaf() {return super.leaf();} 
    inline std::vector<Expr>& children() {return super.children();} 
    inline int opcode() const {return super.opcode();} 
    inline bool is_vector() const {return super.is_vector();} 
    inline int type() const {return super.type();} 
    inline const Arg& leaf() const {return super.leaf();} 
    inline const std::vector<Expr>& children() const {return super.children();} 
    inline bool empty() const {return super.empty();}     
};

Context ExtractContext(const Expr& arg);

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

//Appropriate function types to be called
typedef void (*funcptr0_noret_t)();
typedef void (*funcptr1_noret_t)(int64_t);
typedef void (*funcptr2_noret_t)(int64_t, int64_t);
typedef void (*funcptr3_noret_t)(int64_t, int64_t, int64_t);
typedef void (*funcptr4_noret_t)(int64_t, int64_t, int64_t, int64_t);
typedef void (*funcptr5_noret_t)(int64_t, int64_t, int64_t, int64_t, int64_t);
typedef void (*funcptr6_noret_t)(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);
typedef void (*funcptr7_noret_t)(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);
typedef void (*funcptr8_noret_t)(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);

typedef int64_t (*funcptr0_t)();
typedef int64_t (*funcptr1_t)(int64_t);
typedef int64_t (*funcptr2_t)(int64_t, int64_t);
typedef int64_t (*funcptr3_t)(int64_t, int64_t, int64_t);
typedef int64_t (*funcptr4_t)(int64_t, int64_t, int64_t, int64_t);
typedef int64_t (*funcptr5_t)(int64_t, int64_t, int64_t, int64_t, int64_t);
typedef int64_t (*funcptr6_t)(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);
typedef int64_t (*funcptr7_t)(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);
typedef int64_t (*funcptr8_t)(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t);

//TODO(ch): Unfortunately, execution of condtions cannot be considered as lazy. Code with effects(assignments or function calls, in future) will be
//done independently of status of already evaluated conditions. It's result of code collection procedure traits. Probably, it should be fixed, but it's not easy.
#define USE_CONTEXT_(ctx) loops::Context __loops_ctx__(ctx);
#define STARTFUNC_(funcname, ...) if(loops::__Loops_FuncScopeBracket_ __loops_func_{&__loops_ctx__, (funcname), {__VA_ARGS__}}) ; else
#define CONST_(x) loops::__loops_const_(&__loops_ctx__, x)
#define DEF_(x) loops::__loops_def_<eltyp>(&__loops_ctx__)
#define VCONST_(eltyp, x) loops::__loops_vconst_<eltyp>(&__loops_ctx__, eltyp(x))
#define VDEF_(eltyp) loops::__loops_vdef_<eltyp>(&__loops_ctx__)
#define IF_(expr) if(loops::__Loops_CFScopeBracket_ __loops_cf_{loops::__Loops_CondPrefixMarker_(__loops_ctx__), loops::__Loops_CFScopeBracket_::IF, (expr)}) ; else
#define ELIF_(expr) if(loops::__Loops_CFScopeBracket_ __loops_cf_{loops::__Loops_CondPrefixMarker_(__loops_ctx__), loops::__Loops_CFScopeBracket_::ELIF, (expr)}) ; else
#define ELSE_ if(loops::__Loops_CFScopeBracket_ __loops_cf_{__loops_ctx__}) ; else
#define WHILE_(expr) if(loops::__Loops_CFScopeBracket_ __loops_cf_{loops::__Loops_CondPrefixMarker_(__loops_ctx__), loops::__Loops_CFScopeBracket_::WHILE, (expr)}) ; else
#define BREAK_ loops::__Loops_CF_rvalue_(&__loops_ctx__).break_()
#define CONTINUE_ loops::__Loops_CF_rvalue_(&__loops_ctx__).continue_()
#define RETURN_(...) loops::__Loops_CF_rvalue_(&__loops_ctx__).return_(__VA_ARGS__)

//Call signature:
//
//IReg CALL_(pfunc0 fptr);
//IReg CALL_(pfunc1 fptr, const IExpr& ar0);
//...
//IReg CALL_(pfunc8 fptr, const IExpr& arg0, ..., const IExpr& arg7);
//
//or
//
//CALL_(pfunc0_ret0 fptr);
//CALL_(pfunc1_ret0 fptr, const IExpr& ar0);
//...
//CALL_(pfunc8_ret0 fptr, const IExpr& arg0, ..., const IExpr& arg7);
//
//Note: first form returns IReg and can be used in expression. Calculation of expressions with functions can behave differently to
//C++ behaviour, e.g in such condition:
//if(a < b || func(c) < d)
//func will not be called in C++ if first subcondition is correct.
//Similar code generated by Loops WILL call function anyway.
//General rule: if function is written in line, it will be called since execution have entered the line.
#define CALL_(...) loops::__Loops_CF_rvalue_(&__loops_ctx__).call_(__VA_ARGS__)


///////////////////////////// integer operations ///////////////////////
// Load with zero/sign extension:
static inline IExpr loadx(const IExpr& base, int depth);
static inline IExpr loadx(const IExpr& base, const IExpr& offset, int depth);
static inline IExpr loadx(const IExpr& base, int64_t offset, int depth);

template<typename _Tp> IExpr load_(const IExpr& base);
template<typename _Tp> IExpr load_(const IExpr& base, const IExpr& offset);
template<typename _Tp> IExpr load_(const IExpr& base, int64_t offset);

// store part of register
static inline void storex(const IExpr& base, const IExpr& r, int depth);
static inline void storex(const IExpr& base, int64_t a, int depth);
static inline void storex(const IExpr& base, const IExpr& offset, const IExpr& r, int depth);
static inline void storex(const IExpr& base, int64_t offset, const IExpr& r, int depth);
static inline void storex(const IExpr& base, const IExpr& offset, int64_t a, int depth);
static inline void storex(const IExpr& base, int64_t offset, int64_t a, int depth);
static inline void store(const IExpr& base, const IExpr& r);
static inline void store(const IExpr& base, const IExpr& offset, const IExpr& r);
template<typename _Tp> void store_(const IExpr& base, const IExpr& r);
template<typename _Tp> void store_(const IExpr& base, int64_t a);
template<typename _Tp> void store_(const IExpr& base, const IExpr& offset, const IExpr& r);
template<typename _Tp> void store_(const IExpr& base, int64_t offset, const IExpr& r);
template<typename _Tp> void store_(const IExpr& base, const IExpr& offset, int64_t a);
template<typename _Tp> void store_(const IExpr& base, int64_t offset, int64_t a);


// Integer arithmetic and bitwise operations:
static inline IExpr operator + (const IExpr& a, const IExpr& b);
static inline IExpr operator + (const IExpr& a, int64_t b);
static inline IExpr operator + (int64_t a, const IExpr& b);
static inline IExpr operator - (const IExpr& a, const IExpr& b);
static inline IExpr operator - (const IExpr& a, int64_t b);
static inline IExpr operator - (int64_t a, const IExpr& b);
static inline IExpr operator * (const IExpr& a, const IExpr& b);
static inline IExpr operator * (const IExpr& a, int64_t b);
static inline IExpr operator * (int64_t a, const IExpr& b);
static inline IExpr operator / (const IExpr& a, const IExpr& b);
static inline IExpr operator / (const IExpr& a, int64_t b);
static inline IExpr operator / (int64_t a, const IExpr& b);
static inline IExpr operator % (const IExpr& a, const IExpr& b);
static inline IExpr operator % (const IExpr& a, int64_t b);
static inline IExpr operator % (int64_t a, const IExpr& b);
static inline IExpr operator - (const IExpr& a);
static inline IExpr operator >> (const IExpr& a, const IExpr& b);
static inline IExpr operator >> (const IExpr& a, int64_t b);
static inline IExpr operator >> (int64_t a, const IExpr& b);
static inline IExpr ushift_right(const IExpr& a, const IExpr& b);
static inline IExpr ushift_right(const IExpr& a, int64_t b);
static inline IExpr ushift_right(int64_t a, const IExpr& b);
static inline IExpr operator <<(const IExpr& a, const IExpr& b);
static inline IExpr operator <<(const IExpr& a, int64_t b);
static inline IExpr operator <<(int64_t a, const IExpr& b);
static inline IExpr operator & (const IExpr& a, const IExpr& b);
static inline IExpr operator & (const IExpr& a, int64_t b);
static inline IExpr operator & (int64_t a, const IExpr& b);
static inline IExpr operator | (const IExpr& a, const IExpr& b);
static inline IExpr operator | (const IExpr& a, int64_t b);
static inline IExpr operator | (int64_t a, const IExpr& b);
static inline IExpr operator ^ (const IExpr& a, const IExpr& b);
static inline IExpr operator ^ (const IExpr& a, int64_t b);
static inline IExpr operator ^ (int64_t a, const IExpr& b);
static inline IExpr operator ~ (const IExpr& a);

// Comparisson and logical operations:
static inline IExpr operator == (const IExpr& a, const IExpr& b);
static inline IExpr operator == (const IExpr& a, int64_t b);
static inline IExpr operator == (int64_t a, const IExpr& b);
static inline IExpr operator != (const IExpr& a, const IExpr& b);
static inline IExpr operator != (const IExpr& a, int64_t b);
static inline IExpr operator != (int64_t a, const IExpr& b);
static inline IExpr operator <= (const IExpr& a, const IExpr& b);
static inline IExpr operator <= (const IExpr& a, int64_t b);
static inline IExpr operator <= (int64_t a, const IExpr& b);
static inline IExpr ule(const IExpr& a, const IExpr& b);
static inline IExpr ule(const IExpr& a, int64_t b);
static inline IExpr ule(int64_t a, const IExpr& b);
static inline IExpr operator >= (const IExpr& a, const IExpr& b);
static inline IExpr operator >= (const IExpr& a, int64_t b);
static inline IExpr operator >= (int64_t a, const IExpr& b);
static inline IExpr uge(const IExpr& a, const IExpr& b);
static inline IExpr uge(const IExpr& a, int64_t b);
static inline IExpr uge(int64_t a, const IExpr& b);
static inline IExpr operator > (const IExpr& a, const IExpr& b);
static inline IExpr operator > (const IExpr& a, int64_t b);
static inline IExpr operator >  (int64_t a, const IExpr& b);
static inline IExpr ugt(const IExpr& a, const IExpr& b);
static inline IExpr ugt(const IExpr& a, int64_t b);
static inline IExpr ugt(int64_t a, const IExpr& b);
static inline IExpr operator < (const IExpr& a, const IExpr& b);
static inline IExpr operator < (const IExpr& a, int64_t b);
static inline IExpr operator <  (int64_t a, const IExpr& b);
static inline IExpr ult(const IExpr& a, const IExpr& b);
static inline IExpr ult(const IExpr& a, int64_t b);
static inline IExpr ult(int64_t a, const IExpr& b);
static inline IExpr operator && (const IExpr& a, const IExpr& b);
static inline IExpr operator || (const IExpr& a, const IExpr& b);
static inline IExpr operator ! (const IExpr& a);

//Augmenting operations:
static inline IReg& operator += (IReg& a, const IExpr& b);
static inline IReg& operator += (IReg& a, int64_t b);
static inline IReg& operator -= (IReg& a, const IExpr& b);
static inline IReg& operator -= (IReg& a, int64_t b);
static inline IReg& operator *= (IReg& a, const IExpr& b);
static inline IReg& operator *= (IReg& a, int64_t b);
static inline IReg& operator /= (IReg& a, const IExpr& b);
static inline IReg& operator /= (IReg& a, int64_t b);
static inline IReg& operator %= (IReg& a, const IExpr& b);
static inline IReg& operator %= (IReg& a, int64_t b);
static inline IReg& operator >>= (IReg& a, const IExpr& b);
static inline IReg& operator >>= (IReg& a, int64_t b);
static inline IReg& operator <<= (IReg& a, const IExpr& b);
static inline IReg& operator <<= (IReg& a, int64_t b);
static inline IReg& operator &= (IReg& a, const IExpr& b);
static inline IReg& operator &= (IReg& a, int64_t b);
static inline IReg& operator |= (IReg& a, const IExpr& b);
static inline IReg& operator |= (IReg& a, int64_t b);
static inline IReg& operator ^= (IReg& a, const IExpr& b);
static inline IReg& operator ^= (IReg& a, int64_t b);

//Other integer operations:
static inline IExpr select(const IExpr& cond, const IExpr& true_, const IExpr& false_);
static inline IExpr select(const IExpr& cond, int64_t true_, const IExpr& false_);
static inline IExpr select(const IExpr& cond, const IExpr& true_, int64_t false_);
static inline IExpr select(const IExpr& cond, int64_t true_, int64_t false_);
static inline IExpr max(const IExpr& a, const IExpr& b);
static inline IExpr max(const IExpr& a, int64_t b);
static inline IExpr max(int64_t a, const IExpr& b);
static inline IExpr min(const IExpr& a, const IExpr& b);
static inline IExpr min(const IExpr& a, int64_t b);
static inline IExpr min(int64_t a, const IExpr& b);
static inline IExpr abs(const IExpr& a);
static inline IExpr sign(const IExpr& a);
IExpr pow(const IExpr& a, int p);

///////////////////////////// vector operations ///////////////////////
// Load with zero/sign extension:
template<typename _Tp> VExpr<_Tp> loadvec(const IExpr& base);
template<typename _Tp> VExpr<_Tp> loadvec(const IExpr& base, const IExpr& offset);
template<typename _Tp> VExpr<_Tp> loadvec(const IExpr& base, int64_t offset);
template<typename _Tp> VExpr<_Tp> loadlane(const IExpr& base, int64_t lane_index);
//TODO(ch): find a way to delete next warning:
//WARNING! It's assumed here, that res1 and res2 are not initialized yet.
//template<typename _Tp> std::pair<Expr, Expr> loadvec_deinterleave2(const Expr& base); //TODO(ch): optimal form of signature
template<typename _Tp> void loadvec_deinterleave2(VReg<_Tp>& res1, VReg<_Tp>& res2, const IExpr& base);

// Store:
template<typename _Tp> void storevec(const IExpr& base, const VExpr<_Tp>& r);
template<typename _Tp> void storevec(const IExpr& base, const    VReg<_Tp>& r);
template<typename _Tp> void storevec(const IExpr& base, const IExpr& offset, const VExpr<_Tp>& r);
template<typename _Tp> void storevec(const IExpr& base, const IExpr& offset, const    VReg<_Tp>& r);
template<typename _Tp> void storelane(const IExpr& base, const VExpr<_Tp>& r, int64_t lane_index);
template<typename _Tp> void storelane(const IExpr& base, const    VReg<_Tp>& r, int64_t lane_index);

// Casts:
//template<typename _Tp> VExpr<_Tp> add_wrap(const VExpr<_Tp>& a, const VExpr<_Tp>& b);
//template<typename _Tp> VExpr<_Tp> sub_wrap(const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Dp, typename _Tp> VExpr<_Dp> cast(const VExpr<_Tp>& a);
template<typename _Dp, typename _Tp> VExpr<_Dp> cast(const    VReg<_Tp>& a);
template<typename _Dp, typename _Tp> VExpr<_Dp> reinterpret(const VExpr<_Tp>& a);
template<typename _Dp, typename _Tp> VExpr<_Dp> reinterpret(const    VReg<_Tp>& a);
template<typename _Dp> VExpr<_Dp> trunc(const VExpr<f16_t>& a); //Convert with rounding to zero
template<typename _Dp> VExpr<_Dp> trunc(const    VReg<f16_t>& a);
template<typename _Dp> VExpr<_Dp> trunc(const VExpr<float>& a);
template<typename _Dp> VExpr<_Dp> trunc(const    VReg<float>& a);
template<typename _Dp> VExpr<_Dp> trunc(const VExpr<double>& a);
template<typename _Dp> VExpr<_Dp> trunc(const    VReg<double>& a);
template<typename _Dp> VExpr<_Dp> floor(const VExpr<f16_t>& a); //Convert with rounding to minus infinity
template<typename _Dp> VExpr<_Dp> floor(const    VReg<f16_t>& a);
template<typename _Dp> VExpr<_Dp> floor(const VExpr<float>& a);
template<typename _Dp> VExpr<_Dp> floor(const    VReg<float>& a);
template<typename _Dp> VExpr<_Dp> floor(const VExpr<double>& a);
template<typename _Dp> VExpr<_Dp> floor(const    VReg<double>& a);

//TODO(ch): cvtTp -> ceil, cvtTe -> round, also cast(double <=> float, float <=> f16_t)
template<typename _Tp> VExpr<_Tp> broadcast(const IExpr& scalar);
template<typename _Tp> VExpr<_Tp> broadcast(const VExpr<_Tp>& inp, int64_t ilane_index);
template<typename _Tp> VExpr<_Tp> broadcast(const    VReg<_Tp>& inp, int64_t ilane_index);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::duplicatetype> cast_low(const VExpr<_Tp>& r);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::duplicatetype> cast_low(const    VReg<_Tp>& r);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::duplicatetype> cast_high(const VExpr<_Tp>& r);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::duplicatetype> cast_high(const    VReg<_Tp>& r);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::halftype> shrink(const VExpr<_Tp>& r0, const VExpr<_Tp>& r1);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::halftype> shrink(const VExpr<_Tp>& r0, const    VReg<_Tp>& r1);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::halftype> shrink(const    VReg<_Tp>& r0, const VExpr<_Tp>& r1);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::halftype> shrink(const    VReg<_Tp>& r0, const    VReg<_Tp>& r1);

//Lane manipulations:
//template<typename _Tp> IExpr all(VExpr<_Tp>& a);
//template<typename _Tp> IExpr any(VExpr<_Tp>& a);
template<typename _Tp> IExpr getlane(const VExpr<_Tp>& r, int64_t lane_index);
template<typename _Tp> IExpr getlane(const    VReg<_Tp>& r, int64_t lane_index);
template<typename _Tp> void setlane(const VExpr<_Tp>& v, int64_t lane_index, const IExpr& i);
template<typename _Tp> void setlane(const    VReg<_Tp>& v, int64_t lane_index, const IExpr& i);
template<typename _Tp> void setlane(const VExpr<_Tp>& v, int64_t lane_index, const VExpr<_Tp>& inp, int64_t ilane_index);
template<typename _Tp> void setlane(const VExpr<_Tp>& v, int64_t lane_index, const    VReg<_Tp>& inp, int64_t ilane_index);
template<typename _Tp> void setlane(const    VReg<_Tp>& v, int64_t lane_index, const VExpr<_Tp>& inp, int64_t ilane_index);
template<typename _Tp> void setlane(const    VReg<_Tp>& v, int64_t lane_index, const    VReg<_Tp>& inp, int64_t ilane_index);
template<typename _Tp> VExpr<_Tp> reduce_max(const VExpr<_Tp>& r);
template<typename _Tp> VExpr<_Tp> reduce_max(const    VReg<_Tp>& r);
template<typename _Tp> VExpr<_Tp> reduce_min(const VExpr<_Tp>& r);
template<typename _Tp> VExpr<_Tp> reduce_min(const    VReg<_Tp>& r);
template<typename _Tp> VExpr<_Tp> reduce_sum(const VExpr<_Tp>& r);
template<typename _Tp> VExpr<_Tp> reduce_sum(const    VReg<_Tp>& r);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::duplicatetype> reduce_wsum(const VExpr<_Tp>& r);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::duplicatetype> reduce_wsum(const    VReg<_Tp>& r);
template<typename _Tp> VExpr<_Tp> ext(const VExpr<_Tp>& n, const VExpr<_Tp>& m, int64_t index);
template<typename _Tp> VExpr<_Tp> ext(const VExpr<_Tp>& n, const    VReg<_Tp>& m, int64_t index);
template<typename _Tp> VExpr<_Tp> ext(const    VReg<_Tp>& n, const VExpr<_Tp>& m, int64_t index);
template<typename _Tp> VExpr<_Tp> ext(const    VReg<_Tp>& n, const    VReg<_Tp>& m, int64_t index);

// Arithmetic and bitwise operations:
template<typename _Tp> VExpr<_Tp> operator + (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator + (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator + (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator + (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator - (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator - (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator - (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator - (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator * (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator * (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator * (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator * (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator / (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator / (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator / (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator / (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
//template<typename _Tp> VExpr<_Tp> operator % (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator - (const VExpr<_Tp>& a);
template<typename _Tp> VExpr<_Tp> operator - (const    VReg<_Tp>& a);

template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const VExpr<_Tp>& b, const VExpr<_Tp>& c);
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const VExpr<_Tp>& b, const    VReg<_Tp>& c);
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const    VReg<_Tp>& b, const VExpr<_Tp>& c);
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const    VReg<_Tp>& b, const    VReg<_Tp>& c);
template<typename _Tp> VExpr<_Tp> fma(const    VReg<_Tp>& a, const VExpr<_Tp>& b, const VExpr<_Tp>& c);
template<typename _Tp> VExpr<_Tp> fma(const    VReg<_Tp>& a, const VExpr<_Tp>& b, const    VReg<_Tp>& c);
template<typename _Tp> VExpr<_Tp> fma(const    VReg<_Tp>& a, const    VReg<_Tp>& b, const VExpr<_Tp>& c);
template<typename _Tp> VExpr<_Tp> fma(const    VReg<_Tp>& a, const    VReg<_Tp>& b, const    VReg<_Tp>& c);
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const VExpr<_Tp>& b, const VExpr<_Tp>& c, int64_t index);
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const VExpr<_Tp>& b, const    VReg<_Tp>& c, int64_t index);
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const    VReg<_Tp>& b, const VExpr<_Tp>& c, int64_t index);
template<typename _Tp> VExpr<_Tp> fma(const VExpr<_Tp>& a, const    VReg<_Tp>& b, const    VReg<_Tp>& c, int64_t index);
template<typename _Tp> VExpr<_Tp> fma(const    VReg<_Tp>& a, const VExpr<_Tp>& b, const VExpr<_Tp>& c, int64_t index);
template<typename _Tp> VExpr<_Tp> fma(const    VReg<_Tp>& a, const VExpr<_Tp>& b, const    VReg<_Tp>& c, int64_t index);
template<typename _Tp> VExpr<_Tp> fma(const    VReg<_Tp>& a, const    VReg<_Tp>& b, const VExpr<_Tp>& c, int64_t index);
template<typename _Tp> VExpr<_Tp> fma(const    VReg<_Tp>& a, const    VReg<_Tp>& b, const    VReg<_Tp>& c, int64_t index);

template<typename _Tp> VExpr<_Tp> pow(const VExpr<_Tp>& a, int p);
template<typename _Tp> VExpr<_Tp> pow(const VReg<_Tp>& a, int p);

struct exp_consts;
exp_consts expInit(Context CTX);
VExpr<float> exp(const VExpr<float>& x, const exp_consts& expt);
static inline VExpr<float> exp(const VReg<float>& x, const exp_consts& expt);

//template<typename _Tp, typename _Sp> VExpr<_Tp> operator >> (const VExpr<_Tp>& a, const VExpr<_Sp>& b);
template<typename _Tp> VExpr<_Tp> operator >> (const VExpr<_Tp>& a, int64_t b);
//template<typename _Tp, typename _Sp> VExpr<_Tp> ushift_right(const VExpr<_Tp>& a, const VExpr<_Sp>& b);
template<typename _Tp> VExpr<_Tp> ushift_right (const VExpr<_Tp>& a, int64_t b);
template<typename _Tp> VExpr<_Tp> ushift_right (const VReg<_Tp>& a, int64_t b);
template<typename _Tp, typename _Sp> VExpr<_Tp> operator << (const VExpr<_Tp>& a, const VExpr<_Sp>& b);
template<typename _Tp, typename _Sp> VExpr<_Tp> operator << (const    VReg<_Tp>& a, const    VReg<_Sp>& b);
template<typename _Tp, typename _Sp> VExpr<_Tp> operator << (const    VReg<_Tp>& a, const VExpr<_Sp>& b);
template<typename _Tp, typename _Sp> VExpr<_Tp> operator << (const VExpr<_Tp>& a, const    VReg<_Sp>& b);
template<typename _Tp> VExpr<_Tp> operator << (const VExpr<_Tp>& a, int64_t b);
//template<typename _Tp, typename _Sp> VExpr<_Tp> ushift_left(const VExpr<_Tp>& a, int64_t b);
template<typename _Tp, typename _Sp> VExpr<_Tp> ushift_left (const VExpr<_Tp>& a, const VExpr<_Sp>& b);
template<typename _Tp, typename _Sp> VExpr<_Tp> ushift_left (const    VReg<_Tp>& a, const    VReg<_Sp>& b);
template<typename _Tp, typename _Sp> VExpr<_Tp> ushift_left (const    VReg<_Tp>& a, const VExpr<_Sp>& b);
template<typename _Tp, typename _Sp> VExpr<_Tp> ushift_left (const VExpr<_Tp>& a, const    VReg<_Sp>& b);
template<typename _Tp> VExpr<_Tp> operator & (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator & (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator & (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator & (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator | (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator | (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator | (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator | (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator ^ (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator ^ (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator ^ (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator ^ (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> operator ~ (const VExpr<_Tp>& a);
template<typename _Tp> VExpr<_Tp> operator ~ (const    VReg<_Tp>& a);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> popcount(const VExpr<_Tp>& r);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> popcount(const VReg<_Tp>&  r);

// Vector comparisson and masking:
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator == (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator == (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator == (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator == (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator != (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator != (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator != (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator != (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator >= (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator >= (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator >= (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator >= (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator <= (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator <= (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator <= (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator <= (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator >  (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator >  (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator >  (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator >  (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator <  (const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator <  (const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator <  (const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<typename ElemTraits<_Tp>::masktype> operator <  (const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> select(const VExpr<typename ElemTraits<_Tp>::masktype>& flag, const VExpr<_Tp>& iftrue, const VExpr<_Tp>& iffalse);
template<typename _Tp> VExpr<_Tp> select(const VExpr<typename ElemTraits<_Tp>::masktype>& flag, const VExpr<_Tp>& iftrue, const    VReg<_Tp>& iffalse);
template<typename _Tp> VExpr<_Tp> select(const VExpr<typename ElemTraits<_Tp>::masktype>& flag, const    VReg<_Tp>& iftrue, const VExpr<_Tp>& iffalse);
template<typename _Tp> VExpr<_Tp> select(const VExpr<typename ElemTraits<_Tp>::masktype>& flag, const    VReg<_Tp>& iftrue, const    VReg<_Tp>& iffalse);
template<typename _Tp> VExpr<_Tp> select(const    VReg<typename ElemTraits<_Tp>::masktype>& flag, const VExpr<_Tp>& iftrue, const VExpr<_Tp>& iffalse);
template<typename _Tp> VExpr<_Tp> select(const    VReg<typename ElemTraits<_Tp>::masktype>& flag, const VExpr<_Tp>& iftrue, const    VReg<_Tp>& iffalse);
template<typename _Tp> VExpr<_Tp> select(const    VReg<typename ElemTraits<_Tp>::masktype>& flag, const    VReg<_Tp>& iftrue, const VExpr<_Tp>& iffalse);
template<typename _Tp> VExpr<_Tp> select(const    VReg<typename ElemTraits<_Tp>::masktype>& flag, const    VReg<_Tp>& iftrue, const    VReg<_Tp>& iffalse);
template<typename _Tp> VExpr<_Tp> max(const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> max(const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> max(const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> max(const VExpr<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> min(const VExpr<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> min(const    VReg<_Tp>& a, const    VReg<_Tp>& b);
template<typename _Tp> VExpr<_Tp> min(const    VReg<_Tp>& a, const VExpr<_Tp>& b);
template<typename _Tp> VExpr<_Tp> min(const VExpr<_Tp>& a, const    VReg<_Tp>& b);
//template<typename _Tp> VExpr<_Tp> abs(const VExpr<_Tp>& a);
//template<typename _Tp> VExpr<_Tp> sign(const VExpr<_Tp>& a);

//Augmenting operations:
template<typename _Tp> VReg<_Tp>& operator += (VReg<_Tp>& _a, const VExpr<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator += (VReg<_Tp>& _a, const    VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator -= (VReg<_Tp>& _a, const VExpr<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator -= (VReg<_Tp>& _a, const    VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator *= (VReg<_Tp>& _a, const VExpr<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator *= (VReg<_Tp>& _a, const    VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator /= (VReg<_Tp>& _a, const VExpr<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator /= (VReg<_Tp>& _a, const    VReg<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator %=  (VReg<_Tp>& _a, const VExpr<_Tp>& b);
//template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& _a, const VExpr<_Tp>& b)
template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& _a, int64_t _b);
template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& _a, const VExpr<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator >>= (VReg<_Tp>& _a, const    VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator <<= (VReg<_Tp>& _a, int64_t _b);
template<typename _Tp> VReg<_Tp>& operator  &= (VReg<_Tp>& _a, const VExpr<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator  &= (VReg<_Tp>& _a, const    VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator  |= (VReg<_Tp>& _a, const VExpr<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator  |= (VReg<_Tp>& _a, const    VReg<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator  ^= (VReg<_Tp>& _a, const VExpr<_Tp>& b);
template<typename _Tp> VReg<_Tp>& operator  ^= (VReg<_Tp>& _a, const    VReg<_Tp>& b);
}
#include "loops.ipp"
#endif
