/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_COMPUTE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_COMPUTE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/reg_base_impl/npu_arch_3510/reg_compute_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_COMPUTE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/reg_compute_intf_impl.h"
#include "impl/c_api/reg_base_impl/scalar_compute_intf_impl.h"
#include "impl/c_api/reg_base_impl/utils_intf_impl.h"

__simd_callee__ inline void asc_float2half_rd_v2_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2half_rd_sat_v2_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2half_rn_v2_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2half_rn_sat_v2_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2half_rna_v2_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2half_rna_sat_v2_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2half_ro_v2_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_O, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2half_ro_sat_v2_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_O, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2half_ru_v2_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2half_ru_sat_v2_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2half_rz_v2_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2half_rz_sat_v2_impl(vector_half& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

// part_odd
__simd_callee__ inline void asc_uint322int16_v2_impl(vector_int16_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint322int16_sat_v2_impl(vector_int16_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_v2_impl(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_v3_impl(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_v4_impl(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_sat_v2_impl(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_sat_v3_impl(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_sat_v4_impl(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2float_v2_impl(vector_float& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e4m32float_v2_impl(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e4m32float_v3_impl(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e4m32float_v4_impl(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int162float_v2_impl(vector_float& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int4x22int16_v2_impl(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_s42s16(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int4x22int16_v3_impl(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_s42s16(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int4x22int16_v4_impl(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_s42s16(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int322int16_v2_impl(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int322int16_sat_v2_impl(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int322uint8_v2_impl(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int322uint8_sat_v2_impl(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int322uint8_v3_impl(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int322uint8_sat_v3_impl(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int322uint8_v4_impl(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int322uint8_sat_v4_impl(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_v2_impl(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vsqz(dst, src, mask, MODE_STORED);
    }
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(
    vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(
    vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(
    vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_squeeze_with_status_impl(
    vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

__simd_callee__ inline void asc_bfloat162float_v2_impl(vector_float& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2hif8_rh_v2_impl(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2hif8_rh_sat_v2_impl(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2hif8_rna_v2_impl(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2hif8_rna_sat_v2_impl(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int8_rd_v2_impl(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int8_rd_sat_v2_impl(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int8_ru_v2_impl(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int8_ru_sat_v2_impl(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int8_rz_v2_impl(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int8_rz_sat_v2_impl(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int8_rn_v2_impl(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int8_rn_sat_v2_impl(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int8_rna_v2_impl(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int8_rna_sat_v2_impl(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int162uint32_v2_impl(vector_uint32_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int4x22bfloat16_v2_impl(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_s42bf16(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int4x22bfloat16_v3_impl(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_s42bf16(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int4x22bfloat16_v4_impl(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_s42bf16(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint162uint8_v2_impl(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint162uint8_sat_v2_impl(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rd_v2_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rd_sat_v2_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rd_v3_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rd_sat_v3_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rd_v4_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rd_sat_v4_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rn_v2_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rn_sat_v2_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rn_v3_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rn_sat_v3_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rn_v4_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rn_sat_v4_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rna_v2_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rna_sat_v2_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rna_v3_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rna_sat_v3_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rna_v4_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rna_sat_v4_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_ru_v2_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_ru_sat_v2_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_ru_v3_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_ru_sat_v3_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_ru_v4_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_ru_sat_v4_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rz_v2_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rz_sat_v2_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rz_v3_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rz_sat_v3_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rz_v4_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int4x2_rz_sat_v4_impl(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_hif82float_v2_impl(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_hif82float_v3_impl(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_hif82float_v4_impl(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_hif82half_v2_impl(vector_half& dst, vector_hifloat8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint82half_v2_impl(vector_half& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint162uint32_v2_impl(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2uint8_rd_v2_impl(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2uint8_rd_sat_v2_impl(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2uint8_rn_v2_impl(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2uint8_rn_sat_v2_impl(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2uint8_rna_v2_impl(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2uint8_rna_sat_v2_impl(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2uint8_ru_v2_impl(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2uint8_ru_sat_v2_impl(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2uint8_rz_v2_impl(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2uint8_rz_sat_v2_impl(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int4x22half_v2_impl(vector_half& dst, vector_int4x2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_s42f16(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int4x22half_v3_impl(vector_half& dst, vector_int4x2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_s42f16(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int4x22half_v4_impl(vector_half& dst, vector_int4x2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_s42f16(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int82int16_v2_impl(vector_int16_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int162uint8_v2_impl(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int162uint8_sat_v2_impl(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162int32_rn_v2_impl(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162int32_rn_sat_v2_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162int32_rna_v2_impl(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162int32_rna_sat_v2_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162int32_rd_v2_impl(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162int32_rd_sat_v2_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162int32_ru_v2_impl(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162int32_ru_sat_v2_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162int32_rz_v2_impl(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162int32_rz_sat_v2_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int642float_rn_v2_impl(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int642float_rna_v2_impl(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int642float_rd_v2_impl(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int642float_ru_v2_impl(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int642float_rz_v2_impl(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int82half_v2_impl(vector_half& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int162int32_v2_impl(vector_int32_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint322uint8_v2_impl(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint322uint8_sat_v2_impl(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint322uint8_v3_impl(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint322uint8_sat_v3_impl(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint322uint8_v4_impl(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint322uint8_sat_v4_impl(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int16_rd_v2_impl(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int16_rd_sat_v2_impl(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int16_rn_v2_impl(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int16_rn_sat_v2_impl(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int16_rna_v2_impl(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int16_rna_sat_v2_impl(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int16_ru_v2_impl(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int16_ru_sat_v2_impl(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int16_rz_v2_impl(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int16_rz_sat_v2_impl(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rd_v2_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rd_v3_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rd_v4_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rn_v2_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rn_v3_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rn_v4_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rna_v2_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rna_v3_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rna_v4_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_ru_v2_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_ru_v3_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_ru_v4_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rz_v2_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rz_v3_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rz_v4_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_sat_v2_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_v2_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_sat_v3_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_v3_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_sat_v4_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_v4_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_sat_v2_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_v2_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_sat_v3_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_v3_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_sat_v4_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_v4_impl(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e2m1x22bfloat16_v2_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e2m1x22bfloat16_v3_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e2m1x22bfloat16_v4_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint82uint32_v2_impl(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint82uint32_v3_impl(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint82uint32_v4_impl(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint322uint16_sat_v2_impl(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint322uint16_v2_impl(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_muls_v2_impl(vector_half& dst, vector_float src, float value, vector_bool mask)
{
    if ASC_IS_AIV {
        vmulscvt(dst, src, value, mask, PART_ODD);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rd_v2_impl(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rd_sat_v2_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rn_v2_impl(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rn_sat_v2_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rna_v2_impl(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rna_sat_v2_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_ru_v2_impl(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_ru_sat_v2_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rz_v2_impl(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rz_sat_v2_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int64_rd_v2_impl(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int64_rd_sat_v2_impl(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int64_rn_v2_impl(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int64_rn_sat_v2_impl(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int64_rna_v2_impl(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int64_rna_sat_v2_impl(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int64_ru_v2_impl(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int64_ru_sat_v2_impl(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int64_rz_v2_impl(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2int64_rz_sat_v2_impl(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int32_rd_v2_impl(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int32_rn_v2_impl(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int32_rna_v2_impl(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int32_ru_v2_impl(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2int32_rz_v2_impl(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e5m22float_v2_impl(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e5m22float_v3_impl(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e5m22float_v4_impl(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_sat_v2_impl(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_v2_impl(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_sat_v3_impl(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_v3_impl(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_sat_v4_impl(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_v4_impl(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int322int64_v2_impl(vector_int64_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int322uint16_v2_impl(vector_uint16_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int322uint16_sat_v2_impl(vector_uint16_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int642int32_v2_impl(vector_int32_t& dst, vector_int64_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int642int32_sat_v2_impl(vector_int32_t& dst, vector_int64_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_uint82uint16_v2_impl(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_exp_sub_v2_impl(vector_float& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vexpdif(dst, src0, src1, mask, PART_ODD);
    }
}

__simd_callee__ inline void asc_exp_sub_v2_impl(
    vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    if ASC_IS_AIV {
        vexpdif(dst, src0, src1, mask, PART_ODD);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rd_v2_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rd_v3_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rd_v4_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rn_v2_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rn_v3_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rn_v4_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rna_v2_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rna_v3_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rna_v4_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_ru_v2_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_ru_v3_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_ru_v4_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rz_v2_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rz_v3_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rz_v4_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e1m2x22bfloat16_v2_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e1m2x22bfloat16_v3_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e1m2x22bfloat16_v4_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int82int32_v2_impl(vector_int32_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int82int32_v3_impl(vector_int32_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_int82int32_v4_impl(vector_int32_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

// ==========asc_create_iter_reg (deprecated, please use asc_update_addr_reg instead)=========
#define ASC_DEFINE_ITER_REG_DEPRECATED_API(bxx)                                                                      \
    [[deprecated("NOTICE: asc_create_iter_reg_" #bxx " is deprecated. "                                              \
                 "Please use asc_update_addr_reg_" #bxx                                                              \
                 " instead.")]] __simd_callee__ inline addr_reg asc_create_iter_reg_##bxx(uint32_t offset)           \
    {                                                                                                                \
        return asc_update_addr_reg_##bxx(offset);                                                                    \
    }                                                                                                                \
    [[deprecated(                                                                                                    \
        "NOTICE: asc_create_iter_reg_" #bxx " is deprecated. "                                                       \
        "Please use asc_update_addr_reg_" #bxx                                                                       \
        " instead.")]] __simd_callee__ inline addr_reg asc_create_iter_reg_##bxx(uint32_t offset0, uint32_t offset1) \
    {                                                                                                                \
        return asc_update_addr_reg_##bxx(offset0, offset1);                                                          \
    }                                                                                                                \
    [[deprecated("NOTICE: asc_create_iter_reg_" #bxx " is deprecated. "                                              \
                 "Please use asc_update_addr_reg_" #bxx " instead.")]] __simd_callee__ inline addr_reg               \
        asc_create_iter_reg_##bxx(uint32_t offset0, uint32_t offset1, uint32_t offset2)                              \
    {                                                                                                                \
        return asc_update_addr_reg_##bxx(offset0, offset1, offset2);                                                 \
    }                                                                                                                \
    [[deprecated("NOTICE: asc_create_iter_reg_" #bxx " is deprecated. "                                              \
                 "Please use asc_update_addr_reg_" #bxx " instead.")]] __simd_callee__ inline addr_reg               \
        asc_create_iter_reg_##bxx(uint32_t offset0, uint32_t offset1, uint32_t offset2, uint32_t offset3)            \
    {                                                                                                                \
        return asc_update_addr_reg_##bxx(offset0, offset1, offset2, offset3);                                        \
    }

ASC_DEFINE_ITER_REG_DEPRECATED_API(b32)
ASC_DEFINE_ITER_REG_DEPRECATED_API(b16)
ASC_DEFINE_ITER_REG_DEPRECATED_API(b8)

#undef ASC_DEFINE_ITER_REG_DEPRECATED_API

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(
    vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    asc_and_impl(dst, src0, src1, mask);
}

// ==========asc_float2half(rd/ru/rz/rn/rna/ro)==========
[[deprecated("NOTICE: asc_float2half_rd(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rd(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_float2half_rd(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rd_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_rd_sat(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rd_sat(vector_half& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2half_rd_sat(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rd_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_rd_v2 is deprecated. "
             "Currently asc_float2half_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rd_v2(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rd_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2half_rd_sat_v2 is deprecated. "
    "Currently asc_float2half_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rd_sat_v2(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rd_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_ru(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_ru(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_float2half_ru(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_ru_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_ru_sat(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_ru_sat(vector_half& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2half_ru_sat(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_ru_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_ru_v2 is deprecated. "
             "Currently asc_float2half_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_ru_v2(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_ru_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2half_ru_sat_v2 is deprecated. "
    "Currently asc_float2half_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_ru_sat_v2(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_ru_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_rz(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rz(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_float2half_rz(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rz_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_rz_sat(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rz_sat(vector_half& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2half_rz_sat(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rz_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_rz_v2 is deprecated. "
             "Currently asc_float2half_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rz_v2(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rz_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2half_rz_sat_v2 is deprecated. "
    "Currently asc_float2half_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rz_sat_v2(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rz_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_rn(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rn(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_float2half_rn(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rn_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_rn_sat(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rn_sat(vector_half& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2half_rn_sat(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rn_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_rn_v2 is deprecated. "
             "Currently asc_float2half_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rn_v2(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2half_rn_sat_v2 is deprecated. "
    "Currently asc_float2half_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rn_sat_v2(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_rna(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rna(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_float2half_rna(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rna_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_rna_sat(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_rna_sat(vector_half& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2half_rna_sat(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rna_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_rna_v2 is deprecated. "
             "Currently asc_float2half_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rna_v2(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2half_rna_sat_v2 is deprecated. "
    "Currently asc_float2half_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_rna_sat_v2(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_rna_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_ro(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_ro(vector_half& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_float2half_ro(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_ro_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_ro_sat(vector_half& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2half_ro_sat(vector_half& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2half_ro_sat(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_ro_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2half_ro_v2 is deprecated. "
             "Currently asc_float2half_ro_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_ro_v2(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_ro_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2half_ro_sat_v2 is deprecated. "
    "Currently asc_float2half_ro_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2half_ro_sat_v2(vector_half& dst, vector_float src, vector_bool mask)
{
    asc_float2half_ro_sat_v2_impl(dst, src, mask);
}

// ==========asc_uint322int16==========
[[deprecated("NOTICE: asc_uint322int16(vector_int16_t& dst, vector_uint32_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint322int16(vector_int16_t& dst, vector_uint32_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint322int16(vector_int16_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322int16_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint322int16_sat(vector_int16_t& dst, vector_uint32_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint322int16_sat(vector_int16_t& dst, vector_uint32_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint322int16_sat(vector_int16_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322int16_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint322int16_v2 is deprecated. "
             "Currently asc_uint322int16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322int16_v2(vector_int16_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322int16_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint322int16_sat_v2 is deprecated. "
             "Currently asc_uint322int16_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322int16_sat_v2(vector_int16_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322int16_sat_v2_impl(dst, src, mask);
}

// ==========asc_e4m32float==========
[[deprecated("NOTICE: asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask) is deprecated. "
             "Please use asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_e4m32float(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_e4m32float_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e4m32float_v2 is deprecated. "
             "Currently asc_e4m32float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e4m32float_v2(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_e4m32float_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e4m32float_v3 is deprecated. "
             "Currently asc_e4m32float_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e4m32float_v3(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_e4m32float_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e4m32float_v4 is deprecated. "
             "Currently asc_e4m32float_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e4m32float_v4(vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_e4m32float_v4_impl(dst, src, mask);
}

// ==========asc_int162float==========
[[deprecated("NOTICE: asc_int162float(vector_float& dst, vector_int16_t src, vector_bool mask) is deprecated. "
             "Please use asc_int162float(vector_float& dst, vector_int16_t src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_int162float(vector_float& dst, vector_int16_t src, vector_bool mask)
{
    asc_int162float_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int162float_v2 is deprecated. "
             "Currently asc_int162float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int162float_v2(vector_float& dst, vector_int16_t src, vector_bool mask)
{
    asc_int162float_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22int16_v2 is deprecated. "
             "Currently asc_int4x22int16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22int16_v2(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22int16_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22int16_v3 is deprecated. "
             "Currently asc_int4x22int16_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22int16_v3(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22int16_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22int16_v4 is deprecated. "
             "Currently asc_int4x22int16_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22int16_v4(vector_int16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22int16_v4_impl(dst, src, mask);
}

// ==========asc_int322int16==========
[[deprecated("NOTICE: asc_int322int16(vector_int16_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322int16(vector_int16_t& dst, vector_int32_t src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_int322int16(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322int16_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322int16_sat(vector_int16_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322int16_sat(vector_int16_t& dst, vector_int32_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int322int16_sat(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322int16_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322int16_v2 is deprecated. "
             "Currently asc_int322int16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322int16_v2(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322int16_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322int16_sat_v2 is deprecated. "
             "Currently asc_int322int16_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322int16_sat_v2(vector_int16_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322int16_sat_v2_impl(dst, src, mask);
}

// ==========asc_int322uint8==========
[[deprecated("NOTICE: asc_int322uint8(vector_uint8_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322uint8(vector_uint8_t& dst, vector_int32_t src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_int322uint8(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322uint8_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322uint8_sat(vector_uint8_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322uint8_sat(vector_uint8_t& dst, vector_int32_t src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_int322uint8_sat(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322uint8_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322uint8_v2 is deprecated. "
             "Currently asc_int322uint8_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint8_v2(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322uint8_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322uint8_sat_v2 is deprecated. "
             "Currently asc_int322uint8_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint8_sat_v2(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322uint8_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322uint8_v3 is deprecated. "
             "Currently asc_int322uint8_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint8_v3(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322uint8_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322uint8_sat_v3 is deprecated. "
             "Currently asc_int322uint8_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint8_sat_v3(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322uint8_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322uint8_v4 is deprecated. "
             "Currently asc_int322uint8_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint8_v4(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322uint8_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322uint8_sat_v4 is deprecated. "
             "Currently asc_int322uint8_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint8_sat_v4(vector_uint8_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322uint8_sat_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_v2 is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_v2(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    asc_squeeze_v2_impl(dst, src, mask);
}

// ==========asc_squeeze_with_status(uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t/half/float/bfloat16_t/hifloat8_t/fp8_e4m3fn_t/fp8_e5m2_t/fp8_e8m0_t)==========
[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_squeeze_with_status is deprecated. "
             "Please use asc_squeeze_and_storeunalign instead.")]] __simd_callee__ inline void
asc_squeeze_with_status(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    asc_squeeze_with_status_impl(dst, src, mask);
}

// ==========asc_half2int8(rd/ru/rz/rn/rna)==========
[[deprecated("NOTICE: asc_half2int8_rd(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rd(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2int8_rd(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rd_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rd_sat(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rd_sat(vector_int8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2int8_rd_sat(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rd_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rd_v2 is deprecated. "
             "Currently asc_half2int8_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rd_v2(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rd_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rd_sat_v2 is deprecated. "
             "Currently asc_half2int8_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rd_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rd_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_ru(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_ru(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2int8_ru(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_ru_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_ru_sat(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_ru_sat(vector_int8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2int8_ru_sat(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_ru_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_ru_v2 is deprecated. "
             "Currently asc_half2int8_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_ru_v2(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_ru_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_ru_sat_v2 is deprecated. "
             "Currently asc_half2int8_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_ru_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_ru_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rz(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rz(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2int8_rz(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rz_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rz_sat(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rz_sat(vector_int8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2int8_rz_sat(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rz_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rz_v2 is deprecated. "
             "Currently asc_half2int8_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rz_v2(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rz_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rz_sat_v2 is deprecated. "
             "Currently asc_half2int8_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rz_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rz_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rn(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rn(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2int8_rn(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rn_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rn_sat(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rn_sat(vector_int8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2int8_rn_sat(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rn_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rn_v2 is deprecated. "
             "Currently asc_half2int8_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rn_v2(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rn_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rn_sat_v2 is deprecated. "
             "Currently asc_half2int8_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rn_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rna(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rna(vector_int8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2int8_rna(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rna_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rna_sat(vector_int8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int8_rna_sat(vector_int8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2int8_rna_sat(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rna_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int8_rna_v2 is deprecated. "
             "Currently asc_half2int8_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rna_v2(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int8_rna_sat_v2 is deprecated. "
    "Currently asc_half2int8_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int8_rna_sat_v2(vector_int8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int8_rna_sat_v2_impl(dst, src, mask);
}

// ==========asc_half2hif8(rh/rna)==========
[[deprecated("NOTICE: asc_half2hif8_rh(vector_hifloat8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2hif8_rh(vector_hifloat8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2hif8_rh(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rh_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2hif8_rh_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2hif8_rh_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2hif8_rh_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rh_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2hif8_rh_v2 is deprecated. "
             "Currently asc_half2hif8_rh_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2hif8_rh_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rh_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2hif8_rh_sat_v2 is deprecated. "
             "Currently asc_half2hif8_rh_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2hif8_rh_sat_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rh_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2hif8_rna(vector_hifloat8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2hif8_rna(vector_hifloat8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2hif8_rna(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rna_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2hif8_rna_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2hif8_rna_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2hif8_rna_sat(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rna_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2hif8_rna_v2 is deprecated. "
             "Currently asc_half2hif8_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2hif8_rna_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2hif8_rna_sat_v2 is deprecated. "
    "Currently asc_half2hif8_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2hif8_rna_sat_v2(vector_hifloat8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2hif8_rna_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_bfloat162float(vector_float& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
             "Please use asc_bfloat162float(vector_float& dst, vector_bfloat16_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162float(vector_float& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162float_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_bfloat162float_v2 is deprecated. "
             "Currently asc_bfloat162float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162float_v2(vector_float& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162float_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22bfloat16_v2 is deprecated. "
             "Currently asc_int4x22bfloat16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22bfloat16_v2(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22bfloat16_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22bfloat16_v3 is deprecated. "
             "Currently asc_int4x22bfloat16_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22bfloat16_v3(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22bfloat16_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22bfloat16_v4 is deprecated. "
             "Currently asc_int4x22bfloat16_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22bfloat16_v4(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22bfloat16_v4_impl(dst, src, mask);
}

// ==========asc_int162uint32==========
[[deprecated("NOTICE: asc_int162uint32(vector_uint32_t& dst, vector_int16_t src, vector_bool mask) is deprecated. "
             "Please use asc_int162uint32(vector_uint32_t& dst, vector_int16_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int162uint32(vector_uint32_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_int162uint32_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int162uint32_v2 is deprecated. "
             "Currently asc_int162uint32_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int162uint32_v2(vector_uint32_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_int162uint32_v2_impl(dst, src, mask);
}

// ==========asc_uint162uint8==========
[[deprecated("NOTICE: asc_uint162uint8(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint162uint8(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint162uint8(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_uint162uint8_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint162uint8_sat(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint162uint8_sat(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint162uint8_sat(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_uint162uint8_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint162uint8_v2 is deprecated. "
             "Currently asc_uint162uint8_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint162uint8_v2(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_uint162uint8_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint162uint8_sat_v2 is deprecated. "
             "Currently asc_uint162uint8_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint162uint8_sat_v2(vector_uint8_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_uint162uint8_sat_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rn(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rn(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN) "
    "instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rn(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rn_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rn_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rn_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rn_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rn_sat_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rn_v2 is deprecated. "
    "Currently asc_bfloat162int32_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rn_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rn_sat_v2 is deprecated. "
    "Currently asc_bfloat162int32_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rn_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rna(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rna(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rna(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rna_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rna_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rna_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rna_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rna_sat_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rna_v2 is deprecated. "
    "Currently asc_bfloat162int32_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rna_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rna_sat_v2 is deprecated. "
    "Currently asc_bfloat162int32_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rna_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rna_sat_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rd(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rd(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN) "
    "instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rd(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rd_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rd_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rd_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rd_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rd_sat_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rd_v2 is deprecated. "
    "Currently asc_bfloat162int32_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rd_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rd_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rd_sat_v2 is deprecated. "
    "Currently asc_bfloat162int32_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rd_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rd_sat_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_ru(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_ru(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN) "
    "instead.")]] __simd_callee__ inline void
asc_bfloat162int32_ru(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_ru_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_ru_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_ru_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162int32_ru_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_ru_sat_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_ru_v2 is deprecated. "
    "Currently asc_bfloat162int32_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_ru_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_ru_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_ru_sat_v2 is deprecated. "
    "Currently asc_bfloat162int32_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_ru_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_ru_sat_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rz(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rz(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, ASC_POSITION_EVEN) "
    "instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rz(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rz_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rz_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162int32_rz_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_bfloat162int32_rz_sat(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rz_sat_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rz_v2 is deprecated. "
    "Currently asc_bfloat162int32_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rz_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rz_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162int32_rz_sat_v2 is deprecated. "
    "Currently asc_bfloat162int32_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162int32_rz_sat_v2(vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162int32_rz_sat_v2_impl(dst, src, mask);
}

// ==========asc_int642float_rn==========
[[deprecated("NOTICE: asc_int642float_rn(vector_float& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642float_rn(vector_float& dst, vector_int64_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int642float_rn(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642float_rn_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int642float_rn_v2 is deprecated. "
             "Currently asc_int642float_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642float_rn_v2(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642float_rn_v2_impl(dst, src, mask);
}

// ==========asc_int642float_rna==========
[[deprecated("NOTICE: asc_int642float_rna(vector_float& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642float_rna(vector_float& dst, vector_int64_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int642float_rna(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642float_rna_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int642float_rna_v2 is deprecated. "
             "Currently asc_int642float_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642float_rna_v2(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642float_rna_v2_impl(dst, src, mask);
}

// ==========asc_int642float_rd==========
[[deprecated("NOTICE: asc_int642float_rd(vector_float& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642float_rd(vector_float& dst, vector_int64_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int642float_rd(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642float_rd_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int642float_rd_v2 is deprecated. "
             "Currently asc_int642float_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642float_rd_v2(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642float_rd_v2_impl(dst, src, mask);
}

// ==========asc_int642float_ru==========
[[deprecated("NOTICE: asc_int642float_ru(vector_float& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642float_ru(vector_float& dst, vector_int64_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int642float_ru(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642float_ru_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int642float_ru_v2 is deprecated. "
             "Currently asc_int642float_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642float_ru_v2(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642float_ru_v2_impl(dst, src, mask);
}

// ==========asc_int642float_rz==========
[[deprecated("NOTICE: asc_int642float_rz(vector_float& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642float_rz(vector_float& dst, vector_int64_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int642float_rz(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642float_rz_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int642float_rz_v2 is deprecated. "
             "Currently asc_int642float_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642float_rz_v2(vector_float& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642float_rz_v2_impl(dst, src, mask);
}

// ==========asc_int82half==========
[[deprecated("NOTICE: asc_int82half(vector_half& dst, vector_int8_t src, vector_bool mask) is deprecated. "
             "Please use asc_int82half(vector_half& dst, vector_int8_t src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_int82half(vector_half& dst, vector_int8_t src, vector_bool mask)
{
    asc_int82half_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int82half_v2 is deprecated. "
             "Currently asc_int82half_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int82half_v2(vector_half& dst, vector_int8_t src, vector_bool mask)
{
    asc_int82half_v2_impl(dst, src, mask);
}

// ==========asc_int162int32==========
[[deprecated("NOTICE: asc_int162int32(vector_int32_t& dst, vector_int16_t src, vector_bool mask) is deprecated. "
             "Please use asc_int162int32(vector_int32_t& dst, vector_int16_t src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_int162int32(vector_int32_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_int162int32_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int162int32_v2 is deprecated. "
             "Currently asc_int162int32_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int162int32_v2(vector_int32_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_int162int32_v2_impl(dst, src, mask);
}

// ==========asc_uint322uint8==========
[[deprecated("NOTICE: asc_uint322uint8(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint322uint8(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_uint322uint8(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322uint8_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint322uint8_sat(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint322uint8_sat(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_uint322uint8_sat(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322uint8_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint322uint8_v2 is deprecated. "
             "Currently asc_uint322uint8_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint8_v2(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322uint8_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint322uint8_sat_v2 is deprecated. "
             "Currently asc_uint322uint8_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint8_sat_v2(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322uint8_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint322uint8_v3 is deprecated. "
             "Currently asc_uint322uint8_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint8_v3(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322uint8_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint322uint8_sat_v3 is deprecated. "
             "Currently asc_uint322uint8_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint8_sat_v3(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322uint8_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint322uint8_v4 is deprecated. "
             "Currently asc_uint322uint8_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint8_v4(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322uint8_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint322uint8_sat_v4 is deprecated. "
             "Currently asc_uint322uint8_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint8_sat_v4(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322uint8_sat_v4_impl(dst, src, mask);
}

// ==========asc_hif82half==========
[[deprecated("NOTICE: asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask) is deprecated. "
             "Please use asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_hif82half(vector_half& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_hif82half_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_hif82half_v2 is deprecated. "
             "Currently asc_hif82half_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_hif82half_v2(vector_half& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_hif82half_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rd_v2 is deprecated. "
             "Currently asc_half2int4x2_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rd_sat_v2 is deprecated. "
    "Currently asc_half2int4x2_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rd_v3 is deprecated. "
             "Currently asc_half2int4x2_rd_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rd_sat_v3 is deprecated. "
    "Currently asc_half2int4x2_rd_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rd_v4 is deprecated. "
             "Currently asc_half2int4x2_rd_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rd_sat_v4 is deprecated. "
    "Currently asc_half2int4x2_rd_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rd_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rd_sat_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_ru_v2 is deprecated. "
             "Currently asc_half2int4x2_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_ru_sat_v2 is deprecated. "
    "Currently asc_half2int4x2_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_ru_v3 is deprecated. "
             "Currently asc_half2int4x2_ru_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_ru_sat_v3 is deprecated. "
    "Currently asc_half2int4x2_ru_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_ru_v4 is deprecated. "
             "Currently asc_half2int4x2_ru_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_ru_sat_v4 is deprecated. "
    "Currently asc_half2int4x2_ru_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_ru_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_ru_sat_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rz_v2 is deprecated. "
             "Currently asc_half2int4x2_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rz_sat_v2 is deprecated. "
    "Currently asc_half2int4x2_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rz_v3 is deprecated. "
             "Currently asc_half2int4x2_rz_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rz_sat_v3 is deprecated. "
    "Currently asc_half2int4x2_rz_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rz_v4 is deprecated. "
             "Currently asc_half2int4x2_rz_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rz_sat_v4 is deprecated. "
    "Currently asc_half2int4x2_rz_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rz_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rz_sat_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rn_v2 is deprecated. "
             "Currently asc_half2int4x2_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rn_sat_v2 is deprecated. "
    "Currently asc_half2int4x2_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rn_v3 is deprecated. "
             "Currently asc_half2int4x2_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rn_sat_v3 is deprecated. "
    "Currently asc_half2int4x2_rn_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rn_v4 is deprecated. "
             "Currently asc_half2int4x2_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rn_sat_v4 is deprecated. "
    "Currently asc_half2int4x2_rn_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rn_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rn_sat_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rna_v2 is deprecated. "
             "Currently asc_half2int4x2_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rna_sat_v2 is deprecated. "
    "Currently asc_half2int4x2_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rna_v3 is deprecated. "
             "Currently asc_half2int4x2_rna_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rna_sat_v3 is deprecated. "
    "Currently asc_half2int4x2_rna_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int4x2_rna_v4 is deprecated. "
             "Currently asc_half2int4x2_rna_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2int4x2_rna_sat_v4 is deprecated. "
    "Currently asc_half2int4x2_rna_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int4x2_rna_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int4x2_rna_sat_v4_impl(dst, src, mask);
}

// ==========asc_uint82half==========
[[deprecated("NOTICE: asc_uint82half(vector_half& dst, vector_uint8_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint82half(vector_half& dst, vector_uint8_t src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_uint82half(vector_half& dst, vector_uint8_t src, vector_bool mask)
{
    asc_uint82half_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint82half_v2 is deprecated. "
             "Currently asc_uint82half_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint82half_v2(vector_half& dst, vector_uint8_t src, vector_bool mask)
{
    asc_uint82half_v2_impl(dst, src, mask);
}

// ==========asc_uint162uint32==========
[[deprecated("NOTICE: asc_uint162uint32(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint162uint32(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint162uint32(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_uint162uint32_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint162uint32_v2 is deprecated. "
             "Currently asc_uint162uint32_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint162uint32_v2(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_uint162uint32_v2_impl(dst, src, mask);
}

// ==========asc_hif82float==========
[[deprecated("NOTICE: asc_hif82float(vector_float& dst, vector_hifloat8_t src, vector_bool mask) is deprecated. "
             "Please use asc_hif82float(vector_float& dst, vector_hifloat8_t src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_hif82float(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_hif82float_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_hif82float_v2 is deprecated. "
             "Currently asc_hif82float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_hif82float_v2(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_hif82float_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_hif82float_v3 is deprecated. "
             "Currently asc_hif82float_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_hif82float_v3(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_hif82float_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_hif82float_v4 is deprecated. "
             "Currently asc_hif82float_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_hif82float_v4(vector_float& dst, vector_hifloat8_t src, vector_bool mask)
{
    asc_hif82float_v4_impl(dst, src, mask);
}

//=================asc_half2uint8===============
[[deprecated("NOTICE: asc_half2uint8_rd(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rd(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2uint8_rd(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rd_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_rd_sat(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rd_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2uint8_rd_sat(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rd_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_rd_v2 is deprecated. "
             "Currently asc_half2uint8_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rd_v2(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rd_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2uint8_rd_sat_v2 is deprecated. "
    "Currently asc_half2uint8_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rd_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rd_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_rn(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rn(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2uint8_rn(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rn_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_rn_sat(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rn_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2uint8_rn_sat(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rn_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_rn_v2 is deprecated. "
             "Currently asc_half2uint8_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rn_v2(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2uint8_rn_sat_v2 is deprecated. "
    "Currently asc_half2uint8_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rn_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_rna(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rna(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2uint8_rna(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rna_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_rna_sat(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rna_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2uint8_rna_sat(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rna_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_rna_v2 is deprecated. "
             "Currently asc_half2uint8_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rna_v2(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2uint8_rna_sat_v2 is deprecated. "
    "Currently asc_half2uint8_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rna_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rna_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_ru(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_ru(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2uint8_ru(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_ru_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_ru_sat(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_ru_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2uint8_ru_sat(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_ru_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_ru_v2 is deprecated. "
             "Currently asc_half2uint8_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_ru_v2(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_ru_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2uint8_ru_sat_v2 is deprecated. "
    "Currently asc_half2uint8_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_ru_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_ru_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_rz(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rz(vector_uint8_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2uint8_rz(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rz_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_rz_sat(vector_uint8_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2uint8_rz_sat(vector_uint8_t& dst, vector_half src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_half2uint8_rz_sat(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rz_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2uint8_rz_v2 is deprecated. "
             "Currently asc_half2uint8_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rz_v2(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rz_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_half2uint8_rz_sat_v2 is deprecated. "
    "Currently asc_half2uint8_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2uint8_rz_sat_v2(vector_uint8_t& dst, vector_half src, vector_bool mask)
{
    asc_half2uint8_rz_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22half_v2 is deprecated. "
             "Currently asc_int4x22half_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22half_v2(vector_half& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22half_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22half_v3 is deprecated. "
             "Currently asc_int4x22half_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22half_v3(vector_half& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22half_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int4x22half_v4 is deprecated. "
             "Currently asc_int4x22half_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int4x22half_v4(vector_half& dst, vector_int4x2_t src, vector_bool mask)
{
    asc_int4x22half_v4_impl(dst, src, mask);
}

//===============asc_int82int16==============
[[deprecated("NOTICE: asc_int82int16(vector_int16_t& dst, vector_int8_t src, vector_bool mask) is deprecated. "
             "Please use asc_int82int16(vector_int16_t& dst, vector_int8_t src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_int82int16(vector_int16_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_int82int16_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int82int16_v2 is deprecated. "
             "Currently asc_int82int16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int82int16_v2(vector_int16_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_int82int16_v2_impl(dst, src, mask);
}

//===============asc_int162uint8===============
[[deprecated("NOTICE: asc_int162uint8(vector_uint8_t& dst, vector_int16_t src, vector_bool mask) is deprecated. "
             "Please use asc_int162uint8(vector_uint8_t& dst, vector_int16_t src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_int162uint8(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_int162uint8_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int162uint8_sat(vector_uint8_t& dst, vector_int16_t src, vector_bool mask) is deprecated. "
             "Please use asc_int162uint8_sat(vector_uint8_t& dst, vector_int16_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int162uint8_sat(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_int162uint8_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int162uint8_v2 is deprecated. "
             "Currently asc_int162uint8_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int162uint8_v2(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_int162uint8_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int162uint8_sat_v2 is deprecated. "
             "Currently asc_int162uint8_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int162uint8_sat_v2(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_int162uint8_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_or floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_or(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    asc_or_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_or floating-point overloads are deprecated since 9.2.0. Floating-point data types are not "
             "supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_or(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    asc_or_impl(dst, src0, src1, mask);
}

// ==========asc_float2int16_rd/ru/rz/rn/rna==========
[[deprecated("NOTICE: asc_float2int16_rd(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rd(vector_int16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rd(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rd_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_rd_sat(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rd_sat(vector_int16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rd_sat(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rd_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_rd_v2 is deprecated. "
             "Currently asc_float2int16_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rd_v2(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rd_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2int16_rd_sat_v2 is deprecated. "
    "Currently asc_float2int16_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rd_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rd_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_rn(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rn(vector_int16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rn(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rn_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_rn_sat(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rn_sat(vector_int16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rn_sat(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rn_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_rn_v2 is deprecated. "
             "Currently asc_float2int16_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rn_v2(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2int16_rn_sat_v2 is deprecated. "
    "Currently asc_float2int16_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rn_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_rna(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rna(vector_int16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rna(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rna_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_rna_sat(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rna_sat(vector_int16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rna_sat(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rna_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_rna_v2 is deprecated. "
             "Currently asc_float2int16_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rna_v2(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2int16_rna_sat_v2 is deprecated. "
    "Currently asc_float2int16_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rna_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rna_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_ru(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_ru(vector_int16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_ru(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_ru_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_ru_sat(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_ru_sat(vector_int16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_ru_sat(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_ru_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_ru_v2 is deprecated. "
             "Currently asc_float2int16_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_ru_v2(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_ru_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2int16_ru_sat_v2 is deprecated. "
    "Currently asc_float2int16_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_ru_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_ru_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_rz(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rz(vector_int16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rz(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rz_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_rz_sat(vector_int16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int16_rz_sat(vector_int16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int16_rz_sat(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rz_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int16_rz_v2 is deprecated. "
             "Currently asc_float2int16_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rz_v2(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rz_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2int16_rz_sat_v2 is deprecated. "
    "Currently asc_float2int16_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int16_rz_sat_v2(vector_int16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int16_rz_sat_v2_impl(dst, src, mask);
}

// ==========asc_float2int64_rd/ru/rz/rn/rna==========
[[deprecated("NOTICE: asc_float2int64_rd(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rd(vector_int64_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rd(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rd_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_rd_sat(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rd_sat(vector_int64_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rd_sat(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rd_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_rd_v2 is deprecated. "
             "Currently asc_float2int64_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rd_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rd_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2int64_rd_sat_v2 is deprecated. "
    "Currently asc_float2int64_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rd_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rd_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_rn(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rn(vector_int64_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rn(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rn_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_rn_sat(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rn_sat(vector_int64_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rn_sat(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rn_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_rn_v2 is deprecated. "
             "Currently asc_float2int64_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rn_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2int64_rn_sat_v2 is deprecated. "
    "Currently asc_float2int64_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rn_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_rna(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rna(vector_int64_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rna(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rna_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_rna_sat(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rna_sat(vector_int64_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rna_sat(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rna_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_rna_v2 is deprecated. "
             "Currently asc_float2int64_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rna_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2int64_rna_sat_v2 is deprecated. "
    "Currently asc_float2int64_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rna_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rna_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_ru(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_ru(vector_int64_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_ru(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_ru_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_ru_sat(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_ru_sat(vector_int64_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_ru_sat(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_ru_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_ru_v2 is deprecated. "
             "Currently asc_float2int64_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_ru_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_ru_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2int64_ru_sat_v2 is deprecated. "
    "Currently asc_float2int64_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_ru_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_ru_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_rz(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rz(vector_int64_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rz(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rz_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_rz_sat(vector_int64_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2int64_rz_sat(vector_int64_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2int64_rz_sat(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rz_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2int64_rz_v2 is deprecated. "
             "Currently asc_float2int64_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rz_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rz_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2int64_rz_sat_v2 is deprecated. "
    "Currently asc_float2int64_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2int64_rz_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
{
    asc_float2int64_rz_sat_v2_impl(dst, src, mask);
}

// ==========asc_bfloat162e2m1x2_rd/rn/rna/ru/rz==========
[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rd(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_rd(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rd(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rd_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rd_v2 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rd_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rd_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rd_v3 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rd_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rd_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rd_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rd_v4 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rd_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rd_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rd_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rn(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_rn(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rn(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rn_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rn_v2 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rn_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rn_v3 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rn_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rn_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rn_v4 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rn_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rn_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rna(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_rna(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rna(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rna_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rna_v2 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rna_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rna_v3 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rna_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rna_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rna_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rna_v4 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rna_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rna_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rna_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_ru(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_ru(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_ru(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_ru_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_ru_v2 is deprecated. "
    "Currently asc_bfloat162e2m1x2_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_ru_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_ru_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_ru_v3 is deprecated. "
    "Currently asc_bfloat162e2m1x2_ru_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_ru_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_ru_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_ru_v4 is deprecated. "
    "Currently asc_bfloat162e2m1x2_ru_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_ru_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_ru_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rz(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e2m1x2_rz(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rz(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rz_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rz_v2 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rz_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rz_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rz_v3 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rz_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rz_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rz_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e2m1x2_rz_v4 is deprecated. "
    "Currently asc_bfloat162e2m1x2_rz_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e2m1x2_rz_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e2m1x2_rz_v4_impl(dst, src, mask);
}

// ==========asc_float2hif8_rh/rna==========
[[deprecated("NOTICE: asc_float2hif8_rh_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2hif8_rh_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2hif8_rh_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rh(vector_hifloat8_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2hif8_rh(vector_hifloat8_t& dst, vector_float src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2hif8_rh(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rh_sat_v2 is deprecated. "
    "Currently asc_float2hif8_rh_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_sat_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rh_v2 is deprecated. "
             "Currently asc_float2hif8_rh_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rh_sat_v3 is deprecated. "
    "Currently asc_float2hif8_rh_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_sat_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rh_v3 is deprecated. "
             "Currently asc_float2hif8_rh_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rh_sat_v4 is deprecated. "
    "Currently asc_float2hif8_rh_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_sat_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_sat_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rh_v4 is deprecated. "
             "Currently asc_float2hif8_rh_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rh_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rh_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rna_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2hif8_rna_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2hif8_rna_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rna(vector_hifloat8_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2hif8_rna(vector_hifloat8_t& dst, vector_float src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2hif8_rna(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rna_sat_v2 is deprecated. "
    "Currently asc_float2hif8_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_sat_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rna_v2 is deprecated. "
             "Currently asc_float2hif8_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rna_sat_v3 is deprecated. "
    "Currently asc_float2hif8_rna_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_sat_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rna_v3 is deprecated. "
             "Currently asc_float2hif8_rna_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2hif8_rna_sat_v4 is deprecated. "
    "Currently asc_float2hif8_rna_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_sat_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_sat_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2hif8_rna_v4 is deprecated. "
             "Currently asc_float2hif8_rna_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2hif8_rna_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
{
    asc_float2hif8_rna_v4_impl(dst, src, mask);
}

// ==========asc_uint82uint32==========
[[deprecated("NOTICE: asc_uint82uint32(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint82uint32(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_uint82uint32(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_uint82uint32_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint82uint32_v2 is deprecated. "
             "Currently asc_uint82uint32_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint82uint32_v2(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_uint82uint32_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint82uint32_v3 is deprecated. "
             "Currently asc_uint82uint32_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint82uint32_v3(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_uint82uint32_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint82uint32_v4 is deprecated. "
             "Currently asc_uint82uint32_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint82uint32_v4(vector_uint32_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_uint82uint32_v4_impl(dst, src, mask);
}

// ==========asc_uint322uint16==========
[[deprecated(
    "NOTICE: asc_uint322uint16_sat(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask) is deprecated. "
    "Please use asc_uint322uint16_sat(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask, ASC_POSITION_EVEN) "
    "instead.")]] __simd_callee__ inline void
asc_uint322uint16_sat(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322uint16_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint322uint16(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint322uint16(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint322uint16(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322uint16_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_uint322uint16_sat_v2 is deprecated. "
    "Currently asc_uint322uint16_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint16_sat_v2(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322uint16_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint322uint16_v2 is deprecated. "
             "Currently asc_uint322uint16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint322uint16_v2(vector_uint16_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_uint322uint16_v2_impl(dst, src, mask);
}

// ==========asc_e2m1x22bfloat16==========
[[deprecated(
    "NOTICE: asc_e2m1x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask) is deprecated. "
    "Please use asc_e2m1x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_e2m1x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    asc_e2m1x22bfloat16_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e2m1x22bfloat16_v2 is deprecated. "
             "Currently asc_e2m1x22bfloat16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e2m1x22bfloat16_v2(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    asc_e2m1x22bfloat16_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e2m1x22bfloat16_v3 is deprecated. "
             "Currently asc_e2m1x22bfloat16_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e2m1x22bfloat16_v3(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    asc_e2m1x22bfloat16_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e2m1x22bfloat16_v4 is deprecated. "
             "Currently asc_e2m1x22bfloat16_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e2m1x22bfloat16_v4(vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask)
{
    asc_e2m1x22bfloat16_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_muls_v2 is deprecated. "
             "Currently asc_muls_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_muls_v2(vector_half& dst, vector_float src, float value, vector_bool mask)
{
    asc_muls_v2_impl(dst, src, value, mask);
}

// ==========asc_float2bfloat16==========
[[deprecated("NOTICE: asc_float2bfloat16_rd(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2bfloat16_rd(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rd(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rd_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rd_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2bfloat16_rd_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rd_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rd_sat_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rd_v2 is deprecated. "
    "Currently asc_float2bfloat16_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rd_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rd_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rd_sat_v2 is deprecated. "
    "Currently asc_float2bfloat16_rd_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rd_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rd_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2bfloat16_rn(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2bfloat16_rn(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rn(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rn_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rn_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2bfloat16_rn_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rn_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rn_sat_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rn_v2 is deprecated. "
    "Currently asc_float2bfloat16_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rn_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rn_sat_v2 is deprecated. "
    "Currently asc_float2bfloat16_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rn_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rna(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2bfloat16_rna(vector_bfloat16_t& dst, vector_float src, vector_bool mask, ASC_POSITION_EVEN) "
    "instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rna(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rna_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rna_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2bfloat16_rna_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rna_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rna_sat_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rna_v2 is deprecated. "
    "Currently asc_float2bfloat16_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rna_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rna_sat_v2 is deprecated. "
    "Currently asc_float2bfloat16_rna_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rna_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rna_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2bfloat16_ru(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2bfloat16_ru(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_ru(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_ru_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_ru_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2bfloat16_ru_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_ru_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_ru_sat_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_ru_v2 is deprecated. "
    "Currently asc_float2bfloat16_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_ru_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_ru_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_ru_sat_v2 is deprecated. "
    "Currently asc_float2bfloat16_ru_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_ru_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_ru_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2bfloat16_rz(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2bfloat16_rz(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rz(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rz_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rz_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2bfloat16_rz_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask, "
    "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_float2bfloat16_rz_sat(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rz_sat_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rz_v2 is deprecated. "
    "Currently asc_float2bfloat16_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rz_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rz_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2bfloat16_rz_sat_v2 is deprecated. "
    "Currently asc_float2bfloat16_rz_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2bfloat16_rz_sat_v2(vector_bfloat16_t& dst, vector_float src, vector_bool mask)
{
    asc_float2bfloat16_rz_sat_v2_impl(dst, src, mask);
}

// ==========asc_float2e5m2==========
[[deprecated("NOTICE: asc_float2e5m2_rn(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2e5m2_rn(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2e5m2_rn(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e5m2_rn_v2 is deprecated. "
             "Currently asc_float2e5m2_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_v2(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e5m2_rn_v3 is deprecated. "
             "Currently asc_float2e5m2_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_v3(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e5m2_rn_v4 is deprecated. "
             "Currently asc_float2e5m2_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_v4(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e5m2_rn_sat(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2e5m2_rn_sat(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2e5m2_rn_sat(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_sat_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2e5m2_rn_sat_v2 is deprecated. "
    "Currently asc_float2e5m2_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_sat_v2(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2e5m2_rn_sat_v3 is deprecated. "
    "Currently asc_float2e5m2_rn_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_sat_v3(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_sat_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2e5m2_rn_sat_v4 is deprecated. "
    "Currently asc_float2e5m2_rn_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e5m2_rn_sat_v4(vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e5m2_rn_sat_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2float(vector_float& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2float(vector_float& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2float(vector_float& dst, vector_half src, vector_bool mask)
{
    asc_half2float_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2float_v2 is deprecated. "
             "Currently asc_half2float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2float_v2(vector_float& dst, vector_half src, vector_bool mask)
{
    asc_half2float_v2_impl(dst, src, mask);
}

// ==========asc_half2int32==========
[[deprecated("NOTICE: asc_half2int32_rd(vector_int32_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int32_rd(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2int32_rd(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int32_rd_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int32_rd_v2 is deprecated. "
             "Currently asc_half2int32_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int32_rd_v2(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int32_rd_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int32_rn(vector_int32_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int32_rn(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2int32_rn(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int32_rn_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int32_rn_v2 is deprecated. "
             "Currently asc_half2int32_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int32_rn_v2(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int32_rn_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int32_rna(vector_int32_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int32_rna(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2int32_rna(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int32_rna_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int32_rna_v2 is deprecated. "
             "Currently asc_half2int32_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int32_rna_v2(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int32_rna_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int32_ru(vector_int32_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int32_ru(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2int32_ru(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int32_ru_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int32_ru_v2 is deprecated. "
             "Currently asc_half2int32_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int32_ru_v2(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int32_ru_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int32_rz(vector_int32_t& dst, vector_half src, vector_bool mask) is deprecated. "
             "Please use asc_half2int32_rz(vector_int32_t& dst, vector_half src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_half2int32_rz(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int32_rz_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_half2int32_rz_v2 is deprecated. "
             "Currently asc_half2int32_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_half2int32_rz_v2(vector_int32_t& dst, vector_half src, vector_bool mask)
{
    asc_half2int32_rz_v2_impl(dst, src, mask);
}

// ==========asc_e5m22float==========
[[deprecated("NOTICE: asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask) is deprecated. "
             "Please use asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_e5m22float(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_e5m22float_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e5m22float_v2 is deprecated. "
             "Currently asc_e5m22float_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e5m22float_v2(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_e5m22float_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e5m22float_v3 is deprecated. "
             "Currently asc_e5m22float_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e5m22float_v3(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_e5m22float_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e5m22float_v4 is deprecated. "
             "Currently asc_e5m22float_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e5m22float_v4(vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    asc_e5m22float_v4_impl(dst, src, mask);
}

// ==========asc_int322int64==========
[[deprecated("NOTICE: asc_int322int64(vector_int64_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322int64(vector_int64_t& dst, vector_int32_t src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_int322int64(vector_int64_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322int64_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322int64_v2 is deprecated. "
             "Currently asc_int322int64_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322int64_v2(vector_int64_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322int64_v2_impl(dst, src, mask);
}

// ==========asc_int322uint16==========
[[deprecated("NOTICE: asc_int322uint16(vector_uint16_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322uint16(vector_uint16_t& dst, vector_int32_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int322uint16(vector_uint16_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322uint16_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322uint16_sat(vector_uint16_t& dst, vector_int32_t src, vector_bool mask) is deprecated. "
             "Please use asc_int322uint16_sat(vector_uint16_t& dst, vector_int32_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int322uint16_sat(vector_uint16_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322uint16_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322uint16_v2 is deprecated. "
             "Currently asc_int322uint16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint16_v2(vector_uint16_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322uint16_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int322uint16_sat_v2 is deprecated. "
             "Currently asc_int322uint16_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int322uint16_sat_v2(vector_uint16_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_int322uint16_sat_v2_impl(dst, src, mask);
}

// ==========asc_int642int32==========
[[deprecated("NOTICE: asc_int642int32(vector_int32_t& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642int32(vector_int32_t& dst, vector_int64_t src, vector_bool mask, ASC_POSITION_EVEN) "
             "instead.")]] __simd_callee__ inline void
asc_int642int32(vector_int32_t& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642int32_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int642int32_sat(vector_int32_t& dst, vector_int64_t src, vector_bool mask) is deprecated. "
             "Please use asc_int642int32_sat(vector_int32_t& dst, vector_int64_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_int642int32_sat(vector_int32_t& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642int32_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int642int32_v2 is deprecated. "
             "Currently asc_int642int32_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642int32_v2(vector_int32_t& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642int32_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int642int32_sat_v2 is deprecated. "
             "Currently asc_int642int32_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int642int32_sat_v2(vector_int32_t& dst, vector_int64_t src, vector_bool mask)
{
    asc_int642int32_sat_v2_impl(dst, src, mask);
}

// ==========asc_uint82uint16==========
[[deprecated("NOTICE: asc_uint82uint16(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask) is deprecated. "
             "Please use asc_uint82uint16(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask, "
             "ASC_POSITION_EVEN) instead.")]] __simd_callee__ inline void
asc_uint82uint16(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_uint82uint16_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_uint82uint16_v2 is deprecated. "
             "Currently asc_uint82uint16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_uint82uint16_v2(vector_uint16_t& dst, vector_uint8_t src, vector_bool mask)
{
    asc_uint82uint16_v2_impl(dst, src, mask);
}

// ==========asc_pack(bool/uint16_t/int16_t/uint32_t/int32_t)==========
[[deprecated("NOTICE: asc_pack is deprecated.  Please use asc_pack_to_low instead.")]] __simd_callee__ inline void
asc_pack(vector_uint8_t& dst, vector_uint16_t src)
{
    asc_pack_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack is deprecated.  Please use asc_pack_to_low instead.")]] __simd_callee__ inline void
asc_pack(vector_uint8_t& dst, vector_int16_t src)
{
    asc_pack_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack is deprecated.  Please use asc_pack_to_low instead.")]] __simd_callee__ inline void
asc_pack(vector_uint16_t& dst, vector_uint32_t src)
{
    asc_pack_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack is deprecated.  Please use asc_pack_to_low instead.")]] __simd_callee__ inline void
asc_pack(vector_uint16_t& dst, vector_int32_t src)
{
    asc_pack_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack is deprecated.  Please use asc_pack_to_low instead.")]] __simd_callee__ inline void
asc_pack(vector_bool& dst, vector_bool src)
{
    asc_pack_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack_v2 is deprecated.  Please use asc_pack_to_high instead.")]] __simd_callee__ inline void
asc_pack_v2(vector_uint8_t& dst, vector_uint16_t src)
{
    asc_pack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack_v2 is deprecated.  Please use asc_pack_to_high instead.")]] __simd_callee__ inline void
asc_pack_v2(vector_uint8_t& dst, vector_int16_t src)
{
    asc_pack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack_v2 is deprecated.  Please use asc_pack_to_high instead.")]] __simd_callee__ inline void
asc_pack_v2(vector_uint16_t& dst, vector_uint32_t src)
{
    asc_pack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack_v2 is deprecated.  Please use asc_pack_to_high instead.")]] __simd_callee__ inline void
asc_pack_v2(vector_uint16_t& dst, vector_int32_t src)
{
    asc_pack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_pack_v2 is deprecated.  Please use asc_pack_to_high instead.")]] __simd_callee__ inline void
asc_pack_v2(vector_bool& dst, vector_bool src)
{
    asc_pack_v2_impl(dst, src);
}

// ==========asc_exp_sub(half/float)==========
[[deprecated("NOTICE: asc_exp_sub with half inputs and without src_pos is deprecated. "
             "Please use asc_exp_sub_half2float with src_pos instead.")]] __simd_callee__ inline void
asc_exp_sub(vector_float& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    asc_exp_sub_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_exp_sub_v2 with half inputs is deprecated. "
             "Please use asc_exp_sub_half2float with src_pos instead.")]] __simd_callee__ inline void
asc_exp_sub_v2(vector_float& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    asc_exp_sub_v2_impl(dst, src0, src1, mask);
}

[[deprecated("NOTICE: asc_exp_sub_v2 with float inputs is deprecated. "
             "Please use asc_exp_sub instead.")]] __simd_callee__ inline void
asc_exp_sub_v2(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    asc_exp_sub_v2_impl(dst, src0, src1, mask);
}

// ==========asc_float2e4m3_rn=========
[[deprecated(
    "NOTICE: asc_float2e4m3_rn_sat(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask) is deprecated. "
    "Please use asc_float2e4m3_rn_sat(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2e4m3_rn_sat(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_sat_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask) is deprecated. "
             "Please use asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2e4m3_rn_sat_v2 is deprecated. "
    "Currently asc_float2e4m3_rn_sat_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_sat_v2(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_sat_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e4m3_rn_v2 is deprecated. "
             "Currently asc_float2e4m3_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_v2(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2e4m3_rn_sat_v3 is deprecated. "
    "Currently asc_float2e4m3_rn_sat_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_sat_v3(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_sat_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e4m3_rn_v3 is deprecated. "
             "Currently asc_float2e4m3_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_v3(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_float2e4m3_rn_sat_v4 is deprecated. "
    "Currently asc_float2e4m3_rn_sat_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_sat_v4(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_sat_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_float2e4m3_rn_v4 is deprecated. "
             "Currently asc_float2e4m3_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_float2e4m3_rn_v4(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
{
    asc_float2e4m3_rn_v4_impl(dst, src, mask);
}

// ==========asc_bfloat162e1m2x2_rd/rn/rna/ru/rz==========
[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rd(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_rd(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rd(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rd_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rd_v2 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rd_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rd_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rd_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rd_v3 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rd_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rd_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rd_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rd_v4 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rd_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rd_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rd_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rn(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_rn(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rn(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rn_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rn_v2 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rn_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rn_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rn_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rn_v3 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rn_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rn_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rn_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rn_v4 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rn_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rn_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rn_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rna(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_rna(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rna(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rna_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rna_v2 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rna_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rna_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rna_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rna_v3 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rna_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rna_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rna_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rna_v4 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rna_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rna_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rna_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_ru(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_ru(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_ru(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_ru_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_ru_v2 is deprecated. "
    "Currently asc_bfloat162e1m2x2_ru_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_ru_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_ru_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_ru_v3 is deprecated. "
    "Currently asc_bfloat162e1m2x2_ru_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_ru_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_ru_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_ru_v4 is deprecated. "
    "Currently asc_bfloat162e1m2x2_ru_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_ru_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_ru_v4_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rz(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask) is deprecated. "
    "Please use asc_bfloat162e1m2x2_rz(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rz(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rz_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rz_v2 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rz_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rz_v2(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rz_v2_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rz_v3 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rz_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rz_v3(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rz_v3_impl(dst, src, mask);
}

[[deprecated(
    "NOTICE: asc_bfloat162e1m2x2_rz_v4 is deprecated. "
    "Currently asc_bfloat162e1m2x2_rz_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_bfloat162e1m2x2_rz_v4(vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    asc_bfloat162e1m2x2_rz_v4_impl(dst, src, mask);
}

// ==========asc_e1m2x22bfloat16==========
[[deprecated(
    "NOTICE: asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask) is deprecated. "
    "Please use asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask, "
    "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    asc_e1m2x22bfloat16_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e1m2x22bfloat16_v2 is deprecated. "
             "Currently asc_e1m2x22bfloat16_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e1m2x22bfloat16_v2(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    asc_e1m2x22bfloat16_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e1m2x22bfloat16_v3 is deprecated. "
             "Currently asc_e1m2x22bfloat16_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e1m2x22bfloat16_v3(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    asc_e1m2x22bfloat16_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_e1m2x22bfloat16_v4 is deprecated. "
             "Currently asc_e1m2x22bfloat16_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_e1m2x22bfloat16_v4(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
{
    asc_e1m2x22bfloat16_v4_impl(dst, src, mask);
}

// ==========asc_int82int32==========
[[deprecated("NOTICE: asc_int82int32(vector_int32_t& dst, vector_int8_t src, vector_bool mask) is deprecated. "
             "Please use asc_int82int32(vector_int32_t& dst, vector_int8_t src, vector_bool mask, "
             "ASC_DISPERSE_FIRST_QUARTER) instead.")]] __simd_callee__ inline void
asc_int82int32(vector_int32_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_int82int32_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int82int32_v2 is deprecated. "
             "Currently asc_int82int32_v2 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int82int32_v2(vector_int32_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_int82int32_v2_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int82int32_v3 is deprecated. "
             "Currently asc_int82int32_v3 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int82int32_v3(vector_int32_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_int82int32_v3_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_int82int32_v4 is deprecated. "
             "Currently asc_int82int32_v4 is an unsupported API on current device.")]] __simd_callee__ inline void
asc_int82int32_v4(vector_int32_t& dst, vector_int8_t src, vector_bool mask)
{
    asc_int82int32_v4_impl(dst, src, mask);
}

[[deprecated("NOTICE: asc_clear_ar_spr is deprecated. Please use asc_squeeze_and_storeunalign_init instead.")]]
__simd_callee__ inline void asc_clear_ar_spr()
{
    asc_clear_ar_spr_impl();
}

// ==========asc_storealign_pack_v2(float/u32/s32)=========
[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ int32_t* dst_align32b, vector_int32_t src, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ float* dst_align32b, vector_float src, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, mask);
}

// ==========asc_storealign_pack_v2(float/u32/s32)=========
[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ int32_t* dst_align32b, vector_int32_t src, addr_reg offset, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, addr_reg offset, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ float* dst_align32b, vector_float src, addr_reg offset, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, offset, mask);
}

// storealign_pack_v2
[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ int32_t* dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ uint32_t* dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter instead.")]] __simd_callee__ inline void
asc_storealign_pack_v2(__ubuf__ float* dst_align32b, vector_float src, int32_t offset, vector_bool mask)
{
    asc_storealign_pack_v2_impl(dst_align32b, src, offset, mask);
}

// storealign_pack_v2
[[deprecated("NOTICE: asc_storealign_pack_postupdate_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter_postupdate instead.")]] __simd_callee__ inline void
asc_storealign_pack_postupdate_v2(__ubuf__ int32_t*& dst_align32b, vector_int32_t src, int32_t offset, vector_bool mask)
{
    asc_storealign_pack_postupdate_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_postupdate_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter_postupdate instead.")]] __simd_callee__ inline void
asc_storealign_pack_postupdate_v2(
    __ubuf__ uint32_t*& dst_align32b, vector_uint32_t src, int32_t offset, vector_bool mask)
{
    asc_storealign_pack_postupdate_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storealign_pack_postupdate_v2 is deprecated. "
             "Please use asc_storealign_pack_quarter_postupdate instead.")]] __simd_callee__ inline void
asc_storealign_pack_postupdate_v2(__ubuf__ float*& dst_align32b, vector_float src, int32_t offset, vector_bool mask)
{
    asc_storealign_pack_postupdate_v2_impl(dst_align32b, src, offset, mask);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate with a b8 destination and vector_bool source is deprecated. "
             "Please use the b16 or b32 overload instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t*& dst, vector_store_unalign& src0, vector_bool src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

// ==========asc_storeunalign_postupdate(u8/s8/half/u16/s16/float/u32/s32/s64/bf16/e4m3/e5m2/e8m0/e1m2/e2m1)=========
[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ half* dst, vector_store_unalign& src0, vector_half src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ float* dst, vector_store_unalign& src0, vector_float src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_storeunalign_postupdate is deprecated. Please use asc_squeeze_and_storeunalign "
             "instead.")]]
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

// unpack v2
[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

// brc_v2
[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

/// brc_v3(B16/B32)
[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_v2 is deprecated. "
             "Please use asc_loadalign_unpack4 instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v2(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock instead.")]] __simd_callee__ inline void
asc_loadalign_brc_v3(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_unpack_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_unpack4_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_unpack_postupdate_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    asc_loadalign_unpack_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v2 is deprecated. "
             "Please use asc_loadalign_brc_datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v2(vector_float& dst, __ubuf__ float*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v2_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

[[deprecated("NOTICE: asc_loadalign_brc_postupdate_v3 is deprecated. "
             "Please use asc_loadalign_brc_elem2datablock_postupdate instead.")]] __simd_callee__ inline void
asc_loadalign_brc_postupdate_v3(vector_float& dst, __ubuf__ float*& src, int32_t offset)
{
    asc_loadalign_brc_postupdate_v3_impl(dst, src, offset);
}

//===========asc_storeunalign_post(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float/int64)===========
[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int8_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint8_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int4b_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int16_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint16_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ half* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ bfloat16_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int32_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint32_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ float* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_storeunalign_post is deprecated. Please use asc_squeeze_and_storeunalign_finalize instead.")]]
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int64_t* dst, vector_store_unalign src)
{
    asc_storeunalign_post_impl(dst, src);
}

[[deprecated("NOTICE: asc_scatter(vector_int8_t& src, __ubuf__ int8_t* dst, vector_uint16_t index, vector_bool mask) "
             "is deprecated. "
             "Please use asc_scatter(__ubuf__ int8_t* dst, vector_int8_t src, vector_uint16_t index, vector_bool mask) "
             "instead.")]] __simd_callee__ inline void
asc_scatter(vector_int8_t& src, __ubuf__ int8_t* dst, vector_uint16_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_uint8_t& src, __ubuf__ uint8_t* dst, vector_uint16_t index, vector_bool mask) "
             "is deprecated. "
             "Please use asc_scatter(__ubuf__ uint8_t* dst, vector_uint8_t src, vector_uint16_t index, vector_bool "
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_uint8_t& src, __ubuf__ uint8_t* dst, vector_uint16_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_int16_t& src, __ubuf__ int16_t* dst, vector_uint16_t index, vector_bool mask) "
             "is deprecated. "
             "Please use asc_scatter(__ubuf__ int16_t* dst, vector_int16_t src, vector_uint16_t index, vector_bool "
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_int16_t& src, __ubuf__ int16_t* dst, vector_uint16_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_uint16_t& src, __ubuf__ uint16_t* dst, vector_uint16_t index, vector_bool "
             "mask) is deprecated. "
             "Please use asc_scatter(__ubuf__ uint16_t* dst, vector_uint16_t src, vector_uint16_t index, vector_bool "
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_uint16_t& src, __ubuf__ uint16_t* dst, vector_uint16_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_int32_t& src, __ubuf__ int32_t* dst, vector_uint32_t index, vector_bool mask) "
             "is deprecated. "
             "Please use asc_scatter(__ubuf__ int32_t* dst, vector_int32_t src, vector_uint32_t index, vector_bool "
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_int32_t& src, __ubuf__ int32_t* dst, vector_uint32_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_uint32_t& src, __ubuf__ uint32_t* dst, vector_uint32_t index, vector_bool "
             "mask) is deprecated. "
             "Please use asc_scatter(__ubuf__ uint32_t* dst, vector_uint32_t src, vector_uint32_t index, vector_bool "
             "mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_uint32_t& src, __ubuf__ uint32_t* dst, vector_uint32_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_bfloat16_t& src, __ubuf__ bfloat16_t* dst, vector_uint16_t index, vector_bool "
             "mask) is deprecated. "
             "Please use asc_scatter(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, vector_uint16_t index, "
             "vector_bool mask) instead.")]] __simd_callee__ inline void
asc_scatter(vector_bfloat16_t& src, __ubuf__ bfloat16_t* dst, vector_uint16_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated(
    "NOTICE: asc_scatter(vector_half& src, __ubuf__ half* dst, vector_uint16_t index, vector_bool mask) is deprecated. "
    "Please use asc_scatter(__ubuf__ half* dst, vector_half src, vector_uint16_t index, vector_bool mask) "
    "instead.")]] __simd_callee__ inline void
asc_scatter(vector_half& src, __ubuf__ half* dst, vector_uint16_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

[[deprecated("NOTICE: asc_scatter(vector_float& src, __ubuf__ float* dst, vector_uint32_t index, vector_bool mask) is "
             "deprecated. "
             "Please use asc_scatter(__ubuf__ float* dst, vector_float src, vector_uint32_t index, vector_bool mask) "
             "instead.")]] __simd_callee__ inline void
asc_scatter(vector_float& src, __ubuf__ float* dst, vector_uint32_t index, vector_bool mask)
{
    asc_scatter_impl(src, dst, index, mask);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_COMPUTE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_COMPUTE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_REG_COMPUTE_INTF_IMPL_H
