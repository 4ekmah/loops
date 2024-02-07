/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_TEST_BASIC_HPP__
#define __LOOPS_TEST_BASIC_HPP__

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
//DUBUG: Create test, which uses compile_all method.
TEST(basic, a_plus_b)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    IReg a, b;
    STARTFUNC_(test_info_->name(), &a, &b)
    {
        a+=b;
        RETURN_(a);
    }
    typedef int64_t (*a_plus_b_f)(int64_t a, int64_t b);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    a_plus_b_f tested = reinterpret_cast<a_plus_b_f>(func.ptr());
    std::vector<int64_t> A = {3,2,4,2,3,4,1};
    std::vector<int64_t> B = {4,4,5,5,4,6,5};
    for(size_t n = 0; n < A.size(); n++)
        ASSERT_EQ(tested(A[n],B[n]), A[n]+B[n]);
}

TEST(basic, min_max_scalar)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    IReg ptr, n, minpos_addr, maxpos_addr;
    STARTFUNC_(test_info_->name(), &ptr, &n, &minpos_addr, &maxpos_addr)
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
    typedef int64_t (*min_max_scalar_f)(const int* ptr, int64_t n, int* minpos, int* maxpos);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
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

#if __LOOPS_OS == __LOOPS_WINDOWS
#undef min // Windows.h implements min and max as macro.
#undef max //
#endif
TEST(basic, min_max_select)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    IReg ptr, n, minpos_addr, maxpos_addr;
    STARTFUNC_(test_info_->name(), &ptr, &n, &minpos_addr, &maxpos_addr)
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
    typedef int64_t (*min_max_select_f)(const int* ptr, int64_t n, int* minpos, int* maxpos);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
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

TEST(basic, triangle_types)
{
    enum {NOT_A_TRIANGLE, RIGHT_TRIANGLE, EQUILATERAL_TRIANGLE, ISOSCELES_TRIANGLE, ACUTE_TRIANGLE, OBTUSE_TRIANGLE};
    Context ctx;
    USE_CONTEXT_(ctx);
    IReg a, b, c;
    STARTFUNC_(test_info_->name(), &a, &b, &c)
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
    typedef int64_t (*triangle_types_f)(int64_t a, int64_t b, int64_t c);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
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

TEST(basic, nonnegative_odd)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    IReg ptr, n;
    STARTFUNC_(test_info_->name(), &ptr, &n)
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
    typedef int64_t (*nonnegative_odd_f)(const int* ptr, int64_t n);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
    nonnegative_odd_f tested = reinterpret_cast<nonnegative_odd_f>(func.ptr());
    std::vector<int> v = { 8, 2, -5, 7, 6 };
    ASSERT_EQ(tested(&v[0], v.size()), 3);
    ASSERT_EQ(tested(0, 0), -1);
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

TEST(basic, all_loads_all_stores)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    {
        IReg iptr, ityp, optr, otyp, n;
        STARTFUNC_(test_info_->name(), &iptr, &ityp, &optr, &otyp, &n)
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
    }
    typedef int64_t (*all_loads_all_stores_f)(const void* iptr, int ityp, void* optr, int otyp, int64_t n);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
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
TEST(basic, nullify_msb_lsb)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    {
        IReg in, elsb, emsb;
        STARTFUNC_(test_info_->name(), &in, &elsb, &emsb)
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
    }
    typedef int64_t (*nullify_msb_lsb_f)(uint64_t in, uint64_t* elsb, uint64_t* emsb);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
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

//Implementations(loops and reference) is taken from wikipedia: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
TEST(basic, bresenham)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    {
        IReg canvas, w, x0, y0, x1, y1, filler;
        STARTFUNC_(test_info_->name(), &canvas, &w, &x0, &y0, &x1, &y1, &filler)
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
    }
    typedef void (*bresenham_f)(uint8_t* canvas, int64_t w, int64_t x0, int64_t y0, int64_t x1, int64_t y1, uint64_t filler);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
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

TEST(basic, conditionpainter)
{
    const int xmin = -5, xmax = 5, ymin = -5, ymax = 5;
    const int h = ymax - ymin + 1;
    const int w = xmax - xmin + 1;
    Context ctx;
    USE_CONTEXT_(ctx);
    IReg ptr;
    STARTFUNC_(test_info_->name(), &ptr)
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
    typedef int64_t (*conditionpainter_f)(int64_t* canvas);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT(func);
    EXPECT_ASSEMBLY_CORRECT(func);
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
    int64_t canvas_ref[w*h];
    int64_t canvas[3*w*h];
    memset(canvas_ref, 0, w*h*sizeof(int64_t));
    memset(canvas, 0, 3*w*h*sizeof(int64_t));
    tested(canvas + w*h);
    ASSERT_EQ(memok((uint8_t*)&canvas[0], w * sizeof(int64_t), h), true);
    conditionpainter_ref(canvas_ref);
    for(int y = 0; y < h; y++)
        for(int x = 0; x < w; x++)
            ASSERT_EQ(canvas_ref[y*w + x], canvas[y*w + x + w*h]);
}

static void hw()
{
    get_test_ostream()<<"Hello world!"<<std::endl;
}
TEST(calls, helloworld_call)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    STARTFUNC_(test_info_->name())
    {
        CALL_(hw);
        RETURN_();
    }
    typedef void(*helloworld_call_f)();
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    // EXPECT_IR_CORRECT(func);  //DUBUG: switch on or what?
    // EXPECT_ASSEMBLY_CORRECT(func);
    helloworld_call_f tested = reinterpret_cast<helloworld_call_f>(func.ptr());
    reset_test_ostream();
    tested();
    std::string res = get_test_ostream_result();
    reset_test_ostream();
    hw();
    std::string refres = get_test_ostream_result();
    reset_test_ostream();
    ASSERT_EQ(res, refres);
}

static void snake_dprint(int64_t x, int64_t y)
{
    get_test_ostream()<<"(x, y) = ("<< (int)x << ", " << (int)y << ")" << std::endl;
}
TEST(calls, snake)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    {
        IReg ptr, h, w;
        STARTFUNC_(test_info_->name(), &ptr, &h, &w)
        {
            IReg diagamount = (h + w - 1);
            IReg curvalue = CONST_(0);
            IReg curdx = CONST_(1);
            IReg curdy = -curdx;
            IReg dn = CONST_(0);
            WHILE_(dn < diagamount)
            {
                IReg x = CONST_(0);
                IReg y = CONST_(0);
                IF_(dn&1) //Diagonal moving down
                {
                    IReg wm1 = w - 1;
                    x = min(wm1, dn);
                    y = select(dn > wm1, dn - (wm1), 0);
                }
                ELSE_ //Diagonal moving up
                {
                    IReg hm1 = h - 1;
                    x = select(dn > hm1, dn - (hm1), 0);
                    y = min(hm1, dn);
                }
                WHILE_(x>=0 && x < w && y >= 0 && y < h)
                {
                    CALL_(snake_dprint, x, y);
                    store_<uint8_t>(ptr, y * w + x, curvalue);
                    curvalue += 1;
                    x += curdx;
                    y += curdy;
                }
                curdx = -curdx;
                curdy = -curdy;
                dn += 1;
            }
            RETURN_();
        }
    }
    typedef void(*snake_f)(uint8_t*, int64_t, int64_t);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    // EXPECT_IR_CORRECT(func);  //DUBUG: switch on or what?
    // EXPECT_ASSEMBLY_CORRECT(func);
    snake_f tested = reinterpret_cast<snake_f>(func.ptr());
    const int h = 10, w = 5;
    uint8_t canvas[3 * h*w];
    uint8_t canvas_ref[h*w];
    memset(canvas, 0, 3*h*w);
    memset(canvas_ref, 0, h*w);
    reset_test_ostream();
    tested(&(canvas[h*w]), h, w);
    ASSERT_EQ(memok(&canvas[0], w, h), true);
    std::string dprint = get_test_ostream_result();
    reset_test_ostream();
    int diagamount = (h + w - 1);
    uint8_t curvalue = 0;
    int curdx = 1;
    int curdy = -1;
    for(int dn = 0; dn < diagamount; dn++)
    {
        int x = 0;
        int y = 0;
        if(dn&1) //Diagonal moving down
        {
            if(dn > w - 1)
            {
                x = w-1;
                y = dn - (w-1);
            }
            else
            {
                x = dn;
                y = 0;
            }
        }
        else //Diagonal moving up
        {
            if(dn > h-1)
            {
                x = dn - (h-1);
                y = h-1;
            }
            else
            {
                x = 0;
                y = dn;
            }
        }
        while(x>=0 && x < w && y >= 0 && y < h)
        {
            get_test_ostream()<<"(x, y) = ("<< (int)x << ", " << (int)y << ")" << std::endl;
            canvas_ref[y * w + x] = curvalue;
            curvalue ++;
            x += curdx;
            y += curdy;
        }
        curdx = -curdx;
        curdy = -curdy;
    }
    std::string dprint_ref = get_test_ostream_result();
    reset_test_ostream();
    for(int y = 0; y < h ; y++)
        for(int x = 0; x < w ; x++)
            ASSERT_EQ(canvas[y * w + x + h*w], canvas_ref[y * w + x]);
    ASSERT_EQ(dprint, dprint_ref);
}

static int64_t lesser_dbl(int64_t a, int64_t b)
{
    union uconv_ //TODO: create general template for conversion. 
    {
        int64_t val64;
        double val;
        uconv_() : val64(0) {} 
    } conv0, conv1;
    conv0.val64 = a;
    conv1.val64 = b;
    return conv0.val < conv1.val;
}
TEST(calls, sort_double)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    IReg ptr, n;
    STARTFUNC_(test_info_->name(), &ptr, &n)
    {
        n <<= 3; //*sizeof(double)
        IReg nm1 = n - sizeof(double);
        IReg curpos = CONST_(0);
        WHILE_(curpos < nm1)
        {
            IReg minpos = curpos;
            IReg ipos = minpos + sizeof(double);
            WHILE_(ipos < n)
            {
                // IF_(CALL_(lesser_dbl, load_<double>(ptr, ipos), load_<double>(ptr, minpos))) //TODO(ch): There is some bug on intel here, causing segfault. Uncomment and fix it. 
                IF_(CALL_(lesser_dbl, load_<double>(ptr + ipos), load_<double>(ptr + minpos)))
                    minpos = ipos;
                ipos += sizeof(double);
            }
            IF_(minpos != curpos)
            {
                //TODO(ch): There is some bug on intel here, causing segfault. Uncomment next four lines and fix it:
                // IReg cur_ = load_<double>(ptr, curpos); 
                // IReg min_ = load_<double>(ptr, minpos);
                // store_<double>(ptr, minpos, cur_);
                // store_<double>(ptr, curpos, min_);
                IReg cur_ = load_<double>(ptr + curpos); 
                IReg min_ = load_<double>(ptr + minpos);
                store_<double>(ptr + minpos, cur_);
                store_<double>(ptr + curpos, min_);
            }
            curpos += sizeof(double);
        }
        RETURN_();
    }
    typedef void(*sort_double_f)(double*, int64_t);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    // EXPECT_IR_CORRECT(func);  //DUBUG: switch on or what?
    // EXPECT_ASSEMBLY_CORRECT(func);
        sort_double_f tested = reinterpret_cast<sort_double_f>(func.ptr());
    std::vector<double> arr = {7.3, 2.0, 5.3, 10.0, -500000.0, -17.0, 70.0, 1.9, 71212.7878, 12.0};
    std::vector<double> arr_ref = arr;
    tested(&(arr[0]), arr.size());
    std::sort(arr_ref.begin(), arr_ref.end());
    for(int pos = 0; pos < (int)arr_ref.size(); pos++)
        ASSERT_EQ(arr[pos], arr_ref[pos]);

}

#endif//__LOOPS_TEST_BASIC_HPP__
