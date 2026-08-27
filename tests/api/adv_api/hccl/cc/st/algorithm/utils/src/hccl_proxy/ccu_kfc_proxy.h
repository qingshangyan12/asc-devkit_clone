/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_CCU_KFC_PROXY_H
#define HCCL_CCU_KFC_PROXY_H

#include "ccu_kernel_proxy.h"

#include <array>
#include <functional>

namespace HcclSim {
namespace CcuSt {

constexpr uint32_t CCU_KFC_QUEUE_DEPTH = 64;
constexpr uint32_t CCU_KFC_WORDS_PER_SLOT = 64;
constexpr uint32_t CCU_KFC_WORDS_PER_DIE = 32;
constexpr uint64_t CCU_KFC_FINALIZE = UINT64_MAX;
constexpr uint64_t CCU_KFC_COLLECTIVE_MASK = 0x7f;
constexpr uint64_t CCU_KFC_ALL_TO_ALL_V = 8;

inline uint64_t DecodeKfcCollective(uint64_t opId)
{
    return opId & CCU_KFC_COLLECTIVE_MASK;
}

struct CcuKfcMemory {
    std::array<std::array<uint64_t, CCU_KFC_WORDS_PER_SLOT>, CCU_KFC_QUEUE_DEPTH> xn{};
    std::array<uint64_t, CCU_KFC_QUEUE_DEPTH> startCke{};
    std::array<uint64_t, CCU_KFC_QUEUE_DEPTH> completionCke{};

    uint64_t XnAddress() const;
    uint64_t CkeAddress() const;
};

struct CcuKfcStartupArgs {
    uint64_t xnAddr{0};
    uint64_t ckeAddr{0};
    uint64_t dieNum{1};
    uint64_t missionNum{1};
    uint64_t missionIndex{0};
    uint64_t token{0};

    std::vector<uint64_t> Pack() const;
};

using CcuKfcMessage = std::array<uint64_t, CCU_KFC_WORDS_PER_SLOT>;
using CcuKfcTaskArgBuilder = std::function<Result(const CcuKfcMessage &,
    const CcuKfcStartupArgs &, std::vector<uint64_t> &)>;

struct CcuKfcRegistration {
    InstanceHandle instance{0};
    KernelHandle serverKernel{0};
    KernelHandle collectiveKernel{0};
    uint32_t rankId{0};
    uint32_t rankSize{0};
    uint64_t opId{0};
    uint64_t collective{CCU_KFC_ALL_TO_ALL_V};
    std::vector<ChannelHandle> channels;
    CcuKfcStartupArgs startup;
    CcuKfcTaskArgBuilder taskArgBuilder;
};

Result MakeKfcRegistration(InstanceHandle instance, KernelHandle serverKernel,
    KernelHandle collectiveKernel, uint64_t opId, const CcuKfcStartupArgs &startup,
    const CcuKfcTaskArgBuilder &taskArgBuilder, CcuKfcRegistration &registration);

enum class CcuKfcSlotState {
    FREE,
    READY,
    COMPLETED,
    FINALIZED
};

class CcuKfcQueueProxy {
public:
    explicit CcuKfcQueueProxy(CcuKfcMemory &memory);

    Result Commit(const CcuKfcMessage &message);
    Result Finalize();
    Result MarkCompleted(uint32_t slot);
    Result MarkFinalized(uint32_t slot);
    Result Wait();

    uint32_t ProducerSlot() const;
    uint32_t ConsumerSlot() const;
    uint32_t WaitSlot() const;
    CcuKfcSlotState State(uint32_t slot) const;
    const CcuKfcMessage &Message(uint32_t slot) const;
    const CcuKfcMemory &Memory() const;

private:
    CcuKfcMemory &memory_;
    std::array<CcuKfcSlotState, CCU_KFC_QUEUE_DEPTH> states_{};
    uint32_t producerSlot_{0};
    uint32_t consumerSlot_{0};
    uint32_t waitSlot_{0};
};

} // namespace CcuSt
} // namespace HcclSim

#endif // HCCL_CCU_KFC_PROXY_H
