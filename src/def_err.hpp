/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#ifndef __LOOPS_DEF_ERR_HPP__
#define __LOOPS_DEF_ERR_HPP__

#include "loops/defines.hpp"
#if __LOOPS_LANGUAGE  == __LOOPS_CPP
#include <stdexcept>
#include <exception>
#include <string>
#endif

enum 
{
    LOOPS_ERR_SUCCESS = 0, 
    LOOPS_ERR_NULL_POINTER,
    LOOPS_ERR_POINTER_ARITHMETIC_ERROR,
    LOOPS_ERR_OUT_OF_MEMORY,
    LOOPS_ERR_UNKNOWN_FLAG,
    LOOPS_ERR_POSITIVE_SIZE_NEEDED,
    LOOPS_ERR_UNIMAGINARY_BIG_STRING,
    LOOPS_ERR_UNPRINTABLE_OPERATION,
    LOOPS_ERR_UNKNOWN_TYPE,
    LOOPS_ERR_UNKNOWN_CONDITION,
    LOOPS_ERR_INCORRECT_OPERATION_FORMAT,
    LOOPS_ERR_INCORRECT_ARGUMENT, 
    LOOPS_ERR_UNKNOWN_ARGUMENT_TYPE,
    LOOPS_ERR_INTERNAL_UNKNOWN_PRINT_DESTINATION,
    LOOPS_ERR_INTERNAL_BUFFER_SIZE_MISCALCULATION,
    LOOPS_ERR_INTERNAL_INCORRECT_OFFSET,
};

#define LOOPS_ASSERT_LINE_(x) #x
#define LOOPS_ASSERT_LINE(x) LOOPS_ASSERT_LINE_(x)

#undef Assert
#define Assert(expr) \
    if (expr)        \
        ;            \
    else             \
        throw std::runtime_error("Assertion '" #expr "' failed at " __FILE__ ":" LOOPS_ASSERT_LINE(__LINE__))
#define AssertMsg(expr, msg) \
    if (expr)                \
        ;                    \
    else                     \
        throw std::runtime_error(msg)

char* get_errstring(int errid);
#if 0 //__LOOPS_LANGUAGE  == __LOOPS_CPP //DUBUG: uncomment
class loops_exception : public std::exception
{
private:
    int errid;
public:
    loops_exception(int a_errid) : std::exception(), errid(a_errid) {}
    virtual char* what() { return get_errstring(errid); };
};
#define LOOPS_THROW(x) throw loops_exception(x);
#else
#define LOOPS_THROW(x) do { return (x); } while(0)
#endif

/*Macro for calling functions, which are able to throw exceptions. Rethrow them even if there is no exception support in language.*/
#define LOOPS_CALL_THROW(x) do { int __loops_err__ = x; if(__loops_err__ != LOOPS_ERR_SUCCESS) LOOPS_THROW(__loops_err__); } while(0)

#endif //__LOOPS_DEF_ERR_HPP__
