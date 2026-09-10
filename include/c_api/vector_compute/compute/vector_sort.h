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
#error "vector_compute/compute/vector_sort.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_SORT_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_SORT_H
#define INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_SORT_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"
#endif
__aicore__ inline void asc_get_vms4_sr(uint16_t sorted_num[4]);

/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @override_id asc_bitsort__PUf16_PUf16_PUu32_i32
 * @archs 3510 2201
 * @brief 位图排序函数，一次迭代可以完成32个元素的排序。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 重复迭代次数，每次迭代完成32个元素的排序，下次迭代src0和src1各跳过32个元素，dst跳过256 Byte空间。取值范围：[0, 255]。
 */
__aicore__ inline void asc_bitsort(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @override_id asc_bitsort__PUf32_PUf32_PUu32_i32
 * @archs 3510 2201
 * @brief 位图排序函数，一次迭代可以完成32个元素的排序。
 * @param [out] dst 目的操作数（矢量）的起始地址。
 * @param [in] src0 源操作数（矢量）的起始地址。
 * @param [in] src1 源操作数（矢量）的起始地址。
 * @param [in] repeat 重复迭代次数，每次迭代完成32个元素的排序，下次迭代src0和src1各跳过32个元素，dst跳过256 Byte空间。取值范围：[0, 255]。
 */
__aicore__ inline void asc_bitsort(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @override_id asc_mrgsort4__PUf16_AmrgPUf16_u8_u16_u16_u16_u16_b_u8
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
__aicore__ inline void asc_mrgsort4(
    __ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit);

/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @override_id asc_mrgsort4__PUf32_AmrgPUf32_u8_u16_u16_u16_u16_b_u8
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
__aicore__ inline void asc_mrgsort4(
    __ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_SORT_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_SORT_H
#endif
