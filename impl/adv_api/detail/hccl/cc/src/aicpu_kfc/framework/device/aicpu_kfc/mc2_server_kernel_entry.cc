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

#include "aicpu_kfc/decoupler/comm_kfc_dispatcher.h"
#include "common/aicpu_kfc_def.h"
#include "common/aicpu_kfc_utils.h"
#include "common/aicpu_timing_recorder.h"
#include "hccl_msg.h"
#include "log.h"

using namespace HcclApi;

#ifndef MC2_AICPU_TIMING_ENABLE
#define MC2_AICPU_TIMING_ENABLE 0
#endif

#if MC2_AICPU_TIMING_ENABLE
namespace {
constexpr char A5_HCCL_KERNEL_SCENARIO[] = "a5_hccl_kernel";
} // namespace
#endif

extern "C" __attribute__((visibility("default"))) uint32_t Mc2ServerKernel(void* args[])
{
#if MC2_AICPU_TIMING_ENABLE
    Hccl::AicpuTimingRecorder timingRecorder(A5_HCCL_KERNEL_SCENARIO);
    (void)timingRecorder.Record(Hccl::AicpuTimingRecorder::Stage::KERNEL_ENTRY);
#endif
    if (args == nullptr) {
        HCCL_ERROR("args is null.");
        return HCCL_E_PARA;
    }

    constexpr int DESC_POS = 0;
    uint64_t descValue = reinterpret_cast<uint64_t>(args[DESC_POS]);
    auto* desc = reinterpret_cast<CommKfcParamDesc*>(&descValue);
    AicpuKfcUtils::PrintHcclCommParamDesc(*desc);
#if MC2_AICPU_TIMING_ENABLE
    return CommKfcDispatcher::Run(&(args[1]), desc->itemNum, timingRecorder);
#else
    return CommKfcDispatcher::Run(&(args[1]), desc->itemNum);
#endif
}
