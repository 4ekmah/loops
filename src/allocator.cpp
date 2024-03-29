/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

//TODO(ch): write some words, that this code was basically copied from xbyak
#include "allocator.hpp"
#include "loops/defines.hpp"
#include <assert.h>
#include <stdexcept>
#include <string>
#include <memory>
#if __LOOPS_OS == __LOOPS_MAC
#include <unistd.h>
#include <sys/mman.h>
#include <libkern/OSCacheControl.h>
#elif __LOOPS_OS == __LOOPS_WINDOWS
#include <windows.h>
#elif __LOOPS_OS == __LOOPS_LINUX
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#else
#error Unknown OS
#endif

namespace loops
{
#if __LOOPS_OS == __LOOPS_MAC
uint8_t* Allocator::allocate(size_t size)
{
    //Allocate memory:
    size_t pageSize = sysconf(_SC_PAGESIZE);
    const size_t alignedSizeM1 = pageSize - 1;
    size = (size + alignedSizeM1) & ~alignedSizeM1;
#ifdef MAP_ANONYMOUS
    int mode = MAP_PRIVATE | MAP_ANONYMOUS;
#elif defined(MAP_ANON)
    int mode = MAP_PRIVATE | MAP_ANON;
#else
#error "not supported"
#endif
    mode |= MAP_JIT;
    void *p = mmap(NULL, size, PROT_READ | PROT_WRITE, mode, -1, 0);
    if (p == MAP_FAILED)
      throw std::runtime_error("Memory allocation failure.");
    assert(p);
    uint8_t* res = reinterpret_cast<uint8_t*>(p);
    m_table[res] = size;
    return reinterpret_cast<uint8_t*>(p);
}

void Allocator::protect2Execution(uint8_t* a_buffer)
{
    size_t pageSize = sysconf(_SC_PAGESIZE);
    size_t iaddr = reinterpret_cast<size_t>(a_buffer);
    size_t roundAddr = iaddr & ~(pageSize - static_cast<size_t>(1));

    auto proret = mprotect(reinterpret_cast<void*>(roundAddr), m_table[a_buffer] + (iaddr - roundAddr), PROT_READ | PROT_EXEC);
    if (proret != 0)
        throw std::runtime_error("Memory protection failure.");
    sys_icache_invalidate(a_buffer, m_table[a_buffer]);
}

void Allocator::free(uint8_t* a_buffer)
{
    auto found = m_table.find(a_buffer);
    if(found != m_table.end())
    {
        munmap(a_buffer, found->second);
        m_table.erase(found);
    }
}

#elif __LOOPS_OS == __LOOPS_WINDOWS
    uint8_t* Allocator::allocate(size_t size)
    {
        static const size_t ALIGN_PAGE_SIZE = 4096;
        const size_t alignedSizeM1 = ALIGN_PAGE_SIZE - 1;
        size = (size + alignedSizeM1) & ~alignedSizeM1;
        uint8_t* result = reinterpret_cast<uint8_t*>(VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
        if(result == nullptr) 
            throw std::runtime_error("Memory allocation failure.");
        m_table[result] = size;
        return result;
    }

    void Allocator::protect2Execution(uint8_t* a_buffer)
    {
        DWORD mode = PAGE_EXECUTE_READ;
        DWORD oldProtect = 0;
        if(!VirtualProtect(static_cast<void*>(a_buffer), m_table[a_buffer], mode, &oldProtect))
            throw std::runtime_error("Memory protection failure.");
        if (!FlushInstructionCache(GetCurrentProcess(), a_buffer, m_table[a_buffer]))
            throw std::runtime_error("Memory cache flushing failure.");
    }

    void Allocator::free(uint8_t* a_buffer)
    {
        auto found = m_table.find(a_buffer);
        if(found != m_table.end())
        {
            VirtualFree(LPVOID(a_buffer), found->second,  MEM_RELEASE);
            m_table.erase(found);
        }
    }
#elif __LOOPS_OS == __LOOPS_LINUX
uint8_t* Allocator::allocate(size_t size)
{
    //Allocate memory:
    size_t pageSize = sysconf(_SC_PAGESIZE);
    const size_t alignedSizeM1 = pageSize - 1;
    size = (size + alignedSizeM1) & ~alignedSizeM1;
#ifdef MAP_ANONYMOUS
    int mode = MAP_PRIVATE | MAP_ANONYMOUS;
#elif defined(MAP_ANON)
    int mode = MAP_PRIVATE | MAP_ANON;
#else
#error "not supported"
#endif
    // mode |= MAP_JIT;
    void *p = mmap(NULL, size, PROT_READ | PROT_WRITE, mode, -1, 0);
    if (p == MAP_FAILED)
      throw std::runtime_error("Memory allocation failure.");
    assert(p);
    uint8_t* res = reinterpret_cast<uint8_t*>(p);
    m_table[res] = size;
    return reinterpret_cast<uint8_t*>(p);
}

void Allocator::protect2Execution(uint8_t* a_buffer)
{
    size_t pageSize = sysconf(_SC_PAGESIZE);
    size_t iaddr = reinterpret_cast<size_t>(a_buffer);
    size_t roundAddr = iaddr & ~(pageSize - static_cast<size_t>(1));

    auto proret = mprotect(reinterpret_cast<void*>(roundAddr), m_table[a_buffer] + (iaddr - roundAddr), PROT_READ | PROT_EXEC);
    if (proret != 0)
        throw std::runtime_error("Memory protection failure.");
#ifdef __clang__
    __clear_cache((char*)a_buffer, (char*)a_buffer + m_table[a_buffer]);
#else// __GNUC__
    __builtin___clear_cache((char*)a_buffer, (char*)a_buffer + m_table[a_buffer]);
#endif
}

void Allocator::free(uint8_t* a_buffer)
{
    auto found = m_table.find(a_buffer);
    if(found != m_table.end())
    {
        munmap(a_buffer, found->second);
        m_table.erase(found);
    }
}
#else
#error Unknown OS
#endif

Allocator::~Allocator()
{
    while(m_table.begin() != m_table.end())
        free(m_table.begin()->first);
}
}
