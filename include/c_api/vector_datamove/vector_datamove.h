/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "vector_datamove.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_DATAMOVE_VECTOR_DATAMOVE_H
#endif

#ifndef INCLUDE_C_API_VECTOR_DATAMOVE_VECTOR_DATAMOVE_H
#define INCLUDE_C_API_VECTOR_DATAMOVE_VECTOR_DATAMOVE_H

#include "c_api/defs/defs.h"
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/vector_datamove_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/vector_datamove_intf_impl.h"
#endif

#include "c_api/composite/vector_compute_composite.h"

//===========asc_set_copy_pad_val(int8/uint8/int16/uint16/half/bfloat16/int32/uint32/float)===========
/**
 * @brief 该接口与[asc_copy_gm2ub_align](./asc_copy_gm2ub_align/asc_copy_gm2ub_align.md)配合使用，用于设置连续搬运数据块左右两侧的填充值。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_copy_pad_val.md
 */
__aicore__ inline void asc_set_copy_pad_val(int8_t pad_value);

__aicore__ inline void asc_set_copy_pad_val(uint8_t pad_value);

__aicore__ inline void asc_set_copy_pad_val(int16_t pad_value);

__aicore__ inline void asc_set_copy_pad_val(uint16_t pad_value);

__aicore__ inline void asc_set_copy_pad_val(half pad_value);

__aicore__ inline void asc_set_copy_pad_val(bfloat16_t pad_value);

__aicore__ inline void asc_set_copy_pad_val(int32_t pad_value);

__aicore__ inline void asc_set_copy_pad_val(uint32_t pad_value);

__aicore__ inline void asc_set_copy_pad_val(float pad_value);

/**
 * @brief 将数据从Unified Buffer（UB）搬运到UB。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2ub.md
 */
__aicore__ inline void asc_copy_ub2ub(
    __ubuf__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap);
__aicore__ inline void asc_copy_ub2ub(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size);

/**
 * @brief 将数据从Unified Buffer（UB）搬运到UB。支持高维切分。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy.md
 */
__aicore__ inline void asc_copy(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_copy(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_copy(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count);

__aicore__ inline void asc_copy(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_copy(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count);

__aicore__ inline void asc_copy(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_copy(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t count);

__aicore__ inline void asc_copy(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @brief 将数据从Global Memory (GM)搬运到Unified Buffer（UB）。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub.md
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_2201.md
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub_arch_3510.md
 */
__aicore__ inline void asc_copy_gm2ub(
    __ubuf__ void* dst, __gm__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);
__aicore__ inline void asc_copy_gm2ub(__ubuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm.md
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_2201.md
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md
 */
__aicore__ inline void asc_copy_ub2gm(__gm__ void* dst, __ubuf__ void* src, uint32_t size);
__aicore__ inline void asc_copy_ub2gm(
    __gm__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap);

// asc_copy_gm2ub_align  int8_t / uint8_t / half / bfloat16_t / int16_t / uint16_t / float / int32_t / uint32_t
/**
 * @brief 提供数据非对齐搬运的功能，将数据从Global Memory (GM)搬运到Unified Buffer（UB），并支持8位/16位/32位数据类型搬运。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align.md
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_2201.md
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_3510.md
 */
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ half* dst, __gm__ half* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ half* dst, __gm__ half* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ float* dst, __gm__ float* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ float* dst, __gm__ float* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size);

// asc_copy_ub2gm_align
/**
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align.md
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_2201.md
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510.md
 */
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ half* dst, __ubuf__ half* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ half* dst, __ubuf__ half* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint16_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ float* dst, __ubuf__ float* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ float* dst, __ubuf__ float* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ double* dst, __ubuf__ double* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ double* dst, __ubuf__ double* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int64_t* dst, __ubuf__ int64_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ int64_t* dst, __ubuf__ int64_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint32_t size);

// ==========asc_copy_gm2ub==========
__aicore__ inline void asc_copy_gm2ub(
    __ubuf__ void* dst, __gm__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_stride,
    uint16_t dst_stride);
__aicore__ inline void asc_copy_ub2gm(
    __gm__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t dst_stride,
    uint16_t src_stride);
// ==========asc_copy_gm2ub_align(int8_t/uint8_t/fp8_e5m2_t/fp8_e4m3fn_t/hifloat8_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);
__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t size);

// ==========asc_copy_ub2gm_align(int8_t/uint8_t/fp8_e5m2_t/fp8_e4m3fn_t/hifloat8_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ half* dst, __ubuf__ half* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ float* dst, __ubuf__ float* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);
__aicore__ inline void asc_copy_ub2gm_align(__gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(__gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align(__gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t size);

/**
 * @brief 本接口用于设置[asc_copy_ub2gm_align](./asc_copy_ub2gm_align/asc_copy_ub2gm_align.md)使用循环搬运模式时的两层循环迭代次数。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_ub2gm_loop_size.md
 */
__aicore__ inline void asc_set_ub2gm_loop_size(uint32_t loop1_size, uint32_t loop2_size);
/**
 * @brief 本接口用于设置[asc_copy_ub2gm_align](./asc_copy_ub2gm_align/asc_copy_ub2gm_align.md)使用循环搬运模式时，内层循环中相邻两次迭代源操作数数据块起始地址之间的偏移量，以及目的操作数数据块起始地址之间的偏移量。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_ub2gm_loop1_stride.md
 */
__aicore__ inline void asc_set_ub2gm_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride);
/**
 * @brief 本接口用于设置[asc_copy_ub2gm_align](./asc_copy_ub2gm_align/asc_copy_ub2gm_align.md)使用循环搬运模式时，外层循环中相邻两次迭代源操作数数据块起始地址之间的偏移量，以及目的操作数数据块起始地址之间的偏移量。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_ub2gm_loop2_stride.md
 */
__aicore__ inline void asc_set_ub2gm_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride);
/**
 * @brief 本接口用于设置[asc_copy_gm2ub_align](./asc_copy_gm2ub_align/asc_copy_gm2ub_align.md)使用循环搬运模式时的两层循环迭代次数。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_gm2ub_loop_size.md
 */
__aicore__ inline void asc_set_gm2ub_loop_size(uint64_t loop1_size, uint64_t loop2_size);
/**
 * @brief 本接口用于设置[asc_copy_gm2ub_align](./asc_copy_gm2ub_align/asc_copy_gm2ub_align.md)使用循环搬运模式时，内层循环中相邻两次迭代源操作数数据块起始地址之间的偏移量，以及目的操作数数据块起始地址之间的偏移量。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_gm2ub_loop1_stride.md
 */
__aicore__ inline void asc_set_gm2ub_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride);
/**
 * @brief 本接口用于设置[asc_copy_gm2ub_align](./asc_copy_gm2ub_align/asc_copy_gm2ub_align.md)使用循环搬运模式时，外层循环中相邻两次迭代源操作数数据块起始地址之间的偏移量，以及目的操作数数据块起始地址之间的偏移量。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_gm2ub_loop2_stride.md
 */
__aicore__ inline void asc_set_gm2ub_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride);
// ==========asc_set_ndim_loop_stride==========
/**
 * @brief 本接口用于设置[asc_ndim_copy_gm2ub](asc_ndim_copy_gm2ub.md)进行NDDMA多维数据搬运时，`loop0`至`loop4`各循环维度的源操作数步长和目的操作数步长。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_ndim_loop_stride.md
 */
__aicore__ inline void asc_set_ndim_loop0_stride(uint64_t dst_stride, uint64_t src_stride);
__aicore__ inline void asc_set_ndim_loop1_stride(uint64_t dst_stride, uint64_t src_stride);
__aicore__ inline void asc_set_ndim_loop2_stride(uint64_t dst_stride, uint64_t src_stride);
__aicore__ inline void asc_set_ndim_loop3_stride(uint64_t dst_stride, uint64_t src_stride);
__aicore__ inline void asc_set_ndim_loop4_stride(uint64_t dst_stride, uint64_t src_stride);
//===========asc_set_ndim_pad_count===========
/**
 * @brief 本接口用于配置NDDMA高维切分搬运中`loop1`至`loop4`各维左右两侧的填充元素数。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_ndim_pad_count.md
 */
__aicore__ inline void asc_set_ndim_pad_count(asc_ndim_pad_count_config& config);
//===========asc_set_ndim_pad_value(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float)===========
/**
 * @brief 本接口用于设置[asc_ndim_copy_gm2ub](asc_ndim_copy_gm2ub.md)在常数填充模式下使用的填充值。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_set_ndim_pad_value.md
 */
__aicore__ inline void asc_set_ndim_pad_value(int8_t pad_value);
__aicore__ inline void asc_set_ndim_pad_value(uint8_t pad_value);
__aicore__ inline void asc_set_ndim_pad_value(int16_t pad_value);
__aicore__ inline void asc_set_ndim_pad_value(uint16_t pad_value);
__aicore__ inline void asc_set_ndim_pad_value(half pad_value);
__aicore__ inline void asc_set_ndim_pad_value(bfloat16_t pad_value);
__aicore__ inline void asc_set_ndim_pad_value(int32_t pad_value);
__aicore__ inline void asc_set_ndim_pad_value(uint32_t pad_value);
__aicore__ inline void asc_set_ndim_pad_value(float pad_value);
//===========asc_ndim_copy_gm2ub(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float)===========
/**
 * @brief 该接口用于将多维数据从Global Memory（GM）搬运到Unified Buffer（UB）。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_ndim_copy_gm2ub.md
 */
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e8m0_t* dst, __gm__ fp8_e8m0_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ half* dst, __gm__ half* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ float* dst, __gm__ float* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode);
//======asc_ndim_copy_dci=====
/**
 * @brief 刷新NDDMA DataCache。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_ndim_copy_dci.md
 */
__aicore__ inline void asc_ndim_copy_dci();
//======asc_copy_ub2l1=======
/**
 * @brief 提供数据搬运功能，将数据从Unified Buffer（UB）搬运到L1 Buffer。
 * @see docs/zh/api/SIMD-API/c_api/vector_datamove/asc_copy_ub2l1.md
 */
__aicore__ inline void asc_copy_ub2l1(
    __cbuf__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap);
__aicore__ inline void asc_copy_ub2l1(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ half* dst, __ubuf__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ float* dst, __ubuf__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_store_l2_cache_mode)
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e8m0_t* dst, __gm__ fp8_e8m0_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ half* dst, __gm__ half* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_load_l2_cache_mode)
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ float* dst, __gm__ float* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode);

//======asc_set_gm2ub_pad=======
ASC_DEPRECATED(9.1.0, "2027/09/07", asc_set_copy_pad_val)
__aicore__ inline void asc_set_gm2ub_pad(uint32_t pad_val);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_DATAMOVE_VECTOR_DATAMOVE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_DATAMOVE_VECTOR_DATAMOVE_H

#endif
