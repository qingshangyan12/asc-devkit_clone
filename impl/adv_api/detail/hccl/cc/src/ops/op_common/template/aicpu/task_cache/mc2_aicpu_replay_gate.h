/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_AICPU_REPLAY_GATE_H
#define MC2_AICPU_REPLAY_GATE_H

#include <algorithm>
#include <limits>

#include "alg_param.h"
#include "hcomm_primitives.h"
#include "log.h"

namespace mc2_ops_hccl {

struct Mc2AicpuReplayGate {
    bool enabled = false;
    ThreadHandle mainThread = 0;
    ThreadHandle releaseThread = 0;
    uint32_t notifyIdx = 0U;
};

inline HcclResult BuildMc2AicpuReplayGate(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx, Mc2AicpuReplayGate& gate)
{
    gate = Mc2AicpuReplayGate{};
    if (param.opType != HCCL_CMD_ALLREDUCE || param.engine != COMM_ENGINE_AICPU_TS ||
        param.deviceOrderThread == 0) {
        return HCCL_SUCCESS;
    }
    CHK_PRT_RET(
        resCtx.threads.empty(), HCCL_ERROR("[MC2_REPLAY_GATE][Build] main thread is missing."), HCCL_E_PARA);

    const ThreadHandle mainThread = resCtx.threads.front();
    CHK_PRT_RET(
        mainThread == 0 || mainThread == param.deviceOrderThread || resCtx.unfoldThread == param.deviceOrderThread,
        HCCL_ERROR(
            "[MC2_REPLAY_GATE][Build] invalid thread relation, main[0x%llx], release[0x%llx], unfold[0x%llx].",
            mainThread, param.deviceOrderThread, resCtx.unfoldThread),
        HCCL_E_PARA);
    CHK_PRT_RET(
        std::find(resCtx.threads.begin(), resCtx.threads.end(), param.deviceOrderThread) != resCtx.threads.end(),
        HCCL_ERROR("[MC2_REPLAY_GATE][Build] release thread aliases an algorithm thread."), HCCL_E_PARA);

    uint32_t maxNotifyNum = resCtx.notifyNumOnMainThread;
    for (uint32_t notifyNum : resCtx.notifyNumPerThread) {
        maxNotifyNum = std::max(maxNotifyNum, notifyNum);
    }
    CHK_PRT_RET(
        maxNotifyNum == std::numeric_limits<uint32_t>::max(),
        HCCL_ERROR("[MC2_REPLAY_GATE][Build] notify index overflow."), HCCL_E_PARA);

    gate.enabled = true;
    gate.mainThread = mainThread;
    gate.releaseThread = param.deviceOrderThread;
    gate.notifyIdx = maxNotifyNum + 1U;
    return HCCL_SUCCESS;
}

inline HcclResult ArmMc2AicpuReplayGate(const Mc2AicpuReplayGate& gate)
{
    if (!gate.enabled) {
        return HCCL_SUCCESS;
    }
    return static_cast<HcclResult>(HcommThreadNotifyWaitOnThread(gate.mainThread, gate.notifyIdx, CUSTOM_TIMEOUT));
}

inline HcclResult ReleaseMc2AicpuReplayGate(const Mc2AicpuReplayGate& gate)
{
    if (!gate.enabled) {
        return HCCL_SUCCESS;
    }
    return static_cast<HcclResult>(
        HcommThreadNotifyRecordOnThread(gate.releaseThread, gate.mainThread, gate.notifyIdx));
}

} // namespace mc2_ops_hccl

#endif // MC2_AICPU_REPLAY_GATE_H
