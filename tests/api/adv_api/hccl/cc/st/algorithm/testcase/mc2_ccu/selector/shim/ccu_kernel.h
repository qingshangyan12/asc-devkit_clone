/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file be in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CCU_ST_SELECTOR_SHIM_CCU_KERNEL_H
#define CCU_ST_SELECTOR_SHIM_CCU_KERNEL_H

#include <cstdint>
#include <vector>

// CcuResult enum (matches ccu_types.h)
enum CcuResult {
    CCU_SUCCESS = 0,
    CCU_E_PTR = 1,
    CCU_E_PARA = 2,
    CCU_E_INTERNAL = 3,
    CCU_E_NOT_SUPPORT = 4,
};

// CCU handle types
using CcuInsHandle = uint64_t;
using CcuKernelHandle = uint64_t;

#endif // CCU_ST_SELECTOR_SHIM_CCU_KERNEL_H
