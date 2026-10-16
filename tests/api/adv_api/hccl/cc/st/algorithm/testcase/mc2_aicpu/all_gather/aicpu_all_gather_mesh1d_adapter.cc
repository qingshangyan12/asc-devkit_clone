/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "aicpu_all_gather_mesh1d_adapter.h"

#include "aicpu_all_gather.h"

namespace HcclSim {
namespace AicpuSt {
namespace {

std::vector<uint8_t> BuildMesh1DExpectation(uint32_t rankSize, const AicpuStScenario& scenario, RankId rank)
{
    (void)rank; // AllGather 语义与观察者 rank 无关
    return AicpuStAllGather::BuildConcatExpectation(rankSize, scenario);
}

} // namespace

void RegisterAllGatherMesh1DVariant()
{
    AicpuStAllGather::RegisterVariant("InsAllGatherMesh1D", BuildMesh1DExpectation);
}

} // namespace AicpuSt
} // namespace HcclSim
