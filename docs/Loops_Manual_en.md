# Loops Reference Manual

## Loops purpose

Loops is small and fast JIT-compiler, made like high-level C++ library. It is cross-platform, supports different 64-bit CPU architectures, different operational systems and compilers. Library interface imitate usual execution code for lowering of entry barrier. It's specification is vectorized code, main purpose is to write heavy computational kernels. Work with library is similar to work with SIMD intrinsics.<br/><br/>
Ability to compile code you need on fly makes possible to create _optimal_ kernel for any parameters set(e.g., eural networks operators) and to fuse loops of kernel, when it is known their call sequence, what avoids redundant cache pumping(DUBUG: i don't think прокачка кэша is pumping). Other words, main puprose of project is manual vector optimization.<br/><br/>
Project is inspired by [xbyak](https://github.com/herumi/xbyak). Xbyak is JIT-_assembler_, when loops is JIT-_compiler_. Main advantages of loops is automatic register allocation and being cross-platform.

## License
Loop is open-source project, distributed under permissive license Apache 2.

## Compatibility 
Supported architectures: 
* Aarch64. NEON vectors.
* x86_64. AVX2 vectors.
* Risc-V64. No vectors yet.

Supported operational systems:
* Linux: Aarch64, x86_64, Risc-V64.
* MacOS: Aarch64.
* Windows: x86_64.

Of course, there is a lot of compiler versions, which will compile library, there we only enumerating _verified_ compilers versions.<br/>
Verified supported compilers:
* gcc(DUBUG: get version) 
* clang(DUBUG: get version) 
* MSVC for Visual Studio 2019, Visual Studio 2022

## How download, build library and run samples

### Linux and Macos 

You'll need git, CMake and C++ compiler installed on your system.
1. Download from git and enter directory:
```
git clone https://github.com/4ekmah/loops
cd loops
```
2. Make build directory and configure:
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
```
3. Build:
```
make -j8
```

### Windows 

You will need installed VSCode, CMake and Visual Studio or separate Visual Studio Build Tools(https://aka.ms/vs/stable/vs_BuildTools.exe) . Also add CMake extension to VSCode. 
1. Download from git in Git Bash:
```
git clone https://github.com/4ekmah/loops
```
2. Open directory in VSCode. There you are able to configure, build and run examples. 

### Samples and tests

There is two runnable examples:
1. minmaxloc - get position and value of minimal element in array of integers.  
2. bresenham - implements line drawing algorithm by Bresenham.

Both examples uses only scalar instructions, so if you need SIMD examples, look in tests:  
1. testregress - big set of regression tests with different approaches and ideas.
2. testperf - regression and performannce tests for loopslayers, checks kernels compiled with different parameters.

## Basic concepts
Let's consider library interface and how to work with it.
### loops::Context, loops::Func

Since we are compiling functions in the runtime, we have the place to keep them. 
 **loops::Context** is such a storage. Mostly it is created once per program run. **loops::Func** represents one separate function and provides ability to take runnable pointer to that function. <br/><br/>
Let's look at simplified **loops::Context** interface.
```cpp
class Context
{
public:
    Context();
    virtual ~Context();
    void getFuncs(std::vector<Func>& funcs);
    Func getFunc(const std::string& name);
    bool hasFunc(const std::string& name);
    std::string getPlatformName() const;
    int vbytes() const;
    template<typename _Tp> inline int vlanes() const;
    void compileAll();
    std::vector<std::string> get_all_passes();
};
```
Class is made like smart pointer, therefore copy, move, assignment operations are cheap and don't cause some sufficient data transfer. On destruction, all memory, given for functions will be deallocated, so, all pointers will become illegal. **Context** is thread-unsafe, as well as loops at all, so synchronization is library's user's responsibility <br/><br/>
Main purpose of context is to contain functions. Functions are accessed by name with **getFunc**, **hasFunc** methods. Name is assigned on function creation. Other way is to get vector of all functions by **getFuncs** method.<br/><br/>
Also **Context** gives information about compiling platform, so user can adjust his code.
* **getPlatformName** - architecture name. Possible values: "AArch64", "Intel64" or "Risc-V".
* **vbytes** - size of SIMD register in bytes. E.g., 16 bytes on Arm, 32 bytes on Intel. 
* **vlanes** - amount of lanes of given type in SIMD register.
###
**compileAll** method gives ability to compile all declared in context functions simultaneously, can be helpful. It is not neccesary to call this function, compilation will happen automatically, when you'll request function pointer.
<br/><br/>
**get_all_passes** method will give you list of names of compilers passes. You can use this name in **Func::printIR** get the code after certain stage. It can be handy for debug.
<br/><br/>
Let's look at simplified **loops::Func** interface.
```cpp
class Func
{
public:
    Func();
    virtual ~Func();
    std::string name() const;
    void* ptr();
    enum { PC_OPNUM = 1 , PC_OP = 2, PC_HEX = 4 };
    void printIR(std::ostream& out, int columns = PC_OPNUM | PC_OP, const std::string& passname = "") const;
    void printAssembly(std::ostream& out, int columns = PC_OPNUM | PC_OP | PC_HEX) const;
};
```
**loops::Func** is a smart pointer as well as **loops::Context**. Copy, movement, assignments are cheap.
<br/><br/>
There is two code print methods:
* **printAssembly** prints current architecture's assembly.
* **printIR** prints cross-platform _intermediate representation_. **passname**  argument gives ability to choose stage to print IR after.
###
Both methods have **columns** to configure output columns:
* **PC_OPNUM** - line number with colon
* **PC_OP** - instruction
* **PC_HEX** - hexacademical represenation of encoded instruction. **printAssembly** only
###

Most important method is **ptr**, which gives you pointer to compiled function. This pointer can be casted to pointer to function of correct signature and called. There is two possible signatures of functions made by loops:
```cpp
void func(int64_t a0, int64_t a1, ...)
```
или 
```cpp
int64_t func(int64_t a0, int64_t a1, ...)
```
I.e it returns **void** or **int64_t**. There can be from zero to infinity of 64-bit arguments. It's possible to pass **double** or, what is really important, any kind of pointer, because pointers are 64-bit.
<br/><br/>
**NOTE:** I have to underline, that notation above is not about variadic functions, with flexible arguments amount. Every compiled loops function have certain number of arguments.
### Macro
Loops programmer is metaprogrammer, he write the program, which write other program. Our goal was to make metaprogramming really similar to usual. For better imitation there used macro, imitates function definition, embranchments, loops and other management constructions.
<br/><br/>
E.g., let's consider generation of the simplest function, sums up two integer numbers:
```cpp
#include "loops/loops.hpp"
#include <iostream>
int main()
{
    loops::Context CTX; 
    USE_CONTEXT_(CTX);
    loops::IReg a, b;
    STARTFUNC_("sum", &a, &b)
    {   
        loops::IReg c = a + b;
        RETURN_(c);
    } 
    loops::Func sfunc = CTX.getFunc("sum");
    typedef int64_t (*sum_t)(int64_t a, int64_t b);
    sum_t func = (sum_t)sfunc.ptr();
    std::cout << func(5, 4) << "\n";
    return 0;
}
```
All uppers case names with underline at the end are macro.
<br/><br/>
List of all loops macro:
* **USE_CONTEXT_(ctx)** - activates macro system in current base block, ties it up with given context.
* **STARTFUNC_(name, &arg0, &arg1,... )** - definition of function with given name and arguments. Brace after contains the body of generated function.
* **CONST_(x)** - scalar register generation with initialization by given number and attaching this register to current function. Usage example:
```cpp
    loops::IReg i = CONST_(0); 
```
* **VCONST_(type, x)** - SIMD register generation with initialization by given number and attaching this register to current function. Usage example: 
```cpp
    loops::VReg<float> pi = VCONST_(float, 3.141528f); 
```
* **DEF_(x)** - scalar register declaration with no initialization and attaching this register to current function. Usage example:
```cpp
    loops::IReg i = DEF_(); 
```
* **VDEF_(type)** - SIMD register declaration with no initialization and attaching this register to current function. Usage example: 
```cpp
    loops::VReg<float> a = VDEF_(float); 
```
* **IF_**, **ELIF_**, **ELSE_**, **WHILE_** - usual management construction, acts they have to in generated code.
* **RETURN_** - function return.
* **BREAK_/BREAK_(levels)** - stop the iteration and leave the loop. There is non-typical variation with specifying nesting level of loop to leave. Example:
```cpp
    WHILE_(i < 100)
        WHILE_(j < 100)
        {
            IF_(/*condition*/)
                BREAK_(2); //Leaving loop WHILE_(i < 100)
        }  
```
* **CONTINUE_/CONTINUE_(levels)** - - stop the iteration and start new. There is non-typical variation with specifying nesting level of loop to continue. Example:
* **CALL_(funcptr, arg0, arg1, ...)** - call outer function from generated code. Function return a value to be assigned to **IReg**. Return and all arguments types are 64-bit. Maximum amount of arguments is equal to amount of arguments passed via registers, accordingly to _calling convention_, i.e 4 on Windows on x86_64, 6 on Linux on x86_64, 8 on Arm and Risc-V.
* **VOID_CALL_(funcptr, arg0, arg1, ...)** - call outer function from generated code. Function doesn't return value. See **CALL_** about types and amount of arguments.

### IReg/VReg
**IReg** represents general purpose register, or, in a broader sense, scalar variable. Mostly, you can work with it, like with usual varibale: make arithmetical or bitwise actions, compare and so on. IReg keeps information about function it rely to. This the neccessry so that during metaprogramming, instructions are adding to buffer of function being created. But, when **IReg** are constructing without other **IReg** in right side expression, it's nowhere to get information about home function. **CONST_** macro help with it. So, let's consider example:
```cpp
    loops::IReg a = CONST_(1); 
    loops::IReg b = CONST_(2); 
    loops::IReg c = a + b; 
    loops::IReg d = с + 2; 
```
We have to use **CONST_** macro for **a** and **b** variables to provide information about current function. For **с** and **d** loops can infer information from the right side.
<br/><br/>
Sometimes, there is no need to set some initial value, so you can use **DEF_** macro (**VDEF_(type)** for **VReg**).
**IReg** as well as **VReg** must be constructed inside **STARTFUNC_** block. Only exception - definition of function's arguments:
```cpp
    USE_CONTEXT_(CTX);
    loops::IReg ptr, n, minpos_addr, maxpos_addr;
    STARTFUNC_("minmaxloc", &ptr, &n, &minpos_addr, &maxpos_addr)
    {
        //... 
    }
```
Of course, you can call some generation functions from **STARTFUNC_**, but you have to initialize context there for correct work of macros:
```cpp
    loops::IReg pow(const loops::IReg& a, int power, loops::Context CTX)
    {
        USE_CONTEXT_(CTX);
        loops::IReg mul = CONST_(1);
        for(int i = 0; i < times; i++)
            mul *= a;
        return mul;  
    }
    //...
    STARTFUNC_("subgenerated", &a)
    {
        //...
        loops::IReg b = pow(a, 3, CTX);
        //... 
    }
```
Formally, variables, descripted by **IReg** haven't type, but in most cases it's assumed, that this is int64_t.
<br/><br/>
First exception is work with memory - loads and stores. 
Первое исключение - это операции с памятью - загрузки и выгрузки. Basically, **IReg** is located on registers(like variables with **register** keyword in good old C), so memory for this variables is external environment. Therefore, load/store from/to memory have to be manual. loops allows to load to registers data of different types and sizes. If you are loading integers, smaller than 64 bit, higher part of register will be padded by zero bits of sign bits, depending on the type. List of types is same like list of types, allowed for **VReg** below. Let's consider example transforms element of array from **int32_t** to **int64_t**:
```cpp
    IReg inptr, outptr, size;
    STARTFUNC_("transform", &inptr, &outptr, &size)
    {
        loops::IReg i = CONST_(0);
        WHILE_(i < size)
        {
            loops::IReg loaded = loops::load_<int32_t>(inptr + i * sizeof(int32_t)); 
            loops::store_<int64_t>(outptr + i * sizeof(int64_t), loaded);
            i = i + 1;
        } 
    }
```
Look at address arithmetics. In loops it is always measured in bytes, not elements.
<br/>
Second exception is unsigned comparisson operations, where **IReg** is considered, like **uint64_t**. List of such a commands:
* **ule** - less or equal
* **uge** - greater or equal
* **ugt** - greater
* **ult** - less
####
**VReg<_Tp>** represent SIMD registers and corresponds to 256-bit **ymm** on AVX2 by Intel, or 128-bit **q**-registers on NEON by Arm. Risc-V doesn't have support of vector registers yet.
<br/>
Unlike **IReg**, **VReg** is typed, so it's always appropriate instructions are choosen for all arithmetical or other operations over them.
<br/><br/>
List of supported types:
* **int8_t**
* **uint8_t**
* **int16_t**
* **uint16_t**
* **int32_t**
* **uint32_t**
* **int64_t**
* **uint64_t**
* **float16_t**
* **float**
* **double**
####
**VCONST_(type, val)** or **VDEF(type)** are used for initialization.
<br/>
Let's consider as example vectorized elementwise application of function **exp(x+y)** to two equal **float** arrays. It's assumed, that their size is multiple of amount of lanes in SIMD array.
```cpp
    USE_CONTEXT_(CTX);
    IReg xptr, yptr, outptr, size;
    STARTFUNC_("transform", &xptr, &yptr, &outptr, &size)
    {
        loops::IReg i = CONST_(0);
        auto expc = expInit(ctx);
        WHILE_(i < size)
        {
            loops::VReg<float> x_in = loops::loadvec<float>(xptr, i * sizeof(float));
            loops::VReg<float> y_in = loops::loadvec<float>(yptr, i * sizeof(float));
            loops::VReg<float> res = exp(x_in + y_in, expc);
            loops::storevec(outptr, i * sizeof(float), res);
            i = i + CTX.vlanes<float>();
        }
    }
```
Note, that step over array is made by amount of lanes which can be taken in platform independent manner with **CTX.vlanes<float>()**.
<br/><br/>
Comparison masks deserve a special mention. This is common technique for SIMD registers, when you need to do something with elements depending on the conditions.Instead of extraction elements from SIMD register and independent processing, there generated SIMD masks, made by comparisson operations. This mask have one-only fields in corresponding elements, where condition is right, and zero-only fields otherwise. Further it is easy to apply this mask by usual binary AND to tested register and all unneccessary elements will be zeroed. I'm focusing attention on this, because masks in loops are SIMD unsigned integers by default, thus there needed **reinterpret** operations for applying. Let's consider as example sum of all positive elements in **int32_t** array.
```cpp
    USE_CONTEXT_(CTX);
    IReg ptr, size;
    STARTFUNC_("possum", &ptr, &size)
    {
        loops::IReg i = CONST_(0);
        loops::VReg<int32_t> vzero = VCONST_(int32_t, 0);
        loops::VReg<int32_t> vsum = VCONST_(int32_t, 0);
        WHILE_(i < size)
        {
            loops::VReg<int32_t> loaded = loops::loadvec<int32_t>(ptr, i * sizeof(int32_t));
            loops::VReg<uint32_t> mask = (loaded > vzero);
            vsum += loops::reinterpret<int32_t>(loops::reinterpret<uint32_t>(loaded) & mask);
            i = i + CTX.vlanes<int32_t>();
        }
        loops::IReg sum = loops::getlane(loops::reduce_sum(vsum), 0);
        RETURN_(sum);
    }
```
There is more powerful operation for more heavy cases - **select** function, which is analog of well-known ternary operator. See full list of SIMD operation below.
#### Deferred and massive register initialization
The rule is to initialize registers, when they appears in the metaprogram, but sometimes it's impossible, like you need to define container of registers. It can be even worse if you don't know, how much registers you will need and which of them will be used. But you can initialized register after it's definition. Unfortunately, **IReg/VReg** assigment operator in loops used for other purpose - metaprogramming imitates usula programming, so assignment means addition **mov** instruction to function buffer. That is the reason why it is used **copyidx** method for deferred initialization. First, you define empty **IReg** or **VReg**, than, depending on the situation you are assign or not assign to it real register index. I had a situation in a practice , when, depending on compilation parameters, mask was neccessary or not:
```cpp
    loops::VReg<int32_t> x;
    //...
    loops::VReg<uint32_t> mask;
    if(horizontal_padding)
         mask.copyidx((x - VCONST_(int32_t, padding_size)) > VCONST_(int32_t, 0));
```
**copyidx** argument is expression, which will have register index until call and method will copy it and initialize **mask** variable by attaching it to function.
<br/><br/>
Register array initialization:
```cpp
    std::vector<loops::IReg> reg_arr;
    for(int i = 0; i < (int)reg_arr.size(); i++)
        reg.copyidx(CONST_(i)); 
```
## IExpr, VExpr. How to write your own generator functions
If you will examine loops interface with attention, you'll find, that most of defined operation works with **IExpr/VExpr**, not with **IReg/VReg**. This is special classe, which keeps expression tree until it is assigned to some register or used as condition in **IF_**, **ELIF_** or **WHILE_**. Assignment and management constructions are kind of synchronization points, which appends code to function buffer. Mostly, user don't have to interact with these classes, but there is important exception: situations, when he want to create small generator for multiple usage, e.g., his own variation of computational function like **sin**, **pow** and so on. Let's consider as example function of smart constant multiplication, which will automatically substitute multiplication with shift, when multiplier is a power of 2:
```cpp
loops::IExpr effective_const_mul(const loops::IReg& m1, int m2)
{
    if(m2 == 1)
        return loops::IExpr(m1);
    else if(m2>0 && (((m2 - 1) & m2) == 0))
    {
        int degree = -1;
        while(m2 > 0)
        {
            degree++;
            m2 >>= 1;
        }
        return m1 << degree;
    }
    else
        return m1*m2;
}
```
Generally, it is more correct way to write subgenerators, than direct usage fo **IReg/VReg**, like it was in **pow** example in **IReg/VReg** section, but there is restrictions. **IReg** assignments are not allowed in **IF_**, **ELIF_** or **WHILE_**, because it will cause adding code to buffer and the whole idea of **IExpr/VExpr** will be lost.
<br/><br/>
That mean, that only reason to use **loops::Context** and **USE_CONTEXT_** in subgenerator code is to create constants. B.t.w, there is special function **ExtractContext**, which takes context from expression. Standart usage looks like this:
```cpp
IExpr pow(const IExpr& a, int p)
{
    Context CTX = ExtractContext(a.notype());
    USE_CONTEXT_(CTX);
    if(p == 0)
        return CONST_(1);
    //...
}
```
**VExpr** is parameterized by type as well as **VReg**.
<br/>
## How to append instuction.
There is not special interface for appending custom instructions yet. So, if you need to add something from the repertoire of a particular architecture, you can change the source code of the library. 
<br/><br/>
In general, this is not so easy, but in _most cases_, when instruction have one output register, multiple input register, enconding can be reduced to sequence of simple fields and there is register allocation limitations, it can be done with few simple steps.
1. You need to append IR version of instruction to big *enum* with instructions in loops.hpp header file. It's good to use some separate number range for avoid possible conflicts with loops updates.
```cpp
    OP_MYOP            = 1000,
``` 
2. Assign printing string to IR instruction in printer.cpp source file in big **opstrings_** name map.
```cpp
    LOOPS_HASHMAP_ELEM(loops::OP_MYOP             , "myop"                  ) ,
```
3. Append assembly version of instruction to header file of target platform(backend_aarch64.hpp, backend_intel64.hpp, backend_riscV.hpp). E.g., let's consider Arm:
```cpp
    AARCH64_MYOP       = 1000
``` 
4. Assign printing string to assembly instruction in source file of target platform(backend_aarch64.cpp, backend_intel64.cpp, backend_riscV.cpp) to function opstrings_getter_:
```cpp
    case (loops::AARCH64_MYOP        ) : return "myop"        ;
```
5. Add IR to assembly transformation rule in source file of target platform. You need i64STLookup function on Intel, a64STLookup on Arm, r64STLookup on Risc-V. Let's pretend, that our instruction have one vector output, one vector input and one scalar input. SyT rule is consist of target assembly instruction, and the list of argument. **SAcop** just copies arguments from IR, but there are different argument descriptors, e.g. **SAimm** just adds immediate argument. Our rule with consistency check will looks approximately like this:
```cpp
    case (OP_MYOP):
        if (index.size() == 3 &&
            index[0].tag == Arg::VREG &&
            index[1].tag == Arg::VREG &&
            index[1].tag == Arg::IREG &&
            index.args[0].elemtype == index.args[1].elemtype)
                return SyT(AARCH64_MYOP, { SAcop(0), SAcop(1), SAcop(2) });
        break;
```
6.  Add assembly instruction _encoding_ transformation rule in source file of target platform. You need i64BTLookup function on Intel, a64BTLookup on Arm, r64BTLookup on Risc-V. Like in point five, first you need to check arguments consistency and, second to create _encoding_-rule. It consists of details, which are transfromed to bitfields, extracted from argument body or constant. **BTreg(n, f, fl)** uses **f** bit  of **n** argument, checking this argument is _register_,**BTimm(n, f, fl)** uses **f** bit of **n** argument, checking this register is _immediate_, **BTsta(s, f)** uses **f** bits of input  входного static field **s**. ALso, it is very important to set **fl** flags, which point to compiler, which arguments are input, and which are output, it is critical information for _register allocator_. Our rule with consistency check will looks approximately like this:
```cpp
    case (AARCH64_MYOP):
        if (index.size() == 3 &&
            index[0].tag == Arg::VREG &&
            index[1].tag == Arg::VREG &&
            index[1].tag == Arg::IREG &&
            index.args[0].elemtype == index.args[1].elemtype)
            return BiT({ BTsta(0b01001110011, 11), BTreg(2, 5, In), BTsta(0b111101, 6), 
                         BTreg(1, 5, In), BTreg(0, 5, Out) });
        break;
```
7. Last step to make it work, you need to append function to user interface, which will allow you to use this instruction in your code. In our cse it will look like this:
```cpp
template<typename _Tp>
VExpr<_Tp> myop(const VExpr<_Tp>& a, const IExpr& b)
{ 
    return VExpr<_Tp>(OP_MYOP, {r.notype(), b.notype()});
}
```
## List of SIMD operations
This section contain all SIMD operation available for loops programmer.All this operations works on Arm, and almost all works on Intel. If operation is not supported on Intel, it have to be mentioned, but I advice to check all operation.

### Memory operations
* **loadvec(ptr, offset)/loadvec(ptr)** - load vector from memory by address **ptr + offset**  
* **loadlane(ptr, lane_index)** - load to element number **lane_index** of vector register from address **ptr**. Arm only
* **loadvec_deinterleave2(res1, res2, ptr)** - load with initialization two vectors - all even(from 0) element will fill **res1**, all odd will fill **res2**, source address is **ptr**. Arm only
* **storevec(ptr, offset, v)/storevec(ptr, v)** - store vector **v** to memory by **ptr + offset** address
* **storelane(ptr, v, lane_index)** - save element number **lane_index** from vector **v** to memory by address **ptr**. Arm only

### Type casts
* **cast(v)** - elementwise type cast
* **reinterpret(v)** - replacing type of vector with no cast. Operation doesn't cause any real actions in target program
* **trunc(v)**  - cast to integer(signed or unsigned) with rounding to zero. Supported types - **float16_t**, **float**, **double**
* **floor(v)**  - cast to integer(signed or unsigned) with rounding to minus infinity.
* **broadcast(s)/broadcast(v, lane_index)** - filling vector register with elements from scalar register **s** or element number **lane_index** from vector register **v**. Second variation works only on Arm.
* **cast_low(v)** - widening cast, doubling size of element, e.g. **int16_t** to **int32_t**. Target register can keep only half of values, this instruction create register with lower half.
* **cast_high(v)** - widening cast, doubling size of element, e.g. **int16_t** to **int32_t**. Target register can keep only half of values, this instruction create register with higher half.
* **shrink(v0, v1)** - shrinikng cast, halfing size of element, e.g. **int32_t** to **int16_t**. Target register can keep double set of values, so instruction need two input regsiters. Arm only

### Element manipulations

* **getlane(v, lane_index)** - extracts elemnt number **lane_index** from vector **v** to scalar register
* **setlane(v, lane_index, s)/setlane(v, lane_index_v, vi, lane_index_vi)** - sets element number **lane_index_v** in vector **v** to value from scalar register **s** or from element number **lane_index_vi** of vector **vi**. Second variation is Arm-only
* **ext(v0, v1, lane_index)** - concatenation of two vector an extraction subvector starts from **lane_index**

### Arithmetical operations
* **operator+(v0, v1)** - elementwise addition
* **operator-(v0, v1)** - elementwise subtraction
* **operator*(v0, v1)** - elementwise multiplication
* **operator/(v0, v1)** - elementwise division
* **operator-(v0)** - elementwise unary minus
* **fma(v0,v1,v2)/fma(v0,v1,v2,lane_index)** - elementwise **v0+v1*v2** or **v0+v1*v2[lane_index]**. Second variation is Arm only
* **pow(v,p)** - elementwise **p** power
* **expInit(CTX)** - initialization of constants neccessary for fast elementwise exponent
* **exp(v, expt)** - fast elementwise exponent
* **max(v0, v1)** - elementwise maximum
* **min(v0, v1)** - elementwise minimum

* **reduce_max(v)** - set zero element of target to maximum element of source. Arm only
* **reduce_min(v)** - set zero element of target to minimum element of source. Arm only
* **reduce_sum(v)** - set zero element of target to sum of elements of source
* **reduce_wsum(v)** - set zero element of target, which have double size elements to sum of elements of source. Helps against overflow

 
### Bitwise operation

* **operator>>(v, s)** - elementwise bit shift to right
* **ushift_right(v, s)** - elementwise unsigned bit shift to right
* **operator<<(v, s)** - elementwise bit shift to left 
* **ushift_left(v, s)** - elementwise  unsigned bit shift to left

* **operator&(v0, v1)** - bitwise AND
* **operator|(v0, v1)** - bitwise OR
* **operator^(v0, v1)** - bitwise EXCLUSIVE OR
* **operator~(v)** - bitwise NOT
* **popcount(v)** - bytewise non-zero bits count. Arm only

### Операции сравнения и маскирования

* **operator==(v0, v1)** - elementwise equal, output is mask, filled with ones for element condition was right, zeroes condition was wrong
* **operator!=(v0, v1)** - elementwise inequal, output is mask, filled with ones for element condition was right, zeroes condition was wrong
* **operator>=(v0, v1)** - elementwise greater or equal, output is mask, filled with ones for element condition was right, zeroes condition was wrong
* **operator<=(v0, v1)** - elementwise less or equal, output is mask, filled with ones for element condition was right, zeroes condition was wrong
* **operator>(v0, v1)** - elementwise greatee, output is mask, filled with ones for element condition was right, zeroes condition was wrong
* **operator<(v0, v1)** - elementwise less, output is mask, filled with ones for element condition was right, zeroes condition was wrong
* **select(mask, v0, v1)** - elementwise ternary operator, choosing by mask elements from **v0** for right elements and from **v1** for wrong elements