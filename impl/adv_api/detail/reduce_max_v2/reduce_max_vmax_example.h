/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of 
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef __ASCENDC_API_REDUCE_MAX_VMAX_EXAMPLE_H__
#define __ASCENDC_API_REDUCE_MAX_VMAX_EXAMPLE_H__

#include "kernel_operator_vec_reduce_impl.h"

namespace AscendC {

// 示例 1: 使用 BlockReduceMaxImpl 对 int32_t 数据进行块级归约
// 每个块包含 8 个 int32_t 元素（32字节/4字节=8）
template <typename T = int32_t>
__aicore__ inline void BlockReduceMaxExample(__ubuf__ T* dst, __ubuf__ T* src,
    const int32_t numBlocks, const int32_t mask)
{
    // 参数说明:
    // - dst: 目标地址（存储每个块的最大值）
    // - src: 源地址（待归约的数据）
    // - numBlocks: 块数量（repeatTime）
    // - mask: mask 值（控制有效元素，0 表示使用全部元素）
    
    // 调用 BlockReduceMaxImpl
    // 对于 int32_t 类型，自动使用 vmax 指令实现
    BlockReduceMaxImpl<T>(dst, src, numBlocks, mask,
                         1,  // dstRepStride
                         1,  // srcBlkStride
                         1); // srcRepStride
    
    // 结果: dst[i] = max(src[i*8 .. i*8+mask-1]), for i in [0, numBlocks-1]
}

// 示例 2: 使用 BinaryReduceMax 进行通用二分归约
template <typename T = int32_t>
__aicore__ inline void BinaryReduceMaxExample(__ubuf__ T* dst, __ubuf__ T* src, int32_t len)
{
    BinaryReduceMax<T>(dst, src, len);
    
    // 结果: dst[0] = max(src[0..len-1])
}

// 示例 3: 完整的 kernel 实现示例
/*
extern "C" __global__ __aicore__ void reduce_max_int32_kernel(GM_ADDR dstGm, GM_ADDR srcGm,
                                                               uint32_t numElements) {
    TPipe pipe;
    
    // 计算需要的块数
    constexpr uint32_t elePerBlk = 32 / sizeof(int32_t);  // 每块 8 个元素
    uint32_t numBlocks = (numElements + elePerBlk - 1) / elePerBlk;
    
    // 初始化队列
    TQue<QuePosition::VECIN, 1> inQueue;
    TQue<QuePosition::VECOUT, 1> outQueue;
    pipe.InitBuffer(inQueue, 1, numBlocks * elePerBlk * sizeof(int32_t));
    pipe.InitBuffer(outQueue, 1, numBlocks * sizeof(int32_t));
    
    // 获取 local tensor
    LocalTensor<int32_t> srcLocal = inQueue.AllocTensor<int32_t>();
    LocalTensor<int32_t> dstLocal = outQueue.AllocTensor<int32_t>();
    
    // 从 GM 复制数据到 Local
    GlobalTensor<int32_t> srcGmGlobal = GlobalTensor<int32_t>(srcGm);
    GlobalTensor<int32_t> dstGmGlobal = GlobalTensor<int32_t>(dstGm);
    DataCopy(srcLocal, srcGmGlobal, numBlocks * elePerBlk);
    
    // 等待复制完成
    inQueue.EnQue(srcLocal);
    outQueue.DeQue<int32_t>();
    
    // 计算最后一个块的有效元素数
    uint32_t lastBlockValid = (numElements % elePerBlk) == 0 ? 
                               elePerBlk : (numElements % elePerBlk);
    
    // 使用 BlockReduceMaxImpl 进行归约
    // 如果数据正好是块大小的整数倍，mask 为 0
    // 否则，最后一个块的 mask 为有效元素数
    if ((numElements % elePerBlk) == 0) {
        BlockReduceMaxImpl<int32_t>(
            reinterpret_cast<__ubuf__ int32_t*>(dstLocal.GetData()),
            reinterpret_cast<__ubuf__ int32_t*>(srcLocal.GetData()),
            numBlocks,     // repeatTime
            0,             // mask (0 表示全部元素有效)
            1,             // dstRepStride
            1,             // srcBlkStride
            1);            // srcRepStride
    } else {
        // 处理最后一个块的不完整情况
        BlockReduceMaxImpl<int32_t>(
            reinterpret_cast<__ubuf__ int32_t*>(dstLocal.GetData()),
            reinterpret_cast<__ubuf__ int32_t*>(srcLocal.GetData()),
            numBlocks,     // repeatTime
            lastBlockValid, // mask
            1,             // dstRepStride
            1,             // srcBlkStride
            1);            // srcRepStride
    }
    
    // 将结果复制回 GM
    outQueue.EnQue(dstLocal);
    outQueue.DeQue<int32_t>();
    DataCopy(dstGmGlobal, dstLocal, numBlocks);
    
    // 释放资源
    outQueue.FreeTensor(dstLocal);
    inQueue.DeQue<int32_t>();
    inQueue.FreeTensor(srcLocal);
}
*/

}  // namespace AscendC

#endif  // __ASCENDC_API_REDUCE_MAX_VMAX_EXAMPLE_H__
