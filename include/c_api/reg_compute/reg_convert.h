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
#error "reg_convert.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_REG_CONVERT_H
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_REG_CONVERT_H
#define INCLUDE_C_API_REG_COMPUTE_REG_CONVERT_H
#include "c_api/defs/defs.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"
#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
// ASC_RETURN_VALUE_VCONV_API
__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint32_t asc_int162uint32(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_uint32_t asc_int162uint32(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int32_t asc_int642int32(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int32_t asc_int642int32(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int32_t asc_int642int32_sat(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int32_t asc_int642int32_sat(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_int82half(
    vector_int8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_half asc_int82half(
    vector_int8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_uint8_t asc_uint162uint8(
    vector_uint16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_uint8_t asc_uint162uint8(
    vector_uint16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_uint8_t asc_uint162uint8_sat(
    vector_uint16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_uint8_t asc_uint162uint8_sat(
    vector_uint16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int32_t asc_int82int32(
    vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_int32_t asc_int82int32(
    vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_int32_t asc_int82int32(
    vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_int32_t asc_int82int32(
    vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e2m1x22bfloat16(
    vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e2m1x22bfloat16(
    vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e2m1x22bfloat16(
    vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e2m1x22bfloat16(
    vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_uint16_t asc_int322uint16(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_uint16_t asc_int322uint16(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_uint16_t asc_int322uint16_sat(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_uint16_t asc_int322uint16_sat(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_float asc_int162float(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_float asc_int162float(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int16_t asc_int82int16(
    vector_int8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int16_t asc_int82int16(
    vector_int8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_uint32_t asc_uint82uint32(
    vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_uint32_t asc_uint82uint32(
    vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_uint32_t asc_uint82uint32(
    vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_uint32_t asc_uint82uint32(
    vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8_sat(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8_sat(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8_sat(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_int322uint8_sat(
    vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_int64_t asc_int322int64(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int64_t asc_int322int64(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int32_t asc_int162int32(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int32_t asc_int162int32(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_uint8_t asc_int162uint8(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_uint8_t asc_int162uint8(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_uint8_t asc_int162uint8_sat(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_uint8_t asc_int162uint8_sat(
    vector_int16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_e1m2x22bfloat16(
    vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e1m2x22bfloat16(
    vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e1m2x22bfloat16(
    vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_e1m2x22bfloat16(
    vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_uint16_t asc_uint82uint16(
    vector_uint8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_uint16_t asc_uint82uint16(
    vector_uint8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_float asc_e5m22float(
    vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_e5m22float(
    vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_e5m22float(
    vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_e5m22float(
    vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint16_t asc_uint322uint16(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_uint16_t asc_uint322uint16(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_uint16_t asc_uint322uint16_sat(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_uint16_t asc_uint322uint16_sat(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8_sat(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8_sat(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8_sat(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_uint8_t asc_uint322uint8_sat(
    vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_half asc_hif82half(
    vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_half asc_hif82half(
    vector_hifloat8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int16_t asc_int322int16(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_int322int16(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int16_t asc_int322int16_sat(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_int322int16_sat(
    vector_int32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_float asc_e4m32float(
    vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_e4m32float(
    vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_e4m32float(
    vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_e4m32float(
    vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_half asc_float2half_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_float2half_ro(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_half asc_float2half_ro(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_float2half_ro_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_half asc_float2half_ro_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_float2half_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_half asc_float2half_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_float2half_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_half asc_float2half_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_sat(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_float asc_hif82float(
    vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_hif82float(
    vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_hif82float(
    vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_float asc_hif82float(
    vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_float2int16_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rn(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rn_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rz(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rz_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_ru(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_ru_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rd(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rd_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rna(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_int64_t asc_float2int64_rna_sat(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_int16_t asc_uint322int16(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_uint322int16(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_int16_t asc_uint322int16_sat(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_int16_t asc_uint322int16_sat(
    vector_uint32_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_half asc_uint82half(
    vector_uint8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_half asc_uint82half(
    vector_uint8_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_uint32_t asc_uint162uint32(
    vector_uint16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline vector_uint32_t asc_uint162uint32(
    vector_uint16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline vector_float asc_int642float_ru(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_float asc_int642float_ru(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_float asc_int642float_rd(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_float asc_int642float_rd(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_float asc_int642float_rn(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_float asc_int642float_rn(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_float asc_int642float_rz(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_float asc_int642float_rz(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline vector_float asc_int642float_rna(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

__simd_callee__ inline vector_float asc_int642float_rna(
    vector_int64_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

#endif

// ==========asc_bfloat162half(rd/ru/rz/rn/rna)==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rd__vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rd(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rd_sat__vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rd_sat(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_ru__vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_ru(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_ru_sat__vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_ru_sat(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rz__vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rz(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rz_sat__vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rz_sat(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rn__vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rn(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rn_sat__vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rn_sat(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rna__vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rna(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rna_sat__vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_half asc_bfloat162half_rna_sat(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rd__Rvf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rd(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rd_sat__Rvf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rd_sat(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_ru__Rvf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_ru(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_ru_sat__Rvf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_ru_sat(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rz__Rvf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rz(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rz_sat__Rvf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rz_sat(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rn__Rvf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rn(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rn_sat__Rvf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rn_sat(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rna__Rvf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rna(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162half_rna_sat__Rvf16_vbf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`half`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_bfloat162half_rna_sat(vector_half& dst, vector_bfloat16_t src, vector_bool mask);

// ==========asc_half2bfloat16==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rd__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`bfloat16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_half2bfloat16_rd(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rn__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`bfloat16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_half2bfloat16_rn(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rna__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`bfloat16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_half2bfloat16_rna(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_ru__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`bfloat16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_half2bfloat16_ru(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rz__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`bfloat16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_bfloat16_t asc_half2bfloat16_rz(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rd__Rvbf16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`bfloat16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2bfloat16_rd(vector_bfloat16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rn__Rvbf16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`bfloat16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2bfloat16_rn(vector_bfloat16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rna__Rvbf16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`bfloat16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2bfloat16_rna(vector_bfloat16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_ru__Rvbf16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`bfloat16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2bfloat16_ru(vector_bfloat16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2bfloat16_rz__Rvbf16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`bfloat16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2bfloat16_rz(vector_bfloat16_t& dst, vector_half src, vector_bool mask);

// ==========asc_int4x22int16==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int4x22int16__Rvi16_vi4x2_vb
 * @archs 3510
 * @brief 将int4x2_t类型转化为int16_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22int16(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask);

// ==========asc_int4x22bfloat16==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int4x22bfloat16__Rvbf16_vi4x2_vb
 * @archs 3510
 * @brief 将int4x2_t类型转化为bfloat16_t类型，无舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22bfloat16(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask);

// ==========asc_int322float(rd/ru/rz/rn/rna)==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_rd__Rvf32_vi32_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型数据转换成`float`类型，并支持多种舍入模式，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322float_rd(vector_float& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_ru__Rvf32_vi32_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型数据转换成`float`类型，并支持多种舍入模式，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322float_ru(vector_float& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_rz__Rvf32_vi32_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型数据转换成`float`类型，并支持多种舍入模式，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322float_rz(vector_float& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_rn__Rvf32_vi32_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型数据转换成`float`类型，并支持多种舍入模式，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322float_rn(vector_float& dst, vector_int32_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322float_rna__Rvf32_vi32_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型数据转换成`float`类型，并支持多种舍入模式，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_int322float_rna(vector_float& dst, vector_int32_t src, vector_bool mask);

// ==========asc_half2int16(rd/rn/rna/ru/rz)==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rn__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rn(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rn_sat__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rn_sat(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rna__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rna(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rna_sat__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rna_sat(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rd__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rd(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rd_sat__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rd_sat(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_ru__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_ru(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_ru_sat__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_ru_sat(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rz__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rz(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rz_sat__vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_int16_t asc_half2int16_rz_sat(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rn__Rvi16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rn(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rn_sat__Rvi16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rn_sat(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rna__Rvi16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rna(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rna_sat__Rvi16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rna_sat(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rd__Rvi16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rd(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rd_sat__Rvi16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rd_sat(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_ru__Rvi16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_ru(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_ru_sat__Rvi16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_ru_sat(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rz__Rvi16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rz(vector_int16_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int16_rz_sat__Rvi16_vf16_vb
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int16_t`类型，结果写入`dst`。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2int16_rz_sat(vector_int16_t& dst, vector_half src, vector_bool mask);

// ==========asc_half2int4x2==========
// rd - sat:dis/en - v1 v2 v3 v4
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rd__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rd(vector_int4x2_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rd_sat__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rd_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask);

// ru - sat:dis/en - v1 v2 v3 v4
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_ru__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_ru(vector_int4x2_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_ru_sat__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_ru_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask);

// rz - sat:dis/en - v1 v2 v3 v4
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rz__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rz(vector_int4x2_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rz_sat__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rz_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask);

// rn - sat:dis/en - v1 v2 v3 v4
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rn__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rn(vector_int4x2_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rn_sat__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rn_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask);

// rna - sat:dis/en - v1 v2 v3 v4
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rna__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rna(vector_int4x2_t& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rna_sat__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_half2int4x2_rna_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask);

//===================asc_int162half=================
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_rd__Rvf16_vi16_vb
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为half类型（16位浮点数），并将结果写入dst。源操作数和目的操作数位宽相同，转换后元素数量不变。支持5种舍入模式，关于舍入模式的详细说明请参见[舍入模式与饱和模式](rounding_mode.md)。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162half_rd(vector_half& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_rn__Rvf16_vi16_vb
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为half类型（16位浮点数），并将结果写入dst。源操作数和目的操作数位宽相同，转换后元素数量不变。支持5种舍入模式，关于舍入模式的详细说明请参见[舍入模式与饱和模式](rounding_mode.md)。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162half_rn(vector_half& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_rna__Rvf16_vi16_vb
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为half类型（16位浮点数），并将结果写入dst。源操作数和目的操作数位宽相同，转换后元素数量不变。支持5种舍入模式，关于舍入模式的详细说明请参见[舍入模式与饱和模式](rounding_mode.md)。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162half_rna(vector_half& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_ru__Rvf16_vi16_vb
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为half类型（16位浮点数），并将结果写入dst。源操作数和目的操作数位宽相同，转换后元素数量不变。支持5种舍入模式，关于舍入模式的详细说明请参见[舍入模式与饱和模式](rounding_mode.md)。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162half_ru(vector_half& dst, vector_int16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162half_rz__Rvf16_vi16_vb
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为half类型（16位浮点数），并将结果写入dst。源操作数和目的操作数位宽相同，转换后元素数量不变。支持5种舍入模式，关于舍入模式的详细说明请参见[舍入模式与饱和模式](rounding_mode.md)。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int162half_rz(vector_half& dst, vector_int16_t src, vector_bool mask);

//================asc_int4x22half==============
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int4x22half__Rvf16_vi4x2_vb
 * @archs 3510
 * @brief 将int4x2类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_int4x22half(vector_half& dst, vector_int4x2_t src, vector_bool mask);

// ==========asc_float2int32_rd/ru/rz/rn/rna)==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rd__Rvi32_vf32_vb
 * @archs 3510
 * @brief 将`float`类型数据转换为`int32_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rd(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rd_sat__Rvi32_vf32_vb
 * @archs 3510
 * @brief 将`float`类型数据转换为`int32_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rd_sat(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_ru__Rvi32_vf32_vb
 * @archs 3510
 * @brief 将`float`类型数据转换为`int32_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_ru(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_ru_sat__Rvi32_vf32_vb
 * @archs 3510
 * @brief 将`float`类型数据转换为`int32_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_ru_sat(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rz__Rvi32_vf32_vb
 * @archs 3510
 * @brief 将`float`类型数据转换为`int32_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rz(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rz_sat__Rvi32_vf32_vb
 * @archs 3510
 * @brief 将`float`类型数据转换为`int32_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rz_sat(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rn__Rvi32_vf32_vb
 * @archs 3510
 * @brief 将`float`类型数据转换为`int32_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rn(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rn_sat__Rvi32_vf32_vb
 * @archs 3510
 * @brief 将`float`类型数据转换为`int32_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rn_sat(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rna__Rvi32_vf32_vb
 * @archs 3510
 * @brief 将`float`类型数据转换为`int32_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rna(vector_int32_t& dst, vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int32_rna_sat__Rvi32_vf32_vb
 * @archs 3510
 * @brief 将`float`类型数据转换为`int32_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_float2int32_rna_sat(vector_int32_t& dst, vector_float src, vector_bool mask);

// ==========asc_ceil==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_ceil__vf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照CEIL（向正无穷方向舍入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`ceil`语义：向正无穷方向舍入，即取大于或等于输入值的最小整数，正数向数值增大的方向取整（如1.3 -> 2.0），负数向0方向取整（如-1.7 -> -1.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_ceil(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_ceil__vbf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照CEIL（向正无穷方向舍入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`ceil`语义：向正无穷方向舍入，即取大于或等于输入值的最小整数，正数向数值增大的方向取整（如1.3 -> 2.0），负数向0方向取整（如-1.7 -> -1.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_ceil(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_ceil__vf32_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照CEIL（向正无穷方向舍入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`ceil`语义：向正无穷方向舍入，即取大于或等于输入值的最小整数，正数向数值增大的方向取整（如1.3 -> 2.0），负数向0方向取整（如-1.7 -> -1.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_ceil(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_ceil__Rvf16_vf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照CEIL（向正无穷方向舍入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`ceil`语义：向正无穷方向舍入，即取大于或等于输入值的最小整数，正数向数值增大的方向取整（如1.3 -> 2.0），负数向0方向取整（如-1.7 -> -1.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_ceil(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_ceil__Rvbf16_vbf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照CEIL（向正无穷方向舍入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`ceil`语义：向正无穷方向舍入，即取大于或等于输入值的最小整数，正数向数值增大的方向取整（如1.3 -> 2.0），负数向0方向取整（如-1.7 -> -1.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_ceil(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_ceil__Rvf32_vf32_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照CEIL（向正无穷方向舍入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`ceil`语义：向正无穷方向舍入，即取大于或等于输入值的最小整数，正数向数值增大的方向取整（如1.3 -> 2.0），负数向0方向取整（如-1.7 -> -1.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_ceil(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_floor==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_floor__vf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照FLOOR（向负无穷方向舍入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`floor`语义：向负无穷方向舍入，即取小于或等于输入值的最大整数，正数向0方向取整（如1.7 -> 1.0），负数向数值减小的方向取整（如-1.3 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_floor(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_floor__vbf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照FLOOR（向负无穷方向舍入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`floor`语义：向负无穷方向舍入，即取小于或等于输入值的最大整数，正数向0方向取整（如1.7 -> 1.0），负数向数值减小的方向取整（如-1.3 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_floor(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_floor__vf32_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照FLOOR（向负无穷方向舍入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`floor`语义：向负无穷方向舍入，即取小于或等于输入值的最大整数，正数向0方向取整（如1.7 -> 1.0），负数向数值减小的方向取整（如-1.3 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_floor(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_floor__Rvf16_vf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照FLOOR（向负无穷方向舍入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`floor`语义：向负无穷方向舍入，即取小于或等于输入值的最大整数，正数向0方向取整（如1.7 -> 1.0），负数向数值减小的方向取整（如-1.3 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_floor(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_floor__Rvbf16_vbf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照FLOOR（向负无穷方向舍入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`floor`语义：向负无穷方向舍入，即取小于或等于输入值的最大整数，正数向0方向取整（如1.7 -> 1.0），负数向数值减小的方向取整（如-1.3 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_floor(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_floor__Rvf32_vf32_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照FLOOR（向负无穷方向舍入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`floor`语义：向负无穷方向舍入，即取小于或等于输入值的最大整数，正数向0方向取整（如1.7 -> 1.0），负数向数值减小的方向取整（如-1.3 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_floor(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_rint==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_rint__vf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照RINT（四舍六入五成双）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`rint`在舍入方向设置为`FE_TONEAREST`时的语义：舍入到最接近的整数；当输入值恰好位于两个整数中间时，舍入到偶数（如1.5 -> 2.0、2.5 -> 2.0、-2.5 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_rint(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_rint__vbf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照RINT（四舍六入五成双）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`rint`在舍入方向设置为`FE_TONEAREST`时的语义：舍入到最接近的整数；当输入值恰好位于两个整数中间时，舍入到偶数（如1.5 -> 2.0、2.5 -> 2.0、-2.5 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_rint(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_rint__vf32_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照RINT（四舍六入五成双）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`rint`在舍入方向设置为`FE_TONEAREST`时的语义：舍入到最接近的整数；当输入值恰好位于两个整数中间时，舍入到偶数（如1.5 -> 2.0、2.5 -> 2.0、-2.5 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_rint(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_rint__Rvf16_vf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照RINT（四舍六入五成双）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`rint`在舍入方向设置为`FE_TONEAREST`时的语义：舍入到最接近的整数；当输入值恰好位于两个整数中间时，舍入到偶数（如1.5 -> 2.0、2.5 -> 2.0、-2.5 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_rint(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_rint__Rvbf16_vbf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照RINT（四舍六入五成双）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`rint`在舍入方向设置为`FE_TONEAREST`时的语义：舍入到最接近的整数；当输入值恰好位于两个整数中间时，舍入到偶数（如1.5 -> 2.0、2.5 -> 2.0、-2.5 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_rint(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_rint__Rvf32_vf32_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照RINT（四舍六入五成双）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`rint`在舍入方向设置为`FE_TONEAREST`时的语义：舍入到最接近的整数；当输入值恰好位于两个整数中间时，舍入到偶数（如1.5 -> 2.0、2.5 -> 2.0、-2.5 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_rint(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_round==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_round__vf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照ROUND（四舍五入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`round`语义：舍入到最接近的整数；当输入值恰好位于两个整数中间时，向远离0的方向舍入（如1.4 -> 1.0、1.5 -> 2.0、-1.5 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_round(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_round__vbf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照ROUND（四舍五入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`round`语义：舍入到最接近的整数；当输入值恰好位于两个整数中间时，向远离0的方向舍入（如1.4 -> 1.0、1.5 -> 2.0、-1.5 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_round(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_round__vf32_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照ROUND（四舍五入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`round`语义：舍入到最接近的整数；当输入值恰好位于两个整数中间时，向远离0的方向舍入（如1.4 -> 1.0、1.5 -> 2.0、-1.5 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_round(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_round__Rvf16_vf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照ROUND（四舍五入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`round`语义：舍入到最接近的整数；当输入值恰好位于两个整数中间时，向远离0的方向舍入（如1.4 -> 1.0、1.5 -> 2.0、-1.5 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_round(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_round__Rvbf16_vbf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照ROUND（四舍五入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`round`语义：舍入到最接近的整数；当输入值恰好位于两个整数中间时，向远离0的方向舍入（如1.4 -> 1.0、1.5 -> 2.0、-1.5 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_round(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_round__Rvf32_vf32_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照ROUND（四舍五入）舍入模式舍入到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回舍入结果，后者将舍入结果写入`dst`。舍入规则等价于C标准库`round`语义：舍入到最接近的整数；当输入值恰好位于两个整数中间时，向远离0的方向舍入（如1.4 -> 1.0、1.5 -> 2.0、-1.5 -> -2.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的操作数（矢量数据寄存器），用于通过引用和参数输出结果时保存舍入结果。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_round(vector_float& dst, vector_float src, vector_bool mask);

// ==========asc_trunc==========
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_trunc__vf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照TRUNC（向零方向截断）截断到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回截断结果，后者将截断结果写入`dst`。截断规则等价于C标准库`trunc`语义：向零方向舍入，即丢弃小数部分取整，正数向0方向取整（如1.7 -> 1.0），负数向0方向取整（如-1.7 -> -1.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源矢量数据寄存器，dtype须与dst一致，提供待截断的浮点元素。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_half asc_trunc(vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_trunc__vbf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照TRUNC（向零方向截断）截断到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回截断结果，后者将截断结果写入`dst`。截断规则等价于C标准库`trunc`语义：向零方向舍入，即丢弃小数部分取整，正数向0方向取整（如1.7 -> 1.0），负数向0方向取整（如-1.7 -> -1.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源矢量数据寄存器，dtype须与dst一致，提供待截断的浮点元素。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_bfloat16_t asc_trunc(vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_trunc__vf32_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照TRUNC（向零方向截断）截断到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回截断结果，后者将截断结果写入`dst`。截断规则等价于C标准库`trunc`语义：向零方向舍入，即丢弃小数部分取整，正数向0方向取整（如1.7 -> 1.0），负数向0方向取整（如-1.7 -> -1.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [in] src 源矢量数据寄存器，dtype须与dst一致，提供待截断的浮点元素。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline vector_float asc_trunc(vector_float src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_trunc__Rvf16_vf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照TRUNC（向零方向截断）截断到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回截断结果，后者将截断结果写入`dst`。截断规则等价于C标准库`trunc`语义：向零方向舍入，即丢弃小数部分取整，正数向0方向取整（如1.7 -> 1.0），负数向0方向取整（如-1.7 -> -1.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的矢量数据寄存器，dtype须与src一致，掩码为真位置写入向零方向截断结果（仍以原浮点类型表示整数），掩码为假位置清零。
 * @param [in] src 源矢量数据寄存器，dtype须与dst一致，提供待截断的浮点元素。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_trunc(vector_half& dst, vector_half src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_trunc__Rvbf16_vbf16_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照TRUNC（向零方向截断）截断到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回截断结果，后者将截断结果写入`dst`。截断规则等价于C标准库`trunc`语义：向零方向舍入，即丢弃小数部分取整，正数向0方向取整（如1.7 -> 1.0），负数向0方向取整（如-1.7 -> -1.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的矢量数据寄存器，dtype须与src一致，掩码为真位置写入向零方向截断结果（仍以原浮点类型表示整数），掩码为假位置清零。
 * @param [in] src 源矢量数据寄存器，dtype须与dst一致，提供待截断的浮点元素。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_trunc(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_trunc__Rvf32_vf32_vb
 * @archs 3510
 * @brief 将`src`中的浮点数元素按照TRUNC（向零方向截断）截断到整数值，结果仍保持原浮点数据类型。接口支持通过函数返回值返回结果，也支持通过引用和参数输出结果：前者直接返回截断结果，后者将截断结果写入`dst`。截断规则等价于C标准库`trunc`语义：向零方向舍入，即丢弃小数部分取整，正数向0方向取整（如1.7 -> 1.0），负数向0方向取整（如-1.7 -> -1.0），整数值保持不变。未被`mask`筛选的元素置零。
 * @param [out] dst 目的矢量数据寄存器，dtype须与src一致，掩码为真位置写入向零方向截断结果（仍以原浮点类型表示整数），掩码为假位置清零。
 * @param [in] src 源矢量数据寄存器，dtype须与dst一致，提供待截断的浮点元素。
 * @param [in] mask 源掩码寄存器，用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
__simd_callee__ inline void asc_trunc(vector_float& dst, vector_float src, vector_bool mask);

// ==========Position-aware conversion overloads==========
__simd_callee__ inline void asc_float2e4m3_rn(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn_sat(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn_sat(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn_sat(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e4m3_rn_sat(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint32__Rvu32_vi16_vb_icposeven
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为uint32_t类型，并将结果写入dst。转换过程将16位有符号整数转换为32位无符号整数。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162uint32(
    vector_uint32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint32__Rvu32_vi16_vb_icposodd
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为uint32_t类型，并将结果写入dst。转换过程将16位有符号整数转换为32位无符号整数。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162uint32(
    vector_uint32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642int32__Rvi32_vi64_vb_icposeven
 * @archs 3510
 * @brief 将`int64_t`类型的源操作数转换成`int32_t`类型，并支持不同的饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642int32(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642int32__Rvi32_vi64_vb_icposodd
 * @archs 3510
 * @brief 将`int64_t`类型的源操作数转换成`int32_t`类型，并支持不同的饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642int32(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642int32_sat__Rvi32_vi64_vb_icposeven
 * @archs 3510
 * @brief 将`int64_t`类型的源操作数转换成`int32_t`类型，并支持不同的饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642int32_sat(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642int32_sat__Rvi32_vi64_vb_icposodd
 * @archs 3510
 * @brief 将`int64_t`类型的源操作数转换成`int32_t`类型，并支持不同的饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642int32_sat(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rd(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rd(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd_sat__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rd_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd_sat__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rd_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn_sat__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rn_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn_sat__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rn_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna_sat__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rna_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna_sat__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rna_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_ru(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_ru(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru_sat__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_ru_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru_sat__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_ru_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rz(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rz(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz_sat__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rz_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz_sat__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_int8_t asc_half2int8_rz_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rd(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rd(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd_sat__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rd_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rd_sat__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rd_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rn(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rn(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn_sat__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rn_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rn_sat__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rn_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rna(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rna(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna_sat__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rna_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rna_sat__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rna_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_ru(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_ru(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru_sat__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_ru_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_ru_sat__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_ru_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rz(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rz(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz_sat__Rvi8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rz_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int8_rz_sat__Rvi8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int8_rz_sat(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82half__Rvf16_vi8_vb_icposeven
 * @archs 3510
 * @brief 将`int8_t`类型数据转换为`half`类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int82half(
    vector_half& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82half__Rvf16_vi8_vb_icposodd
 * @archs 3510
 * @brief 将`int8_t`类型数据转换为`half`类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int82half(
    vector_half& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint8__Rvu8_vu16_vb_icposeven
 * @archs 3510
 * @brief 将`uint16_t`类型数据转换为`uint8_t`类型，无舍入模式，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint162uint8(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint8__Rvu8_vu16_vb_icposodd
 * @archs 3510
 * @brief 将`uint16_t`类型数据转换为`uint8_t`类型，无舍入模式，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint162uint8(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint8_sat__Rvu8_vu16_vb_icposeven
 * @archs 3510
 * @brief 将`uint16_t`类型数据转换为`uint8_t`类型，无舍入模式，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint162uint8_sat(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint8_sat__Rvu8_vu16_vb_icposodd
 * @archs 3510
 * @brief 将`uint16_t`类型数据转换为`uint8_t`类型，无舍入模式，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint162uint8_sat(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_int82int32(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int82int32(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int82int32(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int82int32(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e2m1x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e2m1x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e2m1x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e2m1x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint16__Rvu16_vi32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型元素转换为`uint16_t`类型，并支持不同的饱和模式，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 目的操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline void asc_int322uint16(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint16__Rvu16_vi32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型元素转换为`uint16_t`类型，并支持不同的饱和模式，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 目的操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline void asc_int322uint16(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint16_sat__Rvu16_vi32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型元素转换为`uint16_t`类型，并支持不同的饱和模式，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 目的操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline void asc_int322uint16_sat(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint16_sat__Rvu16_vi32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型元素转换为`uint16_t`类型，并支持不同的饱和模式，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 目的操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline void asc_int322uint16_sat(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162float__Rvf32_vi16_vb_icposeven
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为float类型，并将结果写入dst。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162float(
    vector_float& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162float__Rvf32_vi16_vb_icposodd
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为float类型，并将结果写入dst。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162float(
    vector_float& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2float__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`float`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_float asc_half2float(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2float__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`float`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_float asc_half2float(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2float__Rvf32_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`float`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2float(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2float__Rvf32_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`float`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2float(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82int16__Rvi16_vi8_vb_icposeven
 * @archs 3510
 * @brief 将`int8_t`类型数据转换为`int16_t`类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int82int16(
    vector_int16_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82int16__Rvi16_vi8_vb_icposodd
 * @archs 3510
 * @brief 将`int8_t`类型数据转换为`int16_t`类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int82int16(
    vector_int16_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_uint82uint32(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_uint82uint32(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_uint82uint32(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_uint82uint32(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_int322uint8(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8_sat(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8_sat(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8_sat(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_int322uint8_sat(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int64__Rvi64_vi32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型元素转换为`int64_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择输入数据索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 源操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline void asc_int322int64(
    vector_int64_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int64__Rvi64_vi32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型元素转换为`int64_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择输入数据索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 源操作数位置。`ASC_POSITION_EVEN`表示偶数索引，`ASC_POSITION_ODD`表示奇数索引。
 */
__simd_callee__ inline void asc_int322int64(
    vector_int64_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rh(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rh(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh_sat__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rh_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh_sat__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rh_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna_sat__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rna_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna_sat__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline vector_hifloat8_t asc_half2hif8_rna_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh__Rvhf8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2hif8_rh(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh__Rvhf8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2hif8_rh(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh_sat__Rvhf8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2hif8_rh_sat(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rh_sat__Rvhf8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2hif8_rh_sat(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna__Rvhf8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2hif8_rna(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna__Rvhf8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2hif8_rna(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna_sat__Rvhf8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2hif8_rna_sat(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2hif8_rna_sat__Rvhf8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`hifloat8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择写入`dst`索引为偶数的位置；取`ASC_POSITION_ODD`时选择写入`dst`索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2hif8_rna_sat(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162int32__Rvi32_vi16_vb_icposeven
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为int32_t类型，并将结果写入dst。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162int32(
    vector_int32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162int32__Rvi32_vi16_vb_icposodd
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为int32_t类型，并将结果写入dst。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162int32(
    vector_int32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162float__vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`float`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_float asc_bfloat162float(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162float__vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`float`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_float asc_bfloat162float(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162float__Rvf32_vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`float`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162float(
    vector_float& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162float__Rvf32_vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`float`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162float(
    vector_float& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint8__Rvu8_vi16_vb_icposeven
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为uint8_t类型，并将结果写入dst。支持非饱和与饱和两种模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162uint8(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint8__Rvu8_vi16_vb_icposodd
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为uint8_t类型，并将结果写入dst。支持非饱和与饱和两种模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162uint8(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint8_sat__Rvu8_vi16_vb_icposeven
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为uint8_t类型，并将结果写入dst。支持非饱和与饱和两种模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162uint8_sat(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int162uint8_sat__Rvu8_vi16_vb_icposodd
 * @archs 3510
 * @brief `src`中int16_t类型的元素转换为uint8_t类型，并将结果写入dst。支持非饱和与饱和两种模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int162uint8_sat(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_e1m2x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e1m2x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e1m2x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e1m2x22bfloat16(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82uint16__Rvu16_vu8_vb_icposeven
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint82uint16](../reg_compute/reg_convert/asc_uint82uint16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择读取源操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint82uint16(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82uint16__Rvu16_vu8_vb_icposodd
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint82uint16](../reg_compute/reg_convert/asc_uint82uint16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择读取源操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint82uint16(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_e5m22float(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e5m22float(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e5m22float(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e5m22float(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn_sat(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn_sat(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn_sat(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2e5m2_rn_sat(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint16__Rvu16_vu32_vb_icposeven
 * @archs 3510
 * @brief 将`uint32_t`类型转换为`uint16_t`类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint322uint16(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint16__Rvu16_vu32_vb_icposodd
 * @archs 3510
 * @brief 将`uint32_t`类型转换为`uint16_t`类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint322uint16(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint16_sat__Rvu16_vu32_vb_icposeven
 * @archs 3510
 * @brief 将`uint32_t`类型转换为`uint16_t`类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint322uint16_sat(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint16_sat__Rvu16_vu32_vb_icposodd
 * @archs 3510
 * @brief 将`uint32_t`类型转换为`uint16_t`类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint322uint16_sat(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_uint322uint8(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8_sat(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8_sat(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8_sat(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_uint322uint8_sat(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_hif82half__Rvf16_vhf8_vb_icposeven
 * @archs 3510
 * @brief `src`中hifloat8_t数据类型的元素转换为half数据类型，并将结果写入dst。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_hif82half(
    vector_half& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_hif82half__Rvf16_vhf8_vb_icposodd
 * @archs 3510
 * @brief `src`中hifloat8_t数据类型的元素转换为half数据类型，并将结果写入dst。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_hif82half(
    vector_half& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int16__Rvi16_vi32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型元素转化为`int16_t`类型，支持饱和/非饱和模式，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int322int16(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int16__Rvi16_vi32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型元素转化为`int16_t`类型，支持饱和/非饱和模式，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int322int16(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int16_sat__Rvi16_vi32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型元素转化为`int16_t`类型，支持饱和/非饱和模式，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int322int16_sat(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322int16_sat__Rvi16_vi32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`int32_t`类型元素转化为`int16_t`类型，支持饱和/非饱和模式，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int322int16_sat(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_e4m32float(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e4m32float(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e4m32float(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_e4m32float(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_ru__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_ru(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_ru__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_ru(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_ru_sat__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_ru_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_ru_sat__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_ru_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rna__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rna(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rna__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rna(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rna_sat__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rna_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rna_sat__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rna_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rd__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rd(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rd__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rd(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rd_sat__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rd_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rd_sat__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rd_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rn__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rn(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rn__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rn(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rn_sat__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rn_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rn_sat__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rn_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rz__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rz(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rz__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rz(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rz_sat__Rvbf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rz_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2bfloat16_rz_sat__Rvbf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个`float`类型数据转换成`bfloat16_t`类型，并支持多种舍入模式与饱和模式，结果写入`dst`。 由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据位置参数选择数据写入索引为奇数的位置或偶数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），类型为`std::integral_constant`或`std::integral_constant`。取`ASC_POSITION_EVEN`时选择将结果写入目的操作数索引为偶数的位置；取`ASC_POSITION_ODD`时选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2bfloat16_rz_sat(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rn__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rn__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_ru__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_ru(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_ru__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_ru(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rd__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rd(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rd__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rd(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rz__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rz(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rz__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rz(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rna__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rna__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_half2int32_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rn__Rvi32_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rn(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rn__Rvi32_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rn(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_ru__Rvi32_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_ru(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_ru__Rvi32_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_ru(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rd__Rvi32_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rd(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rd__Rvi32_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rd(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rz__Rvi32_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rz(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rz__Rvi32_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rz(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rna__Rvi32_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rna(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int32_rna__Rvi32_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_half2int32_rna(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rz__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rz(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rz__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rz(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rz_sat__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rz_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rz_sat__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rz_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rn__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rn(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rn__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rn(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rn_sat__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rn_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rn_sat__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rn_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rna__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rna(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rna__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rna(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rna_sat__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rna_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rna_sat__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rna_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rd__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rd(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rd__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rd(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rd_sat__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rd_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_rd_sat__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_rd_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ro__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_ro(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ro__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_ro(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ro_sat__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_ro_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ro_sat__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_ro_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ru__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_ru(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ru__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_ru(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ru_sat__Rvf16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_ru_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2half_ru_sat__Rvf16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换成`half`类型，并支持多种舍入模式与饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2half_ru_sat(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_float2hif8_rna(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rna_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_float2hif8_rh_sat(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz__vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rz(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz__vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rz(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz_sat__vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rz_sat(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz_sat__vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rz_sat(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn__vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rn(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn__vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rn(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn_sat__vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rn_sat(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn_sat__vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rn_sat(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna__vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rna(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna__vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rna(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna_sat__vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rna_sat(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna_sat__vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rna_sat(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru__vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_ru(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru__vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_ru(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru_sat__vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_ru_sat(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru_sat__vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_ru_sat(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd__vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rd(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd__vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rd(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd_sat__vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rd_sat(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd_sat__vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline vector_int32_t asc_bfloat162int32_rd_sat(
    vector_bfloat16_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rz(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rz(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz_sat__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rz_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rz_sat__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rz_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rn(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rn(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn_sat__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rn_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rn_sat__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rn_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rna(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rna(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna_sat__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rna_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rna_sat__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rna_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_ru(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_ru(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru_sat__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_ru_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_ru_sat__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_ru_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rd(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rd(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd_sat__Rvi32_vbf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rd_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162int32_rd_sat__Rvi32_vbf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`bfloat16_t`类型元素转换为`int32_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为1:2，读取数据时需要将一个`VL`大小的数据分为两部分，根据参数`src_pos`选择读取`src`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 * @param [in] src_pos 位置选择标签（编译器标签类型），类型为std::integral_constant或std::integral_constant。取`ASC_POSITION_EVEN`时选择读取源数据索引为偶数的位置；取`ASC_POSITION_ODD`时选择读取源数据索引为奇数的位置。
 */
__simd_callee__ inline void asc_bfloat162int32_rd_sat(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_hif82float(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_hif82float(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_hif82float(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        src_quarter_pos);

__simd_callee__ inline void asc_hif82float(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        src_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rz__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rz(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rz__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rz(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rz_sat__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rz_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rz_sat__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rz_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rd__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rd(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rd__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rd(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rd_sat__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rd_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rd_sat__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rd_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rna__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rna(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rna__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rna(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rna_sat__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rna_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rna_sat__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rna_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_ru__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_ru(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_ru__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_ru(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_ru_sat__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_ru_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_ru_sat__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_ru_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rn__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rn(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rn__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rn(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rn_sat__Rvi16_vf32_vb_icposeven
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rn_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2int16_rn_sat__Rvi16_vf32_vb_icposodd
 * @archs 3510
 * @brief 将`float`类型数据转换为`int16_t`类型，并支持多种舍入模式与饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_float2int16_rn_sat(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

__simd_callee__ inline void asc_float2int64_rn(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_float2int64_rn(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_float2int64_rn_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_float2int64_rn_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_float2int64_rz(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_float2int64_rz(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_float2int64_rz_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_float2int64_rz_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_float2int64_ru(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_float2int64_ru(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_float2int64_ru_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_float2int64_ru_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_float2int64_rd(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_float2int64_rd(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_float2int64_rd_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_float2int64_rd_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_float2int64_rna(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_float2int64_rna(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_float2int64_rna_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

__simd_callee__ inline void asc_float2int64_rna_sat(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322int16__Rvi16_vu32_vb_icposeven
 * @archs 3510
 * @brief 将`uint32_t`类型数据转为`int16_t`类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint322int16(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322int16__Rvi16_vu32_vb_icposodd
 * @archs 3510
 * @brief 将`uint32_t`类型数据转为`int16_t`类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint322int16(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322int16_sat__Rvi16_vu32_vb_icposeven
 * @archs 3510
 * @brief 将`uint32_t`类型数据转为`int16_t`类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint322int16_sat(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322int16_sat__Rvi16_vu32_vb_icposodd
 * @archs 3510
 * @brief 将`uint32_t`类型数据转为`int16_t`类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint322int16_sat(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82half__Rvf16_vu8_vb_icposeven
 * @archs 3510
 * @brief 将`uint8_t`类型数据转为`half`类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint82half(
    vector_half& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82half__Rvf16_vu8_vb_icposodd
 * @archs 3510
 * @brief 将`uint8_t`类型数据转为`half`类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint82half(
    vector_half& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint32__Rvu32_vu16_vb_icposeven
 * @archs 3510
 * @brief 将`uint16_t`类型数据转为`uint32_t`类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint162uint32(
    vector_uint32_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint162uint32__Rvu32_vu16_vb_icposodd
 * @archs 3510
 * @brief 将`uint16_t`类型数据转为`uint32_t`类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] src_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择读取源操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择读取源操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_uint162uint32(
    vector_uint32_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rn(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rn(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rn(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rn(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_ru(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_ru(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_ru(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_ru(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rd(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rd(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rd(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rd(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rna(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rna(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rna(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rna(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rz(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rz(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rz(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e1m2x2_rz(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rd(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rd(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rd(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rd(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rz(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rz(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rz(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rz(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rn(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rn(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rn(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rn(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_ru(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_ru(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_ru(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_ru(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rna(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rna(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rna(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos);

__simd_callee__ inline void asc_bfloat162e2m1x2_rna(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_ru__Rvf32_vi64_vb_icposeven
 * @archs 3510
 * @brief 将`int64_t`类型数据转换为`float`，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642float_ru(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_ru__Rvf32_vi64_vb_icposodd
 * @archs 3510
 * @brief 将`int64_t`类型数据转换为`float`，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642float_ru(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rd__Rvf32_vi64_vb_icposeven
 * @archs 3510
 * @brief 将`int64_t`类型数据转换为`float`，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642float_rd(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rd__Rvf32_vi64_vb_icposodd
 * @archs 3510
 * @brief 将`int64_t`类型数据转换为`float`，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642float_rd(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rn__Rvf32_vi64_vb_icposeven
 * @archs 3510
 * @brief 将`int64_t`类型数据转换为`float`，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642float_rn(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rn__Rvf32_vi64_vb_icposodd
 * @archs 3510
 * @brief 将`int64_t`类型数据转换为`float`，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642float_rn(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rz__Rvf32_vi64_vb_icposeven
 * @archs 3510
 * @brief 将`int64_t`类型数据转换为`float`，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642float_rz(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rz__Rvf32_vi64_vb_icposodd
 * @archs 3510
 * @brief 将`int64_t`类型数据转换为`float`，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642float_rz(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rna__Rvf32_vi64_vb_icposeven
 * @archs 3510
 * @brief 将`int64_t`类型数据转换为`float`，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642float_rna(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int642float_rna__Rvf32_vi64_vb_icposodd
 * @archs 3510
 * @brief 将`int64_t`类型数据转换为`float`，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 * @param [in] dst_pos 位置选择标签（编译器标签类型），取值如下：&bull; `ASC_POSITION_EVEN`：选择将结果写入目的操作数索引为偶数的位置&bull; `ASC_POSITION_ODD`：选择将结果写入目的操作数索引为奇数的位置。
 */
__simd_callee__ inline void asc_int642float_rna(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rd(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rd(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd_sat__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rd_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd_sat__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rd_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_ru(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_ru(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru_sat__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_ru_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru_sat__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_ru_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rna(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna_sat__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rna_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna_sat__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rna_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn_sat__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rn_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn_sat__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rn_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rz(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rz(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz_sat__vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rz_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz_sat__vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline vector_uint8_t asc_half2uint8_rz_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rd(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rd(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd_sat__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rd_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rd_sat__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rd_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_ru(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_ru(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru_sat__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_ru_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_ru_sat__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_ru_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rna(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rna(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna_sat__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rna_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rna_sat__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rna_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rn(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rn(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn_sat__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rn_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rn_sat__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rn_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rz(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rz(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz_sat__Rvu8_vf16_vb_icposeven
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rz_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos);

/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2uint8_rz_sat__Rvu8_vf16_vb_icposodd
 * @archs 3510
 * @brief 根据`mask`将`src`中的每个参与计算的`half`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数类型位宽比为2:1，写入数据时需要将一个`VL`大小的数据分为两部分，根据参数`dst_pos`选择写入`dst`索引为偶数的位置或奇数的位置。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
__simd_callee__ inline void asc_half2uint8_rz_sat(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos);

// ==========Deprecated conversion interfaces==========

// Legacy overloads without explicit position arguments.
[[deprecated("NOTICE: asc_float2half_rd(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rd(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rd__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_float2half_rd(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_rd_sat(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rd_sat(vector_half& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rd_sat__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2half_rd_sat(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_ru(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_ru(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ru__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_float2half_ru(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_ru_sat(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_ru_sat(vector_half& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ru_sat__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2half_ru_sat(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_rz(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rz(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rz__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_float2half_rz(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_rz_sat(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rz_sat(vector_half& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rz_sat__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2half_rz_sat(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_rn(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rn(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rn__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_float2half_rn(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_rn_sat(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rn_sat(vector_half& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rn_sat__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2half_rn_sat(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_rna(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rna(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rna__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_float2half_rna(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_rna_sat(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rna_sat(vector_half& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rna_sat__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2half_rna_sat(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_ro(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_ro(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ro__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_float2half_ro(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_ro_sat(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_ro_sat(vector_half& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ro_sat__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2half_ro_sat(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322int16(vector_int16_t& dst, vector_uint32_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint322int16(vector_int16_t& dst, vector_uint32_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322int16__Rvi16_vu32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint322int16](../reg_compute/reg_convert/asc_uint322int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint322int16(vector_int16_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322int16_sat(vector_int16_t& dst, vector_uint32_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint322int16_sat(vector_int16_t& dst, vector_uint32_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322int16_sat__Rvi16_vu32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint322int16](../reg_compute/reg_convert/asc_uint322int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint322int16_sat(vector_int16_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask) is deprecated. "
             "Please use asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e4m32float__Rvf32_vf8e4m3_vb
 * @archs 3510
 * @brief 将fp8_e4m3fn_t类型转化为float类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int162float(vector_float& dst, vector_int16_t src, vector_bool mask) is deprecated. "
             "Please use asc_int162float(vector_float& dst, vector_int16_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162float__Rvf32_vi16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int162float](../reg_compute/reg_convert/asc_int162float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_int162float(vector_float& dst, vector_int16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322int16(vector_int16_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322int16(vector_int16_t& dst, vector_int32_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322int16__Rvi16_vi32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int322int16](../reg_compute/reg_convert/asc_int322int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_int322int16(vector_int16_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322int16_sat(vector_int16_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322int16_sat(vector_int16_t& dst, vector_int32_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322int16_sat__Rvi16_vi32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int322int16](../reg_compute/reg_convert/asc_int322int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int322int16_sat(vector_int16_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322uint8(vector_uint8_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322uint8(vector_uint8_t& dst, vector_int32_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint8__Rvu8_vi32_vb
 * @archs 3510
 * @brief 将int32_t类型转化为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_int322uint8(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322uint8_sat(vector_uint8_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322uint8_sat(vector_uint8_t& dst, vector_int32_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint8_sat__Rvu8_vi32_vb
 * @archs 3510
 * @brief 将int32_t类型转化为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_int322uint8_sat(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rd(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rd(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rd__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2int8_rd(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rd_sat(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rd_sat(vector_int8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rd_sat__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2int8_rd_sat(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_ru(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_ru(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_ru__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2int8_ru(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_ru_sat(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_ru_sat(vector_int8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_ru_sat__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2int8_ru_sat(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rz(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rz(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rz__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2int8_rz(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rz_sat(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rz_sat(vector_int8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rz_sat__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2int8_rz_sat(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rn(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rn(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rn__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2int8_rn(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rn_sat(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rn_sat(vector_int8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rn_sat__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2int8_rn_sat(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rna(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rna(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rna__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2int8_rna(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rna_sat(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rna_sat(vector_int8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rna_sat__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2int8_rna_sat(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2hif8_rh(vector_hifloat8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2hif8_rh(vector_hifloat8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rh__Rvhf8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2hif8](../reg_compute/reg_convert/asc_half2hif8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2hif8_rh(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2hif8_rh_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2hif8_rh_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rh_sat__Rvhf8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2hif8](../reg_compute/reg_convert/asc_half2hif8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2hif8_rh_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2hif8_rna(vector_hifloat8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2hif8_rna(vector_hifloat8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rna__Rvhf8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2hif8](../reg_compute/reg_convert/asc_half2hif8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2hif8_rna(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2hif8_rna_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2hif8_rna_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rna_sat__Rvhf8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2hif8](../reg_compute/reg_convert/asc_half2hif8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2hif8_rna_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_bfloat162float(vector_float& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
             "Please use asc_bfloat162float(vector_float& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162float__Rvf32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162float](../reg_compute/reg_convert/asc_bfloat162float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162float(vector_float& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int162uint32(vector_uint32_t& dst, vector_int16_t src, vector_bool mask) is deprecated. "
             "Please use asc_int162uint32(vector_uint32_t& dst, vector_int16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162uint32__Rvu32_vi16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int162uint32](../reg_compute/reg_convert/asc_int162uint32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int162uint32(vector_uint32_t& dst, vector_int16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint162uint8(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint162uint8(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint162uint8__Rvu8_vu16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint162uint8](../reg_compute/reg_convert/asc_uint162uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint162uint8(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint162uint8_sat(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint162uint8_sat(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint162uint8_sat__Rvu8_vu16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint162uint8](../reg_compute/reg_convert/asc_uint162uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint162uint8_sat(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rn(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rn(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rn__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rn(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rn_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rn_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rn_sat__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rn_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rna(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rna(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rna__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rna(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rna_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rna_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rna_sat__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rna_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rd(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rd(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rd__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rd(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rd_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rd_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rd_sat__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rd_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_ru(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_ru(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_ru__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "instead.")]] __simd_callee__ inline void
asc_bfloat162int32_ru(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_ru_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_ru_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_ru_sat__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162int32_ru_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rz(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rz(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rz__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rz(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rz_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rz_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rz_sat__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rz_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642float_rn(vector_float& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642float_rn(vector_float& dst, vector_int64_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rn__Rvf32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642float](../reg_compute/reg_convert/asc_int642float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int642float_rn(vector_float& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642float_rna(vector_float& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642float_rna(vector_float& dst, vector_int64_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rna__Rvf32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642float](../reg_compute/reg_convert/asc_int642float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int642float_rna(vector_float& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642float_rd(vector_float& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642float_rd(vector_float& dst, vector_int64_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rd__Rvf32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642float](../reg_compute/reg_convert/asc_int642float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int642float_rd(vector_float& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642float_ru(vector_float& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642float_ru(vector_float& dst, vector_int64_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_ru__Rvf32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642float](../reg_compute/reg_convert/asc_int642float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int642float_ru(vector_float& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642float_rz(vector_float& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642float_rz(vector_float& dst, vector_int64_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rz__Rvf32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642float](../reg_compute/reg_convert/asc_int642float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int642float_rz(vector_float& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int82half(vector_half& dst, vector_int8_t src, vector_bool mask) is deprecated. "
             "Please use asc_int82half(vector_half& dst, vector_int8_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int82half__Rvf16_vi8_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int82half](../reg_compute/reg_convert/asc_int82half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_int82half(vector_half& dst, vector_int8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int162int32(vector_int32_t& dst, vector_int16_t src, vector_bool mask) is deprecated. "
             "Please use asc_int162int32(vector_int32_t& dst, vector_int16_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162int32__Rvi32_vi16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int162int32](../reg_compute/reg_convert/asc_int162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_int162int32(vector_int32_t& dst, vector_int16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322uint8(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint322uint8(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint8__Rvu8_vu32_vb
 * @archs 3510
 * @brief 将uint32_t类型数据转换为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_uint322uint8(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322uint8_sat(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint322uint8_sat(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint8_sat__Rvu8_vu32_vb
 * @archs 3510
 * @brief 将uint32_t类型数据转换为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_uint322uint8_sat(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask) is deprecated. "
             "Please use asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_hif82half__Rvf16_vhf8_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_hif82half](../reg_compute/reg_convert/asc_hif82half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint82half(vector_half& dst, vector_uint8_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint82half(vector_half& dst, vector_uint8_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint82half__Rvf16_vu8_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint82half](../reg_compute/reg_convert/asc_uint82half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_uint82half(vector_half& dst, vector_uint8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint162uint32(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint162uint32(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint162uint32__Rvu32_vu16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint162uint32](../reg_compute/reg_convert/asc_uint162uint32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint162uint32(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_hif82float(vector_float& dst, vector_hifloat8_t src, vector_bool mask) is deprecated. "
             "Please use asc_hif82float(vector_float& dst, vector_hifloat8_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_hif82float__Rvf32_vhf8_vb
 * @archs 3510
 * @brief 将hifloat8_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_hif82float(vector_float& dst, vector_hifloat8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_rd(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rd(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rd__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2uint8_rd(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_rd_sat(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rd_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rd_sat__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2uint8_rd_sat(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_rn(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rn(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rn__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2uint8_rn(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_rn_sat(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rn_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rn_sat__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2uint8_rn_sat(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_rna(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rna(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rna__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2uint8_rna(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_rna_sat(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rna_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rna_sat__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2uint8_rna_sat(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_ru(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_ru(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_ru__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2uint8_ru(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_ru_sat(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_ru_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_ru_sat__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2uint8_ru_sat(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_rz(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rz(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rz__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2uint8_rz(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_rz_sat(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rz_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rz_sat__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2uint8_rz_sat(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_int82int16(vector_int16_t& dst, vector_int8_t src, vector_bool mask) is deprecated. "
             "Please use asc_int82int16(vector_int16_t& dst, vector_int8_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int82int16__Rvi16_vi8_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int82int16](../reg_compute/reg_convert/asc_int82int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_int82int16(vector_int16_t& dst, vector_int8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int162uint8(vector_uint8_t& dst, vector_int16_t src, vector_bool mask) is deprecated. "
             "Please use asc_int162uint8(vector_uint8_t& dst, vector_int16_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162uint8__Rvu8_vi16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int162uint8](../reg_compute/reg_convert/asc_int162uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_int162uint8(vector_uint8_t& dst, vector_int16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int162uint8_sat(vector_uint8_t& dst, vector_int16_t src, vector_bool mask) is deprecated. "
             "Please use asc_int162uint8_sat(vector_uint8_t& dst, vector_int16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162uint8_sat__Rvu8_vi16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int162uint8](../reg_compute/reg_convert/asc_int162uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int162uint8_sat(vector_uint8_t& dst, vector_int16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_float2bfloat16_rd(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2bfloat16_rd(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rd__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rd(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rd_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2bfloat16_rd_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rd_sat__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rd_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2bfloat16_rn(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2bfloat16_rn(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rn__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rn(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rn_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2bfloat16_rn_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rn_sat__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rn_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rna(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2bfloat16_rna(vector_bfloat16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rna__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rna(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rna_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2bfloat16_rna_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rna_sat__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rna_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2bfloat16_ru(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2bfloat16_ru(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_ru__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_ru(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_ru_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2bfloat16_ru_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_ru_sat__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_ru_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2bfloat16_rz(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2bfloat16_rz(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rz__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rz(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rz_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2bfloat16_rz_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rz_sat__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rz_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int32_rd(vector_int32_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int32_rd(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rd__Rvi32_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int32](../reg_compute/reg_convert/asc_half2int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2int32_rd(vector_int32_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int32_rn(vector_int32_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int32_rn(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rn__Rvi32_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int32](../reg_compute/reg_convert/asc_half2int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2int32_rn(vector_int32_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int32_rna(vector_int32_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int32_rna(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rna__Rvi32_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int32](../reg_compute/reg_convert/asc_half2int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2int32_rna(vector_int32_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int32_ru(vector_int32_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int32_ru(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_ru__Rvi32_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int32](../reg_compute/reg_convert/asc_half2int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2int32_ru(vector_int32_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int32_rz(vector_int32_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int32_rz(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rz__Rvi32_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int32](../reg_compute/reg_convert/asc_half2int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2int32_rz(vector_int32_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask) is deprecated. "
             "Please use asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e5m22float__Rvf32_vf8e5m2_vb
 * @archs 3510
 * @brief 将vector_fp8_e5m2_t类型的源操作数以256B为单位分为四部分，读取其中一部分元素，将其转换成vector_float类型并写入目的操作数。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322int64(vector_int64_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322int64(vector_int64_t& dst, vector_int32_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322int64__Rvi64_vi32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int322int64](../reg_compute/reg_convert/asc_int322int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_int322int64(vector_int64_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322uint16(vector_uint16_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322uint16(vector_uint16_t& dst, vector_int32_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint16__Rvu16_vi32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int322uint16](../reg_compute/reg_convert/asc_int322uint16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int322uint16(vector_uint16_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322uint16_sat(vector_uint16_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322uint16_sat(vector_uint16_t& dst, vector_int32_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint16_sat__Rvu16_vi32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int322uint16](../reg_compute/reg_convert/asc_int322uint16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int322uint16_sat(vector_uint16_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642int32(vector_int32_t& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642int32(vector_int32_t& dst, vector_int64_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642int32__Rvi32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642int32](../reg_compute/reg_convert/asc_int642int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "instead.")]] __simd_callee__ inline void
asc_int642int32(vector_int32_t& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642int32_sat(vector_int32_t& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642int32_sat(vector_int32_t& dst, vector_int64_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642int32_sat__Rvi32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642int32](../reg_compute/reg_convert/asc_int642int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int642int32_sat(vector_int32_t& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint82uint16(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint82uint16(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint82uint16(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_rd(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rd(vector_int64_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rd__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rd(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_rd_sat(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rd_sat(vector_int64_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rd_sat__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rd_sat(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_rn(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rn(vector_int64_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rn__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rn(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_rn_sat(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rn_sat(vector_int64_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rn_sat__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rn_sat(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_rna(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rna(vector_int64_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rna__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rna(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_rna_sat(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rna_sat(vector_int64_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rna_sat__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rna_sat(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_ru(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_ru(vector_int64_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_ru__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_ru(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_ru_sat(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_ru_sat(vector_int64_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_ru_sat__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_ru_sat(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_rz(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rz(vector_int64_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rz__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rz(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_rz_sat(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rz_sat(vector_int64_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rz_sat__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rz_sat(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2e4m3_rn_sat(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2e4m3_rn_sat(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e4m3_rn_sat__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为fp8_e4m3fn_t类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2e4m3_rn_sat(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e4m3_rn__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为fp8_e4m3fn_t类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_rd(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rd(vector_int16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rd__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rd(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_rd_sat(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rd_sat(vector_int16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rd_sat__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rd_sat(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_rn(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rn(vector_int16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rn__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rn(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_rn_sat(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rn_sat(vector_int16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rn_sat__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rn_sat(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_rna(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rna(vector_int16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rna__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rna(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_rna_sat(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rna_sat(vector_int16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rna_sat__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rna_sat(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_ru(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_ru(vector_int16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_ru__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_ru(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_ru_sat(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_ru_sat(vector_int16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_ru_sat__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_ru_sat(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_rz(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rz(vector_int16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rz__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rz(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_rz_sat(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rz_sat(vector_int16_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rz_sat__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rz_sat(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rd(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_rd(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rd__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rd(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rn(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_rn(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rn__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rn(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rna(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_rna(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rna__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rna(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_ru(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_ru(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_ru__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_ru(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rz(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_rz(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rz__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rz(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_float2hif8_rh_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2hif8_rh_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rh_sat__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2hif8_rh_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2hif8_rh(vector_hifloat8_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2hif8_rh(vector_hifloat8_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rh__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2hif8_rh(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2hif8_rna_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2hif8_rna_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rna_sat__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2hif8_rna_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2hif8_rna(vector_hifloat8_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2hif8_rna(vector_hifloat8_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rna__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2hif8_rna(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_uint82uint32(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint82uint32(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82uint32__Rvu32_vu8_vb
 * @archs 3510
 * @brief 将uint8_t类型转化为uint32_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_uint82uint32(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_uint322uint16_sat(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask) is deprecated. "
    "Please use asc_uint322uint16_sat(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint16_sat__Rvu16_vu32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint322uint16](../reg_compute/reg_convert/asc_uint322uint16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "instead.")]] __simd_callee__ inline void
asc_uint322uint16_sat(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322uint16(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint322uint16(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint16__Rvu16_vu32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint322uint16](../reg_compute/reg_convert/asc_uint322uint16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint322uint16(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_e2m1x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask) is deprecated. "
    "Please use asc_e2m1x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e2m1x22bfloat16__Rvbf16_vf4e2m1x2_vb
 * @archs 3510
 * @brief 将fp4x2_e2m1_t类型转化为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（矢量寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_e2m1x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rd(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_rd(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rd__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rd(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rn(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_rn(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rn__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rn(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rna(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_rna(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rna__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rna(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_ru(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_ru(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_ru__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_ru(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rz(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_rz(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rz__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rz(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask) is deprecated. "
    "Please use asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e1m2x22bfloat16__Rvbf16_vf4e1m2x2_vb
 * @archs 3510
 * @brief 将fp4x2_e1m2_t类型转化为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int82int32(vector_int32_t& dst, vector_int8_t src, vector_bool mask) is deprecated. "
             "Please use asc_int82int32(vector_int32_t& dst, vector_int8_t src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82int32__Rvi32_vi8_vb
 * @archs 3510
 * @brief 将int8_t类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_int82int32(vector_int32_t& dst, vector_int8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_float2e5m2_rn(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2e5m2_rn(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e5m2_rn__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @brief 将float类型数据转换为fp8_e5m2类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2e5m2_rn(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2e5m2_rn_sat(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2e5m2_rn_sat(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask, "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e5m2_rn_sat__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @brief 将float类型数据转换为fp8_e5m2类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2e5m2_rn_sat(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_half2float(vector_float& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2float(vector_float& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2float__Rvf32_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2float](../reg_compute/reg_convert/asc_half2float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "instead.")]] __simd_callee__ inline void
asc_half2float(vector_float& dst, vector_half src, vector_bool mask);
[[deprecated("NOTICE: asc_float2half_rd_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rd_v2__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2half_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rd_v2(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2half_rd_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rd_sat_v2__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2half_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rd_sat_v2(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_ru_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ru_v2__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2half_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_ru_v2(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2half_ru_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ru_sat_v2__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2half_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_ru_sat_v2(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_rz_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rz_v2__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2half_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rz_v2(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2half_rz_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rz_sat_v2__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2half_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rz_sat_v2(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rn_v2__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2half_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rn_v2(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2half_rn_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rn_sat_v2__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2half_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rn_sat_v2(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rna_v2__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2half_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rna_v2(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2half_rna_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_rna_sat_v2__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2half_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rna_sat_v2(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2half_ro_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ro_v2__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2half_ro_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_ro_v2(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2half_ro_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2half_ro_sat_v2__Rvf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2half](../reg_compute/reg_convert/asc_float2half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2half_ro_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_ro_sat_v2(vector_half& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322int16_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322int16_v2__Rvi16_vu32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint322int16](../reg_compute/reg_convert/asc_uint322int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_uint322int16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322int16_v2(vector_int16_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322int16_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322int16_sat_v2__Rvi16_vu32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint322int16](../reg_compute/reg_convert/asc_uint322int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_uint322int16_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322int16_sat_v2(vector_int16_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_e4m32float_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e4m32float_v2__Rvf32_vf8e4m3_vb
 * @archs 3510
 * @brief 将fp8_e4m3fn_t类型转化为float类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_e4m32float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e4m32float_v2(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

[[deprecated("NOTICE: asc_e4m32float_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e4m32float_v3__Rvf32_vf8e4m3_vb
 * @archs 3510
 * @brief 将fp8_e4m3fn_t类型转化为float类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_e4m32float_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e4m32float_v3(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

[[deprecated("NOTICE: asc_e4m32float_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e4m32float_v4__Rvf32_vf8e4m3_vb
 * @archs 3510
 * @brief 将fp8_e4m3fn_t类型转化为float类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_e4m32float_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e4m32float_v4(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int162float_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162float_v2__Rvf32_vi16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int162float](../reg_compute/reg_convert/asc_int162float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int162float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int162float_v2(vector_float& dst, vector_int16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int4x22int16_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int4x22int16_v2__Rvi16_vi4x2_vb
 * @archs 3510
 * @brief 将int4x2_t类型转化为int16_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int4x22int16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22int16_v2(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int4x22int16_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int4x22int16_v3__Rvi16_vi4x2_vb
 * @archs 3510
 * @brief 将int4x2_t类型转化为int16_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int4x22int16_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22int16_v3(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int4x22int16_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int4x22int16_v4__Rvi16_vi4x2_vb
 * @archs 3510
 * @brief 将int4x2_t类型转化为int16_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int4x22int16_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22int16_v4(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322int16_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322int16_v2__Rvi16_vi32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int322int16](../reg_compute/reg_convert/asc_int322int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_int322int16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322int16_v2(vector_int16_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322int16_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322int16_sat_v2__Rvi16_vi32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int322int16](../reg_compute/reg_convert/asc_int322int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_int322int16_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322int16_sat_v2(vector_int16_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322uint8_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint8_v2__Rvu8_vi32_vb
 * @archs 3510
 * @brief 将int32_t类型转化为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "Currently asc_int322uint8_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint8_v2(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322uint8_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint8_sat_v2__Rvu8_vi32_vb
 * @archs 3510
 * @brief 将int32_t类型转化为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "Currently asc_int322uint8_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint8_sat_v2(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322uint8_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint8_v3__Rvu8_vi32_vb
 * @archs 3510
 * @brief 将int32_t类型转化为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "Currently asc_int322uint8_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint8_v3(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322uint8_sat_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint8_sat_v3__Rvu8_vi32_vb
 * @archs 3510
 * @brief 将int32_t类型转化为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "Currently asc_int322uint8_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint8_sat_v3(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322uint8_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint8_v4__Rvu8_vi32_vb
 * @archs 3510
 * @brief 将int32_t类型转化为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "Currently asc_int322uint8_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint8_v4(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322uint8_sat_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int322uint8_sat_v4__Rvu8_vi32_vb
 * @archs 3510
 * @brief 将int32_t类型转化为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "Currently asc_int322uint8_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint8_sat_v4(vector_uint8_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rd_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rd_v2__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int8_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rd_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rd_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rd_sat_v2__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int8_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rd_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_ru_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_ru_v2__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int8_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_ru_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_ru_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_ru_sat_v2__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int8_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_ru_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rz_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rz_v2__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int8_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rz_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rz_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rz_sat_v2__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int8_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rz_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rn_v2__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int8_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rn_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rn_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rn_sat_v2__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int8_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rn_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int8_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rna_v2__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int8_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rna_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int8_rna_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int8_rna_sat_v2__Rvi8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int8](../reg_compute/reg_convert/asc_half2int8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_half2int8_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rna_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2hif8_rh_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rh_v2__Rvhf8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2hif8](../reg_compute/reg_convert/asc_half2hif8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2hif8_rh_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2hif8_rh_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2hif8_rh_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rh_sat_v2__Rvhf8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2hif8](../reg_compute/reg_convert/asc_half2hif8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2hif8_rh_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2hif8_rh_sat_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2hif8_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rna_v2__Rvhf8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2hif8](../reg_compute/reg_convert/asc_half2hif8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2hif8_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2hif8_rna_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2hif8_rna_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2hif8_rna_sat_v2__Rvhf8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2hif8](../reg_compute/reg_convert/asc_half2hif8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_half2hif8_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2hif8_rna_sat_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_bfloat162float_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162float_v2__Rvf32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162float](../reg_compute/reg_convert/asc_bfloat162float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_bfloat162float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162float_v2(vector_float& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int4x22bfloat16_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int4x22bfloat16_v2__Rvbf16_vi4x2_vb
 * @archs 3510
 * @brief 将int4x2_t类型转化为bfloat16_t类型，无舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int4x22bfloat16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22bfloat16_v2(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int4x22bfloat16_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int4x22bfloat16_v3__Rvbf16_vi4x2_vb
 * @archs 3510
 * @brief 将int4x2_t类型转化为bfloat16_t类型，无舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int4x22bfloat16_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22bfloat16_v3(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int4x22bfloat16_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int4x22bfloat16_v4__Rvbf16_vi4x2_vb
 * @archs 3510
 * @brief 将int4x2_t类型转化为bfloat16_t类型，无舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int4x22bfloat16_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22bfloat16_v4(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int162uint32_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162uint32_v2__Rvu32_vi16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int162uint32](../reg_compute/reg_convert/asc_int162uint32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int162uint32_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int162uint32_v2(vector_uint32_t& dst, vector_int16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint162uint8_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint162uint8_v2__Rvu8_vu16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint162uint8](../reg_compute/reg_convert/asc_uint162uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_uint162uint8_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint162uint8_v2(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint162uint8_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint162uint8_sat_v2__Rvu8_vu16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint162uint8](../reg_compute/reg_convert/asc_uint162uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_uint162uint8_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint162uint8_sat_v2(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rn_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_bfloat162int32_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rn_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rn_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rn_sat_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_bfloat162int32_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rn_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rna_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_bfloat162int32_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rna_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rna_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rna_sat_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_bfloat162int32_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rna_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rd_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rd_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_bfloat162int32_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rd_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rd_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rd_sat_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_bfloat162int32_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rd_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_ru_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_ru_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_bfloat162int32_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_ru_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_ru_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_ru_sat_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_bfloat162int32_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_ru_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rz_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rz_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_bfloat162int32_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rz_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162int32_rz_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_bfloat162int32_rz_sat_v2__Rvi32_vbf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_bfloat162int32](../reg_compute/reg_convert/asc_bfloat162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_bfloat162int32_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rz_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642float_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rn_v2__Rvf32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642float](../reg_compute/reg_convert/asc_int642float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int642float_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642float_rn_v2(vector_float& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642float_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rna_v2__Rvf32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642float](../reg_compute/reg_convert/asc_int642float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int642float_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642float_rna_v2(vector_float& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642float_rd_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rd_v2__Rvf32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642float](../reg_compute/reg_convert/asc_int642float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int642float_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642float_rd_v2(vector_float& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642float_ru_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_ru_v2__Rvf32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642float](../reg_compute/reg_convert/asc_int642float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int642float_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642float_ru_v2(vector_float& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642float_rz_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642float_rz_v2__Rvf32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642float](../reg_compute/reg_convert/asc_int642float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int642float_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642float_rz_v2(vector_float& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int82half_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int82half_v2__Rvf16_vi8_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int82half](../reg_compute/reg_convert/asc_int82half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int82half_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int82half_v2(vector_half& dst, vector_int8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int162int32_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162int32_v2__Rvi32_vi16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int162int32](../reg_compute/reg_convert/asc_int162int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int162int32_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int162int32_v2(vector_int32_t& dst, vector_int16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322uint8_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint8_v2__Rvu8_vu32_vb
 * @archs 3510
 * @brief 将uint32_t类型数据转换为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "Currently asc_uint322uint8_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint8_v2(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322uint8_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint8_sat_v2__Rvu8_vu32_vb
 * @archs 3510
 * @brief 将uint32_t类型数据转换为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "Currently asc_uint322uint8_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint8_sat_v2(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322uint8_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint8_v3__Rvu8_vu32_vb
 * @archs 3510
 * @brief 将uint32_t类型数据转换为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "Currently asc_uint322uint8_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint8_v3(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322uint8_sat_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint8_sat_v3__Rvu8_vu32_vb
 * @archs 3510
 * @brief 将uint32_t类型数据转换为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "Currently asc_uint322uint8_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint8_sat_v3(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322uint8_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint8_v4__Rvu8_vu32_vb
 * @archs 3510
 * @brief 将uint32_t类型数据转换为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "Currently asc_uint322uint8_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint8_v4(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322uint8_sat_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint322uint8_sat_v4__Rvu8_vu32_vb
 * @archs 3510
 * @brief 将uint32_t类型数据转换为uint8_t类型，支持饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。`mask`未筛选的元素在输出中置零。
 */
             "Currently asc_uint322uint8_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint8_sat_v4(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_hif82half_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_hif82half_v2__Rvf16_vhf8_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_hif82half](../reg_compute/reg_convert/asc_hif82half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_hif82half_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_hif82half_v2(vector_half& dst, vector_hifloat8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_rd_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rd_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_rd_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rd_sat_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_rd_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rd_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_rd_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_rd_sat_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rd_sat_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_rd_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_rd_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rd_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_rd_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_rd_sat_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rd_sat_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_rd_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_ru_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_ru_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_ru_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_ru_sat_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_ru_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_ru_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_ru_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_ru_sat_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_ru_sat_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_ru_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_ru_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_ru_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_ru_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_ru_sat_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_ru_sat_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_ru_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_rz_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rz_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_rz_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rz_sat_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_rz_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rz_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_rz_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_rz_sat_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rz_sat_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_rz_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_rz_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rz_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_rz_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_rz_sat_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rz_sat_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_rz_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rn_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_rn_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rn_sat_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_rn_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rn_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_rn_sat_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rn_sat_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_rn_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_rn_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rn_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_rn_sat_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rn_sat_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_rn_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rna_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_rna_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rna_sat_v2__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_rna_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rna_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_rna_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_rna_sat_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rna_sat_v3__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_rna_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int4x2_rna_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rna_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_half2int4x2_rna_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2int4x2_rna_sat_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_half2int4x2_rna_sat_v4__Rvi4x2_vf16_vb
 * @archs 3510
 * @brief 将half类型数据转为int4x2类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_half2int4x2_rna_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_uint82half_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint82half_v2__Rvf16_vu8_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint82half](../reg_compute/reg_convert/asc_uint82half.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_uint82half_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint82half_v2(vector_half& dst, vector_uint8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint162uint32_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint162uint32_v2__Rvu32_vu16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint162uint32](../reg_compute/reg_convert/asc_uint162uint32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_uint162uint32_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint162uint32_v2(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_hif82float_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_hif82float_v2__Rvf32_vhf8_vb
 * @archs 3510
 * @brief 将hifloat8_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_hif82float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_hif82float_v2(vector_float& dst, vector_hifloat8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_hif82float_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_hif82float_v3__Rvf32_vhf8_vb
 * @archs 3510
 * @brief 将hifloat8_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_hif82float_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_hif82float_v3(vector_float& dst, vector_hifloat8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_hif82float_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_hif82float_v4__Rvf32_vhf8_vb
 * @archs 3510
 * @brief 将hifloat8_t类型数据转换为float类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_hif82float_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_hif82float_v4(vector_float& dst, vector_hifloat8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_rd_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rd_v2__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2uint8_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rd_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2uint8_rd_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rd_sat_v2__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_half2uint8_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rd_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rn_v2__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2uint8_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rn_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2uint8_rn_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rn_sat_v2__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_half2uint8_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rn_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rna_v2__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2uint8_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rna_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2uint8_rna_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rna_sat_v2__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_half2uint8_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rna_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_ru_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_ru_v2__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2uint8_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_ru_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2uint8_ru_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_ru_sat_v2__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_half2uint8_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_ru_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2uint8_rz_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rz_v2__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2uint8_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rz_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_half2uint8_rz_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2uint8_rz_sat_v2__Rvu8_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2uint8](../reg_compute/reg_convert/asc_half2uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_half2uint8_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rz_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_int4x22half_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int4x22half_v2__Rvf16_vi4x2_vb
 * @archs 3510
 * @brief 将int4x2类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int4x22half_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22half_v2(vector_half& dst, vector_int4x2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int4x22half_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int4x22half_v3__Rvf16_vi4x2_vb
 * @archs 3510
 * @brief 将int4x2类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int4x22half_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22half_v3(vector_half& dst, vector_int4x2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int4x22half_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int4x22half_v4__Rvf16_vi4x2_vb
 * @archs 3510
 * @brief 将int4x2类型数据转换为half类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int4x22half_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22half_v4(vector_half& dst, vector_int4x2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int82int16_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int82int16_v2__Rvi16_vi8_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int82int16](../reg_compute/reg_convert/asc_int82int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int82int16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int82int16_v2(vector_int16_t& dst, vector_int8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int162uint8_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162uint8_v2__Rvu8_vi16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int162uint8](../reg_compute/reg_convert/asc_int162uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int162uint8_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int162uint8_v2(vector_uint8_t& dst, vector_int16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int162uint8_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int162uint8_sat_v2__Rvu8_vi16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int162uint8](../reg_compute/reg_convert/asc_int162uint8.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int162uint8_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int162uint8_sat_v2(vector_uint8_t& dst, vector_int16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rd_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rd_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_float2bfloat16_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rd_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rd_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rd_sat_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_float2bfloat16_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rd_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rn_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_float2bfloat16_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rn_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rn_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rn_sat_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_float2bfloat16_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rn_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rna_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_float2bfloat16_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rna_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rna_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rna_sat_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_float2bfloat16_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rna_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_ru_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_ru_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_float2bfloat16_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_ru_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_ru_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_ru_sat_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_float2bfloat16_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_ru_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rz_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rz_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_float2bfloat16_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rz_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2bfloat16_rz_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2bfloat16_rz_sat_v2__Rvbf16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
    "Currently asc_float2bfloat16_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rz_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int32_rd_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rd_v2__Rvi32_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int32](../reg_compute/reg_convert/asc_half2int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int32_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int32_rd_v2(vector_int32_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int32_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rn_v2__Rvi32_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int32](../reg_compute/reg_convert/asc_half2int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int32_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int32_rn_v2(vector_int32_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int32_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rna_v2__Rvi32_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int32](../reg_compute/reg_convert/asc_half2int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int32_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int32_rna_v2(vector_int32_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int32_ru_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_ru_v2__Rvi32_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int32](../reg_compute/reg_convert/asc_half2int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int32_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int32_ru_v2(vector_int32_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_half2int32_rz_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2int32_rz_v2__Rvi32_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2int32](../reg_compute/reg_convert/asc_half2int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2int32_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int32_rz_v2(vector_int32_t& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_e5m22float_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e5m22float_v2__Rvf32_vf8e5m2_vb
 * @archs 3510
 * @brief 将vector_fp8_e5m2_t类型的源操作数以256B为单位分为四部分，读取其中一部分元素，将其转换成vector_float类型并写入目的操作数。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_e5m22float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e5m22float_v2(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_e5m22float_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e5m22float_v3__Rvf32_vf8e5m2_vb
 * @archs 3510
 * @brief 将vector_fp8_e5m2_t类型的源操作数以256B为单位分为四部分，读取其中一部分元素，将其转换成vector_float类型并写入目的操作数。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_e5m22float_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e5m22float_v3(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_e5m22float_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e5m22float_v4__Rvf32_vf8e5m2_vb
 * @archs 3510
 * @brief 将vector_fp8_e5m2_t类型的源操作数以256B为单位分为四部分，读取其中一部分元素，将其转换成vector_float类型并写入目的操作数。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_e5m22float_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e5m22float_v4(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322int64_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322int64_v2__Rvi64_vi32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int322int64](../reg_compute/reg_convert/asc_int322int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_int322int64_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322int64_v2(vector_int64_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322uint16_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint16_v2__Rvu16_vi32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int322uint16](../reg_compute/reg_convert/asc_int322uint16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_int322uint16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint16_v2(vector_uint16_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int322uint16_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int322uint16_sat_v2__Rvu16_vi32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int322uint16](../reg_compute/reg_convert/asc_int322uint16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_int322uint16_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint16_sat_v2(vector_uint16_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642int32_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642int32_v2__Rvi32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642int32](../reg_compute/reg_convert/asc_int642int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int642int32_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642int32_v2(vector_int32_t& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int642int32_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_int642int32_sat_v2__Rvi32_vi64_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_int642int32](../reg_compute/reg_convert/asc_int642int32.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int642int32_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642int32_sat_v2(vector_int32_t& dst, vector_int64_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint82uint16_v2 is deprecated. "
             "Currently asc_uint82uint16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint82uint16_v2(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_rd_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rd_v2__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2int64_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rd_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2int64_rd_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rd_sat_v2__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2int64_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rd_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rn_v2__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2int64_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rn_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2int64_rn_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rn_sat_v2__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2int64_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rn_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rna_v2__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2int64_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rna_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2int64_rna_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rna_sat_v2__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2int64_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rna_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_ru_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_ru_v2__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2int64_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_ru_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2int64_ru_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_ru_sat_v2__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2int64_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_ru_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int64_rz_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rz_v2__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2int64_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rz_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2int64_rz_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int64_rz_sat_v2__Rvi64_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2int64_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rz_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2e4m3_rn_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e4m3_rn_sat_v2__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为fp8_e4m3fn_t类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2e4m3_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_sat_v2(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2e4m3_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e4m3_rn_v2__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为fp8_e4m3fn_t类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2e4m3_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_v2(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2e4m3_rn_sat_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e4m3_rn_sat_v3__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为fp8_e4m3fn_t类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2e4m3_rn_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_sat_v3(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2e4m3_rn_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e4m3_rn_v3__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为fp8_e4m3fn_t类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2e4m3_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_v3(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2e4m3_rn_sat_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e4m3_rn_sat_v4__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为fp8_e4m3fn_t类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2e4m3_rn_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_sat_v4(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2e4m3_rn_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e4m3_rn_v4__Rvf8e4m3_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为fp8_e4m3fn_t类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2e4m3_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_v4(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_rd_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rd_v2__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2int16_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rd_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2int16_rd_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rd_sat_v2__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2int16_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rd_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rn_v2__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2int16_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rn_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2int16_rn_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rn_sat_v2__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2int16_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rn_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rna_v2__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2int16_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rna_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2int16_rna_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rna_sat_v2__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2int16_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rna_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_ru_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_ru_v2__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2int16_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_ru_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2int16_ru_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_ru_sat_v2__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2int16_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_ru_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2int16_rz_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rz_v2__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2int16_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rz_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2int16_rz_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_float2int16_rz_sat_v2__Rvi16_vf32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_float2int16](../reg_compute/reg_convert/asc_float2int16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2int16_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rz_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rd_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rd_v2__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rd_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rd_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rd_v3__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_rd_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rd_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rd_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rd_v4__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_rd_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rd_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rn_v2__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rn_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rn_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rn_v3__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rn_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rn_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rn_v4__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rn_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rna_v2__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rna_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rna_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rna_v3__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_rna_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rna_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rna_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rna_v4__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_rna_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rna_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_ru_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_ru_v2__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_ru_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_ru_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_ru_v3__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_ru_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_ru_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_ru_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_ru_v4__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_ru_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_ru_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rz_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rz_v2__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rz_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rz_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rz_v3__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_rz_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rz_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rz_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e2m1x2_rz_v4__Rvf4e2m1x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e2m1_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e2m1x2_rz_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rz_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2hif8_rh_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rh_sat_v2__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2hif8_rh_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_sat_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2hif8_rh_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rh_v2__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2hif8_rh_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2hif8_rh_sat_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rh_sat_v3__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2hif8_rh_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_sat_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2hif8_rh_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rh_v3__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2hif8_rh_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2hif8_rh_sat_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rh_sat_v4__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2hif8_rh_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_sat_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2hif8_rh_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rh_v4__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2hif8_rh_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2hif8_rna_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rna_sat_v2__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2hif8_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_sat_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2hif8_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rna_v2__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2hif8_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2hif8_rna_sat_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rna_sat_v3__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2hif8_rna_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_sat_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2hif8_rna_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rna_v3__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2hif8_rna_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2hif8_rna_sat_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rna_sat_v4__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2hif8_rna_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_sat_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2hif8_rna_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2hif8_rna_v4__Rvhf8_vf32_vb
 * @archs 3510
 * @brief 将float类型转化为hifloat8_t类型，并支持多种舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2hif8_rna_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_uint82uint32_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82uint32_v2__Rvu32_vu8_vb
 * @archs 3510
 * @brief 将uint8_t类型转化为uint32_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_uint82uint32_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint82uint32_v2(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint82uint32_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82uint32_v3__Rvu32_vu8_vb
 * @archs 3510
 * @brief 将uint8_t类型转化为uint32_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_uint82uint32_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint82uint32_v3(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint82uint32_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_uint82uint32_v4__Rvu32_vu8_vb
 * @archs 3510
 * @brief 将uint8_t类型转化为uint32_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_uint82uint32_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint82uint32_v4(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_uint322uint16_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint16_sat_v2__Rvu16_vu32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint322uint16](../reg_compute/reg_convert/asc_uint322uint16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_uint322uint16_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint16_sat_v2(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_uint322uint16_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_uint322uint16_v2__Rvu16_vu32_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制目的操作数写入位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_uint322uint16](../reg_compute/reg_convert/asc_uint322uint16.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_uint322uint16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint16_v2(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_e2m1x22bfloat16_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e2m1x22bfloat16_v2__Rvbf16_vf4e2m1x2_vb
 * @archs 3510
 * @brief 将fp4x2_e2m1_t类型转化为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（矢量寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_e2m1x22bfloat16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e2m1x22bfloat16_v2(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask);

[[deprecated("NOTICE: asc_e2m1x22bfloat16_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e2m1x22bfloat16_v3__Rvbf16_vf4e2m1x2_vb
 * @archs 3510
 * @brief 将fp4x2_e2m1_t类型转化为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（矢量寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_e2m1x22bfloat16_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e2m1x22bfloat16_v3(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask);

[[deprecated("NOTICE: asc_e2m1x22bfloat16_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e2m1x22bfloat16_v4__Rvbf16_vf4e2m1x2_vb
 * @archs 3510
 * @brief 将fp4x2_e2m1_t类型转化为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（矢量寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_e2m1x22bfloat16_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e2m1x22bfloat16_v4(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rd_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rd_v2__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rd_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rd_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rd_v3__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_rd_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rd_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rd_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rd_v4__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_rd_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rd_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rn_v2__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rn_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rn_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rn_v3__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rn_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rn_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rn_v4__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rn_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rna_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rna_v2__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rna_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rna_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rna_v3__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_rna_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rna_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rna_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rna_v4__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_rna_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rna_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_ru_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_ru_v2__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_ru_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_ru_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_ru_v3__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_ru_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_ru_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_ru_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_ru_v4__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_ru_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_ru_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rz_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rz_v2__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rz_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rz_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rz_v3__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_rz_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rz_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rz_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_bfloat162e1m2x2_rz_v4__Rvf4e1m2x2_vbf16_vb
 * @archs 3510
 * @brief 将bfloat16_t类型转化为fp4x2_e1m2_t类型，并支持多种舍入模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_bfloat162e1m2x2_rz_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rz_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_e1m2x22bfloat16_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e1m2x22bfloat16_v2__Rvbf16_vf4e1m2x2_vb
 * @archs 3510
 * @brief 将fp4x2_e1m2_t类型转化为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_e1m2x22bfloat16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e1m2x22bfloat16_v2(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_e1m2x22bfloat16_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e1m2x22bfloat16_v3__Rvbf16_vf4e1m2x2_vb
 * @archs 3510
 * @brief 将fp4x2_e1m2_t类型转化为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_e1m2x22bfloat16_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e1m2x22bfloat16_v3(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_e1m2x22bfloat16_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_e1m2x22bfloat16_v4__Rvbf16_vf4e1m2x2_vb
 * @archs 3510
 * @brief 将fp4x2_e1m2_t类型转化为bfloat16_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_e1m2x22bfloat16_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e1m2x22bfloat16_v4(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int82int32_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82int32_v2__Rvi32_vi8_vb
 * @archs 3510
 * @brief 将int8_t类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int82int32_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int82int32_v2(vector_int32_t& dst, vector_int8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int82int32_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82int32_v3__Rvi32_vi8_vb
 * @archs 3510
 * @brief 将int8_t类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int82int32_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int82int32_v3(vector_int32_t& dst, vector_int8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_int82int32_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_int82int32_v4__Rvi32_vi8_vb
 * @archs 3510
 * @brief 将int8_t类型转化为int32_t类型。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_int82int32_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int82int32_v4(vector_int32_t& dst, vector_int8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_float2e5m2_rn_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e5m2_rn_v2__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @brief 将float类型数据转换为fp8_e5m2类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2e5m2_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_v2(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2e5m2_rn_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e5m2_rn_v3__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @brief 将float类型数据转换为fp8_e5m2类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2e5m2_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_v3(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_float2e5m2_rn_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e5m2_rn_v4__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @brief 将float类型数据转换为fp8_e5m2类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
             "Currently asc_float2e5m2_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_v4(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2e5m2_rn_sat_v2 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e5m2_rn_sat_v2__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @brief 将float类型数据转换为fp8_e5m2类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2e5m2_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_sat_v2(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2e5m2_rn_sat_v3 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e5m2_rn_sat_v3__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @brief 将float类型数据转换为fp8_e5m2类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2e5m2_rn_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_sat_v3(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

[[deprecated(
    "NOTICE: asc_float2e5m2_rn_sat_v4 is deprecated. "
/**
 * @ingroup simd_c_api.reg_compute.reg_convert
 * @override_id asc_float2e5m2_rn_sat_v4__Rvf8e5m2_vf32_vb
 * @archs 3510
 * @brief 将float类型数据转换为fp8_e5m2类型，支持RINT舍入模式，饱和/非饱和模式。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 源操作数掩码（掩码寄存器），用于指示在计算过程中哪些元素参与计算。对应位置为1时参与计算，为0时不参与计算。mask未筛选的元素在输出中置零。
 */
    "Currently asc_float2e5m2_rn_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_sat_v4(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_half2float_v2 is deprecated. "
/**
 * @ingroup simd_c_api.deprecated_interface
 * @override_id asc_half2float_v2__Rvf32_vf16_vb
 * @archs 3510
 * @brief **该接口通过接口后缀来控制源操作数读取位置（仅包括源操作数、目的操作数、掩码寄存器三个参数）的原型已废弃，请使用[asc_half2float](../reg_compute/reg_convert/asc_half2float.md)的显式位置参数重载替代。
 * @param [out] dst 目的操作数（矢量数据寄存器）。
 * @param [in] src 源操作数（矢量数据寄存器）。
 * @param [in] mask 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。
 */
             "Currently asc_half2float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2float_v2(vector_float& dst, vector_half src, vector_bool mask);

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_REG_CONVERT_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_REG_COMPUTE_REG_CONVERT_H

#endif
