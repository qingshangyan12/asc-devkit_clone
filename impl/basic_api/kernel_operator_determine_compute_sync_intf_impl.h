/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_operator_determine_compute_sync_intf_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_determine_compute_sync_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_determine_compute_sync_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_IMPL_H
#define ASCENDC_MODULE_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_IMPL_H

#if __NPU_ARCH__ == 1001
#include "dav_c100/kernel_operator_determine_compute_sync_impl.h"
#elif __NPU_ARCH__ == 2002
#include "dav_m200/kernel_operator_determine_compute_sync_impl.h"
#elif __NPU_ARCH__ == 2201
#include "dav_c220/kernel_operator_determine_compute_sync_impl.h"
#elif __NPU_ARCH__ == 3002
#include "dav_m300/kernel_operator_determine_compute_sync_impl.h"
#elif __NPU_ARCH__ == 3102
#include "dav_m310/kernel_operator_determine_compute_sync_impl.h"
#elif __NPU_ARCH__ == 3510
#include "dav_3510/kernel_operator_determine_compute_sync_impl.h"
#elif (__NPU_ARCH__ == 5102)
#include "dav_m510/kernel_operator_determine_compute_sync_impl.h"
#elif __NPU_ARCH__ == 3003
#include "dav_l300/kernel_operator_determine_compute_sync_impl.h"
#elif __NPU_ARCH__ == 3113
#include "dav_l311/kernel_operator_determine_compute_sync_impl.h"
#endif

namespace AscendC {
/*
 * @ingroup WaitPreBlock
 * @brief wait previous compute to finish
 * @param [in] global memory workspace
 */
__aicore__ inline void InitDetermineComputeWorkspace(
    GlobalTensor<int32_t>& gmWorkspace, LocalTensor<int32_t>& ubWorkspace)
{
    InitDetermineComputeWorkspaceCalc(gmWorkspace, ubWorkspace);
}
/*
 * @ingroup WaitPreBlock
 * @brief wait previous compute to finish
 * @param [in] global memory workspace
 */
__aicore__ inline void WaitPreBlock(GlobalTensor<int32_t>& gmWorkspace, LocalTensor<int32_t>& ubWorkspace)
{
    WaitPreBlockCalc(gmWorkspace, ubWorkspace);
}

/*
 * @ingroup NotifyNextBlock
 * @brief wait previous compute to finish
 * @param [in] global memory workspace
 */
__aicore__ inline void NotifyNextBlock(GlobalTensor<int32_t>& gmWorkspace, LocalTensor<int32_t>& ubWorkspace)
{
    NotifyNextBlockCalc(gmWorkspace, ubWorkspace);
}

template <typename GmWorkspace, typename UbWorkspace>
__aicore__ inline void CheckDetermineComputePointerPositions(
    const GmWorkspace& gmWorkspace, const UbWorkspace& ubWorkspace)
{
    using GmType = Internal::WritableOperandPrimT<GmWorkspace>;
    using UbType = Internal::WritableOperandPrimT<UbWorkspace>;
    static_assert(
        Std::is_same<GmType, int32_t>::value && Std::is_same<UbType, int32_t>::value,
        "Determine-compute workspaces must contain int32_t");
    ASCENDC_ASSERT((GetPhyType(GetOperandPosition(gmWorkspace)) == Hardware::GM), {
        KERNEL_LOG(KERNEL_ERROR, "Determine-compute gmWorkspace must be in Global Memory");
    });
    ASCENDC_ASSERT((GetPhyType(GetOperandPosition(ubWorkspace)) == Hardware::UB), {
        KERNEL_LOG(KERNEL_ERROR, "Determine-compute ubWorkspace must be in Unified Buffer");
    });
}

template <typename GmWorkspace>
__aicore__ inline void DetermineComputeCopyGM2UB(
    __ubuf__ int32_t* dst, const GmWorkspace& gmWorkspace, __gm__ int32_t* src, uint32_t count)
{
    DataCopyParams params;
    DataCopyCheck<int32_t>(count, params);
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyGM2UBImpl(dst, src, params, GetOperandCacheMode(gmWorkspace));
#else
    DataCopyGM2UBImpl(dst, src, params);
#endif
}

template <typename GmWorkspace>
__aicore__ inline void DetermineComputeCopyUB2GM(
    const GmWorkspace& gmWorkspace, __gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    DataCopyParams params;
    DataCopyCheck<int32_t>(count, params);
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
    DataCopyUB2GMImpl(dst, src, params, GetOperandCacheMode(gmWorkspace));
#else
    DataCopyUB2GMImpl(dst, src, params);
#endif
}

__aicore__ inline bool CheckUBWorkspacePointer(__ubuf__ int32_t* ubWorkspace, int64_t blockIdx, int64_t blockNum)
{
    const int32_t repeatTime = ubWorkspace[blockNum * B32_DATA_NUM_PER_BLOCK];
    int64_t offset = 0;
    for (; offset < blockIdx * B32_DATA_NUM_PER_BLOCK; offset += B32_DATA_NUM_PER_BLOCK) {
        if (ubWorkspace[offset] != repeatTime) {
            return false;
        }
    }
    for (; offset < blockNum * B32_DATA_NUM_PER_BLOCK; offset += B32_DATA_NUM_PER_BLOCK) {
        if (ubWorkspace[offset] != 0) {
            return false;
        }
    }
    return true;
}

template <
    typename GmWorkspace, typename UbWorkspace,
    typename Std::enable_if<
        Internal::IsPointerOperand<GmWorkspace> || Internal::IsPointerOperand<UbWorkspace>, bool>::type>
__aicore__ inline void InitDetermineComputeWorkspace(const GmWorkspace& gmWorkspace, const UbWorkspace& ubWorkspace)
{
    CheckDetermineComputePointerPositions(gmWorkspace, ubWorkspace);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    if ASCEND_IS_AIV {
        __gm__ int32_t* gmPtr = (__gm__ int32_t*)GetUnderlyingPtr(gmWorkspace);
        __ubuf__ int32_t* ubPtr = (__ubuf__ int32_t*)GetUnderlyingPtr(ubWorkspace);
        PipeBarrier<PIPE_ALL>();
        const int64_t blockNum = GetBlockNum();
        if (GetBlockIdx() == 0) {
            DuplicateImpl(ubPtr, 0, B32_DATA_NUM_PER_BLOCK * blockNum);
            event_t eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
            SetFlag<HardEvent::V_MTE3>(eventID);
            WaitFlag<HardEvent::V_MTE3>(eventID);
            DetermineComputeCopyUB2GM(gmWorkspace, gmPtr, ubPtr, B32_DATA_NUM_PER_BLOCK * blockNum);
        }
        ubPtr[blockNum * B32_DATA_NUM_PER_BLOCK] = 1;
        PipeBarrier<PIPE_ALL>();
    }
#else
    ASCENDC_ASSERT((false), {
        KERNEL_LOG(KERNEL_ERROR, "Pointer InitDetermineComputeWorkspace is not supported on this device");
    });
#endif
}

template <
    typename GmWorkspace, typename UbWorkspace,
    typename Std::enable_if<
        Internal::IsPointerOperand<GmWorkspace> || Internal::IsPointerOperand<UbWorkspace>, bool>::type>
__aicore__ inline void WaitPreBlock(const GmWorkspace& gmWorkspace, const UbWorkspace& ubWorkspace)
{
    CheckDetermineComputePointerPositions(gmWorkspace, ubWorkspace);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    if ASCEND_IS_AIV {
        __gm__ int32_t* gmPtr = (__gm__ int32_t*)GetUnderlyingPtr(gmWorkspace);
        __ubuf__ int32_t* ubPtr = (__ubuf__ int32_t*)GetUnderlyingPtr(ubWorkspace);
        PipeBarrier<PIPE_ALL>();
        const int64_t blockIdx = GetBlockIdx();
        const int64_t blockNum = GetBlockNum();
        bool matchFlag;
        do {
            DetermineComputeCopyGM2UB(ubPtr, gmWorkspace, gmPtr, B32_DATA_NUM_PER_BLOCK * blockNum);
            event_t eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE2_S));
            SetFlag<HardEvent::MTE2_S>(eventID);
            WaitFlag<HardEvent::MTE2_S>(eventID);
            matchFlag = CheckUBWorkspacePointer(ubPtr, blockIdx, blockNum);
            eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_MTE2));
            SetFlag<HardEvent::S_MTE2>(eventID);
            WaitFlag<HardEvent::S_MTE2>(eventID);
        } while (!matchFlag);
        PipeBarrier<PIPE_ALL>();
    }
#else
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "Pointer WaitPreBlock is not supported on this device"); });
#endif
}

template <
    typename GmWorkspace, typename UbWorkspace,
    typename Std::enable_if<
        Internal::IsPointerOperand<GmWorkspace> || Internal::IsPointerOperand<UbWorkspace>, bool>::type>
__aicore__ inline void NotifyNextBlock(const GmWorkspace& gmWorkspace, const UbWorkspace& ubWorkspace)
{
    CheckDetermineComputePointerPositions(gmWorkspace, ubWorkspace);
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    if ASCEND_IS_AIV {
        __gm__ int32_t* gmPtr = (__gm__ int32_t*)GetUnderlyingPtr(gmWorkspace);
        __ubuf__ int32_t* ubPtr = (__ubuf__ int32_t*)GetUnderlyingPtr(ubWorkspace);
        PipeBarrier<PIPE_ALL>();
        const int64_t blockIdx = GetBlockIdx();
        const int64_t blockNum = GetBlockNum();
        const int32_t repeatTime = ubPtr[blockNum * B32_DATA_NUM_PER_BLOCK];
        if (blockIdx + 1 == blockNum) {
            DuplicateImpl(ubPtr, 0, blockNum * B32_DATA_NUM_PER_BLOCK);
            event_t eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
            SetFlag<HardEvent::V_MTE3>(eventID);
            WaitFlag<HardEvent::V_MTE3>(eventID);
            DetermineComputeCopyUB2GM(gmWorkspace, gmPtr, ubPtr, blockNum * B32_DATA_NUM_PER_BLOCK);
        } else {
            const int64_t offset = blockIdx * B32_DATA_NUM_PER_BLOCK;
            event_t eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
            SetFlag<HardEvent::S_V>(eventID);
            WaitFlag<HardEvent::S_V>(eventID);
            DuplicateImpl(ubPtr + offset, repeatTime, B32_DATA_NUM_PER_BLOCK);
            eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
            SetFlag<HardEvent::V_MTE3>(eventID);
            WaitFlag<HardEvent::V_MTE3>(eventID);
            DetermineComputeCopyUB2GM(gmWorkspace, gmPtr + offset, ubPtr + offset, B32_DATA_NUM_PER_BLOCK);
        }
        ubPtr[blockNum * B32_DATA_NUM_PER_BLOCK] = repeatTime + 1;
        PipeBarrier<PIPE_ALL>();
    }
#else
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "Pointer NotifyNextBlock is not supported on this device"); });
#endif
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_IMPL_H__
#endif
