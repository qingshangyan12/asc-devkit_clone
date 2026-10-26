/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software: you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 * -----------------------------------------------------------------------------------------------------------
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_VECTOR_COMPUTE_IMPL_ASC_SHIFT_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_VECTOR_COMPUTE_IMPL_ASC_SHIFT_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_2201/utils_impl/utils_impl.h"

#if (ASC_DEVKIT_VERSION_NUM >= 90200000)

__simd_callee__ inline void asc_shiftleft(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshl(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft(
    vector_int8_t& dst, vector_int8_t src0, vector_uint8_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshl(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshl(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft(
    vector_int16_t& dst, vector_int16_t src0, vector_uint16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshl(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshl(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft(
    vector_int32_t& dst, vector_int32_t src0, vector_uint32_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshl(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline vector_uint8_t asc_shiftleft(vector_uint8_t src0, vector_uint8_t shift, vector_bool mask)
{
    vector_uint8_t dst;
    asc_shiftleft(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_shiftleft(vector_int8_t src0, vector_uint8_t shift, vector_bool mask)
{
    vector_int8_t dst;
    asc_shiftleft(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_shiftleft(vector_uint16_t src0, vector_uint16_t shift, vector_bool mask)
{
    vector_uint16_t dst;
    asc_shiftleft(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_shiftleft(vector_int16_t src0, vector_uint16_t shift, vector_bool mask)
{
    vector_int16_t dst;
    asc_shiftleft(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_shiftleft(vector_uint32_t src0, vector_uint32_t shift, vector_bool mask)
{
    vector_uint32_t dst;
    asc_shiftleft(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_shiftleft(vector_int32_t src0, vector_uint32_t shift, vector_bool mask)
{
    vector_int32_t dst;
    asc_shiftleft(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_shiftright(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshr(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright(
    vector_int8_t& dst, vector_int8_t src0, vector_uint8_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshr(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshr(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright(
    vector_int16_t& dst, vector_int16_t src0, vector_uint16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshr(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshr(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright(
    vector_int32_t& dst, vector_int32_t src0, vector_uint32_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshr(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline vector_uint8_t asc_shiftright(vector_uint8_t src0, vector_uint8_t shift, vector_bool mask)
{
    vector_uint8_t dst;
    asc_shiftright(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_shiftright(vector_int8_t src0, vector_uint8_t shift, vector_bool mask)
{
    vector_int8_t dst;
    asc_shiftright(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_shiftright(vector_uint16_t src0, vector_uint16_t shift, vector_bool mask)
{
    vector_uint16_t dst;
    asc_shiftright(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_shiftright(vector_int16_t src0, vector_uint16_t shift, vector_bool mask)
{
    vector_int16_t dst;
    asc_shiftright(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_shiftright(vector_uint32_t src0, vector_uint32_t shift, vector_bool mask)
{
    vector_uint32_t dst;
    asc_shiftright(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_shiftright(vector_int32_t src0, vector_uint32_t shift, vector_bool mask)
{
    vector_int32_t dst;
    asc_shiftright(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_uint8_t& dst, vector_uint8_t src, uint16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_int8_t& dst, vector_int8_t src, uint16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_uint16_t& dst, vector_uint16_t src, uint16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_int16_t& dst, vector_int16_t src, uint16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_uint32_t& dst, vector_uint32_t src, uint32_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_int32_t& dst, vector_int32_t src, uint32_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline vector_uint8_t asc_shiftleft_scalar(vector_uint8_t src, uint16_t shift, vector_bool mask)
{
    vector_uint8_t dst;
    asc_shiftleft_scalar(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_shiftleft_scalar(vector_int8_t src, uint16_t shift, vector_bool mask)
{
    vector_int8_t dst;
    asc_shiftleft_scalar(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_shiftleft_scalar(vector_uint16_t src, uint16_t shift, vector_bool mask)
{
    vector_uint16_t dst;
    asc_shiftleft_scalar(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_shiftleft_scalar(vector_int16_t src, uint16_t shift, vector_bool mask)
{
    vector_int16_t dst;
    asc_shiftleft_scalar(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_shiftleft_scalar(vector_uint32_t src, uint32_t shift, vector_bool mask)
{
    vector_uint32_t dst;
    asc_shiftleft_scalar(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_shiftleft_scalar(vector_int32_t src, uint32_t shift, vector_bool mask)
{
    vector_int32_t dst;
    asc_shiftleft_scalar(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline void asc_shiftright_scalar(
    vector_uint8_t& dst, vector_uint8_t src, uint16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar(
    vector_int8_t& dst, vector_int8_t src, uint16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar(
    vector_uint16_t& dst, vector_uint16_t src, uint16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar(
    vector_int16_t& dst, vector_int16_t src, uint16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar(
    vector_uint32_t& dst, vector_uint32_t src, uint32_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar(
    vector_int32_t& dst, vector_int32_t src, uint32_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline vector_uint8_t asc_shiftright_scalar(vector_uint8_t src, uint16_t shift, vector_bool mask)
{
    vector_uint8_t dst;
    asc_shiftright_scalar(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_shiftright_scalar(vector_int8_t src, uint16_t shift, vector_bool mask)
{
    vector_int8_t dst;
    asc_shiftright_scalar(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_shiftright_scalar(vector_uint16_t src, uint16_t shift, vector_bool mask)
{
    vector_uint16_t dst;
    asc_shiftright_scalar(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_shiftright_scalar(vector_int16_t src, uint16_t shift, vector_bool mask)
{
    vector_int16_t dst;
    asc_shiftright_scalar(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_shiftright_scalar(vector_uint32_t src, uint32_t shift, vector_bool mask)
{
    vector_uint32_t dst;
    asc_shiftright_scalar(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_shiftright_scalar(vector_int32_t src, uint32_t shift, vector_bool mask)
{
    vector_int32_t dst;
    asc_shiftright_scalar(dst, src, shift, mask);
    return dst;
}

#endif

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
