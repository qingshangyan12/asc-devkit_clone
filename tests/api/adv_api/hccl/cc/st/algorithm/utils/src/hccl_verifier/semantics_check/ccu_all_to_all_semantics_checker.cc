/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_all_to_all_semantics_checker.h"

#include <algorithm>
#include <set>
#include <sstream>

namespace HcclSim {
namespace CcuSt {
namespace {

constexpr uint16_t OUTPUT_NOTIFY_BIT = 1U << 1U;
constexpr uint16_t TOKEN_NOTIFY_BIT = 1U << 2U;
constexpr uint16_t PRE_SYNC_MASK = OUTPUT_NOTIFY_BIT | TOKEN_NOTIFY_BIT;
constexpr uint16_t POST_SYNC_BIT = 1U << 5U;

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

CheckResult CcuAllToAllSemanticsChecker::CheckRegistration(
    const Program &program, const AllToAllKernelConfig &config) const
{
    CheckResult common = CcuProgramChecker().Check(program);
    if (!common.Ok()) {
        return common;
    }
    if (config.rankSize < 2 || config.rankId >= config.rankSize ||
        config.channels.size() != config.rankSize - 1 || program.rankId != config.rankId ||
        program.rankSize != config.rankSize || program.channels != config.channels) {
        return Failure("AllToAll compiled rank metadata is invalid");
    }

    std::set<uint32_t> peers;
    for (ChannelHandle channel : config.channels) {
        const uint32_t peer = ChannelRemoteRank(channel);
        if (ChannelLocalRank(channel) != config.rankId || peer >= config.rankSize || peer == config.rankId ||
            !peers.insert(peer).second || program.channelVariables.count({peer, 0}) != 1 ||
            program.channelVariables.count({peer, 1}) != 1 ||
            program.channelVariables.count({peer, 2}) != 1) {
            return Failure("AllToAll channels are not a unique full mesh with input/output/token variables");
        }
    }
    if (program.arguments.size() != ALL_TO_ALL_TASK_ARG_COUNT) {
        return Failure("AllToAll must bind 11 dynamic task arguments");
    }

    const size_t channelCount = config.channels.size();
    if (Count(program, OpCode::WRITE_VARIABLE_NOTIFY) != 2 * channelCount ||
        Count(program, OpCode::NOTIFY_WAIT) != 2 * channelCount ||
        Count(program, OpCode::NOTIFY_RECORD) != channelCount ||
        Count(program, OpCode::WRITE) != channelCount ||
        Count(program, OpCode::LOCAL_COPY) != 1 ||
        Count(program, OpCode::EVENT_WAIT) != 1) {
        return Failure("AllToAll primitive sequence does not match the current Mesh1D kernel");
    }

    size_t outputPublish = 0;
    size_t tokenPublish = 0;
    size_t preSyncWait = 0;
    size_t postRecord = 0;
    size_t postWait = 0;
    for (const Operation &operation : program.operations) {
        if (operation.code == OpCode::WRITE_VARIABLE_NOTIFY) {
            if (operation.index == 1 && operation.immediate == 0) {
                if (operation.mask != OUTPUT_NOTIFY_BIT) {
                    return Failure("AllToAll output publish mask is invalid");
                }
                ++outputPublish;
            } else if (operation.index == 2 && operation.immediate == 0) {
                if (operation.mask != TOKEN_NOTIFY_BIT) {
                    return Failure("AllToAll token publish mask is invalid");
                }
                ++tokenPublish;
            } else {
                return Failure("AllToAll pre-sync publish contract is invalid");
            }
        } else if (operation.code == OpCode::NOTIFY_WAIT) {
            if (operation.index == 0) {
                if (operation.mask != PRE_SYNC_MASK) {
                    return Failure("AllToAll pre-sync wait mask is invalid");
                }
                ++preSyncWait;
            } else if (operation.index == 1) {
                if (operation.mask != POST_SYNC_BIT) {
                    return Failure("AllToAll post-sync wait mask is invalid");
                }
                ++postWait;
            } else {
                return Failure("AllToAll notify wait index is invalid");
            }
        } else if (operation.code == OpCode::NOTIFY_RECORD) {
            if (operation.index != 1 || operation.mask != POST_SYNC_BIT) {
                return Failure("AllToAll post-sync record contract is invalid");
            }
            ++postRecord;
        } else if ((operation.code == OpCode::WRITE || operation.code == OpCode::LOCAL_COPY) &&
            program.events.count(operation.immediate) != 1) {
            return Failure("AllToAll data operation event contract is invalid");
        }
    }
    if (outputPublish != channelCount || tokenPublish != channelCount || preSyncWait != channelCount ||
        postRecord != channelCount || postWait != channelCount) {
        return Failure("AllToAll pre/post synchronization contract is invalid");
    }
    return {};
}

CheckResult CcuAllToAllSemanticsChecker::CheckTaskArgs(const Program &program,
    const std::vector<uint64_t> &taskArgs, const AllToAllLaunchConfig &config) const
{
    CheckResult common = CcuTaskArgChecker().Check(program, taskArgs);
    if (!common.Ok()) {
        return common;
    }
    if (taskArgs != PrepareAllToAllTaskArgs(config)) {
        return Failure("AllToAll task argument value or rank binding is invalid");
    }
    return {};
}

CheckResult CcuAllToAllSemanticsChecker::CaptureRuntimeExpectation(
    const std::vector<RankMemory> &memories, const std::vector<AllToAllLaunchConfig> &configs,
    AllToAllRuntimeExpectation &expectation) const
{
    const size_t rankSize = memories.size();
    if (rankSize < 2 || configs.size() != rankSize) {
        return Failure("AllToAll runtime rank count is invalid", Result::PARAM_ERROR);
    }
    expectation.outputs.resize(rankSize);
    for (size_t rank = 0; rank < rankSize; ++rank) {
        if (configs[rank].rankId != rank) {
            return Failure("AllToAll runtime config matrix is invalid", Result::PARAM_ERROR);
        }
        expectation.outputs[rank] = memories[rank].output;
    }
    for (size_t sourceRank = 0; sourceRank < rankSize; ++sourceRank) {
        const std::vector<uint8_t> &source = memories[sourceRank].input;
        const uint64_t sliceSize = configs[sourceRank].sliceSize;
        for (size_t destinationRank = 0; destinationRank < rankSize; ++destinationRank) {
            const uint64_t sourceOffset = configs[sourceRank].srcOffset +
                destinationRank * configs[sourceRank].srcStride;
            const uint64_t destinationOffset = configs[sourceRank].dstOffset;
            if (sourceOffset > source.size() || sliceSize > source.size() - sourceOffset ||
                destinationOffset > expectation.outputs[destinationRank].size() ||
                sliceSize > expectation.outputs[destinationRank].size() - destinationOffset) {
                return Failure("AllToAll expected transfer is out of range", Result::OUT_OF_RANGE);
            }
            std::copy_n(source.begin() + static_cast<size_t>(sourceOffset), static_cast<size_t>(sliceSize),
                expectation.outputs[destinationRank].begin() + static_cast<size_t>(destinationOffset));
        }
    }
    return {};
}

CheckResult CcuAllToAllSemanticsChecker::CheckRuntime(const AllToAllRuntimeExpectation &expectation,
    const std::vector<RankMemory> &memories, const RuntimeWorld &world) const
{
    CheckResult common = CcuRuntimeChecker().CheckCompleted(world, memories.size());
    if (!common.Ok()) {
        return common;
    }
    if (expectation.outputs.size() != memories.size()) {
        return Failure("AllToAll expected output rank count is invalid");
    }
    for (size_t rank = 0; rank < memories.size(); ++rank) {
        if (memories[rank].output != expectation.outputs[rank]) {
            const auto mismatch = std::mismatch(memories[rank].output.begin(), memories[rank].output.end(),
                expectation.outputs[rank].begin());
            std::ostringstream message;
            message << "AllToAll output mismatch at rank " << rank;
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
