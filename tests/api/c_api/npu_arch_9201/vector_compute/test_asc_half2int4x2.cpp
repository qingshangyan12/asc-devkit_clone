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
    Vcvt_rd_first, asc_half2int4x2_rd_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 0);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_second, asc_half2int4x2_rd_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER,
    1);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_third, asc_half2int4x2_rd_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 2);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_fourth, asc_half2int4x2_rd_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER,
    3);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_sat_first, asc_half2int4x2_rd_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FIRST_QUARTER, 4);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_sat_second, asc_half2int4x2_rd_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 5);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_sat_third, asc_half2int4x2_rd_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_THIRD_QUARTER, 6);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_sat_fourth, asc_half2int4x2_rd_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 7);

// ==========rz==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_first, asc_half2int4x2_rz_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 8);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_second, asc_half2int4x2_rz_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER,
    9);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_third, asc_half2int4x2_rz_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 10);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_fourth, asc_half2int4x2_rz_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER,
    11);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_sat_first, asc_half2int4x2_rz_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FIRST_QUARTER, 12);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_sat_second, asc_half2int4x2_rz_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 13);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_sat_third, asc_half2int4x2_rz_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_THIRD_QUARTER, 14);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_sat_fourth, asc_half2int4x2_rz_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 15);

// ==========rn==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_first, asc_half2int4x2_rn_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 16);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_second, asc_half2int4x2_rn_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER,
    17);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_third, asc_half2int4x2_rn_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 18);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_fourth, asc_half2int4x2_rn_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER,
    19);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_sat_first, asc_half2int4x2_rn_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FIRST_QUARTER, 20);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_sat_second, asc_half2int4x2_rn_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 21);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_sat_third, asc_half2int4x2_rn_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_THIRD_QUARTER, 22);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_sat_fourth, asc_half2int4x2_rn_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 23);

// ==========rna==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_first, asc_half2int4x2_rna_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER,
    24);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_second, asc_half2int4x2_rna_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER,
    25);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_third, asc_half2int4x2_rna_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER,
    26);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_fourth, asc_half2int4x2_rna_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER,
    27);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_sat_first, asc_half2int4x2_rna_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FIRST_QUARTER, 28);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_sat_second, asc_half2int4x2_rna_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 29);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_sat_third, asc_half2int4x2_rna_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_THIRD_QUARTER, 30);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_sat_fourth, asc_half2int4x2_rna_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 31);

// ==========ru==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_first, asc_half2int4x2_ru_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 32);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_second, asc_half2int4x2_ru_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER,
    33);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_third, asc_half2int4x2_ru_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 34);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_fourth, asc_half2int4x2_ru_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER,
    35);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_sat_first, asc_half2int4x2_ru_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FIRST_QUARTER, 36);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_sat_second, asc_half2int4x2_ru_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 37);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_sat_third, asc_half2int4x2_ru_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_THIRD_QUARTER, 38);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_sat_fourth, asc_half2int4x2_ru_sat_merge, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 39);
