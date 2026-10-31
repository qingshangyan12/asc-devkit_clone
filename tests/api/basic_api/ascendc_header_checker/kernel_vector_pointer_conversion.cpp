/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// Parse every extended public header on every header-checker configuration.
// Instantiate portable operations here; device execution is covered by the
// VECTOR pointer examples in element_wise_arithmetic.
#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#endif
#include "basic_api/kernel_operator_data_copy_intf.h"
#include "basic_api/kernel_operator_vec_binary_intf.h"
#include "basic_api/kernel_operator_vec_binary_scalar_intf.h"
#include "basic_api/kernel_operator_vec_brcb_intf.h"
#include "basic_api/kernel_operator_vec_cmpsel_intf.h"
#include "basic_api/kernel_operator_vec_createvecindex_intf.h"
#include "basic_api/kernel_operator_vec_duplicate_intf.h"
#include "basic_api/kernel_operator_vec_gather_intf.h"
#include "basic_api/kernel_operator_vec_gather_mask_intf.h"
#include "basic_api/kernel_operator_vec_mulcast_intf.h"
#include "basic_api/kernel_operator_vec_reduce_intf.h"
#include "basic_api/kernel_operator_vec_scatter_intf.h"
#include "basic_api/kernel_operator_vec_ternary_scalar_intf.h"
#include "basic_api/kernel_operator_vec_transpose_intf.h"
#include "basic_api/kernel_operator_vec_vconv_intf.h"
#include "basic_api/kernel_operator_limits_intf.h"
#include "basic_api/kernel_operator_proposal_intf.h"
#include "basic_api/kernel_operator_mm_intf.h"

using namespace AscendC;

__aicore__ void CheckPointerCopy(LocalTensor<float>& dst, LocalTensor<float>& src)
{
    __ubuf__ float* dstPtr = (__ubuf__ float*)dst.GetPhyAddr();
    __ubuf__ float* srcPtr = (__ubuf__ float*)src.GetPhyAddr();
    uint64_t bits[2] = {0xFFFFFFFF, 0};
    CopyRepeatParams params{1, 1, 8, 8};
    Copy(dstPtr, src, uint64_t(32), uint8_t(1), params);
    Copy(dst, srcPtr, uint64_t(32), uint8_t(1), params);
    // Some backends only provide an unsupported-operation stub for bit-mask Copy.
    // Check its public overload here; supported-target instantiation is tested separately.
    static_assert(Std::is_same<decltype(Copy<float, false>(dstPtr, srcPtr, bits, uint8_t(1), params)), void>::value);
}

__aicore__ void CheckPointerCast(LocalTensor<half>& dst, LocalTensor<float>& src)
{
    __ubuf__ half* dstPtr = (__ubuf__ half*)dst.GetPhyAddr();
    __ubuf__ float* srcPtr = (__ubuf__ float*)src.GetPhyAddr();
    Cast(dstPtr, src, RoundMode::CAST_NONE, uint32_t(32));
    Cast(dst, srcPtr, RoundMode::CAST_NONE, uint32_t(32));
    Cast(dstPtr, srcPtr, RoundMode::CAST_NONE, uint32_t(32));
}

__aicore__ void CheckPointerDuplicate(LocalTensor<float>& dst)
{
    __ubuf__ float* dstPtr = (__ubuf__ float*)dst.GetPhyAddr();
    uint64_t bits[2] = {0xFFFFFFFF, 0};
    Duplicate(dstPtr, 1.0f, uint64_t(32), uint8_t(1), uint16_t(1), uint16_t(8));
    Duplicate<float, false>(dstPtr, 1.0f, bits, uint8_t(1), uint16_t(1), uint16_t(8));
}

__aicore__ void CheckPointerTypeDeduction(LocalTensor<float>& dst, LocalTensor<float>& src)
{
    __ubuf__ float* dstPtr = (__ubuf__ float*)dst.GetPhyAddr();
    __ubuf__ float* srcPtr = (__ubuf__ float*)src.GetPhyAddr();
    MrgSortSrcList sources(srcPtr, src, srcPtr, src);
    MrgSort4Info params;
    static_assert(Std::is_same<decltype(MrgSort(dstPtr, sources, params)), void>::value);
    static_assert(Std::is_same<decltype(MrgSort(dst, sources, params)), void>::value);
    __ubuf__ half* dataPointers[NCHW_CONV_ADDR_LIST_SIZE];
    TransDataTo5HDParams transposeParams;
    static_assert(Std::is_same<decltype(TransDataTo5HD(dataPointers, dataPointers, transposeParams)), void>::value);
    // uint64_t tables contain addresses; their type cannot identify the stored data type.
    __ubuf__ uint64_t* addresses = nullptr;
    static_assert(Std::is_same<decltype(TransDataTo5HD<half>(addresses, addresses, transposeParams)), void>::value);
}
