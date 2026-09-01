/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_all_gather_kfc_server_adapter.h"

#include "alg_param.h"
#include "ccu_all_gather.h"
#include "ccu_kernel_kfc_server.h"
#include "ccu_kernel_proxy.h"
#include "kfc_server_message.h"

#include <limits>
#include <vector>

namespace HcclSim {
namespace CcuSt {
namespace {

// The server loads one die's 32 words per turn, but its circular-queue cursor
// advances by CCU_PARAM_NUM_MAX to reserve the second-die payload as well.
constexpr uint32_t KFC_QUEUE_PARAM_WORDS = mc2_ops_hccl::CCU_PARAM_NUM_MAX;
constexpr uint32_t KFC_QUEUE_ENTRY_COUNT = 2;
constexpr uint32_t KFC_CKE_WORD_OFFSET = KFC_QUEUE_PARAM_WORDS * KFC_QUEUE_ENTRY_COUNT;
constexpr uint32_t KFC_CKE_REGION_WORDS = mc2_ops_hccl::CCU_TASK_NUM_MAX;
constexpr uint32_t KFC_PARAMETER_WORDS = KFC_CKE_WORD_OFFSET + 2 * KFC_CKE_REGION_WORDS;
constexpr uint64_t SOURCE_BASE_OFFSET = 7;
constexpr uint64_t DESTINATION_BASE_OFFSET = 11;
constexpr uint64_t GUARD_SIZE = 32;

struct AllGatherKfcServerConfig {
    mc2_ops_hccl::CcuKernelArgKfcServer kernelArg;
};

void* ConvertAllGatherKfcServerArg(const void* productArg, uint32_t argNum)
{
    if (productArg == nullptr || argNum != 1U) {
        return nullptr;
    }
    const auto* source = static_cast<const mc2_ops_hccl::CcuKernelArgKfcServer*>(productArg);
    if (source->opParam.opType != HcclCMDType::HCCL_CMD_ALLGATHER) {
        return nullptr;
    }
    auto* config = new AllGatherKfcServerConfig();
    config->kernelArg = *source;
    return config;
}

Result CaptureAllGatherKfcServerKernel(void* kernelArg)
{
    if (kernelArg == nullptr) {
        return Result::PARAM_ERROR;
    }
    auto& config = *static_cast<AllGatherKfcServerConfig*>(kernelArg);
    const auto& arg = config.kernelArg;
    if (arg.opParam.opType != HcclCMDType::HCCL_CMD_ALLGATHER || arg.rankSize < 2U || arg.rankId >= arg.rankSize ||
        arg.channelCount != arg.rankSize - 1U) {
        return Result::PARAM_ERROR;
    }

    std::vector<ChannelHandle> channels(arg.channels, arg.channels + arg.channelCount);
    CompilerContext::Current().ConfigureProgram(arg.rankId, static_cast<uint32_t>(arg.rankSize), channels);
    const CcuResult result = mc2_ops_hccl::CcuKfcServerKernel(static_cast<CcuKernelArg>(&config.kernelArg));
    return result == CCU_SUCCESS ? Result::SUCCESS : Result::CONTRACT_ERROR;
}

bool g_registered = []() {
    RegisterCaptureFunction(
        "CcuKernelKfcServer", CaptureAllGatherKfcServerKernel, ConvertAllGatherKfcServerArg,
        [](void* pointer) { delete static_cast<AllGatherKfcServerConfig*>(pointer); });
    CcuStAllGather::RegisterVariant("CcuSchedAllGatherSoleMesh", BuildAllGatherKfcServerScenario);
    return true;
}();

void FillKfcQueue(
    RankMemory& memory, uint32_t rank, uint64_t sliceSize, uint64_t inputAddress, uint64_t outputAddress)
{
    memory.parameter.assign(KFC_PARAMETER_WORDS, 0U);
    if (sliceSize == 0U) {
        // The production template does not enqueue an empty AllGather task.
        // Let the test server consume the termination marker immediately.
        memory.parameter[0] = std::numeric_limits<uint64_t>::max();
        memory.parameter[KFC_CKE_WORD_OFFSET] = 1U;
        return;
    }

    // Match CcuTempKfcAllGatherMesh1DMem2Mem::KernelRun's queue control word.
    memory.parameter[HcclKfcProtocol::KFC_CONCURRENT_AG_OP_ID] = 1U;
    memory.parameter[HcclKfcProtocol::KFC_CONCURRENT_AG_MESH_INPUT] = inputAddress;
    memory.parameter[HcclKfcProtocol::KFC_CONCURRENT_AG_MESH_OUTPUT] = outputAddress;
