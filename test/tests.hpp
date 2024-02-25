/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_TESTS_HPP__
#define __LOOPS_TESTS_HPP__
#include "loops/loops.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <sstream>
#include <ostream>
#include <iomanip>
#include <math.h>
#if __LOOPS_OS == __LOOPS_WINDOWS
#include <Windows.h>
#else 
#include <sys/time.h>
#endif

#include "src/func_impl.hpp"

//Reference listing archives creation reference:
//
//There is two flags managing process of updating listing and archives:
//
//RECREATE_REFERENCE_TEXTS enable listing files rewriting for files differs
//with listings have been taken on test run(exception if files with tolerable defects: 
//see lower). Also this flag swithched on delete old <arch>_<os>.zip archive and 
//recreates it with updated listings.
//NOTE: all the listing files in test/refasm/listings will be zipped, not just used
//in tests, so be careful with test disabling - it's possible to rewrite something you 
//don't to rewrite.
//
//RECREATE_TOLERABLE_DEFECT. There is a tests with unstable corresponding listings. 
//For now the only case is function calling from loops-generated code. Loops call
//function by hardcoding it's address in registers. Of course, on different test runs
//address will be different and listings will be different. Structure will be the same,
//but constants will changes all the time. For this reason there exist special versions
//of GTEST except macro EXPECT_IR_CORRECT_TOLERABLE_DEFECT and 
//EXPECT_ASSEMBLY_CORRECT_TOLERABLE_DEFECT which consider mistakes in some certain
//tokens on some certain lines as equality. The list of positions in which mistakes 
//are acceptable is prelimiraly created and keeped in test/refasm/listing/defect.bin
//and in corresponding place in archive. So RECREATE_TOLERABLE_DEFECT create this 
//list, and saves it.
//
//So, the algorithm of full regeneration of listing package is:
//1.) Delete test/refasm/<arch>_<os>.zip archive and test/refasm/listings folder/
//2.) Run test suite with RECREATE_REFERENCE_TEXTS=true and
//RECREATE_TOLERABLE_DEFECT=false. It will create listings.
//3.) Run 5 or 10 times test suite with RECREATE_REFERENCE_TEXTS=false and
//RECREATE_TOLERABLE_DEFECT=true. It will collect differences in all 
//possible positions and create defect.bin file.
//4.) Run test suite with RECREATE_REFERENCE_TEXTS=true and
//RECREATE_TOLERABLE_DEFECT=true. It will save defect file into archive.

#define RECREATE_REFERENCE_TEXTS false
#define RECREATE_TOLERABLE_DEFECT false

void switch_spill_stress_test_mode_on(loops::Func& func);
void direct_translation_on(loops::Func& func);

bool intermediate_representation_is_stable(loops::Func func, std::string& errmessage, bool tolerable_defect = false);
bool assembly_is_stable(loops::Func func, std::string& errmessage, bool tolerable_defect = false);
#define EXPECT_IR_CORRECT(func) do { std::string _errmsg_; EXPECT_TRUE(intermediate_representation_is_stable(func, _errmsg_)) << _errmsg_; } while(false)
#define EXPECT_ASSEMBLY_CORRECT(func) do { std::string _errmsg_; EXPECT_TRUE(assembly_is_stable(func, _errmsg_)) << _errmsg_; } while(false)

#define EXPECT_IR_CORRECT_TOLERABLE_DEFECT(func) do { std::string _errmsg_; EXPECT_TRUE(intermediate_representation_is_stable(func, _errmsg_, true)) << _errmsg_; } while(false)
#define EXPECT_ASSEMBLY_CORRECT_TOLERABLE_DEFECT(func) do { std::string _errmsg_; EXPECT_TRUE(assembly_is_stable(func, _errmsg_, true)) << _errmsg_; } while(false)

#define PREPARE_ASSEMBLY_TESTING(testname) loops::Func func = ctx.getFunc(testname); direct_translation_on(func); Func* _f = get_assembly_reg_param(func)
#define DEFINE_ASSEMBLY_REG(name, number) IReg name##_0; name##_0.func = _f; name##_0.idx = number; IExpr name##_1(name##_0); Expr name = name##_1.notype()

void unzip_listings();
void refresh_zip_listings();

template<typename _Tp> struct typestring {};

template<> struct typestring<int32_t> { static inline std::string get() { return "int32_t"; } };
template<> struct typestring<uint32_t> { static inline std::string get() { return "uint32_t"; } };
template<> struct typestring<float> { static inline std::string get() { return "float"; } };
template<> struct typestring<double> { static inline std::string get() { return "double"; } };

bool memok(uint8_t* canvas, int64_t w, int64_t h);
//WARNING: test ostream is not thread-safe
std::ostream& get_test_ostream();
void reset_test_ostream();
std::string get_test_ostream_result();

loops::Func* get_assembly_reg_param(loops::Func& func);

template<class T>
void print_channel(T* data, int H, int W)
{
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
            std::cout << std::setw(6) << data[i*W+j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template<class T>
void print_channel(T* data, int H, int W, int stride) 
{
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
            std::cout << std::setw(6) << data[i*stride+j];
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

template<typename _Tp>
loops::Expr iregtyped(const loops::Expr& reg)
{
    Assert(reg.opcode() == loops::EXPR_LEAF && reg.leaf().tag == loops::Arg::IREG);
    loops::IReg ar; ar.func = reg.func(); ar.idx = reg.leaf().idx;
    loops::IExpr ret(ar);
    ret.type() = loops::ElemTraits<_Tp>::depth;
    return ret.notype();
}

template<typename _Tp>
loops::Expr immtyped(int64_t val, loops::Func* func)
{
    loops::Expr ret(val);
    ret.func() = func;
    ret.type() = loops::ElemTraits<_Tp>::depth;
    return ret;
}

struct Timer
{
    Timer() { reset(); }
    void reset() {
        min_time = gmean_time = timestamp = 0; niters = 0;
    }
    void start() {
        timestamp = ticks();
    }
    void stop() {
        double t = ticks() - timestamp;
        min_time = niters == 0 || t < min_time ? t : min_time;
        gmean_time += ::log(t);
        niters++;
    }
    double gmean_ms() const {
        return ::exp(gmean_time/(niters > 0 ? niters : 1));
    }
    double min_ms() const { return min_time; }
    
    double min_time, gmean_time, timestamp;
    char buf[128];
    int niters;
    
    static double ticks()
    {
    #if __LOOPS_OS == __LOOPS_WINDOWS
        LARGE_INTEGER freq;
        LARGE_INTEGER pc;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&pc);

        return pc.QuadPart * 1000.0 / freq.QuadPart;
    #else 
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000.0 + tv.tv_usec / 1000.0;
    #endif
    }
    std::string str()
    {
        std::stringstream outstr;
        outstr << "min=" << std::setprecision(3) << min_ms() << " ms, gmean=" << std::setprecision(3) << gmean_ms() << " ms" << std::endl;
        return outstr.str();
    }
};

#endif//__LOOPS_TESTS_HPP__
