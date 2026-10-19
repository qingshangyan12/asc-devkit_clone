/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See the License for the specific language governing permissions and limitations under the License.
 */
#ifndef TOPO_MATCH_ONE_LEVEL
#define TOPO_MATCH_ONE_LEVEL

#include "topo_match_base.h"

namespace mc2_ops_hccl {

class TopoMatchOneLevel : public TopoMatchBase {
public:
    explicit TopoMatchOneLevel(OpExecuteConfig engine = OpExecuteConfig::AICPU_TS);
    ~TopoMatchOneLevel() override = default;

    std::string Describe() const override
    {
        return "Topo Match for One Level Algorithm.";
    }

    HcclResult MatchTopo(
        HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo,
        AlgHierarchyInfoForAllLevel& algHierarchyInfo) override;

private:
    OpExecuteConfig engine_{OpExecuteConfig::AICPU_TS};
};

} // namespace mc2_ops_hccl

#endif // TOPO_MATCH_ONE_LEVEL
