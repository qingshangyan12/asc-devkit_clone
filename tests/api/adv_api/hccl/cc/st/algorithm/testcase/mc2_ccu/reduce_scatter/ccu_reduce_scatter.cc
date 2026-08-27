/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_reduce_scatter.h"

#include <algorithm>
#include <vector>
#include "hccl_types.h"
#include "alg_param.h"
#include "mc2_type.h"

namespace HcclSim {
namespace CcuSt {

std::map<std::string, CcuStReduceScatter::ScenarioBuilder>& CcuStReduceScatter::GetVariantRegistry()
{
    static std::map<std::string, ScenarioBuilder> registry;
    return registry;
}

void CcuStReduceScatter::RegisterVariant(const std::string& algName, ScenarioBuilder builder)
{
    GetVariantRegistry()[algName] = std::move(builder);
}

void CcuStReduceScatter::PrepareOpParam(OpParam& opParam, uint32_t rankSize, const CcuStScenario& scenario)
{
    const uint32_t typeSize = DATATYPE_SIZE_TABLE[scenario.dataType];
    const uint64_t totalCount = scenario.count;
    const uint64_t perRankCount = (rankSize > 0) ? (totalCount / rankSize) : 0;
    opParam.DataDes.count = perRankCount;
    opParam.DataDes.dataType = scenario.dataType;
    opParam.DataDes.outputType = scenario.outputType;
    opParam.reduceType = scenario.reduceType;
    (void)typeSize;
}

ScenarioData CcuStReduceScatter::BuildScenarioData(const CcuStScenario& scenario, const std::vector<KernelHandle>& handles)
{
    auto& registry = GetVariantRegistry();
    auto it = registry.find(scenario.expectedAlgName);
    if (it != registry.end()) {
        HCCL_INFO("[CcuStReduceScatter] BuildScenarioData found variant for [%s]", scenario.expectedAlgName.c_str());
        return it->second(scenario, handles);
    }
    HCCL_ERROR("[CcuStReduceScatter] BuildScenarioData no variant for [%s], registry size[%zu]",
               scenario.expectedAlgName.c_str(), registry.size());
    return {};
}

} // namespace CcuSt
} // namespace HcclSim
