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
#error "scalar_bit.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SCALAR_COMPUTE_COMPUTE_SCALAR_BIT_H
#endif

#ifndef INCLUDE_C_API_SCALAR_COMPUTE_COMPUTE_SCALAR_BIT_H
#define INCLUDE_C_API_SCALAR_COMPUTE_COMPUTE_SCALAR_BIT_H

#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/scalar_bit_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/scalar_bit_intf_impl.h"
#endif

/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @brief Counts consecutive bits from the highest value bit that equal the sign bit of a 64-bit signed integer, returning -1 for all-zero/all-one input.
 *
 * @override_id asc_sflbits__i64
 * @archs 2201 3510
 * @param [in] value 待统计的64位有符号整数，取值范围为[-2^63, 2^63-1]。
 */
__aicore__ inline int64_t asc_sflbits(int64_t value);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @brief Finds the first bit set to 0 from the least significant bit of a uint64_t and returns its bit index, or -1 when no zero bit exists.
 *
 * @override_id asc_ffz__u64
 * @archs 2201 3510
 * @param [in] value 输入数据。数据类型为`uint64_t`，取值范围为[0, 2^64-1]。
 */
__aicore__ inline int64_t asc_ffz(uint64_t value);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @brief Finds the first bit set to 1 from the least significant bit of a uint64_t and returns its bit index, or -1 when no such bit exists.
 *
 * @override_id asc_ffs__u64
 * @archs 2201 3510
 * @param [in] value 输入数据。数据类型为`uint64_t`，取值范围为[0, 2^64-1]。
 */
__aicore__ inline int64_t asc_ffs(uint64_t value);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @brief Returns the number of bits set to 1 in the binary representation of the input uint64_t value, in the range [0, 64].
 *
 * @override_id asc_popc__u64
 * @archs 2201 3510
 * @param [in] value 待统计的64位无符号整数。取值范围为[0, 2^64-1]。
 */
__aicore__ inline int64_t asc_popc(uint64_t value);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @brief Returns the number of leading zero bits in the binary form of a uint64_t, from the MSB to the first 1, and 64 when the input is 0.
 *
 * @override_id asc_clz__u64
 * @archs 2201 3510
 * @param [in] value_in 待统计的64位无符号整数。取值范围为[0, 2^64-1]。
 */
__aicore__ inline int64_t asc_clz(uint64_t value_in);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @brief Returns the number of bits set to 0 in the binary representation of the input uint64_t value, in the range [0, 64].
 *
 * @override_id asc_zero_bits_cnt__u64
 * @archs 2201 3510
 * @param [in] value 待统计的64位无符号整数，取值范围为[0, 2^64-1]。
 */
__aicore__ inline int64_t asc_zero_bits_cnt(uint64_t value);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @brief Sets the bit at index idx of the uint64_t value to 1, leaving other bits unchanged, and returns the modified uint64_t.
 *
 * @override_id asc_set_nthbit__u64_i64
 * @archs 2201 3510
 * @param [in] bits 待置位的64位无符号整数，取值范围为[0, 2^64-1]。执行此计算后输入数据不变。
 * @param [in] idx 位索引，指定需要设置为1的二进制位的位置。实际位索引按`(idx % 64 + 64) % 64`计算，索引范围为[0, 63]。
 */
__aicore__ inline uint64_t asc_set_nthbit(uint64_t bits, int64_t idx);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @brief Clears bit idx of the 64-bit unsigned integer bits, leaving other bits unchanged, and returns the modified uint64_t value.
 *
 * @override_id asc_clear_nthbit__u64_i64
 * @archs 2201 3510
 * @param [in] bits 64位无符号整数。取值范围为[0，2^64−1]。
 * @param [in] idx 位索引。实际位索引按`(idx % 64 + 64) % 64`计算，范围为[0，63]。
 */
__aicore__ inline uint64_t asc_clear_nthbit(uint64_t bits, int64_t idx);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SCALAR_COMPUTE_COMPUTE_SCALAR_BIT_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SCALAR_COMPUTE_COMPUTE_SCALAR_BIT_H
#endif
