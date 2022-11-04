/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "loopslayers/loopslayers.h"

#if __LOOPS_ARCH == __LOOPS_AARCH64
#include "generators/depthwiseconv.hpp" 
#endif //__LOOPS_ARCH == __LOOPS_AARCH64

loops::Context CTX;

dwconv_f32_t generate_dwc_f32(int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type, float alpha)
{
#if __LOOPS_ARCH != __LOOPS_AARCH64
        return 0;
#else
    if(kh <= 0 || kw <= 0 || padding_top < 0 || padding_left < 0 || padding_bottom < 0 || padding_right < 0 || kh * kw > 39*39 || 
       (activation_type != ACT_NONE && activation_type != ACT_RELU && activation_type != ACT_RELU6 && activation_type != ACT_LRELU))
        return 0;
    return (dwconv_f32_t)loops::DepthwiseconvGenerator<float>(CTX).generate(kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation_type, alpha);
#endif 
}

void calc_dwc_algs_limits_f32(dwc_algs_limits* out, int C, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right)
{
#if __LOOPS_ARCH != __LOOPS_AARCH64
        *out = dwc_algs_limits(0,0,0,0,0,0,0,0,0,0);
        return;
#else
        if(
           C <= 0 ||
           H <= 0 ||
           W <= 0 ||
           H0 <= 0 ||
           W0 <= 0 ||
           kh <= 0 ||
           kw <= 0 || 
           (H0 != H - kh + 1 + padding_top + padding_bottom) ||
           (W0 != W - kw + 1 + padding_left + padding_right) ||
           padding_top < 0 ||
           padding_left < 0 ||
           padding_bottom < 0 ||
           padding_right < 0 ||
           kh * kw > 39*39)
        {
            *out = dwc_algs_limits(0,0,0,0,0,0,0,0,0,0);
            return;
        }
        *out = loops::DepthwiseconvGenerator<float>(CTX).calc_dwc_algs_limits(C, H, W, kh, kw, H0, W0, padding_top, padding_left, padding_bottom, padding_right);
        return;
#endif
}

dwconv_f16_t generate_dwc_f16(int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type, float alpha)
{
#if __LOOPS_ARCH != __LOOPS_AARCH64
        return 0;
#else
    if(kh <= 0 || kw <= 0 || padding_top < 0 || padding_left < 0 || padding_bottom < 0 || padding_right < 0 || kh * kw > 39*39 || 
       (activation_type != ACT_NONE && activation_type != ACT_RELU && activation_type != ACT_RELU6 && activation_type != ACT_LRELU))
        return 0;
    return (dwconv_f16_t)loops::DepthwiseconvGenerator<loops::f16_t>(CTX).generate(kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation_type, alpha);
#endif 
}

void calc_dwc_algs_limits_f16(dwc_algs_limits* out, int C, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right)
{
#if __LOOPS_ARCH != __LOOPS_AARCH64
        *out = dwc_algs_limits(0,0,0,0,0,0,0,0,0,0);
        return;
#else
        if(
           C <= 0 ||
           H <= 0 ||
           W <= 0 ||
           H0 <= 0 ||
           W0 <= 0 ||
           kh <= 0 ||
           kw <= 0 || 
           (H0 != H - kh + 1 + padding_top + padding_bottom) ||
           (W0 != W - kw + 1 + padding_left + padding_right) ||
           padding_top < 0 ||
           padding_left < 0 ||
           padding_bottom < 0 ||
           padding_right < 0 ||
           kh * kw > 39*39)
        {
            *out = dwc_algs_limits(0,0,0,0,0,0,0,0,0,0);
            return;
        }
        *out = loops::DepthwiseconvGenerator<loops::f16_t>(CTX).calc_dwc_algs_limits(C, H, W, kh, kw, H0, W0, padding_top, padding_left, padding_bottom, padding_right);
        return;
#endif
}