/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "include/adv_api/hccl/hccl_mc2.h"
#include "log.h"
#include "sal.h"
#include "alg_env_config.h"
#include "hccl_inner.h"
#include "param_check.h"
#include "hccl_alloc_ctx_res.h"
#include "op_common.h"
#include "adapter_acl.h"

#include <cstddef>

using namespace mc2_ops_hccl;

HcclResult Mc2GetCcArgs(void** ccArgs)
{
    CHK_PTR_NULL(ccArgs);

    Mc2CcArgs* ccArgsMem = static_cast<Mc2CcArgs*>(malloc(sizeof(Mc2CcArgs)));
    if (ccArgsMem == nullptr) {
        HCCL_ERROR("[Mc2GetCcArgs] malloc Mc2CcArgs mem failed, please check.");
        return HCCL_E_INTERNAL;
    }
    ccArgsMem->Init();
    *ccArgs = ccArgsMem;
    HCCL_RUN_INFO("[Mc2GetCcArgs] malloc Mc2CcArgs success, please fill mem[%p->%p] in it.", ccArgs, *ccArgs);

    return HCCL_SUCCESS;
}

HcclResult Mc2FreeCcArgs(void* ccArgs)
{
    CHK_PTR_NULL(ccArgs);

    free(ccArgs);
    ccArgs = nullptr;

    return HCCL_SUCCESS;
}

HcclResult Mc2SetCcSrcDataType(void* ccArgs, uint8_t srcDataType)
{
    CHK_PTR_NULL(ccArgs);
    CHK_RET(HcomCheckDataType(static_cast<HcclDataType>(srcDataType)));

    Mc2CcArgs* ccArgsPtr = static_cast<Mc2CcArgs*>(ccArgs);
    ccArgsPtr->srcDataType = static_cast<HcclDataType>(srcDataType);

    return HCCL_SUCCESS;
}

HcclResult Mc2SetCcDstDataType(void* ccArgs, uint8_t dstDataType)
{
    CHK_PTR_NULL(ccArgs);
    CHK_RET(HcomCheckDataType(static_cast<HcclDataType>(dstDataType)));

    Mc2CcArgs* ccArgsPtr = static_cast<Mc2CcArgs*>(ccArgs);
    ccArgsPtr->dstDataType = static_cast<HcclDataType>(dstDataType);

    return HCCL_SUCCESS;
}

HcclResult Mc2SetCcReduceType(void* ccArgs, uint32_t reduceType)
{
    CHK_PTR_NULL(ccArgs);
    CHK_RET(HcomCheckReductionOp(static_cast<HcclReduceOp>(reduceType)));

    Mc2CcArgs* ccArgsPtr = static_cast<Mc2CcArgs*>(ccArgs);
    ccArgsPtr->reduceType = static_cast<HcclReduceOp>(reduceType);

    return HCCL_SUCCESS;
}

HcclResult Mc2SetCcAlgConfig(void* ccArgs, char* algConfig)
{
    CHK_PTR_NULL(ccArgs);
    CHK_PTR_NULL(algConfig);

    Mc2CcArgs* ccArgsPtr = static_cast<Mc2CcArgs*>(ccArgs);
    s32 ret = strcpy_s(ccArgsPtr->algConfig, sizeof(ccArgsPtr->algConfig), algConfig);
    if (ret != EOK) {
        HCCL_ERROR("[%s] strcpy_s algConfig failed, ret[%d]", __func__, ret);
        return HCCL_E_PARA;
    }

    return HCCL_SUCCESS;
}

HcclResult Mc2SetCcCommEngine(void* ccArgs, uint8_t commEngine)
{
    CHK_PTR_NULL(ccArgs);
    if (commEngine != COMM_ENGINE_AICPU_TS && commEngine != COMM_ENGINE_AIV && commEngine != COMM_ENGINE_CCU) {
        HCCL_ERROR("[%s] commEngine[%u] not supported", __func__, commEngine);
        return HCCL_E_NOT_SUPPORT;
    }

    Mc2CcArgs* ccArgsPtr = static_cast<Mc2CcArgs*>(ccArgs);
    ccArgsPtr->commEngine = static_cast<CommEngine>(commEngine);

    return HCCL_SUCCESS;
}

HcclResult Mc2SetCcGroupName(void* ccArgs, char* groupName)
{
    CHK_PTR_NULL(ccArgs);
    CHK_PTR_NULL(groupName);

    Mc2CcArgs* ccArgsPtr = static_cast<Mc2CcArgs*>(ccArgs);
    s32 ret = strcpy_s(ccArgsPtr->groupName, sizeof(ccArgsPtr->groupName), groupName);
    if (ret != EOK) {
        HCCL_ERROR("[%s] strcpy_s groupName failed, ret[%d]", __func__, ret);
        return HCCL_E_PARA;
    }
    return HCCL_SUCCESS;
}

namespace {
HcclResult ValidateCcType(uint8_t ccType)
{
    switch (static_cast<HcclCMDType>(ccType)) {
        case HcclCMDType::HCCL_CMD_ALLGATHER:
        case HcclCMDType::HCCL_CMD_ALLREDUCE:
        case HcclCMDType::HCCL_CMD_REDUCE_SCATTER:
        case HcclCMDType::HCCL_CMD_ALLTOALL:
        case HcclCMDType::HCCL_CMD_ALLTOALLV:
            break;
        default:
            HCCL_ERROR("[%s] unsupported ccType[%u]", __func__, ccType);
            return HCCL_E_NOT_SUPPORT;
    }
    return HCCL_SUCCESS;
}

struct Mc2CcResourceTiling {
    Mc2InitTilingInner init;
    Mc2CcTilingInner cc;
};

HcclResult BuildCcResourceTiling(uint8_t ccType, const Mc2CcArgs& args, Mc2CcResourceTiling& tiling)
{
    tiling.init.version = INIT_TILING_VERSION;
    tiling.init.mc2HcommCnt = 1;
    tiling.init.offset[0] = offsetof(Mc2CcResourceTiling, cc);
    tiling.cc.opType = ccType;
    tiling.cc.commEngine = static_cast<uint8_t>(OpExecuteConfig::AICPU_TS);
    tiling.cc.srcDataType = static_cast<uint8_t>(args.srcDataType);
    tiling.cc.dstDataType = static_cast<uint8_t>(args.dstDataType);
    tiling.cc.reduceType = static_cast<uint32_t>(args.reduceType);
    CHK_SAFETY_FUNC_RET(strcpy_s(tiling.cc.groupName, sizeof(tiling.cc.groupName), args.groupName));
    CHK_SAFETY_FUNC_RET(strcpy_s(tiling.cc.algConfig, sizeof(tiling.cc.algConfig), args.algConfig));
    return HCCL_SUCCESS;
}

HcclResult Mc2AcquireCcResCtxImpl(
    HcclComm comm, uint8_t ccType, void* ccArgs, void** ccResCtx, uint32_t* ccResCtxSize, void* schd)
{
    HCCL_RUN_INFO(
        "[MC2_CLIENT_A5] enter asc-devkit common Mc2AcquireCcResCtx, "
        "comm[%p], ccType[%u], ccArgs[%p].",
        comm, static_cast<uint32_t>(ccType), ccArgs);
    HcclUs startut = TIME_NOW();
    CHK_PTR_NULL(ccResCtx);
    CHK_PTR_NULL(ccResCtxSize);
    *ccResCtx = nullptr;
    *ccResCtxSize = 0;

    if (schd != nullptr) {
        *static_cast<HcommCcuSchd*>(schd) = {};
    }
    CHK_PTR_NULL(comm);
    CHK_PTR_NULL(ccArgs);
    const auto& args = *static_cast<const Mc2CcArgs*>(ccArgs);
    CHK_RET(ValidateCcType(ccType));

    DevType deviceType = DevType::DEV_TYPE_COUNT;
    CHK_RET(hrtGetDeviceType(deviceType));
    if (deviceType != DevType::DEV_TYPE_950) {
        HCCL_ERROR("[%s] unsupported deviceType[%u]", __func__, deviceType);
        return HCCL_E_NOT_SUPPORT;
    }
    CHK_RET(InitEnvConfig());
    uint32_t rankSize = 0;
    uint32_t rankId = 0;
    CHK_RET(HcclGetRankSize(comm, &rankSize));
    CHK_RET(HcclGetRankId(comm, &rankId));
    CHK_RET(HcomCheckUserRank(rankSize, rankId));

    char commName[COMM_INDENTIFIER_MAX_LENGTH]{};
    CHK_RET(HcclGetCommName(comm, commName));

    if (args.commEngine != COMM_ENGINE_AICPU_TS) {
        HCCL_ERROR("[%s] unsupported commEngine[%u], only AICPU_TS is supported", __func__, args.commEngine);
        return HCCL_E_NOT_SUPPORT;
    }

    Mc2CcResourceTiling tiling{};
    CHK_RET(BuildCcResourceTiling(ccType, args, tiling));
    const void* ccTilingList[MAX_HCOM_NUM]{};
    uint32_t tilingNum = 0;
    CHK_RET(HcclGetTilingList(&tiling, ccTilingList, tilingNum));
    std::string ctxTag;
    void* result = nullptr;
    CHK_RET(AllocComResourceByTilingAicpu(
        comm, nullptr, &tiling, ccTilingList, tilingNum, commName, rankSize, rankId, &result, ctxTag));
    CHK_RET(LogHcclExit("Mc2AcquireCcResCtx", ctxTag.c_str(), startut));
    *ccResCtx = result;
    *ccResCtxSize = sizeof(OpResCtx);
    return HCCL_SUCCESS;
}

} // namespace

HcclResult __attribute__((visibility("default"))) Mc2AcquireCcResCtx(
    HcclComm comm, uint8_t ccType, void* ccArgs, void** ccResCtx, uint32_t* ccResCtxSize, void* schd)
{
    return Mc2AcquireCcResCtxImpl(comm, ccType, ccArgs, ccResCtx, ccResCtxSize, schd);
}
