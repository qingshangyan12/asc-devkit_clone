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
 * \file kernel_operator_common_ptr_impl.h
 * \brief Pointer overloads for common CUBE-related helpers (InitOutput).
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_common_ptr_impl.h is an internal header file and must not be used directly.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_COMMON_PTR_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_COMMON_PTR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_COMMON_PTR_IMPL_H

namespace AscendC {

// NOTICE: InitOutput has been deprecated; prefer Fill. Pointer overload kept for task-book alignment.
template <typename T>
__aicore__ inline __in_pipe__(V) __out_pipe__(MTE3) void InitOutput(__gm__ T* gmWorkspaceAddr, uint32_t size,
    T value = 0)
{
#if (__NPU_ARCH__ == 2201) || (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
#if (__NPU_ARCH__ != 5102)
    if ASCEND_IS_AIC {
        return;
    }
#endif
    LocalTensor<T> popBuffer;
    bool ret = PopStackBuffer<T, TPosition::LCM>(popBuffer);
    (void)ret;
    uint32_t maxBurstSize = (MAX_REPEAT_TIMES * ONE_BLK_SIZE) / sizeof(T);
    uint32_t popSize = popBuffer.GetSize() >= maxBurstSize ? maxBurstSize : popBuffer.GetSize();
    uint32_t round = size / popSize;
    uint32_t tail = size % popSize;
    uint32_t roundSize = round != 0 ? popSize : 0;
    DuplicateImpl<T>((__ubuf__ T*)popBuffer.GetPhyAddr(), value, popSize);
    event_t eventIDVToMTE3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
    SetFlag<HardEvent::V_MTE3>(eventIDVToMTE3);
    WaitFlag<HardEvent::V_MTE3>(eventIDVToMTE3);
    struct DataCopyExtParams repeatParams;
    uint32_t comOffset = 0;
    repeatParams = {1, static_cast<uint32_t>(roundSize * sizeof(T)), 0, 0, 0};
    for (int index = 0; index < round; ++index) {
        DataCopyPadUB2GMImpl(gmWorkspaceAddr + comOffset, (__ubuf__ T*)popBuffer.GetPhyAddr(), repeatParams);
        comOffset += roundSize;
    }
    repeatParams = {1, static_cast<uint32_t>(tail * sizeof(T)), 0, 0, 0};
    if (tail != 0) {
        comOffset = round * roundSize;
        DataCopyPadUB2GMImpl(gmWorkspaceAddr + comOffset, (__ubuf__ T*)popBuffer.GetPhyAddr(), repeatParams);
    }
#else
    (void)gmWorkspaceAddr;
    (void)size;
    (void)value;
#endif
}

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_COMMON_PTR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_COMMON_PTR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_COMMON_PTR_IMPL_H__
#endif
