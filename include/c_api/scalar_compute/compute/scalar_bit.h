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
#include "impl/c_api/instr_impl/npu_arch_2201/scalar_compute_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/scalar_compute_impl.h"
#endif

/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @override_id asc_sflbits__i64
 * @archs 3510 2201
 * @brief 对一个64位有符号整数，取其最高位（即符号位）作为参考位，统计从次高位（即最高数值位）开始与符号位相同的连续比特位的个数。
 * @param [in] value 待统计的64位有符号整数，取值范围为[-2^63, 2^63-1]。
 */
__aicore__ inline int64_t asc_sflbits(int64_t value);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @override_id asc_ffz__u64
 * @archs 3510 2201
 * @brief FindFirstZero接口，用于在输入`uint64_t`数据的二进制表示中，从最低位向最高位查找第一个值为0的比特，并返回其位索引；若未找到，则返回-1。
 * @param [in] value 输入数据。数据类型为`uint64_t`，取值范围为[0, 2^64-1]。
 */
__aicore__ inline int64_t asc_ffz(uint64_t value);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @override_id asc_ffs__u64
 * @archs 3510 2201
 * @brief FindFirstSet接口，用于在输入`uint64_t`数据的二进制表示中，从最低位向最高位查找第一个值为1的比特，并返回其位索引；若未找到，则返回-1。
 * @param [in] value 输入数据。数据类型为`uint64_t`，取值范围为[0, 2^64-1]。
 */
__aicore__ inline int64_t asc_ffs(uint64_t value);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @override_id asc_popc__u64
 * @archs 3510 2201
 * @brief 获取输入`uint64_t`数据的二进制表示中值为1的位数。
 * @param [in] value 待统计的64位无符号整数。取值范围为[0, 2^64-1]。
 */
__aicore__ inline int64_t asc_popc(uint64_t value);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @override_id asc_clz__u64
 * @archs 3510 2201
 * @brief 计算一个`uint64_t`类型整数在二进制表示下的前导零个数，即从二进制最高位开始，到第一个出现二进制1为止，中间连续的0的数量。
 * @param [in] value_in 待统计的64位无符号整数。取值范围为[0, 2^64-1]。
 */
__aicore__ inline int64_t asc_clz(uint64_t value_in);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @override_id asc_zero_bits_cnt__u64
 * @archs 3510 2201
 * @brief 获取输入`uint64_t`数据的二进制表示中值为0的位数。
 * @param [in] value 待统计的64位无符号整数，取值范围为[0, 2^64-1]。
 */
__aicore__ inline int64_t asc_zero_bits_cnt(uint64_t value);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @override_id asc_set_nthbit__u64_i64
 * @archs 3510 2201
 * @brief 将`uint64_t`数据的指定二进制位置为1，其余位保持不变。返回修改后的`uint64_t`整数。
 * @param [in] bits 待置位的64位无符号整数，取值范围为[0, 2^64-1]。执行此计算后输入数据不变。
 * @param [in] idx 位索引，指定需要设置为1的二进制位的位置。实际位索引按`(idx % 64 + 64) % 64`计算，索引范围为[0, 63]。
 */
__aicore__ inline uint64_t asc_set_nthbit(uint64_t bits, int64_t idx);
/**
 * @ingroup simd_c_api.scalar_compute.scalar_bit
 * @override_id asc_clear_nthbit__u64_i64
 * @archs 3510 2201
 * @brief 对一个64位无符号整数执行位清除操作，将`bits`中第`idx`位清零，其余位保持不变。返回修改后的64位无符号整数。
 * @param [in] bits 64位无符号整数。取值范围为[0，2^64−1]。
 * @param [in] idx 位索引。实际位索引按`(idx % 64 + 64) % 64`计算，范围为[0，63]。
 */
__aicore__ inline uint64_t asc_clear_nthbit(uint64_t bits, int64_t idx);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SCALAR_COMPUTE_COMPUTE_SCALAR_BIT_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_SCALAR_COMPUTE_COMPUTE_SCALAR_BIT_H
#endif
