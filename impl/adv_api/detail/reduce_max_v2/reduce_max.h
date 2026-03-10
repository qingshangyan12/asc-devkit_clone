/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef REDUCE_MAX_H
#define REDUCE_MAX_H

#include "reduce_max_impl.h"

namespace AscendC {

// ============================================================================
// ReduceMax - 统一对外接口
// 支持三种数据类型：half, float, int32_t
// 支持两种归约模式：AR (All-to-Reduce) / RA (Reduce-to-All)
// ============================================================================
// 功能: 在张量的指定维度上执行最大值归约运算
// 参数:
//   dstTensor: 目标张量，存储归约后的最大值
//   srcTensor: 源张量，包含待归约的数据
//   tmpBuffer: 临时缓冲区，用于中间计算
//   srcShape: 源张量的形状信息 [first, last]
//   isReuseSource: 是否原地操作（仅支持 RA 模式的 half/float）
//
// 支持的数据类型:
//   - half:   使用 vcgmax 指令（块内归约）
//   - float:   使用 vcgmax 指令（块内归约）
//   - int32_t: 使用 vmax 指令（二分归约）
//
// 支持的归约模式:
//   - Pattern::Reduce::AR (All-to-Reduce): 在最后一维上归约
//     输入形状: [first, last] → 输出形状: [first]
//   - Pattern::Reduce::RA (Reduce-to-All): 在第一维上归约
//     输入形状: [first, last] → 输出形状: [last]
//
// 示例代码:
//   // AR 模式示例
//   LocalTensor<float> srcTensor = AllocTensor<float>(32 * 128);  // [32, 128]
//   LocalTensor<float> dstTensor = AllocTensor<float>(32);        // [32]
//   LocalTensor<uint8_t> tmpBuffer = AllocTensor<uint8_t>(1024);
//   uint32_t srcShape[2] = {32, 128};
//   ReduceMax<float, Pattern::Reduce::AR>(
//       dstTensor, srcTensor, tmpBuffer, srcShape, false);
//   // 结果: dstTensor[i] = max(srcTensor[i*128 : i*128+127])
//
//   // RA 模式示例
//   LocalTensor<int32_t> srcTensor = AllocTensor<int32_t>(16 * 64);  // [16, 64]
//   LocalTensor<int32_t> dstTensor = AllocTensor<int32_t>(64);        // [64]
//   LocalTensor<uint8_t> tmpBuffer = AllocTensor<uint8_t>(512);
//   uint32_t srcShape[2] = {16, 64};
//   ReduceMax<int32_t, Pattern::Reduce::RA>(
//       dstTensor, srcTensor, tmpBuffer, srcShape, false);
//   // 结果: dstTensor[j] = max(srcTensor[j], srcTensor[j+64], ..., srcTensor[j+15*64])
template <typename T, typename pattern>
__aicore__ inline void ReduceMax(const LocalTensor<T>& dstTensor,
    const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& tmpBuffer,
    const uint32_t srcShape[], bool isReuseSource = false)
{
    Internal::ReduceMaxImplV2<T, pattern, false>(dstTensor, srcTensor, 
                                               tmpBuffer, srcShape, isReuseSource);
}

} // namespace AscendC
#endif // REDUCE_MAX_H
