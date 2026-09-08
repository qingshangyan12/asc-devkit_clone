/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_BROADCAST_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_BROADCAST_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/memory_base_impl/vector_broadcast_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_BROADCAST_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/utils_impl.h"

__aicore__ inline void asc_duplicate_impl(
    __ubuf__ half* dst, half src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, 1, dst_repeat_stride, 0);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ half* dst, half src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_impl(
    __ubuf__ int16_t* dst, int16_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, 1, dst_repeat_stride, 0);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ int16_t* dst, int16_t src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_impl(
    __ubuf__ uint16_t* dst, uint16_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, 1, dst_repeat_stride, 0);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_impl(
    __ubuf__ bfloat16_t* dst, bfloat16_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, 1, dst_repeat_stride, 0);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_impl(
    __ubuf__ float* dst, float src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, 1, dst_repeat_stride, 0);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ float* dst, float src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_impl(
    __ubuf__ int32_t* dst, int32_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, 1, dst_repeat_stride, 0);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ int32_t* dst, int32_t src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_duplicate_impl(
    __ubuf__ uint32_t* dst, uint32_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    if ASC_IS_AIV {
        vector_dup(dst, src, repeat, dst_block_stride, 1, dst_repeat_stride, 0);
    }
}

__aicore__ inline void asc_duplicate_impl(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_duplicate_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_brcb_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    if ASC_IS_AIV {
        vbrcb(dst, src, dst_block_stride, dst_repeat_stride, repeat);
    }
}

__aicore__ inline void asc_brcb_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    if ASC_IS_AIV {
        vbrcb(dst, src, dst_block_stride, dst_repeat_stride, repeat);
    }
}

// ==========asc_duplicate(half/int16_t/uint16_t/bfloat16_t/float/int32_t/uint32_t)==========
__aicore__ inline void asc_duplicate(
    __ubuf__ half* dst, half src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, dst_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(
    __ubuf__ int16_t* dst, int16_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, dst_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(
    __ubuf__ uint16_t* dst, uint16_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, dst_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(
    __ubuf__ bfloat16_t* dst, bfloat16_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, dst_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(
    __ubuf__ float* dst, float src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, dst_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(
    __ubuf__ int32_t* dst, int32_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, dst_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

__aicore__ inline void asc_duplicate(
    __ubuf__ uint32_t* dst, uint32_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride)
{
    asc_duplicate_impl(dst, src, repeat, dst_block_stride, dst_repeat_stride);
}

__aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count)
{
    asc_duplicate_impl(dst, src, count);
}

// ==========asc_brcb(uint16_t/uint32_t)==========
__aicore__ inline void asc_brcb(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    asc_brcb_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_brcb(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    asc_brcb_impl(dst, src, dst_block_stride, dst_repeat_stride, repeat);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_BROADCAST_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_BROADCAST_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_BROADCAST_INTF_IMPL_H
