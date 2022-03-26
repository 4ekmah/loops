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
    class allocator
    {
        public:
        virtual uint8_t* allocate(size_t a_size) = 0;
        virtual uint8_t* expand(uint8_t* a_buffer, size_t a_size) = 0;
        virtual void erase(uint8_t* a_buffer) = 0;
        virtual void protect2execution(uint8_t* a_buffer) = 0;
    };

    class nonsys_alloc : public allocator
    {
    private:
        std::list<std::vector<uint8_t> > m_storage;
        std::unordered_map<uint8_t*, std::vector<uint8_t>* > m_bufmap;
    public:
        virtual uint8_t* allocate(size_t size) final;
        virtual uint8_t* expand(uint8_t* buffer, size_t size) final;
        virtual void erase(uint8_t* buffer) final;
        virtual void protect2execution(uint8_t* a_buffer) final;
        static allocator* get_instance(); //TODO(ch): I hope, it's temporary solution. And definetely non-threadsafe.
    };

    class arm64_alloc : public allocator
    {
    private:
        size_t m_cached_size; //TODO(ch): element of disposable implementation.
//        std::list<std::vector<uint8_t> > m_storage;
//        std::unordered_map<uint8_t*, std::vector<uint8_t>* > m_bufmap;
    public:
        virtual uint8_t* allocate(size_t size) final;
        virtual uint8_t* expand(uint8_t* buffer, size_t size) final;
        virtual void erase(uint8_t* buffer) final;
        virtual void protect2execution(uint8_t* a_buffer) final;
        static allocator* get_instance(); //TODO(ch): I hope, it's temporary solution. And definetely non-threadsafe.
    };

};

#endif // __LOOPS_ALLOCATOR_HPP__
