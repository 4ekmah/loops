/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#ifndef __LOOPS_TEST_AARCH64_HPP__
#define __LOOPS_TEST_AARCH64_HPP__
#include "loops/loops.hpp"
#if __LOOPS_ARCH == __LOOPS_AARCH64
#include "tests.hpp"
#include <iostream>
#include <vector>

namespace loops
{
LTEST(ten_args_to_sum, { //There we are testing stack parameter passing.
    IReg a0, a1, a2, a3, a4, a5, a6, a7, a8, a9;
    STARTFUNC_(TESTNAME, &a0, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9)
    {
        getImpl(getImpl(&CTX)->getCurrentFunc())->overrideRegisterSet(RB_INT, { 0, 1, 2, 3, 4, 5, 6, 7 }, { 0, 1, 2, 3, 4, 5, 6, 7 }, {}, { 18, 19, 20, 21, 22 });
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
    });
LTESTexe(ten_args_to_sum, {
    typedef int64_t(*ten_args_to_sum_f)(int64_t a0, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6, int64_t a7, int64_t a8, int64_t a9);
    ten_args_to_sum_f tested = reinterpret_cast<ten_args_to_sum_f>(EXEPTR);
    std::vector<int> v = { 1,1,1,1,1,1,1,1,3,5 };
    EXPECT_EQ(tested(v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9]),(int64_t)(55));
    });

LTEST(nullify_msb_lsb_v, {
    IReg iptr, omptr, olptr, n;
    STARTFUNC_(TESTNAME, &iptr, &omptr, &olptr, &n)
    {
        IReg offset  = CONST_(0);
        n *= sizeof(uint32_t);
        VReg<uint32_t> one  = VCONST_(uint32_t, 1);
        WHILE_(offset < n)
        {
            VReg<uint32_t> in = loadvec<uint32_t>(iptr, offset);
            VReg<uint32_t> msb = in | ushift_right(in,1);
            msb |= ushift_right(msb,  2);
            msb |= ushift_right(msb,  4);
            msb |= ushift_right(msb,  8);
            msb |= ushift_right(msb, 16);
            msb += one;  //It's assumed, that 0x80000000 bit is switched off.
            msb = ushift_right(msb, 1);
            msb ^= in;
            storevec(omptr, offset, msb);
            VReg<uint32_t> lsb = in & ~(in - one);
            lsb ^= in;
            storevec(olptr, offset, lsb);
            offset += CTX.vbytes();
        }
        RETURN_();
    }
    });
LTESTexe(nullify_msb_lsb_v, {
    typedef uint32_t (*clear_lsb_msb_v)(const uint32_t* src, uint32_t* msbdest, uint32_t* lsbdest, int64_t n);
    clear_lsb_msb_v tested = reinterpret_cast<clear_lsb_msb_v>(EXEPTR);
    std::vector<uint32_t> v =   { 0x60000000, 2, 0xf0, 7, 0x0fffffff, 0b101010101, 1234, 4321};
    std::vector<uint32_t> lsb = { 0, 0, 0, 0, 0, 0, 0, 0 };
    std::vector<uint32_t> msb = { 0, 0, 0, 0, 0, 0, 0, 0 };
    tested(&v[0], &msb[0], &lsb[0], v.size());
    for (size_t vnum = 0; vnum < v.size(); vnum++ )
    {
        uint32_t tchk = v[vnum];
        uint32_t relsb = tchk ^ (tchk & ~(tchk - 1));
        uint32_t remsb = tchk | tchk >> 1;
        remsb |= remsb >> 2;
        remsb |= remsb >> 4;
        remsb |= remsb >> 8;
        remsb |= remsb >> 16;
        remsb = (remsb + 1) >> 1;
        remsb ^= tchk;
        EXPECT_EQ(msb[vnum], remsb);
        EXPECT_EQ(lsb[vnum], relsb);
    }
    });

static void hw()
{
    get_test_ostream()<<"Hello world!"<<std::endl;
}
LTEST(helloworld_call, {
    STARTFUNC_(TESTNAME)
    {
        CALL_(hw);
        RETURN_();
    }
    });
LTESTexe(helloworld_call, {
    typedef void(*helloworld_call_f)();
    helloworld_call_f tested = reinterpret_cast<helloworld_call_f>(EXEPTR);
    reset_test_ostream();
    tested();
    std::string res = get_test_ostream_result();
    reset_test_ostream();
    get_test_ostream()<<"Hello world!"<<std::endl;
    std::string refres = get_test_ostream_result();
    reset_test_ostream();
    EXPECT_EQ(res, refres);
    });

static void snake_dprint(int64_t x, int64_t y)
{
    get_test_ostream()<<"(x, y) = ("<< (int)x << ", " << (int)y << ")" << std::endl;
}
LTEST(snake, {
    IReg ptr, h, w;
    STARTFUNC_(TESTNAME, &ptr, &h, &w)
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
    });
LTESTexe(snake, {
    typedef void(*snake_f)(uint8_t*, int64_t, int64_t);
    snake_f tested = reinterpret_cast<snake_f>(EXEPTR);
    const int h = 10, w = 5;
    uint8_t canvas[3 * h*w];
    uint8_t canvas_ref[h*w];
    memset(canvas, 0, 3*h*w);
    memset(canvas_ref, 0, h*w);
    reset_test_ostream();
    tested(&(canvas[h*w]), h, w);
    EXPECT_EQ(memok(&canvas[0], w, h), true);
    std::string dprint = get_test_ostream_result();
    reset_test_ostream();
    int diagamount = (h + w - 1);
    int curvalue = 0;
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
            EXPECT_EQ(canvas[y * w + x + h*w], canvas_ref[y * w + x]);
    EXPECT_EQ(dprint, dprint_ref);
    });

static int64_t lesser_dbl(int64_t a, int64_t b)
{
    return (*(double*)(&a)) < (*(double*)(&b));
}
LTEST(sort_double, {
    IReg ptr, n;
    STARTFUNC_(TESTNAME, &ptr, &n)
    {
        n <<= 3; //*sizeof(double)
        IReg nm1 = n - 8;
        IReg curpos = CONST_(0);
        WHILE_(curpos < nm1)
        {
            IReg minpos = curpos + 8;
            IReg ipos = minpos;
            WHILE_(ipos < n)
            {
                IF_(CALL_(lesser_dbl, load_<double>(ptr, ipos), load_<double>(ptr, minpos)))
                    minpos = ipos;
                ipos += 8;
            }
            IF_(minpos != curpos)
            {
                IReg cur_ = load_<double>(ptr, curpos);
                IReg min_ = load_<double>(ptr, minpos);
                store_<double>(ptr, minpos, cur_);
                store_<double>(ptr, curpos, min_);
            }
            curpos += 8;
        }
        RETURN_();
    }
    });
LTESTexe(sort_double, {
    typedef void(*sort_double_f)(double*, int64_t);
    sort_double_f tested = reinterpret_cast<sort_double_f>(EXEPTR);
    std::vector<double> arr = {7.3, 2.0, 5.3, 10.0, -500000.0, -17.0, 70.0, 1.9, 71212.7878, 12.0};
    std::vector<double> arr_ref = arr;
    tested(&(arr[0]), arr.size());
    std::sort(arr_ref.begin(), arr_ref.end());
    for(int pos = 0; pos < arr_ref.size(); pos++)
        EXPECT_EQ(arr[pos], arr_ref[pos]);
    });

//TODO(ch)[IMPORTANT]: There obviously needed test, which prooves, that CALL_ of function, which uses vector, doesn't corrupt vector registers.

#define DEFINE_CERTAIN_REG(name, number) IReg name##_0; name##_0.func = _f; name##_0.idx = number; IExpr name##_1(name##_0); Expr name = name##_1.notype()
LTESTcomposer(instruction_set_test, {
    FuncImpl* _f = getImpl(getImpl(&CTX)->getCurrentFunc());
    DEFINE_CERTAIN_REG(x0, 0);
    DEFINE_CERTAIN_REG(w0, 0);
    DEFINE_CERTAIN_REG(x7, 7);
    DEFINE_CERTAIN_REG(x15, 15);
    DEFINE_CERTAIN_REG(w15, 15);
    Expr v0_16u = VExpr<uint8_t>::make(vregHid<uint8_t>(0,_f)).notype();
    Expr v31_16u = VExpr<uint8_t>::make(vregHid<uint8_t>(31,_f)).notype();
    Expr v0_8u = VExpr<uint16_t>::make(vregHid<uint16_t>(0,_f)).notype();
    Expr v31_8u = VExpr<uint16_t>::make(vregHid<uint16_t>(31,_f)).notype();
    Expr v0_4u = VExpr<uint32_t>::make(vregHid<uint32_t>(0,_f)).notype();
    Expr v31_4u = VExpr<uint32_t>::make(vregHid<uint32_t>(31,_f)).notype();
    Expr v0_2u = VExpr<uint64_t>::make(vregHid<uint64_t>(0,_f)).notype();
    Expr v31_2u = VExpr<uint64_t>::make(vregHid<uint64_t>(31,_f)).notype();
    Expr v0_16s = VExpr<int8_t>::make(vregHid<int8_t>(0,_f)).notype();
    Expr v1_16s = VExpr<int8_t>::make(vregHid<int8_t>(1,_f)).notype();
    Expr v31_16s = VExpr<int8_t>::make(vregHid<int8_t>(31,_f)).notype();
    Expr v0_8s = VExpr<int16_t>::make(vregHid<int16_t>(0,_f)).notype();
    Expr v1_8s = VExpr<int16_t>::make(vregHid<int16_t>(1,_f)).notype();
    Expr v31_8s = VExpr<int16_t>::make(vregHid<int16_t>(31,_f)).notype();
    Expr v0_4s = VExpr<int32_t>::make(vregHid<int32_t>(0,_f)).notype();
    Expr v1_4s = VExpr<int32_t>::make(vregHid<int32_t>(1,_f)).notype();
    Expr v31_4s = VExpr<int32_t>::make(vregHid<int32_t>(31,_f)).notype();
    Expr v0_2s = VExpr<int64_t>::make(vregHid<int64_t>(0,_f)).notype();
    Expr v1_2s = VExpr<int64_t>::make(vregHid<int64_t>(1,_f)).notype();
    Expr v31_2s = VExpr<int64_t>::make(vregHid<int64_t>(31,_f)).notype();
    Expr v0_8f = VExpr<f16_t>::make(vregHid<f16_t>(0,_f)).notype();
    Expr v15_8f = VExpr<f16_t>::make(vregHid<f16_t>(15,_f)).notype();
    Expr v31_8f = VExpr<f16_t>::make(vregHid<f16_t>(31,_f)).notype();
    Expr v0_4f = VExpr<float>::make(vregHid<float>(0,_f)).notype();
    Expr v31_4f = VExpr<float>::make(vregHid<float>(31,_f)).notype();
    Expr v0_2f = VExpr<double>::make(vregHid<double>(0,_f)).notype();
    Expr v31_2f = VExpr<double>::make(vregHid<double>(31,_f)).notype();
    Expr q0 = VExpr<uint64_t>::make(vregHid<uint64_t>(0,_f)).notype();
    Expr q31 = VExpr<uint64_t>::make(vregHid<uint64_t>(31,_f)).notype();
    
    newiopNoret(OP_MOV, {  x0, argIImm(         -1) });
    newiopNoret(OP_MOV, { x15, argIImm(         -1) });
    newiopNoret(OP_MOV, {  x0, argIImm(-(0xFFFF+1)) });

    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<uint64_t>(x0) });
    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<int64_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<uint64_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<int64_t>(x0) });

    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<uint32_t>(x0) });
    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<int32_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<uint32_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<int32_t>(x0) });

    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<uint16_t>(x0) });
    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<int16_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<uint16_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<int16_t>(x0) });

    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<uint8_t>(x0) });
    newiopNoret(OP_STORE, { x0, argIImm(256), iregtyped<int8_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<uint8_t>(x0) });
    newiopNoret(OP_STORE, { x0, x0, iregtyped<int8_t>(x0) });

    newiopNoret(OP_ARM_STP, {  x0, argIImm(  0),  x0,  x0 });
    newiopNoret(OP_ARM_STP, { x15, argIImm(  0),  x0,  x0 });
    newiopNoret(OP_ARM_STP, {  x0, argIImm( 63),  x0,  x0 });
    newiopNoret(OP_ARM_STP, {  x0, argIImm(-64),  x0,  x0 });
    newiopNoret(OP_ARM_STP, {  x0, argIImm(  0), x15,  x0 });
    newiopNoret(OP_ARM_STP, {  x0, argIImm(  0),  x0, x15 });

    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint64_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<int64_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint32_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<int32_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint16_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<int16_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<uint8_t>(x0), x0, x15});

    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x0), x0 });
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x0), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x15), x0, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x0), x15, argIImm(256)});
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x0), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x15), x0, x0});
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x0), x15, x0});
    newiopNoret(OP_LOAD, { iregtyped<int8_t>(x0), x0, x15});

    newiopNoret(OP_ARM_LDP, { x0, x7,  x15, argIImm(  0) });
    newiopNoret(OP_ARM_LDP, { x7, x0,  x15, argIImm(  0) });
    newiopNoret(OP_ARM_LDP, { x7, x15,  x0, argIImm(  0) });
    newiopNoret(OP_ARM_LDP, { x0, x7,  x15, argIImm( 63) });
    newiopNoret(OP_ARM_LDP, { x0, x7,  x15, argIImm(-64) });

    newiopNoret(OP_SELECT, { x0, OP_EQ, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_NE, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_LT, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_GT, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_LE, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_GE, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_S,  x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_NS, x0, x0 });

    newiopNoret(OP_SELECT, { x15, OP_EQ, x0, x0 });
    newiopNoret(OP_SELECT, { x0, OP_EQ, x15, x0 });
    newiopNoret(OP_SELECT, { x0, OP_EQ, x0, x15 });

    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_EQ });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_NE });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_LT });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_GT });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_LE });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_GE });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_S  });
    newiopNoret(OP_ARM_CINC, { x0 ,  x0, OP_NS });
    newiopNoret(OP_ARM_CINC, { x15,  x0, OP_EQ });
    newiopNoret(OP_ARM_CINC, { x0 , x15, OP_EQ });

    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_EQ });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_NE });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_LT });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_GT });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_LE });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_GE });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_S  });
    newiopNoret(OP_ARM_CNEG, { x0 ,  x0, OP_NS });
    newiopNoret(OP_ARM_CNEG, { x15,  x0, OP_EQ });
    newiopNoret(OP_ARM_CNEG, { x0 , x15, OP_EQ });

    newiopNoret(OP_SAR, { x0,  x0, argIImm(63) });
    newiopNoret(OP_SAR, { x15, x0, argIImm(63) });
    newiopNoret(OP_SAR, { x0, x15, argIImm(63) });

    newiopNoret(OP_SAR, { x0 ,  x0,  x0 });
    newiopNoret(OP_SAR, { x15,  x0,  x0 });
    newiopNoret(OP_SAR, { x0 , x15,  x0 });
    newiopNoret(OP_SAR, { x0 ,  x0, x15 });

    newiopNoret(OP_SHR, { x0,  x0, argIImm(63) });
    newiopNoret(OP_SHR, { x15, x0, argIImm(63) });
    newiopNoret(OP_SHR, { x0, x15, argIImm(63) });

    newiopNoret(OP_SHR, { x0 ,  x0,  x0 });
    newiopNoret(OP_SHR, { x15,  x0,  x0 });
    newiopNoret(OP_SHR, { x0 , x15,  x0 });
    newiopNoret(OP_SHR, { x0 ,  x0, x15 });

    newiopNoret(OP_SHL, { x0,  x0, argIImm(63) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(31) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(15) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(7) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(1) });
    newiopNoret(OP_SHL, { x0,  x0, argIImm(0) });

    newiopNoret(OP_SHL, { x15, x0, argIImm(63) });
    newiopNoret(OP_SHL, { x0, x15, argIImm(63) });

    newiopNoret(OP_SHL, { x0 ,  x0,  x0 });
    newiopNoret(OP_SHL, { x15,  x0,  x0 });
    newiopNoret(OP_SHL, { x0 , x15,  x0 });
    newiopNoret(OP_SHL, { x0 ,  x0, x15 });

    newiopNoret(OP_AND, { x0 ,   x0,  x0 });
    newiopNoret(OP_AND, { x15,   x0,  x0 });
    newiopNoret(OP_AND, { x0 ,  x15,  x0 });
    newiopNoret(OP_AND, { x0 ,  x0 , x15 });
    newiopNoret(OP_AND, { x0 ,  x0 , argIImm(0xFF) });
    newiopNoret(OP_AND, { x0 ,  x0 , argIImm(0xFF00) });
    newiopNoret(OP_AND, { x0 ,  x0 , argIImm(0xFF00000000000000) });
    newiopNoret(OP_AND, { x15,  x0 , argIImm(0xFF) });
    newiopNoret(OP_AND, { x0,  x15 , argIImm(0xFF) });

    newiopNoret(OP_OR, { x0 ,   x0,  x0 });
    newiopNoret(OP_OR, { x15,   x0,  x0 });
    newiopNoret(OP_OR, { x0 ,  x15,  x0 });
    newiopNoret(OP_OR, { x0 ,  x0 , x15 });
    newiopNoret(OP_OR, { x0 ,  x0 , argIImm(0xFF) });
    newiopNoret(OP_OR, { x0 ,  x0 , argIImm(0xFF00) });
    newiopNoret(OP_OR, { x0 ,  x0 , argIImm(0xFF00000000000000) });
    newiopNoret(OP_OR, { x15,  x0 , argIImm(0xFF) });
    newiopNoret(OP_OR, { x0,  x15 , argIImm(0xFF) });

    newiopNoret(OP_XOR, { x0 ,   x0,  x0 });
    newiopNoret(OP_XOR, { x15,   x0,  x0 });
    newiopNoret(OP_XOR, { x0 ,  x15,  x0 });
    newiopNoret(OP_XOR, { x0 ,  x0 , x15 });
    newiopNoret(OP_XOR, { x0 ,  x0 , argIImm(0xFF) });
    newiopNoret(OP_XOR, { x0 ,  x0 , argIImm(0xFF00) });
    newiopNoret(OP_XOR, { x0 ,  x0 , argIImm(0xFF00000000000000) });
    newiopNoret(OP_XOR, { x15,  x0 , argIImm(0xFF) });
    newiopNoret(OP_XOR, { x0,  x15 , argIImm(0xFF) });

    newiopNoret(OP_NOT, { x0 , x0 });
    newiopNoret(OP_NOT, { x15 , x0});
    newiopNoret(OP_NOT, { x0 , x15});

    newiopNoret(OP_NEG, { x0 , x0 });
    newiopNoret(OP_NEG, { x15 , x0});
    newiopNoret(OP_NEG, { x0 , x15});

    newiopNoret(VOP_LOAD, { q0 ,  x0 });
    newiopNoret(VOP_LOAD, { q0 ,  x0, argIImm(32784) });
    newiopNoret(VOP_LOAD, { q31,  x0 });
    newiopNoret(VOP_LOAD, { q0 , x15 });
    newiopNoret(VOP_LOAD, { q0 ,  x0, x0  });
    newiopNoret(VOP_LOAD, { q31,  x0, x0  });
    newiopNoret(VOP_LOAD, { q0 , x15, x0  });
    newiopNoret(VOP_LOAD, { q0 ,  x0, x15 });

    newiopNoret(VOP_STORE, { x0, q0 });
    newiopNoret(VOP_STORE, { x0, argIImm(32784), q0 });
    newiopNoret(VOP_STORE, {  x0, q31 });
    newiopNoret(VOP_STORE, { x15, q0 });
    newiopNoret(VOP_STORE, {  x0, x0 , q0  });
    newiopNoret(VOP_STORE, {  x0, x0 , q31 });
    newiopNoret(VOP_STORE, { x15, x0 , q0  });
    newiopNoret(VOP_STORE, {  x0, x15, q0  });

    newiopNoret(VOP_ADD, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_ADD, { v31_16u, v0_16u,  v0_16u  });
    newiopNoret(VOP_ADD, { v0_16u,  v31_16u, v0_16u  });
    newiopNoret(VOP_ADD, { v0_16u,  v0_16u,  v31_16u });
    newiopNoret(VOP_ADD, { v0_8u,  v0_8u,  v0_8u  });
    newiopNoret(VOP_ADD, { v31_8u, v0_8u,  v0_8u  });
    newiopNoret(VOP_ADD, { v0_8u,  v31_8u, v0_8u  });
    newiopNoret(VOP_ADD, { v0_8u,  v0_8u,  v31_8u });
    newiopNoret(VOP_ADD, { v0_4u,  v0_4u,  v0_4u  });
    newiopNoret(VOP_ADD, { v31_4u, v0_4u,  v0_4u  });
    newiopNoret(VOP_ADD, { v0_4u,  v31_4u, v0_4u  });
    newiopNoret(VOP_ADD, { v0_4u,  v0_4u,  v31_4u });
    newiopNoret(VOP_ADD, { v0_2u,  v0_2u,  v0_2u  });
    newiopNoret(VOP_ADD, { v31_2u, v0_2u,  v0_2u  });
    newiopNoret(VOP_ADD, { v0_2u,  v31_2u, v0_2u  });
    newiopNoret(VOP_ADD, { v0_2u,  v0_2u,  v31_2u });
    newiopNoret(VOP_ADD, { v0_8f,  v0_8f,  v0_8f  });
    newiopNoret(VOP_ADD, { v31_8f, v0_8f,  v0_8f  });
    newiopNoret(VOP_ADD, { v0_8f,  v31_8f, v0_8f  });
    newiopNoret(VOP_ADD, { v0_8f,  v0_8f,  v31_8f });
    newiopNoret(VOP_ADD, { v0_4f,  v0_4f,  v0_4f  });
    newiopNoret(VOP_ADD, { v31_4f, v0_4f,  v0_4f  });
    newiopNoret(VOP_ADD, { v0_4f,  v31_4f, v0_4f  });
    newiopNoret(VOP_ADD, { v0_4f,  v0_4f,  v31_4f });
    newiopNoret(VOP_ADD, { v0_2f,  v0_2f,  v0_2f  });
    newiopNoret(VOP_ADD, { v31_2f, v0_2f,  v0_2f  });
    newiopNoret(VOP_ADD, { v0_2f,  v31_2f, v0_2f  });
    newiopNoret(VOP_ADD, { v0_2f,  v0_2f,  v31_2f });

    newiopNoret(VOP_SUB, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_SUB, { v31_16u, v0_16u,  v0_16u  });
    newiopNoret(VOP_SUB, { v0_16u,  v31_16u, v0_16u  });
    newiopNoret(VOP_SUB, { v0_16u,  v0_16u,  v31_16u });
    newiopNoret(VOP_SUB, { v0_8u,  v0_8u,  v0_8u  });
    newiopNoret(VOP_SUB, { v31_8u, v0_8u,  v0_8u  });
    newiopNoret(VOP_SUB, { v0_8u,  v31_8u, v0_8u  });
    newiopNoret(VOP_SUB, { v0_8u,  v0_8u,  v31_8u });
    newiopNoret(VOP_SUB, { v0_4u,  v0_4u,  v0_4u  });
    newiopNoret(VOP_SUB, { v31_4u, v0_4u,  v0_4u  });
    newiopNoret(VOP_SUB, { v0_4u,  v31_4u, v0_4u  });
    newiopNoret(VOP_SUB, { v0_4u,  v0_4u,  v31_4u });
    newiopNoret(VOP_SUB, { v0_2u,  v0_2u,  v0_2u  });
    newiopNoret(VOP_SUB, { v31_2u, v0_2u,  v0_2u  });
    newiopNoret(VOP_SUB, { v0_2u,  v31_2u, v0_2u  });
    newiopNoret(VOP_SUB, { v0_2u,  v0_2u,  v31_2u });
    newiopNoret(VOP_SUB, { v0_8f,  v0_8f,  v0_8f  });
    newiopNoret(VOP_SUB, { v31_8f, v0_8f,  v0_8f  });
    newiopNoret(VOP_SUB, { v0_8f,  v31_8f, v0_8f  });
    newiopNoret(VOP_SUB, { v0_8f,  v0_8f,  v31_8f });
    newiopNoret(VOP_SUB, { v0_4f,  v0_4f,  v0_4f  });
    newiopNoret(VOP_SUB, { v31_4f, v0_4f,  v0_4f  });
    newiopNoret(VOP_SUB, { v0_4f,  v31_4f, v0_4f  });
    newiopNoret(VOP_SUB, { v0_4f,  v0_4f,  v31_4f });
    newiopNoret(VOP_SUB, { v0_2f,  v0_2f,  v0_2f  });
    newiopNoret(VOP_SUB, { v31_2f, v0_2f,  v0_2f  });
    newiopNoret(VOP_SUB, { v0_2f,  v31_2f, v0_2f  });
    newiopNoret(VOP_SUB, { v0_2f,  v0_2f,  v31_2f });

    newiopNoret(VOP_MUL, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_MUL, { v31_16u, v0_16u,  v0_16u  });
    newiopNoret(VOP_MUL, { v0_16u,  v31_16u, v0_16u  });
    newiopNoret(VOP_MUL, { v0_16u,  v0_16u,  v31_16u });
    newiopNoret(VOP_MUL, { v0_8u,  v0_8u,  v0_8u  });
    newiopNoret(VOP_MUL, { v31_8u, v0_8u,  v0_8u  });
    newiopNoret(VOP_MUL, { v0_8u,  v31_8u, v0_8u  });
    newiopNoret(VOP_MUL, { v0_8u,  v0_8u,  v31_8u });
    newiopNoret(VOP_MUL, { v0_4u,  v0_4u,  v0_4u  });
    newiopNoret(VOP_MUL, { v31_4u, v0_4u,  v0_4u  });
    newiopNoret(VOP_MUL, { v0_4u,  v31_4u, v0_4u  });
    newiopNoret(VOP_MUL, { v0_4u,  v0_4u,  v31_4u });
    newiopNoret(VOP_MUL, { v0_8f,  v0_8f,  v0_8f  });
    newiopNoret(VOP_MUL, { v31_8f, v0_8f,  v0_8f  });
    newiopNoret(VOP_MUL, { v0_8f,  v31_8f, v0_8f  });
    newiopNoret(VOP_MUL, { v0_8f,  v0_8f,  v31_8f });
    newiopNoret(VOP_MUL, { v0_4f,  v0_4f,  v0_4f  });
    newiopNoret(VOP_MUL, { v31_4f, v0_4f,  v0_4f  });
    newiopNoret(VOP_MUL, { v0_4f,  v31_4f, v0_4f  });
    newiopNoret(VOP_MUL, { v0_4f,  v0_4f,  v31_4f });
    newiopNoret(VOP_MUL, { v0_2f,  v0_2f,  v0_2f  });
    newiopNoret(VOP_MUL, { v31_2f, v0_2f,  v0_2f  });
    newiopNoret(VOP_MUL, { v0_2f,  v31_2f, v0_2f  });
    newiopNoret(VOP_MUL, { v0_2f,  v0_2f,  v31_2f });

    newiopNoret(VOP_DIV, { v0_4f,  v0_4f,  v0_4f  });
    newiopNoret(VOP_DIV, { v31_4f, v0_4f,  v0_4f  });
    newiopNoret(VOP_DIV, { v0_4f,  v31_4f, v0_4f  });
    newiopNoret(VOP_DIV, { v0_4f,  v0_4f,  v31_4f });
    newiopNoret(VOP_DIV, { v0_2f,  v0_2f,  v0_2f  });
    newiopNoret(VOP_DIV, { v31_2f, v0_2f,  v0_2f  });
    newiopNoret(VOP_DIV, { v0_2f,  v31_2f, v0_2f  });
    newiopNoret(VOP_DIV, { v0_2f,  v0_2f,  v31_2f });
    newiopNoret(VOP_DIV, { v0_8f,  v0_8f,  v0_8f  });
    newiopNoret(VOP_DIV, { v31_8f, v0_8f,  v0_8f  });
    newiopNoret(VOP_DIV, { v0_8f,  v31_8f, v0_8f  });
    newiopNoret(VOP_DIV, { v0_8f,  v0_8f,  v31_8f });

    newiopNoret(VOP_SAL, { v0_16u,  v0_16u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v31_16u,  v0_16u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_16u,  v31_16u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_16u,  v0_16u,  argIImm(7)  });
    newiopNoret(VOP_SAL, { v0_8u,  v0_8u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v31_8u,  v0_8u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_8u,  v31_8u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_8u,  v0_8u,  argIImm(15)  });
    newiopNoret(VOP_SAL, { v0_4u,  v0_4u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v31_4u,  v0_4u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_4u,  v31_4u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_4u,  v0_4u,  argIImm(31)  });
    newiopNoret(VOP_SAL, { v0_2u,  v0_2u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v31_2u,  v0_2u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_2u,  v31_2u,  argIImm(1)  });
    newiopNoret(VOP_SAL, { v0_2u,  v0_2u,  argIImm(63)  });

    newiopNoret(VOP_SAL, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_SAL, { v31_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_SAL, { v0_16u,  v31_16u,  v0_16u  });
    newiopNoret(VOP_SAL, { v0_16u,  v0_16u,  v31_16u  });
    newiopNoret(VOP_SAL, { v0_8u,  v0_8u,  v0_8u  });
    newiopNoret(VOP_SAL, { v31_8u,  v0_8u,  v0_8u  });
    newiopNoret(VOP_SAL, { v0_8u,  v31_8u,  v0_8u  });
    newiopNoret(VOP_SAL, { v0_8u,  v0_8u,  v31_8u  });
    newiopNoret(VOP_SAL, { v0_4u,  v0_4u,  v0_4u  });
    newiopNoret(VOP_SAL, { v31_4u,  v0_4u,  v0_4u  });
    newiopNoret(VOP_SAL, { v0_4u,  v31_4u,  v0_4u  });
    newiopNoret(VOP_SAL, { v0_4u,  v0_4u,  v31_4u  });
    newiopNoret(VOP_SAL, { v0_2u,  v0_2u,  v0_2u  });
    newiopNoret(VOP_SAL, { v31_2u,  v0_2u,  v0_2u  });
    newiopNoret(VOP_SAL, { v0_2u,  v31_2u,  v0_2u  });
    newiopNoret(VOP_SAL, { v0_2u,  v0_2u,  v31_2u  });

    newiopNoret(VOP_SAL, { v0_16s,  v0_16s,  v0_16s  });
    newiopNoret(VOP_SAL, { v31_16s,  v0_16s,  v0_16s  });
    newiopNoret(VOP_SAL, { v0_16s,  v31_16s,  v0_16s  });
    newiopNoret(VOP_SAL, { v0_16s,  v0_16s,  v31_16s  });
    newiopNoret(VOP_SAL, { v0_8s,  v0_8s,  v0_8s  });
    newiopNoret(VOP_SAL, { v31_8s,  v0_8s,  v0_8s  });
    newiopNoret(VOP_SAL, { v0_8s,  v31_8s,  v0_8s  });
    newiopNoret(VOP_SAL, { v0_8s,  v0_8s,  v31_8s  });
    newiopNoret(VOP_SAL, { v0_4s,  v0_4s,  v0_4s  });
    newiopNoret(VOP_SAL, { v31_4s,  v0_4s,  v0_4s  });
    newiopNoret(VOP_SAL, { v0_4s,  v31_4s,  v0_4s  });
    newiopNoret(VOP_SAL, { v0_4s,  v0_4s,  v31_4s  });
    newiopNoret(VOP_SAL, { v0_2s,  v0_2s,  v0_2s  });
    newiopNoret(VOP_SAL, { v31_2s,  v0_2s,  v0_2s  });
    newiopNoret(VOP_SAL, { v0_2s,  v31_2s,  v0_2s  });
    newiopNoret(VOP_SAL, { v0_2s,  v0_2s,  v31_2s  });

    newiopNoret(VOP_SAR, { v0_16u,  v0_16u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_16u,  v0_16u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_16u,  v31_16u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_16u,  v0_16u,  argIImm(7)  });
    newiopNoret(VOP_SAR, { v0_8u,  v0_8u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_8u,  v0_8u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_8u,  v31_8u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_8u,  v0_8u,  argIImm(15)  });
    newiopNoret(VOP_SAR, { v0_4u,  v0_4u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_4u,  v0_4u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_4u,  v31_4u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_4u,  v0_4u,  argIImm(31)  });
    newiopNoret(VOP_SAR, { v0_2u,  v0_2u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_2u,  v0_2u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_2u,  v31_2u,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_2u,  v0_2u,  argIImm(63)  });

    newiopNoret(VOP_SAR, { v0_16s,  v0_16s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_16s,  v0_16s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_16s,  v31_16s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_16s,  v0_16s,  argIImm(7)  });
    newiopNoret(VOP_SAR, { v0_8s,  v0_8s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_8s,  v0_8s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_8s,  v31_8s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_8s,  v0_8s,  argIImm(15)  });
    newiopNoret(VOP_SAR, { v0_4s,  v0_4s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_4s,  v0_4s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_4s,  v31_4s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_4s,  v0_4s,  argIImm(31)  });
    newiopNoret(VOP_SAR, { v0_2s,  v0_2s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v31_2s,  v0_2s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_2s,  v31_2s,  argIImm(1)  });
    newiopNoret(VOP_SAR, { v0_2s,  v0_2s,  argIImm(63)  });

    newiopNoret(VOP_AND, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_AND, { v31_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_AND, { v0_16u,  v31_16u,  v0_16u  });
    newiopNoret(VOP_AND, { v0_16u,  v0_16u,  v31_16u  });

    newiopNoret(VOP_OR, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_OR, { v31_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_OR, { v0_16u,  v31_16u,  v0_16u  });
    newiopNoret(VOP_OR, { v0_16u,  v0_16u,  v31_16u  });

    newiopNoret(VOP_XOR, { v0_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_XOR, { v31_16u,  v0_16u,  v0_16u  });
    newiopNoret(VOP_XOR, { v0_16u,  v31_16u,  v0_16u  });
    newiopNoret(VOP_XOR, { v0_16u,  v0_16u,  v31_16u  });

    newiopNoret(VOP_NOT, { v0_16u,  v0_16u  });
    newiopNoret(VOP_NOT, { v31_16u,  v0_16u });
    newiopNoret(VOP_NOT, { v0_16u,  v31_16u });

    newiopNoret(OP_MOV, { v0_16u,  argIImm(0)  });
    newiopNoret(OP_MOV, { v31_16u,  argIImm(0)  });
    newiopNoret(OP_MOV, { v0_16u,  argIImm(255)  });
    newiopNoret(OP_MOV, { v0_8u,  argIImm(0)  });
    newiopNoret(OP_MOV, { v31_8u,  argIImm(0)  });
    newiopNoret(OP_MOV, { v0_8u,  argIImm(255)  });
    newiopNoret(OP_MOV, { v0_4u,  argIImm(0)  });
    newiopNoret(OP_MOV, { v31_4u,  argIImm(0)  });
    newiopNoret(OP_MOV, { v0_4u,  argIImm(255)  });
    newiopNoret(OP_MOV, {  v0_8s, argIImm(  -1) });
    newiopNoret(OP_MOV, { v31_8s, argIImm(  -1) });
    newiopNoret(OP_MOV, {  v0_8s, argIImm(-256) });
    newiopNoret(OP_MOV, {  v0_4s, argIImm(  -1) });
    newiopNoret(OP_MOV, { v31_4s, argIImm(  -1) });
    newiopNoret(OP_MOV, {  v0_4s, argIImm(-256) });

    newiopNoret(VOP_MIN, { v0_8f,  v0_8f, v0_8f  });
    newiopNoret(VOP_MIN, { v31_8f, v0_8f, v0_8f  });
    newiopNoret(VOP_MIN, { v0_8f, v31_8f, v0_8f  });
    newiopNoret(VOP_MIN, { v0_8f,  v0_8f, v31_8f  });
    newiopNoret(VOP_MIN, { v0_4f,  v0_4f, v0_4f  });
    newiopNoret(VOP_MIN, { v31_4f, v0_4f, v0_4f  });
    newiopNoret(VOP_MIN, { v0_4f, v31_4f, v0_4f  });
    newiopNoret(VOP_MIN, { v0_4f,  v0_4f, v31_4f  });
    newiopNoret(VOP_MIN, { v0_2f,  v0_2f, v0_2f  });
    newiopNoret(VOP_MIN, { v31_2f, v0_2f, v0_2f  });
    newiopNoret(VOP_MIN, { v0_2f, v31_2f, v0_2f  });
    newiopNoret(VOP_MIN, { v0_2f,  v0_2f, v31_2f  });

    newiopNoret(VOP_MAX, { v0_8f,  v0_8f, v0_8f  });
    newiopNoret(VOP_MAX, { v31_8f, v0_8f, v0_8f  });
    newiopNoret(VOP_MAX, { v0_8f, v31_8f, v0_8f  });
    newiopNoret(VOP_MAX, { v0_8f,  v0_8f, v31_8f  });
    newiopNoret(VOP_MAX, { v0_4f,  v0_4f, v0_4f  });
    newiopNoret(VOP_MAX, { v31_4f, v0_4f, v0_4f  });
    newiopNoret(VOP_MAX, { v0_4f, v31_4f, v0_4f  });
    newiopNoret(VOP_MAX, { v0_4f,  v0_4f, v31_4f  });
    newiopNoret(VOP_MAX, { v0_2f,  v0_2f, v0_2f  });
    newiopNoret(VOP_MAX, { v31_2f, v0_2f, v0_2f  });
    newiopNoret(VOP_MAX, { v0_2f, v31_2f, v0_2f  });
    newiopNoret(VOP_MAX, { v0_2f,  v0_2f, v31_2f  });

    newiopNoret(VOP_FMA, { v0_8f , v0_8f ,  v0_8f, v0_8f  });
    newiopNoret(VOP_FMA, { v31_8f, v31_8f,  v0_8f, v0_8f });
    newiopNoret(VOP_FMA, { v0_8f , v0_8f ,  v31_8f, v0_8f });
    newiopNoret(VOP_FMA, { v0_8f , v0_8f ,  v0_8f, v31_8f });
    newiopNoret(VOP_FMA, { v0_4f , v0_4f ,  v0_4f, v0_4f  });
    newiopNoret(VOP_FMA, { v31_4f, v31_4f,  v0_4f, v0_4f });
    newiopNoret(VOP_FMA, { v0_4f , v0_4f ,  v31_4f, v0_4f });
    newiopNoret(VOP_FMA, { v0_4f , v0_4f ,  v0_4f, v31_4f });
    newiopNoret(VOP_FMA, { v0_2f , v0_2f ,  v0_2f, v0_2f  });
    newiopNoret(VOP_FMA, { v31_2f, v31_2f,  v0_2f, v0_2f });
    newiopNoret(VOP_FMA, { v0_2f , v0_2f ,  v31_2f, v0_2f });
    newiopNoret(VOP_FMA, { v0_2f , v0_2f ,  v0_2f, v31_2f });

    newiopNoret(VOP_TRUNC, { v0_8s , v0_8f  });
    newiopNoret(VOP_TRUNC, { v31_8s, v0_8f  });
    newiopNoret(VOP_TRUNC, { v0_8s , v31_8f });
    newiopNoret(VOP_TRUNC, { v0_4s , v0_4f  });
    newiopNoret(VOP_TRUNC, { v31_4s, v0_4f  });
    newiopNoret(VOP_TRUNC, { v0_4s , v31_4f });
    newiopNoret(VOP_TRUNC, { v0_2s , v0_2f  });
    newiopNoret(VOP_TRUNC, { v31_2s, v0_2f  });
    newiopNoret(VOP_TRUNC, { v0_2s , v31_2f });
    newiopNoret(VOP_TRUNC, { v0_8u , v0_8f  });
    newiopNoret(VOP_TRUNC, { v31_8u, v0_8f  });
    newiopNoret(VOP_TRUNC, { v0_8u , v31_8f });
    newiopNoret(VOP_TRUNC, { v0_4u , v0_4f  });
    newiopNoret(VOP_TRUNC, { v31_4u, v0_4f  });
    newiopNoret(VOP_TRUNC, { v0_4u , v31_4f });
    newiopNoret(VOP_TRUNC, { v0_2u , v0_2f  });
    newiopNoret(VOP_TRUNC, { v31_2u, v0_2f  });
    newiopNoret(VOP_TRUNC, { v0_2u , v31_2f });

    newiopNoret(VOP_FLOOR, { v0_8s , v0_8f  });
    newiopNoret(VOP_FLOOR, { v31_8s , v0_8f  });
    newiopNoret(VOP_FLOOR, { v0_8s , v31_8f  });
    newiopNoret(VOP_FLOOR, { v0_4s , v0_4f  });
    newiopNoret(VOP_FLOOR, { v31_4s , v0_4f  });
    newiopNoret(VOP_FLOOR, { v0_4s , v31_4f  });
    newiopNoret(VOP_FLOOR, { v0_2s , v0_2f  });
    newiopNoret(VOP_FLOOR, { v31_2s , v0_2f  });
    newiopNoret(VOP_FLOOR, { v0_2s , v31_2f  });
    newiopNoret(VOP_FLOOR, { v0_8u , v0_8f  });
    newiopNoret(VOP_FLOOR, { v31_8u , v0_8f  });
    newiopNoret(VOP_FLOOR, { v0_8u , v31_8f  });
    newiopNoret(VOP_FLOOR, { v0_4u , v0_4f  });
    newiopNoret(VOP_FLOOR, { v31_4u , v0_4f  });
    newiopNoret(VOP_FLOOR, { v0_4u , v31_4f  });
    newiopNoret(VOP_FLOOR, { v0_2u , v0_2f  });
    newiopNoret(VOP_FLOOR, { v31_2u , v0_2f  });
    newiopNoret(VOP_FLOOR, { v0_2u , v31_2f  });

    newiopNoret(VOP_CAST, { v0_8f , v0_8s  });
    newiopNoret(VOP_CAST, { v31_8f, v0_8s  });
    newiopNoret(VOP_CAST, { v0_8f , v31_8s });
    newiopNoret(VOP_CAST, { v0_4f , v0_4s  });
    newiopNoret(VOP_CAST, { v31_4f, v0_4s  });
    newiopNoret(VOP_CAST, { v0_4f , v31_4s });
    newiopNoret(VOP_CAST, { v0_2f , v0_2s  });
    newiopNoret(VOP_CAST, { v31_2f, v0_2s  });
    newiopNoret(VOP_CAST, { v0_2f , v31_2s });
    newiopNoret(VOP_CAST, { v0_8f , v0_8u  });
    newiopNoret(VOP_CAST, { v31_8f, v0_8u  });
    newiopNoret(VOP_CAST, { v0_8f , v31_8u });
    newiopNoret(VOP_CAST, { v0_4f , v0_4u  });
    newiopNoret(VOP_CAST, { v31_4f, v0_4u  });
    newiopNoret(VOP_CAST, { v0_4f , v31_4u });
    newiopNoret(VOP_CAST, { v0_2f , v0_2u  });
    newiopNoret(VOP_CAST, { v31_2f, v0_2u  });
    newiopNoret(VOP_CAST, { v0_2f , v31_2u });

    newiopNoret(VOP_GT, { v0_8u , v0_8f , v0_8f });
    newiopNoret(VOP_GT, { v31_8u, v0_8f , v0_8f });
    newiopNoret(VOP_GT, { v0_8u , v31_8f, v0_8f });
    newiopNoret(VOP_GT, { v0_8u , v0_8f , v31_8f});
    newiopNoret(VOP_GT, { v0_4u , v0_4f , v0_4f });
    newiopNoret(VOP_GT, { v31_4u, v0_4f , v0_4f });
    newiopNoret(VOP_GT, { v0_4u , v31_4f, v0_4f });
    newiopNoret(VOP_GT, { v0_4u , v0_4f , v31_4f});
    newiopNoret(VOP_GT, { v0_2u , v0_2f , v0_2f });
    newiopNoret(VOP_GT, { v31_2u, v0_2f , v0_2f });
    newiopNoret(VOP_GT, { v0_2u , v31_2f, v0_2f });
    newiopNoret(VOP_GT, { v0_2u , v0_2f , v31_2f});

    newiopNoret(VOP_GE, { v0_8u , v0_8f , v0_8f });
    newiopNoret(VOP_GE, { v31_8u, v0_8f , v0_8f });
    newiopNoret(VOP_GE, { v0_8u , v31_8f, v0_8f });
    newiopNoret(VOP_GE, { v0_8u , v0_8f , v31_8f});
    newiopNoret(VOP_GE, { v0_4u , v0_4f , v0_4f });
    newiopNoret(VOP_GE, { v31_4u, v0_4f , v0_4f });
    newiopNoret(VOP_GE, { v0_4u , v31_4f, v0_4f });
    newiopNoret(VOP_GE, { v0_4u , v0_4f , v31_4f});
    newiopNoret(VOP_GE, { v0_2u , v0_2f , v0_2f });
    newiopNoret(VOP_GE, { v31_2u, v0_2f , v0_2f });
    newiopNoret(VOP_GE, { v0_2u , v31_2f, v0_2f });
    newiopNoret(VOP_GE, { v0_2u , v0_2f , v31_2f});

    newiopNoret(VOP_EQ, { v0_8u , v0_8f , v0_8f });
    newiopNoret(VOP_EQ, { v31_8u, v0_8f , v0_8f });
    newiopNoret(VOP_EQ, { v0_8u , v31_8f, v0_8f });
    newiopNoret(VOP_EQ, { v0_8u , v0_8f , v31_8f});
    newiopNoret(VOP_EQ, { v0_4u , v0_4f , v0_4f });
    newiopNoret(VOP_EQ, { v31_4u, v0_4f , v0_4f });
    newiopNoret(VOP_EQ, { v0_4u , v31_4f, v0_4f });
    newiopNoret(VOP_EQ, { v0_4u , v0_4f , v31_4f});
    newiopNoret(VOP_EQ, { v0_2u , v0_2f , v0_2f });
    newiopNoret(VOP_EQ, { v31_2u, v0_2f , v0_2f });
    newiopNoret(VOP_EQ, { v0_2u , v31_2f, v0_2f });
    newiopNoret(VOP_EQ, { v0_2u , v0_2f , v31_2f});

    newiopNoret(VOP_NEG, { v0_16s , v0_16s });
    newiopNoret(VOP_NEG, { v31_16s , v0_16s });
    newiopNoret(VOP_NEG, { v0_16s , v31_16s });
    newiopNoret(VOP_NEG, { v0_8s , v0_8s });
    newiopNoret(VOP_NEG, { v31_8s , v0_8s });
    newiopNoret(VOP_NEG, { v0_8s , v31_8s });
    newiopNoret(VOP_NEG, { v0_4s , v0_4s });
    newiopNoret(VOP_NEG, { v31_4s , v0_4s });
    newiopNoret(VOP_NEG, { v0_4s , v31_4s });
    newiopNoret(VOP_NEG, { v0_2s , v0_2s });
    newiopNoret(VOP_NEG, { v31_2s , v0_2s });
    newiopNoret(VOP_NEG, { v0_2s , v31_2s });
    newiopNoret(VOP_NEG, { v0_8f , v0_8f });
    newiopNoret(VOP_NEG, { v31_8f , v0_8f });
    newiopNoret(VOP_NEG, { v0_8f , v31_8f });
    newiopNoret(VOP_NEG, { v0_4f , v0_4f });
    newiopNoret(VOP_NEG, { v31_4f , v0_4f });
    newiopNoret(VOP_NEG, { v0_4f , v31_4f });
    newiopNoret(VOP_NEG, { v0_2f , v0_2f });
    newiopNoret(VOP_NEG, { v31_2f , v0_2f });
    newiopNoret(VOP_NEG, { v0_2f , v31_2f });

    newiopNoret(OP_ARM_MOVK, { x0 , argIImm(0)     , argIImm(16) });
    newiopNoret(OP_ARM_MOVK, { x15, argIImm(0)     , argIImm(16) });
    newiopNoret(OP_ARM_MOVK, { x0 , argIImm(0xffff), argIImm(16) });
    newiopNoret(OP_ARM_MOVK, { x0 , argIImm(0)     , argIImm(32) });
    newiopNoret(OP_ARM_MOVK, { x15, argIImm(0)     , argIImm(32) });
    newiopNoret(OP_ARM_MOVK, { x0 , argIImm(0xffff), argIImm(32) });
    newiopNoret(OP_ARM_MOVK, { x0 , argIImm(0)     , argIImm(48) });
    newiopNoret(OP_ARM_MOVK, { x15, argIImm(0)     , argIImm(48) });
    newiopNoret(OP_ARM_MOVK, { x0 , argIImm(0xffff), argIImm(48) });

    newiopNoret(VOP_BROADCAST, { v0_16s, w0 });
    newiopNoret(VOP_BROADCAST, { v31_16s, w0 });
    newiopNoret(VOP_BROADCAST, { v0_16s, w15 });
    newiopNoret(VOP_BROADCAST, { v0_8s, w0 });
    newiopNoret(VOP_BROADCAST, { v31_8s, w0 });
    newiopNoret(VOP_BROADCAST, { v0_8s, w15 });
    newiopNoret(VOP_BROADCAST, { v0_4s, w0 });
    newiopNoret(VOP_BROADCAST, { v31_4s, w0 });
    newiopNoret(VOP_BROADCAST, { v0_4s, w15 });
    newiopNoret(VOP_BROADCAST, { v0_2s, x0 });
    newiopNoret(VOP_BROADCAST, { v31_2s, x0 });
    newiopNoret(VOP_BROADCAST, { v0_2s, x15 });

    newiopNoret(VOP_BROADCAST, { v0_16u,  v0_16u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v31_16u, v0_16u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_16u, v31_16u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_16u,  v0_16u, argIImm(15)});
    newiopNoret(VOP_BROADCAST, { v0_8u,  v0_8u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v31_8u, v0_8u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_8u, v31_8u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_8u,  v0_8u, argIImm(7)});
    newiopNoret(VOP_BROADCAST, { v0_4u,  v0_4u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v31_4u, v0_4u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_4u, v31_4u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_4u,  v0_4u, argIImm(3)});
    newiopNoret(VOP_BROADCAST, { v0_2u,  v0_2u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v31_2u, v0_2u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_2u, v31_2u, argIImm(0)});
    newiopNoret(VOP_BROADCAST, { v0_2u,  v0_2u, argIImm(1)});

    newiopNoret(VOP_CAST_LOW, { v0_8s, v0_16s });
    newiopNoret(VOP_CAST_LOW, { v31_8s, v0_16s });
    newiopNoret(VOP_CAST_LOW, { v0_8s, v31_16s });
    newiopNoret(VOP_CAST_LOW, { v0_4s, v0_8s });
    newiopNoret(VOP_CAST_LOW, { v31_4s, v0_8s });
    newiopNoret(VOP_CAST_LOW, { v0_4s, v31_8s });
    newiopNoret(VOP_CAST_LOW, { v0_2s, v0_4s });
    newiopNoret(VOP_CAST_LOW, { v31_2s, v0_4s });
    newiopNoret(VOP_CAST_LOW, { v0_2s, v31_4s });
    newiopNoret(VOP_CAST_LOW, { v0_8u, v0_16u });
    newiopNoret(VOP_CAST_LOW, { v31_8u, v0_16u });
    newiopNoret(VOP_CAST_LOW, { v0_8u, v31_16u });
    newiopNoret(VOP_CAST_LOW, { v0_4u, v0_8u });
    newiopNoret(VOP_CAST_LOW, { v31_4u, v0_8u });
    newiopNoret(VOP_CAST_LOW, { v0_4u, v31_8u });
    newiopNoret(VOP_CAST_LOW, { v0_2u, v0_4u });
    newiopNoret(VOP_CAST_LOW, { v31_2u, v0_4u });
    newiopNoret(VOP_CAST_LOW, { v0_2u, v31_4u });

    newiopNoret(VOP_CAST_HIGH, { v0_8s, v0_16s });
    newiopNoret(VOP_CAST_HIGH, { v31_8s, v0_16s });
    newiopNoret(VOP_CAST_HIGH, { v0_8s, v31_16s });
    newiopNoret(VOP_CAST_HIGH, { v0_4s, v0_8s });
    newiopNoret(VOP_CAST_HIGH, { v31_4s, v0_8s });
    newiopNoret(VOP_CAST_HIGH, { v0_4s, v31_8s });
    newiopNoret(VOP_CAST_HIGH, { v0_2s, v0_4s });
    newiopNoret(VOP_CAST_HIGH, { v31_2s, v0_4s });
    newiopNoret(VOP_CAST_HIGH, { v0_2s, v31_4s });
    newiopNoret(VOP_CAST_HIGH, { v0_8u, v0_16u });
    newiopNoret(VOP_CAST_HIGH, { v31_8u, v0_16u });
    newiopNoret(VOP_CAST_HIGH, { v0_8u, v31_16u });
    newiopNoret(VOP_CAST_HIGH, { v0_4u, v0_8u });
    newiopNoret(VOP_CAST_HIGH, { v31_4u, v0_8u });
    newiopNoret(VOP_CAST_HIGH, { v0_4u, v31_8u });
    newiopNoret(VOP_CAST_HIGH, { v0_2u, v0_4u });
    newiopNoret(VOP_CAST_HIGH, { v31_2u, v0_4u });
    newiopNoret(VOP_CAST_HIGH, { v0_2u, v31_4u });

    newiopNoret(VOP_SHRINK_LOW, { v0_16s, v0_8s });
    newiopNoret(VOP_SHRINK_LOW, { v31_16s, v0_8s });
    newiopNoret(VOP_SHRINK_LOW, { v0_16s, v31_8s });
    newiopNoret(VOP_SHRINK_LOW, { v0_8s, v0_4s });
    newiopNoret(VOP_SHRINK_LOW, { v31_8s, v0_4s });
    newiopNoret(VOP_SHRINK_LOW, { v0_8s, v31_4s });
    newiopNoret(VOP_SHRINK_LOW, { v0_4s, v0_2s });
    newiopNoret(VOP_SHRINK_LOW, { v31_4s, v0_2s });
    newiopNoret(VOP_SHRINK_LOW, { v0_4s, v31_2s });

    newiopNoret(VOP_SHRINK_HIGH, { v0_16s, v0_8s });
    newiopNoret(VOP_SHRINK_HIGH, { v31_16s, v0_8s });
    newiopNoret(VOP_SHRINK_HIGH, { v0_16s, v31_8s });
    newiopNoret(VOP_SHRINK_HIGH, { v0_8s, v0_4s });
    newiopNoret(VOP_SHRINK_HIGH, { v31_8s, v0_4s });
    newiopNoret(VOP_SHRINK_HIGH, { v0_8s, v31_4s });
    newiopNoret(VOP_SHRINK_HIGH, { v0_4s, v0_2s });
    newiopNoret(VOP_SHRINK_HIGH, { v31_4s, v0_2s });
    newiopNoret(VOP_SHRINK_HIGH, { v0_4s, v31_2s });

    newiopNoret(VOP_REDUCE_MAX, { v0_16s, v0_16s });
    newiopNoret(VOP_REDUCE_MAX, { v31_16s, v0_16s });
    newiopNoret(VOP_REDUCE_MAX, { v0_16s, v31_16s });
    newiopNoret(VOP_REDUCE_MAX, { v0_8s, v0_8s });
    newiopNoret(VOP_REDUCE_MAX, { v31_8s, v0_8s });
    newiopNoret(VOP_REDUCE_MAX, { v0_8s, v31_8s });
    newiopNoret(VOP_REDUCE_MAX, { v0_4s, v0_4s });
    newiopNoret(VOP_REDUCE_MAX, { v31_4s, v0_4s });
    newiopNoret(VOP_REDUCE_MAX, { v0_4s, v31_4s });
    newiopNoret(VOP_REDUCE_MAX, { v0_16u, v0_16u });
    newiopNoret(VOP_REDUCE_MAX, { v31_16u, v0_16u });
    newiopNoret(VOP_REDUCE_MAX, { v0_16u, v31_16u });
    newiopNoret(VOP_REDUCE_MAX, { v0_8u, v0_8u });
    newiopNoret(VOP_REDUCE_MAX, { v31_8u, v0_8u });
    newiopNoret(VOP_REDUCE_MAX, { v0_8u, v31_8u });
    newiopNoret(VOP_REDUCE_MAX, { v0_4u, v0_4u });
    newiopNoret(VOP_REDUCE_MAX, { v31_4u, v0_4u });
    newiopNoret(VOP_REDUCE_MAX, { v0_4u, v31_4u });
    newiopNoret(VOP_REDUCE_MAX, { v0_8f, v0_8f });
    newiopNoret(VOP_REDUCE_MAX, { v31_8f, v0_8f });
    newiopNoret(VOP_REDUCE_MAX, { v0_8f, v31_8f });
    newiopNoret(VOP_REDUCE_MAX, { v0_4f, v0_4f });
    newiopNoret(VOP_REDUCE_MAX, { v31_4f, v0_4f });
    newiopNoret(VOP_REDUCE_MAX, { v0_4f, v31_4f });

    newiopNoret(VOP_REDUCE_MIN, { v0_16s, v0_16s });
    newiopNoret(VOP_REDUCE_MIN, { v31_16s, v0_16s });
    newiopNoret(VOP_REDUCE_MIN, { v0_16s, v31_16s });
    newiopNoret(VOP_REDUCE_MIN, { v0_8s, v0_8s });
    newiopNoret(VOP_REDUCE_MIN, { v31_8s, v0_8s });
    newiopNoret(VOP_REDUCE_MIN, { v0_8s, v31_8s });
    newiopNoret(VOP_REDUCE_MIN, { v0_4s, v0_4s });
    newiopNoret(VOP_REDUCE_MIN, { v31_4s, v0_4s });
    newiopNoret(VOP_REDUCE_MIN, { v0_4s, v31_4s });
    newiopNoret(VOP_REDUCE_MIN, { v0_16u, v0_16u });
    newiopNoret(VOP_REDUCE_MIN, { v31_16u, v0_16u });
    newiopNoret(VOP_REDUCE_MIN, { v0_16u, v31_16u });
    newiopNoret(VOP_REDUCE_MIN, { v0_8u, v0_8u });
    newiopNoret(VOP_REDUCE_MIN, { v31_8u, v0_8u });
    newiopNoret(VOP_REDUCE_MIN, { v0_8u, v31_8u });
    newiopNoret(VOP_REDUCE_MIN, { v0_4u, v0_4u });
    newiopNoret(VOP_REDUCE_MIN, { v31_4u, v0_4u });
    newiopNoret(VOP_REDUCE_MIN, { v0_4u, v31_4u });
    newiopNoret(VOP_REDUCE_MIN, { v0_8f, v0_8f });
    newiopNoret(VOP_REDUCE_MIN, { v31_8f, v0_8f });
    newiopNoret(VOP_REDUCE_MIN, { v0_8f, v31_8f });
    newiopNoret(VOP_REDUCE_MIN, { v0_4f, v0_4f });
    newiopNoret(VOP_REDUCE_MIN, { v31_4f, v0_4f });
    newiopNoret(VOP_REDUCE_MIN, { v0_4f, v31_4f });

    newiopNoret(VOP_ARM_LD1, { v0_16s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v31_16s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v0_16s, argIImm(15), x0});
    newiopNoret(VOP_ARM_LD1, { v0_16s, argIImm(0), x15});

    newiopNoret(VOP_ARM_LD1, { v0_8s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v31_8s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v0_8s, argIImm(7), x0});
    newiopNoret(VOP_ARM_LD1, { v0_8s, argIImm(0), x15});

    newiopNoret(VOP_ARM_LD1, { v0_4s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v31_4s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v0_4s, argIImm(3), x0});
    newiopNoret(VOP_ARM_LD1, { v0_4s, argIImm(0), x15});

    newiopNoret(VOP_ARM_LD1, { v0_2s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v31_2s, argIImm(0), x0});
    newiopNoret(VOP_ARM_LD1, { v0_2s, argIImm(1), x0});
    newiopNoret(VOP_ARM_LD1, { v0_2s, argIImm(0), x15});

    newiopNoret(VOP_ARM_LD2, {  v0_16s, v1_16s,  x0});
    newiopNoret(VOP_ARM_LD2, { v31_16s, v0_16s,  x0});
    newiopNoret(VOP_ARM_LD2, {  v0_16s, v1_16s, x15});

    newiopNoret(VOP_ARM_LD2, {  v0_8s, v1_8s,  x0});
    newiopNoret(VOP_ARM_LD2, { v31_8s, v0_8s,  x0});
    newiopNoret(VOP_ARM_LD2, {  v0_8s, v1_8s, x15});

    newiopNoret(VOP_ARM_LD2, {  v0_4s, v1_4s,  x0});
    newiopNoret(VOP_ARM_LD2, { v31_4s, v0_4s,  x0});
    newiopNoret(VOP_ARM_LD2, {  v0_4s, v1_4s, x15});

    newiopNoret(VOP_ARM_LD2, {  v0_2s, v1_2s,  x0});
    newiopNoret(VOP_ARM_LD2, { v31_2s, v0_2s,  x0});
    newiopNoret(VOP_ARM_LD2, {  v0_2s, v1_2s, x15});

    newiopNoret(VOP_ARM_LD1, {  v0_16s,  x0 });
    newiopNoret(VOP_ARM_LD1, { v31_16s,  x0 });
    newiopNoret(VOP_ARM_LD1, {  v0_16s, x15 });

    newiopNoret(VOP_ARM_LD1, {  v0_8s , x0 });
    newiopNoret(VOP_ARM_LD1, { v31_8s , x0 });
    newiopNoret(VOP_ARM_LD1, {  v0_8s ,x15 });

    newiopNoret(VOP_ARM_LD1, {  v0_4s , x0 });
    newiopNoret(VOP_ARM_LD1, { v31_4s , x0 });
    newiopNoret(VOP_ARM_LD1, {  v0_4s ,x15 });

    newiopNoret(VOP_ARM_LD1, {  v0_2s , x0 });
    newiopNoret(VOP_ARM_LD1, { v31_2s , x0 });
    newiopNoret(VOP_ARM_LD1, {  v0_2s ,x15 });

//    newiopNoret(VOP_ARM_LD4, {  v0_16s,  x0 });
//    newiopNoret(VOP_ARM_LD4, { v31_16s,  x0 });
//    newiopNoret(VOP_ARM_LD4, {  v0_16s, x15 });
//
//    newiopNoret(VOP_ARM_LD4, {  v0_8s , x0 });
//    newiopNoret(VOP_ARM_LD4, { v31_8s , x0 });
//    newiopNoret(VOP_ARM_LD4, {  v0_8s ,x15 });
//
//    newiopNoret(VOP_ARM_LD4, {  v0_4s , x0 });
//    newiopNoret(VOP_ARM_LD4, { v31_4s , x0 });
//    newiopNoret(VOP_ARM_LD4, {  v0_4s ,x15 });
//
//    newiopNoret(VOP_ARM_LD4, {  v0_2s , x0 });
//    newiopNoret(VOP_ARM_LD4, { v31_2s , x0 });
//    newiopNoret(VOP_ARM_LD4, {  v0_2s ,x15 });

    newiopNoret(VOP_ARM_ST1, {  x0, v0_16s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v31_16s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v0_16s, argIImm(15)});
    newiopNoret(VOP_ARM_ST1, {  x15, v0_16s, argIImm(0)});

    newiopNoret(VOP_ARM_ST1, {  x0, v0_8s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v31_8s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v0_8s, argIImm(7)});
    newiopNoret(VOP_ARM_ST1, {  x15, v0_8s, argIImm(0)});

    newiopNoret(VOP_ARM_ST1, {  x0, v0_4s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v31_4s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v0_4s, argIImm(3)});
    newiopNoret(VOP_ARM_ST1, {  x15, v0_4s, argIImm(0)});

    newiopNoret(VOP_ARM_ST1, {  x0, v0_2s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v31_2s, argIImm(0)});
    newiopNoret(VOP_ARM_ST1, {  x0, v0_2s, argIImm(1)});
    newiopNoret(VOP_ARM_ST1, {  x15, v0_2s, argIImm(0)});

    newiopNoret(VOP_ARM_ST1, {  x0,  v0_16s });
    newiopNoret(VOP_ARM_ST1, { x15,  v0_16s });
    newiopNoret(VOP_ARM_ST1, {  x0, v31_16s });

    newiopNoret(VOP_ARM_ST1, {  x0,  v0_8s });
    newiopNoret(VOP_ARM_ST1, { x15,  v0_8s });
    newiopNoret(VOP_ARM_ST1, {  x0, v31_8s });

    newiopNoret(VOP_ARM_ST1, {  x0,  v0_4s });
    newiopNoret(VOP_ARM_ST1, { x15,  v0_4s });
    newiopNoret(VOP_ARM_ST1, {  x0, v31_4s });

    newiopNoret(VOP_ARM_ST1, {  x0,  v0_2s });
    newiopNoret(VOP_ARM_ST1, { x15,  v0_2s });
    newiopNoret(VOP_ARM_ST1, {  x0, v31_2s });

//    newiopNoret(VOP_ARM_ST4, {  x0,  v0_16s });
//    newiopNoret(VOP_ARM_ST4, { x15,  v0_16s });
//    newiopNoret(VOP_ARM_ST4, {  x0, v31_16s });
//
//    newiopNoret(VOP_ARM_ST4, {  x0,  v0_8s });
//    newiopNoret(VOP_ARM_ST4, { x15,  v0_8s });
//    newiopNoret(VOP_ARM_ST4, {  x0, v31_8s });
//
//    newiopNoret(VOP_ARM_ST4, {  x0,  v0_4s });
//    newiopNoret(VOP_ARM_ST4, { x15,  v0_4s });
//    newiopNoret(VOP_ARM_ST4, {  x0, v31_4s });
//
//    newiopNoret(VOP_ARM_ST4, {  x0,  v0_2s });
//    newiopNoret(VOP_ARM_ST4, { x15,  v0_2s });
//    newiopNoret(VOP_ARM_ST4, {  x0, v31_2s });

    newiopNoret(VOP_ARM_EXT, { v0_16s, v0_16s, v0_16s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v31_16s, v0_16s, v0_16s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_16s, v31_16s, v0_16s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_16s, v0_16s, v31_16s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_16s, v0_16s, v0_16s, argIImm(15)});

    newiopNoret(VOP_ARM_EXT, { v0_8s, v0_8s, v0_8s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v31_8s, v0_8s, v0_8s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_8s, v31_8s, v0_8s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_8s, v0_8s, v31_8s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_8s, v0_8s, v0_8s, argIImm(7)});

    newiopNoret(VOP_ARM_EXT, { v0_4s, v0_4s, v0_4s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v31_4s, v0_4s, v0_4s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_4s, v31_4s, v0_4s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_4s, v0_4s, v31_4s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_4s, v0_4s, v0_4s, argIImm(3)});

    newiopNoret(VOP_ARM_EXT, { v0_2s, v0_2s, v0_2s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v31_2s, v0_2s, v0_2s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_2s, v31_2s, v0_2s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_2s, v0_2s, v31_2s, argIImm(0)});
    newiopNoret(VOP_ARM_EXT, { v0_2s, v0_2s, v0_2s, argIImm(1)});

    newiopNoret(VOP_FMA, { v0_8f, v0_8f, v0_8f, v0_8f, argIImm(0)});
    newiopNoret(VOP_FMA, { v31_8f, v31_8f, v0_8f, v0_8f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_8f, v0_8f, v31_8f, v0_8f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_8f, v0_8f, v0_8f, v15_8f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_8f, v0_8f, v0_8f, v0_8f, argIImm(7)});

    newiopNoret(VOP_FMA, { v0_4f, v0_4f, v0_4f, v0_4f, argIImm(0)});
    newiopNoret(VOP_FMA, { v31_4f, v31_4f, v0_4f, v0_4f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_4f, v0_4f, v31_4f, v0_4f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_4f, v0_4f, v0_4f, v31_4f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_4f, v0_4f, v0_4f, v0_4f, argIImm(3)});

    newiopNoret(VOP_FMA, { v0_2f, v0_2f, v0_2f, v0_2f, argIImm(0)});
    newiopNoret(VOP_FMA, { v31_2f, v31_2f, v0_2f, v0_2f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_2f, v0_2f, v31_2f, v0_2f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_2f, v0_2f, v0_2f, v31_2f, argIImm(0)});
    newiopNoret(VOP_FMA, { v0_2f, v0_2f, v0_2f, v0_2f, argIImm(1)});

    newiopNoret(VOP_GT, { v0_16u, v0_16u, v0_16u});
    newiopNoret(VOP_GT, { v31_16u, v0_16u, v0_16u});
    newiopNoret(VOP_GT, { v0_16u, v31_16u, v0_16u});
    newiopNoret(VOP_GT, { v0_16u, v0_16u, v31_16u});
    newiopNoret(VOP_GT, { v0_8u, v0_8u, v0_8u});
    newiopNoret(VOP_GT, { v31_8u, v0_8u, v0_8u});
    newiopNoret(VOP_GT, { v0_8u, v31_8u, v0_8u});
    newiopNoret(VOP_GT, { v0_8u, v0_8u, v31_8u});
    newiopNoret(VOP_GT, { v0_4u, v0_4u, v0_4u});
    newiopNoret(VOP_GT, { v31_4u, v0_4u, v0_4u});
    newiopNoret(VOP_GT, { v0_4u, v31_4u, v0_4u});
    newiopNoret(VOP_GT, { v0_4u, v0_4u, v31_4u});
    newiopNoret(VOP_GT, { v0_2u, v0_2u, v0_2u});
    newiopNoret(VOP_GT, { v31_2u, v0_2u, v0_2u});
    newiopNoret(VOP_GT, { v0_2u, v31_2u, v0_2u});
    newiopNoret(VOP_GT, { v0_2u, v0_2u, v31_2u});

    newiopNoret(VOP_GE, { v0_16u, v0_16u, v0_16u});
    newiopNoret(VOP_GE, { v31_16u, v0_16u, v0_16u});
    newiopNoret(VOP_GE, { v0_16u, v31_16u, v0_16u});
    newiopNoret(VOP_GE, { v0_16u, v0_16u, v31_16u});
    newiopNoret(VOP_GE, { v0_8u, v0_8u, v0_8u});
    newiopNoret(VOP_GE, { v31_8u, v0_8u, v0_8u});
    newiopNoret(VOP_GE, { v0_8u, v31_8u, v0_8u});
    newiopNoret(VOP_GE, { v0_8u, v0_8u, v31_8u});
    newiopNoret(VOP_GE, { v0_4u, v0_4u, v0_4u});
    newiopNoret(VOP_GE, { v31_4u, v0_4u, v0_4u});
    newiopNoret(VOP_GE, { v0_4u, v31_4u, v0_4u});
    newiopNoret(VOP_GE, { v0_4u, v0_4u, v31_4u});
    newiopNoret(VOP_GE, { v0_2u, v0_2u, v0_2u});
    newiopNoret(VOP_GE, { v31_2u, v0_2u, v0_2u});
    newiopNoret(VOP_GE, { v0_2u, v31_2u, v0_2u});
    newiopNoret(VOP_GE, { v0_2u, v0_2u, v31_2u});

    newiopNoret(VOP_EQ, { v0_16u, v0_16s, v0_16s});
    newiopNoret(VOP_EQ, { v31_16u, v0_16s, v0_16s});
    newiopNoret(VOP_EQ, { v0_16u, v31_16s, v0_16s});
    newiopNoret(VOP_EQ, { v0_16u, v0_16s, v31_16s});
    newiopNoret(VOP_EQ, { v0_8u, v0_8s, v0_8s});
    newiopNoret(VOP_EQ, { v31_8u, v0_8s, v0_8s});
    newiopNoret(VOP_EQ, { v0_8u, v31_8s, v0_8s});
    newiopNoret(VOP_EQ, { v0_8u, v0_8s, v31_8s});
    newiopNoret(VOP_EQ, { v0_4u, v0_4s, v0_4s});
    newiopNoret(VOP_EQ, { v31_4u, v0_4s, v0_4s});
    newiopNoret(VOP_EQ, { v0_4u, v31_4s, v0_4s});
    newiopNoret(VOP_EQ, { v0_4u, v0_4s, v31_4s});
    newiopNoret(VOP_EQ, { v0_2u, v0_2s, v0_2s});
    newiopNoret(VOP_EQ, { v31_2u, v0_2s, v0_2s});
    newiopNoret(VOP_EQ, { v0_2u, v31_2s, v0_2s});
    newiopNoret(VOP_EQ, { v0_2u, v0_2s, v31_2s});

    newiopNoret(VOP_GE, { v0_16u, v0_16s, v0_16s});
    newiopNoret(VOP_GE, { v31_16u, v0_16s, v0_16s});
    newiopNoret(VOP_GE, { v0_16u, v31_16s, v0_16s});
    newiopNoret(VOP_GE, { v0_16u, v0_16s, v31_16s});
    newiopNoret(VOP_GE, { v0_8u, v0_8s, v0_8s});
    newiopNoret(VOP_GE, { v31_8u, v0_8s, v0_8s});
    newiopNoret(VOP_GE, { v0_8u, v31_8s, v0_8s});
    newiopNoret(VOP_GE, { v0_8u, v0_8s, v31_8s});
    newiopNoret(VOP_GE, { v0_4u, v0_4s, v0_4s});
    newiopNoret(VOP_GE, { v31_4u, v0_4s, v0_4s});
    newiopNoret(VOP_GE, { v0_4u, v31_4s, v0_4s});
    newiopNoret(VOP_GE, { v0_4u, v0_4s, v31_4s});
    newiopNoret(VOP_GE, { v0_2u, v0_2s, v0_2s});
    newiopNoret(VOP_GE, { v31_2u, v0_2s, v0_2s});
    newiopNoret(VOP_GE, { v0_2u, v31_2s, v0_2s});
    newiopNoret(VOP_GE, { v0_2u, v0_2s, v31_2s});

    newiopNoret(VOP_GT, { v0_16u, v0_16s, v0_16s});
    newiopNoret(VOP_GT, { v31_16u, v0_16s, v0_16s});
    newiopNoret(VOP_GT, { v0_16u, v31_16s, v0_16s});
    newiopNoret(VOP_GT, { v0_16u, v0_16s, v31_16s});
    newiopNoret(VOP_GT, { v0_8u, v0_8s, v0_8s});
    newiopNoret(VOP_GT, { v31_8u, v0_8s, v0_8s});
    newiopNoret(VOP_GT, { v0_8u, v31_8s, v0_8s});
    newiopNoret(VOP_GT, { v0_8u, v0_8s, v31_8s});
    newiopNoret(VOP_GT, { v0_4u, v0_4s, v0_4s});
    newiopNoret(VOP_GT, { v31_4u, v0_4s, v0_4s});
    newiopNoret(VOP_GT, { v0_4u, v31_4s, v0_4s});
    newiopNoret(VOP_GT, { v0_4u, v0_4s, v31_4s});
    newiopNoret(VOP_GT, { v0_2u, v0_2s, v0_2s});
    newiopNoret(VOP_GT, { v31_2u, v0_2s, v0_2s});
    newiopNoret(VOP_GT, { v0_2u, v31_2s, v0_2s});
    newiopNoret(VOP_GT, { v0_2u, v0_2s, v31_2s});

    newiopNoret(VOP_GETLANE, { x0,  v0_16u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x15, v0_16u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0, v31_16u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,  v0_16u, argIImm(15) });
    newiopNoret(VOP_GETLANE, { x0,   v0_8u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x15,  v0_8u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,  v31_8u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,   v0_8u, argIImm(7)});
    newiopNoret(VOP_GETLANE, { x0,   v0_4u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x15,  v0_4u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,  v31_4u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,   v0_4u, argIImm(3)});
    newiopNoret(VOP_GETLANE, { x0,   v0_2u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x15,  v0_2u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,  v31_2u, argIImm(0)});
    newiopNoret(VOP_GETLANE, { x0,   v0_2u, argIImm(1)});

    newiopNoret(VOP_SETLANE, {  v0_16u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, { v31_16u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {  v0_16u, argIImm(15),  x0});
    newiopNoret(VOP_SETLANE, {  v0_16u,  argIImm(0), x15});
    newiopNoret(VOP_SETLANE, {   v0_8u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {  v31_8u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {   v0_8u,  argIImm(7),  x0});
    newiopNoret(VOP_SETLANE, {   v0_8u,  argIImm(0), x15});
    newiopNoret(VOP_SETLANE, {   v0_4u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {  v31_4u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {   v0_4u,  argIImm(3),  x0});
    newiopNoret(VOP_SETLANE, {   v0_4u,  argIImm(0), x15});
    newiopNoret(VOP_SETLANE, {   v0_2u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {  v31_2u,  argIImm(0),  x0});
    newiopNoret(VOP_SETLANE, {   v0_2u,  argIImm(1),  x0});
    newiopNoret(VOP_SETLANE, {   v0_2u,  argIImm(0), x15});

    newiopNoret(VOP_SETLANE, {  v0_16u,  argIImm(0),  v0_16u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v31_16u,  argIImm(0),  v0_16u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_16u,  argIImm(15),  v0_16u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_16u,  argIImm(0),  v31_16u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_16u,  argIImm(0),  v0_16u, argIImm(15)});
    newiopNoret(VOP_SETLANE, {  v0_8u,  argIImm(0),  v0_8u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v31_8u,  argIImm(0),  v0_8u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_8u,  argIImm(7),  v0_8u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_8u,  argIImm(0),  v31_8u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_8u,  argIImm(0),  v0_8u, argIImm(7)});
    newiopNoret(VOP_SETLANE, {  v0_4u,  argIImm(0),  v0_4u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v31_4u,  argIImm(0),  v0_4u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_4u,  argIImm(3),  v0_4u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_4u,  argIImm(0),  v31_4u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_4u,  argIImm(0),  v0_4u, argIImm(3)});
    newiopNoret(VOP_SETLANE, {  v0_2u,  argIImm(0),  v0_2u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v31_2u,  argIImm(0),  v0_2u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_2u,  argIImm(1),  v0_2u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_2u,  argIImm(0),  v31_2u, argIImm(0)});
    newiopNoret(VOP_SETLANE, {  v0_2u,  argIImm(0),  v0_2u, argIImm(1)});

    newiopNoret(VOP_SELECT, { v0_16u, v0_16u, v0_16u,   v0_16u});
    newiopNoret(VOP_SELECT, { v31_16u, v31_16u, v0_16u, v0_16u});
    newiopNoret(VOP_SELECT, { v0_16u, v0_16u, v31_16u,  v0_16u});
    newiopNoret(VOP_SELECT, { v0_16u, v0_16u, v0_16u,  v31_16u});

    newiopNoret(OP_CALL_NORET, {  x0 });
    newiopNoret(OP_CALL_NORET, { x15 });
});
#undef DEFINE_CERTAIN_REG
};
#endif
#endif// __LOOPS_ARCH == __LOOPS_AARCH64
