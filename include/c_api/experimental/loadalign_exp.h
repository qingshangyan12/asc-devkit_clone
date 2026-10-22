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
#error "loadalign_exp.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_EXPERIMENTAL_LOADALIGN_EXP_H
#endif

#ifndef INCLUDE_C_API_EXPERIMENTAL_LOADALIGN_EXP_H
#define INCLUDE_C_API_EXPERIMENTAL_LOADALIGN_EXP_H

#ifndef CANN_ASC_USE_EXPERIMENTAL
#error "this is experimental feature, pls set CANN_ASC_USE_EXPERIMENTAL = ON to use it"
#endif
// WARNING: The APIs in this header are unstable and may change or be removed in future releases.

#include "c_api/reg_compute/load/loadalign.h"
#include "c_api/experimental/pair.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/experimental/loadalign_intf_impl_exp.h"
#endif

__simd_callee__ inline asc_int8_t_int8_t_pair asc_loadalign_deintlv(__ubuf__ int8_t* src);

__simd_callee__ inline asc_uint8_t_uint8_t_pair asc_loadalign_deintlv(__ubuf__ uint8_t* src);

__simd_callee__ inline asc_fp4x2_e2m1_t_fp4x2_e2m1_t_pair asc_loadalign_deintlv(__ubuf__ fp4x2_e2m1_t* src);

__simd_callee__ inline asc_fp4x2_e1m2_t_fp4x2_e1m2_t_pair asc_loadalign_deintlv(__ubuf__ fp4x2_e1m2_t* src);

__simd_callee__ inline asc_int4x2_t_int4x2_t_pair asc_loadalign_deintlv(__ubuf__ int4b_t* src);

__simd_callee__ inline asc_fp8_e8m0_t_fp8_e8m0_t_pair asc_loadalign_deintlv(__ubuf__ fp8_e8m0_t* src);

__simd_callee__ inline asc_fp8_e5m2_t_fp8_e5m2_t_pair asc_loadalign_deintlv(__ubuf__ fp8_e5m2_t* src);

__simd_callee__ inline asc_fp8_e4m3fn_t_fp8_e4m3fn_t_pair asc_loadalign_deintlv(__ubuf__ fp8_e4m3fn_t* src);

__simd_callee__ inline asc_hifloat8_t_hifloat8_t_pair asc_loadalign_deintlv(__ubuf__ hifloat8_t* src);

__simd_callee__ inline asc_int16_t_int16_t_pair asc_loadalign_deintlv(__ubuf__ int16_t* src);

__simd_callee__ inline asc_uint16_t_uint16_t_pair asc_loadalign_deintlv(__ubuf__ uint16_t* src);

__simd_callee__ inline asc_half_half_pair asc_loadalign_deintlv(__ubuf__ half* src);

__simd_callee__ inline asc_bfloat16_t_bfloat16_t_pair asc_loadalign_deintlv(__ubuf__ bfloat16_t* src);

__simd_callee__ inline asc_int32_t_int32_t_pair asc_loadalign_deintlv(__ubuf__ int32_t* src);

__simd_callee__ inline asc_uint32_t_uint32_t_pair asc_loadalign_deintlv(__ubuf__ uint32_t* src);

__simd_callee__ inline asc_float_float_pair asc_loadalign_deintlv(__ubuf__ float* src);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_EXPERIMENTAL_LOADALIGN_EXP_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_EXPERIMENTAL_LOADALIGN_EXP_H
#endif
