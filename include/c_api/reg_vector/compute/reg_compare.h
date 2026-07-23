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
#error "reg_compare.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_COMPARE_H
#define INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_COMPARE_H

#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"

// ==========asc_eq(int8_t/uint8_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
__simd_callee__ inline void asc_eq(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_eq(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_eq(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_eq(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_eq(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_eq(vector_bool& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

__simd_callee__ inline void asc_eq(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_eq(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_eq(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask);

// ==========asc_eq_scalar(int8_t/uint8_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_eq_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

//==========asc_lt(u8/s8/half/u16/s16/float/u32/s32/bf16)==========
__simd_callee__ inline void asc_lt(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_lt(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_lt(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_lt(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_lt(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_lt(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask);

__simd_callee__ inline void asc_lt(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_lt(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_lt(vector_bool& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

__simd_callee__ inline void asc_lt_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

//=================asc_gt=====================
__simd_callee__ inline void asc_gt(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_gt(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_gt(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_gt(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask);

__simd_callee__ inline void asc_gt(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_gt(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_gt(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_gt(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_gt(vector_bool& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

//=================asc_gt_scalar=====================
__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

__simd_callee__ inline void asc_gt_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

// ==========asc_ne(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t/half/float)==========
__simd_callee__ inline void asc_ne(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_ne(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_ne(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_ne(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_ne(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_ne(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_ne(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_ne(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask);

// ==========asc_ne_scalar(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t/half/float/bfloat16_t)==========
__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

__simd_callee__ inline void asc_ne_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

// ==========asc_le(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t/half/float)==========
__simd_callee__ inline void asc_le(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_le(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_le(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_le(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_le(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_le(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_le(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_le(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask);

__simd_callee__ inline void asc_le(vector_bool& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

// ==========asc_le_scalar(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t/half/float)==========
__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

__simd_callee__ inline void asc_le_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

// ==========asc_ge(uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_ge(vector_bool& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

__simd_callee__ inline void asc_ge(vector_bool& dst, vector_float src0, vector_float src1, vector_bool mask);

// ==========asc_ge_scalar(uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int8_t src, int8_t value, vector_bool mask);

__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int16_t src, int16_t value, vector_bool mask);

__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_int32_t src, int32_t value, vector_bool mask);

__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_half src, half value, vector_bool mask);

__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

__simd_callee__ inline void asc_ge_scalar(vector_bool& dst, vector_float src, float value, vector_bool mask);

#endif
