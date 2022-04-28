/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

//TODO(ch): write some words, that this code was basically copied from xbyak
#include "allocator.hpp"
#include <string>
#include <memory>
#if defined(__APPLE__)
#include <unistd.h>
#include <sys/mman.h>
#include <libkern/OSCacheControl.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

namespace loops
{
#if defined(__APPLE__)
uint8_t* Allocator::allocate(size_t size)
{
    m_cachedSize = size;
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
      throw std::string("Memory allocation failure.");
    assert(p);
    return reinterpret_cast<uint8_t*>(p);
}

void Allocator::protect2Execution(uint8_t* a_buffer)
{
    size_t pageSize = sysconf(_SC_PAGESIZE);
    size_t iaddr = reinterpret_cast<size_t>(a_buffer);
    size_t roundAddr = iaddr & ~(pageSize - static_cast<size_t>(1));

    auto proret = mprotect(reinterpret_cast<void*>(roundAddr), 4096/*size*/ + (iaddr - roundAddr), PROT_READ | PROT_EXEC);
    if (proret != 0)
        throw std::string("Memory protection failure.");
    sys_icache_invalidate(a_buffer, m_cachedSize);
}
#elif defined(_WIN32)
    uint8_t* Allocator::allocate(size_t size)
    {
        m_cachedSize = size;
        static const size_t ALIGN_PAGE_SIZE = 4096;
        const size_t alignedSizeM1 = ALIGN_PAGE_SIZE - 1;
        size = (size + alignedSizeM1) & ~alignedSizeM1;
        uint8_t* result = reinterpret_cast<uint8_t*>(VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
        if(result == nullptr) 
            throw std::string("Memory allocation failure.");
        return result;
    }

    void Allocator::protect2Execution(uint8_t* a_buffer)
    {
        DWORD mode = PAGE_EXECUTE_READ;
        DWORD oldProtect = 0;
        if(!VirtualProtect(static_cast<void*>(a_buffer), m_cachedSize, mode, &oldProtect))
            throw std::string("Memory protection failure.");
        if (!FlushInstructionCache(GetCurrentProcess(), a_buffer, m_cachedSize))
            throw std::string("Memory cache flushing failure.");
    }
#endif

uint8_t* Allocator::expand(uint8_t* buffer, size_t size)
{
    throw std::string("Allocator: not implemented.");
}

std::shared_ptr<Allocator> ASingleton;
Allocator* Allocator::getInstance()
{
    if (ASingleton.get() == nullptr)
        ASingleton.reset(new Allocator());
    return ASingleton.get();
}

Allocator::Allocator():m_cachedSize(0) {}
};