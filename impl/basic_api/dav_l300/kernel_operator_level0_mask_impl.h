/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_DAV_L300_LEVEL0_MASK_IMPL_H
#define ASCENDC_DAV_L300_LEVEL0_MASK_IMPL_H

#include "../kernel_utils.h"

namespace AscendC {
namespace Internal {
template <bool isMaskBitMode>
__aicore__ inline void PrepareLevel0MaskL300(
    const uint64_t maskArray[], const uint64_t maskCount, BasicAPIMaskStruct& maskArrayStruct)
{
    uint16_t maskArraySize = (maskArray == nullptr) ? 0 : MASK_ARRAY_SIZE;
    for (uint16_t i = 0; i < maskArraySize; i++) {
        maskArrayStruct.maskArray[i] = maskArray[i];
    }
}
} // namespace Internal
} // namespace AscendC

#endif // ASCENDC_DAV_L300_LEVEL0_MASK_IMPL_H
