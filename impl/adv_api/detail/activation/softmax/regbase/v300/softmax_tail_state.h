/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_ADV_API_SOFTMAX_V300_TAIL_STATE_H
#define IMPL_ADV_API_SOFTMAX_V300_TAIL_STATE_H

#include "softmax_common_impl.h"

namespace AscendC {
namespace Internal {
template <bool offset1First = false>
__aicore__ inline bool AdvanceSoftmaxV300Tail(
    const SoftMaxTiling& tiling, uint32_t i, uint32_t& offset1, uint32_t& offset2, uint32_t& splitSize,
    uint32_t& reduceSize, ReduceLastND& reduceParam)
{
    offset1 += tiling.splitSize;
    offset2 += tiling.reduceSize;
    if (i == (tiling.rangeM - 1)) {
        if (tiling.tailM == 0) {
            return false;
        }
        if constexpr (offset1First) {
            offset1 = tiling.rangeM * tiling.splitSize;
            offset2 = tiling.rangeM * tiling.reduceSize;
        } else {
            offset2 = tiling.rangeM * tiling.reduceSize;
            offset1 = tiling.rangeM * tiling.splitSize;
        }
        splitSize = tiling.tailSplitSize;
        reduceSize = tiling.tailReduceSize;
        reduceParam.originalSrcM = tiling.tailM;
        reduceParam.srcM = tiling.tailM;
        reduceParam.dstM = tiling.tailM;
    }
    return true;
}
} // namespace Internal
} // namespace AscendC

#endif // IMPL_ADV_API_SOFTMAX_V300_TAIL_STATE_H
