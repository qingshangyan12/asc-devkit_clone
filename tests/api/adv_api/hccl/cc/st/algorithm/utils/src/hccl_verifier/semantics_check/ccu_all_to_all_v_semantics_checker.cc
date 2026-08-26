/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_all_to_all_v_semantics_checker.h"

#include <algorithm>
#include <map>
#include <set>
#include <sstream>

namespace HcclSim {
namespace CcuSt {
namespace {

CheckResult Failure(const std::string &message, Result result = Result::CONTRACT_ERROR)
{
    return CheckResult{result, message};
}

size_t Count(const Program &program, OpCode code)
{
    return static_cast<size_t>(std::count_if(program.operations.begin(), program.operations.end(),
        [code](const Operation &operation) { return operation.code == code; }));
}

} // namespace

CheckResult CcuAllToAllVSemanticsChecker::CheckRegistration(
    const Program &program, const AllToAllVKernelConfig &config) const
{
    CheckResult common = CcuProgramChecker().Check(program);
    if (!common.Ok()) {
        return common;
    }
    if (config.rankSize < 2 || config.rankId >= config.rankSize ||
        config.channels.size() != config.rankSize - 1 || program.rankId != config.rankId ||
        program.rankSize != config.rankSize || program.channels != config.channels) {
        return Failure("AllToAllV compiled rank metadata is invalid");
    }

    std::set<uint32_t> peers;
    for (ChannelHandle channel : config.channels) {
        const uint32_t peer = ChannelRemoteRank(channel);
        if (ChannelLocalRank(channel) != config.rankId || peer >= config.rankSize || peer == config.rankId ||
            !peers.insert(peer).second || program.channelVariables.count({peer, 0}) != 1 ||
            program.channelVariables.count({peer, 1}) != 1) {
            return Failure("AllToAllV channels are not a unique full mesh with output/token variables");
        }
    }
    if (program.arguments.size() != ALL_TO_ALL_V_TASK_ARG_COUNT) {
        return Failure("AllToAllV must bind 10 dynamic task arguments");
    }

    const size_t channelCount = config.channels.size();
    const size_t dataEventCount = (config.rankSize + 15U) / 16U;
    if (Count(program, OpCode::LOAD_MEMORY) != config.rankSize * ALL_TO_ALL_V_EXT_FIELD_COUNT ||
        Count(program, OpCode::WRITE_VARIABLE_NOTIFY) != 2 * channelCount ||
        Count(program, OpCode::NOTIFY_RECORD) != channelCount ||
        Count(program, OpCode::NOTIFY_WAIT) != 2 * channelCount ||
        Count(program, OpCode::WRITE) != channelCount || Count(program, OpCode::LOCAL_COPY) != 0 ||
        Count(program, OpCode::GROUP_COPY) != 1 || Count(program, OpCode::EVENT_RECORD) != config.rankSize ||
        Count(program, OpCode::EVENT_WAIT) != dataEventCount) {
        return Failure("AllToAllV primitive sequence does not match the current Mesh1D kernel");
    }

    uint64_t expectedOffset = 0;
    for (const Operation &operation : program.operations) {
        if (operation.code == OpCode::LOAD_MEMORY) {
            if (operation.byteOffset != expectedOffset) {
                return Failure("AllToAllV parameter table Load is not contiguous");
            }
            expectedOffset += sizeof(uint64_t);
        }
    }

    constexpr uint32_t preSyncCkeIndex = 0;
    constexpr uint32_t postSyncCkeIndex = 1;
    constexpr uint16_t outputBit = 1U << 0;
    constexpr uint16_t tokenBit = 1U << 1;
    constexpr uint16_t preSyncBits = outputBit | tokenBit;
    constexpr uint16_t postSyncBit = 1U << 5;
    size_t outputPublish = 0;
    size_t tokenPublish = 0;
    size_t preWait = 0;
    size_t postRecord = 0;
    size_t postWait = 0;
    using EventKey = std::pair<ResourceHandle, uint16_t>;
    std::set<EventKey> writeEvents;
    std::set<EventKey> recordEvents;
    std::map<ResourceHandle, uint16_t> waitMasks;
    size_t lastWriteIndex = 0;
    size_t firstWaitIndex = program.operations.size();
    for (size_t operationIndex = 0; operationIndex < program.operations.size(); ++operationIndex) {
        const Operation &operation = program.operations[operationIndex];
        if (operation.code == OpCode::WRITE_VARIABLE_NOTIFY) {
            outputPublish += operation.index == 0 && operation.immediate == preSyncCkeIndex &&
                operation.mask == outputBit;
            tokenPublish += operation.index == 1 && operation.immediate == preSyncCkeIndex &&
                operation.mask == tokenBit;
        } else if (operation.code == OpCode::NOTIFY_WAIT) {
            preWait += operation.index == preSyncCkeIndex && operation.mask == preSyncBits;
            postWait += operation.index == postSyncCkeIndex && operation.mask == postSyncBit;
        } else if (operation.code == OpCode::NOTIFY_RECORD) {
            postRecord += operation.index == postSyncCkeIndex && operation.mask == postSyncBit;
        } else if (operation.code == OpCode::WRITE) {
            if (operation.mask == 0 || (operation.mask & (operation.mask - 1)) != 0 ||
                program.events.count(operation.immediate) != 1 ||
                !writeEvents.insert({operation.immediate, operation.mask}).second) {
                return Failure("AllToAllV Write event contract is invalid");
            }
            lastWriteIndex = operationIndex;
        } else if (operation.code == OpCode::EVENT_RECORD) {
            if (operation.mask == 0 || (operation.mask & (operation.mask - 1)) != 0 ||
                !recordEvents.insert({operation.dst, operation.mask}).second) {
                return Failure("AllToAllV EventRecord contract is invalid");
            }
        } else if (operation.code == OpCode::EVENT_WAIT) {
            if (operation.mask == 0 || waitMasks.count(operation.src0) != 0) {
                return Failure("AllToAllV grouped EventWait contract is invalid");
            }
            waitMasks[operation.src0] = operation.mask;
            firstWaitIndex = std::min(firstWaitIndex, operationIndex);
        }
    }
    if (outputPublish != channelCount || tokenPublish != channelCount || preWait != channelCount ||
        postRecord != channelCount || postWait != channelCount) {
        return Failure("AllToAllV pre/post synchronization contract is invalid");
    }
    if (writeEvents.size() != channelCount || recordEvents.size() != config.rankSize ||
        waitMasks.size() != dataEventCount || lastWriteIndex >= firstWaitIndex) {
        return Failure("AllToAllV data operations are not submitted before grouped completion waits");
    }
    for (const EventKey &event : writeEvents) {
        if (recordEvents.count(event) != 1) {
            return Failure("AllToAllV zero-length completion path does not match its Write event");
        }
    }
    std::map<ResourceHandle, uint16_t> recordMasks;
    for (const EventKey &event : recordEvents) {
        recordMasks[event.first] |= event.second;
    }
    if (recordMasks != waitMasks) {
        return Failure("AllToAllV grouped EventWait masks do not cover every rank");
    }

    const GroupCopyDescriptor &groupCopy = program.groupCopies.front();
    const std::array<ResourceHandle, 4> expectedGroupSize = {{program.arguments[6].variable,
        program.arguments[7].variable, program.arguments[8].variable, program.arguments[9].variable}};
    if (groupCopy.groupSize != expectedGroupSize || groupCopy.destination == 0 || groupCopy.source == 0 ||
        groupCopy.length != expectedGroupSize[3]) {
        return Failure("AllToAllV GroupCopy does not bind the four goSize task arguments");
    }
    return {};
}

CheckResult CcuAllToAllVSemanticsChecker::CheckTaskArgs(const Program &program,
    const std::vector<uint64_t> &taskArgs, const AllToAllVLaunchConfig &config) const
{
    CheckResult common = CcuTaskArgChecker().Check(program, taskArgs);
    if (!common.Ok()) {
        return common;
    }
    if (config.rankId >= config.peers.size() || config.parameterAddress == 0 ||
        taskArgs != PrepareAllToAllVTaskArgs(config)) {
        return Failure("AllToAllV task argument value or rank binding is invalid");
    }
    return {};
}

CheckResult CcuAllToAllVSemanticsChecker::CaptureRuntimeExpectation(
    const std::vector<RankMemory> &memories, const std::vector<AllToAllVLaunchConfig> &configs,
    AllToAllVRuntimeExpectation &expectation) const
{
    const size_t rankSize = memories.size();
    if (rankSize < 2 || configs.size() != rankSize) {
        return Failure("AllToAllV runtime rank count is invalid", Result::PARAM_ERROR);
    }
    expectation.outputs.resize(rankSize);
    for (size_t rank = 0; rank < rankSize; ++rank) {
        if (configs[rank].rankId != rank || configs[rank].peers.size() != rankSize) {
            return Failure("AllToAllV runtime config matrix is invalid", Result::PARAM_ERROR);
        }
        expectation.outputs[rank] = memories[rank].output;
    }
    for (size_t sourceRank = 0; sourceRank < rankSize; ++sourceRank) {
        const std::vector<uint8_t> &source = memories[sourceRank].inPlace ?
            memories[sourceRank].output : memories[sourceRank].input;
        for (size_t destinationRank = 0; destinationRank < rankSize; ++destinationRank) {
            const AllToAllVPeerParam &send = configs[sourceRank].peers[destinationRank];
            const uint64_t sourceOffset = configs[sourceRank].sourceBaseOffset + send.sendOffset;
            const uint64_t destinationOffset = configs[destinationRank].destinationBaseOffset +
                configs[destinationRank].peers[sourceRank].recvOffset;
            if (sourceOffset > source.size() || send.sendSize > source.size() - sourceOffset ||
                destinationOffset > expectation.outputs[destinationRank].size() ||
                send.sendSize > expectation.outputs[destinationRank].size() - destinationOffset) {
                return Failure("AllToAllV expected transfer is out of range", Result::OUT_OF_RANGE);
            }
            std::copy_n(source.begin() + static_cast<size_t>(sourceOffset), static_cast<size_t>(send.sendSize),
                expectation.outputs[destinationRank].begin() + static_cast<size_t>(destinationOffset));
        }
    }
    return {};
}

CheckResult CcuAllToAllVSemanticsChecker::CheckRuntime(const AllToAllVRuntimeExpectation &expectation,
    const std::vector<RankMemory> &memories, const RuntimeWorld &world) const
{
    CheckResult common = CcuRuntimeChecker().CheckCompleted(world, memories.size());
    if (!common.Ok()) {
        return common;
    }
    if (expectation.outputs.size() != memories.size()) {
        return Failure("AllToAllV expected output rank count is invalid");
    }
    for (size_t rank = 0; rank < memories.size(); ++rank) {
        if (memories[rank].output != expectation.outputs[rank]) {
            const auto mismatch = std::mismatch(memories[rank].output.begin(), memories[rank].output.end(),
                expectation.outputs[rank].begin());
            std::ostringstream message;
            message << "AllToAllV output mismatch at rank " << rank;
            if (mismatch.first != memories[rank].output.end()) {
                message << ", byte " << std::distance(memories[rank].output.begin(), mismatch.first);
            }
            return Failure(message.str());
        }
    }
    return {};
}

} // namespace CcuSt
} // namespace HcclSim
