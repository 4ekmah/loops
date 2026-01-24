/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_COLLECTIONS_HPP__
#define __LOOPS_COLLECTIONS_HPP__
#include "runtime.hpp"
#include <utlist.h>
typedef const char* loops_cstring;

// TODO[CPP2ANSIC]: pure C, even newest standarts don't have function overriding, so 'class methods' cannot have same names.
// ===================================================== LIST =====================================================

#define LOOPS_LIST(T) loops_list_ ##T

#define LOOPS_LIST_DECLARE(T)                                                                  \
typedef struct loops_list_inner_ ## T                                                          \
{                                                                                              \
    T val;                                                                                     \
    struct loops_list_inner_ ## T* next;                                                       \
} loops_list_inner_ ## T;                                                                      \
typedef struct loops_list_ ## T ## _                                                           \
{                                                                                              \
    loops_list_inner_ ## T* impl;                                                              \
} loops_list_ ## T ## _;                                                                       \
typedef struct loops_list_ ## T ## _* loops_list_ ##T;                                         \
int loops_list_construct(loops_list_ ## T* result);                                            \
void loops_list_destruct(loops_list_ ## T to_del, loops_destructor element_destructor = NULL); \
int loops_list_push_back(loops_list_ ## T ll, T val);                                          \
int loops_list_tail(loops_list_ ## T ll, T* res);                                              \
int loops_list_head(loops_list_ ## T ll, T* res)

#define LOOPS_LIST_DEFINE(T)                                                                        \
int loops_list_construct(loops_list_ ## T* result)                                                  \
{                                                                                                   \
    *result = (loops_list_ ## T)malloc(sizeof(loops_list_ ## T ## _));                              \
    if(*result == NULL)                                                                             \
        return LOOPS_ERR_OUT_OF_MEMORY;                                                             \
    (*result)->impl = NULL;                                                                         \
    return LOOPS_ERR_SUCCESS;                                                                       \
}                                                                                                   \
void loops_list_destruct(loops_list_ ## T to_del, loops_destructor element_destructor)              \
{                                                                                                   \
    if(to_del)                                                                                      \
    {                                                                                               \
        if(to_del->impl != NULL)                                                                    \
        {                                                                                           \
            loops_list_inner_ ## T* elt;                                                            \
            loops_list_inner_ ## T* tmp;                                                            \
            loops_list_inner_ ## T* impl = to_del->impl;                                            \
            LL_FOREACH_SAFE(impl, elt, tmp)                                                         \
            {                                                                                       \
                LL_DELETE(impl, elt);                                                               \
                if(element_destructor == NULL)                                                      \
                    element_destructor(&elt->val);                                                  \
                free(elt);                                                                          \
            }                                                                                       \
        }                                                                                           \
        free(to_del);                                                                               \
    }                                                                                               \
}                                                                                                   \
int loops_list_push_back(loops_list_ ## T ll, T val)                                                \
{                                                                                                   \
    loops_list_inner_ ## T* impl = ll->impl;                                                        \
    loops_list_inner_ ## T* tail = (loops_list_inner_ ## T*)malloc(sizeof(loops_list_inner_ ## T)); \
    if(tail == NULL)                                                                                \
        LOOPS_THROW(LOOPS_ERR_OUT_OF_MEMORY);                                                       \
    tail->next = NULL;                                                                              \
    tail->val = val;                                                                                \
    LL_APPEND(impl, tail);                                                                          \
    ll->impl = impl;                                                                                \
    return LOOPS_ERR_SUCCESS;                                                                       \
}                                                                                                   \
int loops_list_tail(loops_list_ ## T ll, T* res)                                                    \
{                                                                                                   \
    loops_list_inner_ ## T* impl = ll->impl;                                                        \
    while(impl != NULL && impl->next != NULL)                                                       \
        impl = impl->next;                                                                          \
    if(impl == NULL)                                                                                \
        return LOOPS_ERR_NULL_POINTER;                                                              \
    *res = impl->val;                                                                               \
    return LOOPS_ERR_SUCCESS;                                                                       \
}                                                                                                   \
int loops_list_head(loops_list_ ## T ll, T* res)                                                    \
{                                                                                                   \
    loops_list_inner_ ## T* impl = ll->impl;                                                        \
    if(impl == NULL)                                                                                \
        return LOOPS_ERR_NULL_POINTER;                                                              \
    *res = impl->val;                                                                               \
    return LOOPS_ERR_SUCCESS;                                                                       \
}

// ====================================================== SPAN ======================================================
#define LOOPS_SPAN(T) loops_span_ ##T

#define LOOPS_SPAN_DECLARE(T)                                          \
typedef struct loops_span_ ## T ## _                                   \
{                                                                      \
    T* data;                                                           \
    int size;                                                          \
    int managed;                                                       \
} loops_span_ ## T ## _;                                               \
typedef struct loops_span_ ## T ## _* loops_span_ ## T;                \
int loops_span_construct_alloc(loops_span_ ## T* result, int size);    \
void loops_span_destruct(loops_span_ ## T to_del)

#define LOOPS_SPAN_DEFINE(T)                                             \
int loops_span_construct_alloc(loops_span_ ## T* result, int size)       \
{                                                                        \
    if(size <= 0)                                                        \
        return LOOPS_ERR_POSITIVE_SIZE_NEEDED;                           \
    (*result) = (loops_span_ ## T)malloc(sizeof(loops_span_ ## T ## _)); \
    if(*result == NULL)                                                  \
        return LOOPS_ERR_OUT_OF_MEMORY;                                  \
    (*result)->data = (T*)malloc(sizeof(T) * size);                      \
    if((*result)->data == NULL)                                          \
    {                                                                    \
        free(*result);                                                   \
        *result = NULL;                                                  \
        return LOOPS_ERR_OUT_OF_MEMORY;                                  \
    }                                                                    \
    (*result)->size = size;                                              \
    (*result)->managed = 1;                                              \
    return LOOPS_ERR_SUCCESS;                                            \
}                                                                        \
void loops_span_destruct(loops_span_ ## T to_del)                        \
{                                                                        \
    if(to_del)                                                           \
    {                                                                    \
        if(to_del->managed)                                              \
            free(to_del->data);                                          \
        free(to_del);                                                    \
    }                                                                    \
}

LOOPS_SPAN_DECLARE(int);
LOOPS_SPAN_DECLARE(char);
LOOPS_SPAN_DECLARE(uint8_t);

#endif//__LOOPS_COLLECTIONS_HPP__
