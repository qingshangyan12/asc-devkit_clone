/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_ADV_API_SOFTMAX_SHAPE_METADATA_H
#define IMPL_ADV_API_SOFTMAX_SHAPE_METADATA_H

#include "softmax_common_shape_process.h"

namespace AscendC {
namespace Internal {
__aicore__ inline void ResolveSoftmaxLastAxisMetadata(
    const ShapeInfo& srcShape, const SoftMaxShapeInfo& softmaxShapeInfo, LastAxisShapeND& srcNDinfo,
    LastAxisShapeND& originalSrcShape)
{
    if (softmaxShapeInfo.srcM == 0 || softmaxShapeInfo.srcK == 0) {
        srcNDinfo = GetLastAxisShapeND(srcShape);
        originalSrcShape = GetLastAxisOriginShapeND(srcShape);
    } else {
        srcNDinfo = {softmaxShapeInfo.srcM, softmaxShapeInfo.srcK};
        originalSrcShape = {softmaxShapeInfo.oriSrcM, softmaxShapeInfo.oriSrcK};
    }
}
} // namespace Internal
} // namespace AscendC

#endif // IMPL_ADV_API_SOFTMAX_SHAPE_METADATA_H
