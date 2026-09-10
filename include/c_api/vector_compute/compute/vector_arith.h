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
#error "vector_compute/compute/vector_arith.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_ARITH_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_ARITH_H
#define INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_ARITH_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"
#endif
/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add__PUf16_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_add(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add__PUf32_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_add(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add__PUi16_PUi16_PUi16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_add(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_add(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add__PUi32_PUi32_PUi32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_add(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_scalar__PUf16_PUf16_f16_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_add_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_scalar__PUf16_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_add_scalar(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_scalar__PUf32_PUf32_f32_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_add_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_scalar__PUf32_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_add_scalar(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_scalar__PUi16_PUi16_i16_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_add_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_scalar__PUi16_PUi16_i16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_add_scalar(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_scalar__PUi32_PUi32_i32_u32
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_add_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_add_scalar__PUi32_PUi32_i32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量加法运算。
 * @param [out] dst 目的操作数。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_add_scalar(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_div__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量除法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址
 * @param [in] src0 源操作数（矢量）的起始地址
 * @param [in] src1 源操作数（矢量）的起始地址
 * @param [in] count 参与连续计算的元素个数
 */
__aicore__ inline void asc_div(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_div__PUf16_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量除法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址
 * @param [in] src0 源操作数（矢量）的起始地址
 * @param [in] src1 源操作数（矢量）的起始地址
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_div(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_div__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量除法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址
 * @param [in] src0 源操作数（矢量）的起始地址
 * @param [in] src1 源操作数（矢量）的起始地址
 * @param [in] count 参与连续计算的元素个数
 */
__aicore__ inline void asc_div(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_div__PUf32_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量除法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址
 * @param [in] src0 源操作数（矢量）的起始地址
 * @param [in] src1 源操作数（矢量）的起始地址
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_div(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_exp__PUf16_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素取自然指数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_exp(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_exp__PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素取自然指数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_exp(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_exp__PUf32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素取自然指数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_exp(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_exp__PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素取自然指数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_exp(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_log__PUf16_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素取自然对数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_log(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_log__PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素取自然对数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_log(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_log__PUf32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素取自然对数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_log(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_log__PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素取自然对数。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_log(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_abs__PUf16_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素取绝对值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_abs(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_abs__PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素取绝对值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_abs(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_abs__PUf32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素取绝对值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_abs(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_abs__PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素取绝对值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_abs(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行逐元素取最大值运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_max(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max__PUf16_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行逐元素取最大值运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_max(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行逐元素取最大值运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_max(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max__PUf32_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行逐元素取最大值运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_max(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 执行逐元素取最大值运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_max(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max__PUi16_PUi16_PUi16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行逐元素取最大值运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_max(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @brief 执行逐元素取最大值运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_max(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max__PUi32_PUi32_PUi32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行逐元素取最大值运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_max(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_scalar__PUf16_PUf16_f16_u32
 * @archs 2201
 * @brief 矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_max_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_scalar__PUf16_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_max_scalar(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_scalar__PUf32_PUf32_f32_u32
 * @archs 2201
 * @brief 矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_max_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_scalar__PUf32_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_max_scalar(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_scalar__PUi16_PUi16_i16_u32
 * @archs 2201
 * @brief 矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_max_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_scalar__PUi16_PUi16_i16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_max_scalar(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_scalar__PUi32_PUi32_i32_u32
 * @archs 2201
 * @brief 矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_max_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_max_scalar__PUi32_PUi32_i32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 矢量src的逐个元素与标量value比较大小，接着按照对应的比特位将最大值存入dst中。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_max_scalar(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素求最小值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min__PUf16_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素求最小值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_min(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素求最小值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min__PUf32_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素求最小值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_min(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 按元素求最小值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min__PUi16_PUi16_PUi16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素求最小值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_min(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @brief 按元素求最小值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min__PUi32_PUi32_PUi32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 按元素求最小值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_min(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul__PUf16_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_mul(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul__PUf32_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_mul(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 执行矢量乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul__PUi16_PUi16_PUi16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_mul(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @brief 执行矢量乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul__PUi32_PUi32_PUi32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_mul(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_scalar__PUf16_PUf16_f16_u32
 * @archs 2201
 * @brief 执行矢量与标量的乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_scalar__PUf16_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量与标量的乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_mul_scalar(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_scalar__PUf32_PUf32_f32_u32
 * @archs 2201
 * @brief 执行矢量与标量的乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_scalar__PUf32_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量与标量的乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_mul_scalar(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_scalar__PUi16_PUi16_i16_u32
 * @archs 2201
 * @brief 执行矢量与标量的乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_scalar__PUi16_PUi16_i16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量与标量的乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_mul_scalar(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_scalar__PUi32_PUi32_i32_u32
 * @archs 2201
 * @brief 执行矢量与标量的乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_mul_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_mul_scalar__PUi32_PUi32_i32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量与标量的乘法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 标量源操作数。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_mul_scalar(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub__PUf16_PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub__PUf16_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_sub(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub__PUf32_PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub__PUf32_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_sub(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub__PUi16_PUi16_PUi16_u32
 * @archs 2201
 * @brief 执行矢量减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub__PUi16_PUi16_PUi16_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_sub(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub__PUi32_PUi32_PUi32_u32
 * @archs 2201
 * @brief 执行矢量减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sub(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub__PUi32_PUi32_PUi32_u8_u8_u8_u8_u8_u8_u8
 * @archs 2201
 * @brief 执行矢量减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src0_block_stride 源操作数0单次迭代内不同DataBlock间地址步长。
 * @param [in] src1_block_stride 源操作数1单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src0_repeat_stride 源操作数0相邻迭代间相同DataBlock的地址步长。
 * @param [in] src1_repeat_stride 源操作数1相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_sub(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_scalar__PUf16_PUf16_f16_u32
 * @archs 2201
 * @brief 执行矢量和标量的减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_sub_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_scalar__PUf16_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量和标量的减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_sub_scalar(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_scalar__PUf32_PUf32_f32_u32
 * @archs 2201
 * @brief 执行矢量和标量的减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_sub_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_scalar__PUf32_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量和标量的减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_sub_scalar(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_scalar__PUi16_PUi16_i16_u32
 * @archs 2201
 * @brief 执行矢量和标量的减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_sub_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_scalar__PUi16_PUi16_i16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量和标量的减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_sub_scalar(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_scalar__PUi32_PUi32_i32_u32
 * @archs 2201
 * @brief 执行矢量和标量的减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_sub_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sub_scalar__PUi32_PUi32_i32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量和标量的减法运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_sub_scalar(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_rcp__PUf16_PUf16_u32
 * @archs 2201
 * @brief 执行矢量的取倒数运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_rcp(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_rcp__PUf16_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量的取倒数运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_rcp(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_rcp__PUf32_PUf32_u32
 * @archs 2201
 * @brief 执行矢量的取倒数运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_rcp(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_rcp__PUf32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 执行矢量的取倒数运算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_rcp(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_rsqrt__PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素进行开方后取倒数的计算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_rsqrt(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_rsqrt__PUf16_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素进行开方后取倒数的计算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_rsqrt(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_rsqrt__PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素进行开方后取倒数的计算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_rsqrt(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_rsqrt__PUf32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素进行开方后取倒数的计算。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_rsqrt(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sqrt__PUf16_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素做开方。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_sqrt(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sqrt__PUf16_PUf16_u32
 * @archs 2201
 * @brief 按元素做开方。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sqrt(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sqrt__PUf32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 按元素做开方。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_sqrt(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_sqrt__PUf32_PUf32_u32
 * @archs 2201
 * @brief 按元素做开方。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_sqrt(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_scalar__PUf16_PUf16_f16_u32
 * @archs 2201
 * @brief 源操作数矢量逐元素与标量相比，如果比标量值大，则取标量值，比标量值小，则取源操作数值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_scalar__PUf16_PUf16_f16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 源操作数矢量逐元素与标量相比，如果比标量值大，则取标量值，比标量值小，则取源操作数值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_min_scalar(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_scalar__PUf32_PUf32_f32_u32
 * @archs 2201
 * @brief 源操作数矢量逐元素与标量相比，如果比标量值大，则取标量值，比标量值小，则取源操作数值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_scalar__PUf32_PUf32_f32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 源操作数矢量逐元素与标量相比，如果比标量值大，则取标量值，比标量值小，则取源操作数值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_min_scalar(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_scalar__PUi16_PUi16_i16_u32
 * @archs 2201
 * @brief 源操作数矢量逐元素与标量相比，如果比标量值大，则取标量值，比标量值小，则取源操作数值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_scalar__PUi16_PUi16_i16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 源操作数矢量逐元素与标量相比，如果比标量值大，则取标量值，比标量值小，则取源操作数值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_min_scalar(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_scalar__PUi32_PUi32_i32_u32
 * @archs 2201
 * @brief 源操作数矢量逐元素与标量相比，如果比标量值大，则取标量值，比标量值小，则取源操作数值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_min_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_arith
 * @override_id asc_min_scalar__PUi32_PUi32_i32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 源操作数矢量逐元素与标量相比，如果比标量值大，则取标量值，比标量值小，则取源操作数值。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] value 源操作数（标量）。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_min_scalar(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_ARITH_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_ARITH_H
#endif
