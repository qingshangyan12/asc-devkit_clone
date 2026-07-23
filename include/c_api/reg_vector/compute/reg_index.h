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
#error "reg_index.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_INDEX_H
#define INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_INDEX_H

#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"

// ==========asc_arange(int8_t/int16_t/int32_t/half/float)==========
__simd_callee__ inline void asc_arange(vector_int8_t& dst, int8_t value);

__simd_callee__ inline void asc_arange(vector_int16_t& dst, int16_t value);

__simd_callee__ inline void asc_arange(vector_int32_t& dst, int32_t value);

__simd_callee__ inline void asc_arange(vector_half& dst, half value);

__simd_callee__ inline void asc_arange(vector_float& dst, float value);

__simd_callee__ inline void asc_arange_descend(vector_int8_t& dst, int8_t value);

__simd_callee__ inline void asc_arange_descend(vector_int16_t& dst, int16_t value);

__simd_callee__ inline void asc_arange_descend(vector_int32_t& dst, int32_t value);

__simd_callee__ inline void asc_arange_descend(vector_half& dst, half value);

__simd_callee__ inline void asc_arange_descend(vector_float& dst, float value);

#endif
