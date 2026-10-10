/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software: you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "tests/api/c_api/npu_arch_9201/utils/test_vcvt_instr_utils.h"

// ==========asc_half2e4m3==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Half2e4m3_rn_even, asc_half2e4m3_rn, vcvt, vector_fp8_e4m3fn_t, vector_half, ASC_POSITION_EVEN, ROUND_R, RS_DISABLE,
    PART_EVEN, MODE_ZEROING, 0);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Half2e4m3_rn_odd, asc_half2e4m3_rn, vcvt, vector_fp8_e4m3fn_t, vector_half, ASC_POSITION_ODD, ROUND_R, RS_DISABLE,
    PART_ODD, MODE_ZEROING, 1);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Half2e4m3_rn_sat_even, asc_half2e4m3_rn_sat, vcvt, vector_fp8_e4m3fn_t, vector_half, ASC_POSITION_EVEN, ROUND_R,
    RS_ENABLE, PART_EVEN, MODE_ZEROING, 2);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Half2e4m3_rn_sat_odd, asc_half2e4m3_rn_sat, vcvt, vector_fp8_e4m3fn_t, vector_half, ASC_POSITION_ODD, ROUND_R,
    RS_ENABLE, PART_ODD, MODE_ZEROING, 3);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Half2e4m3_rn_merge_even, asc_half2e4m3_rn_merge, vcvt, vector_fp8_e4m3fn_t, vector_half, ASC_POSITION_EVEN, ROUND_R,
    RS_DISABLE, PART_EVEN, MODE_MERGING, 4);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Half2e4m3_rn_merge_odd, asc_half2e4m3_rn_merge, vcvt, vector_fp8_e4m3fn_t, vector_half, ASC_POSITION_ODD, ROUND_R,
    RS_DISABLE, PART_ODD, MODE_MERGING, 5);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Half2e4m3_rn_sat_merge_even, asc_half2e4m3_rn_sat_merge, vcvt, vector_fp8_e4m3fn_t, vector_half, ASC_POSITION_EVEN,
    ROUND_R, RS_ENABLE, PART_EVEN, MODE_MERGING, 6);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_2(
    Half2e4m3_rn_sat_merge_odd, asc_half2e4m3_rn_sat_merge, vcvt, vector_fp8_e4m3fn_t, vector_half, ASC_POSITION_ODD,
    ROUND_R, RS_ENABLE, PART_ODD, MODE_MERGING, 7);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_2(
    Half2e4m3_rn_even, asc_half2e4m3_rn, vcvt, vector_fp8_e4m3fn_t, vector_half, ASC_POSITION_EVEN, ROUND_R, RS_DISABLE,
    PART_EVEN, MODE_ZEROING, 8);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_2(
    Half2e4m3_rn_odd, asc_half2e4m3_rn, vcvt, vector_fp8_e4m3fn_t, vector_half, ASC_POSITION_ODD, ROUND_R, RS_DISABLE,
    PART_ODD, MODE_ZEROING, 9);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_2(
    Half2e4m3_rn_sat_even, asc_half2e4m3_rn_sat, vcvt, vector_fp8_e4m3fn_t, vector_half, ASC_POSITION_EVEN, ROUND_R,
    RS_ENABLE, PART_EVEN, MODE_ZEROING, 10);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_2(
    Half2e4m3_rn_sat_odd, asc_half2e4m3_rn_sat, vcvt, vector_fp8_e4m3fn_t, vector_half, ASC_POSITION_ODD, ROUND_R,
    RS_ENABLE, PART_ODD, MODE_ZEROING, 11);
TEST_VECTOR_COMPUTE_VCVT_INSTR_2(
    Half2e4m3_rn_v2, asc_half2e4m3_rn_v2, vcvt, vector_fp8_e4m3fn_t, vector_half, ROUND_R, RS_DISABLE, PART_ODD,
    MODE_ZEROING, 12);
TEST_VECTOR_COMPUTE_VCVT_INSTR_2(
    Half2e4m3_rn_sat_v2, asc_half2e4m3_rn_sat_v2, vcvt, vector_fp8_e4m3fn_t, vector_half, ROUND_R, RS_ENABLE, PART_ODD,
    MODE_ZEROING, 13);
