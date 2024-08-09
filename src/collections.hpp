/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_COLLECTIONS_HPP__
#define __LOOPS_COLLECTIONS_HPP__
#include "runtime.hpp"
#include <uthash.h>
#include <utlist.h>
typedef const char* loops_cstring;

typedef int K;
typedef loops_cstring V;

// ===================================================== HASHMAP =====================================================

#define LOOPS_HASHMAP(K, V) loops_hashmap_ ## K ## _ ## V
#define LOOPS_HASHMAP_STATIC(K, V) static loops_hashmap_static_ ## K ## _ ## V
#define LOOPS_HASHMAP_ELEM(K, ...) {K, __VA_ARGS__, {}}

#define LOOPS_HASHMAP_DECLARE(K, V)                                                                                              \
    typedef struct loops_hashmap_static_ ## K ## _ ## V                                                                          \
    {                                                                                                                            \
        K key;                                                                                                                   \
        V val;                                                                                                                   \
        UT_hash_handle hh;                                                                                                       \
    } loops_hashmap_static_ ## K ## _ ## V;                                                                                      \
    typedef struct loops_hashmap_ ## K ## _ ## V ## _                                                                            \
    {                                                                                                                            \
        bool _static;                                                                                                            \
        loops_hashmap_static_ ## K ## _ ## V* impl;                                                                              \
    } loops_hashmap_ ## K ## _ ## V ## _;                                                                                        \
    typedef struct loops_hashmap_ ## K ## _ ## V ## _* loops_hashmap_ ## K ## _ ## V;                                            \
int loops_hashmap_construct(loops_hashmap_ ## K ## _ ## V* result);                                                              \
int loops_hashmap_construct_static(loops_hashmap_ ## K ## _ ## V* result, loops_hashmap_static_ ## K ## _ ## V* input, int size);\
void loops_hashmap_destruct(loops_hashmap_ ## K ## _ ## V to_del);                                                               \
int loops_hashmap_add(loops_hashmap_ ## K ## _ ## V lm, K key, V val);                                                           \
int loops_hashmap_has(loops_hashmap_ ## K ## _ ## V lm, K key, bool* res);                                                       \
int loops_hashmap_get(loops_hashmap_ ## K ## _ ## V lm, K key, V* res)                                     

//TODO[CPP2ANSIC]: loops_hashmap_get have to return pointer, not value.

#define LOOPS_HASHMAP_DEFINE(K, V)                                                                                              \
int loops_hashmap_construct(loops_hashmap_ ## K ## _ ## V* result)                                                              \
{                                                                                                                               \
    *result = (loops_hashmap_ ## K ## _ ## V)malloc(sizeof(loops_hashmap_ ## K ## _ ## V ## _));                                \
    if(*result == NULL)                                                                                                         \
        return LOOPS_ERR_OUT_OF_MEMORY;                                                                                         \
    (*result)->_static = false;                                                                                                 \
    (*result)->impl = NULL;                                                                                                     \
    return LOOPS_ERR_SUCCESS;                                                                                                   \
}                                                                                                                               \
int loops_hashmap_construct_static(loops_hashmap_ ## K ## _ ## V* result, loops_hashmap_static_ ## K ## _ ## V* input, int size)\
{                                                                                                                               \
    int elnum;                                                                                                                  \
    *result = (loops_hashmap_ ## K ## _ ## V)malloc(sizeof(loops_hashmap_ ## K ## _ ## V ## _));                                \
    if(*result == NULL)                                                                                                         \
        return LOOPS_ERR_OUT_OF_MEMORY;                                                                                         \
    (*result)->_static = true;                                                                                                  \
    (*result)->impl = NULL;                                                                                                     \
    loops_hashmap_static_ ## K ## _ ## V* utcontainer = (*result)->impl;                                                        \
    for(elnum = 0; elnum < size; elnum++)                                                                                       \
        HASH_ADD(hh, utcontainer, key, sizeof(K), input + elnum);                                                               \
    (*result)->impl = utcontainer;                                                                                              \
    return LOOPS_ERR_SUCCESS;                                                                                                   \
}                                                                                                                               \
void loops_hashmap_destruct(loops_hashmap_ ## K ## _ ## V to_del)                                                               \
{                                                                                                                               \
    if(to_del)                                                                                                                  \
    {                                                                                                                           \
        if(to_del->impl != NULL)                                                                                                \
        {                                                                                                                       \
            loops_hashmap_static_ ## K ## _ ## V*  map_to_free = to_del->impl;                                                  \
            loops_hashmap_static_ ## K ## _ ## V* current;                                                                      \
            loops_hashmap_static_ ## K ## _ ## V* tmp;                                                                          \
            HASH_ITER(hh, map_to_free, current, tmp)                                                                            \
            {                                                                                                                   \
                HASH_DEL(map_to_free, current);                                                                                 \
                if(!to_del->_static)                                                                                            \
                    free(current);                                                                                              \
            }                                                                                                                   \
        }                                                                                                                       \
        free(to_del);                                                                                                           \
    }                                                                                                                           \
}                                                                                                                               \
int loops_hashmap_add(loops_hashmap_ ## K ## _ ## V lm, K key, V val)                                                           \
{                                                                                                                               \
    if(lm == NULL)                                                                                                              \
        return LOOPS_ERR_NULL_POINTER;                                                                                          \
    loops_hashmap_static_ ## K ## _ ## V* to_add =                                                                              \
        (loops_hashmap_static_ ## K ## _ ## V*)malloc(sizeof(loops_hashmap_static_ ## K ## _ ## V));                            \
    to_add->key = key;                                                                                                          \
    to_add->val = val;                                                                                                          \
    if(to_add == NULL)                                                                                                          \
        return LOOPS_ERR_OUT_OF_MEMORY;                                                                                         \
    loops_hashmap_static_ ## K ## _ ## V* utcontainer = lm->impl;                                                               \
    HASH_ADD(hh, utcontainer, key, sizeof(K), to_add);                                                                          \
    if(lm->impl == NULL)                                                                                                        \
        lm->impl = utcontainer;                                                                                                 \
    return LOOPS_ERR_SUCCESS;                                                                                                   \
}                                                                                                                               \
int loops_hashmap_has(loops_hashmap_ ## K ## _ ## V lm, K key, bool* res)                                                       \
{                                                                                                                               \
    if(lm == NULL)                                                                                                              \
        return LOOPS_ERR_NULL_POINTER;                                                                                          \
    loops_hashmap_static_ ## K ## _ ## V* impl = lm->impl;                                                                      \
    if(impl == nullptr)                                                                                                         \
    {                                                                                                                           \
        *res = false;                                                                                                           \
        return LOOPS_ERR_SUCCESS;                                                                                               \
    }                                                                                                                           \
    loops_hashmap_static_ ## K ## _ ## V* found;                                                                                \
    HASH_FIND(hh, impl, &key, sizeof(K), found);                                                                                \
    *res = (found != NULL);                                                                                                     \
    return LOOPS_ERR_SUCCESS;                                                                                                   \
}                                                                                                                               \
int loops_hashmap_get(loops_hashmap_ ## K ## _ ## V lm, K key, V* res)                                                          \
{                                                                                                                               \
    if(lm == NULL)                                                                                                              \
        return LOOPS_ERR_NULL_POINTER;                                                                                          \
    loops_hashmap_static_ ## K ## _ ## V* impl = lm->impl;                                                                      \
    if(impl == nullptr)                                                                                                         \
        return LOOPS_ERR_ELEMENT_NOT_FOUND;                                                                                     \
    loops_hashmap_static_ ## K ## _ ## V* found;                                                                                \
    HASH_FIND(hh, impl, &key, sizeof(K), found);                                                                                \
    if(found == NULL)                                                                                                           \
        return LOOPS_ERR_ELEMENT_NOT_FOUND;                                                                                     \
    *res = found->val;                                                                                                          \
    return LOOPS_ERR_SUCCESS;                                                                                                   \
}

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
    bool managed;                                                      \
} loops_span_inner_ ## T ## _;                                         \
typedef struct loops_span_ ## T ## _* loops_span_ ## T;                \
int loops_span_construct(loops_span_ ## T* result, T* data, int size); \
int loops_span_construct_alloc(loops_span_ ## T* result, int size);    \
void loops_span_destruct(loops_span_ ## T to_del)

#define LOOPS_SPAN_DEFINE(T)                                             \
int loops_span_construct(loops_span_ ## T* result, T* data, int size)    \
{                                                                        \
    (*result) = (loops_span_ ## T)malloc(sizeof(loops_span_ ## T ##_));  \
    if(*result == NULL)                                                  \
        return LOOPS_ERR_OUT_OF_MEMORY;                                  \
    (*result)->data = data;                                              \
    (*result)->size = size;                                              \
    (*result)->managed = false;                                          \
    return LOOPS_ERR_SUCCESS;                                            \
}                                                                        \
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
    (*result)->managed = true;                                           \
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

LOOPS_HASHMAP_DECLARE(int, loops_cstring);
LOOPS_HASHMAP_DECLARE(int, int);
LOOPS_SPAN_DECLARE(int);
LOOPS_SPAN_DECLARE(char);
LOOPS_SPAN_DECLARE(uint8_t);

#endif//__LOOPS_COLLECTIONS_HPP__
