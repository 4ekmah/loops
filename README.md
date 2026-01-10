# Loops

Loops is a minimalistic, fast, cross-platform C++ JIT engine with register allocation, mainly intended for generating vectorized code.

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

## How to build

### Linux and Macos 

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

## Samples and tests

There are two simple runnable examples:
1. **minmaxloc** - finds the position and value of the maximum element in an array of integers.
2. **bresenham** - an implementation of the Bresenham line drawing algorithm..

Both examples use only scalar instructions. For SIMD examples, see the test code:
1. **testregress** - a large set of regression tests demonstrating different ideas and approaches.
2. **testperf** - see next paragragh.

## Loopslayers

Loopslayers is a small library of generators for neural network kernels.

Currently includes:
1. Depthwise convolution with padding. Strides 1 or 2. No dilations. Activations: none, ReLU, ReLU6, LeakyReLU. AArch64 only.
2. Max pooling with padding. Strides 1 or 2. No dilations. Activations: none, ReLU, ReLU6, LeakyReLU. Works on AArch64, and partially (stride 1 and FP32 only) on Intel.

**testperf** checks kernels supported on the current architecture and measures execution time for several cases used in well-known neural network models.

## How to append the library to your project

The built library can be linked to another project via:
1. Adding the include path <loops_dir>\include
2. Adding the library search path <build_dir>\
3. Linking <build_dir>\libloops.a and, if needed, <build_dir>\libloopslayers.a (or .lib if your system is Windows)

## Manual 

More information about basic concepts, interfaces, usage, and approaches can be found in the reference manual: "docs/Loops_Manual_en.md"