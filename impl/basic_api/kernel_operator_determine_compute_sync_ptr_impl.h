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
 * \file kernel_operator_determine_compute_sync_ptr_impl.h
 * \brief Pointer overloads for InitDetermineComputeWorkspace / WaitPreBlock / NotifyNextBlock.
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_determine_compute_sync_ptr_impl.h is an internal header file and must not be used directly.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_PTR_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_DETERMINE_COMPUTE_SYNC_PTR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_DETERMINE_COMPUTE_SYNC_PTR_IMPL_H

namespace AscendC {

namespace CubePtrDetail {
__aicore__ inline void BindGmUb(
    GlobalTensor<int32_t>& gmTensor, LocalTensor<int32_t>& ubTensor, __gm__ int32_t* gmWorkspace,
    __ubuf__ int32_t* ubWorkspace)
{
    gmTensor.SetGlobalBuffer(gmWorkspace);
    // On device, LocalTensor::bufferAddr is used as the absolute __ubuf__ address.
    const uint32_t ubElems = static_cast<uint32_t>((GetBlockNum() + 1) * B32_DATA_NUM_PER_BLOCK);
    ubTensor = LocalTensor<int32_t>(TPosition::VECCALC, static_cast<uint32_t>(reinterpret_cast<uintptr_t>(ubWorkspace)),
        ubElems);
}
} // namespace CubePtrDetail

__aicore__ inline void InitDetermineComputeWorkspace(__gm__ int32_t* gmWorkspace, __ubuf__ int32_t* ubWorkspace)
{
    GlobalTensor<int32_t> gmTensor;
    LocalTensor<int32_t> ubTensor;
    CubePtrDetail::BindGmUb(gmTensor, ubTensor, gmWorkspace, ubWorkspace);
    InitDetermineComputeWorkspaceCalc(gmTensor, ubTensor);
}

__aicore__ inline void WaitPreBlock(__gm__ int32_t* gmWorkspace, __ubuf__ int32_t* ubWorkspace)
{
    GlobalTensor<int32_t> gmTensor;
    LocalTensor<int32_t> ubTensor;
    CubePtrDetail::BindGmUb(gmTensor, ubTensor, gmWorkspace, ubWorkspace);
    WaitPreBlockCalc(gmTensor, ubTensor);
}

__aicore__ inline void NotifyNextBlock(__gm__ int32_t* gmWorkspace, __ubuf__ int32_t* ubWorkspace)
{
    GlobalTensor<int32_t> gmTensor;
    LocalTensor<int32_t> ubTensor;
    CubePtrDetail::BindGmUb(gmTensor, ubTensor, gmWorkspace, ubWorkspace);
    NotifyNextBlockCalc(gmTensor, ubTensor);
}

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_DETERMINE_COMPUTE_SYNC_PTR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_PTR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DETERMINE_COMPUTE_SYNC_PTR_IMPL_H__
#endif
