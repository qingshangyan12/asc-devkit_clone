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
#error "composite/vector_compute_composite.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_VECTOR_COMPUTE_COMPOSITE_H
#endif

#ifndef INCLUDE_C_API_COMPOSITE_VECTOR_COMPUTE_COMPOSITE_H
#define INCLUDE_C_API_COMPOSITE_VECTOR_COMPUTE_COMPOSITE_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/vector_compute_composite_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/vector_compute_composite_intf_impl.h"
#endif
ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector add for half data: computes dst_i = src0_i + src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_add_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector add for float data: computes dst_i = src0_i + src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_add_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector add for int16_t data: computes dst_i = src0_i + src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_add_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector add for int32_t data: computes dst_i = src0_i + src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_add_sync__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar add for half data: computes dst_i = src_i + value for count elements and writes the results to dst.
 *
 * @override_id asc_add_scalar_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_add_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar add for float data: computes dst_i = src_i + value for count elements and writes the results to dst.
 *
 * @override_id asc_add_scalar_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_add_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar add for int16_t data: computes dst_i = src_i + value for count elements and writes the results to dst.
 *
 * @override_id asc_add_scalar_sync__PUi16_PUi16_i16_u32
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_add_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar add for int32_t data: computes dst_i = src_i + value for count elements and writes the results to dst.
 *
 * @override_id asc_add_scalar_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_add_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_brcb and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync broadcast for uint16_t data: each iteration takes 8 values from src and fills each into one of 8 DataBlocks (32 bytes) of dst.
 *
 * @override_id asc_brcb_sync__PUu16_PUu16_u16_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_brcb_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_brcb and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync broadcast for uint32_t data: each iteration takes 8 values from src and fills each into one of 8 DataBlocks (32 bytes) of dst.
 *
 * @override_id asc_brcb_sync__PUu32_PUu32_u16_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_brcb_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bitsort and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync bitmap sort for half data: each iteration sorts 32 elements; scores in src0 and indexes in src1 are sorted by descending score into dst as (Score, Index) pairs.
 *
 * @override_id asc_bitsort_sync__PUf16_PUf16_PUu32_i32
 * @archs 2201 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 重复迭代次数，每次迭代完成32个元素的排序，下次迭代src0和src1各跳过32个元素，dst跳过256 Byte空间。取值范围：[0, 255]。
 */
__aicore__ inline void asc_bitsort_sync(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bitsort and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync bitmap sort for float data: each iteration sorts 32 elements; scores in src0 and indexes in src1 are sorted by descending score into dst as (Score, Index) pairs.
 *
 * @override_id asc_bitsort_sync__PUf32_PUf32_PUu32_i32
 * @archs 2201 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 重复迭代次数，每次迭代完成32个元素的排序，下次迭代src0和src1各跳过32个元素，dst跳过256 Byte空间。取值范围：[0, 255]。
 */
__aicore__ inline void asc_bitsort_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface transposing a 16*16 two-dimensional matrix data block of int16_t elements; use asc_transpose with asc_sync.
 *
 * @override_id asc_transpose_sync__PUi16_PUi16
 * @archs 2201 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__aicore__ inline void asc_transpose_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_transpose and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface transposing a 16*16 two-dimensional matrix data block of uint16_t elements; use asc_transpose with asc_sync.
 *
 * @override_id asc_transpose_sync__PUu16_PUu16
 * @archs 2201 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__aicore__ inline void asc_transpose_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_datablock_reduce_sum and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync DataBlock-level sum reduction for half data: sums all elements within each DataBlock of src using pairwise binary-tree addition.
 *
 * @override_id asc_datablock_reduce_sum_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_datablock_reduce_sum and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync DataBlock-level sum reduction for float data: sums all elements within each DataBlock of src using pairwise binary-tree addition.
 *
 * @override_id asc_datablock_reduce_sum_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_datablock_reduce_max and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync DataBlock-level maximum reduction for half data: computes the maximum within each DataBlock of src and writes it to dst.
 *
 * @override_id asc_datablock_reduce_max_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_max_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_datablock_reduce_max and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync DataBlock-level maximum reduction for float data: computes the maximum within each DataBlock of src and writes it to dst.
 *
 * @override_id asc_datablock_reduce_max_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_max_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_datablock_reduce_min and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync DataBlock-level minimum reduction for half data: computes the minimum within each DataBlock of src and writes it to dst.
 *
 * @override_id asc_datablock_reduce_min_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_min_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_datablock_reduce_min and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync DataBlock-level minimum reduction for float data: computes the minimum within each DataBlock of src and writes it to dst.
 *
 * @override_id asc_datablock_reduce_min_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_min_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_sum and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface summing all elements within each Repeat via binary-tree pairwise addition for half data; use asc_repeat_reduce_sum and asc_sync.
 *
 * @override_id asc_repeat_reduce_sum_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_sum and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface summing all elements within each Repeat via binary-tree pairwise addition for float data; use asc_repeat_reduce_sum and asc_sync.
 *
 * @override_id asc_repeat_reduce_sum_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_max_index_value and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat maximum reduction writing [index, value] to the dst output parameter with a uint16 index for half input; returns nothing.
 *
 * @override_id asc_repeat_reduce_max_index_value_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_index_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_max_index_value and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat maximum reduction writing [index, value] to the dst output parameter with a uint32 index for float input; returns nothing.
 *
 * @override_id asc_repeat_reduce_max_index_value_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_index_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_max_value_index and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat maximum reduction writing [value, index] to the dst output parameter with a uint16 index for half input; returns nothing.
 *
 * @override_id asc_repeat_reduce_max_value_index_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_value_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_max_value_index and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat maximum reduction writing [value, index] to the dst output parameter with a uint32 index for float input; returns nothing.
 *
 * @override_id asc_repeat_reduce_max_value_index_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_value_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_max_only_value and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat maximum reduction writing only the maximum value [value] for half input to the dst output parameter; returns nothing.
 *
 * @override_id asc_repeat_reduce_max_only_value_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_only_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_max_only_value and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat maximum reduction writing only the maximum value [value] for float input to the dst output parameter; returns nothing.
 *
 * @override_id asc_repeat_reduce_max_only_value_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_only_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_max_only_index and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat maximum reduction writing only the index [index] of the first maximum for half input to the dst output parameter; index type fixed uint32.
 *
 * @override_id asc_repeat_reduce_max_only_index_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_only_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_max_only_index and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat maximum reduction writing only the index [index] of the first maximum for float input to the dst output parameter; index type fixed uint32.
 *
 * @override_id asc_repeat_reduce_max_only_index_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_only_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_min_index_value and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat minimum reduction writing [index, value] to the dst output parameter with a uint16 index for half input; returns nothing.
 *
 * @override_id asc_repeat_reduce_min_index_value_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_index_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_min_index_value and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat minimum reduction writing [index, value] to the dst output parameter with a uint32 index for float input; returns nothing.
 *
 * @override_id asc_repeat_reduce_min_index_value_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_index_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_min_value_index and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat minimum reduction writing [value, index] to the dst output parameter with a uint16 index for half input; returns nothing.
 *
 * @override_id asc_repeat_reduce_min_value_index_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_value_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_min_value_index and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat minimum reduction writing [value, index] to the dst output parameter with a uint32 index for float input; returns nothing.
 *
 * @override_id asc_repeat_reduce_min_value_index_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_value_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_min_only_value and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat minimum reduction writing only the minimum value [value] for half input to the dst output parameter; returns nothing.
 *
 * @override_id asc_repeat_reduce_min_only_value_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_only_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_min_only_value and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat minimum reduction writing only the minimum value [value] for float input to the dst output parameter; returns nothing.
 *
 * @override_id asc_repeat_reduce_min_only_value_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_only_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_min_only_index and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat minimum reduction writing only the index [index] of the first minimum for half input to the dst output parameter; index type fixed uint32.
 *
 * @override_id asc_repeat_reduce_min_only_index_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_only_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_repeat_reduce_min_only_index and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous per-Repeat minimum reduction writing only the index [index] of the first minimum for float input to the dst output parameter; index type fixed uint32.
 *
 * @override_id asc_repeat_reduce_min_only_index_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_only_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_eq and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API comparing half elements at corresponding indices of src0 and src1, storing the 1/0 comparison bits in a compare register; void return.
 *
 * @override_id asc_eq_sync__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_eq_sync(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_eq and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API comparing float elements at corresponding indices of src0 and src1, storing the 1/0 comparison bits in a compare register; void return.
 *
 * @override_id asc_eq_sync__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_eq_sync(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_eq and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API comparing half elements at corresponding indices of src0 and src1, storing the 1/0 comparison bits into output parameter dst via void return.
 *
 * @override_id asc_eq_sync__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_eq_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_eq and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API comparing float elements at corresponding indices of src0 and src1, storing the 1/0 comparison bits into output parameter dst via void return.
 *
 * @override_id asc_eq_sync__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_eq_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_eq and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API comparing int32_t elements at corresponding indices of src0 and src1, storing the 1/0 comparison bits into output parameter dst via void return.
 *
 * @override_id asc_eq_sync__PUu8_PUi32_PUi32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_eq_sync(
    __ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162float and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface converting bfloat16_t data to float with no rounding mode; use asc_bfloat162float with asc_sync.
 *
 * @override_id asc_bfloat162float_sync__PUf32_PUbf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162float_sync(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting bfloat16_t elements to int32_t with ROUND (round half away from zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_bfloat162int32_rna_sync__PUi32_PUbf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting bfloat16_t elements to int32_t with CEIL (toward positive infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_bfloat162int32_ru_sync__PUi32_PUbf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting bfloat16_t elements to int32_t with FLOOR (toward negative infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_bfloat162int32_rd_sync__PUi32_PUbf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting bfloat16_t elements to int32_t with RINT (round half to even) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_bfloat162int32_rn_sync__PUi32_PUbf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_bfloat162int32_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting bfloat16_t elements to int32_t with TRUNC (toward zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_bfloat162int32_rz_sync__PUi32_PUbf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to bfloat16_t with RINT (round half to even) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2bfloat16_rn_sync__PUbf16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_rn_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to bfloat16_t with ROUND (round half away from zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2bfloat16_rna_sync__PUbf16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_rna_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to bfloat16_t with FLOOR (toward negative infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2bfloat16_rd_sync__PUbf16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_rd_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to bfloat16_t with CEIL (toward positive infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2bfloat16_ru_sync__PUbf16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_ru_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2bfloat16_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to bfloat16_t with TRUNC (toward zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2bfloat16_rz_sync__PUbf16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_rz_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2float_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API performing precision conversion on float elements with RINT (round half to even) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2float_rn_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_rn_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2float_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API performing precision conversion on float elements with FLOOR (toward negative infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2float_rd_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_rd_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2float_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API performing precision conversion on float elements with CEIL (toward positive infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2float_ru_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_ru_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2float_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API performing precision conversion on float elements with ROUND (round half away from zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2float_rna_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_rna_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2float_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API performing precision conversion on float elements with TRUNC (toward zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2float_rz_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_rz_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous float-to-half conversion using RINT rounding only when precision is lost and no rounding otherwise, writing to the dst output parameter; returns nothing.
 *
 * @override_id asc_float2half_sync__PUf16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous float-to-half conversion using RINT rounding (round half to even), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_float2half_rn_sync__PUf16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_rn_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous float-to-half conversion using ROUND rounding (half up), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_float2half_rna_sync__PUf16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_rna_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous float-to-half conversion using FLOOR rounding (toward negative infinity), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_float2half_rd_sync__PUf16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_rd_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous float-to-half conversion using CEIL rounding (toward positive infinity), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_float2half_ru_sync__PUf16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_ru_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous float-to-half conversion using TRUNC rounding (toward zero), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_float2half_rz_sync__PUf16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_rz_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2half_ro and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous float-to-half conversion using ODD rounding (Von Neumann, round to nearest odd), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_float2half_ro_sync__PUf16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_ro_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int32_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int32_t with ROUND (round half away from zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int32_rna_sync__PUi32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int32_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int32_t with CEIL (toward positive infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int32_ru_sync__PUi32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int32_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int32_t with FLOOR (toward negative infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int32_rd_sync__PUi32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int32_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int32_t with RINT (round half to even) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int32_rn_sync__PUi32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int32_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int32_t with TRUNC (toward zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int32_rz_sync__PUi32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int64_t with ROUND (round half away from zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int64_rna_sync__PUi64_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_rna_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int64_t with CEIL (toward positive infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int64_ru_sync__PUi64_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_ru_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int64_t with FLOOR (toward negative infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int64_rd_sync__PUi64_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_rd_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int64_t with RINT (round half to even) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int64_rn_sync__PUi64_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_rn_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int64_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int64_t with TRUNC (toward zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int64_rz_sync__PUi64_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_rz_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2float and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface converting half data to float with no rounding mode; use asc_half2float with asc_sync.
 *
 * @override_id asc_half2float_sync__PUf32_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2float_sync(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half-to-int4 conversion using RINT rounding only when precision is lost and no rounding otherwise, writing to the dst output parameter; returns nothing.
 *
 * @override_id asc_half2int4_sync__PUi4x2_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half-to-int4 conversion using ROUND rounding (half up), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_half2int4_rna_sync__PUi4x2_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_rna_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half-to-int4 conversion using CEIL rounding (toward positive infinity), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_half2int4_ru_sync__PUi4x2_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_ru_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half-to-int4 conversion using FLOOR rounding (toward negative infinity), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_half2int4_rd_sync__PUi4x2_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_rd_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half-to-int4 conversion using RINT rounding (round half to even), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_half2int4_rn_sync__PUi4x2_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_rn_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int4_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half-to-int4 conversion using TRUNC rounding (toward zero), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_half2int4_rz_sync__PUi4x2_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_rz_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half-to-int8 conversion with no specified rounding mode, writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_half2int8_sync__PUi8_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half-to-int8 conversion using ROUND rounding (half up), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_half2int8_rna_sync__PUi8_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_rna_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half-to-int8 conversion using CEIL rounding (toward positive infinity), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_half2int8_ru_sync__PUi8_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_ru_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half-to-int8 conversion using FLOOR rounding (toward negative infinity), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_half2int8_rd_sync__PUi8_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_rd_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half-to-int8 conversion using RINT rounding (round half to even), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_half2int8_rn_sync__PUi8_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_rn_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int8_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous half-to-int8 conversion using TRUNC rounding (toward zero), writing the results to the dst output parameter; returns nothing.
 *
 * @override_id asc_half2int8_rz_sync__PUi8_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_rz_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int16_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting half elements to int16_t with ROUND (round half away from zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_half2int16_rna_sync__PUi16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_rna_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int16_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting half elements to int16_t with CEIL (toward positive infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_half2int16_ru_sync__PUi16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_ru_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int16_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting half elements to int16_t with FLOOR (toward negative infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_half2int16_rd_sync__PUi16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_rd_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int16_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting half elements to int16_t with RINT (round half to even) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_half2int16_rn_sync__PUi16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_rn_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int16_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting half elements to int16_t with TRUNC (toward zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_half2int16_rz_sync__PUi16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_rz_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int32_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync conversion: converts count half elements from src to int32_t using ROUND (round half up) rounding and writes the results to dst.
 *
 * @override_id asc_half2int32_rna_sync__PUi32_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int32_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync conversion: converts count half elements from src to int32_t using CEIL (round toward positive infinity) rounding and writes the results to dst.
 *
 * @override_id asc_half2int32_ru_sync__PUi32_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int32_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync conversion: converts count half elements from src to int32_t using FLOOR (round toward negative infinity) rounding and writes the results to dst.
 *
 * @override_id asc_half2int32_rd_sync__PUi32_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int32_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync conversion: converts count half elements from src to int32_t using RINT (round half to even) rounding and writes the results to dst.
 *
 * @override_id asc_half2int32_rn_sync__PUi32_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2int32_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync conversion: converts count half elements from src to int32_t using TRUNC (round toward zero) rounding and writes the results to dst.
 *
 * @override_id asc_half2int32_rz_sync__PUi32_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_div and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector division for half data: computes dst_i = src0_i / src1_i for count elements; take care of divide-by-zero.
 *
 * @override_id asc_div_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址
 * @param [in] src0 源操作数（矢量）的起始地址
 * @param [in] src1 源操作数（矢量）的起始地址
 * @param [in] count 参与连续计算的元素个数
 */
__aicore__ inline void asc_div_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_div and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector division for float data: computes dst_i = src0_i / src1_i for count elements; take care of divide-by-zero.
 *
 * @override_id asc_div_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址
 * @param [in] src0 源操作数（矢量）的起始地址
 * @param [in] src1 源操作数（矢量）的起始地址
 * @param [in] count 参与连续计算的元素个数
 */
__aicore__ inline void asc_div_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_duplicate and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous duplicate that fills the dst output parameter with a scalar half src value for count elements; replaced by asc_duplicate plus asc_sync.
 *
 * @override_id asc_duplicate_sync__PUf16_f16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ half* dst, half src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_duplicate and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous duplicate that fills the dst output parameter with a scalar int16 src value for count elements; replaced by asc_duplicate plus asc_sync.
 *
 * @override_id asc_duplicate_sync__PUi16_i16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ int16_t* dst, int16_t src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_duplicate and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous duplicate that fills the dst output parameter with a scalar uint16 src value for count elements; replaced by asc_duplicate plus asc_sync.
 *
 * @override_id asc_duplicate_sync__PUu16_u16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_duplicate and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous duplicate that fills the dst output parameter with a scalar bfloat16 src value for count elements; replaced by asc_duplicate plus asc_sync.
 *
 * @override_id asc_duplicate_sync__PUbf16_bf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_duplicate and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous duplicate that fills the dst output parameter with a scalar float src value for count elements; replaced by asc_duplicate plus asc_sync.
 *
 * @override_id asc_duplicate_sync__PUf32_f32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ float* dst, float src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_duplicate and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous duplicate that fills the dst output parameter with a scalar int32 src value for count elements; replaced by asc_duplicate plus asc_sync.
 *
 * @override_id asc_duplicate_sync__PUi32_i32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ int32_t* dst, int32_t src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_duplicate and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous duplicate that fills the dst output parameter with a scalar uint32 src value for count elements; replaced by asc_duplicate plus asc_sync.
 *
 * @override_id asc_duplicate_sync__PUu32_u32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_exp and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync elementwise natural exponential for half data: computes dst_i = e^src_i for count elements and writes the results to dst.
 *
 * @override_id asc_exp_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_exp_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_exp and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync elementwise natural exponential for float data: computes dst_i = e^src_i for count elements and writes the results to dst.
 *
 * @override_id asc_exp_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_exp_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_log and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync elementwise natural logarithm for half data: computes dst_i = ln(src_i) for count elements and writes the results to dst.
 *
 * @override_id asc_log_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_log_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_log and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync elementwise natural logarithm for float data: computes dst_i = ln(src_i) for count elements and writes the results to dst.
 *
 * @override_id asc_log_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_log_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gt_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for half: sets 1 bit per element in uint8_t dst for src > value (1 true, 0 false), little-endian.
 *
 * @override_id asc_gt_scalar_sync__PUu8_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_gt_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gt_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for float: sets 1 bit per element in uint8_t dst for src > value (1 true, 0 false), little-endian.
 *
 * @override_id asc_gt_scalar_sync__PUu8_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_gt_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_abs and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync elementwise absolute value for half data: computes dst_i = |src_i| for count elements and writes the results to dst.
 *
 * @override_id asc_abs_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_abs_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_abs and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync elementwise absolute value for float data: computes dst_i = |src_i| for count elements and writes the results to dst.
 *
 * @override_id asc_abs_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_abs_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_shiftleft and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync left shift for int16_t data: computes dst_i = src_i << distance for count elements; distance must be non-negative and within the data bit width.
 *
 * @override_id asc_shiftleft_sync__PUi16_PUi16_u32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] distance 源操作数（标量），表示左移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftleft_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_shiftleft and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync left shift for uint16_t data: computes dst_i = src_i << distance for count elements; distance must be non-negative and within the data bit width.
 *
 * @override_id asc_shiftleft_sync__PUu16_PUu16_u32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] distance 源操作数（标量），表示左移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftleft_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_shiftleft and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync left shift for int32_t data: computes dst_i = src_i << distance for count elements; distance must be non-negative and within the data bit width.
 *
 * @override_id asc_shiftleft_sync__PUi32_PUi32_u32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] distance 源操作数（标量），表示左移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftleft_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_shiftleft and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync left shift for uint32_t data: computes dst_i = src_i << distance for count elements; distance must be non-negative and within the data bit width.
 *
 * @override_id asc_shiftleft_sync__PUu32_PUu32_u32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] distance 源操作数（标量），表示左移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftleft_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_cast_half2int8 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface multiplying half elements and converting the product to int8_t with RINT rounding and saturation; use asc_mul_cast_half2int8 with asc_sync.
 *
 * @override_id asc_mul_cast_half2int8_sync__PUi8_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_cast_half2int8_sync(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_cast_half2uint8 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface multiplying half elements and converting the product to uint8_t with RINT rounding and saturation; use asc_mul_cast_half2uint8 with asc_sync.
 *
 * @override_id asc_mul_cast_half2uint8_sync__PUu8_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数0（矢量）的起始地址。
 * @param [in] src1 源操作数1（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_cast_half2uint8_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_ne and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on half operands: result bits for src0 != src1 (1 true, 0 false) are stored to a register read by asc_get_cmp_mask.
 *
 * @override_id asc_ne_sync__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride dst单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride src0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride src1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride dst相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride src0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride src1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_ne_sync(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_ne and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on float operands: result bits for src0 != src1 (1 true, 0 false) are stored to a register read by asc_get_cmp_mask.
 *
 * @override_id asc_ne_sync__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride dst单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride src0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride src1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride dst相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride src0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride src1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_ne_sync(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_ne and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on half operands: writes 1 bit per element for src0 != src1 (1 true, 0 false) into uint8_t dst, little-endian.
 *
 * @override_id asc_ne_sync__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride dst单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride src0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride src1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride dst相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride src0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride src1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_ne_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_ne and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on float operands: writes 1 bit per element for src0 != src1 (1 true, 0 false) into uint8_t dst, little-endian.
 *
 * @override_id asc_ne_sync__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride dst单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride src0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride src1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride dst相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride src0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride src1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_ne_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_ge_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for half: sets 1 bit per element in uint8_t dst for src >= value (1 true, 0 false), little-endian.
 *
 * @override_id asc_ge_scalar_sync__PUu8_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_ge_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_ge_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for float: sets 1 bit per element in uint8_t dst for src >= value (1 true, 0 false), little-endian.
 *
 * @override_id asc_ge_scalar_sync__PUu8_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_ge_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_deq_int322half and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface quantizing int32_t elements to half via dst = Cast(src * scale) with asc_set_deq_scale; use asc_deq_int322half with asc_sync.
 *
 * @override_id asc_deq_int322half_sync__PUf16_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_deq_int322half_sync(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mrgsort4 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync merge sort for half data: merges up to 4 sorted queues into 1 queue ordered by descending score; input usually comes from asc_bitsort.
 *
 * @override_id asc_mrgsort4_sync__PUf16_AmrgPUf16_u8_u16_u16_u16_u16_b_u8
 * @archs 2201 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数，每一次源操作数和目的操作数计算并跳过四个队列总长度。
 * @param [in] element_length_0 源队列0的长度。
 * @param [in] element_length_1 源队列1的长度。
 * @param [in] element_length_2 源队列2的长度。
 * @param [in] element_length_3 源队列3的长度。
 * @param [in] if_exhausted_suspension 某条队列耗尽后，指令是否需要停止。
 * @param [in] valid_bit 有效队列个数。有效取值如下： 3：前两条队列有效。 7：前三条队列有效。 15：四条队列全部有效。
 */
__aicore__ inline void asc_mrgsort4_sync(
    __ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mrgsort4 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync merge sort for float data: merges up to 4 sorted queues into 1 queue ordered by descending score; input usually comes from asc_bitsort.
 *
 * @override_id asc_mrgsort4_sync__PUf32_AmrgPUf32_u8_u16_u16_u16_u16_b_u8
 * @archs 2201 3510
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数，每一次源操作数和目的操作数计算并跳过四个队列总长度。
 * @param [in] element_length_0 源队列0的长度。
 * @param [in] element_length_1 源队列1的长度。
 * @param [in] element_length_2 源队列2的长度。
 * @param [in] element_length_3 源队列3的长度。
 * @param [in] if_exhausted_suspension 某条队列耗尽后，指令是否需要停止。
 * @param [in] valid_bit 有效队列个数。有效取值如下： 3：前两条队列有效。 7：前三条队列有效。 15：四条队列全部有效。
 */
__aicore__ inline void asc_mrgsort4_sync(
    __ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync elementwise ReLU for half data: computes dst_i = relu(src_i) for count elements and writes the results to dst.
 *
 * @override_id asc_relu_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_relu_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync elementwise ReLU for float data: computes dst_i = relu(src_i) for count elements and writes the results to dst.
 *
 * @override_id asc_relu_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_relu_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync elementwise ReLU for int32_t data: computes dst_i = relu(src_i) for count elements and writes the results to dst.
 *
 * @override_id asc_relu_sync__PUi32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_relu_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_max and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector maximum for half data: computes dst_i = max(src0_i, src1_i) for count elements and writes the results to dst.
 *
 * @override_id asc_max_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_max_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_max and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector maximum for float data: computes dst_i = max(src0_i, src1_i) for count elements and writes the results to dst.
 *
 * @override_id asc_max_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_max_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_max and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector maximum for int16_t data: computes dst_i = max(src0_i, src1_i) for count elements and writes the results to dst.
 *
 * @override_id asc_max_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_max_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_max and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector maximum for int32_t data: computes dst_i = max(src0_i, src1_i) for count elements and writes the results to dst.
 *
 * @override_id asc_max_sync__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_max_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_max_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar maximum for half data: computes dst_i = max(src_i, value) for count elements and writes the results to dst.
 *
 * @override_id asc_max_scalar_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_max_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_max_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar maximum for float data: computes dst_i = max(src_i, value) for count elements and writes the results to dst.
 *
 * @override_id asc_max_scalar_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_max_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_max_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar maximum for int16_t data: computes dst_i = max(src_i, value) for count elements and writes the results to dst.
 *
 * @override_id asc_max_scalar_sync__PUi16_PUi16_i16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_max_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_max_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar maximum for int32_t data: computes dst_i = max(src_i, value) for count elements and writes the results to dst.
 *
 * @override_id asc_max_scalar_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_max_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_min and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector minimum for half data: computes dst_i = min(src0_i, src1_i) for count elements and writes the results to dst.
 *
 * @override_id asc_min_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_min and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector minimum for float data: computes dst_i = min(src0_i, src1_i) for count elements and writes the results to dst.
 *
 * @override_id asc_min_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_min and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector minimum for int16_t data: computes dst_i = min(src0_i, src1_i) for count elements and writes the results to dst.
 *
 * @override_id asc_min_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_min and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector minimum for int32_t data: computes dst_i = min(src0_i, src1_i) for count elements and writes the results to dst.
 *
 * @override_id asc_min_sync__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fma and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fused multiply-add for half sources and half dst: computes dst_i = (src0_i * src1_i) + dst_i for count elements.
 *
 * @override_id asc_fma_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_fma_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fma and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fused multiply-add for float sources and float dst: computes dst_i = (src0_i * src1_i) + dst_i for count elements.
 *
 * @override_id asc_fma_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_fma_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_fma and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fused multiply-add for half sources and float dst: computes dst_i = (src0_i * src1_i) + dst_i for count elements.
 *
 * @override_id asc_fma_sync__PUf32_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_fma_sync(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector multiply for half data: computes dst_i = src0_i * src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_mul_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector multiply for float data: computes dst_i = src0_i * src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_mul_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector multiply for int16_t data: computes dst_i = src0_i * src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_mul_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector multiply for int32_t data: computes dst_i = src0_i * src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_mul_sync__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar multiply for half data: computes dst_i = src_i * value for count elements and writes the results to dst.
 *
 * @override_id asc_mul_scalar_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar multiply for float data: computes dst_i = src_i * value for count elements and writes the results to dst.
 *
 * @override_id asc_mul_scalar_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar multiply for int16_t data: computes dst_i = src_i * value for count elements and writes the results to dst.
 *
 * @override_id asc_mul_scalar_sync__PUi16_PUi16_i16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar multiply for int32_t data: computes dst_i = src_i * value for count elements and writes the results to dst.
 *
 * @override_id asc_mul_scalar_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_select and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface selecting each element from src0 or src1 by asc_set_cmp_mask bits (1: src0, 0: src1) for half data; use asc_select with asc_sync.
 *
 * @override_id asc_select_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数0（矢量）的起始地址。
 * @param [in] src1 源操作数1（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_select_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_select and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface selecting each element from src0 or src1 by asc_set_cmp_mask bits (1: src0, 0: src1) for float data; use asc_select with asc_sync.
 *
 * @override_id asc_select_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数0（矢量）的起始地址。
 * @param [in] src1 源操作数1（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_select_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector subtraction for half data: computes dst_i = src0_i - src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_sub_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector subtraction for float data: computes dst_i = src0_i - src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_sub_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector subtraction for int16_t data: computes dst_i = src0_i - src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_sub_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector subtraction for int32_t data: computes dst_i = src0_i - src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_sub_sync__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar subtraction for half data: computes dst_i = src_i - value for count elements and writes the results to dst.
 *
 * @override_id asc_sub_scalar_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar subtraction for float data: computes dst_i = src_i - value for count elements and writes the results to dst.
 *
 * @override_id asc_sub_scalar_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar subtraction for int16_t data: computes dst_i = src_i - value for count elements and writes the results to dst.
 *
 * @override_id asc_sub_scalar_sync__PUi16_PUi16_i16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar subtraction for int32_t data: computes dst_i = src_i - value for count elements and writes the results to dst.
 *
 * @override_id asc_sub_scalar_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_or and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync bitwise OR for int16_t data: computes dst_i = src0_i | src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_or_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_or_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_or and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync bitwise OR for uint16_t data: computes dst_i = src0_i | src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_or_sync__PUu16_PUu16_PUu16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_or_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_not and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync bitwise NOT for int16_t data: computes dst_i = ~src_i for count elements and writes the results to dst.
 *
 * @override_id asc_not_sync__PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_not_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_not and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync bitwise NOT for uint16_t data: computes dst_i = ~src_i for count elements and writes the results to dst.
 *
 * @override_id asc_not_sync__PUu16_PUu16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_not_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_rcp and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface computing the element-wise reciprocal dst = 1/src for half data; use asc_rcp with asc_sync.
 *
 * @override_id asc_rcp_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_rcp_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_rcp and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface computing the element-wise reciprocal dst = 1/src for float data; use asc_rcp with asc_sync.
 *
 * @override_id asc_rcp_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_rcp_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_shiftright and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API arithmetically right-shifting each int16_t source element by value bits, writing results to output parameter dst via void return.
 *
 * @override_id asc_shiftright_sync__PUi16_PUi16_i32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量），表示右移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftright_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_shiftright and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API arithmetically right-shifting each int32_t source element by value bits, writing results to output parameter dst via void return.
 *
 * @override_id asc_shiftright_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量），表示右移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftright_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_shiftright and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API logically right-shifting each uint16_t source element by value bits, writing results to output parameter dst via void return.
 *
 * @override_id asc_shiftright_sync__PUu16_PUu16_u32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量），表示右移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftright_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_shiftright and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API logically right-shifting each uint32_t source element by value bits, writing results to output parameter dst via void return.
 *
 * @override_id asc_shiftright_sync__PUu32_PUu32_u32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量），表示右移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftright_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_shiftright_round and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API arithmetically right-shifting each int16_t source element by value bits with rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_shiftright_round_sync__PUi16_PUi16_i32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量），表示右移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftright_round_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_shiftright_round and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API arithmetically right-shifting each int32_t source element by value bits with rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_shiftright_round_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量），表示右移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftright_round_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_add and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync multiply-add for half data: computes dst_i = (src0_i * dst_i) + src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_mul_add_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_add_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_add and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync multiply-add for float data: computes dst_i = (src0_i * dst_i) + src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_mul_add_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_add_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_add_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fused multiply-add-ReLU for half data: computes dst_i = relu((src0_i * dst_i) + src1_i) for count elements and writes to dst.
 *
 * @override_id asc_mul_add_relu_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_add_relu_sync(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_mul_add_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync fused multiply-add-ReLU for float data: computes dst_i = relu((src0_i * dst_i) + src1_i) for count elements and writes to dst.
 *
 * @override_id asc_mul_add_relu_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_add_relu_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_eq_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for half: sets 1 bit per element in uint8_t dst for src == value (1 true, 0 false), little-endian.
 *
 * @override_id asc_eq_scalar_sync__PUu8_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_eq_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_eq_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for float: sets 1 bit per element in uint8_t dst for src == value (1 true, 0 false), little-endian.
 *
 * @override_id asc_eq_scalar_sync__PUu8_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_eq_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_eq_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for int32_t: sets 1 bit per element in uint8_t dst for src == value (1 true, 0 false), little-endian.
 *
 * @override_id asc_eq_scalar_sync__PUu8_PUi32_i32_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_eq_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_deq_int162b8_h and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync quantization: converts count int16_t elements to int8_t stored in the high half of each DataBlock; call asc_set_deq_scale for parameters first.
 *
 * @override_id asc_deq_int162b8_h_sync__PUi8_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_deq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_deq_int162b8_h and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync quantization: converts count int16_t elements to uint8_t stored in the high half of each DataBlock; call asc_set_deq_scale for parameters first.
 *
 * @override_id asc_deq_int162b8_h_sync__PUu8_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_deq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_deq_int162b8_l and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync quantization: converts count int16_t elements to int8_t stored in the low half of each DataBlock; call asc_set_deq_scale for parameters first.
 *
 * @override_id asc_deq_int162b8_l_sync__PUi8_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_deq_int162b8_l and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync quantization: converts count int16_t elements to uint8_t stored in the low half of each DataBlock; call asc_set_deq_scale for parameters first.
 *
 * @override_id asc_deq_int162b8_l_sync__PUu8_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int42half and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface converting int4_t data to half; use asc_int42half with asc_sync.
 *
 * @override_id asc_int42half_sync__PUf16_PUi4x2_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int42half_sync(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int82half and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface converting int8_t data to half; use asc_int82half with asc_sync.
 *
 * @override_id asc_int82half_sync__PUf16_PUi8_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int82half_sync(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_uint82half and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface converting uint8_t data to half; use asc_uint82half with asc_sync.
 *
 * @override_id asc_uint82half_sync__PUf16_PUu8_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_uint82half_sync(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int162float and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface converting int16_t data to float; use asc_int162float with asc_sync.
 *
 * @override_id asc_int162float_sync__PUf32_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162float_sync(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322float and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting int32_t elements to float with no rounding (RINT when lossy), writing results to output parameter dst via void return.
 *
 * @override_id asc_int322float_sync__PUf32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322float_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting int32_t elements to float with ROUND (round half away from zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_int322float_rna_sync__PUf32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_rna_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322float_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting int32_t elements to float with CEIL (toward positive infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_int322float_ru_sync__PUf32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_ru_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322float_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting int32_t elements to float with FLOOR (toward negative infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_int322float_rd_sync__PUf32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_rd_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322float_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting int32_t elements to float with RINT (round half to even) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_int322float_rn_sync__PUf32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_rn_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322float_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting int32_t elements to float with TRUNC (toward zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_int322float_rz_sync__PUf32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_rz_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322int16 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface converting int32_t data to int16_t; use asc_int322int16 with asc_sync.
 *
 * @override_id asc_int322int16_sync__PUi16_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322int16_sync(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int322int64 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface converting int32_t data to int64_t; use asc_int322int64 with asc_sync.
 *
 * @override_id asc_int322int64_sync__PUi64_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322int64_sync(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642int32 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface converting int64_t data to int32_t; use asc_int642int32 with asc_sync.
 *
 * @override_id asc_int642int32_sync__PUi32_PUi64_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int642int32_sync(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gather and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous gather that collects int16 elements from src into the dst output parameter using the byte offsets given in src_offset; returns nothing.
 *
 * @override_id asc_gather_sync__PUi16_PUi16_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gather and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous gather that collects uint16 elements from src into the dst output parameter using the byte offsets given in src_offset; returns nothing.
 *
 * @override_id asc_gather_sync__PUu16_PUu16_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gather and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous gather that collects half elements from src into the dst output parameter using the byte offsets given in src_offset; returns nothing.
 *
 * @override_id asc_gather_sync__PUf16_PUf16_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gather and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous gather that collects bfloat16 elements from src into the dst output parameter using the byte offsets given in src_offset; returns nothing.
 *
 * @override_id asc_gather_sync__PUbf16_PUbf16_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gather and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous gather that collects int32 elements from src into the dst output parameter using the byte offsets given in src_offset; returns nothing.
 *
 * @override_id asc_gather_sync__PUi32_PUi32_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gather and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous gather that collects uint32 elements from src into the dst output parameter using the byte offsets given in src_offset; returns nothing.
 *
 * @override_id asc_gather_sync__PUu32_PUu32_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gather and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous gather that collects float elements from src into the dst output parameter using the byte offsets given in src_offset; returns nothing.
 *
 * @override_id asc_gather_sync__PUf32_PUf32_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_and and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync bitwise AND for int16_t data: computes dst_i = src0_i & src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_and_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_and_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_and and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync bitwise AND for uint16_t data: computes dst_i = src0_i & src1_i for count elements and writes the results to dst.
 *
 * @override_id asc_and_sync__PUu16_PUu16_PUu16_u32
 * @archs 2201
 * @param [out] dst 目的操作数。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_and_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_leakyrelu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync Leaky ReLU for half data with scalar alpha: outputs src when src > 0, otherwise alpha * src, for count elements.
 *
 * @override_id asc_leakyrelu_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址，为待处理数据。
 * @param [in] value 源操作数（标量），leaky_relu中alpha的值。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_leakyrelu_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_leakyrelu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync Leaky ReLU for float data with scalar alpha: outputs src when src > 0, otherwise alpha * src, for count elements.
 *
 * @override_id asc_leakyrelu_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址，为待处理数据。
 * @param [in] value 源操作数（标量），leaky_relu中alpha的值。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_leakyrelu_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_lt_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for half: sets 1 bit per element in uint8_t dst for src < value (1 true, 0 false), little-endian.
 *
 * @override_id asc_lt_scalar_sync__PUu8_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_lt_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_lt_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for float: sets 1 bit per element in uint8_t dst for src < value (1 true, 0 false), little-endian.
 *
 * @override_id asc_lt_scalar_sync__PUu8_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_lt_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API computing relu(src0 - src1) element-wise on half vectors, storing results into output parameter dst via void return.
 *
 * @override_id asc_sub_relu_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_relu_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API computing relu(src0 - src1) element-wise on float vectors, storing results into output parameter dst via void return.
 *
 * @override_id asc_sub_relu_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API computing relu(src0 - src1) element-wise on int16_t vectors, storing results into output parameter dst via void return.
 *
 * @override_id asc_sub_relu_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API computing relu(src0 - src1) element-wise on half vectors and converting the result to int8_t (f162s8) in output parameter dst via void return.
 *
 * @override_id asc_sub_relu_sync__PUi8_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API computing relu(src0 - src1) element-wise on float vectors and converting the result to half (f322f16) in output parameter dst via void return.
 *
 * @override_id asc_sub_relu_sync__PUf16_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sub_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API computing relu(src0 - src1) element-wise on int16_t vectors and converting the result to int8_t (s162s8) in output parameter dst via void return.
 *
 * @override_id asc_sub_relu_sync__PUi8_PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int16_t with ROUND (round half away from zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int16_rna_sync__PUi16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_rna_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int16_t with CEIL (toward positive infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int16_ru_sync__PUi16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_ru_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int16_t with FLOOR (toward negative infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int16_rd_sync__PUi16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_rd_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int16_t with RINT (round half to even) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int16_rn_sync__PUi16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_rn_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_float2int16_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting float elements to int16_t with TRUNC (toward zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_float2int16_rz_sync__PUi16_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_rz_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_rsqrt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface computing the element-wise reciprocal square root dst_i = 1/sqrt(src_i) for half data; use asc_rsqrt with asc_sync.
 *
 * @override_id asc_rsqrt_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_rsqrt_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_rsqrt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface computing the element-wise reciprocal square root dst_i = 1/sqrt(src_i) for float data; use asc_rsqrt with asc_sync.
 *
 * @override_id asc_rsqrt_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_rsqrt_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous element-wise add followed by ReLU on half vectors, writing relu(src0+src1) to the dst output parameter; returns nothing.
 *
 * @override_id asc_add_relu_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_relu_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous element-wise add followed by ReLU on float vectors, writing relu(src0+src1) to the dst output parameter; returns nothing.
 *
 * @override_id asc_add_relu_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous element-wise add followed by ReLU on int16 vectors, writing relu(src0+src1) to the dst output parameter; returns nothing.
 *
 * @override_id asc_add_relu_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_ge and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on half operands: result bits for src0 >= src1 (1 true, 0 false) are stored to a register read by asc_get_cmp_mask.
 *
 * @override_id asc_ge_sync__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_ge_sync(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_ge and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on float operands: result bits for src0 >= src1 (1 true, 0 false) are stored to a register read by asc_get_cmp_mask.
 *
 * @override_id asc_ge_sync__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_ge_sync(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_ge and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on half operands: writes 1 bit per element for src0 >= src1 (1 true, 0 false) into uint8_t dst, little-endian.
 *
 * @override_id asc_ge_sync__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_ge_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_ge and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on float operands: writes 1 bit per element for src0 >= src1 (1 true, 0 false) into uint8_t dst, little-endian.
 *
 * @override_id asc_ge_sync__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_ge_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_reduce and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface using a built-in or user-defined gather mask (src1) to select uint16_t elements from src0 into dst; use asc_reduce with asc_sync.
 *
 * @override_id asc_reduce_sync__PUu16_PUu16_PUu16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 在用户自定义模式下，由用户自定义的数据收集的掩码。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_reduce_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_reduce and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface using a built-in or user-defined gather mask (src1) to select uint32_t elements from src0 into dst; use asc_reduce with asc_sync.
 *
 * @override_id asc_reduce_sync__PUu32_PUu32_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 在用户自定义模式下，由用户自定义的数据收集的掩码。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_reduce_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642float_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync conversion variant asc_int642float_rna_sync: converts count int64_t elements from src to float and writes the results to dst.
 *
 * @override_id asc_int642float_rna_sync__PUf32_PUi64_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_rna_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642float_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync conversion variant asc_int642float_ru_sync: converts count int64_t elements from src to float and writes the results to dst.
 *
 * @override_id asc_int642float_ru_sync__PUf32_PUi64_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_ru_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642float_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync conversion variant asc_int642float_rd_sync: converts count int64_t elements from src to float and writes the results to dst.
 *
 * @override_id asc_int642float_rd_sync__PUf32_PUi64_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_rd_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642float_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync conversion variant asc_int642float_rn_sync: converts count int64_t elements from src to float and writes the results to dst.
 *
 * @override_id asc_int642float_rn_sync__PUf32_PUi64_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_rn_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int642float_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync conversion variant asc_int642float_rz_sync: converts count int64_t elements from src to float and writes the results to dst.
 *
 * @override_id asc_int642float_rz_sync__PUf32_PUi64_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_rz_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_vdeq_int162b8_h and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync quantization: converts count int16_t elements to int8_t stored in the upper half of each DataBlock; call asc_set_deq_scale for parameters first.
 *
 * @override_id asc_vdeq_int162b8_h_sync__PUi8_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_vdeq_int162b8_h and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync quantization: converts count int16_t elements to uint8_t stored in the upper half of each DataBlock; call asc_set_deq_scale for parameters first.
 *
 * @override_id asc_vdeq_int162b8_h_sync__PUu8_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_vdeq_int162b8_l and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync quantization: converts count int16_t elements to int8_t stored in the lower half of each DataBlock; call asc_set_deq_scale for parameters first.
 *
 * @override_id asc_vdeq_int162b8_l_sync__PUi8_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_vdeq_int162b8_l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_vdeq_int162b8_l and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync quantization: converts count int16_t elements to uint8_t stored in the lower half of each DataBlock; call asc_set_deq_scale for parameters first.
 *
 * @override_id asc_vdeq_int162b8_l_sync__PUu8_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_vdeq_int162b8_l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sqrt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface computing the element-wise square root dst_i = sqrt(src_i) for half data; use asc_sqrt with asc_sync.
 *
 * @override_id asc_sqrt_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sqrt_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_sqrt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface computing the element-wise square root dst_i = sqrt(src_i) for float data; use asc_sqrt with asc_sync.
 *
 * @override_id asc_sqrt_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sqrt_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_axpy and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync AXPY for half src and half dst: computes dst_i = src_i * value + dst_i for count elements.
 *
 * @override_id asc_axpy_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_axpy_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_axpy and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync AXPY for float src and float dst: computes dst_i = src_i * value + dst_i for count elements.
 *
 * @override_id asc_axpy_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_axpy_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_axpy and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync AXPY for half src and float dst: computes dst_i = src_i * value + dst_i for count elements.
 *
 * @override_id asc_axpy_sync__PUf32_PUf16_f16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_axpy_sync(__ubuf__ float* dst, __ubuf__ half* src, half value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_lt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on half operands: result bits for src0 < src1 (1 true, 0 false) are stored to a register read by asc_get_cmp_mask.
 *
 * @override_id asc_lt_sync__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_lt_sync(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_lt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on float operands: result bits for src0 < src1 (1 true, 0 false) are stored to a register read by asc_get_cmp_mask.
 *
 * @override_id asc_lt_sync__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_lt_sync(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_lt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on half operands: writes 1 bit per element for src0 < src1 (1 true, 0 false) into uint8_t dst, little-endian.
 *
 * @override_id asc_lt_sync__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_lt_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_lt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on float operands: writes 1 bit per element for src0 < src1 (1 true, 0 false) into uint8_t dst, little-endian.
 *
 * @override_id asc_lt_sync__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_lt_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_ne_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for half: sets 1 bit per element in uint8_t dst for src != value (1 true, 0 false), little-endian.
 *
 * @override_id asc_ne_scalar_sync__PUu8_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_ne_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_ne_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for float: sets 1 bit per element in uint8_t dst for src != value (1 true, 0 false), little-endian.
 *
 * @override_id asc_ne_scalar_sync__PUu8_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_ne_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int162half and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting int16_t elements to half with no rounding (RINT when lossy), writing results to output parameter dst via void return.
 *
 * @override_id asc_int162half_sync__PUf16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int162half_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting int16_t elements to half with ROUND (round half away from zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_int162half_rna_sync__PUf16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_rna_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int162half_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting int16_t elements to half with CEIL (toward positive infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_int162half_ru_sync__PUf16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_ru_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int162half_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting int16_t elements to half with FLOOR (toward negative infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_int162half_rd_sync__PUf16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_rd_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int162half_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting int16_t elements to half with RINT (round half to even) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_int162half_rn_sync__PUf16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_rn_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_int162half_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting int16_t elements to half with TRUNC (toward zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_int162half_rz_sync__PUf16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_rz_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gather_datablock and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync DataBlock-granularity gather for uint16_t data: collects DataBlocks from src into dst according to the src_offset address offsets.
 *
 * @override_id asc_gather_datablock_sync__PUu16_PUu16_PUu32_u16_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 每个Datablock在源操作数中的地址偏移。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_gather_datablock_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t dst_block_stride, uint8_t repeat);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gather_datablock and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync DataBlock-granularity gather for uint32_t data: collects DataBlocks from src into dst according to the src_offset address offsets.
 *
 * @override_id asc_gather_datablock_sync__PUu32_PUu32_PUu32_u16_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 每个Datablock在源操作数中的地址偏移。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_gather_datablock_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t dst_block_stride, uint8_t repeat);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_min_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar minimum for half data: computes dst_i = min(src_i, value) for count elements and writes the results to dst.
 *
 * @override_id asc_min_scalar_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_min_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar minimum for float data: computes dst_i = min(src_i, value) for count elements and writes the results to dst.
 *
 * @override_id asc_min_scalar_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_min_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar minimum for int16_t data: computes dst_i = min(src_i, value) for count elements and writes the results to dst.
 *
 * @override_id asc_min_scalar_sync__PUi16_PUi16_i16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_min_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar minimum for int32_t data: computes dst_i = min(src_i, value) for count elements and writes the results to dst.
 *
 * @override_id asc_min_scalar_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous element-wise add plus ReLU on half sources with the result converted to int8 (f162s8), writing relu(src0+src1) to the dst output parameter.
 *
 * @override_id asc_add_relu_sync__PUi8_PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous element-wise add plus ReLU on float sources with the result converted to half (f322f16), writing relu(src0+src1) to the dst output parameter.
 *
 * @override_id asc_add_relu_sync__PUf16_PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_add_relu and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous element-wise add plus ReLU on int16 sources with the result converted to int8 (s162s8), writing relu(src0+src1) to the dst output parameter.
 *
 * @override_id asc_add_relu_sync__PUi8_PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on half operands: result bits for src0 > src1 (1 true, 0 false) are stored to a register read by asc_get_cmp_mask.
 *
 * @override_id asc_gt_sync__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_gt_sync(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on float operands: result bits for src0 > src1 (1 true, 0 false) are stored to a register read by asc_get_cmp_mask.
 *
 * @override_id asc_gt_sync__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_gt_sync(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on half operands: writes 1 bit per element for src0 > src1 (1 true, 0 false) into uint8_t dst, little-endian.
 *
 * @override_id asc_gt_sync__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_gt_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_gt and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on float operands: writes 1 bit per element for src0 > src1 (1 true, 0 false) into uint8_t dst, little-endian.
 *
 * @override_id asc_gt_sync__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_gt_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting half elements to uint8_t with NONE rounding (RINT when lossy, else no rounding), writing results to output parameter dst via void return.
 *
 * @override_id asc_half2uint8_sync__PUu8_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_rna and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting half elements to uint8_t with ROUND (round half away from zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_half2uint8_rna_sync__PUu8_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_rna_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_ru and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting half elements to uint8_t with CEIL (toward positive infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_half2uint8_ru_sync__PUu8_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_ru_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_rd and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting half elements to uint8_t with FLOOR (toward negative infinity) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_half2uint8_rd_sync__PUu8_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_rd_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_rn and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting half elements to uint8_t with RINT (round half to even) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_half2uint8_rn_sync__PUu8_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_rn_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_half2uint8_rz and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite API converting half elements to uint8_t with TRUNC (toward zero) rounding, writing results to output parameter dst via void return.
 *
 * @override_id asc_half2uint8_rz_sync__PUu8_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_rz_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_le and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on half operands: result bits for src0 <= src1 (1 true, 0 false) are stored to a register read by asc_get_cmp_mask.
 *
 * @override_id asc_le_sync__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_le_sync(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_le and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on float operands: result bits for src0 <= src1 (1 true, 0 false) are stored to a register read by asc_get_cmp_mask.
 *
 * @override_id asc_le_sync__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_le_sync(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_le and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on half operands: writes 1 bit per element for src0 <= src1 (1 true, 0 false) into uint8_t dst, little-endian.
 *
 * @override_id asc_le_sync__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_le_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_le and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync compare on float operands: writes 1 bit per element for src0 <= src1 (1 true, 0 false) into uint8_t dst, little-endian.
 *
 * @override_id asc_le_sync__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_le_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_le_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for half: sets 1 bit per element in uint8_t dst for src <= value (1 true, 0 false), little-endian.
 *
 * @override_id asc_le_scalar_sync__PUu8_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_le_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_le_scalar and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync vector-scalar compare for float: sets 1 bit per element in uint8_t dst for src <= value (1 true, 0 false), little-endian.
 *
 * @override_id asc_le_scalar_sync__PUu8_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_le_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pair_reduce_sum and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync pairwise reduction sum for half data: adds adjacent (odd/even) element pairs, e.g. (a1+a2, a3+a4, ...), for count elements.
 *
 * @override_id asc_pair_reduce_sum_sync__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_pair_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_pair_reduce_sum and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync pairwise reduction sum for float data: adds adjacent (odd/even) element pairs, e.g. (a1+a2, a3+a4, ...), for count elements.
 *
 * @override_id asc_pair_reduce_sum_sync__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_pair_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync UB-to-UB copy for int16_t data: moves count elements from src to dst with high-dimensional slicing support.
 *
 * @override_id asc_copy_sync__PUi16_PUi16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_copy_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync UB-to-UB copy for uint16_t data: moves count elements from src to dst with high-dimensional slicing support.
 *
 * @override_id asc_copy_sync__PUu16_PUu16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_copy_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync UB-to-UB copy for int32_t data: moves count elements from src to dst with high-dimensional slicing support.
 *
 * @override_id asc_copy_sync__PUi32_PUi32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_copy_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync UB-to-UB copy for uint32_t data: moves count elements from src to dst with high-dimensional slicing support.
 *
 * @override_id asc_copy_sync__PUu32_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_copy_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t count);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface copying data from Unified Buffer (UB) to UB; use asc_copy_ub2ub with asc_sync.
 *
 * @override_id asc_copy_ub2ub_sync__PUv_PUv_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB地址。
 * @param [in] src 源UB地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_ub2ub_sync(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface copying data from Global Memory (GM) to Unified Buffer (UB); use asc_copy_gm2ub with asc_sync.
 *
 * @override_id asc_copy_gm2ub_sync__PUv_PGv_u32
 * @archs 2201
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2ub_sync(__ubuf__ void* dst, __gm__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface copying data from Unified Buffer (UB) to Global Memory (GM); use asc_copy_ub2gm with asc_sync.
 *
 * @override_id asc_copy_ub2gm_sync__PGv_PUv_u32
 * @archs 2201
 * @param [out] dst 目的GM地址。
 * @param [in] src 源UB地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_ub2gm_sync(__gm__ void* dst, __ubuf__ void* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous GM-to-UB unaligned copy for int8 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_gm2ub_align plus asc_sync.
 *
 * @override_id asc_copy_gm2ub_align_sync__PUi8_PGi8_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous GM-to-UB unaligned copy for uint8 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_gm2ub_align plus asc_sync.
 *
 * @override_id asc_copy_gm2ub_align_sync__PUu8_PGu8_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous GM-to-UB unaligned copy for half data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_gm2ub_align plus asc_sync.
 *
 * @override_id asc_copy_gm2ub_align_sync__PUf16_PGf16_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ half* dst, __gm__ half* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous GM-to-UB unaligned copy for bfloat16 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_gm2ub_align plus asc_sync.
 *
 * @override_id asc_copy_gm2ub_align_sync__PUbf16_PGbf16_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous GM-to-UB unaligned copy for int16 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_gm2ub_align plus asc_sync.
 *
 * @override_id asc_copy_gm2ub_align_sync__PUi16_PGi16_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous GM-to-UB unaligned copy for uint16 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_gm2ub_align plus asc_sync.
 *
 * @override_id asc_copy_gm2ub_align_sync__PUu16_PGu16_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous GM-to-UB unaligned copy for float data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_gm2ub_align plus asc_sync.
 *
 * @override_id asc_copy_gm2ub_align_sync__PUf32_PGf32_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ float* dst, __gm__ float* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous GM-to-UB unaligned copy for int32 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_gm2ub_align plus asc_sync.
 *
 * @override_id asc_copy_gm2ub_align_sync__PUi32_PGi32_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous GM-to-UB unaligned copy for uint32 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_gm2ub_align plus asc_sync.
 *
 * @override_id asc_copy_gm2ub_align_sync__PUu32_PGu32_u32
 * @archs 2201 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous UB-to-GM copy for uint8 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_ub2gm_align plus asc_sync.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGu8_PUu8_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous UB-to-GM copy for int8 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_ub2gm_align plus asc_sync.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGi8_PUi8_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous UB-to-GM copy for half data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_ub2gm_align plus asc_sync.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGf16_PUf16_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ half* dst, __ubuf__ half* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous UB-to-GM copy for uint16 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_ub2gm_align plus asc_sync.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGu16_PUu16_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous UB-to-GM copy for int16 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_ub2gm_align plus asc_sync.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGi16_PUi16_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous UB-to-GM copy for bfloat16 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_ub2gm_align plus asc_sync.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGbf16_PUbf16_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous UB-to-GM copy for uint32 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_ub2gm_align plus asc_sync.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGu32_PUu32_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous UB-to-GM copy for float data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_ub2gm_align plus asc_sync.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGf32_PUf32_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ float* dst, __ubuf__ float* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous UB-to-GM copy for int32 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_ub2gm_align plus asc_sync.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGi32_PUi32_u32
 * @archs 2201 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous UB-to-GM copy for double data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_ub2gm_align plus asc_sync.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGf64_PUf64_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ double* dst, __ubuf__ double* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous UB-to-GM copy for int64 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_ub2gm_align plus asc_sync.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGi64_PUi64_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int64_t* dst, __ubuf__ int64_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous UB-to-GM copy for uint64 data writing to the dst output parameter with padding to 32-byte alignment; replaced by asc_copy_ub2gm_align plus asc_sync.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGu64_PUu64_u32
 * @archs 2201
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync unaligned GM-to-UB copy for hifloat8_t data: moves size bytes and pads non-32-byte-aligned lengths to 32 bytes; use asc_copy_gm2ub_align and asc_sync.
 *
 * @override_id asc_copy_gm2ub_align_sync__PUhf8_PGhf8_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync unaligned GM-to-UB copy for fp8_e5m2_t data: moves size bytes and pads non-32-byte-aligned lengths to 32 bytes; use asc_copy_gm2ub_align and asc_sync.
 *
 * @override_id asc_copy_gm2ub_align_sync__PUf8e5m2_PGf8e5m2_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_gm2ub_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync unaligned GM-to-UB copy for fp8_e4m3fn_t data: moves size bytes and pads non-32-byte-aligned lengths to 32 bytes; use asc_copy_gm2ub_align and asc_sync.
 *
 * @override_id asc_copy_gm2ub_align_sync__PUf8e4m3_PGf8e4m3_u32
 * @archs 3510
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync unaligned UB-to-GM copy for hifloat8_t data: moves size bytes; non-32-byte-aligned lengths are auto-padded with dummy data discarded when writing GM.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGhf8_PUhf8_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync unaligned UB-to-GM copy for fp8_e5m2_t data: moves size bytes; non-32-byte-aligned lengths are auto-padded with dummy data discarded when writing GM.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGf8e5m2_PUf8e5m2_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2gm_align and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated sync unaligned UB-to-GM copy for fp8_e4m3fn_t data: moves size bytes; non-32-byte-aligned lengths are auto-padded with dummy data discarded when writing GM.
 *
 * @override_id asc_copy_ub2gm_align_sync__PGf8e4m3_PUf8e4m3_u32
 * @archs 3510
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t size);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_copy_ub2l1 and asc_sync)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated synchronous composite interface copying contiguous data from Unified Buffer (UB) to L1 Buffer with sync after copy, effective on AIV only; use asc_copy_ub2l1 with asc_sync.
 *
 * @override_id asc_copy_ub2l1_sync__PL1v_PUv_u32
 * @archs 3510
 * @param [out] dst 目的L1 Buffer的起始地址。
 * @param [in] src 源UB的起始地址。
 * @param [in] size 搬运数据大小，单位为字节。
 */
__aicore__ inline void asc_copy_ub2l1_sync(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_VECTOR_COMPUTE_COMPOSITE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_VECTOR_COMPUTE_COMPOSITE_H
#endif
