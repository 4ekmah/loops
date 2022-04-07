/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_ALLOCATOR_HPP__
#define __LOOPS_ALLOCATOR_HPP__

#include <list>
#include <unordered_map>
#include <vector>

namespace loops
{
    class Allocator
    {
    public:
        uint8_t* allocate(size_t size);
        uint8_t* expand(uint8_t* buffer, size_t size);
        void protect2Execution(uint8_t* a_buffer);
        static Allocator* getInstance(); //TODO(ch): I hope, it's temporary solution. And definetely non-threadsafe.
    private:
        size_t m_cachedSize; //TODO(ch): element of disposable implementation.
    };

};

#endif // __LOOPS_ALLOCATOR_HPP__
