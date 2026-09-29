/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_MC2_RESOURCE_DFX_H
#define ASCENDC_MC2_RESOURCE_DFX_H

#include "log.h"

#define MC2_DFX_LOG(...) \
    do { \
        HCCL_ERROR(__VA_ARGS__); \
        HCCL_RUN_INFO(__VA_ARGS__); \
    } while (0)

// Diagnostic-only return tracing. Success never emits a message. The normal log
// prefix supplies PID/TID; file/line and expression identify the exact exit edge.
namespace Mc2ResourceDfx {
inline HcclResult Result(HcclResult result, const char* file, int line, const char* function, const char* call)
{
    if (result != HCCL_SUCCESS) {
        HCCL_ERROR("[MC2_ALLOC_DFX_v1] file[%s:%d] function[%s] call[%s] ret[%d]",
            file, line, function, call, static_cast<int>(result));
        HCCL_RUN_INFO("[MC2_ALLOC_DFX_v1] file[%s:%d] function[%s] call[%s] ret[%d]",
            file, line, function, call, static_cast<int>(result));
    }
    return result;
}
} // namespace Mc2ResourceDfx

#define MC2_DFX_RESULT(call) Mc2ResourceDfx::Result((call), __FILE__, __LINE__, __func__, #call)
#define MC2_DFX_CHK_RET(call) \
    do { \
        const HcclResult mc2DfxRet = MC2_DFX_RESULT(call); \
        if (mc2DfxRet != HCCL_SUCCESS) { \
            return mc2DfxRet; \
        } \
    } while (0)
#define MC2_DFX_CHK_PTR_NULL(ptr) \
    do { \
        if ((ptr) == nullptr) { \
            return Mc2ResourceDfx::Result(HCCL_E_PTR, __FILE__, __LINE__, __func__, #ptr " == nullptr"); \
        } \
    } while (0)
#define MC2_DFX_CHK_PRT_RET(condition, log, result) \
    do { \
        if (condition) { \
            log; \
            return Mc2ResourceDfx::Result((result), __FILE__, __LINE__, __func__, #condition); \
        } \
    } while (0)

#endif // ASCENDC_MC2_RESOURCE_DFX_H
