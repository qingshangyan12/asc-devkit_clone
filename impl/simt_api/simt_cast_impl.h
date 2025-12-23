/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef ASCENDC_MODULE_SIMT_CAST_C_IMPL_H
#define ASCENDC_MODULE_SIMT_CAST_C_IMPL_H

__simt_callee__ inline half hfloor(half x)
{
    return __floorf(x);
}

__simt_callee__ inline bfloat16_t hfloor(bfloat16_t x)
{
    return __floorf(x);
}

__simt_callee__ inline half hrint(half x)
{
    return __rintf(x);
}

__simt_callee__ inline bfloat16_t hrint(bfloat16_t x)
{
    return __rintf(x);
}

__simt_callee__ inline half hceil(half x)
{
    return __ceilf(x);
}

__simt_callee__ inline bfloat16_t hceil(bfloat16_t x)
{
    return __ceilf(x);
}

__simt_callee__ inline half htrunc(half x)
{
    if (x > (half)0) {
        return __floorf(x);
    } else {
        return __ceilf(x);
    }
}

__simt_callee__ inline bfloat16_t htrunc(bfloat16_t x)
{
    if (x > (bfloat16_t)0) {
        return __floorf(x);
    } else {
        return __ceilf(x);
    }
}

#endif  // ASCENDC_MODULE_SIMT_CAST_C_IMPL_H