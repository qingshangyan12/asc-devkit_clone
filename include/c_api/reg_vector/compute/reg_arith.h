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
#error "reg_arith.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_ARITH_H
#define INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_ARITH_H

#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"

// ==========asc_relu(half/int32_t/float)==========
__simd_callee__ inline void asc_relu(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_relu(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline void asc_relu(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_sub(uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_sub(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_sub(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_sub(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_sub(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_sub(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_sub(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

__simd_callee__ inline void asc_sub(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_sub(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_sub(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

__simd_callee__ inline void asc_sub(
    vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_sub(
    vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

// ==========asc_subc(uint32_t/int32_t)==========
__simd_callee__ inline void asc_subc(
    vector_bool& carry, vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool carry_src,
    vector_bool mask);

__simd_callee__ inline void asc_subc(
    vector_bool& carry, vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool carry_src,
    vector_bool mask);

// ==========asc_min(uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_min(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_min(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_min(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_min(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_min(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_min(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_min(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_min(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

__simd_callee__ inline void asc_min(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

// ==========asc_neg(int8_t/int16_t/half/int32_t/float)==========
__simd_callee__ inline void asc_neg(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

__simd_callee__ inline void asc_neg(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_neg(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline void asc_neg(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_neg(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_abs(int8_t/int16_t/int32_t/half/float)==========
__simd_callee__ inline void asc_abs(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

__simd_callee__ inline void asc_abs(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_abs(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline void asc_abs(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_abs(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_mull(uint32_t/int32_t)==========
__simd_callee__ inline void asc_mull(
    vector_uint32_t& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_mull(
    vector_int32_t& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

// ==========asc_sqrt(half/float)==========
__simd_callee__ inline void asc_sqrt(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_sqrt(vector_float& dst, vector_float src, vector_bool mask);


// ==========asc_mul(int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
__simd_callee__ inline void asc_mul(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_mul(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_mul(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_mul(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

__simd_callee__ inline void asc_mul(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_mul(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_mul(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

// ==========asc_mul_scalar(int16_t/uint16_t/half/int32_t/uint32_t/float)==========
__simd_callee__ inline void asc_mul_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

__simd_callee__ inline void asc_mul_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_mul_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);

__simd_callee__ inline void asc_mul_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

__simd_callee__ inline void asc_mul_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_mul_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);

// ==========asc_muls==========
__simd_callee__ inline void asc_muls(vector_half& dst, vector_float src, float value, vector_bool mask);

__simd_callee__ inline void asc_muls_v2(vector_half& dst, vector_float src, float value, vector_bool mask);

// ==========asc_add(u8/s8/u16/s18/u32/s32)=========
__simd_callee__ inline void asc_add(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_add(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_add(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_add(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_add(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_add(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

__simd_callee__ inline void asc_add(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_add(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_add(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

__simd_callee__ inline void asc_add(
    vector_bool& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_add(
    vector_bool& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

// // ==========asc_addc(uint32_t/int32_t)==========
__simd_callee__ inline void asc_addc(
    vector_bool& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1, vector_bool src2,
    vector_bool mask);

__simd_callee__ inline void asc_addc(
    vector_bool& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1, vector_bool src2,
    vector_bool mask);

//===============asc_max===============
__simd_callee__ inline void asc_max(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_max(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_max(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_max(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_max(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_max(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_max(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

__simd_callee__ inline void asc_max(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_max(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

//================asc_min_scalar=================
__simd_callee__ inline void asc_min_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

__simd_callee__ inline void asc_min_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_min_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

__simd_callee__ inline void asc_min_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_min_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask);

__simd_callee__ inline void asc_min_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

__simd_callee__ inline void asc_min_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);

__simd_callee__ inline void asc_min_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);



// ==========asc_exp(half/float)==========
__simd_callee__ inline void asc_exp(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_exp(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_add_scalar(int8_t/uint8_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
__simd_callee__ inline void asc_add_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask);

__simd_callee__ inline void asc_add_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

__simd_callee__ inline void asc_add_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

__simd_callee__ inline void asc_add_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_add_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);

__simd_callee__ inline void asc_add_scalar(
    vector_bfloat16_t& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

__simd_callee__ inline void asc_add_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

__simd_callee__ inline void asc_add_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_add_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);

// ==========asc_ln(half/float)==========
__simd_callee__ inline void asc_ln(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_ln(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_div(half/float)==========
__simd_callee__ inline void asc_div(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_div(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

// ==========asc_max_scalar(int8_t/int16_t/int32_t/uint8_t/uint16_t/uint32_t/half/float/bfloat16_t)==========
__simd_callee__ inline void asc_max_scalar(vector_int8_t& dst, vector_int8_t src, int8_t value, vector_bool mask);

__simd_callee__ inline void asc_max_scalar(vector_int16_t& dst, vector_int16_t src, int16_t value, vector_bool mask);

__simd_callee__ inline void asc_max_scalar(vector_int32_t& dst, vector_int32_t src, int32_t value, vector_bool mask);

__simd_callee__ inline void asc_max_scalar(vector_uint8_t& dst, vector_uint8_t src, uint8_t value, vector_bool mask);

__simd_callee__ inline void asc_max_scalar(vector_uint16_t& dst, vector_uint16_t src, uint16_t value, vector_bool mask);

__simd_callee__ inline void asc_max_scalar(vector_uint32_t& dst, vector_uint32_t src, uint32_t value, vector_bool mask);

__simd_callee__ inline void asc_max_scalar(vector_half& dst, vector_half src, half value, vector_bool mask);

__simd_callee__ inline void asc_max_scalar(vector_float& dst, vector_float src, float value, vector_bool mask);

__simd_callee__ inline void asc_max_scalar(
    vector_bfloat16_t& dst, vector_bfloat16_t src, bfloat16_t value, vector_bool mask);

//===================asc_prelu=====================
__simd_callee__ inline void asc_prelu(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

__simd_callee__ inline void asc_prelu(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

//================asc_leakyrelu===================
__simd_callee__ inline void asc_leakyrelu(vector_float& dst, vector_float src, float value, vector_bool mask);

__simd_callee__ inline void asc_leakyrelu(vector_half& dst, vector_half src, half value, vector_bool mask);

#endif
