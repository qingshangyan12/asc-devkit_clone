/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_sim_world.h"

#include <cstring>
#include <limits>
#include <sstream>

namespace HcclSim {
namespace CcuSt {

uint64_t RankMemory::InputAddress()
{
    return static_cast<const RankMemory &>(*this).InputAddress();
}

uint64_t RankMemory::OutputAddress()
{
    return static_cast<const RankMemory &>(*this).OutputAddress();
}

uint64_t RankMemory::ParameterAddress()
{
    return static_cast<const RankMemory &>(*this).ParameterAddress();
}

uint64_t RankMemory::InputAddress() const
{
    return inPlace ? OutputAddress() : reinterpret_cast<uint64_t>(const_cast<uint8_t *>(input.data()));
}

uint64_t RankMemory::OutputAddress() const
{
    return reinterpret_cast<uint64_t>(const_cast<uint8_t *>(output.data()));
}

uint64_t RankMemory::ParameterAddress() const
{
    return reinterpret_cast<uint64_t>(const_cast<uint64_t *>(parameter.data()));
}

struct RuntimeWorld::RankState {
    const Program *program{nullptr};
    const RankLaunch *launch{nullptr};
    size_t pc{0};
    bool completed{false};
    std::vector<TaskParam> taskParams;
    std::map<ResourceHandle, uint64_t> variables;
    std::map<ResourceHandle, uint64_t> addresses;
    std::map<ResourceHandle, uint16_t> events;
    std::map<std::pair<uint32_t, uint32_t>, uint16_t> notifies;
};

bool RuntimeWorld::Evaluate(const RankState &state, const Operation &operation) const
{
    auto iterator = state.variables.find(operation.src0);
    const uint64_t value = iterator == state.variables.end() ? 0 : iterator->second;
    return operation.equal ? value == operation.immediate : value != operation.immediate;
}

bool RuntimeWorld::ResolveMemory(std::vector<RankState> &states, uint64_t address, uint64_t size,
    uint8_t *&pointer, uint32_t &ownerRank)
{
    if (size > std::numeric_limits<uint64_t>::max() - address) {
        return false;
    }
    const uint64_t end = address + size;
    for (size_t index = 0; index < states.size(); ++index) {
        RankMemory *memory = states[index].launch->memory;
        const uint64_t outputBase = memory->OutputAddress();
        if (address >= outputBase && end <= outputBase + memory->output.size()) {
            pointer = memory->output.data() + (address - outputBase);
            ownerRank = static_cast<uint32_t>(index);
            return true;
        }
        if (!memory->inPlace) {
            const uint64_t inputBase = memory->InputAddress();
            if (address >= inputBase && end <= inputBase + memory->input.size()) {
                pointer = memory->input.data() + (address - inputBase);
                ownerRank = static_cast<uint32_t>(index);
                return true;
            }
        }
        const uint64_t parameterBase = memory->ParameterAddress();
        const uint64_t parameterSize = memory->parameter.size() * sizeof(uint64_t);
        if (address >= parameterBase && end <= parameterBase + parameterSize) {
            pointer = reinterpret_cast<uint8_t *>(memory->parameter.data()) + (address - parameterBase);
            ownerRank = static_cast<uint32_t>(index);
            return true;
        }
    }
    return false;
}

bool RuntimeWorld::ExecuteOne(std::vector<RankState> &states, size_t rankIndex, Result &result)
{
    RankState &state = states[rankIndex];
    if (state.completed) {
        return false;
    }
    if (state.pc >= state.program->operations.size()) {
        state.completed = true;
        lastExecutionStats_[rankIndex].completed = true;
        return true;
    }

    const Operation &operation = state.program->operations[state.pc];
    switch (operation.code) {
        case OpCode::LOAD_ARG:
        {
            const uint32_t missionIndex = operation.index / TASK_PARAM_SLOT_COUNT;
            const uint32_t slotIndex = operation.index % TASK_PARAM_SLOT_COUNT;
            if (missionIndex >= state.taskParams.size() ||
                slotIndex >= state.taskParams[missionIndex].copiedArgCount) {
                result = Result::CONTRACT_ERROR;
                SetError("LoadArg index exceeds taskArgs");
                return false;
            }
            state.variables[operation.dst] = state.taskParams[missionIndex].args[slotIndex];
            break;
        }
        case OpCode::LOAD_MEMORY: {
            uint8_t *source = nullptr;
            uint32_t ownerRank = 0;
            const uint64_t baseAddress = state.variables[operation.src0];
            if (operation.byteOffset > std::numeric_limits<uint64_t>::max() - baseAddress ||
                !ResolveMemory(states, baseAddress + operation.byteOffset, sizeof(uint64_t), source, ownerRank)) {
                result = Result::OUT_OF_RANGE;
                SetError("CCU Load address is out of range");
                return false;
            }
            if (ownerRank != rankIndex) {
                result = Result::CONTRACT_ERROR;
                SetError("CCU Load address resolves to another rank");
                return false;
            }
            uint64_t value = 0;
            std::memcpy(&value, source, sizeof(value));
            state.variables[operation.dst] = value;
            break;
        }
        case OpCode::STORE_MEMORY: {
            uint8_t *destination = nullptr;
            uint32_t ownerRank = 0;
            if (!ResolveMemory(states, state.variables[operation.dst], sizeof(uint64_t), destination, ownerRank)) {
                result = Result::OUT_OF_RANGE;
                SetError("CCU Store address is out of range");
                return false;
            }
            if (ownerRank != rankIndex) {
                result = Result::CONTRACT_ERROR;
                SetError("CCU Store address resolves to another rank");
                return false;
            }
            const uint64_t value = state.variables[operation.src0];
            std::memcpy(destination, &value, sizeof(value));
            break;
        }
        case OpCode::ASSIGN_IMM:
            state.variables[operation.dst] = operation.immediate;
            break;
        case OpCode::ASSIGN_VAR:
            state.variables[operation.dst] = state.variables[operation.src0];
            break;
        case OpCode::ADD_VAR:
            state.variables[operation.dst] = state.variables[operation.src0] + state.variables[operation.src1];
            break;
        case OpCode::ADDRESS_ASSIGN_VAR:
            state.addresses[operation.dst] = state.variables[operation.src0];
            break;
        case OpCode::ADDRESS_ASSIGN_ADDRESS:
            state.addresses[operation.dst] = state.addresses[operation.src0];
            break;
        case OpCode::ADDRESS_ADD_VAR:
            state.addresses[operation.dst] += state.variables[operation.src0];
            break;
        case OpCode::WRITE_VARIABLE_NOTIFY: {
            const uint32_t remoteRank = ChannelRemoteRank(operation.channel);
            if (remoteRank >= states.size()) {
                result = Result::PARAM_ERROR;
                SetError("channel remote rank is invalid");
                return false;
            }
            RankState &remote = states[remoteRank];
            auto variableIterator = remote.program->channelVariables.find(
                {static_cast<uint32_t>(rankIndex), operation.index});
            if (variableIterator == remote.program->channelVariables.end()) {
                result = Result::CONTRACT_ERROR;
                SetError("remote channel variable was not registered");
                return false;
            }
            remote.variables[variableIterator->second] = state.variables[operation.src0];
            for (ChannelHandle channel : remote.program->channels) {
                remote.notifies[{ChannelRemoteRank(channel), static_cast<uint32_t>(operation.immediate)}] |=
                    operation.mask;
            }
            break;
        }
        case OpCode::NOTIFY_WAIT: {
            const uint32_t peer = ChannelRemoteRank(operation.channel);
            uint16_t &value = state.notifies[{peer, operation.index}];
            if ((value & operation.mask) != operation.mask) {
                return false;
            }
            value &= static_cast<uint16_t>(~operation.mask);
            break;
        }
        case OpCode::NOTIFY_RECORD: {
            const uint32_t remoteRank = ChannelRemoteRank(operation.channel);
            for (ChannelHandle channel : states[remoteRank].program->channels) {
                states[remoteRank].notifies[{ChannelRemoteRank(channel), operation.index}] |= operation.mask;
            }
            break;
        }
        case OpCode::EVENT_RECORD:
            state.events[operation.dst] |= operation.mask;
            break;
        case OpCode::EVENT_WAIT: {
            uint16_t &value = state.events[operation.src0];
            if ((value & operation.mask) != operation.mask) {
                return false;
            }
            value &= static_cast<uint16_t>(~operation.mask);
            break;
        }
        case OpCode::WRITE:
        case OpCode::LOCAL_COPY:
        case OpCode::GROUP_COPY: {
            auto localIterator = state.program->localAddresses.find(operation.src0);
            if (localIterator == state.program->localAddresses.end()) {
                result = Result::CONTRACT_ERROR;
                SetError("local address handle is invalid");
                return false;
            }
            ResourceHandle destinationAddress = 0;
            uint32_t expectedDestinationRank = static_cast<uint32_t>(rankIndex);
            if (operation.code == OpCode::WRITE) {
                auto remoteIterator = state.program->remoteAddresses.find(operation.dst);
                if (remoteIterator == state.program->remoteAddresses.end()) {
                    result = Result::CONTRACT_ERROR;
                    SetError("remote address handle is invalid");
                    return false;
                }
                destinationAddress = remoteIterator->second.first;
                expectedDestinationRank = ChannelRemoteRank(operation.channel);
            } else {
                auto destinationIterator = state.program->localAddresses.find(operation.dst);
                if (destinationIterator == state.program->localAddresses.end()) {
                    result = Result::CONTRACT_ERROR;
                    SetError("local destination handle is invalid");
                    return false;
                }
                destinationAddress = destinationIterator->second.first;
            }

            const uint64_t length = state.variables[operation.src1];
            uint8_t *sourcePointer = nullptr;
            uint8_t *destinationPointer = nullptr;
            uint32_t sourceRank = 0;
            uint32_t destinationRank = 0;
            if (!ResolveMemory(states, state.addresses[localIterator->second.first], length,
                    sourcePointer, sourceRank) ||
                !ResolveMemory(states, state.addresses[destinationAddress], length,
                    destinationPointer, destinationRank)) {
                result = Result::OUT_OF_RANGE;
                SetError("CCU memory operation is out of range");
                return false;
            }
            if (sourceRank != rankIndex || destinationRank != expectedDestinationRank) {
                result = Result::CONTRACT_ERROR;
                SetError("CCU address resolves to the wrong rank");
                return false;
            }
            std::memmove(destinationPointer, sourcePointer, static_cast<size_t>(length));
            if (operation.code != OpCode::GROUP_COPY) {
                state.events[operation.immediate] |= operation.mask;
            }
            break;
        }
        case OpCode::IF_BEGIN:
            if (!Evaluate(state, operation)) {
                state.pc = operation.peerIndex + 1;
                return true;
            }
            break;
        case OpCode::IF_END:
            break;
        case OpCode::WHILE_BEGIN:
            if (!Evaluate(state, operation)) {
                state.pc = operation.peerIndex + 1;
                return true;
            }
            break;
        case OpCode::WHILE_END:
            state.pc = operation.peerIndex;
            return true;
    }
    ++lastExecutionStats_[rankIndex].executed[operation.code];
    ++state.pc;
    return true;
}

Result RuntimeWorld::Launch(const std::vector<RankLaunch> &launches)
{
    lastError_.clear();
    lastExecutionStats_.assign(launches.size(), RankExecutionStats{});
    if (launches.empty()) {
        SetError("no ranks were provided");
        return Result::PARAM_ERROR;
    }

    std::vector<RankState> states(launches.size());
    for (size_t rank = 0; rank < launches.size(); ++rank) {
        const Program *program = RegisterManager::Global().GetProgram(launches[rank].kernel);
        if (program == nullptr || launches[rank].memory == nullptr || program->rankId != rank ||
            program->rankSize != launches.size()) {
            SetError("rank launch does not match compiled program");
            return Result::CONTRACT_ERROR;
        }
        if (launches[rank].taskArgs.size() != program->arguments.size()) {
            SetError("taskArgs size does not match LoadArg contract");
            return Result::CONTRACT_ERROR;
        }
        states[rank].program = program;
        states[rank].launch = &launches[rank];
        states[rank].taskParams = PackTaskArgs(launches[rank].taskArgs);
    }

    Result result = Result::SUCCESS;
    while (true) {
        bool allCompleted = true;
        bool progressed = false;
        for (size_t rank = 0; rank < states.size(); ++rank) {
            if (!states[rank].completed) {
                allCompleted = false;
                progressed = ExecuteOne(states, rank, result) || progressed;
                if (result != Result::SUCCESS) {
                    return result;
                }
            }
        }
        if (allCompleted) {
            return Result::SUCCESS;
        }
        if (!progressed) {
            SetError("all ranks are blocked on CCU events or notifies");
            return Result::DEADLOCK;
        }
    }
}

const std::string &RuntimeWorld::LastError() const
{
    return lastError_;
}

const std::vector<RankExecutionStats> &RuntimeWorld::LastExecutionStats() const
{
    return lastExecutionStats_;
}

void RuntimeWorld::SetError(const std::string &message)
{
    lastError_ = message;
}

} // namespace CcuSt
} // namespace HcclSim
