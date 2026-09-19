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
#error "vector_compute/compute/vector_permute_sel.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_PERMUTE_SEL_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_PERMUTE_SEL_H
#define INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_PERMUTE_SEL_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/vector_permute_sel_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/vector_permute_sel_intf_impl.h"
#endif
/**
 * @brief 用于设置transpose的16个DataBlock地址，将操作数地址序列与地址寄存器关联，接口要求前8个和后8个地址序列与地址寄存器分别关联。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_select.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_set_va_reg.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_squeeze.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transpose.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transto5hd.md
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int8_t** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint8_t** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int16_t** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint16_t** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ half** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int32_t** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint32_t** src_array);

__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ float** src_array);

/**
 * @brief 用于实现16*16的二维矩阵数据块转置。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_select.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_set_va_reg.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_squeeze.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transpose.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transto5hd.md
 */
__aicore__ inline void asc_transpose(__ubuf__ int16_t* dst, __ubuf__ int16_t* src);

__aicore__ inline void asc_transpose(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src);

/**
 * @brief 数据格式转换，一般用于将NCHW格式转换成NC1HWC0格式。特别的，也可以用于二维矩阵数据块的转置。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_select.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_set_va_reg.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_squeeze.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transpose.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transto5hd.md
 */
__aicore__ inline void asc_transto5hd_b8(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half);

__aicore__ inline void asc_transto5hd_b16(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride);

__aicore__ inline void asc_transto5hd_b32(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride);

/**
 * @brief 执行矢量选择操作，给定两个源操作数src0和src1，根据条件选择元素，得到目的操作数dst。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_select.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_set_va_reg.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_squeeze.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transpose.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transto5hd.md
 */
__aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

__aicore__ inline void asc_select(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

__aicore__ inline void asc_select(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @brief 通过比较掩码src1，将src0中的向量压缩成更短的向量，存储到dst中。
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_select.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_set_va_reg.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_squeeze.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transpose.md
 * @see docs/zh/api/SIMD-API/c_api/vector_compute/vector_permute_sel/asc_transto5hd.md
 */
__aicore__ inline void asc_squeeze(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_squeeze(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_transto5hd_b32 and asc_sync)
__aicore__ inline void asc_transto5hd_b32_sync(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_transto5hd_b16 and asc_sync)
__aicore__ inline void asc_transto5hd_b16_sync(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride);

ASC_DEPRECATED(9.2.0, "2027/09/07", asc_transto5hd_b8 and asc_sync)
__aicore__ inline void asc_transto5hd_b8_sync(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_PERMUTE_SEL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_PERMUTE_SEL_H
#endif
