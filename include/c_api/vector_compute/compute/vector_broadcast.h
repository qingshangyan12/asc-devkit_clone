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
#error "vector_compute/compute/vector_broadcast.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_BROADCAST_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_BROADCAST_H
#define INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_BROADCAST_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"
#endif
/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_brcb__PUu16_PUu16_u16_u16_u8
 * @archs 2201
 * @brief 给定一个输入矢量，每一次取输入矢量中的8个数填充到结果矢量的8个datablock（32Bytes）中去，每个数对应一个datablock。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_brcb(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_brcb__PUu32_PUu32_u16_u16_u8
 * @archs 2201
 * @brief 给定一个输入矢量，每一次取输入矢量中的8个数填充到结果矢量的8个datablock（32Bytes）中去，每个数对应一个datablock。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_brcb(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride,
    uint8_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUf16_f16_u8_u16_u16
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_duplicate(
    __ubuf__ half* dst, half src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUf16_f16_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUi16_i16_u8_u16_u16
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_duplicate(
    __ubuf__ int16_t* dst, int16_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUi16_i16_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUu16_u16_u8_u16_u16
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_duplicate(
    __ubuf__ uint16_t* dst, uint16_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUu16_u16_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUbf16_bf16_u8_u16_u16
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_duplicate(
    __ubuf__ bfloat16_t* dst, bfloat16_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUbf16_bf16_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUf32_f32_u8_u16_u16
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_duplicate(
    __ubuf__ float* dst, float src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUf32_f32_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUi32_i32_u8_u16_u16
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_duplicate(
    __ubuf__ int32_t* dst, int32_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUi32_i32_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUu32_u32_u8_u16_u16
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_duplicate(
    __ubuf__ uint32_t* dst, uint32_t src, uint8_t repeat, uint16_t dst_block_stride, uint16_t dst_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_broadcast
 * @override_id asc_duplicate__PUu32_u32_u32
 * @archs 2201
 * @brief 执行矢量复制（Duplicate）操作，将标量值复制填充到矢量中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（标量）。
 * @param [in] count 参与连续复制的元素个数。
 */
__aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_BROADCAST_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_BROADCAST_H
#endif
