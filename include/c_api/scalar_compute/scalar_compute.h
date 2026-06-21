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
 * @file scalar_compute.h
 * @brief This file provides scalar compute interfaces for bit operations and type conversions.
 * @details These interfaces support various bit manipulation operations including bit counting,
 *          bit searching, and bit setting/clearing. Also provides float to integer conversion
 *          operations with different rounding modes and device memory store operations.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_SCALAR_COMPUTE_H
#define INCLUDE_C_API_SCALAR_COMPUTE_H

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

#include "../../../impl/c_api/instr_impl/npu_arch_2201/scalar_compute_impl.h"

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

#include "../../../impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl.h"

#endif

/*
 * @ingroup ScalarCompute
 * @brief Count sign leading bits
 * @param [in] value Input value to analyze
 * @return Number of sign leading bits (bits with same value as sign bit)
 * @details Counts the number of leading bits that have the same value as the sign bit.
 *          For positive numbers, this counts leading zeros; for negative numbers,
 *          this counts leading ones.
 */
__aicore__ inline int64_t asc_sflbits(int64_t value);

/*
 * @ingroup ScalarCompute
 * @brief Find first zero bit
 * @param [in] value Input value to search
 * @return Position of the first zero bit (0-based index), or -1 if all bits are 1
 * @details Searches for the first zero bit (bit value 0) from least significant bit (LSB)
 *          to most significant bit (MSB) and returns its position.
 */
__aicore__ inline int64_t asc_ffz(uint64_t value);

/*
 * @ingroup ScalarCompute
 * @brief Find first set bit
 * @param [in] value Input value to search
 * @return Position of the first set bit (1-based index), or 0 if no bits are set
 * @details Searches for the first set bit (bit value 1) from least significant bit (LSB)
 *          to most significant bit (MSB) and returns its position (1-based).
 */
__aicore__ inline int64_t asc_ffs(uint64_t value);

/*
 * @ingroup ScalarCompute
 * @brief Population count (count set bits)
 * @param [in] value Input value to count
 * @return Number of bits set to 1 in the input value
 * @details Counts the total number of bits that are set to 1 in the input value.
 *          Also known as "Hamming weight" or "bit count".
 */
__aicore__ inline int64_t asc_popc(uint64_t value);

/*
 * @ingroup ScalarCompute
 * @brief Clear nth bit
 * @param [in] bits Input value to modify
 * @param [in] idx Bit position to clear (0-based index)
 * @return Modified value with the specified bit cleared to 0
 * @details Clears (sets to 0) the bit at the specified position in the input value.
 *          The position is 0-based, starting from the least significant bit.
 */
__aicore__ inline uint64_t asc_clear_nthbit(uint64_t bits, int64_t idx);

/*
 * @ingroup ScalarCompute
 * @brief Set nth bit
 * @param [in] bits Input value to modify
 * @param [in] idx Bit position to set (0-based index)
 * @return Modified value with the specified bit set to 1
 * @details Sets (sets to 1) the bit at the specified position in the input value.
 *          The position is 0-based, starting from the least significant bit.
 */
__aicore__ inline uint64_t asc_set_nthbit(uint64_t bits, int64_t idx);

/*
 * @ingroup ScalarCompute
 * @brief Count leading zeros
 * @param [in] value_in Input value to analyze
 * @return Number of leading zero bits before the first set bit
 * @details Counts the number of leading zero bits (from most significant bit)
 *          before encountering the first bit set to 1. Returns 64 if input is 0.
 */
__aicore__ inline int64_t asc_clz(uint64_t value_in);

/*
 * @ingroup ScalarCompute
 * @brief Count zero bits
 * @param [in] value Input value to count
 * @return Total number of zero bits in the input value
 * @details Counts the total number of bits that are set to 0 in the input value.
 *          Equivalent to (64 - popc(value)) for 64-bit values.
 */
__aicore__ inline int64_t asc_zero_bits_cnt(uint64_t value);

/*
 * @ingroup ScalarCompute
 * @brief Convert float to int32 with round down mode
 * @param [in] value Float value to convert
 * @return Integer result after rounding down
 * @details Converts a float value to int32 using round-down (floor) mode.
 *          Always rounds towards negative infinity.
 */
__aicore__ inline int32_t asc_float2int32_rd(float value);

/*
 * @ingroup ScalarCompute
 * @brief Convert float to int32 with round up mode
 * @param [in] value Float value to convert
 * @return Integer result after rounding up
 * @details Converts a float value to int32 using round-up (ceil) mode.
 *          Always rounds towards positive infinity.
 */
__aicore__ inline int32_t asc_float2int32_ru(float value);

/*
 * @ingroup ScalarCompute
 * @brief Convert float to int32 with round to nearest mode
 * @param [in] value Float value to convert
 * @return Integer result after rounding to nearest
 * @details Converts a float value to int32 using round-to-nearest mode.
 *          Rounds to the nearest integer value.
 */
__aicore__ inline int32_t asc_float2int32_rn(float value);

/*
 * @ingroup ScalarCompute
 * @brief Convert float to int32 with round to nearest away from zero mode
 * @param [in] value Float value to convert
 * @return Integer result after rounding to nearest away from zero
 * @details Converts a float value to int32 using round-to-nearest-away-from-zero mode.
 *          Rounds to the nearest integer, and when exactly halfway, rounds away from zero.
 */
__aicore__ inline int32_t asc_float2int32_rna(float value);

/*
 * @ingroup ScalarCompute
 * @brief Store int8 value to device memory
 * @param [in] addr Device memory address to store
 * @param [in] value Value to store
 * @details Stores an int8 value to the specified device memory address (Global Memory).
 */
__aicore__ inline void asc_store_dev(__gm__ int8_t* addr, int8_t value);

__aicore__ inline void asc_store_dev(__gm__ uint8_t* addr, uint8_t value);

__aicore__ inline void asc_store_dev(__gm__ int16_t* addr, int16_t value);

__aicore__ inline void asc_store_dev(__gm__ uint16_t* addr, uint16_t value);

__aicore__ inline void asc_store_dev(__gm__ int32_t* addr, int32_t value);

__aicore__ inline void asc_store_dev(__gm__ uint32_t* addr, uint32_t value);

__aicore__ inline void asc_store_dev(__gm__ int64_t* addr, int64_t value);

__aicore__ inline void asc_store_dev(__gm__ uint64_t* addr, uint64_t value);

namespace __asc_aicore {

__aicore__ inline int32_t asc_atomic_add(__gm__ int32_t* address, int32_t val);

__aicore__ inline uint32_t asc_atomic_add(__gm__ uint32_t* address, uint32_t val);

__aicore__ inline float asc_atomic_add(__gm__ float* address, float val);

__aicore__ inline int64_t asc_atomic_add(__gm__ int64_t* address, int64_t val);

__aicore__ inline uint64_t asc_atomic_add(__gm__ uint64_t* address, uint64_t val);

__aicore__ inline int32_t asc_atomic_sub(__gm__ int32_t* address, int32_t val);

__aicore__ inline uint32_t asc_atomic_sub(__gm__ uint32_t* address, uint32_t val);

__aicore__ inline float asc_atomic_sub(__gm__ float* address, float val);

__aicore__ inline int64_t asc_atomic_sub(__gm__ int64_t* address, int64_t val);

__aicore__ inline uint64_t asc_atomic_sub(__gm__ uint64_t* address, uint64_t val);

__aicore__ inline int32_t asc_atomic_exch(__gm__ int32_t* address, int32_t val);

__aicore__ inline uint32_t asc_atomic_exch(__gm__ uint32_t* address, uint32_t val);

__aicore__ inline float asc_atomic_exch(__gm__ float* address, float val);

__aicore__ inline int64_t asc_atomic_exch(__gm__ int64_t* address, int64_t val);

__aicore__ inline uint64_t asc_atomic_exch(__gm__ uint64_t* address, uint64_t val);

__aicore__ inline int32_t asc_atomic_max(__gm__ int32_t* address, int32_t val);

__aicore__ inline uint32_t asc_atomic_max(__gm__ uint32_t* address, uint32_t val);

__aicore__ inline float asc_atomic_max(__gm__ float* address, float val);

__aicore__ inline int64_t asc_atomic_max(__gm__ int64_t* address, int64_t val);

__aicore__ inline uint64_t asc_atomic_max(__gm__ uint64_t* address, uint64_t val);

__aicore__ inline int32_t asc_atomic_min(__gm__ int32_t* address, int32_t val);

__aicore__ inline uint32_t asc_atomic_min(__gm__ uint32_t* address, uint32_t val);

__aicore__ inline float asc_atomic_min(__gm__ float* address, float val);

__aicore__ inline int64_t asc_atomic_min(__gm__ int64_t* address, int64_t val);

__aicore__ inline uint64_t asc_atomic_min(__gm__ uint64_t* address, uint64_t val);

__aicore__ inline int32_t asc_atomic_cas(__gm__ int32_t* address, int32_t compare, int32_t val);

__aicore__ inline uint32_t asc_atomic_cas(__gm__ uint32_t* address, uint32_t compare, uint32_t val);

__aicore__ inline float asc_atomic_cas(__gm__ float* address, float compare, float val);

__aicore__ inline int64_t asc_atomic_cas(__gm__ int64_t* address, int64_t compare, int64_t val);

__aicore__ inline uint64_t asc_atomic_cas(__gm__ uint64_t* address, uint64_t compare, uint64_t val);

__aicore__ inline int32_t asc_atomic_and(__gm__ int32_t* address, int32_t val);

__aicore__ inline uint32_t asc_atomic_and(__gm__ uint32_t* address, uint32_t val);

__aicore__ inline int64_t asc_atomic_and(__gm__ int64_t* address, int64_t val);

__aicore__ inline uint64_t asc_atomic_and(__gm__ uint64_t* address, uint64_t val);

__aicore__ inline int32_t asc_atomic_or(__gm__ int32_t* address, int32_t val);

__aicore__ inline uint32_t asc_atomic_or(__gm__ uint32_t* address, uint32_t val);

__aicore__ inline int64_t asc_atomic_or(__gm__ int64_t* address, int64_t val);

__aicore__ inline uint64_t asc_atomic_or(__gm__ uint64_t* address, uint64_t val);

__aicore__ inline int32_t asc_atomic_xor(__gm__ int32_t* address, int32_t val);

__aicore__ inline uint32_t asc_atomic_xor(__gm__ uint32_t* address, uint32_t val);

__aicore__ inline int64_t asc_atomic_xor(__gm__ int64_t* address, int64_t val);

__aicore__ inline uint64_t asc_atomic_xor(__gm__ uint64_t* address, uint64_t val);

__aicore__ inline uint32_t asc_atomic_inc(__gm__ uint32_t* address, uint32_t val);

__aicore__ inline uint64_t asc_atomic_inc(__gm__ uint64_t* address, uint64_t val);

__aicore__ inline uint32_t asc_atomic_dec(__gm__ uint32_t* address, uint32_t val);

__aicore__ inline uint64_t asc_atomic_dec(__gm__ uint64_t* address, uint64_t val);

}

#endif // INCLUDE_C_API_SCALAR_COMPUTE_H

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif
