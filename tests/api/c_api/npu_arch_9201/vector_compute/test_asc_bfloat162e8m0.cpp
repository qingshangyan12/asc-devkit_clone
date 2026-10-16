/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software: you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License).
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "tests/api/c_api/npu_arch_9201/utils/test_vcvt_instr_utils.h"

// ==========asc_bfloat162e8m0==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Bf162e8m0_ru_even, asc_bfloat162e8m0_ru, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_POSITION_EVEN,
    0);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Bf162e8m0_ru_odd, asc_bfloat162e8m0_ru, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_POSITION_ODD, 1);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Bf162e8m0_ru_sat_even, asc_bfloat162e8m0_ru_sat, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t,
    ASC_POSITION_EVEN, 2);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Bf162e8m0_ru_sat_odd, asc_bfloat162e8m0_ru_sat, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t,
    ASC_POSITION_ODD, 3);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Bf162e8m0_rz_even, asc_bfloat162e8m0_rz, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_POSITION_EVEN,
    4);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Bf162e8m0_rz_odd, asc_bfloat162e8m0_rz, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_POSITION_ODD, 5);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Bf162e8m0_rz_sat_even, asc_bfloat162e8m0_rz_sat, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t,
    ASC_POSITION_EVEN, 6);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Bf162e8m0_rz_sat_odd, asc_bfloat162e8m0_rz_sat, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t,
    ASC_POSITION_ODD, 7);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_2(
    Bf162e8m0_ru_even, asc_bfloat162e8m0_ru, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_POSITION_EVEN,
    8);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_2(
    Bf162e8m0_ru_odd, asc_bfloat162e8m0_ru, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_POSITION_ODD, 9);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_2(
    Bf162e8m0_ru_sat_even, asc_bfloat162e8m0_ru_sat, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t,
    ASC_POSITION_EVEN, 10);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_2(
    Bf162e8m0_ru_sat_odd, asc_bfloat162e8m0_ru_sat, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t,
    ASC_POSITION_ODD, 11);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_2(
    Bf162e8m0_rz_even, asc_bfloat162e8m0_rz, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_POSITION_EVEN,
    12);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_2(
    Bf162e8m0_rz_odd, asc_bfloat162e8m0_rz, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_POSITION_ODD, 13);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_2(
    Bf162e8m0_rz_sat_even, asc_bfloat162e8m0_rz_sat, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t,
    ASC_POSITION_EVEN, 14);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_2(
    Bf162e8m0_rz_sat_odd, asc_bfloat162e8m0_rz_sat, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t,
    ASC_POSITION_ODD, 15);
