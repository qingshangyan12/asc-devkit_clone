/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kfc_sim_world.h"

namespace HcclSim {
namespace CcuSt {

void CcuKfcSimWorld::SetError(const std::string &message)
{
    lastError_ = message;
}

Result CcuKfcSimWorld::DispatchOne(const std::vector<CcuKfcRegistration> &registrations,
    const std::vector<CcuKfcQueueProxy *> &queues, std::vector<RankMemory> &memories)
{
    lastError_.clear();
    if (stopped_ || registrations.size() < 2 || registrations.size() != queues.size() ||
        registrations.size() != memories.size()) {
        SetError("invalid or stopped KFC world");
        return Result::STATE_ERROR;
    }

    bool finalize = true;
    std::vector<RankLaunch> launches(registrations.size());
    for (size_t rank = 0; rank < registrations.size(); ++rank) {
        const CcuKfcRegistration &registration = registrations[rank];
        CcuKfcQueueProxy *queue = queues[rank];
        if (queue == nullptr || registration.rankId != rank || registration.rankSize != registrations.size()) {
            SetError("KFC registration/rank topology mismatch");
            return Result::PARAM_ERROR;
        }
        const uint32_t slot = queue->ConsumerSlot();
        if (queue->State(slot) != CcuKfcSlotState::READY || queue->Memory().startCke[slot] != 1 ||
            queue->Memory().completionCke[slot] != 0) {
            SetError("KFC queue has no ready message");
            return Result::STATE_ERROR;
        }
        const auto &message = queue->Message(slot);
        const bool rankFinalize = message[0] == CCU_KFC_FINALIZE;
        finalize = finalize && rankFinalize;
        if (!rankFinalize && message[0] != registration.opId) {
            SetError("KFC message opId does not match registered operation");
            return Result::CONTRACT_ERROR;
        }
        launches[rank].kernel = registration.collectiveKernel;
        launches[rank].memory = &memories[rank];
    }

    if (!finalize) {
        for (CcuKfcQueueProxy *queue : queues) {
            if (queue->Message(queue->ConsumerSlot())[0] == CCU_KFC_FINALIZE) {
                SetError("KFC finalize must be collective across ranks");
                return Result::CONTRACT_ERROR;
            }
        }
        for (size_t rank = 0; rank < registrations.size(); ++rank) {
            const CcuKfcRegistration &registration = registrations[rank];
            const CcuKfcMessage &message = queues[rank]->Message(queues[rank]->ConsumerSlot());
            const Result buildResult = registration.taskArgBuilder(
                message, registration.startup, launches[rank].taskArgs);
            if (buildResult != Result::SUCCESS) {
                SetError("KFC operation adapter rejected the message");
                return buildResult;
            }
        }
        const Result launchResult = algorithmWorld_.Launch(launches);
        if (launchResult != Result::SUCCESS) {
            SetError(algorithmWorld_.LastError());
            return launchResult;
        }
    }

    for (CcuKfcQueueProxy *queue : queues) {
        const uint32_t slot = queue->ConsumerSlot();
        const Result transition = finalize ? queue->MarkFinalized(slot) : queue->MarkCompleted(slot);
        if (transition != Result::SUCCESS) {
            SetError("KFC queue completion transition failed");
            return transition;
        }
    }
    stopped_ = finalize;
    return Result::SUCCESS;
}

const RuntimeWorld &CcuKfcSimWorld::AlgorithmWorld() const { return algorithmWorld_; }
const std::string &CcuKfcSimWorld::LastError() const { return lastError_; }
bool CcuKfcSimWorld::Stopped() const { return stopped_; }

} // namespace CcuSt
} // namespace HcclSim
