/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_DEFS_PAIR_H
#endif

#ifndef INCLUDE_C_API_DEFS_PAIR_H
#define INCLUDE_C_API_DEFS_PAIR_H

#include "c_api/defs/type.h"

struct asc_add_carry_uint32_t_result {
    vector_uint32_t first;
    vector_bool second;
};

struct asc_add_carry_int32_t_result {
    vector_int32_t first;
    vector_bool second;
};

struct asc_sub_carry_uint32_t_result {
    vector_uint32_t first;
    vector_bool second;
};

struct asc_sub_carry_int32_t_result {
    vector_int32_t first;
    vector_bool second;
};

struct asc_mull_int32_t_result {
    vector_int32_t first;
    vector_int32_t second;
};

struct asc_mull_uint32_t_result {
    vector_uint32_t first;
    vector_uint32_t second;
};

struct asc_intlv_uint8_t_result {
    vector_uint8_t first;
    vector_uint8_t second;
};

struct asc_intlv_int8_t_result {
    vector_int8_t first;
    vector_int8_t second;
};

struct asc_intlv_uint16_t_result {
    vector_uint16_t first;
    vector_uint16_t second;
};

struct asc_intlv_int16_t_result {
    vector_int16_t first;
    vector_int16_t second;
};

struct asc_intlv_uint32_t_result {
    vector_uint32_t first;
    vector_uint32_t second;
};

struct asc_intlv_int32_t_result {
    vector_int32_t first;
    vector_int32_t second;
};

struct asc_intlv_half_result {
    vector_half first;
    vector_half second;
};

struct asc_intlv_float_result {
    vector_float first;
    vector_float second;
};

struct asc_intlv_fp8_e8m0_t_result {
    vector_fp8_e8m0_t first;
    vector_fp8_e8m0_t second;
};

struct asc_intlv_fp8_e5m2_t_result {
    vector_fp8_e5m2_t first;
    vector_fp8_e5m2_t second;
};

struct asc_intlv_fp8_e4m3fn_t_result {
    vector_fp8_e4m3fn_t first;
    vector_fp8_e4m3fn_t second;
};

struct asc_intlv_hifloat8_t_result {
    vector_hifloat8_t first;
    vector_hifloat8_t second;
};

struct asc_intlv_bfloat16_t_result {
    vector_bfloat16_t first;
    vector_bfloat16_t second;
};

struct asc_intlv_b8_result {
    vector_bool first;
    vector_bool second;
};

struct asc_intlv_b16_result {
    vector_bool first;
    vector_bool second;
};

struct asc_intlv_b32_result {
    vector_bool first;
    vector_bool second;
};

struct asc_deintlv_int32_t_result {
    vector_int32_t first;
    vector_int32_t second;
};

struct asc_deintlv_uint32_t_result {
    vector_uint32_t first;
    vector_uint32_t second;
};

struct asc_deintlv_int16_t_result {
    vector_int16_t first;
    vector_int16_t second;
};

struct asc_deintlv_uint16_t_result {
    vector_uint16_t first;
    vector_uint16_t second;
};

struct asc_deintlv_int8_t_result {
    vector_int8_t first;
    vector_int8_t second;
};

struct asc_deintlv_uint8_t_result {
    vector_uint8_t first;
    vector_uint8_t second;
};

struct asc_deintlv_fp8_e4m3fn_t_result {
    vector_fp8_e4m3fn_t first;
    vector_fp8_e4m3fn_t second;
};

struct asc_deintlv_hifloat8_t_result {
    vector_hifloat8_t first;
    vector_hifloat8_t second;
};

struct asc_deintlv_fp8_e5m2_t_result {
    vector_fp8_e5m2_t first;
    vector_fp8_e5m2_t second;
};

struct asc_deintlv_fp8_e8m0_t_result {
    vector_fp8_e8m0_t first;
    vector_fp8_e8m0_t second;
};

struct asc_deintlv_bfloat16_t_result {
    vector_bfloat16_t first;
    vector_bfloat16_t second;
};

struct asc_deintlv_float_result {
    vector_float first;
    vector_float second;
};

struct asc_deintlv_half_result {
    vector_half first;
    vector_half second;
};

struct asc_deintlv_b8_result {
    vector_bool first;
    vector_bool second;
};

struct asc_deintlv_b16_result {
    vector_bool first;
    vector_bool second;
};

struct asc_deintlv_b32_result {
    vector_bool first;
    vector_bool second;
};

struct asc_loadalign_deintlv_int8_t_result {
    vector_int8_t first;
    vector_int8_t second;
};

struct asc_loadalign_deintlv_uint8_t_result {
    vector_uint8_t first;
    vector_uint8_t second;
};

struct asc_loadalign_deintlv_fp4x2_e2m1_t_result {
    vector_fp4x2_e2m1_t first;
    vector_fp4x2_e2m1_t second;
};

struct asc_loadalign_deintlv_fp4x2_e1m2_t_result {
    vector_fp4x2_e1m2_t first;
    vector_fp4x2_e1m2_t second;
};

struct asc_loadalign_deintlv_int4x2_t_result {
    vector_int4x2_t first;
    vector_int4x2_t second;
};

struct asc_loadalign_deintlv_fp8_e8m0_t_result {
    vector_fp8_e8m0_t first;
    vector_fp8_e8m0_t second;
};

struct asc_loadalign_deintlv_fp8_e5m2_t_result {
    vector_fp8_e5m2_t first;
    vector_fp8_e5m2_t second;
};

struct asc_loadalign_deintlv_fp8_e4m3fn_t_result {
    vector_fp8_e4m3fn_t first;
    vector_fp8_e4m3fn_t second;
};

struct asc_loadalign_deintlv_hifloat8_t_result {
    vector_hifloat8_t first;
    vector_hifloat8_t second;
};

struct asc_loadalign_deintlv_int16_t_result {
    vector_int16_t first;
    vector_int16_t second;
};

struct asc_loadalign_deintlv_uint16_t_result {
    vector_uint16_t first;
    vector_uint16_t second;
};

struct asc_loadalign_deintlv_half_result {
    vector_half first;
    vector_half second;
};

struct asc_loadalign_deintlv_bfloat16_t_result {
    vector_bfloat16_t first;
    vector_bfloat16_t second;
};

struct asc_loadalign_deintlv_int32_t_result {
    vector_int32_t first;
    vector_int32_t second;
};

struct asc_loadalign_deintlv_uint32_t_result {
    vector_uint32_t first;
    vector_uint32_t second;
};

struct asc_loadalign_deintlv_float_result {
    vector_float first;
    vector_float second;
};

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_DEFS_PAIR_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_DEFS_PAIR_H
#endif
