/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CCU_LOG_H
#define CCU_LOG_H

#include "ccu_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CCU_CHK_RET(expr) do { CcuResult ret = (expr); if (ret != CCU_SUCCESS) return ret; } while(0)

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <stdexcept>
#include <string>

namespace ccu {

inline void ThrowIfFailed(CcuResult result, const std::string& message) {
    if (result != CCU_SUCCESS) {
        throw std::runtime_error(message + ": result=" + std::to_string(result));
    }
}

}

#define CCU_THROW_IF_FAILED(expr, msg) ccu::ThrowIfFailed((expr), (msg))

#endif

#endif