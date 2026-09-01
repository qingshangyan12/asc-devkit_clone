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
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_bfloat16_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_half& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, ROUND_F, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_half& dst, vector_hifloat8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_half& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_half& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_half& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int16_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int16_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int16_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int32_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int32_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int32_t& dst, vector_int64_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int32_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int64_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_uint16_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_uint32_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_cast_impl(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_half src, vector_bool mask, asc_position_mode src_pos)
{
    if ASC_IS_AIV {
        if (src_pos == asc_position_mode::EVEN) {
            vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
        } else {
            vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
        }
    }
}
__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask, asc_position_mode dst_pos)
{
    if ASC_IS_AIV {
        if (dst_pos == asc_position_mode::EVEN) {
            vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
        } else {
            vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
        }
    }
}
__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask, asc_position_quarter_mode src_pos)
{
    if ASC_IS_AIV {
        if (src_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) {
            vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
        } else if (src_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) {
            vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
        } else if (src_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) {
            vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
        } else {
            vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
        }
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask, asc_position_quarter_mode src_pos)
{
    if (src_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) {
        asc_e1m2x22bfloat16_impl(dst, src, mask);
    } else if (src_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) {
        asc_e1m2x22bfloat16_v2_impl(dst, src, mask);
    } else if (src_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) {
        asc_e1m2x22bfloat16_v3_impl(dst, src, mask);
    } else {
        asc_e1m2x22bfloat16_v4_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask, asc_position_quarter_mode src_pos)
{
    if (src_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) {
        asc_e2m1x22bfloat16_impl(dst, src, mask);
    } else if (src_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) {
        asc_e2m1x22bfloat16_v2_impl(dst, src, mask);
    } else if (src_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) {
        asc_e2m1x22bfloat16_v3_impl(dst, src, mask);
    } else {
        asc_e2m1x22bfloat16_v4_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_bfloat16_t src, vector_bool mask, asc_position_mode src_pos)
{
    src_pos == asc_position_mode::ODD ? asc_bfloat162float_v2_impl(dst, src, mask) :
                                        asc_bfloat162float_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask, asc_position_quarter_mode src_pos)
{
    if (src_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) {
        asc_e5m22float_impl(dst, src, mask);
    } else if (src_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) {
        asc_e5m22float_v2_impl(dst, src, mask);
    } else if (src_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) {
        asc_e5m22float_v3_impl(dst, src, mask);
    } else {
        asc_e5m22float_v4_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask, asc_position_quarter_mode src_pos)
{
    if (src_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) {
        asc_hif82float_impl(dst, src, mask);
    } else if (src_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) {
        asc_hif82float_v2_impl(dst, src, mask);
    } else if (src_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) {
        asc_hif82float_v3_impl(dst, src, mask);
    } else {
        asc_hif82float_v4_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int16_t src, vector_bool mask, asc_position_mode src_pos)
{
    src_pos == asc_position_mode::ODD ? asc_int162float_v2_impl(dst, src, mask) : asc_int162float_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_hifloat8_t src, vector_bool mask, asc_position_mode src_pos)
{
    src_pos == asc_position_mode::ODD ? asc_hif82half_v2_impl(dst, src, mask) : asc_hif82half_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int8_t src, vector_bool mask, asc_position_mode src_pos)
{
    src_pos == asc_position_mode::ODD ? asc_int82half_v2_impl(dst, src, mask) : asc_int82half_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_uint8_t src, vector_bool mask, asc_position_mode src_pos)
{
    src_pos == asc_position_mode::ODD ? asc_uint82half_v2_impl(dst, src, mask) : asc_uint82half_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int8_t src, vector_bool mask, asc_position_mode src_pos)
{
    src_pos == asc_position_mode::ODD ? asc_int82int16_v2_impl(dst, src, mask) : asc_int82int16_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int16_t src, vector_bool mask, asc_position_mode src_pos)
{
    src_pos == asc_position_mode::ODD ? asc_int162int32_v2_impl(dst, src, mask) : asc_int162int32_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask, asc_position_quarter_mode src_pos)
{
    if (src_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) {
        asc_int82int32_impl(dst, src, mask);
    } else if (src_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) {
        asc_int82int32_v2_impl(dst, src, mask);
    } else if (src_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) {
        asc_int82int32_v3_impl(dst, src, mask);
    } else {
        asc_int82int32_v4_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_int32_t src, vector_bool mask, asc_position_mode src_pos)
{
    src_pos == asc_position_mode::ODD ? asc_int322int64_v2_impl(dst, src, mask) : asc_int322int64_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask, asc_position_mode src_pos)
{
    src_pos == asc_position_mode::ODD ? asc_uint82uint16_v2_impl(dst, src, mask) :
                                        asc_uint82uint16_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_int16_t src, vector_bool mask, asc_position_mode src_pos)
{
    src_pos == asc_position_mode::ODD ? asc_int162uint32_v2_impl(dst, src, mask) :
                                        asc_int162uint32_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint16_t src, vector_bool mask, asc_position_mode src_pos)
{
    src_pos == asc_position_mode::ODD ? asc_uint162uint32_v2_impl(dst, src, mask) :
                                        asc_uint162uint32_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask, asc_position_quarter_mode src_pos)
{
    if (src_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) {
        asc_uint82uint32_impl(dst, src, mask);
    } else if (src_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) {
        asc_uint82uint32_v2_impl(dst, src, mask);
    } else if (src_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) {
        asc_uint82uint32_v3_impl(dst, src, mask);
    } else {
        asc_uint82uint32_v4_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask, asc_cast_round_mode round_mode,
    asc_cast_saturation_mode saturation_mode, asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    switch (round_mode) {
        case ASC_CAST_RN:
            if (sat) {
                odd ? asc_float2bfloat16_rn_sat_v2_impl(dst, src, mask) :
                      asc_float2bfloat16_rn_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2bfloat16_rn_v2_impl(dst, src, mask) : asc_float2bfloat16_rn_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RNA:
            if (sat) {
                odd ? asc_float2bfloat16_rna_sat_v2_impl(dst, src, mask) :
                      asc_float2bfloat16_rna_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2bfloat16_rna_v2_impl(dst, src, mask) : asc_float2bfloat16_rna_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RD:
            if (sat) {
                odd ? asc_float2bfloat16_rd_sat_v2_impl(dst, src, mask) :
                      asc_float2bfloat16_rd_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2bfloat16_rd_v2_impl(dst, src, mask) : asc_float2bfloat16_rd_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RU:
            if (sat) {
                odd ? asc_float2bfloat16_ru_sat_v2_impl(dst, src, mask) :
                      asc_float2bfloat16_ru_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2bfloat16_ru_v2_impl(dst, src, mask) : asc_float2bfloat16_ru_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RZ:
        default:
            if (sat) {
                odd ? asc_float2bfloat16_rz_sat_v2_impl(dst, src, mask) :
                      asc_float2bfloat16_rz_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2bfloat16_rz_v2_impl(dst, src, mask) : asc_float2bfloat16_rz_impl(dst, src, mask);
            }
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask, asc_cast_round_mode round_mode)
{
    switch (round_mode) {
        case ASC_CAST_RN:
            asc_half2bfloat16_rn_impl(dst, src, mask);
            break;
        case ASC_CAST_RNA:
            asc_half2bfloat16_rna_impl(dst, src, mask);
            break;
        case ASC_CAST_RD:
            asc_half2bfloat16_rd_impl(dst, src, mask);
            break;
        case ASC_CAST_RU:
            asc_half2bfloat16_ru_impl(dst, src, mask);
            break;
        case ASC_CAST_RZ:
        default:
            asc_half2bfloat16_rz_impl(dst, src, mask);
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int32_t src, vector_bool mask, asc_cast_round_mode round_mode)
{
    switch (round_mode) {
        case ASC_CAST_RN:
            asc_int322float_rn_impl(dst, src, mask);
            break;
        case ASC_CAST_RNA:
            asc_int322float_rna_impl(dst, src, mask);
            break;
        case ASC_CAST_RD:
            asc_int322float_rd_impl(dst, src, mask);
            break;
        case ASC_CAST_RU:
            asc_int322float_ru_impl(dst, src, mask);
            break;
        case ASC_CAST_RZ:
        default:
            asc_int322float_rz_impl(dst, src, mask);
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask, asc_cast_round_mode round_mode, asc_position_mode src_pos)
{
    bool odd = src_pos == asc_position_mode::ODD;
    switch (round_mode) {
        case ASC_CAST_RN:
            odd ? asc_int642float_rn_v2_impl(dst, src, mask) : asc_int642float_rn_impl(dst, src, mask);
            break;
        case ASC_CAST_RNA:
            odd ? asc_int642float_rna_v2_impl(dst, src, mask) : asc_int642float_rna_impl(dst, src, mask);
            break;
        case ASC_CAST_RD:
            odd ? asc_int642float_rd_v2_impl(dst, src, mask) : asc_int642float_rd_impl(dst, src, mask);
            break;
        case ASC_CAST_RU:
            odd ? asc_int642float_ru_v2_impl(dst, src, mask) : asc_int642float_ru_impl(dst, src, mask);
            break;
        case ASC_CAST_RZ:
        default:
            odd ? asc_int642float_rz_v2_impl(dst, src, mask) : asc_int642float_rz_impl(dst, src, mask);
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask, asc_cast_saturation_mode saturation_mode,
    asc_position_quarter_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    if (dst_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) {
        sat ? asc_float2e4m3_rn_sat_impl(dst, src, mask) : asc_float2e4m3_rn_impl(dst, src, mask);
    } else if (dst_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) {
        sat ? asc_float2e4m3_rn_sat_v2_impl(dst, src, mask) : asc_float2e4m3_rn_v2_impl(dst, src, mask);
    } else if (dst_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) {
        sat ? asc_float2e4m3_rn_sat_v3_impl(dst, src, mask) : asc_float2e4m3_rn_v3_impl(dst, src, mask);
    } else {
        sat ? asc_float2e4m3_rn_sat_v4_impl(dst, src, mask) : asc_float2e4m3_rn_v4_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask, asc_cast_saturation_mode saturation_mode,
    asc_position_quarter_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    if (dst_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) {
        sat ? asc_float2e5m2_rn_sat_impl(dst, src, mask) : asc_float2e5m2_rn_impl(dst, src, mask);
    } else if (dst_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) {
        sat ? asc_float2e5m2_rn_sat_v2_impl(dst, src, mask) : asc_float2e5m2_rn_v2_impl(dst, src, mask);
    } else if (dst_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) {
        sat ? asc_float2e5m2_rn_sat_v3_impl(dst, src, mask) : asc_float2e5m2_rn_v3_impl(dst, src, mask);
    } else {
        sat ? asc_float2e5m2_rn_sat_v4_impl(dst, src, mask) : asc_float2e5m2_rn_v4_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask, asc_cast_round_mode round_mode,
    asc_cast_saturation_mode saturation_mode)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    switch (round_mode) {
        case ASC_CAST_RN:
            sat ? asc_bfloat162half_rn_sat_impl(dst, src, mask) : asc_bfloat162half_rn_impl(dst, src, mask);
            break;
        case ASC_CAST_RNA:
            sat ? asc_bfloat162half_rna_sat_impl(dst, src, mask) : asc_bfloat162half_rna_impl(dst, src, mask);
            break;
        case ASC_CAST_RD:
            sat ? asc_bfloat162half_rd_sat_impl(dst, src, mask) : asc_bfloat162half_rd_impl(dst, src, mask);
            break;
        case ASC_CAST_RU:
            sat ? asc_bfloat162half_ru_sat_impl(dst, src, mask) : asc_bfloat162half_ru_impl(dst, src, mask);
            break;
        case ASC_CAST_RZ:
        default:
            sat ? asc_bfloat162half_rz_sat_impl(dst, src, mask) : asc_bfloat162half_rz_impl(dst, src, mask);
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask, asc_cast_round_mode round_mode,
    asc_cast_saturation_mode saturation_mode, asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    switch (round_mode) {
        case ASC_CAST_RN:
            if (sat) {
                odd ? asc_float2half_rn_sat_v2_impl(dst, src, mask) : asc_float2half_rn_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2half_rn_v2_impl(dst, src, mask) : asc_float2half_rn_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RNA:
            if (sat) {
                odd ? asc_float2half_rna_sat_v2_impl(dst, src, mask) : asc_float2half_rna_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2half_rna_v2_impl(dst, src, mask) : asc_float2half_rna_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RD:
            if (sat) {
                odd ? asc_float2half_rd_sat_v2_impl(dst, src, mask) : asc_float2half_rd_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2half_rd_v2_impl(dst, src, mask) : asc_float2half_rd_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RU:
            if (sat) {
                odd ? asc_float2half_ru_sat_v2_impl(dst, src, mask) : asc_float2half_ru_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2half_ru_v2_impl(dst, src, mask) : asc_float2half_ru_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RO:
            if (sat) {
                odd ? asc_float2half_ro_sat_v2_impl(dst, src, mask) : asc_float2half_ro_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2half_ro_v2_impl(dst, src, mask) : asc_float2half_ro_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RZ:
        default:
            if (sat) {
                odd ? asc_float2half_rz_sat_v2_impl(dst, src, mask) : asc_float2half_rz_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2half_rz_v2_impl(dst, src, mask) : asc_float2half_rz_impl(dst, src, mask);
            }
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask, asc_cast_round_mode round_mode,
    asc_cast_saturation_mode saturation_mode, asc_position_quarter_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool rna = round_mode == ASC_CAST_RNA;
    if (dst_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) {
        if (rna) {
            sat ? asc_float2hif8_rna_sat_impl(dst, src, mask) : asc_float2hif8_rna_impl(dst, src, mask);
        } else {
            sat ? asc_float2hif8_rh_sat_impl(dst, src, mask) : asc_float2hif8_rh_impl(dst, src, mask);
        }
    } else if (dst_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) {
        if (rna) {
            sat ? asc_float2hif8_rna_sat_v2_impl(dst, src, mask) : asc_float2hif8_rna_v2_impl(dst, src, mask);
        } else {
            sat ? asc_float2hif8_rh_sat_v2_impl(dst, src, mask) : asc_float2hif8_rh_v2_impl(dst, src, mask);
        }
    } else if (dst_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) {
        if (rna) {
            sat ? asc_float2hif8_rna_sat_v3_impl(dst, src, mask) : asc_float2hif8_rna_v3_impl(dst, src, mask);
        } else {
            sat ? asc_float2hif8_rh_sat_v3_impl(dst, src, mask) : asc_float2hif8_rh_v3_impl(dst, src, mask);
        }
    } else {
        if (rna) {
            sat ? asc_float2hif8_rna_sat_v4_impl(dst, src, mask) : asc_float2hif8_rna_v4_impl(dst, src, mask);
        } else {
            sat ? asc_float2hif8_rh_sat_v4_impl(dst, src, mask) : asc_float2hif8_rh_v4_impl(dst, src, mask);
        }
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask, asc_cast_round_mode round_mode,
    asc_cast_saturation_mode saturation_mode, asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    bool rna = round_mode == ASC_CAST_RNA;
    if (rna) {
        if (sat) {
            odd ? asc_half2hif8_rna_sat_v2_impl(dst, src, mask) : asc_half2hif8_rna_sat_impl(dst, src, mask);
        } else {
            odd ? asc_half2hif8_rna_v2_impl(dst, src, mask) : asc_half2hif8_rna_impl(dst, src, mask);
        }
    } else {
        if (sat) {
            odd ? asc_half2hif8_rh_sat_v2_impl(dst, src, mask) : asc_half2hif8_rh_sat_impl(dst, src, mask);
        } else {
            odd ? asc_half2hif8_rh_v2_impl(dst, src, mask) : asc_half2hif8_rh_impl(dst, src, mask);
        }
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask, asc_cast_round_mode round_mode,
    asc_cast_saturation_mode saturation_mode, asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    switch (round_mode) {
        case ASC_CAST_RN:
            if (sat) {
                odd ? asc_float2int16_rn_sat_v2_impl(dst, src, mask) : asc_float2int16_rn_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2int16_rn_v2_impl(dst, src, mask) : asc_float2int16_rn_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RNA:
            if (sat) {
                odd ? asc_float2int16_rna_sat_v2_impl(dst, src, mask) : asc_float2int16_rna_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2int16_rna_v2_impl(dst, src, mask) : asc_float2int16_rna_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RD:
            if (sat) {
                odd ? asc_float2int16_rd_sat_v2_impl(dst, src, mask) : asc_float2int16_rd_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2int16_rd_v2_impl(dst, src, mask) : asc_float2int16_rd_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RU:
            if (sat) {
                odd ? asc_float2int16_ru_sat_v2_impl(dst, src, mask) : asc_float2int16_ru_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2int16_ru_v2_impl(dst, src, mask) : asc_float2int16_ru_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RZ:
        default:
            if (sat) {
                odd ? asc_float2int16_rz_sat_v2_impl(dst, src, mask) : asc_float2int16_rz_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2int16_rz_v2_impl(dst, src, mask) : asc_float2int16_rz_impl(dst, src, mask);
            }
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask, asc_cast_round_mode round_mode,
    asc_cast_saturation_mode saturation_mode)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    switch (round_mode) {
        case ASC_CAST_RN:
            sat ? asc_half2int16_rn_sat_impl(dst, src, mask) : asc_half2int16_rn_impl(dst, src, mask);
            break;
        case ASC_CAST_RNA:
            sat ? asc_half2int16_rna_sat_impl(dst, src, mask) : asc_half2int16_rna_impl(dst, src, mask);
            break;
        case ASC_CAST_RD:
            sat ? asc_half2int16_rd_sat_impl(dst, src, mask) : asc_half2int16_rd_impl(dst, src, mask);
            break;
        case ASC_CAST_RU:
            sat ? asc_half2int16_ru_sat_impl(dst, src, mask) : asc_half2int16_ru_impl(dst, src, mask);
            break;
        case ASC_CAST_RZ:
        default:
            sat ? asc_half2int16_rz_sat_impl(dst, src, mask) : asc_half2int16_rz_impl(dst, src, mask);
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, asc_cast_round_mode round_mode,
    asc_cast_saturation_mode saturation_mode, asc_position_mode src_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = src_pos == asc_position_mode::ODD;
    switch (round_mode) {
        case ASC_CAST_RN:
            if (sat) {
                odd ? asc_bfloat162int32_rn_sat_v2_impl(dst, src, mask) :
                      asc_bfloat162int32_rn_sat_impl(dst, src, mask);
            } else {
                odd ? asc_bfloat162int32_rn_v2_impl(dst, src, mask) : asc_bfloat162int32_rn_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RNA:
            if (sat) {
                odd ? asc_bfloat162int32_rna_sat_v2_impl(dst, src, mask) :
                      asc_bfloat162int32_rna_sat_impl(dst, src, mask);
            } else {
                odd ? asc_bfloat162int32_rna_v2_impl(dst, src, mask) : asc_bfloat162int32_rna_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RD:
            if (sat) {
                odd ? asc_bfloat162int32_rd_sat_v2_impl(dst, src, mask) :
                      asc_bfloat162int32_rd_sat_impl(dst, src, mask);
            } else {
                odd ? asc_bfloat162int32_rd_v2_impl(dst, src, mask) : asc_bfloat162int32_rd_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RU:
            if (sat) {
                odd ? asc_bfloat162int32_ru_sat_v2_impl(dst, src, mask) :
                      asc_bfloat162int32_ru_sat_impl(dst, src, mask);
            } else {
                odd ? asc_bfloat162int32_ru_v2_impl(dst, src, mask) : asc_bfloat162int32_ru_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RZ:
        default:
            if (sat) {
                odd ? asc_bfloat162int32_rz_sat_v2_impl(dst, src, mask) :
                      asc_bfloat162int32_rz_sat_impl(dst, src, mask);
            } else {
                odd ? asc_bfloat162int32_rz_v2_impl(dst, src, mask) : asc_bfloat162int32_rz_impl(dst, src, mask);
            }
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask, asc_cast_round_mode round_mode,
    asc_cast_saturation_mode saturation_mode)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    switch (round_mode) {
        case ASC_CAST_RN:
            sat ? asc_float2int32_rn_sat_impl(dst, src, mask) : asc_float2int32_rn_impl(dst, src, mask);
            break;
        case ASC_CAST_RNA:
            sat ? asc_float2int32_rna_sat_impl(dst, src, mask) : asc_float2int32_rna_impl(dst, src, mask);
            break;
        case ASC_CAST_RD:
            sat ? asc_float2int32_rd_sat_impl(dst, src, mask) : asc_float2int32_rd_impl(dst, src, mask);
            break;
        case ASC_CAST_RU:
            sat ? asc_float2int32_ru_sat_impl(dst, src, mask) : asc_float2int32_ru_impl(dst, src, mask);
            break;
        case ASC_CAST_RZ:
        default:
            sat ? asc_float2int32_rz_sat_impl(dst, src, mask) : asc_float2int32_rz_impl(dst, src, mask);
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask, asc_cast_round_mode round_mode, asc_position_mode src_pos)
{
    bool odd = src_pos == asc_position_mode::ODD;
    switch (round_mode) {
        case ASC_CAST_RN:
            odd ? asc_half2int32_rn_v2_impl(dst, src, mask) : asc_half2int32_rn_impl(dst, src, mask);
            break;
        case ASC_CAST_RNA:
            odd ? asc_half2int32_rna_v2_impl(dst, src, mask) : asc_half2int32_rna_impl(dst, src, mask);
            break;
        case ASC_CAST_RD:
            odd ? asc_half2int32_rd_v2_impl(dst, src, mask) : asc_half2int32_rd_impl(dst, src, mask);
            break;
        case ASC_CAST_RU:
            odd ? asc_half2int32_ru_v2_impl(dst, src, mask) : asc_half2int32_ru_impl(dst, src, mask);
            break;
        case ASC_CAST_RZ:
        default:
            odd ? asc_half2int32_rz_v2_impl(dst, src, mask) : asc_half2int32_rz_impl(dst, src, mask);
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask, asc_cast_round_mode round_mode,
    asc_cast_saturation_mode saturation_mode, asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    switch (round_mode) {
        case ASC_CAST_RN:
            if (sat) {
                odd ? asc_float2int64_rn_sat_v2_impl(dst, src, mask) : asc_float2int64_rn_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2int64_rn_v2_impl(dst, src, mask) : asc_float2int64_rn_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RNA:
            if (sat) {
                odd ? asc_float2int64_rna_sat_v2_impl(dst, src, mask) : asc_float2int64_rna_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2int64_rna_v2_impl(dst, src, mask) : asc_float2int64_rna_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RD:
            if (sat) {
                odd ? asc_float2int64_rd_sat_v2_impl(dst, src, mask) : asc_float2int64_rd_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2int64_rd_v2_impl(dst, src, mask) : asc_float2int64_rd_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RU:
            if (sat) {
                odd ? asc_float2int64_ru_sat_v2_impl(dst, src, mask) : asc_float2int64_ru_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2int64_ru_v2_impl(dst, src, mask) : asc_float2int64_ru_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RZ:
        default:
            if (sat) {
                odd ? asc_float2int64_rz_sat_v2_impl(dst, src, mask) : asc_float2int64_rz_sat_impl(dst, src, mask);
            } else {
                odd ? asc_float2int64_rz_v2_impl(dst, src, mask) : asc_float2int64_rz_impl(dst, src, mask);
            }
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask, asc_cast_round_mode round_mode,
    asc_cast_saturation_mode saturation_mode, asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    switch (round_mode) {
        case ASC_CAST_RN:
            if (sat) {
                odd ? asc_half2int8_rn_sat_v2_impl(dst, src, mask) : asc_half2int8_rn_sat_impl(dst, src, mask);
            } else {
                odd ? asc_half2int8_rn_v2_impl(dst, src, mask) : asc_half2int8_rn_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RNA:
            if (sat) {
                odd ? asc_half2int8_rna_sat_v2_impl(dst, src, mask) : asc_half2int8_rna_sat_impl(dst, src, mask);
            } else {
                odd ? asc_half2int8_rna_v2_impl(dst, src, mask) : asc_half2int8_rna_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RD:
            if (sat) {
                odd ? asc_half2int8_rd_sat_v2_impl(dst, src, mask) : asc_half2int8_rd_sat_impl(dst, src, mask);
            } else {
                odd ? asc_half2int8_rd_v2_impl(dst, src, mask) : asc_half2int8_rd_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RU:
            if (sat) {
                odd ? asc_half2int8_ru_sat_v2_impl(dst, src, mask) : asc_half2int8_ru_sat_impl(dst, src, mask);
            } else {
                odd ? asc_half2int8_ru_v2_impl(dst, src, mask) : asc_half2int8_ru_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RZ:
        default:
            if (sat) {
                odd ? asc_half2int8_rz_sat_v2_impl(dst, src, mask) : asc_half2int8_rz_sat_impl(dst, src, mask);
            } else {
                odd ? asc_half2int8_rz_v2_impl(dst, src, mask) : asc_half2int8_rz_impl(dst, src, mask);
            }
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask, asc_cast_round_mode round_mode,
    asc_cast_saturation_mode saturation_mode, asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    switch (round_mode) {
        case ASC_CAST_RN:
            if (sat) {
                odd ? asc_half2uint8_rn_sat_v2_impl(dst, src, mask) : asc_half2uint8_rn_sat_impl(dst, src, mask);
            } else {
                odd ? asc_half2uint8_rn_v2_impl(dst, src, mask) : asc_half2uint8_rn_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RNA:
            if (sat) {
                odd ? asc_half2uint8_rna_sat_v2_impl(dst, src, mask) : asc_half2uint8_rna_sat_impl(dst, src, mask);
            } else {
                odd ? asc_half2uint8_rna_v2_impl(dst, src, mask) : asc_half2uint8_rna_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RD:
            if (sat) {
                odd ? asc_half2uint8_rd_sat_v2_impl(dst, src, mask) : asc_half2uint8_rd_sat_impl(dst, src, mask);
            } else {
                odd ? asc_half2uint8_rd_v2_impl(dst, src, mask) : asc_half2uint8_rd_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RU:
            if (sat) {
                odd ? asc_half2uint8_ru_sat_v2_impl(dst, src, mask) : asc_half2uint8_ru_sat_impl(dst, src, mask);
            } else {
                odd ? asc_half2uint8_ru_v2_impl(dst, src, mask) : asc_half2uint8_ru_impl(dst, src, mask);
            }
            break;
        case ASC_CAST_RZ:
        default:
            if (sat) {
                odd ? asc_half2uint8_rz_sat_v2_impl(dst, src, mask) : asc_half2uint8_rz_sat_impl(dst, src, mask);
            } else {
                odd ? asc_half2uint8_rz_v2_impl(dst, src, mask) : asc_half2uint8_rz_impl(dst, src, mask);
            }
            break;
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask, asc_cast_saturation_mode saturation_mode,
    asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    if (sat) {
        odd ? asc_int322int16_sat_v2_impl(dst, src, mask) : asc_int322int16_sat_impl(dst, src, mask);
    } else {
        odd ? asc_int322int16_v2_impl(dst, src, mask) : asc_int322int16_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask, asc_cast_saturation_mode saturation_mode,
    asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    if (sat) {
        odd ? asc_uint322int16_sat_v2_impl(dst, src, mask) : asc_uint322int16_sat_impl(dst, src, mask);
    } else {
        odd ? asc_uint322int16_v2_impl(dst, src, mask) : asc_uint322int16_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask, asc_cast_saturation_mode saturation_mode,
    asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    if (sat) {
        odd ? asc_int642int32_sat_v2_impl(dst, src, mask) : asc_int642int32_sat_impl(dst, src, mask);
    } else {
        odd ? asc_int642int32_v2_impl(dst, src, mask) : asc_int642int32_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask, asc_cast_saturation_mode saturation_mode,
    asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    if (sat) {
        odd ? asc_int322uint16_sat_v2_impl(dst, src, mask) : asc_int322uint16_sat_impl(dst, src, mask);
    } else {
        odd ? asc_int322uint16_v2_impl(dst, src, mask) : asc_int322uint16_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask, asc_cast_saturation_mode saturation_mode,
    asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    if (sat) {
        odd ? asc_uint322uint16_sat_v2_impl(dst, src, mask) : asc_uint322uint16_sat_impl(dst, src, mask);
    } else {
        odd ? asc_uint322uint16_v2_impl(dst, src, mask) : asc_uint322uint16_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask, asc_cast_saturation_mode saturation_mode,
    asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    if (sat) {
        odd ? asc_int162uint8_sat_v2_impl(dst, src, mask) : asc_int162uint8_sat_impl(dst, src, mask);
    } else {
        odd ? asc_int162uint8_v2_impl(dst, src, mask) : asc_int162uint8_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask, asc_cast_saturation_mode saturation_mode,
    asc_position_quarter_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    if (dst_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) {
        sat ? asc_int322uint8_sat_impl(dst, src, mask) : asc_int322uint8_impl(dst, src, mask);
    } else if (dst_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) {
        sat ? asc_int322uint8_sat_v2_impl(dst, src, mask) : asc_int322uint8_v2_impl(dst, src, mask);
    } else if (dst_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) {
        sat ? asc_int322uint8_sat_v3_impl(dst, src, mask) : asc_int322uint8_v3_impl(dst, src, mask);
    } else {
        sat ? asc_int322uint8_sat_v4_impl(dst, src, mask) : asc_int322uint8_v4_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask, asc_cast_saturation_mode saturation_mode,
    asc_position_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    bool odd = dst_pos == asc_position_mode::ODD;
    if (sat) {
        odd ? asc_uint162uint8_sat_v2_impl(dst, src, mask) : asc_uint162uint8_sat_impl(dst, src, mask);
    } else {
        odd ? asc_uint162uint8_v2_impl(dst, src, mask) : asc_uint162uint8_impl(dst, src, mask);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask, asc_cast_saturation_mode saturation_mode,
    asc_position_quarter_mode dst_pos)
{
    bool sat = saturation_mode == ASC_CAST_SAT;
    if (dst_pos == asc_position_quarter_mode::DISPERSE_FIRST_QUARTER) {
        sat ? asc_uint322uint8_sat_impl(dst, src, mask) : asc_uint322uint8_impl(dst, src, mask);
    } else if (dst_pos == asc_position_quarter_mode::DISPERSE_SECOND_QUARTER) {
        sat ? asc_uint322uint8_sat_v2_impl(dst, src, mask) : asc_uint322uint8_v2_impl(dst, src, mask);
    } else if (dst_pos == asc_position_quarter_mode::DISPERSE_THIRD_QUARTER) {
        sat ? asc_uint322uint8_sat_v3_impl(dst, src, mask) : asc_uint322uint8_v3_impl(dst, src, mask);
    } else {
        sat ? asc_uint322uint8_sat_v4_impl(dst, src, mask) : asc_uint322uint8_v4_impl(dst, src, mask);
    }
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
