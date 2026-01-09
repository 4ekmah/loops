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
**IReg** репрезентирует скалярный регистр или, в более общем смысле, переменную. В основном, работа с ними происходит, как как с обычными переменными: над ними можно производить арифметические действия, их можно сравнивать, и производить множество других действий. IReg хранит в себе информацию о той функции, к которой он приписан. Это необходимо для того, чтобы при метапрограммировании инструкции попадали в буфер создающейся функции. Однако, когда объект IReg конструируется без помощи других IReg, взять информацию о функции приписки неоткуда, поэтому конструирование обычно происходит с помощью макроса **CONST_**. Рассмотрим пример:
```cpp
    loops::IReg a = CONST_(1); 
    loops::IReg b = CONST_(2); 
    loops::IReg c = a + b; 
    loops::IReg d = с + 2; 
```
Для переменных **a** и **b** приходится использовать макрос CONST_ чтобы передать им информацию о текущей функции, в буфер которой надо добавлять инструкции, а для **с** и **d** эту информацию loops выведет из выражений справа.
<br/><br/>
В случае, если нет необходимости сразу задавать какое то значение, можно воспользоваться макросом **DEF_**(**VDEF_(type)** для **VReg**).
**IReg**, как и **VReg** должен конструироваться только внутри блока **STARTFUNC_**. Единственное исключение - это создание аргументов самой создаваемой функции:
```cpp
    USE_CONTEXT_(CTX);
    loops::IReg ptr, n, minpos_addr, maxpos_addr;
    STARTFUNC_("minmaxloc", &ptr, &n, &minpos_addr, &maxpos_addr)
    {
        //... 
    }
```
Вызывать из **STARTFUNC_** генерирующие функции тоже, конечно, можно, но там необходимо инициализировать контекст для правильной работы макросов:
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
У **IReg** формально нет типа, но обычно подразумевается, что это int64_t.
<br/><br/>
Первое исключение - это операции с памятью - загрузки и выгрузки. Стоит учитывать, что **IReg** располагается на регистрах(примерно, как переменные с ключевым словом **register** в старом Си) и память для этих переменных - внешняя среда. Следовательно, загрузка/выгрузка из памяти должна быть ручной. loops позволяет грузить в регистры данные разных типов и размеров. При загрузке целых, верхняя часть будет дополнена нулями или битом знака, в зависимости от типа. Список типов такой же, как список доступных типов **VReg** ниже. В качестве примера рассмотрим преобразование элементов массива из **int32_t** в **int64_t**:
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
Обратите внимание на адресную арифметику. Она везде в loops измеряется в байтах, а не в размерах типов.
<br/>
Второе исключение - это беззнаковые операции сравнения, в которых IReg воспринимается, как uint64_t. Список таких команд:
* **ule** - меньше или равно
* **uge** - больше или равно
* **ugt** - больше
* **ult** - меньше
####
**VReg<_Tp>** репрезентирует векторные регистры и соотвествует 256-битным **ymm** на Intel AVX2, либо 128-битным **q**-регистрам Arm NEON. На Risc-V поддержки векторных регистров пока нет.  
<br/>
В отличии от **IReg**, **VReg** типизированны, благодаря чему для всех совершаемых с ними арифметических и иных операций автоматически подбираются соответсвующие инструкции.
<br/><br/>
Список поддерживемых типов:
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
Для инициализации используются **VCONST_(type, val)**, либо **VDEF(type)**.
<br/>
В качестве примера рассмотрим векторизованное поэлементное применение функции **exp(x+y)** к двум **float** массивам в предположении кратности размера этих массивов количеству элементов в векторном регистре:
```cpp
    USE_CONTEXT_(CTX);
    IReg xptr, yptr, outptr, size;
    STARTFUNC_("transform", &xptr, &yptr, &outptr, &size)
    {
        loops::IReg i = CONST_(0);
        WHILE_(i < size)
        {
            loops::VReg<float> x_in = loops::loadvec<float>(xptr, i * sizeof(float));
            loops::VReg<float> y_in = loops::loadvec<float>(yptr, i * sizeof(float));
            loops::VReg<float> res = exp(x_in + y_in);
            loops::storevec(outptr, i * sizeof(float), res);
            i = i + CTX.vlanes<float>();
        }
    }
```
Обратите внимание, что шаг по массиву делается на количество lane'ов, которое платформо-независимо добывается с помощью конструкции **CTX.vlanes<float>()**.
<br/><br/>
Отдельного упоминания стоят маски сравнений. Это распространенная техника при работе с векторными регистрами, когда вам нужно сделать какие-то действия с элементами, в зависимости от условий. Вместо того, чтобы извлекать элементы векторного регистра по одному и работать с ними по отдельности, с помощью условных операторов генерируются маски, которые представляют собой векторный регистр, где в каждый элемент результата сравнения, где условие было верно - битовое поле, заполненное единицами, и нулями - где оно было неверно. Дальше достаточно только применить эту маску к протестированному регистру и все ненужные элементы занулятся. Здесь на этом акцентируется внимание, потому что маски в loops - по определению целового беззнакового типа, а значит для их примениния нужна операция **reinterpret**. Рассмотрим в качестве примера получения суммы всех положительных целых чисел в массиве **int32_t**.
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
Для более сложных случаев предусмотрен векторный аналог тернарного оператора, функция **select**. Полный список векторных операций смотрите далее в документе. 
#### Отложенная или массовая инициализация регистров
В принципе, регистры должны инициализироваться сразу, как появляются в метапрограмме, но это не всегда возможно, например, когда если есть необходимость объявить целый контейнер регистров, что может осложняться тем, что неизвестно, сколько их будет надо и все ли они будут вообще нужны. Для этой цели используется отложенная инициализация. К сожалению, на операцию присваивания или конструктор копирования в loops завязан другой функционал, благодаря которому меатпрограммирование хорошо маскируется под обычное программирование и присваивание значит, что в генерируемой программе нужно присвоить что-то регистру, поэтому для отложенной инициализации используется метод **copyidx**. При этом создаётся пустышка IReg или VReg, а потом, в зависимости от ситуации, ей либо назначается, либо не назначается настоящий номер регистра. В моей практике встречалась ситуация, в которой в зависимости от параметров была нужна, либо не нужна маска: 
```cpp
    loops::VReg<int32_t> x;
    //...
    loops::VReg<uint32_t> mask;
    if(horizontal_padding)
         mask.copyidx((x - VCONST_(int32_t, padding_size)) > VCONST_(int32_t, 0));
```
Аргументом **copyidx** является выражение, которое к моменту копирования индекса регистра его уже приобретёт и этот индекс инициализирует переменную **mask**, привязав её к функции. 
<br/><br/>
Инициализация массивов регистров:
```cpp
    std::vector<loops::IReg> reg_arr;
    for(int i = 0; i < (int)reg_arr.size(); i++)
        reg.copyidx(CONST_(i)); 
```
## IExpr, VExpr. Как писать свои генерирующие функции
Если вы начнёте внимательно изучать интерфейс loops, вы обнаружите, что большинство объявленных операций работают не c **IReg/VReg**, а с **IExpr/VExpr**. Это специальные классы, которые хранят в себе дерево выражений вплоть до присваивания его какому-либо регистру или использования в качестве условия в **IF_**, **ELIF_** или **WHILE_**. Присваивания и управлющие конструкции представляют из себя некие точки синхронизации, гарантированно добавляющие код буфер генерируемой функции. В большинстве случаев, пользователю нет необходимости взаимодействовать с этими классами, но есть важное исключение - ситуации, когда вы хотите создать небольшой генератор, который вы будете многократно переиспользовать, например, свою версию вычислительной функции вроде **sin**, **pow** и.т.п. В качестве примера приведём операцию умного умножения на константу, которая автоматически заменяет умножения на степень двойки сдвигом:
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
В целом, это более правильный подход при написании подгенераторов, чем работа с **IReg/VReg**, как в примере с функцией **pow** в разделе про **IReg/VReg**, однако, есть ограничения. В таких генераторах не должно быть присваиваний **IReg**, **IF_**, **ELIF_** или **WHILE_**, потому что при этом произойдёт добавление кода в буфер и весь смысл **IExpr/VExpr** будет утерян.
<br/><br/>
Как следствие, **loops::Context** и **USE_CONTEXT_** в таком коде вам может понадобиться, только если вы создаёте константы. На этот случай сущестует специальная функция **ExtractContext**, которая получает контекст из выражения. Стандартное её применение выглядит так:
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
**VExpr**, также как и **VReg** параметризуется типом.
<br/>
## Как добавить инструкцию
На данный момент не существует специализрованного интерфейса для добавления кастомных инструкций, но если вам необходимо добавить что-то из репертуара какой-то конкретной архитектуры, можно поменять исходный код библиотеки. 
<br/><br/>
В общем случае, это непростая процедура, но в _подавляющем большинстве_ случаев, когда инструкция имеет один выходной регистр, несколько входных, энкодинг сводится к простым полям и нет никаких ограничений на распределение регистров это действительно делается в несколько простых шагов. 
1. Вам надо добавить IR-версию инструкции в большой *enum* с инструкциями, в заголовочном файле loops.hpp, желательно с каким-нибудь отдельным диапазоном, чтобы не вступать в конфликт с обновлениями loops:
```cpp
    OP_MYOP            = 1000,
``` 
2. Присвоить строку IR-инструкции в файле printer.cpp в большой карте имён opstrings_.
```cpp
    LOOPS_HASHMAP_ELEM(loops::OP_MYOP             , "myop"                  ) ,
```
3. Добавить Assembly-версию в большой *enum* с инструкциями, в заголовочном файле целевой платформы(backend_aarch64.hpp, backend_intel64.hpp, backend_riscV.hpp). Пусть, для примера, речь идёт про ARM:
```cpp
    AARCH64_MYOP       = 164
``` 
4. Присвоить строку IR-инструкции в функции opstrings_getter_ в исходном файле целевой платформы(backend_aarch64.cpp, backend_intel64.cpp, backend_riscV.cpp).
```cpp
    case (loops::AARCH64_MYOP        ) : return "myop"        ;
```
5. В исходном файле целевой платформы добавить правило преобразования инструкции из IR в ассемблерную. На Intel это функция i64STLookup, на Arm это функция a64STLookup, на Risc-V это функция r64STLookup. Положим, наша инструкция имеет один векторный выход, один векторный и один скалярный вход. В правиле SyT указывается тип целевой ассемблерной инструкции и список аргументов. **SAcop** просто копирует аргументы, но есть и другие описатели аргументов, например **SAimm** порождает _immediate_ аргумент. Наше правило с проверками корректности будет выглядеть примерно так:
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
6. В исходном файле целевой платформы добавить _encoding_ ассемблерной инструкции. На Intel это функция i64BTLookup, на Arm это функция a64BTLookup, на Risc-V это функция r64BTLookup. Также как и в пятом пункте, сначала проводится проверка корректности аргументов, а затем создаётся _encoding_-правило. Оно состоит из деталей, которые превращаются в битовые поля, либо из тела аргументов, либо константные. **BTreg(n, f, fl)** использует **f** бит **n**-го аргумента, проверяя, что это _регистр_, **BTimm(n, f, fl)** использует **f** бит **n**-го аргумента, проверяя, что это _immediate_, **BTsta(s, f)** использует **f** бит входного неизменяемого поля **s**. Очень важно расставить также флаги **fl**, которые указывают компилятору, какие аргументы входные, а какие выходные, это необходимо для работы _аллокатора регистров_. Наше правило с проверками корректности будет выглядеть примерно так:
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
7. Чтобы всё заработало, остаётся только добавить функцию интерфейса в файл loops.hpp, которая позволит вам использоввать вашу инструкцию в работе. В нашем прмиере она будет выглядеть примерно так: 
```cpp
template<typename _Tp>
VExpr<_Tp> myop(const VExpr<_Tp>& a, const IExpr& b)
{ 
    return VExpr<_Tp>(OP_MYOP, {r.notype(), b.notype()});
}
```
## Список векторных операций
В этом разделе приведены все векторные операции, доступные программситу на loops. Все эти операции гарантированно работают на Arm и почти все на Intel. Если операция не работает на Intel, это должно быть указано, но советую проверять.

### Операции с памятью
* **loadvec(ptr, offset)/loadvec(ptr)** - загрузить вектор из памяти по адресу **ptr + offset**  
* **loadlane(ptr, lane_index)** - загрузить в элемент номер **lane_index** вектора из указанного по адресу **ptr** участка памяти. Только Arm
* **loadvec_deinterleave2(res1, res2, ptr)** - загрузить с инициализацией два вектора - все четные элементы(с 0) заполнят **res1**, все нечетные - **res2**, адрес источника - **ptr**. Только Arm
* **storevec(ptr, offset, v)/storevec(ptr, v)** - сохранить вектор **v** в память по адресу **ptr + offset**
* **storelane(ptr, v, lane_index)** - сохранить элемент номер **lane_index** из вектора **v** в память по адресу **ptr**. Только Arm

### Преобразования типов
* **cast(v)** - поэлементное преобразование типа
* **reinterpret(v)** - замена типа вектора, без преобразования. Операция не порождает никаких реальных действий 
* **trunc(v)**  - преобразование к целому(знаковому или беззнаковому) с округлением в сторону нуля. Поддерживаемые типы данных - **float16_t**, **float**, **double**
* **floor(v)**  - преобразование к целому(знаковому или беззнаковому) с округлением в сторону минус бесконечности.
* **broadcast(s)/broadcast(v, lane_index)** - заполнение векторного регистра значением из скалярного регистра **s** или элемента **lane_index** векторного регистра **v**. Второй вариант работает только на Arm.
* **cast_low(v)** - преобразование, увеличивающее размер элемента вдвое, например, **int16_t** в **int32_t**. В целевом регистре уложится только половина значений, данная инструкция порождает нижнюю половину
* **cast_high(v)** - преобразование, увеличивающее размер элемента вдвое, например, **int16_t** в **int32_t**. В целевом регистре уложится только половина значений, данная инструкция порождает верхнюю половину
* **shrink(v0, v1)** - преобразование, уменьшающее размер элемента вдвое, например, **int32_t** в **int16_t**. В целевом регистре уложится вдвое больше значений, поэтому требуется два входных регистра. Только Arm

### Манипуляции с элементами

* **getlane(v, lane_index)** - извлекает в скалярный регистр элемент номер **lane_index** из вектора **v**
* **setlane(v, lane_index, s)/setlane(v, lane_index_v, vi, lane_index_vi)** - устанавливает элемент номер **lane_index_v** из вектора **v** значение, хранящимся либо в скалярном регистре **s**, либо в элементе номер **lane_index_vi** из вектора **vi**. Вторая вариация работает только на Arm
* **ext(v0, v1, lane_index)** - склейка двух векторов и извлечение из неё подвектора начиная с элемента **lane_index** 

### Арифметические операции
* **operator+(v0, v1)** - поэлементное сложение
* **operator-(v0, v1)** - поэлементное вычитание
* **operator*(v0, v1)** - поэлементное умножение
* **operator/(v0, v1)** - поэлементное деление
* **operator-(v0)** - поэлементный унарный минус
* **fma(v0,v1,v2)/fma(v0,v1,v2,lane_index)** - поэлементное **v0+v1*v2** либо **v0+v1*v2[lane_index]**. Вторая вариация работает только на Arm
* **pow(v,p)** - поэлементное возведение в степень **p**
* **expInit(CTX)** - инициализация констант, необходимых для быстрой поэлементной экспоненты
* **exp(v, expt)** - быстрая поэлементная экспонента
* **max(v0, v1)** - поэлементный максимум
* **min(v0, v1)** - поэлементный минимум

* **reduce_max(v)** - укладывает максимальный элемент вектора в нулевой элемент целевого вектора. Только Arm
* **reduce_min(v)** - укладывает минимальный элемент вектора в нулевой элемент целевого вектора. Только Arm
* **reduce_sum(v)** - укладывает сумму элементов вектора в нулевой элемент целевого вектора 
* **reduce_wsum(v)** - укладывает сумму элементов вектора в нулевой элемент целевого вектора c элементами большего размера. Позволяет бороться с переполнением

### Побитовые операции

* **operator>>(v, s)** - поэлементный битовый сдвиг вправо
* **ushift_right(v, s)** - поэлементный беззнаковый битовый сдвиг вправо
* **operator<<(v, s)** - поэлементный битовый сдвиг влево
* **ushift_left(v, s)** - поэлементный беззнаковый битовый сдвиг влево
* **ushift_left(v, s)** - поэлементный беззнаковый битовый сдвиг влево
* **ushift_left(v, s)** - поэлементный беззнаковый битовый сдвиг влево
* **ushift_left(v, s)** - поэлементный беззнаковый битовый сдвиг влево

* **operator&(v0, v1)** - побитовое И
* **operator|(v0, v1)** - побитовое ИЛИ
* **operator^(v0, v1)** - побитовое ИСКЛЮЧАЮЩЕЕ ИЛИ 
* **operator~(v)** - побитовое НЕ
* **popcount(v)** - побайтовый подсчёт ненулевых битов

### Операции сравнения и маскирования

* **operator==(v0, v1)** - поэлементное равенство, выходной вектор - маска, заполненная единицами для тех элементов, где условие верно и нулями - где неверно
* **operator!=(v0, v1)** - поэлементное неравенство, выходной вектор - маска, заполненная единицами для тех элементов, где условие верно и нулями - где неверно
* **operator>=(v0, v1)** - поэлементное больше или равно, выходной вектор - маска, заполненная единицами для тех элементов, где условие верно и нулями - где неверно
* **operator<=(v0, v1)** - поэлементное меньше или равно, выходной вектор - маска, заполненная единицами для тех элементов, где условие верно и нулями - где неверно
* **operator>(v0, v1)** - поэлементное больше, выходной вектор - маска, заполненная единицами для тех элементов, где условие верно и нулями - где неверно
* **operator<(v0, v1)** - поэлементное меньше, выходной вектор - маска, заполненная единицами для тех элементов, где условие верно и нулями - где неверно
* **select(mask, v0, v1)** - поэлементный тернарный оператор, выбирающий по маске элементы векторов из **v0** для верных элементов и из **v1** для ложных элементов