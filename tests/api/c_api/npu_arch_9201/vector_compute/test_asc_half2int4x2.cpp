/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "tests/api/c_api/npu_arch_3510/utils/test_vcvt_instr_utils.h"

TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd_m, asc_half2int4x2_rd_m, vcvt_f162s4, vector_int4x2_t, vector_half, 0);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd_sat_m, asc_half2int4x2_rd_sat_m, vcvt_f162s4, vector_int4x2_t, vector_half, 1);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd_v2_m, asc_half2int4x2_rd_v2_m, vcvt_f162s4, vector_int4x2_t, vector_half, 2);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(
    vcvt_rd_sat_v2_m, asc_half2int4x2_rd_sat_v2_m, vcvt_f162s4, vector_int4x2_t, vector_half, 3);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd_v3_m, asc_half2int4x2_rd_v3_m, vcvt_f162s4, vector_int4x2_t, vector_half, 4);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(
    vcvt_rd_sat_v3_m, asc_half2int4x2_rd_sat_v3_m, vcvt_f162s4, vector_int4x2_t, vector_half, 5);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(vcvt_rd_v4_m, asc_half2int4x2_rd_v4_m, vcvt_f162s4, vector_int4x2_t, vector_half, 6);
TEST_VECTOR_COMPUTE_VCVT_INSTR_1(
    vcvt_rd_sat_v4_m, asc_half2int4x2_rd_sat_v4_m, vcvt_f162s4, vector_int4x2_t, vector_half, 7);
