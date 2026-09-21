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
    "impl/c_api/reg_base_impl/npu_arch_9201/reg_arith_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_REG_ARITH_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_REG_ARITH_INTF_IMPL_H

__simd_callee__ inline void asc_add_merge(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vadd(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_add_merge(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vadd(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_add_merge(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vadd(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_add_merge(
    vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vadd(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_add_merge(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vadd(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_add_merge(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vadd(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_add_merge(
    vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vadd(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_add_merge(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vadd(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_add_merge(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vadd(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_max_merge(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmax(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_max_merge(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmax(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_max_merge(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmax(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_max_merge(
    vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmax(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_max_merge(
    vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmax(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_max_merge(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmax(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_max_merge(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmax(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_max_merge(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmax(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_max_merge(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmax(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_min_merge(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmin(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_min_merge(
    vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmin(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_min_merge(
    vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmin(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_min_merge(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmin(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_min_merge(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmin(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_min_merge(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmin(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_min_merge(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmin(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_min_merge(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmin(dst, src0, src1, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_min_merge(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vmin(dst, src0, src1, mask, MODE_MERGING);
    }
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
