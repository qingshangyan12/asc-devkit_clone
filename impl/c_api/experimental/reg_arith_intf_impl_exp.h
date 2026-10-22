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
    "impl/c_api/experimental/reg_arith_intf_impl_exp.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_EXPERIMENTAL_REG_ARITH_INTF_IMPL_EXP_H
#define IMPL_C_API_EXPERIMENTAL_REG_ARITH_INTF_IMPL_EXP_H

#ifndef CANN_ASC_USE_EXPERIMENTAL
#error "this is experimental feature, pls set CANN_ASC_USE_EXPERIMENTAL = ON to use it"
#endif

__simd_callee__ inline asc_uint32_t_bool_pair asc_add_carry(
    vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    asc_uint32_t_bool_pair result;
    asc_add(result.second, result.first, src0, src1, mask);
    return result;
}

__simd_callee__ inline asc_int32_t_bool_pair asc_add_carry(vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    asc_int32_t_bool_pair result;
    asc_add(result.second, result.first, src0, src1, mask);
    return result;
}

__simd_callee__ inline asc_uint32_t_bool_pair asc_add_carry(
    vector_uint32_t src0, vector_uint32_t src1, vector_bool carry_src, vector_bool mask)
{
    asc_uint32_t_bool_pair result;
    asc_addc(result.second, result.first, src0, src1, carry_src, mask);
    return result;
}

__simd_callee__ inline asc_int32_t_bool_pair asc_add_carry(
    vector_int32_t src0, vector_int32_t src1, vector_bool carry_src, vector_bool mask)
{
    asc_int32_t_bool_pair result;
    asc_addc(result.second, result.first, src0, src1, carry_src, mask);
    return result;
}

__simd_callee__ inline asc_uint32_t_bool_pair asc_sub_carry(
    vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    asc_uint32_t_bool_pair result;
    asc_sub(result.second, result.first, src0, src1, mask);
    return result;
}

__simd_callee__ inline asc_int32_t_bool_pair asc_sub_carry(vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    asc_int32_t_bool_pair result;
    asc_sub(result.second, result.first, src0, src1, mask);
    return result;
}

__simd_callee__ inline asc_uint32_t_bool_pair asc_sub_carry(
    vector_uint32_t src0, vector_uint32_t src1, vector_bool carry_src, vector_bool mask)
{
    asc_uint32_t_bool_pair result;
    asc_subc(result.second, result.first, src0, src1, carry_src, mask);
    return result;
}

__simd_callee__ inline asc_int32_t_bool_pair asc_sub_carry(
    vector_int32_t src0, vector_int32_t src1, vector_bool carry_src, vector_bool mask)
{
    asc_int32_t_bool_pair result;
    asc_subc(result.second, result.first, src0, src1, carry_src, mask);
    return result;
}

__simd_callee__ inline asc_int32_t_int32_t_pair asc_mull(vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    asc_int32_t_int32_t_pair result;
    asc_mull(result.first, result.second, src0, src1, mask);
    return result;
}

__simd_callee__ inline asc_uint32_t_uint32_t_pair asc_mull(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    asc_uint32_t_uint32_t_pair result;
    asc_mull(result.first, result.second, src0, src1, mask);
    return result;
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
