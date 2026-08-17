/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_all_to_all_v_mesh1d_adapter.h"

#include "ccu_kernel_all_to_all_v_mesh1d.h"
#include "ccu_primitives_stub.h"

namespace HcclSim {
namespace CcuSt {

Result CaptureAllToAllVMesh1DKernel(void *kernelArg)
{
    if (kernelArg == nullptr) {
        return Result::PARAM_ERROR;
    }
    const auto &config = *static_cast<AllToAllVKernelConfig *>(kernelArg);
    if (config.rankSize < 2 || config.rankId >= config.rankSize ||
        config.channels.size() != config.rankSize - 1) {
        return Result::PARAM_ERROR;
    }

    std::array<AscendC::ccu::Variable, ALL_TO_ALL_V_TASK_ARG_COUNT> arguments;
    for (uint32_t index = 0; index < arguments.size(); ++index) {
        if (AscendC::ccu::LoadArg(arguments[index], index) != Result::SUCCESS) {
            return Result::CONTRACT_ERROR;
        }
    }
    CompilerContext::Current().ConfigureProgram(config.rankId, config.rankSize, config.channels);
    const CcuResult result = mc2_ops_hccl::CcuAlltoAllVMesh1DKernel(
        arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5],
        arguments[6], arguments[7], arguments[8], arguments[9], config.channels.data(),
        static_cast<uint32_t>(config.channels.size()), config.rankSize, config.rankId);
    return result == CCU_SUCCESS ? Result::SUCCESS : Result::CONTRACT_ERROR;
}

std::vector<uint64_t> PrepareAllToAllVTaskArgs(const AllToAllVLaunchConfig &config)
{
    return {config.inputAddress, config.outputAddress, config.token, config.sourceBaseOffset,
        config.destinationBaseOffset, config.parameterAddress, config.goSize[0], config.goSize[1],
        config.goSize[2], config.goSize[3]};
}

Result BuildAllToAllVKfcTaskArgs(const CcuKfcMessage &message,
    const CcuKfcStartupArgs &startup, std::vector<uint64_t> &taskArgs)
{
    if (message[0] == CCU_KFC_FINALIZE || DecodeKfcCollective(message[0]) != CCU_KFC_ALL_TO_ALL_V ||
        startup.token == 0) {
        return Result::CONTRACT_ERROR;
    }
    taskArgs = {message[1], message[2], startup.token, message[3], message[4], message[9],
        message[5], message[6], message[7], message[8]};
    return Result::SUCCESS;
}

} // namespace CcuSt
} // namespace HcclSim
