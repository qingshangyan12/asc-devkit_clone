/**
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
#include "dlhcomm_function.h"

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

extern "C" HcclResult HcclDfxRegOpInfoByCommId(char* commId, void* dfxOpInfo)
{
    return HCCL_SUCCESS;
}

namespace mc2_ops_hccl {

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

void GetScatterOpInfo(const void* opInfo, char* output, size_t size)
{
}

HcclResult CheckAsymmetricTopoSupport(HcclCMDType cmdType, const TopoInfoWithNetLayerDetails* topoInfo)
{
    return HCCL_SUCCESS;
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

DlHcommFunction::~DlHcommFunction() = default;
DlHcommFunction& DlHcommFunction::GetInstance()
{
    static DlHcommFunction instance;
    return instance;
}
HcclResult DlHcommFunction::DlHcommFunctionInit()
{
    return HCCL_SUCCESS;
}
HcclResult DlHcommFunction::DlHcommFunctionInterInit()
{
    return HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl

extern "C" {
bool HcommIsSupportHcommCcuInsResDescCreate() { return false; }
bool HcommIsSupportHcommCcuInsResDescDestroy() { return false; }
bool HcommIsSupportHcommCcuInsResDescSetNum() { return false; }
bool HcommIsSupportHcommCcuInsResDescQueryNum() { return false; }
bool HcommIsSupportHcommCcuInsCreate() { return false; }
bool HcommIsSupportHcommCcuInsDestroy() { return false; }
bool HcommIsSupportHcommCcuInsQueryResDesc() { return false; }
bool HcommIsSupportHcommCcuQueryRemainResDesc() { return false; }
bool HcommIsSupportHcommCcuKernelQueryResReq() { return false; }
bool HcommIsSupportHcclCommAssignCcuIns() { return false; }
}
