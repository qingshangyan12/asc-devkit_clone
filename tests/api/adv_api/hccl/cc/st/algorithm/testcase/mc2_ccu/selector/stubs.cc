/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file be in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstring>
#include <cstdlib>
#include <string>
#include <map>
#include <unordered_map>
#include <memory>
#include <vector>

#include "alg_param.h"
#include "hccl_types.h"
#include "cann_host_bridge.h"
#include "ccu_log.h"
#include "ccu_kernel_alg_base.h"
#include "hccl_res.h"
#include "ccu_assist_pub.h"
#include "template_utils.h"
#include "alg_env_config.h"
#include "config_log.h"
#include "op_common.h"

extern "C" CcuKernelHandle RecordCcuKernelRegistrationForTest();

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

HcclResult ConvertCcuToHccl(CcuResult ccuResult)
{
    return HCCL_SUCCESS;
}

namespace mc2_ops_hccl {

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

void GetScatterOpInfo(const void* opInfo, char* output, size_t size)
{
}

HcclResult CheckAsymmetricTopoSupport(HcclCMDType cmdType, const TopoInfoWithNetLayerDetails* topoInfo)
{
    return HCCL_SUCCESS;
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

const u32& GetExternalInputIntraRoceSwitch()
{
    static u32 value = 0;
    return value;
}

const bool& GetExternalInputInterHccsDisable()
{
    static bool value = false;
    return value;
}

const std::map<HcclCMDType, std::vector<HcclAlgoType>> GetExternalInputHcclAlgoConfigAllType()
{
    return {};
}

u64 GetDebugConfig()
{
    return 0;
}

HcclResult SetOpParamAlgTag(OpParam& param, const std::string& algName)
{
    std::string launchMode = (param.engine == COMM_ENGINE_AICPU || param.engine == COMM_ENGINE_AICPU_TS)
                              ? "device" : "host";

    std::string algTag = std::string(param.tag) + "_" + algName + "_" + launchMode;

    if (param.engine == COMM_ENGINE_CCU) {
        HcclDataType dataType = HCCL_DATA_TYPE_FP16;
        if (param.opType == HcclCMDType::HCCL_CMD_ALLTOALL ||
            param.opType == HcclCMDType::HCCL_CMD_ALLTOALLV) {
            dataType = param.all2AllVDataDes.sendType;
        } else {
            dataType = param.DataDes.dataType;
        }

        algTag += "_" + std::to_string(static_cast<int>(dataType));

        if (param.opType == HcclCMDType::HCCL_CMD_ALLREDUCE ||
            param.opType == HcclCMDType::HCCL_CMD_REDUCE ||
            param.opType == HcclCMDType::HCCL_CMD_REDUCE_SCATTER) {
            algTag += "_" + std::to_string(static_cast<int>(param.reduceType));
        }
    }

    int ret = sprintf_s(param.algTag, sizeof(param.algTag), "%s", algTag.c_str());
    if (ret <= 0) {
        return HcclResult::HCCL_E_INTERNAL;
    }
    return HcclResult::HCCL_SUCCESS;
}

HcclResult SingleRankProc(const OpParam& param)
{
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CheckHostDPUOnly(const HcclComm comm, const TopoInfoWithNetLayerDetails* topoInfo, bool &hostDPUOnly)
{
    hostDPUOnly = false;
    return HCCL_SUCCESS;
}

bool UseCannBridge(const OpParam& param)
{
    return false;
}

HcclResult SelectViaCann(OpParam& ascParam, TopoInfoWithNetLayerDetails* ascTopo, std::string& algName)
{
    return HcclResult::HCCL_E_NOT_SUPPORT;
}

std::unique_ptr<InsCollAlgBase> GetAlgExecViaCann(HcclCMDType opType, const std::string& algName)
{
    return nullptr;
}

uint64_t DataTypeSizeGet(HcclDataType type)
{
    return DATATYPE_SIZE_TABLE[type];
}

HcclResult LocalCopy(const ThreadHandle& thread, const DataSlice& srcSlice, const DataSlice& dstSlice)
{
    return HCCL_SUCCESS;
}

std::vector<uint64_t> CalGoSize(uint64_t size)
{
    return {size};
}

std::vector<uint64_t> CalGoSize(uint64_t size, const LoopGroupConfig &config)
{
    return {size};
}

CcuResult GroupCopy(CcuKernelCtxBase &ctx, ccu::LocalAddr dst, ccu::LocalAddr src, GroupOpSizeVars goSize)
{
    return CCU_SUCCESS;
}

HcclResult HcclGetAlgRes(HcclComm comm, OpParam& param, std::unique_ptr<InsCollAlgBase>& executor,
    TopoInfoWithNetLayerDetails* topoInfo, std::unique_ptr<AlgResourceCtxSerializable>& resCtxHost,
    void** resCtxDevice, bool& isResourceReused)
{
    if (resCtxDevice == nullptr) {
        return HCCL_E_PTR;
    }

    void *ctx = nullptr;
    uint64_t size = 0;
    if (HcclEngineCtxGet(comm, param.algTag, param.engine, &ctx, &size) == HCCL_SUCCESS) {
        isResourceReused = true;
        *resCtxDevice = ctx;
        param.ctxSize = size;
        return HCCL_SUCCESS;
    }

    isResourceReused = false;

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
        if (param.isKfc) {
            CcuKernelHandle kernelHandle = RecordCcuKernelRegistrationForTest();
            resCtxHost->ccuKernels.push_back(kernelHandle);
            resCtxHost->ccuKernelNum.push_back(1);
        }
    }

    std::vector<char> seq = resCtxHost->Serialize();
    uint64_t newSize = seq.size();

    HcclResult ret = HcclEngineCtxCreate(comm, param.algTag, param.engine, newSize, resCtxDevice);
    if (ret != HCCL_SUCCESS || *resCtxDevice == nullptr) {
        return HCCL_E_PTR;
    }

    memcpy_s(*resCtxDevice, newSize, seq.data(), newSize);
    param.ctxSize = newSize;

    return HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl
