/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef ASCENDC_MODULE_SIMT_BF16_C_INTERFACE_H
#define ASCENDC_MODULE_SIMT_BF16_C_INTERFACE_H

__simt_callee__ inline bool __hisnan(bfloat16_t x);

__simt_callee__ inline bool __hisinf(bfloat16_t x);

__simt_callee__ inline bfloat16_t __habs(bfloat16_t x);

__simt_callee__ inline bfloat16_t __hfma(bfloat16_t x, bfloat16_t y, bfloat16_t z);

__simt_callee__ inline bfloat16_t __hmax(bfloat16_t x, bfloat16_t y);

__simt_callee__ inline bfloat16_t __hmin(bfloat16_t x, bfloat16_t y);

__simt_callee__ inline bfloat16_t hcos(bfloat16_t x);
__simt_callee__ inline bfloat16_t hsin(bfloat16_t x);
__simt_callee__ inline bfloat16_t htanh(bfloat16_t x);

__simt_callee__ inline bfloat16_t hexp(bfloat16_t x);
__simt_callee__ inline bfloat16_t hexp2(bfloat16_t x);
__simt_callee__ inline bfloat16_t hexp10(bfloat16_t x);

__simt_callee__ inline bfloat16_t hlog(bfloat16_t x);
__simt_callee__ inline bfloat16_t hlog2(bfloat16_t x);
__simt_callee__ inline bfloat16_t hlog10(bfloat16_t x);

__simt_callee__ inline bfloat16_t hsqrt(bfloat16_t x);
__simt_callee__ inline bfloat16_t hrsqrt(bfloat16_t x);

__simt_callee__ inline bfloat16_t hrcp(bfloat16_t x);

#include "impl/simt_api/simt_bf16_impl.h"
#endif  // ASCENDC_MODULE_SIMT_BF16_C_INTERFACE_H