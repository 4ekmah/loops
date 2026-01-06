# Loops

Loops is minimalistic, fast, cross-platform C++ JIT engine with register allocation, mostly for generating vectorized code.

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


## How to build

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

## Samples and tests

There is two runnable examples:
1. minmaxloc - get position and value of minimal element in array of integers.  
2. bresenham - implements line drawing algorithm by Bresenham.

Both examples uses only scalar instructions, so if you need SIMD examples, look in tests:  
1. testregress - big set of regression tests with different approaches and ideas.
2. testperf - look next paragragh. 

## Loopslayers

Loopslayers is small library of generators for neural network kernels.

Currently has:
1. Depthwise convolution with padding. Strides 1 or 2. No dilations. Activatons: none, ReLU, ReLU6, LeakyReLU. Only for Aarch64.
2. Maxpooling with padding. Strides 1 or 2. No dilations. Activatons: none, ReLU, ReLU6, LeakyReLU. Works on Aarch64, partially(stride 1 and fp32 only) on Intel.

testperf checks kernels, supported on current architecture and measure time of executions for some certain cases used in some well-known neural models.

## How to append library to your project

Library built can be linked to other project via:
1. Appending include path <loops_dir>\include
2. Appending link library path <build_dir>\
3. Linking <build_dir>\libloops.a and, if needed <build_dir>\libloopslayers.a. (Or .lib, if your system is Windows)


## Manual 

Look more information about basic concepts, interfaces, usage and approaches in reference manual "docs/Loops_Manual_en.md" //DUBUG: rename to "doc/" != "docs/"