/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "tests.hpp"
#include "src/acceleration.hpp"
#include "src/common.hpp"

#if __LOOPS_ARCH == __LOOPS_INTEL64
#include "src/backend_intel64.hpp"
#endif

#include <chrono>
#include <filesystem>
#include <vector>

using namespace loops;

TEST(basic, fastset)
{
    fastset values({3, 1, 63, 3, 0});
    EXPECT_EQ(values.size(), 4u);
    EXPECT_EQ(values.raw(), uint64_t(11) | (uint64_t(1) << 63));
    EXPECT_EQ(values.count(0), 1u);
    EXPECT_EQ(values.count(2), 0u);
    EXPECT_EQ(values.count(-1), 0u);
    EXPECT_EQ(values.count(64), 0u);

    std::vector<int> ordered(values.begin(), values.end());
    EXPECT_EQ(ordered, std::vector<int>({0, 1, 3, 63}));

    std::pair<fastset::iterator, bool> inserted = values.insert(2);
    EXPECT_TRUE(inserted.second);
    EXPECT_EQ(*inserted.first, 2);
    EXPECT_EQ(*values.find(2), 2);
    EXPECT_EQ(values.find(4), values.end());

    inserted = values.insert(2);
    EXPECT_FALSE(inserted.second);
    EXPECT_EQ(values.size(), 5u);

    fastset extra({62, 4});
    values.insert(extra.begin(), extra.end());
    ordered.assign(values.begin(), values.end());
    EXPECT_EQ(ordered, std::vector<int>({0, 1, 2, 3, 4, 62, 63}));


    EXPECT_EQ(values.erase(3), 1u);
    EXPECT_EQ(values.erase(3), 0u);
    EXPECT_EQ(values.erase(100), 0u);
    fastset::iterator next = values.erase(values.find(1));
    EXPECT_EQ(*next, 2);
    ordered.assign(values.begin(), values.end());
    EXPECT_EQ(ordered, std::vector<int>({0, 2, 4, 62, 63}));

    fastset raw((uint64_t(1) << 63) | (uint64_t(1) << 5));
    ordered.assign(raw.begin(), raw.end());
    EXPECT_EQ(ordered, std::vector<int>({5, 63}));

    fastset united({1, 62});
    united |= fastset({5, 62});
    ordered.assign(united.begin(), united.end());
    EXPECT_EQ(ordered, std::vector<int>({1, 5, 62}));

    EXPECT_THROW(values.insert(-1), std::out_of_range);
    EXPECT_THROW(values.insert(64), std::out_of_range);

    values.clear();
    EXPECT_TRUE(values.empty());
    EXPECT_EQ(values.begin(), values.end());
}

TEST(basic, dynamic_bitset)
{
    dynamic_bitset values;
    EXPECT_TRUE(values.empty());
    EXPECT_EQ(values.begin(), values.end());
    for(int value : {130, 64, 2, 63, 130})
        values.insert(value);
    std::vector<int> ordered(values.begin(), values.end());
    EXPECT_EQ(ordered, std::vector<int>({2, 63, 64, 130}));
    EXPECT_EQ(values.size(), 4u);
    EXPECT_EQ(values.count(65), 0u);
    EXPECT_EQ(*values.find(130), 130);
    EXPECT_EQ(values.erase(63), 1u);
    EXPECT_EQ(values.erase(63), 0u);

    dynamic_bitset other;
    for(int value : {64, 0, 1024})
        other.insert(value);
    values |= other;
    ordered.assign(values.begin(), values.end());
    EXPECT_EQ(ordered, std::vector<int>({0, 2, 64, 130, 1024}));
    EXPECT_EQ(values.size(), 5u);

    dynamic_bitset copied = values;
    ordered.assign(copied.begin(), copied.end());
    EXPECT_EQ(ordered, std::vector<int>({0, 2, 64, 130, 1024}));
    EXPECT_THROW(values.insert(-1), std::out_of_range);

    values.clear();
    EXPECT_TRUE(values.empty());
    EXPECT_EQ(values.begin(), values.end());
}

TEST(basic, small_vector)
{
    small_vector<int, 2> values({1, 2});
    values.push_back(3);
    values.insert(values.begin(), 0);
    values.insert(values.begin() + 2, 9);
    std::vector<int> ordered(values.begin(), values.end());
    EXPECT_EQ(ordered, std::vector<int>({0, 1, 9, 2, 3}));

    const int tail[] = {4, 5};
    values.insert(values.end(), tail, tail + 2);
    ordered.assign(values.begin(), values.end());
    EXPECT_EQ(ordered, std::vector<int>({0, 1, 9, 2, 3, 4, 5}));

    small_vector<int, 2> copied(values);
    ordered.assign(copied.begin(), copied.end());
    EXPECT_EQ(ordered, std::vector<int>({0, 1, 9, 2, 3, 4, 5}));
    copied[0] = 7;
    EXPECT_EQ(values.front(), 0);
    EXPECT_EQ(copied.front(), 7);

    small_vector<int, 2> moved(std::move(copied));
    ordered.assign(moved.begin(), moved.end());
    EXPECT_EQ(ordered, std::vector<int>({7, 1, 9, 2, 3, 4, 5}));
    EXPECT_TRUE(copied.empty());

    moved = {8, 6};
    ordered.assign(moved.begin(), moved.end());
    EXPECT_EQ(ordered, std::vector<int>({8, 6}));
    moved.clear();
    EXPECT_TRUE(moved.empty());
}

TEST(basic, dsu_arena)
{
    using IntSet = DisjointSetUnion<int>;
    IntSet::Arena arena;
    IntSet a(arena, 1);
    IntSet b(arena, 2);
    IntSet c(arena, 3);
    IntSet a_copy = a;
    EXPECT_TRUE(a_copy.isConnected(a));
    EXPECT_FALSE(a_copy.isConnected(b));
    a.merge(b);
    EXPECT_TRUE(a.isConnected(b));
    EXPECT_TRUE(a_copy.isConnected(b));
    EXPECT_FALSE(a.isConnected(c));
    *a_copy = 42;
    EXPECT_EQ(*a, 42);
    EXPECT_EQ(*b, 42);

    IntSet isolated = a.makeIsolated(42);
    EXPECT_FALSE(isolated.isConnected(a));
    IntSet empty;
    EXPECT_TRUE(empty.isEmpty());
    EXPECT_FALSE(empty.isConnected(a));

    int* stable_address = &*a;
    for(int i = 0; i < 1000; i++)
        IntSet extra(arena, i);
    EXPECT_EQ(&*a, stable_address);
}

TEST(basic, fasthash)
{
    fasthash table;
    EXPECT_TRUE(table.empty());
    EXPECT_EQ(table.find(10), nullptr);
    for (uint64_t key = 0; key < 80; ++key)
        EXPECT_TRUE(table.insert(key * 3, key + 100));
    EXPECT_EQ(table.size(), 80u);
    EXPECT_FALSE(table.insert(30, 777));
    EXPECT_TRUE(table.contains(30));
    EXPECT_EQ(table.at(30), 777u);
    EXPECT_FALSE(table.contains(31));
    EXPECT_THROW(table.at(31), std::out_of_range);

    table.compact();
    EXPECT_EQ(table.size(), 80u);
    EXPECT_LE(table.size() * 4, table.capacity() * 3);
    for (uint64_t key = 0; key < 80; ++key)
        EXPECT_EQ(table.at(key * 3), key == 10 ? 777 : key + 100);

    const uint64_t nonce = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    const std::filesystem::path path = std::filesystem::temp_directory_path() /
        ("loops_fasthash_" + std::to_string(nonce) + ".acctable");
    table.write(path.string());
    fasthash restored;
    restored.read(path.string());
    std::filesystem::remove(path);
    EXPECT_EQ(restored.size(), table.size());
    EXPECT_EQ(restored.capacity(), table.capacity());
    for (uint64_t key = 0; key < 80; ++key)
        EXPECT_EQ(restored.at(key * 3), table.at(key * 3));
}

TEST(basic, syntop_acceleration_key)
{
    Syntop op(OP_ADD, {argReg(RB_INT, 7), argReg(RB_INT, 8), argIImm(19)});
    const uint64_t expected = uint64_t(OP_ADD) | (uint64_t(3) << 8) |
        (uint64_t(Arg::IREG) << 12) | (uint64_t(Arg::IREG) << 15) |
        (uint64_t(Arg::IIMMEDIATE) << 18);
    EXPECT_EQ(syntopKey(op), expected);
    EXPECT_LE(syntopKey(op), uint64_t(1) << 42);

    op.args[0].idx = 100;
    op.args[0].elemtype = TYPE_FP64;
    op.args[0].flags = AF_ADDRESS;
    op.args[2].value = -100;
    EXPECT_EQ(syntopKey(op), expected);

    const uint64_t packed = packFastsets(fastset({0, 2, 9}), fastset({1, 9}));
    EXPECT_EQ(unpackFastset(packed, RB_INT), fastset({0, 2, 9}));
    EXPECT_EQ(unpackFastset(packed, RB_VEC), fastset({1, 9}));
    EXPECT_THROW(packFastsets(fastset({10}), fastset()), std::out_of_range);
    EXPECT_THROW(unpackFastset(packed, RB_AMOUNT), std::out_of_range);
}

#if __LOOPS_ARCH == __LOOPS_INTEL64
TEST(basic, syntop_acceleration_defs_and_control_flow)
{
    Intel64Backend backend;
    std::vector<Syntop> operations;
    operations.emplace_back(OP_IF_CSTART, std::initializer_list<Arg>{});
    operations.emplace_back(OP_ELIF_CSTART, std::initializer_list<Arg>{argIImm(0), argIImm(1)});
    operations.emplace_back(OP_IF_CEND, std::initializer_list<Arg>{});
    operations.emplace_back(OP_ELSE, std::initializer_list<Arg>{argIImm(0), argIImm(1)});
    operations.emplace_back(OP_ENDIF, std::initializer_list<Arg>{argIImm(0)});
    operations.emplace_back(OP_WHILE_CSTART, std::initializer_list<Arg>{argIImm(0)});
    operations.emplace_back(OP_WHILE_CEND, std::initializer_list<Arg>{});
    operations.emplace_back(OP_ENDWHILE, std::initializer_list<Arg>{argIImm(0), argIImm(1)});
    operations.emplace_back(OP_BREAK, std::initializer_list<Arg>{argIImm(0)});
    operations.emplace_back(OP_CONTINUE, std::initializer_list<Arg>{argIImm(0)});
    for (const Syntop& op : operations)
    {
        SCOPED_TRACE(op.opcode);
        EXPECT_TRUE(inputAccelerationTable().contains(syntopKey(op)));
        EXPECT_TRUE(outputAccelerationTable().contains(syntopKey(op)));
        for (int basket = 0; basket < RB_AMOUNT; ++basket)
        {
            EXPECT_EQ(backend.getInRegistersIdxs(op, basket), fastset());
            EXPECT_EQ(backend.getOutRegistersIdxs(op, basket), fastset());
            EXPECT_EQ(backend.getUsedRegistersIdxs(op, basket), fastset());
        }
    }

    const Syntop scalar_def(OP_DEF, {argReg(RB_INT, 0)});
    EXPECT_TRUE(inputAccelerationTable().contains(syntopKey(scalar_def)));
    EXPECT_TRUE(outputAccelerationTable().contains(syntopKey(scalar_def)));
    EXPECT_EQ(backend.getInRegistersIdxs(scalar_def, RB_INT), fastset());
    EXPECT_EQ(backend.getOutRegistersIdxs(scalar_def, RB_INT), fastset({0}));
    EXPECT_EQ(backend.getUsedRegistersIdxs(scalar_def, RB_INT), fastset());
    EXPECT_EQ(backend.getOutRegistersIdxs(scalar_def, RB_VEC), fastset());

    const Syntop vector_def(VOP_DEF, {argReg(RB_VEC, 0)});
    EXPECT_TRUE(inputAccelerationTable().contains(syntopKey(vector_def)));
    EXPECT_TRUE(outputAccelerationTable().contains(syntopKey(vector_def)));
    EXPECT_EQ(backend.getInRegistersIdxs(vector_def, RB_VEC), fastset());
    EXPECT_EQ(backend.getOutRegistersIdxs(vector_def, RB_VEC), fastset({0}));
    EXPECT_EQ(backend.getUsedRegistersIdxs(vector_def, RB_VEC), fastset());
    EXPECT_EQ(backend.getOutRegistersIdxs(vector_def, RB_INT), fastset());
}

TEST(basic, syntop_acceleration_mov_immediate)
{
    Intel64Backend backend;
    const Syntop zero(OP_MOV, {argReg(RB_INT, 0), argIImm(0)});
    const uint64_t key = syntopKey(zero);
    EXPECT_TRUE(inputAccelerationTable().contains(key));
    EXPECT_TRUE(outputAccelerationTable().contains(key));
    for (int64_t value : {int64_t(0), int64_t(1), int64_t(0x123456789)})
    {
        SCOPED_TRACE(value);
        const Syntop op(OP_MOV, {argReg(RB_INT, 0), argIImm(value)});
        EXPECT_EQ(syntopKey(op), key);
        EXPECT_EQ(backend.getInRegistersIdxs(op, RB_INT), fastset());
        EXPECT_EQ(backend.getOutRegistersIdxs(op, RB_INT), fastset({0}));
        EXPECT_EQ(backend.getUsedRegistersIdxs(op, RB_INT), fastset());
        EXPECT_EQ(backend.getInRegistersIdxs(op, RB_VEC), fastset());
        EXPECT_EQ(backend.getOutRegistersIdxs(op, RB_VEC), fastset());
        EXPECT_EQ(backend.getUsedRegistersIdxs(op, RB_VEC), fastset());
    }
}
#endif
