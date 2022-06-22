#include "loops/loops.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

typedef int (*minmaxpos_t)(const int* ptr, int64_t n,
                            int* minpos, int* maxpos);
loops::Func genminmaxloc(loops::Context& CTX)
{
    using namespace loops;
    IReg ptr, n, minpos_addr, maxpos_addr;
    CTX.startFunc("minmaxloc", {&ptr, &n, &minpos_addr, &maxpos_addr});
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
    return CTX.getFunc("minmaxloc");
}

//This implementation(loops and reference) is taken from wikipedia: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
typedef void (*bresenham_t)(uint8_t* canvas, int64_t w, int64_t x0, int64_t y0, int64_t x1, int64_t y1, uint64_t filler);
loops::Func genbresenham(loops::Context& CTX)
{
    using namespace loops;
    IReg canvas, w, x0, y0, x1, y1, filler;
    CTX.startFunc("bresenham", {&canvas, &w, &x0, &y0, &x1, &y1, &filler });
    IReg dx = abs(x1 - x0);
    IReg sx = sign(x1 - x0);
    IReg dy = -abs(y1 - y0);
    IReg sy = sign(y1 - y0);
    IReg error = dx + dy;
    CTX.doif_(canvas != 0); //TODO(ch): this is substitution of while(true) 
        store_<uint8_t>(canvas, y0 * w + x0, filler);
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
    CTX.endFunc();
    return CTX.getFunc("bresenham");
}

void BresenhamRef(uint8_t* canvas, int64_t w, int64_t x0, int64_t y0, int64_t x1, int64_t y1, uint64_t filler)
{
    int64_t dx = std::abs(x1 - x0);
    int64_t sx = (x1 - x0) == 0 ? 0 : ((x1 - x0) > 0? 1 : -1);
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

void printCanvas(uint8_t* canvas, int64_t w, int64_t h)
{
    std::cout << "+";
    for (int i = 0; i < w; i++)
        std::cout << "-";
    std::cout << "+"<<std::endl;

    for (int i = 0; i < h; i++)
    {
        std::cout << "|";
        for (int j = 0; j < w; j++)
            std::cout << canvas[w*i + j];
        std::cout << "|" << std::endl;
    }
    std::cout << "+";
    for (int i = 0; i < w; i++)
        std::cout << "-";
    std::cout << "+" << std::endl;
}


int main(int argc, char** argv)
{
    loops::Context CTX;
    loops::Func mmlfunc = genminmaxloc(CTX);
    loops::Func brsfunc = genbresenham(CTX);
    CTX.compileAll();

    {//MinMaxLoc usage
        std::cout << "--------MINMAXLOCEXAMPLE---------" << std::endl;
        std::cout << "======--BYTECODE-LISTING--=======" << std::endl;
        mmlfunc.printBytecode(std::cout);
        std::string platform = CTX.getPlatformName();
        std::transform(platform.begin(), platform.end(), platform.begin(), ::toupper);
        std::cout << "======--" << platform << "--LISTING--====== = " << std::endl;
        mmlfunc.printAssembly(std::cout);
        std::cout << "======---FUNCTION-USAGE---=======" << std::endl;
        minmaxpos_t f = reinterpret_cast<minmaxpos_t>(mmlfunc.ptr());
        std::vector<int> v = { 8, 2, -5, 7, 6 };
        int minpos = -1, maxpos = -1;
        int retval = f(&v[0], v.size(), &minpos, &maxpos);
        std::cout << "retval=" << retval<< ", minpos="<< (int)minpos <<", maxpos="<< (int)maxpos << std::endl;
        std::cout << std::endl << std::endl << std::endl;
    }

    {//Bresenham usage
        std::cout << "--------BRESENHAMEXAMPLE---------" << std::endl;
        std::cout << "======--BYTECODE-LISTING--=======" << std::endl;
        brsfunc.printBytecode(std::cout);
        std::string platform = CTX.getPlatformName();
        std::transform(platform.begin(), platform.end(), platform.begin(), ::toupper);
        std::cout << "======--" << platform << "--LISTING--====== = " << std::endl;
        brsfunc.printAssembly(std::cout);
        std::cout << "======---FUNCTION-USAGE---=======" << std::endl;
        bresenham_t f = reinterpret_cast<bresenham_t>(brsfunc.ptr());
        int64_t w = 15;
        int64_t h = 15;
        std::vector<uint8_t> canvas(w * h, ' ');
        f(&canvas[0], w, 0, 0, 14, 14, '1');
        f(&canvas[0], w, 0, 11, 7, 7, '2');
        f(&canvas[0], w, 14, 7, 11, 14, '3');
        f(&canvas[0], w, 5, 3, 8, 3, '4');
        f(&canvas[0], w, 9, 2, 9, 13, '5');
        f(&canvas[0], w, 14, 1, 14, 1, '6');
        printCanvas(&canvas[0], w, h);
        std::vector<uint8_t> canvasRef(w * h, ' ');
        BresenhamRef(&canvasRef[0], w, 0, 0, 14, 14, '1');
        BresenhamRef(&canvasRef[0], w, 0, 11, 7, 7, '2');
        BresenhamRef(&canvasRef[0], w, 14, 7, 11, 14, '3');
        BresenhamRef(&canvasRef[0], w, 5, 3, 8, 3, '4');
        BresenhamRef(&canvasRef[0], w, 9, 2, 9, 13, '5');
        BresenhamRef(&canvasRef[0], w, 14, 1, 14, 1, '6');
        printCanvas(&canvasRef[0], w, h);
    }
    
    return 0;
}
