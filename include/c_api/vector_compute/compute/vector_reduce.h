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
#error "vector_compute/compute/vector_reduce.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_REDUCE_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_REDUCE_H
#define INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_REDUCE_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/vector_reduce_intf_impl.h"
#endif

/**
 * @brief 以内置固定模式对应的二进制或者用户自定义输入的数值对应的gather mask（数据收集的掩码），从源操作数中选取元素写入目的操作数中。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_max_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_min_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_rsvd_count.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_pair_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_reduce.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md
 */
__aicore__ inline void asc_reduce(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint16_t repeat,
    uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride);

__aicore__ inline void asc_reduce(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);

__aicore__ inline void asc_reduce(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint16_t repeat,
    uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride);

__aicore__ inline void asc_reduce(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint32_t count);

/**
 * @brief 此接口用于获取执行[asc_repeat_reduce_max](asc_repeat_reduce_max.md)操作后的最大值，以及第一个最大值时的索引。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_max_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_min_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_rsvd_count.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_pair_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_reduce.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md
 */
__aicore__ inline void asc_get_reduce_max_cnt(half& val, uint32_t& index);

__aicore__ inline void asc_get_reduce_max_cnt(float& val, uint32_t& index);

/**
 * @brief 此接口用于获取执行[asc_repeat_reduce_min](asc_repeat_reduce_min.md)操作后的最小值，以及第一个最小值时的索引。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_max_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_min_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_rsvd_count.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_pair_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_reduce.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md
 */
__aicore__ inline void asc_get_reduce_min_cnt(half& val, uint32_t& index);

__aicore__ inline void asc_get_reduce_min_cnt(float& val, uint32_t& index);

/**
 * @brief 此接口用于获取执行[asc_reduce](asc_reduce.md)操作后剩余的元素数量。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_max_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_min_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_rsvd_count.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_pair_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_reduce.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md
 */
__aicore__ inline int64_t asc_get_rsvd_count();

__aicore__ inline int64_t asc_get_acc_val();

/**
 * @brief 对每个DataBlock内所有元素求和。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_max_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_min_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_rsvd_count.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_pair_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_reduce.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md
 */
__aicore__ inline void asc_datablock_reduce_sum(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_datablock_reduce_sum(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @brief 执行数据块内的求最大值规约（Reduce Max）操作。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_max_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_min_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_rsvd_count.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_pair_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_reduce.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md
 */
__aicore__ inline void asc_datablock_reduce_max(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_datablock_reduce_max(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_datablock_reduce_max(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @brief 执行数据块内的求最小值规约（Reduce Min）操作。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_max_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_min_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_rsvd_count.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_pair_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_reduce.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md
 */
__aicore__ inline void asc_datablock_reduce_min(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_datablock_reduce_min(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_datablock_reduce_min(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @brief 对每个Repeat内所有元素求和。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_max_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_min_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_rsvd_count.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_pair_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_reduce.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md
 */
__aicore__ inline void asc_repeat_reduce_sum(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_sum(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @brief 对每个Repeat内所有元素求最大值，以及Repeat内的首个最大值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_max_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_min_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_rsvd_count.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_pair_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_reduce.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md
 */
__aicore__ inline void asc_repeat_reduce_max_index_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_max_index_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_max_value_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_max_value_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_max_only_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_max_only_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_max_only_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_max_only_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @brief 对每个Repeat内所有元素求最小值，以及Repeat内的首个最小值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_max_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_min_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_rsvd_count.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_pair_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_reduce.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md
 */
__aicore__ inline void asc_repeat_reduce_min_index_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_min_index_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_min_value_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_min_value_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_min_only_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_min_only_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_min_only_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_repeat_reduce_min_only_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @brief 对输入数据做归约操作，得到数据总和。相邻两个（奇偶）元素求和，例如（a1，a2，a3，a4，a5，a6…），相邻两个数据求和为（a1+a2，a3+a4，a5+a6…）。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_max_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_reduce_min_cnt.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_get_rsvd_count.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_pair_reduce_sum.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_reduce.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_max.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_min.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md
 */
__aicore__ inline void asc_pair_reduce_sum(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_pair_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_pair_reduce_sum(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

__aicore__ inline void asc_pair_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_REDUCE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_REDUCE_H
#endif
