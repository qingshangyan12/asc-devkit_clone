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

#include <limits>

using namespace mc2_ops_hccl;

namespace {
const char* GetMc2OpTypeName(HcclCMDType opType)
{
    switch (opType) {
        case HcclCMDType::HCCL_CMD_ALLGATHER:
            return "AllGather";
        case HcclCMDType::HCCL_CMD_ALLREDUCE:
            return "AllReduce";
        case HcclCMDType::HCCL_CMD_REDUCE_SCATTER:
            return "ReduceScatter";
        case HcclCMDType::HCCL_CMD_ALLTOALL:
            return "AllToAll";
        case HcclCMDType::HCCL_CMD_ALLTOALLV:
            return "AllToAllV";
        default:
            return "Unknown";
    }
}

} // namespace

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
    if (commEngine != COMM_ENGINE_AICPU && commEngine != COMM_ENGINE_AIV && commEngine != COMM_ENGINE_CCU) {
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

HcclResult Mc2SetCcDebugMode(void* ccArgs, uint8_t debugMode)
{
    CHK_PTR_NULL(ccArgs);
    if (!((debugMode >= 1 && debugMode <= 4) || debugMode >= 250)) {
        HCCL_ERROR("[%s] debugMode[%u] only supports [1,4] or [250,255]", __func__, debugMode);
        return HCCL_E_PARA;
    }

    Mc2CcArgs* ccArgsPtr = static_cast<Mc2CcArgs*>(ccArgs);
    ccArgsPtr->debugMode = debugMode;
    return HCCL_SUCCESS;
}

namespace {
HcclResult TryCcAlgAndPrepareEngine(
    HcclComm comm, const char* algConfig, OpParam& opParam, std::string& algName,
    std::unique_ptr<TopoInfoWithNetLayerDetails>& topoInfo, bool& forcedAlgAccepted)
{
    forcedAlgAccepted = false;
    if (algConfig == nullptr || algConfig[0] == '\0' || strchr(algConfig, '=') != nullptr) {
        return HCCL_SUCCESS;
    }
    const std::string forcedAlgName(algConfig);

    OpParam opParamBackup = opParam;
    CHK_RET(PrepareTopoInfoForOp(comm, opParam, topoInfo));
    HcclResult ret = PrepareEngineForAlg(opParam, forcedAlgName);
    if (ret != HCCL_SUCCESS) {
        opParam = opParamBackup;
        HCCL_ERROR(
            "[MC2_FORCE_ALG] prepare engine failed, algName[%s], ret[%d].", forcedAlgName.c_str(),
            static_cast<int>(ret));
        return ret;
    }

    ret = CheckForcedAlgResource(comm, opParam, topoInfo.get(), forcedAlgName);
    if (ret != HCCL_SUCCESS) {
        opParam = opParamBackup;
        topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
        algName.clear();
        forcedAlgAccepted = false;
        HCCL_WARNING(
            "[MC2_FORCE_ALG] fallback to default selector, opType[%u], algConfig[%s].",
            static_cast<u32>(opParam.opType), forcedAlgName.c_str());
        return HCCL_SUCCESS;
    }

    algName = forcedAlgName;
    forcedAlgAccepted = true;
    HCCL_INFO("[MC2_FORCE_ALG] accepted, opType[%u], algName[%s].", static_cast<u32>(opParam.opType), algName.c_str());
    return HCCL_SUCCESS;
}

HcclResult GetCcOpParamResCtx(
    HcclComm comm, const std::string& algName, OpParam& opParam, TopoInfoWithNetLayerDetails* topoInfo,
    void** resCtxOut)
{
    CHK_PTR_NULL(topoInfo);
    CHK_PTR_NULL(resCtxOut);
    *resCtxOut = nullptr;
    auto executor = GetAlgExecutorForOp(opParam, algName);
    CHK_PRT_RET(
        executor.get() == nullptr, HCCL_ERROR("Fail to find executor for algName[%s]", algName.c_str()), HCCL_E_PARA);
    std::unique_ptr<AlgResourceCtxSerializable> resCtxHost = std::make_unique<AlgResourceCtxSerializable>();
    resCtxHost->isHcommBatchTransferOnThreadSupported = HcommIsSupportHcommBatchTransferOnThread();
    HCCL_DEBUG(
        "[MC2_BATCH_TRANSFER][HostSupport] opType[%u], algName[%s], supported[%d].",
        static_cast<uint32_t>(opParam.opType), algName.c_str(),
        static_cast<int>(resCtxHost->isHcommBatchTransferOnThreadSupported));
    bool isResourceReused = false;

    CHK_RET(HcclGetAlgRes(comm, opParam, executor, topoInfo, resCtxHost, resCtxOut, isResourceReused));
    CHK_PTR_NULL(*resCtxOut);
    opParam.cacheValid = isResourceReused;
    opParam.resCtx = *resCtxOut;
    return HCCL_SUCCESS;
}

HcclResult SelectAndAcquireCcAlgorithm(HcclComm comm, const char* algConfig, OpParam& opParam)
{
    const OpParam initialParam = opParam;
    std::string algName;
    auto topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    bool forcedAlgAccepted = false;
    CHK_RET(TryCcAlgAndPrepareEngine(comm, algConfig, opParam, algName, topoInfo, forcedAlgAccepted));

    // A failed forced allocation may retry once using the default selector.
    while (true) {
        if (!forcedAlgAccepted) {
            CHK_RET(SelectAlgAndPrepareEngine(comm, opParam, algName, topoInfo));
        }
        if (opParam.engine != COMM_ENGINE_AICPU_TS) {
            HCCL_ERROR("[%s] selected engine[%u] is not AICPU_TS", __func__, opParam.engine);
            return HCCL_E_NOT_SUPPORT;
        }
        CHK_RET(FillOpParamAlgName(opParam, algName));

        bool skipGetRes = false;
        CHK_RET(HandleSingleRankAndCommMode(comm, opParam, skipGetRes));
        if (skipGetRes) {
            return HCCL_SUCCESS;
        }

        void* resCtx = nullptr;
        HcclResult ret = GetCcOpParamResCtx(comm, algName, opParam, topoInfo.get(), &resCtx);
        if (ret == HCCL_SUCCESS || !forcedAlgAccepted || resCtx != nullptr) {
            return ret;
        }
        HCCL_WARNING(
            "[MC2_FORCE_ALG] fallback to default selector after resource failure, algName[%s], ret[%d].",
            algName.c_str(), static_cast<int>(ret));
        opParam = initialParam;
        algName.clear();
        topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
        forcedAlgAccepted = false;
    }
}

HcclResult AcquirePreparedCcOpParam(HcclComm comm, const char* algConfig, uint32_t rankSize, OpParam& opParam)
{
    OpParam prepared = opParam;
    std::vector<uint64_t> sendCounts;
    const bool isAlltoAll = prepared.opType == HcclCMDType::HCCL_CMD_ALLTOALL;
    if (isAlltoAll) {
        constexpr uint64_t ALLTOALL_DEFAULT_SEND_COUNTS = 200ULL * 1024 * 1024;
        sendCounts.assign(rankSize, ALLTOALL_DEFAULT_SEND_COUNTS);
        prepared.all2AllVDataDes.sendCounts = sendCounts.data();
    }
    CHK_RET(SelectAndAcquireCcAlgorithm(comm, algConfig, prepared));
    if (isAlltoAll) {
        prepared.all2AllVDataDes.sendCounts = opParam.all2AllVDataDes.sendCounts;
    }
    opParam = prepared;
    return HCCL_SUCCESS;
}

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

HcclResult InitCcOpParam(
    HcclComm comm, uint8_t ccType, const Mc2CcArgs& args, const char* commName, uint32_t rankSize, OpParam& opParam)
{
    opParam.hcclComm = comm;
    opParam.opType = static_cast<HcclCMDType>(ccType);
    opParam.stream = nullptr;
    opParam.engine = COMM_ENGINE_AICPU;
    opParam.commOpExpansionMode = HcclOpExpansionMode::HCCL_OP_EXPANSION_MODE_AI_CPU;
    CHK_SAFETY_FUNC_RET(strcpy_s(opParam.commName, sizeof(opParam.commName), commName));
    const std::string topoTag = "mc2_cc_" + std::to_string(ccType) + "_" + std::to_string(args.srcDataType) + "_" +
                                std::string(opParam.commName);
    CHK_RET(HcclCheckTag(topoTag.c_str()));
    const bool isReduce =
        opParam.opType == HcclCMDType::HCCL_CMD_ALLREDUCE || opParam.opType == HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
    CHK_RET(CheckDataType(args.srcDataType, isReduce));
    CHK_RET(PrepareOpsCommParam(topoTag, opParam));

    if (opParam.opType == HcclCMDType::HCCL_CMD_ALLTOALL || opParam.opType == HcclCMDType::HCCL_CMD_ALLTOALLV) {
        opParam.reduceType = HCCL_REDUCE_SUM;
        opParam.all2AllVDataDes = {};
        opParam.all2AllVDataDes.sendType = args.srcDataType;
        opParam.all2AllVDataDes.recvType = args.dstDataType;
        opParam.varMemSize = ALL_TO_ALL_V_VECTOR_NUM * static_cast<uint64_t>(rankSize) * sizeof(uint64_t);
    } else {
        opParam.reduceType = opParam.opType == HcclCMDType::HCCL_CMD_ALLGATHER ? HCCL_REDUCE_SUM : args.reduceType;
        opParam.DataDes.dataType = args.srcDataType;
        if (opParam.opType == HcclCMDType::HCCL_CMD_ALLREDUCE) {
            opParam.DataDes.outputType = opParam.DataDes.dataType;
        }
        opParam.DataDes.count = 0;
    }
    return HCCL_SUCCESS;
}

HcclResult GetOrCreateCcMemory(HcclComm comm, const std::string& tag, uint64_t size, void** address)
{
    uint64_t allocatedSize = size;
    if (HcclEngineCtxGet(comm, tag.c_str(), COMM_ENGINE_AICPU, address, &allocatedSize) == HCCL_SUCCESS) {
        if (allocatedSize < size) {
            HCCL_ERROR(
                "[%s] context[%s] is too small: allocated[%llu], required[%llu]", __func__, tag.c_str(),
                static_cast<unsigned long long>(allocatedSize), static_cast<unsigned long long>(size));
            return HCCL_E_MEMORY;
        }
    } else {
        CHK_RET(HcclEngineCtxCreate(comm, tag.c_str(), COMM_ENGINE_AICPU, size, address));
    }
    CHK_PTR_NULL(*address);
    return HCCL_SUCCESS;
}

HcclResult AllocCcResCtx(HcclComm comm, const std::string& ctxTag, const OpParam& opParam, CcResCtx& ctx, void** result)
{
    if (opParam.varMemSize > std::numeric_limits<size_t>::max() - sizeof(OpParam)) {
        return HCCL_E_PARA;
    }
    ctx.opParamSize = sizeof(OpParam) + opParam.varMemSize;
    std::vector<uint8_t> serializedParam(ctx.opParamSize, 0);
    CHK_SAFETY_FUNC_RET(memcpy_s(serializedParam.data(), serializedParam.size(), &opParam, sizeof(OpParam)));

    const std::string paramTag = ctxTag + "_param";
    void* paramAddress = nullptr;
    CHK_RET(GetOrCreateCcMemory(comm, paramTag, ctx.opParamSize, &paramAddress));
    CHK_RET(HcclEngineCtxCopy(comm, COMM_ENGINE_AICPU, paramTag.c_str(), serializedParam.data(), ctx.opParamSize, 0));
    ctx.opParam = reinterpret_cast<uint64_t>(paramAddress);

    ctx.workSpaceSize = 20ULL * 1024 * 1024;
    void* workspace = nullptr;
    CHK_RET(GetOrCreateCcMemory(comm, ctxTag + "_workspace", ctx.workSpaceSize, &workspace));
    ctx.workSpace = reinterpret_cast<uint64_t>(workspace);

    const std::string resTag = ctxTag + "_ctx";
    void* deviceCtx = nullptr;
    CHK_RET(GetOrCreateCcMemory(comm, resTag, sizeof(CcResCtx), &deviceCtx));
    CHK_RET(HcclEngineCtxCopy(comm, COMM_ENGINE_AICPU, resTag.c_str(), &ctx, sizeof(CcResCtx), 0));
    *result = deviceCtx;
    return HCCL_SUCCESS;
}

HcclResult AcquireCcResCtxAicpu(
    HcclComm comm, uint8_t ccType, const Mc2CcArgs& args, const char* commName, uint32_t rankSize, uint32_t rankId,
    void** ccResCtx, const std::string& ctxTag)
{
    OpParam opParam{};
    CHK_RET(InitCcOpParam(comm, ccType, args, commName, rankSize, opParam));
    CHK_RET(AcquirePreparedCcOpParam(comm, args.algConfig, rankSize, opParam));

    CcResCtx ctx{};
    ctx.rankId = rankId;
    ctx.rankSize = rankSize;
    ctx.opType = ccType;
    ctx.debugMode = args.debugMode;
    void* result = nullptr;
    CHK_RET(AllocCcResCtx(comm, ctxTag, opParam, ctx, &result));

    const std::string srcDataTypeName = GetDataTypeEnumStr(args.srcDataType);
    const std::string dstDataTypeName = GetDataTypeEnumStr(args.dstDataType);
    HCCL_RUN_INFO(
        "[MC2_ALG_INFO] rank[%u], group[%s], opType[%s](%u), algName[%s], "
        "srcDataType[%s](%u), dstDataType[%s](%u), engine[%u].",
        rankId, args.groupName, GetMc2OpTypeName(opParam.opType), static_cast<uint32_t>(opParam.opType),
        opParam.algName, srcDataTypeName.c_str(), static_cast<uint32_t>(args.srcDataType), dstDataTypeName.c_str(),
        static_cast<uint32_t>(args.dstDataType), static_cast<uint32_t>(opParam.engine));
    HCCL_INFO(
        "[Mc2AcquireCcResCtx] allocated: commName[%s], tag[%s], ccResCtx[%p], ccResCtxSize[%llu], "
        "workSpace[%p], workSpaceSize[%llu], opParam[%p], opParamSize[%llu].",
        commName, ctxTag.c_str(), result, static_cast<unsigned long long>(sizeof(CcResCtx)),
        reinterpret_cast<void*>(ctx.workSpace), static_cast<unsigned long long>(ctx.workSpaceSize),
        reinterpret_cast<void*>(ctx.opParam), static_cast<unsigned long long>(ctx.opParamSize));
    *ccResCtx = result;
    return HCCL_SUCCESS;
}

HcclResult DispatchAcquireCcResByCommEngine(
    HcclComm comm, uint8_t ccType, const Mc2CcArgs& args, const char* commName, uint32_t rankSize, uint32_t rankId,
    void** ccResCtx, const std::string& ctxTag)
{
    if (args.commEngine == COMM_ENGINE_AICPU) {
        CHK_RET(AcquireCcResCtxAicpu(comm, ccType, args, commName, rankSize, rankId, ccResCtx, ctxTag));
    } else if (args.commEngine == COMM_ENGINE_CCU) {
        HCCL_ERROR("[%s] CCU resource acquisition is not supported yet", __func__);
        return HCCL_E_NOT_SUPPORT;
    } else {
        HCCL_ERROR("[%s] unsupported commEngine[%u]", __func__, args.commEngine);
        return HCCL_E_NOT_SUPPORT;
    }
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

    // 根据通信参数生成资源 tag，用于查找和复用资源
    const std::string ctxTag = "mc2_cc_" + std::string(args.groupName) + "_" + std::to_string(ccType) + "_" +
                               std::string(args.algConfig) + "_" + std::to_string(args.commEngine);
    if (ctxTag.size() + sizeof("_workspace") - 1 > HCCL_RES_TAG_MAX_LEN) {
        HCCL_ERROR("[%s] resource tag exceeds maximum length", __func__);
        return HCCL_E_PARA;
    }

    void* result = nullptr;
    CHK_RET(DispatchAcquireCcResByCommEngine(comm, ccType, args, commName, rankSize, rankId, &result, ctxTag));
    CHK_RET(LogHcclExit("Mc2AcquireCcResCtx", ctxTag.c_str(), startut));
    *ccResCtx = result;
    *ccResCtxSize = sizeof(CcResCtx);
    return HCCL_SUCCESS;
}

} // namespace

HcclResult __attribute__((visibility("default"))) Mc2AcquireCcResCtx(
    HcclComm comm, uint8_t ccType, void* ccArgs, void** ccResCtx, uint32_t* ccResCtxSize, void* schd)
{
    return Mc2AcquireCcResCtxImpl(comm, ccType, ccArgs, ccResCtx, ccResCtxSize, schd);
}
