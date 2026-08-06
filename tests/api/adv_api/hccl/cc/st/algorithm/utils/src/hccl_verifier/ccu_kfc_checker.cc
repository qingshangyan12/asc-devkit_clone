/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kfc_checker.h"

#include <algorithm>
#include <set>

namespace HcclSim {
namespace CcuSt {
namespace {
CheckResult Failure(const std::string &message)
{
    return CheckResult{Result::CONTRACT_ERROR, message};
}
} // namespace

CheckResult CcuKfcChecker::CheckRegistration(const CcuKfcRegistration &registration,
    const CcuKfcQueueProxy &queue) const
{
    const Program *server = RegisterManager::Global().GetProgram(registration.serverKernel);
    const Program *collective = RegisterManager::Global().GetProgram(registration.collectiveKernel);
    if (registration.instance == 0 || registration.serverKernel == registration.collectiveKernel ||
        server == nullptr || collective == nullptr || registration.rankId != collective->rankId ||
        registration.rankSize != collective->rankSize || registration.channels != collective->channels ||
        server->rankId != collective->rankId || server->rankSize != collective->rankSize ||
        server->channels != collective->channels || registration.opId == CCU_KFC_FINALIZE ||
        DecodeKfcCollective(registration.opId) != registration.collective || !registration.taskArgBuilder) {
        return Failure("KFC registration does not match the captured server and collective programs");
    }
    const CcuKfcStartupArgs &args = registration.startup;
    if (args.Pack().size() != 6 || args.xnAddr != queue.Memory().XnAddress() ||
        args.ckeAddr != queue.Memory().CkeAddress() || args.dieNum != 1 || args.missionNum != 1 ||
        args.missionIndex != 0 || args.token == 0) {
        return Failure("KFC six startup arguments do not match the queue resources");
    }
    return {};
}

CheckResult CcuKfcChecker::CheckContext(const std::vector<CcuKfcRegistration> &registrations) const
{
    if (registrations.size() < 2) {
        return Failure("KFC context has too few ranks");
    }
    std::set<uint32_t> ranks;
    const uint64_t opId = registrations.front().opId;
    for (const CcuKfcRegistration &registration : registrations) {
        if (registration.opId != opId || registration.collective != registrations.front().collective ||
            registration.rankSize != registrations.size() || !ranks.insert(registration.rankId).second) {
            return Failure("KFC context has inconsistent operation or rank metadata");
        }
    }
    return {};
}

CheckResult CcuKfcChecker::CheckServerProgram(const Program &program, uint64_t opId) const
{
    CheckResult common = CcuProgramChecker().Check(program);
    if (!common.Ok()) {
        return common;
    }
    if (program.arguments.size() != 6) {
        return Failure("KFC server must bind six startup arguments");
    }

    size_t loadCount = 0;
    size_t storeCount = 0;
    size_t whileCount = 0;
    bool hasFinalize = false;
    bool hasDispatch = false;
    bool hasWrap = false;
    size_t queueStrideConstants = 0;
    std::map<ResourceHandle, std::set<uint64_t>> loadOffsets;
    for (const Operation &operation : program.operations) {
        if (operation.code == OpCode::LOAD_MEMORY) {
            ++loadCount;
            loadOffsets[operation.src0].insert(operation.byteOffset);
        } else if (operation.code == OpCode::STORE_MEMORY) {
            ++storeCount;
        } else if (operation.code == OpCode::WHILE_BEGIN) {
            ++whileCount;
        } else if (operation.code == OpCode::IF_BEGIN) {
            hasFinalize = hasFinalize || (operation.equal && operation.immediate == CCU_KFC_FINALIZE);
            hasDispatch = hasDispatch || (operation.equal && operation.immediate == opId);
            hasWrap = hasWrap || (operation.equal && operation.immediate == CCU_KFC_QUEUE_DEPTH);
        } else if (operation.code == OpCode::ASSIGN_IMM) {
            queueStrideConstants +=
                operation.immediate == CCU_KFC_WORDS_PER_SLOT * sizeof(uint64_t);
        }
    }
    bool hasContiguousMessageLoad = false;
    for (const auto &entry : loadOffsets) {
        bool contiguous = true;
        for (uint32_t index = 0; index < CCU_KFC_WORDS_PER_DIE; ++index) {
            contiguous = contiguous && entry.second.count(index * sizeof(uint64_t)) == 1;
        }
        hasContiguousMessageLoad = hasContiguousMessageLoad || contiguous;
    }
    if (loadCount < CCU_KFC_WORDS_PER_DIE + 1U || storeCount != 2U || whileCount < 2U ||
        !hasFinalize || !hasDispatch || !hasWrap || queueStrideConstants < 2U || !hasContiguousMessageLoad) {
        return Failure("KFC server queue, dispatch, finalize, or 64-slot IR contract is incomplete");
    }
    return {};
}

CheckResult CcuKfcChecker::CheckQueue(const CcuKfcQueueProxy &queue) const
{
    for (uint32_t slot = 0; slot < CCU_KFC_QUEUE_DEPTH; ++slot) {
        const CcuKfcSlotState state = queue.State(slot);
        const uint64_t start = queue.Memory().startCke[slot];
        const uint64_t completion = queue.Memory().completionCke[slot];
        if ((state == CcuKfcSlotState::FREE && (start != 0 || completion != 0)) ||
            (state == CcuKfcSlotState::READY && (start != 1 || completion != 0)) ||
            (state == CcuKfcSlotState::COMPLETED && (start != 0 || completion != 1)) ||
            (state == CcuKfcSlotState::FINALIZED && (start != 0 || completion != 0))) {
            return Failure("KFC slot state and CKE signals are inconsistent");
        }
    }
    return {};
}

} // namespace CcuSt
} // namespace HcclSim
