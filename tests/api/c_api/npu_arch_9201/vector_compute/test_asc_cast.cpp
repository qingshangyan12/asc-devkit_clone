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

// ==========asc_cast bfloat16 to e4m3==========

// ==========rn==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_rn_nosat_even, asc_cast, vcvt, vector_fp8_e4m3fn_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_POSITION_EVEN, 0);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_rn_sat_even, asc_cast, vcvt, vector_fp8_e4m3fn_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_POSITION_EVEN, 1);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_rn_nosat_odd, asc_cast, vcvt, vector_fp8_e4m3fn_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_POSITION_ODD, 2);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_rn_sat_odd, asc_cast, vcvt, vector_fp8_e4m3fn_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_POSITION_ODD, 3);

// ==========asc_cast bfloat16 to e5m2==========

// ==========rn==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_rn_nosat_even, asc_cast, vcvt, vector_fp8_e5m2_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_POSITION_EVEN, 4);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_rn_sat_even, asc_cast, vcvt, vector_fp8_e5m2_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_POSITION_EVEN, 5);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_rn_nosat_odd, asc_cast, vcvt, vector_fp8_e5m2_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_POSITION_ODD, 6);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_rn_sat_odd, asc_cast, vcvt, vector_fp8_e5m2_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_SAT, ASC_POSITION_ODD,
    7);

// ==========asc_cast bfloat16 to e6m2==========

// ==========rn==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_POS_INSTR_3(
    Cast_rn_even, asc_cast, vcvt_bf162e6m2, vector_fp8_e6m2_t, vector_bfloat16_t, ASC_CAST_RN, ASC_POSITION_EVEN, 8);
TEST_VECTOR_COMPUTE_VCVT_ROUND_POS_INSTR_3(
    Cast_rn_odd, asc_cast, vcvt_bf162e6m2, vector_fp8_e6m2_t, vector_bfloat16_t, ASC_CAST_RN, ASC_POSITION_ODD, 9);

// ==========asc_cast bfloat16 to e8m0==========

// ==========ru==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_ru_nosat_even, asc_cast, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_CAST_RU, ASC_CAST_NO_SAT,
    ASC_POSITION_EVEN, 10);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_ru_sat_even, asc_cast, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_CAST_RU, ASC_CAST_SAT,
    ASC_POSITION_EVEN, 11);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_ru_nosat_odd, asc_cast, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_CAST_RU, ASC_CAST_NO_SAT,
    ASC_POSITION_ODD, 12);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_ru_sat_odd, asc_cast, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_CAST_RU, ASC_CAST_SAT,
    ASC_POSITION_ODD, 13);

// ==========rz==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_rz_nosat_even, asc_cast, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_CAST_RZ, ASC_CAST_NO_SAT,
    ASC_POSITION_EVEN, 14);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_rz_sat_even, asc_cast, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_CAST_RZ, ASC_CAST_SAT,
    ASC_POSITION_EVEN, 15);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_rz_nosat_odd, asc_cast, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_CAST_RZ, ASC_CAST_NO_SAT,
    ASC_POSITION_ODD, 16);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_rz_sat_odd, asc_cast, vcvt_bf162e8m0, vector_fp8_e8m0_t, vector_bfloat16_t, ASC_CAST_RZ, ASC_CAST_SAT,
    ASC_POSITION_ODD, 17);
