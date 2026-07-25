/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "atomic.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_ATOMIC_ATOMIC_H
#define INCLUDE_C_API_ATOMIC_ATOMIC_H

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/instr_impl/npu_arch_2201/atomic_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/atomic_impl.h"
#endif

/**
 * @brief Enable the atomic-add operation for the given data type.
 * Enables the atomic accumulation on subsequent data transfers from
 * Unified Buffer / L0C Buffer / L1 Buffer to Global Memory.
 * Elements are accumulated element-wise and the sum is written back
 * to Global Memory. Each variant operates on a specific data type:
 *   asc_set_atomic_add_float16() : half
 *   asc_set_atomic_add_float()   : float
 *   asc_set_atomic_add_bfloat()  : bfloat16_t
 *   asc_set_atomic_add_int8()    : int8_t
 *   asc_set_atomic_add_int16()   : int16_t
 *   asc_set_atomic_add_int32()   : int32_t
 *
 * @note Pipeline type: PIPE_S
 * @note It is recommended to call asc_set_atomic_none() afterwards to disable
 *   the atomic-add operation so that subsequent instructions are not affected.
 * @note The instruction does not zero the Global Memory data before execution;
 *   developers must add a manual clear operation when needed.
 * @note For Ascend 950PR / Ascend 950DT, the L1 Buffer to Global Memory path
 *   is not supported.
 *
 * @par Example
 * @code
 * asc_copy_ub2gm(dst, src0, total_length * sizeof(int8_t));
 * asc_sync_pipe(PIPE_MTE3);
 * asc_set_atomic_add_int8();
 * asc_copy_ub2gm(dst, src1, total_length * sizeof(int8_t));
 * asc_set_atomic_none();
 * @endcode
 * @return None.
 * @see asc_set_atomic_none
 */
__aicore__ inline void asc_set_atomic_add_float();
__aicore__ inline void asc_set_atomic_add_int8();
__aicore__ inline void asc_set_atomic_add_int16();
__aicore__ inline void asc_set_atomic_add_int32();

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)

/**
 * @brief Clear the atomic operation state.
 * Disables any active atomic add / max / min operation so that subsequent
 * data transfers to Global Memory are not affected.
 *
 * @note Pipeline type: PIPE_S
 * @return None.
 */
__aicore__ inline void asc_set_atomic_none();

/**
 * @brief Get the atomic operation enable bit and operation type.
 * @deprecated Use asc_atomic_add for atomic add operation instead.
 * @note Pipeline type: PIPE_S
 * @return None.
 */
__aicore__ inline void asc_get_store_atomic_config(asc_store_atomic_config& config);

/**
 * @brief Set the atomic operation enable bit and operation type.
 * @note Pipeline type: PIPE_S
 * @return None.
 */
__aicore__ inline void asc_set_store_atomic_config_v1(uint16_t type, uint16_t op);

__aicore__ inline void asc_set_atomic_add_bfloat();
__aicore__ inline void asc_set_atomic_add_float16();

/**
 * @brief Enable the atomic-max operation for the given data type.
 * Enables the atomic compare-and-take-maximum operation on subsequent data
 * transfers from Unified Buffer / L0C Buffer / L1 Buffer to Global Memory.
 * Elements are compared element-wise and the larger value is written back
 * to Global Memory. Each variant operates on a specific data type:
 *   asc_set_atomic_max_float()   : float
 *   asc_set_atomic_max_float16() : half
 *   asc_set_atomic_max_bfloat()  : bfloat16_t
 *   asc_set_atomic_max_int8()    : int8_t
 *   asc_set_atomic_max_int16()   : int16_t
 *   asc_set_atomic_max_int32()   : int32_t
 *
 * @note Pipeline type: PIPE_S
 * @note It is recommended to call asc_set_atomic_none() afterwards to disable
 *   the atomic-max operation so that subsequent instructions are not affected.
 * @note For Ascend 950PR / Ascend 950DT, the L1 Buffer to Global Memory path
 *   is not supported.
 *
 * @par Example
 * @code
 * asc_copy_ub2gm(dst, src0, total_length * sizeof(int8_t));
 * asc_sync_pipe(PIPE_MTE3);
 * asc_set_atomic_max_int8();
 * asc_copy_ub2gm(dst, src1, total_length * sizeof(int8_t));
 * asc_set_atomic_none();
 * @endcode
 * @return None.
 * @see asc_set_atomic_none
 */
__aicore__ inline void asc_set_atomic_max_bfloat();
__aicore__ inline void asc_set_atomic_max_float();
__aicore__ inline void asc_set_atomic_max_float16();
__aicore__ inline void asc_set_atomic_max_int8();
__aicore__ inline void asc_set_atomic_max_int16();
__aicore__ inline void asc_set_atomic_max_int32();

/**
 * @brief Enable the atomic-min operation for the given data type.
 * Enables the atomic compare-and-take-minimum operation on subsequent data
 * transfers from Unified Buffer / L0C Buffer / L1 Buffer to Global Memory.
 * Elements are compared element-wise and the smaller value is written back
 * to Global Memory. Each variant operates on a specific data type:
 *   asc_set_atomic_min_float()   : float
 *   asc_set_atomic_min_float16() : half
 *   asc_set_atomic_min_bfloat()  : bfloat16_t
 *   asc_set_atomic_min_int8()    : int8_t
 *   asc_set_atomic_min_int16()   : int16_t
 *   asc_set_atomic_min_int32()   : int32_t
 *
 * @note Pipeline type: PIPE_S
 * @note It is recommended to call asc_set_atomic_none() afterwards to disable
 *   the atomic-min operation so that subsequent instructions are not affected.
 * @note For Ascend 950PR / Ascend 950DT, the L1 Buffer to Global Memory path
 *   is not supported.
 *
 * @par Example
 * @code
 * asc_copy_ub2gm(dst, src0, total_length * sizeof(int8_t));
 * asc_sync_pipe(PIPE_MTE3);
 * asc_set_atomic_min_int8();
 * asc_copy_ub2gm(dst, src1, total_length * sizeof(int8_t));
 * asc_set_atomic_none();
 * @endcode
 * @return None.
 * @see asc_set_atomic_none
 */
__aicore__ inline void asc_set_atomic_min_bfloat();
__aicore__ inline void asc_set_atomic_min_float();
__aicore__ inline void asc_set_atomic_min_float16();
__aicore__ inline void asc_set_atomic_min_int8();
__aicore__ inline void asc_set_atomic_min_int16();
__aicore__ inline void asc_set_atomic_min_int32();

#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)

__aicore__ inline void asc_set_atomic_add_float16();

__aicore__ inline void asc_set_atomic_max_float();

/**
 * @brief Get the atomic operation enable bit and operation type.
 * @deprecated Use asc_atomic_add for atomic add operation instead.
 * @note Pipeline type: PIPE_S
 * @return None.
 */
[[deprecated("NOTICE: asc_get_store_atomic_config is deprecated on Ascend 950PR/Ascend 950DT."
             "Please use asc_atomic_add instead for atomic add operation.")]]
__aicore__ inline void asc_get_store_atomic_config(asc_store_atomic_config& config);

/**
 * @brief Set the atomic operation enable bit and operation type.
 * @deprecated Use asc_atomic_add for atomic add operation instead.
 * @note Pipeline type: PIPE_S
 * @return None.
 */
[[deprecated("NOTICE: asc_set_store_atomic_config_v2 is deprecated."
             "Please use asc_atomic_add instead for atomic add operation.")]]
__aicore__ inline void asc_set_store_atomic_config_v2(uint16_t type, uint16_t op);

__aicore__ inline void asc_set_atomic_max_int8();
__aicore__ inline void asc_set_atomic_max_int16();
__aicore__ inline void asc_set_atomic_max_int32();

__aicore__ inline void asc_set_atomic_min_int8();
__aicore__ inline void asc_set_atomic_min_int16();
__aicore__ inline void asc_set_atomic_min_int32();

#endif

// ==================== Deprecated interfaces ====================

/**
 * @brief Enable the atomic-add operation for int32_t.
 * @deprecated Use asc_set_atomic_add_int32() instead.
 * @note Pipeline type: PIPE_S
 * @return None.
 * @see asc_set_atomic_add_int32
 */
[[deprecated("NOTICE: asc_set_atomic_add_int is deprecated. Please use asc_set_atomic_add_int32 instead.")]]
__aicore__ inline void asc_set_atomic_add_int();

/**
 * @brief Enable the atomic-max operation for int32_t.
 * @deprecated Use asc_set_atomic_max_int32() instead.
 * @note Pipeline type: PIPE_S
 * @return None.
 * @see asc_set_atomic_max_int32
 */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
[[deprecated("NOTICE: asc_set_atomic_max_int is deprecated. Please use asc_set_atomic_max_int32 instead.")]]
__aicore__ inline void asc_set_atomic_max_int();
#endif

/**
 * @brief Enable the atomic-min operation for int32_t.
 * @deprecated Use asc_set_atomic_min_int32() instead.
 * @note Pipeline type: PIPE_S
 * @return None.
 * @see asc_set_atomic_min_int32
 */
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
[[deprecated("NOTICE: asc_set_atomic_min_int is deprecated. Please use asc_set_atomic_min_int32 instead.")]]
__aicore__ inline void asc_set_atomic_min_int();
#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
/**
 * @brief Enable the atomic-max operation for int32_t.
 * @deprecated Use asc_set_atomic_max_int32() instead.
 * @note Pipeline type: PIPE_S
 * @return None.
 * @see asc_set_atomic_max_int32
 */
[[deprecated("NOTICE: asc_set_atomic_max_int is deprecated. Please use asc_set_atomic_max_int32 instead.")]]
__aicore__ inline void asc_set_atomic_max_int();

/**
 * @brief Enable the atomic-min operation for int32_t.
 * @deprecated Use asc_set_atomic_min_int32() instead.
 * @note Pipeline type: PIPE_S
 * @return None.
 * @see asc_set_atomic_min_int32
 */
[[deprecated("NOTICE: asc_set_atomic_min_int is deprecated. Please use asc_set_atomic_min_int32 instead.")]]
__aicore__ inline void asc_set_atomic_min_int();
#endif

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif
