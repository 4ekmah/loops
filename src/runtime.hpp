/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#ifndef __LOOPS_RUNTIME_HPP__
#define __LOOPS_RUNTIME_HPP__

#include "loops/defines.hpp"
#if __LOOPS_LANGUAGE  == __LOOPS_CPP
#include <stdexcept>
#include <exception>
#include <string>
#endif
/*
TODO[CPP2ANSIC]: Since core of project will be rewrtitten on C89, we need many things from C++, but rewritten on C.
Example: collections, based on uthash, are already written in collections.hpp file.

Most critical thing are exception handling and automatic objects destruction on stack unwinding or function return.
To implement this, we need next details: 
1.) Universal destructor interface(loops_destructor). Among other things, this destructor interface have to be used 
for destruction of collection's elements.
2.) Structures keeping list of destructors, macroses for function intialization(or basic block[have to be checked, that 
C89 suuport basic block initialization]), and function deinitilization(calling destructors for all currently existing 
objects).
3.) Macro for constucting such "stack" objects, which emplace it with destructors to the list.
4.) All LOOPS_CALL_THROW / LOOPS_THROW, Assert macro have to be rewritten to call destructor list.
5.) There needed Assert macro to be replaced to work in system LOOPS_CALL_THROW.
6.) I think, as much code as possible, have to be written without __LOOPS_LANGUAGE divarication. Of course, loops have 
to build in C++ environment, but we don't need substitute basic infrastructure when we are moving to this environment.
E.g., exceptions definition have to be universal and have to be C-like. Same for module initialization. 
*/
typedef void (*loops_destructor)(void* obj);

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
    LOOPS_ERR_ELEMENT_NOT_FOUND,
};

 //TODO[CPP2ANSIC]: All "Assert" entries have to be replaced with C-version of assert, returning exception via return code.

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

const char* get_errstring(int errid);
#if __LOOPS_LANGUAGE  == __LOOPS_CPP
class loops_exception : public std::exception
{
private:
    int errid;
public:
    loops_exception(int a_errid) : std::exception(), errid(a_errid) {}
    virtual const char* what() { return get_errstring(errid); };
};
#define LOOPS_THROW(x) throw loops_exception(x)
#else
#define LOOPS_THROW(x) do { return (x); } while(0)
#endif

/*Macro for calling functions, which are able to throw exceptions. Rethrow them even if there is no exception support in language.*/
#define LOOPS_CALL_THROW(x) do { int __loops_err__ = x; if(__loops_err__ != LOOPS_ERR_SUCCESS) LOOPS_THROW(__loops_err__); } while(0)

#endif //__LOOPS_RUNTIME_HPP__
