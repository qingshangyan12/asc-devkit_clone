/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file ndtri_tiling_impl.cpp
 * \brief
 */
#include "../../../../include/adv_api/math/ndtri_tiling.h"

#include <cstdint>

#include "graph/tensor.h"
#include "../../../../../impl/adv_api/detail/host_log.h"

namespace AscendC {
// Ndtri needs NO temporary UB on Ascend 950PR (__NPU_ARCH__ == 3510): the kernel is register
// resident. Every intermediate of both the central and the tail branch lives in a vector register,
// so UB is touched exactly twice per 256B repeat — one LoadAlign, one StoreAlign.
// Per the high level API tiling contract, 0 means "no temporary space is used/required".
void GetNdtriTmpBufferFactorSize(const uint32_t typeSize, uint32_t& maxLiveNodeCount, uint32_t& extraBuffer)
{
    (void)typeSize;
    maxLiveNodeCount = 0;
    extraBuffer = 0;
}

void GetNdtriMaxMinTmpSize(
    const ge::Shape& srcShape, const uint32_t typeSize, const bool isReuseSource, uint32_t& maxValue,
    uint32_t& minValue)
{
    (void)typeSize;
    (void)isReuseSource;
    const uint32_t inputSize = srcShape.GetShapeSize();
    ASCENDC_HOST_ASSERT(inputSize > 0, return, "Input Shape size must be greater than 0.");

    minValue = 0;
    maxValue = 0;
}
} // namespace AscendC
