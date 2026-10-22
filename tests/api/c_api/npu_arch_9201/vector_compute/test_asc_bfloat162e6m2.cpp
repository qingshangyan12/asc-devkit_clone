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

// ==========asc_bfloat162e6m2==========
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_3(
    Bf162e6m2_rn_even, asc_bfloat162e6m2_rn, vcvt_bf162e6m2, vector_fp8_e6m2_t, vector_bfloat16_t, ASC_POSITION_EVEN,
    0);
TEST_VECTOR_COMPUTE_VCVT_POS_INSTR_3(
    Bf162e6m2_rn_odd, asc_bfloat162e6m2_rn, vcvt_bf162e6m2, vector_fp8_e6m2_t, vector_bfloat16_t, ASC_POSITION_ODD, 1);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_3(
    Bf162e6m2_rn_even, asc_bfloat162e6m2_rn, vcvt_bf162e6m2, vector_fp8_e6m2_t, vector_bfloat16_t, ASC_POSITION_EVEN,
    2);
TEST_VECTOR_COMPUTE_VCVT_RETURN_POS_INSTR_3(
    Bf162e6m2_rn_odd, asc_bfloat162e6m2_rn, vcvt_bf162e6m2, vector_fp8_e6m2_t, vector_bfloat16_t, ASC_POSITION_ODD, 3);
