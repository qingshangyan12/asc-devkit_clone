/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_UTILS_IMPL_UTILS_C_API_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_UTILS_IMPL_UTILS_C_API_IMPL_H

#include "utils/base/sys_constants.h"

constexpr uint32_t C_API_AIC_TYPE = AscendC::AIC;
constexpr uint32_t C_API_AIV_TYPE = AscendC::AIV;
constexpr uint32_t C_API_MIX_TYPE = AscendC::MIX;

#define ASC_IS_AIV ASCEND_IS_AIV
#define ASC_IS_AIC ASCEND_IS_AIC

#endif
