/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_VECTOR_SORT_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_VECTOR_SORT_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/vector_sort_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_VECTOR_SORT_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_impl.h"

__aicore__ inline void asc_bitsort_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    if ASC_IS_AIV {
        uint64_t config = (static_cast<uint64_t>(repeat) & 0xff) << 56;
        vbs(dst, src0, src1, config);
    }
}

__aicore__ inline void asc_bitsort_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    if ASC_IS_AIV {
        uint64_t config = (static_cast<uint64_t>(repeat) & 0xff) << 56;
        vbs(dst, src0, src1, config);
    }
}

__aicore__ inline void asc_mrgsort4_impl(
    __ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    if ASC_IS_AIV {
        vmrgsort4(
            dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
            if_exhausted_suspension, valid_bit);
    }
}

__aicore__ inline void asc_mrgsort4_impl(
    __ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    if ASC_IS_AIV {
        vmrgsort4(
            dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
            if_exhausted_suspension, valid_bit);
    }
}

// ==========asc_bitsort(half/float)==========
__aicore__ inline void asc_bitsort(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_impl(dst, src0, src1, repeat);
}

__aicore__ inline void asc_bitsort(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_impl(dst, src0, src1, repeat);
}

// ==========asc_mrgsort4(half/float)==========
__aicore__ inline void asc_mrgsort4(
    __ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    asc_mrgsort4_impl(
        dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
}

__aicore__ inline void asc_mrgsort4(
    __ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    asc_mrgsort4_impl(
        dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_VECTOR_SORT_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_VECTOR_SORT_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_VECTOR_SORT_INTF_IMPL_H
