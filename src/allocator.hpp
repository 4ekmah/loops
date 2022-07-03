/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_ALLOCATOR_HPP__
#define __LOOPS_ALLOCATOR_HPP__

#include <stdint.h>
#include <stddef.h>
#include <unordered_map>

namespace loops
{
    class Allocator
    {
    public:
        Allocator(){};
        uint8_t* allocate(size_t size);
        uint8_t* expand(uint8_t* buffer, size_t size);
        void protect2Execution(uint8_t* a_buffer);
    private:
        std::unordered_map<uint8_t*, size_t> m_table; //[ptr] = size
    };
};

#endif // __LOOPS_ALLOCATOR_HPP__
