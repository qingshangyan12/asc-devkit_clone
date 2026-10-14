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

// ========== 9201 same-name convert APIs ==========

TEST_9201_REG_CONVERT_POS(
    Samebfloat162e1m2x2rd, asc_bfloat162e1m2x2_rd, vector_fp4x2_e1m2_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    0);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samebfloat162e1m2x2rd, asc_bfloat162e1m2x2_rd, vector_fp4x2_e1m2_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    1);

TEST_9201_REG_CONVERT_POS(
    Samebfloat162e1m2x2rn, asc_bfloat162e1m2x2_rn, vector_fp4x2_e1m2_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    2);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samebfloat162e1m2x2rn, asc_bfloat162e1m2x2_rn, vector_fp4x2_e1m2_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    3);

TEST_9201_REG_CONVERT_POS(
    Samebfloat162e1m2x2rna, asc_bfloat162e1m2x2_rna, vector_fp4x2_e1m2_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    4);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samebfloat162e1m2x2rna, asc_bfloat162e1m2x2_rna, vector_fp4x2_e1m2_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    5);

TEST_9201_REG_CONVERT_POS(
    Samebfloat162e1m2x2ru, asc_bfloat162e1m2x2_ru, vector_fp4x2_e1m2_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    6);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samebfloat162e1m2x2ru, asc_bfloat162e1m2x2_ru, vector_fp4x2_e1m2_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    7);

TEST_9201_REG_CONVERT_POS(
    Samebfloat162e1m2x2rz, asc_bfloat162e1m2x2_rz, vector_fp4x2_e1m2_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    8);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samebfloat162e1m2x2rz, asc_bfloat162e1m2x2_rz, vector_fp4x2_e1m2_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    9);

TEST_9201_REG_CONVERT_POS(
    Samebfloat162e2m1x2rd, asc_bfloat162e2m1x2_rd, vector_fp4x2_e2m1_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    10);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samebfloat162e2m1x2rd, asc_bfloat162e2m1x2_rd, vector_fp4x2_e2m1_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    11);

TEST_9201_REG_CONVERT_POS(
    Samebfloat162e2m1x2rn, asc_bfloat162e2m1x2_rn, vector_fp4x2_e2m1_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    12);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samebfloat162e2m1x2rn, asc_bfloat162e2m1x2_rn, vector_fp4x2_e2m1_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    13);

TEST_9201_REG_CONVERT_POS(
    Samebfloat162e2m1x2rna, asc_bfloat162e2m1x2_rna, vector_fp4x2_e2m1_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    14);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samebfloat162e2m1x2rna, asc_bfloat162e2m1x2_rna, vector_fp4x2_e2m1_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    15);

TEST_9201_REG_CONVERT_POS(
    Samebfloat162e2m1x2ru, asc_bfloat162e2m1x2_ru, vector_fp4x2_e2m1_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    16);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samebfloat162e2m1x2ru, asc_bfloat162e2m1x2_ru, vector_fp4x2_e2m1_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    17);

TEST_9201_REG_CONVERT_POS(
    Samebfloat162e2m1x2rz, asc_bfloat162e2m1x2_rz, vector_fp4x2_e2m1_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    18);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samebfloat162e2m1x2rz, asc_bfloat162e2m1x2_rz, vector_fp4x2_e2m1_t, vector_bfloat16_t, ASC_DISPERSE_FIRST_QUARTER,
    19);

TEST_9201_REG_CONVERT_POS(
    Samefloat2bfloat16rd, asc_float2bfloat16_rd, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 20);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2bfloat16rd, asc_float2bfloat16_rd, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 21);

TEST_9201_REG_CONVERT_POS(
    Samefloat2bfloat16rdsat, asc_float2bfloat16_rd_sat, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 22);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2bfloat16rdsat, asc_float2bfloat16_rd_sat, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 23);

TEST_9201_REG_CONVERT_POS(
    Samefloat2bfloat16rn, asc_float2bfloat16_rn, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 24);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2bfloat16rn, asc_float2bfloat16_rn, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 25);

TEST_9201_REG_CONVERT_POS(
    Samefloat2bfloat16rnsat, asc_float2bfloat16_rn_sat, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 26);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2bfloat16rnsat, asc_float2bfloat16_rn_sat, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 27);

TEST_9201_REG_CONVERT_POS(
    Samefloat2bfloat16rna, asc_float2bfloat16_rna, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 28);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2bfloat16rna, asc_float2bfloat16_rna, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 29);

TEST_9201_REG_CONVERT_POS(
    Samefloat2bfloat16rnasat, asc_float2bfloat16_rna_sat, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 30);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2bfloat16rnasat, asc_float2bfloat16_rna_sat, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 31);

TEST_9201_REG_CONVERT_POS(
    Samefloat2bfloat16ru, asc_float2bfloat16_ru, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 32);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2bfloat16ru, asc_float2bfloat16_ru, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 33);

TEST_9201_REG_CONVERT_POS(
    Samefloat2bfloat16rusat, asc_float2bfloat16_ru_sat, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 34);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2bfloat16rusat, asc_float2bfloat16_ru_sat, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 35);

TEST_9201_REG_CONVERT_POS(
    Samefloat2bfloat16rz, asc_float2bfloat16_rz, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 36);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2bfloat16rz, asc_float2bfloat16_rz, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 37);

TEST_9201_REG_CONVERT_POS(
    Samefloat2bfloat16rzsat, asc_float2bfloat16_rz_sat, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 38);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2bfloat16rzsat, asc_float2bfloat16_rz_sat, vector_bfloat16_t, vector_float, ASC_POSITION_EVEN, 39);

TEST_9201_REG_CONVERT_POS(
    Samefloat2e4m3rn, asc_float2e4m3_rn, vector_fp8_e4m3fn_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 40);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2e4m3rn, asc_float2e4m3_rn, vector_fp8_e4m3fn_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 41);

TEST_9201_REG_CONVERT_POS(
    Samefloat2e4m3rnsat, asc_float2e4m3_rn_sat, vector_fp8_e4m3fn_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 42);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2e4m3rnsat, asc_float2e4m3_rn_sat, vector_fp8_e4m3fn_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 43);

TEST_9201_REG_CONVERT_POS(
    Samefloat2e5m2rn, asc_float2e5m2_rn, vector_fp8_e5m2_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 44);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2e5m2rn, asc_float2e5m2_rn, vector_fp8_e5m2_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 45);

TEST_9201_REG_CONVERT_POS(
    Samefloat2e5m2rnsat, asc_float2e5m2_rn_sat, vector_fp8_e5m2_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 46);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2e5m2rnsat, asc_float2e5m2_rn_sat, vector_fp8_e5m2_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 47);

TEST_9201_REG_CONVERT_POS(
    Samefloat2hif8rh, asc_float2hif8_rh, vector_hifloat8_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 48);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2hif8rh, asc_float2hif8_rh, vector_hifloat8_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 49);

TEST_9201_REG_CONVERT_POS(
    Samefloat2hif8rhsat, asc_float2hif8_rh_sat, vector_hifloat8_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 50);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2hif8rhsat, asc_float2hif8_rh_sat, vector_hifloat8_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 51);

TEST_9201_REG_CONVERT_POS(
    Samefloat2hif8rna, asc_float2hif8_rna, vector_hifloat8_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 52);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2hif8rna, asc_float2hif8_rna, vector_hifloat8_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 53);

TEST_9201_REG_CONVERT_POS(
    Samefloat2hif8rnasat, asc_float2hif8_rna_sat, vector_hifloat8_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 54);
TEST_9201_REG_CONVERT_RETURN_POS(
    Samefloat2hif8rnasat, asc_float2hif8_rna_sat, vector_hifloat8_t, vector_float, ASC_DISPERSE_FIRST_QUARTER, 55);
