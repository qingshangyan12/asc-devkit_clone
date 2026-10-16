/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "tests/api/c_api/npu_arch_9201/utils/test_reg_convert_instr_utils.h"

// ========== 9201 unique merging convert APIs ==========

TEST_9201_REG_CONVERT_POS(
    Mergingbfloat162e1m2x2rdmerge, asc_bfloat162e1m2x2_rd_merge, vector_fp4x2_e1m2_t, vector_bfloat16_t,
    ASC_DISPERSE_FIRST_QUARTER, 0);

TEST_9201_REG_CONVERT_POS(
    Mergingbfloat162e1m2x2rnmerge, asc_bfloat162e1m2x2_rn_merge, vector_fp4x2_e1m2_t, vector_bfloat16_t,
    ASC_DISPERSE_FIRST_QUARTER, 1);

TEST_9201_REG_CONVERT_POS(
    Mergingbfloat162e1m2x2rnamerge, asc_bfloat162e1m2x2_rna_merge, vector_fp4x2_e1m2_t, vector_bfloat16_t,
    ASC_DISPERSE_FIRST_QUARTER, 2);

TEST_9201_REG_CONVERT_POS(
    Mergingbfloat162e1m2x2rumerge, asc_bfloat162e1m2x2_ru_merge, vector_fp4x2_e1m2_t, vector_bfloat16_t,
    ASC_DISPERSE_FIRST_QUARTER, 3);

TEST_9201_REG_CONVERT_POS(
    Mergingbfloat162e1m2x2rzmerge, asc_bfloat162e1m2x2_rz_merge, vector_fp4x2_e1m2_t, vector_bfloat16_t,
    ASC_DISPERSE_FIRST_QUARTER, 4);

TEST_9201_REG_CONVERT_POS(
    Mergingbfloat162e2m1x2rdmerge, asc_bfloat162e2m1x2_rd_merge, vector_fp4x2_e2m1_t, vector_bfloat16_t,
    ASC_DISPERSE_FIRST_QUARTER, 5);

TEST_9201_REG_CONVERT_POS(
    Mergingbfloat162e2m1x2rnmerge, asc_bfloat162e2m1x2_rn_merge, vector_fp4x2_e2m1_t, vector_bfloat16_t,
    ASC_DISPERSE_FIRST_QUARTER, 6);

TEST_9201_REG_CONVERT_POS(
    Mergingbfloat162e2m1x2rnamerge, asc_bfloat162e2m1x2_rna_merge, vector_fp4x2_e2m1_t, vector_bfloat16_t,
    ASC_DISPERSE_FIRST_QUARTER, 7);

TEST_9201_REG_CONVERT_POS(
    Mergingbfloat162e2m1x2rumerge, asc_bfloat162e2m1x2_ru_merge, vector_fp4x2_e2m1_t, vector_bfloat16_t,
    ASC_DISPERSE_FIRST_QUARTER, 8);

TEST_9201_REG_CONVERT_POS(
    Mergingbfloat162e2m1x2rzmerge, asc_bfloat162e2m1x2_rz_merge, vector_fp4x2_e2m1_t, vector_bfloat16_t,
    ASC_DISPERSE_FIRST_QUARTER, 9);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2bfloat16rdmerge, asc_float2bfloat16_rd_merge, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 10);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2bfloat16rdsatmerge, asc_float2bfloat16_rd_sat_merge, vector_bfloat16_t, vector_float,
    ASC_POSITION_EVEN, 11);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2bfloat16rnmerge, asc_float2bfloat16_rn_merge, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 12);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2bfloat16rnsatmerge, asc_float2bfloat16_rn_sat_merge, vector_bfloat16_t, vector_float,
    ASC_POSITION_EVEN, 13);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2bfloat16rnamerge, asc_float2bfloat16_rna_merge, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN,
    14);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2bfloat16rnasatmerge, asc_float2bfloat16_rna_sat_merge, vector_bfloat16_t, vector_float,
    ASC_POSITION_EVEN, 15);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2bfloat16rumerge, asc_float2bfloat16_ru_merge, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 16);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2bfloat16rusatmerge, asc_float2bfloat16_ru_sat_merge, vector_bfloat16_t, vector_float,
    ASC_POSITION_EVEN, 17);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2bfloat16rzmerge, asc_float2bfloat16_rz_merge, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 18);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2bfloat16rzsatmerge, asc_float2bfloat16_rz_sat_merge, vector_bfloat16_t, vector_float,
    ASC_POSITION_EVEN, 19);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2e4m3rnmerge, asc_float2e4m3_rn_merge, vector_fp8_e4m3fn_t, vector_float, ASC_DISPERSE_FIRST_QUARTER,
    20);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2e4m3rnsatmerge, asc_float2e4m3_rn_sat_merge, vector_fp8_e4m3fn_t, vector_float,
    ASC_DISPERSE_FIRST_QUARTER, 21);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2e5m2rnmerge, asc_float2e5m2_rn_merge, vector_fp8_e5m2_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 22);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2e5m2rnsatmerge, asc_float2e5m2_rn_sat_merge, vector_fp8_e5m2_t, vector_float,
    ASC_DISPERSE_FIRST_QUARTER, 23);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2hif8rhm, asc_float2hif8_rh_merge, vector_hifloat8_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 24);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2hif8rhsatm, asc_float2hif8_rh_sat_merge, vector_hifloat8_t, vector_float, ASC_DISPERSE_FIRST_QUARTER,
    25);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2hif8rnamerge, asc_float2hif8_rna_merge, vector_hifloat8_t, vector_float, ASC_DISPERSE_FIRST_QUARTER,
    26);

TEST_9201_REG_CONVERT_POS(
    Mergingfloat2hif8rnasatmerge, asc_float2hif8_rna_sat_merge, vector_hifloat8_t, vector_float,
    ASC_DISPERSE_FIRST_QUARTER, 27);
