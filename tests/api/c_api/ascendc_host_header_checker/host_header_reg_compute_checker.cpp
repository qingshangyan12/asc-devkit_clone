/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "c_api/reg_compute/reg_convert.h"
#include "c_api/reg_compute/reg_load.h"
#include "c_api/reg_compute/reg_store.h"
#include "c_api/reg_compute/reg_vector.h"

static void test_host_c_api_reg_compute_0()
{
    (void)static_cast<void(*)(vector_int8_t& vector_int8_t vector_bool)>(&asc_abs);
    (void)static_cast<void(*)(vector_half& vector_half vector_half vector_bool)>(&asc_abs_sub);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t vector_uint8_t vector_bool)>(&asc_add);
    (void)static_cast<void(*)(vector_int8_t& vector_int8_t int8_t vector_bool)>(&asc_add_scalar);
    (void)static_cast<void(*)(vector_bool& vector_uint32_t& vector_uint32_t vector_uint32_t vector_bool vector_bool)>(&asc_addc);
    (void)static_cast<void(*)(vector_bool& vector_bool vector_bool vector_bool)>(&asc_and);
    (void)static_cast<void(*)(vector_int8_t& int8_t)>(&asc_arange);
    (void)static_cast<void(*)(vector_int8_t& int8_t)>(&asc_arange_descend);
    (void)static_cast<void(*)(vector_half& vector_half half vector_bool)>(&asc_axpy);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rd);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rd_v2);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rd_v3);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rd_v4);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rn);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rn_v2);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rn_v3);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rn_v4);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rna);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rna_v2);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rna_v3);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rna_v4);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_ru);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_ru_v2);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_ru_v3);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_ru_v4);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rz);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rz_v2);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rz_v3);
    (void)static_cast<void(*)(vector_fp4x2_e1m2_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e1m2x2_rz_v4);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rd);
}

static void test_host_c_api_reg_compute_1()
{
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rd_v2);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rd_v3);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rd_v4);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rn);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rn_v2);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rn_v3);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rn_v4);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rna);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rna_v2);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rna_v3);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rna_v4);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_ru);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_ru_v2);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_ru_v3);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_ru_v4);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rz);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rz_v2);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rz_v3);
    (void)static_cast<void(*)(vector_fp4x2_e2m1_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162e2m1x2_rz_v4);
    (void)static_cast<void(*)(vector_float& vector_bfloat16_t vector_bool)>(&asc_bfloat162float);
    (void)static_cast<void(*)(vector_float& vector_bfloat16_t vector_bool)>(&asc_bfloat162float_v2);
    (void)static_cast<void(*)(vector_half& vector_bfloat16_t vector_bool)>(&asc_bfloat162half_rd);
    (void)static_cast<void(*)(vector_half& vector_bfloat16_t vector_bool)>(&asc_bfloat162half_rd_sat);
    (void)static_cast<void(*)(vector_half& vector_bfloat16_t vector_bool)>(&asc_bfloat162half_rn);
    (void)static_cast<void(*)(vector_half& vector_bfloat16_t vector_bool)>(&asc_bfloat162half_rn_sat);
    (void)static_cast<void(*)(vector_half& vector_bfloat16_t vector_bool)>(&asc_bfloat162half_rna);
    (void)static_cast<void(*)(vector_half& vector_bfloat16_t vector_bool)>(&asc_bfloat162half_rna_sat);
    (void)static_cast<void(*)(vector_half& vector_bfloat16_t vector_bool)>(&asc_bfloat162half_ru);
    (void)static_cast<void(*)(vector_half& vector_bfloat16_t vector_bool)>(&asc_bfloat162half_ru_sat);
    (void)static_cast<void(*)(vector_half& vector_bfloat16_t vector_bool)>(&asc_bfloat162half_rz);
}

static void test_host_c_api_reg_compute_2()
{
    (void)static_cast<void(*)(vector_half& vector_bfloat16_t vector_bool)>(&asc_bfloat162half_rz_sat);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rd);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rd_sat);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rd_sat_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rd_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rn);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rn_sat);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rn_sat_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rn_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rna);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rna_sat);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rna_sat_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rna_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_ru);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_ru_sat);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_ru_sat_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_ru_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rz);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rz_sat);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rz_sat_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_bfloat16_t vector_bool)>(&asc_bfloat162int32_rz_v2);
    (void)static_cast<void(*)(vector_half& vector_half vector_bool)>(&asc_ceil);
    (void)static_cast<void(*)()>(&asc_clear_ar_spr);
    (void)static_cast<void(*)(vector_int8_t& vector_int8_t vector_bool)>(&asc_copy);
    (void)static_cast<addr_reg(*)(uint32_t)>(&asc_create_addr_reg_b16);
    (void)static_cast<addr_reg(*)(uint32_t)>(&asc_create_addr_reg_b32);
    (void)static_cast<addr_reg(*)(uint32_t)>(&asc_create_addr_reg_b8);
    (void)static_cast<addr_reg(*)(uint32_t)>(&asc_create_iter_reg_b16);
    (void)static_cast<addr_reg(*)(uint32_t)>(&asc_create_iter_reg_b32);
    (void)static_cast<addr_reg(*)(uint32_t)>(&asc_create_iter_reg_b8);
}

static void test_host_c_api_reg_compute_3()
{
    (void)static_cast<void(*)(vector_uint16_t& vector_uint8_t vector_bool)>(&asc_cumulative_histogram_bin0);
    (void)static_cast<void(*)(vector_uint16_t& vector_uint8_t vector_bool)>(&asc_cumulative_histogram_bin1);
    (void)static_cast<void(*)(vector_int32_t& vector_int32_t& vector_int32_t vector_int32_t)>(&asc_deintlv);
    (void)static_cast<void(*)(vector_bool& vector_bool& vector_bool vector_bool)>(&asc_deintlv_b16);
    (void)static_cast<void(*)(vector_bool& vector_bool& vector_bool vector_bool)>(&asc_deintlv_b32);
    (void)static_cast<void(*)(vector_bool& vector_bool& vector_bool vector_bool)>(&asc_deintlv_b8);
    (void)static_cast<void(*)(vector_half& vector_half vector_half vector_bool)>(&asc_div);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t vector_bool)>(&asc_duplicate);
    (void)static_cast<void(*)(vector_uint8_t& uint8_t vector_bool)>(&asc_duplicate_scalar);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_fp4x2_e1m2_t vector_bool)>(&asc_e1m2x22bfloat16);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_fp4x2_e1m2_t vector_bool)>(&asc_e1m2x22bfloat16_v2);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_fp4x2_e1m2_t vector_bool)>(&asc_e1m2x22bfloat16_v3);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_fp4x2_e1m2_t vector_bool)>(&asc_e1m2x22bfloat16_v4);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_fp4x2_e2m1_t vector_bool)>(&asc_e2m1x22bfloat16);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_fp4x2_e2m1_t vector_bool)>(&asc_e2m1x22bfloat16_v2);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_fp4x2_e2m1_t vector_bool)>(&asc_e2m1x22bfloat16_v3);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_fp4x2_e2m1_t vector_bool)>(&asc_e2m1x22bfloat16_v4);
    (void)static_cast<void(*)(vector_float& vector_fp8_e4m3fn_t vector_bool)>(&asc_e4m32float);
    (void)static_cast<void(*)(vector_float& vector_fp8_e4m3fn_t vector_bool)>(&asc_e4m32float_v2);
    (void)static_cast<void(*)(vector_float& vector_fp8_e4m3fn_t vector_bool)>(&asc_e4m32float_v3);
    (void)static_cast<void(*)(vector_float& vector_fp8_e4m3fn_t vector_bool)>(&asc_e4m32float_v4);
    (void)static_cast<void(*)(vector_float& vector_fp8_e5m2_t vector_bool)>(&asc_e5m22float);
    (void)static_cast<void(*)(vector_float& vector_fp8_e5m2_t vector_bool)>(&asc_e5m22float_v2);
    (void)static_cast<void(*)(vector_float& vector_fp8_e5m2_t vector_bool)>(&asc_e5m22float_v3);
    (void)static_cast<void(*)(vector_float& vector_fp8_e5m2_t vector_bool)>(&asc_e5m22float_v4);
    (void)static_cast<void(*)(vector_bool& vector_int8_t vector_int8_t vector_bool)>(&asc_eq);
    (void)static_cast<void(*)(vector_bool& vector_int8_t int8_t vector_bool)>(&asc_eq_scalar);
    (void)static_cast<void(*)(vector_half& vector_half vector_bool)>(&asc_exp);
    (void)static_cast<void(*)(vector_float& vector_half vector_half vector_bool)>(&asc_exp_sub);
    (void)static_cast<void(*)(vector_float& vector_half vector_half vector_bool)>(&asc_exp_sub_v2);
}

static void test_host_c_api_reg_compute_4()
{
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rd);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rd_sat);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rd_sat_v2);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rd_v2);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rn);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rn_sat);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rn_sat_v2);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rn_v2);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rna);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rna_sat);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rna_sat_v2);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rna_v2);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_ru);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_ru_sat);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_ru_sat_v2);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_ru_v2);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rz);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rz_sat);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rz_sat_v2);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_float vector_bool)>(&asc_float2bfloat16_rz_v2);
    (void)static_cast<void(*)(vector_fp8_e4m3fn_t& vector_float vector_bool)>(&asc_float2e4m3_rn);
    (void)static_cast<void(*)(vector_fp8_e4m3fn_t& vector_float vector_bool)>(&asc_float2e4m3_rn_sat);
    (void)static_cast<void(*)(vector_fp8_e4m3fn_t& vector_float vector_bool)>(&asc_float2e4m3_rn_sat_v2);
    (void)static_cast<void(*)(vector_fp8_e4m3fn_t& vector_float vector_bool)>(&asc_float2e4m3_rn_sat_v3);
    (void)static_cast<void(*)(vector_fp8_e4m3fn_t& vector_float vector_bool)>(&asc_float2e4m3_rn_sat_v4);
    (void)static_cast<void(*)(vector_fp8_e4m3fn_t& vector_float vector_bool)>(&asc_float2e4m3_rn_v2);
    (void)static_cast<void(*)(vector_fp8_e4m3fn_t& vector_float vector_bool)>(&asc_float2e4m3_rn_v3);
    (void)static_cast<void(*)(vector_fp8_e4m3fn_t& vector_float vector_bool)>(&asc_float2e4m3_rn_v4);
    (void)static_cast<void(*)(vector_fp8_e5m2_t& vector_float vector_bool)>(&asc_float2e5m2_rn);
    (void)static_cast<void(*)(vector_fp8_e5m2_t& vector_float vector_bool)>(&asc_float2e5m2_rn_sat);
}

static void test_host_c_api_reg_compute_5()
{
    (void)static_cast<void(*)(vector_fp8_e5m2_t& vector_float vector_bool)>(&asc_float2e5m2_rn_sat_v2);
    (void)static_cast<void(*)(vector_fp8_e5m2_t& vector_float vector_bool)>(&asc_float2e5m2_rn_sat_v3);
    (void)static_cast<void(*)(vector_fp8_e5m2_t& vector_float vector_bool)>(&asc_float2e5m2_rn_sat_v4);
    (void)static_cast<void(*)(vector_fp8_e5m2_t& vector_float vector_bool)>(&asc_float2e5m2_rn_v2);
    (void)static_cast<void(*)(vector_fp8_e5m2_t& vector_float vector_bool)>(&asc_float2e5m2_rn_v3);
    (void)static_cast<void(*)(vector_fp8_e5m2_t& vector_float vector_bool)>(&asc_float2e5m2_rn_v4);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rd);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rd_sat);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rd_sat_v2);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rd_v2);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rn);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rn_sat);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rn_sat_v2);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rn_v2);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rna);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rna_sat);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rna_sat_v2);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rna_v2);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_ro);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_ro_sat);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_ro_sat_v2);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_ro_v2);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_ru);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_ru_sat);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_ru_sat_v2);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_ru_v2);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rz);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rz_sat);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rz_sat_v2);
    (void)static_cast<void(*)(vector_half& vector_float vector_bool)>(&asc_float2half_rz_v2);
}

static void test_host_c_api_reg_compute_6()
{
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rh);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rh_sat);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rh_sat_v2);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rh_sat_v3);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rh_sat_v4);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rh_v2);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rh_v3);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rh_v4);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rna);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rna_sat);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rna_sat_v2);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rna_sat_v3);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rna_sat_v4);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rna_v2);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rna_v3);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_float vector_bool)>(&asc_float2hif8_rna_v4);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rd);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rd_sat);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rd_sat_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rd_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rn);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rn_sat);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rn_sat_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rn_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rna);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rna_sat);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rna_sat_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rna_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_ru);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_ru_sat);
}

static void test_host_c_api_reg_compute_7()
{
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_ru_sat_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_ru_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rz);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rz_sat);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rz_sat_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_float vector_bool)>(&asc_float2int16_rz_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_float vector_bool)>(&asc_float2int32_rd);
    (void)static_cast<void(*)(vector_int32_t& vector_float vector_bool)>(&asc_float2int32_rd_sat);
    (void)static_cast<void(*)(vector_int32_t& vector_float vector_bool)>(&asc_float2int32_rn);
    (void)static_cast<void(*)(vector_int32_t& vector_float vector_bool)>(&asc_float2int32_rn_sat);
    (void)static_cast<void(*)(vector_int32_t& vector_float vector_bool)>(&asc_float2int32_rna);
    (void)static_cast<void(*)(vector_int32_t& vector_float vector_bool)>(&asc_float2int32_rna_sat);
    (void)static_cast<void(*)(vector_int32_t& vector_float vector_bool)>(&asc_float2int32_ru);
    (void)static_cast<void(*)(vector_int32_t& vector_float vector_bool)>(&asc_float2int32_ru_sat);
    (void)static_cast<void(*)(vector_int32_t& vector_float vector_bool)>(&asc_float2int32_rz);
    (void)static_cast<void(*)(vector_int32_t& vector_float vector_bool)>(&asc_float2int32_rz_sat);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rd);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rd_sat);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rd_sat_v2);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rd_v2);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rn);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rn_sat);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rn_sat_v2);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rn_v2);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rna);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rna_sat);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rna_sat_v2);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rna_v2);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_ru);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_ru_sat);
}

static void test_host_c_api_reg_compute_8()
{
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_ru_sat_v2);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_ru_v2);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rz);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rz_sat);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rz_sat_v2);
    (void)static_cast<void(*)(vector_int64_t& vector_float vector_bool)>(&asc_float2int64_rz_v2);
    (void)static_cast<void(*)(vector_half& vector_half vector_bool)>(&asc_floor);
    (void)static_cast<void(*)(vector_uint16_t& vector_uint8_t vector_bool)>(&asc_frequency_histogram_bin0);
    (void)static_cast<void(*)(vector_uint16_t& vector_uint8_t vector_bool)>(&asc_frequency_histogram_bin1);
    (void)static_cast<void(*)(vector_int16_t& __ubuf__ int8_t* vector_uint16_t vector_bool)>(&asc_gather);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t* vector_uint32_t vector_bool)>(&asc_gather_datablock);
    (void)static_cast<void(*)(vector_bool& vector_int8_t vector_int8_t vector_bool)>(&asc_ge);
    (void)static_cast<void(*)(vector_bool& vector_int8_t int8_t vector_bool)>(&asc_ge_scalar);
    (void)static_cast<vector_bool(*)()>(&asc_get_mask_spr_b16);
    (void)static_cast<vector_bool(*)()>(&asc_get_mask_spr_b32);
    (void)static_cast<void(*)(vector_bool& vector_uint8_t vector_uint8_t vector_bool)>(&asc_gt);
    (void)static_cast<void(*)(vector_bool& vector_uint8_t uint8_t vector_bool)>(&asc_gt_scalar);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_half vector_bool)>(&asc_half2bfloat16_rd);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_half vector_bool)>(&asc_half2bfloat16_rn);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_half vector_bool)>(&asc_half2bfloat16_rna);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_half vector_bool)>(&asc_half2bfloat16_ru);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_half vector_bool)>(&asc_half2bfloat16_rz);
    (void)static_cast<void(*)(vector_float& vector_half vector_bool)>(&asc_half2float);
    (void)static_cast<void(*)(vector_float& vector_half vector_bool)>(&asc_half2float_v2);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_half vector_bool)>(&asc_half2hif8_rh);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_half vector_bool)>(&asc_half2hif8_rh_sat);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_half vector_bool)>(&asc_half2hif8_rh_sat_v2);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_half vector_bool)>(&asc_half2hif8_rh_v2);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_half vector_bool)>(&asc_half2hif8_rna);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_half vector_bool)>(&asc_half2hif8_rna_sat);
}

static void test_host_c_api_reg_compute_9()
{
    (void)static_cast<void(*)(vector_hifloat8_t& vector_half vector_bool)>(&asc_half2hif8_rna_sat_v2);
    (void)static_cast<void(*)(vector_hifloat8_t& vector_half vector_bool)>(&asc_half2hif8_rna_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_half vector_bool)>(&asc_half2int16_rd);
    (void)static_cast<void(*)(vector_int16_t& vector_half vector_bool)>(&asc_half2int16_rd_sat);
    (void)static_cast<void(*)(vector_int16_t& vector_half vector_bool)>(&asc_half2int16_rn);
    (void)static_cast<void(*)(vector_int16_t& vector_half vector_bool)>(&asc_half2int16_rn_sat);
    (void)static_cast<void(*)(vector_int16_t& vector_half vector_bool)>(&asc_half2int16_rna);
    (void)static_cast<void(*)(vector_int16_t& vector_half vector_bool)>(&asc_half2int16_rna_sat);
    (void)static_cast<void(*)(vector_int16_t& vector_half vector_bool)>(&asc_half2int16_ru);
    (void)static_cast<void(*)(vector_int16_t& vector_half vector_bool)>(&asc_half2int16_ru_sat);
    (void)static_cast<void(*)(vector_int16_t& vector_half vector_bool)>(&asc_half2int16_rz);
    (void)static_cast<void(*)(vector_int16_t& vector_half vector_bool)>(&asc_half2int16_rz_sat);
    (void)static_cast<void(*)(vector_int32_t& vector_half vector_bool)>(&asc_half2int32_rd);
    (void)static_cast<void(*)(vector_int32_t& vector_half vector_bool)>(&asc_half2int32_rd_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_half vector_bool)>(&asc_half2int32_rn);
    (void)static_cast<void(*)(vector_int32_t& vector_half vector_bool)>(&asc_half2int32_rn_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_half vector_bool)>(&asc_half2int32_rna);
    (void)static_cast<void(*)(vector_int32_t& vector_half vector_bool)>(&asc_half2int32_rna_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_half vector_bool)>(&asc_half2int32_ru);
    (void)static_cast<void(*)(vector_int32_t& vector_half vector_bool)>(&asc_half2int32_ru_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_half vector_bool)>(&asc_half2int32_rz);
    (void)static_cast<void(*)(vector_int32_t& vector_half vector_bool)>(&asc_half2int32_rz_v2);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rd);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rd_sat);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rd_sat_v2);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rd_sat_v3);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rd_sat_v4);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rd_v2);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rd_v3);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rd_v4);
}

static void test_host_c_api_reg_compute_10()
{
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rn);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rn_sat);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rn_sat_v2);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rn_sat_v3);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rn_sat_v4);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rn_v2);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rn_v3);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rn_v4);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rna);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rna_sat);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rna_sat_v2);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rna_sat_v3);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rna_sat_v4);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rna_v2);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rna_v3);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rna_v4);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_ru);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_ru_sat);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_ru_sat_v2);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_ru_sat_v3);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_ru_sat_v4);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_ru_v2);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_ru_v3);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_ru_v4);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rz);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rz_sat);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rz_sat_v2);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rz_sat_v3);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rz_sat_v4);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rz_v2);
}

static void test_host_c_api_reg_compute_11()
{
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rz_v3);
    (void)static_cast<void(*)(vector_int4x2_t& vector_half vector_bool)>(&asc_half2int4x2_rz_v4);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rd);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rd_sat);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rd_sat_v2);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rd_v2);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rn);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rn_sat);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rn_sat_v2);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rn_v2);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rna);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rna_sat);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rna_sat_v2);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rna_v2);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_ru);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_ru_sat);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_ru_sat_v2);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_ru_v2);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rz);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rz_sat);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rz_sat_v2);
    (void)static_cast<void(*)(vector_int8_t& vector_half vector_bool)>(&asc_half2int8_rz_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rd);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rd_sat);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rd_sat_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rd_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rn);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rn_sat);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rn_sat_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rn_v2);
}

static void test_host_c_api_reg_compute_12()
{
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rna);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rna_sat);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rna_sat_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rna_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_ru);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_ru_sat);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_ru_sat_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_ru_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rz);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rz_sat);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rz_sat_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_half vector_bool)>(&asc_half2uint8_rz_v2);
    (void)static_cast<void(*)(vector_float& vector_hifloat8_t vector_bool)>(&asc_hif82float);
    (void)static_cast<void(*)(vector_float& vector_hifloat8_t vector_bool)>(&asc_hif82float_v2);
    (void)static_cast<void(*)(vector_float& vector_hifloat8_t vector_bool)>(&asc_hif82float_v3);
    (void)static_cast<void(*)(vector_float& vector_hifloat8_t vector_bool)>(&asc_hif82float_v4);
    (void)static_cast<void(*)(vector_half& vector_hifloat8_t vector_bool)>(&asc_hif82half);
    (void)static_cast<void(*)(vector_half& vector_hifloat8_t vector_bool)>(&asc_hif82half_v2);
    (void)static_cast<void(*)(vector_float& vector_int16_t vector_bool)>(&asc_int162float);
    (void)static_cast<void(*)(vector_float& vector_int16_t vector_bool)>(&asc_int162float_v2);
    (void)static_cast<void(*)(vector_half& vector_int16_t vector_bool)>(&asc_int162half_rd);
    (void)static_cast<void(*)(vector_half& vector_int16_t vector_bool)>(&asc_int162half_rn);
    (void)static_cast<void(*)(vector_half& vector_int16_t vector_bool)>(&asc_int162half_rna);
    (void)static_cast<void(*)(vector_half& vector_int16_t vector_bool)>(&asc_int162half_ru);
    (void)static_cast<void(*)(vector_half& vector_int16_t vector_bool)>(&asc_int162half_rz);
    (void)static_cast<void(*)(vector_int32_t& vector_int16_t vector_bool)>(&asc_int162int32);
    (void)static_cast<void(*)(vector_int32_t& vector_int16_t vector_bool)>(&asc_int162int32_v2);
    (void)static_cast<void(*)(vector_uint32_t& vector_int16_t vector_bool)>(&asc_int162uint32);
    (void)static_cast<void(*)(vector_uint32_t& vector_int16_t vector_bool)>(&asc_int162uint32_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_int16_t vector_bool)>(&asc_int162uint8);
}

static void test_host_c_api_reg_compute_13()
{
    (void)static_cast<void(*)(vector_uint8_t& vector_int16_t vector_bool)>(&asc_int162uint8_sat);
    (void)static_cast<void(*)(vector_uint8_t& vector_int16_t vector_bool)>(&asc_int162uint8_sat_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_int16_t vector_bool)>(&asc_int162uint8_v2);
    (void)static_cast<void(*)(vector_float& vector_int32_t vector_bool)>(&asc_int322float_rd);
    (void)static_cast<void(*)(vector_float& vector_int32_t vector_bool)>(&asc_int322float_rn);
    (void)static_cast<void(*)(vector_float& vector_int32_t vector_bool)>(&asc_int322float_rna);
    (void)static_cast<void(*)(vector_float& vector_int32_t vector_bool)>(&asc_int322float_ru);
    (void)static_cast<void(*)(vector_float& vector_int32_t vector_bool)>(&asc_int322float_rz);
    (void)static_cast<void(*)(vector_int16_t& vector_int32_t vector_bool)>(&asc_int322int16);
    (void)static_cast<void(*)(vector_int16_t& vector_int32_t vector_bool)>(&asc_int322int16_sat);
    (void)static_cast<void(*)(vector_int16_t& vector_int32_t vector_bool)>(&asc_int322int16_sat_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_int32_t vector_bool)>(&asc_int322int16_v2);
    (void)static_cast<void(*)(vector_int64_t& vector_int32_t vector_bool)>(&asc_int322int64);
    (void)static_cast<void(*)(vector_int64_t& vector_int32_t vector_bool)>(&asc_int322int64_v2);
    (void)static_cast<void(*)(vector_uint16_t& vector_int32_t vector_bool)>(&asc_int322uint16);
    (void)static_cast<void(*)(vector_uint16_t& vector_int32_t vector_bool)>(&asc_int322uint16_sat);
    (void)static_cast<void(*)(vector_uint16_t& vector_int32_t vector_bool)>(&asc_int322uint16_sat_v2);
    (void)static_cast<void(*)(vector_uint16_t& vector_int32_t vector_bool)>(&asc_int322uint16_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_int32_t vector_bool)>(&asc_int322uint8);
    (void)static_cast<void(*)(vector_uint8_t& vector_int32_t vector_bool)>(&asc_int322uint8_sat);
    (void)static_cast<void(*)(vector_uint8_t& vector_int32_t vector_bool)>(&asc_int322uint8_sat_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_int32_t vector_bool)>(&asc_int322uint8_sat_v3);
    (void)static_cast<void(*)(vector_uint8_t& vector_int32_t vector_bool)>(&asc_int322uint8_sat_v4);
    (void)static_cast<void(*)(vector_uint8_t& vector_int32_t vector_bool)>(&asc_int322uint8_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_int32_t vector_bool)>(&asc_int322uint8_v3);
    (void)static_cast<void(*)(vector_uint8_t& vector_int32_t vector_bool)>(&asc_int322uint8_v4);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_int4x2_t vector_bool)>(&asc_int4x22bfloat16);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_int4x2_t vector_bool)>(&asc_int4x22bfloat16_v2);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_int4x2_t vector_bool)>(&asc_int4x22bfloat16_v3);
    (void)static_cast<void(*)(vector_bfloat16_t& vector_int4x2_t vector_bool)>(&asc_int4x22bfloat16_v4);
}

static void test_host_c_api_reg_compute_14()
{
    (void)static_cast<void(*)(vector_half& vector_int4x2_t vector_bool)>(&asc_int4x22half);
    (void)static_cast<void(*)(vector_half& vector_int4x2_t vector_bool)>(&asc_int4x22half_v2);
    (void)static_cast<void(*)(vector_half& vector_int4x2_t vector_bool)>(&asc_int4x22half_v3);
    (void)static_cast<void(*)(vector_half& vector_int4x2_t vector_bool)>(&asc_int4x22half_v4);
    (void)static_cast<void(*)(vector_int16_t& vector_int4x2_t vector_bool)>(&asc_int4x22int16);
    (void)static_cast<void(*)(vector_int16_t& vector_int4x2_t vector_bool)>(&asc_int4x22int16_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_int4x2_t vector_bool)>(&asc_int4x22int16_v3);
    (void)static_cast<void(*)(vector_int16_t& vector_int4x2_t vector_bool)>(&asc_int4x22int16_v4);
    (void)static_cast<void(*)(vector_float& vector_int64_t vector_bool)>(&asc_int642float_rd);
    (void)static_cast<void(*)(vector_float& vector_int64_t vector_bool)>(&asc_int642float_rd_v2);
    (void)static_cast<void(*)(vector_float& vector_int64_t vector_bool)>(&asc_int642float_rn);
    (void)static_cast<void(*)(vector_float& vector_int64_t vector_bool)>(&asc_int642float_rn_v2);
    (void)static_cast<void(*)(vector_float& vector_int64_t vector_bool)>(&asc_int642float_rna);
    (void)static_cast<void(*)(vector_float& vector_int64_t vector_bool)>(&asc_int642float_rna_v2);
    (void)static_cast<void(*)(vector_float& vector_int64_t vector_bool)>(&asc_int642float_ru);
    (void)static_cast<void(*)(vector_float& vector_int64_t vector_bool)>(&asc_int642float_ru_v2);
    (void)static_cast<void(*)(vector_float& vector_int64_t vector_bool)>(&asc_int642float_rz);
    (void)static_cast<void(*)(vector_float& vector_int64_t vector_bool)>(&asc_int642float_rz_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_int64_t vector_bool)>(&asc_int642int32);
    (void)static_cast<void(*)(vector_int32_t& vector_int64_t vector_bool)>(&asc_int642int32_sat);
    (void)static_cast<void(*)(vector_int32_t& vector_int64_t vector_bool)>(&asc_int642int32_sat_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_int64_t vector_bool)>(&asc_int642int32_v2);
    (void)static_cast<void(*)(vector_half& vector_int8_t vector_bool)>(&asc_int82half);
    (void)static_cast<void(*)(vector_half& vector_int8_t vector_bool)>(&asc_int82half_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_int8_t vector_bool)>(&asc_int82int16);
    (void)static_cast<void(*)(vector_int16_t& vector_int8_t vector_bool)>(&asc_int82int16_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_int8_t vector_bool)>(&asc_int82int32);
    (void)static_cast<void(*)(vector_int32_t& vector_int8_t vector_bool)>(&asc_int82int32_v2);
    (void)static_cast<void(*)(vector_int32_t& vector_int8_t vector_bool)>(&asc_int82int32_v3);
    (void)static_cast<void(*)(vector_int32_t& vector_int8_t vector_bool)>(&asc_int82int32_v4);
}

static void test_host_c_api_reg_compute_15()
{
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t& vector_uint8_t vector_uint8_t)>(&asc_intlv);
    (void)static_cast<void(*)(vector_bool& vector_bool& vector_bool vector_bool)>(&asc_intlv_b16);
    (void)static_cast<void(*)(vector_bool& vector_bool& vector_bool vector_bool)>(&asc_intlv_b32);
    (void)static_cast<void(*)(vector_bool& vector_bool& vector_bool vector_bool)>(&asc_intlv_b8);
    (void)static_cast<void(*)(vector_bool& vector_uint8_t vector_uint8_t vector_bool)>(&asc_le);
    (void)static_cast<void(*)(vector_bool& vector_uint8_t uint8_t vector_bool)>(&asc_le_scalar);
    (void)static_cast<void(*)(vector_float& vector_float float vector_bool)>(&asc_leakyrelu);
    (void)static_cast<void(*)(vector_half& vector_half vector_bool)>(&asc_ln);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*)>(&asc_load);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*)>(&asc_loadalign);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*)>(&asc_loadalign_brc);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*)>(&asc_loadalign_brc_datablock);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*& int32_t)>(&asc_loadalign_brc_datablock_postupdate);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*)>(&asc_loadalign_brc_elem);
    (void)static_cast<void(*)(vector_int16_t& __ubuf__ int16_t*)>(&asc_loadalign_brc_elem2datablock);
    (void)static_cast<void(*)(vector_int16_t& __ubuf__ int16_t*& int32_t)>(&asc_loadalign_brc_elem2datablock_postupdate);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*& int32_t)>(&asc_loadalign_brc_elem_postupdate);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*& int32_t)>(&asc_loadalign_brc_postupdate);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*& int32_t)>(&asc_loadalign_brc_postupdate_v2);
    (void)static_cast<void(*)(vector_int16_t& __ubuf__ int16_t*& int32_t)>(&asc_loadalign_brc_postupdate_v3);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*)>(&asc_loadalign_brc_v2);
    (void)static_cast<void(*)(vector_int16_t& __ubuf__ int16_t*)>(&asc_loadalign_brc_v3);
    (void)static_cast<void(*)(vector_int8_t& vector_int8_t& __ubuf__ int8_t*)>(&asc_loadalign_deintlv);
    (void)static_cast<void(*)(vector_int8_t& vector_int8_t& __ubuf__ int8_t*& int32_t)>(&asc_loadalign_deintlv_postupdate);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*)>(&asc_loadalign_downsample);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*& int32_t)>(&asc_loadalign_downsample_postupdate);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*& int32_t)>(&asc_loadalign_postupdate);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*)>(&asc_loadalign_unpack);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*)>(&asc_loadalign_unpack4);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*& int32_t)>(&asc_loadalign_unpack4_postupdate);
}

static void test_host_c_api_reg_compute_16()
{
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*& int32_t)>(&asc_loadalign_unpack_postupdate);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*& int32_t)>(&asc_loadalign_unpack_postupdate_v2);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*)>(&asc_loadalign_unpack_v2);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*)>(&asc_loadalign_upsample);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t*& int32_t)>(&asc_loadalign_upsample_postupdate);
    (void)static_cast<void(*)(vector_int8_t& vector_load_unalign& __ubuf__ int8_t *src1)>(&asc_loadunalign);
    (void)static_cast<void(*)(vector_int8_t& vector_load_unalign& __ubuf__ int8_t*& uint32_t)>(&asc_loadunalign_postupdate);
    (void)static_cast<void(*)(vector_load_unalign& __ubuf__ int8_t*)>(&asc_loadunalign_pre);
    (void)static_cast<void(*)(vector_bool& vector_uint8_t vector_uint8_t vector_bool)>(&asc_lt);
    (void)static_cast<void(*)(vector_bool& vector_uint8_t uint8_t vector_bool)>(&asc_lt_scalar);
    (void)static_cast<void(*)(vector_half& vector_half vector_half vector_bool)>(&asc_madd);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t vector_uint8_t vector_bool)>(&asc_max);
    (void)static_cast<void(*)(vector_int8_t& vector_int8_t int8_t vector_bool)>(&asc_max_scalar);
    (void)static_cast<void(*)(vector_int8_t& vector_int8_t vector_int8_t vector_bool)>(&asc_min);
    (void)static_cast<void(*)(vector_int32_t& vector_int32_t int32_t vector_bool)>(&asc_min_scalar);
    (void)static_cast<void(*)(vector_int16_t& vector_int16_t vector_int16_t vector_bool)>(&asc_mul);
    (void)static_cast<void(*)(vector_int16_t& vector_int16_t int16_t vector_bool)>(&asc_mul_scalar);
    (void)static_cast<void(*)(vector_uint32_t& vector_uint32_t& vector_uint32_t vector_uint32_t vector_bool)>(&asc_mull);
    (void)static_cast<void(*)(vector_half& vector_float float vector_bool)>(&asc_muls);
    (void)static_cast<void(*)(vector_half& vector_float float vector_bool)>(&asc_muls_v2);
    (void)static_cast<void(*)(vector_bool& vector_uint8_t vector_uint8_t vector_bool)>(&asc_ne);
    (void)static_cast<void(*)(vector_bool& vector_uint8_t uint8_t vector_bool)>(&asc_ne_scalar);
    (void)static_cast<void(*)(vector_int8_t& vector_int8_t vector_bool)>(&asc_neg);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t vector_bool)>(&asc_not);
    (void)static_cast<void(*)(vector_int8_t& vector_int8_t vector_int8_t vector_bool)>(&asc_or);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint16_t)>(&asc_pack);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint16_t)>(&asc_pack_v2);
    (void)static_cast<void(*)(vector_half& vector_half vector_bool)>(&asc_pair_reduce_sum);
    (void)static_cast<void(*)(vector_float& vector_float vector_float vector_bool)>(&asc_prelu);
    (void)static_cast<void(*)(vector_int16_t& vector_int16_t vector_bool)>(&asc_reduce_max);
}

static void test_host_c_api_reg_compute_17()
{
    (void)static_cast<void(*)(vector_half& vector_half vector_bool)>(&asc_reduce_max_datablock);
    (void)static_cast<void(*)(vector_int16_t& vector_int16_t vector_bool)>(&asc_reduce_min);
    (void)static_cast<void(*)(vector_int16_t& vector_int16_t vector_bool)>(&asc_reduce_min_datablock);
    (void)static_cast<void(*)(vector_int32_t& vector_int16_t vector_bool)>(&asc_reduce_sum);
    (void)static_cast<void(*)(vector_half& vector_half vector_bool)>(&asc_reduce_sum_datablock);
    (void)static_cast<void(*)(vector_half& vector_half vector_bool)>(&asc_relu);
    (void)static_cast<void(*)(vector_half& vector_half vector_bool)>(&asc_rint);
    (void)static_cast<void(*)(vector_half& vector_half vector_bool)>(&asc_round);
    (void)static_cast<void(*)(vector_int8_t& __ubuf__ int8_t* vector_uint16_t vector_bool)>(&asc_scatter);
    (void)static_cast<void(*)(vector_bool& vector_bool vector_bool vector_bool)>(&asc_select);
    (void)static_cast<void(*)(ub_addr8_t __ubuf__ int8_t**)>(&asc_set_va_reg);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t vector_int8_t vector_bool)>(&asc_shiftleft);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t int16_t vector_bool)>(&asc_shiftleft_scalar);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t vector_int8_t vector_bool)>(&asc_shiftright);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t int16_t vector_bool)>(&asc_shiftright_scalar);
    (void)static_cast<void(*)(vector_half& vector_half vector_bool)>(&asc_sqrt);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t vector_bool)>(&asc_squeeze);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t vector_bool)>(&asc_squeeze_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t vector_bool)>(&asc_squeeze_with_status);
    (void)static_cast<void(*)(__ubuf__ int8_t* vector_int8_t)>(&asc_store);
    (void)static_cast<void(*)(__ubuf__ int8_t* vector_int8_t vector_bool)>(&asc_storealign);
    (void)static_cast<void(*)(__ubuf__ int8_t* vector_int8_t)>(&asc_storealign_1st);
    (void)static_cast<void(*)(__ubuf__ int8_t*& vector_int8_t int32_t)>(&asc_storealign_1st_postupdate);
    (void)static_cast<void(*)(__ubuf__ int8_t* vector_int8_t vector_int8_t)>(&asc_storealign_intlv);
    (void)static_cast<void(*)(__ubuf__ int16_t* vector_int16_t vector_bool)>(&asc_storealign_pack);
    (void)static_cast<void(*)(__ubuf__ int16_t*& vector_int16_t int32_t vector_bool)>(&asc_storealign_pack_postupdate);
    (void)static_cast<void(*)(__ubuf__ int32_t*& vector_int32_t int32_t vector_bool)>(&asc_storealign_pack_postupdate_v2);
    (void)static_cast<void(*)(__ubuf__ int32_t* vector_int32_t vector_bool)>(&asc_storealign_pack_quarter);
    (void)static_cast<void(*)(__ubuf__ int32_t*& vector_int32_t int32_t vector_bool)>(&asc_storealign_pack_quarter_postupdate);
    (void)static_cast<void(*)(__ubuf__ int32_t* vector_int32_t vector_bool)>(&asc_storealign_pack_v2);
}

static void test_host_c_api_reg_compute_18()
{
    (void)static_cast<void(*)(__ubuf__ int8_t*& vector_int8_t int32_t vector_bool)>(&asc_storealign_postupdate);
    (void)static_cast<void(*)(__ubuf__ int8_t* vector_store_unalign& vector_int8_t uint32_t)>(&asc_storeunalign);
    (void)static_cast<void(*)(__ubuf__ int8_t* vector_store_unalign int32_t)>(&asc_storeunalign_post);
    (void)static_cast<void(*)(__ubuf__ int8_t*& vector_store_unalign int32_t)>(&asc_storeunalign_post_postupdate);
    (void)static_cast<void(*)(__ubuf__ int8_t*& vector_store_unalign& vector_int8_t uint32_t)>(&asc_storeunalign_postupdate);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint8_t vector_uint8_t vector_bool)>(&asc_sub);
    (void)static_cast<void(*)(vector_bool& vector_uint32_t& vector_uint32_t vector_uint32_t vector_bool vector_bool)>(&asc_subc);
    (void)static_cast<void(*)(vector_half& vector_half vector_bool)>(&asc_trunc);
    (void)static_cast<void(*)(vector_uint32_t& vector_uint16_t vector_bool)>(&asc_uint162uint32);
    (void)static_cast<void(*)(vector_uint32_t& vector_uint16_t vector_bool)>(&asc_uint162uint32_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint16_t vector_bool)>(&asc_uint162uint8);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint16_t vector_bool)>(&asc_uint162uint8_sat);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint16_t vector_bool)>(&asc_uint162uint8_sat_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint16_t vector_bool)>(&asc_uint162uint8_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_uint32_t vector_bool)>(&asc_uint322int16);
    (void)static_cast<void(*)(vector_int16_t& vector_uint32_t vector_bool)>(&asc_uint322int16_sat);
    (void)static_cast<void(*)(vector_int16_t& vector_uint32_t vector_bool)>(&asc_uint322int16_sat_v2);
    (void)static_cast<void(*)(vector_int16_t& vector_uint32_t vector_bool)>(&asc_uint322int16_v2);
    (void)static_cast<void(*)(vector_uint16_t& vector_uint32_t vector_bool)>(&asc_uint322uint16);
    (void)static_cast<void(*)(vector_uint16_t& vector_uint32_t vector_bool)>(&asc_uint322uint16_sat);
    (void)static_cast<void(*)(vector_uint16_t& vector_uint32_t vector_bool)>(&asc_uint322uint16_sat_v2);
    (void)static_cast<void(*)(vector_uint16_t& vector_uint32_t vector_bool)>(&asc_uint322uint16_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint32_t vector_bool)>(&asc_uint322uint8);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint32_t vector_bool)>(&asc_uint322uint8_sat);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint32_t vector_bool)>(&asc_uint322uint8_sat_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint32_t vector_bool)>(&asc_uint322uint8_sat_v3);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint32_t vector_bool)>(&asc_uint322uint8_sat_v4);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint32_t vector_bool)>(&asc_uint322uint8_v2);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint32_t vector_bool)>(&asc_uint322uint8_v3);
    (void)static_cast<void(*)(vector_uint8_t& vector_uint32_t vector_bool)>(&asc_uint322uint8_v4);
}

static void test_host_c_api_reg_compute_19()
{
    (void)static_cast<void(*)(vector_half& vector_uint8_t vector_bool)>(&asc_uint82half);
    (void)static_cast<void(*)(vector_half& vector_uint8_t vector_bool)>(&asc_uint82half_v2);
    (void)static_cast<void(*)(vector_uint16_t& vector_uint8_t vector_bool)>(&asc_uint82uint16);
    (void)static_cast<void(*)(vector_uint16_t& vector_uint8_t vector_bool)>(&asc_uint82uint16_v2);
    (void)static_cast<void(*)(vector_uint32_t& vector_uint8_t vector_bool)>(&asc_uint82uint32);
    (void)static_cast<void(*)(vector_uint32_t& vector_uint8_t vector_bool)>(&asc_uint82uint32_v2);
    (void)static_cast<void(*)(vector_uint32_t& vector_uint8_t vector_bool)>(&asc_uint82uint32_v3);
    (void)static_cast<void(*)(vector_uint32_t& vector_uint8_t vector_bool)>(&asc_uint82uint32_v4);
    (void)static_cast<void(*)(vector_uint16_t& vector_uint8_t)>(&asc_unpack_lower);
    (void)static_cast<void(*)(vector_uint16_t& vector_uint8_t)>(&asc_unpack_upper);
    (void)static_cast<void(*)(vector_uint8_t& vector_bool)>(&asc_unsqueeze);
    (void)static_cast<vector_bool(*)(uint32_t&)>(&asc_update_mask_b16);
    (void)static_cast<vector_bool(*)(uint32_t&)>(&asc_update_mask_b32);
    (void)static_cast<vector_bool(*)(uint32_t&)>(&asc_update_mask_b8);
    (void)static_cast<void(*)(vector_int32_t& vector_int32_t vector_int32_t vector_bool)>(&asc_xor);
}

