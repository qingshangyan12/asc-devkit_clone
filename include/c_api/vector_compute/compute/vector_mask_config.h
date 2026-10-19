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
#error "vector_compute/compute/vector_mask_config.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_MASK_CONFIG_H
#endif

#ifndef INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_MASK_CONFIG_H
#define INCLUDE_C_API_VECTOR_COMPUTE_COMPUTE_VECTOR_MASK_CONFIG_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
#include "impl/c_api/memory_base_impl/vector_mask_config_intf_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/vector_mask_config_intf_impl.h"
#endif
/**
 * @ingroup simd_c_api.vector_compute.vector_mask_config
 * @brief Sets the Mask mode to Counter mode so callers pass the element count and the vector unit infers iterations; use together with asc_set_vector_mask.
 *
 * @override_id asc_set_mask_count__z
 * @archs 2201
 */
__aicore__ inline void asc_set_mask_count();

/**
 * @ingroup simd_c_api.vector_compute.vector_mask_config
 * @brief Sets the Mask mode to the system default Normal mode, where the Mask parameter controls which elements participate in each iteration; use with asc_set_vector_mask.
 *
 * @override_id asc_set_mask_norm__z
 * @archs 2201
 */
__aicore__ inline void asc_set_mask_norm();

/**
 * @ingroup simd_c_api.vector_compute.vector_mask_config
 * @brief Sets the vector mask value indicating which elements participate; supports Normal and Counter modes and requires the Mask mode to be set beforehand.
 *
 * @override_id asc_set_vector_mask__u64_u64
 * @archs 2201 3510
 * @param [in] mask_high 高64位Mask值。
 * @param [in] mask_low 低64位Mask值。
 */
__aicore__ inline void asc_set_vector_mask(uint64_t mask_high, uint64_t mask_low);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_MASK_CONFIG_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_MASK_CONFIG_H
#endif
