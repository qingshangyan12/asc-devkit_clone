/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef ASCENDC_MODULE_SIMT_BF16_C_IMPL_H
#define ASCENDC_MODULE_SIMT_BF16_C_IMPL_H

__simt_callee__ inline bool __hisnan(bfloat16_t x)
{
    return __isnan(x);
}

__simt_callee__ inline bool __hisinf(bfloat16_t x)
{
    return __isinf(x);
}

__simt_callee__ inline bfloat16_t __habs(bfloat16_t x)
{
    uint16_t bits = *(uint16_t*)&x;
    bits &= 0x7FFF;
    return *(bfloat16_t*)&bits;
}

__simt_callee__ inline bfloat16_t __hfma(bfloat16_t x, bfloat16_t y, bfloat16_t z)
{
    return __fma(x, y, z);
}

__simt_callee__ inline bfloat16_t __hmax(bfloat16_t x, bfloat16_t y)
{
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

__simt_callee__ inline bfloat16_t __hmin(bfloat16_t x, bfloat16_t y)
{
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

__simt_callee__ inline bfloat16_t hcos(bfloat16_t x)
{
    float tmp = __cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
    tmp = cosf(tmp);
    return __cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(tmp);
}

__simt_callee__ inline bfloat16_t hsin(bfloat16_t x)
{
    float tmp = __cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
    tmp = sinf(tmp);
    return __cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(tmp);
}

__simt_callee__ inline bfloat16_t htanh(bfloat16_t x)
{
    float tmp = __cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
    tmp = tanhf(tmp);
    return __cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(tmp);
}

__simt_callee__ inline bfloat16_t hexp(bfloat16_t x)
{
    float tmp = __cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
    tmp = expf(tmp);
    return __cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(tmp);
}

__simt_callee__ inline bfloat16_t hexp2(bfloat16_t x)
{
    float tmp = __cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
    tmp = exp2f(tmp);
    return __cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(tmp);
}
__simt_callee__ inline bfloat16_t hexp10(bfloat16_t x)
{
    float tmp = __cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
    tmp = exp10f(tmp);
    return __cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(tmp);
}

__simt_callee__ inline bfloat16_t hlog(bfloat16_t x)
{
    float tmp = __cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
    tmp = logf(tmp);
    return __cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(tmp);
}

__simt_callee__ inline bfloat16_t hlog2(bfloat16_t x)
{
    float tmp = __cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
    tmp = log2f(tmp);
    return __cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(tmp);
}

__simt_callee__ inline bfloat16_t hlog10(bfloat16_t x)
{
    float tmp = __cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
    tmp = log10f(tmp);
    return __cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(tmp);
}

__simt_callee__ inline bfloat16_t hsqrt(bfloat16_t x)
{
    float tmp = __cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
    tmp = sqrtf(tmp);
    return __cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(tmp);
}

__simt_callee__ inline bfloat16_t hrsqrt(bfloat16_t x)
{
    float tmp = __cvt_float<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(x);
    tmp = 1.0f / sqrtf(tmp);
    return __cvt_bfloat16_t<ROUND::R, RoundingSaturation::RS_DISABLE_VALUE>(tmp);
}

__simt_callee__ inline bfloat16_t hrcp(bfloat16_t x)
{
    return (bfloat16_t)1.0 / x;
}

#endif  // ASCENDC_MODULE_SIMT_BF16_C_IMPL_H