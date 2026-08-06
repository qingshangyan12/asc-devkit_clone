/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kernel_proxy.h"
#include "ccu_primitives_stub.h"

#include <algorithm>
#include <exception>

namespace HcclSim {
namespace CcuSt {

RegisterManager &RegisterManager::Global()
{
    static RegisterManager manager;
    return manager;
}

void RegisterManager::Reset()
{
    nextKernelHandle_ = 1;
    kernels_.clear();
    instances_.clear();
    lastCompiledGroup_.clear();
    CompilerContext::SetCurrent(nullptr);
}

Result RegisterManager::RegisterStart(InstanceHandle instance)
{
    InstanceState &state = instances_[instance];
    if (state.registering) {
        return Result::STATE_ERROR;
    }
    state.registering = true;
    state.currentGroup.clear();
    return Result::SUCCESS;
}

Result RegisterManager::Register(InstanceHandle instance, const std::string &name,
    const KernelFunction &function, void *kernelArg, KernelHandle &handle)
{
    auto stateIterator = instances_.find(instance);
    if (stateIterator == instances_.end() || !stateIterator->second.registering || !function) {
        return Result::STATE_ERROR;
    }

    KernelRecord record;
    record.name = name;
    CompilerContext compiler(record.program);
    CompilerContext::SetCurrent(&compiler);
    Result result = Result::SUCCESS;
    try {
        result = function(kernelArg);
    } catch (const std::exception &) {
        result = Result::STATE_ERROR;
    } catch (...) {
        result = Result::STATE_ERROR;
    }
    CompilerContext::SetCurrent(nullptr);
    if (result != Result::SUCCESS) {
        return result;
    }

    handle = nextKernelHandle_++;
    kernels_.emplace(handle, std::move(record));
    stateIterator->second.currentGroup.push_back(handle);
    return Result::SUCCESS;
}

Result RegisterManager::RegisterEnd(InstanceHandle instance)
{
    auto stateIterator = instances_.find(instance);
    if (stateIterator == instances_.end() || !stateIterator->second.registering) {
        return Result::STATE_ERROR;
    }
    stateIterator->second.registering = false;
    const std::vector<KernelHandle> currentGroup = stateIterator->second.currentGroup;
    for (KernelHandle handle : currentGroup) {
        auto kernelIterator = kernels_.find(handle);
        if (kernelIterator == kernels_.end()) {
            lastCompiledGroup_.clear();
            return Result::NOT_FOUND;
        }
        Result result = CompilerContext(kernelIterator->second.program).Finalize();
        if (result != Result::SUCCESS) {
            lastCompiledGroup_.clear();
            return result;
        }
    }
    for (KernelHandle handle : currentGroup) {
        auto kernelIterator = kernels_.find(handle);
        kernelIterator->second.compiled = true;
    }
    lastCompiledGroup_ = currentGroup;
    return Result::SUCCESS;
}

const Program *RegisterManager::GetProgram(KernelHandle handle) const
{
    auto iterator = kernels_.find(handle);
    if (iterator == kernels_.end() || !iterator->second.compiled) {
        return nullptr;
    }
    return &iterator->second.program;
}

const std::vector<KernelHandle> &RegisterManager::GetLastCompiledGroup() const
{
    return lastCompiledGroup_;
}

bool RegisterManager::IsRegistering(InstanceHandle instance) const
{
    auto iterator = instances_.find(instance);
    return iterator != instances_.end() && iterator->second.registering;
}

std::vector<TaskParam> PackTaskArgs(const std::vector<uint64_t> &taskArgs)
{
    std::vector<TaskParam> taskParams;
    for (size_t offset = 0; offset < taskArgs.size(); offset += TASK_PARAM_SLOT_COUNT) {
        TaskParam taskParam;
        taskParam.copiedArgCount = static_cast<uint32_t>(
            std::min<size_t>(TASK_PARAM_SLOT_COUNT, taskArgs.size() - offset));
        for (uint32_t index = 0; index < taskParam.copiedArgCount; ++index) {
            taskParam.args[index] = taskArgs[offset + index];
        }
        taskParams.push_back(taskParam);
    }
    return taskParams;
}

ChannelHandle MakeChannel(uint32_t localRank, uint32_t remoteRank)
{
    return (static_cast<uint64_t>(localRank) << 32U) | remoteRank;
}

uint32_t ChannelLocalRank(ChannelHandle channel)
{
    return static_cast<uint32_t>(channel >> 32U);
}

uint32_t ChannelRemoteRank(ChannelHandle channel)
{
    return static_cast<uint32_t>(channel & 0xffffffffULL);
}

} // namespace CcuSt
} // namespace HcclSim
