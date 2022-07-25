/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_TEST_BASIC_HPP__
#define __LOOPS_TEST_BASIC_HPP__

#include "loops/loops.hpp"
#include "tests.hpp"
#include <iostream>
#include "../src/common.hpp"        //TODO(ch): .. in path is bad practice. Configure project
#include "../src/func_impl.hpp"     //TODO(ch): .. in path is bad practice. Configure project
#include "../src/reg_allocator.hpp" //TODO(ch): .. in path is bad practice. Configure project

namespace loops
{
LTEST(a_plus_b, {
    IReg a, b;
    STARTFUNC_(TESTNAME, &a, &b)
    {
        a+=b;
        RETURN_(a);
    }
});
LTESTexe(a_plus_b, {
    typedef int64_t (*a_plus_b_f)(int64_t a, int64_t b);
    a_plus_b_f tested = reinterpret_cast<a_plus_b_f>(EXEPTR);
    std::vector<int64_t> A = {3,2,4,2,3,4,1};
    std::vector<int64_t> B = {4,4,5,5,4,6,5};
    for(size_t n = 0; n < A.size(); n++)
        TEST_EQ(tested(A[n],B[n]), A[n]+B[n]);
});

LTEST(min_max_scalar, {
    IReg ptr, n, minpos_addr, maxpos_addr;
    STARTFUNC_(TESTNAME, &ptr, &n, &minpos_addr, &maxpos_addr)
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
});
LTESTexe(min_max_scalar, {
    typedef int (*minmaxscalar_f)(const int* ptr, int64_t n, int* minpos, int* maxpos);
    std::vector<int> v = { 8, 2, -5, 7, 6 };
    int minpos = -1, maxpos = -1;
    minmaxscalar_f tested = reinterpret_cast<minmaxscalar_f>(EXEPTR);
    int retval = tested(&v[0], v.size(), &minpos, &maxpos);
    int rminpos = 0, rmaxpos = 0;
    for(size_t num = 0; num < v.size(); num++)
    {
        if(v[rminpos] > v[num])
            rminpos = v[num];
        if(v[rmaxpos] < v[num])
            rmaxpos = v[num];
    }
    TEST_EQ(rmaxpos, maxpos);
    TEST_EQ(rminpos, minpos);
    TEST_EQ(retval, 0);
});

LTEST(min_max_select, {
    using namespace loops;
    IReg ptr, n, minpos_addr, maxpos_addr;
    STARTFUNC_(TESTNAME, &ptr, &n, &minpos_addr, &maxpos_addr)
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
        IReg elemsize = CONST_(sizeof(int));
        minpos >>= 2;// sizeof(int);
        maxpos >>= 2;// sizeof(int);
        store_<int>(minpos_addr, minpos);
        store_<int>(maxpos_addr, maxpos);
        RETURN_(0);
    }
    });
LTESTexe(min_max_select, {
    typedef int (*minmaxselect_f)(const int* ptr, int64_t n, int* minpos, int* maxpos);
    std::vector<int> v = { 8, 2, -5, 7, 6 };
    int minpos = -1, maxpos = -1;
    minmaxselect_f tested = reinterpret_cast<minmaxselect_f>(EXEPTR);
    int retval = tested(&v[0], v.size(), &minpos, &maxpos);
    int rminpos = 0, rmaxpos = 0;
    for (size_t num = 0; num < v.size(); num++)
    {
        if (v[rminpos] > v[num])
            rminpos = v[num];
        if (v[rmaxpos] < v[num])
            rmaxpos = v[num];
    }
    TEST_EQ(rmaxpos, maxpos);
    TEST_EQ(rminpos, minpos);
    TEST_EQ(retval, 0);
    });

enum {NOT_A_TRIANGLE, RIGHT_TRIANGLE, EQUILATERAL_TRIANGLE, ISOSCELES_TRIANGLE, ACUTE_TRIANGLE, OBTUSE_TRIANGLE};
LTEST(triangle_types, {
    IReg a, b, c;
    STARTFUNC_(TESTNAME, &a, &b, &c)
    {
        IF_(a <= 0)
            RETURN_(NOT_A_TRIANGLE);
        ELIF_(b <= 0)
            RETURN_(NOT_A_TRIANGLE);
        ELIF_(c <= 0)
            RETURN_(NOT_A_TRIANGLE);
        ELSE_
        {
            IReg apb = a + b;
            IReg apc = a + c;
            IReg bpc = b + c;
            IF_(a > bpc)
                RETURN_(NOT_A_TRIANGLE);
            ELIF_(b > apc)
                RETURN_(NOT_A_TRIANGLE);
            ELIF_(c > apb)
                RETURN_(NOT_A_TRIANGLE);
            ELSE_
            {
                IF_(a == b)
                {
                    IF_(b == c)
                        RETURN_(EQUILATERAL_TRIANGLE);
                    ELSE_
                        RETURN_(ISOSCELES_TRIANGLE);
                }
                ELIF_(a == c)
                    RETURN_(ISOSCELES_TRIANGLE);
                ELIF_(b == c)
                    RETURN_(ISOSCELES_TRIANGLE);

                IReg a2 = a*a;
                IReg b2 = b*b;
                IReg c2 = c*c;
                IReg a2pb2 = a2+b2;
                IReg a2pc2 = a2+c2;
                IReg b2pc2 = b2+c2;
                IF_(a2 == b2pc2)
                    RETURN_(RIGHT_TRIANGLE);
                ELIF_(b2 == a2pc2)
                    RETURN_(RIGHT_TRIANGLE);
                ELIF_(c2 == a2pb2)
                    RETURN_(RIGHT_TRIANGLE);

                IReg cosa = b2pc2 - a2;
                IReg cosb = a2pc2 - b2;
                IReg cosc = a2pb2 - c2;
                IF_(cosa < 0)
                    RETURN_(OBTUSE_TRIANGLE);
                ELIF_(cosb < 0)
                    RETURN_(OBTUSE_TRIANGLE);
                ELIF_(cosc < 0)
                    RETURN_(OBTUSE_TRIANGLE);
            }
        }
        RETURN_(ACUTE_TRIANGLE);
    }
});
LTESTexe(triangle_types, {
    typedef int (*triangle_types_f)(int64_t a, int64_t b, int64_t c);
    triangle_types_f tested = reinterpret_cast<triangle_types_f>(EXEPTR);
    TEST_EQ(tested(-1,2,3), int(NOT_A_TRIANGLE));
    TEST_EQ(tested(1,-2,3), int(NOT_A_TRIANGLE));
    TEST_EQ(tested(1,2,-3), int(NOT_A_TRIANGLE));
    TEST_EQ(tested(1,2,5), int(NOT_A_TRIANGLE));
    TEST_EQ(tested(5,2,1), int(NOT_A_TRIANGLE));
    TEST_EQ(tested(1,5,2), int(NOT_A_TRIANGLE));
    TEST_EQ(tested(1,1,1), int(EQUILATERAL_TRIANGLE));
    TEST_EQ(tested(1,1,2), int(ISOSCELES_TRIANGLE));
    TEST_EQ(tested(1,2,1), int(ISOSCELES_TRIANGLE));
    TEST_EQ(tested(2,1,1), int(ISOSCELES_TRIANGLE));
    TEST_EQ(tested(3,4,5), int(RIGHT_TRIANGLE));
    TEST_EQ(tested(5,4,3), int(RIGHT_TRIANGLE));
    TEST_EQ(tested(3,5,4), int(RIGHT_TRIANGLE));
    TEST_EQ(tested(5,6,9), int(OBTUSE_TRIANGLE));
    TEST_EQ(tested(9,5,6), int(OBTUSE_TRIANGLE));
    TEST_EQ(tested(5,9,6), int(OBTUSE_TRIANGLE));
    TEST_EQ(tested(7,5,6), int(ACUTE_TRIANGLE));
});

LTEST(nonnegative_odd, {
    IReg ptr, n;
    STARTFUNC_(TESTNAME, &ptr, &n)
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
});
LTESTexe(nonnegative_odd, {
    typedef int (*nonnegative_odd_f)(const int* ptr, int64_t n);
    nonnegative_odd_f tested = reinterpret_cast<nonnegative_odd_f>(EXEPTR);
    std::vector<int> v = { 8, 2, -5, 7, 6 };
    TEST_EQ(tested(&v[0], v.size()), 3);
    TEST_EQ(tested(0, 0), -1);
});

LTEST(all_loads_all_stores, {
    IReg iptr, ityp, optr, otyp, n;
    STARTFUNC_(TESTNAME, &iptr, &ityp, &optr, &otyp, &n )
    {
        if (CTX.getPlatformName() == "AArch64")
            getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, { 0, 1, 2, 3, 4 }, { 0, 1, 2, 3, 4 }, {}, { 18, 19, 20, 21, 22 });
        else if("Intel64" && OSname() == "Linux") 
            getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, { 7, 6, 2, 1, 8 }, { 0 }, {}, { 12, 13, 14, 15 });

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
    });
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
LTESTexe(all_loads_all_stores, {
    typedef int64_t(*all_loads_all_stores_f)(const void* iptr, int ityp, void* optr, int otyp, int64_t n);
    all_loads_all_stores_f tested = reinterpret_cast<all_loads_all_stores_f>(EXEPTR);

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
            TEST_EQ(chck, true);
        }
    });

LTEST(nullify_msb_lsb, {
    IReg in, elsb, emsb;
    STARTFUNC_(TESTNAME, &in, &elsb, &emsb )
    {
        IReg msb = in | ushift_right(in,1);
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
    });
LTESTexe(nullify_msb_lsb, {
    typedef void (*erode_msb_lsb_f)(uint64_t in, uint64_t* elsb, uint64_t* emsb);
    erode_msb_lsb_f tested = reinterpret_cast<erode_msb_lsb_f>(EXEPTR);
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
        TEST_EQ(remsb, emsb);
        TEST_EQ(relsb, elsb);
    }
    });

//This implementation(loops and reference) is taken from wikipedia: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
LTEST(bresenham, {
    IReg canvas, w, x0, y0, x1, y1, filler;
    STARTFUNC_(TESTNAME, &canvas, &w, &x0, &y0, &x1, &y1, &filler)
    {
        if (CTX.getPlatformName() == "AArch64")
            getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, { 0, 1, 2, 3, 4, 5, 6 }, { 0, 1, 2, 3, 4, 5, 6 }, {}, { 18, 19, 20, 21, 22 });
        else if("Intel64" && OSname() == "Linux") 
            getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, { 7, 6, 2, 1, 8, 9 }, { 0 }, {}, { 12, 13, 14, 15 });

        IReg dx = abs(x1 - x0);
        IReg sx = sign(x1 - x0);
        IReg dy = -abs(y1 - y0);
        IReg sy = sign(y1 - y0);
        IReg error = dx + dy;
        WHILE_(canvas != 0)      //TODO(ch): this is substitution of while(true)
        {
            store_<uint8_t>(canvas, y0* w + x0, filler);
            IF_(x0 == x1)
                IF_(y0 == y1)
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
    });
static void BresenhamRef(uint8_t* canvas, int64_t w, int64_t x0, int64_t y0, int64_t x1, int64_t y1, uint64_t filler)
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
}
LTESTexe(bresenham, {
    typedef void (*bresenham_f)(uint8_t* canvas, int64_t w, int64_t x0, int64_t y0, int64_t x1, int64_t y1, uint64_t filler);
    bresenham_f tested = reinterpret_cast<bresenham_f>(EXEPTR);
    int64_t w = 15;
    int64_t h = 15;
    std::vector<uint8_t> canvas(w * h, ' ');
    tested(&canvas[0], w, 0, 0, 14, 14, '1');
    tested(&canvas[0], w, 0, 11, 7, 7, '2');
    tested(&canvas[0], w, 14, 7, 11, 14, '3');
    tested(&canvas[0], w, 5, 3, 8, 3, '4');
    tested(&canvas[0], w, 9, 2, 9, 13, '5');
    tested(&canvas[0], w, 14, 1, 14, 1, '6');
    std::vector<uint8_t> canvasRef(w * h, ' ');
    BresenhamRef(&canvasRef[0], w, 0, 0, 14, 14, '1');
    BresenhamRef(&canvasRef[0], w, 0, 11, 7, 7, '2');
    BresenhamRef(&canvasRef[0], w, 14, 7, 11, 14, '3');
    BresenhamRef(&canvasRef[0], w, 5, 3, 8, 3, '4');
    BresenhamRef(&canvasRef[0], w, 9, 2, 9, 13, '5');
    BresenhamRef(&canvasRef[0], w, 14, 1, 14, 1, '6');
    for (int symbn = 0; symbn < w * h; symbn++)
        TEST_EQ(canvas[symbn], canvasRef[symbn]);
    });
};
#endif//__LOOPS_TEST_BASIC_HPP__
