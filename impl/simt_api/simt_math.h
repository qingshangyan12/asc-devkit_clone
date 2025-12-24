/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef ASCENDC_MODULE_SIMT_MATH_C_INTERFACE_H
#define ASCENDC_MODULE_SIMT_MATH_C_INTERFACE_H

__simt_callee__ inline float expf(float x);

__simt_callee__ inline long int lroundf(float x);

__simt_callee__ inline long long int llroundf(float x);

__simt_callee__ inline long int lrintf(float x);

__simt_callee__ inline long long int llrintf(float x);

__simt_callee__ inline float truncf(float x);

__simt_callee__ inline float roundf(float x);

__simt_callee__ inline float rintf(float x);

__simt_callee__ inline float floorf(float x);

__simt_callee__ inline float ceilf(float x);

__simt_callee__ inline float sqrtf(float x);

__simt_callee__ inline float rsqrtf(float x);

__simt_callee__ inline float log2f(float x);

__simt_callee__ inline float logf(float x);

__simt_callee__ inline float expf(float x);

__simt_callee__ inline float fmaf(float x, float y, float z);

__simt_callee__ inline float fabsf(float x);

__simt_callee__ inline float normcdfinvf(float x);

__simt_callee__ inline float modff(float x, float &n);

#include "impl/simt_api/simt_math_impl.h"
#endif  // ASCENDC_MODULE_SIMT_MATH_C_INTERFACE_H
