/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_ATOMIC_ATOMIC_H
#define INCLUDE_C_API_ATOMIC_ATOMIC_H

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

#include "instr_impl/npu_arch_2201/atomic_impl.h"

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

#include "instr_impl/npu_arch_3510/atomic_impl.h"

#endif

#if defined(__NPU_ARCH__) 

/*
 * @ingroup AtomicOperations
 * @brief Set atomic add operation mode
 * @details Configure subsequent store operations to use (int8/int16/int32/half/bfloat16/float) atomic add semantics
 */
__aicore__ inline void asc_set_atomic_add_int8();

__aicore__ inline void asc_set_atomic_add_int16();

__aicore__ inline void asc_set_atomic_add_int();

__aicore__ inline void asc_set_atomic_add_half();

__aicore__ inline void asc_set_atomic_add_bfloat16();

__aicore__ inline void asc_set_atomic_add_float();

/*
 * @ingroup AtomicOperations
 * @brief Set atomic max operation mode
 * @details Configure subsequent store operations to use (int8/int16/int32/half/bfloat16/float) atomic add semantics
 */
__aicore__ inline void asc_set_atomic_max_int8();

__aicore__ inline void asc_set_atomic_max_int16();

__aicore__ inline void asc_set_atomic_max_int();

__aicore__ inline void asc_set_atomic_max_half();

__aicore__ inline void asc_set_atomic_max_bfloat16();

__aicore__ inline void asc_set_atomic_max_float();

/*
 * @ingroup AtomicOperations
 * @brief Set atomic min operation mode
 * @details Configure subsequent store operations to use (int8/int16/int32/half/bfloat16/float) atomic add semantics
 */
__aicore__ inline void asc_set_atomic_min_int8();

__aicore__ inline void asc_set_atomic_min_int16();

__aicore__ inline void asc_set_atomic_min_int();

__aicore__ inline void asc_set_atomic_min_half();

__aicore__ inline void asc_set_atomic_min_bfloat16();

__aicore__ inline void asc_set_atomic_min_float();

/*
 * @ingroup AtomicOperations
 * @brief Set no atomic operation mode
 * @details Configure subsequent store operations to use direct store semantics without atomic operations
 */
__aicore__ inline void asc_set_atomic_none();

/*
 * @ingroup AtomicOperations
 * @brief Set store atomic configuration
 * @param [in] atomic operation enable bit (1 means float, 2 means half etc.)
 * @param [in] op atomic operation opcode (0 means add etc.)
 * @details Configure subsequent store operations with custom atomic operation parameters
 */
__aicore__ inline void asc_set_store_atomic_config(uint16_t type, uint16_t op);

/*
 * @ingroup AtomicOperations
 * @brief Get current store atomic configuration
 * @param [out] config store atomic configuration structure to receive current configuration
 * @details Retrieve the current atomic operation configuration settings
 */
__aicore__ inline void asc_get_store_atomic_config(asc_store_atomic_config& config);

/*
 * @ingroup AtomicOperations
 * @brief Architecture-specific atomic configuration functions (deprecated)
 * @details These functions are deprecated and will be moved to lower-level implementation files.
 *          Users should use the unified asc_set_store_atomic_config() function instead.
 * @deprecated Use asc_set_store_atomic_config() for all architectures
 */
#if __NPU_ARCH__ == 2201
__aicore__ inline void asc_set_store_atomic_config_v1(uint16_t type, uint16_t op);
#elif __NPU_ARCH__ == 3510
__aicore__ inline void asc_set_store_atomic_config_v2(uint16_t type, uint16_t op);
#endif

#endif // __NPU_ARCH__
#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif
