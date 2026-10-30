/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#define CANN_ASC_USE_EXPERIMENTAL
#include "c_api/experimental/reg_arith_exp.h"
#include "c_api/experimental/reg_permute_sel_exp.h"
#include "c_api/experimental/loadalign_exp.h"
#include "c_api/asc_simd.h"

static_assert(sizeof(asc_uint32_t_bool_pair) > 0);
static_assert(sizeof(asc_float_float_pair) > 0);
