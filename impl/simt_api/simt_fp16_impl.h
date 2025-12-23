/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef ASCENDC_MODULE_SIMT_FP16_C_IMPL_H
#define ASCENDC_MODULE_SIMT_FP16_C_IMPL_H

constexpr uint32_t HALF_INF = 0x7C00;
constexpr uint32_t HALF_NEG_INF = 0xFC00;

__simt_callee__ inline bool __hisnan(half x)
{
    return __isnan(x);
}

__simt_callee__ inline bool __hisinf(half x)
{
    return __isinf(x);
}

__simt_callee__ inline half __hfma(half x, half y, half z)
{
    return __fma(x, y, z);
}

__simt_callee__ inline half __habs(half x)
{
    uint16_t bits = *(uint16_t*)&x;
    bits &= 0x7FFF;
    return *(half*)&bits;
}

__aicore__ inline bool ispositiveinf(float x)
{
    uint16_t* intX = (uint16_t*)&x;
    return *intX == HALF_INF;
}

__aicore__ inline bool isnegativeinf(float x)
{
    uint16_t* intX = (uint16_t*)&x;
    return *intX == HALF_NEG_INF;
}

__simt_callee__ inline half __hmax(half x, half y)
{
    if (__hisnan(x)) {
        return y;
    } else if (__hisnan(y)) {
        return x;
    }
    if (ispositiveinf(x)) {
        return x;
    } else if (ispositiveinf(y)) {
        return y;
    }
    if (isnegativeinf(x)) {
        return y;
    } else if (isnegativeinf(y)) {
        return x;
    }
    if (x > y) {
        return x;
    } else {
        return y;
    }
}

__simt_callee__ inline half __hmin(half x, half y)
{
    if (__hisnan(x)) {
        return y;
    } else if (__hisnan(y)) {
        return x;
    }
    if (isnegativeinf(x)) {
        return x;
    } else if (isnegativeinf(y)) {
        return y;
    }
    if (ispositiveinf(x)) {
        return y;
    } else if (ispositiveinf(y)) {
        return x;
    }
    if (x < y) {
        return x;
    } else {
        return y;
    }
}

#endif  // ASCENDC_MODULE_SIMT_FP16_C_IMPL_H