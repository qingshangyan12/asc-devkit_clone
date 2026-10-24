/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "../../../../common/mc2_perf_probe.h"
#include <cstdint>

#include "aicpu_kfc/decoupler/comm_kfc_dispatcher.h"
#include "common/aicpu_kfc_def.h"
#include "common/aicpu_kfc_utils.h"
#include "hccl_msg.h"
#include "log.h"

using namespace HcclApi;

// Package identity is emitted once at module load, outside the hot kernel loop.
__attribute__((constructor)) static void Mc2AbcdPackageIdentity()
{
    HCCL_RUN_INFO("[MC2_ABCD_BUILD] variant=C base=9dfffb2ba02d3f1fb80e1f4e6b7fc4e4d1a92158 protocol=1");
}

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
    Mc2PerfProbe::Start();
    const uint32_t status = Mc2ServerKernelBody(args);
    Mc2PerfProbe::Finish(status);
    return status;
}
