/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPSDEFS_HPP__
#define __LOOPSDEFS_HPP__

#define __LOOPS_AARCH64 1
#define __LOOPS_INTEL64 2
#define __LOOPS_RISCV   3

#define __LOOPS_WINDOWS 1
#define __LOOPS_LINUX   2
#define __LOOPS_MAC     3

#define __LOOPS_C       1
#define __LOOPS_CPP     2

#if defined(__linux__)
    #define __LOOPS_OS   __LOOPS_LINUX
#elif defined(__APPLE__)
    #define __LOOPS_OS   __LOOPS_MAC
#elif defined(_WIN64)
    #define __LOOPS_OS   __LOOPS_WINDOWS
#else
    #error Unknown OS
#endif

// #ifdef __GNUC__   //DUBUG: uncomment!
//     #if defined(__x86_64__)
//         #define __LOOPS_ARCH   __LOOPS_INTEL64
//     #elif defined(__aarch64__)
//         #define __LOOPS_ARCH   __LOOPS_AARCH64
//     #elif defined(__riscv)
//         #define __LOOPS_ARCH   __LOOPS_RISCV
//     #else
//         #error Unknown CPU
//     #endif
// #elif _MSC_VER
//     #if defined(_M_X64)
//         #define __LOOPS_ARCH   __LOOPS_INTEL64
//     #elif defined(_M_ARM64)
//         #define __LOOPS_ARCH   __LOOPS_AARCH64
//     #else
//         #error Unknown CPU
//     #endif
// #else
//     #error Unknown compiler
// #endif

#define __LOOPS_ARCH   __LOOPS_RISCV

#if defined(__cplusplus)
#define __LOOPS_LANGUAGE  __LOOPS_CPP
#else
#define __LOOPS_LANGUAGE  __LOOPS_C
#endif


#endif //__LOOPSDEFS_HPP__
