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
#include "hccl_res.h"
#include "hcomm/pkg_inc/hcomm/ccu/ccu_assist_pub.h"
#include <memory>
#include <string>
#include <chrono>
#include <cstdlib>
#include <map>
#include <unordered_map>

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

extern "C" unsigned int HcclLaunchAicpuKernel(mc2_ops_hccl::OpParam* param)
{
    return 0;
}

namespace mc2_ops_hccl {

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
    if (GetExternalInputHcclCcuMSMode()) {
        param.opExecuteConfig = OpExecuteConfig::CCU_MS;
        param.engine = CommEngine::COMM_ENGINE_CCU;
    } else if (GetExternalInputHcclCcuSchedMode()) {
        param.opExecuteConfig = OpExecuteConfig::CCU_SCHED;
        param.engine = CommEngine::COMM_ENGINE_CCU;
    } else if (GetExternalInputHcclAicpuUnfold()) {
        param.opExecuteConfig = OpExecuteConfig::AICPU_TS;
        param.engine = CommEngine::COMM_ENGINE_AICPU_TS;
    } else {
        param.opExecuteConfig = OpExecuteConfig::AICPU_TS;
        param.engine = CommEngine::COMM_ENGINE_AICPU_TS;
    }
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
    static const std::map<HcclCMDType, std::string> ccuAlgMap = {
        {HcclCMDType::HCCL_CMD_ALLGATHER, "CcuAllGatherMesh1D"},
        {HcclCMDType::HCCL_CMD_REDUCE_SCATTER, "CcuReduceScatterMesh1D"},
        {HcclCMDType::HCCL_CMD_ALLREDUCE, "CcuAllReduceMesh1D"},
        {HcclCMDType::HCCL_CMD_ALLTOALL, "CcuAlltoAllMesh1D"},
        {HcclCMDType::HCCL_CMD_ALLTOALLV, "CcuAlltoAllVMesh1D"},
    };
    static const std::map<HcclCMDType, std::string> aicpuAlgMap = {
        {HcclCMDType::HCCL_CMD_ALLGATHER, "InsAllGatherMesh"},
        {HcclCMDType::HCCL_CMD_REDUCE_SCATTER, "InsReduceScatterNHR"},
        {HcclCMDType::HCCL_CMD_ALLREDUCE, "InsAllReduceNHR"},
        {HcclCMDType::HCCL_CMD_ALLTOALL, "InsAlltoAllMesh"},
        {HcclCMDType::HCCL_CMD_ALLTOALLV, "InsAlltoAllvMesh"},
    };

    auto& algMap = (param.engine == COMM_ENGINE_CCU) ? ccuAlgMap : aicpuAlgMap;
    auto it = algMap.find(param.opType);
    if (it != algMap.end()) {
        algName = it->second;
        return HcclResult::HCCL_SUCCESS;
    }
    return HcclResult::HCCL_E_NOT_SUPPORT;
}

HcclResult SetCommEngine(OpParam& param)
{
    static const std::unordered_map<OpExecuteConfig, CommEngine> ConfigToEngineMap = {
        {OpExecuteConfig::HOSTCPU_TS, COMM_ENGINE_CPU_TS},
        {OpExecuteConfig::AICPU_TS,   COMM_ENGINE_AICPU_TS},
        {OpExecuteConfig::CCU_MS,     COMM_ENGINE_CCU},
        {OpExecuteConfig::CCU_SCHED,  COMM_ENGINE_CCU},
        {OpExecuteConfig::AICPU,      COMM_ENGINE_AICPU},
        {OpExecuteConfig::HOSTCPU,    COMM_ENGINE_CPU},
    };

    auto it = ConfigToEngineMap.find(param.opExecuteConfig);
    if (it != ConfigToEngineMap.end()) {
        param.engine = it->second;
        return HCCL_SUCCESS;
    }
    return HcclResult::HCCL_SUCCESS;
}

HcclResult InitEnvConfig()
{
    return HcclResult::HCCL_SUCCESS;
}

static bool ReadEnvVarCcuMSMode()
{
    const char* envValue = std::getenv("HCCL_OP_EXPANSION_MODE");
    return (envValue != nullptr && std::string(envValue) == "CCU_MS");
}

static bool ReadEnvVarCcuSchedMode()
{
    const char* envValue = std::getenv("HCCL_OP_EXPANSION_MODE");
    return (envValue != nullptr && std::string(envValue) == "CCU_SCHED");
}

static bool ReadEnvVarAicpuUnfold()
{
    const char* envValue = std::getenv("HCCL_OP_EXPANSION_MODE");
    return (envValue != nullptr && std::string(envValue) == "AI_CPU");
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

const bool& GetExternalInputHcclCcuMSMode()
{
    static bool value = false;
    value = ReadEnvVarCcuMSMode();
    return value;
}

const bool& GetExternalInputHcclCcuSchedMode()
{
    static bool value = false;
    value = ReadEnvVarCcuSchedMode();
    return value;
}

const bool& GetExternalInputHcclAicpuUnfold()
{
    static bool value = false;
    value = ReadEnvVarAicpuUnfold();
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
    
    if (param.engine == COMM_ENGINE_CCU) {
        void *cclBufferPtr = nullptr;
        uint64_t cclBufferSize = 0;
        HcclResult ret = HcclGetHcclBuffer(comm, &cclBufferPtr, &cclBufferSize);
        
        if (ret == HCCL_SUCCESS && cclBufferPtr != nullptr && cclBufferSize > 0) {
            uint64_t cclBufferAddr = reinterpret_cast<uint64_t>(cclBufferPtr);
            uint64_t token = hcomm::CcuRep::GetTokenInfo(cclBufferAddr, cclBufferSize);
            
            if (resCtxHost->kfcServerArgs.size() >= 6U) {
                resCtxHost->kfcServerArgs[5] = token;
            }
        }
    }
    
    std::vector<char> seq = resCtxHost->Serialize();
    uint64_t size = seq.size();
    void* ctx = std::malloc(size);
    if (ctx == nullptr) {
        return HCCL_E_PTR;
    }
    memcpy(ctx, seq.data(), size);
    *resCtxDevice = ctx;
    param.ctxSize = size;
    isSuspend = false;
    return HcclResult::HCCL_SUCCESS;
}

}
