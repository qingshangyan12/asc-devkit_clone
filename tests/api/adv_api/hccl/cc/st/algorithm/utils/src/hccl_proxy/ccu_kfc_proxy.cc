/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kfc_proxy.h"

namespace HcclSim {
namespace CcuSt {

uint64_t CcuKfcMemory::XnAddress() const
{
    return reinterpret_cast<uint64_t>(const_cast<uint64_t *>(xn[0].data()));
}

uint64_t CcuKfcMemory::CkeAddress() const
{
    return reinterpret_cast<uint64_t>(const_cast<uint64_t *>(startCke.data()));
}

std::vector<uint64_t> CcuKfcStartupArgs::Pack() const
{
    return {xnAddr, ckeAddr, dieNum, missionNum, missionIndex, token};
}

Result MakeKfcRegistration(InstanceHandle instance, KernelHandle serverKernel,
    KernelHandle collectiveKernel, uint64_t opId, const CcuKfcStartupArgs &startup,
    const CcuKfcTaskArgBuilder &taskArgBuilder, CcuKfcRegistration &registration)
{
    const Program *server = RegisterManager::Global().GetProgram(serverKernel);
    const Program *collective = RegisterManager::Global().GetProgram(collectiveKernel);
    if (instance == 0 || server == nullptr || collective == nullptr || opId == CCU_KFC_FINALIZE ||
        DecodeKfcCollective(opId) == 0 || !taskArgBuilder || startup.xnAddr == 0 || startup.ckeAddr == 0 ||
        startup.dieNum != 1 || startup.missionNum != 1 || startup.missionIndex != 0 || startup.token == 0 ||
        server->rankId != collective->rankId || server->rankSize != collective->rankSize ||
        server->channels != collective->channels) {
        return Result::PARAM_ERROR;
    }
    registration.instance = instance;
    registration.serverKernel = serverKernel;
    registration.collectiveKernel = collectiveKernel;
    registration.rankId = collective->rankId;
    registration.rankSize = collective->rankSize;
    registration.opId = opId;
    registration.collective = DecodeKfcCollective(opId);
    registration.channels = collective->channels;
    registration.startup = startup;
    registration.taskArgBuilder = taskArgBuilder;
    return Result::SUCCESS;
}

CcuKfcQueueProxy::CcuKfcQueueProxy(CcuKfcMemory &memory) : memory_(memory)
{
    states_.fill(CcuKfcSlotState::FREE);
}

Result CcuKfcQueueProxy::Commit(const CcuKfcMessage &message)
{
    if (message[0] == CCU_KFC_FINALIZE) {
        return Result::PARAM_ERROR;
    }
    if (states_[producerSlot_] != CcuKfcSlotState::FREE || memory_.startCke[producerSlot_] != 0 ||
        memory_.completionCke[producerSlot_] != 0) {
        return Result::STATE_ERROR;
    }
    memory_.xn[producerSlot_] = message;
    memory_.startCke[producerSlot_] = 1;
    states_[producerSlot_] = CcuKfcSlotState::READY;
    producerSlot_ = (producerSlot_ + 1U) % CCU_KFC_QUEUE_DEPTH;
    return Result::SUCCESS;
}

Result CcuKfcQueueProxy::Finalize()
{
    CcuKfcMessage message{};
    message[0] = CCU_KFC_FINALIZE;
    if (states_[producerSlot_] != CcuKfcSlotState::FREE || memory_.startCke[producerSlot_] != 0 ||
        memory_.completionCke[producerSlot_] != 0) {
        return Result::STATE_ERROR;
    }
    memory_.xn[producerSlot_] = message;
    memory_.startCke[producerSlot_] = 1;
    states_[producerSlot_] = CcuKfcSlotState::READY;
    producerSlot_ = (producerSlot_ + 1U) % CCU_KFC_QUEUE_DEPTH;
    return Result::SUCCESS;
}

Result CcuKfcQueueProxy::MarkCompleted(uint32_t slot)
{
    if (slot != consumerSlot_ || states_[slot] != CcuKfcSlotState::READY ||
        memory_.startCke[slot] != 1 || memory_.completionCke[slot] != 0) {
        return Result::STATE_ERROR;
    }
    memory_.startCke[slot] = 0;
    memory_.completionCke[slot] = 1;
    states_[slot] = CcuKfcSlotState::COMPLETED;
    consumerSlot_ = (consumerSlot_ + 1U) % CCU_KFC_QUEUE_DEPTH;
    return Result::SUCCESS;
}

Result CcuKfcQueueProxy::MarkFinalized(uint32_t slot)
{
    if (slot != consumerSlot_ || states_[slot] != CcuKfcSlotState::READY ||
        memory_.startCke[slot] != 1 || memory_.completionCke[slot] != 0 ||
        memory_.xn[slot][0] != CCU_KFC_FINALIZE) {
        return Result::STATE_ERROR;
    }
    memory_.startCke[slot] = 0;
    states_[slot] = CcuKfcSlotState::FINALIZED;
    consumerSlot_ = (consumerSlot_ + 1U) % CCU_KFC_QUEUE_DEPTH;
    return Result::SUCCESS;
}

Result CcuKfcQueueProxy::Wait()
{
    if (states_[waitSlot_] != CcuKfcSlotState::COMPLETED || memory_.completionCke[waitSlot_] != 1) {
        return Result::STATE_ERROR;
    }
    memory_.completionCke[waitSlot_] = 0;
    memory_.xn[waitSlot_].fill(0);
    states_[waitSlot_] = CcuKfcSlotState::FREE;
    waitSlot_ = (waitSlot_ + 1U) % CCU_KFC_QUEUE_DEPTH;
    return Result::SUCCESS;
}

uint32_t CcuKfcQueueProxy::ProducerSlot() const { return producerSlot_; }
uint32_t CcuKfcQueueProxy::ConsumerSlot() const { return consumerSlot_; }
uint32_t CcuKfcQueueProxy::WaitSlot() const { return waitSlot_; }
CcuKfcSlotState CcuKfcQueueProxy::State(uint32_t slot) const { return states_.at(slot); }
const CcuKfcMessage &CcuKfcQueueProxy::Message(uint32_t slot) const
{
    return memory_.xn.at(slot);
}
const CcuKfcMemory &CcuKfcQueueProxy::Memory() const { return memory_; }

} // namespace CcuSt
} // namespace HcclSim
