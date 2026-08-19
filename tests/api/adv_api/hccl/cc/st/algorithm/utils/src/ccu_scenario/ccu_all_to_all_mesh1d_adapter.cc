/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_all_to_all_mesh1d_adapter.h"

#include "ccu_kernel_all_to_all_mesh1d.h"
#include "ccu_primitives_stub.h"
#include "alg_param.h"

namespace HcclSim {
namespace CcuSt {

namespace {

void *ConvertAllToAllArg(const void *productArg, uint32_t argNum)
{
    if (productArg == nullptr) {
        return nullptr;
    }
    const auto *src = static_cast<const mc2_ops_hccl::CcuKernelArgAlltoAllMesh1D *>(productArg);
    auto *config = new AllToAllKernelConfig();
    config->rankId = src->rankId;
    config->rankSize = static_cast<uint32_t>(src->rankSize);
    config->loadFromMem = src->loadFromMem;
    for (uint32_t i = 0; i < src->channelCount; ++i) {
        uint32_t peerRank = i;
        if (peerRank >= config->rankId) {
            peerRank++;
        }
        config->channels.push_back(MakeChannel(config->rankId, peerRank));
    }
    return config;
}

bool g_registered = []() {
    RegisterCaptureFunction("CcuKernelAlltoAllMesh1D", CaptureAllToAllMesh1DKernel, ConvertAllToAllArg,
        [](void *p) { delete static_cast<AllToAllKernelConfig *>(p); });
    return true;
}();
} // namespace

Result CaptureAllToAllMesh1DKernel(void *kernelArg)
{
    if (kernelArg == nullptr) {
        return Result::PARAM_ERROR;
    }
    const auto &config = *static_cast<AllToAllKernelConfig *>(kernelArg);
    if (config.rankSize < 2 || config.rankId >= config.rankSize ||
        config.channels.size() != config.rankSize - 1) {
        return Result::PARAM_ERROR;
    }

    std::array<AscendC::ccu::Variable, ALL_TO_ALL_TASK_ARG_COUNT> arguments;
    for (uint32_t index = 0; index < arguments.size(); ++index) {
        if (AscendC::ccu::LoadArg(arguments[index], index) != Result::SUCCESS) {
            return Result::CONTRACT_ERROR;
        }
    }
    CompilerContext::Current().ConfigureProgram(config.rankId, config.rankSize, config.channels);

    mc2_ops_hccl::CcuKernelArgAlltoAllMesh1D kernelArgData;
    kernelArgData.rankSize = config.rankSize;
    kernelArgData.rankId = config.rankId;
    kernelArgData.loadFromMem = config.loadFromMem;
    kernelArgData.channelCount = static_cast<uint32_t>(config.channels.size());
    for (size_t i = 0; i < config.channels.size(); ++i) {
        kernelArgData.channels[i] = config.channels[i];
    }

    CcuKernelArg argPtr = static_cast<CcuKernelArg>(&kernelArgData);
    const CcuResult result = mc2_ops_hccl::CcuAlltoAllMesh1DKernel(
        arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6],
        arguments[7], arguments[8], arguments[9], arguments[10], argPtr);
    return result == CCU_SUCCESS ? Result::SUCCESS : Result::CONTRACT_ERROR;
}

std::vector<uint64_t> PrepareAllToAllTaskArgs(const AllToAllLaunchConfig &config)
{
    return {config.inputAddress, config.outputAddress, config.token, config.sliceSize,
        config.srcStride, config.writeLoopCount, config.dstOffset,
        config.goSize[0], config.goSize[1], config.goSize[2], config.goSize[3]};
}

} // namespace CcuSt
} // namespace HcclSim
