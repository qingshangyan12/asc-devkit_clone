/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef ASCENDC_MODULE_SIMT_FP16_C_INTERFACE_H
#define ASCENDC_MODULE_SIMT_FP16_C_INTERFACE_H

__simt_callee__ inline bool __hisnan(half x);

__simt_callee__ inline bool __hisinf(half x);

__simt_callee__ inline half __hfma(half x, half y, half z);

__simt_callee__ inline half __habs(half x);

__simt_callee__ inline half __hmax(half x, half y);

__simt_callee__ inline half __hmin(half x, half y);

__simt_callee__ inline half hcos(half x);
__simt_callee__ inline half hsin(half x);
__simt_callee__ inline half htanh(half x);

__simt_callee__ inline half hexp(half x);
__simt_callee__ inline half hexp2(half x);
__simt_callee__ inline half hexp10(half x);

__simt_callee__ inline half hlog(half x);
__simt_callee__ inline half hlog2(half x);
__simt_callee__ inline half hlog10(half x);

__simt_callee__ inline half hsqrt(half x);
__simt_callee__ inline half hrsqrt(half x);

__simt_callee__ inline half hrcp(half x);

#include "impl/simt_api/simt_fp16_impl.h"
#endif  // ASCENDC_MODULE_SIMT_FP16_C_INTERFACE_H