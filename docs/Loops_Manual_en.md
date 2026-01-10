# Loops Reference Manual

## Loops purpose

Loops is a small and fast JIT-compiler, implemented as high-level C++ library. It is cross-platform and supports different 64-bit CPU architectures, different operating systems and compilers. The library interface imitates usual program code, which lowers the entry barrier. Its specialization is vectorized code; the main purpose of the library is to write computationally heavy kernels. Working with the library is similar to working with SIMD intrinsics.<br/><br/>
The ability to compile the required code on the fly makes it possible to build _optimal_ kernels for any set of parameters(e.g., neural network operators), as well as to fuse kernel loops when their call sequence is already known. The last helps to avoid redundant data transfers through the cache. In other words, the main purpose of the project is manual vector optimization.<br/><br/>
The project is inspired by [xbyak](https://github.com/herumi/xbyak). Xbyak is a JIT _assembler_, whereas loops is a JIT _compiler_. The main advantages of loops are automatic register allocation and being cross-platform.

## License
Loops is an open-source project distributed under the permissive Apache 2 license.

## Compatibility 
Supported architectures:
* Aarch64. NEON vectors.
* x86_64. AVX2 vectors.
* Risc-V64. No vector support yet.

Supported operating systems:
* Linux: Aarch64, x86_64, Risc-V64.
* MacOS: Aarch64.
* Windows: x86_64.

Of course, many compiler versions can build the library. There we are enumerating only _verified_ compilers versions.<br/>
Verified supported compilers:
* GCC 12.2.0, 13.2.0
* Clang 15.0.0
* MSVC for Visual Studio 2019, Visual Studio 2022

## How to download, build the library, and run samples

### Linux and macOS 

You need git, CMake, and a C++ compiler installed on your system.
1. Clone the repository and enter the directory:
```
git clone https://github.com/4ekmah/loops
cd loops
```
2. Create a build directory and configure the project:
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
```
3. Build the project:
```
make -j8
```

### Windows 

You need VSCode, CMake, and Visual Studio installed, or Visual Studio Build Tools(https://aka.ms/vs/stable/vs_BuildTools.exe). You also need to appennd the CMake extension to VSCode.
1. Clone the repository using Git Bash:
```
git clone https://github.com/4ekmah/loops
```
2. Open the loops directory in VSCode. There you can configure, build, and run examples and tests.

### Samples and tests

There are two simple runnable examples:
1. minmaxloc - finds the position and value of the maximum element in an array of integers.
2. bresenham - an implementation of the Bresenham line drawing algorithm..

Both examples use only scalar instructions. For SIMD examples, see the test code:
1. testregress - a large set of regression tests demonstrating different ideas and approaches.
2. testperf - regression and performance tests for the loopslayers library. These tests verify correctness and measure execution time of kernels compiled with different parameters.

## Basic concepts
Let us consider the library interface and how to work with it.
### loops::Context, loops::Func

Since functions are compiled at runtime, there must be a place to store them. **loops::Context** serves as such a storage and is usually created once. **loops::Func** represents a single function and allows obtaining a runnable pointer to it. <br/><br/>
Below is a simplified interface of **loops::Context**:
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
The class behaves like a smart pointer, therefore copy, move, and assignment operations are cheap and do not involve noticeable data movement. When the context is destroyed, all memory allocated for functions is deallocated, and all function pointers become invalid. **loops::Context**, as well as loops in general, is not thread-safe, so synchronization is the responsibility of the library user.<br/><br/>

The main purpose of the context is to contain functions. Functions can be accessed by name using **getFunc** or **hasFunc**, or by retrieving the vector of all functions via **getFuncs**.<br/><br/>
In addition, **loops::Context** provides information about the compilation platform, so user can adjust his metaprogram:
* **getPlatformName** - architecture name. Possible values: "AArch64", "Intel64" or "Risc-V".
* **vbytes** - size of a SIMD register in bytes (e.g., 16 bytes on Arm, 32 bytes on Intel).
* **vlanes** - number of elements of the given type in a SIMD register.
###
The **compileAll** method allows compiling all functions defined in the context at once. This is optional; compilation will occur automatically when a function pointer is requested.
<br/><br/>
The **get_all_passes** method returns a list of compiler pass names. By specifying a pass name in **Func::printIR**, you can print the IR after a specific pass, which can be handy for debugging.
<br/><br/>
Now let us look at the simplified interface of **loops::Func**:
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
Like **loops::Context**, **loops::Func** is a smart pointer. Copying and moving are cheap operations.
<br/><br/>
There are two methods for printing function code:
* **printAssembly** prints the assembly representation for the current architecture.
* **printIR** prints the cross-platform _intermediate representation_. The **passname** argument allows selecting the compiler pass after which the IR is printed.
###
Both methods allow configuring output columns via the **columns** argument:
* **PC_OPNUM** - line number with a colon
* **PC_OP** - instruction
* **PC_HEX** - hexadecimal representation of the encoded instruction (only for **printAssembly**)
###
The most important method is **ptr**, which returns a pointer to the compiled function. This pointer can be cast to a function pointer of the required signature and called. There are two possible signatures for loops-generated functions:
```cpp
void func(int64_t a0, int64_t a1, ...)
```
or
```cpp
int64_t func(int64_t a0, int64_t a1, ...)
```
That is, the return type is either **void** or **int64_t**, and the function can take from zero to an arbitrary number of 64-bit arguments. You can pass **double** values or, importantly, any pointers, since pointers are 64-bit.
<br/><br/>
**NOTE:** Despite the notation above, these are not variadic functions. Each compiled loops function has a fixed number of arguments.
### Macro
A loops programmer is a metaprogrammer, meaning they write a program that generates another program. Our goal was to make metaprogramming as close as possible to ordinary programming. To achieve this, macros are used to imitate function definitions, branches, loops, and other control-flow constructs.
<br/><br/>
For example, let us consider generating the simplest function that adds two integers:
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
All names written in uppercase and ending with an underscore are macros.
<br/><br/>
List of all loops macros:
* **USE_CONTEXT_(ctx)** - activates the macro system in the current basic block and binds it to the given context.
* **STARTFUNC_(name, &arg0, &arg1,... )** - defines a function with the given name and arguments. The following braces contain the body of the generated function.
* **CONST_(x)** - creates a scalar register initialized with the given value and binds it to the current function. Example:
```cpp
    loops::IReg i = CONST_(0); 
```
* **VCONST_(type, x)** - creates a SIMD register with all elements initialized to the given value and binds it to the current function. Example:
```cpp
    loops::VReg<float> pi = VCONST_(float, 3.141528f); 
```
* **DEF_(x)** - declares a scalar register without initialization and binds it to the current function. Example:
```cpp
    loops::IReg i = DEF_(); 
```
* **VDEF_(type)** - declares a SIMD register without initialization and binds it to the current function. Example:
```cpp
    loops::VReg<float> a = VDEF_(float); 
```
* **IF_**, **ELIF_**, **ELSE_**, **WHILE_** - standard control-flow constructs that behave as expected in the generated code.
* **RETURN_** - returns a value from the function.
* **BREAK_/BREAK_(levels)** - breaks out of a loop. There is a non-standard variation that allows specifying the nesting level of the loop to exit. Example:
```cpp
    WHILE_(i < 100)
        WHILE_(j < 100)
        {
            IF_(/*condition*/)
                BREAK_(2); //Leaving loop WHILE_(i < 100)
        }  
```
* **CONTINUE_/CONTINUE_(levels)** - skips to the next loop iteration. There is also a variation with an explicit nesting level.
* **CALL_(funcptr, arg0, arg1, ...)** - calls an external function from generated code. The function returns a value that can be assigned to an **IReg**. The return type and all argument types are 64-bit. The maximum number of arguments corresponds to the _calling convention_: up to 4 on Windows x86_64, up to 6 on Linux x86_64, and up to 8 on Arm and RISC-V.
* **VOID_CALL_(funcptr, arg0, arg1, ...)** - calls an external function that does not return a value. See CALL_ for details on argument types and limits.

### IReg/VReg
**IReg** represents a scalar register or, more generally, a scalar variable. In most cases, it can be used like a normal variable: arithmetic operations, comparisons, and many other operations are supported. IReg keeps information about the function it belongs to, which is necessary so that instructions are added to the correct function buffer during metaprogramming. However, when an **IReg** is constructed without using other **IReg** objects on the right-hand side, there is no way to infer the owning function. For this reason, the **CONST_** macro is usually used. Consider the following example:
```cpp
    loops::IReg a = CONST_(1); 
    loops::IReg b = CONST_(2); 
    loops::IReg c = a + b; 
    loops::IReg d = с + 2; 
```
For variables **a** and **b**, the **CONST_** macro is required to provide information about the current function. For **c** and **d**, loops can infer this information from the right-hand side expressions.
<br/><br/>
If there is no need to assign an initial value, the **DEF_** macro can be used (**VDEF_(type)** for **VReg**).
**IReg**, as well as **VReg**, must be constructed only inside a **STARTFUNC_** block. The only exception is the declaration of function arguments:
```cpp
    USE_CONTEXT_(CTX);
    loops::IReg ptr, n, minpos_addr, maxpos_addr;
    STARTFUNC_("minmaxloc", &ptr, &n, &minpos_addr, &maxpos_addr)
    {
        //... 
    }
```
Obviously, calling generator functions from inside **STARTFUNC_** is also allowed, but in that case the context must be initialized inside the generator for correct macro behavior:
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
Formally, variables represented by **IReg** have no explicit type, but in most cases they are assumed to be **int64_t**.
<br/><br/>

The first exception is memory operations — loads and stores. Basically, **IReg** values reside in registers(similar to variables declared with the **register** keyword in good old C), so memory is considered as an external environment. Therefore, memory loads and stores must be performed manually. Loops allows loading values of different types and sizes. When loading integer types smaller than 64 bits, the upper part of the register is filled with zeros or sign bits, depending on the type. The list of supported types is the same as for **VReg** (see below). As an example, consider converting an array from **int32_t** to **int64_t**:
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
Note that address arithmetic in loops is always measured in bytes, not in element sizes.
<br/>
The second exception concerns unsigned comparison operations, where **IReg** is treated as **uint64_t**. The list of such operations:
* **ule** - unsigned less or equal
* **uge** - unsigned greater or equal
* **ugt** - unsigned greater
* **ult** - unsigned less
####

**VReg<_Tp>** represents SIMD registers and corresponds to 256-bit **ymm** registers on Intel AVX2 or 128-bit **q** registers on Arm NEON. Vector registers are not yet supported on RISC-V.
<br/>
Unlike **IReg**, **VReg** is typed, which allows loops to automatically select the appropriate instructions for all arithmetic and other operations.
<br/><br/>
Supported element types:
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
Initialization is done using **VCONST_(type, val)** or **VDEF_(type)**.
<br/>
As an example, consider a vectorized element-wise application of **exp(x+y)** to two **float** arrays, assuming that their size is a multiple of the number of lanes in a SIMD register:
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
Note that the loop step is equal to the number of lanes, which can be obtained in a platform-independent way using **CTX.vlanes<float>()**.
<br/><br/>
Comparison masks deserve special mention. This is a common technique when working with SIMD registers, where certain operations must be applied to elements depending on a condition. Instead of extracting SIMD elements one by one and processing them individually, comparison operations generate masks. A mask is a SIMD register where elements corresponding to a true condition contain all bits set to one, and elements corresponding to a false condition contain zeros. After that, it is sufficient to apply this mask to the tested register, and all unnecessary elements will be zeroed. This is emphasized here because masks in loops are, by definition, unsigned integer SIMD registers. Therefore, the **reinterpret** operations are required to apply them. As an example, consider computing the sum of all positive elements in an **int32_t** array:
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

For more complex cases, the vector equivalent of the ternary operator is provided — the **select** function. A full list of vector operations is given later in this document.
#### Deferred or bulk register initialization


In general, registers should be initialized as soon as they appear in the metaprogram, but this is not always possible. For example, you may need to declare a whole container of registers, while the number of required registers or whether they will be used at all is not known in advance. For such cases, deferred initialization is used. Unfortunately, the **IReg/VReg** assignment operator and copy constructor in loops are tied to other functionality: since metaprogramming closely imitates ordinary programming, assignment means that **mov** instruction will be added to function buffer. Therefore, deferred initialization is implemented via the **copyidx** method.
<br/><br/>
First, an empty **IReg** or **VReg** is created. Later, depending on the situation, it may or may not be assigned a real register index. In practice, a typical use case is when a mask is required only under certain conditions:
```cpp
    loops::VReg<int32_t> x;
    //...
    loops::VReg<uint32_t> mask;
    if(horizontal_padding)
         mask.copyidx((x - VCONST_(int32_t, padding_size)) > VCONST_(int32_t, 0));
```
The argument of **copyidx** is an expression that already has a register index at the moment of copying. This index is then copied and used to initialize the **mask** variable, binding it to the current function.
<br/><br/>
Initialization of register arrays:
```cpp
    std::vector<loops::IReg> reg_arr;
    for(int i = 0; i < (int)reg_arr.size(); i++)
        reg.copyidx(CONST_(i)); 
```
## IExpr, VExpr. How to write your own generator functions

If you examine the loops interface closely, you will notice that most declared operations work with **IExpr** and **VExpr**, rather than with **IReg** and **VReg**. These are special classes that store an expression tree until it is assigned to a register or used as a condition in **IF_**, **ELIF_**, or **WHILE_**. Assignments and control-flow constructs act as synchronization points that guarantee code emission into the function buffer. In most cases, users do not need to interact with these classes directly. However, there is an important exception: when you want to create a small generator that will be reused multiple times, for example, a custom computational function such as **sin**, **pow**, and similar. As an example, consider an optimized constant multiplication that automatically replaces multiplication by a power of two with a bit shift:
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
In general, this is a more correct approach for writing subgenerators than directly using **IReg** or **VReg**, as in the **pow** example in the **IReg/VReg** section. However, there are limitations. Such generators must not contain **IReg** assignments, **IF_**, **ELIF_**, or **WHILE_**, because these operations would emit code into the function buffer and thus break the idea of **IExpr/VExpr**.
<br/><br/>
As a consequence, **loops::Context** and **USE_CONTEXT_** are only needed in such code when constants must be created. For this purpose, a special function **ExtractContext** is provided, which extracts the context from an expression. A typical usage looks like this:
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
## How to append an instruction
There is no special interface for appending custom instructions yet. So, if you need to add something from the repertoire of a particular architecture, you have to modify the source code of the library.
<br/><br/>
In general, this is not very easy, but in _most cases_, when an instruction has one output register, multiple input registers, encoding can be reduced to a sequence of simple fields, and there are register allocation limitations, it can be done in a few simple steps.
1. You need to append the IR version of the instruction to the large **enum** with instructions in the loops.hpp header file. It is recommended to use a separate number range to avoid possible conflicts with future loops updates.
```cpp
    OP_MYOP            = 1000,
``` 
2. Assign a printing string to the IR instruction in the printer.cpp source file in the large **opstrings_** map.
```cpp
    LOOPS_HASHMAP_ELEM(loops::OP_MYOP             , "myop"                  ) ,
```
3. Append the assembly version of the instruction to the header file of the target platform (backend_aarch64.hpp, backend_intel64.hpp, backend_riscV.hpp). For example, let us consider Arm:
```cpp
    AARCH64_MYOP       = 1000
``` 
4. Assign a printing string to the assembly instruction in the source file of the target platform (backend_aarch64.cpp, backend_intel64.cpp, backend_riscV.cpp) in the **opstrings_getter_** function:
```cpp
    case (loops::AARCH64_MYOP        ) : return "myop"        ;
```
5. Add an IR-to-assembly transformation rule in the source file of the target platform. You need the **i64STLookup** function on Intel, **a64STLookup** on Arm, and **r64STLookup** on RISC-V. Let us assume that our instruction has one vector output, one vector input, and one scalar input. The **SyT** rule consists of the target assembly instruction and the list of arguments. **SAcop** simply copies arguments from IR, but there are different argument descriptors; for example, **SAimm** adds an immediate argument. Our rule with a consistency check will look approximately like this:
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
6. Add an assembly instruction _encoding_ transformation rule in the source file of the target platform. You need the **i64BTLookup** function on Intel, **a64BTLookup** on  Arm, and **r64BTLookup** on RISC-V. As in step five, first you need to check argument consistency, and second, create an encoding rule. It consists of details that are transformed into bit fields, extracted from argument bodies or constants. **BTreg(n, f, fl)** uses **f** bits of the **n**-th argument, checking that this argument is a register. **BTimm(n, f, fl)** uses **f** bits of the **n**-th argument, checking that this argument is an immediate. **BTsta(s, f)** uses **f** bits of the input static field **s**. Also, it is very important to set the **fl** flags, which tell the compiler which arguments are inputs and which are outputs. This is critical information for the _register allocator_. Our rule with a consistency check will look approximately like this:
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
7. As the last step to make it work, you need to append a function to the user interface that allows you to use this instruction in your code. In our case, it will look like this:
```cpp
template<typename _Tp>
VExpr<_Tp> myop(const VExpr<_Tp>& a, const IExpr& b)
{ 
    return VExpr<_Tp>(OP_MYOP, {r.notype(), b.notype()});
}
```
## List of SIMD operations
This section contains all SIMD operations available to a loops programmer. All these operations work on Arm, and almost all of them work on Intel. If an operation is not supported on Intel, it is mentioned explicitly, but it is recommended to check supported operations.

### Memory operations
* **loadvec(ptr, offset)/loadvec(ptr)** - load vector from memory at address **ptr + offset**  
* **loadlane(ptr, lane_index)** - load into element number **lane_index** of a vector register from address **ptr**. Arm only
* **loadvec_deinterleave2(res1, res2, ptr)** - load and initialize two vectors: all even(starting from 0) elements fill **res1**, all odd elements fill **res2**, the source address is **ptr**. Arm only
* **storevec(ptr, offset, v)/storevec(ptr, v)** - store vector **v** to memory at address **ptr + offset** 
* **storelane(ptr, v, lane_index)** - save element number **lane_index** from vector **v** to memory at address **ptr**. Arm only

### Type casts
* **cast(v)** - element-wise type cast
* **reinterpret(v)** - replace the type of a vector without casting. This operation doesn't cause any real actions in the target program
* **trunc(v)**  - cast to integer (signed or unsigned) with rounding toward zero. Supported types - **float16_t**, **float**, **double**
* **floor(v)**  - cast to integer (signed or unsigned) with rounding toward minus infinity. Supported types - **float16_t**, **float**, **double**
* **broadcast(s)/broadcast(v, lane_index)** - fill a vector register with elements from scalar register **s** or from element number **lane_index** of vector register **v**. The second variant works only on Arm
* **cast_low(v)** - widening cast, doubling the element size, e.g. **int16_t** to **int32_t**. The target register can keep only half of the values; this instruction creates a register with the lower half
* **cast_high(v)** - widening cast, doubling the element size, e.g. **int16_t** to **int32_t**. The target register can keep only half of the values; this instruction creates a register with the higher half
* **shrink(v0, v1)** - shrinking cast, halving the element size, e.g. **int32_t** to **int16_t**. The target register can keep a double set of values, so the instruction needs two input registers. Arm only

### Element manipulations

* **getlane(v, lane_index)** - extract elemnt number **lane_index** from vector **v** into a scalar register
* **setlane(v, lane_index, s)/setlane(v, lane_index_v, vi, lane_index_vi)** - set element number **lane_index_v** in vector **v** to a value from scalar register **s** or from element number **lane_index_vi** of vector **vi**. The second variant is Arm only
* **ext(v0, v1, lane_index)** - concatenate two vectors and extract a subvector starts from **lane_index**

### Arithmetical operations
* **operator+(v0, v1)** - element-wise addition
* **operator-(v0, v1)** - element-wise subtraction
* **operator*(v0, v1)** - element-wise multiplication
* **operator/(v0, v1)** - element-wise division
* **operator-(v0)** - element-wise unary minus
* **fma(v0,v1,v2)/fma(v0,v1,v2,lane_index)** - element-wise **v0+v1*v2** or **v0+v1*v2[lane_index]**. The second variation is Arm only
* **pow(v,p)** - element-wise power **p**
* **expInit(CTX)** - initialization of constants necessary for fast element-wise exponent
* **exp(v, expt)** - fast element-wise exponent
* **max(v0, v1)** - element-wise maximum
* **min(v0, v1)** - element-wise minimum
* **reduce_max(v)** - set the zero element of the result to the maximum element of the source. Arm only
* **reduce_min(v)** - set the zero element of the result to the minimum element of the source. Arm only
* **reduce_sum(v)** - set the zero element of the result to the sum of elements of the source
* **reduce_wsum(v)** - set the zero element of the result, which has double-size elements, to the sum of elements of the source. Helps to avoid overflow

 
### Bitwise operation

* **operator>>(v, s)** - element-wise bit shift to the right
* **ushift_right(v, s)** - element-wise unsigned bit shift to the right
* **operator<<(v, s)** - element-wise bit shift to the left 
* **ushift_left(v, s)** - element-wise  unsigned bit shift to the left

* **operator&(v0, v1)** - bitwise AND
* **operator|(v0, v1)** - bitwise OR
* **operator^(v0, v1)** - bitwise EXCLUSIVE OR
* **operator~(v)** - bitwise NOT
* **popcount(v)** - byte-wise count of non-zero bits. Arm only

### Comparisson and masking operations

* **operator==(v0, v1)** - element-wise equal; the output is a mask filled with ones where the condition is true and zeros where it is false
* **operator!=(v0, v1)** - element-wise inequal; the output is a mask filled with ones where the condition is true and zeros where it is false
* **operator>=(v0, v1)** - element-wise greater or equal; the output is a mask filled with ones where the condition is true and zeros where it is false
* **operator<=(v0, v1)** - element-wise less or equal; the output is a mask filled with ones where the condition is true and zeros where it is false
* **operator>(v0, v1)** - element-wise greatee; the output is a mask filled with ones where the condition is true and zeros where it is false
* **operator<(v0, v1)** - element-wise less; the output is a mask filled with ones where the condition is true and zeros where it is false
* **select(mask, v0, v1)** - element-wise ternary operator: select elements from **v0** where the mask is true and from **v1** where the mask is false