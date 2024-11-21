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
    EXPECT_IR_CORRECT_TOLERABLE_DEFECT(func);
    EXPECT_ASSEMBLY_CORRECT_TOLERABLE_DEFECT(func);
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
    EXPECT_IR_CORRECT_TOLERABLE_DEFECT(func);
    EXPECT_ASSEMBLY_CORRECT_TOLERABLE_DEFECT(func);
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
    EXPECT_IR_CORRECT_TOLERABLE_DEFECT(func);
    EXPECT_ASSEMBLY_CORRECT_TOLERABLE_DEFECT(func);
        sort_double_f tested = reinterpret_cast<sort_double_f>(func.ptr());
    std::vector<double> arr = {7.3, 2.0, 5.3, 10.0, -500000.0, -17.0, 70.0, 1.9, 71212.7878, 12.0};
    std::vector<double> arr_ref = arr;
    tested(&(arr[0]), arr.size());
    std::sort(arr_ref.begin(), arr_ref.end());
    for(int pos = 0; pos < (int)arr_ref.size(); pos++)
        ASSERT_EQ(arr[pos], arr_ref[pos]);

}