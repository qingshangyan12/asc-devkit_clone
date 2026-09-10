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
#include "impl/c_api/instr_impl/npu_arch_2201/vector_compute_impl.h"
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"
#endif
/**
 * @ingroup simd_c_api.vector_compute.vector_mask_config
 * @override_id asc_set_mask_count__z
 * @archs 2201
 * @brief 设置Mask模式为Counter模式。该模式下，不需要开发者去感知迭代次数、处理非对齐的尾块等操作，可直接传入数据计算数量，实际迭代次数由Vector计算单元自动推断。在前n个数据计算类的接口内部会自动设置该模式，不需要自行设置。
 */
__aicore__ inline void asc_set_mask_count();

/**
 * @ingroup simd_c_api.vector_compute.vector_mask_config
 * @override_id asc_set_mask_norm__z
 * @archs 2201
 * @brief 设置Mask模式为Normal模式，该模式为系统默认模式。该模式下，Mask参数可控制高维切分计算过程中单次迭代内参与计算的元素。
 */
__aicore__ inline void asc_set_mask_norm();

/**
 * @ingroup simd_c_api.vector_compute.vector_mask_config
 * @override_id asc_set_vector_mask__u64_u64
 * @archs 3510 2201
 * @brief 本API用于设置Mask值。某些矢量计算接口需要提前设置Mask，用于指示哪些数据参与计算。 单独使用该API时可引入`c_api/vector_compute/compute/vector_mask_config.h`，整体使用C API时可引入`c_api/asc_simd.h`。
 */
__aicore__ inline void asc_set_vector_mask(uint64_t mask_high, uint64_t mask_low);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_MASK_CONFIG_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_VECTOR_COMPUTE_COMPUTE_VECTOR_MASK_CONFIG_H
#endif
