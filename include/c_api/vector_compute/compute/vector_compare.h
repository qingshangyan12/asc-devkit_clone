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
#error "vector_compute/compute/vector_compare.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_COMPARE_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_COMPARE_H
#define INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_COMPARE_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"
#endif
/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_get_cmp_mask__PUv
 * @archs 2201
 * @brief 此接口用于获取Compare操作的比较结果。
 * @param [out] dst 存放比较操作结果的地址。
 */
__aicore__ inline void asc_get_cmp_mask(__ubuf__ void* dst);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_set_cmp_mask__PUv
 * @archs 2201
 * @brief 为[asc_select](../vector_permute_sel/asc_select.md)操作设置作用于选择的Mask掩码。
 * @param [in] sel_mask 用于选择的Mask掩码的起始地址。
 */
__aicore__ inline void asc_set_cmp_mask(__ubuf__ void* sel_mask);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_eq(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_eq(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_eq(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_eq(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq__PUu8_PUi32_PUi32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_eq(
    __ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_gt_scalar__PUu8_PUf16_f16_u8_u16_u16_u16_u16
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
__aicore__ inline void asc_gt_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_gt_scalar__PUu8_PUf32_f32_u8_u16_u16_u16_u16
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
__aicore__ inline void asc_gt_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ne__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_ne(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ne__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_ne(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ne__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_ne(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ne__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_ne(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ge_scalar__PUu8_PUf16_f16_u8_u16_u16_u16_u16
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
__aicore__ inline void asc_ge_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ge_scalar__PUu8_PUf32_f32_u8_u16_u16_u16_u16
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
__aicore__ inline void asc_ge_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq_scalar__PUu8_PUf16_f16_u8_u16_u16_u16_u16
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
__aicore__ inline void asc_eq_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq_scalar__PUu8_PUf32_f32_u8_u16_u16_u16_u16
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
__aicore__ inline void asc_eq_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_eq_scalar__PUu8_PUi32_i32_u8_u16_u16_u16_u16
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
__aicore__ inline void asc_eq_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_lt_scalar__PUu8_PUf16_f16_u8_u16_u16_u16_u16
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
__aicore__ inline void asc_lt_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_lt_scalar__PUu8_PUf32_f32_u8_u16_u16_u16_u16
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
__aicore__ inline void asc_lt_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ge__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_ge(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ge__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_ge(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ge__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_ge(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_ge__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_ge(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_lt__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_lt(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_lt__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_lt(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_lt__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_lt(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_lt__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_lt(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

__aicore__ inline void asc_ne_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

__aicore__ inline void asc_ne_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_gt__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_gt(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_gt__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_gt(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_gt__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_gt(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_gt__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_gt(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_le__PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_le(
    __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_le__PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_le(
    __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
    uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_le__PUu8_PUf16_PUf16_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_le(
    __ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_le__PUu8_PUf32_PUf32_u8_u8_u8_u8_u8_u8_u8
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
__aicore__ inline void asc_le(
    __ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride,
    uint8_t src1_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_le_scalar__PUu8_PUf16_f16_u8_u16_u16_u16_u16
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
__aicore__ inline void asc_le_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/**
 * @ingroup simd_c_api.vector_compute.vector_compare
 * @override_id asc_le_scalar__PUu8_PUf32_f32_u8_u16_u16_u16_u16
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
__aicore__ inline void asc_le_scalar(
    __ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_COMPARE_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_COMPARE_H
#endif
