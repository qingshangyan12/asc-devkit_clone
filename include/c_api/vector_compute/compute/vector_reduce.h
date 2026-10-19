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
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional form: gathers elements from uint16 src0 into dst over repeat iterations using pattern_mode (built-in pattern or user-defined mask via src1) with block/repeat strides.
 *
 * @override_id asc_reduce__PUu16_PUu16_PUu16_u16_u8_u8_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 在用户自定义模式下，由用户自定义的数据收集的掩码。
 * @param [in] repeat 迭代次数。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] pattern_mode gather mask（数据收集的掩码），分为内置固定模式和用户自定义模式两种，根据内置固定模式对应的二进制或者用户自定义输入的数值对应的二进制从源操作数中选取元素写入目的操作数中。 &bull;内置固定模式：pattern_mode数据类型为uint8_t，取值范围为[1, 7]，所有迭代使用相同的gather mask。不支持配置src1。 &nbsp;&nbsp;&bull; 1：01010101...0101 # 每个repeat取偶数索引元素 &nbsp;&nbsp;&bull; 2：10101010...1010 # 每个repeat取奇数索引元素 &nbsp;&nbsp;&bull; 3：00010001...0001 # 每个repeat内每四个元素取第一个元素 &nbsp;&nbsp;&bull; 4：00100010...0010 # 每个repeat每四个元素取第二个元素 &nbsp;&nbsp;&bull; 5：01000100...0100 # 每个repeat内每四个元素取第三个元素 &nbsp;&nbsp;&bull; 6：10001000...1000 # 每个repeat内每四个元素区第四个元素 &nbsp;&nbsp;&bull; 7：11111111...1111 # 每个repeat内取全部元素 &bull;用户自定义模式：pattern_mode取值为0时为用户自定义模式，用户通过src1参数输入自定义的gather mask，迭代间隔由src1_repeat_stride决定。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_reduce(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint16_t repeat,
    uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Count form: gathers elements from uint16 src0 into dst according to the built-in fixed pattern or a user-supplied gather mask for the first count elements.
 *
 * @override_id asc_reduce__PUu16_PUu16_PUu16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 在用户自定义模式下，由用户自定义的数据收集的掩码。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_reduce(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional form: gathers elements from uint32 src0 into dst over repeat iterations using pattern_mode (built-in pattern or user-defined mask via src1) with block/repeat strides.
 *
 * @override_id asc_reduce__PUu32_PUu32_PUu32_u16_u8_u8_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 在用户自定义模式下，由用户自定义的数据收集的掩码。
 * @param [in] repeat 迭代次数。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] pattern_mode gather mask（数据收集的掩码），分为内置固定模式和用户自定义模式两种，根据内置固定模式对应的二进制或者用户自定义输入的数值对应的二进制从源操作数中选取元素写入目的操作数中。 &bull;内置固定模式：pattern_mode数据类型为uint8_t，取值范围为[1, 7]，所有迭代使用相同的gather mask。不支持配置src1。 &nbsp;&nbsp;&bull; 1：01010101...0101 # 每个repeat取偶数索引元素 &nbsp;&nbsp;&bull; 2：10101010...1010 # 每个repeat取奇数索引元素 &nbsp;&nbsp;&bull; 3：00010001...0001 # 每个repeat内每四个元素取第一个元素 &nbsp;&nbsp;&bull; 4：00100010...0010 # 每个repeat每四个元素取第二个元素 &nbsp;&nbsp;&bull; 5：01000100...0100 # 每个repeat内每四个元素取第三个元素 &nbsp;&nbsp;&bull; 6：10001000...1000 # 每个repeat内每四个元素区第四个元素 &nbsp;&nbsp;&bull; 7：11111111...1111 # 每个repeat内取全部元素 &bull;用户自定义模式：pattern_mode取值为0时为用户自定义模式，用户通过src1参数输入自定义的gather mask，迭代间隔由src1_repeat_stride决定。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_reduce(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint16_t repeat,
    uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Count form: gathers elements from uint32 src0 into dst according to the built-in fixed pattern or a user-supplied gather mask for the first count elements.
 *
 * @override_id asc_reduce__PUu32_PUu32_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 在用户自定义模式下，由用户自定义的数据收集的掩码。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_reduce(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Reads the maximum value and the index of the first maximum produced by a completed asc_repeat_reduce_max into half val and uint32 index (void return, output references).
 *
 * @override_id asc_get_reduce_max_cnt__Rf16_Ru32
 * @archs 2201
 * @param [out] val 最大值。
 * @param [out] index 第一个最大值时的索引。
 */
__aicore__ inline void asc_get_reduce_max_cnt(half& val, uint32_t& index);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Reads the maximum value and the index of the first maximum produced by a completed asc_repeat_reduce_max into float val and uint32 index (void return, output references).
 *
 * @override_id asc_get_reduce_max_cnt__Rf32_Ru32
 * @archs 2201
 * @param [out] val 最大值。
 * @param [out] index 第一个最大值时的索引。
 */
__aicore__ inline void asc_get_reduce_max_cnt(float& val, uint32_t& index);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Reads the minimum value and the index of the first minimum produced by a completed asc_repeat_reduce_min into half val and uint32 index (void return, output references).
 *
 * @override_id asc_get_reduce_min_cnt__Rf16_Ru32
 * @archs 2201
 * @param [out] val 最小值。
 * @param [out] index 第一个最小值时的索引。
 */
__aicore__ inline void asc_get_reduce_min_cnt(half& val, uint32_t& index);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Reads the minimum value and the index of the first minimum produced by a completed asc_repeat_reduce_min into float val and uint32 index (void return, output references).
 *
 * @override_id asc_get_reduce_min_cnt__Rf32_Ru32
 * @archs 2201
 * @param [out] val 最小值。
 * @param [out] index 第一个最小值时的索引。
 */
__aicore__ inline void asc_get_reduce_min_cnt(float& val, uint32_t& index);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Returns as int64_t the number of remaining elements after a completed asc_reduce operation (non-void return).
 *
 * @override_id asc_get_rsvd_count__z
 * @archs 2201
 */
__aicore__ inline int64_t asc_get_rsvd_count();

__aicore__ inline int64_t asc_get_acc_val();

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional form: sums each 32-byte DataBlock of half src by binary-tree addition (half saturates at 65504) and writes per-block totals to dst over repeat iterations via block/repeat strides.
 *
 * @override_id asc_datablock_reduce_sum__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。 输入类型位宽为16bit时，单位为16Byte，输入类型位宽为32bit时，单位为32Byte。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_datablock_reduce_sum(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Count form: sums each 32-byte DataBlock of half src by binary-tree addition (half saturates at 65504) and writes the per-block totals for the first count elements to dst.
 *
 * @override_id asc_datablock_reduce_sum__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional form: sums each 32-byte DataBlock of float src by binary-tree addition and writes per-block totals to dst over repeat iterations via block/repeat strides.
 *
 * @override_id asc_datablock_reduce_sum__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。 输入类型位宽为16bit时，单位为16Byte，输入类型位宽为32bit时，单位为32Byte。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_datablock_reduce_sum(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Count form: sums each 32-byte DataBlock of float src by binary-tree addition and writes the per-block totals for the first count elements to dst.
 *
 * @override_id asc_datablock_reduce_sum__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional form: computes per-32-byte-DataBlock maximum of half src and writes them to dst over repeat iterations using dst/block/repeat stride parameters.
 *
 * @override_id asc_datablock_reduce_max__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。 输入类型位宽为16bit时，单位为16Byte，输入类型位宽为32bit时，单位为32Byte。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_datablock_reduce_max(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Count form: computes the maximum of each 32-byte DataBlock of half src for the first count elements and writes the per-block results to dst.
 *
 * @override_id asc_datablock_reduce_max__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_max(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional form: computes per-32-byte-DataBlock maximum of float src and writes them to dst over repeat iterations using dst/block/repeat stride parameters.
 *
 * @override_id asc_datablock_reduce_max__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。 输入类型位宽为16bit时，单位为16Byte，输入类型位宽为32bit时，单位为32Byte。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_datablock_reduce_max(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Count form: computes the maximum of each 32-byte DataBlock of float src for the first count elements and writes the per-block results to dst.
 *
 * @override_id asc_datablock_reduce_max__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_max(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional form: computes per-32-byte-DataBlock minimum of half src and writes them to dst over repeat iterations using dst/block/repeat stride parameters.
 *
 * @override_id asc_datablock_reduce_min__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。 输入类型位宽为16bit时，单位为16Byte，输入类型位宽为32bit时，单位为32Byte。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_datablock_reduce_min(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Count form: computes the minimum of each 32-byte DataBlock of half src for the first count elements and writes the per-block results to dst.
 *
 * @override_id asc_datablock_reduce_min__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_min(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional form: computes per-32-byte-DataBlock minimum of float src and writes them to dst over repeat iterations using dst/block/repeat stride parameters.
 *
 * @override_id asc_datablock_reduce_min__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。 输入类型位宽为16bit时，单位为16Byte，输入类型位宽为32bit时，单位为32Byte。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_datablock_reduce_min(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Count form: computes the minimum of each 32-byte DataBlock of float src for the first count elements and writes the per-block results to dst.
 *
 * @override_id asc_datablock_reduce_min__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_min(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional form: sums all elements within each repeat of half src by binary-tree addition (half results saturate at 65504) and writes per-repeat totals to dst over repeat iterations.
 *
 * @override_id asc_repeat_reduce_sum__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 若输出数据类型为half，则单位为2字节。  若输出数据类型为float，则单位为4字节。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_sum(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Count form: sums all elements within each repeat of half src by binary-tree addition (half results saturate at 65504) and writes the per-repeat totals for the first count elements to dst.
 *
 * @override_id asc_repeat_reduce_sum__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional form: sums all elements within each repeat of float src by binary-tree addition and writes per-repeat totals to dst over repeat iterations.
 *
 * @override_id asc_repeat_reduce_sum__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 若输出数据类型为half，则单位为2字节。  若输出数据类型为float，则单位为4字节。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_sum(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Count form: sums all elements within each repeat of float src by binary-tree addition and writes the per-repeat totals for the first count elements to dst.
 *
 * @override_id asc_repeat_reduce_sum__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat max over half: writes [index, value] pairs of each 256-byte Repeat to dst using repeat and dst/src strides; the index is uint16.
 *
 * @override_id asc_repeat_reduce_max_index_value__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_max_index_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the maximum of each 256-byte Repeat for the first count half elements and writes [index, value] pairs to dst; the index is uint16.
 *
 * @override_id asc_repeat_reduce_max_index_value__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat max over float: writes [index, value] pairs of each 256-byte Repeat to dst using repeat and dst/src strides; the index is uint32.
 *
 * @override_id asc_repeat_reduce_max_index_value__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_max_index_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the maximum of each 256-byte Repeat for the first count float elements and writes [index, value] pairs to dst; the index is uint32.
 *
 * @override_id asc_repeat_reduce_max_index_value__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat max over half: writes [value, index] pairs of each 256-byte Repeat to dst using repeat and dst/src strides; the index is uint16.
 *
 * @override_id asc_repeat_reduce_max_value_index__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_max_value_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the maximum of each 256-byte Repeat for the first count half elements and writes [value, index] pairs to dst; the index is uint16.
 *
 * @override_id asc_repeat_reduce_max_value_index__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat max over float: writes [value, index] pairs of each 256-byte Repeat to dst using repeat and dst/src strides; the index is uint32.
 *
 * @override_id asc_repeat_reduce_max_value_index__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_max_value_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the maximum of each 256-byte Repeat for the first count float elements and writes [value, index] pairs to dst; the index is uint32.
 *
 * @override_id asc_repeat_reduce_max_value_index__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat max over half: writes only the maximum value of each 256-byte Repeat to dst using repeat and dst/src strides; no index is returned.
 *
 * @override_id asc_repeat_reduce_max_only_value__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_max_only_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the maximum of each 256-byte Repeat for the first count half elements and writes only the maximum value to dst; no index is returned.
 *
 * @override_id asc_repeat_reduce_max_only_value__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat max over float: writes only the maximum value of each 256-byte Repeat to dst using repeat and dst/src strides; no index is returned.
 *
 * @override_id asc_repeat_reduce_max_only_value__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_max_only_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the maximum of each 256-byte Repeat for the first count float elements and writes only the maximum value to dst; no index is returned.
 *
 * @override_id asc_repeat_reduce_max_only_value__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat max over half: writes only the maximum index (always uint32) of each 256-byte Repeat to dst using repeat and dst/src strides.
 *
 * @override_id asc_repeat_reduce_max_only_index__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_max_only_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the maximum of each 256-byte Repeat for the first count half elements and writes only the maximum index (always uint32) to dst; no value is returned.
 *
 * @override_id asc_repeat_reduce_max_only_index__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat max over float: writes only the maximum index (always uint32) of each 256-byte Repeat to dst using repeat and dst/src strides.
 *
 * @override_id asc_repeat_reduce_max_only_index__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_max_only_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the maximum of each 256-byte Repeat for the first count float elements and writes only the maximum index (always uint32) to dst; no value is returned.
 *
 * @override_id asc_repeat_reduce_max_only_index__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat min over half: writes [index, value] pairs of each 256-byte Repeat to dst using repeat and dst/src strides; the index is uint16.
 *
 * @override_id asc_repeat_reduce_min_index_value__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_min_index_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the minimum of each 256-byte Repeat for the first count half elements and writes [index, value] pairs to dst; the index is uint16.
 *
 * @override_id asc_repeat_reduce_min_index_value__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat min over float: writes [index, value] pairs of each 256-byte Repeat to dst using repeat and dst/src strides; the index is uint32.
 *
 * @override_id asc_repeat_reduce_min_index_value__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_min_index_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the minimum of each 256-byte Repeat for the first count float elements and writes [index, value] pairs to dst; the index is uint32.
 *
 * @override_id asc_repeat_reduce_min_index_value__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat min over half: writes [value, index] pairs of each 256-byte Repeat to dst using repeat and dst/src strides; the index is uint16.
 *
 * @override_id asc_repeat_reduce_min_value_index__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_min_value_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the minimum of each 256-byte Repeat for the first count half elements and writes [value, index] pairs to dst; the index is uint16.
 *
 * @override_id asc_repeat_reduce_min_value_index__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat min over float: writes [value, index] pairs of each 256-byte Repeat to dst using repeat and dst/src strides; the index is uint32.
 *
 * @override_id asc_repeat_reduce_min_value_index__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_min_value_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the minimum of each 256-byte Repeat for the first count float elements and writes [value, index] pairs to dst; the index is uint32.
 *
 * @override_id asc_repeat_reduce_min_value_index__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat min over half: writes only the minimum value of each 256-byte Repeat to dst using repeat and dst/src strides; no index is returned.
 *
 * @override_id asc_repeat_reduce_min_only_value__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_min_only_value(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the minimum of each 256-byte Repeat for the first count half elements and writes only the minimum value to dst; no index is returned.
 *
 * @override_id asc_repeat_reduce_min_only_value__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat min over float: writes only the minimum value of each 256-byte Repeat to dst using repeat and dst/src strides; no index is returned.
 *
 * @override_id asc_repeat_reduce_min_only_value__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_min_only_value(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the minimum of each 256-byte Repeat for the first count float elements and writes only the minimum value to dst; no index is returned.
 *
 * @override_id asc_repeat_reduce_min_only_value__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat min over half: writes only the minimum index (always uint32) of each 256-byte Repeat to dst using repeat and dst/src strides.
 *
 * @override_id asc_repeat_reduce_min_only_index__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_min_only_index(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the minimum of each 256-byte Repeat for the first count half elements and writes only the minimum index (always uint32) to dst; no value is returned.
 *
 * @override_id asc_repeat_reduce_min_only_index__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional repeat min over float: writes only the minimum index (always uint32) of each 256-byte Repeat to dst using repeat and dst/src strides.
 *
 * @override_id asc_repeat_reduce_min_only_index__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间的地址步长，单位为一个repeat计算后的长度，具体如下： 返回最值和索引时，若输出数据类型为T，则单位为sizeof(T)*2。 仅返回最值且输入类型为T时，单位为sizeof(T)。 仅返回索引时，单位为4字节(sizeof(uint32))
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_repeat_reduce_min_only_index(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Computes the minimum of each 256-byte Repeat for the first count float elements and writes only the minimum index (always uint32) to dst; no value is returned.
 *
 * @override_id asc_repeat_reduce_min_only_index__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional form: sums adjacent odd/even element pairs of half src and writes results to dst over repeat iterations using dst/block/repeat stride parameters.
 *
 * @override_id asc_pair_reduce_sum__PUf16_PUf16_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。 以一个repeat归约后的长度为单位，PairReduce完成后，一个repeat的长度减半。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_pair_reduce_sum(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Count form: sums adjacent odd/even element pairs of half src (a1+a2, a3+a4, ...) and writes the first count/2 results to dst.
 *
 * @override_id asc_pair_reduce_sum__PUf16_PUf16_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_pair_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief High-dimensional form: sums adjacent odd/even element pairs of float src and writes results to dst over repeat iterations using dst/block/repeat stride parameters.
 *
 * @override_id asc_pair_reduce_sum__PUf32_PUf32_u8_u16_u16_u16
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。 以一个repeat归约后的长度为单位，PairReduce完成后，一个repeat的长度减半。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_pair_reduce_sum(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride, uint16_t src_block_stride,
    uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @brief Count form: sums adjacent odd/even element pairs of float src (a1+a2, a3+a4, ...) and writes the first count/2 results to dst.
 *
 * @override_id asc_pair_reduce_sum__PUf32_PUf32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_pair_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_REDUCE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_REDUCE_H
#endif
