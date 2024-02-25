/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/

#include "loops/loops.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

//This implementation(loops and reference) is taken from wikipedia: https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
typedef void (*bresenham_t)(uint8_t* canvas, int64_t w, int64_t x0, int64_t y0, int64_t x1, int64_t y1, uint64_t filler);
loops::Func genbresenham(loops::Context& CTX)
{
    using namespace loops;
    IReg canvas, w, x0, y0, x1, y1, filler;
    USE_CONTEXT_(CTX);
    STARTFUNC_("bresenham", &canvas, &w, &x0, &y0, &x1, &y1, &filler)
    {
        IReg dx = abs(x1 - x0);
        IReg sx = sign(x1 - x0);
        IReg dy = -abs(y1 - y0);
        IReg sy = sign(y1 - y0);
        IReg error = dx + dy;
        WHILE_(canvas != 0) //TODO(ch): this is substitution of while(true)
        {
            store_<uint8_t>(canvas, y0 * w + x0, filler);
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
    return CTX.getFunc("bresenham");
}

void BresenhamRef(uint8_t* canvas, int64_t w, int64_t x0, int64_t y0, int64_t x1, int64_t y1, uint64_t filler)
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

void printCanvas(uint8_t* canvas, int64_t w, int64_t h)
{
    std::cout << "+";
    for (int i = 0; i < w; i++)
        std::cout << "-";
    std::cout << "+" << std::endl;

    for (int i = 0; i < h; i++)
    {
        std::cout << "|";
        for (int j = 0; j < w; j++)
            std::cout << canvas[w * i + j];
        std::cout << "|" << std::endl;
    }
    std::cout << "+";
    for (int i = 0; i < w; i++)
        std::cout << "-";
    std::cout << "+" << std::endl;
}

int main(int /*argc*/, char** /*argv*/)
{
    loops::Context CTX;
    loops::Func brsfunc = genbresenham(CTX);

    std::cout << "--------BRESENHAMEXAMPLE---------" << std::endl;
    std::cout << "=========--IR-LISTING--==========" << std::endl;
    brsfunc.printIR(std::cout);
    std::string platform = CTX.getPlatformName();
    std::transform(platform.begin(), platform.end(), platform.begin(), [](char t) {return (char)::toupper(t); });
    std::cout << "======--" << platform << "--LISTING--====== = " << std::endl;
    brsfunc.printAssembly(std::cout);
    std::cout << "======--FUNCTION-OUTPUT---=======" << std::endl;
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

    return 0;
}
