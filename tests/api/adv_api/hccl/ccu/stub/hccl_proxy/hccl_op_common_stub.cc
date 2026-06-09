/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "alg_param.h"
#include "hccl_types.h"
#include "execute_selector.h"
#include "coll_alg_v2_exec_registry.h"
#include "executor_base.h"
#include "executor_v2_base.h"
#include "alg_env_config.h"
#include <memory>
#include <string>

namespace mc2_ops_hccl {

// Stub executor for CCU tests
class MockInsCollAlgBase : public InsCollAlgBase {
public:
    MockInsCollAlgBase() = default;
    ~MockInsCollAlgBase() override = default;

    std::string Describe() const override { return "MockInsCollAlgBase"; }

    HcclResult CalcAlgHierarchyInfo(HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo,
                                    AlgHierarchyInfoForAllLevel& algHierarchyInfo) override
    {
        algHierarchyInfo.infos.resize(1);
        algHierarchyInfo.infos[0].resize(1);
        return HCCL_SUCCESS;
    }

    HcclResult CalcRes(HcclComm comm, const OpParam& param,
                       const TopoInfoWithNetLayerDetails* topoInfo,
                       const AlgHierarchyInfoForAllLevel& algHierarchyInfo,
                       AlgResourceRequest& resourceRequest) override
    {
        return HCCL_SUCCESS;
    }

    HcclResult Orchestrate(const OpParam& param, const AlgResourceCtxSerializable& resCtx) override
    {
        return HCCL_SUCCESS;
    }
};

// Executor registry stub
CollAlgExecRegistryV2& CollAlgExecRegistryV2::Instance()
{
    static CollAlgExecRegistryV2 instance;
    return instance;
}

std::unique_ptr<InsCollAlgBase> CollAlgExecRegistryV2::GetAlgExec(HcclCMDType opType, const std::string& algTag)
{
    return std::make_unique<MockInsCollAlgBase>();
}

// ExecuteSelector stub
HcclResult ExecuteSelector::Run(OpParam& param, TopoInfoWithNetLayerDetails* topoInfo, std::string& algName) const
{
    static const std::map<HcclCMDType, std::string> ccuAlgMap = {
        {HcclCMDType::HCCL_CMD_ALLGATHER, "CcuAllGatherMesh1D"},
        {HcclCMDType::HCCL_CMD_REDUCE_SCATTER, "CcuReduceScatterMesh1D"},
        {HcclCMDType::HCCL_CMD_ALLREDUCE, "CcuAllReduceMesh1D"},
        {HcclCMDType::HCCL_CMD_ALLTOALL, "CcuAlltoAllMesh1D"},
        {HcclCMDType::HCCL_CMD_ALLTOALLV, "CcuAlltoAllVMesh1D"},
    };

    auto it = ccuAlgMap.find(param.opType);
    if (it != ccuAlgMap.end()) {
        algName = it->second;
        return HcclResult::HCCL_SUCCESS;
    }
    return HcclResult::HCCL_E_NOT_SUPPORT;
}

// Env config stubs
const bool& GetExternalInputHcclAivOnlyMode()
{
    static bool value = false;
    return value;
}

const bool& GetExternalInputHcclEnableEntryLog()
{
    static bool value = false;
    return value;
}

const bool& GetExternalInputHcclCcuMSMode()
{
    static bool value = false;
    value = (std::getenv("HCCL_OP_EXPANSION_MODE") != nullptr && 
             std::string(std::getenv("HCCL_OP_EXPANSION_MODE")) == "CCU_MS");
    return value;
}

const bool& GetExternalInputHcclCcuSchedMode()
{
    static bool value = false;
    value = (std::getenv("HCCL_OP_EXPANSION_MODE") != nullptr && 
             std::string(std::getenv("HCCL_OP_EXPANSION_MODE")) == "CCU_SCHED");
    return value;
}

const bool& GetExternalInputHcclAicpuUnfold()
{
    static bool value = false;
    value = (std::getenv("HCCL_OP_EXPANSION_MODE") != nullptr && 
             std::string(std::getenv("HCCL_OP_EXPANSION_MODE")) == "AI_CPU");
    return value;
}

HcclResult InitEnvConfig()
{
    return HcclResult::HCCL_SUCCESS;
}

}
