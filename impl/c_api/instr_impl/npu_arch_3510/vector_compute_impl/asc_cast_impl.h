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
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_CAST_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_CAST_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_cast_impl(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_P0, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_P0, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_bfloat16_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_P0, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_P0, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_P0, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P0, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P0, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_half& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, RS_DISABLE, ROUND_F, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_half& dst, vector_hifloat8_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_half& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_half& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_half& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P0, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int16_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, RS_DISABLE, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int16_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int16_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int32_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, RS_DISABLE, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int32_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int32_t& dst, vector_int64_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int32_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_P0, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int64_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_uint16_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, RS_ENABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_uint32_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, PART_P0, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, RS_DISABLE, PART_P0, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
}
__simd_callee__ inline void asc_cast_impl(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV { vcvt(dst, src, mask, RS_DISABLE, PART_P0, MODE_ZEROING); }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_half src, vector_bool mask, asc_position_mode src_pos)
{
    if ASC_IS_AIV {
        if (src_pos == asc_position_mode::EVEN) { vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING); }
        else { vcvt(dst, src, mask, PART_ODD, MODE_ZEROING); }
    }
}
__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask, asc_position_mode dst_pos)
{
    if ASC_IS_AIV {
        if (dst_pos == asc_position_mode::EVEN) { vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING); }
        else { vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING); }
    }
}
__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask, asc_position_quarter_mode src_pos)
{
    if ASC_IS_AIV {
        if (src_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) { vcvt(dst, src, mask, PART_P0, MODE_ZEROING); }
        else if (src_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) { vcvt(dst, src, mask, PART_P1, MODE_ZEROING); }
        else if (src_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) { vcvt(dst, src, mask, PART_P2, MODE_ZEROING); }
        else { vcvt(dst, src, mask, PART_P3, MODE_ZEROING); }
    }
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
