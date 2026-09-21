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

// ==========rd==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_even, asc_half2int8_rd_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_EVEN, 0);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_odd, asc_half2int8_rd_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_ODD, 1);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_sat_even, asc_half2int8_rd_sat_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_EVEN, 2);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_sat_odd, asc_half2int8_rd_sat_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_ODD, 3);

// ==========ru==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_even, asc_half2int8_ru_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_EVEN, 4);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_odd, asc_half2int8_ru_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_ODD, 5);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_sat_even, asc_half2int8_ru_sat_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_EVEN, 6);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_sat_odd, asc_half2int8_ru_sat_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_ODD, 7);

// ==========rz==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_even, asc_half2int8_rz_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_EVEN, 8);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_odd, asc_half2int8_rz_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_ODD, 9);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_sat_even, asc_half2int8_rz_sat_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_EVEN, 10);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_sat_odd, asc_half2int8_rz_sat_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_ODD, 11);

// ==========rn==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_even, asc_half2int8_rn_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_EVEN, 12);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_odd, asc_half2int8_rn_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_ODD, 13);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_sat_even, asc_half2int8_rn_sat_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_EVEN, 14);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_sat_odd, asc_half2int8_rn_sat_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_ODD, 15);

// ==========rna==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_even, asc_half2int8_rna_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_EVEN, 16);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_odd, asc_half2int8_rna_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_ODD, 17);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_sat_even, asc_half2int8_rna_sat_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_EVEN, 18);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_sat_odd, asc_half2int8_rna_sat_merge, vcvt, vector_int8_t, vector_half, ASC_POSITION_ODD, 19);
