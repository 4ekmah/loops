/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "allocator.hpp"
#include <string>
#if defined(__APPLE__)
#include <unistd.h>
#include <sys/mman.h>
#include <libkern/OSCacheControl.h>
#endif


namespace loops
{

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
      throw std::string("Memory allocation failure");
    assert(p);
    return reinterpret_cast<uint8_t*>(p);
}

uint8_t* Allocator::expand(uint8_t* buffer, size_t size)
{
    throw std::string("Allocator: not implemented.");
}

void Allocator::protect2Execution(uint8_t* a_buffer)
{
    size_t pageSize = sysconf(_SC_PAGESIZE);
    size_t iaddr = reinterpret_cast<size_t>(a_buffer);
    size_t roundAddr = iaddr & ~(pageSize - static_cast<size_t>(1));

    auto proret = mprotect(reinterpret_cast<void *>(roundAddr), 4096/*size*/ + (iaddr- roundAddr), PROT_READ | PROT_EXEC);
    if(proret != 0)
        throw std::string("Memory protection fail");
    sys_icache_invalidate(a_buffer, m_cachedSize);
}

Allocator arm_alloc;
Allocator* Allocator::getInstance()
{
    return &arm_alloc;
}
};
