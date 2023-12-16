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

#define RECREATE_REFERENCE_TEXTS false
namespace loops
{
bool intermediate_representation_is_stable(Func func, std::string& errmessage);
bool assembly_is_stable(Func func, std::string& errmessage);
void switch_spill_stress_test_mode_on(Func& func); //DUBUG: And something similar for arguments amount control.
void direct_translation_on(Func& func);
Func* get_assembly_reg_param(Func& func);

//DUBUG: rename ASSERT -> EXPECT there.
#define ASSERT_IR_CORRECT(func) do { std::string _errmsg_; EXPECT_TRUE(intermediate_representation_is_stable(func, _errmsg_)) << _errmsg_; } while(false)
#define ASSERT_ASSEMBLY_CORRECT(func) do { std::string _errmsg_; EXPECT_TRUE(assembly_is_stable(func, _errmsg_)) << _errmsg_; } while(false)

#define PREPARE_ASSEMBLY_TESTING(testname) loops::Func func = ctx.getFunc(testname); direct_translation_on(func); Func* _f = get_assembly_reg_param(func)
#define DEFINE_ASSEMBLY_REG(name, number) IReg name##_0; name##_0.func = _f; name##_0.idx = number; IExpr name##_1(name##_0); Expr name = name##_1.notype()

class Test
{
public:
    Test(std::ostream& out, Context& ctx): CTX(ctx), m_out(&out) {}
    virtual ~Test() {}
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

#define EXPECT_EQ(a,b) if(!expect_eq((a),(b))) return false;
protected:
    Context CTX;
private:
    std::ostream* m_out;
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
    }
private:
    std::list<std::shared_ptr<Test> > m_testList;
    Context CTX;
    std::ostream* m_out;
    TestSuite(std::ostream& a_out = std::cout);
};
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

bool memok(uint8_t* canvas, int64_t w, int64_t h);

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

//WARNING: test ostream is not thread-safe
std::ostream& get_test_ostream();
void reset_test_ostream();
std::string get_test_ostream_result();

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

#endif//__LOOPS_TESTS_HPP__
