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
#include "impl/c_api/memory_base_impl/vector_sort_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/vector_sort_intf_impl.h"
#endif
/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @brief Output-parameter interface: writes into sorted_num how many Region Proposals each of the 4 queues already processed after asc_mrgsort4; returns nothing.
 *
 * @override_id asc_get_vms4_sr__Amrgu16
 * @archs 2201
 * @param [out] sorted_num 每个队列已经理过的Region Proposal个数。队列数量ASC_C_API_MRGSORT_ELEMENT_LEN值为4。
 */
__aicore__ inline void asc_get_vms4_sr(uint16_t sorted_num[4]);

/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @brief Sorts 32 half score elements in descending order per iteration and writes (score, index) 8-byte structures into dst; iterations are sorted independently; src1 is the uint32 index.
 *
 * @override_id asc_bitsort__PUf16_PUf16_PUu32_i32
 * @archs 2201 3510
 * @param [out] dst 目的操作数在UB中的起始地址。
 * @param [in] src0 score在UB中的起始地址。
 * @param [in] src1 index在UB中的起始地址。
 * @param [in] repeat 迭代次数，取值范围为[0, 255]。每次迭代处理32个元素；下次迭代时，`src0`和`src1`各跳过32个元素，`dst`跳过256字节。不同迭代之间不进行排序。
 */
__aicore__ inline void asc_bitsort(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @brief Sorts 32 float score elements in descending order per iteration and writes (score, index) 8-byte structures into dst; iterations are sorted independently; src1 is the uint32 index.
 *
 * @override_id asc_bitsort__PUf32_PUf32_PUu32_i32
 * @archs 2201 3510
 * @param [out] dst 目的操作数在UB中的起始地址。
 * @param [in] src0 score在UB中的起始地址。
 * @param [in] src1 index在UB中的起始地址。
 * @param [in] repeat 迭代次数，取值范围为[0, 255]。每次迭代处理32个元素；下次迭代时，`src0`和`src1`各跳过32个元素，`dst`跳过256字节。不同迭代之间不进行排序。
 */
__aicore__ inline void asc_bitsort(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat);

/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @brief Merges 2, 3 or 4 descending score-sorted half queues (typically asc_bitsort output) into one ordered queue in dst; valid_bit selects active queues, iterations are not merged.
 *
 * @override_id asc_mrgsort4__PUf16_AmrgPUf16_u8_u16_u16_u16_u16_b_u8
 * @archs 2201 3510
 * @param [out] dst UB中的目的队列起始地址，要求32字节对齐。
 * @param [in] src 包含4条源队列起始地址的指针数组，`ASC_C_API_MRGSORT_ELEMENT_LEN`为`4`。每个队列起始地址要求8字节对齐，数组下标0～3分别对应4条队列。4个数组项均须初始化，无效队列可复用合法UB地址。
 * @param [in] repeat 迭代次数，执行归并时取值为[1, 255]。取值大于1时须满足[约束说明](#约束说明)中的多次迭代约束。多次迭代产生多条分别有序的结果队列，不会自动将它们合为一条。
 * @param [in] element_length_0 起始地址为`src[0]`的队列的长度，即队列中数据单元的个数，每个数据单元占8字节，因此单位为8字节。 对于有效队列，取值范围为[1, 4095]；对于无效队列，取值为0。
 * @param [in] element_length_1 起始地址为`src[1]`的队列的长度，单位及范围同`element_length_0`。
 * @param [in] element_length_2 起始地址为`src[2]`的队列的长度，单位及范围同`element_length_0`。
 * @param [in] element_length_3 起始地址为`src[3]`的队列的长度，单位及范围同`element_length_0`。
 * @param [in] if_exhausted_suspension 某个队列耗尽时，控制指令是否停止执行，类型为bool，取值说明如下： &nbsp;&nbsp;&bull; `false`：归并排序过程中某条有效队列耗尽后继续处理其余队列，输出所有有效队列的值，单次迭代的输出长度为有效队列长度之和。 &nbsp;&nbsp;&bull; `true`：归并排序过程中任意一有效队列耗尽时停止排序，只输出已经完成排序的有序前缀，不能将有效队列长度之和作为单次迭代的实际输出长度。
 * @param [in] valid_bit 有效队列位掩码，有效取值为`3`、`7`、`15`，含义如下： &nbsp;&nbsp;&bull; `3`（`0b0011`）：有效队列为起始地址为`src[0]`和`src[1]`的队列。 &nbsp;&nbsp;&bull; `7`（`0b0111`）：有效队列为起始地址为`src[0]`、`src[1]`和`src[2]`的队列。 &nbsp;&nbsp;&bull; `15`（`0b1111`）：全部4条队列均有效。
 */
__aicore__ inline void asc_mrgsort4(
    __ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit);

/**
 * @ingroup simd_c_api.vector_compute.vector_sort
 * @brief Merges 2, 3 or 4 descending score-sorted float queues (typically asc_bitsort output) into one ordered queue in dst; valid_bit selects active queues, iterations are not merged.
 *
 * @override_id asc_mrgsort4__PUf32_AmrgPUf32_u8_u16_u16_u16_u16_b_u8
 * @archs 2201 3510
 * @param [out] dst UB中的目的队列起始地址，要求32字节对齐。
 * @param [in] src 包含4条源队列起始地址的指针数组，`ASC_C_API_MRGSORT_ELEMENT_LEN`为`4`。每个队列起始地址要求8字节对齐，数组下标0～3分别对应4条队列。4个数组项均须初始化，无效队列可复用合法UB地址。
 * @param [in] repeat 迭代次数，执行归并时取值为[1, 255]。取值大于1时须满足[约束说明](#约束说明)中的多次迭代约束。多次迭代产生多条分别有序的结果队列，不会自动将它们合为一条。
 * @param [in] element_length_0 起始地址为`src[0]`的队列的长度，即队列中数据单元的个数，每个数据单元占8字节，因此单位为8字节。 对于有效队列，取值范围为[1, 4095]；对于无效队列，取值为0。
 * @param [in] element_length_1 起始地址为`src[1]`的队列的长度，单位及范围同`element_length_0`。
 * @param [in] element_length_2 起始地址为`src[2]`的队列的长度，单位及范围同`element_length_0`。
 * @param [in] element_length_3 起始地址为`src[3]`的队列的长度，单位及范围同`element_length_0`。
 * @param [in] if_exhausted_suspension 某个队列耗尽时，控制指令是否停止执行，类型为bool，取值说明如下： &nbsp;&nbsp;&bull; `false`：归并排序过程中某条有效队列耗尽后继续处理其余队列，输出所有有效队列的值，单次迭代的输出长度为有效队列长度之和。 &nbsp;&nbsp;&bull; `true`：归并排序过程中任意一有效队列耗尽时停止排序，只输出已经完成排序的有序前缀，不能将有效队列长度之和作为单次迭代的实际输出长度。
 * @param [in] valid_bit 有效队列位掩码，有效取值为`3`、`7`、`15`，含义如下： &nbsp;&nbsp;&bull; `3`（`0b0011`）：有效队列为起始地址为`src[0]`和`src[1]`的队列。 &nbsp;&nbsp;&bull; `7`（`0b0111`）：有效队列为起始地址为`src[0]`、`src[1]`和`src[2]`的队列。 &nbsp;&nbsp;&bull; `15`（`0b1111`）：全部4条队列均有效。
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
