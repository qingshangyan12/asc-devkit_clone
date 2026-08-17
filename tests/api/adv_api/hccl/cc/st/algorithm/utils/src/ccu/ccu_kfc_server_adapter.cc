/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_kfc_server_adapter.h"

#include "ccu_kernel_kfc_server.h"
#include "ccu_primitives_stub.h"

#include <algorithm>

namespace HcclSim {
namespace CcuSt {

static_assert(
    static_cast<uint32_t>(KfcServerTestRole::DEFAULT) ==
        static_cast<uint32_t>(mc2_ops_hccl::KfcServerRole::DEFAULT),
    "test and production KFC roles differ");
static_assert(
    static_cast<uint32_t>(KfcServerTestRole::ALL_GATHER_MESH) ==
        static_cast<uint32_t>(mc2_ops_hccl::KfcServerRole::ALL_GATHER_MESH),
    "test and production KFC roles differ");
static_assert(
    static_cast<uint32_t>(KfcServerTestRole::ALL_GATHER_NHR) ==
        static_cast<uint32_t>(mc2_ops_hccl::KfcServerRole::ALL_GATHER_NHR),
    "test and production KFC roles differ");

Result CaptureKfcServerKernel(void *kernelArg)
{
    if (kernelArg == nullptr) {
        return Result::PARAM_ERROR;
    }
    const auto &config = *static_cast<KfcServerKernelConfig *>(kernelArg);
    if (config.rankSize < 2 || config.rankId >= config.rankSize || config.opId == UINT64_MAX ||
        config.channels.size() != config.rankSize - 1) {
        return Result::PARAM_ERROR;
    }

    mc2_ops_hccl::CcuKernelArgKfcServer argument{};
    argument.rankId = config.rankId;
    argument.rankSize = config.rankSize;
    argument.loadFromMem = true;
    argument.role = static_cast<mc2_ops_hccl::KfcServerRole>(config.role);
    argument.channelCount = static_cast<uint32_t>(config.channels.size());
    std::copy(config.channels.begin(), config.channels.end(), argument.channels);
    mc2_ops_hccl::OpParam opParam{};
    opParam.opType = HcclCMDType::HCCL_CMD_ALLTOALLV;
    opParam.isMc2 = true;
    argument.opParam = opParam;
    argument.supportedOpIds.push_back(config.opId);
    argument.supportedOpParams.push_back(opParam);

    CompilerContext::Current().ConfigureProgram(config.rankId, config.rankSize, config.channels);
    const CcuResult result = mc2_ops_hccl::CcuKfcServerKernel(static_cast<CcuKernelArg>(&argument));
    return result == CCU_SUCCESS ? Result::SUCCESS : Result::CONTRACT_ERROR;
}

} // namespace CcuSt
} // namespace HcclSim
