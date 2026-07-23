/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "reg_load.h cannot be used with compile flag --enable-simt enabled."
#endif

#ifndef INCLUDE_C_API_REG_COMPUTE_REG_LOAD_H
#define INCLUDE_C_API_REG_COMPUTE_REG_LOAD_H

#include "c_api/reg_vector/load/reg_loadalign.h"
#include "c_api/reg_vector/load/reg_loadunalign.h"
#include "c_api/reg_vector/gather/reg_gather_load.h"
#include "c_api/composite/reg_loadunalign.h"

#endif
