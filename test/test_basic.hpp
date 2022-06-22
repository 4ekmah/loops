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
    CTX.startFunc(TESTNAME, {&a, &b});
    a+=b;
    CTX.return_(a);
    CTX.endFunc();
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
    CTX.startFunc(TESTNAME, {&ptr, &n, &minpos_addr, &maxpos_addr});
    IReg i = CTX.const_(0);
    IReg minpos = CTX.const_(0);
    IReg maxpos = CTX.const_(0);
    IReg minval = load_<int>(ptr);
    IReg maxval = minval;
    n *= sizeof(int);
    CTX.do_();
        IReg x = load_<int>(ptr, i);
        CTX.if_(x < minval);
            minval = x;
            minpos = i;
        CTX.endif_();
        CTX.if_(x > maxval);
            maxval = x;
            maxpos = i;
        CTX.endif_();
        i += sizeof(int);
    CTX.while_(i < n);
    IReg elemsize = CTX.const_(sizeof(int));
    minpos /= elemsize;
    maxpos /= elemsize;
    store_<int>(minpos_addr, minpos);
    store_<int>(maxpos_addr, maxpos);
    CTX.return_(0);
    CTX.endFunc();
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
    CTX.startFunc(TESTNAME, {&ptr, &n, &minpos_addr, &maxpos_addr});
    IReg i = CTX.const_(0);
    IReg minpos = CTX.const_(0);
    IReg maxpos = CTX.const_(0);
    IReg minval = load_<int>(ptr);
    IReg maxval = minval;
    n <<= 2;// sizeof(int) == 4;
    CTX.do_();
        IReg x = load_<int>(ptr, i);
        minpos = select(x < minval, i, minpos);
        minval = min(x, minval);
        maxpos = select(x > maxval, i, maxpos);
        maxval = max(x, maxval);
        i += sizeof(int);
    CTX.while_(i < n);
    IReg elemsize = CTX.const_(sizeof(int));
    minpos >>= 2;// sizeof(int);
    maxpos >>= 2;// sizeof(int);
    store_<int>(minpos_addr, minpos);
    store_<int>(maxpos_addr, maxpos);
    CTX.return_(0);
    CTX.endFunc();
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
    CTX.startFunc(TESTNAME, {&a,&b,&c});
    CTX.if_(a <= 0);
        CTX.return_(NOT_A_TRIANGLE);
    CTX.elif_(b <= 0);
        CTX.return_(NOT_A_TRIANGLE);
    CTX.elif_(c <= 0);
        CTX.return_(NOT_A_TRIANGLE);
    CTX.else_();
        IReg apb = a + b;
        IReg apc = a + c;
        IReg bpc = b + c;
        CTX.if_(a > bpc);
            CTX.return_(NOT_A_TRIANGLE);
        CTX.elif_(b > apc);
            CTX.return_(NOT_A_TRIANGLE);
        CTX.elif_(c > apb);
            CTX.return_(NOT_A_TRIANGLE);
        CTX.else_();
            CTX.if_(a == b);
                CTX.if_(b == c);
                    CTX.return_(EQUILATERAL_TRIANGLE);
                CTX.else_();
                    CTX.return_(ISOSCELES_TRIANGLE);
                CTX.endif_();
            CTX.elif_(a == c);
                CTX.return_(ISOSCELES_TRIANGLE);
            CTX.elif_(b == c);
                CTX.return_(ISOSCELES_TRIANGLE);
            CTX.endif_();

            IReg a2 = a*a;
            IReg b2 = b*b;
            IReg c2 = c*c;
            IReg a2pb2 = a2+b2;
            IReg a2pc2 = a2+c2;
            IReg b2pc2 = b2+c2;
            CTX.if_(a2 == b2pc2);
                CTX.return_(RIGHT_TRIANGLE);
            CTX.elif_(b2 == a2pc2);
                CTX.return_(RIGHT_TRIANGLE);
            CTX.elif_(c2 == a2pb2);
                CTX.return_(RIGHT_TRIANGLE);
            CTX.endif_();

            IReg cosa = b2pc2 - a2;
            IReg cosb = a2pc2 - b2;
            IReg cosc = a2pb2 - c2;
            CTX.if_(cosa < 0);
                CTX.return_(OBTUSE_TRIANGLE);
            CTX.elif_(cosb < 0);
                CTX.return_(OBTUSE_TRIANGLE);
            CTX.elif_(cosc < 0);
                CTX.return_(OBTUSE_TRIANGLE);
            CTX.endif_();

        CTX.endif_();
    CTX.endif_();
    CTX.return_(ACUTE_TRIANGLE);
    CTX.endFunc();
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
    IReg ptr, n, toFind;
    CTX.startFunc(TESTNAME, {&ptr, &n});
    IReg i = CTX.const_(0);
    IReg res = CTX.const_(-(int64_t)sizeof(int));
    n *= sizeof(int);
    CTX.doif_(i < n);
        IReg x = load_<int>(ptr, i);
        CTX.if_(x < 0);
            i += sizeof(int);
            CTX.continue_();
        CTX.endif_();
        IReg mod = x % 2;
        CTX.if_(mod != 0);
            res = i;
            CTX.break_();
        CTX.endif_();
        i += sizeof(int);
    CTX.enddo_();
    res /= sizeof(int);
    CTX.return_(res);
    CTX.endFunc();
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
    CTX.startFunc(TESTNAME, { &iptr, &ityp, &optr, &otyp, &n });
    IReg num = CTX.const_(0);
    IReg i_offset = CTX.const_(0);
    IReg o_offset = CTX.const_(0);
    IReg ielemsize = select(ityp > TYPE_I8, CTX.const_(2), CTX.const_(1));
    ielemsize = select(ityp > TYPE_I16, CTX.const_(4), ielemsize);
    ielemsize = select(ityp > TYPE_I32, CTX.const_(8), ielemsize);
    IReg oelemsize = select(otyp > TYPE_I8, CTX.const_(2), CTX.const_(1));
    oelemsize = select(otyp > TYPE_I16, CTX.const_(4), oelemsize);
    oelemsize = select(otyp > TYPE_I32, CTX.const_(8), oelemsize);
    CTX.do_();
        IReg x = CTX.const_(0); //TODO(ch): we need to have variable definition without init value(probably with context reference or something...). [I think, we can introduce pure def instruction, which will dissappear on liveness analysis].
        CTX.if_(ityp == TYPE_U8);
            x = load_<uint8_t>(iptr, i_offset);
        CTX.elif_(ityp == TYPE_I8);
            x = load_<int8_t>(iptr, i_offset);
        CTX.elif_(ityp == TYPE_U16);
            x = load_<uint16_t>(iptr, i_offset);
        CTX.elif_(ityp == TYPE_I16);
            x = load_<int16_t>(iptr, i_offset);
        CTX.elif_(ityp == TYPE_U32);
            x = load_<uint32_t>(iptr, i_offset);
        CTX.elif_(ityp == TYPE_I32);
            x = load_<int32_t>(iptr, i_offset);
        CTX.elif_(ityp == TYPE_U64);
            x = load_<uint64_t>(iptr, i_offset);
        CTX.else_();
            x = load_<int64_t>(iptr, i_offset);
        CTX.endif_();

        CTX.if_(otyp == TYPE_U8);
            store_<uint8_t>(optr, o_offset, x);
        CTX.elif_(otyp == TYPE_I8);
            store_<int8_t>(optr, o_offset, x);
        CTX.elif_(otyp == TYPE_U16);
            store_<uint16_t>(optr, o_offset, x);
        CTX.elif_(otyp == TYPE_I16);
            store_<int16_t>(optr, o_offset, x);
        CTX.elif_(otyp == TYPE_U32);
            store_<uint32_t>(optr, o_offset, x);
        CTX.elif_(otyp == TYPE_I32);
            store_<int32_t>(optr, o_offset, x);
        CTX.elif_(otyp == TYPE_U64);
            store_<uint64_t>(optr, o_offset, x);
        CTX.else_();
            store_<int64_t>(optr, o_offset, x);
        CTX.endif_();

        i_offset += ielemsize;
        o_offset += oelemsize;
        num += 1;
    CTX.while_(num < n);
    if (CTX.getPlatformName() == "AArch64")
    {
        getImpl(&CTX)->getRegisterAllocator()->getRegisterPool().overrideRegisterSet(makeBitmask64({ 0, 1, 2, 3, 4 }), makeBitmask64({ 0, 1, 2, 3, 4 }), makeBitmask64({}), makeBitmask64({ 18, 19, 20, 21, 22 }));
        CTX.endFunc();
        getImpl(&CTX)->getRegisterAllocator()->getRegisterPool().overrideRegisterSet(0, 0, 0, 0);
    }
    else
        CTX.endFunc();
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

LTEST(erode_msb_lsb, {
    IReg in, elsb, emsb;
    CTX.startFunc(TESTNAME, {&in, &elsb, &emsb });
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
    CTX.return_();
    CTX.endFunc();
    });
LTESTexe(erode_msb_lsb, {
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
    CTX.startFunc(TESTNAME, {&canvas, &w, &x0, &y0, &x1, &y1, &filler });
    IReg dx = abs(x1 - x0);
    IReg sx = sign(x1 - x0);
    IReg dy = -abs(y1 - y0);
    IReg sy = sign(y1 - y0);
    IReg error = dx + dy;
    CTX.doif_(canvas != 0); //TODO(ch): this is substitution of while(true)
        store_<uint8_t>(canvas, y0* w + x0, filler);
        CTX.if_(x0 == x1);
            CTX.if_(y0 == y1);
                CTX.break_();
            CTX.endif_();
        CTX.endif_();
        IReg e2 = error << 1;
        CTX.if_(e2 >= dy);
            CTX.if_(x0 == x1); //looks like it break of "if".
                CTX.break_();
            CTX.endif_();
            error = error + dy;
            x0 = x0 + sx;
        CTX.endif_();
        CTX.if_(e2 <= dx);
            CTX.if_(y0 == y1); //looks like it break of "if".
                CTX.break_();
            CTX.endif_();
            error = error + dx;
            y0 = y0 + sy;
        CTX.endif_();
    CTX.enddo_();
    CTX.return_();
    if (CTX.getPlatformName() == "AArch64")
    {
        getImpl(&CTX)->getRegisterAllocator()->getRegisterPool().overrideRegisterSet(makeBitmask64({ 0, 1, 2, 3, 4, 5, 6 }), makeBitmask64({ 0, 1, 2, 3, 4, 5, 6 }), makeBitmask64({}), makeBitmask64({ 18, 19, 20, 21, 22 }));
        CTX.endFunc();
        getImpl(&CTX)->getRegisterAllocator()->getRegisterPool().overrideRegisterSet(0, 0, 0, 0);
    }
    else 
        CTX.endFunc();
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
