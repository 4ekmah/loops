/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_TESTS_HPP__
#define __LOOPS_TESTS_HPP__
#include "loops/loops.hpp"
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <iomanip>
#include <math.h>
#if __LOOPS_OS == __LOOPS_WINDOWS
#include <Windows.h>
#else 
#include <sys/time.h>
#endif

#include "src/func_impl.hpp"

namespace loops
{

class Test
{
public:
    Test(std::ostream& out, Context& ctx): m_out(&out), CTX(ctx) {}
    virtual void generateCode() = 0;
    virtual bool testExecution(const std::string& fixName) = 0;
    bool testAssembly(const std::string& a_fixtureName, bool a_rewriteIfWrong);
    virtual std::vector<std::string> fixturesNames() const = 0;
    template<typename T>
    inline bool expect_eq(const T& tstd, const T& ref)
    {
        bool res = (tstd == ref);
        if (!res)
            (*m_out)<<"    Failed:"<<tstd<<"!="<<ref<<std::endl;
        return res;
    }
    template<typename T>
    inline bool expect_near(const T& tstd, const T& ref, const T& err)
    {
        bool res = (::abs(tstd - ref) <= err);
        if (!res)
            (*m_out)<<"    Failed: distance between "<<tstd<<" and "<<ref<<" more than "<<err<<"."<<std::endl;
        return res;
        
    }

    static std::string OSname();
#define EXPECT_EQ(a,b) if(!expect_eq((a),(b))) return false;
#define EXPECT_NEAR(a,b,err) if(!expect_near((a),(b),(err))) return false
protected:
    Context CTX;
private:
    std::ostream* m_out;
    bool checkListingEquality(const std::string& curL, const std::string& refL, const std::string& errMes);
};

class TestSuite
{
public:
    static TestSuite* getInstance();
    void run(bool rewriteListings = false);
    template<typename T>
    void regTest()
    {
        std::shared_ptr<T> toAdd = std::make_shared<T>(*m_out, CTX);
        m_testList.push_back(std::static_pointer_cast<Test>(toAdd));
    };
private:
    std::list<std::shared_ptr<Test> > m_testList;
    Context CTX;
    std::ostream* m_out;
    TestSuite(std::ostream& a_out = std::cout);
};
};

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
    char* str()
    {
        sprintf(buf, "min=%.3gms, gmean=%.3gms", min_ms(), gmean_ms());
        return buf;
    }
};

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

//TODO(ch): Interesting solution for test substitution is class derivation
//with using RTTI for taking name of class. Still not really easy to decide what to 
// do with fixtures, but there is a thought to think.
#define LTEST(funcname, ...)                                    \
class funcname: public Test                                     \
{                                                               \
public:                                                         \
    funcname(std::ostream& out, Context& ctx): Test(out,ctx){}  \
    virtual void generateCode() override                        \
    {                                                           \
        std::string TESTNAME = #funcname;                       \
        USE_CONTEXT_(CTX)                                       \
        __VA_ARGS__                                             \
    }                                                           \
    virtual bool testExecution(const std::string& fixName)      \
                                                       override;\
    virtual std::vector<std::string> fixturesNames() const      \
                                                     override   \
    { return std::vector<std::string>(1, #funcname);}           \
};                                                              \
class funcname##_reg                                            \
{                                                               \
public:                                                         \
    funcname##_reg()                                            \
    {                                                           \
        TestSuite::getInstance()->regTest<funcname>();          \
    };                                                          \
};                                                              \
funcname##_reg funcname##_reg_instance

#define LTESTexe(funcname, ...)                                 \
bool funcname::testExecution(const std::string& fixName)        \
{                                                               \
    void* EXEPTR = CTX.getFunc(fixName).ptr();                  \
    __VA_ARGS__                                                 \
    return true;                                                \
}

#define PTEST_1(funcname, _pT1, _p1, ...)                       \
class funcname: public Test                                     \
{                                                               \
public:                                                         \
    funcname(std::ostream& out, Context& ctx): Test(out,ctx) {} \
    static std::vector<std::string> m_fixturesNames;            \
    static std::vector<void (*)(loops::Context)> m_generators;  \
    static std::map<std::string,                                \
                bool (funcname::*)(loops::Context)> m_executors;\
    virtual std::vector<std::string> fixturesNames() const      \
                                                     override   \
    { return m_fixturesNames;}                                  \
    virtual bool testExecution(const std::string& fixName)      \
                                                        override\
    {                                                           \
        return (this->*m_executors[fixName])(CTX);              \
    }                                                           \
    virtual void generateCode() override                        \
    {                                                           \
        for(auto generator : m_generators)                      \
            generator(CTX);                                     \
    }                                                           \
    template<_pT1 _p1>                                          \
    static std::string pName();                                 \
    template<_pT1 _p1>                                          \
    bool testParameterizedExecution(loops::Context CTX);        \
    template<_pT1 _p1>                                          \
    static void genParameterizedCode(loops::Context CTX)        \
    {                                                           \
        std::string TESTNAME = pName<_p1>();                    \
        USE_CONTEXT_(CTX)                                       \
        __VA_ARGS__                                             \
        m_fixturesNames.push_back(TESTNAME);                    \
        m_executors[TESTNAME] =                                 \
                     &funcname::testParameterizedExecution<_p1>;\
    }                                                           \
};                                                              \
std::vector<std::string> funcname::m_fixturesNames;             \
std::vector<void (*)(loops::Context)> funcname::m_generators;   \
std::map<std::string, bool (funcname::*)(loops::Context)>       \
                                          funcname::m_executors;\
class funcname##_reg                                            \
{                                                               \
public:                                                         \
    funcname##_reg()                                            \
    {                                                           \
        TestSuite::getInstance()->regTest<funcname>();          \
    };                                                          \
};                                                              \
funcname##_reg funcname##_reg_instance

#define PTESTexe_1(funcname, _pT1, _p1, ...)                    \
template<_pT1 _p1>                                              \
bool funcname::testParameterizedExecution(loops::Context CTX)   \
{                                                               \
    void* EXEPTR = CTX.getFunc(funcname::pName<_p1>()).ptr();   \
    __VA_ARGS__                                                 \
    return true;                                                \
}

#define PTESTfix_1(funcname, _p1)                               \
template<>                                                      \
std::string funcname::pName<_p1>()                              \
{                                                               \
    return std::string(#funcname) + "_" + #_p1;                 \
}                                                               \
class funcname##_gfix_##_p1                                     \
{                                                               \
public:                                                         \
    funcname##_gfix_##_p1()                                     \
    {                                                           \
        funcname::m_generators.push_back(                       \
              &(funcname::genParameterizedCode<_p1>));          \
    };                                                          \
};                                                              \
funcname##_gfix_##_p1 inst##funcname##_gfix_##_p1


#define PTEST_2(funcname, _pT1, _p1, _pT2, _p2, ...)            \
class funcname: public Test                                     \
{                                                               \
public:                                                         \
    funcname(std::ostream& out, Context& ctx): Test(out,ctx) {} \
    static std::vector<std::string> m_fixturesNames;            \
    static std::vector<void (*)(loops::Context)> m_generators;  \
    static std::map<std::string,                                \
                bool (funcname::*)(loops::Context)> m_executors;\
    virtual std::vector<std::string> fixturesNames() const      \
                                                     override   \
    { return m_fixturesNames;}                                  \
    virtual bool testExecution(const std::string& fixName)      \
                                                        override\
    {                                                           \
        return (this->*m_executors[fixName])(CTX);              \
    }                                                           \
    virtual void generateCode() override                        \
    {                                                           \
        for(auto generator : m_generators)                      \
            generator(CTX);                                     \
    }                                                           \
    template<_pT1 _p1, _pT2 _p2>                                \
    static std::string pName();                                 \
    template<_pT1 _p1, _pT2 _p2>                                \
    bool testParameterizedExecution(loops::Context CTX);        \
    template<_pT1 _p1, _pT2 _p2>                                \
    static void genParameterizedCode(loops::Context CTX)        \
    {                                                           \
        std::string TESTNAME = pName<_p1, _p2>();               \
        USE_CONTEXT_(CTX)                                       \
        __VA_ARGS__                                             \
        m_fixturesNames.push_back(TESTNAME);                    \
        m_executors[TESTNAME] =                                 \
                &funcname::testParameterizedExecution<_p1, _p2>;\
    }                                                           \
};                                                              \
std::vector<std::string> funcname::m_fixturesNames;             \
std::vector<void (*)(loops::Context)> funcname::m_generators;   \
std::map<std::string, bool (funcname::*)(loops::Context)>       \
                                          funcname::m_executors;\
class funcname##_reg                                            \
{                                                               \
public:                                                         \
    funcname##_reg()                                            \
    {                                                           \
        TestSuite::getInstance()->regTest<funcname>();          \
    };                                                          \
};                                                              \
funcname##_reg funcname##_reg_instance

#define PTESTexe_2(funcname, _pT1, _p1, _pT2, _p2, ...)         \
template<_pT1 _p1, _pT2 _p2>                                    \
bool funcname::testParameterizedExecution(loops::Context CTX)   \
{                                                               \
    void* EXEPTR = CTX.getFunc(funcname::pName<_p1, _p2>()).ptr();\
    __VA_ARGS__                                                 \
    return true;                                                \
}

#define PTESTfix_2(funcname, _p1, _p2)                          \
template<>                                                      \
std::string funcname::pName<_p1, _p2>()                         \
{                                                               \
    return std::string(#funcname) + "_" + #_p1 + "_" + #_p2;    \
}                                                               \
class funcname##_gfix_##_p1##_p2                                \
{                                                               \
public:                                                         \
    funcname##_gfix_##_p1##_p2()                                \
    {                                                           \
        funcname::m_generators.push_back(                       \
              &(funcname::genParameterizedCode<_p1, _p2>));     \
    };                                                          \
};                                                              \
funcname##_gfix_##_p1##_p2 inst##funcname##_gfix_##_p1##_p2


#define LTESTcomposer(funcname, ...)                            \
class funcname: public Test                                     \
{                                                               \
public:                                                         \
    funcname(std::ostream& out, Context& ctx): Test(out,ctx) {} \
    virtual void generateCode() override                        \
    {                                                           \
        std::string TESTNAME = #funcname;                       \
        CTX.startFunc(TESTNAME, {});                            \
        __VA_ARGS__                                             \
        loops::Func func = CTX.getFunc(TESTNAME);               \
        getImpl(&func)->directTranslationOn();                  \
        getImpl(&CTX)->endFunc();                               \
    }                                                           \
    virtual bool testExecution(const std::string& fixName)      \
                                                     override   \
    { return true; }                                            \
    virtual std::vector<std::string> fixturesNames() const      \
                                                     override   \
    { return std::vector<std::string>(1, #funcname);}           \
};                                                              \
class funcname##_reg                                            \
{                                                               \
public:                                                         \
    funcname##_reg()                                            \
    {                                                           \
        TestSuite::getInstance()->regTest<funcname>();          \
    };                                                          \
};                                                              \
funcname##_reg funcname##_reg_instance


//Utils

template<typename _Tp>           //DUBUGG: Urgent on Arm. TODO(ch): delete these "utilities" and use regular methodology for load instructions in instruction_set_tests
static inline void load2(const loops::IReg& dest, const loops::IReg& base)
{
    using namespace loops;
    IReg dummy = load_<_Tp>(base);
    FuncImpl* func = static_cast<FuncImpl*>(dummy.func);
    Syntfunc& sfunc = const_cast<Syntfunc&>(func->getData());
    sfunc.program.back()[0].idx = dest.idx;
}

template<typename _Tp>
static inline void load2(const loops::IReg& dest, const loops::IReg& base, const loops::IReg& offset)
{
    using namespace loops;
    IReg dummy = load_<_Tp>(base, offset);
    FuncImpl* func = static_cast<FuncImpl*>(dummy.func);
    Syntfunc& sfunc = const_cast<Syntfunc&>(func->getData());
    sfunc.program.back()[0].idx = dest.idx;
}

template<typename _Tp>
static inline void load2(const loops::IReg& dest, const loops::IReg& base, int64_t offset)
{
    using namespace loops;
    IReg dummy = load_<_Tp>(base, offset);
    FuncImpl* func = static_cast<FuncImpl*>(dummy.func);
    Syntfunc& sfunc = const_cast<Syntfunc&>(func->getData());
    sfunc.program.back()[0].idx = dest.idx;
}

#endif//__LOOPS_TESTS_HPP__
