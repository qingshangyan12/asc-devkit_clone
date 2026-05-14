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
#include "param_check.h"
#include "sal.h"
#include <memory>
#include <string>
#include <chrono>
#include <cstdlib>

extern bool HcclCheckLogLevel(int logType, int moduleId);
extern bool IsErrorToWarn();

bool HcclCheckLogLevel(int logType, int moduleId)
{
    return false;
}

bool IsErrorToWarn()
{
    return false;
}

extern "C" unsigned int HcclLaunchAicpuKernel(ops_hccl::OpParam* param)
{
    return 0;
}

namespace ops_hccl {

InsCollAlgBase::InsCollAlgBase() {}
InsCollAlgBase::~InsCollAlgBase() {}
std::string InsCollAlgBase::Describe() const { return "InsCollAlgBase"; }
HcclResult InsCollAlgBase::FastLaunch(const OpParam& param, const CcuFastLaunchCtx* resCtx) { return HCCL_SUCCESS; }

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

HcclResult HcclCheckTag(const char *tag)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CheckDataType(HcclDataType dataType, bool needReduce)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult HcomCheckDataType(HcclDataType dataType)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult HcomCheckReductionOp(HcclReduceOp op)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult HcomCheckUserRank(const u32 totalRanks, const u32 userRank)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult LogHcclExit(const std::string &opName, const char *tag, HcclUs startut)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult HcclGetOpExpansionMode(HcclComm comm, OpParam& param)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult HcclCalcTopoInfo(HcclComm comm, OpParam& param, std::unique_ptr<TopoInfoWithNetLayerDetails>& topoInfo)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult InitRankInfo(HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo)
{
    return HcclResult::HCCL_SUCCESS;
}

void GetScatterOpInfo(const void* opInfo, char* output, size_t size)
{
}

HcclResult CheckAsymmetricTopoSupport(HcclCMDType cmdType, const TopoInfoWithNetLayerDetails* topoInfo)
{
    return HCCL_SUCCESS;
}

ExecuteSelector::ExecuteSelector()
{
}

HcclResult ExecuteSelector::Run(OpParam& param, TopoInfoWithNetLayerDetails* topoInfo, std::string& algName) const
{
    algName = "CcuAllReduceMesh1D";
    return HcclResult::HCCL_SUCCESS;
}

HcclResult SetCommEngine(OpParam& param)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult InitEnvConfig()
{
    return HcclResult::HCCL_SUCCESS;
}

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

HcclResult SetOpParamAlgTag(OpParam& param, const std::string& algTag)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult SingleRankProc(const OpParam& param)
{
    return HcclResult::HCCL_SUCCESS;
}

CollAlgExecRegistryV2& CollAlgExecRegistryV2::Instance()
{
    static CollAlgExecRegistryV2 instance;
    return instance;
}

std::unique_ptr<InsCollAlgBase> CollAlgExecRegistryV2::GetAlgExec(HcclCMDType opType, const std::string& algTag)
{
    return std::make_unique<MockInsCollAlgBase>();
}

HcclResult HcclGetAlgRes(HcclComm comm, OpParam& param, std::unique_ptr<InsCollAlgBase>& executor,
    TopoInfoWithNetLayerDetails* topoInfo, std::unique_ptr<AlgResourceCtxSerializable>& resCtxHost,
    void** resCtxDevice, bool& isSuspend)
{
    if (resCtxDevice == nullptr) {
        return HCCL_E_PTR;
    }
    void* ctx = std::malloc(sizeof(AlgResourceCtxSerializable));
    if (ctx == nullptr) {
        return HCCL_E_PTR;
    }
    *resCtxDevice = ctx;
    isSuspend = false;
    return HcclResult::HCCL_SUCCESS;
}

}
