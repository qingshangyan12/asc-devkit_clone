/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_COMPUTE_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_COMPUTE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/memory_base_impl/vector_compute_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_COMPUTE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/utils_intf_impl.h"

__aicore__ inline void asc_max_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmax(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_max_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmax(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_max_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmax(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_max_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmax(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_max_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_max_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_max_impl(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_max_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_max_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_max_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_max_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_max_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmul(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_mul_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmul(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_mul_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmul(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_mul_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmul(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_mul_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_mul_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_impl(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_mul_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_mul_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_mul_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_add_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vadd(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_add_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vadd(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_add_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vadd(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_add_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vadd(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_add_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_add_impl(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_add_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_add_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsub(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_sub_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsub(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_sub_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsub(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_sub_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsub(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_sub_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_impl(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_min_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmin(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_min_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmin(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_min_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmin(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_min_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmin(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_min_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_min_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_min_impl(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_min_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_min_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_min_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_min_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_min_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_div_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vdiv(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_div_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vdiv(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_div_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_div_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_div_impl(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_div_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_get_reduce_max_cnt_impl(half& val, uint32_t& index)
{
    if ASC_IS_AIV {
        constexpr uint8_t offset = 32;
        int64_t max_min_cnt = get_max_min_cnt();
        union {
            half h;
            uint16_t u;
        } u162half = {.u = static_cast<uint16_t>(0xffff & max_min_cnt)};
        val = u162half.h;
        index = 0xffffffff & (max_min_cnt >> offset);
    }
}

__aicore__ inline void asc_get_reduce_max_cnt_impl(float& val, uint32_t& index)
{
    if ASC_IS_AIV {
        constexpr uint8_t offset = 32;
        int64_t max_min_cnt = get_max_min_cnt();
        union {
            float f;
            uint32_t u;
        } u322float = {.u = static_cast<uint32_t>(0xffffffff & max_min_cnt)};
        val = u322float.f;
        index = 0xffffffff & (max_min_cnt >> offset);
    }
}

__aicore__ inline void asc_get_reduce_min_cnt_impl(half& val, uint32_t& index)
{
    if ASC_IS_AIV {
        constexpr uint8_t offset = 32;
        int64_t max_min_cnt = get_max_min_cnt();
        union {
            half h;
            uint16_t u;
        } u162half = {.u = static_cast<uint16_t>(0xffff & max_min_cnt)};
        val = u162half.h;
        index = 0xffffffff & (max_min_cnt >> offset);
    }
}

__aicore__ inline void asc_get_reduce_min_cnt_impl(float& val, uint32_t& index)
{
    if ASC_IS_AIV {
        constexpr uint8_t offset = 32;
        int64_t max_min_cnt = get_max_min_cnt();
        union {
            float f;
            uint32_t u;
        } u322float = {.u = static_cast<uint32_t>(0xffffffff & max_min_cnt)};
        val = u322float.f;
        index = 0xffffffff & (max_min_cnt >> offset);
    }
}

__aicore__ inline void asc_datablock_reduce_sum_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcgadd(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_datablock_reduce_sum_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_sum_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_datablock_reduce_sum_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcgadd(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_datablock_reduce_sum_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_sum_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_datablock_reduce_max_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcgmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_datablock_reduce_max_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_max_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_datablock_reduce_max_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcgmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_datablock_reduce_max_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_max_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_datablock_reduce_min_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcgmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_datablock_reduce_min_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_min_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_datablock_reduce_min_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcgmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_datablock_reduce_min_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_datablock_reduce_min_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_sum_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcadd(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, 0);
    }
}

__aicore__ inline void asc_repeat_reduce_sum_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_sum_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_sum_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcadd(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, 0);
    }
}

__aicore__ inline void asc_repeat_reduce_sum_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_sum_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_index_value_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::INDEX_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_max_value_index_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::VALUE_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_max_only_value_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_max_only_index_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_max_index_value_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_index_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_value_index_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_value_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_only_value_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_only_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_only_index_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_only_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_index_value_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::INDEX_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_max_value_index_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::VALUE_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_max_only_value_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_max_only_index_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmax(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_max_index_value_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_index_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_value_index_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_value_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_only_value_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_only_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_max_only_index_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_max_only_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_index_value_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::INDEX_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_min_value_index_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::VALUE_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_min_only_value_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_min_only_index_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_min_index_value_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_index_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_value_index_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_value_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_only_value_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_only_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_only_index_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_only_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_index_value_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::INDEX_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_min_value_index_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::VALUE_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_min_only_value_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_VALUE);
    }
}

__aicore__ inline void asc_repeat_reduce_min_only_index_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmin(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride, Order_t::ONLY_INDEX);
    }
}

__aicore__ inline void asc_repeat_reduce_min_index_value_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_index_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_value_index_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_value_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_only_value_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_only_value_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_repeat_reduce_min_only_index_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_repeat_reduce_min_only_index_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_REDUCE_DEFAULT_REPEAT_STRIDE,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline int64_t asc_get_acc_val_impl()
{
    if ASC_IS_AIV {
        return get_acc_val();
    }
    return 0;
}

__aicore__ inline void asc_max_scalar_impl(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vmaxs(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_max_scalar_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_max_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_max_scalar_impl(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vmaxs(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_max_scalar_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_max_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_max_scalar_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vmaxs(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_max_scalar_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_max_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_max_scalar_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vmaxs(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_max_scalar_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_max_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_scalar_impl(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vmuls(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_mul_scalar_impl(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vmuls(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_mul_scalar_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vmuls(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_mul_scalar_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vmuls(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_mul_scalar_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_mul_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_scalar_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_mul_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_scalar_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_mul_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_scalar_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_mul_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_add_scalar_impl(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vadds(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_add_scalar_impl(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vadds(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_add_scalar_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vadds(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_add_scalar_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vadds(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_add_scalar_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_add_scalar_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_add_scalar_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_add_scalar_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_scalar_impl(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    float tmpValue = static_cast<float>(value);
    tmpValue = 0 - tmpValue;
    asc_add_scalar_impl(
        dst, src, static_cast<half>(tmpValue), repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar_impl(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    float tmpValue = static_cast<float>(value);
    tmpValue = 0 - tmpValue;
    asc_add_scalar_impl(
        dst, src, static_cast<float>(tmpValue), repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    float tmpValue = static_cast<float>(value);
    tmpValue = 0 - tmpValue;
    asc_add_scalar_impl(
        dst, src, static_cast<int16_t>(tmpValue), repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    float tmpValue = static_cast<float>(value);
    tmpValue = 0 - tmpValue;
    asc_add_scalar_impl(
        dst, src, static_cast<int32_t>(tmpValue), repeat, dst_block_stride, src_block_stride, dst_repeat_stride,
        src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_scalar_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_scalar_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_scalar_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_min_scalar half
__aicore__ inline void asc_min_scalar_impl(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vmins(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_min_scalar_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_min_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_min_scalar float
__aicore__ inline void asc_min_scalar_impl(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vmins(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_min_scalar_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_min_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_min_scalar int16_t
__aicore__ inline void asc_min_scalar_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vmins(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_min_scalar_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_min_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_min_scalar int32_t
__aicore__ inline void asc_min_scalar_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vmins(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_min_scalar_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_min_scalar_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_leakyrelu_impl(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vlrelu(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_leakyrelu_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_leakyrelu_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    set_mask_norm();
    asc_set_mask_count_end();
}

__aicore__ inline void asc_leakyrelu_impl(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vlrelu(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_leakyrelu_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_leakyrelu_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    set_mask_norm();
    asc_set_mask_count_end();
}

__aicore__ inline void asc_exp_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vexp(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_exp_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vexp(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_exp_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_exp_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_exp_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_exp_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_log_impl_half(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vln(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_log_count_impl_half(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_log_impl_half(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_log_sync_impl_half(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_log_count_impl_half(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_log_impl_float(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vln(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_log_count_impl_float(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_log_impl_float(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_log_sync_impl_float(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_log_count_impl_float(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_abs_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vabs(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_abs_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_abs_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_abs_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vabs(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_abs_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_abs_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_rcp half
__aicore__ inline void asc_rcp_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vrec(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_rcp_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_rcp_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_rcp float
__aicore__ inline void asc_rcp_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vrec(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_rcp_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_rcp_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_sqrt_impl half
__aicore__ inline void asc_sqrt_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vsqrt(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_sqrt_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sqrt_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_sqrt_impl float
__aicore__ inline void asc_sqrt_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vsqrt(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_sqrt_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sqrt_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_rsqrt half
__aicore__ inline void asc_rsqrt_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vrsqrt(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_rsqrt_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_rsqrt_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_rsqrt float
__aicore__ inline void asc_rsqrt_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vrsqrt(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_rsqrt_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_rsqrt_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_relu_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vrelu(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_relu_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_relu_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_relu_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vrelu(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_relu_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_relu_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_relu_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vrelu(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_relu_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_relu_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_not_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vnot(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_not_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_not_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_not_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vnot(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_not_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_not_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_and_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vand(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_and_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_and_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_and_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vand(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_and_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_and_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_or_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vor(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_or_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_or_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_or_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vor(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_or_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_or_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_shiftleft_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshl(dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_shiftleft_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shiftleft_impl(
        dst, src, distance, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_shiftleft_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshl(dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_shiftleft_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shiftleft_impl(
        dst, src, distance, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_shiftleft_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshl(dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_shiftleft_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shiftleft_impl(
        dst, src, distance, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_shiftleft_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshl(dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_shiftleft_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shiftleft_impl(
        dst, src, distance, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_shiftright int16_t
__aicore__ inline void asc_shiftright_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshr(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride, false);
    }
}

__aicore__ inline void asc_shiftright_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shiftright_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_shiftright int32_t
__aicore__ inline void asc_shiftright_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshr(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride, false);
    }
}

__aicore__ inline void asc_shiftright_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shiftright_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_shiftright uint16_t
__aicore__ inline void asc_shiftright_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshr(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride, false);
    }
}

__aicore__ inline void asc_shiftright_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shiftright_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_shiftright uint32_t
__aicore__ inline void asc_shiftright_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshr(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride, false);
    }
}

__aicore__ inline void asc_shiftright_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shiftright_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_shiftright_round int16_t
__aicore__ inline void asc_shiftright_round_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshr(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride, true);
    }
}

__aicore__ inline void asc_shiftright_round_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shiftright_round_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_shiftright_round int32_t
__aicore__ inline void asc_shiftright_round_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vshr(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride, true);
    }
}

__aicore__ inline void asc_shiftright_round_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_shiftright_round_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline uint64_t asc_make_deq_scale_config(float scale, int16_t offset, bool sign_mode)
{
    constexpr uint64_t sign_mode_bit = 46;
    constexpr uint64_t offset_mask = 0x1ff;
    constexpr uint64_t offset_bit = 37;
    uint64_t cfg =
        ((static_cast<uint64_t>(sign_mode) << sign_mode_bit) | ((offset & offset_mask) << offset_bit) |
         *(reinterpret_cast<uint32_t*>(&scale)));
    return cfg;
}

__aicore__ inline void asc_set_deq_scale_impl(
    __ubuf__ uint64_t* tmp, float scale_arr[ASC_VDEQ_SIZE], int16_t offset_arr[ASC_VDEQ_SIZE],
    bool sign_mode_arr[ASC_VDEQ_SIZE])
{
    if ASC_IS_AIV {
        for (uint8_t i = 0; i < ASC_VDEQ_SIZE; i++) {
            float scale = scale_arr[i];
            int16_t offset = offset_arr[i];
            bool sign_mode = sign_mode_arr[i];
            uint64_t cfg = asc_make_deq_scale_config(scale, offset, sign_mode);
            tmp[i] = cfg;
        }
        constexpr uint64_t deq_addr = 5;
        set_deqscale(((uint64_t)tmp) >> deq_addr);
    }
}

__aicore__ inline void asc_set_deq_scale_impl(float scale, int16_t offset, bool sign_mode)
{
    if ASC_IS_AIV {
        set_deqscale(asc_make_deq_scale_config(scale, offset, sign_mode));
    }
}

__aicore__ inline void asc_set_deq_scale_impl(half scale)
{
    if ASC_IS_AIV {
        set_deqscale(scale);
    }
}

__aicore__ inline void asc_set_mask_count_impl()
{
    if ASC_IS_AIV {
        set_mask_count();
    }
}

__aicore__ inline void asc_set_mask_norm_impl()
{
    if ASC_IS_AIV {
        set_mask_norm();
    }
}

__aicore__ inline void asc_set_vector_mask_impl(uint64_t mask_high, uint64_t mask_low)
{
    if ASC_IS_AIV {
        set_vector_mask(mask_high, mask_low);
    }
}

__aicore__ inline void asc_lt_impl(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmp_lt(
            src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_lt_impl(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmp_lt(
            src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_lt_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_lt(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_lt_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_lt(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_gt_impl(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmp_gt(
            src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_gt_impl(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmp_gt(
            src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_gt_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_gt(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_gt_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_gt(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

// asc_ge(vcmp) half
__aicore__ inline void asc_ge_impl(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmp_ge(
            src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

// asc_ge(vcmp) float
__aicore__ inline void asc_ge_impl(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmp_ge(
            src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

// asc_ge(vcmpv) half
__aicore__ inline void asc_ge_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_ge(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

// asc_ge(vcmpv) float
__aicore__ inline void asc_ge_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_ge(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_eq_impl(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmp_eq(
            src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_eq_impl(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmp_eq(
            src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_eq_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_eq(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_eq_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_eq(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_eq_impl(
    __ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_eq(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

// vcmp_ne(half, half)
__aicore__ inline void asc_ne_impl(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmp_ne(
            src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

// vcmp_ne(float, float)
__aicore__ inline void asc_ne_impl(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmp_ne(
            src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

// vcmpv_ne(uint8, half, half)
__aicore__ inline void asc_ne_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_ne(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

// vcmpv_ne(uint8, float, float)
__aicore__ inline void asc_ne_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_ne(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_le_impl(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmp_le(
            src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_le_impl(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmp_le(
            src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_le_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_le(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_le_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpv_le(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_lt_scalar_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_lt(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_lt_scalar_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_lt(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_gt_scalar_impl_half(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_gt(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_gt_scalar_sync_impl_half(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_gt_scalar_impl_half(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_gt_scalar_impl_float(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_gt(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_gt_scalar_sync_impl_float(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_gt_scalar_impl_float(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_ge_scalar_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_ge(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_ge_scalar_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_ge(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

// asc_eq_scalar half
__aicore__ inline void asc_eq_scalar_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_eq(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

// asc_eq_scalar float
__aicore__ inline void asc_eq_scalar_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_eq(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

// asc_eq_scalar int32_t
__aicore__ inline void asc_eq_scalar_impl(
    __ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_eq(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_ne_scalar_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_ne(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_ne_scalar_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_ne(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_le_scalar_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_le(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_le_scalar_impl(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcmpvs_le(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_get_cmp_mask_impl(__ubuf__ void* dst)
{
    if ASC_IS_AIV {
        get_cmpmask(dst);
    }
}

__aicore__ inline void asc_set_cmp_mask_impl(__ubuf__ void* sel_mask)
{
    if ASC_IS_AIV {
        set_cmpmask(sel_mask);
    }
}

// asc_select half
__aicore__ inline void asc_select_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsel(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_select_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_select_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

// asc_select float
__aicore__ inline void asc_select_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsel(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_select_impl(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_select_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

// asc_reduce uint16_t
__aicore__ inline void asc_reduce_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint16_t repeat,
    uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vreducev2(dst, src0, src1, repeat, src0_block_stride, pattern_mode, src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_reduce_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_reduce_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, 0,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_reduce uint32_t
__aicore__ inline void asc_reduce_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint16_t repeat,
    uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vreducev2(dst, src0, src1, repeat, src0_block_stride, pattern_mode, src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_reduce_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_reduce_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, 0,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_add_relu_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vaddreluconv_f162s8(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride, false);
    }
}

__aicore__ inline void asc_add_relu_impl(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_add_relu_impl(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vaddreluconv_s162s8(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride, false);
    }
}

__aicore__ inline void asc_add_relu_impl(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_add_relu_impl(
    __ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vaddreluconv_f322f16(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride, false);
    }
}

__aicore__ inline void asc_add_relu_impl(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

// asc_add_relu half
__aicore__ inline void asc_add_relu_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vaddrelu(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_add_relu_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

// asc_add_relu float
__aicore__ inline void asc_add_relu_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vaddrelu(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_add_relu_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

// asc_add_relu int16_t
__aicore__ inline void asc_add_relu_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vaddrelu(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_add_relu_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_add_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

// asc_axpy_impl half half
__aicore__ inline void asc_axpy_impl(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vaxpy(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_axpy_impl(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_axpy_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_axpy_impl float float
__aicore__ inline void asc_axpy_impl(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vaxpy(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_axpy_impl(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_axpy_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_axpy_impl float half
__aicore__ inline void asc_axpy_impl(
    __ubuf__ float* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vaxpy(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_axpy_impl(__ubuf__ float* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_axpy_impl(
        dst, src, value, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_bitsort_impl_half(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    if ASC_IS_AIV {
        vbitsort(dst, src0, src1, repeat);
    }
}

__aicore__ inline void asc_bitsort_sync_impl_half(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_impl_half(dst, src0, src1, repeat);
    asc_sync_post_process();
}

__aicore__ inline void asc_bitsort_impl_float(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    if ASC_IS_AIV {
        vbitsort(dst, src0, src1, repeat);
    }
}

__aicore__ inline void asc_bitsort_sync_impl_float(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_impl_float(dst, src0, src1, repeat);
    asc_sync_post_process();
}

__aicore__ inline void asc_fma_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmla(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_fma_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_fma_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_fma_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmla(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_fma_impl(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_fma_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_fma_impl(
    __ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmla(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_fma_impl(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_fma_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_gather_datablock_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t dst_block_stride, uint8_t repeat)
{
    if ASC_IS_AIV {
        vgatherb(dst, src_offset, (uint64_t)src, dst_repeat_stride, dst_block_stride, repeat);
    }
}

__aicore__ inline void asc_gather_datablock_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t dst_block_stride, uint8_t repeat)
{
    if ASC_IS_AIV {
        vgatherb(dst, src_offset, (uint64_t)src, dst_repeat_stride, dst_block_stride, repeat);
    }
}

// asc_gather int16_t
__aicore__ inline void asc_gather_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    if ASC_IS_AIV {
        vgather((__ubuf__ uint16_t*)dst, src_offset, (uint64_t)src, dst_repeat_stride, repeat);
    }
}

__aicore__ inline void asc_gather_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_gather_impl(dst, src, src_offset, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT.U8);
    asc_set_mask_count_end();
}

// asc_gather uint16_t
__aicore__ inline void asc_gather_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    if ASC_IS_AIV {
        vgather(dst, src_offset, (uint64_t)src, dst_repeat_stride, repeat);
    }
}

__aicore__ inline void asc_gather_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_gather_impl(dst, src, src_offset, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT.U8);
    asc_set_mask_count_end();
}

// asc_gather half
__aicore__ inline void asc_gather_impl(
    __ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride, uint8_t repeat)
{
    if ASC_IS_AIV {
        vgather((__ubuf__ uint16_t*)dst, src_offset, (uint64_t)src, dst_repeat_stride, repeat);
    }
}

__aicore__ inline void asc_gather_impl(
    __ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_gather_impl(dst, src, src_offset, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT.U8);
    asc_set_mask_count_end();
}

// asc_gather bfloat16_t
__aicore__ inline void asc_gather_impl(
    __ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    if ASC_IS_AIV {
        vgather((__ubuf__ uint16_t*)dst, src_offset, (uint64_t)src, dst_repeat_stride, repeat);
    }
}

__aicore__ inline void asc_gather_impl(
    __ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_gather_impl(dst, src, src_offset, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT.U8);
    asc_set_mask_count_end();
}

// asc_gather int32_t
__aicore__ inline void asc_gather_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    if ASC_IS_AIV {
        vgather((__ubuf__ uint32_t*)dst, src_offset, (uint64_t)src, dst_repeat_stride, repeat);
    }
}

__aicore__ inline void asc_gather_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_gather_impl(dst, src, src_offset, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT.U8);
    asc_set_mask_count_end();
}

// asc_gather uint32_t
__aicore__ inline void asc_gather_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    if ASC_IS_AIV {
        vgather(dst, src_offset, (uint64_t)src, dst_repeat_stride, repeat);
    }
}

__aicore__ inline void asc_gather_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_gather_impl(dst, src, src_offset, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT.U8);
    asc_set_mask_count_end();
}

// asc_gather float
__aicore__ inline void asc_gather_impl(
    __ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride, uint8_t repeat)
{
    if ASC_IS_AIV {
        vgather((__ubuf__ uint32_t*)dst, src_offset, (uint64_t)src, dst_repeat_stride, repeat);
    }
}

__aicore__ inline void asc_gather_impl(
    __ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_gather_impl(dst, src, src_offset, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_get_vms4_sr_impl(uint16_t sorted_num[ASC_C_API_MRGSORT_ELEMENT_LEN])
{
    if ASC_IS_AIV {
        *(reinterpret_cast<uint64_t*>(sorted_num)) = (uint64_t)get_vms4_sr();
    }
}

// repeat == 1, no limit
// repeat > 1, limit: len0==len1==len2==len3, valid_bit==0xb1111, if_exhausted_suspension==false
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

// asc_mul_add half
__aicore__ inline void asc_mul_add_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmadd(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_mul_add_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_mul_add_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

// asc_mul_add float
__aicore__ inline void asc_mul_add_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmadd(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_mul_add_impl(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_mul_add_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_add_relu_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmaddrelu(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_mul_add_relu_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_mul_add_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_add_relu_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmaddrelu(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_mul_add_relu_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_mul_add_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_cast_half2int8_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmulconv_f162s8(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_mul_cast_half2int8_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    constexpr uint8_t dst_repeat_stride =
        static_cast<uint8_t>(ASC_C_API_DEFAULT_REPEAT_STRIDE.U8 / (sizeof(half) / sizeof(int8_t)));
    asc_mul_cast_half2int8_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, dst_repeat_stride,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_mul_cast_half2uint8_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vmulconv_f162u8(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_mul_cast_half2uint8_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    constexpr uint8_t dst_repeat_stride =
        static_cast<uint8_t>(ASC_C_API_DEFAULT_REPEAT_STRIDE.U8 / (sizeof(half) / sizeof(uint8_t)));
    asc_mul_cast_half2uint8_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, dst_repeat_stride,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_relu_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsubreluconv_f162s8(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride, false);
    }
}

__aicore__ inline void asc_sub_relu_impl(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_relu_impl(
    __ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsubreluconv_f322f16(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride, false);
    }
}

__aicore__ inline void asc_sub_relu_impl(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_relu_impl(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsubreluconv_s162s8(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride, false);
    }
}

__aicore__ inline void asc_sub_relu_impl(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_relu_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsubrelu(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_sub_relu_impl(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_relu_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsubrelu(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_sub_relu_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_sub_relu_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vsubrelu(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_sub_relu_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_sub_relu_impl(
        dst, src0, src1, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8,
        ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2int8_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s8(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int8_impl(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int8_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2int8_rna_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s8a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int8_rna_impl(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int8_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2int8_ru_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s8c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int8_ru_impl(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int8_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2int8_rd_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s8f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int8_rd_impl(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int8_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2int8_rn_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s8r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int8_rn_impl(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int8_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2int8_rz_impl(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s8z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int8_rz_impl(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int8_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int642float_rd_impl(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s642f32f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int642float_rd_impl(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int642float_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int642float_rn_impl(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s642f32r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int642float_rn_impl(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int642float_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int642float_rna_impl(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s642f32a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int642float_rna_impl(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int642float_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int642float_ru_impl(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s642f32c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int642float_ru_impl(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int642float_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int642float_rz_impl(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s642f32z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int642float_rz_impl(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int642float_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_bfloat162float_impl(
    __ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_bf162f32(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_bfloat162float_impl(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_bfloat162float_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_bfloat162int32_rna
__aicore__ inline void asc_bfloat162int32_rna_impl(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_bf162s32a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_bfloat162int32_rna_impl(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_bfloat162int32_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_bfloat162int32_ru
__aicore__ inline void asc_bfloat162int32_ru_impl(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_bf162s32c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_bfloat162int32_ru_impl(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_bfloat162int32_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_bfloat162int32_rd
__aicore__ inline void asc_bfloat162int32_rd_impl(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_bf162s32f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_bfloat162int32_rd_impl(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_bfloat162int32_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_bfloat162int32_rn
__aicore__ inline void asc_bfloat162int32_rn_impl(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_bf162s32r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_bfloat162int32_rn_impl(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_bfloat162int32_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_bfloat162int32_rz
__aicore__ inline void asc_bfloat162int32_rz_impl(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_bf162s32z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_bfloat162int32_rz_impl(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_bfloat162int32_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2float_impl(
    __ubuf__ float* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162f32(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2float_impl(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2float_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int16_rna
__aicore__ inline void asc_half2int16_rna_impl(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s16a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int16_rna_impl(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int16_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int16_ru
__aicore__ inline void asc_half2int16_ru_impl(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s16c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int16_ru_impl(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int16_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int16_rd
__aicore__ inline void asc_half2int16_rd_impl(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s16f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int16_rd_impl(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int16_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int16_rn
__aicore__ inline void asc_half2int16_rn_impl(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s16r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int16_rn_impl(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int16_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int16_rz
__aicore__ inline void asc_half2int16_rz_impl(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s16z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int16_rz_impl(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int16_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int32_rna
__aicore__ inline void asc_half2int32_rna_impl(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s32a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int32_rna_impl(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int32_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int32_ru
__aicore__ inline void asc_half2int32_ru_impl(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s32c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int32_ru_impl(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int32_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int32_rd
__aicore__ inline void asc_half2int32_rd_impl(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s32f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int32_rd_impl(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int32_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int32_rn
__aicore__ inline void asc_half2int32_rn_impl(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s32r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int32_rn_impl(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int32_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int32_rz
__aicore__ inline void asc_half2int32_rz_impl(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s32z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int32_rz_impl(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int32_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int4
__aicore__ inline void asc_half2int4_impl(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s4(
            (__ubuf__ void*)dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int4_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int4_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_ONE_FOURTH_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int4_rna
__aicore__ inline void asc_half2int4_rna_impl(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s4a(
            (__ubuf__ void*)dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int4_rna_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int4_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_ONE_FOURTH_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int4_ru
__aicore__ inline void asc_half2int4_ru_impl(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s4c(
            (__ubuf__ void*)dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int4_ru_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int4_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_ONE_FOURTH_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int4_rd
__aicore__ inline void asc_half2int4_rd_impl(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s4f(
            (__ubuf__ void*)dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int4_rd_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int4_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_ONE_FOURTH_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int4_r
__aicore__ inline void asc_half2int4_rn_impl(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s4r(
            (__ubuf__ void*)dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int4_rn_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int4_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_ONE_FOURTH_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_half2int4_rz
__aicore__ inline void asc_half2int4_rz_impl(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162s4z(
            (__ubuf__ void*)dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2int4_rz_impl(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2int4_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_ONE_FOURTH_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2uint8_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162u8(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2uint8_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2uint8_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2uint8_rd_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162u8f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2uint8_rd_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2uint8_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2uint8_rn_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162u8r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2uint8_rn_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2uint8_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2uint8_rna_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162u8a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2uint8_rna_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2uint8_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2uint8_ru_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162u8c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2uint8_ru_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2uint8_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_half2uint8_rz_impl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f162u8z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_half2uint8_rz_impl(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_half2uint8_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2half
__aicore__ inline void asc_float2half_impl(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322f16(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2half_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2half_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2half_rn
__aicore__ inline void asc_float2half_rn_impl(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322f16r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2half_rn_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2half_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2half_rna
__aicore__ inline void asc_float2half_rna_impl(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322f16a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2half_rna_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2half_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2half_rd
__aicore__ inline void asc_float2half_rd_impl(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322f16f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2half_rd_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2half_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2half_ru
__aicore__ inline void asc_float2half_ru_impl(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322f16c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2half_ru_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2half_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2half_rz
__aicore__ inline void asc_float2half_rz_impl(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322f16z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2half_rz_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2half_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2half_ro
__aicore__ inline void asc_float2half_ro_impl(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322f16o(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2half_ro_impl(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2half_ro_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2float_rn
__aicore__ inline void asc_float2float_rn_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322f32r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2float_rn_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2float_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2float_rd
__aicore__ inline void asc_float2float_rd_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322f32f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2float_rd_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2float_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2float_ru
__aicore__ inline void asc_float2float_ru_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322f32c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2float_ru_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2float_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2float_rna
__aicore__ inline void asc_float2float_rna_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322f32a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2float_rna_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2float_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2float_rz
__aicore__ inline void asc_float2float_rz_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322f32z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2float_rz_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2float_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int16_rd_impl(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s16f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int16_rd_impl(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int16_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int16_rn_impl(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s16r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int16_rn_impl(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int16_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int16_rna_impl(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s16a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int16_rna_impl(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int16_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int16_ru_impl(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s16c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int16_ru_impl(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int16_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int16_rz_impl(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s16z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int16_rz_impl(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int16_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int32_rd_impl(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s32f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int32_rd_count_impl(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int32_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int32_rn_impl(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s32r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int32_rn_count_impl(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int32_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int32_rna_impl(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s32a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int32_rna_count_impl(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int32_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int32_ru_impl(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s32c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int32_ru_count_impl(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int32_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int32_rz_impl(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s32z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int32_rz_count_impl(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int32_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int64_rd_impl(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s64f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int64_rd_count_impl(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int64_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int64_rn_impl(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s64r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int64_rn_count_impl(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int64_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int64_rna_impl(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s64a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int64_rna_count_impl(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int64_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int64_ru_impl(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s64c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int64_ru_count_impl(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int64_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_float2int64_rz_impl(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322s64z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2int64_rz_count_impl(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2int64_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2bfloat16_rn
__aicore__ inline void asc_float2bfloat16_rn_impl(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322bf16r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2bfloat16_rn_impl(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2bfloat16_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2bfloat16_rna
__aicore__ inline void asc_float2bfloat16_rna_impl(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322bf16a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2bfloat16_rna_impl(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2bfloat16_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2bfloat16_rd
__aicore__ inline void asc_float2bfloat16_rd_impl(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322bf16f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2bfloat16_rd_impl(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2bfloat16_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2bfloat16_ru
__aicore__ inline void asc_float2bfloat16_ru_impl(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322bf16c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2bfloat16_ru_impl(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2bfloat16_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_float2bfloat16_rz
__aicore__ inline void asc_float2bfloat16_rz_impl(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_f322bf16z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_float2bfloat16_rz_impl(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_float2bfloat16_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_deq_int322half_impl(
    __ubuf__ half* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_deq(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_deq_int322half_impl(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    constexpr uint8_t dst_repeat_stride =
        static_cast<uint8_t>(ASC_C_API_DEFAULT_REPEAT_STRIDE.U8 / (sizeof(int32_t) / sizeof(half)));
    asc_deq_int322half_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        dst_repeat_stride, ASC_C_API_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

// asc_deq_int162b8_h int8_t
__aicore__ inline void asc_deq_int162b8_h_impl(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_deqs162b8h(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_deq_int162b8_h_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_deq_int162b8_h_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_deq_int162b8_h uint8_t
__aicore__ inline void asc_deq_int162b8_h_impl(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_deqs162b8h(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_deq_int162b8_h_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_deq_int162b8_h_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_deq_int162b8_l int8_t
__aicore__ inline void asc_deq_int162b8_l_impl(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_deqs162b8l(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_deq_int162b8_l_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_deq_int162b8_l_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_deq_int162b8_l uint8_t
__aicore__ inline void asc_deq_int162b8_l_impl(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_deqs162b8l(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_deq_int162b8_l_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_deq_int162b8_l_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int42half_impl(
    __ubuf__ half* dst, __ubuf__ int4b_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s42f16(
            dst, (__ubuf__ void*)src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int42half_impl(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int42half_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U8, ASC_C_API_ONE_FOURTH_DEFAULT_REPEAT_STRIDE.U8);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int82half_impl(
    __ubuf__ half* dst, __ubuf__ int8_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s82f16(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int82half_impl(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int82half_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_uint82half_impl(
    __ubuf__ half* dst, __ubuf__ uint8_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_u82f16(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_uint82half_impl(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_uint82half_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int162half_impl(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s162f16(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int162half_impl(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int162half_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int162half_rd_impl(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s162f16f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int162half_rd_impl(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int162half_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int162half_rn_impl(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s162f16r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int162half_rn_impl(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int162half_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int162half_rna_impl(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s162f16a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int162half_rna_impl(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int162half_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int162half_ru_impl(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s162f16c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int162half_ru_impl(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int162half_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int162half_rz_impl(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s162f16z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int162half_rz_impl(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int162half_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int162float_impl(
    __ubuf__ float* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s162f32(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int162float_impl(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int162float_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int322float_impl(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s322f32(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int322float_impl(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int322float_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int322float_rd_impl(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s322f32f(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int322float_rd_impl(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int322float_rd_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int322float_rn_impl(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s322f32r(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int322float_rn_impl(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int322float_rn_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int322float_rna_impl(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s322f32a(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int322float_rna_impl(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int322float_rna_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int322float_ru_impl(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s322f32c(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int322float_ru_impl(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int322float_ru_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int322float_rz_impl(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s322f32z(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int322float_rz_impl(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int322float_rz_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int322int16_impl(
    __ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s322s16(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int322int16_impl(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int322int16_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int322int64_impl(
    __ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s322s64(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int322int64_impl(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int322int64_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_int642int32_impl(
    __ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_s642s32(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_int642int32_impl(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_int642int32_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_HALF_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_vdeqs162b8h int8_t
__aicore__ inline void asc_vdeqs162b8h_impl(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_vdeqs162b8h(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_vdeqs162b8h_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_vdeqs162b8h_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

// asc_vdeqs162b8h uint8_t
__aicore__ inline void asc_vdeqs162b8h_impl(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vconv_vdeqs162b8h(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_vdeqs162b8h_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_vdeqs162b8h_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_vdeqs162b8l_impl(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    vconv_vdeqs162b8l(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_vdeqs162b8l_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_vdeqs162b8l_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_vdeqs162b8l_impl(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    vconv_vdeqs162b8l(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_vdeqs162b8l_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_vdeqs162b8l_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_transpose_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vtranspose(dst, src);
    }
}

__aicore__ inline void asc_transpose_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vtranspose(dst, src);
    }
}

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

#define ASC_TRANSTO5HD_B8_CALL(dst_va, src_va, repeat, dst_stride, src_stride, dst_high_half, src_high_half) \
    do {                                                                                                     \
        if (!(dst_high_half)) {                                                                              \
            if (!(src_high_half)) {                                                                          \
                scatter_vnchwconv_b8(dst_va, src_va, repeat, dst_stride, src_stride, false, false);          \
            } else {                                                                                         \
                scatter_vnchwconv_b8(dst_va, src_va, repeat, dst_stride, src_stride, false, true);           \
            }                                                                                                \
        } else if (!(src_high_half)) {                                                                       \
            scatter_vnchwconv_b8(dst_va, src_va, repeat, dst_stride, src_stride, true, false);               \
        } else {                                                                                             \
            scatter_vnchwconv_b8(dst_va, src_va, repeat, dst_stride, src_stride, true, true);                \
        }                                                                                                    \
    } while (0)

__aicore__ inline void asc_transto5hd_b8_impl(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half)
{
    if ASC_IS_AIV {
        if (dst == ub_addr8_t::VA0) {
            if (src == ub_addr8_t::VA2) {
                ASC_TRANSTO5HD_B8_CALL(VA0, VA2, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA4) {
                ASC_TRANSTO5HD_B8_CALL(VA0, VA4, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA6) {
                ASC_TRANSTO5HD_B8_CALL(VA0, VA6, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            }
        } else if (dst == ub_addr8_t::VA2) {
            if (src == ub_addr8_t::VA0) {
                ASC_TRANSTO5HD_B8_CALL(VA2, VA0, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA4) {
                ASC_TRANSTO5HD_B8_CALL(VA2, VA4, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA6) {
                ASC_TRANSTO5HD_B8_CALL(VA2, VA6, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            }
        } else if (dst == ub_addr8_t::VA4) {
            if (src == ub_addr8_t::VA0) {
                ASC_TRANSTO5HD_B8_CALL(VA4, VA0, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA2) {
                ASC_TRANSTO5HD_B8_CALL(VA4, VA2, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA6) {
                ASC_TRANSTO5HD_B8_CALL(VA4, VA6, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            }
        } else if (dst == ub_addr8_t::VA6) {
            if (src == ub_addr8_t::VA0) {
                ASC_TRANSTO5HD_B8_CALL(VA6, VA0, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA2) {
                ASC_TRANSTO5HD_B8_CALL(VA6, VA2, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            } else if (src == ub_addr8_t::VA4) {
                ASC_TRANSTO5HD_B8_CALL(VA6, VA4, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
            }
        }
    }
}

#undef ASC_TRANSTO5HD_B8_CALL

__aicore__ inline void asc_transto5hd_b16_impl(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    if ASC_IS_AIV {
        if (dst == ub_addr8_t::VA0) {
            if (src == ub_addr8_t::VA2) {
                scatter_vnchwconv_b16(VA0, VA2, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA4) {
                scatter_vnchwconv_b16(VA0, VA4, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA6) {
                scatter_vnchwconv_b16(VA0, VA6, repeat, dst_stride, src_stride);
            }
        } else if (dst == ub_addr8_t::VA2) {
            if (src == ub_addr8_t::VA0) {
                scatter_vnchwconv_b16(VA2, VA0, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA4) {
                scatter_vnchwconv_b16(VA2, VA4, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA6) {
                scatter_vnchwconv_b16(VA2, VA6, repeat, dst_stride, src_stride);
            }
        } else if (dst == ub_addr8_t::VA4) {
            if (src == ub_addr8_t::VA0) {
                scatter_vnchwconv_b16(VA4, VA0, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA2) {
                scatter_vnchwconv_b16(VA4, VA2, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA6) {
                scatter_vnchwconv_b16(VA4, VA6, repeat, dst_stride, src_stride);
            }
        } else if (dst == ub_addr8_t::VA6) {
            if (src == ub_addr8_t::VA0) {
                scatter_vnchwconv_b16(VA6, VA0, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA2) {
                scatter_vnchwconv_b16(VA6, VA2, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA4) {
                scatter_vnchwconv_b16(VA6, VA4, repeat, dst_stride, src_stride);
            }
        }
    }
}

__aicore__ inline void asc_transto5hd_b32_impl(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    if ASC_IS_AIV {
        if (dst == ub_addr8_t::VA0) {
            if (src == ub_addr8_t::VA2) {
                scatter_vnchwconv_b32(VA0, VA2, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA4) {
                scatter_vnchwconv_b32(VA0, VA4, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA6) {
                scatter_vnchwconv_b32(VA0, VA6, repeat, dst_stride, src_stride);
            }
        } else if (dst == ub_addr8_t::VA2) {
            if (src == ub_addr8_t::VA0) {
                scatter_vnchwconv_b32(VA2, VA0, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA4) {
                scatter_vnchwconv_b32(VA2, VA4, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA6) {
                scatter_vnchwconv_b32(VA2, VA6, repeat, dst_stride, src_stride);
            }
        } else if (dst == ub_addr8_t::VA4) {
            if (src == ub_addr8_t::VA0) {
                scatter_vnchwconv_b32(VA4, VA0, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA2) {
                scatter_vnchwconv_b32(VA4, VA2, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA6) {
                scatter_vnchwconv_b32(VA4, VA6, repeat, dst_stride, src_stride);
            }
        } else if (dst == ub_addr8_t::VA6) {
            if (src == ub_addr8_t::VA0) {
                scatter_vnchwconv_b32(VA6, VA0, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA2) {
                scatter_vnchwconv_b32(VA6, VA2, repeat, dst_stride, src_stride);
            } else if (src == ub_addr8_t::VA4) {
                scatter_vnchwconv_b32(VA6, VA4, repeat, dst_stride, src_stride);
            }
        }
    }
}

__aicore__ inline int64_t asc_get_rsvd_count_impl()
{
    if ASC_IS_AIV {
        return get_rsvd_cnt();
    }
    return 0;
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ int8_t** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ uint8_t** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ int16_t** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ uint16_t** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ half** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ int32_t** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ uint32_t** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_set_va_reg_impl(ub_addr8_t addr, __ubuf__ float** src_array)
{
    if ASC_IS_AIV {
        uint64_t vaRegArray[8];
        for (int32_t i = 0; i < 8; ++i) {
            vaRegArray[i] = (uint64_t)src_array[i];
        }
        set_va_reg_sb(addr, vaRegArray);
    }
}

__aicore__ inline void asc_pair_reduce_sum_impl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcpadd(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_pair_reduce_sum_impl(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_pair_reduce_sum_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_pair_reduce_sum_impl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    if ASC_IS_AIV {
        vcpadd(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
    }
}

__aicore__ inline void asc_pair_reduce_sum_impl(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_pair_reduce_sum_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_copy_impl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    vcopy(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_copy_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_copy_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_copy_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    vcopy(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_copy_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_copy_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_copy_impl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    vcopy(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_copy_impl(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_copy_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_copy_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    vcopy(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_copy_impl(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t count)
{
    asc_set_mask_count_begin(count);
    asc_copy_impl(
        dst, src, ASC_C_API_DEFAULT_REPEAT.U8, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16, ASC_C_API_DEFAULT_BLOCK_STRIDE.U16,
        ASC_C_API_DEFAULT_REPEAT_STRIDE.U16, ASC_C_API_DEFAULT_REPEAT_STRIDE.U16);
    asc_set_mask_count_end();
}

__aicore__ inline void asc_squeeze_impl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vreduce(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

__aicore__ inline void asc_squeeze_impl(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    if ASC_IS_AIV {
        vreduce(
            dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
            src0_repeat_stride, src1_repeat_stride);
    }
}

// ==========asc_add(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_add_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_add_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_add_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_add_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_add_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_sub(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_sub(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_sub_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_sub_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_sub_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_sub_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_sub_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_mul(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_mul(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_mul_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_mul_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_mul_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_mul_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_mul_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_mul_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_div(half/float)==========
__aicore__ inline void asc_div(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_div_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_div(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_div_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_div(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_div_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_div(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_div_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_max(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_max(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_max_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_max(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_max_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_max(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_max_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_max(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_max_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_max(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_max_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_max(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_max_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_max(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_max_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_max(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_max_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_min(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_min(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_min_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_min(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_min_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_min(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_min_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_min(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_min_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_min(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_min_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_min(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_min_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_min(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count)
{
    asc_min_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_min(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_min_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_datablock_reduce(half/float)==========
__aicore__ inline void asc_datablock_reduce_sum(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_datablock_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_sum(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_datablock_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_max(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_datablock_reduce_max_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_max_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_max(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_datablock_reduce_max_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_max_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_min(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_datablock_reduce_min_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_datablock_reduce_min_impl(dst, src, count);
}

__aicore__ inline void asc_datablock_reduce_min(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_datablock_reduce_min_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_datablock_reduce_min_impl(dst, src, count);
}

// ==========asc_repeat_reduce(half/float)==========
__aicore__ inline void asc_repeat_reduce_sum(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_sum(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_index_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_index_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_index_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_value_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_value_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_value_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_only_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_max_only_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_max_only_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_index_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_index_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_index_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_value_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_value_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_value_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_only_value_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, count);
}

__aicore__ inline void asc_repeat_reduce_min_only_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_repeat_reduce_min_only_index_impl(dst, src, count);
}

// ===========asc_get_acc_val=================
__aicore__ inline int64_t asc_get_acc_val() { return asc_get_acc_val_impl(); }

// ============asc_get_reduce_max_cnt==========
__aicore__ inline void asc_get_reduce_max_cnt(half& val, uint32_t& index) { asc_get_reduce_max_cnt_impl(val, index); }

__aicore__ inline void asc_get_reduce_max_cnt(float& val, uint32_t& index) { asc_get_reduce_max_cnt_impl(val, index); }

__aicore__ inline void asc_get_reduce_min_cnt(half& val, uint32_t& index) { asc_get_reduce_min_cnt_impl(val, index); }

__aicore__ inline void asc_get_reduce_min_cnt(float& val, uint32_t& index) { asc_get_reduce_min_cnt_impl(val, index); }

// ==========asc_max_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_max_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_max_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_max_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_max_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_max_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_max_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_max_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_max_scalar(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_max_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_mul_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_mul_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_mul_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_mul_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_mul_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_mul_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_mul_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_mul_scalar(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_mul_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_add_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_add_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_add_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_add_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_add_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_add_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_add_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_add_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_add_scalar(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_add_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_sub_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_sub_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sub_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sub_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sub_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sub_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_sub_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_sub_scalar(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sub_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_leakyrelu(half/float)==========
__aicore__ inline void asc_leakyrelu(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_leakyrelu_impl(dst, src, value, count);
}

__aicore__ inline void asc_leakyrelu(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_leakyrelu_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_leakyrelu(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_leakyrelu_impl(dst, src, value, count);
}

__aicore__ inline void asc_leakyrelu(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_leakyrelu_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_exp(half/float)==========
__aicore__ inline void asc_exp(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_exp_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_exp(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_exp_impl(dst, src, count);
}

__aicore__ inline void asc_exp(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_exp_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_exp(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_exp_impl(dst, src, count);
}

// ==========asc_abs(half/float)==========
__aicore__ inline void asc_abs(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_abs_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_abs(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_abs_impl(dst, src, count);
}

__aicore__ inline void asc_abs(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_abs_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_abs(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_abs_impl(dst, src, count);
}

// ==========asc_rcp(half/float)==========
__aicore__ inline void asc_rcp(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_rcp_impl(dst, src, count);
}

__aicore__ inline void asc_rcp(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_rcp_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_rcp(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_rcp_impl(dst, src, count);
}

__aicore__ inline void asc_rcp(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_rcp_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_rsqrt(half/float)==========
__aicore__ inline void asc_rsqrt(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_rsqrt_impl(dst, src, count);
}

__aicore__ inline void asc_rsqrt(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_rsqrt_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_rsqrt(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_rsqrt_impl(dst, src, count);
}

__aicore__ inline void asc_rsqrt(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_rsqrt_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_sqrt(half/float)==========
__aicore__ inline void asc_sqrt(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sqrt_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sqrt(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_sqrt_impl(dst, src, count);
}

__aicore__ inline void asc_sqrt(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_sqrt_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_sqrt(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_sqrt_impl(dst, src, count);
}

// ==========asc_relu(half/float/int32_t)==========
__aicore__ inline void asc_relu(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_relu_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_relu(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_relu_impl(dst, src, count);
}

__aicore__ inline void asc_relu(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_relu_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_relu(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_relu_impl(dst, src, count);
}

__aicore__ inline void asc_relu(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_relu_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_relu(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_relu_impl(dst, src, count);
}

// ==========asc_log(half/float)==========
__aicore__ inline void asc_log(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_log_impl_half(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_log(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_log_count_impl_half(dst, src, count);
}

__aicore__ inline void asc_log(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_log_impl_float(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_log(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_log_count_impl_float(dst, src, count);
}

// ==========asc_not(int16_t/uint16_t)==========
__aicore__ inline void asc_not(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_not_impl(dst, src, count);
}

__aicore__ inline void asc_not(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_not_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_not(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
{
    asc_not_impl(dst, src, count);
}

__aicore__ inline void asc_not(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_not_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_and(int16_t/uint16_t)==========
__aicore__ inline void asc_and(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_and_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_and(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_and_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_and(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_and_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_and(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_and_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_or(int16_t/uint16_t)==========
__aicore__ inline void asc_or(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_or_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_or(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_or_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_or(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_or_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_or(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_or_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_shiftleft(int16_t/uint16_t/int32_t/uint32_t)==========
__aicore__ inline void asc_shiftleft(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shiftleft_impl(
        dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_shiftleft(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shiftleft(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shiftleft_impl(
        dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_shiftleft(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shiftleft(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shiftleft_impl(
        dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_shiftleft(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_impl(dst, src, distance, count);
}

__aicore__ inline void asc_shiftleft(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shiftleft_impl(
        dst, src, distance, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_shiftleft(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint32_t count)
{
    asc_shiftleft_impl(dst, src, distance, count);
}

// ==========asc_shiftright(int16_t/int32_t/uint16_t/uint32_t)==========
__aicore__ inline void asc_shiftright(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shiftright_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_shiftright(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shiftright_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_shiftright(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint32_t count)
{
    asc_shiftright_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shiftright_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_shiftright(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count)
{
    asc_shiftright_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shiftright_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_shiftright_round(int16_t/int32_t)==========
__aicore__ inline void asc_shiftright_round(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_round_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright_round(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shiftright_round_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_shiftright_round(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_shiftright_round_impl(dst, src, value, count);
}

__aicore__ inline void asc_shiftright_round(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_shiftright_round_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_min_scalar(half/float/int16_t/int32_t)==========
__aicore__ inline void asc_min_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_min_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_min_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_min_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_min_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_min_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_min_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count)
{
    asc_min_scalar_impl(dst, src, value, count);
}

__aicore__ inline void asc_min_scalar(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_min_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_set_deq_scale(float scale, int16_t offset, bool sign_mode)
{
    asc_set_deq_scale_impl(scale, offset, sign_mode);
}

__aicore__ inline void asc_set_deq_scale(
    __ubuf__ uint64_t* tmp, float scale_arr[ASC_VDEQ_SIZE], int16_t offset_arr[ASC_VDEQ_SIZE],
    bool sign_mode_arr[ASC_VDEQ_SIZE])
{
    asc_set_deq_scale_impl(tmp, scale_arr, offset_arr, sign_mode_arr);
}

__aicore__ inline void asc_set_deq_scale(half scale) { asc_set_deq_scale_impl(scale); }

__aicore__ inline void asc_set_mask_count() { asc_set_mask_count_impl(); }

__aicore__ inline void asc_set_mask_norm() { asc_set_mask_norm_impl(); }

__aicore__ inline void asc_set_vector_mask(uint64_t mask_high, uint64_t mask_low)
{
    asc_set_vector_mask_impl(mask_high, mask_low);
}

// ==========asc_lt(half/float)==========
__aicore__ inline void asc_lt(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_lt_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_lt_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_lt(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_lt_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_gt(half/float)==========
__aicore__ inline void asc_gt(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_gt_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_gt_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_gt(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_gt_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_ge(half/float/uint16_t)==========
__aicore__ inline void asc_ge(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ge_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ge(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ge_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ge(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_ge_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ge(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_ge_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_eq(half/float/uint8_t-half/uint8_t-float/uint8_t-int32_t)==========
__aicore__ inline void asc_eq(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_eq_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_eq_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_eq_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_eq(
    __ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_eq_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_ne(half/float)==========
__aicore__ inline void asc_ne(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_ne_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_ne_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_ne(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_ne_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_le(half/float)==========
__aicore__ inline void asc_le(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_le_impl(
        src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_le_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_le(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_le_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_lt_scalar(half/float)==========
__aicore__ inline void asc_lt_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_lt_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_lt_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_lt_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_gt_scalar(dst:uint8_t, src:half/float)==========
__aicore__ inline void asc_gt_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_gt_scalar_impl_half(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_gt_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_gt_scalar_impl_float(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_ge_scalar(half/float)==========
__aicore__ inline void asc_ge_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ge_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_ge_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ge_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_eq_scalar(half/float/int32_t)==========
__aicore__ inline void asc_eq_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_eq_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_eq_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_eq_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_ne_scalar(dst:uint8_t, src:half/float)==========
__aicore__ inline void asc_ne_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ne_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_ne_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_ne_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_le_scalar(half/float)==========
__aicore__ inline void asc_le_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_le_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_le_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_le_scalar_impl(
        dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_get_cmp_mask(__ubuf__ void* dst) { asc_get_cmp_mask_impl(dst); }

__aicore__ inline void asc_set_cmp_mask(__ubuf__ void* sel_mask) { asc_set_cmp_mask_impl(sel_mask); }

// ==========asc_select(half/float)==========
__aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_select_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_select(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_select_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_select_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_select(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_select_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_reduce(uint16_t/uint32_t)==========
__aicore__ inline void asc_reduce(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint16_t repeat,
    uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_reduce_impl(dst, src0, src1, repeat, src0_block_stride, pattern_mode, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_reduce(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count)
{
    asc_reduce_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_reduce(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint16_t repeat,
    uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride)
{
    asc_reduce_impl(dst, src0, src1, repeat, src0_block_stride, pattern_mode, src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_reduce(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint32_t count)
{
    asc_reduce_impl(dst, src0, src1, count);
}

// ==========asc_add_relu(f162s8)==========
__aicore__ inline void asc_add_relu(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_add_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_add_relu(f322f16)==========
__aicore__ inline void asc_add_relu(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu(
    __ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_add_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_add_relu(s162s8)==========
__aicore__ inline void asc_add_relu(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_add_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_add_relu(half/float/int16_t)==========
__aicore__ inline void asc_add_relu(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_add_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_add_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_add_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_add_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_add_relu(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_add_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_add_relu(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_add_relu_impl(dst, src0, src1, count);
}

// ==========asc_axpy(half/float/float half)==========
__aicore__ inline void asc_axpy(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_axpy_impl(dst, src, value, count);
}

__aicore__ inline void asc_axpy(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_axpy_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count)
{
    asc_axpy_impl(dst, src, value, count);
}

__aicore__ inline void asc_axpy(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_axpy_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ half* src, half value, uint32_t count)
{
    asc_axpy_impl(dst, src, value, count);
}

__aicore__ inline void asc_axpy(
    __ubuf__ float* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_axpy_impl(dst, src, value, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_bitsort(half/float)==========
__aicore__ inline void asc_bitsort(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_impl_half(dst, src0, src1, repeat);
}

__aicore__ inline void asc_bitsort(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_impl_float(dst, src0, src1, repeat);
}

// ==========asc_fma(half/float)==========
__aicore__ inline void asc_fma(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_fma_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_fma(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_fma_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_fma(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_fma_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_fma(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_fma_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_fma(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_fma_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_fma(
    __ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_fma_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_gather_datablock(uint16_t/uint32_t)==========
__aicore__ inline void asc_gather_datablock(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t dst_block_stride, uint8_t repeat)
{
    asc_gather_datablock_impl(dst, src, src_offset, dst_repeat_stride, dst_block_stride, repeat);
}

__aicore__ inline void asc_gather_datablock(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t dst_block_stride, uint8_t repeat)
{
    asc_gather_datablock_impl(dst, src, src_offset, dst_repeat_stride, dst_block_stride, repeat);
}

// ==========asc_gather(int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
__aicore__ inline void asc_gather(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    asc_gather_impl(dst, src, src_offset, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_gather(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
}

__aicore__ inline void asc_gather(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    asc_gather_impl(dst, src, src_offset, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_gather(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
}

__aicore__ inline void asc_gather(
    __ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_gather_impl(dst, src, src_offset, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_gather(__ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
}

__aicore__ inline void asc_gather(
    __ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    asc_gather_impl(dst, src, src_offset, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_gather(
    __ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
}

__aicore__ inline void asc_gather(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    asc_gather_impl(dst, src, src_offset, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_gather(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
}

__aicore__ inline void asc_gather(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat)
{
    asc_gather_impl(dst, src, src_offset, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_gather(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
}

__aicore__ inline void asc_gather(
    __ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride, uint8_t repeat)
{
    asc_gather_impl(dst, src, src_offset, dst_repeat_stride, repeat);
}

__aicore__ inline void asc_gather(
    __ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset, uint32_t count)
{
    asc_gather_impl(dst, src, src_offset, count);
}

// ==========asc_get_vms4_sr==========
__aicore__ inline void asc_get_vms4_sr(uint16_t sorted_num[4]) { asc_get_vms4_sr_impl(sorted_num); }

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

// ==========asc_mul_add(half/float)==========
__aicore__ inline void asc_mul_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_mul_add_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_add_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_mul_add_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_mul_add_relu(half/float)==========
__aicore__ inline void asc_mul_add_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_add_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_relu(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_mul_add_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_add_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_mul_add_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_mul_add_relu(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_mul_add_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_mul_cast_half2int8==========
__aicore__ inline void asc_mul_cast_half2int8(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_mul_cast_half2int8_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_cast_half2int8(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_cast_half2int8_impl(dst, src0, src1, count);
}

// ==========asc_mul_cast_half2uint8==========
__aicore__ inline void asc_mul_cast_half2uint8(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_mul_cast_half2uint8_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_mul_cast_half2uint8(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_mul_cast_half2uint8_impl(dst, src0, src1, count);
}

// ==========asc_sub_relu(f162s8/f322f16/s162s8)==========
__aicore__ inline void asc_sub_relu(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_sub_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub_relu(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu(
    __ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_sub_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub_relu(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_sub_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_sub_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_sub_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_sub_relu(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count)
{
    asc_sub_relu_impl(dst, src0, src1, count);
}

__aicore__ inline void asc_sub_relu(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_sub_relu_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

// ==========asc_bfloat162float==========
__aicore__ inline void asc_bfloat162float(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162float_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162float(
    __ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bfloat162float_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_bfloat162int32==========
__aicore__ inline void asc_bfloat162int32_rna(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bfloat162int32_rna_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bfloat162int32_rna(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rna_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_ru(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bfloat162int32_ru_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bfloat162int32_ru(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_ru_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_rd(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bfloat162int32_rd_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bfloat162int32_rd(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rd_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_rn(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bfloat162int32_rn_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bfloat162int32_rn(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rn_impl(dst, src, count);
}

__aicore__ inline void asc_bfloat162int32_rz(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_bfloat162int32_rz_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_bfloat162int32_rz(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count)
{
    asc_bfloat162int32_rz_impl(dst, src, count);
}

// ==========asc_half2float==========
__aicore__ inline void asc_half2float(
    __ubuf__ float* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2float_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2float(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2float_impl(dst, src, count);
}

// ==========asc_half2int16==========
__aicore__ inline void asc_half2int16_rna(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_rna(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rna_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_ru(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_ru(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_ru_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_rd(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_rd(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rd_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_rn(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_rn(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rn_impl(dst, src, count);
}

__aicore__ inline void asc_half2int16_rz(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int16_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int16_rz(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int16_rz_impl(dst, src, count);
}

// ==========asc_half2int32==========
__aicore__ inline void asc_half2int32_rna(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_rna(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rna_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_ru(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_ru(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_ru_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_rd(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_rd(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rd_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_rn(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_rn(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rn_impl(dst, src, count);
}

__aicore__ inline void asc_half2int32_rz(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int32_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int32_rz(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int32_rz_impl(dst, src, count);
}

// ==========asc_half2int4==========
__aicore__ inline void asc_half2int4(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_rna(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_rna(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rna_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_ru(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_ru(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_ru_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_rd(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_rd(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rd_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_rn(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_rn(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rn_impl(dst, src, count);
}

__aicore__ inline void asc_half2int4_rz(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int4_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int4_rz(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int4_rz_impl(dst, src, count);
}

// ==========asc_half2int8==========
__aicore__ inline void asc_half2int8(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_rna(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_rna(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rna_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_ru(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_ru(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_ru_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_rd(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_rd(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rd_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_rn(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_rn(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rn_impl(dst, src, count);
}

__aicore__ inline void asc_half2int8_rz(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2int8_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2int8_rz(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2int8_rz_impl(dst, src, count);
}

// ==========asc_half2uint8==========
__aicore__ inline void asc_half2uint8(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_rna(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_rna(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rna_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_ru(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_ru(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_ru_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_rd(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_rd(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rd_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_rn(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_rn(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rn_impl(dst, src, count);
}

__aicore__ inline void asc_half2uint8_rz(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_half2uint8_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_half2uint8_rz(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
{
    asc_half2uint8_rz_impl(dst, src, count);
}

// ==========asc_float2half==========
__aicore__ inline void asc_float2half(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_rn(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_rn(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rn_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_rna(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_rna(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rna_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_rd(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_rd(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rd_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_ru(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_ru(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_ru_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_rz(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_rz(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_rz_impl(dst, src, count);
}

__aicore__ inline void asc_float2half_ro(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2half_ro_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2half_ro(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2half_ro_impl(dst, src, count);
}

// ==========asc_float2float==========
__aicore__ inline void asc_float2float_rn(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_rn(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rn_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_rd(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_rd(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rd_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_ru(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_ru(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_ru_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_rna(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_rna_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_rna(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rna_impl(dst, src, count);
}

__aicore__ inline void asc_float2float_rz(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2float_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2float_rz(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2float_rz_impl(dst, src, count);
}

// ==========asc_float2int16==========
__aicore__ inline void asc_float2int16_rna(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_rna_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_rna(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rna_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_ru(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_ru(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_ru_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_rd(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_rd(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rd_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_rn(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_rn(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rn_impl(dst, src, count);
}

__aicore__ inline void asc_float2int16_rz(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int16_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int16_rz(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int16_rz_impl(dst, src, count);
}

// ==========asc_float2int32==========
__aicore__ inline void asc_float2int32_rna(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_rna_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_rna(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rna_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_ru(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_ru(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_ru_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_rd(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_rd(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rd_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_rn(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_rn(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rn_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int32_rz(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int32_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int32_rz(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int32_rz_count_impl(dst, src, count);
}

// ==========asc_float2int64==========
__aicore__ inline void asc_float2int64_rna(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_rna_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_rna(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rna_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_ru(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_ru(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_ru_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_rd(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_rd(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rd_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_rn(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_rn(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rn_count_impl(dst, src, count);
}

__aicore__ inline void asc_float2int64_rz(
    __ubuf__ int64_t* dst, __ubuf__ float* src, const uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2int64_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2int64_rz(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2int64_rz_count_impl(dst, src, count);
}

// ==========asc_float2bfloat16==========
__aicore__ inline void asc_float2bfloat16_rn(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bfloat16_rn_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bfloat16_rn(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rn_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_rna(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bfloat16_rna_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bfloat16_rna(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rna_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_rd(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bfloat16_rd_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bfloat16_rd(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rd_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_ru(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bfloat16_ru_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bfloat16_ru(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_ru_impl(dst, src, count);
}

__aicore__ inline void asc_float2bfloat16_rz(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_float2bfloat16_rz_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_float2bfloat16_rz(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
{
    asc_float2bfloat16_rz_impl(dst, src, count);
}

// ==========asc_deq_int322half==========
__aicore__ inline void asc_deq_int322half(
    __ubuf__ half* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int322half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int322half(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_deq_int322half_impl(dst, src, count);
}

// ==========asc_deq_int162b8==========
__aicore__ inline void asc_deq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_h(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int162b8_h_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_h_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_h(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int162b8_h_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_l(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int162b8_l_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_deq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_deq_int162b8_l_impl(dst, src, count);
}

__aicore__ inline void asc_deq_int162b8_l(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_deq_int162b8_l_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_int42half==========
__aicore__ inline void asc_int42half(
    __ubuf__ half* dst, __ubuf__ int4b_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_int42half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int42half(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint32_t count)
{
    asc_int42half_impl(dst, src, count);
}

// ==========asc_int82half==========
__aicore__ inline void asc_int82half(
    __ubuf__ half* dst, __ubuf__ int8_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int82half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int82half(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count)
{
    asc_int82half_impl(dst, src, count);
}

// ==========asc_uint82half==========
__aicore__ inline void asc_uint82half(
    __ubuf__ half* dst, __ubuf__ uint8_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_uint82half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_uint82half(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count)
{
    asc_uint82half_impl(dst, src, count);
}

// ==========asc_int162half==========
__aicore__ inline void asc_int162half(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_impl(dst, src, count);
}

// ==========asc_int162half_rna==========
__aicore__ inline void asc_int162half_rna(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_rna_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_rna(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rna_impl(dst, src, count);
}

// ==========asc_int162half_ru==========
__aicore__ inline void asc_int162half_ru(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_ru(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_ru_impl(dst, src, count);
}

// ==========asc_int162half_rd==========
__aicore__ inline void asc_int162half_rd(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_rd(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rd_impl(dst, src, count);
}

// ==========asc_int162half_rn==========
__aicore__ inline void asc_int162half_rn(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_rn(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rn_impl(dst, src, count);
}

// ==========asc_int162half_rz==========
__aicore__ inline void asc_int162half_rz(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162half_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162half_rz(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162half_rz_impl(dst, src, count);
}

// ==========asc_int162float==========
__aicore__ inline void asc_int162float(
    __ubuf__ float* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int162float_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int162float(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_int162float_impl(dst, src, count);
}

// ==========asc_int322float==========
__aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_impl(dst, src, count);
}

__aicore__ inline void asc_int322float(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_int322float_rna==========
__aicore__ inline void asc_int322float_rna(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rna_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_rna(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_rna_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_int322float_ru==========
__aicore__ inline void asc_int322float_ru(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_ru_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_ru(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_int322float_rd==========
__aicore__ inline void asc_int322float_rd(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rd_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_rd(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_int322float_rn==========
__aicore__ inline void asc_int322float_rn(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rn_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_rn(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_int322float_rz==========
__aicore__ inline void asc_int322float_rz(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322float_rz_impl(dst, src, count);
}

__aicore__ inline void asc_int322float_rz(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322float_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_int322int16==========
__aicore__ inline void asc_int322int16(
    __ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322int16_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322int16(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int16_impl(dst, src, count);
}

// ==========asc_int322int64==========
__aicore__ inline void asc_int322int64(
    __ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int322int64_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int322int64(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_int322int64_impl(dst, src, count);
}

// ==========asc_int642int32==========
__aicore__ inline void asc_int642int32(
    __ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642int32_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642int32(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642int32_impl(dst, src, count);
}

// ==========asc_int642float_rna==========
__aicore__ inline void asc_int642float_rna(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642float_rna_impl(
        dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642float_rna(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rna_impl(dst, src, count);
}

// ==========asc_int642float_ru==========
__aicore__ inline void asc_int642float_ru(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642float_ru_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642float_ru(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_ru_impl(dst, src, count);
}

// ==========asc_int642float_rd==========
__aicore__ inline void asc_int642float_rd(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642float_rd_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642float_rd(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rd_impl(dst, src, count);
}

// ==========asc_int642float_rn==========
__aicore__ inline void asc_int642float_rn(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642float_rn_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642float_rn(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rn_impl(dst, src, count);
}

// ==========asc_int642float_rz==========
__aicore__ inline void asc_int642float_rz(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_int642float_rz_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_int642float_rz(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count)
{
    asc_int642float_rz_impl(dst, src, count);
}

// ==========asc_vdeq_int162b8_h==========
__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_impl(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_h(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_vdeqs162b8h_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_impl(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_h(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_vdeqs162b8h_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_vdeq_int162b8_l(int8_t/uint8_t)==========
__aicore__ inline void asc_vdeq_int162b8_l(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_vdeqs162b8l_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_vdeq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8l_impl(dst, src, count);
}

__aicore__ inline void asc_vdeq_int162b8_l(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride)
{
    asc_vdeqs162b8l_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_vdeq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8l_impl(dst, src, count);
}

// ==========asc_transpose(int16_t/uint16_t)==========
__aicore__ inline void asc_transpose(__ubuf__ int16_t* dst, __ubuf__ int16_t* src) { asc_transpose_impl(dst, src); }

__aicore__ inline void asc_transpose(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src) { asc_transpose_impl(dst, src); }

// ==========asc_transto5hd(b8/b16/b32)==========
__aicore__ inline void asc_transto5hd_b8(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half)
{
    asc_transto5hd_b8_impl(dst, src, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
}

__aicore__ inline void asc_transto5hd_b16(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    asc_transto5hd_b16_impl(dst, src, repeat, dst_stride, src_stride);
}

__aicore__ inline void asc_transto5hd_b32(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    asc_transto5hd_b32_impl(dst, src, repeat, dst_stride, src_stride);
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

// ==========asc_get_rsvd_count==========
__aicore__ inline int64_t asc_get_rsvd_count() { return asc_get_rsvd_count_impl(); }

//==========asc_pair_reduce_sum(half/float)============
__aicore__ inline void asc_pair_reduce_sum(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_pair_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_pair_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
{
    asc_pair_reduce_sum_impl(dst, src, count);
}

__aicore__ inline void asc_pair_reduce_sum(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride)
{
    asc_pair_reduce_sum_impl(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
}

__aicore__ inline void asc_pair_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
{
    asc_pair_reduce_sum_impl(dst, src, count);
}

// ==========asc_copy(int16_t/uint16_t/int32_t/uint32_t)==========
__aicore__ inline void asc_copy(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_copy_impl(dst, src, count);
}

__aicore__ inline void asc_copy(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_copy_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_copy(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count)
{
    asc_copy_impl(dst, src, count);
}

__aicore__ inline void asc_copy(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_copy_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_copy(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count)
{
    asc_copy_impl(dst, src, count);
}

__aicore__ inline void asc_copy(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_copy_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

__aicore__ inline void asc_copy(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t count)
{
    asc_copy_impl(dst, src, count);
}

__aicore__ inline void asc_copy(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
{
    asc_copy_impl(dst, src, repeat, dst_block_stride, src_block_stride, dst_repeat_stride, src_repeat_stride);
}

// ==========asc_set_va_reg==========
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int8_t** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint8_t** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int16_t** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint16_t** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ half** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int32_t** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint32_t** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ float** src_array)
{
    asc_set_va_reg_impl(addr, src_array);
}

// =========asc_squeeze=========
__aicore__ inline void asc_squeeze(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_squeeze_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

__aicore__ inline void asc_squeeze(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride)
{
    asc_squeeze_impl(
        dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride,
        src0_repeat_stride, src1_repeat_stride);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_COMPUTE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_COMPUTE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_COMPUTE_INTF_IMPL_H
