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
#error "pair.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_EXPERIMENTAL_PAIR_H
#endif

#ifndef INCLUDE_C_API_EXPERIMENTAL_PAIR_H
#define INCLUDE_C_API_EXPERIMENTAL_PAIR_H

#ifndef CANN_ASC_USE_EXPERIMENTAL
#error "this is experimental feature, pls set CANN_ASC_USE_EXPERIMENTAL = ON to use it"
#endif
// WARNING: The APIs in this header are unstable and may change or be removed in future releases.

#include "c_api/defs/type.h"

struct asc_uint32_t_bool_pair {
    vector_uint32_t first;
    vector_bool second;
};

struct asc_int32_t_bool_pair {
    vector_int32_t first;
    vector_bool second;
};

struct asc_int32_t_int32_t_pair {
    vector_int32_t first;
    vector_int32_t second;
};

struct asc_uint32_t_uint32_t_pair {
    vector_uint32_t first;
    vector_uint32_t second;
};

struct asc_uint8_t_uint8_t_pair {
    vector_uint8_t first;
    vector_uint8_t second;
};

struct asc_int8_t_int8_t_pair {
    vector_int8_t first;
    vector_int8_t second;
};

struct asc_uint16_t_uint16_t_pair {
    vector_uint16_t first;
    vector_uint16_t second;
};

struct asc_int16_t_int16_t_pair {
    vector_int16_t first;
    vector_int16_t second;
};

struct asc_half_half_pair {
    vector_half first;
    vector_half second;
};

struct asc_float_float_pair {
    vector_float first;
    vector_float second;
};

struct asc_fp8_e8m0_t_fp8_e8m0_t_pair {
    vector_fp8_e8m0_t first;
    vector_fp8_e8m0_t second;
};

struct asc_fp8_e5m2_t_fp8_e5m2_t_pair {
    vector_fp8_e5m2_t first;
    vector_fp8_e5m2_t second;
};

struct asc_fp8_e4m3fn_t_fp8_e4m3fn_t_pair {
    vector_fp8_e4m3fn_t first;
    vector_fp8_e4m3fn_t second;
};

struct asc_hifloat8_t_hifloat8_t_pair {
    vector_hifloat8_t first;
    vector_hifloat8_t second;
};

struct asc_bfloat16_t_bfloat16_t_pair {
    vector_bfloat16_t first;
    vector_bfloat16_t second;
};

struct asc_bool_bool_pair {
    vector_bool first;
    vector_bool second;
};

struct asc_fp4x2_e2m1_t_fp4x2_e2m1_t_pair {
    vector_fp4x2_e2m1_t first;
    vector_fp4x2_e2m1_t second;
};

struct asc_fp4x2_e1m2_t_fp4x2_e1m2_t_pair {
    vector_fp4x2_e1m2_t first;
    vector_fp4x2_e1m2_t second;
};

struct asc_int4x2_t_int4x2_t_pair {
    vector_int4x2_t first;
    vector_int4x2_t second;
};

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_EXPERIMENTAL_PAIR_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_EXPERIMENTAL_PAIR_H
#endif
