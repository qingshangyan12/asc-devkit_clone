/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/**
 * @file vector_compute.h
 * @brief This file provides vector compute interfaces for various mathematical operations.
 * @details These interfaces support vector operations including arithmetic operations (add, mul, div),
 *          type conversions (float2half, half2int, etc.), comparison operations (eq, ne, gt, etc.),
 *          reduction operations (reduce_sum, reduce_max, reduce_min, etc.), sorting operations,
 *          and mathematical functions (exp, log, abs, relu, etc.). Operations support various
 *          data types including half, float, int8_t, int16_t, int32_t, bfloat16_t, etc.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_VECTOR_COMPUTE_H
#define INCLUDE_C_API_VECTOR_COMPUTE_VECTOR_COMPUTE_H

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "instr_impl/npu_arch_2201/vector_compute_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "instr_impl/npu_arch_3510/vector_compute_impl.h"
#endif

/*
 * @ingroup VectorCompute
 * @brief Set vector address register for various data types
 * @param [in] addr Address offset for the register
 * @param [in] src_array Pointer to array in Unified Buffer
 * @details Configures vector address register to point to data in Unified Buffer for vector operations.
 *          This is a hardware-level configuration for vector instruction execution.
 */
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int8_t** src_array);
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint8_t** src_array);
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int16_t** src_array);
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint16_t** src_array);
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ half** src_array);
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ int32_t** src_array);
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ uint32_t** src_array);
__aicore__ inline void asc_set_va_reg(ub_addr8_t addr, __ubuf__ float** src_array);

/*
 * @ingroup VectorCompute
 * @brief Set comparison mask from buffer
 * @param [in] sel_mask Buffer containing selection mask data
 * @details Sets the comparison mask register using data from Unified Buffer.
 *          This mask will be used for subsequent vector operations.
 */
__aicore__ inline void asc_set_cmp_mask(__ubuf__ void* sel_mask);

/*
 * @ingroup VectorCompute
 * @brief Get comparison mask result
 * @param [out] dst Destination buffer to store mask result
 * @details Retrieves the result mask from the last comparison operation.
 *          The mask contains 1 for elements that matched the comparison condition.
 */
__aicore__ inline void asc_get_cmp_mask(__ubuf__ void* dst);

/*
 * @ingroup VectorCompute
 * @brief Get reduce max count result for half/float precision
 * @param [out] val Maximum value found
 * @param [out] index Index of the maximum value
 * @details Retrieves the maximum value and its index from the last reduce max operation.
 */
__aicore__ inline void asc_get_reduce_max_cnt(half& val, uint32_t& index);
__aicore__ inline void asc_get_reduce_max_cnt(float& val, uint32_t& index);

/*
 * @ingroup VectorCompute
 * @brief Get reduce min count result for half/float precision
 * @param [out] val Minimum value found
 * @param [out] index Index of the minimum value
 * @details Retrieves the minimum value and its index from the last reduce min operation.
 */
__aicore__ inline void asc_get_reduce_min_cnt(half& val, uint32_t& index);
__aicore__ inline void asc_get_reduce_min_cnt(float& val, uint32_t& index);

/*
 * @ingroup VectorCompute
 * @brief Get reserved count value
 * @return Reserved count value from hardware
 * @details Retrieves the hardware reserved count value used for vector instruction control.
 */
__aicore__ inline int64_t asc_get_rsvd_count();

/*
 * @ingroup VectorCompute
 * @brief Get VMS4 sorted result
 * @param [out] sorted_num Array of 4 sorted numbers
 * @details Retrieves the sorted result from the last VMS4 (Vector Merge Sort 4) operation.
 */
__aicore__ inline void asc_get_vms4_sr(uint16_t sorted_num[4]);

/*
 * @ingroup VectorCompute
 * @brief Set mask count mode
 * @details Enables mask count mode for vector operations.
 *          In this mode, mask values are counted rather than applied.
 */
__aicore__ inline void asc_set_mask_count();

/*
 * @ingroup VectorCompute
 * @brief Set mask normal mode
 * @details Enables normal mask mode for vector operations.
 *          In this mode, masks are applied directly to vector operations.
 */
__aicore__ inline void asc_set_mask_norm();

/*
 * @ingroup VectorCompute
 * @brief Set vector mask registers
 * @param [in] mask1 High 64-bit mask value
 * @param [in] mask0 Low 64-bit mask value
 * @details Sets both vector mask registers (mask0 and mask1) for controlling vector operations.
 *          Masks determine which elements participate in vector instructions.
 */
__aicore__ inline void asc_set_vector_mask(uint64_t mask1, uint64_t mask0);

/*
 * @ingroup VectorCompute
 * @brief Set dequantization scale with offset
 * @param [in] scale Scale value for dequantization
 * @param [in] offset Offset value for dequantization
 * @param [in] enable_signed Whether to use signed mode
 * @details Configures dequantization parameters for converting quantized data to floating point.
 */
__aicore__ inline void asc_set_deq_scale(float scale, int16_t offset, bool enable_signed);
__aicore__ inline void asc_set_deq_scale(__ubuf__ uint64_t* tmp, float scale_arr[ASC_VDEQ_SIZE],
                int16_t offset_arr[ASC_VDEQ_SIZE], bool enable_signed_arr[ASC_VDEQ_SIZE]);

/*
 * @ingroup VectorCompute
 * @brief Set dequantization scale for half precision
 * @param [in] scale Half precision scale value
 * @details Configures dequantization scale for half precision data conversion.
 */
__aicore__ inline void asc_set_deq_scale(half scale);

/*
 * @ingroup VectorCompute
 * @brief Get accumulator value
 * @return Current value in the accumulator register
 * @details Retrieves the value stored in the vector accumulator register.
 */
__aicore__ inline int64_t asc_get_acc_val();

// ============================================================================
// Arithmetic Operations - Basic arithmetic computations
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Vector addition for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer  
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs element-wise addition: dst[i] = src0[i] + src1[i].
 *          Supports half, float, int16_t, int32_t data types.
 *          This is a hardware-level vector instruction with block and repeat stride controls.
 */
__aicore__ inline void asc_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_add(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_add(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);


// ==========asc_add_scalar(half/float/int16_t/int32_t)==========
/*
 * @ingroup VectorCompute
 * @brief Vector addition with scalar for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Scalar value to add
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Performs element-wise addition with a scalar: dst[i] = src[i] + value.
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_add_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_add_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_add_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_add_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

// ============================================================================
// Broadcast Operations - Data broadcasting across blocks
// ============================================================================

// ============================================================================
// Broadcast Operations - Data broadcasting across blocks
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Broadcast block operation for uint16/uint32 data types
 * @param [out] dst Destination buffer
 * @param [in] src Source buffer to broadcast
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] repeat Number of repeat operations
 * @details Broadcasts a block of data to multiple locations in the destination buffer.
 *          Supports uint16_t and uint32_t data types.
 */
__aicore__ inline void asc_brcb(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat);
__aicore__ inline void asc_brcb(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat);

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

// ============================================================================
// Sorting and Selection Operations - Sorting and selection algorithms
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Bitonic sort for half/float data types
 * @param [out] dst Destination buffer for sorted result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer (sort keys)
 * @param [in] repeat Number of repeat operations
 * @details Performs bitonic sort on half or float data.
 *          This is a hardware-level sorting instruction.
 */
__aicore__ inline void asc_bitsort(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat);
__aicore__ inline void asc_bitsort(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat);

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

/*
 * @ingroup VectorCompute
 * @brief Bitonic sort for half/float data types
 * @param [out] dst Destination buffer for sorted result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer (sort keys)
 * @param [in] repeat Number of repeat operations
 * @details Performs bitonic sort on half or float data.
 *          This is a hardware-level sorting instruction.
 */
__aicore__ inline void asc_bitsort(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat);
__aicore__ inline void asc_bitsort(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat);

/*
 * @ingroup VectorCompute
 * @brief Merge sort 4 for half/float data types
 * @param [out] dst Destination buffer for sorted result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] element_length_0 Length of first element group
 * @param [in] element_length_1 Length of second element group
 * @param [in] element_length_2 Length of third element group
 * @param [in] element_length_3 Length of fourth element group
 * @param [in] enable_exhausted_suspension Whether to suspend when exhausted
 * @param [in] valid_bit Valid bit mask
 * @details Performs merge sort on 4 element groups for half or float data.
 */
__aicore__ inline void asc_mrgsort4(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t element_length_0,
                                    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3,
                                    bool enable_exhausted_suspension, uint8_t valid_bit);
__aicore__ inline void asc_mrgsort4(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t element_length_0,
                                    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3,
                                    bool enable_exhausted_suspension, uint8_t valid_bit);

// ============================================================================
// Gather and enable_transpose Operations - Data gathering and transposing
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief enable_transpose operation for int16/uint16 data types
 * @param [out] dst Destination buffer for transposed result
 * @param [in] src Source buffer
 * @details Performs matrix enable_transpose on int16_t or uint16_t data.
 */
__aicore__ inline void asc_transpose(__ubuf__ int16_t* dst, __ubuf__ int16_t* src);
__aicore__ inline void asc_transpose(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src);

#endif

// ============================================================================
// Reduction Operations - Aggregate values across vector elements
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Data block reduce sum operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs sum reduction across data blocks. Supports half and float data types.
 */
__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Data block reduce max operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs max reduction across data blocks. Supports half and float data types.
 */
__aicore__ inline void asc_datablock_reduce_max(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_datablock_reduce_max(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Data block reduce min operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs min reduction across data blocks. Supports half and float data types.
 */
__aicore__ inline void asc_datablock_reduce_min(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_datablock_reduce_min(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce sum operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs sum reduction with repeat pattern. Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce max operations returning index and value for half/float data types
 * @param [out] dst Destination buffer for result (index and value pair)
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs max reduction with repeat pattern, returns both index and value.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce max operations returning value and index for half/float data types
 * @param [out] dst Destination buffer for result (value and index pair)
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs max reduction with repeat pattern, returns both value and index.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce max operations returning only value for half/float data types
 * @param [out] dst Destination buffer for result (value only)
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs max reduction with repeat pattern, returns only value.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce max operations returning only index for half/float data types
 * @param [out] dst Destination buffer for result (index only)
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs max reduction with repeat pattern, returns only index.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce min operations returning index and value for half/float data types
 * @param [out] dst Destination buffer for result (index and value pair)
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs min reduction with repeat pattern, returns both index and value.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce min operations returning value and index for half/float data types
 * @param [out] dst Destination buffer for result (value and index pair)
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs min reduction with repeat pattern, returns both value and index.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce min operations returning only value for half/float data types
 * @param [out] dst Destination buffer for result (value only)
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs min reduction with repeat pattern, returns only value.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce min operations returning only index for half/float data types
 * @param [out] dst Destination buffer for result (index only)
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs min reduction with repeat pattern, returns only index.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_repeat_stride, uint16_t src_block_stride, uint16_t src_repeat_stride);


// ============================================================================
// Comparison Operations - Element-wise comparison between vectors
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Vector equal comparison for half/float/int32 data types
 * @param [out] dst Destination buffer for result (optional, uint8 mask)
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src0_block_stride First source block stride for data layout
 * @param [in] src1_block_stride Second source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src0_repeat_stride First source repeat stride for iteration control
 * @param [in] src1_repeat_stride Second source repeat stride for iteration control
 * @details Performs element-wise equal comparison: result = (src0[i] == src1[i]).
 *          Supports half, float, and int32_t data types. Result is stored as boolean mask.
 */
__aicore__ inline void asc_eq(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_eq(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_eq(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

// ============================================================================
// Type Conversion Operations - Convert between different data types
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Convert bfloat16 to float
 * @param [out] dst Destination buffer for float result
 * @param [in] src Source buffer with bfloat16 data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts bfloat16_t elements to single-precision float.
 */
__aicore__ inline void asc_bfloat162float(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Convert bfloat16 to int32 with various rounding modes
 * @param [out] dst Destination buffer for int32 result
 * @param [in] src Source buffer with bfloat16 data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts bfloat16_t elements to int32_t with different rounding modes.
 *          Rounding modes: rna (round to nearest away), ru (round up), rd (round down),
 *          rn (round to nearest), rz (round toward zero).
 */
__aicore__ inline void asc_bfloat162int32_rna(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_bfloat162int32_ru(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_bfloat162int32_rd(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_bfloat162int32_rn(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_bfloat162int32_rz(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Convert float to bfloat16 with various rounding modes
 * @param [out] dst Destination buffer for bfloat16 result
 * @param [in] src Source buffer with float data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts single-precision float elements to bfloat16_t with different rounding modes.
 *          Rounding modes: rn, rna, rd, ru, rz.
 */
__aicore__ inline void asc_float2bfloat16_rn(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2bfloat16_rna(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2bfloat16_rd(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2bfloat16_ru(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2bfloat16_rz(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Convert float to float with various rounding modes
 * @param [out] dst Destination buffer for float result
 * @param [in] src Source buffer with float data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs float to float conversion with specified rounding modes.
 *          Rounding modes: rn, rd, ru, rna, rz.
 */
__aicore__ inline void asc_float2float_rn(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2float_rd(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2float_ru(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2float_rna(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2float_rz(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Convert float to half precision with various rounding modes
 * @param [out] dst Destination buffer for half result
 * @param [in] src Source buffer with float data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts single-precision float elements to half precision with different rounding modes.
 *          Rounding modes: default, rn, rna, rd, ru, rz, ro (round to odd).
 */
__aicore__ inline void asc_float2half(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2half_rn(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2half_rna(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2half_rd(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2half_ru(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2half_rz(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2half_ro(__ubuf__ half* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Convert float to int32 with various rounding modes
 * @param [out] dst Destination buffer for int32 result
 * @param [in] src Source buffer with float data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts float elements to int32_t with different rounding modes.
 *          Rounding modes: rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_float2int32_rna(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2int32_ru(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2int32_rd(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2int32_rn(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2int32_rz(__ubuf__ int32_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Convert float to int64 with various rounding modes
 * @param [out] dst Destination buffer for int64 result
 * @param [in] src Source buffer with float data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts float elements to int64_t with different rounding modes.
 *          Rounding modes: rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_float2int64_rna(__ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2int64_ru(__ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2int64_rd(__ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2int64_rn(__ubuf__ int64_t* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2int64_rz(__ubuf__ int64_t* dst, __ubuf__ float* src, const uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Convert half to float precision
 * @param [out] dst Destination buffer for float result
 * @param [in] src Source buffer with half data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts half precision elements to single-precision float.
 */
__aicore__ inline void asc_half2float(__ubuf__ float* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Convert half to int4 with various rounding modes
 * @param [out] dst Destination buffer for int4 result
 * @param [in] src Source buffer with half data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts half precision elements to int4b_t with different rounding modes.
 *          Rounding modes: default, rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_half2int4(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int4_rna(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int4_ru(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int4_rd(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int4_rn(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int4_rz(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Convert half to int8 with various rounding modes
 * @param [out] dst Destination buffer for int8 result
 * @param [in] src Source buffer with half data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts half precision elements to int8_t with different rounding modes.
 *          Rounding modes: default, rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int8_rna(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int8_ru(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int8_rd(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int8_rn(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int8_rz(__ubuf__ int8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Convert half to int16 with various rounding modes
 * @param [out] dst Destination buffer for int16 result
 * @param [in] src Source buffer with half data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts half precision elements to int16_t with different rounding modes.
 *          Rounding modes: rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_half2int16_rna(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int16_ru(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int16_rd(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int16_rn(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int16_rz(__ubuf__ int16_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Convert half to int32 with various rounding modes
 * @param [out] dst Destination buffer for int32 result
 * @param [in] src Source buffer with half data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts half precision elements to int32_t with different rounding modes.
 *          Rounding modes: rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_half2int32_rna(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int32_ru(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int32_rd(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int32_rn(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2int32_rz(__ubuf__ int32_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


// ============================================================================
// Mathematical Functions - Mathematical and activation functions
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Vector division for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer (dividend)
 * @param [in] src1 Second source buffer (divisor)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src0_block_stride First source block stride for data layout
 * @param [in] src1_block_stride Second source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src0_repeat_stride First source repeat stride for iteration control
 * @param [in] src1_repeat_stride Second source repeat stride for iteration control
 * @details Performs element-wise division: dst[i] = src0[i] / src1[i].
 *          Supports half and float data types.
 */
__aicore__ inline void asc_div(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_div(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Duplicate scalar value to buffer for various data types
 * @param [out] dst Destination buffer
 * @param [in] src Scalar value to duplicate
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @details Duplicates a scalar value across the destination buffer.
 *          Supports half, int16_t, uint16_t, bfloat16_t, float, int32_t, uint32_t data types.
 */
__aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride);
__aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride);
__aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride);
__aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride);
__aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride);
__aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride);
__aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t dst_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Exponential function for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Computes exponential function: dst[i] = exp(src[i]).
 *          Supports half and float data types.
 */
__aicore__ inline void asc_exp(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_exp(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Natural logarithm for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Computes natural logarithm: dst[i] = log(src[i]).
 *          Supports half and float data types.
 */
__aicore__ inline void asc_log(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);
__aicore__ inline void asc_log(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Greater than comparison with scalar for half/float data types
 * @param [out] dst Destination buffer for result (uint8 mask)
 * @param [in] src Source buffer
 * @param [in] value Scalar value to compare
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs element-wise greater-than comparison with scalar: result = (src[i] > value).
 *          Supports half and float data types. Result is stored as boolean mask.
 */
__aicore__ inline void asc_gt_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);
__aicore__ inline void asc_gt_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Absolute value for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Computes absolute value: dst[i] = abs(src[i]).
 *          Supports half and float data types.
 */
__aicore__ inline void asc_abs(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_abs(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


// ============================================================================
// Bit Operations - Bitwise operations and shifts
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Left shift operation for int16/uint16/int32/uint32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] distance Number of bits to shift
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs left shift: dst[i] = src[i] << distance.
 *          Supports int16_t, uint16_t, int32_t, uint32_t data types.
 */
__aicore__ inline void asc_shiftleft(__ubuf__ int16_t* dst, __ubuf__ int16_t* src,  uint32_t distance, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_shiftleft(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src,  uint32_t distance, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_shiftleft(__ubuf__ int32_t* dst, __ubuf__ int32_t* src,  uint32_t distance, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_shiftleft(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src,  uint32_t distance, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Multiply with cast to int8 for half data type
 * @param [out] dst Destination buffer for result (int8)
 * @param [in] src0 First source buffer (half)
 * @param [in] src1 Second source buffer (half)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src0_block_stride First source block stride for data layout
 * @param [in] src1_block_stride Second source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src0_repeat_stride First source repeat stride for iteration control
 * @param [in] src1_repeat_stride Second source repeat stride for iteration control
 * @details Performs multiplication of half values and casts result to int8: dst[i] = (int8_t)(src0[i] * src1[i]).
 */
__aicore__ inline void asc_mul_cast_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Multiply with cast to uint8 for half data type
 * @param [out] dst Destination buffer for result (uint8)
 * @param [in] src0 First source buffer (half)
 * @param [in] src1 Second source buffer (half)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src0_block_stride First source block stride for data layout
 * @param [in] src1_block_stride Second source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src0_repeat_stride First source repeat stride for iteration control
 * @param [in] src1_repeat_stride Second source repeat stride for iteration control
 * @details Performs multiplication of half values and casts result to uint8: dst[i] = (uint8_t)(src0[i] * src1[i]).
 */
__aicore__ inline void asc_mul_cast_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Vector not-equal comparison for half/float data types
 * @param [out] dst Destination buffer for result (optional, uint8 mask)
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src0_block_stride First source block stride for data layout
 * @param [in] src1_block_stride Second source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src0_repeat_stride First source repeat stride for iteration control
 * @param [in] src1_repeat_stride Second source repeat stride for iteration control
 * @details Performs element-wise not-equal comparison: result = (src0[i] != src1[i]).
 *          Supports half and float data types. Result is stored as boolean mask.
 */
__aicore__ inline void asc_ne(__ubuf__ half* src0, __ubuf__ half* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_ne(__ubuf__ float* src0, __ubuf__ float* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_ne(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, 
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_ne(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, 
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Greater-or-equal comparison with scalar for half/float data types
 * @param [out] dst Destination buffer for result (uint8 mask)
 * @param [in] src Source buffer
 * @param [in] value Scalar value to compare
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs element-wise greater-or-equal comparison with scalar: result = (src[i] >= value).
 *          Supports half and float data types. Result is stored as boolean mask.
 */
__aicore__ inline void asc_ge_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_ge_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Dequantize int32 to half precision
 * @param [out] dst Destination buffer for half result
 * @param [in] src Source buffer with int32 data
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Dequantizes int32_t elements to half precision.
 */
__aicore__ inline void asc_deq_int322half(__ubuf__ half* dst, __ubuf__ int32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Merge sort 4 for half/float data types
 * @param [out] dst Destination buffer for sorted result
 * @param [in] src Source buffer array
 * @param [in] repeat Number of repeat operations
 * @param [in] element_length_0 Length of first element group
 * @param [in] element_length_1 Length of second element group
 * @param [in] element_length_2 Length of third element group
 * @param [in] element_length_3 Length of fourth element group
 * @param [in] enable_exhausted_suspension Whether to suspend when exhausted
 * @param [in] valid_bit Valid bit mask
 * @details Performs merge sort on 4 element groups for half or float data.
 */
__aicore__ inline void asc_mrgsort4(__ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN],
    uint8_t repeat, uint16_t element_length_0, uint16_t element_length_1, uint16_t element_length_2,
    uint16_t element_length_3, bool enable_exhausted_suspension, uint8_t valid_bit);
__aicore__ inline void asc_mrgsort4(__ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN],
    uint8_t repeat, uint16_t element_length_0, uint16_t element_length_1, uint16_t element_length_2,
    uint16_t element_length_3, bool enable_exhausted_suspension, uint8_t valid_bit);

/*
 * @ingroup VectorCompute
 * @brief ReLU activation for half/float/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Applies ReLU activation: dst[i] = max(0, src[i]).
 *          Supports half, float, and int32_t data types.
 */
__aicore__ inline void asc_relu(__ubuf__ half* dst, __ubuf__ half* src,  uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_relu(__ubuf__ float* dst, __ubuf__ float* src,  uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_relu(__ubuf__ int32_t* dst, __ubuf__ int32_t* src,  uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Element-wise maximum for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src0_block_stride First source block stride for data layout
 * @param [in] src1_block_stride Second source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src0_repeat_stride First source repeat stride for iteration control
 * @param [in] src1_repeat_stride Second source repeat stride for iteration control
 * @details Performs element-wise maximum: dst[i] = max(src0[i], src1[i]).
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_max(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_max(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_max(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_max(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Element-wise maximum with scalar for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Scalar value to compare
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs element-wise maximum with scalar: dst[i] = max(src[i], value).
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_max_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_max_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_max_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_max_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Element-wise minimum for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src0_block_stride First source block stride for data layout
 * @param [in] src1_block_stride Second source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src0_repeat_stride First source repeat stride for iteration control
 * @param [in] src1_repeat_stride Second source repeat stride for iteration control
 * @details Performs element-wise minimum: dst[i] = min(src0[i], src1[i]).
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_min(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_min(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_min(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_min(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Fused multiply-add operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src0_block_stride First source block stride for data layout
 * @param [in] src1_block_stride Second source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src0_repeat_stride First source repeat stride for iteration control
 * @param [in] src1_repeat_stride Second source repeat stride for iteration control
 * @details Performs fused multiply-add operation with destination accumulation.
 *          Supports half, float, and mixed float/half data types.
 */
__aicore__ inline void asc_fma(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_fma(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_fma(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Vector multiplication for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src0_block_stride First source block stride for data layout
 * @param [in] src1_block_stride Second source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src0_repeat_stride First source repeat stride for iteration control
 * @param [in] src1_repeat_stride Second source repeat stride for iteration control
 * @details Performs element-wise multiplication: dst[i] = src0[i] * src1[i].
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_mul(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_mul(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_mul(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_mul(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Vector multiplication with scalar for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Scalar value to multiply
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs element-wise multiplication with a scalar: dst[i] = src[i] * value.
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_mul_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_mul_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_mul_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_mul_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Select operation based on mask for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer (selected when mask is true)
 * @param [in] src1 Second source buffer (selected when mask is false)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src0_block_stride First source block stride for data layout
 * @param [in] src1_block_stride Second source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src0_repeat_stride First source repeat stride for iteration control
 * @param [in] src1_repeat_stride Second source repeat stride for iteration control
 * @details Selects elements from src0 or src1 based on comparison mask.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Vector subtraction for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src0_block_stride First source block stride for data layout
 * @param [in] src1_block_stride Second source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src0_repeat_stride First source repeat stride for iteration control
 * @param [in] src1_repeat_stride Second source repeat stride for iteration control
 * @details Performs element-wise subtraction: dst[i] = src0[i] - src1[i].
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_sub(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_sub(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_sub(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_sub(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, 
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Vector subtraction with scalar for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Scalar value to subtract
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs element-wise subtraction with a scalar: dst[i] = src[i] - value.
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_sub_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_sub_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_sub_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_sub_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat, 
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Bitwise OR operation for int16/uint16 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src0_block_stride First source block stride for data layout
 * @param [in] src1_block_stride Second source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src0_repeat_stride First source repeat stride for iteration control
 * @param [in] src1_repeat_stride Second source repeat stride for iteration control
 * @details Performs bitwise OR: dst[i] = src0[i] | src1[i].
 *          Supports int16_t and uint16_t data types.
 */
__aicore__ inline void asc_or(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_or(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Bitwise NOT operation for int16/uint16 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs bitwise NOT: dst[i] = ~src[i].
 *          Supports int16_t and uint16_t data types.
 */
__aicore__ inline void asc_not(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
                               uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_not(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeat,
                               uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                               uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Reciprocal (inverse) for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Computes reciprocal: dst[i] = 1 / src[i].
 *          Supports half and float data types.
 */
__aicore__ inline void asc_rcp(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_rcp(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Right shift operation for int16/int32/uint16/uint32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Number of bits to shift
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs right shift: dst[i] = src[i] >> value.
 *          Supports int16_t, int32_t, uint16_t, uint32_t data types.
 */
__aicore__ inline void asc_shiftright(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_shiftright(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_shiftright(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint8_t repeat,
uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Element-wise equality comparison with scalar for half/float/int32 data types
 * @param [out] dst Destination buffer for comparison result (uint8_t)
 * @param [in] src Source buffer
 * @param [in] value Scalar value to compare against
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Performs element-wise equality comparison: dst[i] = (src[i] == value) ? 1 : 0.
 *          Supports half, float, and int32_t data types.
 */
__aicore__ inline void asc_eq_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_eq_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_eq_scalar(__ubuf__ uint8_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Dequantize int16 to 8-bit data (high/low byte extraction)
 * @param [out] dst Destination buffer (int8_t or uint8_t)
 * @param [in] src Source buffer (int16_t)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Extracts high or low byte from int16_t data:
 *          - asc_deq_int162b8_h: extracts high byte
 *          - asc_deq_int162b8_l: extracts low byte
 *          Supports signed (int8_t) and unsigned (uint8_t) output types.
 */
__aicore__ inline void asc_deq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);
__aicore__ inline void asc_deq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);
__aicore__ inline void asc_deq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);
__aicore__ inline void asc_deq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Convert 4-bit integer to half precision
 * @param [out] dst Destination buffer (half)
 * @param [in] src Source buffer (int4b_t)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts 4-bit integer values to half precision floating point.
 */
__aicore__ inline void asc_int42half(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Convert 8-bit signed integer to half precision
 * @param [out] dst Destination buffer (half)
 * @param [in] src Source buffer (int8_t)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts int8_t values to half precision floating point.
 */
__aicore__ inline void asc_int82half(__ubuf__ half* dst, __ubuf__ int8_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Convert 8-bit unsigned integer to half precision
 * @param [out] dst Destination buffer (half)
 * @param [in] src Source buffer (uint8_t)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts uint8_t values to half precision floating point.
 */
__aicore__ inline void asc_uint82half(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Convert 16-bit signed integer to single precision
 * @param [out] dst Destination buffer (float)
 * @param [in] src Source buffer (int16_t)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts int16_t values to single precision floating point.
 */
__aicore__ inline void asc_int162float(__ubuf__ float* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Convert 32-bit signed integer to single precision with optional rounding modes
 * @param [out] dst Destination buffer (float)
 * @param [in] src Source buffer (int32_t)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts int32_t values to single precision floating point.
 *          Available variants:
 *          - asc_int322float: default conversion
 *          - asc_int322float_rna: round to nearest, ties away from zero
 *          - asc_int322float_ru: round up toward +infinity
 *          - asc_int322float_rd: round down toward -infinity
 *          - asc_int322float_rn: round to nearest, ties to even
 *          - asc_int322float_rz: round toward zero
 */
__aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int322float_rna(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int322float_ru(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int322float_rd(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int322float_rn(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int322float_rz(__ubuf__ float* dst, __ubuf__ int32_t* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Convert 32-bit signed integer to 16-bit signed integer
 * @param [out] dst Destination buffer (int16_t)
 * @param [in] src Source buffer (int32_t)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts int32_t values to int16_t with saturation.
 */
__aicore__ inline void asc_int322int16(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Convert 32-bit signed integer to 64-bit signed integer
 * @param [out] dst Destination buffer (int64_t)
 * @param [in] src Source buffer (int32_t)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts int32_t values to int64_t with sign extension.
 */
__aicore__ inline void asc_int322int64(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Convert 64-bit signed integer to 32-bit signed integer
 * @param [out] dst Destination buffer (int32_t)
 * @param [in] src Source buffer (int64_t)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride for data layout
 * @param [in] src_block_stride Source block stride for data layout
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] src_repeat_stride Source repeat stride for iteration control
 * @details Converts int64_t values to int32_t with saturation.
 */
__aicore__ inline void asc_int642int32(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Gather elements from source buffer using indices for various data types
 * @param [out] dst Destination buffer
 * @param [in] src Source buffer
 * @param [in] src_offset Buffer containing indices for gathering
 * @param [in] dst_repeat_stride Destination repeat stride for iteration control
 * @param [in] repeat Number of repeat operations
 * @details Gathers elements from source buffer at positions specified by src_offset.
 *          dst[i] = src[src_offset[i]]
 *          Supports int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, and float data types.
 */
__aicore__ inline void asc_gather(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset,
    uint16_t dst_repeat_stride, uint8_t repeat);
__aicore__ inline void asc_gather(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset,
    uint16_t dst_repeat_stride, uint8_t repeat);
__aicore__ inline void asc_gather(__ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset,
    uint16_t dst_repeat_stride, uint8_t repeat);
__aicore__ inline void asc_gather(__ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset,
    uint16_t dst_repeat_stride, uint8_t repeat);
__aicore__ inline void asc_gather(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset,
    uint16_t dst_repeat_stride, uint8_t repeat);
__aicore__ inline void asc_gather(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset,
    uint16_t dst_repeat_stride, uint8_t repeat);
__aicore__ inline void asc_gather(__ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset,
    uint16_t dst_repeat_stride, uint8_t repeat);

/*
 * @ingroup VectorCompute
 * @brief enable_transpose matrix for int16/uint16 data types
 * @param [out] dst Destination buffer for transposed result
 * @param [in] src Source buffer
 * @details Performs matrix transposition. Supports int16_t and uint16_t data types.
 */
__aicore__ inline void asc_transpose(__ubuf__ int16_t* dst, __ubuf__ int16_t* src);
__aicore__ inline void asc_transpose(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src);


/*
 * @ingroup VectorCompute
 * @brief Bitwise AND operation for int16/uint16 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs element-wise bitwise AND: dst[i] = src0[i] & src1[i].
 *          Supports int16_t and uint16_t data types.
 */
__aicore__ inline void asc_and(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_and(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Leaky ReLU activation function for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Slope value for negative inputs
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Performs Leaky ReLU: dst[i] = src[i] if src[i] > 0 else value * src[i].
 *          Supports half and float data types.
 */
__aicore__ inline void asc_leakyrelu(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_leakyrelu(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Element-wise less than comparison with scalar for half/float data types
 * @param [out] dst Destination buffer for comparison result (uint8_t)
 * @param [in] src0 Source buffer
 * @param [in] value Scalar value to compare against
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride Source repeat stride
 * @details Performs element-wise less than comparison: dst[i] = (src0[i] < value) ? 1 : 0.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_lt_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src0, half value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src0_block_stride, uint16_t dst_repeat_stride, uint16_t src0_repeat_stride);
__aicore__ inline void asc_lt_scalar(__ubuf__ uint8_t* dst, __ubuf__ float* src0, float value, uint8_t repeat, uint16_t dst_block_stride, uint16_t src0_block_stride, uint16_t dst_repeat_stride, uint16_t src0_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Subtract followed by ReLU activation for various data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs subtraction followed by ReLU: dst[i] = max(0, src0[i] - src1[i]).
 *          Supports various data type combinations including half, float, int16_t, int8_t.
 */
__aicore__ inline void asc_sub_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_sub_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_sub_relu(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_sub_relu(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_sub_relu(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_sub_relu(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Convert single precision to 16-bit signed integer with various rounding modes
 * @param [out] dst Destination buffer (int16_t)
 * @param [in] src Source buffer (float)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Converts float values to int16_t with specified rounding modes:
 *          - asc_float2int16_rna: round to nearest, ties away from zero
 *          - asc_float2int16_ru: round up toward +infinity
 *          - asc_float2int16_rd: round down toward -infinity
 *          - asc_float2int16_rn: round to nearest, ties to even
 *          - asc_float2int16_rz: round toward zero
 */
__aicore__ inline void asc_float2int16_rna(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2int16_ru(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2int16_rd(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2int16_rn(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_float2int16_rz(__ubuf__ int16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Reciprocal square root for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Computes reciprocal square root: dst[i] = 1 / sqrt(src[i]).
 *          Supports half and float data types.
 */
__aicore__ inline void asc_rsqrt(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_rsqrt(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
    uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Add followed by ReLU activation for half/float/int16 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs addition followed by ReLU: dst[i] = max(0, src0[i] + src1[i]).
 *          Supports half, float, and int16_t data types.
 */
__aicore__ inline void asc_add_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    int8_t src0_repeat_stride, int8_t src1_repeat_stride);
__aicore__ inline void asc_add_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    int8_t src0_repeat_stride, int8_t src1_repeat_stride);
__aicore__ inline void asc_add_relu(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    int8_t src0_repeat_stride, int8_t src1_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Element-wise greater than or equal comparison for half/float data types
 * @param [out] dst Destination buffer for comparison result (uint8_t, optional)
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs element-wise greater than or equal comparison: dst[i] = (src0[i] >= src1[i]) ? 1 : 0.
 *          Supports half and float data types. Some variants output to uint8_t buffer.
 */
__aicore__ inline void asc_ge(__ubuf__ half* scr0, __ubuf__ half* scr1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, int8_t src0_repeat_stride,
    int8_t src1_repeat_stride);
__aicore__ inline void asc_ge(__ubuf__ float* scr0, __ubuf__ float* scr1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, int8_t src0_repeat_stride,
    int8_t src1_repeat_stride);
__aicore__ inline void asc_ge(__ubuf__ uint8_t* dst, __ubuf__ half* scr0, __ubuf__ half* scr1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    int8_t src0_repeat_stride, int8_t src1_repeat_stride);
__aicore__ inline void asc_ge(__ubuf__ uint8_t* dst, __ubuf__ float* scr0, __ubuf__ float* scr1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    int8_t src0_repeat_stride, int8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Reduce operation for uint16/uint32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] src0_block_stride First source block stride
 * @param [in] pattern_mode Pattern mode for reduction
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs reduction operation combining two source buffers.
 *          Supports uint16_t and uint32_t data types.
 */
__aicore__ inline void asc_reduce(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1,
    uint16_t repeat, uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_reduce(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1,
    uint16_t repeat, uint8_t src0_block_stride, uint8_t pattern_mode, uint16_t src0_repeat_stride, uint8_t src1_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Convert 64-bit signed integer to single precision with various rounding modes
 * @param [out] dst Destination buffer (float)
 * @param [in] src Source buffer (int64_t)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Converts int64_t values to single precision floating point with specified rounding modes:
 *          - asc_int642float_rna: round to nearest, ties away from zero
 *          - asc_int642float_ru: round up toward +infinity
 *          - asc_int642float_rd: round down toward -infinity
 *          - asc_int642float_rn: round to nearest, ties to even
 *          - asc_int642float_rz: round toward zero
 */
__aicore__ inline void asc_int642float_rna(__ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int642float_ru(__ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int642float_rd(__ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int642float_rn(__ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int642float_rz(__ubuf__ float* dst, __ubuf__ int64_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Vector dequantize int16 to 8-bit data (high/low byte extraction)
 * @param [out] dst Destination buffer (int8_t or uint8_t)
 * @param [in] src Source buffer (int16_t)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Vector dequantization extracting high or low byte from int16_t:
 *          - asc_vdeq_int162b8_h: extracts high byte
 *          - asc_vdeq_int162b8_l: extracts low byte
 *          Supports signed (int8_t) and unsigned (uint8_t) output types.
 */
__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);
__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);
__aicore__ inline void asc_vdeq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);
__aicore__ inline void asc_vdeq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint8_t dst_repeat_stride, uint8_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Square root for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Computes square root: dst[i] = sqrt(src[i]).
 *          Supports half and float data types.
 */
__aicore__ inline void asc_sqrt(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_block_stride,
        uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_sqrt(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride,
        uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief AXPY operation (A*X + Y) for half/float data types
 * @param [out] dst Destination buffer for result (Y)
 * @param [in] src Source buffer (X)
 * @param [in] value Scalar multiplier (A)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Performs AXPY: dst[i] = value * src[i] + dst[i].
 *          Supports half and float data types, plus mixed precision (float dst, half src).
 */
__aicore__ inline void asc_axpy(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ half* src, half value, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Element-wise less than comparison for half/float data types
 * @param [out] dst Destination buffer for comparison result (uint8_t, optional)
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs element-wise less than comparison: dst[i] = (src0[i] < src1[i]) ? 1 : 0.
 *          Supports half and float data types. Some variants output to uint8_t buffer.
 */
__aicore__ inline void asc_lt(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
        uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_lt(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride,
        uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_lt(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
        uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_lt(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
        uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Element-wise not equal comparison with scalar for half/float data types
 * @param [out] dst Destination buffer for comparison result (uint8_t)
 * @param [in] src Source buffer
 * @param [in] value Scalar value to compare against
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Performs element-wise not equal comparison: dst[i] = (src[i] != value) ? 1 : 0.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_ne_scalar(__ubuf__ uint8_t *dst, __ubuf__ half *src, half value, uint8_t repeat, uint16_t dst_block_stride,
           uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_ne_scalar(__ubuf__ uint8_t *dst, __ubuf__ float *src, float value, uint8_t repeat, uint16_t dst_block_stride,
           uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Convert 16-bit signed integer to half precision with various rounding modes
 * @param [out] dst Destination buffer (half)
 * @param [in] src Source buffer (int16_t)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Converts int16_t values to half precision floating point with specified rounding modes:
 *          - asc_int162half: default conversion
 *          - asc_int162half_rna: round to nearest, ties away from zero
 *          - asc_int162half_ru: round up toward +infinity
 *          - asc_int162half_rd: round down toward -infinity
 *          - asc_int162half_rn: round to nearest, ties to even
 *          - asc_int162half_rz: round toward zero
 */
__aicore__ inline void asc_int162half(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int162half_rna(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int162half_ru(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int162half_rd(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int162half_rn(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_int162half_rz(__ubuf__ half* dst, __ubuf__ int16_t* src, uint8_t repeat, uint16_t dst_block_stride,
            uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Gather data blocks from source buffer using indices for uint16/uint32 data types
 * @param [out] dst Destination buffer
 * @param [in] src Source buffer
 * @param [in] src_offset Buffer containing indices for gathering
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] dst_block_stride Destination block stride
 * @param [in] repeat Number of repeat operations
 * @details Gathers data blocks from source buffer at positions specified by src_offset.
 *          Supports uint16_t and uint32_t data types.
 */
__aicore__ inline void asc_gather_datablock(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, 
            uint16_t dst_repeat_stride, uint8_t dst_block_stride, uint8_t repeat);
__aicore__ inline void asc_gather_datablock(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, 
            uint16_t dst_repeat_stride, uint8_t dst_block_stride, uint8_t repeat);

/*
 * @ingroup VectorCompute
 * @brief Element-wise minimum with scalar for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Scalar value to compare against
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Performs element-wise minimum: dst[i] = min(src[i], value).
 *          Supports half, float, int16_t, and int32_t data types.
 */
__aicore__ inline void asc_min_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_min_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_min_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_min_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Add followed by ReLU with type conversion for various data type combinations
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs addition followed by ReLU with type conversion:
 *          - f162s8: half inputs, int8_t output
 *          - f322f16: float inputs, half output
 *          - s162s8: int16_t inputs, int8_t output
 */
__aicore__ inline void asc_add_relu(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_add_relu(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_add_relu(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Element-wise greater than comparison for half/float data types
 * @param [out] dst Destination buffer for comparison result (uint8_t, optional)
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs element-wise greater than comparison: dst[i] = (src0[i] > src1[i]) ? 1 : 0.
 *          Supports half and float data types. Some variants output to uint8_t buffer.
 */
__aicore__ inline void asc_gt(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, 
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_gt(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, 
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_gt(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, 
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_gt(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, 
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, 
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
    
/*
 * @ingroup VectorCompute
 * @brief Convert half precision to 8-bit unsigned integer with various rounding modes
 * @param [out] dst Destination buffer (uint8_t)
 * @param [in] src Source buffer (half)
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Converts half values to uint8_t with specified rounding modes:
 *          - asc_half2uint8: default conversion
 *          - asc_half2uint8_rna: round to nearest, ties away from zero
 *          - asc_half2uint8_ru: round up toward +infinity
 *          - asc_half2uint8_rd: round down toward -infinity
 *          - asc_half2uint8_rn: round to nearest, ties to even
 *          - asc_half2uint8_rz: round toward zero
 */
__aicore__ inline void asc_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2uint8_rna(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2uint8_ru(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2uint8_rd(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2uint8_rn(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_half2uint8_rz(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);


/*
 * @ingroup VectorCompute
 * @brief Element-wise less than or equal comparison for half/float data types
 * @param [out] dst Destination buffer for comparison result (uint8_t, optional)
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs element-wise less than or equal comparison: dst[i] = (src0[i] <= src1[i]) ? 1 : 0.
 *          Supports half and float data types. Some variants output to uint8_t buffer.
 */
__aicore__ inline void asc_le(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_le(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
                              uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                              uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_le(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_le(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                              uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                              uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Element-wise less than or equal comparison with scalar for half/float data types
 * @param [out] dst Destination buffer for comparison result (uint8_t)
 * @param [in] src Source buffer
 * @param [in] value Scalar value to compare against
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Performs element-wise less than or equal comparison: dst[i] = (src[i] <= value) ? 1 : 0.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_le_scalar(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                     uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride,
                                     uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Pair reduce sum operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_block_stride Source block stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Performs sum reduction on paired elements. Supports half and float data types.
 */
__aicore__ inline void asc_pair_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint8_t repeat, uint16_t dst_repeat_stride,
                                           uint16_t src_block_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_pair_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_repeat_stride,
                                           uint16_t src_block_stride, uint16_t src_repeat_stride);

// ============================================================================
// Data Manipulation Operations - Copy and squeeze operations
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Copy data from source to destination for int16/uint16/int32/uint32 data types
 * @param [out] dst Destination buffer
 * @param [in] src Source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Copies data from source to destination buffer. Supports int16_t, uint16_t, int32_t, uint32_t.
 */
__aicore__ inline void asc_copy(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_copy(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_copy(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_copy(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Squeeze operation for uint16/uint32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs squeeze operation combining two sources. Supports uint16_t and uint32_t.
 */
__aicore__ inline void asc_squeeze(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_squeeze(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif    

