/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_REG_LOGIC_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_REG_LOGIC_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/reg_logic_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_REG_LOGIC_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_impl.h"

__simd_callee__ inline void asc_and_impl(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask)
{
    if ASC_IS_AIV {
        pand(dst, src0, src1, mask);
    }
}

__simd_callee__ inline void asc_and_impl(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vand(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_and_impl(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vand(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_and_impl(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vand(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_and_impl(
    vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vand(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_and_impl(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vand(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_and_impl(
    vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vand(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_and_impl(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vand(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_and_impl(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vand(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_and_impl(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vand(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_and_impl(
    vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vand(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_and_impl(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vand(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar_impl(
    vector_uint8_t& dst, vector_uint8_t src, int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar_impl(
    vector_int8_t& dst, vector_int8_t src, int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar_impl(
    vector_uint16_t& dst, vector_uint16_t src, int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar_impl(
    vector_int16_t& dst, vector_int16_t src, int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar_impl(
    vector_uint32_t& dst, vector_uint32_t src, int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_scalar_impl(
    vector_int32_t& dst, vector_int32_t src, int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshls(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline vector_uint8_t asc_shiftleft_scalar_impl(vector_uint8_t src, int16_t shift, vector_bool mask)
{
    vector_uint8_t dst;
    asc_shiftleft_scalar_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_shiftleft_scalar_impl(vector_int8_t src, int16_t shift, vector_bool mask)
{
    vector_int8_t dst;
    asc_shiftleft_scalar_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_shiftleft_scalar_impl(vector_uint16_t src, int16_t shift, vector_bool mask)
{
    vector_uint16_t dst;
    asc_shiftleft_scalar_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_shiftleft_scalar_impl(vector_int16_t src, int16_t shift, vector_bool mask)
{
    vector_int16_t dst;
    asc_shiftleft_scalar_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_shiftleft_scalar_impl(vector_uint32_t src, int16_t shift, vector_bool mask)
{
    vector_uint32_t dst;
    asc_shiftleft_scalar_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_shiftleft_scalar_impl(vector_int32_t src, int16_t shift, vector_bool mask)
{
    vector_int32_t dst;
    asc_shiftleft_scalar_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline void asc_shiftright_scalar_impl(
    vector_uint8_t& dst, vector_uint8_t src, int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar_impl(
    vector_int8_t& dst, vector_int8_t src, int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar_impl(
    vector_uint16_t& dst, vector_uint16_t src, int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar_impl(
    vector_int16_t& dst, vector_int16_t src, int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar_impl(
    vector_uint32_t& dst, vector_uint32_t src, int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_scalar_impl(
    vector_int32_t& dst, vector_int32_t src, int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshrs(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline vector_uint8_t asc_shiftright_scalar_impl(vector_uint8_t src, int16_t shift, vector_bool mask)
{
    vector_uint8_t dst;
    asc_shiftright_scalar_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_shiftright_scalar_impl(vector_int8_t src, int16_t shift, vector_bool mask)
{
    vector_int8_t dst;
    asc_shiftright_scalar_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_shiftright_scalar_impl(vector_uint16_t src, int16_t shift, vector_bool mask)
{
    vector_uint16_t dst;
    asc_shiftright_scalar_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_shiftright_scalar_impl(vector_int16_t src, int16_t shift, vector_bool mask)
{
    vector_int16_t dst;
    asc_shiftright_scalar_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_shiftright_scalar_impl(vector_uint32_t src, int16_t shift, vector_bool mask)
{
    vector_uint32_t dst;
    asc_shiftright_scalar_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_shiftright_scalar_impl(vector_int32_t src, int16_t shift, vector_bool mask)
{
    vector_int32_t dst;
    asc_shiftright_scalar_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline void asc_shiftleft_impl(
    vector_uint8_t& dst, vector_uint8_t src, vector_int8_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshl(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_impl(
    vector_int8_t& dst, vector_int8_t src, vector_int8_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshl(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_impl(
    vector_uint16_t& dst, vector_uint16_t src, vector_int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshl(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_impl(
    vector_int16_t& dst, vector_int16_t src, vector_int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshl(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_impl(
    vector_uint32_t& dst, vector_uint32_t src, vector_int32_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshl(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftleft_impl(
    vector_int32_t& dst, vector_int32_t src, vector_int32_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshl(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline vector_uint8_t asc_shiftleft_impl(vector_uint8_t src, vector_int8_t shift, vector_bool mask)
{
    vector_uint8_t dst;
    asc_shiftleft_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_shiftleft_impl(vector_int8_t src, vector_int8_t shift, vector_bool mask)
{
    vector_int8_t dst;
    asc_shiftleft_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_shiftleft_impl(vector_uint16_t src, vector_int16_t shift, vector_bool mask)
{
    vector_uint16_t dst;
    asc_shiftleft_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_shiftleft_impl(vector_int16_t src, vector_int16_t shift, vector_bool mask)
{
    vector_int16_t dst;
    asc_shiftleft_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_shiftleft_impl(vector_uint32_t src, vector_int32_t shift, vector_bool mask)
{
    vector_uint32_t dst;
    asc_shiftleft_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_shiftleft_impl(vector_int32_t src, vector_int32_t shift, vector_bool mask)
{
    vector_int32_t dst;
    asc_shiftleft_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline void asc_shiftright_impl(
    vector_uint8_t& dst, vector_uint8_t src, vector_int8_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshr(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_impl(
    vector_int8_t& dst, vector_int8_t src, vector_int8_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshr(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_impl(
    vector_uint16_t& dst, vector_uint16_t src, vector_int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshr(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_impl(
    vector_int16_t& dst, vector_int16_t src, vector_int16_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshr(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_impl(
    vector_uint32_t& dst, vector_uint32_t src, vector_int32_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshr(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_shiftright_impl(
    vector_int32_t& dst, vector_int32_t src, vector_int32_t shift, vector_bool mask)
{
    if ASC_IS_AIV {
        vshr(dst, src, shift, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline vector_uint8_t asc_shiftright_impl(vector_uint8_t src, vector_int8_t shift, vector_bool mask)
{
    vector_uint8_t dst;
    asc_shiftright_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_shiftright_impl(vector_int8_t src, vector_int8_t shift, vector_bool mask)
{
    vector_int8_t dst;
    asc_shiftright_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_shiftright_impl(vector_uint16_t src, vector_int16_t shift, vector_bool mask)
{
    vector_uint16_t dst;
    asc_shiftright_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_shiftright_impl(vector_int16_t src, vector_int16_t shift, vector_bool mask)
{
    vector_int16_t dst;
    asc_shiftright_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_shiftright_impl(vector_uint32_t src, vector_int32_t shift, vector_bool mask)
{
    vector_uint32_t dst;
    asc_shiftright_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_shiftright_impl(vector_int32_t src, vector_int32_t shift, vector_bool mask)
{
    vector_int32_t dst;
    asc_shiftright_impl(dst, src, shift, mask);
    return dst;
}

__simd_callee__ inline void asc_not_impl(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vnot(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_not_impl(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vnot(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_not_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vnot(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_not_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vnot(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_not_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vnot(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_not_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vnot(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_not_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vnot(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_not_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vnot(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_not_impl(vector_bool& dst, vector_bool src, vector_bool mask)
{
    if ASC_IS_AIV {
        pnot(dst, src, mask);
    }
}

__simd_callee__ inline void asc_or_impl(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vor(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_or_impl(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vor(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_or_impl(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vor(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_or_impl(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vor(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_or_impl(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vor(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_or_impl(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vor(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_or_impl(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vor(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_or_impl(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vor(dst, src0, src1, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_or_impl(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask)
{
    if ASC_IS_AIV {
        por(dst, src0, src1, mask);
    }
}

// asc_xor(vxor) int32_t
__simd_callee__ inline void asc_xor_impl(
    vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vxor(dst, src0, src1, mask, MODE_UNKNOWN);
    }
}

// asc_xor(vxor) uint32_t
__simd_callee__ inline void asc_xor_impl(
    vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vxor(dst, src0, src1, mask, MODE_UNKNOWN);
    }
}

// asc_xor(vxor) uint16_t
__simd_callee__ inline void asc_xor_impl(
    vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vxor(dst, src0, src1, mask, MODE_UNKNOWN);
    }
}

// asc_xor(vxor) int16_t
__simd_callee__ inline void asc_xor_impl(
    vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vxor(dst, src0, src1, mask, MODE_UNKNOWN);
    }
}

// asc_xor(vxor) uint8_t
__simd_callee__ inline void asc_xor_impl(
    vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vxor(dst, src0, src1, mask, MODE_UNKNOWN);
    }
}

// asc_xor(vxor) int8_t
__simd_callee__ inline void asc_xor_impl(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vxor(dst, src0, src1, mask, MODE_UNKNOWN);
    }
}

// asc_xor(pxor) bool
__simd_callee__ inline void asc_xor_impl(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask)
{
    if ASC_IS_AIV {
        pxor(dst, src0, src1, mask);
    }
}

// ==========asc_shiftleft(u8/s8/u16/s16/u32/s32)==========
__simd_callee__ inline void asc_shiftleft(
    vector_uint8_t& dst, vector_uint8_t src, vector_int8_t shift, vector_bool mask)
{
    asc_shiftleft_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftleft(vector_int8_t& dst, vector_int8_t src, vector_int8_t shift, vector_bool mask)
{
    asc_shiftleft_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftleft(
    vector_uint16_t& dst, vector_uint16_t src, vector_int16_t shift, vector_bool mask)
{
    asc_shiftleft_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftleft(
    vector_int16_t& dst, vector_int16_t src, vector_int16_t shift, vector_bool mask)
{
    asc_shiftleft_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftleft(
    vector_uint32_t& dst, vector_uint32_t src, vector_int32_t shift, vector_bool mask)
{
    asc_shiftleft_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftleft(
    vector_int32_t& dst, vector_int32_t src, vector_int32_t shift, vector_bool mask)
{
    asc_shiftleft_impl(dst, src, shift, mask);
}

__simd_callee__ inline vector_uint8_t asc_shiftleft(vector_uint8_t src, vector_int8_t shift, vector_bool mask)
{
    return asc_shiftleft_impl(src, shift, mask);
}

__simd_callee__ inline vector_int8_t asc_shiftleft(vector_int8_t src, vector_int8_t shift, vector_bool mask)
{
    return asc_shiftleft_impl(src, shift, mask);
}

__simd_callee__ inline vector_uint16_t asc_shiftleft(vector_uint16_t src, vector_int16_t shift, vector_bool mask)
{
    return asc_shiftleft_impl(src, shift, mask);
}

__simd_callee__ inline vector_int16_t asc_shiftleft(vector_int16_t src, vector_int16_t shift, vector_bool mask)
{
    return asc_shiftleft_impl(src, shift, mask);
}

__simd_callee__ inline vector_uint32_t asc_shiftleft(vector_uint32_t src, vector_int32_t shift, vector_bool mask)
{
    return asc_shiftleft_impl(src, shift, mask);
}

__simd_callee__ inline vector_int32_t asc_shiftleft(vector_int32_t src, vector_int32_t shift, vector_bool mask)
{
    return asc_shiftleft_impl(src, shift, mask);
}

// ==========asc_shiftright(u8/s8/u16/s16/u32/s32)==========
__simd_callee__ inline void asc_shiftright(
    vector_uint8_t& dst, vector_uint8_t src, vector_int8_t shift, vector_bool mask)
{
    asc_shiftright_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftright(vector_int8_t& dst, vector_int8_t src, vector_int8_t shift, vector_bool mask)
{
    asc_shiftright_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftright(
    vector_uint16_t& dst, vector_uint16_t src, vector_int16_t shift, vector_bool mask)
{
    asc_shiftright_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftright(
    vector_int16_t& dst, vector_int16_t src, vector_int16_t shift, vector_bool mask)
{
    asc_shiftright_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftright(
    vector_uint32_t& dst, vector_uint32_t src, vector_int32_t shift, vector_bool mask)
{
    asc_shiftright_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftright(
    vector_int32_t& dst, vector_int32_t src, vector_int32_t shift, vector_bool mask)
{
    asc_shiftright_impl(dst, src, shift, mask);
}

__simd_callee__ inline vector_uint8_t asc_shiftright(vector_uint8_t src, vector_int8_t shift, vector_bool mask)
{
    return asc_shiftright_impl(src, shift, mask);
}

__simd_callee__ inline vector_int8_t asc_shiftright(vector_int8_t src, vector_int8_t shift, vector_bool mask)
{
    return asc_shiftright_impl(src, shift, mask);
}

__simd_callee__ inline vector_uint16_t asc_shiftright(vector_uint16_t src, vector_int16_t shift, vector_bool mask)
{
    return asc_shiftright_impl(src, shift, mask);
}

__simd_callee__ inline vector_int16_t asc_shiftright(vector_int16_t src, vector_int16_t shift, vector_bool mask)
{
    return asc_shiftright_impl(src, shift, mask);
}

__simd_callee__ inline vector_uint32_t asc_shiftright(vector_uint32_t src, vector_int32_t shift, vector_bool mask)
{
    return asc_shiftright_impl(src, shift, mask);
}

__simd_callee__ inline vector_int32_t asc_shiftright(vector_int32_t src, vector_int32_t shift, vector_bool mask)
{
    return asc_shiftright_impl(src, shift, mask);
}

// ==========asc_not(u8/s8/u16/s16/half/u32/s32/f32/bool)==========
__simd_callee__ inline void asc_not(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_not_impl(dst, src, mask);
}

__simd_callee__ inline vector_uint8_t asc_not(vector_uint8_t src, vector_bool mask)
{
    vector_uint8_t dst;
    asc_not_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_not(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_not_impl(dst, src, mask);
}

__simd_callee__ inline vector_int8_t asc_not(vector_int8_t src, vector_bool mask)
{
    vector_int8_t dst;
    asc_not_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_not(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_not_impl(dst, src, mask);
}

__simd_callee__ inline vector_uint16_t asc_not(vector_uint16_t src, vector_bool mask)
{
    vector_uint16_t dst;
    asc_not_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_not(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_not_impl(dst, src, mask);
}

__simd_callee__ inline vector_int16_t asc_not(vector_int16_t src, vector_bool mask)
{
    vector_int16_t dst;
    asc_not_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_not(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_not_impl(dst, src, mask);
}

__simd_callee__ inline vector_half asc_not(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_not_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_not(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_not_impl(dst, src, mask);
}

__simd_callee__ inline vector_uint32_t asc_not(vector_uint32_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_not_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_not(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_not_impl(dst, src, mask);
}

__simd_callee__ inline vector_int32_t asc_not(vector_int32_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_not_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_not(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_not_impl(dst, src, mask);
}

__simd_callee__ inline vector_float asc_not(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_not_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_not(vector_bool& dst, vector_bool src, vector_bool mask)
{
    asc_not_impl(dst, src, mask);
}

__simd_callee__ inline vector_bool asc_not(vector_bool src, vector_bool mask)
{
    vector_bool dst;
    asc_not_impl(dst, src, mask);
    return dst;
}

// ==========asc_and(bool/uint8_t/int8_t/fp8_e4m3/fp8_e5m2/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_and(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_bool asc_and(vector_bool src0, vector_bool src1, vector_bool mask)
{
    vector_bool dst;
    asc_and_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_and(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_uint8_t asc_and(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    vector_uint8_t dst;
    asc_and_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_and(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_int8_t asc_and(vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    vector_int8_t dst;
    asc_and_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_and(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_uint16_t asc_and(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    vector_uint16_t dst;
    asc_and_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_and(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_int16_t asc_and(vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    vector_int16_t dst;
    asc_and_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_and(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_uint32_t asc_and(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    vector_uint32_t dst;
    asc_and_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_and(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_int32_t asc_and(vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    vector_int32_t dst;
    asc_and_impl(dst, src0, src1, mask);
    return dst;
}

// ==========asc_shiftleft_scalar(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t)==========
__simd_callee__ inline void asc_shiftleft_scalar(
    vector_uint8_t& dst, vector_uint8_t src, int16_t shift, vector_bool mask)
{
    asc_shiftleft_scalar_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftleft_scalar(vector_int8_t& dst, vector_int8_t src, int16_t shift, vector_bool mask)
{
    asc_shiftleft_scalar_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_uint16_t& dst, vector_uint16_t src, int16_t shift, vector_bool mask)
{
    asc_shiftleft_scalar_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_int16_t& dst, vector_int16_t src, int16_t shift, vector_bool mask)
{
    asc_shiftleft_scalar_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_uint32_t& dst, vector_uint32_t src, int16_t shift, vector_bool mask)
{
    asc_shiftleft_scalar_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftleft_scalar(
    vector_int32_t& dst, vector_int32_t src, int16_t shift, vector_bool mask)
{
    asc_shiftleft_scalar_impl(dst, src, shift, mask);
}

__simd_callee__ inline vector_uint8_t asc_shiftleft_scalar(vector_uint8_t src, int16_t shift, vector_bool mask)
{
    return asc_shiftleft_scalar_impl(src, shift, mask);
}

__simd_callee__ inline vector_int8_t asc_shiftleft_scalar(vector_int8_t src, int16_t shift, vector_bool mask)
{
    return asc_shiftleft_scalar_impl(src, shift, mask);
}

__simd_callee__ inline vector_uint16_t asc_shiftleft_scalar(vector_uint16_t src, int16_t shift, vector_bool mask)
{
    return asc_shiftleft_scalar_impl(src, shift, mask);
}

__simd_callee__ inline vector_int16_t asc_shiftleft_scalar(vector_int16_t src, int16_t shift, vector_bool mask)
{
    return asc_shiftleft_scalar_impl(src, shift, mask);
}

__simd_callee__ inline vector_uint32_t asc_shiftleft_scalar(vector_uint32_t src, int16_t shift, vector_bool mask)
{
    return asc_shiftleft_scalar_impl(src, shift, mask);
}

__simd_callee__ inline vector_int32_t asc_shiftleft_scalar(vector_int32_t src, int16_t shift, vector_bool mask)
{
    return asc_shiftleft_scalar_impl(src, shift, mask);
}

// ==========asc_shiftright_scalar(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t)==========
__simd_callee__ inline void asc_shiftright_scalar(
    vector_uint8_t& dst, vector_uint8_t src, int16_t shift, vector_bool mask)
{
    asc_shiftright_scalar_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftright_scalar(
    vector_int8_t& dst, vector_int8_t src, int16_t shift, vector_bool mask)
{
    asc_shiftright_scalar_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftright_scalar(
    vector_uint16_t& dst, vector_uint16_t src, int16_t shift, vector_bool mask)
{
    asc_shiftright_scalar_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftright_scalar(
    vector_int16_t& dst, vector_int16_t src, int16_t shift, vector_bool mask)
{
    asc_shiftright_scalar_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftright_scalar(
    vector_uint32_t& dst, vector_uint32_t src, int16_t shift, vector_bool mask)
{
    asc_shiftright_scalar_impl(dst, src, shift, mask);
}

__simd_callee__ inline void asc_shiftright_scalar(
    vector_int32_t& dst, vector_int32_t src, int16_t shift, vector_bool mask)
{
    asc_shiftright_scalar_impl(dst, src, shift, mask);
}

__simd_callee__ inline vector_uint8_t asc_shiftright_scalar(vector_uint8_t src, int16_t shift, vector_bool mask)
{
    return asc_shiftright_scalar_impl(src, shift, mask);
}

__simd_callee__ inline vector_int8_t asc_shiftright_scalar(vector_int8_t src, int16_t shift, vector_bool mask)
{
    return asc_shiftright_scalar_impl(src, shift, mask);
}

__simd_callee__ inline vector_uint16_t asc_shiftright_scalar(vector_uint16_t src, int16_t shift, vector_bool mask)
{
    return asc_shiftright_scalar_impl(src, shift, mask);
}

__simd_callee__ inline vector_int16_t asc_shiftright_scalar(vector_int16_t src, int16_t shift, vector_bool mask)
{
    return asc_shiftright_scalar_impl(src, shift, mask);
}

__simd_callee__ inline vector_uint32_t asc_shiftright_scalar(vector_uint32_t src, int16_t shift, vector_bool mask)
{
    return asc_shiftright_scalar_impl(src, shift, mask);
}

__simd_callee__ inline vector_int32_t asc_shiftright_scalar(vector_int32_t src, int16_t shift, vector_bool mask)
{
    return asc_shiftright_scalar_impl(src, shift, mask);
}

// ==========asc_or(int8_t/uint8_t/int16_t/uint16_t/half/int32_t/uint32_t/float/bool)==========
__simd_callee__ inline void asc_or(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    asc_or_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_int8_t asc_or(vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    vector_int8_t dst;
    asc_or_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_or(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    asc_or_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_uint8_t asc_or(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    vector_uint8_t dst;
    asc_or_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_or(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    asc_or_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_int16_t asc_or(vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    vector_int16_t dst;
    asc_or_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_or(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    asc_or_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_uint16_t asc_or(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    vector_uint16_t dst;
    asc_or_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_or(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    asc_or_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_int32_t asc_or(vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    vector_int32_t dst;
    asc_or_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_or(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    asc_or_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_uint32_t asc_or(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    vector_uint32_t dst;
    asc_or_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline void asc_or(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask)
{
    asc_or_impl(dst, src0, src1, mask);
}

__simd_callee__ inline vector_bool asc_or(vector_bool src0, vector_bool src1, vector_bool mask)
{
    vector_bool dst;
    asc_or_impl(dst, src0, src1, mask);
    return dst;
}

//=================asc_xor==================
__simd_callee__ inline void asc_xor(vector_int32_t& dst, vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    asc_xor_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_xor(vector_uint32_t& dst, vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    asc_xor_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_xor(vector_uint16_t& dst, vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    asc_xor_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_xor(vector_int16_t& dst, vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    asc_xor_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_xor(vector_uint8_t& dst, vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    asc_xor_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_xor(vector_int8_t& dst, vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    asc_xor_impl(dst, src0, src1, mask);
}

__simd_callee__ inline void asc_xor(vector_bool& dst, vector_bool src0, vector_bool src1, vector_bool mask)
{
    asc_xor_impl(dst, src0, src1, mask);
}

//=================asc_xor return-value overloads==================
__simd_callee__ inline vector_int32_t asc_xor(vector_int32_t src0, vector_int32_t src1, vector_bool mask)
{
    vector_int32_t dst;
    asc_xor_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_xor(vector_uint32_t src0, vector_uint32_t src1, vector_bool mask)
{
    vector_uint32_t dst;
    asc_xor_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_xor(vector_uint16_t src0, vector_uint16_t src1, vector_bool mask)
{
    vector_uint16_t dst;
    asc_xor_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_xor(vector_int16_t src0, vector_int16_t src1, vector_bool mask)
{
    vector_int16_t dst;
    asc_xor_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_uint8_t asc_xor(vector_uint8_t src0, vector_uint8_t src1, vector_bool mask)
{
    vector_uint8_t dst;
    asc_xor_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_xor(vector_int8_t src0, vector_int8_t src1, vector_bool mask)
{
    vector_int8_t dst;
    asc_xor_impl(dst, src0, src1, mask);
    return dst;
}

__simd_callee__ inline vector_bool asc_xor(vector_bool src0, vector_bool src1, vector_bool mask)
{
    vector_bool dst;
    asc_xor_impl(dst, src0, src1, mask);
    return dst;
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_REG_LOGIC_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_REG_LOGIC_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_REG_LOGIC_INTF_IMPL_H
