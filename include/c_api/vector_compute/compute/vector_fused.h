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
#error "vector_compute/compute/vector_fused.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_FUSED_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_FUSED_H
#define INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_FUSED_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/vector_fused_intf_impl.h"
#endif
/**
 * @brief 设置DEQSCALE（量化参数）寄存器的值。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_set_deq_scale.md
 */
__aicore__ inline void asc_set_deq_scale(float scale, int16_t offset, bool sign_mode);

__aicore__ inline void asc_set_deq_scale(
    __ubuf__ uint64_t* tmp, float scale_arr[ASC_VDEQ_SIZE], int16_t offset_arr[ASC_VDEQ_SIZE],
    bool sign_mode_arr[ASC_VDEQ_SIZE]);

__aicore__ inline void asc_set_deq_scale(half scale);

/**
 * @brief 按元素求积，并将结果转换为int8_t类型。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_mul_cast_half2int8.md
 */
__aicore__ inline void asc_mul_cast_half2int8(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_mul_cast_half2int8(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @brief 按元素求积，并将结果转换为uint8_t类型。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_mul_cast_half2uint8.md
 */
__aicore__ inline void asc_mul_cast_half2uint8(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_mul_cast_half2uint8(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/**
 * @brief 对输入的int32_t类型的数据按元素做量化并转换为half类型。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_deq_int322half.md
 */
__aicore__ inline void asc_deq_int322half(
    __ubuf__ half* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

__aicore__ inline void asc_deq_int322half(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @brief 按元素做线性整流Relu
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_relu.md
 */
__aicore__ inline void asc_relu(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_relu(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);

__aicore__ inline void asc_relu(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_relu(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

__aicore__ inline void asc_relu(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_relu(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @brief 按元素将src0和src1相乘并和dst相加，将最终结果存放进dst中
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_fma.md
 */
__aicore__ inline void asc_fma(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

__aicore__ inline void asc_fma(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_fma(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

__aicore__ inline void asc_fma(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_fma(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

__aicore__ inline void asc_fma(
    __ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @brief 执行矢量的乘加运算。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_mul_add.md
 */
__aicore__ inline void asc_mul_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

__aicore__ inline void asc_mul_add(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_mul_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

__aicore__ inline void asc_mul_add(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @brief 按元素将src0和dst相乘并加上src1，再进行Relu计算（结果和0对比取较大值），最终结果存放进dst中。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_mul_add_relu.md
 */
__aicore__ inline void asc_mul_add_relu(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_mul_add_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

__aicore__ inline void asc_mul_add_relu(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_mul_add_relu(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/**
 * @brief 将int16_t类型转换为int8_t或uint8_t类型，并将数据存放在每个DataBlock的高半块或低半块。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_deq_int162b8.md
 */
__aicore__ inline void asc_deq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_deq_int162b8_h(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

__aicore__ inline void asc_deq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_deq_int162b8_h(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

__aicore__ inline void asc_deq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_deq_int162b8_l(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

__aicore__ inline void asc_deq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_deq_int162b8_l(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

/**
 * @brief 执行矢量Leaky Relu运算。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_leakyrelu.md
 */
__aicore__ inline void asc_leakyrelu(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_leakyrelu(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

__aicore__ inline void asc_leakyrelu(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_leakyrelu(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/**
 * @brief 按元素求差，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_sub_relu.md
 */
__aicore__ inline void asc_sub_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

__aicore__ inline void asc_sub_relu(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_sub_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

__aicore__ inline void asc_sub_relu(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_sub_relu(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

__aicore__ inline void asc_sub_relu(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_sub_relu(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

__aicore__ inline void asc_sub_relu(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_sub_relu(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

__aicore__ inline void asc_sub_relu(
    __ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_sub_relu(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

__aicore__ inline void asc_sub_relu(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @brief 按元素求和，再进行Relu计算（结果和0对比取较大值），并提供转换最终结果的数据类型的功能(s162s8、f322f16、f162s8)。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_add_relu.md
 */
__aicore__ inline void asc_add_relu(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_add_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

__aicore__ inline void asc_add_relu(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_add_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

__aicore__ inline void asc_add_relu(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_add_relu(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/**
 * @brief 将int16_t类型转化为int8_t或uint8_t类型，并将数据存放在每个DataBlock的上半块或下半块。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_add_relu.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_axpy.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_deq_int162b8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_deq_int322half.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_fma.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_leakyrelu.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_mul_add.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_mul_add_relu.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_mul_cast_half2int8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_mul_cast_half2uint8.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_relu.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_set_deq_scale.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_sub_relu.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_vdeq_int162b8.md
 */
__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_vdeq_int162b8_h(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_vdeq_int162b8_h(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

__aicore__ inline void asc_vdeq_int162b8_l(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

__aicore__ inline void asc_vdeq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);

__aicore__ inline void asc_vdeq_int162b8_l(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

__aicore__ inline void asc_vdeq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @brief 源操作数src中每个元素与标量value求积后和目的操作数dst中的对应元素相加
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_axpy.md
 */
__aicore__ inline void asc_axpy(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);

__aicore__ inline void asc_axpy(
    __ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

__aicore__ inline void asc_axpy(
    __ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ half* src, half value, uint32_t count);

__aicore__ inline void asc_axpy(
    __ubuf__ float* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_add_relu(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

__aicore__ inline void asc_add_relu(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_add_relu(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

__aicore__ inline void asc_add_relu(
    __ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_add_relu(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

__aicore__ inline void asc_add_relu(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_FUSED_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_FUSED_H
#endif
