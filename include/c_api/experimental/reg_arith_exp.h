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
#error "reg_arith_exp.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_EXPERIMENTAL_REG_ARITH_EXP_H
#endif

#ifndef INCLUDE_C_API_EXPERIMENTAL_REG_ARITH_EXP_H
#define INCLUDE_C_API_EXPERIMENTAL_REG_ARITH_EXP_H

#ifndef CANN_ASC_USE_EXPERIMENTAL
#error "this is experimental feature, pls set CANN_ASC_USE_EXPERIMENTAL = ON to use it"
#endif
// WARNING: The APIs in this header are unstable and may change or be removed in future releases.

#include "c_api/reg_compute/compute/reg_arith.h"
#include "c_api/experimental/pair.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/experimental/reg_arith_intf_impl_exp.h"
#endif

__simd_callee__ inline asc_uint32_t_bool_pair asc_sub_carry(
    vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline asc_int32_t_bool_pair asc_sub_carry(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline asc_uint32_t_bool_pair asc_sub_carry(
    vector_uint32_t src0, vector_uint32_t src1, vector_bool carry_src, vector_bool mask);

__simd_callee__ inline asc_int32_t_bool_pair asc_sub_carry(
    vector_int32_t src0, vector_int32_t src1, vector_bool carry_src, vector_bool mask);

__simd_callee__ inline asc_int32_t_int32_t_pair asc_mull(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline asc_uint32_t_uint32_t_pair asc_mull(
    vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline asc_uint32_t_bool_pair asc_add_carry(
    vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline asc_int32_t_bool_pair asc_add_carry(vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline asc_uint32_t_bool_pair asc_add_carry(
    vector_uint32_t src0, vector_uint32_t src1, vector_bool carry_src, vector_bool mask);

__simd_callee__ inline asc_int32_t_bool_pair asc_add_carry(
    vector_int32_t src0, vector_int32_t src1, vector_bool carry_src, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_EXPERIMENTAL_REG_ARITH_EXP_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_EXPERIMENTAL_REG_ARITH_EXP_H
#endif
