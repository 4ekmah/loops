/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_TESTS_HPP__
#define __LOOPS_TESTS_HPP__
#include "loops/loops.hpp"
#include <vector>
#include <list>
#include <iostream>
#include "../src/func_impl.hpp"     //TODO(ch): .. in path is bad practice. Configure project

namespace loops
{

class Test
{
public:
    Test(std::ostream& out, Context& ctx): m_out(&out), CTX(ctx) {}
    virtual std::string generateCode() = 0;
    virtual bool testExecution() = 0;
    bool testAssembly(bool a_rewriteIfWrong = false);
    std::string name() const { return m_func.name(); }
    template<typename T>
    inline bool test_eq(const T& tstd, const T& ref)
    {
        bool res = (tstd == ref);
        if (!res)
            (*m_out)<<"    Failed:"<<tstd<<"!="<<ref<<std::endl;
        return res;
    }
#define TEST_EQ(a,b) if(!test_eq((a),(b))) return false;
protected:
    Context CTX;
    Func m_func;
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
    funcname(std::ostream& out, Context& ctx): Test(out,ctx) {} \
    virtual std::string generateCode()                          \
    {                                                           \
        std::string TESTNAME = #funcname;                       \
        __VA_ARGS__                                             \
        m_func = CTX.getFunc(TESTNAME);                         \
        return TESTNAME;                                        \
    }                                                           \
    virtual bool testExecution();                               \
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
bool funcname::testExecution()                                  \
{                                                               \
    void* EXEPTR = m_func.ptr();                                \
    __VA_ARGS__                                                 \
    return true;                                                \
}

#define LTESTcomposer(funcname, ...)                            \
class funcname: public Test                                     \
{                                                               \
public:                                                         \
    funcname(std::ostream& out, Context& ctx): Test(out,ctx) {} \
    virtual std::string generateCode()                          \
    {                                                           \
        std::string TESTNAME = #funcname;                       \
        CTX.startFunc(TESTNAME, {});                            \
        __VA_ARGS__                                             \
        m_func = CTX.getFunc(TESTNAME);                         \
        getImpl(&CTX)->endFunc(true);                           \
        return TESTNAME;                                        \
    }                                                           \
    virtual bool testExecution();                               \
};                                                              \
class funcname##_reg                                            \
{                                                               \
public:                                                         \
    funcname##_reg()                                            \
    {                                                           \
        TestSuite::getInstance()->regTest<funcname>();          \
    };                                                          \
};                                                              \
bool funcname::testExecution()                                  \
{                                                               \
    return true;                                                \
}                                                               \
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
