/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#ifndef __LOOPS_RUNTIME_HPP__
#define __LOOPS_RUNTIME_HPP__

#include "loops/defines.hpp"
#include <stdexcept>
#include <exception>
#include <string>

#define LOOPS_ASSERT_LINE_(x) #x
#define LOOPS_ASSERT_LINE(x) LOOPS_ASSERT_LINE_(x)

#undef LOOPS_ASSERT
#define LOOPS_ASSERT(expr) \
    if (expr)        \
        ;            \
    else             \
        throw loops::exception("Assertion '" #expr "' failed at " __FILE__ ":" LOOPS_ASSERT_LINE(__LINE__))
#define LOOPS_ASSERT_MSG(expr, msg) \
    if (expr)                \
        ;                    \
    else                     \
        throw loops::exception(msg)

namespace loops
{
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
    LOOPS_ERR_INCORRECT_LANE_INDEX,
    LOOPS_ERR_TOO_MUCH_ARGS,
    LOOPS_MEMORY_ALLOCATION_FAILURE,
    LOOPS_MEMORY_PROTECTION_FAILURE,
    LOOPS_SYN_T_NON_EXISTENT_ARG,
};
};
#endif //__LOOPS_RUNTIME_HPP__
