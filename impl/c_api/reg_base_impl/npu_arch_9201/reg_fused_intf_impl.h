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
    "impl/c_api/reg_base_impl/npu_arch_9201/reg_fused_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_REG_FUSED_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_REG_FUSED_INTF_IMPL_H

#include "impl/c_api/reg_base_impl/utils_impl.h"

__simd_callee__ inline vector_half asc_fma(vector_half src0, vector_half src1, vector_half src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vfma(src2, src0, src1, mask, MODE_ZEROING);
    }
    return src2;
}

__simd_callee__ inline vector_bfloat16_t asc_fma(
    vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bfloat16_t src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vfma(src2, src0, src1, mask, MODE_ZEROING);
    }
    return src2;
}

__simd_callee__ inline vector_float asc_fma(vector_float src0, vector_float src1, vector_float src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vfma(src2, src0, src1, mask, MODE_ZEROING);
    }
    return src2;
}

__simd_callee__ inline vector_half asc_fms(vector_half src0, vector_half src1, vector_half src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vfnms(src2, src0, src1, mask, MODE_ZEROING);
    }
    return src2;
}

__simd_callee__ inline vector_bfloat16_t asc_fms(
    vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bfloat16_t src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vfnms(src2, src0, src1, mask, MODE_ZEROING);
    }
    return src2;
}

__simd_callee__ inline vector_float asc_fms(vector_float src0, vector_float src1, vector_float src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vfnms(src2, src0, src1, mask, MODE_ZEROING);
    }
    return src2;
}

__simd_callee__ inline vector_half asc_fnma(vector_half src0, vector_half src1, vector_half src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vfms(src2, src0, src1, mask, MODE_ZEROING);
    }
    return src2;
}

__simd_callee__ inline vector_bfloat16_t asc_fnma(
    vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bfloat16_t src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vfms(src2, src0, src1, mask, MODE_ZEROING);
    }
    return src2;
}

__simd_callee__ inline vector_float asc_fnma(vector_float src0, vector_float src1, vector_float src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vfms(src2, src0, src1, mask, MODE_ZEROING);
    }
    return src2;
}

__simd_callee__ inline vector_half asc_fnms(vector_half src0, vector_half src1, vector_half src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vfnma(src2, src0, src1, mask, MODE_ZEROING);
    }
    return src2;
}

__simd_callee__ inline vector_bfloat16_t asc_fnms(
    vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bfloat16_t src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vfnma(src2, src0, src1, mask, MODE_ZEROING);
    }
    return src2;
}

__simd_callee__ inline vector_float asc_fnms(vector_float src0, vector_float src1, vector_float src2, vector_bool mask)
{
    if ASC_IS_AIV {
        vfnma(src2, src0, src1, mask, MODE_ZEROING);
    }
    return src2;
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
