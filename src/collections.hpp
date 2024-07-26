/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_COLLECTIONS_HPP__
#define __LOOPS_COLLECTIONS_HPP__
#include <uthash.h>

typedef const char* loops_cstring;

typedef int K;
typedef loops_cstring V;

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
int loops_hashmap_get(loops_hashmap_ ## K ## _ ## V lm, K key, V* res);                                     

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

LOOPS_HASHMAP_DECLARE(int, loops_cstring);
LOOPS_HASHMAP_DECLARE(int, int);

#endif//__LOOPS_COLLECTIONS_HPP__
