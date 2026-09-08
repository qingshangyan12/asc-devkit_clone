/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_FUSED_INTF_IMPL_H
#define IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_FUSED_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/memory_base_impl/vector_fused_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_FUSED_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/memory_base_impl/utils_impl.h"

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

__aicore__ inline void asc_vdeqs162b8h_sync_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_vdeqs162b8h_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8h_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_vdeqs162b8l_sync_impl(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8l_impl(dst, src, count);
    asc_sync_post_process();
}

__aicore__ inline void asc_vdeqs162b8l_sync_impl(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
{
    asc_vdeqs162b8l_impl(dst, src, count);
    asc_sync_post_process();
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_FUSED_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_FUSED_INTF_IMPL_H
#endif

#endif // IMPL_C_API_MEMORY_BASE_IMPL_VECTOR_FUSED_INTF_IMPL_H
