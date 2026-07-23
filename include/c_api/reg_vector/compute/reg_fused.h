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
#error "reg_fused.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_FUSED_H
#define INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_FUSED_H

#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"

// ==========asc_abs_sub(half/float)==========
__simd_callee__ inline void asc_abs_sub(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_abs_sub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

// ==========asc_axpy(half/float)==========
__simd_callee__ inline void asc_axpy(vector_half& dst, vector_half src, half value, vector_bool mask);

__simd_callee__ inline void asc_axpy(vector_float& dst, vector_float src, float value, vector_bool mask);

// ==========asc_madd(half/float)==========
__simd_callee__ inline void asc_madd(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_madd(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

// ==========asc_exp_sub(half/float)==========
[[deprecated("NOTICE: asc_exp_sub_v2 in this parameter list is deprecated.  Please use asc_exp_sub instead.")]]
__simd_callee__ inline void asc_exp_sub(vector_float& dst, vector_half src0, vector_half src1, vector_bool mask);

[[deprecated("NOTICE: asc_exp_sub_v2 in this parameter list is deprecated.  Please use asc_exp_sub instead.")]]
__simd_callee__ inline void asc_exp_sub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

[[deprecated("NOTICE: asc_exp_sub_v2 in this parameter list is deprecated.  Please use asc_exp_sub instead.")]]
__simd_callee__ inline void asc_exp_sub_v2(vector_float& dst, vector_half src0, vector_half src1, vector_bool mask);

[[deprecated("NOTICE: asc_exp_sub_v2 in this parameter list is deprecated.  Please use asc_exp_sub instead.")]]
__simd_callee__ inline void asc_exp_sub_v2(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

#endif
