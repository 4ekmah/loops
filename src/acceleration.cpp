/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "acceleration.hpp"
#include "common.hpp"

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <limits>
#include <type_traits>

namespace
{
const uint64_t HASH_MULTIPLIER = UINT64_C(11400714819323198485);
const uint64_t FILE_VERSION = 1;
const uint64_t FILE_BYTE_ORDER = UINT64_C(0x0102030405060708);
const size_t MIN_CAPACITY = 8;
const int PACKED_SET_WIDTH = 16;
const uint64_t PACKED_SET_MASK = (uint64_t(1) << loops::Syntop::SYNTOP_ARGS_MAX) - 1;

struct AccelerationFileHeader
{
    char magic[8];
    uint64_t version;
    uint64_t byte_order;
    uint64_t architecture;
    uint64_t entry_size;
    uint64_t size;
    uint64_t capacity;
};

const char FILE_MAGIC[8] = {'L', 'O', 'O', 'P', 'S', 'A', 'C', 'C'};

#ifndef LOOPS_ACCTABLES_DIR
#define LOOPS_ACCTABLES_DIR "acctables"
#endif

struct GlobalAccelerationTables
{
    GlobalAccelerationTables()
    {
        const char* directory_override = std::getenv("LOOPS_ACCTABLES_DIR");
        const std::string directory = directory_override ? directory_override : LOOPS_ACCTABLES_DIR;
        try
        {
            input.read(loops::accelerationTableFile(directory, true));
        }
        catch (const std::exception&)
        {}
        try
        {
            output.read(loops::accelerationTableFile(directory, false));
        }
        catch (const std::exception&)
        {}
    }

    loops::fasthash input;
    loops::fasthash output;
};

const GlobalAccelerationTables global_acceleration_tables;
}

namespace loops
{
fasthash::fasthash() : m_size(0), m_hash_shift(0)
{}

size_t fasthash::findSlot(uint64_t key) const
{
    size_t slot = static_cast<size_t>((key * HASH_MULTIPLIER) >> m_hash_shift);
    while (m_entries[slot].key != EMPTY_KEY && m_entries[slot].key != key)
        slot = (slot + 1) & (m_entries.size() - 1);
    return slot;
}

void fasthash::setCapacity(size_t capacity)
{
    if (capacity < MIN_CAPACITY || (capacity & (capacity - 1)) != 0)
        throw std::invalid_argument("fasthash capacity must be a power of two and at least 8.");
    m_entries.assign(capacity, Entry{EMPTY_KEY, 0});
    size_t capacity_log2 = 0;
    while ((size_t(1) << capacity_log2) < capacity)
        ++capacity_log2;
    m_hash_shift = 64 - capacity_log2;
}

void fasthash::rehash(size_t capacity)
{
    std::vector<Entry> previous;
    previous.swap(m_entries);
    m_size = 0;
    setCapacity(capacity);
    for (const Entry& entry : previous)
        if (entry.key != EMPTY_KEY)
            insert(entry.key, entry.value);
}

bool fasthash::insert(uint64_t key, uint64_t value)
{
    if (key == EMPTY_KEY)
        throw std::invalid_argument("fasthash reserved key.");
    if (m_entries.empty() || (m_size + 1) * 4 > m_entries.size() * 3)
        rehash(m_entries.empty() ? MIN_CAPACITY : m_entries.size() * 2);
    const size_t slot = findSlot(key);
    if (m_entries[slot].key == key)
    {
        m_entries[slot].value = value;
        return false;
    }
    m_entries[slot] = Entry{key, value};
    ++m_size;
    return true;
}

const uint64_t* fasthash::find(uint64_t key) const
{
    if (m_entries.empty())
        return nullptr;
    const size_t slot = findSlot(key);
    return m_entries[slot].key == key ? &m_entries[slot].value : nullptr;
}

bool fasthash::contains(uint64_t key) const
{
    return find(key) != nullptr;
}

uint64_t fasthash::at(uint64_t key) const
{
    const uint64_t* result = find(key);
    if (!result)
        throw std::out_of_range("fasthash key");
    return *result;
}

void fasthash::compact()
{
    if (m_size == 0)
    {
        m_entries.clear();
        m_hash_shift = 0;
        return;
    }
    size_t capacity = MIN_CAPACITY;
    while (m_size * 4 > capacity * 3)
        capacity *= 2;
    rehash(capacity);
}

void fasthash::write(const std::string& path) const
{
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output)
        throw std::runtime_error("Cannot open acceleration table for writing: " + path);
    const AccelerationFileHeader header = {
        {FILE_MAGIC[0], FILE_MAGIC[1], FILE_MAGIC[2], FILE_MAGIC[3],
         FILE_MAGIC[4], FILE_MAGIC[5], FILE_MAGIC[6], FILE_MAGIC[7]},
        FILE_VERSION,
        FILE_BYTE_ORDER,
        __LOOPS_ARCH,
        sizeof(Entry),
        m_size,
        m_entries.size()
    };
    output.write(reinterpret_cast<const char*>(&header), sizeof(header));
    if (!m_entries.empty())
        output.write(reinterpret_cast<const char*>(m_entries.data()), m_entries.size() * sizeof(Entry));
    if (!output)
        throw std::runtime_error("Cannot write acceleration table: " + path);
}

void fasthash::read(const std::string& path)
{
    std::ifstream input(path, std::ios::binary | std::ios::ate);
    if (!input)
        throw std::runtime_error("Cannot open acceleration table for reading: " + path);
    const std::streamoff file_size = input.tellg();
    if (file_size < static_cast<std::streamoff>(sizeof(AccelerationFileHeader)))
        throw std::runtime_error("Truncated acceleration table: " + path);
    input.seekg(0);
    AccelerationFileHeader header;
    input.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (!input || std::memcmp(header.magic, FILE_MAGIC, sizeof(FILE_MAGIC)) != 0 ||
        header.version != FILE_VERSION || header.byte_order != FILE_BYTE_ORDER ||
        header.architecture != __LOOPS_ARCH || header.entry_size != sizeof(Entry))
        throw std::runtime_error("Incompatible acceleration table: " + path);
    if (header.capacity != 0 &&
        (header.capacity < MIN_CAPACITY || (header.capacity & (header.capacity - 1)) != 0 ||
         header.size >= header.capacity || header.size * 4 > header.capacity * 3))
        throw std::runtime_error("Invalid acceleration table size: " + path);
    if (header.capacity == 0 && header.size != 0)
        throw std::runtime_error("Invalid acceleration table size: " + path);
    const uint64_t payload_size = header.capacity * sizeof(Entry);
    if (header.capacity > (std::numeric_limits<uint64_t>::max() - sizeof(AccelerationFileHeader)) / sizeof(Entry) ||
        sizeof(AccelerationFileHeader) + payload_size != static_cast<uint64_t>(file_size) ||
        payload_size > static_cast<uint64_t>(std::numeric_limits<std::streamsize>::max()) ||
        header.capacity > std::numeric_limits<size_t>::max())
        throw std::runtime_error("Invalid acceleration table file size: " + path);

    fasthash result;
    if (header.capacity != 0)
        result.setCapacity(static_cast<size_t>(header.capacity));
    result.m_size = static_cast<size_t>(header.size);
    if (header.capacity != 0)
        input.read(reinterpret_cast<char*>(result.m_entries.data()), static_cast<std::streamsize>(payload_size));
    if (!input)
        throw std::runtime_error("Truncated acceleration table entries: " + path);

    fasthash validation;
    size_t occupied = 0;
    for (const Entry& entry : result.m_entries)
        if (entry.key != EMPTY_KEY)
        {
            ++occupied;
            if (!validation.insert(entry.key, entry.value))
                throw std::runtime_error("Duplicate acceleration table key: " + path);
        }
    if (occupied != result.m_size)
        throw std::runtime_error("Invalid acceleration table element count: " + path);
    *this = std::move(result);
}

uint64_t syntopKey(const Syntop& op)
{
    if (op.opcode < 0 || op.opcode > 0xff || op.args_size < 0 || op.args_size > Syntop::SYNTOP_ARGS_MAX)
        throw std::out_of_range("Syntop acceleration key");
    uint64_t result = static_cast<uint64_t>(op.opcode) | (static_cast<uint64_t>(op.args_size) << 8);
    for (int argnum = 0; argnum < op.args_size; ++argnum)
    {
        const int tag = op.args[argnum].tag;
        if (tag < 0 || tag > 7)
            throw std::out_of_range("Syntop argument tag");
        result |= static_cast<uint64_t>(tag) << (12 + argnum * 3);
    }
    return result;
}

uint64_t packFastsets(const fastset& integer_set, const fastset& vector_set)
{
    if ((integer_set.raw() & ~PACKED_SET_MASK) != 0 || (vector_set.raw() & ~PACKED_SET_MASK) != 0)
        throw std::out_of_range("Packed fastset index");
    return integer_set.raw() | (vector_set.raw() << PACKED_SET_WIDTH);
}

fastset unpackFastset(uint64_t packed, int basketNum)
{
    if (basketNum < 0 || basketNum >= RB_AMOUNT)
        throw std::out_of_range("Packed fastset basket");
    return fastset((packed >> (basketNum * PACKED_SET_WIDTH)) & PACKED_SET_MASK);
}

std::string accelerationTableFile(const std::string& directory, bool input)
{
#if __LOOPS_ARCH == __LOOPS_INTEL64
    const char* architecture = "intel64";
#elif __LOOPS_ARCH == __LOOPS_AARCH64
    const char* architecture = "aarch64";
#else
    const char* architecture = "riscv";
#endif
    const std::string filename = std::string(architecture) + (input ? "_input.acctable" : "_output.acctable");
    return directory.empty() ? filename : directory + "/" + filename;
}

const fasthash& inputAccelerationTable()
{
    return global_acceleration_tables.input;
}

const fasthash& outputAccelerationTable()
{
    return global_acceleration_tables.output;
}
}

static_assert(sizeof(loops::fastset) == sizeof(uint64_t), "fastset must contain only one uint64_t.");
static_assert(alignof(loops::fastset) == alignof(uint64_t), "fastset alignment must match uint64_t.");
static_assert(std::is_standard_layout<loops::fastset>::value, "fastset must have standard layout.");
static_assert(std::is_trivially_copyable<loops::fastset>::value, "fastset must be trivially copyable.");
static_assert(sizeof(loops::fastset_iterator) == sizeof(uint64_t), "fastset iterator must contain only one uint64_t.");
static_assert(alignof(loops::fastset_iterator) == alignof(uint64_t), "fastset iterator alignment must match uint64_t.");
static_assert(std::is_standard_layout<loops::fastset_iterator>::value, "fastset iterator must have standard layout.");
static_assert(std::is_trivially_copyable<loops::fastset_iterator>::value, "fastset iterator must be trivially copyable.");
static_assert(sizeof(loops::fasthash::Entry) == sizeof(uint64_t) * 2, "fasthash entry must contain two uint64_t values.");
static_assert(std::is_standard_layout<loops::fasthash::Entry>::value, "fasthash entry must have standard layout.");
static_assert(std::is_trivially_copyable<loops::fasthash::Entry>::value, "fasthash entry must be trivially copyable.");
static_assert(std::is_standard_layout<AccelerationFileHeader>::value, "Acceleration file header must have standard layout.");
static_assert(std::is_trivially_copyable<AccelerationFileHeader>::value, "Acceleration file header must be trivially copyable.");
