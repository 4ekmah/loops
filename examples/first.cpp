#include "loops/loops.hpp"
#include <vector>

typedef int (*minmaxfunc_t)(const int* ptr, int64_t n,
                            int64_t* minpos, int64_t* maxpos);

namespace loops
{
minmaxfunc_t gencode(Context& ctx)
{
    // TODO
    return 0;
}
}

int main(int argc, char** argv)
{
    loops::Context ctx;
    minmaxfunc_t f = gencode(ctx);

    std::vector<int> v = {1, 2, -5, 7, 8};
    int64_t minpos = -1, maxpos = -1;
    int retval = f(&v[0], (int64_t)v.size(), &minpos, &maxpos);
    printf("retval=%d, minpos=%d, maxpos=%d\n", retval, (int)minpos, (int)maxpos);

    return 0;
}
