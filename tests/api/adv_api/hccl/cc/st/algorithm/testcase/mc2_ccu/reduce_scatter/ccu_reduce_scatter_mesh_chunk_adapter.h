/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_CCU_REDUCE_SCATTER_MESH_CHUNK_ADAPTER_H
#define HCCL_CCU_REDUCE_SCATTER_MESH_CHUNK_ADAPTER_H

#include "ccu_fixture.h"
#include "ccu_kernel_proxy.h"

#include <array>
#include <vector>

namespace HcclSim {
namespace CcuSt {

// 与 CcuReduceScatterMesh1DReadReducePipelinedKernel 的 9 个 ccu::Variable 形参一一对应
// （token 亦占一槽，KFC 直调模式下由 taskArgs 提供；chunkCount=4*(rankSize-1) 为派生标量不占槽）。
constexpr uint32_t REDUCE_SCATTER_CHUNK_PIPELINED_TASK_ARG_COUNT = 9;
// 与 CcuReduceScatterMesh1DMsAccumKernel 的 8 个 ccu::Variable 形参一一对应（goSize 占 4 槽）。
constexpr uint32_t REDUCE_SCATTER_TWO_SHOT_TASK_ARG_COUNT = 8;

struct ReduceScatterChunkKernelConfig {
    uint32_t rankId{0};
    uint32_t rankSize{0};
    HcclDataType dataType{HCCL_DATA_TYPE_FP16};
    HcclDataType outputType{HCCL_DATA_TYPE_FP16};
    HcclReduceOp reduceType{HCCL_REDUCE_SUM};
    std::vector<ChannelHandle> channels;
};

struct ReduceScatterChunkPipelinedLaunchConfig {
    uint64_t inputAddress{0};
    uint64_t outputAddress{0};
    uint64_t token{0};
    uint64_t localInputOffset{0};
    uint64_t sliceSize{0};
    uint64_t chunkBaseSize{0};
    uint64_t chunkAlign{0};
    uint64_t chunkExtraUnits{0};
    uint64_t chunkTail{0};
};

struct ReduceScatterTwoShotLaunchConfig {
    uint64_t inputAddress{0};
    uint64_t outputAddress{0};
    uint64_t token{0};
    uint64_t localInputOffset{0};
    std::array<uint64_t, 4> goSize{};
};

Result CaptureCcuKfcReduceScatterChunkPipelinedKernel(void* kernelArg);
Result CaptureCcuKfcReduceScatterTwoShotKernel(void* kernelArg);
std::vector<uint64_t> PrepareReduceScatterChunkPipelinedTaskArgs(const ReduceScatterChunkPipelinedLaunchConfig& config);
std::vector<uint64_t> PrepareReduceScatterTwoShotTaskArgs(const ReduceScatterTwoShotLaunchConfig& config);
ScenarioData BuildReduceScatterChunkPipelinedScenario(const CcuStScenario& scenario, const std::vector<KernelHandle>& handles);
ScenarioData BuildReduceScatterTwoShotScenario(const CcuStScenario& scenario, const std::vector<KernelHandle>& handles);

} // namespace CcuSt
} // namespace HcclSim

#endif // HCCL_CCU_REDUCE_SCATTER_MESH_CHUNK_ADAPTER_H
