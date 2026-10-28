/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_ADV_API_TILING_ACTIVATION_SOFTMAX_TILING_FIELDS_H
#define IMPL_ADV_API_TILING_ACTIVATION_SOFTMAX_TILING_FIELDS_H

#include <cstdint>

namespace AscendC {
namespace Internal {
template <typename Tiling>
inline void SetSoftmaxBasicTilingFields(
    Tiling& softmaxTiling, uint32_t srcM, uint32_t srcK, uint32_t elementNumPerBlk, uint32_t baseM)
{
    softmaxTiling.set_srcM(srcM);
    softmaxTiling.set_srcK(srcK);
    softmaxTiling.set_srcSize(srcM * srcK);
    softmaxTiling.set_outMaxM(srcM);
    softmaxTiling.set_outMaxK(elementNumPerBlk);
    softmaxTiling.set_outMaxSize(srcM * elementNumPerBlk);
    softmaxTiling.set_splitM(baseM);
    softmaxTiling.set_splitK(srcK);
    softmaxTiling.set_splitSize(baseM * srcK);
    softmaxTiling.set_reduceM(baseM);
    softmaxTiling.set_reduceK(elementNumPerBlk);
    softmaxTiling.set_reduceSize(baseM * elementNumPerBlk);
}
} // namespace Internal
} // namespace AscendC

#endif // IMPL_ADV_API_TILING_ACTIVATION_SOFTMAX_TILING_FIELDS_H
