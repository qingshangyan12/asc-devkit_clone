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
#warning \
    "impl/c_api/experimental/reg_permute_sel_intf_impl_exp.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_EXPERIMENTAL_REG_PERMUTE_SEL_INTF_IMPL_EXP_H
#define IMPL_C_API_EXPERIMENTAL_REG_PERMUTE_SEL_INTF_IMPL_EXP_H

#ifndef CANN_ASC_USE_EXPERIMENTAL
#error "this is experimental feature, pls set CANN_ASC_USE_EXPERIMENTAL = ON to use it"
#endif

__simd_callee__ inline asc_uint8_t_uint8_t_pair asc_intlv(vector_uint8_t src0, vector_uint8_t src1)
{
    asc_uint8_t_uint8_t_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_int8_t_int8_t_pair asc_intlv(vector_int8_t src0, vector_int8_t src1)
{
    asc_int8_t_int8_t_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_uint16_t_uint16_t_pair asc_intlv(vector_uint16_t src0, vector_uint16_t src1)
{
    asc_uint16_t_uint16_t_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_int16_t_int16_t_pair asc_intlv(vector_int16_t src0, vector_int16_t src1)
{
    asc_int16_t_int16_t_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_uint32_t_uint32_t_pair asc_intlv(vector_uint32_t src0, vector_uint32_t src1)
{
    asc_uint32_t_uint32_t_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_int32_t_int32_t_pair asc_intlv(vector_int32_t src0, vector_int32_t src1)
{
    asc_int32_t_int32_t_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_half_half_pair asc_intlv(vector_half src0, vector_half src1)
{
    asc_half_half_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_float_float_pair asc_intlv(vector_float src0, vector_float src1)
{
    asc_float_float_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_fp8_e8m0_t_fp8_e8m0_t_pair asc_intlv(vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1)
{
    asc_fp8_e8m0_t_fp8_e8m0_t_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_fp8_e5m2_t_fp8_e5m2_t_pair asc_intlv(vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1)
{
    asc_fp8_e5m2_t_fp8_e5m2_t_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_fp8_e4m3fn_t_fp8_e4m3fn_t_pair asc_intlv(vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1)
{
    asc_fp8_e4m3fn_t_fp8_e4m3fn_t_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_hifloat8_t_hifloat8_t_pair asc_intlv(vector_hifloat8_t src0, vector_hifloat8_t src1)
{
    asc_hifloat8_t_hifloat8_t_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_bfloat16_t_bfloat16_t_pair asc_intlv(vector_bfloat16_t src0, vector_bfloat16_t src1)
{
    asc_bfloat16_t_bfloat16_t_pair result;
    asc_intlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_bool_bool_pair asc_intlv_b8(vector_bool src0, vector_bool src1)
{
    asc_bool_bool_pair result;
    asc_intlv_b8(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_bool_bool_pair asc_intlv_b16(vector_bool src0, vector_bool src1)
{
    asc_bool_bool_pair result;
    asc_intlv_b16(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_bool_bool_pair asc_intlv_b32(vector_bool src0, vector_bool src1)
{
    asc_bool_bool_pair result;
    asc_intlv_b32(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_int32_t_int32_t_pair asc_deintlv(vector_int32_t src0, vector_int32_t src1)
{
    asc_int32_t_int32_t_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_uint32_t_uint32_t_pair asc_deintlv(vector_uint32_t src0, vector_uint32_t src1)
{
    asc_uint32_t_uint32_t_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_int16_t_int16_t_pair asc_deintlv(vector_int16_t src0, vector_int16_t src1)
{
    asc_int16_t_int16_t_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_uint16_t_uint16_t_pair asc_deintlv(vector_uint16_t src0, vector_uint16_t src1)
{
    asc_uint16_t_uint16_t_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_int8_t_int8_t_pair asc_deintlv(vector_int8_t src0, vector_int8_t src1)
{
    asc_int8_t_int8_t_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_uint8_t_uint8_t_pair asc_deintlv(vector_uint8_t src0, vector_uint8_t src1)
{
    asc_uint8_t_uint8_t_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_fp8_e4m3fn_t_fp8_e4m3fn_t_pair asc_deintlv(
    vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1)
{
    asc_fp8_e4m3fn_t_fp8_e4m3fn_t_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_hifloat8_t_hifloat8_t_pair asc_deintlv(vector_hifloat8_t src0, vector_hifloat8_t src1)
{
    asc_hifloat8_t_hifloat8_t_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_fp8_e5m2_t_fp8_e5m2_t_pair asc_deintlv(vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1)
{
    asc_fp8_e5m2_t_fp8_e5m2_t_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_fp8_e8m0_t_fp8_e8m0_t_pair asc_deintlv(vector_fp8_e8m0_t src0, vector_fp8_e8m0_t src1)
{
    asc_fp8_e8m0_t_fp8_e8m0_t_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_bfloat16_t_bfloat16_t_pair asc_deintlv(vector_bfloat16_t src0, vector_bfloat16_t src1)
{
    asc_bfloat16_t_bfloat16_t_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_float_float_pair asc_deintlv(vector_float src0, vector_float src1)
{
    asc_float_float_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_half_half_pair asc_deintlv(vector_half src0, vector_half src1)
{
    asc_half_half_pair result;
    asc_deintlv(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_bool_bool_pair asc_deintlv_b8(vector_bool src0, vector_bool src1)
{
    asc_bool_bool_pair result;
    asc_deintlv_b8(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_bool_bool_pair asc_deintlv_b16(vector_bool src0, vector_bool src1)
{
    asc_bool_bool_pair result;
    asc_deintlv_b16(result.first, result.second, src0, src1);
    return result;
}

__simd_callee__ inline asc_bool_bool_pair asc_deintlv_b32(vector_bool src0, vector_bool src1)
{
    asc_bool_bool_pair result;
    asc_deintlv_b32(result.first, result.second, src0, src1);
    return result;
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
