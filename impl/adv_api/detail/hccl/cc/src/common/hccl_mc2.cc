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
#include "ccu_assist_pub.h"
#include "hccl_ccu_res.h"
#include "adapter_acl.h"
#include "kfc_server_protocol.h"
#include "hcomm_host_profiling_dl.h"
#include "rt_external.h"
#include "runtime/rt.h"
#include "mmpa_api.h"
#include <cmath>
#include <cstddef>
#include <limits>
#include <memory>

using namespace mc2_ops_hccl;

namespace {
struct Mc2CcArgs {
    static constexpr uint32_t GROUP_NAME_SIZE = 128U;
    static constexpr uint32_t ALG_CONFIG_SIZE = 128U;

    HcclDataType srcDataType;
    HcclDataType dstDataType;
    HcclReduceOp reduceType;
    char groupName[GROUP_NAME_SIZE];
    char algConfig[ALG_CONFIG_SIZE];
    uint8_t commEngine;

    void Init()
    {
        srcDataType = HCCL_DATA_TYPE_FP16;
        dstDataType = HCCL_DATA_TYPE_FP16;
        reduceType = HCCL_REDUCE_SUM;
        groupName[0] = '\0';
    }
};

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

// 公共逻辑：构造topoTag/ctxTag并校验ccTiling参数
HcclResult BuildTagsAndValidate(
    const void* ccTilingList[], uint32_t tilingNum, const char* commName, u32 rankSize, u32 userRank,
    std::string topoTag[], std::string& ctxTag)
{
    for (uint32_t i = 0U; i < tilingNum; ++i) {
        const Mc2CcTilingInner* ccTiling = static_cast<const Mc2CcTilingInner*>(ccTilingList[i]);
        topoTag[i] = std::to_string(ccTiling->opType) + "_" + std::to_string(ccTiling->srcDataType) + "_" +
                     std::string(commName);
        CHK_RET(HcclCheckTag(topoTag[i].c_str()));
        bool isReduce;
        CHK_RET(CheckIsReduce(ccTiling, &isReduce));
        CHK_RET(CheckDataType(static_cast<HcclDataType>(ccTiling->srcDataType), isReduce));

        if (i == 0) {
            ctxTag = std::string(ccTiling->groupName) + "_" + std::to_string(ccTiling->opType) + "_" +
                     std::string(ccTiling->algConfig) + "_" + std::to_string(ccTiling->commEngine);
        } else {
            ctxTag += "_" + std::to_string(ccTiling->opType) + "_" + std::string(ccTiling->algConfig) + "_" +
                      std::to_string(ccTiling->commEngine);
        }
    }
    CHK_RET(HcomCheckUserRank(rankSize, userRank));
    return HCCL_SUCCESS;
}

bool HcclIsCcuAlgorithmRegistered(uint32_t opType, const char* algName)
{
    if (algName == nullptr || algorithmMap.count(algName) == 0U) {
        return false;
    }
    return CollAlgExecRegistryV2::Instance().IsRegistered(static_cast<HcclCMDType>(opType), algName);
}

bool CheckCcuAlgorithmsRegistered(const void* ccTilingList[], uint32_t tilingNum)
{
    HCCL_INFO("[CheckCcuAlgorithmsRegistered]Start CheckCcuAlgorithmsRegistered!");
    if (tilingNum > 1) {
        HCCL_WARNING("[AllocComResourceByTilingCcu] tilingNum[%u] is not supported in mc2_client.", tilingNum);
        return false;
    }
    for (uint32_t i = 0U; i < tilingNum; ++i) {
        const auto* ccTiling = static_cast<const Mc2CcTilingInner*>(ccTilingList[i]);
        std::string algName;
        if (!GetForcedAlgName(ccTiling, algName)) {
            HCCL_WARNING("[AllocComResourceByTilingCcu] algName[%s] is not supported in mc2_client.", algName.c_str());
            return false;
        }
        if (!HcclIsCcuAlgorithmRegistered(ccTiling->opType, algName.c_str())) {
            HCCL_WARNING(
                "[AllocComResourceByTilingCcu] algorithm[%s] is not registered in mc2_client for opType[%u].",
                algName.c_str(), ccTiling->opType);
            return false;
        }
    }
    return true;
}

// AICPU引擎资源分配流程
HcclResult AllocComResourceByTilingAicpu(
    HcclComm comm, void* stream, void* mc2Tiling, const void* ccTilingList[], uint32_t tilingNum, const char* commName,
    u32 rankSize, u32 userRank, void** opResCtx, std::string& ctxTag)
{
    std::string topoTag[Hccl::MC2_MAX_OP_NUM];
    CHK_RET(BuildTagsAndValidate(ccTilingList, tilingNum, commName, rankSize, userRank, topoTag, ctxTag));

    std::vector<OpParam> opParamVec(tilingNum);
    for (uint32_t i = 0U; i < tilingNum; ++i) {
        CHK_RET(
            GetOpParam(comm, stream, topoTag[i], static_cast<const Mc2CcTilingInner*>(ccTilingList[i]), opParamVec[i]));
    }

    CHK_RET(HcclAllocOpResCtx(comm, ctxTag, opParamVec, mc2Tiling, ccTilingList, opResCtx));

    for (uint32_t i = 0U; i < tilingNum; ++i) {
        const Mc2CcTilingInner* ccTiling = static_cast<const Mc2CcTilingInner*>(ccTilingList[i]);
        const OpParam& opParam = opParamVec[i];
        const HcclDataType srcDataType = static_cast<HcclDataType>(ccTiling->srcDataType);
        const HcclDataType dstDataType = static_cast<HcclDataType>(ccTiling->dstDataType);
        const std::string srcDataTypeName = GetDataTypeEnumStr(srcDataType);
        const std::string dstDataTypeName = GetDataTypeEnumStr(dstDataType);
        HCCL_RUN_INFO(
            "[MC2_ALG_INFO] rank[%u], group[%s], opType[%s](%u), algName[%s], "
            "srcDataType[%s](%u), dstDataType[%s](%u), engine[%u].",
            userRank, ccTiling->groupName, GetMc2OpTypeName(opParam.opType), static_cast<uint32_t>(opParam.opType),
            opParam.algName, srcDataTypeName.c_str(), static_cast<uint32_t>(srcDataType), dstDataTypeName.c_str(),
            static_cast<uint32_t>(dstDataType), static_cast<uint32_t>(opParam.engine));
    }

    return HCCL_SUCCESS;
}

// CCU引擎资源分配流程
HcclResult AllocComResourceByTilingCcu(
    HcclComm comm, void* stream, void* mc2Tiling, const void* ccTilingList[], uint32_t tilingNum, const char* commName,
    u32 rankSize, u32 userRank, void** opResCtx, std::string& ctxTag, bool checkOnly = false)
{
    HCCL_INFO("[AllocComResourceByTilingCcu]start AllocComResourceByTilingCcu!");
    std::string topoTag[Hccl::MC2_MAX_OP_NUM];
    CHK_RET(BuildTagsAndValidate(ccTilingList, tilingNum, commName, rankSize, userRank, topoTag, ctxTag));
    HCCL_INFO("[AllocComResourceByTilingCcu]BuildTagsAndValidate successfully!");

    // 构建 OpResCtx 基础字段（workspace、XN、CKE等）
    OpResCtx resCtx{};
    if (checkOnly) {
        resCtx.rankId = userRank;
        resCtx.rankSize = rankSize;
        HCCL_INFO("[AllocComResourceByTilingCcu]checkOnly, skip AllocCcuOpResCtx.");
    } else {
        CHK_RET(AllocCcuOpResCtx(comm, ctxTag, rankSize, userRank, resCtx));
        HCCL_INFO("[AllocComResourceByTilingCcu]AllocCcuOpResCtx successfully!");
        HCCL_INFO(
            "[AllocComResourceByTilingCcu]allocated: workspace[%p], size[%llu]", (void*)resCtx.workSpace,
            resCtx.workSpaceSize);
    }

    // 逐算子选择算法 + 资源准备（executor->CalcRes + GetAlgResCcu）
    CHK_RET(CcuSelectAlg(comm, stream, topoTag, ccTilingList, tilingNum, mc2Tiling, resCtx, checkOnly));
    HCCL_INFO("[AllocComResourceByTilingCcu]CcuSelectAlg successfully!");
    if (checkOnly) {
        HCCL_INFO("[AllocComResourceByTilingCcu]checkOnly, end AllocComResourceByTilingCcu!");
        return HCCL_SUCCESS;
    }

    // 申请OpResCtx硬件内存并写入
    std::string tagOpResCtx = ctxTag + "_opResCtx";
    uint64_t opResCtxSize = sizeof(OpResCtx);
    CHK_RET(GetOrCreateCcuCtx(comm, tagOpResCtx, opResCtxSize, opResCtx));
    aclError aclRet = aclrtMemcpy(*opResCtx, opResCtxSize, &resCtx, opResCtxSize, ACL_MEMCPY_HOST_TO_DEVICE);
    HCCL_INFO(
        "[CCU_DEBUG] opResCtxPtr=%p, *opResCtx=%p, size=%llu ws=0x%llx wsSize=0x%llx xn=0x%llx cke=0x%llx rankId=%llu "
        "rankSize=%llu",
        opResCtx, *opResCtx, opResCtxSize, resCtx.workSpace, resCtx.workSpaceSize, resCtx.xnAddr, resCtx.ckeAddr,
        resCtx.rankId, resCtx.rankSize);
    CHK_RET(aclRet == ACL_ERROR_NONE ? HCCL_SUCCESS : HCCL_E_RUNTIME);
    HCCL_INFO("[AllocComResourceByTilingCcu]end AllocComResourceByTilingCcu!");
    return HCCL_SUCCESS;
}

namespace {
// 按commEngine分发到对应引擎的资源分配流程
HcclResult DispatchAllocByCommEngine(
    HcclComm comm, void* stream, void* mc2Tiling, const void* ccTilingList[], uint32_t tilingNum, const char* commName,
    u32 rankSize, u32 userRank, uint8_t commEngine, void** opResCtx, bool checkOnly, HcclUs startut)
{
    // 根据commEngine类型分发到对应的资源分配流程
    std::string ctxTag;
    if (commEngine == static_cast<uint8_t>(OpExecuteConfig::AICPU_TS)) {
        HCCL_INFO("[HcclAllocComResourceByTiling]commEngine == AICPU_TS!");
        CHK_RET(AllocComResourceByTilingAicpu(
            comm, stream, mc2Tiling, ccTilingList, tilingNum, commName, rankSize, userRank, opResCtx, ctxTag));
    } else if (commEngine == static_cast<uint8_t>(OpExecuteConfig::CCU_SCHED)) {
        HCCL_INFO("[HcclAllocComResourceByTiling]commEngine == CCU_SCHED!");
        if (!CheckCcuAlgorithmsRegistered(ccTilingList, tilingNum)) {
            HCCL_INFO("[HcclAllocComResourceByTiling]Current ccu algorithm is not supported in mc2_client.");
            return HCCL_E_ALG_NOT_SUPPORTED;
        }
        CHK_RET(CheckCcuKfcFlow(mc2Tiling, ccTilingList, tilingNum, rankSize));
        HCCL_INFO("[MC2_DEBUG] before AllocComResourceByTilingCcu.");
        HcclResult ret = AllocComResourceByTilingCcu(
            comm, stream, mc2Tiling, ccTilingList, tilingNum, commName, rankSize, userRank, opResCtx, ctxTag,
            checkOnly);
        HCCL_INFO("[MC2_DEBUG] after AllocComResourceByTilingCcu, ret[%d].", ret);
        CHK_PRT_RET(
            ret != HCCL_SUCCESS,
            HCCL_ERROR("[MC2_CCU_RESOURCE_ALLOC_FAIL] Failed to allocate CCU resource, please check topo information."),
            ret);
    } else {
        HCCL_ERROR("[%s] unsupported commEngine[%u]", __func__, commEngine);
        return HCCL_E_NOT_SUPPORT;
    }

    // 记录退出日志和性能统计信息
    CHK_RET(LogHcclExit("HcclAllocComResourceByTiling", ctxTag.c_str(), startut));
    return HCCL_SUCCESS;
}

// checkOnly为true时走真实资源链探测资源是否充足（供CheckOpResSufficient复用）
HcclResult HcclAllocComResourceByTilingImpl(
    HcclComm comm, void* stream, void* mc2Tiling, void** opResCtx, bool checkOnly = false)
{
    HCCL_RUN_INFO(
        "[MC2_CLIENT_A5] enter asc-devkit common HcclAllocComResourceByTiling, "
        "comm[%p], stream[%p], tiling[%p].",
        comm, stream, mc2Tiling);
    // 记录开始时间，用于性能统计
    HcclUs startut = TIME_NOW();

    // 获取设备类型
    DevType deviceType = DevType::DEV_TYPE_COUNT;
    CHK_RET(hrtGetDeviceType(deviceType));
    // 检查设备类型是否支持新流程，950或910_95支持新流程，其他设备走老流程
    if (deviceType != DevType::DEV_TYPE_950) {
        HCCL_ERROR("[%s] invalid deviceType[%u]", __func__, deviceType);
        return HCCL_E_NOT_SUPPORT;
    }
    // 初始化环境变量配置，解析HCCL相关的环境变量
    // 包括算子展开模式、确定性计算、通信方式、日志开关等配置
    CHK_RET(InitEnvConfig());
    // 检查输入参数的合法性（comm、sendBuf、recvBuf、stream不能为空）
    CHK_RET(CheckInputParam(comm, mc2Tiling, stream));

    // 获取通信域中的rank数量
    u32 rankSize = INVALID_VALUE_RANKSIZE;
    CHK_RET(HcclGetRankSize(comm, &rankSize));

    // 获取当前rank的ID
    u32 userRank = INVALID_VALUE_RANKID;
    CHK_RET(HcclGetRankId(comm, &userRank));

    // 获取通信域名称
    char commName[COMM_INDENTIFIER_MAX_LENGTH];
    CHK_RET(HcclGetCommName(comm, commName));

    const void* ccTilingList[Hccl::MC2_MAX_OP_NUM];
    uint32_t tilingNum;
    CHK_RET(HcclGetTilingList(mc2Tiling, ccTilingList, tilingNum));

    // 校验commengine
    uint8_t commEngine;
    CHK_RET(ObtainCommEngine(ccTilingList, tilingNum, commEngine));

    // onlyCheck场景：单rank必然资源充足、非CCU_SCHED引擎（AICPU_TS等）暂不支持，直接返回SUCCESS
    // （rankSize为通信域属性，对所有tiling一致，此处统一提前返回）
    if (checkOnly && (rankSize == 1 || commEngine != static_cast<uint8_t>(OpExecuteConfig::CCU_SCHED))) {
        HCCL_INFO(
            "[HcclAllocComResourceByTiling]checkOnly, rankSize[%u], commEngine[%u], return SUCCESS.", rankSize,
            commEngine);
        return HCCL_SUCCESS;
    }
    return DispatchAllocByCommEngine(
        comm, stream, mc2Tiling, ccTilingList, tilingNum, commName, rankSize, userRank, commEngine, opResCtx, checkOnly,
        startut);
}

} // namespace

HcclResult __attribute__((visibility("default"))) HcclAllocComResourceByTiling(
    HcclComm comm, void* stream, void* mc2Tiling, void** opResCtx)
{
    return HcclAllocComResourceByTilingImpl(comm, stream, mc2Tiling, opResCtx);
}

HcclResult __attribute__((visibility("default"))) CheckOpResSufficient(HcclComm comm, void* stream, void* mc2Tiling)
{
    void* opResCtx = nullptr;
    HcclResult ret = HcclAllocComResourceByTilingImpl(comm, stream, mc2Tiling, &opResCtx, true);
    HCCL_RUN_INFO(
        "[CheckOpResSufficient] finished, comm[%p], stream[%p], tiling[%p], ret[%d].", comm, stream, mc2Tiling, ret);
    return ret;
}

extern "C" HcclResult __attribute__((visibility("default"))) HcclAllocComResourceByTilingA5Mc2(
    HcclComm comm, void* stream, void* mc2Tiling, void** opResCtx)
{
    HCCL_RUN_INFO(
        "[MC2_CLIENT_A5_AICPU] enter asc-devkit explicit A5 MC2 resource allocator, "
        "comm[%p], stream[%p], tiling[%p].",
        comm, stream, mc2Tiling);
    return HcclAllocComResourceByTilingImpl(comm, stream, mc2Tiling, opResCtx);
}

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
    if (commEngine != static_cast<uint8_t>(OpExecuteConfig::CCU_MS) &&
        commEngine != static_cast<uint8_t>(OpExecuteConfig::CCU_SCHED) &&
        commEngine != static_cast<uint8_t>(OpExecuteConfig::AICPU) &&
        commEngine != static_cast<uint8_t>(OpExecuteConfig::AICPU_TS)) {
        HCCL_ERROR("[%s] commEngine[%u] not supported", __func__, commEngine);
        return HCCL_E_NOT_SUPPORT;
    }

    Mc2CcArgs* ccArgsPtr = static_cast<Mc2CcArgs*>(ccArgs);
    ccArgsPtr->commEngine = commEngine;

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

HcclResult PrepareCcOpParam(
    uint8_t ccType, const Mc2CcArgs& args, const std::string& topoTag, const char* commName, uint32_t rankSize,
    OpParam& opParam)
{
    opParam.opType = static_cast<HcclCMDType>(ccType);
    const bool isReduce =
        opParam.opType == HcclCMDType::HCCL_CMD_ALLREDUCE || opParam.opType == HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
    CHK_RET(CheckDataType(args.srcDataType, isReduce));
    CHK_RET(PrepareOpsCommParam(topoTag, opParam));
    CHK_SAFETY_FUNC_RET(strcpy_s(opParam.commName, sizeof(opParam.commName), commName));
    opParam.stream = nullptr;
    opParam.engine = OpExecuteConfigToCommEngine(args.commEngine);
    opParam.opExecuteConfig = static_cast<OpExecuteConfig>(args.commEngine);
    opParam.commOpExpansionMode = HcclOpExpansionMode::HCCL_OP_EXPANSION_MODE_AI_CPU;
    opParam.reduceType = isReduce ? args.reduceType : HCCL_REDUCE_SUM;

    if (opParam.opType == HcclCMDType::HCCL_CMD_ALLTOALL || opParam.opType == HcclCMDType::HCCL_CMD_ALLTOALLV) {
        opParam.varMemSize = ALL_TO_ALL_V_VECTOR_NUM * rankSize * sizeof(uint64_t);
        opParam.all2AllVDataDes.sendType = args.srcDataType;
        opParam.all2AllVDataDes.recvType = args.dstDataType;
        opParam.all2AllVDataDes.sendCounts = nullptr;
        opParam.all2AllVDataDes.recvCounts = nullptr;
        opParam.all2AllVDataDes.sdispls = nullptr;
        opParam.all2AllVDataDes.rdispls = nullptr;
    } else {
        opParam.DataDes.dataType = args.srcDataType;
        opParam.DataDes.count = 0;
        if (opParam.opType == HcclCMDType::HCCL_CMD_ALLREDUCE) {
            opParam.DataDes.outputType = args.srcDataType;
        }
    }
    return HCCL_SUCCESS;
}

HcclResult Mc2AcquireCcResCtxImpl(HcclComm comm, uint8_t ccType, void* ccArgs, void** ccResCtx, uint32_t* ccResCtxSize)
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

    std::string ctxTag;
    OpParam opParam{};
    void* result = nullptr;
    switch (static_cast<OpExecuteConfig>(args.commEngine)) {
        case OpExecuteConfig::AICPU_TS: {
            const std::string topoTag =
                std::to_string(ccType) + "_" + std::to_string(args.srcDataType) + "_" + commName;
            CHK_RET(HcclCheckTag(topoTag.c_str()));
            ctxTag = std::string(args.groupName) + "_" + std::to_string(ccType) + "_" + args.algConfig + "_" +
                     std::to_string(args.commEngine);
            CHK_RET(PrepareCcOpParam(ccType, args, topoTag, commName, rankSize, opParam));
            CHK_RET(PrepareCcAlgResources(comm, args.algConfig, opParam, true));
            // The resource storage engine is AICPU; the selected execution engine is stored in opParam.
            CHK_RET(HcclAllocOpResCtx(comm, ctxTag, opParam, OpExecuteConfigToCommEngine(args.commEngine), &result));
            break;
        }
        case OpExecuteConfig::CCU_MS:
            HCCL_ERROR("[%s] CCU_MS resource acquisition is not supported", __func__);
            return HCCL_E_NOT_SUPPORT;
        case OpExecuteConfig::CCU_SCHED:
            HCCL_ERROR("[%s] CCU_SCHED resource acquisition is not supported", __func__);
            return HCCL_E_NOT_SUPPORT;
        case OpExecuteConfig::AICPU:
            HCCL_ERROR("[%s] AICPU resource acquisition is not supported", __func__);
            return HCCL_E_NOT_SUPPORT;
        default:
            HCCL_ERROR("[%s] unsupported commEngine[%u]", __func__, args.commEngine);
            return HCCL_E_NOT_SUPPORT;
    }

    const std::string srcDataTypeName = GetDataTypeEnumStr(args.srcDataType);
    const std::string dstDataTypeName = GetDataTypeEnumStr(args.dstDataType);
    HCCL_RUN_INFO(
        "[MC2_ALG_INFO] rank[%u], group[%s], opType[%s](%u), algName[%s], "
        "srcDataType[%s](%u), dstDataType[%s](%u), engine[%u].",
        rankId, args.groupName, GetMc2OpTypeName(opParam.opType), static_cast<uint32_t>(opParam.opType),
        opParam.algName, srcDataTypeName.c_str(), static_cast<uint32_t>(args.srcDataType), dstDataTypeName.c_str(),
        static_cast<uint32_t>(args.dstDataType), static_cast<uint32_t>(opParam.engine));
    CHK_RET(LogHcclExit("Mc2AcquireCcResCtx", ctxTag.c_str(), startut));
    *ccResCtx = result;
    *ccResCtxSize = sizeof(OpResCtx);
    return HCCL_SUCCESS;
}

} // namespace

HcclResult __attribute__((visibility("default"))) Mc2AcquireCcResCtx(
    HcclComm comm, uint8_t ccType, void* ccArgs, void** ccResCtx, uint32_t* ccResCtxSize)
{
    return Mc2AcquireCcResCtxImpl(comm, ccType, ccArgs, ccResCtx, ccResCtxSize);
}

namespace {
CcuResult CopyOpResCtxToHost(const void* opResCtx, OpResCtx& opResHost)
{
    HCCL_INFO("[CcuKernelLaunch]Obtain OpResCtx.");
    aclError aclRet = aclrtMemcpy(&opResHost, sizeof(OpResCtx), opResCtx, sizeof(OpResCtx), ACL_MEMCPY_DEVICE_TO_HOST);
    CHK_PRT_RET(
        aclRet != ACL_SUCCESS,
        HCCL_ERROR(
            "[CcuKernelLaunch] aclrtMemcpy D2H opResCtx failed, ret[%d], src[%p], size[%zu].", aclRet, opResCtx,
            sizeof(OpResCtx)),
        CCU_E_INTERNAL);
    CHK_PRT_RET(
        opResHost.algInfo[0].opParam == 0U,
        HCCL_ERROR("invalid ccu op resource ctx, opParam[%llu].", opResHost.algInfo[0].opParam), CCU_E_PARA);
    CHK_PRT_RET(
        opResHost.workSpace == 0U || opResHost.workSpaceSize == 0U,
        HCCL_ERROR(
            "invalid ccu op resource ctx, workSpace[%llu], workSpaceSize[%llu].", opResHost.workSpace,
            opResHost.workSpaceSize),
        CCU_E_PARA);
    return CCU_SUCCESS;
}

CcuResult CopyOpParamToHost(const OpResCtx& opResHost, OpParam& opParamHost)
{
    HCCL_INFO("[CcuKernelLaunch]Obtain OpParam.");
    void* opParamDev = reinterpret_cast<void*>(opResHost.algInfo[0].opParam);
    aclError aclRet =
        aclrtMemcpy(&opParamHost, sizeof(OpParam), opParamDev, sizeof(OpParam), ACL_MEMCPY_DEVICE_TO_HOST);
    CHK_PRT_RET(
        aclRet != ACL_SUCCESS,
        HCCL_ERROR(
            "[CcuKernelLaunch] aclrtMemcpy D2H OpParam failed, ret[%d], src[%p], size[%zu].", aclRet, opParamDev,
            sizeof(OpParam)),
        CCU_E_INTERNAL);
    CHK_PRT_RET(
        opParamHost.resCtx == nullptr || opParamHost.ctxSize == 0U,
        HCCL_ERROR("invalid ccu op resource ctx, resCtx[%p], ctxSize[%llu].", opParamHost.resCtx, opParamHost.ctxSize),
        CCU_E_PARA);
    return CCU_SUCCESS;
}

CcuResult LoadResourceCtx(const OpParam& opParamHost, AlgResourceCtxSerializable& resourceCtx)
{
    HCCL_INFO("[CcuKernelLaunch]Obtain resCtx.");
    auto* resCtx = static_cast<char*>(opParamHost.resCtx);
    std::vector<char> seq(opParamHost.ctxSize);
    HCCL_INFO("[CcuKernelLaunch]Start aclrtMemcpy D2H.");
    aclError aclRet =
        aclrtMemcpy(seq.data(), opParamHost.ctxSize, resCtx, opParamHost.ctxSize, ACL_MEMCPY_DEVICE_TO_HOST);
    CHK_PRT_RET(
        aclRet != ACL_SUCCESS,
        HCCL_ERROR(
            "[CcuKernelLaunch] aclrtMemcpy D2H failed, ret[%d], dst[%p], src[%p], size[%llu].", aclRet, seq.data(),
            resCtx, opParamHost.ctxSize),
        CCU_E_INTERNAL);
    HCCL_INFO("[CcuKernelLaunch]Start resourceCtx DeSerialize.");
    resourceCtx.DeSerialize(seq);
    return CCU_SUCCESS;
}

CcuResult GetLaunchMissionNum(const AlgResourceCtxSerializable& resourceCtx, uint32_t& missionNum)
{
    CHK_PRT_RET(resourceCtx.threads.empty(), HCCL_ERROR("empty ccu threads"), CCU_E_PARA);
    CHK_PRT_RET(resourceCtx.ccuKernels.empty(), HCCL_ERROR("empty ccu kernels"), CCU_E_PARA);
    CHK_PRT_RET(
        resourceCtx.kfcServerArgSize != KFC_SERVER_ARG_NUM || resourceCtx.kfcServerArgs.size() < KFC_SERVER_ARG_NUM,
        HCCL_ERROR("invalid kfcServerArgs, kfcServerArgSize[%u].", resourceCtx.kfcServerArgSize), CCU_E_PTR);
    missionNum = static_cast<uint32_t>(resourceCtx.kfcServerArgs[KFC_SERVER_MISSION_NUM_ARG_INDEX]);
    CHK_PRT_RET(
        missionNum == 0U || missionNum > KFC_SERVER_MAX_MISSION_NUM ||
            resourceCtx.kfcServerArgs.size() != missionNum * KFC_SERVER_ARG_NUM,
        HCCL_ERROR("invalid mission layout, missionNum[%u], args[%zu]", missionNum, resourceCtx.kfcServerArgs.size()),
        CCU_E_PARA);
    const uint64_t xnAddr = resourceCtx.kfcServerArgs[KFC_SERVER_XN_ADDR_ARG_INDEX];
    const uint64_t ckeAddr = resourceCtx.kfcServerArgs[KFC_SERVER_CKE_ADDR_ARG_INDEX];
    for (uint32_t missionIndex = 0; missionIndex < missionNum; ++missionIndex) {
        const size_t offset = missionIndex * KFC_SERVER_ARG_NUM;
        CHK_PRT_RET(
            resourceCtx.kfcServerArgs[offset + KFC_SERVER_XN_ADDR_ARG_INDEX] != xnAddr ||
                resourceCtx.kfcServerArgs[offset + KFC_SERVER_CKE_ADDR_ARG_INDEX] != ckeAddr ||
                resourceCtx.kfcServerArgs[offset + KFC_SERVER_MISSION_NUM_ARG_INDEX] != missionNum ||
                resourceCtx.kfcServerArgs[offset + KFC_SERVER_MISSION_INDEX_ARG_INDEX] != missionIndex,
            HCCL_ERROR("inconsistent KFC launch args for mission[%u]", missionIndex), CCU_E_PARA);
    }
    CHK_PRT_RET(
        resourceCtx.threads.size() < missionNum || resourceCtx.ccuKernels.size() < missionNum,
        HCCL_ERROR(
            "insufficient launch handles, missionNum[%u], threads[%zu], kernels[%zu]", missionNum,
            resourceCtx.threads.size(), resourceCtx.ccuKernels.size()),
        CCU_E_PARA);
    for (uint32_t missionIndex = 0; missionIndex < missionNum; ++missionIndex) {
        for (uint32_t previousIndex = 0; previousIndex < missionIndex; ++previousIndex) {
            CHK_PRT_RET(
                resourceCtx.threads[missionIndex] == resourceCtx.threads[previousIndex],
                HCCL_ERROR(
                    "missions[%u] and [%u] use duplicate threadHandle[0x%llx]", previousIndex, missionIndex,
                    static_cast<unsigned long long>(resourceCtx.threads[missionIndex])),
                CCU_E_PARA);
        }
    }
    return CCU_SUCCESS;
}

void LogKernelLaunchArgs(
    const AlgResourceCtxSerializable& resourceCtx, uint32_t missionIndex, ThreadHandle threadHandle,
    CcuKernelHandle kernelHandle)
{
    const size_t offset = missionIndex * KFC_SERVER_ARG_NUM;
    if (resourceCtx.kfcServerArgs.size() >= offset + KFC_SERVER_ARG_NUM) {
        HCCL_INFO(
            "[CcuKernelLaunch] HcommCcuKernelLaunch args: "
            "mission[%u], threadHandle[0x%llx], kernelHandle[0x%llx], argSize[%u], "
            "xnAddr[0x%llx], ckeAddr[0x%llx], dieNum[%llu], missionNum[%llu], "
            "missionIndex[%llu], token[%llu]",
            missionIndex, static_cast<unsigned long long>(threadHandle), static_cast<unsigned long long>(kernelHandle),
            resourceCtx.kfcServerArgSize, static_cast<unsigned long long>(resourceCtx.kfcServerArgs[offset]),
            static_cast<unsigned long long>(resourceCtx.kfcServerArgs[offset + 1]),
            static_cast<unsigned long long>(resourceCtx.kfcServerArgs[offset + 2]),
            static_cast<unsigned long long>(resourceCtx.kfcServerArgs[offset + 3]),
            static_cast<unsigned long long>(resourceCtx.kfcServerArgs[offset + 4]),
            static_cast<unsigned long long>(resourceCtx.kfcServerArgs[offset + 5]));
    } else {
        HCCL_INFO(
            "[CcuKernelLaunch] HcommCcuKernelLaunch args: "
            "threadHandle[0x%llx], kernelHandle[0x%llx], argSize[%u], kfcServerArgsSize[%zu]",
            static_cast<unsigned long long>(threadHandle), static_cast<unsigned long long>(kernelHandle),
            resourceCtx.kfcServerArgSize, resourceCtx.kfcServerArgs.size());
    }
}
} // namespace

CcuResult CcuKernelLaunch(const HcclComm comm, void* opResCtx)
{
    CHK_PRT_RET(comm == nullptr, HCCL_ERROR("[%s] comm is nullptr.", __func__), CCU_E_PTR);
    CHK_PRT_RET(opResCtx == nullptr, HCCL_ERROR("[%s] opResCtx is nullptr.", __func__), CCU_E_PTR);

    // HcclEngineCtxCreate分配的OpResCtx、OpParam和序列化资源均位于device，需逐层拷贝到host。
    OpResCtx opResHost{};
    CcuResult ret = CopyOpResCtxToHost(opResCtx, opResHost);
    if (ret != CCU_SUCCESS) {
        return ret;
    }
    OpParam opParamHost{};
    ret = CopyOpParamToHost(opResHost, opParamHost);
    if (ret != CCU_SUCCESS) {
        return ret;
    }
    AlgResourceCtxSerializable resourceCtx;
    ret = LoadResourceCtx(opParamHost, resourceCtx);
    if (ret != CCU_SUCCESS) {
        return ret;
    }
    uint32_t missionNum = 0;
    ret = GetLaunchMissionNum(resourceCtx, missionNum);
    if (ret != CCU_SUCCESS) {
        return ret;
    }
    HCCL_INFO("[CcuKernelLaunch] start [%u] KFC server missions.", missionNum);
    for (uint32_t missionIndex = 0; missionIndex < missionNum; ++missionIndex) {
        const ThreadHandle threadHandle = resourceCtx.threads[missionIndex];
        const CcuKernelHandle kernelHandle = resourceCtx.ccuKernels[missionIndex];
        CHK_PRT_RET(
            threadHandle == 0 || kernelHandle == 0, HCCL_ERROR("invalid launch handles for mission[%u]", missionIndex),
            CCU_E_PARA);
        LogKernelLaunchArgs(resourceCtx, missionIndex, threadHandle, kernelHandle);
        const void* kfcArgs =
            static_cast<const void*>(resourceCtx.kfcServerArgs.data() + missionIndex * KFC_SERVER_ARG_NUM);
        ret = HcommCcuKernelLaunch(threadHandle, kernelHandle, kfcArgs, resourceCtx.kfcServerArgSize);
        if (ret != CCU_SUCCESS) {
            HCCL_ERROR("[CcuKernelLaunch] mission[%u] launch failed, ret[%d]", missionIndex, ret);
            return ret;
        }
    }
    return CCU_SUCCESS;
}

namespace {
constexpr uint16_t MC2_AICPU_HCCL_DEFAULT_TIME = 1836U;
constexpr uint32_t MC2_AICPU_PARAM_LEN = 32U;
constexpr char MC2_AICPU_SERVER_SO_NAME[] = "libmc2_server.so";
constexpr char MC2_AICPU_SERVER_KERNEL_NAME[] = "Mc2ServerKernel";

constexpr uint64_t MC2_AICPU_SIMPLE_CTX_PROTOCOL = 4U;

struct Mc2AicpuHostArgs {
    uint64_t ctxArgs[2];
    char soName[MC2_AICPU_PARAM_LEN];
    char kernelName[MC2_AICPU_PARAM_LEN];
    char opName[MC2_AICPU_PARAM_LEN];
};
static_assert(offsetof(Mc2AicpuHostArgs, soName) == sizeof(uint64_t) * 2U, "ctxArgs must precede soName");

HcclResult Mc2GetAicpuTimeout(uint16_t* time)
{
    CHK_PTR_NULL(time);
    CHK_RET(InitEnvConfig());

    uint32_t opExecuteTimeoutMs = 0;
    if (aclrtGetOpExecuteTimeout(&opExecuteTimeoutMs) != ACL_SUCCESS) {
        HCCL_ERROR("[Mc2GetAicpuTimeout, %s] failed to get op execute timeout.", __func__);
        return HCCL_E_RUNTIME;
    }
    const double opExecuteTimeoutS = static_cast<double>(opExecuteTimeoutMs) / 1000.0;

    double hcclTimeoutS = static_cast<double>(MC2_AICPU_HCCL_DEFAULT_TIME);
    double externalHcclTimeoutS = 0.0;
    if (GetExternalInputExecTimeout(externalHcclTimeoutS)) {
        hcclTimeoutS = externalHcclTimeoutS;
    }

    const double totalTimeoutS = opExecuteTimeoutS + hcclTimeoutS;
    constexpr uint16_t maxLaunchTimeout = std::numeric_limits<uint16_t>::max() - 1U;
    const double maxTimeout = static_cast<double>(maxLaunchTimeout);
    const uint16_t launchTimeout =
        totalTimeoutS >= maxTimeout ? maxLaunchTimeout : static_cast<uint16_t>(std::lround(totalTimeoutS));
    if (totalTimeoutS > maxTimeout) {
        HCCL_WARNING(
            "[Mc2GetAicpuTimeout] timeout[%f]s exceeds max[%u]s, clamp to %u.", totalTimeoutS, maxLaunchTimeout,
            launchTimeout);
    }
    *time = launchTimeout;
    HCCL_INFO(
        "[Mc2GetAicpuTimeout] opExecuteTimeout is %ums, hcclTimeout is %fs, launchTimeout is %us.", opExecuteTimeoutMs,
        hcclTimeoutS, *time);
    return HCCL_SUCCESS;
}

HcclResult Mc2PrepareAicpuHostArgs(Mc2AicpuHostArgs& hostArgs, void* opResCtx)
{
    hostArgs = {};
    hostArgs.ctxArgs[0] = MC2_AICPU_SIMPLE_CTX_PROTOCOL;
    hostArgs.ctxArgs[1] = reinterpret_cast<uint64_t>(opResCtx);

    int32_t soRet = strcpy_s(hostArgs.soName, sizeof(hostArgs.soName), MC2_AICPU_SERVER_SO_NAME);
    int32_t kernelRet = strcpy_s(hostArgs.kernelName, sizeof(hostArgs.kernelName), MC2_AICPU_SERVER_KERNEL_NAME);
    int32_t opRet = strcpy_s(hostArgs.opName, sizeof(hostArgs.opName), MC2_AICPU_SERVER_KERNEL_NAME);
    CHK_PRT_RET(
        (soRet != EOK) || (kernelRet != EOK) || (opRet != EOK),
        HCCL_ERROR(
            "[%s] fill so/kernel/op name failed, soRet[%d], kernelRet[%d], opRet[%d].", __func__, soRet, kernelRet,
            opRet),
        HCCL_E_INTERNAL);
    return HCCL_SUCCESS;
}
} // namespace

HcclResult __attribute__((visibility("default"))) Mc2AicpuKernelLaunch(
    uint32_t blockDim, aclrtStream stream, void* opResCtx, uint64_t opResCtxSize)
{
    CHK_PTR_NULL(opResCtx);
    CHK_PTR_NULL(stream);
    CHK_PRT_RET(
        opResCtxSize < sizeof(OpResCtx),
        HCCL_ERROR(
            "[%s] opResCtxSize[%llu] is smaller than sizeof(OpResCtx)[%zu].", __func__,
            static_cast<unsigned long long>(opResCtxSize), sizeof(OpResCtx)),
        HCCL_E_PARA);

    std::unique_ptr<Mc2AicpuHostArgs> hostArgs = std::make_unique<Mc2AicpuHostArgs>();
    CHK_RET(Mc2PrepareAicpuHostArgs(*hostArgs, opResCtx));

    rtAicpuArgsEx_t aicpuArgs = {};
    aicpuArgs.args = hostArgs.get();
    aicpuArgs.argsSize = static_cast<uint32_t>(sizeof(Mc2AicpuHostArgs));
    aicpuArgs.soNameAddrOffset = static_cast<uint32_t>(offsetof(Mc2AicpuHostArgs, soName));
    aicpuArgs.kernelNameAddrOffset = static_cast<uint32_t>(offsetof(Mc2AicpuHostArgs, kernelName));
    aicpuArgs.isNoNeedH2DCopy = false;

    uint16_t time = 0;
    CHK_RET(Mc2GetAicpuTimeout(&time));
    aicpuArgs.timeout = time;

    const uint32_t launchBlocks = blockDim == 0U ? 1U : blockDim;
    const uint64_t launchBeginTime = HcommGetProfilingSysCycleTime();
    rtError_t rtRet = rtAicpuKernelLaunchExWithArgs(
        KERNEL_TYPE_AICPU_KFC, MC2_AICPU_SERVER_KERNEL_NAME, launchBlocks, &aicpuArgs, nullptr, stream,
        RT_KERNEL_USE_SPECIAL_TIMEOUT);
    CHK_PRT_RET(
        rtRet != RT_ERROR_NONE,
        HCCL_ERROR(
            "[%s] rtAicpuKernelLaunchExWithArgs failed, ret[%d], kernelName[%s], numBlocks[%u], stream[%p].", __func__,
            rtRet, MC2_AICPU_SERVER_KERNEL_NAME, launchBlocks, stream),
        HCCL_E_RUNTIME);

    HcclResult reportRet = HcommProfilingReportKernel(launchBeginTime, MC2_AICPU_SERVER_KERNEL_NAME);
    if (reportRet != HCCL_SUCCESS) {
        HCCL_WARNING(
            "[%s] HcommProfilingReportKernel failed, ret[%d], kernelName[%s].", __func__, reportRet,
            MC2_AICPU_SERVER_KERNEL_NAME);
    }

    HCCL_INFO(
        "[Mc2AicpuKernelLaunch] %s launch successfully, numBlocks is %u, ctx is %p.", MC2_AICPU_SERVER_KERNEL_NAME,
        launchBlocks, opResCtx);
    return HCCL_SUCCESS;
}
