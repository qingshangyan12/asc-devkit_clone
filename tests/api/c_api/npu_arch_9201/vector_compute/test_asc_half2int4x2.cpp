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
    Vcvt_rd_first, asc_half2int4x2_rd_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 0);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_second, asc_half2int4x2_rd_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER, 1);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_third, asc_half2int4x2_rd_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 2);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_fourth, asc_half2int4x2_rd_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER, 3);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_sat_first, asc_half2int4x2_rd_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER,
    4);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_sat_second, asc_half2int4x2_rd_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 5);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_sat_third, asc_half2int4x2_rd_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER,
    6);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rd_sat_fourth, asc_half2int4x2_rd_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 7);

// ==========rz==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_first, asc_half2int4x2_rz_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 8);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_second, asc_half2int4x2_rz_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER, 9);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_third, asc_half2int4x2_rz_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 10);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_fourth, asc_half2int4x2_rz_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER, 11);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_sat_first, asc_half2int4x2_rz_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER,
    12);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_sat_second, asc_half2int4x2_rz_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 13);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_sat_third, asc_half2int4x2_rz_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER,
    14);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rz_sat_fourth, asc_half2int4x2_rz_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 15);

// ==========rn==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_first, asc_half2int4x2_rn_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 16);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_second, asc_half2int4x2_rn_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER, 17);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_third, asc_half2int4x2_rn_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 18);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_fourth, asc_half2int4x2_rn_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER, 19);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_sat_first, asc_half2int4x2_rn_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER,
    20);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_sat_second, asc_half2int4x2_rn_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 21);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_sat_third, asc_half2int4x2_rn_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER,
    22);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rn_sat_fourth, asc_half2int4x2_rn_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 23);

// ==========rna==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_first, asc_half2int4x2_rna_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 24);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_second, asc_half2int4x2_rna_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER, 25);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_third, asc_half2int4x2_rna_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 26);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_fourth, asc_half2int4x2_rna_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER, 27);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_sat_first, asc_half2int4x2_rna_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FIRST_QUARTER, 28);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_sat_second, asc_half2int4x2_rna_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 29);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_sat_third, asc_half2int4x2_rna_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_THIRD_QUARTER, 30);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_rna_sat_fourth, asc_half2int4x2_rna_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 31);

// ==========ru==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_first, asc_half2int4x2_ru_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 32);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_second, asc_half2int4x2_ru_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER, 33);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_third, asc_half2int4x2_ru_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 34);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_fourth, asc_half2int4x2_ru_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER, 35);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_sat_first, asc_half2int4x2_ru_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER,
    36);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_sat_second, asc_half2int4x2_ru_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 37);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_sat_third, asc_half2int4x2_ru_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER,
    38);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_1(
    Vcvt_ru_sat_fourth, asc_half2int4x2_ru_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 39);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rd_first, asc_half2int4x2_rd_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 40);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rd_sat_first, asc_half2int4x2_rd_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER,
    41);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rd_second, asc_half2int4x2_rd_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER, 42);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rd_sat_second, asc_half2int4x2_rd_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 43);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rd_third, asc_half2int4x2_rd_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 44);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rd_sat_third, asc_half2int4x2_rd_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER,
    45);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rd_fourth, asc_half2int4x2_rd_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER, 46);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rd_sat_fourth, asc_half2int4x2_rd_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 47);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rz_first, asc_half2int4x2_rz_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 48);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rz_sat_first, asc_half2int4x2_rz_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER,
    49);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rz_second, asc_half2int4x2_rz_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER, 50);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rz_sat_second, asc_half2int4x2_rz_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 51);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rz_third, asc_half2int4x2_rz_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 52);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rz_sat_third, asc_half2int4x2_rz_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER,
    53);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rz_fourth, asc_half2int4x2_rz_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER, 54);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rz_sat_fourth, asc_half2int4x2_rz_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 55);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rn_first, asc_half2int4x2_rn_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 56);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rn_sat_first, asc_half2int4x2_rn_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER,
    57);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rn_second, asc_half2int4x2_rn_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER, 58);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rn_sat_second, asc_half2int4x2_rn_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 59);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rn_third, asc_half2int4x2_rn_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 60);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rn_sat_third, asc_half2int4x2_rn_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER,
    61);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rn_fourth, asc_half2int4x2_rn_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER, 62);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rn_sat_fourth, asc_half2int4x2_rn_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 63);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rna_first, asc_half2int4x2_rna_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 64);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rna_sat_first, asc_half2int4x2_rna_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FIRST_QUARTER, 65);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rna_second, asc_half2int4x2_rna_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER, 66);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rna_sat_second, asc_half2int4x2_rna_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 67);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rna_third, asc_half2int4x2_rna_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 68);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rna_sat_third, asc_half2int4x2_rna_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_THIRD_QUARTER, 69);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rna_fourth, asc_half2int4x2_rna_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER, 70);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_rna_sat_fourth, asc_half2int4x2_rna_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 71);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_ru_first, asc_half2int4x2_ru_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER, 72);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_ru_sat_first, asc_half2int4x2_ru_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FIRST_QUARTER,
    73);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_ru_second, asc_half2int4x2_ru_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_SECOND_QUARTER, 74);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_ru_sat_second, asc_half2int4x2_ru_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_SECOND_QUARTER, 75);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_ru_third, asc_half2int4x2_ru_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER, 76);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_ru_sat_third, asc_half2int4x2_ru_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_THIRD_QUARTER,
    77);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_ru_fourth, asc_half2int4x2_ru_m, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_DISPERSE_FOURTH_QUARTER, 78);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_1(
    Vcvt_ru_sat_fourth, asc_half2int4x2_ru_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half,
    ASC_DISPERSE_FOURTH_QUARTER, 79);
