/*
This is a part of Loops project.
Distributed under Apache 2 license.
See https://github.com/4ekmah/loops/LICENSE
*/
#include "loopslayers/loopslayers.h"

#if __LOOPS_ARCH == __LOOPS_AARCH64
#include "generators/depthwiseconv.hpp" 
#include "generators/maxpool.hpp"
#endif //__LOOPS_ARCH == __LOOPS_AARCH64

loops_context create_context()
{
#if __LOOPS_ARCH != __LOOPS_AARCH64
    return 0;
#else
    return (void*)(new loops::Context);
#endif
} 

void free_context(loops_context ctx)
{
    delete ((loops::Context*)(ctx));
}

dwconv_f32_t generate_dwc_f32(loops_context ctx, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x, int dilation_y, int dilation_x, int activation_type, float alpha)
{
#if __LOOPS_ARCH != __LOOPS_AARCH64
        return 0;
#else
    loops::Context& CTX = *(loops::Context*)(ctx);
    if(kh <= 0 || kw <= 0 || padding_top < 0 || padding_left < 0 || padding_bottom < 0 || padding_right < 0 ||
       kh * kw > 39*39 || stride_x < 1 || stride_y < 1 || dilation_x != 1 || dilation_y != 1 ||
       (activation_type != ACT_NONE && activation_type != ACT_RELU && activation_type != ACT_RELU6 && activation_type != ACT_LRELU))
        return 0;
    try  
    {
        return (dwconv_f32_t)loops::DepthwiseconvGenerator<float>(CTX).generate(kh, kw, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, activation_type, alpha);
    }
    catch(...)
    {
        return 0; 
    };
#endif 
}

void calc_dwc_algs_limits_f32(loops_context ctx, dwc_algs_limits* out, int NC, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x, int dilation_y, int dilation_x)
{
#if __LOOPS_ARCH != __LOOPS_AARCH64
        *out = dwc_algs_limits(0,0,0,0,0,0,0,0,0,0);
        return;
#else
    loops::Context& CTX = *(loops::Context*)(ctx);
    if(NC <= 0 ||
       H <= 0 ||
       W <= 0 ||
       H0 <= 0 ||
       W0 <= 0 ||
       kh <= 0 ||
       kw <= 0 || 
       (H0 != (H + padding_top + padding_bottom - kh) / stride_y + 1) ||
       (W0 != (W + padding_left + padding_right - kw) / stride_x + 1) ||
       padding_top < 0 ||
       padding_left < 0 ||
       padding_bottom < 0 ||
       padding_right < 0 ||
       stride_x < 1 ||
       stride_y < 1 ||
       dilation_x != 1 ||
       dilation_y != 1 ||
       kh * kw > 39*39)
    {
        *out = dwc_algs_limits(0,0,0,0,0,0,0,0,0,0);
        return;
    }
    *out = loops::DepthwiseconvGenerator<float>(CTX).calc_dwc_algs_limits(NC, H, W, kh, kw, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x);
    return;
#endif
}

dwconv_f16_t generate_dwc_f16(loops_context ctx, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x, int dilation_y, int dilation_x, int activation_type, float alpha)
{
#if __LOOPS_ARCH != __LOOPS_AARCH64
        return 0;
#else
    loops::Context& CTX = *(loops::Context*)(ctx);
    if(kh <= 0 || kw <= 0 || padding_top < 0 || padding_left < 0 || padding_bottom < 0 || padding_right < 0 ||
       kh * kw > 39*39 || stride_x < 1 || stride_y < 1 || dilation_x != 1 || dilation_y != 1 ||
       (activation_type != ACT_NONE && activation_type != ACT_RELU && activation_type != ACT_RELU6 && activation_type != ACT_LRELU))
        return 0;
    try  
    {
        return (dwconv_f16_t)loops::DepthwiseconvGenerator<loops::f16_t>(CTX).generate(kh, kw, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x, activation_type, alpha);
    }
    catch(...)
    {
        return 0;
    };
#endif 
}

void calc_dwc_algs_limits_f16(loops_context ctx, dwc_algs_limits* out, int NC, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right, int stride_y, int stride_x, int dilation_y, int dilation_x)
{
#if __LOOPS_ARCH != __LOOPS_AARCH64
        *out = dwc_algs_limits(0,0,0,0,0,0,0,0,0,0);
        return;
#else
    loops::Context& CTX = *(loops::Context*)(ctx);
    if(NC <= 0 ||
       H <= 0 ||
       W <= 0 ||
       H0 <= 0 ||
       W0 <= 0 ||
       kh <= 0 ||
       kw <= 0 || 
       (H0 != (H + padding_top + padding_bottom - kh) / stride_y + 1) ||
       (W0 != (W + padding_left + padding_right - kw) / stride_x + 1) ||
       padding_top < 0 ||
       padding_left < 0 ||
       padding_bottom < 0 ||
       padding_right < 0 ||
       stride_x < 1 ||
       stride_y < 1 ||
       dilation_x != 1 ||
       dilation_y != 1 ||
       kh * kw > 39*39)
    {
            *out = dwc_algs_limits(0,0,0,0,0,0,0,0,0,0);
            return;
    }
    *out = loops::DepthwiseconvGenerator<loops::f16_t>(CTX).calc_dwc_algs_limits(NC, H, W, kh, kw, H0, W0, padding_top, padding_left, padding_bottom, padding_right, stride_y, stride_x);
    return;
#endif
}

bool good_alg_limits(struct dwc_algs_limits* out)
{
    return out->Cms != 0 || out->Cme != 0 || out->Cis != 0 || out->Cie != 0 || out->Yms != 0 ||
           out->Yme != 0 || out->Yis != 0 || out->Yie != 0 || out->Xis != 0 || out->Xie != 0;
}

loops::Context gCTX;
maxpool_f32_t generate_maxpool_f32(int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type, float alpha)
{
#if __LOOPS_ARCH != __LOOPS_AARCH64
        return 0;
#else
    if(kh <= 0 || kw <= 0 || padding_top < 0 || padding_left < 0 || padding_bottom < 0 || padding_right < 0 || kh * kw > 39*39 || 
       (activation_type != ACT_NONE && activation_type != ACT_RELU && activation_type != ACT_RELU6 && activation_type != ACT_LRELU))
        return 0;
    return loops::MaxpoolGenerator(gCTX).generate(kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation_type, alpha);
#endif 
}

// maxpool_f32_t generate_maxpool_f32(loops_context ctx, int kh, int kw, int padding_top, int padding_left, int padding_bottom, int padding_right, int activation_type, float alpha)
// {
// #if __LOOPS_ARCH != __LOOPS_AARCH64
//         return 0;
// #else
//     loops::Context& CTX = *(loops::Context*)(ctx);
//     if(kh <= 0 || kw <= 0 || padding_top < 0 || padding_left < 0 || padding_bottom < 0 || padding_right < 0 || kh * kw > 39*39 ||
//        (activation_type != ACT_NONE && activation_type != ACT_RELU && activation_type != ACT_RELU6 && activation_type != ACT_LRELU))
//         return 0;
//     try  
//     {
//         return (maxpool_f32_t)loops::MaxpoolGenerator<float>(CTX).generate(kh, kw, padding_top, padding_left, padding_bottom, padding_right, activation_type, alpha);
//     }
//     catch(...)
//     {
//         return 0; 
//     };
// #endif 
// }

void calc_maxpool_algs_limits_f32(maxpool_algs_limits* out, int C, int W, int H, int kw, int kh, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right)
{
#if __LOOPS_ARCH != __LOOPS_AARCH64
        *out = maxpool_algs_limits(0,0,0,0,0,0,0,0,0,0);
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
            *out = maxpool_algs_limits(0,0,0,0,0,0,0,0,0,0);
            return;
        }
        *out = loops::MaxpoolGenerator(gCTX).calc_maxpool_algs_limits(C, W, H, kw, kh, H0, W0, padding_top, padding_left, padding_bottom, padding_right);
        return;
#endif
}

// void calc_maxpool_algs_limits_f32(loops_context ctx, maxpool_algs_limits* out, int NC, int H, int W, int kh, int kw, int64_t H0, int64_t W0, int padding_top, int padding_left, int padding_bottom, int padding_right)
// {
// #if __LOOPS_ARCH != __LOOPS_AARCH64
//         *out = maxpool_algs_limits(0,0,0,0,0,0,0,0,0,0);
//         return;
// #else
//     loops::Context& CTX = *(loops::Context*)(ctx);
//     if(NC <= 0 ||
//        H <= 0 ||
//        W <= 0 ||
//        H0 <= 0 ||
//        W0 <= 0 ||
//        kh <= 0 ||
//        kw <= 0 || 
//        (H0 != H - kh + 1 + padding_top + padding_bottom) ||
//        (W0 != W - kw + 1 + padding_left + padding_right) ||
//        padding_top < 0 ||
//        padding_left < 0 ||
//        padding_bottom < 0 ||
//        padding_right < 0 ||
//        kh * kw > 39*39)
//     {
//         *out = maxpool_algs_limits(0,0,0,0,0,0,0,0,0,0);
//         return;
//     }
//     *out = loops::MaxpoolGenerator<float>(CTX).calc_maxpool_algs_limits(NC, H, W, kh, kw, H0, W0, padding_top, padding_left, padding_bottom, padding_right);
//     return;
// #endif
// }
