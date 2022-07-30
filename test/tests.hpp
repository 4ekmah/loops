/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_TESTS_HPP__
#define __LOOPS_TESTS_HPP__
#include "loops/loops.hpp"
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include "../src/func_impl.hpp"     //TODO(ch): .. in path is bad practice. Configure project

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
    inline bool test_eq(const T& tstd, const T& ref)
    {
        bool res = (tstd == ref);
        if (!res)
            (*m_out)<<"    Failed:"<<tstd<<"!="<<ref<<std::endl;
        return res;
    }
    static std::string OSname();
#define TEST_EQ(a,b) if(!test_eq((a),(b))) return false;
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
    {                                                           \
        return (this->*m_executors[fixName])(CTX);              \
    }                                                           \
    virtual void generateCode()                                 \
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
    {                                                           \
        return (this->*m_executors[fixName])(CTX);              \
    }                                                           \
    virtual void generateCode()                                 \
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
        getImpl(&func)->setDirectTranslation(true);             \
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

template<typename _Tp>
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
