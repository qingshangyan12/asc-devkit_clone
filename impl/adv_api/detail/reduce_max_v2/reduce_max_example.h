/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef REDUCE_MAX_EXAMPLE_H
#define REDUCE_MAX_EXAMPLE_H

#include "reduce_max.h"

namespace AscendC {

// ============================================================================
// ReduceMax 使用示例
// ============================================================================

// 示例1: AR 模式 - half 类型（使用 vcgmax 指令）
// 场景: 批量数据的全局最大值池化
template <typename T>
__aicore__ inline void ReduceMaxARExampleHalf()
{
    // 输入: [32, 128] 张量
    constexpr uint32_t first = 32;
    constexpr uint32_t last = 128;
    
    LocalTensor<T> srcTensor = AllocTensor<T>(first * last);
    LocalTensor<T> dstTensor = AllocTensor<T>(first);
    LocalTensor<uint8_t> tmpBuffer = AllocTensor<uint8_t>(1024);
    
    // 填充数据（示例）
    for (uint32_t i = 0; i < first * last; i++) {
        srcTensor[i] = static_cast<T>(i % 100);
    }
    
    // 执行 AR 模式归约
    uint32_t srcShape[2] = {first, last};
    ReduceMax<T, Pattern::Reduce::AR>(
        dstTensor, srcTensor, tmpBuffer, srcShape, false);
    
    // 结果: dstTensor[i] = max(srcTensor[i*128 : i*128+127])
    //       = max(i*128 % 100, (i*128+1) % 100, ..., (i*128+127) % 100)
}

// 示例2: AR 模式 - float 类型（使用 vcgmax 指令）
// 场景: 图像分类的 batch-level 归约
template <typename T>
__aicore__ inline void ReduceMaxARExampleFloat()
{
    // 输入: [64, 256] 张量
    constexpr uint32_t first = 64;
    constexpr uint32_t last = 256;
    
    LocalTensor<T> srcTensor = AllocTensor<T>(first * last);
    LocalTensor<T> dstTensor = AllocTensor<T>(first);
    LocalTensor<uint8_t> tmpBuffer = AllocTensor<uint8_t>(2048);
    
    // 执行 AR 模式归约
    uint32_t srcShape[2] = {first, last};
    ReduceMax<T, Pattern::Reduce::AR>(
        dstTensor, srcTensor, tmpBuffer, srcShape, false);
    
    // 结果: dstTensor[i] = max(srcTensor[i*256 : i*256+255])
}

// 示例3: AR 模式 - int32_t 类型（使用 vmax 指令）
// 场景: 整数数据的行级最大值
template <typename T>
__aicore__ inline void ReduceMaxARExampleInt32()
{
    // 输入: [16, 64] 张量
    constexpr uint32_t first = 16;
    constexpr uint32_t last = 64;
    
    LocalTensor<T> srcTensor = AllocTensor<T>(first * last);
    LocalTensor<T> dstTensor = AllocTensor<T>(first);
    LocalTensor<uint8_t> tmpBuffer = AllocTensor<uint8_t>(1024);
    
    // 填充数据（示例）
    for (uint32_t i = 0; i < first * last; i++) {
        srcTensor[i] = i % 50;
    }
    
    // 执行 AR 模式归约
    uint32_t srcShape[2] = {first, last};
    ReduceMax<T, Pattern::Reduce::AR>(
        dstTensor, srcTensor, tmpBuffer, srcShape, false);
    
    // 结果: dstTensor[i] = max(srcTensor[i*64 : i*64+63])
    //       = max(i*64 % 50, (i*64+1) % 50, ..., (i*64+63) % 50)
}

// 示例4: RA 模式 - half 类型（使用 vcgmax 指令）
// 场景: 多层特征的全局最大值
template <typename T>
__aicore__ inline void ReduceMaxRAExampleHalf()
{
    // 输入: [8, 128] 张量
    constexpr uint32_t first = 8;
    constexpr uint32_t last = 128;
    
    LocalTensor<T> srcTensor = AllocTensor<T>(first * last);
    LocalTensor<T> dstTensor = AllocTensor<T>(last);
    LocalTensor<uint8_t> tmpBuffer = AllocTensor<uint8_t>(1024);
    
    // 执行 RA 模式归约
    uint32_t srcShape[2] = {first, last};
    ReduceMax<T, Pattern::Reduce::RA>(
        dstTensor, srcTensor, tmpBuffer, srcShape, false);
    
    // 结果: dstTensor[j] = max(srcTensor[j], srcTensor[j+128], ..., srcTensor[j+7*128])
}

// 示例5: RA 模式 - float 类型（使用 vcgmax 指令）
// 场景: 深度网络的特征聚合
template <typename T>
__aicore__ inline void ReduceMaxRAExampleFloat()
{
    // 输入: [16, 256] 张量
    constexpr uint32_t first = 16;
    constexpr uint32_t last = 256;
    
    LocalTensor<T> srcTensor = AllocTensor<T>(first * last);
    LocalTensor<T> dstTensor = AllocTensor<T>(last);
    LocalTensor<uint8_t> tmpBuffer = AllocTensor<uint8_t>(2048);
    
    // 执行 RA 模式归约
    uint32_t srcShape[2] = {first, last};
    ReduceMax<T, Pattern::Reduce::RA>(
        dstTensor, srcTensor, tmpBuffer, srcShape, false);
    
    // 结果: dstTensor[j] = max(srcTensor[j], srcTensor[j+256], ..., srcTensor[j+15*256])
}

// 示例6: RA 模式 - int32_t 类型（使用 vmax 指令）
// 场景: 时间序列的多帧归约
template <typename T>
__aicore__ inline void ReduceMaxRAExampleInt32()
{
    // 输入: [32, 64] 张量
    constexpr uint32_t first = 32;
    constexpr uint32_t last = 64;
    
    LocalTensor<T> srcTensor = AllocTensor<T>(first * last);
    LocalTensor<T> dstTensor = AllocTensor<T>(last);
    LocalTensor<uint8_t> tmpBuffer = AllocTensor<uint8_t>(1024);
    
    // 填充数据（示例）
    for (uint32_t i = 0; i < first * last; i++) {
        srcTensor[i] = (i % 100) - 50;  // -50 到 49
    }
    
    // 执行 RA 模式归约
    uint32_t srcShape[2] = {first, last};
    ReduceMax<T, Pattern::Reduce::RA>(
        dstTensor, srcTensor, tmpBuffer, srcShape, false);
    
    // 结果: dstTensor[j] = max(srcTensor[j], srcTensor[j+64], ..., srcTensor[j+31*64])
}

// 示例7: 完整的 kernel 示例 - 混合使用三种数据类型
class ReduceMaxKernelExample {
public:
    __aicore__ ReduceMaxKernelExample() {}
    
    ~ReduceMaxKernelExample() {}
    
    // 处理 half 类型（AR 模式）
    __aicore__ inline void ProcessHalfAR()
    {
        LocalTensor<half> src = GetLocalTensor<half>();
        LocalTensor<half> dst = GetLocalTensor<half>();
        LocalTensor<uint8_t> tmp = GetLocalTensor<uint8_t>();
        uint32_t srcShape[2] = {GetTilingKey().get_uint32_t("first"),
                                GetTilingKey().get_uint32_t("last")};
        
        ReduceMax<half, Pattern::Reduce::AR>(
            dst, src, tmp, srcShape, false);
    }
    
    // 处理 float 类型（AR 模式）
    __aicore__ inline void ProcessFloatAR()
    {
        LocalTensor<float> src = GetLocalTensor<float>();
        LocalTensor<float> dst = GetLocalTensor<float>();
        LocalTensor<uint8_t> tmp = GetLocalTensor<uint8_t>();
        uint32_t srcShape[2] = {GetTilingKey().get_uint32_t("first"),
                                GetTilingKey().get_uint32_t("last")};
        
        ReduceMax<float, Pattern::Reduce::AR>(
            dst, src, tmp, srcShape, false);
    }
    
    // 处理 int32_t 类型（RA 模式）
    __aicore__ inline void ProcessInt32RA()
    {
        LocalTensor<int32_t> src = GetLocalTensor<int32_t>();
        LocalTensor<int32_t> dst = GetLocalTensor<int32_t>();
        LocalTensor<uint8_t> tmp = GetLocalTensor<uint8_t>();
        uint32_t srcShape[2] = {GetTilingKey().get_uint32_t("first"),
                                GetTilingKey().get_uint32_t("last")};
        
        ReduceMax<int32_t, Pattern::Reduce::RA>(
            dst, src, tmp, srcShape, false);
    }
    
    __aicore__ inline void Process()
    {
        // 根据数据类型选择处理函数
        uint32_t dtype = GetTilingKey().get_uint32_t("dtype");
        
        if (dtype == 0) {
            ProcessHalfAR();
        } else if (dtype == 1) {
            ProcessFloatAR();
        } else if (dtype == 2) {
            ProcessInt32RA();
        }
    }
};

} // namespace AscendC
#endif // REDUCE_MAX_EXAMPLE_H
