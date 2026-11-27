/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#ifndef __LOOPS_ACCELERATION_HPP__
#define __LOOPS_ACCELERATION_HPP__

#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace loops
{
class fastset_iterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = const int*;
    using reference = int;

    fastset_iterator() : m_remaining(0) {}
    explicit fastset_iterator(uint64_t remaining) : m_remaining(remaining) {}

    int operator*() const
    {
#if defined(_MSC_VER)
        unsigned long index;
        _BitScanForward64(&index, m_remaining);
        return static_cast<int>(index);
#else
        return __builtin_ctzll(m_remaining);
#endif
    }

    fastset_iterator& operator++()
    {
        m_remaining &= m_remaining - 1;
        return *this;
    }

    fastset_iterator operator++(int)
    {
        fastset_iterator result(*this);
        ++*this;
        return result;
    }

    bool operator==(const fastset_iterator& other) const { return m_remaining == other.m_remaining; }
    bool operator!=(const fastset_iterator& other) const { return m_remaining != other.m_remaining; }

private:
    uint64_t m_remaining;
};

class fastset
{
public:
    using value_type = int;
    using iterator = fastset_iterator;
    using const_iterator = fastset_iterator;

    fastset() : m_bits(0) {}
    explicit fastset(uint64_t bits) : m_bits(bits) {}
    fastset(std::initializer_list<int> values) : m_bits(0)
    {
        for (int value : values)
            insert(value);
    }

    iterator begin() { return iterator(m_bits); }
    iterator end() { return iterator(); }
    const_iterator begin() const { return const_iterator(m_bits); }
    const_iterator end() const { return const_iterator(); }
    const_iterator cbegin() const { return const_iterator(m_bits); }
    const_iterator cend() const { return const_iterator(); }

    bool empty() const { return m_bits == 0; }
    size_t size() const
    {
#if defined(_MSC_VER)
        uint64_t remaining = m_bits;
        size_t result = 0;
        while (remaining)
        {
            remaining &= remaining - 1;
            ++result;
        }
        return result;
#else
        return static_cast<size_t>(__builtin_popcountll(m_bits));
#endif
    }

    size_t count(int value) const
    {
        return value >= 0 && value < 64 && (m_bits & (uint64_t(1) << value)) ? 1 : 0;
    }

    iterator find(int value) const
    {
        return count(value) ? iterator(m_bits & (~uint64_t(0) << value)) : end();
    }

    std::pair<iterator, bool> insert(int value)
    {
        if (value < 0 || value >= 64)
            throw std::out_of_range("fastset index");
        const uint64_t mask = uint64_t(1) << value;
        const bool inserted = (m_bits & mask) == 0;
        m_bits |= mask;
        return std::make_pair(iterator(m_bits & (~uint64_t(0) << value)), inserted);
    }

    template<typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        for (; first != last; ++first)
            insert(*first);
    }

    size_t erase(int value)
    {
        if (!count(value))
            return 0;
        m_bits &= ~(uint64_t(1) << value);
        return 1;
    }

    iterator erase(iterator position)
    {
        iterator result = position;
        erase(*result);
        ++result;
        return result;
    }

    void clear() { m_bits = 0; }
    uint64_t raw() const { return m_bits; }

    fastset& operator|=(const fastset& other)
    {
        m_bits |= other.m_bits;
        return *this;
    }

    bool operator==(const fastset& other) const { return m_bits == other.m_bits; }
    bool operator!=(const fastset& other) const { return m_bits != other.m_bits; }

private:
    uint64_t m_bits;
};

class dynamic_bitset_iterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = const int*;
    using reference = int;

    dynamic_bitset_iterator() : m_words(nullptr), m_value(npos()) {}
    dynamic_bitset_iterator(const std::vector<uint64_t>* words, size_t value) : m_words(words), m_value(value == npos() ? npos() : findNext(*words, value)) {}

    int operator*() const { return static_cast<int>(m_value); }
    dynamic_bitset_iterator& operator++()
    {
        m_value = findNext(*m_words, m_value + 1);
        return *this;
    }
    dynamic_bitset_iterator operator++(int)
    {
        dynamic_bitset_iterator result(*this);
        ++*this;
        return result;
    }
    bool operator==(const dynamic_bitset_iterator& other) const { return m_words == other.m_words && m_value == other.m_value; }
    bool operator!=(const dynamic_bitset_iterator& other) const { return !(*this == other); }

private:
    static constexpr size_t npos() { return ~size_t(0); }
    static size_t findNext(const std::vector<uint64_t>& words, size_t value)
    {
        size_t word_index = value >> 6;
        if(word_index >= words.size())
            return npos();
        uint64_t word = words[word_index] & (~uint64_t(0) << (value & 63));
        while(true)
        {
            if(word)
            {
#if defined(_MSC_VER)
                unsigned long index;
                _BitScanForward64(&index, word);
                return (word_index << 6) + index;
#else
                return (word_index << 6) + __builtin_ctzll(word);
#endif
            }
            word_index++;
            if(word_index >= words.size())
                return npos();
            word = words[word_index];
        }
    }

    const std::vector<uint64_t>* m_words;
    size_t m_value;
};

class dynamic_bitset
{
public:
    using value_type = int;
    using iterator = dynamic_bitset_iterator;
    using const_iterator = dynamic_bitset_iterator;

    iterator begin() const { return iterator(&m_words, 0); }
    iterator end() const { return iterator(&m_words, ~size_t(0)); }
    const_iterator cbegin() const { return iterator(&m_words, 0); }
    const_iterator cend() const { return iterator(&m_words, ~size_t(0)); }

    bool empty() const { return m_size == 0; }
    size_t size() const { return m_size; }
    size_t count(int value) const
    {
        return value >= 0 && (size_t(value) >> 6) < m_words.size() && (m_words[size_t(value) >> 6] & (uint64_t(1) << (value & 63))) ? 1 : 0;
    }
    iterator find(int value) const { return count(value) ? iterator(&m_words, value) : end(); }

    std::pair<iterator, bool> insert(int value)
    {
        if(value < 0)
            throw std::out_of_range("dynamic_bitset index");
        const size_t word_index = size_t(value) >> 6;
        if(word_index >= m_words.size())
        {
            size_t new_size = m_words.empty() ? 1 : m_words.size();
            while(new_size <= word_index)
                new_size *= 2;
            m_words.resize(new_size, 0);
        }
        uint64_t& word = m_words[word_index];
        const uint64_t mask = uint64_t(1) << (value & 63);
        const bool inserted = (word & mask) == 0;
        if(inserted)
        {
            word |= mask;
            ++m_size;
        }
        return std::make_pair(iterator(&m_words, value), inserted);
    }

    template<typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        for(; first != last; ++first)
            insert(*first);
    }

    size_t erase(int value)
    {
        if(!count(value))
            return 0;
        m_words[size_t(value) >> 6] &= ~(uint64_t(1) << (value & 63));
        --m_size;
        return 1;
    }

    dynamic_bitset& operator|=(const dynamic_bitset& other)
    {
        if(m_words.size() < other.m_words.size())
            m_words.resize(other.m_words.size(), 0);
        for(size_t i = 0; i < other.m_words.size(); i++)
        {
            const uint64_t before = m_words[i];
            const uint64_t combined = before | other.m_words[i];
            m_size += bitCount(combined) - bitCount(before);
            m_words[i] = combined;
        }
        return *this;
    }

    void clear()
    {
        m_words.clear();
        m_size = 0;
    }

private:
    static size_t bitCount(uint64_t value)
    {
#if defined(_MSC_VER)
        size_t result = 0;
        while(value)
        {
            value &= value - 1;
            ++result;
        }
        return result;
#else
        return static_cast<size_t>(__builtin_popcountll(value));
#endif
    }

    std::vector<uint64_t> m_words;
    size_t m_size = 0;
};

template<typename T, size_t N>
class small_vector
{
    static_assert(N > 0, "small_vector requires inline capacity");
public:
    using value_type = T;
    using iterator = T*;
    using const_iterator = const T*;

    small_vector() : m_size(0), m_heap_active(false) {}
    small_vector(std::initializer_list<T> values) : small_vector()
    {
        for(const T& value : values)
            push_back(value);
    }
    small_vector(const small_vector& other)
        : m_heap(other.m_heap), m_local(other.m_local), m_size(other.m_size), m_heap_active(other.m_heap_active) {}
    small_vector(small_vector&& other) noexcept(std::is_nothrow_move_constructible<T>::value)
        : m_heap(std::move(other.m_heap)), m_local(std::move(other.m_local)), m_size(other.m_size), m_heap_active(other.m_heap_active)
    {
        other.m_size = 0;
        other.m_heap_active = false;
    }
    small_vector& operator=(const small_vector& other)
    {
        if(this != &other)
            *this = small_vector(other);
        return *this;
    }
    small_vector& operator=(small_vector&& other) noexcept(std::is_nothrow_move_assignable<T>::value)
    {
        if(this != &other)
        {
            m_heap = std::move(other.m_heap);
            m_local = std::move(other.m_local);
            m_size = other.m_size;
            m_heap_active = other.m_heap_active;
            other.m_size = 0;
            other.m_heap_active = false;
        }
        return *this;
    }
    small_vector& operator=(std::initializer_list<T> values)
    {
        clear();
        for(const T& value : values)
            push_back(value);
        return *this;
    }

    bool empty() const { return m_size == 0; }
    size_t size() const { return m_size; }
    T* data() { return m_heap_active ? m_heap.data() : m_local.data(); }
    const T* data() const { return m_heap_active ? m_heap.data() : m_local.data(); }
    iterator begin() { return data(); }
    iterator end() { return m_size == 0 ? begin() : data() + m_size; }
    const_iterator begin() const { return data(); }
    const_iterator end() const { return m_size == 0 ? begin() : data() + m_size; }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }
    T& operator[](size_t index) { return data()[index]; }
    const T& operator[](size_t index) const { return data()[index]; }
    T& front() { return *begin(); }
    const T& front() const { return *begin(); }
    T& back() { return data()[m_size - 1]; }
    const T& back() const { return data()[m_size - 1]; }

    void clear()
    {
        if(m_heap_active)
            m_heap.clear();
        else
            for(size_t i = 0; i < m_size; i++)
                m_local[i] = T();
        m_size = 0;
    }

    void push_back(const T& value) { insert(end(), value); }
    void push_back(T&& value) { insert(end(), std::move(value)); }
    iterator insert(const_iterator position, const T& value) { return insertValue(position, value); }
    iterator insert(const_iterator position, T&& value) { return insertValue(position, std::move(value)); }
    template<typename InputIterator>
    iterator insert(const_iterator position, InputIterator first, InputIterator last)
    {
        const size_t initial = position - begin();
        size_t index = initial;
        for(; first != last; ++first)
        {
            insert(begin() + index, *first);
            ++index;
        }
        return begin() + initial;
    }

private:
    void activateHeap()
    {
        m_heap.clear();
        m_heap.reserve(m_size * 2);
        for(size_t i = 0; i < m_size; i++)
            m_heap.push_back(std::move(m_local[i]));
        m_heap_active = true;
    }

    template<typename U>
    iterator insertValue(const_iterator position, U&& value)
    {
        const size_t index = position - begin();
        if(!m_heap_active && m_size < N)
        {
            for(size_t i = m_size; i > index; i--)
                m_local[i] = std::move(m_local[i - 1]);
            m_local[index] = std::forward<U>(value);
            ++m_size;
            return m_local.data() + index;
        }
        if(!m_heap_active)
            activateHeap();
        m_heap.insert(m_heap.begin() + index, std::forward<U>(value));
        m_size = m_heap.size();
        return m_heap.data() + index;
    }

    std::vector<T> m_heap;
    std::array<T, N> m_local;
    size_t m_size;
    bool m_heap_active;
};

struct Syntop;

class fasthash
{
public:
    struct Entry
    {
        uint64_t key;
        uint64_t value;
    };

    fasthash();

    bool insert(uint64_t key, uint64_t value);
    bool contains(uint64_t key) const;
    const uint64_t* find(uint64_t key) const;
    uint64_t at(uint64_t key) const;

    void write(const std::string& path) const;
    void read(const std::string& path);
    void compact();

    bool empty() const { return m_size == 0; }
    size_t size() const { return m_size; }
    size_t capacity() const { return m_entries.size(); }

private:
    static const uint64_t EMPTY_KEY = UINT64_MAX;

    size_t findSlot(uint64_t key) const;
    void rehash(size_t capacity);
    void setCapacity(size_t capacity);

    std::vector<Entry> m_entries;
    size_t m_size;
    size_t m_hash_shift;
};

uint64_t syntopKey(const Syntop& op);
uint64_t packFastsets(const fastset& integer_set, const fastset& vector_set);
fastset unpackFastset(uint64_t packed, int basketNum);
std::string accelerationTableFile(const std::string& directory, bool input);
const fasthash& inputAccelerationTable();
const fasthash& outputAccelerationTable();
}

#endif
