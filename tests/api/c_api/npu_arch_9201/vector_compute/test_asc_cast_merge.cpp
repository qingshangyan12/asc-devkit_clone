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

// ==========asc_cast_merge half to int4x2==========

// ==========rd==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rd_nosat_first, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RD, ASC_CAST_NO_SAT,
    ASC_DISPERSE_FIRST_QUARTER, 0);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rd_sat_first, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RD, ASC_CAST_SAT,
    ASC_DISPERSE_FIRST_QUARTER, 1);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rd_nosat_second, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RD, ASC_CAST_NO_SAT,
    ASC_DISPERSE_SECOND_QUARTER, 2);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rd_sat_second, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RD, ASC_CAST_SAT,
    ASC_DISPERSE_SECOND_QUARTER, 3);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rd_nosat_third, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RD, ASC_CAST_NO_SAT,
    ASC_DISPERSE_THIRD_QUARTER, 4);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rd_sat_third, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RD, ASC_CAST_SAT,
    ASC_DISPERSE_THIRD_QUARTER, 5);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rd_nosat_fourth, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RD, ASC_CAST_NO_SAT,
    ASC_DISPERSE_FOURTH_QUARTER, 6);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rd_sat_fourth, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RD, ASC_CAST_SAT,
    ASC_DISPERSE_FOURTH_QUARTER, 7);

// ==========rn==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rn_nosat_first, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_DISPERSE_FIRST_QUARTER, 8);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rn_sat_first, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_DISPERSE_FIRST_QUARTER, 9);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rn_nosat_second, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_DISPERSE_SECOND_QUARTER, 10);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rn_sat_second, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_DISPERSE_SECOND_QUARTER, 11);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rn_nosat_third, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_DISPERSE_THIRD_QUARTER, 12);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rn_sat_third, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_DISPERSE_THIRD_QUARTER, 13);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rn_nosat_fourth, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_DISPERSE_FOURTH_QUARTER, 14);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rn_sat_fourth, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_DISPERSE_FOURTH_QUARTER, 15);

// ==========rna==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rna_nosat_first, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RNA,
    ASC_CAST_NO_SAT, ASC_DISPERSE_FIRST_QUARTER, 16);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rna_sat_first, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RNA, ASC_CAST_SAT,
    ASC_DISPERSE_FIRST_QUARTER, 17);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rna_nosat_second, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RNA,
    ASC_CAST_NO_SAT, ASC_DISPERSE_SECOND_QUARTER, 18);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rna_sat_second, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RNA, ASC_CAST_SAT,
    ASC_DISPERSE_SECOND_QUARTER, 19);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rna_nosat_third, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RNA,
    ASC_CAST_NO_SAT, ASC_DISPERSE_THIRD_QUARTER, 20);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rna_sat_third, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RNA, ASC_CAST_SAT,
    ASC_DISPERSE_THIRD_QUARTER, 21);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rna_nosat_fourth, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RNA,
    ASC_CAST_NO_SAT, ASC_DISPERSE_FOURTH_QUARTER, 22);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rna_sat_fourth, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RNA, ASC_CAST_SAT,
    ASC_DISPERSE_FOURTH_QUARTER, 23);

// ==========ru==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_ru_nosat_first, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RU, ASC_CAST_NO_SAT,
    ASC_DISPERSE_FIRST_QUARTER, 24);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_ru_sat_first, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RU, ASC_CAST_SAT,
    ASC_DISPERSE_FIRST_QUARTER, 25);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_ru_nosat_second, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RU, ASC_CAST_NO_SAT,
    ASC_DISPERSE_SECOND_QUARTER, 26);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_ru_sat_second, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RU, ASC_CAST_SAT,
    ASC_DISPERSE_SECOND_QUARTER, 27);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_ru_nosat_third, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RU, ASC_CAST_NO_SAT,
    ASC_DISPERSE_THIRD_QUARTER, 28);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_ru_sat_third, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RU, ASC_CAST_SAT,
    ASC_DISPERSE_THIRD_QUARTER, 29);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_ru_nosat_fourth, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RU, ASC_CAST_NO_SAT,
    ASC_DISPERSE_FOURTH_QUARTER, 30);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_ru_sat_fourth, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RU, ASC_CAST_SAT,
    ASC_DISPERSE_FOURTH_QUARTER, 31);

// ==========rz==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rz_nosat_first, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RZ, ASC_CAST_NO_SAT,
    ASC_DISPERSE_FIRST_QUARTER, 32);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rz_sat_first, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RZ, ASC_CAST_SAT,
    ASC_DISPERSE_FIRST_QUARTER, 33);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rz_nosat_second, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RZ, ASC_CAST_NO_SAT,
    ASC_DISPERSE_SECOND_QUARTER, 34);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rz_sat_second, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RZ, ASC_CAST_SAT,
    ASC_DISPERSE_SECOND_QUARTER, 35);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rz_nosat_third, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RZ, ASC_CAST_NO_SAT,
    ASC_DISPERSE_THIRD_QUARTER, 36);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rz_sat_third, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RZ, ASC_CAST_SAT,
    ASC_DISPERSE_THIRD_QUARTER, 37);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rz_nosat_fourth, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RZ, ASC_CAST_NO_SAT,
    ASC_DISPERSE_FOURTH_QUARTER, 38);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rz_sat_fourth, asc_cast_merge, vcvt_f162s4, vector_int4x2_t, vector_half, ASC_CAST_RZ, ASC_CAST_SAT,
    ASC_DISPERSE_FOURTH_QUARTER, 39);

// ==========asc_cast_merge half to int8==========

// ==========rd==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rd_nosat_even, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RD, ASC_CAST_NO_SAT,
    ASC_POSITION_EVEN, 40);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rd_sat_even, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RD, ASC_CAST_SAT,
    ASC_POSITION_EVEN, 41);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rd_nosat_odd, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RD, ASC_CAST_NO_SAT,
    ASC_POSITION_ODD, 42);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rd_sat_odd, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RD, ASC_CAST_SAT,
    ASC_POSITION_ODD, 43);

// ==========rn==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rn_nosat_even, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_POSITION_EVEN, 44);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rn_sat_even, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_POSITION_EVEN, 45);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rn_nosat_odd, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_POSITION_ODD, 46);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rn_sat_odd, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_POSITION_ODD, 47);

// ==========rna==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rna_nosat_even, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RNA, ASC_CAST_NO_SAT,
    ASC_POSITION_EVEN, 48);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rna_sat_even, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RNA, ASC_CAST_SAT,
    ASC_POSITION_EVEN, 49);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rna_nosat_odd, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RNA, ASC_CAST_NO_SAT,
    ASC_POSITION_ODD, 50);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rna_sat_odd, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RNA, ASC_CAST_SAT,
    ASC_POSITION_ODD, 51);

// ==========ru==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_ru_nosat_even, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RU, ASC_CAST_NO_SAT,
    ASC_POSITION_EVEN, 52);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_ru_sat_even, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RU, ASC_CAST_SAT,
    ASC_POSITION_EVEN, 53);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_ru_nosat_odd, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RU, ASC_CAST_NO_SAT,
    ASC_POSITION_ODD, 54);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_ru_sat_odd, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RU, ASC_CAST_SAT,
    ASC_POSITION_ODD, 55);

// ==========rz==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rz_nosat_even, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RZ, ASC_CAST_NO_SAT,
    ASC_POSITION_EVEN, 56);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rz_sat_even, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RZ, ASC_CAST_SAT,
    ASC_POSITION_EVEN, 57);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rz_nosat_odd, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RZ, ASC_CAST_NO_SAT,
    ASC_POSITION_ODD, 58);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_1(
    Cast_merge_rz_sat_odd, asc_cast_merge, vcvt, vector_int8_t, vector_half, ASC_CAST_RZ, ASC_CAST_SAT,
    ASC_POSITION_ODD, 59);

// ==========asc_cast_merge bfloat16 to e4m3==========

// ==========rn==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_merge_rn_nosat_even, asc_cast_merge, vcvt, vector_fp8_e4m3fn_t, vector_bfloat16_t, ASC_CAST_RN,
    ASC_CAST_NO_SAT, ASC_POSITION_EVEN, 60);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_merge_rn_sat_even, asc_cast_merge, vcvt, vector_fp8_e4m3fn_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_POSITION_EVEN, 61);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_merge_rn_nosat_odd, asc_cast_merge, vcvt, vector_fp8_e4m3fn_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_POSITION_ODD, 62);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_merge_rn_sat_odd, asc_cast_merge, vcvt, vector_fp8_e4m3fn_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_POSITION_ODD, 63);

// ==========asc_cast_merge bfloat16 to e5m2==========

// ==========rn==========
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_merge_rn_nosat_even, asc_cast_merge, vcvt, vector_fp8_e5m2_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_POSITION_EVEN, 64);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_merge_rn_sat_even, asc_cast_merge, vcvt, vector_fp8_e5m2_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_POSITION_EVEN, 65);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_merge_rn_nosat_odd, asc_cast_merge, vcvt, vector_fp8_e5m2_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_NO_SAT,
    ASC_POSITION_ODD, 66);
TEST_VECTOR_COMPUTE_VCVT_ROUND_SAT_POS_INSTR_2(
    Cast_merge_rn_sat_odd, asc_cast_merge, vcvt, vector_fp8_e5m2_t, vector_bfloat16_t, ASC_CAST_RN, ASC_CAST_SAT,
    ASC_POSITION_ODD, 67);
