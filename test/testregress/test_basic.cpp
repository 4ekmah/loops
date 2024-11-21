/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "loops/loops.hpp"
#include "tests.hpp"
#include <iostream>
#include <cstring>
#include <math.h>
#include <stdexcept>
#include <algorithm>
#include "src/common.hpp"
#include "src/func_impl.hpp"
#include "src/reg_allocator.hpp"

using namespace loops;

Func make_a_plus_b(Context ctx, const std::string& fname)
{
    USE_CONTEXT_(ctx);
    IReg a, b;
    STARTFUNC_(fname, &a, &b)
    {
        a+=b;
        RETURN_(a);
    }
    return ctx.getFunc(fname);
}
void test_a_plus_b(Func func)
{
    typedef int64_t (*a_plus_b_f)(int64_t a, int64_t b);
    a_plus_b_f tested = reinterpret_cast<a_plus_b_f>(func.ptr());
    std::vector<int64_t> A = {3,2,4,2,3,4,1};
    std::vector<int64_t> B = {4,4,5,5,4,6,5};
    for(size_t n = 0; n < A.size(); n++)
        ASSERT_EQ(tested(A[n],B[n]), A[n]+B[n]);
}

TEST(basic, a_plus_b)
{
    Context ctx;
    loops::Func func = make_a_plus_b(ctx, test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    test_a_plus_b(func);
}

Func make_min_max_scalar(Context ctx, const std::string& fname)
{
    USE_CONTEXT_(ctx);
    IReg ptr, n, minpos_addr, maxpos_addr;
    STARTFUNC_(fname, &ptr, &n, &minpos_addr, &maxpos_addr)
    {
        IReg i = CONST_(0);
        IReg minpos = CONST_(0);
        IReg maxpos = CONST_(0);
        IReg minval = load_<int>(ptr);
        IReg maxval = minval;
        n *= sizeof(int);
        WHILE_(i < n)
        {
            IReg x = load_<int>(ptr, i);
            IF_(x < minval)
            {
                minval = x;
                minpos = i;
            }
            IF_(x > maxval)
            {
                maxval = x;
                maxpos = i;
            }
            i += sizeof(int);
        }
        IReg elemsize = CONST_(sizeof(int));
        minpos /= elemsize;
        maxpos /= elemsize;
        store_<int>(minpos_addr, minpos);
        store_<int>(maxpos_addr, maxpos);
        RETURN_(0);
    }
    return ctx.getFunc(fname);
}

void test_min_max_scalar(Func func)
{
    typedef int64_t (*min_max_scalar_f)(const int* ptr, int64_t n, int* minpos, int* maxpos);
    min_max_scalar_f tested = reinterpret_cast<min_max_scalar_f>(func.ptr());
    std::vector<int> v = { 8, 2, -5, 7, 6 };
    int minpos = -1, maxpos = -1;
    int64_t retval = tested(&v[0], v.size(), &minpos, &maxpos);
    int rminpos = 0, rmaxpos = 0;
    for(int num = 0; num < (int)v.size(); num++)
    {
        if(v[rminpos] > v[num])
            rminpos = num;
        if(v[rmaxpos] < v[num])
            rmaxpos = num;
    }
    ASSERT_EQ(rmaxpos, maxpos);
    ASSERT_EQ(rminpos, minpos);
    ASSERT_EQ(retval, 0);
}

TEST(basic, min_max_scalar)
{
    Context ctx;
    loops::Func func = make_min_max_scalar(ctx, test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    test_min_max_scalar(func);
}

Func make_ten_args_to_sum(Context ctx, const std::string& fname)
{
    USE_CONTEXT_(ctx);
    IReg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
    STARTFUNC_(fname, &a0, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9)
    {
        IReg res = a0 * 1;
        res += a1 * 2;
        res += a2 * 3;
        res += a3 * 4;
        res += a4 * 5;
        res += a5 * 6;
        res += a6 * 7;
        res += a7 * 8;
        res += a8 * 3;
        res += a9 * 2;
        RETURN_(res);
    }
    return ctx.getFunc(fname);
}

void test_ten_args_to_sum(Func func)
{
    typedef int64_t(*ten_args_to_sum_f)(int64_t a0, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6, int64_t a7, int64_t a8, int64_t a9);
    ten_args_to_sum_f tested = reinterpret_cast<ten_args_to_sum_f>(func.ptr());
    std::vector<int> v = { 1,1,1,1,1,1,1,1,3,5 };
    ASSERT_EQ(tested(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9]),(int64_t)(55));
}

TEST(basic, ten_args_to_sum) //There we are testing stack parameter passing.
{
    Context ctx;
    loops::Func func = make_ten_args_to_sum(ctx, test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    test_ten_args_to_sum(func);
}

#if __LOOPS_OS == __LOOPS_WINDOWS
#undef min // Windows.h implements min and max as macro.
#undef max //
#endif
Func make_min_max_select(Context ctx, const std::string& fname)
{
    USE_CONTEXT_(ctx);
    IReg ptr, n, minpos_addr, maxpos_addr;
    STARTFUNC_(fname, &ptr, &n, &minpos_addr, &maxpos_addr)
    {
        IReg i = CONST_(0);
        IReg minpos = CONST_(0);
        IReg maxpos = CONST_(0);
        IReg minval = load_<int>(ptr);
        IReg maxval = minval;
        n <<= 2;// sizeof(int) == 4;
        WHILE_(i < n)
        {
            IReg x = load_<int>(ptr, i);
            minpos = select(x < minval, i, minpos);
            minval = min(x, minval);
            maxpos = select(x > maxval, i, maxpos);
            maxval = max(x, maxval);
            i += sizeof(int);
        }
        minpos >>= 2;// * sizeof(int);
        maxpos >>= 2;// * sizeof(int);
        store_<int>(minpos_addr, minpos);
        store_<int>(maxpos_addr, maxpos);
        RETURN_(0);
    }
    return ctx.getFunc(fname);
}

void test_min_max_select(Func func)
{
    typedef int64_t (*min_max_select_f)(const int* ptr, int64_t n, int* minpos, int* maxpos);
    min_max_select_f tested = reinterpret_cast<min_max_select_f>(func.ptr());
    std::vector<int> v = { 8, 2, -5, 7, 6 };
    int minpos = -1, maxpos = -1;
    int64_t retval = tested(&v[0], v.size(), &minpos, &maxpos);
    int rminpos = 0, rmaxpos = 0;
    for(int num = 0; num < (int)v.size(); num++)
    {
        if(v[rminpos] > v[num])
            rminpos = num;
        if(v[rmaxpos] < v[num])
            rmaxpos = num;
    }
    ASSERT_EQ(rmaxpos, maxpos);
    ASSERT_EQ(rminpos, minpos);
    ASSERT_EQ(retval, 0);
}

TEST(basic, min_max_select)
{
    Context ctx;
    loops::Func func = make_min_max_select(ctx, test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    test_min_max_select(func);
}

enum {NOT_A_TRIANGLE, RIGHT_TRIANGLE, EQUILATERAL_TRIANGLE, ISOSCELES_TRIANGLE, ACUTE_TRIANGLE, OBTUSE_TRIANGLE};
Func make_triangle_types(Context ctx, const std::string& fname)
{
    USE_CONTEXT_(ctx);
    IReg a, b, c;
    STARTFUNC_(fname, &a, &b, &c)
    {
        IF_(a <= 0 || b <= 0 || c <= 0)
            RETURN_(NOT_A_TRIANGLE);
        ELIF_(a > b + c || b > a + c || c > a + b)
            RETURN_(NOT_A_TRIANGLE);
        ELIF_(a == b && a == c)
            RETURN_(EQUILATERAL_TRIANGLE);
        ELIF_(a == b || a == c || b == c)
            RETURN_(ISOSCELES_TRIANGLE);
        ELIF_(a*a == b*b + c*c || b*b == a*a + c*c || c*c == a*a +b*b)
            RETURN_(RIGHT_TRIANGLE);
        ELIF_(a*a > b*b + c*c || b*b > a*a + c*c || c*c > a*a + b*b)
            RETURN_(OBTUSE_TRIANGLE);
        ELSE_
            RETURN_(ACUTE_TRIANGLE);
    }
    return ctx.getFunc(fname);
}

void test_triangle_types(Func func)
{
    typedef int64_t (*triangle_types_f)(int64_t a, int64_t b, int64_t c);
    triangle_types_f tested = reinterpret_cast<triangle_types_f>(func.ptr());
    ASSERT_EQ(tested(-1,2,3), int(NOT_A_TRIANGLE));
    ASSERT_EQ(tested(1,-2,3), int(NOT_A_TRIANGLE));
    ASSERT_EQ(tested(1,2,-3), int(NOT_A_TRIANGLE));
    ASSERT_EQ(tested(1,2,5), int(NOT_A_TRIANGLE));
    ASSERT_EQ(tested(5,2,1), int(NOT_A_TRIANGLE));
    ASSERT_EQ(tested(1,5,2), int(NOT_A_TRIANGLE));
    ASSERT_EQ(tested(1,1,1), int(EQUILATERAL_TRIANGLE));
    ASSERT_EQ(tested(1,1,2), int(ISOSCELES_TRIANGLE));
    ASSERT_EQ(tested(1,2,1), int(ISOSCELES_TRIANGLE));
    ASSERT_EQ(tested(2,1,1), int(ISOSCELES_TRIANGLE));
    ASSERT_EQ(tested(3,4,5), int(RIGHT_TRIANGLE));
    ASSERT_EQ(tested(5,4,3), int(RIGHT_TRIANGLE));
    ASSERT_EQ(tested(3,5,4), int(RIGHT_TRIANGLE));
    ASSERT_EQ(tested(5,6,9), int(OBTUSE_TRIANGLE));
    ASSERT_EQ(tested(9,5,6), int(OBTUSE_TRIANGLE));
    ASSERT_EQ(tested(5,9,6), int(OBTUSE_TRIANGLE));
    ASSERT_EQ(tested(7,5,6), int(ACUTE_TRIANGLE));
}

TEST(basic, triangle_types)
{
    Context ctx;
    loops::Func func = make_triangle_types(ctx, test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    test_triangle_types(func);
}
Func make_nonnegative_odd(Context ctx, const std::string& fname)
{
    USE_CONTEXT_(ctx);
    IReg ptr, n;
    STARTFUNC_(fname, &ptr, &n)
    {
        IReg i = CONST_(0);
        IReg res = CONST_(-(int64_t)sizeof(int));
        n *= sizeof(int);
        WHILE_(i < n)
        {
            IReg x = load_<int>(ptr, i);
            IF_(x < 0)
            {
                i += sizeof(int);
                CONTINUE_;
            }
            IReg mod = x % 2;
            IF_(mod != 0)
            {
                res = i;
                BREAK_;
            }
            i += sizeof(int);
        }
        res /= sizeof(int);
        RETURN_(res);
    }
    return ctx.getFunc(fname);
}

void test_nonnegative_odd(Func func)
{
    typedef int64_t (*nonnegative_odd_f)(const int* ptr, int64_t n);
    nonnegative_odd_f tested = reinterpret_cast<nonnegative_odd_f>(func.ptr());
    std::vector<int> v = { 8, 2, -5, 7, 6 };
    ASSERT_EQ(tested(&v[0], v.size()), 3);
    ASSERT_EQ(tested(0, 0), -1);
}

TEST(basic, nonnegative_odd)
{
    Context ctx;
    loops::Func func = make_nonnegative_odd(ctx, test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    test_nonnegative_odd(func);
}

template <typename inT, typename outT>
static inline bool AldrAstrTestInner(const uint8_t* inA, uint8_t* outA, int siz)
{
    bool res = true;
    for (int num = 0; num < siz; num++)
    {
        inT read = *reinterpret_cast<const inT*>(inA + num * loops::ElemTraits<inT>::elemsize);
        outT written = *reinterpret_cast<outT*>(outA + num * loops::ElemTraits<outT>::elemsize);
        res &= static_cast<outT>(read) == written;
    }
    return res;
}

template <typename inT>
static inline bool AldrAstrTest(const uint8_t* inA, uint8_t* outA, int siz, int otyp)
{
    bool chck = false;
    switch (otyp)
    {
    case (loops::TYPE_U8):  chck = AldrAstrTestInner<inT, uint8_t>(inA, outA, siz); break;
    case (loops::TYPE_I8):  chck = AldrAstrTestInner<inT, int8_t>(inA, outA, siz); break;
    case (loops::TYPE_U16): chck = AldrAstrTestInner<inT, uint16_t>(inA, outA, siz); break;
    case (loops::TYPE_I16): chck = AldrAstrTestInner<inT, int16_t>(inA, outA, siz); break;
    case (loops::TYPE_U32): chck = AldrAstrTestInner<inT, uint32_t>(inA, outA, siz); break;
    case (loops::TYPE_I32): chck = AldrAstrTestInner<inT, int32_t>(inA, outA, siz); break;
    case (loops::TYPE_U64): chck = AldrAstrTestInner<inT, uint64_t>(inA, outA, siz); break;
    case (loops::TYPE_I64): chck = AldrAstrTestInner<inT, int64_t>(inA, outA, siz); break;
    };
    return chck;
}

Func make_all_loads_all_stores(Context ctx, const std::string& fname)
{
    USE_CONTEXT_(ctx);
    IReg iptr, ityp, optr, otyp, n;
    STARTFUNC_(fname, &iptr, &ityp, &optr, &otyp, &n)
    {
        IReg num = CONST_(0);
        IReg i_offset = CONST_(0);
        IReg o_offset = CONST_(0);
        IReg ielemsize = select(ityp > TYPE_I8, CONST_(2), CONST_(1));
        ielemsize = select(ityp > TYPE_I16, CONST_(4), ielemsize);
        ielemsize = select(ityp > TYPE_I32, CONST_(8), ielemsize);
        IReg oelemsize = select(otyp > TYPE_I8, CONST_(2), CONST_(1));
        oelemsize = select(otyp > TYPE_I16, CONST_(4), oelemsize);
        oelemsize = select(otyp > TYPE_I32, CONST_(8), oelemsize);
        WHILE_(num < n)
        {
            IReg x = CONST_(0); //TODO(ch): we need to have variable definition without init value(probably with context reference or something...). [I think, we can introduce pure def instruction, which will dissappear on liveness analysis].
            IF_(ityp == TYPE_U8)
                x = load_<uint8_t>(iptr, i_offset);
            ELIF_(ityp == TYPE_I8)
                x = load_<int8_t>(iptr, i_offset);
            ELIF_(ityp == TYPE_U16)
                x = load_<uint16_t>(iptr, i_offset);
            ELIF_(ityp == TYPE_I16)
                x = load_<int16_t>(iptr, i_offset);
            ELIF_(ityp == TYPE_U32)
                x = load_<uint32_t>(iptr, i_offset);
            ELIF_(ityp == TYPE_I32)
                x = load_<int32_t>(iptr, i_offset);
            ELIF_(ityp == TYPE_U64)
                x = load_<uint64_t>(iptr, i_offset);
            ELSE_
                x = load_<int64_t>(iptr, i_offset);

            IF_(otyp == TYPE_U8)
                store_<uint8_t>(optr, o_offset, x);
            ELIF_(otyp == TYPE_I8)
                store_<int8_t>(optr, o_offset, x);
            ELIF_(otyp == TYPE_U16)
                store_<uint16_t>(optr, o_offset, x);
            ELIF_(otyp == TYPE_I16)
                store_<int16_t>(optr, o_offset, x);
            ELIF_(otyp == TYPE_U32)
                store_<uint32_t>(optr, o_offset, x);
            ELIF_(otyp == TYPE_I32)
                store_<int32_t>(optr, o_offset, x);
            ELIF_(otyp == TYPE_U64)
                store_<uint64_t>(optr, o_offset, x);
            ELSE_
                store_<int64_t>(optr, o_offset, x);

            i_offset += ielemsize;
            o_offset += oelemsize;
            num += 1;
        }
    }
    return ctx.getFunc(fname);
}

void test_all_loads_all_stores(Func func)
{
    typedef int64_t (*all_loads_all_stores_f)(const void* iptr, int ityp, void* optr, int otyp, int64_t n);
    all_loads_all_stores_f tested = reinterpret_cast<all_loads_all_stores_f>(func.ptr());
    static const size_t BYTE_ARR_SIZE = 40;
    const uint8_t v[BYTE_ARR_SIZE] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0xff,
        0x02, 0x03, 0x04, 0xff, 0x06, 0x07, 0x00, 0xff,
        0x03, 0xff, 0x05, 0xff, 0x07, 0xff, 0x01, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    uint8_t res[BYTE_ARR_SIZE * sizeof(uint64_t)];
    for (int ityp = loops::TYPE_U8; ityp <= loops::TYPE_I64; ityp++)
        for (int otyp = loops::TYPE_U8; otyp <= loops::TYPE_I64; otyp++)
        {
            static const int siztypes[] = { 1, 1, 2, 2, 4, 4, 8, 8 };
            int siz = BYTE_ARR_SIZE / siztypes[ityp];
            tested(v, ityp, res, otyp, siz);
            bool chck = false;
            switch (ityp)
            {
            case (loops::TYPE_U8):  chck = AldrAstrTest<uint8_t>(v, res, siz, otyp); break;
            case (loops::TYPE_I8):  chck = AldrAstrTest<int8_t>(v, res, siz, otyp); break;
            case (loops::TYPE_U16): chck = AldrAstrTest<uint16_t>(v, res, siz, otyp); break;
            case (loops::TYPE_I16): chck = AldrAstrTest<int16_t>(v, res, siz, otyp); break;
            case (loops::TYPE_U32): chck = AldrAstrTest<uint32_t>(v, res, siz, otyp); break;
            case (loops::TYPE_I32): chck = AldrAstrTest<int32_t>(v, res, siz, otyp); break;
            case (loops::TYPE_U64): chck = AldrAstrTest<uint64_t>(v, res, siz, otyp); break;
            case (loops::TYPE_I64): chck = AldrAstrTest<int64_t>(v, res, siz, otyp); break;
            };
            ASSERT_EQ(chck, true);
        }
}

TEST(basic, all_loads_all_stores)
{
    Context ctx;
    loops::Func func = make_all_loads_all_stores(ctx, test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    test_all_loads_all_stores(func);
}

Func make_nullify_msb_lsb(Context ctx, const std::string& fname)
{
    USE_CONTEXT_(ctx);
    IReg in, elsb, emsb;
    STARTFUNC_(fname, &in, &elsb, &emsb)
    {
        IReg msb = in | ushift_right(in, 1);
        msb |= ushift_right(msb, 2);
        msb |= ushift_right(msb, 4);
        msb |= ushift_right(msb, 8);
        msb |= ushift_right(msb, 16);
        msb |= ushift_right(msb, 32);
        msb += 1;  //It's assumed, that 0x8000000000000000 bit is switched off.
        msb = ushift_right(msb, 1);
        msb ^= in;
        store_<uint64_t>(emsb, msb);
        IReg lsb = in & ~(in - 1);
        lsb ^= in;
        store_<uint64_t>(elsb, lsb);
        RETURN_();
    }
    return ctx.getFunc(fname);
}

void test_nullify_msb_lsb(Func func)
{
    typedef int64_t (*nullify_msb_lsb_f)(uint64_t in, uint64_t* elsb, uint64_t* emsb);
    nullify_msb_lsb_f tested = reinterpret_cast<nullify_msb_lsb_f>(func.ptr());
    std::vector<uint64_t> v = { 0x6000000000000000, 2, 0xf0, 7, 0xffffffff };
    for (uint64_t tchk : v)
    {
        uint64_t elsb;
        uint64_t emsb;
        uint64_t relsb = tchk ^ (tchk & ~(tchk - 1));
        uint64_t remsb;
        tested(tchk, &elsb, &emsb);
        remsb = tchk | tchk >> 1;
        remsb |= remsb >> 2;
        remsb |= remsb >> 4;
        remsb |= remsb >> 8;
        remsb |= remsb >> 16;
        remsb |= remsb >> 32;
        remsb = (remsb + 1) >> 1;
        remsb ^= tchk;
        ASSERT_EQ(remsb, emsb);
        ASSERT_EQ(relsb, elsb);
    }
}

TEST(basic, nullify_msb_lsb)
{
    Context ctx;
    loops::Func func = make_nullify_msb_lsb(ctx, test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    test_nullify_msb_lsb(func);
}

//Bresenham algorithm implementations(loops and reference) is taken from wikipedia: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
Func make_bresenham(Context ctx, const std::string& fname)
{
    USE_CONTEXT_(ctx);
    IReg canvas, w, x0, y0, x1, y1, filler;
    STARTFUNC_(fname, &canvas, &w, &x0, &y0, &x1, &y1, &filler)
    {
        IReg dx = abs(x1 - x0);
        IReg sx = sign(x1 - x0);
        IReg dy = -abs(y1 - y0);
        IReg sy = sign(y1 - y0);
        IReg error = dx + dy;
        WHILE_(canvas != 0)      //TODO(ch): this is substitution of while(true)
        {
            store_<uint8_t>(canvas, y0* w + x0, filler);
            IF_(x0 == x1 && y0 == y1)
                BREAK_;
            IReg e2 = error << 1;
            IF_(e2 >= dy)
            {
                IF_(x0 == x1)
                    BREAK_;
                error = error + dy;
                x0 = x0 + sx;
            }
            IF_(e2 <= dx)
            {
                IF_(y0 == y1)
                    BREAK_;
                error = error + dx;
                y0 = y0 + sy;
            }
        }
        RETURN_();
    }
    return ctx.getFunc(fname);
}

void test_bresenham(Func func)
{
    typedef void (*bresenham_f)(uint8_t* canvas, int64_t w, int64_t x0, int64_t y0, int64_t x1, int64_t y1, uint64_t filler);
    bresenham_f tested = reinterpret_cast<bresenham_f>(func.ptr());

    auto bresenham_ref = [](uint8_t* canvas, int64_t w, int64_t x0, int64_t y0, int64_t x1, int64_t y1, uint64_t filler)
    {
        int64_t dx = std::abs(x1 - x0);
        int64_t sx = (x1 - x0) == 0 ? 0 : ((x1 - x0) > 0 ? 1 : -1);
        int64_t dy = -std::abs(y1 - y0);
        int64_t sy = (y1 - y0) == 0 ? 0 : ((y1 - y0) > 0 ? 1 : -1);
        int64_t error = dx + dy;
        while (true)
        {
            canvas[y0 * w + x0] = (uint8_t)filler;
            if (x0 == x1 && y0 == y1)
                break;
            int64_t e2 = 2 * error;
            if (e2 >= dy)
            {
                if (x0 == x1)
                    break;
                error = error + dy;
                x0 = x0 + sx;
            }
            if (e2 <= dx)
            {
                if (y0 == y1)
                    break;
                error = error + dx;
                y0 = y0 + sy;
            }
        }
    };
    int64_t w = 15;
    int64_t h = 15;
    std::vector<uint8_t> canvas(3 * w * h, 0);
    uint8_t* optr = (&canvas[0]) + w * h;
    memset(optr, ' ', w * h);
    tested(optr, w, 0, 0, 14, 14, '1');
    ASSERT_EQ(memok(&canvas[0], w, h), true);
    tested(optr, w, 0, 11, 7, 7, '2');
    ASSERT_EQ(memok(&canvas[0], w, h), true);
    tested(optr, w, 14, 7, 11, 14, '3');
    ASSERT_EQ(memok(&canvas[0], w, h), true);
    tested(optr, w, 5, 3, 8, 3, '4');
    ASSERT_EQ(memok(&canvas[0], w, h), true);
    tested(optr, w, 9, 2, 9, 13, '5');
    ASSERT_EQ(memok(&canvas[0], w, h), true);
    tested(optr, w, 14, 1, 14, 1, '6');
    ASSERT_EQ(memok(&canvas[0], w, h), true);
    std::vector<uint8_t> canvasRef(w * h, ' ');
    bresenham_ref(&canvasRef[0], w, 0, 0, 14, 14, '1');
    bresenham_ref(&canvasRef[0], w, 0, 11, 7, 7, '2');
    bresenham_ref(&canvasRef[0], w, 14, 7, 11, 14, '3');
    bresenham_ref(&canvasRef[0], w, 5, 3, 8, 3, '4');
    bresenham_ref(&canvasRef[0], w, 9, 2, 9, 13, '5');
    bresenham_ref(&canvasRef[0], w, 14, 1, 14, 1, '6');
    for (int symbn = 0; symbn < w * h; symbn++)
        ASSERT_EQ(optr[symbn], canvasRef[symbn]);
}

TEST(basic, bresenham)
{
    Context ctx;
    loops::Func func = make_bresenham(ctx, test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    test_bresenham(func);
}

Func make_conditionpainter(Context ctx, const std::string& fname)
{
    const int xmin = -5, xmax = 5, ymin = -5, ymax = 5;
    const int w = xmax - xmin + 1;
    USE_CONTEXT_(ctx);
    IReg ptr;
    STARTFUNC_(fname, &ptr)
    {
        IReg y = CONST_(ymin);
        WHILE_(y<=ymax)
        {
            IReg offsety = (y - ymin) * w * sizeof(int64_t);
            IReg x = CONST_(xmin);
            WHILE_(x<=xmax)
            {
                IReg out = (y >= x + 3 && y <=4 && x >= -2 && x <= 0) || (y<=x-1 && x>=0 && y<=0 && x*x + y*y <= 9);
                out += select(((x >= 2 && x <= 4) || (y >= 1 && y <= 3)) && (x*x + y*y <= 16), 2, 0);
                IF_(((2*y >=x && y<= -(x+3)*(x+3))||(y <= 2*x && y >= -(x+3)*(x+3))) && x <= 0)
                    out += 3;
                // store_<int64_t>(ptr, offsety + ((x - xmin) << 3), out); //TODO(ch): There is some bug on intel here, causing segfault. Uncomment and fix it. 
                store_<int64_t>(ptr + offsety + ((x - xmin) << 3), out);
                x += 1;
            }
            y += 1;
        }
        RETURN_();
    }
    return ctx.getFunc(fname);
}

void test_conditionpainter(Func func)
{
    int xmin = -5, xmax = 5, ymin = -5, ymax = 5;
    int h = ymax - ymin + 1;
    int w = xmax - xmin + 1;
    typedef int64_t (*conditionpainter_f)(int64_t* canvas);
    conditionpainter_f tested = reinterpret_cast<conditionpainter_f>(func.ptr());

    auto conditionpainter_ref = [xmin, xmax, ymin, ymax, w](int64_t* ptr)
    {
        for(int y = ymin; y <= ymax; y++)
            for(int x = xmin; x <= xmax; x++)
            {
                int datax = x - xmin;
                int datay = y - ymin;
                ptr[datay * w + datax] = (y >= x + 3 && y <=4 && x >= -2 && x <= 0) || (y<=x-1 && x>=0 && y<=0 && x*x + y*y <= 9);
                ptr[datay * w + datax] += (((x >= 2 && x <= 4) || (y >= 1 && y <= 3)) && (x*x + y*y <= 16) ? 2 : 0);
                if(((2*y >=x && y<= -(x+3)*(x+3))||(y <= 2*x && y >= -(x+3)*(x+3))) && x <= 0)
                    ptr[datay * w + datax] += 3;
            }
    };
    std::vector<int64_t> canvas_ref_(w*h, 0);
    std::vector<int64_t> canvas_(3*w*h, 0);
    int64_t* canvas_ref = canvas_ref_.data();
    int64_t* canvas = canvas_.data();
    tested(canvas + w*h);
    ASSERT_EQ(memok((uint8_t*)&canvas[0], w * sizeof(int64_t), h), true);
    conditionpainter_ref(canvas_ref);
    for(int y = 0; y < h; y++)
        for(int x = 0; x < w; x++)
            ASSERT_EQ(canvas_ref[y*w + x], canvas[y*w + x + w*h]);
}

TEST(basic, conditionpainter)
{
    Context ctx;
    loops::Func func = make_conditionpainter(ctx, test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    test_conditionpainter(func);
}

TEST(basic, compile_all)
{
    Context ctx;
    loops::Func a_plus_b_func = make_a_plus_b(ctx, "a_plus_b");
    loops::Func min_max_scalar_func = make_min_max_scalar(ctx, "min_max_scalar");
    loops::Func ten_args_to_sum_func = make_ten_args_to_sum(ctx, "min_max_scalar");
    loops::Func min_max_select_func = make_min_max_select(ctx, "min_max_select");
    loops::Func triangle_types_func = make_triangle_types(ctx, "triangle_types");
    loops::Func nonnegative_odd_func = make_nonnegative_odd(ctx, "nonnegative_odd");
    loops::Func all_loads_all_stores_func = make_all_loads_all_stores(ctx, "all_loads_all_stores");
    loops::Func nullify_msb_lsb_func = make_nullify_msb_lsb(ctx, "nullify_msb_lsb");
    loops::Func bresenham_func = make_bresenham(ctx, "bresenham");
    loops::Func conditionpainter_func = make_conditionpainter(ctx, "conditionpainter");
    ctx.compileAll();
    test_a_plus_b(a_plus_b_func);
    test_min_max_scalar(min_max_scalar_func);
    test_ten_args_to_sum(ten_args_to_sum_func);
    test_min_max_select(min_max_select_func);
    test_triangle_types(triangle_types_func);
    test_nonnegative_odd(nonnegative_odd_func);
    test_all_loads_all_stores(all_loads_all_stores_func);
    test_nullify_msb_lsb(nullify_msb_lsb_func);
    test_bresenham(bresenham_func);
    test_conditionpainter(conditionpainter_func);
}