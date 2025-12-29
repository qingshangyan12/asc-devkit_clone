/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under
 * the terms and conditions of CANN Open Software License Agreement Version 2.0
 * (the "License"). Please refer to the License for details. You may not use
 * this file except in compliance with the License. THIS SOFTWARE IS PROVIDED ON
 * AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS
 * FOR A PARTICULAR PURPOSE. See LICENSE in the root of the software repository
 * for the full text of the License.
 */

/* !
 * \file adds_custom.cpp
 * \brief
 */

#include "kernel_operator.h"
#include "adds_custom_v1.h"
#include "adds_custom_v2.h"
#include "adds_custom_v3.h"

extern "C" __global__ __aicore__ void adds_custom(GM_ADDR x, GM_ADDR z, GM_ADDR workspace, GM_ADDR tiling)
{
    REGISTER_TILING_DEFAULT(AddsCustomTilingData);
    GET_TILING_DATA(tilingData, tiling);
    KERNEL_TASK_TYPE_DEFAULT(KERNEL_TYPE_MIX_AIV_1_0);
    if (TILING_KEY_IS(1UL)) {
        KernelAddsV1 op;
        op.Init(x, z, &tilingData);
        op.Process();
    } else if (TILING_KEY_IS(2UL)) {
        KernelAddsV2 op;
        op.Init(x, z, &tilingData);
        op.Process();
    } else if (TILING_KEY_IS(3UL)) {
        KernelAddsV3 op;
        op.Init(x, z, &tilingData);
        op.Process();
    }
}
