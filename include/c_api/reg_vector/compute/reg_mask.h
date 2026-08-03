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
#error "reg_mask.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_MASK_H
#define INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_MASK_H

#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"

/*
 *   asc_create_mask_b8(pat_mode);
 *   asc_create_mask_b16(pat_mode);
 *   asc_create_mask_b32(pat_mode);
 *
 *   The parameter pat_mode is defined as follows:
 *
 *    PAT_ALL, // All elements are set to True
 *    PAT_VL1, // The lowest element is set to True
 *    PAT_VL2, // The lowest 2 elements are set to True
 *    PAT_VL3, // The lowest 3 elements are set to True
 *    PAT_VL4, // The lowest 4 elements are set to True
 *    PAT_VL8, // The lowest 8 elements are set to True
 *    PAT_VL16, // The lowest 16 elements are set to True
 *    PAT_VL32, // The lowest 32 elements are set to True
 *    PAT_VL64, // The lowest 64 elements are set to True
 *    PAT_VL128, // The lowest 128 elements are set to True
 *    PAT_M3, // Multiples of 3 elements are set to True
 *    PAT_M4, // Multiples of 4 elements are set to True
 *    PAT_H, // The lowest half elements are set to True
 *    PAT_Q, // The lowest quarter elements are set to True
 *    PAT_ALLF = 15 // All elements are set to False
 *
 *   usage example:
 *       vector_bool mask = asc_create_mask_b8(PAT_VL1);
 */
#define asc_create_mask_b8 pset_b8
#define asc_create_mask_b16 pset_b16
#define asc_create_mask_b32 pset_b32

// ==========asc_update_mask==========
__simd_callee__ inline vector_bool asc_update_mask_b8(uint32_t& value);

__simd_callee__ inline vector_bool asc_update_mask_b16(uint32_t& value);

__simd_callee__ inline vector_bool asc_update_mask_b32(uint32_t& value);

#endif
