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
#error "vector_compute/compute/vector_gather.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_GATHER_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_GATHER_H
#define INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_GATHER_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/vector_gather_intf_impl.h"
#endif
/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers int16_t elements from src into dst using the per-element byte offset src_offset over repeat iterations with dst_repeat_stride (void return, PIPE_V).
 *
 * @override_id asc_gather__PUi16_PUi16_PUu32_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers int16_t elements from src into dst using the per-element byte offset src_offset for count elements (void return, PIPE_V).
 *
 * @override_id asc_gather__PUi16_PUi16_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers uint16_t elements from src into dst using the per-element byte offset src_offset over repeat iterations with dst_repeat_stride (void return, PIPE_V).
 *
 * @override_id asc_gather__PUu16_PUu16_PUu32_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers uint16_t elements from src into dst using the per-element byte offset src_offset for count elements (void return, PIPE_V).
 *
 * @override_id asc_gather__PUu16_PUu16_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers half elements from src into dst using the per-element byte offset src_offset over repeat iterations with dst_repeat_stride (void return, PIPE_V).
 *
 * @override_id asc_gather__PUf16_PUf16_PUu32_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride, uint8_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers half elements from src into dst using the per-element byte offset src_offset for count elements (void return, PIPE_V).
 *
 * @override_id asc_gather__PUf16_PUf16_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers bfloat16_t elements from src into dst using the per-element byte offset src_offset over repeat iterations with dst_repeat_stride (void return, PIPE_V).
 *
 * @override_id asc_gather__PUbf16_PUbf16_PUu32_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers bfloat16_t elements from src into dst using the per-element byte offset src_offset for count elements (void return, PIPE_V).
 *
 * @override_id asc_gather__PUbf16_PUbf16_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers int32_t elements from src into dst using the per-element byte offset src_offset over repeat iterations with dst_repeat_stride (void return, PIPE_V).
 *
 * @override_id asc_gather__PUi32_PUi32_PUu32_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers int32_t elements from src into dst using the per-element byte offset src_offset for count elements (void return, PIPE_V).
 *
 * @override_id asc_gather__PUi32_PUi32_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers uint32_t elements from src into dst using the per-element byte offset src_offset over repeat iterations with dst_repeat_stride (void return, PIPE_V).
 *
 * @override_id asc_gather__PUu32_PUu32_PUu32_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers uint32_t elements from src into dst using the per-element byte offset src_offset for count elements (void return, PIPE_V).
 *
 * @override_id asc_gather__PUu32_PUu32_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers float elements from src into dst using the per-element byte offset src_offset over repeat iterations with dst_repeat_stride (void return, PIPE_V).
 *
 * @override_id asc_gather__PUf32_PUf32_PUu32_u16_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers float elements from src into dst using the per-element byte offset src_offset for count elements (void return, PIPE_V).
 *
 * @override_id asc_gather__PUf32_PUf32_PUu32_u32
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 源操作数的每个元素对应的地址偏移，该偏移量相对于源操作数的起始基地址而言，单位为Bytes。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_gather(
    __ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers uint16 elements from a source vector into dst at DataBlock granularity according to the src_offset address-offset vector, using repeat and stride parameters.
 *
 * @override_id asc_gather_datablock__PUu16_PUu16_PUu32_u16_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 每个Datablock在源操作数中的地址偏移。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_gather_datablock(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t dst_block_stride, uint8_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_gather
 * @brief Gathers uint32 elements from a source vector into dst at DataBlock granularity according to the src_offset address-offset vector, using repeat and stride parameters.
 *
 * @override_id asc_gather_datablock__PUu32_PUu32_PUu32_u16_u8_u8
 * @archs 2201
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] src_offset 每个Datablock在源操作数中的地址偏移。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] repeat 迭代次数。
 */
__aicore__ inline void asc_gather_datablock(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride,
    uint8_t dst_block_stride, uint8_t repeat);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_GATHER_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_GATHER_H
#endif
