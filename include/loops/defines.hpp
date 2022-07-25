/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPSDEFS_HPP__
#define __LOOPSDEFS_HPP__

#define __LOOPS_AARCH64 1
#define __LOOPS_INTEL64 2

#define __LOOPS_WINDOWS 1
#define __LOOPS_LINUX   2
#define __LOOPS_MAC     3

#if defined(__linux__)
    #define __LOOPS_OS   __LOOPS_LINUX
#elif defined(__APPLE__)
    #define __LOOPS_OS   __LOOPS_MAC
#elif defined(_WIN64)
    #define __LOOPS_OS   __LOOPS_WINDOWS
#else
    #error Unknown OS
#endif

#ifdef __GNUC__
    #if defined(__x86_64__)
        #define __LOOPS_ARCH   __LOOPS_INTEL64
    #elif defined(__aarch64__)
        #define __LOOPS_ARCH   __LOOPS_AARCH64
    #else
        #error Unknown CPU
    #endif
#elif _MSC_VER
    #if defined(_M_X64)
        #define __LOOPS_ARCH   __LOOPS_INTEL64
    #elif defined(_M_ARM64)
        #define __LOOPS_ARCH   __LOOPS_AARCH64
    #else
        #error Unknown CPU
    #endif
#else
    #error Unknown compiler
#endif

#endif //__LOOPSDEFS_HPP__
