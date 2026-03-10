/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef REDUCE_MAX_IMPL_H
#define REDUCE_MAX_IMPL_H

#include "kernel_tensor.h"
#include "kernel_basic_intf.h"
#include "reduce_max_v220_impl.h"
#include "reduce_max_vmax_impl.h"

namespace AscendC {
namespace Internal {

// ============================================================================
// ReduceMaxImplV2 - 统一的 ReduceMax 实现入口 (v2版本)
// 根据 T 的类型自动选择实现：
//   - int32_t:  使用 vmax 实现（二分归约）
//   - half/float: 使用 vcgmax 实现（块内归约）
// ============================================================================
// 功能: 提供 ReduceMax 的核心实现，支持三种数据类型（half, float, int32_t）
// 支持模式: AR (All-to-Reduce) / RA (Reduce-to-All)
template <class T, class pattern, bool isReuseSource = false>
__aicore__ inline void ReduceMaxImplV2(const LocalTensor<T>& dstTensor, 
    const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t srcShape[], bool srcInnerPad)
{
    // 类型分发：根据数据类型选择实现
    if constexpr (std::is_same_v<T, int32_t>) {
        // int32_t: 使用 vmax 实现
        uint32_t last = srcShape[1];
        uint32_t first = srcShape[0];
        
        // 计算临时缓冲区需求
        constexpr uint32_t elePerBlk = ONE_BLK_SIZE / sizeof(T);
        uint32_t padLast = AlignUp(last, elePerBlk);
        
        // 参数检查
        static_assert(SupportType<pattern, Pattern::Reduce::AR, Pattern::Reduce::RA>(), 
            "failed to check the reduce pattern, it only supports AR/RA pattern!");
        
        // 调用 int32_t 专用实现
        bool isReduceLast = IsSameType<pattern, Pattern::Reduce::AR>::value;
        ReduceMaxVMaxImpl<T>(dstTensor, srcTensor, sharedTmpBuffer, 
                             first, last, isReduceLast);
        
    } else {
        // half/float: 使用原有 vcgmax 实现
        ReduceMaxV220Impl<T, pattern, isReuseSource>(
            dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
    }
}

} // namespace Internal
} // namespace AscendC
#endif // REDUCE_MAX_IMPL_H
