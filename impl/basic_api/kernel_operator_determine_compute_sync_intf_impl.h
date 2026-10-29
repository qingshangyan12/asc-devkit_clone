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
#elif __NPU_ARCH__ == 9201
#include "dav_9201/kernel_operator_determine_compute_sync_impl.h"
#elif __NPU_ARCH__ == 9202
#include "dav_9202/kernel_operator_determine_compute_sync_impl.h"
#elif (__NPU_ARCH__ == 5102)
#include "dav_m510/kernel_operator_determine_compute_sync_impl.h"
#elif (__NPU_ARCH__ == 5162)
#include "dav_5162/kernel_operator_determine_compute_sync_impl.h"
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
/* **************************************************************************************************
 * Raw-pointer forms of InitDetermineComputeWorkspace / WaitPreBlock / NotifyNextBlock.
 *
 * CLASS OF CHANGE: NOT "take the address and call the same backend".  The Tensor bodies in
 * impl/basic_api/dav_<arch>/kernel_operator_determine_compute_sync_impl.h drive the workspace through
 * LocalTensor / GlobalTensor *operations*, which a raw pointer does not carry.  These overloads are
 * the same bodies re-expressed on the pointer-level primitives those operations reach:
 *
 *   Tensor operation                 pointer-level primitive used below
 *   -------------------------------  -----------------------------------------------------------
 *   ubWorkspace.GetValue(k)          CubePtrOverload::UbScalarRead(ub, k)
 *   ubWorkspace.SetValue(k, v)       CubePtrOverload::UbScalarWrite(ub, k, v)
 *   ubWorkspace[off].GetPhyAddr()    ub + off   (LocalTensor::operator[] advances the buffer
 *                                    address by `offset` elements, kernel_tensor_impl.h:776-800)
 *   gmWorkspace[off]                 gm + off   (GlobalTensor::operator[] likewise, :1348-1365)
 *   DataCopy(gm, ub, n)              DataCopyUB2GMImpl(gm, ub, params, ExtractCacheMode(gm))
 *   DataCopy(ub, gm, n)              DataCopyGM2UBImpl(ub, gm, params, ExtractCacheMode(gm))
 *                                    where `params` is built by the *same* DataCopyCheck<int32_t>(n,
 *                                    params) helper the calCount Tensor overload of DataCopy calls
 *                                    (kernel_operator_data_copy_intf_impl.h:713-734, :741-750 and
 *                                    :760-770), and ExtractCacheMode is the same function the Tensor
 *                                    DataCopy applies to its GlobalTensor operand.
 *   DuplicateImpl((__ubuf__ int32_t*)  DuplicateImpl(ub, ...) -- the Tensor bodies already pass a raw
 *       ubWorkspace.GetPhyAddr(), ...) UB pointer here, so this call is unchanged in substance.
 *   SetFlag / WaitFlag /              unchanged -- already free functions.
 *   PipeBarrier / GetBlockIdx /
 *   GetBlockNum / FetchEventID
 *
 * No AIC rule has to be reproduced here: all three Tensor bodies are wrapped in `if ASCEND_IS_AIV`,
 * so the `if ASCEND_IS_AIC { ... }` early-out that LocalTensor::GetValue / ::SetValue apply to a UB
 * operand (kernel_tensor_impl.h:605-619 and :677-682) is unreachable on every architecture, for the
 * Tensor path and for these overloads alike.
 *
 * What a raw pointer cannot carry, and therefore does not reproduce: an L2 cache hint, which the
 * Tensor path reads from GlobalTensor::address_ via ExtractCacheMode.  These overloads apply
 * ExtractCacheMode to the pointer they were given, so a caller who put a hint on the pointer keeps
 * it; a pointer obtained from GlobalTensor::GetPhyAddr() has the hint bits already masked off on
 * 3510 (kernel_tensor_impl.h:1079-1086).  The workspace Tensor form is default-constructed in
 * practice, so both paths observe cacheMode 0.
 *
 * Reachability: unlike LoadDataWithSparse, every architecture this header is compiled for provides a
 * definition of these three backends (dav_3510 and dav_c220 compute; dav_c100 / dav_m300 / dav_m310
 * / dav_l300 / dav_l311 report not-supported through ASCENDC_ASSERT / ASCENDC_REPORT_NOT_SUPPORT).
 * The pointer overloads are therefore arch-unconditional, exactly like the Tensor declarations above.
 * ************************************************************************************************* */
// Pointer counterpart of the arch-local CheckUBWorkspace.  Deliberately a distinct name rather than
// an overload: CheckUBWorkspace exists only in dav_3510 and dav_c220, and the pointer form must be
// available on every architecture on which the (not-supported) Tensor form is declared.
__aicore__ inline bool CheckUBWorkspacePtr(__ubuf__ int32_t* ubWorkspace, int64_t blockIdx, int64_t blockNum)
{
    int32_t repeatTime =
        CubePtrOverload::UbScalarRead(ubWorkspace, static_cast<uint32_t>(blockNum * B32_DATA_NUM_PER_BLOCK));
    int64_t offset = 0;
    // example: core num is n, current core id is i, current repeat time is k:
    // matched if workspace values are kkkk...k0...000: [k] * (i), [0] * (n-i)
    for (; offset < blockIdx * B32_DATA_NUM_PER_BLOCK; offset += B32_DATA_NUM_PER_BLOCK) {
        if (CubePtrOverload::UbScalarRead(ubWorkspace, static_cast<uint32_t>(offset)) != repeatTime) {
            return false;
        }
    }
    for (; offset < blockNum * B32_DATA_NUM_PER_BLOCK; offset += B32_DATA_NUM_PER_BLOCK) {
        if (CubePtrOverload::UbScalarRead(ubWorkspace, static_cast<uint32_t>(offset)) != 0) {
            return false;
        }
    }
    return true;
}

template <
    typename GmPtr, typename UbPtr, typename Std::enable_if<CubePtrOverload::AnyPtr<GmPtr, UbPtr>::value, bool>::type>
__aicore__ inline void InitDetermineComputeWorkspace(GmPtr gmWorkspace, UbPtr ubWorkspace)
{
    if ASCEND_IS_AIV {
        PipeBarrier<PIPE_ALL>();
        event_t eventID;
        auto blockNum = GetBlockNum();
        __gm__ int32_t* gm = reinterpret_cast<__gm__ int32_t*>(gmWorkspace);
        __ubuf__ int32_t* ub = reinterpret_cast<__ubuf__ int32_t*>(ubWorkspace);
        if (GetBlockIdx() == 0) {
            DuplicateImpl(ub, static_cast<int32_t>(0), B32_DATA_NUM_PER_BLOCK * blockNum);
            eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
            SetFlag<HardEvent::V_MTE3>(eventID);
            WaitFlag<HardEvent::V_MTE3>(eventID);
            DataCopyParams params;
            DataCopyCheck<int32_t>(B32_DATA_NUM_PER_BLOCK * blockNum, params);
#if (__NPU_ARCH__ == 3510)
            DataCopyUB2GMImpl(gm, ub, params, ExtractCacheMode(gm));
#else
            DataCopyUB2GMImpl(gm, ub, params);
#endif
        }
        CubePtrOverload::UbScalarWrite(
            ub, static_cast<uint32_t>(blockNum * B32_DATA_NUM_PER_BLOCK), static_cast<int32_t>(1));
        PipeBarrier<PIPE_ALL>();
    }
}

template <
    typename GmPtr, typename UbPtr, typename Std::enable_if<CubePtrOverload::AnyPtr<GmPtr, UbPtr>::value, bool>::type>
__aicore__ inline void WaitPreBlock(GmPtr gmWorkspace, UbPtr ubWorkspace)
{
    if ASCEND_IS_AIV {
        PipeBarrier<PIPE_ALL>();
        event_t eventID;
        auto blockIdx = GetBlockIdx();
        auto blockNum = GetBlockNum();
        bool matchFlag;
        __gm__ int32_t* gm = reinterpret_cast<__gm__ int32_t*>(gmWorkspace);
        __ubuf__ int32_t* ub = reinterpret_cast<__ubuf__ int32_t*>(ubWorkspace);
        DataCopyParams params;
        DataCopyCheck<int32_t>(blockNum * B32_DATA_NUM_PER_BLOCK, params);
        do {
#if (__NPU_ARCH__ == 3510)
            DataCopyGM2UBImpl(ub, gm, params, ExtractCacheMode(gm));
#else
            DataCopyGM2UBImpl(ub, gm, params);
#endif
            eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE2_S));
            SetFlag<HardEvent::MTE2_S>(eventID);
            WaitFlag<HardEvent::MTE2_S>(eventID);
            matchFlag = CheckUBWorkspacePtr(ub, blockIdx, blockNum);
            eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_MTE2));
            SetFlag<HardEvent::S_MTE2>(eventID);
            WaitFlag<HardEvent::S_MTE2>(eventID);
        } while (!matchFlag);
        PipeBarrier<PIPE_ALL>();
    }
}

template <
    typename GmPtr, typename UbPtr, typename Std::enable_if<CubePtrOverload::AnyPtr<GmPtr, UbPtr>::value, bool>::type>
__aicore__ inline void NotifyNextBlock(GmPtr gmWorkspace, UbPtr ubWorkspace)
{
    if ASCEND_IS_AIV {
        PipeBarrier<PIPE_ALL>();
        event_t eventID;
        auto blockIdx = GetBlockIdx();
        auto blockNum = GetBlockNum();
        __gm__ int32_t* gm = reinterpret_cast<__gm__ int32_t*>(gmWorkspace);
        __ubuf__ int32_t* ub = reinterpret_cast<__ubuf__ int32_t*>(ubWorkspace);
        int32_t repeatTime =
            CubePtrOverload::UbScalarRead(ub, static_cast<uint32_t>(blockNum * B32_DATA_NUM_PER_BLOCK));
        if (blockIdx + 1 == blockNum) {
            DuplicateImpl(ub, static_cast<int32_t>(0), blockNum * B32_DATA_NUM_PER_BLOCK);
            eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
            SetFlag<HardEvent::V_MTE3>(eventID);
            WaitFlag<HardEvent::V_MTE3>(eventID);
            DataCopyParams params;
            DataCopyCheck<int32_t>(blockNum * B32_DATA_NUM_PER_BLOCK, params);
#if (__NPU_ARCH__ == 3510)
            DataCopyUB2GMImpl(gm, ub, params, ExtractCacheMode(gm));
#else
            DataCopyUB2GMImpl(gm, ub, params);
#endif
        } else {
            auto offset = blockIdx * B32_DATA_NUM_PER_BLOCK;
            eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
            SetFlag<HardEvent::S_V>(eventID);
            WaitFlag<HardEvent::S_V>(eventID);
            DuplicateImpl(ub + offset, repeatTime, B32_DATA_NUM_PER_BLOCK);
            eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
            SetFlag<HardEvent::V_MTE3>(eventID);
            WaitFlag<HardEvent::V_MTE3>(eventID);
            DataCopyParams params;
            DataCopyCheck<int32_t>(B32_DATA_NUM_PER_BLOCK, params);
#if (__NPU_ARCH__ == 3510)
            DataCopyUB2GMImpl(gm + offset, ub + offset, params, ExtractCacheMode(gm + offset));
#else
            DataCopyUB2GMImpl(gm + offset, ub + offset, params);
#endif
        }
        CubePtrOverload::UbScalarWrite(ub, static_cast<uint32_t>(blockNum * B32_DATA_NUM_PER_BLOCK), repeatTime + 1);
        PipeBarrier<PIPE_ALL>();
    }
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_INTF_IMPL_H__
#endif
