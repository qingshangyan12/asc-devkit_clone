/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef REDUCE_MAX_COMMON_UTIL_H
#define REDUCE_MAX_COMMON_UTIL_H

#include "kernel_basic_intf.h"
#include "kernel_tensor.h"
#include "../common/check.h"

namespace AscendC {
namespace Internal {

// ============================================================================
// 从 reduce_common_util_impl.h 迁移的通用工具函数
// ============================================================================

// ApiModeV2 枚举 - 归约操作模式 (v2版本)
enum class ApiModeV2 : uint8_t {
    API_MODE_SUM = 0,
    API_MODE_MIN,
    API_MODE_MAX,
    API_MODE_ANY,
    API_MODE_ALL
};

// FindClosestPowerOfTwoV2 - 查找最接近的2的幂 (v2版本)
// 从原路径迁移: asc-devkit/impl/adv_api/detail/reduce/reduce_common_util_impl.h
__aicore__ inline uint32_t FindClosestPowerOfTwoV2(uint32_t n)
{
    ASCENDC_ASSERT(n != 0, { KERNEL_LOG(KERNEL_ERROR, "input n must be non-zero!"); });
    constexpr uint32_t totalShiftBits = 63;
    return totalShiftBits - ScalarCountLeadingZero(n);
}

// ComputeMaskBitV2 - 计算每个块的 Mask 位 (v2版本)
// 从原路径迁移: asc-devkit/impl/adv_api/detail/reduce/reduce_common_util_impl.h
// oneBlkMask: 每个块的mask位数
// oneBlkElems: 每个块的元素数
// blkNum: 块数量
template <class T>
__aicore__ inline void ComputeMaskBitV2(uint32_t oneBlkMask, uint32_t oneBlkElems, uint32_t blkNum,
    uint64_t& maskLow, uint64_t& maskHigh)
{
    // Get bit mask on each blkNum, oneBlkElems means number of elements in one block
    // only support half/float
    if constexpr (sizeof(T) == sizeof(half)) {
        uint32_t maskLBlkNum = blkNum > HALF_DEFAULT_REPEAT_STRIDE ? HALF_DEFAULT_REPEAT_STRIDE : blkNum;
        uint32_t maskHBlkNum = blkNum - maskLBlkNum;
        for (int32_t k = 0; k < maskLBlkNum; k++) {
            maskLow += (((1ULL << oneBlkMask) - 1ULL) << (k * oneBlkElems));
        }
        for (int32_t k = 0; k < maskHBlkNum; k++) {
            maskHigh += (((1ULL << oneBlkMask) - 1ULL) << (k * oneBlkElems));
        }
    } else if constexpr (sizeof(T) == sizeof(float)) {
        for (int32_t k = 0; k < blkNum; k++) {
            maskLow += (((1ULL << oneBlkMask) - 1ULL) << (k * oneBlkElems));
        }
    }
}

// BlockReduceComputeV2 - 块级归约计算调度 (v2版本)
// 从原路径迁移: asc-devkit/impl/adv_api/detail/reduce/reduce_common_util_impl.h
// 根据 ApiModeV2 分发到不同的归约操作（仅保留 MAX 相关）
//只保留MAX相关，枚举类可以删去
template <class T, ApiModeV2 apiMode, MaskMode maskMode = MaskMode::NORMAL>
__aicore__ inline void BlockReduceComputeV2(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const int32_t repeat, const uint64_t mask[], const int32_t blkStride, const int32_t repStride)
{
    if constexpr (maskMode == MaskMode::NORMAL) {
        if constexpr (apiMode == ApiModeV2::API_MODE_MAX || apiMode == ApiModeV2::API_MODE_ANY) {
            BlockReduceMax(dstTensor, srcTensor, repeat, mask, 1, 1, DEFAULT_REPEAT_STRIDE);
        }
    } else {
        if constexpr (apiMode == ApiModeV2::API_MODE_MAX || apiMode == ApiModeV2::API_MODE_ANY) {
            BlockReduceMax<T, false>(dstTensor, srcTensor, 1, MASK_PLACEHOLDER, 1, blkStride, repStride);
        }
    }
}

// WholeReduceComputeV2 - 整体归约计算调度 (v2版本)
// 从原路径迁移: asc-devkit/impl/adv_api/detail/reduce/reduce_common_util_impl.h
// 根据 ApiModeV2 分发到不同的归约操作（仅保留 MAX 相关）
template <class T, ApiModeV2 apiMode>
__aicore__ inline void WholeReduceComputeV2(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const int32_t repeat, const int32_t mask, const int32_t repStride)
{
    if constexpr (apiMode == ApiModeV2::API_MODE_MAX || apiMode == ApiModeV2::API_MODE_ANY) {
        WholeReduceMax(dstTensor, srcTensor, mask, repeat, 1, 1, repStride, ReduceOrder::ORDER_ONLY_VALUE);
    }
}

// ============================================================================
// 从 reduce_common_util_v220_impl.h 迁移的 v220 架构专用工具函数
// ============================================================================

// DoReduceLessThanBlkV2 - 小于一个块的归约 (v2版本)
// 从原路径迁移: asc-devkit/impl/adv_api/detail/reduce/reduce_common_util_v220_impl.h
template <typename T, ApiModeV2 apiMode>
__aicore__ inline void DoReduceLessThanBlkV2(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    uint32_t firstAxis, uint32_t lastAxis)
{
    constexpr uint32_t elePerBlk = ONE_BLK_SIZE / sizeof(T);
    constexpr uint32_t elePerRep = ONE_REPEAT_BYTE_SIZE / sizeof(T);
    uint32_t firstBlkRepeat = DivCeil(firstAxis, DEFAULT_BLK_NUM);
    uint32_t blkMaxRepeat = DivCeil(firstBlkRepeat, MAX_REPEAT_TIMES);
    uint32_t blkRepeatTail =
        firstBlkRepeat % MAX_REPEAT_TIMES == 0 ? MAX_REPEAT_TIMES : firstBlkRepeat % MAX_REPEAT_TIMES;
    uint32_t mainBlkNum = firstAxis < DEFAULT_BLK_NUM ? firstAxis : DEFAULT_BLK_NUM;
    uint64_t mainMaskLow = 0;
    uint64_t mainMaskHigh = 0;
    ComputeMaskBitV2<T>(lastAxis, elePerBlk, mainBlkNum, mainMaskLow, mainMaskHigh);
    uint64_t mainMask[] = { mainMaskLow, mainMaskHigh };
    uint32_t tailBlkNum = firstAxis % DEFAULT_BLK_NUM;
    if (tailBlkNum == 0 || firstAxis < DEFAULT_BLK_NUM) {
        uint32_t blkMainRepeat = MAX_REPEAT_TIMES;
        for (int32_t i = 0; i < blkMaxRepeat; i++) {
            blkMainRepeat = i == blkMaxRepeat - 1 ? blkRepeatTail : MAX_REPEAT_TIMES;
            BlockReduceComputeV2<T, apiMode>(dstTensor[i * MAX_REPEAT_TIMES * DEFAULT_BLK_NUM],
                srcTensor[i * MAX_REPEAT_TIMES * elePerRep], blkMainRepeat, mainMask, 1,
                DEFAULT_REPEAT_STRIDE);
            PipeBarrier<PIPE_V>();
        }
    } else {
        uint64_t tailMaskLow = 0;
        uint64_t tailMaskHigh = 0;
        ComputeMaskBitV2<T>(lastAxis, elePerBlk, tailBlkNum, tailMaskLow, tailMaskHigh);
        uint64_t tailMask[] = { tailMaskLow, tailMaskHigh };
        for (int32_t i = 0; i < blkMaxRepeat; i++) {
            if (i == blkMaxRepeat - 1) {
                BlockReduceComputeV2<T, apiMode>(dstTensor[i * MAX_REPEAT_TIMES * DEFAULT_BLK_NUM],
                    srcTensor[i * MAX_REPEAT_TIMES * elePerRep], blkRepeatTail - 1, mainMask, 1,
                    DEFAULT_REPEAT_STRIDE);
                PipeBarrier<PIPE_V>();
                BlockReduceComputeV2<T, apiMode>(dstTensor[(i * MAX_REPEAT_TIMES + blkRepeatTail - 1) * DEFAULT_BLK_NUM],
                    srcTensor[(i * MAX_REPEAT_TIMES + blkRepeatTail - 1) * elePerRep], 1, tailMask, 1,
                    DEFAULT_REPEAT_STRIDE);
            } else {
                BlockReduceComputeV2<T, apiMode>(dstTensor[i * MAX_REPEAT_TIMES * DEFAULT_BLK_NUM],
                    srcTensor[i * MAX_REPEAT_TIMES * elePerRep], MAX_REPEAT_TIMES, mainMask, 1,
                    DEFAULT_REPEAT_STRIDE);
                PipeBarrier<PIPE_V>();
            }
        }
    }
}

// DoReduceOneBlkV2 - 正好一个块的归约 (v2版本)
template <typename T, ApiModeV2 apiMode>
__aicore__ inline void DoReduceOneBlkV2(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    uint32_t firstAxis, uint32_t lastAxis)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, firstAxis * lastAxis);
    BlockReduceComputeV2<T, apiMode, MaskMode::COUNTER>(dstTensor, srcTensor, 1, MASK_PLACEHOLDER_LIST, 1,
        DEFAULT_REPEAT_STRIDE);
}

// AccValOnBlkV2 - 块内累加/比较 (v2版本)
template <typename T, void (*func)(const LocalTensor<T> &, const LocalTensor<T> &, const LocalTensor<T> &, uint64_t,
    const uint8_t, const BinaryRepeatParams &)>
__aicore__ inline void AccValOnBlkV2(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const LocalTensor<T>& tmpTensor, const BinaryRepeatParams& mainParams, const BinaryRepeatParams& tailParams,
    uint32_t firstAxis, uint32_t lastAxis, uint32_t tmpOffset, uint32_t padLast)
{
    constexpr uint32_t elePerBlk = ONE_BLK_SIZE / sizeof(T);
    uint32_t firstRepeat = DivCeil(firstAxis, MAX_REPEAT_TIMES);
    uint32_t firstRepeatTail = firstAxis % MAX_REPEAT_TIMES == 0 ? MAX_REPEAT_TIMES : firstAxis % MAX_REPEAT_TIMES;
    uint32_t blkCount = lastAxis / elePerBlk;
    uint32_t blkTail = lastAxis % elePerBlk;
    for (int32_t i = 1; i < blkCount; i++) {
        SetVectorMask<T, MaskMode::COUNTER>(0, firstAxis * elePerBlk);
        func(tmpTensor, tmpTensor, srcTensor[i * elePerBlk], MASK_PLACEHOLDER, 1, mainParams);
        PipeBarrier<PIPE_V>();
    }
    if (blkTail != 0) {
        SetMaskNorm();
        SetVectorMask<T, MaskMode::NORMAL>(blkTail);
        uint32_t blkRepeat = MAX_REPEAT_TIMES;
        for (int32_t i = 0; i < firstRepeat; i++) {
            blkRepeat = i == firstRepeat - 1 ? firstRepeatTail : MAX_REPEAT_TIMES;
            func(tmpTensor[i * MAX_REPEAT_TIMES * tmpOffset], tmpTensor[i * MAX_REPEAT_TIMES * tmpOffset],
                srcTensor[i * MAX_REPEAT_TIMES * padLast + blkCount * elePerBlk], blkTail, blkRepeat, tailParams);
            PipeBarrier<PIPE_V>();
        }
        SetMaskCount();
    }
}

// DoReduceByBlkV2 - 多块归约 (v2版本)
template <typename T, bool isReuseSource, ApiModeV2 apiMode,
    void (*func)(const LocalTensor<T> &, const LocalTensor<T> &, const LocalTensor<T> &, uint64_t, const uint8_t,
        const BinaryRepeatParams &)>
__aicore__ inline void DoReduceByBlkV2(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const LocalTensor<T>& tmpTensor, uint32_t firstAxis, uint32_t lastAxis, uint32_t padLast)
{
    constexpr uint32_t elePerBlk = ONE_BLK_SIZE / sizeof(T);
    uint8_t blkStridePerRow = padLast / elePerBlk;
    uint8_t blkStridePerRep = (padLast / elePerBlk) * DEFAULT_BLK_NUM;
    SetMaskCount();
    if constexpr (!isReuseSource) {
        UnaryRepeatParams blockUnaryParams{ 1, blkStridePerRow, DEFAULT_REPEAT_STRIDE, blkStridePerRep };
        SetVectorMask<T, MaskMode::COUNTER>(0, firstAxis * elePerBlk);
        Adds<T, false>(tmpTensor, srcTensor, static_cast<T>(0), MASK_PLACEHOLDER, 1, blockUnaryParams);
        PipeBarrier<PIPE_V>();
        BinaryRepeatParams blockMainParams{ 1, 1, blkStridePerRow, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE, blkStridePerRep};
        BinaryRepeatParams blockTailParams{ 1, 1, 1, 1, 1, blkStridePerRow };
        AccValOnBlkV2<T, func>(dstTensor, srcTensor, tmpTensor, blockMainParams, blockTailParams, firstAxis, lastAxis, elePerBlk, padLast);
        SetVectorMask<T, MaskMode::COUNTER>(0, firstAxis * elePerBlk);
        BlockReduceComputeV2<T, apiMode, MaskMode::COUNTER>(dstTensor, tmpTensor, 1, MASK_PLACEHOLDER_LIST, 1,
            DEFAULT_REPEAT_STRIDE);
    } else {
        BinaryRepeatParams blockMainParams{ blkStridePerRow, blkStridePerRow, blkStridePerRow, blkStridePerRep,
            blkStridePerRep, blkStridePerRep};
        BinaryRepeatParams blockTailParams{ 1, 1, 1, blkStridePerRow, blkStridePerRow, blkStridePerRow };
        AccValOnBlkV2<T, func>(dstTensor, srcTensor, srcTensor, blockMainParams, blockTailParams, firstAxis, lastAxis, padLast, padLast);
        SetVectorMask<T, MaskMode::COUNTER>(0, firstAxis * elePerBlk);
        BlockReduceComputeV2<T, apiMode, MaskMode::COUNTER>(dstTensor, srcTensor, 1, MASK_PLACEHOLDER_LIST, blkStridePerRow,
            blkStridePerRep);
    }
}

// GetReduceValOnRepV2 - 从重复数据中获取归约值 (v2版本)
template <typename T, ApiModeV2 apiMode>
__aicore__ inline void GetReduceValOnRepV2(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const LocalTensor<T>& tmpTensor, uint32_t firstAxis, uint32_t tmpOffset, uint32_t repStride)
{
    constexpr uint32_t elePerRep = ONE_REPEAT_BYTE_SIZE / sizeof(T);
    uint32_t firstRepeat = DivCeil(firstAxis, MAX_REPEAT_TIMES);
    uint32_t firstRepeatTail = firstAxis % MAX_REPEAT_TIMES == 0 ? MAX_REPEAT_TIMES : firstAxis % MAX_REPEAT_TIMES;
    if constexpr (IsSameType<T, half>::value) {
        SetMaskNorm();
        uint32_t blockRepeat = MAX_REPEAT_TIMES;
        for (int32_t i = 0; i < firstRepeat; i++) {
            blockRepeat = i == firstRepeat - 1 ? firstRepeatTail : MAX_REPEAT_TIMES;
            WholeReduceComputeV2<T, apiMode>(dstTensor[i * MAX_REPEAT_TIMES], tmpTensor[i * MAX_REPEAT_TIMES * tmpOffset],
                blockRepeat, elePerRep, repStride);
            PipeBarrier<PIPE_V>();
        }
    } else {
        SetVectorMask<T, MaskMode::COUNTER>(0, firstAxis * elePerRep);
        BlockReduceComputeV2<T, apiMode, MaskMode::COUNTER>(tmpTensor, tmpTensor, 1, MASK_PLACEHOLDER_LIST, 1,
            repStride);
        PipeBarrier<PIPE_V>();
        SetVectorMask<T, MaskMode::COUNTER>(0, firstAxis * DEFAULT_BLK_NUM);
        BlockReduceComputeV2<T, apiMode, MaskMode::COUNTER>(dstTensor, tmpTensor, 1, MASK_PLACEHOLDER_LIST, 1,
            DEFAULT_REPEAT_STRIDE);
    }
}

// AccValOnRepV2 - 重复数据累加/比较 (v2版本)
template <typename T, ApiModeV2 apiMode,
    void (*func)(const LocalTensor<T> &, const LocalTensor<T> &, const LocalTensor<T> &, uint64_t, const uint8_t,
    const BinaryRepeatParams &)>
__aicore__ inline void AccValOnRepV2(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const LocalTensor<T>& tmpTensor, const BinaryRepeatParams& binaryParams, uint32_t firstAxis, uint32_t lastAxis,
    uint32_t tmpOffset, uint32_t repStride, uint32_t padLast)
{
    constexpr uint32_t elePerBlk = ONE_BLK_SIZE / sizeof(T);
    constexpr uint32_t elePerRep = ONE_REPEAT_BYTE_SIZE / sizeof(T);
    uint32_t firstRepeat = DivCeil(firstAxis, MAX_REPEAT_TIMES);
    uint32_t firstRepeatTail = firstAxis % MAX_REPEAT_TIMES == 0 ? MAX_REPEAT_TIMES : firstAxis % MAX_REPEAT_TIMES;
    uint32_t repCount = lastAxis / elePerRep;
    uint32_t repTail = lastAxis % elePerRep;
    for (int32_t i = 1; i < repCount; i++) {
        SetVectorMask<T, MaskMode::COUNTER>(0, firstAxis * elePerRep);
        func(tmpTensor, tmpTensor, srcTensor[i * elePerRep], MASK_PLACEHOLDER, 1, binaryParams);
        PipeBarrier<PIPE_V>();
    }
    if (repTail != 0) {
        SetMaskNorm();
        SetVectorMask<T, MaskMode::NORMAL>(repTail);
        uint32_t repRepeat = MAX_REPEAT_TIMES;
        for (int32_t i = 0; i < firstRepeat; i++) {
            repRepeat = i == firstRepeat - 1 ? firstRepeatTail : MAX_REPEAT_TIMES;
            func(tmpTensor[i * MAX_REPEAT_TIMES * tmpOffset], tmpTensor[i * MAX_REPEAT_TIMES * tmpOffset],
                srcTensor[i * MAX_REPEAT_TIMES * padLast + repCount * elePerRep], repTail, repRepeat, binaryParams);
            PipeBarrier<PIPE_V>();
        }
        SetMaskCount();
    }
    GetReduceValOnRepV2<T, apiMode>(dstTensor, srcTensor, tmpTensor, firstAxis, tmpOffset, repStride);
}

// DoReduceByRepV2 - 重复块归约 (v2版本)
template <typename T, bool isReuseSource, ApiModeV2 apiMode,
    void (*func)(const LocalTensor<T> &, const LocalTensor<T> &, const LocalTensor<T> &, uint64_t, const uint8_t,
        const BinaryRepeatParams &)>
__aicore__ inline void DoReduceByRepV2(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const LocalTensor<T>& tmpTensor, uint32_t firstAxis, uint32_t lastAxis, uint32_t padLast)
{
    constexpr uint32_t elePerBlk = ONE_BLK_SIZE / sizeof(T);
    constexpr uint32_t elePerRep = ONE_REPEAT_BYTE_SIZE / sizeof(T);
    uint8_t repStridePerRow = padLast / elePerBlk;
    SetMaskCount();
    if constexpr (!isReuseSource) {
        UnaryRepeatParams repeatUnaryParams{ 1, 1, DEFAULT_REPEAT_STRIDE, repStridePerRow };
        SetVectorMask<T, MaskMode::COUNTER>(0, firstAxis * elePerRep);
        Adds<T, false>(tmpTensor, srcTensor, static_cast<T>(0), MASK_PLACEHOLDER, 1, repeatUnaryParams);
        PipeBarrier<PIPE_V>();
        BinaryRepeatParams binaryParams{ 1, 1, 1, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE, repStridePerRow};
        AccValOnRepV2<T, apiMode, func>(dstTensor, srcTensor, tmpTensor, binaryParams, firstAxis, lastAxis, elePerRep,
            DEFAULT_REPEAT_STRIDE, padLast);
    } else {
        BinaryRepeatParams binaryParams{ 1, 1, 1, repStridePerRow, repStridePerRow, repStridePerRow};
        AccValOnRepV2<T, apiMode, func>(dstTensor, srcTensor, srcTensor, binaryParams, firstAxis, lastAxis, padLast,
            repStridePerRow, padLast);
    }
}

// DoLongLastReduceV2 - 长维度归约 (v2版本)
template <typename T, bool isReuseSource, ApiModeV2 apiMode,
    void (*func)(const LocalTensor<T> &, const LocalTensor<T> &, const LocalTensor<T> &, uint64_t, const uint8_t,
        const BinaryRepeatParams &)>
__aicore__ inline void DoLongLastReduceV2(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const LocalTensor<T>& tmpTensor, uint32_t firstAxis, uint32_t lastAxis, uint32_t padLast)
{
    constexpr uint32_t elePerBlk = ONE_BLK_SIZE / sizeof(T);
    constexpr uint32_t elePerRep = ONE_REPEAT_BYTE_SIZE / sizeof(T);
    uint32_t repCount = DivCeil(lastAxis, elePerRep);
    uint32_t repTail = lastAxis % elePerRep == 0 ? elePerRep : lastAxis % elePerRep;
    BinaryRepeatParams defaultParams;
    UnaryRepeatParams defaultUnaryParams;
    SetMaskCount();
    if constexpr (!isReuseSource) {
        SetVectorMask<T, MaskMode::COUNTER>(0, elePerRep);
        for (int32_t i = 0; i < firstAxis; i++) {
            Adds<T, false>(tmpTensor[i * elePerRep], srcTensor[i * padLast], static_cast<T>(0), MASK_PLACEHOLDER,
                1, defaultUnaryParams);
            PipeBarrier<PIPE_V>();
        }
        uint32_t mask = elePerRep;
        for (int32_t i = 1; i < repCount; i++) {
            mask = i == repCount - 1 ? repTail : elePerRep;
            SetVectorMask<T, MaskMode::COUNTER>(0, mask);
            for (int32_t j = 0; j < firstAxis; j++) {
                func(tmpTensor[j * elePerRep], tmpTensor[j * elePerRep],
                    srcTensor[j * padLast + i * elePerRep], MASK_PLACEHOLDER, 1, defaultParams);
                PipeBarrier<PIPE_V>();
            }
        }
        GetReduceValOnRepV2<T, apiMode>(dstTensor, srcTensor, tmpTensor, firstAxis, elePerRep, DEFAULT_REPEAT_STRIDE);
    } else {
        uint32_t mask = elePerRep;
        for (int32_t i = 0; i < firstAxis; i++) {
            for (int32_t j = 1; j < repCount; j++) {
                mask = j == repCount - 1 ? repTail : elePerRep;
                SetVectorMask<T, MaskMode::COUNTER>(0, mask);
                if (j == 1) {
                    func(srcTensor[i * elePerRep], srcTensor[i * padLast], srcTensor[i * padLast + j * elePerRep],
                        MASK_PLACEHOLDER, 1, defaultParams);
                    PipeBarrier<PIPE_V>();
                } else {
                    func(srcTensor[i * elePerRep], srcTensor[i * elePerRep], srcTensor[i * padLast + j * elePerRep],
                        MASK_PLACEHOLDER, 1, defaultParams);
                    PipeBarrier<PIPE_V>();
                }
            }
        }
        GetReduceValOnRepV2<T, apiMode>(dstTensor, srcTensor, srcTensor, firstAxis, elePerRep, DEFAULT_REPEAT_STRIDE);
    }
}

// ReduceParamsV2 - 归约参数结构体 (v2版本)
struct ReduceParamsV2 {
public:
    __aicore__ ReduceParamsV2() {}
    __aicore__ ReduceParamsV2(uint32_t first, uint32_t last,
        uint32_t padLast, uint32_t splitK, uint32_t tail, uint32_t elePerBlk)
    {
        this->first = first;
        this->last = last;
        this->padLast = padLast;
        this->splitK = splitK;
        this->tail = tail;
        this->elePerBlk = elePerBlk;
    }
    uint32_t first = 0;
    uint32_t last = 0;
    uint32_t padLast = 0;
    uint32_t splitK = 0;
    uint32_t tail = 0;
    uint32_t elePerBlk = 0;
    BinaryRepeatParams defaultParam = { DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE,
                DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE };
    UnaryRepeatParams defaultUnaryParam = { DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE,
                DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE };
};

// BlkReduceForLoopV2 - 块级归约循环 (v2版本)
template <class T, ApiModeV2 apiMode>
__aicore__ inline void BlkReduceForLoopV2(const LocalTensor<T>& dst,
    const LocalTensor<T>& tmp, uint32_t srcOffset, uint32_t first, uint32_t last) {
    constexpr uint32_t blkReduceDstStride = 8;  // elements uint
    uint32_t srcPerBlkElements = ONE_BLK_SIZE/sizeof(T);
    uint64_t maskHigh = 0;
    uint32_t oneRepElements = srcPerBlkElements * DEFAULT_BLK_NUM;
    uint32_t nMaxRepBlkNum = first / (MAX_REPEAT_TIMES * DEFAULT_BLK_NUM);
    uint32_t tailMaxRepBlkNum = first % (MAX_REPEAT_TIMES * DEFAULT_BLK_NUM);
    uint32_t tailNBlkNum = tailMaxRepBlkNum / DEFAULT_BLK_NUM;
    uint32_t tailRemainOfBlkNum = tailMaxRepBlkNum % DEFAULT_BLK_NUM;
    uint32_t dstOffset = 0;
    uint32_t blkReduceSrcOffset = 0;
    uint32_t oneBlkMask = last > srcPerBlkElements ? srcPerBlkElements : last;
    uint64_t maskLow = 0;
    ComputeMaskBitV2<T>(oneBlkMask, srcPerBlkElements, DEFAULT_BLK_NUM, maskLow, maskHigh);

    uint64_t blkReduceMask[] = { maskLow, maskHigh };
    for (int k = 0; k < nMaxRepBlkNum; k++) {
        BlockReduceComputeV2<T, apiMode>(dst[dstOffset], tmp[srcOffset], MAX_REPEAT_TIMES, blkReduceMask,
            DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE);
        PipeBarrier<PIPE_V>();
    }
    if (tailNBlkNum > 0) {
        dstOffset = nMaxRepBlkNum * MAX_REPEAT_TIMES * blkReduceDstStride;
        blkReduceSrcOffset = srcOffset + nMaxRepBlkNum * MAX_REPEAT_TIMES * DEFAULT_BLK_NUM * srcPerBlkElements;
        BlockReduceComputeV2<T, apiMode>(dst[dstOffset], tmp[blkReduceSrcOffset], tailNBlkNum, blkReduceMask,
            DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE);
        PipeBarrier<PIPE_V>();
    }
    if (tailRemainOfBlkNum > 0) {
        maskLow = 0;
        maskHigh = 0;
        uint32_t tailBlkReduceRep = 1;
        ComputeMaskBitV2<T>(oneBlkMask, srcPerBlkElements, tailRemainOfBlkNum, maskLow, maskHigh);
        uint64_t tailMask[] = { maskLow, maskHigh };
        dstOffset = tailNBlkNum * blkReduceDstStride + (nMaxRepBlkNum * MAX_REPEAT_TIMES * blkReduceDstStride);
        blkReduceSrcOffset = tailNBlkNum * oneRepElements + (nMaxRepBlkNum * MAX_REPEAT_TIMES * oneRepElements);
        BlockReduceComputeV2<T, apiMode>(dst[dstOffset], tmp[blkReduceSrcOffset], tailBlkReduceRep, tailMask,
            DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE);
        PipeBarrier<PIPE_V>();
    }
}

// BinaryReduceByFirstAxisV2 - 第一维归约（RA 模式）(v2版本)
// 从原路径迁移: asc-devkit/impl/adv_api/detail/reduce/reduce_common_util_v220_impl.h
template <typename T, bool isReuseSource,
    void (*func)(const LocalTensor<T> &, const LocalTensor<T> &, const LocalTensor<T> &, uint64_t, const uint8_t,
    const BinaryRepeatParams &)>
__aicore__ inline void BinaryReduceByFirstAxisV2(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const LocalTensor<T>& tmpTensor, uint32_t firstAxis, uint32_t lastAxis, uint32_t padLast)
{
    ASCENDC_ASSERT((dstTensor.GetSize() >= lastAxis), {
        KERNEL_LOG(KERNEL_ERROR, "dstTensor must be greater than or equal to %u, current size if %u",
            lastAxis, dstTensor.GetSize());
    });
    BinaryRepeatParams defaultParam;
    UnaryRepeatParams defaultUnaryParam;
    uint32_t k = FindClosestPowerOfTwoV2(firstAxis);
    uint32_t splitK = 1 << k;
    uint32_t remain = firstAxis - splitK;
    SetMaskCount();
    if constexpr (isReuseSource) {
        // reduce tail part
        if (remain != 0) {
            SetVectorMask<T, MaskMode::COUNTER>(0, padLast * remain);
            func(srcTensor, srcTensor, srcTensor[splitK * padLast], MASK_PLACEHOLDER, 1, defaultParam);
            PipeBarrier<PIPE_V>();
        }
    } else {
        AscendC::CheckTmpBufferSize(tmpTensor.GetSize(), 0, tmpTensor.GetSize());
        // reduce tail part
        if (remain != 0) {
            SetVectorMask<T, MaskMode::COUNTER>(0, splitK * padLast);
            Adds<T, false>(tmpTensor, srcTensor, static_cast<T>(0), MASK_PLACEHOLDER, 1,
                           defaultUnaryParam);
            PipeBarrier<PIPE_V>();
            SetVectorMask<T, MaskMode::COUNTER>(0, padLast * remain);
            func(tmpTensor, tmpTensor, srcTensor[splitK * padLast], MASK_PLACEHOLDER, 1, defaultParam);
            PipeBarrier<PIPE_V>();
        } else if (splitK > 1) { // binary reduce first part from the srcTensor
            splitK >>= 1;
            SetVectorMask<T, MaskMode::COUNTER>(0, padLast * splitK);
            func(tmpTensor, srcTensor, srcTensor[splitK * padLast], MASK_PLACEHOLDER, 1, defaultParam);
            PipeBarrier<PIPE_V>();
        } else { // move src to dst directly if axis size is one.
            SetVectorMask<T, MaskMode::COUNTER>(0, lastAxis);
            Adds<T, false>(dstTensor, srcTensor, static_cast<T>(0), MASK_PLACEHOLDER, 1, defaultUnaryParam);
            PipeBarrier<PIPE_V>();
            return;
        }
    }
    // binary reduce remain 2^k axis 
    LocalTensor<T> currBuff = isReuseSource ? srcTensor : tmpTensor;
    while (splitK > 1) {
        splitK >>= 1;
        SetVectorMask<T, MaskMode::COUNTER>(0, padLast * splitK);
        func(currBuff, currBuff, currBuff[splitK * padLast], MASK_PLACEHOLDER, 1, defaultParam);
        PipeBarrier<PIPE_V>();
    }
    SetVectorMask<T, MaskMode::COUNTER>(0, lastAxis);
    Adds<T, false>(dstTensor, currBuff, static_cast<T>(0), MASK_PLACEHOLDER, 1, defaultUnaryParam);
    PipeBarrier<PIPE_V>();
}

// BlockReduceByLastAxisV2 - 最后一维归约（AR 模式）(v2版本)
// 从原路径迁移: asc-devkit/impl/adv_api/detail/reduce/reduce_common_util_v220_impl.h
template <typename T, bool isReuseSource, ApiModeV2 apiMode,
    void (*func)(const LocalTensor<T> &, const LocalTensor<T> &, const LocalTensor<T> &, uint64_t, const uint8_t,
    const BinaryRepeatParams &)>
__aicore__ inline void BlockReduceByLastAxisV2(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const LocalTensor<T>& tmpTensor, uint32_t firstAxis, uint32_t lastAxis, uint32_t padLast)
{
    ASCENDC_ASSERT((dstTensor.GetSize() >= firstAxis), {
        KERNEL_LOG(KERNEL_ERROR, "dstTensor must be greater than or equal to %u, current size if %u",
            firstAxis, dstTensor.GetSize());
    });
    constexpr uint32_t elePerBlk = ONE_BLK_SIZE / sizeof(T);
    constexpr uint32_t elePerRep = ONE_REPEAT_BYTE_SIZE / sizeof(T);
    if (lastAxis < elePerBlk) {
        DoReduceLessThanBlkV2<T, apiMode>(dstTensor, srcTensor, firstAxis, lastAxis);
    } else if (lastAxis == elePerBlk) {
        DoReduceOneBlkV2<T, apiMode>(dstTensor, srcTensor, firstAxis, lastAxis);
    } else if (lastAxis > elePerBlk && lastAxis < elePerRep) {
        DoReduceByBlkV2<T, isReuseSource, apiMode, func>(dstTensor, srcTensor, tmpTensor, firstAxis, lastAxis, padLast);
    } else if (lastAxis >= elePerRep && lastAxis <= MAX_REPEAT_TIMES * elePerBlk) {
        DoReduceByRepV2<T, isReuseSource, apiMode, func>(dstTensor, srcTensor, tmpTensor, firstAxis, lastAxis, padLast);
    } else {
        DoLongLastReduceV2<T, isReuseSource, apiMode, func>(dstTensor, srcTensor, tmpTensor, firstAxis, lastAxis, padLast);
    }
}

} // namespace Internal
} // namespace AscendC
#endif // REDUCE_MAX_COMMON_UTIL_H
