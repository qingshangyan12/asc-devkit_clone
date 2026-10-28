/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_ADV_API_DEQUANT_TILING_SHAPE_H
#define IMPL_ADV_API_DEQUANT_TILING_SHAPE_H

#include "adv_api/utils/types.h"

namespace AscendC {
namespace Internal {
inline bool ResolveDequantTilingShape(const TensorShape& srcShape, uint32_t& outer, uint32_t& inner)
{
    std::vector<int64_t> shapeDims = srcShape.GetDims();
    if (shapeDims.size() == 2) {
        outer = static_cast<uint32_t>(shapeDims[0]);
        inner = static_cast<uint32_t>(shapeDims[1]);
    } else if (shapeDims.size() == 1) {
        inner = static_cast<uint32_t>(shapeDims[0]);
    } else {
        return false;
    }
    return true;
}
} // namespace Internal
} // namespace AscendC

#endif // IMPL_ADV_API_DEQUANT_TILING_SHAPE_H
