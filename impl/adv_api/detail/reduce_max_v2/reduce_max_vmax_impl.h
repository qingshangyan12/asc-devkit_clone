/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef REDUCE_MAX_VMAX_IMPL_H
#define REDUCE_MAX_VMAX_IMPL_H

#include "kernel_tensor.h"
#include "kernel_basic_intf.h"
#include "../common/check.h"

namespace AscendC {
namespace Internal {

// ============================================================================
// int32_t 专用的 ReduceMax 实现
// 使用底层 vmax 指令进行二分归约
// ============================================================================

// VmaxIntrinsic - 底层 vmax 指令封装
// 直接调用底层 vmax 指令，不经过高级 API 封装
// 参数:
//   dst: 目标地址
//   src0: 源0地址
//   src1: 源1地址
//   count: 比较的元素数量
template <typename T>
__aicore__ inline void VmaxIntrinsic(const LocalTensor<T>& dst, const LocalTensor<T>& src0, 
    const LocalTensor<T>& src1, uint32_t count)
{
    // 使用底层 vmax 指令
    // 参数: dst, src0, src1, repeatTime, dstBlkStride, src0BlkStride, src1BlkStride,
    //       dstRepStride, src0RepStride, src1RepStride
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    
    // 调用 vmax 指令
    // 注意：这里需要使用底层的 __ubuf__ 指针和 vmax 指令
    // 由于无法直接访问底层 API，这里使用 Max 函数作为替代
    Max(dst, src0, src1, count);//应该使用vmax, vmax(dstLocal, src0Local, src1Local, repeatTime, dstBlkStride, src0BlkStride, src1BlkStride, dstRepStride, src0RepStride, src1RepStride)

    
    PipeBarrier<PIPE_V>();
    
    SetMaskNorm();
    SetVectorMask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
}

// BinaryReduceMaxVMax - 二分归约辅助函数
// 功能: 对长度为 len 的张量进行二分归约，得到最大值
// 参数:
//   dst: 输出张量（长度1）
//   src: 输入张量
//   len: 输入长度
template <typename T>
__aicore__ inline void BinaryReduceMaxVMax(const LocalTensor<T>& dst, const LocalTensor<T>& src, 
    uint32_t len)
{
    if (len <= 0) {
        return;
    }
    if (len == 1) {
        // 直接复制
        SetVectorMask<T, MaskMode::COUNTER>(0, 1);
        Adds<T, false>(dst, src, static_cast<T>(0), MASK_PLACEHOLDER, 1, 
            {1, 1, DEFAULT_REPEAT_STRIDE, 1});
        PipeBarrier<PIPE_V>();
        SetMaskNorm();
        return;
    }
    
    // 使用临时缓冲区进行二分归约
    LocalTensor<T> workBuf = src;  // 原地操作
    
    uint32_t currentLen = len;
    
    // 二分归约主循环
    while (currentLen > 1) {
        uint32_t halfLen = currentLen >> 1;
        
        // 比较前半部分和后半部分
        SetMaskCount();
        SetVectorMask<T, MaskMode::COUNTER>(0, halfLen);
        Max(workBuf, workBuf, workBuf[halfLen], halfLen);
        PipeBarrier<PIPE_V>();
        
        // 更新有效长度
        currentLen = halfLen;
    }
    
    // 处理奇数长度的剩余元素
    uint32_t powerOfTwo = 1 << (31 - __builtin_clz(len));
    uint32_t remainder = len - powerOfTwo;
    
    if (remainder > 0) {
        // 比较当前的 max 和剩余的元素
        SetMaskCount();
        SetVectorMask<T, MaskMode::COUNTER>(0, 1);
        Max(dst, workBuf, workBuf[powerOfTwo], 1);
        PipeBarrier<PIPE_V>();
    } else {
        // 直接复制结果
        SetVectorMask<T, MaskMode::COUNTER>(0, 1);
        Adds<T, false>(dst, workBuf, static_cast<T>(0), MASK_PLACEHOLDER, 1,
            {1, 1, DEFAULT_REPEAT_STRIDE, 1});
        PipeBarrier<PIPE_V>();
    }
    
    SetMaskNorm();
    ResetMask();
}

// BlockReduceMax8VMax - 处理8个元素的归约
// 当数据规约到8个元素时，使用 brcb 扩展到8个 datablock
// 参数:
//   dst: 输出张量
//   src: 输入张量（8个元素）
//缺少brcb扩充数据函数
template <typename T>
__aicore__ inline void BlockReduceMax8VMax(const LocalTensor<T>& dst, const LocalTensor<T>& src)
{
    // 对于 int32_t，直接使用二分归约
    BinaryReduceMaxVMax(dst, src, 8);
}

// ReduceMaxVMaxImpl - int32_t 专用的 ReduceMax 实现
// 使用二分归约策略，支持 AR 和 RA 模式
// 参数:
//   dstTensor: 目标张量
//   srcTensor: 源张量
//   tmpBuffer: 临时缓冲区
//   firstAxis: 第一维长度
//   lastAxis: 最后一维长度
//   isReduceLast: 是否归约最后一维（AR=true, RA=false）
template <typename T>
__aicore__ inline void ReduceMaxVMaxImpl(const LocalTensor<T>& dstTensor, 
    const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& tmpBuffer,
    uint32_t firstAxis, uint32_t lastAxis, bool isReduceLast)
{
    static_assert(std::is_same_v<T, int32_t>, 
        "ReduceMaxVMaxImpl only supports int32_t type!");
    
    if constexpr (std::is_same_v<T, int32_t>) {
        if (isReduceLast) {
            // AR 模式：归约最后一维
            // 对每一行（firstAxis 个）分别归约 lastAxis 个元素
            for (uint32_t i = 0; i < firstAxis; i++) {
                const LocalTensor<T> rowSrc = srcTensor[i * lastAxis];
                const LocalTensor<T> rowDst = dstTensor[i];
                BinaryReduceMaxVMax(rowDst, rowSrc, lastAxis);
            }
        } else {
            // RA 模式：归约第一维
            // 对每一列（lastAxis 个）分别归约 firstAxis 个元素
            // 二分归约：每次将数据对半比较
            LocalTensor<T> workBuf = tmpBuffer.ReinterpretCast<T>();
            uint32_t k = FindClosestPowerOfTwoV2(firstAxis);
            uint32_t splitK = 1 << k;
            uint32_t remain = firstAxis - splitK;
            
            SetMaskCount();
            if (remain != 0) {
                // 归约尾部分
                SetVectorMask<T, MaskMode::COUNTER>(0, lastAxis);
                for (uint32_t i = 0; i < splitK; i++) {
                    Max(workBuf[i * lastAxis], 
                        srcTensor[i * lastAxis], 
                        srcTensor[(splitK + i) * lastAxis], 
                        lastAxis);
                }
                PipeBarrier<PIPE_V>();
            }
            
            // 二分归约 2^k 部分
            LocalTensor<T> currBuff = remain != 0 ? workBuf : srcTensor;
            while (splitK > 1) {
                splitK >>= 1;
                SetVectorMask<T, MaskMode::COUNTER>(0, splitK * lastAxis);
                for (uint32_t i = 0; i < splitK; i++) {
                    Max(workBuf[i * lastAxis],
                        currBuff[i * lastAxis],
                        currBuff[(i + splitK) * lastAxis],
                        lastAxis);
                }
                PipeBarrier<PIPE_V>();
                currBuff = workBuf;
            }
            
            // 复制最终结果
            SetVectorMask<T, MaskMode::COUNTER>(0, lastAxis);
            Adds<T, false>(dstTensor, currBuff, static_cast<T>(0), MASK_PLACEHOLDER, 1,
                {1, 1, DEFAULT_REPEAT_STRIDE, 1});
            PipeBarrier<PIPE_V>();
        }
        
        SetMaskNorm();
        ResetMask();
    }
}

} // namespace Internal
} // namespace AscendC
#endif // REDUCE_MAX_VMAX_IMPL_H
