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
#error "vector_compute/compute/vector_convert.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_CONVERT_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_CONVERT_H
#define INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_CONVERT_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"
#endif

// ==========asc_transto5hd(b8/b16/b32)==========
/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_transto5hd_b8__uba8_uba8_u8_u16_u16_b_b
 * @archs 3510 2201
 * @brief 数据格式转换，一般用于将NCHW格式转换成NC1HWC0格式。特别的，也可以用于二维矩阵数据块的转置。本接口单次Repeat内可处理512Byte的数据（16个DataBlock），根据数据类型不同，支持不同shape的矩阵转置（比如数据类型为half时，单次Repeat可完成16*16大小的矩阵转置），同时还可以支持多次Repeat操作。
 * @param [out] dst 目的地址寄存器。
 * @param [in] src 源地址寄存器。
 * @param [in] repeat 重复迭代次数， repeat∈[0,255]。注意事项：&bull;当repeat为1时，目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置加上dst_stride/src_stride。如果要让目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，需要将dst_stride/src_stride置为0。&bull;当repeat大于1时，第一次repeat中目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，第二次会加上dst_stride/src_stride。以此类推。
 * @param [in] dst_stride 相邻迭代间，目的操作数相同DataBlock地址Stride，单位：DataBlock。相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../../general_description_and_constraints.md#repeatStride)。
 * @param [in] src_stride 相邻迭代间，源操作数相同DataBlock地址Stride，单位：DataBlock。相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../../general_description_and_constraints.md#repeatStride)。
 * @param [in] dst_high_half 指定每个dst地址中的数据存储到DataBlock的高半部还是低半部，该配置只配置int8_t/uint8_t的数据类型。支持的数据类型为bool，有以下两种取值：&bull; true:表示存储于DataBlock的高半部。&bull; false:表示存储于DataBlock的低半部。
 * @param [in] src_high_half 指定每个src地址中的数据存储到DataBlock的高半部还是低半部，该配置只配置int8_t/uint8_t的数据类型。支持的数据类型为bool，有以下两种取值：&bull; true:表示存储于DataBlock的高半部。&bull;false:表示存储于DataBlock的低半部。
 */
__aicore__ inline void asc_transto5hd_b8(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half);
/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_transto5hd_b16__uba8_uba8_u8_u16_u16
 * @archs 3510 2201
 * @brief 数据格式转换，一般用于将NCHW格式转换成NC1HWC0格式。特别的，也可以用于二维矩阵数据块的转置。本接口单次Repeat内可处理512Byte的数据（16个DataBlock），根据数据类型不同，支持不同shape的矩阵转置（比如数据类型为half时，单次Repeat可完成16*16大小的矩阵转置），同时还可以支持多次Repeat操作。
 * @param [out] dst 目的地址寄存器。
 * @param [in] src 源地址寄存器。
 * @param [in] repeat 重复迭代次数， repeat∈[0,255]。注意事项：&bull;当repeat为1时，目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置加上dst_stride/src_stride。如果要让目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，需要将dst_stride/src_stride置为0。&bull;当repeat大于1时，第一次repeat中目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，第二次会加上dst_stride/src_stride。以此类推。
 * @param [in] dst_stride 相邻迭代间，目的操作数相同DataBlock地址Stride，单位：DataBlock。相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../../general_description_and_constraints.md#repeatStride)。
 * @param [in] src_stride 相邻迭代间，源操作数相同DataBlock地址Stride，单位：DataBlock。相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../../general_description_and_constraints.md#repeatStride)。
 */
__aicore__ inline void asc_transto5hd_b16(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride);
/**
 * @ingroup simd_c_api.vector_compute.vector_permute_sel
 * @override_id asc_transto5hd_b32__uba8_uba8_u8_u16_u16
 * @archs 3510 2201
 * @brief 数据格式转换，一般用于将NCHW格式转换成NC1HWC0格式。特别的，也可以用于二维矩阵数据块的转置。本接口单次Repeat内可处理512Byte的数据（16个DataBlock），根据数据类型不同，支持不同shape的矩阵转置（比如数据类型为half时，单次Repeat可完成16*16大小的矩阵转置），同时还可以支持多次Repeat操作。
 * @param [out] dst 目的地址寄存器。
 * @param [in] src 源地址寄存器。
 * @param [in] repeat 重复迭代次数， repeat∈[0,255]。注意事项：&bull;当repeat为1时，目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置加上dst_stride/src_stride。如果要让目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，需要将dst_stride/src_stride置为0。&bull;当repeat大于1时，第一次repeat中目的操作数/源操作数的有效起始位置为dst/src序列输入的起始位置，第二次会加上dst_stride/src_stride。以此类推。
 * @param [in] dst_stride 相邻迭代间，目的操作数相同DataBlock地址Stride，单位：DataBlock。相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../../general_description_and_constraints.md#repeatStride)。
 * @param [in] src_stride 相邻迭代间，源操作数相同DataBlock地址Stride，单位：DataBlock。相邻迭代间相同DataBlock的地址步长参数的详细说明请参考[repeatStride](../../general_description_and_constraints.md#repeatStride)。
 */
__aicore__ inline void asc_transto5hd_b32(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162float__PUf32_PUbf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将bfloat16_t类型数据转为float，无舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_bfloat162float(
    __ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162float__PUf32_PUbf16_u32
 * @archs 2201
 * @brief 将bfloat16_t类型数据转为float，无舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162float(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_rna__PUi32_PUbf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_bfloat162int32_rna(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_rna__PUi32_PUbf16_u32
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_rna(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_ru__PUi32_PUbf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_bfloat162int32_ru(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_ru__PUi32_PUbf16_u32
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_ru(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_rd__PUi32_PUbf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_bfloat162int32_rd(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_rd__PUi32_PUbf16_u32
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_rd(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_rn__PUi32_PUbf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_bfloat162int32_rn(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_rn__PUi32_PUbf16_u32
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_rn(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_rz__PUi32_PUbf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_bfloat162int32_rz(
    __ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_bfloat162int32_rz__PUi32_PUbf16_u32
 * @archs 2201
 * @brief 将bfloat16_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_bfloat162int32_rz(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_rn__PUbf16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2bfloat16_rn(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_rn__PUbf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_rn(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_rna__PUbf16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2bfloat16_rna(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_rna__PUbf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_rna(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_rd__PUbf16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2bfloat16_rd(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_rd__PUbf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_rd(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_ru__PUbf16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2bfloat16_ru(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_ru__PUbf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_ru(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_rz__PUbf16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2bfloat16_rz(
    __ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2bfloat16_rz__PUbf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2bfloat16_rz(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_rn__PUf32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2float_rn(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_rn__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_rn(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_rd__PUf32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2float_rd(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_rd__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_rd(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_ru__PUf32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2float_ru(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_ru__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_ru(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_rna__PUf32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2float_rna(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_rna__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_rna(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_rz__PUf32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2float_rz(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2float_rz__PUf32_PUf32_u32
 * @archs 2201
 * @brief 对float类型数据进行精度转换处理。
 * @param [out] dst 目的操作数地址（矢量）的起始地址。
 * @param [in] src 源操作数地址（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2float_rz(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half__PUf16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2half(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_rn__PUf16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2half_rn(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_rn__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_rn(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_rna__PUf16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2half_rna(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_rna__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_rna(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_rd__PUf16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2half_rd(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_rd__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_rd(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_ru__PUf16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2half_ru(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_ru__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_ru(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_rz__PUf16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2half_rz(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_rz__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_rz(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_ro__PUf16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2half_ro(
    __ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2half_ro__PUf16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2half_ro(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_rna__PUi32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int32_rna(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_rna__PUi32_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_rna(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_ru__PUi32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int32_ru(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_ru__PUi32_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_ru(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_rd__PUi32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int32_rd(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_rd__PUi32_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_rd(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_rn__PUi32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int32_rn(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_rn__PUi32_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_rn(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_rz__PUi32_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int32_rz(
    __ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int32_rz__PUi32_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int32_rz(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_rna__PUi64_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int64_rna(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_rna__PUi64_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_rna(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_ru__PUi64_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int64_ru(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_ru__PUi64_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_ru(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_rd__PUi64_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int64_rd(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_rd__PUi64_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_rd(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_rn__PUi64_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int64_rn(
    __ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_rn__PUi64_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_rn(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_rz__PUi64_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int64_rz(
    __ubuf__ int64_t* dst, __ubuf__ float* src, const uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int64_rz__PUi64_PUf32_u32
 * @archs 2201
 * @brief 将float类型转化为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_float2int64_rz(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2float__PUf32_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型转化为float类型，无舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2float(
    __ubuf__ float* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2float__PUf32_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为float类型，无舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2float(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4__PUi4x2_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int4(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4__PUi4x2_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_rna__PUi4x2_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int4_rna(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_rna__PUi4x2_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_rna(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_ru__PUi4x2_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int4_ru(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_ru__PUi4x2_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_ru(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_rd__PUi4x2_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int4_rd(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_rd__PUi4x2_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_rd(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_rn__PUi4x2_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int4_rn(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_rn__PUi4x2_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_rn(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_rz__PUi4x2_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int4_rz(
    __ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int4_rz__PUi4x2_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int4类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int4_rz(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8__PUi8_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int8(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8__PUi8_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_rna__PUi8_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int8_rna(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_rna__PUi8_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_rna(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_ru__PUi8_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int8_ru(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_ru__PUi8_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_ru(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_rd__PUi8_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int8_rd(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_rd__PUi8_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_rd(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_rn__PUi8_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int8_rn(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_rn__PUi8_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_rn(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_rz__PUi8_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int8_rz(
    __ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int8_rz__PUi8_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int8_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int8_rz(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_rna__PUi16_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int16_rna(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_rna__PUi16_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_rna(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_ru__PUi16_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int16_ru(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_ru__PUi16_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_ru(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_rd__PUi16_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int16_rd(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_rd__PUi16_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_rd(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_rn__PUi16_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int16_rn(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_rn__PUi16_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_rn(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_rz__PUi16_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int16_rz(
    __ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int16_rz__PUi16_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为int16_t，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2int16_rz(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_rna__PUi32_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int32_rna(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_rna__PUi32_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_rna(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_ru__PUi32_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int32_ru(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_ru__PUi32_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_ru(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_rd__PUi32_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int32_rd(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_rd__PUi32_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_rd(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_rn__PUi32_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int32_rn(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_rn__PUi32_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_rn(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_rz__PUi32_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2int32_rz(
    __ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2int32_rz__PUi32_PUf16_u32
 * @archs 2201
 * @brief 将half类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_half2int32_rz(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int42half__PUf16_PUi4x2_u8_u16_u16_u8_u8
 * @archs 2201
 * @brief 将int4_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int42half(
    __ubuf__ half* dst, __ubuf__ int4b_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int42half__PUf16_PUi4x2_u32
 * @archs 2201
 * @brief 将int4_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int42half(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int82half__PUf16_PUi8_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int8_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int82half(
    __ubuf__ half* dst, __ubuf__ int8_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int82half__PUf16_PUi8_u32
 * @archs 2201
 * @brief 将int8_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int82half(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_uint82half__PUf16_PUu8_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将uint8_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_uint82half(
    __ubuf__ half* dst, __ubuf__ uint8_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_uint82half__PUf16_PUu8_u32
 * @archs 2201
 * @brief 将uint8_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_uint82half(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162float__PUf32_PUi16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int16_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int162float(
    __ubuf__ float* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162float__PUf32_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162float(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float__PUf32_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float__PUf32_PUi32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int322float(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_rna__PUf32_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_rna(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_rna__PUf32_PUi32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int322float_rna(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_ru__PUf32_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_ru(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_ru__PUf32_PUi32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int322float_ru(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_rd__PUf32_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_rd(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_rd__PUf32_PUi32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int322float_rd(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_rn__PUf32_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_rn(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_rn__PUf32_PUi32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int322float_rn(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_rz__PUf32_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322float_rz(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322float_rz__PUf32_PUi32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int32_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int322float_rz(
    __ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322int16__PUi16_PUi32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int32_t类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int322int16(
    __ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322int16__PUi16_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322int16(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322int64__PUi64_PUi32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int32_t类型数据转换为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int322int64(
    __ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int322int64__PUi64_PUi32_u32
 * @archs 2201
 * @brief 将int32_t类型数据转换为int64_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int322int64(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642int32__PUi32_PUi64_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int64_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int642int32(
    __ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642int32__PUi32_PUi64_u32
 * @archs 2201
 * @brief 将int64_t类型数据转换为int32_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int642int32(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_rna__PUi16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int16_rna(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_rna__PUi16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_rna(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_ru__PUi16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int16_ru(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_ru__PUi16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_ru(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_rd__PUi16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int16_rd(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_rd__PUi16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_rd(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_rn__PUi16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int16_rn(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_rn__PUi16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_rn(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_rz__PUi16_PUf32_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_float2int16_rz(
    __ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_float2int16_rz__PUi16_PUf32_u32
 * @archs 2201
 * @brief 将float类型数据转换为int16_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_float2int16_rz(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_rna__PUf32_PUi64_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int642float_rna(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_rna__PUf32_PUi64_u32
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_rna(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_ru__PUf32_PUi64_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int642float_ru(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_ru__PUf32_PUi64_u32
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_ru(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_rd__PUf32_PUi64_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int642float_rd(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_rd__PUf32_PUi64_u32
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_rd(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_rn__PUf32_PUi64_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int642float_rn(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_rn__PUf32_PUi64_u32
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_rn(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_rz__PUf32_PUi64_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int642float_rz(
    __ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int642float_rz__PUf32_PUi64_u32
 * @archs 2201
 * @brief 将int64_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与连续计算的元素个数。
 */
__aicore__ inline void asc_int642float_rz(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half__PUf16_PUi16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int162half(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half__PUf16_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_rna__PUf16_PUi16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int162half_rna(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_rna__PUf16_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_rna(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_ru__PUf16_PUi16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int162half_ru(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_ru__PUf16_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_ru(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_rd__PUf16_PUi16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int162half_rd(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_rd__PUf16_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_rd(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_rn__PUf16_PUi16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int162half_rn(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_rn__PUf16_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_rn(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_rz__PUf16_PUi16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_int162half_rz(
    __ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_int162half_rz__PUf16_PUi16_u32
 * @archs 2201
 * @brief 将int16_t类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_int162half_rz(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8__PUu8_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2uint8(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8__PUu8_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_rna__PUu8_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2uint8_rna(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_rna__PUu8_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_rna(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_ru__PUu8_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2uint8_ru(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_ru__PUu8_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_ru(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_rd__PUu8_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2uint8_rd(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_rd__PUu8_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_rd(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_rn__PUu8_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2uint8_rn(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_rn__PUu8_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_rn(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_rz__PUu8_PUf16_u8_u16_u16_u16_u16
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] repeat 迭代次数。
 * @param [in] dst_block_stride 目的操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] src_block_stride 源操作数单次迭代内不同DataBlock间地址步长。
 * @param [in] dst_repeat_stride 目的操作数相邻迭代间相同DataBlock的地址步长。
 * @param [in] src_repeat_stride 源操作数相邻迭代间相同DataBlock的地址步长。
 */
__aicore__ inline void asc_half2uint8_rz(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride,
    uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_convert
 * @override_id asc_half2uint8_rz__PUu8_PUf16_u32
 * @archs 2201
 * @brief 将half类型数据转换为uint8_t类型。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src 源操作数（矢量）的起始地址。
 * @param [in] count 参与计算的元素个数。
 */
__aicore__ inline void asc_half2uint8_rz(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_CONVERT_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_CONVERT_H
#endif
