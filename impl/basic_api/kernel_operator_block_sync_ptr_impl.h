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
 * \file kernel_operator_block_sync_ptr_impl.h
 * \brief Pointer overloads for IBSet / IBWait / SyncAll.
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_block_sync_ptr_impl.h is an internal header file and must not be used directly.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_BLOCK_SYNC_PTR_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_BLOCK_SYNC_PTR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_BLOCK_SYNC_PTR_IMPL_H

namespace AscendC {

#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
__aicore__ inline void BlockSyncPtrCopyGm2Ub(__ubuf__ int32_t* dst, __gm__ int32_t* src)
{
    copy_gm_to_ubuf_align_v2(
        static_cast<__ubuf__ int32_t*>(dst), static_cast<__gm__ int32_t*>(src), 0, 1, 32, 0, 0, false, 0, 32, 32);
}

__aicore__ inline void BlockSyncPtrCopyUb2Gm(__gm__ int32_t* dst, __ubuf__ int32_t* src)
{
    copy_ubuf_to_gm_align_v2(
        static_cast<__gm__ int32_t*>(dst), static_cast<__ubuf__ int32_t*>(src), 0, 1, 32, 0, 32, 32);
}
#else
__aicore__ inline void BlockSyncPtrCopyGm2Ub(__ubuf__ int32_t* dst, __gm__ int32_t* src)
{
    copy_gm_to_ubuf(static_cast<__ubuf__ void*>(dst), static_cast<__gm__ void*>(src), 0, 1, 1, 0, 0);
}

__aicore__ inline void BlockSyncPtrCopyUb2Gm(__gm__ int32_t* dst, __ubuf__ int32_t* src)
{
    copy_ubuf_to_gm(static_cast<__gm__ void*>(dst), static_cast<__ubuf__ void*>(src), 0, 1, 1, 0, 0);
}
#endif

template <bool isAIVOnly = true>
__aicore__ inline void SyncAll(
    __gm__ int32_t* gmWorkspace, __ubuf__ int32_t* ubWorkspace, const int32_t usedCores = 0)
{
#if ASCENDC_CPU_DEBUG
    SoftSyncAllImpl<false>(gmWorkspace, ubWorkspace, usedCores);
#else
    SoftSyncAllImpl<isAIVOnly>(gmWorkspace, ubWorkspace, usedCores);
#endif
}

template <bool isAIVOnly = true>
__aicore__ inline void IBSet(
    __gm__ int32_t* gmWorkspace, __ubuf__ int32_t* ubWorkspace, int32_t blockIdx, int32_t eventID)
{
    int32_t blockNum = GetBlockNum();
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
#if (__NPU_ARCH__ != 5102)
    if ASCEND_IS_AIC {
        return;
    }
#endif
    if (!isAIVOnly) {
        blockNum = GetBlockNum() * 2;
    }
#endif
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002
    __ib_set_stub(blockIdx, eventID, isAIVOnly);
#endif
    __gm__ int32_t* localSyncGM = gmWorkspace + (blockNum * 8 * eventID + blockIdx * 8);
    pipe_barrier(PIPE_ALL);

    while (true) {
        BlockSyncPtrCopyGm2Ub(ubWorkspace, localSyncGM);
        event_t eventIdMte2ToS = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE2_S));
        SetFlag<HardEvent::MTE2_S>(eventIdMte2ToS);
        WaitFlag<HardEvent::MTE2_S>(eventIdMte2ToS);
        if (*ubWorkspace == 0) {
            *ubWorkspace = 1;
            event_t eventIdSToMte3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_MTE3));
            SetFlag<HardEvent::S_MTE3>(eventIdSToMte3);
            WaitFlag<HardEvent::S_MTE3>(eventIdSToMte3);
            BlockSyncPtrCopyUb2Gm(localSyncGM, ubWorkspace);
            break;
        }
    }
    pipe_barrier(PIPE_ALL);
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002
    __ib_set_stub(blockIdx, eventID, isAIVOnly);
#endif
}

template <bool isAIVOnly = true>
__aicore__ inline void IBWait(
    __gm__ int32_t* gmWorkspace, __ubuf__ int32_t* ubWorkspace, int32_t blockIdx, int32_t eventID)
{
    int32_t blockNum = GetBlockNum();
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
#if (__NPU_ARCH__ != 5102)
    if ASCEND_IS_AIC {
        return;
    }
#endif
    if (!isAIVOnly) {
        blockNum = GetBlockNum() * 2;
    }
#endif
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002
    __ib_wait_stub(blockIdx, eventID, isAIVOnly);
#endif
    __gm__ int32_t* localSyncGM = gmWorkspace + (blockNum * 8 * eventID + blockIdx * 8);
    pipe_barrier(PIPE_ALL);

    while (true) {
        BlockSyncPtrCopyGm2Ub(ubWorkspace, localSyncGM);
        event_t eventIdMte2ToS = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE2_S));
        SetFlag<HardEvent::MTE2_S>(eventIdMte2ToS);
        WaitFlag<HardEvent::MTE2_S>(eventIdMte2ToS);
        if (*ubWorkspace == 1) {
            *ubWorkspace = 0;
            event_t eventIdSToMte3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_MTE3));
            SetFlag<HardEvent::S_MTE3>(eventIdSToMte3);
            WaitFlag<HardEvent::S_MTE3>(eventIdSToMte3);
            BlockSyncPtrCopyUb2Gm(localSyncGM, ubWorkspace);
            break;
        }
    }
    pipe_barrier(PIPE_ALL);
#if __NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002
    __ib_wait_stub(blockIdx, eventID, isAIVOnly);
#endif
}

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_BLOCK_SYNC_PTR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_BLOCK_SYNC_PTR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_BLOCK_SYNC_PTR_IMPL_H__
#endif
