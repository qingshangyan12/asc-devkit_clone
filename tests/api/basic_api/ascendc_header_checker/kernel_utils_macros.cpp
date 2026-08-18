/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_utils_macros.h"
#endif

#if !defined(likely) || !defined(unlikely)
#error "likely and unlikely must be available from the public macro headers"
#endif

#if !defined(REGISTER_TILING_DEFAULT) || !defined(REGISTER_TILING_FOR_TILINGKEY) || !defined(REGISTER_NONE_TILING)
#error "tiling registration macros must be available from the Basic API public macro header"
#endif

#if !defined(KERNEL_TASK_TYPE) || !defined(KERNEL_TASK_TYPE_DEFAULT)
#error "kernel task type macros must be available from the Basic API public macro header"
#endif

#if !defined(TILING_KEY_IS) || !defined(TILING_KEY_LIST) || !defined(PRELOAD)
#error "tiling key and preload macros must be available from the Basic API public macro header"
#endif

struct HeaderCheckTilingData {
    uint32_t value;
};

REGISTER_TILING_DEFAULT(HeaderCheckTilingData);
REGISTER_TILING_FOR_TILINGKEY("TILING_KEY_VAR == 1", HeaderCheckTilingData);

extern "C" __global__ __aicore__ void KernelTestPublicMacros()
{
    KERNEL_TASK_TYPE_DEFAULT(KERNEL_TYPE_AIV_ONLY);
    KERNEL_TASK_TYPE(1, KERNEL_TYPE_AIC_ONLY);
    REGISTER_NONE_TILING;
    PRELOAD(1);

    if (TILING_KEY_IS(0) || TILING_KEY_LIST(1, 2)) {
        return;
    }

    if (likely(true) && unlikely(false)) {
        return;
    }
}
