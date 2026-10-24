/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_SOFTMAX_BASIC_BLOCK_TILING_H
#define ASCENDC_SOFTMAX_BASIC_BLOCK_TILING_H

#include <cstdint>

namespace AscendC {
namespace Internal {
inline void AdjustSoftmaxBasicBlockBaseM(
    uint32_t& baseM, uint32_t srcM, uint32_t srcK, uint32_t basicTileNum, uint32_t basicBlockUnit,
    uint32_t basicBlockMinSize, uint32_t halfSize)
{
    if (baseM > basicTileNum && srcM % basicTileNum == 0 && srcK % basicBlockUnit == 0) {
        baseM = baseM / basicTileNum * basicTileNum;
        while (srcM % baseM != 0) {
            baseM -= basicTileNum;
        }
        while (baseM * srcK >= basicBlockUnit * basicBlockMinSize) {
            baseM = baseM / halfSize;
        }
    }
}
} // namespace Internal
} // namespace AscendC

#endif // ASCENDC_SOFTMAX_BASIC_BLOCK_TILING_H
