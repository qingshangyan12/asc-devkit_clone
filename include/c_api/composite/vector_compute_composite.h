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
#include "impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h"
#include "impl/c_api/instr_impl/npu_arch_2201/vector_datamove_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"
#include "impl/c_api/instr_impl/npu_arch_3510/vector_datamove_impl.h"
#endif
/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_sync__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_scalar_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_add_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_scalar_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_add_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_scalar_sync__PUi16_PUi16_i16_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_add_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_scalar_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_add_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_brcb_sync__PUu16_PUu16_u16_u16_u8
 * @archs 2201
 * @brief 给定一个输入矢量，每一次取输入矢量中的8个数填充到结果矢量的8个datablock（32Bytes）中去，每个数对应一个datablock。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_brcb_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_brcb_sync__PUu32_PUu32_u16_u16_u8
 * @archs 2201
 * @brief 给定一个输入矢量，每一次取输入矢量中的8个数填充到结果矢量的8个datablock（32Bytes）中去，每个数对应一个datablock。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_brcb_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @override_id asc_bitsort_sync__PUf16_PUf16_PUu32_i32
 * @archs 3510 2201
 * @brief 位图排序函数，一次迭代可以完成32个元素的排序。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 重复迭代次数，每次迭代完成32个元素的排序，下次迭代src0和src1各跳过32个元素，dst跳过256 Byte空间。取值范围：[0, 255]。
 */
__aicore__ inline void asc_bitsort_sync(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @override_id asc_bitsort_sync__PUf32_PUf32_PUu32_i32
 * @archs 3510 2201
 * @brief 位图排序函数，一次迭代可以完成32个元素的排序。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 重复迭代次数，每次迭代完成32个元素的排序，下次迭代src0和src1各跳过32个元素，dst跳过256 Byte空间。取值范围：[0, 255]。
 */
__aicore__ inline void asc_bitsort_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_transpose_sync__PUi16_PUi16
 * @archs 3510 2201
 * @brief 用于实现16*16的二维矩阵数据块转置。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__aicore__ inline void asc_transpose_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_transpose_sync__PUu16_PUu16
 * @archs 3510 2201
 * @brief 用于实现16*16的二维矩阵数据块转置。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 */
__aicore__ inline void asc_transpose_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_datablock_reduce_sum_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 对每个DataBlock内所有元素求和。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_datablock_reduce_sum_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对每个DataBlock内所有元素求和。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_datablock_reduce_max_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行数据块内的求最大值规约（Reduce Max）操作。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_max_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_datablock_reduce_max_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行数据块内的求最大值规约（Reduce Max）操作。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_max_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_datablock_reduce_min_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行数据块内的求最小值规约（Reduce Min）操作。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_min_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_datablock_reduce_min_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行数据块内的求最小值规约（Reduce Min）操作。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_datablock_reduce_min_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_sum_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求和。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_sum_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求和。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_max_index_value_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最大值，以及Repeat内的首个最大值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_index_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_max_index_value_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最大值，以及Repeat内的首个最大值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_index_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_max_value_index_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最大值，以及Repeat内的首个最大值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_value_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_max_value_index_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最大值，以及Repeat内的首个最大值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_value_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_max_only_value_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最大值，以及Repeat内的首个最大值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_only_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_max_only_value_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最大值，以及Repeat内的首个最大值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_only_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_max_only_index_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最大值，以及Repeat内的首个最大值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_only_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_max_only_index_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最大值，以及Repeat内的首个最大值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_max_only_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_min_index_value_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最小值，以及Repeat内的首个最小值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_index_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_min_index_value_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最小值，以及Repeat内的首个最小值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_index_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_min_value_index_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最小值，以及Repeat内的首个最小值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_value_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_min_value_index_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最小值，以及Repeat内的首个最小值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_value_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_min_only_value_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最小值，以及Repeat内的首个最小值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_only_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_min_only_value_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最小值，以及Repeat内的首个最小值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_only_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_min_only_index_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最小值，以及Repeat内的首个最小值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_only_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_repeat_reduce_min_only_index_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对每个Repeat内所有元素求最小值，以及Repeat内的首个最小值元素在当前Repeat中的索引index。一次Repeat计算256字节。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_repeat_reduce_min_only_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq_sync__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 比较src0与src1在对应索引位置的元素大小。若比较结果为真，则输出结果的对应比特位设为1，否则设为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq_sync__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 比较src0与src1在对应索引位置的元素大小。若比较结果为真，则输出结果的对应比特位设为1，否则设为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq_sync__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 比较src0与src1在对应索引位置的元素大小。若比较结果为真，则输出结果的对应比特位设为1，否则设为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq_sync__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 比较src0与src1在对应索引位置的元素大小。若比较结果为真，则输出结果的对应比特位设为1，否则设为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq_sync__PUu8_PUi32_PUi32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 比较src0与src1在对应索引位置的元素大小。若比较结果为真，则输出结果的对应比特位设为1，否则设为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162float_sync__PUf32_PUbf16_u32
 * @archs 2201
 * @brief 将bfloat16_t类型数据转为float，无舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162float_sync(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_rna_sync__PUi32_PUbf16_u32
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_ru_sync__PUi32_PUbf16_u32
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_rd_sync__PUi32_PUbf16_u32
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_rn_sync__PUi32_PUbf16_u32
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_rz_sync__PUi32_PUbf16_u32
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_rn_sync__PUbf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_rn_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_rna_sync__PUbf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_rna_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_rd_sync__PUbf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_rd_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_ru_sync__PUbf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_ru_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_rz_sync__PUbf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_rz_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_rn_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_rn_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_rd_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_rd_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_ru_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_ru_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_rna_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_rna_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_rz_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_rz_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_sync__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_rn_sync__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_rn_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_rna_sync__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_rna_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_rd_sync__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_rd_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_ru_sync__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_ru_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_rz_sync__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_rz_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_ro_sync__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_ro_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_rna_sync__PUi32_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_ru_sync__PUi32_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_rd_sync__PUi32_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_rn_sync__PUi32_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_rz_sync__PUi32_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_rna_sync__PUi64_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_rna_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_ru_sync__PUi64_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_ru_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_rd_sync__PUi64_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_rd_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_rn_sync__PUi64_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_rn_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_rz_sync__PUi64_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_rz_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2float_sync__PUf32_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为float类型，无舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2float_sync(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_sync__PUi4x2_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_rna_sync__PUi4x2_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_rna_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_ru_sync__PUi4x2_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_ru_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_rd_sync__PUi4x2_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_rd_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_rn_sync__PUi4x2_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_rn_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_rz_sync__PUi4x2_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_rz_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_sync__PUi8_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_rna_sync__PUi8_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_rna_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_ru_sync__PUi8_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_ru_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_rd_sync__PUi8_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_rd_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_rn_sync__PUi8_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_rn_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_rz_sync__PUi8_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_rz_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_rna_sync__PUi16_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_rna_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_ru_sync__PUi16_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_ru_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_rd_sync__PUi16_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_rd_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_rn_sync__PUi16_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_rn_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_rz_sync__PUi16_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_rz_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_rna_sync__PUi32_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_ru_sync__PUi32_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_rd_sync__PUi32_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_rn_sync__PUi32_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_rz_sync__PUi32_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_div_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量除法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址
 * @param [in] src0 源操作数（矢量）的起始地址
 * @param [in] src1 源操作数（矢量）的起始地址
 * @param [in] count 参与连续计算的元素个数
 */
__aicore__ inline void asc_div_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_div_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量除法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址
 * @param [in] src0 源操作数（矢量）的起始地址
 * @param [in] src1 源操作数（矢量）的起始地址
 * @param [in] count 参与连续计算的元素个数
 */
__aicore__ inline void asc_div_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate_sync__PUf16_f16_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ half* dst, half src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate_sync__PUi16_i16_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ int16_t* dst, int16_t src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate_sync__PUu16_u16_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate_sync__PUbf16_bf16_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate_sync__PUf32_f32_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ float* dst, float src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate_sync__PUi32_i32_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ int32_t* dst, int32_t src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate_sync__PUu32_u32_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate_sync(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_exp_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素取自然指数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_exp_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_exp_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素取自然指数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_exp_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_log_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素取自然对数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_log_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_log_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素取自然对数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_log_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_gt_scalar_sync__PUu8_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief src中的每个元素逐个与标量value比较大小，如果某个位置上的元素大于value，则输出结果dst上的对应比特位为1，否则为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_gt_scalar_sync__PUu8_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief src中的每个元素逐个与标量value比较大小，如果某个位置上的元素大于value，则输出结果dst上的对应比特位为1，否则为0。
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

/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_abs_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 该接口废弃，请通过[asc_abs](../vector_compute/vector_arith/asc_abs.md)和同步接口[asc_sync](../sync/asc_sync.md)实现精细化调优，达成最优性能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_abs_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_abs_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 该接口废弃，请通过[asc_abs](../vector_compute/vector_arith/asc_abs.md)和同步接口[asc_sync](../sync/asc_sync.md)实现精细化调优，达成最优性能。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_abs_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_shiftleft_sync__PUi16_PUi16_u32_u32
 * @archs 2201
 * @brief 将所有元素左移distance位。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] distance 源操作数（标量），表示左移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftleft_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_shiftleft_sync__PUu16_PUu16_u32_u32
 * @archs 2201
 * @brief 将所有元素左移distance位。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] distance 源操作数（标量），表示左移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftleft_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_shiftleft_sync__PUi32_PUi32_u32_u32
 * @archs 2201
 * @brief 将所有元素左移distance位。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] distance 源操作数（标量），表示左移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftleft_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_shiftleft_sync__PUu32_PUu32_u32_u32
 * @archs 2201
 * @brief 将所有元素左移distance位。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] distance 源操作数（标量），表示左移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftleft_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_mul_cast_half2int8_sync__PUi8_PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素求积，并将结果转换为int8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_cast_half2int8_sync(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_mul_cast_half2uint8_sync__PUu8_PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素求积，并将结果转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数0（矢量）的起始地址。
 * @param [in] src1 源操作数1（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_cast_half2uint8_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ne_sync__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素判断src0 != src1是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。 当结果输出到寄存器中时，可在计算完成后调用[asc_get_cmp_mask](asc_get_cmp_mask.md)获取比较结果。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ne_sync__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素判断src0 != src1是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。 当结果输出到寄存器中时，可在计算完成后调用[asc_get_cmp_mask](asc_get_cmp_mask.md)获取比较结果。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ne_sync__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素判断src0 != src1是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。 当结果输出到寄存器中时，可在计算完成后调用[asc_get_cmp_mask](asc_get_cmp_mask.md)获取比较结果。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ne_sync__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素判断src0 != src1是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。 当结果输出到寄存器中时，可在计算完成后调用[asc_get_cmp_mask](asc_get_cmp_mask.md)获取比较结果。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ge_scalar_sync__PUu8_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素判断src >= value是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ge_scalar_sync__PUu8_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素判断src >= value是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_deq_int322half_sync__PUf16_PUi32_u32
 * @archs 2201
 * @brief 对输入的int32_t类型的数据按元素做量化并转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_deq_int322half_sync(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @override_id asc_mrgsort4_sync__PUf16_AmrgPUf16_u8_u16_u16_u16_u16_b_u8
 * @archs 3510 2201
 * @brief 将已经排好序的最多4条队列，合并排列成1条队列，结果按照score域由大到小排序。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址的数组，待合并的Region Proposal队列。队列数量ASC_C_API_MRGSORT_ELEMENT_LEN值为4。
 * @param [in] repeat 迭代次数，每一次源操作数和目的操作数计算并跳过四个队列总长度。
 * @param [in] element_length_0 源队列0的长度。
 * @param [in] element_length_1 源队列1的长度。
 * @param [in] element_length_2 源队列2的长度。
 * @param [in] element_length_3 源队列3的长度。
 * @param [in] if_exhausted_suspension 某条队列耗尽后，指令是否需要停止。
 * @param [in] valid_bit 有效队列个数。有效取值如下：3：前两条队列有效。7：前三条队列有效。15：四条队列全部有效。
 */
__aicore__ inline void asc_mrgsort4_sync(
    __ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit);

/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @override_id asc_mrgsort4_sync__PUf32_AmrgPUf32_u8_u16_u16_u16_u16_b_u8
 * @archs 3510 2201
 * @brief 将已经排好序的最多4条队列，合并排列成1条队列，结果按照score域由大到小排序。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址的数组，待合并的Region Proposal队列。队列数量ASC_C_API_MRGSORT_ELEMENT_LEN值为4。
 * @param [in] repeat 迭代次数，每一次源操作数和目的操作数计算并跳过四个队列总长度。
 * @param [in] element_length_0 源队列0的长度。
 * @param [in] element_length_1 源队列1的长度。
 * @param [in] element_length_2 源队列2的长度。
 * @param [in] element_length_3 源队列3的长度。
 * @param [in] if_exhausted_suspension 某条队列耗尽后，指令是否需要停止。
 * @param [in] valid_bit 有效队列个数。有效取值如下：3：前两条队列有效。7：前三条队列有效。15：四条队列全部有效。
 */
__aicore__ inline void asc_mrgsort4_sync(
    __ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_relu_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素做线性整流Relu。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_relu_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_relu_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素做线性整流Relu。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_relu_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_relu_sync__PUi32_PUi32_u32
 * @archs 2201
 * @brief 按元素做线性整流Relu。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_relu_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行逐元素取最大值运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_max_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行逐元素取最大值运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_max_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 执行逐元素取最大值运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_max_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_sync__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @brief 执行逐元素取最大值运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_max_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_scalar_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @brief 矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_max_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_scalar_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @brief 矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_max_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_scalar_sync__PUi16_PUi16_i16_u32
 * @archs 2201
 * @brief 矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_max_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_scalar_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @brief 矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_max_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素求最小值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素求最小值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 按元素求最小值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_sync__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @brief 按元素求最小值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_fma_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素将src0和src1相乘并和dst相加。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_fma_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_fma_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素将src0和src1相乘并和dst相加。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_fma_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_fma_sync__PUf32_PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素将src0和src1相乘并和dst相加。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_fma_sync(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 执行矢量乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_sync__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @brief 执行矢量乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_scalar_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @brief 执行矢量与标量的乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_scalar_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @brief 执行矢量与标量的乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_scalar_sync__PUi16_PUi16_i16_u32
 * @archs 2201
 * @brief 执行矢量与标量的乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_scalar_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @brief 执行矢量与标量的乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_select_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量选择操作，给定两个源操作数src0和src1，根据条件选择元素，得到目的操作数dst。此接口配合[asc_set_cmp_mask(sel_mask)](../vector_compare/asc_set_cmp_mask.md)设置，规则为：当sel_mask的比特位是1时，从src0选取；当比特位是0时，从src1选择。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数0（矢量）的起始地址。
 * @param [in] src1 源操作数1（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_select_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_select_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量选择操作，给定两个源操作数src0和src1，根据条件选择元素，得到目的操作数dst。此接口配合[asc_set_cmp_mask(sel_mask)](../vector_compare/asc_set_cmp_mask.md)设置，规则为：当sel_mask的比特位是1时，从src0选取；当比特位是0时，从src1选择。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数0（矢量）的起始地址。
 * @param [in] src1 源操作数1（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_select_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 执行矢量减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_sync__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @brief 执行矢量减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_scalar_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @brief 执行矢量和标量的减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_scalar_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @brief 执行矢量和标量的减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_scalar_sync__PUi16_PUi16_i16_u32
 * @archs 2201
 * @brief 执行矢量和标量的减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_scalar_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @brief 执行矢量和标量的减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_or_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 每对元素按位或运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_or_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_or_sync__PUu16_PUu16_PUu16_u32
 * @archs 2201
 * @brief 每对元素按位或运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_or_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_not_sync__PUi16_PUi16_u32
 * @archs 2201
 * @brief 按元素做按位取反。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_not_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_not_sync__PUu16_PUu16_u32
 * @archs 2201
 * @brief 按元素做按位取反。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_not_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_rcp_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量的取倒数运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_rcp_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_rcp_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量的取倒数运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_rcp_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_shiftright_sync__PUi16_PUi16_i32_u32
 * @archs 2201
 * @brief 对src中的每个元素执行右移，右移大小为value。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量），表示右移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftright_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_shiftright_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @brief 对src中的每个元素执行右移，右移大小为value。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量），表示右移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftright_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_shiftright_sync__PUu16_PUu16_u32_u32
 * @archs 2201
 * @brief 对src中的每个元素执行右移，右移大小为value。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量），表示右移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftright_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_shiftright_sync__PUu32_PUu32_u32_u32
 * @archs 2201
 * @brief 对src中的每个元素执行右移，右移大小为value。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量），表示右移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftright_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_shiftright_round_sync__PUi16_PUi16_i32_u32
 * @archs 2201
 * @brief 对src中的每个元素执行右移，右移大小为value。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量），表示右移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftright_round_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_shiftright_round_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @brief 对src中的每个元素执行右移，右移大小为value。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量），表示右移的位数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_shiftright_round_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_mul_add_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量的乘加运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_add_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_mul_add_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量的乘加运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_add_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_mul_add_relu_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素将src0和dst相乘并加上src1，再进行Relu计算（结果和0对比取较大值），最终结果存放进dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_add_relu_sync(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_mul_add_relu_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素将src0和dst相乘并加上src1，再进行Relu计算（结果和0对比取较大值），最终结果存放进dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_add_relu_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq_scalar_sync__PUu8_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素判断src = value是否成立，若成立则输出结果的对应比特位为1，否则为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq_scalar_sync__PUu8_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素判断src = value是否成立，若成立则输出结果的对应比特位为1，否则为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq_scalar_sync__PUu8_PUi32_i32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素判断src = value是否成立，若成立则输出结果的对应比特位为1，否则为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_deq_int162b8_h_sync__PUi8_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型转换为int8_t或uint8_t类型，并将数据存放在每个DataBlock的高半块或低半块。使用该接口前需要调用[asc_set_deq_scale](asc_set_deq_scale.md)接口设置量化参数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_deq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_deq_int162b8_h_sync__PUu8_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型转换为int8_t或uint8_t类型，并将数据存放在每个DataBlock的高半块或低半块。使用该接口前需要调用[asc_set_deq_scale](asc_set_deq_scale.md)接口设置量化参数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_deq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_deq_int162b8_l_sync__PUi8_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型转换为int8_t或uint8_t类型，并将数据存放在每个DataBlock的高半块或低半块。使用该接口前需要调用[asc_set_deq_scale](asc_set_deq_scale.md)接口设置量化参数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_deq_int162b8_l_sync__PUu8_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型转换为int8_t或uint8_t类型，并将数据存放在每个DataBlock的高半块或低半块。使用该接口前需要调用[asc_set_deq_scale](asc_set_deq_scale.md)接口设置量化参数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int42half_sync__PUf16_PUi4x2_u32
 * @archs 2201
 * @brief 将int4_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int42half_sync(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int82half_sync__PUf16_PUi8_u32
 * @archs 2201
 * @brief 将int8_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int82half_sync(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_uint82half_sync__PUf16_PUu8_u32
 * @archs 2201
 * @brief 将uint8_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_uint82half_sync(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162float_sync__PUf32_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162float_sync(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_sync__PUf32_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_rna_sync__PUf32_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_rna_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_ru_sync__PUf32_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_ru_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_rd_sync__PUf32_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_rd_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_rn_sync__PUf32_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_rn_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_rz_sync__PUf32_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_rz_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322int16_sync__PUi16_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322int16_sync(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322int64_sync__PUi64_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322int64_sync(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642int32_sync__PUi32_PUi64_u32
 * @archs 2201
 * @brief 将int64_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int642int32_sync(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @override_id asc_gather_sync__PUi16_PUi16_PUu32_u32
 * @archs 2201
 * @brief 将src按照src_offset给定的偏移按元素收集到dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @override_id asc_gather_sync__PUu16_PUu16_PUu32_u32
 * @archs 2201
 * @brief 将src按照src_offset给定的偏移按元素收集到dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @override_id asc_gather_sync__PUf16_PUf16_PUu32_u32
 * @archs 2201
 * @brief 将src按照src_offset给定的偏移按元素收集到dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @override_id asc_gather_sync__PUbf16_PUbf16_PUu32_u32
 * @archs 2201
 * @brief 将src按照src_offset给定的偏移按元素收集到dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @override_id asc_gather_sync__PUi32_PUi32_PUu32_u32
 * @archs 2201
 * @brief 将src按照src_offset给定的偏移按元素收集到dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @override_id asc_gather_sync__PUu32_PUu32_PUu32_u32
 * @archs 2201
 * @brief 将src按照src_offset给定的偏移按元素收集到dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @override_id asc_gather_sync__PUf32_PUf32_PUu32_u32
 * @archs 2201
 * @brief 将src按照src_offset给定的偏移按元素收集到dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather_sync(
    __ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_and_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 执行矢量与运算。
 * @param [out] dst 目的操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_and_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_logic
 * @override_id asc_and_sync__PUu16_PUu16_PUu16_u32
 * @archs 2201
 * @brief 执行矢量与运算。
 * @param [out] dst 目的操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_and_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_leakyrelu_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @brief 执行矢量Leaky Relu运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址，为待处理数据。
 * @param [in] value 源操作数（标量），leaky_relu中alpha的值。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_leakyrelu_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_leakyrelu_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @brief 执行矢量Leaky Relu运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址，为待处理数据。
 * @param [in] value 源操作数（标量），leaky_relu中alpha的值。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_leakyrelu_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_lt_scalar_sync__PUu8_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量中每个位置和标量比较，如果值小于标量值则为1，否则为0，结果为每个bit位按小端序排布。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_lt_scalar_sync__PUu8_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量中每个位置和标量比较，如果值小于标量值则为1，否则为0，结果为每个bit位按小端序排布。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_sub_relu_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素求差，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_relu_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_sub_relu_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素求差，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_sub_relu_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 按元素求差，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_sub_relu_sync__PUi8_PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素求差，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_sub_relu_sync__PUf16_PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素求差，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_sub_relu_sync__PUi8_PUi16_PUi16_u32
 * @archs 2201
 * @brief 按元素求差，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub_relu_sync(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_rna_sync__PUi16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_rna_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_ru_sync__PUi16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_ru_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_rd_sync__PUi16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_rd_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_rn_sync__PUi16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_rn_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_rz_sync__PUi16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_rz_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_rsqrt_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素进行开方后取倒数的计算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_rsqrt_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_rsqrt_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素进行开方后取倒数的计算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_rsqrt_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_add_relu_sync__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素求和，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_relu_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_add_relu_sync__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素求和，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_add_relu_sync__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 按元素求和，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ge_sync__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief Ge（greater than or equal to），逐元素比较src0 >= src1是否成立，成立则输出结果为1，否则输出结果为0，每个元素的比较结果占一个bit。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ge_sync__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief Ge（greater than or equal to），逐元素比较src0 >= src1是否成立，成立则输出结果为1，否则输出结果为0，每个元素的比较结果占一个bit。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ge_sync__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief Ge（greater than or equal to），逐元素比较src0 >= src1是否成立，成立则输出结果为1，否则输出结果为0，每个元素的比较结果占一个bit。
 * @param [out] dst 目的操作数（矢量）的起始地址。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ge_sync__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief Ge（greater than or equal to），逐元素比较src0 >= src1是否成立，成立则输出结果为1，否则输出结果为0，每个元素的比较结果占一个bit。
 * @param [out] dst 目的操作数（矢量）的起始地址。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_reduce_sync__PUu16_PUu16_PUu16_u32
 * @archs 2201
 * @brief 以内置固定模式对应的二进制或者用户自定义输入的数值对应的gather mask（数据收集的掩码），从源操作数中选取元素写入目的操作数中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 在用户自定义模式下，由用户自定义的数据收集的掩码。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_reduce_sync(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_reduce_sync__PUu32_PUu32_PUu32_u32
 * @archs 2201
 * @brief 以内置固定模式对应的二进制或者用户自定义输入的数值对应的gather mask（数据收集的掩码），从源操作数中选取元素写入目的操作数中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 在用户自定义模式下，由用户自定义的数据收集的掩码。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_reduce_sync(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_rna_sync__PUf32_PUi64_u32
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_rna_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_ru_sync__PUf32_PUi64_u32
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_ru_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_rd_sync__PUf32_PUi64_u32
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_rd_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_rn_sync__PUf32_PUi64_u32
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_rn_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_rz_sync__PUf32_PUi64_u32
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_rz_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_vdeq_int162b8_h_sync__PUi8_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型转化为int8_t或uint8_t类型，并将数据存放在每个DataBlock的上半块或下半块。使用该接口前需要调用asc_set_deq_scale设置量化参数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_vdeq_int162b8_h_sync__PUu8_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型转化为int8_t或uint8_t类型，并将数据存放在每个DataBlock的上半块或下半块。使用该接口前需要调用asc_set_deq_scale设置量化参数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_vdeq_int162b8_l_sync__PUi8_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型转化为int8_t或uint8_t类型，并将数据存放在每个DataBlock的上半块或下半块。使用该接口前需要调用asc_set_deq_scale设置量化参数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_vdeq_int162b8_l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_vdeq_int162b8_l_sync__PUu8_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型转化为int8_t或uint8_t类型，并将数据存放在每个DataBlock的上半块或下半块。使用该接口前需要调用asc_set_deq_scale设置量化参数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_vdeq_int162b8_l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sqrt_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素做开方。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sqrt_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sqrt_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素做开方。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sqrt_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_axpy_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @brief 源操作数src中每个元素与标量value求积后和目的操作数dst中的对应元素相加。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_axpy_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_axpy_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @brief 源操作数src中每个元素与标量value求积后和目的操作数dst中的对应元素相加。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_axpy_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_axpy_sync__PUf32_PUf16_f16_u32
 * @archs 2201
 * @brief 源操作数src中每个元素与标量value求积后和目的操作数dst中的对应元素相加。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_axpy_sync(__ubuf__ float* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_lt_sync__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素判断src0 < src1是否成立，若成立则输出结果dst上的对应比特位为1，否则为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_lt_sync__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素判断src0 < src1是否成立，若成立则输出结果dst上的对应比特位为1，否则为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_lt_sync__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素判断src0 < src1是否成立，若成立则输出结果dst上的对应比特位为1，否则为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_lt_sync__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素判断src0 < src1是否成立，若成立则输出结果dst上的对应比特位为1，否则为0。
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

__aicore__ inline void asc_ne_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_ne_scalar_sync(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_sync__PUf16_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_rna_sync__PUf16_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_rna_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_ru_sync__PUf16_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_ru_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_rd_sync__PUf16_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_rd_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_rn_sync__PUf16_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_rn_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_rz_sync__PUf16_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_rz_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @override_id asc_gather_datablock_sync__PUu16_PUu16_PUu32_u16_u8_u8
 * @archs 2201
 * @brief 给定一个输入的矢量和一个地址偏移矢量，根据偏移地址按照DataBlock的粒度将输入矢量收集到目的矢量中。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @override_id asc_gather_datablock_sync__PUu32_PUu32_PUu32_u16_u8_u8
 * @archs 2201
 * @brief 给定一个输入的矢量和一个地址偏移矢量，根据偏移地址按照DataBlock的粒度将输入矢量收集到目的矢量中。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_scalar_sync__PUf16_PUf16_f16_u32
 * @archs 2201
 * @brief 源操作数矢量逐元素与标量相比，如果比标量值大，则取标量值，比标量值小，则取源操作数值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_scalar_sync__PUf32_PUf32_f32_u32
 * @archs 2201
 * @brief 源操作数矢量逐元素与标量相比，如果比标量值大，则取标量值，比标量值小，则取源操作数值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_scalar_sync__PUi16_PUi16_i16_u32
 * @archs 2201
 * @brief 源操作数矢量逐元素与标量相比，如果比标量值大，则取标量值，比标量值小，则取源操作数值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_scalar_sync__PUi32_PUi32_i32_u32
 * @archs 2201
 * @brief 源操作数矢量逐元素与标量相比，如果比标量值大，则取标量值，比标量值小，则取源操作数值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_add_relu_sync__PUi8_PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素求和，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_add_relu_sync__PUf16_PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素求和，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_fused
 * @override_id asc_add_relu_sync__PUi8_PUi16_PUi16_u32
 * @archs 2201
 * @brief 按元素求和，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add_relu_sync(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_gt_sync__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素比较两个矢量的大小关系，若比较后的结果为真，则输出结果的对应比特位为1，否则为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_gt_sync__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素比较两个矢量的大小关系，若比较后的结果为真，则输出结果的对应比特位为1，否则为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_gt_sync__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素比较两个矢量的大小关系，若比较后的结果为真，则输出结果的对应比特位为1，否则为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_gt_sync__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素比较两个矢量的大小关系，若比较后的结果为真，则输出结果的对应比特位为1，否则为0。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_sync__PUu8_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_rna_sync__PUu8_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_rna_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_ru_sync__PUu8_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_ru_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_rd_sync__PUu8_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_rd_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_rn_sync__PUu8_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_rn_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_rz_sync__PUu8_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_rz_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_le_sync__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素判断src0 <= src1是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。 当结果输出到寄存器中时，可在计算完成后调用[asc_get_cmp_mask](asc_get_cmp_mask.md)获取比较结果。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_le_sync__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素判断src0 <= src1是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。 当结果输出到寄存器中时，可在计算完成后调用[asc_get_cmp_mask](asc_get_cmp_mask.md)获取比较结果。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_le_sync__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素判断src0 <= src1是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。 当结果输出到寄存器中时，可在计算完成后调用[asc_get_cmp_mask](asc_get_cmp_mask.md)获取比较结果。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_le_sync__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素判断src0 <= src1是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。 当结果输出到寄存器中时，可在计算完成后调用[asc_get_cmp_mask](asc_get_cmp_mask.md)获取比较结果。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_le_scalar_sync__PUu8_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素判断src <= value是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_le_scalar_sync__PUu8_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素判断src <= value是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。
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

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_pair_reduce_sum_sync__PUf16_PUf16_u32
 * @archs 2201
 * @brief 对输入数据做归约操作，得到数据总和。相邻两个（奇偶）元素求和，例如（a1，a2，a3，a4，a5，a6…），相邻两个数据求和为（a1+a2，a3+a4，a5+a6…）。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_pair_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_reduce
 * @override_id asc_pair_reduce_sum_sync__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对输入数据做归约操作，得到数据总和。相邻两个（奇偶）元素求和，例如（a1，a2，a3，a4，a5，a6…），相邻两个数据求和为（a1+a2，a3+a4，a5+a6…）。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_pair_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_datamove
 * @override_id asc_copy_sync__PUi16_PUi16_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到UB。支持高维切分。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_copy_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_datamove
 * @override_id asc_copy_sync__PUu16_PUu16_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到UB。支持高维切分。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_copy_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_datamove
 * @override_id asc_copy_sync__PUi32_PUi32_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到UB。支持高维切分。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_copy_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_datamove
 * @override_id asc_copy_sync__PUu32_PUu32_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到UB。支持高维切分。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_copy_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_datamove
 * @override_id asc_copy_ub2ub_sync__PUv_PUv_u32
 * @archs 3510 2201
 * @brief 将数据从Unified Buffer（UB）搬运到UB。
 * @param [out] dst 目的UB地址。
 * @param [in] src 源UB地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_ub2ub_sync(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub
 * @override_id asc_copy_gm2ub_sync__PUv_PGv_u32
 * @archs 2201
 * @brief 将数据从Global Memory (GM)搬运到Unified Buffer（UB）。
 * @param [out] dst 目的UB地址。
 * @param [in] src 源GM地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_gm2ub_sync(__ubuf__ void* dst, __gm__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm
 * @override_id asc_copy_ub2gm_sync__PGv_PUv_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)。
 * @param [out] dst 目的GM地址。
 * @param [in] src 源UB地址。
 * @param [in] size 搬运数据大小（字节）。
 */
__aicore__ inline void asc_copy_ub2gm_sync(__gm__ void* dst, __ubuf__ void* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @override_id asc_copy_gm2ub_align_sync__PUi8_PGi8_u32
 * @archs 2201
 * @brief 提供数据非对齐搬运的功能，将数据从Global Memory (GM)搬运到Unified Buffer（UB），并支持8位/16位/32位数据类型搬运。
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @override_id asc_copy_gm2ub_align_sync__PUu8_PGu8_u32
 * @archs 2201
 * @brief 提供数据非对齐搬运的功能，将数据从Global Memory (GM)搬运到Unified Buffer（UB），并支持8位/16位/32位数据类型搬运。
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @override_id asc_copy_gm2ub_align_sync__PUf16_PGf16_u32
 * @archs 2201
 * @brief 提供数据非对齐搬运的功能，将数据从Global Memory (GM)搬运到Unified Buffer（UB），并支持8位/16位/32位数据类型搬运。
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ half* dst, __gm__ half* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @override_id asc_copy_gm2ub_align_sync__PUbf16_PGbf16_u32
 * @archs 2201
 * @brief 提供数据非对齐搬运的功能，将数据从Global Memory (GM)搬运到Unified Buffer（UB），并支持8位/16位/32位数据类型搬运。
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @override_id asc_copy_gm2ub_align_sync__PUi16_PGi16_u32
 * @archs 2201
 * @brief 提供数据非对齐搬运的功能，将数据从Global Memory (GM)搬运到Unified Buffer（UB），并支持8位/16位/32位数据类型搬运。
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @override_id asc_copy_gm2ub_align_sync__PUu16_PGu16_u32
 * @archs 2201
 * @brief 提供数据非对齐搬运的功能，将数据从Global Memory (GM)搬运到Unified Buffer（UB），并支持8位/16位/32位数据类型搬运。
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @override_id asc_copy_gm2ub_align_sync__PUf32_PGf32_u32
 * @archs 2201
 * @brief 提供数据非对齐搬运的功能，将数据从Global Memory (GM)搬运到Unified Buffer（UB），并支持8位/16位/32位数据类型搬运。
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ float* dst, __gm__ float* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @override_id asc_copy_gm2ub_align_sync__PUi32_PGi32_u32
 * @archs 2201
 * @brief 提供数据非对齐搬运的功能，将数据从Global Memory (GM)搬运到Unified Buffer（UB），并支持8位/16位/32位数据类型搬运。
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_gm2ub_align
 * @override_id asc_copy_gm2ub_align_sync__PUu32_PGu32_u32
 * @archs 2201
 * @brief 提供数据非对齐搬运的功能，将数据从Global Memory (GM)搬运到Unified Buffer（UB），并支持8位/16位/32位数据类型搬运。
 * @param [out] dst 目的UB的起始地址。需要32字节对齐。
 * @param [in] src 源GM的起始地址。需要1字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @override_id asc_copy_ub2gm_align_sync__PGu8_PUu8_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @override_id asc_copy_ub2gm_align_sync__PGi8_PUi8_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @override_id asc_copy_ub2gm_align_sync__PGf16_PUf16_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ half* dst, __ubuf__ half* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @override_id asc_copy_ub2gm_align_sync__PGu16_PUu16_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @override_id asc_copy_ub2gm_align_sync__PGi16_PUi16_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @override_id asc_copy_ub2gm_align_sync__PGbf16_PUbf16_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @override_id asc_copy_ub2gm_align_sync__PGu32_PUu32_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @override_id asc_copy_ub2gm_align_sync__PGf32_PUf32_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ float* dst, __ubuf__ float* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @override_id asc_copy_ub2gm_align_sync__PGi32_PUi32_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @override_id asc_copy_ub2gm_align_sync__PGf64_PUf64_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ double* dst, __ubuf__ double* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @override_id asc_copy_ub2gm_align_sync__PGi64_PUi64_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int64_t* dst, __ubuf__ int64_t* src, uint32_t size);

/**
 * @ingroup simd_c_api.vector_datamove.asc_copy_ub2gm_align
 * @override_id asc_copy_ub2gm_align_sync__PGu64_PUu64_u32
 * @archs 2201
 * @brief 将数据从Unified Buffer（UB）搬运到Global Memory (GM)，支持8位/16位/32位分块拷贝操作。
 * @param [out] dst 目的GM的起始地址。需要1字节对齐。
 * @param [in] src 源UB的起始地址。需要32字节对齐。
 * @param [in] size 搬运数据大小，单位为字节。取值范围：[0, 2097151]。
 */
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint64_t* dst, __ubuf__ uint64_t* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t size);

__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t size);

__aicore__ inline void asc_copy_ub2l1_sync(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_VECTOR_COMPUTE_COMPOSITE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_COMPOSITE_VECTOR_COMPUTE_COMPOSITE_H
#endif
