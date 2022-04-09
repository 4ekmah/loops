/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/vpisarev/loops/LICENSE
*/

#ifndef __LOOPS_TEST_BASIC_HPP__
#define __LOOPS_TEST_BASIC_HPP__

namespace loops
{
LTEST(min_max_scalar, {
    IReg ptr, n, minpos_addr, maxpos_addr;
    m_ctx.startFunc(TESTNAME, {&ptr, &n, &minpos_addr, &maxpos_addr});
    IReg i = m_ctx.const_(0);
    IReg minpos = m_ctx.const_(0);
    IReg maxpos = m_ctx.const_(0);
    IReg minval = load_<int>(ptr);
    IReg maxval = minval;
    IReg elemsize = m_ctx.const_((int)sizeof(int));
    n *= elemsize;
    m_ctx.do_();
        IReg x = load_<int>(ptr, i);
        m_ctx.if_(x < minval);
            minval = x;
            minpos = i;
        m_ctx.endif_();
        m_ctx.if_(x > maxval);
            maxval = x;
            maxpos = i;
        m_ctx.endif_();
        i += elemsize;
    m_ctx.while_(i < n);
    minpos /= elemsize;
    maxpos /= elemsize;
    store_<int>(minpos_addr, minpos);
    store_<int>(maxpos_addr, maxpos);
    m_ctx.return_(m_ctx.const_(0));
    m_ctx.endFunc();
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


LTEST(a_plus_b, {
    IReg a, b;
    m_ctx.startFunc(TESTNAME, {&a, &b});
    a+=b;
    m_ctx.return_(a);
    m_ctx.endFunc();
});
LTESTexe(a_plus_b, {
    typedef int64_t (*a_plus_b_f)(int64_t a, int64_t b);
    a_plus_b_f tested = reinterpret_cast<a_plus_b_f>(EXEPTR);
    std::vector<int64_t> A = {3,2,4,2,3,4,1};
    std::vector<int64_t> B = {4,4,5,5,4,6,5};
    for(size_t n = 0; n < A.size(); n++)
        TEST_EQ(tested(A[n],B[n]), A[n]+B[n]);
});
};

#endif//__LOOPS_TEST_BASIC_HPP__
