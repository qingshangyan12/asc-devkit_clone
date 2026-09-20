/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "vector_compute/compute/vector_convert.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_CONVERT_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_CONVERT_H
#define INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_CONVERT_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/vector_convert_intf_impl.h"
#endif

/**
 * @brief 将bfloat16_t类型数据转为float，无舍入模式。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_bfloat162float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_bfloat162int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2bfloat16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int64.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int4.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2uint8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int162float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int162half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322int64.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int42half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int642float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int642int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int82half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_uint82half.md
 */
__aicore__ inline void asc_bfloat162float(
    __ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_bfloat162float(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @brief 将bfloat16_t类型数据转换为int32_t类型，并支持多种舍入模式：
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_bfloat162float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_bfloat162int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2bfloat16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int64.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int4.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2uint8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int162float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int162half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322int64.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int42half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int642float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int642int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int82half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_uint82half.md
 */
__aicore__ inline void asc_bfloat162int32_rna(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_bfloat162int32_rna(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

__aicore__ inline void asc_bfloat162int32_ru(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_bfloat162int32_ru(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

__aicore__ inline void asc_bfloat162int32_rd(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_bfloat162int32_rd(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

__aicore__ inline void asc_bfloat162int32_rn(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_bfloat162int32_rn(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

__aicore__ inline void asc_bfloat162int32_rz(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_bfloat162int32_rz(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @brief 将float类型数据转换为bfloat16_t类型，并支持多种舍入模式：
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_bfloat162float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_bfloat162int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2bfloat16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int64.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int4.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2uint8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int162float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int162half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322int64.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int42half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int642float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int642int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int82half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_uint82half.md
 */
__aicore__ inline void asc_float2bfloat16_rn(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2bfloat16_rn(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2bfloat16_rna(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2bfloat16_rna(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2bfloat16_rd(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2bfloat16_rd(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2bfloat16_ru(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2bfloat16_ru(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2bfloat16_rz(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2bfloat16_rz(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @brief 对float类型数据进行精度转换处理,支持多种舍入模式：
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_bfloat162float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_bfloat162int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2bfloat16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int64.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int4.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2uint8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int162float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int162half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322int64.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int42half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int642float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int642int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int82half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_uint82half.md
 */
__aicore__ inline void asc_float2float_rn(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2float_rn(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2float_rd(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2float_rd(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2float_ru(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2float_ru(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2float_rna(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2float_rna(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2float_rz(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2float_rz(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @brief 将float类型数据转换为half类型，支持多种舍入模式：
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_bfloat162float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_bfloat162int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2bfloat16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int64.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int4.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2uint8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int162float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int162half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322int16.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322int64.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int42half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int642float.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int642int32.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int82half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_uint82half.md
 */
__aicore__ inline void asc_float2half(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2half(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2half_rn(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2half_rn(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2half_rna(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2half_rna(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2half_rd(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2half_rd(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2half_ru(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2half_ru(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2half_rz(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2half_rz(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2half_ro(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2half_ro(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @brief 将float类型转化为int32_t类型，并支持多种舍入模式：
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int32.md
 */
__aicore__ inline void asc_float2int32_rna(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int32_rna(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2int32_ru(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int32_ru(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2int32_rd(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int32_rd(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2int32_rn(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int32_rn(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2int32_rz(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int32_rz(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @brief 将float类型转化为int64_t类型，并支持多种舍入模式：
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int64.md
 */
__aicore__ inline void asc_float2int64_rna(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int64_rna(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2int64_ru(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int64_ru(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2int64_rd(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int64_rd(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2int64_rn(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int64_rn(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2int64_rz(
    __ubuf__ int64_t* dst, __ubuf__ float* src, const uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int64_rz(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @brief 将half类型转化为float类型，无舍入模式。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2float.md
 */
__aicore__ inline void asc_half2float(
    __ubuf__ float* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2float(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count);

/**
 * @brief 将half类型数据转换为int4类型，支持多种舍入模式：
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int4.md
 */
__aicore__ inline void asc_half2int4(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int4(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int4_rna(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int4_rna(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int4_ru(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int4_ru(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int4_rd(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int4_rd(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int4_rn(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int4_rn(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int4_rz(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int4_rz(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int8.md
 */
__aicore__ inline void asc_half2int8(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int8_rna(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int8_rna(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int8_ru(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int8_ru(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int8_rd(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int8_rd(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int8_rn(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int8_rn(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int8_rz(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int8_rz(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int16.md
 */
__aicore__ inline void asc_half2int16_rna(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int16_rna(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int16_ru(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int16_ru(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int16_rd(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int16_rd(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int16_rn(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int16_rn(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int16_rz(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int16_rz(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @brief 将half类型转化为int32_t类型，并支持多种舍入模式：
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int32.md
 */
__aicore__ inline void asc_half2int32_rna(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int32_rna(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int32_ru(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int32_ru(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int32_rd(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int32_rd(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int32_rn(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int32_rn(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2int32_rz(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2int32_rz(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @brief 将int4_t类型数据转换为half类型。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int42half.md
 */
__aicore__ inline void asc_int42half(
    __ubuf__ half* dst, __ubuf__ int4b_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

__aicore__ inline void asc_int42half(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint32_t count);

/**
 * @brief 将int8_t类型数据转换为half类型。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int82half.md
 */
__aicore__ inline void asc_int82half(
    __ubuf__ half* dst, __ubuf__ int8_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int82half(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count);

/**
 * @brief 将uint8_t类型数据转换为half类型。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_uint82half.md
 */
__aicore__ inline void asc_uint82half(
    __ubuf__ half* dst, __ubuf__ uint8_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_uint82half(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count);

/**
 * @brief 将int16_t类型数据转换为float类型。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int162float.md
 */
__aicore__ inline void asc_int162float(
    __ubuf__ float* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int162float(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @brief 将int32_t类型数据转换为float类型，支持多种舍入模式：
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322float.md
 */
__aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

__aicore__ inline void asc_int322float(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int322float_rna(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

__aicore__ inline void asc_int322float_rna(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int322float_ru(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

__aicore__ inline void asc_int322float_ru(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int322float_rd(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

__aicore__ inline void asc_int322float_rd(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int322float_rn(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

__aicore__ inline void asc_int322float_rn(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int322float_rz(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

__aicore__ inline void asc_int322float_rz(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @brief 将int32_t类型数据转换为int16_t类型。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322int16.md
 */
__aicore__ inline void asc_int322int16(
    __ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int322int16(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @brief 将int32_t类型数据转换为int64_t类型。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int322int64.md
 */
__aicore__ inline void asc_int322int64(
    __ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int322int64(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @brief 将int64_t类型数据转换为int32_t类型。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int642int32.md
 */
__aicore__ inline void asc_int642int32(
    __ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int642int32(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @brief 将float类型数据转换为int16_t类型，支持多种舍入模式：
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_float2int16.md
 */
__aicore__ inline void asc_float2int16_rna(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int16_rna(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2int16_ru(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int16_ru(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2int16_rd(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int16_rd(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2int16_rn(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int16_rn(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_float2int16_rz(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_float2int16_rz(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @brief 将int64_t类型数据转换为float类型。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int642float.md
 */
__aicore__ inline void asc_int642float_rna(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int642float_rna(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

__aicore__ inline void asc_int642float_ru(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int642float_ru(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

__aicore__ inline void asc_int642float_rd(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int642float_rd(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

__aicore__ inline void asc_int642float_rn(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int642float_rn(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

__aicore__ inline void asc_int642float_rz(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int642float_rz(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @brief 将int16_t类型数据转换为half类型，支持多种舍入模式：
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_int162half.md
 */
__aicore__ inline void asc_int162half(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int162half(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_int162half_rna(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int162half_rna(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_int162half_ru(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int162half_ru(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_int162half_rd(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int162half_rd(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_int162half_rn(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int162half_rn(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_int162half_rz(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_int162half_rz(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @brief 将half类型数据转换为uint8_t类型，支持多种舍入模式：
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2uint8.md
 */
__aicore__ inline void asc_half2uint8(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2uint8_rna(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2uint8_rna(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2uint8_ru(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2uint8_ru(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2uint8_rd(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2uint8_rd(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2uint8_rn(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2uint8_rn(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_half2uint8_rz(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_half2uint8_rz(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_CONVERT_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_CONVERT_H
#endif
