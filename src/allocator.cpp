/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#include "allocator.hpp"
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <libkern/OSCacheControl.h>


namespace loops
{
uint8_t* nonsys_alloc::allocate(size_t a_size)
{
    m_storage.push_back(std::vector<uint8_t>(a_size, 0));
    std::vector<uint8_t>& holden = m_storage.back();
    m_bufmap[&holden[0]] = &holden;
    return &holden[0];
}

uint8_t* nonsys_alloc::expand(uint8_t* a_buffer, size_t a_size)
{
    if(m_bufmap.count(a_buffer) == 0)
        throw std::string("Allocator: accessing non-existent buffer");
    std::vector<uint8_t>& vec = *m_bufmap[a_buffer];
    if(a_size == vec.size())
        return a_buffer;
    if(a_size < vec.size())
        throw std::string("Allocator: buffer shrinking is not supported");
    vec.resize(a_size, 0);
    if(a_buffer != &vec[0])
    {
        m_bufmap.erase(a_buffer);
        a_buffer = &vec[0];
        m_bufmap[a_buffer] = &vec;
    }
    return a_buffer;
}

void nonsys_alloc::erase(uint8_t* a_buffer)
{
    if(m_bufmap.count(a_buffer) == 0)
        throw std::string("Allocator: accessing non-existent buffer");
    m_bufmap.erase(a_buffer);
    std::vector<uint8_t>* vec = m_bufmap[a_buffer];
    std::list<std::vector<uint8_t> >::iterator rator = m_storage.begin();
    while(rator != m_storage.end())
    {
        if(&(*rator) == vec)
        {
            m_storage.erase(rator);
            break;
        }
        ++rator;
    }
}

void nonsys_alloc::protect2execution(uint8_t* a_buffer)
{
    throw std::string("Allocator: cannot prepare for execution this memory.");
}

nonsys_alloc nsalloc;
allocator* nonsys_alloc::get_instance()
{
    return &nsalloc;
}

uint8_t* arm64_alloc::allocate(size_t size)
{
    m_cached_size = size;
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
//#ifdef XBYAK_USE_MAP_JIT
    mode |= MAP_JIT;
//#endif
    void *p = mmap(NULL, size, PROT_READ | PROT_WRITE, mode, -1, 0); //TODO(ch): opposite is munmap.
    if (p == MAP_FAILED)
      throw std::string("Memory allocation failure");
    assert(p);
    return reinterpret_cast<uint8_t*>(p);
}

uint8_t* arm64_alloc::expand(uint8_t* buffer, size_t size)
{
    throw std::string("Allocator: not implemented.");
}

void arm64_alloc::erase(uint8_t* buffer)
{
    throw std::string("Allocator: not implemented.");
}

void arm64_alloc::protect2execution(uint8_t* a_buffer)
{
    size_t pageSize = sysconf(_SC_PAGESIZE);
    size_t iaddr = reinterpret_cast<size_t>(a_buffer);
    size_t roundAddr = iaddr & ~(pageSize - static_cast<size_t>(1));

    auto proret = mprotect(reinterpret_cast<void *>(roundAddr), 4096/*size*/ + (iaddr- roundAddr), PROT_READ | PROT_EXEC);
    if(proret != 0)
        throw std::string("Memory protection fail");
    sys_icache_invalidate(a_buffer, m_cached_size);
}

arm64_alloc arm_alloc;
allocator* arm64_alloc::get_instance()
{
    return &arm_alloc;
}
};
