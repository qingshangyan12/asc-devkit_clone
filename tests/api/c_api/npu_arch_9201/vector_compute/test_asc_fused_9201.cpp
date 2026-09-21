/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "tests/api/c_api/npu_arch_3510/utils/test_binary_instr_utils.h"

// ==========asc_fma (9201 vfma 实现)==========
TEST_VECTOR_COMPUTE_TERNARY_RET_INSTR(AscFma, asc_fma, vfma, vector_half);
TEST_VECTOR_COMPUTE_TERNARY_RET_INSTR(AscFma, asc_fma, vfma, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_TERNARY_RET_INSTR(AscFma, asc_fma, vfma, vector_float);

// ==========asc_fms==========
TEST_VECTOR_COMPUTE_TERNARY_RET_INSTR(AscFms, asc_fms, vfnms, vector_half);
TEST_VECTOR_COMPUTE_TERNARY_RET_INSTR(AscFms, asc_fms, vfnms, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_TERNARY_RET_INSTR(AscFms, asc_fms, vfnms, vector_float);

// ==========asc_fnma==========
TEST_VECTOR_COMPUTE_TERNARY_RET_INSTR(AscFnma, asc_fnma, vfms, vector_half);
TEST_VECTOR_COMPUTE_TERNARY_RET_INSTR(AscFnma, asc_fnma, vfms, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_TERNARY_RET_INSTR(AscFnma, asc_fnma, vfms, vector_float);

// ==========asc_fnms==========
TEST_VECTOR_COMPUTE_TERNARY_RET_INSTR(AscFnms, asc_fnms, vfnma, vector_half);
TEST_VECTOR_COMPUTE_TERNARY_RET_INSTR(AscFnms, asc_fnms, vfnma, vector_bfloat16_t);
TEST_VECTOR_COMPUTE_TERNARY_RET_INSTR(AscFnms, asc_fnms, vfnma, vector_float);
