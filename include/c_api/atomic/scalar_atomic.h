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
#error "scalar_atomic.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ATOMIC_SCALAR_ATOMIC_H
#endif

#ifndef INCLUDE_C_API_ATOMIC_SCALAR_ATOMIC_H
#define INCLUDE_C_API_ATOMIC_SCALAR_ATOMIC_H

#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/scalar_atomic_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/scalar_atomic_intf_impl.h"
#endif

namespace __asc_aicore {
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically adds the int32_t val to the element at the Global Memory address and returns the pre-update old value; read, add and write-back are one atomic operation.
 *
 * @override_id asc_atomic_add__PGi32_i32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline int32_t asc_atomic_add(__gm__ int32_t* address, int32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically adds the uint32_t val to the element at the Global Memory address and returns the pre-update old value; read, add and write-back are one atomic operation.
 *
 * @override_id asc_atomic_add__PGu32_u32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline uint32_t asc_atomic_add(__gm__ uint32_t* address, uint32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically adds the float val to the element at the Global Memory address and returns the pre-update old value; read, add and write-back are one atomic operation.
 *
 * @override_id asc_atomic_add__PGf32_f32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline float asc_atomic_add(__gm__ float* address, float val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically adds the int64_t val to the element at the Global Memory address and returns the pre-update old value; read, add and write-back are one atomic operation.
 *
 * @override_id asc_atomic_add__PGi64_i64
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline int64_t asc_atomic_add(__gm__ int64_t* address, int64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically adds the uint64_t val to the element at the Global Memory address and returns the pre-update old value; read, add and write-back are one atomic operation.
 *
 * @override_id asc_atomic_add__PGu64_u64
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline uint64_t asc_atomic_add(__gm__ uint64_t* address, uint64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores old_value minus val into the int32_t element at the Global Memory address and returns the pre-update old value; the operation is atomic.
 *
 * @override_id asc_atomic_sub__PGi32_i32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline int32_t asc_atomic_sub(__gm__ int32_t* address, int32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores old_value minus val into the uint32_t element at the Global Memory address and returns the pre-update old value; the operation is atomic.
 *
 * @override_id asc_atomic_sub__PGu32_u32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline uint32_t asc_atomic_sub(__gm__ uint32_t* address, uint32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores old_value minus val into the float element at the Global Memory address and returns the pre-update old value; the operation is atomic.
 *
 * @override_id asc_atomic_sub__PGf32_f32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline float asc_atomic_sub(__gm__ float* address, float val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores old_value minus val into the int64_t element at the Global Memory address and returns the pre-update old value; the operation is atomic.
 *
 * @override_id asc_atomic_sub__PGi64_i64
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline int64_t asc_atomic_sub(__gm__ int64_t* address, int64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores old_value minus val into the uint64_t element at the Global Memory address and returns the pre-update old value; the operation is atomic.
 *
 * @override_id asc_atomic_sub__PGu64_u64
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline uint64_t asc_atomic_sub(__gm__ uint64_t* address, uint64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically replaces the int32_t element at the Global Memory address with val and returns the pre-update old value; the read and write-back are atomic.
 *
 * @override_id asc_atomic_exch__PGi32_i32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline int32_t asc_atomic_exch(__gm__ int32_t* address, int32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically replaces the uint32_t element at the Global Memory address with val and returns the pre-update old value; the read and write-back are atomic.
 *
 * @override_id asc_atomic_exch__PGu32_u32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline uint32_t asc_atomic_exch(__gm__ uint32_t* address, uint32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically replaces the float element at the Global Memory address with val and returns the pre-update old value; the read and write-back are atomic.
 *
 * @override_id asc_atomic_exch__PGf32_f32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline float asc_atomic_exch(__gm__ float* address, float val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically replaces the int64_t element at the Global Memory address with val and returns the pre-update old value; the read and write-back are atomic.
 *
 * @override_id asc_atomic_exch__PGi64_i64
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline int64_t asc_atomic_exch(__gm__ int64_t* address, int64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically replaces the uint64_t element at the Global Memory address with val and returns the pre-update old value; the read and write-back are atomic.
 *
 * @override_id asc_atomic_exch__PGu64_u64
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline uint64_t asc_atomic_exch(__gm__ uint64_t* address, uint64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores max(old_value, val) into the int32_t element at the Global Memory address and returns the pre-update old value read from that address.
 *
 * @override_id asc_atomic_max__PGi32_i32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline int32_t asc_atomic_max(__gm__ int32_t* address, int32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores max(old_value, val) into the uint32_t element at the Global Memory address and returns the pre-update old value read from that address.
 *
 * @override_id asc_atomic_max__PGu32_u32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline uint32_t asc_atomic_max(__gm__ uint32_t* address, uint32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores max(old_value, val) into the float element at the Global Memory address and returns the pre-update old value read from that address.
 *
 * @override_id asc_atomic_max__PGf32_f32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline float asc_atomic_max(__gm__ float* address, float val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores max(old_value, val) into the int64_t element at the Global Memory address and returns the pre-update old value read from that address.
 *
 * @override_id asc_atomic_max__PGi64_i64
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline int64_t asc_atomic_max(__gm__ int64_t* address, int64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores max(old_value, val) into the uint64_t element at the Global Memory address and returns the pre-update old value read from that address.
 *
 * @override_id asc_atomic_max__PGu64_u64
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline uint64_t asc_atomic_max(__gm__ uint64_t* address, uint64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores min(old_value, val) into the int32_t element at the Global Memory address and returns the pre-update old value read from that address.
 *
 * @override_id asc_atomic_min__PGi32_i32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline int32_t asc_atomic_min(__gm__ int32_t* address, int32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores min(old_value, val) into the uint32_t element at the Global Memory address and returns the pre-update old value read from that address.
 *
 * @override_id asc_atomic_min__PGu32_u32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline uint32_t asc_atomic_min(__gm__ uint32_t* address, uint32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores min(old_value, val) into the float element at the Global Memory address and returns the pre-update old value read from that address.
 *
 * @override_id asc_atomic_min__PGf32_f32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline float asc_atomic_min(__gm__ float* address, float val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores min(old_value, val) into the int64_t element at the Global Memory address and returns the pre-update old value read from that address.
 *
 * @override_id asc_atomic_min__PGi64_i64
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline int64_t asc_atomic_min(__gm__ int64_t* address, int64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically stores min(old_value, val) into the uint64_t element at the Global Memory address and returns the pre-update old value read from that address.
 *
 * @override_id asc_atomic_min__PGu64_u64
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline uint64_t asc_atomic_min(__gm__ uint64_t* address, uint64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically compares the int32_t element at the Global Memory address with compare and stores val only when equal, returning the pre-update old value.
 *
 * @override_id asc_atomic_cas__PGi32_i32_i32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] compare 源操作数，做比较的值。
 * @param [in] val 源操作数，用于赋值的值。
 */
__aicore__ inline int32_t asc_atomic_cas(__gm__ int32_t* address, int32_t compare, int32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically compares the uint32_t element at the Global Memory address with compare and stores val only when equal, returning the pre-update old value.
 *
 * @override_id asc_atomic_cas__PGu32_u32_u32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] compare 源操作数，做比较的值。
 * @param [in] val 源操作数，用于赋值的值。
 */
__aicore__ inline uint32_t asc_atomic_cas(__gm__ uint32_t* address, uint32_t compare, uint32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically compares the float element at the Global Memory address with compare and stores val only when equal, returning the pre-update old value.
 *
 * @override_id asc_atomic_cas__PGf32_f32_f32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] compare 源操作数，做比较的值。
 * @param [in] val 源操作数，用于赋值的值。
 */
__aicore__ inline float asc_atomic_cas(__gm__ float* address, float compare, float val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically compares the int64_t element at the Global Memory address with compare and stores val only when equal, returning the pre-update old value.
 *
 * @override_id asc_atomic_cas__PGi64_i64_i64
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] compare 源操作数，做比较的值。
 * @param [in] val 源操作数，用于赋值的值。
 */
__aicore__ inline int64_t asc_atomic_cas(__gm__ int64_t* address, int64_t compare, int64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically compares the uint64_t element at the Global Memory address with compare and stores val only when equal, returning the pre-update old value.
 *
 * @override_id asc_atomic_cas__PGu64_u64_u64
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] compare 源操作数，做比较的值。
 * @param [in] val 源操作数，用于赋值的值。
 */
__aicore__ inline uint64_t asc_atomic_cas(__gm__ uint64_t* address, uint64_t compare, uint64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically ANDs the int32_t element at the Global Memory address with val, writes the result back to that address, and returns the pre-update old value.
 *
 * @override_id asc_atomic_and__PGi32_i32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline int32_t asc_atomic_and(__gm__ int32_t* address, int32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically ANDs the uint32_t element at the Global Memory address with val, writes the result back to that address, and returns the pre-update old value.
 *
 * @override_id asc_atomic_and__PGu32_u32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline uint32_t asc_atomic_and(__gm__ uint32_t* address, uint32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically ANDs the int64_t element at the Global Memory address with val, writes the result back to that address, and returns the pre-update old value.
 *
 * @override_id asc_atomic_and__PGi64_i64
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline int64_t asc_atomic_and(__gm__ int64_t* address, int64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically ANDs the uint64_t element at the Global Memory address with val, writes the result back to that address, and returns the pre-update old value.
 *
 * @override_id asc_atomic_and__PGu64_u64
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline uint64_t asc_atomic_and(__gm__ uint64_t* address, uint64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically ORs the int32_t element at the Global Memory address with val, writes the result back to that address, and returns the pre-update old value.
 *
 * @override_id asc_atomic_or__PGi32_i32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline int32_t asc_atomic_or(__gm__ int32_t* address, int32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically ORs the uint32_t element at the Global Memory address with val, writes the result back to that address, and returns the pre-update old value.
 *
 * @override_id asc_atomic_or__PGu32_u32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline uint32_t asc_atomic_or(__gm__ uint32_t* address, uint32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically ORs the int64_t element at the Global Memory address with val, writes the result back to that address, and returns the pre-update old value.
 *
 * @override_id asc_atomic_or__PGi64_i64
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline int64_t asc_atomic_or(__gm__ int64_t* address, int64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically ORs the uint64_t element at the Global Memory address with val, writes the result back to that address, and returns the pre-update old value.
 *
 * @override_id asc_atomic_or__PGu64_u64
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline uint64_t asc_atomic_or(__gm__ uint64_t* address, uint64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically XORs the int32_t element at the Global Memory address with val, writes the result back to that address, and returns the pre-update old value.
 *
 * @override_id asc_atomic_xor__PGi32_i32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline int32_t asc_atomic_xor(__gm__ int32_t* address, int32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically XORs the uint32_t element at the Global Memory address with val, writes the result back to that address, and returns the pre-update old value.
 *
 * @override_id asc_atomic_xor__PGu32_u32
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline uint32_t asc_atomic_xor(__gm__ uint32_t* address, uint32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically XORs the int64_t element at the Global Memory address with val, writes the result back to that address, and returns the pre-update old value.
 *
 * @override_id asc_atomic_xor__PGi64_i64
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline int64_t asc_atomic_xor(__gm__ int64_t* address, int64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically XORs the uint64_t element at the Global Memory address with val, writes the result back to that address, and returns the pre-update old value.
 *
 * @override_id asc_atomic_xor__PGu64_u64
 * @archs 3510
 * @param [in,out] address Global Memory的地址。
 * @param [in] val 标量值，数据类型与`address`指向元素类型一致。
 */
__aicore__ inline uint64_t asc_atomic_xor(__gm__ uint64_t* address, uint64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically increments the uint32_t element at the Global Memory address (0 if old_value is at least val, otherwise old_value+1) and returns the old value.
 *
 * @override_id asc_atomic_inc__PGu32_u32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline uint32_t asc_atomic_inc(__gm__ uint32_t* address, uint32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically increments the uint64_t element at the Global Memory address (0 if old_value is at least val, otherwise old_value+1) and returns the old value.
 *
 * @override_id asc_atomic_inc__PGu64_u64
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline uint64_t asc_atomic_inc(__gm__ uint64_t* address, uint64_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically decrements the uint32_t element at the Global Memory address (val if old_value is 0 or greater than val, otherwise old_value-1) and returns the old value.
 *
 * @override_id asc_atomic_dec__PGu32_u32
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline uint32_t asc_atomic_dec(__gm__ uint32_t* address, uint32_t val);
/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Atomically decrements the uint64_t element at the Global Memory address (val if old_value is 0 or greater than val, otherwise old_value-1) and returns the old value.
 *
 * @override_id asc_atomic_dec__PGu64_u64
 * @archs 3510
 * @param [in] address Global Memory的地址。
 * @param [in] val 源操作数。
 */
__aicore__ inline uint64_t asc_atomic_dec(__gm__ uint64_t* address, uint64_t val);
} // namespace __asc_aicore

/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Sets the atomic-operation enable bits (type, selecting the data type) and atomic operation type (op, 0 = sum) for later stores.
 *
 * @override_id asc_set_store_atomic_config_v1__u16_u16
 * @archs 2201
 * @param [in] type 原子操作启用位。 0：无原子操作。 1：开启原子操作，进行原子操作的数据类型为float。 2：开启原子操作，进行原子操作的数据类型为half。 3：开启原子操作，进行原子操作的数据类型为int16_t。 4：开启原子操作，进行原子操作的数据类型为int32_t。 5：开启原子操作，进行原子操作的数据类型为int8_t。 6：开启原子操作，进行原子操作的数据类型为bfloat16_t。
 * @param [in] op 原子操作类型。 0：求和操作。
 */
__aicore__ inline void asc_set_store_atomic_config_v1(uint16_t type, uint16_t op);

/**
 * @ingroup simd_c_api.atomic.scalar_atomic
 * @brief Reads the atomic-operation enable bits and atomic operation type into the output reference config for verification (void return).
 *
 * @override_id asc_get_store_atomic_config__Ratomiccfg
 * @archs 2201 3510
 * @param [out] config 用于获取原子操作启用位和原子操作类型的值，详细说明请参考[asc_store_atomic_config](../../defs/union/asc_store_atomic_config.md)。
 */
__aicore__ inline void asc_get_store_atomic_config(asc_store_atomic_config& config);

ASC_DEPRECATED(9.1.0, "2027/09/07", asc_atomic_add)
/**
 * @ingroup simd_c_api.deprecated_interface
 * @brief Deprecated: sets the atomic-operation enable bits and type; op 2 selects sum, but atomic add should use asc_atomic_add instead.
 *
 * @override_id asc_set_store_atomic_config_v2__u16_u16
 * @archs 3510
 * @param [in] type 原子操作启用位。 1：开启原子操作，进行原子操作的数据类型为int32_t。 4：开启原子操作，进行原子操作的数据类型为half。 5：开启原子操作，进行原子操作的数据类型为float。 7：开启原子操作，进行原子操作的数据类型为bfloat16_t。 9：开启原子操作，进行原子操作的数据类型为int16_t。 10：开启原子操作，进行原子操作的数据类型为int8_t。 其余值无具体含义。
 * @param [in] op 原子操作类型。 2：求和操作。 其余值无具体含义。
 */
__aicore__ inline void asc_set_store_atomic_config_v2(uint16_t type, uint16_t op);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ATOMIC_SCALAR_ATOMIC_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ATOMIC_SCALAR_ATOMIC_H
#endif
