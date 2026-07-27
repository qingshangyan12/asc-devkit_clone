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
#error "reg_reduce.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_REDUCE_H
#define INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_REDUCE_H

#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"

// ==========asc_reduce_max(uint16_t/int16_t/half/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_reduce_max(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_max(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_max(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_max(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_max(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_reduce_max(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_reduce_min(uint16_t/int16_t/half/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_reduce_min(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_min(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_min(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_min(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_min(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_reduce_min(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_reduce_min_datablock(uint16_t/int16_t/half/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_reduce_min_datablock(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_min_datablock(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_min_datablock(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_min_datablock(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_min_datablock(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_reduce_min_datablock(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_pair_reduce_sum(half/float)==========
__simd_callee__ inline void asc_pair_reduce_sum(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_pair_reduce_sum(vector_float& dst, vector_float src, vector_bool mask);

//==================asc_reduce_max_datablock====================
__simd_callee__ inline void asc_reduce_max_datablock(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_reduce_max_datablock(vector_float& dst, vector_float src, vector_bool mask);

__simd_callee__ inline void asc_reduce_max_datablock(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_max_datablock(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_max_datablock(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_max_datablock(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

// ==========asc_reduce_sum(int16_t/int32_t/uint16_t/uint32_t/half/float)==========
__simd_callee__ inline void asc_reduce_sum(vector_int32_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_reduce_sum_datablock(half/float/uint16_t/int16_t/uint32_t/int32_t)==========
__simd_callee__ inline void asc_reduce_sum_datablock(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum_datablock(vector_float& dst, vector_float src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum_datablock(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum_datablock(vector_int32_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum_datablock(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_reduce_sum_datablock(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

#endif
