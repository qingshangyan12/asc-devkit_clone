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
 * @file vector_compute_emulation.h
 * @brief Software emulation implementation of chip vector compute instructions.
 * @details This file provides software emulation of hardware vector compute instructions.
 *          The purpose is to enhance operator development usability and simplify programming.
 *          Since this is a software emulation layer (not direct hardware instruction execution),
 *          there will be performance overhead compared to native hardware implementations.
 *          These interfaces support vector operations including:
 *          - Arithmetic operations: add, sub, mul, div, fma, etc.
 *          - Type conversions: float2half, half2int, bfloat16 conversions, etc.
 *          - Comparison operations: eq, ne, ge, le, etc.
 *          - Reduction operations: reduce_sum, reduce_max, reduce_min, etc.
 *          - Sorting operations: mrgsort4, etc.
 *          - Mathematical functions: exp, log, abs, relu, sqrt, rsqrt, rcp, etc.
 *          - Bit operations: and, or, not, shiftleft, shiftright, etc.
 *          - Data manipulation: duplicate, copy, gather, enable_transpose, select, etc.
 *          Supported data types: half, float, int8_t, int16_t, int32_t, int64_t,
 *          uint8_t, uint16_t, uint32_t, bfloat16_t, int4b_t, etc.
 * 
 * @note Performance Consideration: These emulated interfaces are designed for development
 *       convenience and debugging purposes. For production deployment requiring optimal
 *       performance, consider using native hardware instructions directly.
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

// ============================================================================
// Arithmetic Operations - Basic arithmetic computations
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Vector addition for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer  
 * @param [in] count Number of elements to process
 * @details Performs element-wise addition: dst[i] = src0[i] + src1[i].
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_add_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_add_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_add(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_add_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_add(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);
__aicore__ inline void asc_add_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Vector addition with scalar for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Scalar value to add
 * @param [in] count Number of elements to process
 * @details Performs element-wise addition with a scalar: dst[i] = src[i] + value.
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_add_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_add_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_add_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_add_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_add_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);
__aicore__ inline void asc_add_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);
__aicore__ inline void asc_add_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);
__aicore__ inline void asc_add_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Vector subtraction for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Performs element-wise subtraction: dst[i] = src0[i] - src1[i].
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_sub(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_sub_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_sub(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_sub_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_sub(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_sub_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_sub(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);
__aicore__ inline void asc_sub_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Vector subtraction with scalar for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Scalar value to subtract
 * @param [in] count Number of elements to process
 * @details Performs element-wise subtraction with a scalar: dst[i] = src[i] - value.
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_sub_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_sub_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_sub_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);
__aicore__ inline void asc_sub_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);
__aicore__ inline void asc_sub_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Vector multiplication for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Performs element-wise multiplication: dst[i] = src0[i] * src1[i].
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_mul(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_mul_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_mul(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_mul_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_mul(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_mul_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_mul(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);
__aicore__ inline void asc_mul_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Vector multiplication with scalar for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Scalar value to multiply
 * @param [in] count Number of elements to process
 * @details Performs element-wise multiplication with a scalar: dst[i] = src[i] * value.
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_mul_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_mul_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_mul_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);
__aicore__ inline void asc_mul_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);
__aicore__ inline void asc_mul_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Vector division for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer (dividend)
 * @param [in] src1 Second source buffer (divisor)
 * @param [in] count Number of elements to process
 * @details Performs element-wise division: dst[i] = src0[i] / src1[i].
 *          Supports half and float data types.
 */
__aicore__ inline void asc_div(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_div_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_div(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_div_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Fused multiply-add operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Performs fused multiply-add operation with destination accumulation.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_fma(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_fma_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_fma(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_fma_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_fma(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_fma_sync(__ubuf__ float* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Multiply-add operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Performs multiply-add operation: dst[i] = dst[i] + src0[i] * src1[i].
 *          Supports half and float data types.
 */
__aicore__ inline void asc_mul_add(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_mul_add_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_mul_add(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_mul_add_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Multiply-add-relu operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Performs multiply-add-relu: dst[i] = relu(dst[i] + src0[i] * src1[i]).
 *          Supports half and float data types.
 */
__aicore__ inline void asc_mul_add_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_mul_add_relu_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_mul_add_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_mul_add_relu_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief AXPY operation (alpha*x plus y) for half/float data types
 * @param [out] dst Destination buffer for result (y vector)
 * @param [in] src Source buffer (x vector)
 * @param [in] value Scalar value (alpha)
 * @param [in] count Number of elements to process
 * @details Performs AXPY operation: dst[i] = value * src[i] + dst[i].
 *          Supports half, float and mixed float/half data types.
 */
__aicore__ inline void asc_axpy(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_axpy_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_axpy_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_axpy(__ubuf__ float* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_axpy_sync(__ubuf__ float* dst, __ubuf__ half* src, half value, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Multiply with cast to int8 for half data type
 * @param [out] dst Destination buffer for result (int8)
 * @param [in] src0 First source buffer (half)
 * @param [in] src1 Second source buffer (half)
 * @param [in] count Number of elements to process
 * @details Performs multiplication of half values and casts result to int8: dst[i] = (int8_t)(src0[i] * src1[i]).
 */
__aicore__ inline void asc_mul_cast_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_mul_cast_half2int8_sync(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Multiply with cast to uint8 for half data type
 * @param [out] dst Destination buffer for result (uint8)
 * @param [in] src0 First source buffer (half)
 * @param [in] src1 Second source buffer (half)
 * @param [in] count Number of elements to process
 * @details Performs multiplication of half values and casts result to uint8: dst[i] = (uint8_t)(src0[i] * src1[i]).
 */
__aicore__ inline void asc_mul_cast_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_mul_cast_half2uint8_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);

// ============================================================================
// Comparison Operations - Element-wise comparison between vectors
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Vector not-equal comparison for half/float data types
 * @param [out] dst Destination buffer for result (optional, uint8 mask)
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs element-wise not-equal comparison: result = (src0[i] != src1[i]).
 *          Supports half and float data types. Result is stored as boolean mask.
 */
__aicore__ inline void asc_ne_sync(__ubuf__ half* src0, __ubuf__ half* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_ne_sync(__ubuf__ float* src0, __ubuf__ float* src1,
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_ne_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, 
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_ne_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, 
    uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
    uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Vector greater-or-equal comparison for half/float data types
 * @param [out] dst Destination buffer for result (optional, uint8 mask)
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs element-wise greater-or-equal comparison: result = (src0[i] >= src1[i]).
 *          Supports half and float data types. Result is stored as boolean mask.
 */
__aicore__ inline void asc_ge_sync(__ubuf__ half* scr0, __ubuf__ half* scr1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, int8_t src0_repeat_stride,
    int8_t src1_repeat_stride);
__aicore__ inline void asc_ge_sync(__ubuf__ float* scr0, __ubuf__ float* scr1, uint8_t repeat, uint8_t dst_block_stride,
    uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, int8_t src0_repeat_stride,
    int8_t src1_repeat_stride);
__aicore__ inline void asc_ge_sync(__ubuf__ uint8_t* dst, __ubuf__ half* scr0, __ubuf__ half* scr1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    int8_t src0_repeat_stride, int8_t src1_repeat_stride);
__aicore__ inline void asc_ge_sync(__ubuf__ uint8_t* dst, __ubuf__ float* scr0, __ubuf__ float* scr1, uint8_t repeat,
    uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
    int8_t src0_repeat_stride, int8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Vector greater-or-equal comparison with scalar for half/float data types
 * @param [out] dst Destination buffer for result (uint8 mask)
 * @param [in] src Source buffer
 * @param [in] value Scalar value to compare
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Performs element-wise greater-or-equal comparison with scalar: result = (src[i] >= value).
 *          Supports half and float data types. Result is stored as boolean mask.
 */
__aicore__ inline void asc_ge_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_ge_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
    uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Vector less-or-equal comparison for half/float data types
 * @param [out] dst Destination buffer for result (optional, uint8 mask)
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src0_block_stride First source block stride
 * @param [in] src1_block_stride Second source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src0_repeat_stride First source repeat stride
 * @param [in] src1_repeat_stride Second source repeat stride
 * @details Performs element-wise less-or-equal comparison: result = (src0[i] <= src1[i]).
 *          Supports half and float data types. Result is stored as boolean mask.
 */
__aicore__ inline void asc_le_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_le_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride,
                                   uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride,
                                   uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_le_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);
__aicore__ inline void asc_le_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat,
                                   uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride,
                                   uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride);

/*
 * @ingroup VectorCompute
 * @brief Vector less-or-equal comparison with scalar for half/float data types
 * @param [out] dst Destination buffer for result (uint8 mask)
 * @param [in] src Source buffer
 * @param [in] value Scalar value to compare
 * @param [in] repeat Number of repeat operations
 * @param [in] dst_block_stride Destination block stride
 * @param [in] src_block_stride Source block stride
 * @param [in] dst_repeat_stride Destination repeat stride
 * @param [in] src_repeat_stride Source repeat stride
 * @details Performs element-wise less-or-equal comparison with scalar: result = (src[i] <= value).
 *          Supports half and float data types. Result is stored as boolean mask.
 */
__aicore__ inline void asc_le_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, half value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src_repeat_stride);
__aicore__ inline void asc_le_scalar_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src, float value, uint8_t repeat,
                                          uint16_t dst_block_stride, uint16_t src_block_stride,
                                          uint16_t dst_repeat_stride, uint16_t src_repeat_stride);

// ============================================================================
// Reduction Operations - Aggregate values across vector elements
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Data block reduce sum operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs sum reduction across data blocks. Supports half and float data types.
 */
__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_datablock_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Data block reduce max operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs max reduction across data blocks. Supports half and float data types.
 */
__aicore__ inline void asc_datablock_reduce_max(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_datablock_reduce_max_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_datablock_reduce_max(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_datablock_reduce_max_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Data block reduce min operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs min reduction across data blocks. Supports half and float data types.
 */
__aicore__ inline void asc_datablock_reduce_min(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_datablock_reduce_min_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_datablock_reduce_min(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_datablock_reduce_min_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce sum operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs sum reduction with repeat pattern. Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce max operations returning index and value for half/float data types
 * @param [out] dst Destination buffer for result (index and value pair)
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs max reduction with repeat pattern, returns both index and value.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_max_index_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_max_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_max_index_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce max operations returning value and index for half/float data types
 * @param [out] dst Destination buffer for result (value and index pair)
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs max reduction with repeat pattern, returns both value and index.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_max_value_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_max_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_max_value_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce max operations returning only value for half/float data types
 * @param [out] dst Destination buffer for result (value only)
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs max reduction with repeat pattern, returns only value.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_max_only_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_max_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_max_only_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce max operations returning only index for half/float data types
 * @param [out] dst Destination buffer for result (index only)
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs max reduction with repeat pattern, returns only index.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_max_only_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_max_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_max_only_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce min operations returning index and value for half/float data types
 * @param [out] dst Destination buffer for result (index and value pair)
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs min reduction with repeat pattern, returns both index and value.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_min_index_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_min_index_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_min_index_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce min operations returning value and index for half/float data types
 * @param [out] dst Destination buffer for result (value and index pair)
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs min reduction with repeat pattern, returns both value and index.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_min_value_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_min_value_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_min_value_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce min operations returning only value for half/float data types
 * @param [out] dst Destination buffer for result (value only)
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs min reduction with repeat pattern, returns only value.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_min_only_value_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_min_only_value(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_min_only_value_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Repeat reduce min operations returning only index for half/float data types
 * @param [out] dst Destination buffer for result (index only)
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs min reduction with repeat pattern, returns only index.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_min_only_index_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_min_only_index(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_repeat_reduce_min_only_index_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Pair reduce sum operation for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs sum reduction on paired elements. Supports half and float data types.
 */
__aicore__ inline void asc_pair_reduce_sum(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_pair_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_pair_reduce_sum(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_pair_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Reduce operation for uint16/uint32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Performs reduction operation on unsigned integers. Supports uint16_t and uint32_t data types.
 */
__aicore__ inline void asc_reduce(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);
__aicore__ inline void asc_reduce_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);
__aicore__ inline void asc_reduce(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint32_t count);
__aicore__ inline void asc_reduce_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src0, __ubuf__ uint32_t* src1, uint32_t count);

// ============================================================================
// Type Conversion Operations - Convert between different data types
// ============================================================================
// Bfloat16 <-> Float conversions
/*
 * @ingroup VectorCompute
 * @brief Convert bfloat16 to float
 * @param [out] dst Destination buffer for float result
 * @param [in] src Source buffer with bfloat16 data
 * @param [in] count Number of elements to process
 * @details Converts bfloat16_t elements to single-precision float.
 */
__aicore__ inline void asc_bfloat162float(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count);
__aicore__ inline void asc_bfloat162float_sync(__ubuf__ float* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert bfloat16 to int32 with various rounding modes
 * @param [out] dst Destination buffer for int32 result
 * @param [in] src Source buffer with bfloat16 data
 * @param [in] count Number of elements to process
 * @details Converts bfloat16_t elements to int32_t with different rounding modes.
 *          Rounding modes: rna (round to nearest away), ru (round up), rd (round down),
 *          rn (round to nearest), rz (round toward zero).
 */
__aicore__ inline void asc_bfloat162int32_rna(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);
__aicore__ inline void asc_bfloat162int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);
__aicore__ inline void asc_bfloat162int32_ru(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);
__aicore__ inline void asc_bfloat162int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);
__aicore__ inline void asc_bfloat162int32_rd(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);
__aicore__ inline void asc_bfloat162int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);
__aicore__ inline void asc_bfloat162int32_rn(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);
__aicore__ inline void asc_bfloat162int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);
__aicore__ inline void asc_bfloat162int32_rz(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);
__aicore__ inline void asc_bfloat162int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ bfloat16_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert float to bfloat16 with various rounding modes
 * @param [out] dst Destination buffer for bfloat16 result
 * @param [in] src Source buffer with float data
 * @param [in] count Number of elements to process
 * @details Converts single-precision float elements to bfloat16_t with different rounding modes.
 *          Rounding modes: rn, rna, rd, ru, rz.
 */
__aicore__ inline void asc_float2bfloat16_rn(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2bfloat16_rn_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2bfloat16_rna(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2bfloat16_rna_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2bfloat16_rd(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2bfloat16_rd_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2bfloat16_ru(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2bfloat16_ru_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2bfloat16_rz(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2bfloat16_rz_sync(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count);

// Float <-> Half conversions
/*
 * @ingroup VectorCompute
 * @brief Convert float to half precision with various rounding modes
 * @param [out] dst Destination buffer for half result
 * @param [in] src Source buffer with float data
 * @param [in] count Number of elements to process
 * @details Converts single-precision float elements to half precision with different rounding modes.
 *          Rounding modes: default, rn, rna, rd, ru, rz, ro (round to odd).
 */
__aicore__ inline void asc_float2half(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_rn(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_rn_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_rna(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_rna_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_rd(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_rd_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_ru(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_ru_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_rz(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_rz_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_ro(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2half_ro_sync(__ubuf__ half* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert half to float precision
 * @param [out] dst Destination buffer for float result
 * @param [in] src Source buffer with half data
 * @param [in] count Number of elements to process
 * @details Converts half precision elements to single-precision float.
 */
__aicore__ inline void asc_half2float(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2float_sync(__ubuf__ float* dst, __ubuf__ half* src, uint32_t count);

// Float <-> Int conversions
/*
 * @ingroup VectorCompute
 * @brief Convert float to int32 with various rounding modes
 * @param [out] dst Destination buffer for int32 result
 * @param [in] src Source buffer with float data
 * @param [in] count Number of elements to process
 * @details Converts float elements to int32_t with different rounding modes.
 *          Rounding modes: rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_float2int32_rna(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int32_ru(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int32_rd(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int32_rn(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int32_rz(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert float to int64 with various rounding modes
 * @param [out] dst Destination buffer for int64 result
 * @param [in] src Source buffer with float data
 * @param [in] count Number of elements to process
 * @details Converts float elements to int64_t with different rounding modes.
 *          Rounding modes: rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_float2int64_rna(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int64_rna_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int64_ru(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int64_ru_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int64_rd(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int64_rd_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int64_rn(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int64_rn_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int64_rz(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int64_rz_sync(__ubuf__ int64_t* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert float to int16 with various rounding modes
 * @param [out] dst Destination buffer for int16 result
 * @param [in] src Source buffer with float data
 * @param [in] count Number of elements to process
 * @details Converts float elements to int16_t with different rounding modes.
 *          Rounding modes: rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_float2int16_rna(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int16_rna_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int16_ru(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int16_ru_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int16_rd(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int16_rd_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int16_rn(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int16_rn_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int16_rz(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2int16_rz_sync(__ubuf__ int16_t* dst, __ubuf__ float* src, uint32_t count);

// Float to Float conversions (with rounding)
/*
 * @ingroup VectorCompute
 * @brief Convert float to float with various rounding modes
 * @param [out] dst Destination buffer for float result
 * @param [in] src Source buffer with float data
 * @param [in] count Number of elements to process
 * @details Performs float to float conversion with specified rounding modes.
 *          Rounding modes: rn, rd, ru, rna, rz.
 */
__aicore__ inline void asc_float2float_rn(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2float_rn_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2float_rd(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2float_rd_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2float_ru(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2float_ru_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2float_rna(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2float_rna_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2float_rz(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_float2float_rz_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

// Half <-> Int conversions
/*
 * @ingroup VectorCompute
 * @brief Convert half to int4 with various rounding modes
 * @param [out] dst Destination buffer for int4 result
 * @param [in] src Source buffer with half data
 * @param [in] count Number of elements to process
 * @details Converts half precision elements to int4b_t with different rounding modes.
 *          Rounding modes: default, rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_half2int4(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int4_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int4_rna(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int4_rna_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int4_ru(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int4_ru_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int4_rd(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int4_rd_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int4_rn(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int4_rn_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int4_rz(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int4_rz_sync(__ubuf__ int4b_t* dst, __ubuf__ half* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert half to int8 with various rounding modes
 * @param [out] dst Destination buffer for int8 result
 * @param [in] src Source buffer with half data
 * @param [in] count Number of elements to process
 * @details Converts half precision elements to int8_t with different rounding modes.
 *          Rounding modes: default, rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_half2int8(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int8_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int8_rna(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int8_rna_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int8_ru(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int8_ru_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int8_rd(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int8_rd_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int8_rn(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int8_rn_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int8_rz(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int8_rz_sync(__ubuf__ int8_t* dst, __ubuf__ half* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert half to uint8 with various rounding modes
 * @param [out] dst Destination buffer for uint8 result
 * @param [in] src Source buffer with half data
 * @param [in] count Number of elements to process
 * @details Converts half precision elements to uint8_t with different rounding modes.
 *          Rounding modes: default, rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_half2uint8(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2uint8_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2uint8_rna(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2uint8_rna_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2uint8_ru(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2uint8_ru_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2uint8_rd(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2uint8_rd_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2uint8_rn(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2uint8_rn_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2uint8_rz(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2uint8_rz_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert half to int16 with various rounding modes
 * @param [out] dst Destination buffer for int16 result
 * @param [in] src Source buffer with half data
 * @param [in] count Number of elements to process
 * @details Converts half precision elements to int16_t with different rounding modes.
 *          Rounding modes: rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_half2int16_rna(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int16_rna_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int16_ru(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int16_ru_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int16_rd(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int16_rd_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int16_rn(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int16_rn_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int16_rz(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int16_rz_sync(__ubuf__ int16_t* dst, __ubuf__ half* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert half to int32 with various rounding modes
 * @param [out] dst Destination buffer for int32 result
 * @param [in] src Source buffer with half data
 * @param [in] count Number of elements to process
 * @details Converts half precision elements to int32_t with different rounding modes.
 *          Rounding modes: rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_half2int32_rna(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int32_rna_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int32_ru(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int32_ru_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int32_rd(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int32_rd_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int32_rn(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int32_rn_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int32_rz(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_half2int32_rz_sync(__ubuf__ int32_t* dst, __ubuf__ half* src, uint32_t count);

// Int <-> Half/Float conversions
/*
 * @ingroup VectorCompute
 * @brief Convert int4 to half precision
 * @param [out] dst Destination buffer for half result
 * @param [in] src Source buffer with int4 data
 * @param [in] count Number of elements to process
 * @details Converts int4b_t elements to half precision.
 */
__aicore__ inline void asc_int42half(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint32_t count);
__aicore__ inline void asc_int42half_sync(__ubuf__ half* dst, __ubuf__ int4b_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert int8 to half precision
 * @param [out] dst Destination buffer for half result
 * @param [in] src Source buffer with int8 data
 * @param [in] count Number of elements to process
 * @details Converts int8_t elements to half precision.
 */
__aicore__ inline void asc_int82half(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count);
__aicore__ inline void asc_int82half_sync(__ubuf__ half* dst, __ubuf__ int8_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert uint8 to half precision
 * @param [out] dst Destination buffer for half result
 * @param [in] src Source buffer with uint8 data
 * @param [in] count Number of elements to process
 * @details Converts uint8_t elements to half precision.
 */
__aicore__ inline void asc_uint82half(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count);
__aicore__ inline void asc_uint82half_sync(__ubuf__ half* dst, __ubuf__ uint8_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert int16 to half precision with various rounding modes
 * @param [out] dst Destination buffer for half result
 * @param [in] src Source buffer with int16 data
 * @param [in] count Number of elements to process
 * @details Converts int16_t elements to half precision with different rounding modes.
 *          Rounding modes: default, rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_int162half(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_int162half_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_int162half_rna(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_int162half_rna_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_int162half_ru(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_int162half_ru_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_int162half_rd(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_int162half_rd_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_int162half_rn(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_int162half_rn_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_int162half_rz(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_int162half_rz_sync(__ubuf__ half* dst, __ubuf__ int16_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert int16 to float precision
 * @param [out] dst Destination buffer for float result
 * @param [in] src Source buffer with int16 data
 * @param [in] count Number of elements to process
 * @details Converts int16_t elements to single-precision float.
 */
__aicore__ inline void asc_int162float(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_int162float_sync(__ubuf__ float* dst, __ubuf__ int16_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert int32 to float precision with various rounding modes
 * @param [out] dst Destination buffer for float result
 * @param [in] src Source buffer with int32 data
 * @param [in] count Number of elements to process
 * @details Converts int32_t elements to single-precision float with different rounding modes.
 *          Rounding modes: default, rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_int322float(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322float_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322float_rna(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322float_rna_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322float_ru(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322float_ru_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322float_rd(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322float_rd_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322float_rn(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322float_rn_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322float_rz(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322float_rz_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert int64 to float precision with various rounding modes
 * @param [out] dst Destination buffer for float result
 * @param [in] src Source buffer with int64 data
 * @param [in] count Number of elements to process
 * @details Converts int64_t elements to single-precision float with different rounding modes.
 *          Rounding modes: rna, ru, rd, rn, rz.
 */
__aicore__ inline void asc_int642float_rna(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);
__aicore__ inline void asc_int642float_rna_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);
__aicore__ inline void asc_int642float_ru(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);
__aicore__ inline void asc_int642float_ru_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);
__aicore__ inline void asc_int642float_rd(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);
__aicore__ inline void asc_int642float_rd_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);
__aicore__ inline void asc_int642float_rn(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);
__aicore__ inline void asc_int642float_rn_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);
__aicore__ inline void asc_int642float_rz(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);
__aicore__ inline void asc_int642float_rz_sync(__ubuf__ float* dst, __ubuf__ int64_t* src, uint32_t count);

// Int <-> Int conversions
/*
 * @ingroup VectorCompute
 * @brief Convert int32 to int16
 * @param [out] dst Destination buffer for int16 result
 * @param [in] src Source buffer with int32 data
 * @param [in] count Number of elements to process
 * @details Converts int32_t elements to int16_t.
 */
__aicore__ inline void asc_int322int16(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322int16_sync(__ubuf__ int16_t* dst, __ubuf__ int32_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert int32 to int64
 * @param [out] dst Destination buffer for int64 result
 * @param [in] src Source buffer with int32 data
 * @param [in] count Number of elements to process
 * @details Converts int32_t elements to int64_t.
 */
__aicore__ inline void asc_int322int64(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_int322int64_sync(__ubuf__ int64_t* dst, __ubuf__ int32_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Convert int64 to int32
 * @param [out] dst Destination buffer for int32 result
 * @param [in] src Source buffer with int64 data
 * @param [in] count Number of elements to process
 * @details Converts int64_t elements to int32_t.
 */
__aicore__ inline void asc_int642int32(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count);
__aicore__ inline void asc_int642int32_sync(__ubuf__ int32_t* dst, __ubuf__ int64_t* src, uint32_t count);

// Dequantization operations
/*
 * @ingroup VectorCompute
 * @brief Dequantize int32 to half
 * @param [out] dst Destination buffer for half result
 * @param [in] src Source buffer with int32 data
 * @param [in] count Number of elements to process
 * @details Dequantizes int32_t elements to half precision.
 */
__aicore__ inline void asc_deq_int322half(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_deq_int322half_sync(__ubuf__ half* dst, __ubuf__ int32_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Dequantize int16 to int8 (high 8 bits)
 * @param [out] dst Destination buffer for int8/uint8 result
 * @param [in] src Source buffer with int16 data
 * @param [in] count Number of elements to process
 * @details Extracts high 8 bits from int16_t, outputs as int8_t or uint8_t.
 */
__aicore__ inline void asc_deq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_deq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Dequantize int16 to int8 (low 8 bits)
 * @param [out] dst Destination buffer for int8/uint8 result
 * @param [in] src Source buffer with int16 data
 * @param [in] count Number of elements to process
 * @details Extracts low 8 bits from int16_t, outputs as int8_t or uint8_t.
 */
__aicore__ inline void asc_deq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_deq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Vector dequantize int16 to int8 (high 8 bits)
 * @param [out] dst Destination buffer for int8/uint8 result
 * @param [in] src Source buffer with int16 data
 * @param [in] count Number of elements to process
 * @details Extracts high 8 bits from int16_t, outputs as int8_t or uint8_t.
 */
__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_vdeq_int162b8_h(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_vdeq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Vector dequantize int16 to int8 (low 8 bits)
 * @param [out] dst Destination buffer for int8/uint8 result
 * @param [in] src Source buffer with int16 data
 * @param [in] count Number of elements to process
 * @details Extracts low 8 bits from int16_t, outputs as int8_t or uint8_t.
 */
__aicore__ inline void asc_vdeq_int162b8_l(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_vdeq_int162b8_l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_vdeq_int162b8_l(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_vdeq_int162b8_l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count);

// ============================================================================
// Mathematical Functions - Standard math operations
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Exponential function for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Computes exponential: dst[i] = exp(src[i]). Supports half and float data types.
 */
__aicore__ inline void asc_exp(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_exp_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_exp(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_exp_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Natural logarithm for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Computes natural logarithm: dst[i] = log(src[i]). Supports half and float data types.
 */
__aicore__ inline void asc_log(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_log_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_log(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_log_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Absolute value for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Computes absolute value: dst[i] = |src[i]|. Supports half and float data types.
 */
__aicore__ inline void asc_abs(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_abs_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_abs(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_abs_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief ReLU activation function for half/float/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Computes ReLU: dst[i] = max(0, src[i]). Supports half, float, and int32_t data types.
 */
__aicore__ inline void asc_relu(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_relu_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_relu(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_relu_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_relu(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_relu_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Maximum of two vectors for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Computes element-wise maximum: dst[i] = max(src0[i], src1[i]).
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_max(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_max_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_max(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_max_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_max(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_max_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_max(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);
__aicore__ inline void asc_max_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Maximum with scalar for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Scalar value
 * @param [in] count Number of elements to process
 * @details Computes element-wise maximum with scalar: dst[i] = max(src[i], value).
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_max_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_max_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_max_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_max_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_max_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);
__aicore__ inline void asc_max_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);
__aicore__ inline void asc_max_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);
__aicore__ inline void asc_max_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Minimum of two vectors for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Computes element-wise minimum: dst[i] = min(src0[i], src1[i]).
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_min(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_min_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_min(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_min_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_min(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_min_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_min(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);
__aicore__ inline void asc_min_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Minimum with scalar for half/float/int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Scalar value
 * @param [in] count Number of elements to process
 * @details Computes element-wise minimum with scalar: dst[i] = min(src[i], value).
 *          Supports half, float, int16_t, int32_t data types.
 */
__aicore__ inline void asc_min_scalar(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_min_scalar_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_min_scalar(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_min_scalar_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_min_scalar(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);
__aicore__ inline void asc_min_scalar_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t value, uint32_t count);
__aicore__ inline void asc_min_scalar(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);
__aicore__ inline void asc_min_scalar_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);
/*
 * @ingroup VectorCompute
 * @brief Reciprocal (1/x) for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Computes reciprocal: dst[i] = 1/src[i]. Supports half and float data types.
 */
__aicore__ inline void asc_rcp(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_rcp(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Reciprocal square root (1/sqrt(x)) for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Computes reciprocal square root: dst[i] = 1/sqrt(src[i]).
 *          Supports half and float data types.
 */
__aicore__ inline void asc_rsqrt(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_rsqrt(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Square root for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Computes square root: dst[i] = sqrt(src[i]). Supports half and float data types.
 */
__aicore__ inline void asc_sqrt(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_sqrt_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count);
__aicore__ inline void asc_sqrt(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);
__aicore__ inline void asc_sqrt_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Leaky ReLU activation function for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Leaky slope parameter
 * @param [in] count Number of elements to process
 * @details Computes Leaky ReLU: dst[i] = src[i] if src[i] > 0, else value * src[i].
 *          Supports half and float data types.
 */
__aicore__ inline void asc_leakyrelu(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_leakyrelu_sync(__ubuf__ half* dst, __ubuf__ half* src, half value, uint32_t count);
__aicore__ inline void asc_leakyrelu(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);
__aicore__ inline void asc_leakyrelu_sync(__ubuf__ float* dst, __ubuf__ float* src, float value, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Subtraction followed by ReLU for half/float/int16 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Computes: dst[i] = relu(src0[i] - src1[i]). Supports half, float, int16_t data types.
 */
__aicore__ inline void asc_sub_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_sub_relu_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_sub_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_sub_relu_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_sub_relu(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_sub_relu_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_sub_relu(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_sub_relu_sync(__ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_sub_relu(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_sub_relu_sync(__ubuf__ half* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_sub_relu(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_sub_relu_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Addition followed by ReLU for half/float/int16 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Computes: dst[i] = relu(src0[i] + src1[i]). Supports half, float, int16_t data types.
 */
__aicore__ inline void asc_add_relu(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_add_relu_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_add_relu(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_add_relu_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);
__aicore__ inline void asc_add_relu(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_add_relu_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);

// ============================================================================
// Data Manipulation Operations - Duplicate, copy, gather, enable_transpose operations
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Fill destination buffer with a scalar value for half/int16/uint16/bfloat16/float/int32/uint32 data types
 * @param [out] dst Destination buffer to fill
 * @param [in] src Scalar value to duplicate
 * @param [in] count Number of elements to fill
 * @details Fills destination buffer with scalar value: dst[i] = src.
 *          Supports half, int16_t, uint16_t, bfloat16_t, float, int32_t, uint32_t data types.
 */
__aicore__ inline void asc_duplicate(__ubuf__ half* dst, half src, uint32_t count);
__aicore__ inline void asc_duplicate_sync(__ubuf__ half* dst, half src, uint32_t count);
__aicore__ inline void asc_duplicate(__ubuf__ int16_t* dst, int16_t src, uint32_t count);
__aicore__ inline void asc_duplicate_sync(__ubuf__ int16_t* dst, int16_t src, uint32_t count);
__aicore__ inline void asc_duplicate(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count);
__aicore__ inline void asc_duplicate_sync(__ubuf__ uint16_t* dst, uint16_t src, uint32_t count);
__aicore__ inline void asc_duplicate(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count);
__aicore__ inline void asc_duplicate_sync(__ubuf__ bfloat16_t* dst, bfloat16_t src, uint32_t count);
__aicore__ inline void asc_duplicate(__ubuf__ float* dst, float src, uint32_t count);
__aicore__ inline void asc_duplicate_sync(__ubuf__ float* dst, float src, uint32_t count);
__aicore__ inline void asc_duplicate(__ubuf__ int32_t* dst, int32_t src, uint32_t count);
__aicore__ inline void asc_duplicate_sync(__ubuf__ int32_t* dst, int32_t src, uint32_t count);
__aicore__ inline void asc_duplicate(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count);
__aicore__ inline void asc_duplicate_sync(__ubuf__ uint32_t* dst, uint32_t src, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Copy data from source to destination buffer for int16/uint16/int32/uint32 data types
 * @param [out] dst Destination buffer
 * @param [in] src Source buffer
 * @param [in] count Number of elements to copy
 * @details Copies data from source to destination: dst[i] = src[i].
 *          Supports int16_t, uint16_t, int32_t, uint32_t data types.
 */
__aicore__ inline void asc_copy(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_copy(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count);
__aicore__ inline void asc_copy(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t count);
__aicore__ inline void asc_copy(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t count);

// ============================================================================
// Bit Operations - Bitwise and shift operations
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Left shift operation for int16/uint16/int32/uint32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] distance Number of bits to shift left
 * @param [in] count Number of elements to process
 * @details Performs left shift: dst[i] = src[i] << distance.
 *          Supports int16_t, uint16_t, int32_t, uint32_t data types.
 */
__aicore__ inline void asc_shiftleft(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint32_t count);
__aicore__ inline void asc_shiftleft_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t distance, uint32_t count);
__aicore__ inline void asc_shiftleft(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint32_t count);
__aicore__ inline void asc_shiftleft_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t distance, uint32_t count);
__aicore__ inline void asc_shiftleft(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint32_t count);
__aicore__ inline void asc_shiftleft_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint32_t distance, uint32_t count);
__aicore__ inline void asc_shiftleft(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint32_t count);
__aicore__ inline void asc_shiftleft_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t distance, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Right shift operation for int16/int32/uint16/uint32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Number of bits to shift right
 * @param [in] count Number of elements to process
 * @details Performs right shift: dst[i] = src[i] >> value.
 *          Supports int16_t, int32_t (arithmetic shift), uint16_t, uint32_t (logical shift) data types.
 */
__aicore__ inline void asc_shiftright(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count);
__aicore__ inline void asc_shiftright(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);
__aicore__ inline void asc_shiftright(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t value, uint32_t count);
__aicore__ inline void asc_shiftright(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t value, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Right shift with rounding for int16/int32 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] value Number of bits to shift right
 * @param [in] count Number of elements to process
 * @details Performs right shift with rounding: dst[i] = round(src[i] >> value).
 *          Supports int16_t and int32_t data types.
 */
__aicore__ inline void asc_shiftright_round(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count);
__aicore__ inline void asc_shiftright_round_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, int32_t value, uint32_t count);
__aicore__ inline void asc_shiftright_round(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);
__aicore__ inline void asc_shiftright_round_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t value, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Bitwise AND operation for int16/uint16 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Performs bitwise AND: dst[i] = src0[i] & src1[i].
 *          Supports int16_t and uint16_t data types.
 */
__aicore__ inline void asc_and(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_and_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_and(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);
__aicore__ inline void asc_and_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Bitwise OR operation for int16/uint16 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Performs bitwise OR: dst[i] = src0[i] | src1[i].
 *          Supports int16_t and uint16_t data types.
 */
__aicore__ inline void asc_or(__ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint32_t count);
__aicore__ inline void asc_or(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Bitwise NOT operation for int16/uint16 data types
 * @param [out] dst Destination buffer for result
 * @param [in] src Source buffer
 * @param [in] count Number of elements to process
 * @details Performs bitwise NOT: dst[i] = ~src[i].
 *          Supports int16_t and uint16_t data types.
 */
__aicore__ inline void asc_not(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint32_t count);
__aicore__ inline void asc_not(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t count);

// ============================================================================
// Sorting and Selection Operations
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief 4-way merge sort for half/float data types
 * @param [out] dst Destination buffer for sorted result
 * @param [in] src Array of 4 source buffers to merge and sort
 * @param [in] count Number of elements to process
 * @details Performs 4-way merge sort, merging 4 sorted input arrays into one sorted output array.
 *          Supports half and float data types.
 */
__aicore__ inline void asc_mrgsort4(__ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint32_t count);
__aicore__ inline void asc_mrgsort4_sync(__ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint32_t count);
__aicore__ inline void asc_mrgsort4(__ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint32_t count);
__aicore__ inline void asc_mrgsort4_sync(__ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief Select elements from two sources for half/float data types
 * @param [out] dst Destination buffer for result
 * @param [in] src0 First source buffer
 * @param [in] src1 Second source buffer
 * @param [in] count Number of elements to process
 * @details Selects elements based on mask: dst[i] = mask[i] ? src1[i] : src0[i].
 *          Supports half and float data types.
 */
__aicore__ inline void asc_select(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count);
__aicore__ inline void asc_select(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint32_t count);

// ============================================================================
// Gather and enable_transpose Operations
// ============================================================================

/*
 * @ingroup VectorCompute
 * @brief Gather elements from source using offset indices for int16/uint16/half/bfloat16/int32/uint32/float data types
 * @param [out] dst Destination buffer for gathered result
 * @param [in] src Source buffer
 * @param [in] src_offset Buffer containing gather indices
 * @param [in] count Number of elements to gather
 * @details Gathers elements using indices: dst[i] = src[src_offset[i]].
 *          Supports int16_t, uint16_t, half, bfloat16_t, int32_t, uint32_t, float data types.
 */
__aicore__ inline void asc_gather(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather(__ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather_sync(__ubuf__ half* dst, __ubuf__ half* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather(__ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather_sync(__ubuf__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather_sync(__ubuf__ int32_t* dst, __ubuf__ int32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather(__ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset, uint32_t count);
__aicore__ inline void asc_gather_sync(__ubuf__ float* dst, __ubuf__ float* src, __ubuf__ uint32_t* src_offset, uint32_t count);

/*
 * @ingroup VectorCompute
 * @brief enable_transpose matrix for int16/uint16 data types
 * @param [out] dst Destination buffer for transposed result
 * @param [in] src Source buffer
 * @details Transposes the input matrix. Supports int16_t and uint16_t data types.
 */
__aicore__ inline void asc_transpose(__ubuf__ int16_t* dst, __ubuf__ int16_t* src);
__aicore__ inline void asc_transpose_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src);
__aicore__ inline void asc_transpose(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src);
__aicore__ inline void asc_transpose_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif
