/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <cstdint>
#include "mc2_server_stage_timing.h"
#include "mc2_server_debug_config.h"

#include "aicpu_kfc/decoupler/comm_kfc_dispatcher.h"
#include "common/aicpu_kfc_def.h"
#include "common/aicpu_kfc_utils.h"
#include "hccl_msg.h"
#include "log.h"

using namespace HcclApi;

static uint32_t Mc2ServerKernelBody(void* args[])
{
    HCCL_INFO("[Mc2ServerKernel]Start Mc2ServerKernel.");
    if (args == nullptr) {
        HCCL_ERROR("args is null.");
        return HCCL_E_PARA;
    }

    constexpr int DESC_POS = 0;
    uint64_t descValue = reinterpret_cast<uint64_t>(args[DESC_POS]);
    if (descValue == MC2_AICPU_SIMPLE_CTX_PROTOCOL) {
        HCCL_INFO("[Mc2ServerKernel]Simple ctx protocol, ctxNum is 1.");
        return CommKfcDispatcher::Run(&(args[1]), 1U);
    }
    auto* desc = reinterpret_cast<CommKfcParamDesc*>(&descValue);
    AicpuKfcUtils::PrintHcclCommParamDesc(*desc);
    HCCL_INFO("[Mc2ServerKernel]Finish Mc2ServerKernel.");
    return CommKfcDispatcher::Run(&(args[1]), desc->itemNum);
}

extern "C" __attribute__((visibility("default"))) uint32_t Mc2ServerKernel(void* args[])
{
    // Enable only a single-context launch; multi-context attribution is unsupported.
    uint64_t descriptor = args ? reinterpret_cast<uint64_t>(args[0]) : 0;
    const auto* desc = reinterpret_cast<const CommKfcParamDesc*>(&descriptor);
    const bool single = descriptor == MC2_AICPU_SIMPLE_CTX_PROTOCOL || desc->itemNum == 1;
    const auto* ctx = args && single ? static_cast<const OpResCtx*>(args[1]) : nullptr;
    Mc2ServerStageTiming::Start(
        ctx && ctx->res[Mc2ServerDebugConfig::DEBUG_MODE_RES_INDEX] == 4, reinterpret_cast<uintptr_t>(ctx));
    const uint32_t status = Mc2ServerKernelBody(args);
    // Both protocol paths return from CommKfcDispatcher::Run before reporting.
    // The message loop and all three measured intervals have ended here.
    Mc2ServerStageTiming::Finish(status);
    return status;
}
