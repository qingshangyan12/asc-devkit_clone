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
#error "reg_permute_sel.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H
#endif

#ifndef INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_PERMUTE_SEL_H
#define INCLUDE_C_API_REG_VECTOR_COMPUTE_REG_PERMUTE_SEL_H

#include "impl/c_api/instr_impl/npu_arch_3510/vector_compute_impl.h"

// ==========asc_squeeze(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t/half/float/bfloat16_t/hifloat8_t/fp8_e4m3fn_t/fp8_e5m2_t/fp8_e8m0_t)==========
__simd_callee__ inline void asc_squeeze(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_squeeze(vector_float& dst, vector_float src, vector_bool mask);

__simd_callee__ inline void asc_squeeze(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(vector_half& dst, vector_half src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(vector_float& dst, vector_float src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask);

__simd_callee__ inline void asc_squeeze_with_status(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask);

// ==========asc_unsqueeze(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t)==========
__simd_callee__ inline void asc_unsqueeze(vector_uint8_t& dst, vector_bool mask);

__simd_callee__ inline void asc_unsqueeze(vector_int8_t& dst, vector_bool mask);

__simd_callee__ inline void asc_unsqueeze(vector_uint16_t& dst, vector_bool mask);

__simd_callee__ inline void asc_unsqueeze(vector_int16_t& dst, vector_bool mask);

__simd_callee__ inline void asc_unsqueeze(vector_uint32_t& dst, vector_bool mask);

__simd_callee__ inline void asc_unsqueeze(vector_int32_t& dst, vector_bool mask);

// ==========asc_pack(bool/uint16_t/int16_t/uint32_t/int32_t)==========
__simd_callee__ inline void asc_pack(vector_uint8_t& dst, vector_uint16_t src);

__simd_callee__ inline void asc_pack(vector_uint8_t& dst, vector_int16_t src);

__simd_callee__ inline void asc_pack(vector_uint16_t& dst, vector_uint32_t src);

__simd_callee__ inline void asc_pack(vector_uint16_t& dst, vector_int32_t src);

__simd_callee__ inline void asc_pack(vector_bool& dst, vector_bool src);

__simd_callee__ inline void asc_pack_v2(vector_uint8_t& dst, vector_uint16_t src);

__simd_callee__ inline void asc_pack_v2(vector_uint8_t& dst, vector_int16_t src);

__simd_callee__ inline void asc_pack_v2(vector_uint16_t& dst, vector_uint32_t src);

__simd_callee__ inline void asc_pack_v2(vector_uint16_t& dst, vector_int32_t src);

__simd_callee__ inline void asc_pack_v2(vector_bool& dst, vector_bool src);

// ==========asc_unpack_lower/upper(uint8_t/int8_t/uint16_t/int16_t/bool)==========
__simd_callee__ inline void asc_unpack_upper(vector_uint16_t& dst, vector_uint8_t src);

__simd_callee__ inline void asc_unpack_lower(vector_uint16_t& dst, vector_uint8_t src);

__simd_callee__ inline void asc_unpack_upper(vector_int16_t& dst, vector_int8_t src);

__simd_callee__ inline void asc_unpack_lower(vector_int16_t& dst, vector_int8_t src);

__simd_callee__ inline void asc_unpack_upper(vector_uint32_t& dst, vector_uint16_t src);

__simd_callee__ inline void asc_unpack_lower(vector_uint32_t& dst, vector_uint16_t src);

__simd_callee__ inline void asc_unpack_upper(vector_int32_t& dst, vector_int16_t src);

__simd_callee__ inline void asc_unpack_lower(vector_int32_t& dst, vector_int16_t src);

__simd_callee__ inline void asc_unpack_upper(vector_bool& dst, vector_bool src);

__simd_callee__ inline void asc_unpack_lower(vector_bool& dst, vector_bool src);

// ==========asc_select(bool/uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_select(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask);

__simd_callee__ inline void asc_select(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask);

__simd_callee__ inline void asc_select(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask);

__simd_callee__ inline void asc_select(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, vector_bool mask);

__simd_callee__ inline void asc_select(
    vector_hifloat8_t& dst, vector_hifloat8_t src0, vector_hifloat8_t src1, vector_bool mask);

__simd_callee__ inline void asc_select(
    vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, vector_bool mask);

__simd_callee__ inline void asc_select(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask);

__simd_callee__ inline void asc_select(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask);

__simd_callee__ inline void asc_select(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

__simd_callee__ inline void asc_select(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

__simd_callee__ inline void asc_select(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask);

__simd_callee__ inline void asc_select(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask);

__simd_callee__ inline void asc_select(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);

// ==========asc_intlv(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t/half/float/fp8_e8m0_t/fp8_e5m2_t/fp8_e4m3fn_t/bfloat16_t)==========
__simd_callee__ inline void asc_intlv_b8(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_intlv_b16(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_intlv_b32(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_intlv(
    vector_uint8_t& dst0, vector_uint8_t& dst1, vector_uint8_t src0, vector_uint8_t src1);

__simd_callee__ inline void asc_intlv(vector_int8_t& dst0, vector_int8_t& dst1, vector_int8_t src0, vector_int8_t src1);

__simd_callee__ inline void asc_intlv(
    vector_uint16_t& dst0, vector_uint16_t& dst1, vector_uint16_t src0, vector_uint16_t src1);

__simd_callee__ inline void asc_intlv(
    vector_int16_t& dst0, vector_int16_t& dst1, vector_int16_t src0, vector_int16_t src1);

__simd_callee__ inline void asc_intlv(
    vector_uint32_t& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1);

__simd_callee__ inline void asc_intlv(
    vector_int32_t& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1);

__simd_callee__ inline void asc_intlv(vector_half& dst0, vector_half& dst1, vector_half src0, vector_half src1);

__simd_callee__ inline void asc_intlv(vector_float& dst0, vector_float& dst1, vector_float src0, vector_float src1);

__simd_callee__ inline void asc_intlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

__simd_callee__ inline void asc_intlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

__simd_callee__ inline void asc_intlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

__simd_callee__ inline void asc_intlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, vector_hifloat8_t src0, vector_hifloat8_t src1);

__simd_callee__ inline void asc_intlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, vector_bfloat16_t src0, vector_bfloat16_t src1);

// ==========asc_deintlv(int32_t/uint32_t/int16_t/uint16_t/int8_t/uint8_t/fp8_e4m3fn/fp8_e5m2/fp8_e8m0/bfloat16_t/float/half)==========
__simd_callee__ inline void asc_deintlv(
    vector_int32_t& dst0, vector_int32_t& dst1, vector_int32_t src0, vector_int32_t src1);

__simd_callee__ inline void asc_deintlv(
    vector_uint32_t& dst0, vector_uint32_t& dst1, vector_uint32_t src0, vector_uint32_t src1);

__simd_callee__ inline void asc_deintlv(
    vector_int16_t& dst0, vector_int16_t& dst1, vector_int16_t src0, vector_int16_t src1);

__simd_callee__ inline void asc_deintlv(
    vector_uint16_t& dst0, vector_uint16_t& dst1, vector_uint16_t src0, vector_uint16_t src1);

__simd_callee__ inline void asc_deintlv(
    vector_int8_t& dst0, vector_int8_t& dst1, vector_int8_t src0, vector_int8_t src1);

__simd_callee__ inline void asc_deintlv(
    vector_uint8_t& dst0, vector_uint8_t& dst1, vector_uint8_t src0, vector_uint8_t src1);

__simd_callee__ inline void asc_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1);

__simd_callee__ inline void asc_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, vector_hifloat8_t src0, vector_hifloat8_t src1);

__simd_callee__ inline void asc_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1);

__simd_callee__ inline void asc_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1);

__simd_callee__ inline void asc_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, vector_bfloat16_t src0, vector_bfloat16_t src1);

__simd_callee__ inline void asc_deintlv(vector_float& dst0, vector_float& dst1, vector_float src0, vector_float src1);

__simd_callee__ inline void asc_deintlv(vector_half& dst0, vector_half& dst1, vector_half src0, vector_half src1);

// ==========asc_deintlv_b8/b16/b32(bool)==========
__simd_callee__ inline void asc_deintlv_b8(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_deintlv_b16(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1);

__simd_callee__ inline void asc_deintlv_b32(vector_bool& dst0, vector_bool& dst1, vector_bool src0, vector_bool src1);

//===================asc_clear_ar_spr=====================
__simd_callee__ inline void asc_clear_ar_spr();

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_int8_t& dst, vector_int8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_int16_t& dst, vector_int16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_int32_t& dst, vector_int32_t src, vector_bool mask);

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_half& dst, vector_half src, vector_bool mask);

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_float& dst, vector_float src, vector_bool mask);

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask);

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask);

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask);

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask);

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. Please use asc_squeeze_with_status instead.")]]
__simd_callee__ inline void asc_squeeze_v2(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask);

#endif
