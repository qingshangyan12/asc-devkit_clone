/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef REDUCE_MAX_V220_IMPL_H
#define REDUCE_MAX_V220_IMPL_H

#include "kernel_tensor.h"
#include "kernel_basic_intf.h"
#include "kernel_tiling/kernel_tiling.h"
#include "reduce_max_common_util.h"
#include "../common/check.h"
#ifdef ASCENDC_CPU_DEBUG
#include "../api_check/kernel_check/reduce/reduce_max/reduce_max_check.h"
#endif // ASCENDC_CPU_DEBUG
#include "../api_check/kernel_api_check.h"

namespace AscendC {
namespace Internal {
// ============================================================================
// ReduceMaxV220Impl - float/half 专用的 ReduceMax 实现 (v2版本)
// 使用 vcgmax 指令进行块内归约
// ============================================================================
// 从原路径迁移: asc-devkit/impl/adv_api/detail/reduce/reduce_max/reduce_max_v220_impl.h
// 功能: float/half 数据类型的 ReduceMax 实现（使用 vcgmax 指令）
// 支持模式: AR (All-to-Reduce) / RA (Reduce-to-All)
template <class T, class pattern, bool isReuseSource = false>
__aicore__ inline void ReduceMaxV220Impl(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
                                      const LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t srcShape[],
                                      bool srcInnerPad)
{
    uint32_t last = srcShape[1];
    uint32_t first = srcShape[0];
    constexpr uint32_t elePerBlk = ONE_BLK_SIZE / sizeof(T);
    uint32_t padLast = AlignUp(last, elePerBlk);
    static_assert(SupportType<T, half, float>(), "failed to check the data type, current api supports data type is half/float!");
    static_assert(SupportType<pattern, Pattern::Reduce::AR, Pattern::Reduce::RA>(), 
        "failed to check the reduce pattern, it only supports AR/RA pattern!");
    CHECK_FUNC_HIGHLEVEL_API(ReduceMax, (T, pattern), (dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad, padLast));
    LocalTensor<T> tmpTensor = sharedTmpBuffer.ReinterpretCast<T>();

    if constexpr (IsSameType<pattern, Pattern::Reduce::AR>::value) {
        BlockReduceByLastAxisV2<T, isReuseSource, ApiModeV2::API_MODE_MAX, Max<T, false>>(
            dstTensor, srcTensor, tmpTensor, first, last, padLast);
    } else {
        BinaryReduceByFirstAxisV2<T, isReuseSource, Max<T, false>>(
            dstTensor, srcTensor, tmpTensor, first, last, padLast);
    }
    SetMaskNorm();
    ResetMask();
}
} // namespace Internal
} // namespace AscendC
#endif // REDUCE_MAX_V220_IMPL_H
