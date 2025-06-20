/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "loops/loops.hpp"
#include "tests.hpp"
#include <iostream>
#include <cstring>
#define  _USE_MATH_DEFINES
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
        VOID_CALL_(hw);
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

#if __LOOPS_OS == __LOOPS_WINDOWS
#undef min // Windows.h implements min and max as macro.
#undef max //
#endif

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
                    VOID_CALL_(snake_dprint, x, y);
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
                IF_(CALL_(lesser_dbl, load_<double>(ptr, ipos), load_<double>(ptr, minpos)))
                    minpos = ipos;
                ipos += sizeof(double);
            }
            IF_(minpos != curpos)
            {
                IReg cur_ = load_<double>(ptr, curpos); 
                IReg min_ = load_<double>(ptr, minpos);
                store_<double>(ptr, minpos, cur_);
                store_<double>(ptr, curpos, min_);
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

inline int64_t double_2_i64(double a)
{
    union uconv
    {
        int64_t val64;
        double val;
        uconv() : val64(0) {} 
    } conv;
    conv.val = a;
    return conv.val64;
}

inline double i64_2_double(int64_t a)
{
    union uconv
    {
        int64_t val64;
        double val;
        uconv() : val64(0) {} 
    } conv;
    conv.val64 = a;
    return conv.val;
}

static int64_t double_apb(int64_t a, int64_t b)
{
    return double_2_i64(i64_2_double(a) + i64_2_double(b));
}

enum {VST_RECTANGLE, VST_CIRCLE, VST_TRIANGLE};
struct virtualstruct
{
    int8_t vs_type;
    double val1; //Rectangle: x1 | Circle: xc | Tringle: x1
    double val2; //Rectangle: y1 | Circle: yc | Tringle: y1
    double val3; //Rectangle: x2 | Circle: R  | Tringle: x2
    double val4; //Rectangle: y2 | Circle: -- | Tringle: y2
    double val5; //Rectangle: -- | Circle: -- | Tringle: x3
    double val6; //Rectangle: -- | Circle: -- | Tringle: y3
};

static int64_t vs_rectangle_area(virtualstruct* obj)
{
    return double_2_i64(std::abs(obj->val1-obj->val3)*std::abs(obj->val2-obj->val4));
}

static int64_t vs_circle_area(virtualstruct* obj)
{
    return double_2_i64(M_PI*obj->val3*obj->val3);
}

static int64_t vs_triangle_area(virtualstruct* obj)
{
    return double_2_i64(0.5*std::abs((obj->val1-obj->val5)*(obj->val4-obj->val6)-(obj->val3-obj->val5)*(obj->val4-obj->val6)));
}

static void vs_rectangle_print(virtualstruct* obj)
{
    get_test_ostream()<< "Rectangle: (x1,y1) = (" << obj->val1 << ", " << obj->val2 << "), (x1,y1) = (" << obj->val3 << ", " << obj->val4 << ")" << std::endl;
}

static void vs_circle_print(virtualstruct* obj)
{
    get_test_ostream()<< "Circle: (xc,yc) = (" << obj->val1 << ", " << obj->val2<<"), R = " << obj->val3 << std::endl;
}

static void vs_triangle_print(virtualstruct* obj)
{
    get_test_ostream()<< "Triangle: (x1,y1) = (" << obj->val1 << ", " << obj->val2 << "), (x2,y2) = (" << obj->val3 << ", " << obj->val4 << "), (x3,y3) = (" << obj->val5 << ", " << obj->val6 << ")" << std::endl;
}

static void vs_rectangle_init(virtualstruct* obj, double x1, double y1, double x2, double y2)
{
    obj->vs_type = VST_RECTANGLE;
    obj->val1 = x1;
    obj->val2 = y1;
    obj->val3 = x2;
    obj->val4 = y2;
}

static void vs_circle_init(virtualstruct* obj, double xc, double yc, double R)
{
    obj->vs_type = VST_CIRCLE;
    obj->val1 = xc;
    obj->val2 = yc;
    obj->val3 = R;
}

static void vs_triangle_init(virtualstruct* obj, double x1, double y1, double x2, double y2, double x3, double y3)
{
    obj->vs_type = VST_TRIANGLE;
    obj->val1 = x1;
    obj->val2 = y1;
    obj->val3 = x2;
    obj->val4 = y2;
    obj->val5 = x3;
    obj->val6 = y3;
}

TEST(calls, area_sum)
{
    Context ctx;
    USE_CONTEXT_(ctx);
    IReg objarray, objamount;
    STARTFUNC_(test_info_->name(), &objarray, &objamount)
    {
        IReg sum = CONST_(0);
        WHILE_(objamount > 0)
        {
            IReg type = load_<int8_t>(objarray);
            IReg printfunc = select(type == VST_RECTANGLE, CONST_((int64_t)(void*)&vs_rectangle_print), select(type == VST_CIRCLE, CONST_((int64_t)(void*)&vs_circle_print), CONST_((int64_t)(void*)&vs_triangle_print)));
            IReg areafunc = select(type == VST_RECTANGLE, CONST_((int64_t)(void*)&vs_rectangle_area), select(type == VST_CIRCLE, CONST_((int64_t)(void*)&vs_circle_area), CONST_((int64_t)(void*)&vs_triangle_area)));
            VOID_CALL_(printfunc, objarray);
            IReg area = CALL_(areafunc, objarray);
            sum = CALL_(&double_apb, sum, area);
            objarray += sizeof(virtualstruct);
            objamount = objamount - 1;
        }
        RETURN_(sum);
    }
    typedef int64_t(*area_sum_f)(virtualstruct*, int64_t);
    loops::Func func = ctx.getFunc(test_info_->name());
    switch_spill_stress_test_mode_on(func);
    EXPECT_IR_CORRECT_TOLERABLE_DEFECT(func);
    EXPECT_ASSEMBLY_CORRECT_TOLERABLE_DEFECT(func);

    virtualstruct arr[10];

    vs_rectangle_init(arr + 0, -5.5, 10, 15, 52);
    vs_circle_init   (arr + 1, 4, 5.9, 1.6);
    vs_triangle_init (arr + 2, -2.5, 15.1, 25.0, 1.6, 17.89, 54.2);
    vs_triangle_init (arr + 3, 1.9, 2.6, 17.0, 23.7, -2.8, 15.6);
    vs_circle_init   (arr + 4, -5.87, -53.7, 6.78);
    vs_rectangle_init(arr + 5, 75.3, 3.57, 1.8, 1.903);
    vs_triangle_init (arr + 6, 7.3, -4.5, 2.7, 1.29, -10.5, 112);
    vs_rectangle_init(arr + 7, 67.41, -12.5, 83.7, 0.5);
    vs_circle_init   (arr + 8, 0.9, -78.6, 15.2);
    vs_triangle_init (arr + 9, 7.91, 84.2, 69.3, -12.1, 87.0, -1.7);

    area_sum_f tested = reinterpret_cast<area_sum_f>(func.ptr());
    reset_test_ostream();
    double area = i64_2_double(tested(arr, 10));
    std::string res = get_test_ostream_result();
    reset_test_ostream();
    double ref_area = 0;
    for(int objnum = 0; objnum < 10; objnum++)
    {
        switch (arr[objnum].vs_type)
        {
        case VST_RECTANGLE:
            vs_rectangle_print(arr + objnum);
            ref_area += i64_2_double(vs_rectangle_area(arr + objnum));
            break;
        case VST_CIRCLE:
            vs_circle_print(arr + objnum);
            ref_area += i64_2_double(vs_circle_area(arr + objnum));
            break;
        case VST_TRIANGLE:
            vs_triangle_print(arr + objnum);
            ref_area += i64_2_double(vs_triangle_area(arr + objnum));
            break;
        default:
            break;
        }
    }
    std::string ref_res = get_test_ostream_result();
    reset_test_ostream();
    ASSERT_EQ(res, ref_res);
    ASSERT_EQ(area, ref_area);
}