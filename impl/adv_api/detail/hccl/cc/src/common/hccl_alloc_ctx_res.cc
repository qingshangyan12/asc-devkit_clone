/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "hccl_alloc_ctx_res.h"
#include "external_alg_resolver.h"
#include "kfc_server_protocol.h"
#include "mc2_comm_info_reporter.h"

// unified 分层重构（§10.2 A5 灰度）：路由表 + Compile/Materialize 入口。
// 编译期开关 MC2_UNIFIED_RESOURCE 默认关——关时 UseUnifiedPath 恒 false，
// 以下所有 unified 分支零执行（合入零风险）。存量函数体逻辑零修改，只加分叉。
#include "unified_route.h"
#include "l1_adapter.h"        // ResourceBroker / L1Adapter::Compile / Materialize（unified 树 l2/）
#include "plan_serializer.h"   // M2/M3：SerializePlanV3（CCU v3 blob——device 写入方消费）
#include "unified_aicpu_exec.h" // R6 WP-A2：kUnifiedAicpuAlgKey（AICPU exec 分叉识别键回填）

#include <algorithm>            // R11 · WP-2：AssembleUnifiedAicpuResCtx 的 remoteRank 展开序重放（std::sort）
#include <limits>

using namespace mc2_ops_hccl;
using namespace hcomm::CcuRep;

namespace {
// ---------------------------------------------------------------------------
// unified 路径的资源申请（§10.2 A5）——ResourceBroker 的真实现壳。
// 真正的申请经 mc2_ops_hccl::HcclGetAlgResUnified 等（op_common.cc）复用现状
// HcclGetThread/HcclChannelAcquire/CCU kernel 注册链。C1：探测路径资源不足
// 返回 HCCL_E_UNAVAIL 由调用方翻译 HCCL_E_RES_NOT_SUFFICIENT（同现状语义）。
// ---------------------------------------------------------------------------
class UnifiedResourceBroker : public refactor::ResourceBroker {
public:
    UnifiedResourceBroker(HcclComm comm, const OpParam& param)
        : comm_(comm), param_(param)
    {
    }

    HcclResult AcquireThreads(u32 threadNum, const std::vector<u32>& notifyNumPerThread,
                              u32 notifyNumOnMainThread, std::string& err) override;

    HcclResult AcquireChannels(const std::vector<refactor::ChannelReq>& channels, std::string& err) override;

    // M3-条件A（M1 审查，2026-09-21）：kernelHandle 出参——注册时刻唯一回传点
    //（hccl_res.h 无按名反查 API，发射链 CcuKernelLaunch 要求 ccuKernels[0]!=0）
    HcclResult AcquireCcuKernel(const std::vector<refactor::CcuKernelPlan>& kernels,
                                refactor::CcuKernelHandle& kernelHandle, std::string& err) override;

    // R11 · WP-1（句柄回收，2026-10-08）：Materialize 申请的 threads/channels 句柄
    // 收集口——R10 前真实现申请后丢弃（组装资源束无句柄来源）。GetUnifiedOpParam-
    // ResCtxImpl 的 ④ 段取回，经 UnifiedResOut 带给 AICPU 组装段落束
    //（AssembleUnifiedAicpuResCtx）。幂等半边：同 tag 二次调用（CCU marker 跳过/
    // AICPU 无守卫）时 Materialize 重新走真实现——同参数 Acquire 幂等返回同批句柄
    //（comm 级线程/通道表，v2 HcclGetAlgRes 缓存命中同型），收集值被覆盖为同值。
    const std::vector<ThreadHandle>& AcquiredThreads() const { return threads_; }
    const std::vector<ChannelHandle>& AcquiredChannels() const { return channelHandles_; }

private:
    HcclComm comm_;
    const OpParam& param_;
    std::vector<ThreadHandle> threads_;         // R11 · WP-1：AcquireThreads 回收
    std::vector<ChannelHandle> channelHandles_; // R11 · WP-1：AcquireChannels 回收
};

} // namespace

// unified 分叉主流程（AICPU / CCU 共用骨架，定义在下方文件作用域——
// 编译期开关关时整个函数零执行，符号虽外部可见但无调用方）：
// Compile → Materialize（经 UnifiedResourceBroker）→ Plan 序列化进 ctx 旁路。
// 灰度回退语义（≠ C2 快速失败）：分叉任一步失败 → 本函数返回非 SUCCESS，
// 调用方回退老路径完整重走（新代码故障的容错，不是用户配置错的降级）。
// M3-缺口1（主会话裁决，2026-09-21）：unified 编译+注册产物的统一出参——
// ctx = Plan blob 的 device 地址（HcclEngineCtxCreate），kernelHandle = CCU 注册
// 产物（AICPU 引擎恒 0，与 Materialize 出参语义一致；CCU 发射链
// resCtxHost.ccuKernels[0] 的唯一来源）。结构体而非散出参：后续第二件注册产物
//（如 CcuInsHandle）不再改签名。
struct UnifiedResOut {
    void* ctx{nullptr};
    mc2_ops_hccl::refactor::CcuKernelHandle kernelHandle{0};
    // R10 · F1 断点③（ctxSize 尺寸链）：v4 blob 的字节数（= HcclEngineCtxCreate 的
    // 分配尺寸 = SerializePlanV3 槽数×8）。AICPU 半边消费方 RunUnifiedAicpuOp 的 ① 段
    // 按 param.ctxSize 切 u64 数组取回 blob（unified_aicpu_exec.cc:69-79：resCtx 非空
    // 但 ctxSize==0 → E_PARA 拒收）——资源期成功路径必须把该值回填 opParam.ctxSize
    //（:1228 区域，PrepareCcAlgResources AICPU 半边）。
    u64 blobSize{0};
    // R11 · resCtx 双语义修复（2026-10-08）：AICPU 半边资源束产物 + blob 第二通道。
    // resCtx 语义回归资源束（opParam.resCtx 指 AlgResourceCtxSerializable 序列化 ctx，
    // tag=UnifiedAlgTag+"_resctx"，组装段 AssembleUnifiedAicpuResCtx 建立并回填
    // opParam.resCtx/ctxSize——ctx 地址/字节数是组装段局部变量，不经本结构体）。
    // blob 地址/尺寸随资源束序列化字段携带（方案 B，主会话裁决：unifiedBlobAddr/
    // unifiedBlobSize 插 kfcServerArgSize 后、topoInfoSeqSize 前，topo 锚数学不动）。
    // "后续第二件注册产物不再改签名"承诺兑现：仍结构体扩字段，不改函数签名。
    // R11 · WP-1：Materialize 申请句柄（broker 回收）——AICPU 组装段落束来源。
    std::vector<ThreadHandle> acquiredThreads;
    std::vector<ChannelHandle> acquiredChannels;
    // R11 · WP-2：claim 值透传（资源束 notify/slave 字段与申请参数同源）+ 通道
    // 申请的 ChannelReq 输入（组装段 remoteRank 对齐重放的输入——HcclChannel-
    // AcquireUnified 的 desc 展开序数学重放，不重跑 comm 级探测）。
    u32 claimSlaveThreadNum{0};
    u32 claimNotifyNumOnMainThread{0};
    std::vector<u32> claimNotifyNumPerThread;
    std::vector<refactor::ChannelReq> acquiredChannelReqs;
};
HcclResult GetUnifiedOpParamResCtxImpl(
    HcclComm comm, OpParam& opParam, TopoInfoWithNetLayerDetails* topoInfo,
    const std::string& algConfig, UnifiedResOut& resOut);

// op_common.cc 侧的真实现（存量树内新增函数，见 op_common.cc 尾部统一登记）。
// 注：这些函数在 mc2_ops_hccl 命名空间（与存量 op_common 函数同域），
// ChannelReq/CcuKernelPlan/ResourceBroker 在 mc2_ops_hccl::refactor 子域。
namespace mc2_ops_hccl {
HcclResult HcclGetAlgResUnified(HcclComm comm, const OpParam& param, u32 threadNum, u32 notifyNum,
    const std::vector<refactor::ChannelReq>& channels, std::string& err,
    std::vector<ThreadHandle>* threadsOut = nullptr);
HcclResult HcclChannelAcquireUnified(HcclComm comm, const OpParam& param,
    const std::vector<refactor::ChannelReq>& channels, std::string& err,
    std::vector<ChannelHandle>* handlesOut = nullptr);
HcclResult HcclCcuKernelRegisterUnified(HcclComm comm, const OpParam& param,
    const std::vector<refactor::CcuKernelPlan>& kernels, refactor::CcuKernelHandle& kernelHandle,
    std::string& err);
HcclResult HcclSerializeUnifiedPlan(HcclComm comm, const OpParam& param, const std::string& unifiedAlgTag,
    const std::string& planBlob, void** resCtxOut);
} // namespace mc2_ops_hccl

HcclResult FillOpParamAlgName(OpParam& opParam, const std::string& algName);

std::unordered_map<HcclCMDType, OpParamPrepareFunc> opParamPrepareFuncMap = {
    {HcclCMDType::HCCL_CMD_ALLGATHER, PrepareParamForAllGather},
    {HcclCMDType::HCCL_CMD_ALLREDUCE, PrepareParamForAllReduce},
    {HcclCMDType::HCCL_CMD_REDUCE_SCATTER, PrepareParamForReduceScatter},
    {HcclCMDType::HCCL_CMD_ALLTOALL, PrepareParamForAlltoAll},
    {HcclCMDType::HCCL_CMD_ALLTOALLV, PrepareParamForAlltoAllV},
};

CommEngine OpExecuteConfigToCommEngine(uint8_t commEngine)
{
    if (static_cast<OpExecuteConfig>(commEngine) == OpExecuteConfig::AICPU_TS) {
        return COMM_ENGINE_AICPU;
    } else if (static_cast<OpExecuteConfig>(commEngine) == OpExecuteConfig::CCU_SCHED) {
        return COMM_ENGINE_CCU;
    }
    return COMM_ENGINE_RESERVED;
}

HcclResult CheckInputParam(const void* comm, const void* mc2Tiling, const void* stream)
{
    // 检查comm是否为空指针
    RPT_INPUT_ERR(
        comm == nullptr, "EI0003", std::vector<std::string>({"ccl_op", "value", "parameter", "expect"}),
        std::vector<std::string>({"HcclAllocComResourceByTiling", "nullptr", "comm", "non-null pointer"}));
    CHK_PTR_NULL(comm);

    // 检查sendBuf是否为空指针
    RPT_INPUT_ERR(
        mc2Tiling == nullptr, "EI0003", std::vector<std::string>({"ccl_op", "value", "parameter", "expect"}),
        std::vector<std::string>({"HcclAllocComResourceByTiling", "nullptr", "mc2Tiling", "non-null pointer"}));
    CHK_PTR_NULL(mc2Tiling);

    // 检查stream是否为空指针
    RPT_INPUT_ERR(
        stream == nullptr, "EI0003", std::vector<std::string>({"ccl_op", "value", "parameter", "expect"}),
        std::vector<std::string>({"HcclAllocComResourceByTiling", "nullptr", "stream", "non-null pointer"}));
    CHK_PTR_NULL(stream);

    return HCCL_SUCCESS;
}

HcclResult HcclGetTilingList(const void* mc2Tiling, const void* p[], uint32_t& cnt)
{
    CHK_PTR_NULL(mc2Tiling);
    CHK_PTR_NULL(p);
    const u32* versionPtr = static_cast<const u32*>(mc2Tiling);
    const u32 version = *(versionPtr++);
    CHK_PRT_RET(version < MC2_TILING_VERSION, HCCL_ERROR("Invalid tiling version %u.", version), HCCL_E_PARA);

    cnt = *(versionPtr++);
    CHK_PRT_RET(cnt == 0U || cnt > MAX_HCOM_NUM, HCCL_ERROR("Invalid hcom tiling number %u.", cnt), HCCL_E_PARA);

    constexpr uint32_t offsetTableEnd = sizeof(uint32_t) * (2U + MAX_CC_TILING_NUM);
    for (uint32_t i = 0U; i < cnt; ++i) {
        const uint32_t offset = versionPtr[i];
        CHK_PRT_RET(
            offset < offsetTableEnd || offset % alignof(Mc2CcTilingInner) != 0U,
            HCCL_ERROR("Invalid hcom tiling offset %u at index %u.", offset, i), HCCL_E_PARA);
        p[i] = reinterpret_cast<const void*>(reinterpret_cast<const u8*>(mc2Tiling) + offset);
    }
    HCCL_INFO("HcclGetTilingList version[%u] cnt[%u]", version, cnt);
    return HCCL_SUCCESS;
}

HcclResult CheckIsReduce(const Mc2CcTilingInner* ccTiling, bool* isReduce)
{
    CHK_PTR_NULL(ccTiling);
    CHK_PTR_NULL(isReduce);
    if (ccTiling->opType == HcclCMDType::HCCL_CMD_REDUCE_SCATTER || ccTiling->opType == HcclCMDType::HCCL_CMD_REDUCE ||
        ccTiling->opType == HcclCMDType::HCCL_CMD_ALLREDUCE) {
        *isReduce = true;
    } else {
        *isReduce = false;
    }
    return HCCL_SUCCESS;
}

HcclResult CheckCommEngine(const void* ccTilingList[], uint32_t tilingNum)
{
    CHK_PTR_NULL(ccTilingList);
    CHK_PRT_RET(
        tilingNum == 0U || tilingNum > MAX_HCOM_NUM, HCCL_ERROR("Invalid hcom tiling number %u.", tilingNum),
        HCCL_E_PARA);
    for (uint32_t i = 0U; i < tilingNum; ++i) {
        const Mc2CcTilingInner* ccTiling = static_cast<const Mc2CcTilingInner*>(ccTilingList[i]);
        CHK_PTR_NULL(ccTiling);
        if (ccTiling->commEngine != static_cast<uint8_t>(COMM_ENGINE_AICPU) &&
            ccTiling->commEngine != static_cast<uint8_t>(COMM_ENGINE_CPU)) {
            HCCL_ERROR("Invalid commEngine %u.", ccTiling->commEngine);
            return HCCL_E_NOT_SUPPORT;
        }
    }
    return HCCL_SUCCESS;
}

HcclResult ObtainCommEngine(const void* ccTilingList[], uint32_t tilingNum, uint8_t& commEngine)
{
    CHK_PTR_NULL(ccTilingList);
    CHK_PRT_RET(
        tilingNum == 0U || tilingNum > MAX_HCOM_NUM, HCCL_ERROR("Invalid hcom tiling number %u.", tilingNum),
        HCCL_E_PARA);
    commEngine = static_cast<uint8_t>(OpExecuteConfig::DEFAULT);
    for (uint32_t i = 0U; i < tilingNum; ++i) {
        const Mc2CcTilingInner* ccTiling = static_cast<const Mc2CcTilingInner*>(ccTilingList[i]);
        CHK_PTR_NULL(ccTiling);
        if (commEngine == static_cast<uint8_t>(OpExecuteConfig::DEFAULT)) {
            commEngine = ccTiling->commEngine;
        }
        if (ccTiling->commEngine != commEngine) {
            HCCL_ERROR("Invalid commEngine %u.", ccTiling->commEngine);
            return HCCL_E_NOT_SUPPORT;
        }
    }

    if (commEngine != static_cast<uint8_t>(OpExecuteConfig::AICPU_TS) &&
        commEngine != static_cast<uint8_t>(OpExecuteConfig::CCU_SCHED)) {
        HCCL_ERROR("Invalid commEngine %u.", commEngine);
        return HCCL_E_NOT_SUPPORT;
    }
    return HCCL_SUCCESS;
}

namespace {
bool IsCcuKfcSupportedOp(uint32_t opType)
{
    return opType == static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER) ||
           opType == static_cast<uint32_t>(HcclCMDType::HCCL_CMD_REDUCE_SCATTER) ||
           opType == static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLTOALL) ||
           opType == static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLTOALLV) ||
           opType == static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLREDUCE);
}
} // namespace

HcclResult CheckCcuKfcFlow(const void* mc2Tiling, const void* ccTilingList[], uint32_t tilingNum, uint32_t rankSize)
{
    CHK_PTR_NULL(mc2Tiling);
    CHK_PTR_NULL(ccTilingList);
    const auto* initTiling = static_cast<const Mc2InitTilingInner*>(mc2Tiling);
    if (initTiling->version != INIT_TILING_CCU_NEW_VERSION) {
        HCCL_ERROR("Invalid CCU KFC tiling version %u, expected %u.", initTiling->version, INIT_TILING_CCU_NEW_VERSION);
        return HCCL_E_NOT_SUPPORT;
    }
    if (tilingNum == 0U || tilingNum > MAX_CC_TILING_NUM || tilingNum != initTiling->mc2HcommCnt) {
        HCCL_ERROR("Invalid CCU KFC tiling number %u, init tiling number %u.", tilingNum, initTiling->mc2HcommCnt);
        return HCCL_E_NOT_SUPPORT;
    }
    uint32_t expectedOpType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_INVALID);
    for (uint32_t i = 0U; i < tilingNum; ++i) {
        const auto* ccTiling = static_cast<const Mc2CcTilingInner*>(ccTilingList[i]);
        CHK_PTR_NULL(ccTiling);
        if (std::strcmp(ccTiling->algConfig, KFC_REDUCE_SCATTER_PEER_ONLY_ALG_NAME) == 0 && rankSize != 2U) {
            HCCL_ERROR(
                "Peer-only ReduceScatter requires rankSize 2, but got rankSize %u at tiling index %u.", rankSize, i);
            return HCCL_E_NOT_SUPPORT;
        }
        if (expectedOpType == static_cast<uint32_t>(HcclCMDType::HCCL_CMD_INVALID)) {
            expectedOpType = ccTiling->opType;
        }
        if (!IsCcuKfcSupportedOp(ccTiling->opType) ||
            ccTiling->commEngine != static_cast<uint8_t>(OpExecuteConfig::CCU_SCHED) ||
            ccTiling->opType != expectedOpType) {
            HCCL_ERROR(
                "Unsupported CCU KFC tiling at index %u, opType %u, commEngine %u.", i, ccTiling->opType,
                ccTiling->commEngine);
            return HCCL_E_NOT_SUPPORT;
        }
    }
    return HCCL_SUCCESS;
}

// 构建 opResCtx 基础字段
HcclResult AllocCcuOpResCtx(HcclComm comm, const std::string& ctxTag, u32 rankSize, u32 userRank, OpResCtx& opResCtx)
{
    // 1. 分配workspace、scratch、comParam（XN）、comSync（CKE）
    constexpr uint32_t comSyncNum = KFC_SERVER_SIGNAL_REGION_NUM;
    constexpr uint64_t scratchSize = 8 * Hccl::MC2_WORKSPACE_SIZE; // 调试: 64MB->128MB，与AIV侧AR chunk对齐
    uint64_t comParamBufSize = Hccl::CCU_TASK_NUM_MAX * Hccl::CCU_PARAM_NUM_MAX * Hccl::CCU_ONE_PARAM_SIZE;
    uint64_t comSyncBufSize = Hccl::CCU_TASK_NUM_MAX * comSyncNum * Hccl::CCU_ONE_PARAM_SIZE;

    std::string tagWorkspace = ctxTag + "_ccu_workspace";
    std::string tagScratch = ctxTag + "_ccu_scratch";
    std::string tagComParam = ctxTag + "_ccu_comParam";
    std::string tagComSync = ctxTag + "_ccu_comSync";

    void* workspacePtr = nullptr;
    void* scratchPtr = nullptr;
    void* comParamPtr = nullptr;
    void* comSyncPtr = nullptr;

    CHK_RET(GetOrCreateCcuCtx(comm, tagWorkspace, Hccl::MC2_WORKSPACE_SIZE, &workspacePtr));
    CHK_RET(GetOrCreateCcuCtx(comm, tagScratch, scratchSize, &scratchPtr));
    CHK_RET(GetOrCreateCcuCtx(comm, tagComParam, comParamBufSize, &comParamPtr));
    CHK_RET(GetOrCreateCcuCtx(comm, tagComSync, comSyncBufSize, &comSyncPtr));

    uint64_t ckeTokenInfo = hcomm::CcuRep::GetTokenInfo(reinterpret_cast<uint64_t>(comSyncPtr), comSyncBufSize);
    HCCL_INFO(
        "[CcuTokenTrace][ComSync] ckeAddr[0x%llx], size[%llu], token[0x%llx]",
        reinterpret_cast<unsigned long long>(comSyncPtr), static_cast<unsigned long long>(comSyncBufSize),
        static_cast<unsigned long long>(ckeTokenInfo));

    opResCtx.workSpace = reinterpret_cast<uint64_t>(workspacePtr);
    opResCtx.workSpaceSize = Hccl::MC2_WORKSPACE_SIZE;
    opResCtx.commEngine = static_cast<uint64_t>(COMM_ENGINE_CCU);
    opResCtx.rankId = userRank;
    opResCtx.rankSize = rankSize;
    opResCtx.xnAddr = reinterpret_cast<uint64_t>(comParamPtr);
    opResCtx.ckeAddr = reinterpret_cast<uint64_t>(comSyncPtr);
    // ReduceScatter KFC follows pr_4523 and partitions this fixed 16 MiB buffer as rankSize * sliceSize.
    opResCtx.res[0] = reinterpret_cast<uint64_t>(scratchPtr);

    HCCL_INFO(
        "[AllocCcuOpResCtx] workspace[%p], workspaceSize[%llu], scratch[%p], scratchSize[%llu], "
        "comParam[%p], comParamBufSize[%llu], comSync[%p], comSyncBufSize[%llu], "
        "rankId[%u], rankSize[%u], xnAddr[0x%llx], ckeAddr[0x%llx]",
        workspacePtr, Hccl::MC2_WORKSPACE_SIZE, scratchPtr, scratchSize, comParamPtr, comParamBufSize, comSyncPtr,
        comSyncBufSize, userRank, rankSize, static_cast<unsigned long long>(opResCtx.xnAddr),
        static_cast<unsigned long long>(opResCtx.ckeAddr));

    return HCCL_SUCCESS;
}

namespace {
HcclResult AllocOpParamMemory(
    HcclComm comm, const std::string& ctxTag, const OpParam& opParam, CommEngine commEngine, uint32_t index,
    OpResCtx& resCtx)
{
    uint64_t opParamAddr = 0;
    std::vector<uint8_t> serializedOpParam;
    CHK_PRT_RET(
        opParam.varMemSize > std::numeric_limits<size_t>::max() - sizeof(OpParam),
        HCCL_ERROR(
            "OpParam[%u] varMemSize[%llu] is too large.", index, static_cast<unsigned long long>(opParam.varMemSize)),
        HCCL_E_PARA);
    const size_t opParamSize = sizeof(OpParam) + static_cast<size_t>(opParam.varMemSize);
    serializedOpParam.assign(opParamSize, 0U);
    CHK_SAFETY_FUNC_RET(memcpy_s(serializedOpParam.data(), serializedOpParam.size(), &opParam, sizeof(OpParam)));

    std::string tagParam = ctxTag + "_" + std::to_string(index);
    void* opParamPtr = nullptr;
    uint64_t allocatedOpParamSize = opParamSize;
    if (HcclEngineCtxGet(comm, tagParam.c_str(), commEngine, &opParamPtr, &allocatedOpParamSize) == HCCL_SUCCESS) {
        CHK_PRT_RET(
            allocatedOpParamSize < opParamSize,
            HCCL_ERROR(
                "OpParam[%u] size[%llu] exceeds capacity[%llu].", index, static_cast<unsigned long long>(opParamSize),
                static_cast<unsigned long long>(allocatedOpParamSize)),
            HCCL_E_MEMORY);
        HCCL_INFO(
            "HcclEngineCtxGet success, tagParam[%s], opParamAddr[%p], opParamSize[%llu]", tagParam.c_str(), opParamPtr,
            static_cast<unsigned long long>(allocatedOpParamSize));
        opParamAddr = reinterpret_cast<uint64_t>(opParamPtr);
    } else {
        CHK_RET(HcclEngineCtxCreate(comm, tagParam.c_str(), commEngine, opParamSize, &opParamPtr));
        opParamAddr = reinterpret_cast<uint64_t>(opParamPtr);
    }
    HCCL_INFO(
        "HcclAllocOpResCtx the %uth opParam: opParamAddr[%llu], opParamSize[%llu]", index,
        static_cast<unsigned long long>(opParamAddr), static_cast<unsigned long long>(opParamSize));

    CHK_RET(HcclEngineCtxCopy(comm, commEngine, tagParam.c_str(), serializedOpParam.data(), opParamSize, 0));
    resCtx.algInfo[index].opParam = opParamAddr;
    resCtx.opParamSize[index] = opParamSize;
    return HCCL_SUCCESS;
}

HcclResult AllocWorkspaceMemory(HcclComm comm, const std::string& ctxTag, CommEngine commEngine, OpResCtx& resCtx)
{
    uint64_t memSize = 20 * 1024 * 1024;
    resCtx.workSpaceSize = memSize;
    std::string tagWorkSpace = ctxTag + "_workSpace";
    void* workSpacePtr = nullptr;
    if (HcclEngineCtxGet(comm, tagWorkSpace.c_str(), commEngine, &workSpacePtr, &memSize) == HCCL_SUCCESS) {
        HCCL_INFO(
            "HcclEngineCtxGet success, tagWorkSpace[%s], workSpaceAddr[%p], workSpaceSize[%u]", tagWorkSpace.c_str(),
            workSpacePtr, memSize);
        resCtx.workSpace = reinterpret_cast<uint64_t>(workSpacePtr);
    } else {
        CHK_RET(HcclEngineCtxCreate(comm, tagWorkSpace.c_str(), commEngine, memSize, &workSpacePtr));
        resCtx.workSpace = reinterpret_cast<uint64_t>(workSpacePtr);
    }
    HCCL_INFO("HcclAllocOpResCtx the workSpace: workSpaceAddr[%u], workSpaceSize[%u]", resCtx.workSpace, memSize);
    return HCCL_SUCCESS;
}

HcclResult FillRankInfo(HcclComm comm, OpResCtx& resCtx)
{
    uint32_t rankSize = 0U;
    uint32_t rankId = 0U;
    CHK_RET(HcclGetRankSize(comm, &rankSize));
    CHK_RET(HcclGetRankId(comm, &rankId));
    resCtx.rankSize = rankSize;
    resCtx.rankId = rankId;
    return HCCL_SUCCESS;
}

HcclResult AllocAndCopyOpResCtx(
    HcclComm comm, const std::string& ctxTag, CommEngine commEngine, const OpResCtx& resCtx, void** opResCtxPtr)
{
    std::string tagOpResCtx = ctxTag + "_opResCtx";
    constexpr uint64_t opResCtxSize = sizeof(OpResCtx);
    uint64_t allocatedSize = opResCtxSize;
    if (HcclEngineCtxGet(comm, tagOpResCtx.c_str(), commEngine, opResCtxPtr, &allocatedSize) == HCCL_SUCCESS) {
        CHK_PRT_RET(
            allocatedSize < opResCtxSize,
            HCCL_ERROR(
                "OpResCtx size[%llu] exceeds capacity[%llu].", static_cast<unsigned long long>(opResCtxSize),
                static_cast<unsigned long long>(allocatedSize)),
            HCCL_E_MEMORY);
        HCCL_INFO(
            "HcclEngineCtxGet success, tagOpResCtx[%s], opResCtxAddr[%p], allocatedSize[%llu]", tagOpResCtx.c_str(),
            opResCtxPtr, static_cast<unsigned long long>(allocatedSize));
    } else {
        CHK_RET(HcclEngineCtxCreate(comm, tagOpResCtx.c_str(), commEngine, opResCtxSize, opResCtxPtr));
    }

    HCCL_INFO(
        "HcclAllocOpResCtx the opResCtx: opResCtxAddr[%p], opResCtxSize[%llu]", opResCtxPtr,
        static_cast<unsigned long long>(opResCtxSize));

    CHK_RET(HcclEngineCtxCopy(comm, commEngine, tagOpResCtx.c_str(), &resCtx, opResCtxSize, 0));
    return HCCL_SUCCESS;
}
} // namespace

HcclResult HcclAllocOpResCtx(
    const HcclComm comm, const std::string& ctxTag, const std::vector<OpParam>& opParamVec, const void* mc2Tiling,
    const void* ccTilingList[], void** opResCtxPtr)
{
    CHK_PTR_NULL(opResCtxPtr);
    OpResCtx resCtx{};
    const auto* initTiling = static_cast<const Mc2InitTilingInner*>(mc2Tiling);
    const auto* ccTiling = static_cast<const Mc2CcTilingInner*>(ccTilingList[0]);

    for (uint32_t i = 0U; i < opParamVec.size(); ++i) {
        const auto* opTiling = static_cast<const Mc2CcTilingInner*>(ccTilingList[i]);
        CHK_RET(AllocOpParamMemory(
            comm, ctxTag, opParamVec[i], OpExecuteConfigToCommEngine(opTiling->commEngine), i, resCtx));
        resCtx.algInfo[i].offset = initTiling->offset[i];
        resCtx.opType[i] = opTiling->opType;
    }
    const CommEngine commEngine = OpExecuteConfigToCommEngine(ccTiling->commEngine);
    resCtx.commEngine = static_cast<uint64_t>(commEngine);
    CHK_RET(AllocWorkspaceMemory(comm, ctxTag, commEngine, resCtx));
    CHK_RET(FillRankInfo(comm, resCtx));
    return AllocAndCopyOpResCtx(comm, ctxTag, commEngine, resCtx, opResCtxPtr);
}

HcclResult HcclAllocOpResCtx(
    HcclComm comm, const std::string& ctxTag, const OpParam& opParam, CommEngine commEngine, void** opResCtxPtr)
{
    CHK_PTR_NULL(opResCtxPtr);
    OpResCtx resCtx{};
    resCtx.commEngine = commEngine;
    CHK_RET(AllocOpParamMemory(comm, ctxTag, opParam, commEngine, 0, resCtx));
    resCtx.opType[0] = static_cast<uint32_t>(opParam.opType);
    CHK_RET(AllocWorkspaceMemory(comm, ctxTag, commEngine, resCtx));
    CHK_RET(FillRankInfo(comm, resCtx));
    return AllocAndCopyOpResCtx(comm, ctxTag, commEngine, resCtx, opResCtxPtr);
}

// AllToAll适配AllToAllV
HcclResult ConvertAlltoAllParam(
    const u64 recvCount, const u32 rankSize, std::vector<u64>& sdispls, std::vector<u64>& rdispls)
{
    CHK_PRT_RET(
        sdispls.size() < rankSize || rdispls.size() < rankSize,
        HCCL_ERROR("[ConvertAlltoAllParam] invalid rankSize[%u].", rankSize), HCCL_E_PARA);
    u64 dataCountOffset = 0;
    for (u64 i = 0; i < rankSize; i++) {
        sdispls[i] = dataCountOffset;
        rdispls[i] = dataCountOffset;
        dataCountOffset += recvCount;
    }
    return HCCL_SUCCESS;
}

HcclResult PrepareOpsCommParam(const std::string& tag, OpParam& param)
{
    param.opMode = OpMode::OPBASE;
    DevType deviceType = DevType::DEV_TYPE_COUNT;
    CHK_RET(hrtGetDeviceType(deviceType));

    // topoInfo的tag，所有相同的算子可以共享
    int ret = sprintf_s(param.tag, sizeof(param.tag), "%s", tag.c_str());
    if (ret <= 0) {
        HCCL_ERROR("failed to fill param.tag");
        return HCCL_E_INTERNAL;
    }

    param.inputPtr = nullptr;
    param.outputPtr = nullptr;
    param.inputSize = 0;
    param.outputSize = 0;
    param.enableDetour = false;
    param.deviceType = deviceType;

    return HCCL_SUCCESS;
}

HcclResult PrintOpsCommParam(OpParam& param)
{
    HCCL_INFO("commName: %s", param.commName);
    HCCL_INFO("tag:%s", param.tag);
    HCCL_INFO("stream: %p", param.stream);
    HCCL_INFO("inputPtr %p", param.inputPtr);
    HCCL_INFO("outputPtr %p", param.outputPtr);
    HCCL_INFO("inputSize %lu", param.inputSize);
    HCCL_INFO("outputSize %lu", param.outputSize);
    HCCL_INFO("opMode %u", static_cast<uint32_t>(param.opMode));
    HCCL_INFO("deviceType %u", static_cast<uint32_t>(param.deviceType));
    return HCCL_SUCCESS;
}

HcclResult PrepareParamForAllGather(
    HcclComm comm, const std::string& tag, const Mc2CcTilingInner* ccTiling, OpParam& param)
{
    HCCL_INFO("PrepareParamForAllGather, ccTiling[%p]", ccTiling);
    u32 userRankSize;
    CHK_RET(HcclGetRankSize(comm, &userRankSize)); // 加上，后面计算outputSize可能使用

    HcclResult ret = PrepareOpsCommParam(tag, param);
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("failed to fill OpsCommParam");
    }

    param.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    param.reduceType = HcclReduceOp::HCCL_REDUCE_SUM;
    param.DataDes.dataType = static_cast<HcclDataType>(ccTiling->srcDataType);
    param.DataDes.outputType = static_cast<HcclDataType>(ccTiling->dstDataType);
    param.DataDes.count = 0;

    HCCL_INFO("Print PrepareParamForAllGather.");
    CHK_RET(PrintOpsCommParam(param));
    HCCL_INFO("opType %u", static_cast<uint32_t>(param.opType));
    HCCL_INFO("DataDes.dataType %u", static_cast<uint32_t>(param.DataDes.dataType));
    HCCL_INFO("DataDes.outputType %u", static_cast<uint32_t>(param.DataDes.outputType));
    HCCL_INFO("DataDes.count %lu", param.DataDes.count);
    HCCL_INFO("Execute PrepareParamForAllGather success.");
    return HCCL_SUCCESS;
}

HcclResult PrepareParamForAllReduce(
    HcclComm comm, const std::string& tag, const Mc2CcTilingInner* ccTiling, OpParam& param)
{
    HCCL_INFO("PrepareParamForAllReduce, ccTiling[%p]", ccTiling);
    u32 userRankSize;
    CHK_RET(HcclGetRankSize(comm, &userRankSize)); // 加上，后面计算outputSize可能使用

    HcclResult ret = PrepareOpsCommParam(tag, param);
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("failed to fill OpsCommParam");
    }

    param.opType = HcclCMDType::HCCL_CMD_ALLREDUCE;
    param.reduceType = static_cast<HcclReduceOp>(ccTiling->reduceType);
    param.DataDes.dataType = static_cast<HcclDataType>(ccTiling->srcDataType);
    param.DataDes.outputType = param.DataDes.dataType;
    param.DataDes.count = 0;

    HCCL_INFO("Print PrepareParamForAllReduce.");
    CHK_RET(PrintOpsCommParam(param));
    HCCL_INFO("opType %u", static_cast<uint32_t>(param.opType));
    HCCL_INFO("reduceType %u", static_cast<uint32_t>(param.reduceType));
    HCCL_INFO("DataDes.dataType %u", static_cast<uint32_t>(param.DataDes.dataType));
    HCCL_INFO("DataDes.outputType %u", static_cast<uint32_t>(param.DataDes.outputType));
    HCCL_INFO("DataDes.count %lu", param.DataDes.count);
    HCCL_INFO("Execute PrepareParamForAllReduce success.");
    return HCCL_SUCCESS;
}

HcclResult PrepareParamForReduceScatter(
    HcclComm comm, const std::string& tag, const Mc2CcTilingInner* ccTiling, OpParam& param)
{
    HCCL_INFO("PrepareParamForReduceScatter, ccTiling[%p]", ccTiling);
    u32 userRankSize;
    CHK_RET(HcclGetRankSize(comm, &userRankSize)); // 加上，后面计算outputSize可能使用

    HcclResult ret = PrepareOpsCommParam(tag, param);
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("failed to fill OpsCommParam");
    }

    param.opType = HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
    param.reduceType = static_cast<HcclReduceOp>(ccTiling->reduceType);
    param.DataDes.dataType = static_cast<HcclDataType>(ccTiling->srcDataType);
    param.DataDes.outputType = static_cast<HcclDataType>(ccTiling->dstDataType);
    param.DataDes.count = 0;

    HCCL_INFO("Print PrepareParamForReduceScatter.");
    CHK_RET(PrintOpsCommParam(param));
    HCCL_INFO("opType %u", static_cast<uint32_t>(param.opType));
    HCCL_INFO("reduceType %u", static_cast<uint32_t>(param.reduceType));
    HCCL_INFO("DataDes.dataType %u", static_cast<uint32_t>(param.DataDes.dataType));
    HCCL_INFO("DataDes.outputType %u", static_cast<uint32_t>(param.DataDes.outputType));
    HCCL_INFO("DataDes.count %lu", param.DataDes.count);
    HCCL_INFO("Execute PrepareParamForReduceScatter success.");
    return HCCL_SUCCESS;
}

HcclResult PrepareParamForAlltoAll(
    HcclComm comm, const std::string& tag, const Mc2CcTilingInner* ccTiling, OpParam& param)
{
    HCCL_INFO("PrepareParamForAlltoAll, ccTiling[%p]", ccTiling);
    u32 userRankSize;
    CHK_RET(HcclGetRankSize(comm, &userRankSize));

    HcclResult ret = PrepareOpsCommParam(tag, param);
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("failed to fill OpsCommParam");
    }

    u64 varMemSize = ALL_TO_ALL_V_VECTOR_NUM * userRankSize * sizeof(u64);
    param.varMemSize = varMemSize;
    param.opType = HcclCMDType::HCCL_CMD_ALLTOALL;
    param.reduceType = HcclReduceOp::HCCL_REDUCE_SUM;
    param.all2AllVDataDes.sendType = static_cast<HcclDataType>(ccTiling->srcDataType);
    param.all2AllVDataDes.recvType = static_cast<HcclDataType>(ccTiling->dstDataType);
    param.all2AllVDataDes.sendCounts = nullptr;
    param.all2AllVDataDes.recvCounts = nullptr;
    param.all2AllVDataDes.sdispls = nullptr;
    param.all2AllVDataDes.rdispls = nullptr;

    HCCL_INFO("Print PrepareParamForAlltoAll.");
    CHK_RET(PrintOpsCommParam(param));
    HCCL_INFO("varMemSize %lu", param.varMemSize);
    HCCL_INFO("opType %u", static_cast<uint32_t>(param.opType));
    HCCL_INFO("all2AllVDataDes.sendType %u", static_cast<uint32_t>(param.all2AllVDataDes.sendType));
    HCCL_INFO("all2AllVDataDes.recvType %u", static_cast<uint32_t>(param.all2AllVDataDes.recvType));
    HCCL_INFO("Execute PrepareParamForAlltoAll success.");
    return HCCL_SUCCESS;
}

HcclResult PrepareParamForAlltoAllV(
    HcclComm comm, const std::string& tag, const Mc2CcTilingInner* ccTiling, OpParam& param)
{
    HCCL_INFO("PrepareParamForAlltoAllV, ccTiling[%p]", ccTiling);
    u32 userRankSize;
    CHK_RET(HcclGetRankSize(comm, &userRankSize));

    HcclResult ret = PrepareOpsCommParam(tag, param);
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("failed to fill OpsCommParam");
    }

    u64 varMemSize = ALL_TO_ALL_V_VECTOR_NUM * userRankSize * sizeof(u64);
    param.varMemSize = varMemSize;
    param.opType = HcclCMDType::HCCL_CMD_ALLTOALLV;
    param.reduceType = HcclReduceOp::HCCL_REDUCE_SUM;
    param.all2AllVDataDes.sendType = static_cast<HcclDataType>(ccTiling->srcDataType);
    param.all2AllVDataDes.recvType = static_cast<HcclDataType>(ccTiling->dstDataType);
    param.all2AllVDataDes.sendCounts = nullptr;
    param.all2AllVDataDes.recvCounts = nullptr;
    param.all2AllVDataDes.sdispls = nullptr;
    param.all2AllVDataDes.rdispls = nullptr;

    HCCL_INFO("Print PrepareParamForAlltoAllV.");
    CHK_RET(PrintOpsCommParam(param));
    HCCL_INFO("varMemSize %lu", param.varMemSize);
    HCCL_INFO("opType %u", static_cast<uint32_t>(param.opType));
    HCCL_INFO("all2AllVDataDes.sendType %u", static_cast<uint32_t>(param.all2AllVDataDes.sendType));
    HCCL_INFO("all2AllVDataDes.recvType %u", static_cast<uint32_t>(param.all2AllVDataDes.recvType));
    return HCCL_SUCCESS;
}

HcclResult PrepareOpParams(HcclComm comm, const std::string& tag, const Mc2CcTilingInner* ccTiling, OpParam& opParam)
{
    auto it = opParamPrepareFuncMap.find(static_cast<HcclCMDType>(ccTiling->opType));
    if (it != opParamPrepareFuncMap.end()) {
        return it->second(comm, tag, ccTiling, opParam);
    }
    HCCL_ERROR("PrepareOpParams error, opType[%d] not found", ccTiling->opType);
    return HCCL_E_INTERNAL;
}

HcclResult InitOpParamByTiling(
    HcclComm comm, void* stream, const std::string& tag, const Mc2CcTilingInner* ccTiling, OpParam& opParam)
{
    opParam.opType = static_cast<HcclCMDType>(ccTiling->opType);
    opParam.stream = reinterpret_cast<aclrtStream>(stream);
    opParam.engine = OpExecuteConfigToCommEngine(ccTiling->commEngine);

    if (ccTiling->commEngine == static_cast<uint8_t>(OpExecuteConfig::AICPU_TS)) {
        opParam.commOpExpansionMode = HcclOpExpansionMode::HCCL_OP_EXPANSION_MODE_AI_CPU;
    } else if (ccTiling->commEngine == static_cast<uint8_t>(OpExecuteConfig::CCU_SCHED)) {
        opParam.commOpExpansionMode = HcclOpExpansionMode::HCCL_OP_EXPANSION_CCU_SCHED;
    }

    CHK_RET(HcclGetCommName(comm, opParam.commName));
    CHK_RET(PrepareOpParams(comm, tag, ccTiling, opParam));
    if (opParam.opType == HcclCMDType::HCCL_CMD_ALLTOALL) {
        opParam.all2AllVDataDes.sendType = static_cast<HcclDataType>(ccTiling->srcDataType);
        // sendCounts指向的host侧数组由调用方(GetOpParam)持有并填充，此处保持nullptr，
        // 避免将数值当作指针使用导致后续解引用非法地址。
    }
    return HCCL_SUCCESS;
}

HcclResult PrepareTopoInfoForOp(HcclComm comm, OpParam& opParam, std::unique_ptr<TopoInfoWithNetLayerDetails>& topoInfo)
{
    HCCL_INFO("[SelectAlgAndPrepareEngine]start SelectAlgAndPrepareEngine!");
    opParam.hcclComm = comm;
    CHK_RET(HcclGetOpExpansionMode(comm, opParam));
    HCCL_INFO("[SelectAlgAndPrepareEngine]  successfully!");
    CHK_RET(HcclCalcTopoInfo(comm, opParam, topoInfo));
    HCCL_INFO("[SelectAlgAndPrepareEngine]HcclCalcTopoInfo successfully!");
    return HCCL_SUCCESS;
}

HcclResult PrepareEngineForAlg(OpParam& opParam, const std::string& algName)
{
    CHK_RET(SetCommEngine(opParam));
    if (GetExternalInputHcclAivOnlyMode() && opParam.engine != COMM_ENGINE_AIV) {
        HCCL_ERROR(
            "[HcclExecOp] opType[%d] currently do not select aiv mode, aiv only not support.",
            static_cast<int>(opParam.opType));
        return HCCL_E_NOT_SUPPORT;
    }
    if ((opParam.engine == COMM_ENGINE_AICPU_TS) || (opParam.engine == COMM_ENGINE_CPU)) {
        HCCL_DEBUG("[Selector] is aicpu mode");
        CHK_RET(LoadAICPUKernel());
    }
    CHK_RET(SetOpParamAlgTag(opParam, algName));
    CHK_RET(FillOpParamAlgName(opParam, algName));
    return HCCL_SUCCESS;
}

std::unique_ptr<InsCollAlgBase> GetAlgExecutorForOp(const OpParam& opParam, const std::string& algName)
{
    if (UseCannBridge(opParam)) {
        return GetAlgExecViaCann(opParam.opType, algName);
    }
    return CollAlgExecRegistryV2::Instance().GetAlgExec(opParam.opType, algName);
}

HcclResult CheckForcedAlgResource(
    HcclComm comm, const OpParam& opParam, TopoInfoWithNetLayerDetails* topoInfo, const std::string& algName)
{
    CHK_PTR_NULL(topoInfo);
    std::unique_ptr<InsCollAlgBase> executor = GetAlgExecutorForOp(opParam, algName);
    CHK_PRT_RET(
        executor == nullptr,
        HCCL_WARNING(
            "[MC2_FORCE_ALG] fallback, algName[%s] is not registered for opType[%u].", algName.c_str(),
            static_cast<u32>(opParam.opType)),
        HCCL_E_NOT_SUPPORT);

    AlgHierarchyInfoForAllLevel algHierarchyInfo;
    HcclResult ret;
    if (UseCannBridge(opParam)) {
        // Match HcclGetAlgRes: bridge executors need the selected algorithm's topology metadata.
        // opParam.algName is filled later, so use the explicit forced algorithm name here.
        AlgAttrs algAttrs = executor->GetAlgoMeta(algName);
        ret = executor->CalcAlgHierarchyInfoV2(topoInfo, algHierarchyInfo, algAttrs);
    } else {
        ret = executor->CalcAlgHierarchyInfo(comm, topoInfo, algHierarchyInfo);
    }
    CHK_PRT_RET(
        ret != HCCL_SUCCESS,
        HCCL_WARNING(
            "[MC2_FORCE_ALG] fallback, CalcAlgHierarchyInfo failed, algName[%s], ret[%d].", algName.c_str(),
            static_cast<int>(ret)),
        HCCL_E_NOT_SUPPORT);

    AlgResourceRequest resRequest;
    ret = executor->CalcRes(comm, opParam, topoInfo, algHierarchyInfo, resRequest);
    CHK_PRT_RET(
        ret != HCCL_SUCCESS,
        HCCL_WARNING(
            "[MC2_FORCE_ALG] fallback, CalcRes failed, algName[%s], ret[%d].", algName.c_str(), static_cast<int>(ret)),
        HCCL_E_NOT_SUPPORT);
    return HCCL_SUCCESS;
}

static bool GetForcedAlgName(const char* config, std::string& algName)
{
    if (config == nullptr || config[0] == '\0') {
        return false;
    }

    std::string algConfig(config);
    if (algConfig.find('=') != std::string::npos) {
        HCCL_INFO("[MC2_FORCE_ALG] legacy algConfig[%s], use default selector.", algConfig.c_str());
        return false;
    }

    algName = algConfig;
    return true;
}

// 外部名定名（解析 -> topo 准备 -> 消歧；规则谓词消费 topo 故不在纯定名函数内）。
// true = algName 有效且 topoInfo 已就绪; false = WARNING 已打且 opParam/topoInfo 复位到入口现场，
// 调用方回退默认 selector。
static bool ResolveExternalAlgName(
    HcclComm comm, const char* algConfig, OpParam& opParam, std::unique_ptr<TopoInfoWithNetLayerDetails>& topoInfo,
    std::string& algName)
{
    algName.clear();
    OpParam opParamBackup = opParam;
    ExternalAlgSpec extSpec;
    std::string extParseErrMsg;
    if (!ParseExternalAlg(algConfig, extSpec, extParseErrMsg)) {
        HCCL_WARNING(
            "[MC2_EXT_ALG] fallback to default selector, algConfig[%s], opType[%u], reason[%s].", algConfig,
            static_cast<u32>(opParam.opType), extParseErrMsg.c_str());
        return false;
    }
    if (PrepareTopoInfoForOp(comm, opParam, topoInfo) != HCCL_SUCCESS) {
        opParam = opParamBackup;
        return false;
    }
    std::string resolvedName;
    std::string resolveErrMsg;
    if (ResolveExternalAlg(opParam, topoInfo.get(), extSpec, resolvedName, resolveErrMsg) != ResolveStatus::RESOLVED) {
        opParam = opParamBackup;
        topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
        HCCL_WARNING(
            "[MC2_EXT_ALG] fallback to default selector, algConfig[%s], opType[%u], reason[%s].", algConfig,
            static_cast<u32>(opParam.opType), resolveErrMsg.c_str());
        return false;
    }
    algName = resolvedName;
    HCCL_INFO(
        "[MC2_EXT_ALG] accepted, opType[%u], algConfig[%s] -> algName[%s].", static_cast<u32>(opParam.opType),
        algConfig, algName.c_str());
    return true;
}

static HcclResult TryForcedAlgAndPrepareEngine(
    HcclComm comm, const char* algConfig, OpParam& opParam, std::string& algName,
    std::unique_ptr<TopoInfoWithNetLayerDetails>& topoInfo, bool& forcedAlgAccepted)
{
    forcedAlgAccepted = false;
    std::string forcedAlgName;
    std::string bareName;
    ExternalAlgSpec extSpec;
    std::string classifyErrMsg;
    const ForcedAlgKind kind = ClassifyForcedAlgConfig(
        algConfig == nullptr ? std::string() : std::string(algConfig), bareName, extSpec, classifyErrMsg);

    OpParam opParamBackup = opParam; // topo 准备会改写 opParam，失败恢复以入口现场为基线
    if (kind == ForcedAlgKind::EXTERNAL) {
        // 外部名需 topo 参与消歧；任一失败 WARNING + 回退默认 selector
        if (!ResolveExternalAlgName(comm, algConfig, opParam, topoInfo, forcedAlgName)) {
            return HCCL_SUCCESS;
        }
    } else if (kind == ForcedAlgKind::BARE_NAME) {
        forcedAlgName = bareName;
        CHK_RET(PrepareTopoInfoForOp(comm, opParam, topoInfo));
    } else {
        if (kind == ForcedAlgKind::LEGACY) {
            HCCL_INFO("[MC2_FORCE_ALG] legacy algConfig[%s], use default selector.", algConfig);
        }
        return HCCL_SUCCESS;
    }

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

bool GetForcedAlgName(const Mc2CcTilingInner* ccTiling, std::string& algName)
{
    return GetForcedAlgName(ccTiling == nullptr ? nullptr : ccTiling->algConfig, algName);
}

HcclResult TryForcedAlgAndPrepareEngine(
    HcclComm comm, const Mc2CcTilingInner* ccTiling, OpParam& opParam, std::string& algName,
    std::unique_ptr<TopoInfoWithNetLayerDetails>& topoInfo, bool& forcedAlgAccepted)
{
    return TryForcedAlgAndPrepareEngine(
        comm, ccTiling == nullptr ? nullptr : ccTiling->algConfig, opParam, algName, topoInfo, forcedAlgAccepted);
}

HcclResult SelectAlgAndPrepareEngine(
    HcclComm comm, OpParam& opParam, std::string& algName, std::unique_ptr<TopoInfoWithNetLayerDetails>& topoInfo)
{
    CHK_RET(PrepareTopoInfoForOp(comm, opParam, topoInfo));

    std::shared_ptr<ExecuteSelector> collAlgSelector = std::make_shared<ExecuteSelector>(ExecuteSelector());
    CHK_RET(collAlgSelector->Run(opParam, topoInfo.get(), algName));
    if (algName.empty()) {
        HCCL_ERROR("[Selector] select algname fail!");
        return HCCL_E_PTR;
    }

    CHK_RET(PrepareEngineForAlg(opParam, algName));
    return HCCL_SUCCESS;
}

HcclResult FillOpParamAlgName(OpParam& opParam, const std::string& algName)
{
    int result = sprintf_s(opParam.algName, sizeof(opParam.algName), "%s", algName.c_str());
    CHK_PRT_RET(result <= 0, HCCL_ERROR("failed to fill opParam.algName"), HCCL_E_INTERNAL);
    HCCL_INFO(
        "[GetOpParam] prepared opParam, opType[%u], algName[%s], algTag[%s].", static_cast<u32>(opParam.opType),
        opParam.algName, opParam.algTag);
    return HCCL_SUCCESS;
}

HcclResult HandleSingleRankAndCommMode(HcclComm comm, OpParam& opParam, bool& skipGetRes)
{
    uint32_t userRankSize = 0;
    CHK_RET(HcclGetRankSize(comm, &userRankSize));
    if (userRankSize == 1) {
        HCCL_WARNING("[%s] ranksize == 1, enter SingleRankProc", __func__);
        CHK_RET(SingleRankProc(opParam));
        skipGetRes = true;
        return HCCL_SUCCESS;
    }

    bool isOpBase = true;
    const char* opModeStr = isOpBase ? "_opbase" : "_offload";
    auto ret = sprintf_s(opParam.commModeTag, sizeof(opParam.commModeTag), "%s_%s", opParam.commName, opModeStr);
    if (ret <= 0) {
        HCCL_ERROR("[%s] failed to fill opParam.commModeTag", __func__);
        return HCCL_E_INTERNAL;
    }
    skipGetRes = false;
    return HCCL_SUCCESS;
}

static HcclResult AcquireAlgResources(
    HcclComm comm, OpParam& opParam, std::unique_ptr<InsCollAlgBase>& executor, TopoInfoWithNetLayerDetails* topoInfo,
    std::unique_ptr<AlgResourceCtxSerializable>& resCtxHost, OpResCtx& opResCtx, void** resCtxOut)
{
    HCCL_INFO("[AcquireAlgResources]start AcquireAlgResources!");
    bool isResourceReused = false;
    ThreadHandle cpuTsThread{0};
    ThreadHandle exportedAicpuTsThread{0};
    if ((opParam.engine == COMM_ENGINE_AICPU_TS) || (opParam.engine == COMM_ENGINE_CPU)) {
        CHK_RET(HcclThreadAcquireWithStream(comm, COMM_ENGINE_CPU_TS, opParam.stream, 1, &cpuTsThread));
        CHK_RET(HcclThreadExportToCommEngine(comm, 1, &cpuTsThread, COMM_ENGINE_AICPU_TS, &exportedAicpuTsThread));
    }
    if (opParam.engine == COMM_ENGINE_CCU) {
        const uint32_t missionNum = GetKfcServerMissionNum(opParam.algName);
        resCtxHost->kfcServerArgs.clear();
        resCtxHost->kfcServerArgs.reserve(missionNum * KFC_SERVER_ARG_NUM);
        for (uint32_t missionIndex = 0; missionIndex < missionNum; ++missionIndex) {
            resCtxHost->kfcServerArgs.insert(
                resCtxHost->kfcServerArgs.end(), {opResCtx.xnAddr, opResCtx.ckeAddr, 1U, missionNum, missionIndex, 0U});
        }
        resCtxHost->kfcServerArgSize = KFC_SERVER_ARG_NUM;
        HCCL_INFO(
            "[AcquireAlgResources] generated [%u] KFC mission args, argSize[%u], xnAddr[0x%llx], "
            "ckeAddr[0x%llx]",
            missionNum, resCtxHost->kfcServerArgSize, static_cast<unsigned long long>(opResCtx.xnAddr),
            static_cast<unsigned long long>(opResCtx.ckeAddr));
        auto resRet = HcclGetAlgRes(comm, opParam, executor, topoInfo, resCtxHost, resCtxOut, isResourceReused);
        if (opParam.checkRes && resRet == HCCL_E_UNAVAIL) {
            HCCL_WARNING("[%s] HCCL_E_UNAVAIL, resource unavailable.", __func__);
            return HCCL_E_RES_NOT_SUFFICIENT;
        }
        CHK_RET(resRet);
        opParam.resCtx = *resCtxOut;
    } else {
        CHK_RET(HcclGetAlgRes(comm, opParam, executor, topoInfo, resCtxHost, resCtxOut, isResourceReused));
        opParam.resCtx = *resCtxOut;
    }
    HCCL_INFO("[AcquireAlgResources]end AcquireAlgResources!");
    return HCCL_SUCCESS;
}

// ---------------------------------------------------------------------------
// unified 分叉实现（§10.2 A5）：Compile → Materialize → Plan 序列化进 ctx 旁路。
// ABI 方案 B（§12-10）：OpParam 一个字节不改——Plan 序列化产物走独立 ctx
//（HcclEngineCtxCreate，键 = UnifiedAlgTag；A2AV 变长表的 varMemSize 模式同思路，
// 但本路径完全旁路 OpParam，连 varMemSize 都不动）。
// ---------------------------------------------------------------------------
HcclResult UnifiedResourceBroker::AcquireThreads(u32 threadNum,
    const std::vector<u32>& notifyNumPerThread, u32 notifyNumOnMainThread, std::string& err)
{
    // MAJOR-1（三轮审查）：notify 数按现状公式消费——
    // maxNotifyNum = max(notifyNumOnMainThread, max(perThread))，申请时 +1（host-device 同步
    // 用一个，对照 HcclGetThread :1073-1090 的 AICPU_TS 分支）
    u32 maxNotifyNum = notifyNumOnMainThread;
    for (u32 n : notifyNumPerThread) {
        if (n > maxNotifyNum) maxNotifyNum = n;
    }
    // R11 · WP-1：句柄回收出参传入（op_common.cc 重载）——失败路径 threads_ 不触
    //（重载内 HcclThreadAcquire 失败先 return，收集值维持空）。
    return mc2_ops_hccl::HcclGetAlgResUnified(
        comm_, param_, threadNum, maxNotifyNum + 1U, {}, err, &threads_);
}

HcclResult UnifiedResourceBroker::AcquireChannels(const std::vector<refactor::ChannelReq>& channels,
    std::string& err)
{
    // R11 · WP-1：句柄回收出参传入（op_common.cc 带出参重载——与 AcquireThreads 同批）。
    return mc2_ops_hccl::HcclChannelAcquireUnified(comm_, param_, channels, err, &channelHandles_);
}

HcclResult UnifiedResourceBroker::AcquireCcuKernel(const std::vector<refactor::CcuKernelPlan>& kernels,
    refactor::CcuKernelHandle& kernelHandle, std::string& err)
{
    return mc2_ops_hccl::HcclCcuKernelRegisterUnified(comm_, param_, kernels, kernelHandle, err);
}

// algConfig 来源：ccTiling->algConfig（GetForcedAlgName 同源数据，但走结构化解析而非整串比对）
HcclResult GetUnifiedOpParamResCtxImpl(
    HcclComm comm, OpParam& opParam, TopoInfoWithNetLayerDetails* topoInfo,
    const std::string& algConfig, UnifiedResOut& resOut)
{
    using namespace refactor;
    // ① 域成员推导（单域 PoC 口径；多域 DomainPlan 随 §6.11 落地）。
    // MINOR-4（三轮审查）：physicalLevels 非空但提取不出域成员时回退 {userRank}，
    // 不做 [0] 越界访问（下游 Compile 对单元素域自然退化，与现状单 rank 判定同向）。
    std::vector<std::vector<u32>> hierarchyRanks;
    std::vector<u32> domainRanks;
    if (!topoInfo->physicalLevels.empty()) {
        // 提取 level0 域成员（localRanks：本 rank 在该层可见的全部 rank，alg_param.h:178+；
        // 完整 DomainPlan 推导随 §6.11）
        domainRanks = topoInfo->physicalLevels[0].localRanks;
    }
    if (domainRanks.empty()) {
        domainRanks = std::vector<u32>{opParam.userRank};     // 提取不出 → 单 rank 回退
    }
    hierarchyRanks.push_back(domainRanks);
    // 单 rank 退化：与现状 HandleSingleRankAndCommMode skipGetRes 同判定（rankSize==1）
    if (topoInfo->userRankSize <= 1U) {
        HCCL_INFO("[UnifiedRes] single rank, skip unified resource path.");
        return HCCL_SUCCESS;                      // 调用方据此直接成功返回（无资源可申请）
    }

    // ------------------------------------------------------------------
    // WP-B0（R6，2026-09-21；终审 C-B0-1 收口补丁扩为双引擎拒收；
    // AICPU 半边 R6 解锁；**CCU 半边 R6.5 解锁**（B-CCU② 落地+过审，2026-09-22）。
    // ——B0 双引擎拒收至此**全部移除**，本块为历史记录——
    //
    // 实证链（count=0 生产链路，两引擎同源问题——历史记录，机制已解）：
    //   ① Mc2CcTilingInner 无 count 字段（hccl_tiling_msg.h:37-51）；
    //   ② 资源期 PrepareParamForAllGather 等显式 count=0（:511/:539/:568）；
    //   ③ 真值 count 由 exec 期设备侧 KFC 适配器注入（adapter.cc:142），资源期不可得；
    //   ④ 后果（B0 时期）：Compile 的 sliceSize = count*dts = 0 → Plan 全零静默下发。
    //
    // 两半边的解（方向 B：blob=结构底版 + exec 期实例化，count 占位槽不消费）：
    //   · AICPU（R6 解锁）：unified_aicpu_exec.cc ④' 段 msg.dataCnt 实例化，
    //     消费链 unified_exec/aicpu_consumer.h——gate ⑤ 翻转断言守门；
    //   · CCU（R6.5 解锁）：写入方 CcuPrepareForUnifiedPlan（hccl_ccu_v0_prepare.h）
    //     从纯拷贝升级为"拷贝+数据槽覆写"（11 现值覆写 + 5 常量透传，v2
    //     CcuPrepareForAllGatherM2M :273-296 逐式同源；含 goSize 2GiB 上界拒收
    //     （比 v2 严的新防线）与 RS 帧级拒收（R7 前无 RS 流量））——golden 守门
    //     poc_unified_ccu_overwrite_test.cc（36 断言，R5 ST A 案交叉锚）。
    //
    // 拆闸收尾（WP-C3）：本函数两引擎拒收分支全删——unified 生产入口对
    // AG×AICPU/AG×CCU 均放行（三开关照旧：编译期宏 + 环境变量 + 路由表 AG 行）。
    // CCU 全链复活项（R5 遗留半状态）：M1 注册链/M2 写入方（已升级覆写）/M3 组装段/
    // marker 幂等守卫——marker 残留复验与全链冒烟判据见 gate/overwrite 测试。
    // 灰度回退语义不变：分叉任一步失败 → 调用方落回老路径完整重走。
    // ------------------------------------------------------------------

    // ② Compile（含 Validate——用户配置错在此快速失败，§12-7 C2）
    Plan plan;
    std::string err;
    const HcclResult ret = L1Adapter::Compile(opParam, *topoInfo, hierarchyRanks, algConfig, plan, err);
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR("[UnifiedRes] Compile failed, ret[%d], err[%s]. retreating to legacy path.",
            static_cast<int>(ret), err.c_str());
        return ret;                               // 灰度回退：调用方走老路径
    }

    // ③ Materialize：ResourceClaim → 真实申请（真实现壳）。
    // M3-缺口1（裁决接通）：kernelHandle 经 UnifiedResOut 出参带回（CCU 注册时刻
    // 唯一回传；AICPU 引擎恒 0，与 Materialize 出参语义一致）——组装段
    //（ProcessCcuTiling 分叉处）消费它落 resCtxHost.ccuKernels[0]。
    // C1 幂等守卫（终审改形态，正向判定）：unified 无 algTag 缓存，同 tag 二次调用
    // 重跑 Materialize 会重复注册——不依赖"HcommCcuKernelRegister 同参数幂等"假设
    //（M1 MAJOR-2 挂账的未验证项）。判据 = 注册标记 ctx（tag = UnifiedAlgTag+"_resctx"，
    // M3 组装段最后一步落盘）Get 命中 → 上次 unified 调用已完成组装（含注册），
    // 从 device 读回 resCtx 反序列化取 ccuKernels[0] 作为 handle，跳过 Materialize。
    const Engine engine = L1Adapter::EngineOf(opParam);
    if (engine == Engine::CCU) {
        const std::string markerTag = UnifiedAlgTag(algConfig, engine) + "_resctx";
        void* markerCtx = nullptr;
        uint64_t markerSize = 0;
        if (HcclEngineCtxGet(comm, markerTag.c_str(), COMM_ENGINE_AIV, &markerCtx, &markerSize) == HCCL_SUCCESS &&
            markerCtx != nullptr && markerSize > 0) {
            // 读回反序列化取 handle（AlgResourceCtxSerializable 的 ccuKernels[0]）
            std::vector<char> seq(static_cast<size_t>(markerSize));
            aclError aclRet = aclrtMemcpy(seq.data(), markerSize, markerCtx, markerSize, ACL_MEMCPY_DEVICE_TO_HOST);
            if (aclRet == ACL_SUCCESS) {
                AlgResourceCtxSerializable priorCtx;
                priorCtx.DeSerialize(seq);
                if (!priorCtx.ccuKernels.empty() && priorCtx.ccuKernels[0] != 0) {
                    resOut.kernelHandle = priorCtx.ccuKernels[0];
                    HCCL_INFO("[UnifiedRes] registration idempotent-skip: marker[%s] hit, "
                              "reuse kernelHandle[0x%llx] (no re-register).",
                        markerTag.c_str(), static_cast<unsigned long long>(resOut.kernelHandle));
                    // blob 仍需重建（本轮 Plan 语义：ctx 可能被同 tag 覆盖——HcclSerializeUnifiedPlan
                    // 的 ④ 段继续执行；跳过的只是 Materialize 的注册半边）
                } else {
                    HCCL_WARNING("[UnifiedRes] marker hit but handle is 0/empty — fall through to register.");
                }
            } else {
                HCCL_WARNING("[UnifiedRes] marker read-back failed, ret[%d] — fall through to register.", aclRet);
            }
        }
    }
    UnifiedResourceBroker broker(comm, opParam);
    HcclResult mret = HCCL_SUCCESS;
    if (resOut.kernelHandle == 0) {
        mret = L1Adapter::Materialize(plan, engine, broker, err, resOut.kernelHandle);
    } else {
        // 幂等跳过：注册半边已在上次调用完成；线程半边（AcquireThreads）CCU 不占
        //（claim.slaveThreadNum=0 → threadNum=1 由组装段自理），通道半边同参数幂等
        //（AcquireChannels 幂等返回既有句柄，v2 同式）——两者跳过无害。
        HCCL_INFO("[UnifiedRes] Materialize registration-side skipped (idempotent hit).");
    }
    if (mret != HCCL_SUCCESS) {
        // C1：探测路径资源不足 → 现状同映射（checkRes 场景由上层 CheckOpResSufficient 消费）
        HCCL_ERROR("[UnifiedRes] Materialize failed, ret[%d], err[%s]. retreating to legacy path.",
            static_cast<int>(mret), err.c_str());
        return mret;
    }

    // ④ Plan 序列化进 ctx 旁路（OpParam 不动，§12-10 方案 B）。
    // M2/M3（尾款轮，2026-09-21，历史形态）：CCU 引擎切 v3 真序列化（SerializePlanV3
    // 定长槽位格式——device 写入方 CcuPrepareForUnifiedPlan 消费的 UPL3 blob）；AICPU
    // 引擎维持 v1 文本摘要（当时的认知：其消费方只有日志/维测，无 device 写入方）。
    // blob 以 u64 槽序列按字节视图写 ctx（vector<u64> → string 视图搬运）。
    // R10 · F1 断点①（序列化错配修复，2026-10-08）：AICPU 引擎 v1→v4 切换——exec 期
    // 消费方 RunUnifiedAicpuOp（unified/exec/unified_aicpu_exec.cc:87）以 ParseUnifiedBlobV4
    // 解析 param.resCtx（校验 UPL3 魔数/版本 4），v1 文本摘要（SerializePlanForCtx 纯
    // 日志字符串）必被拒收——两端各自正确、中段格式断裂（R9 审查考古立案的第四轮
    // BLOCKER-1 同型）。本处即第四轮注释预留的"真注册链接线时切换"点：两引擎统一走
    // SerializePlanV3 字节视图（v4 blob）。SerializePlanForCtx 函数保留（日志/维测
    // 口径，action_spec.h:314 退役预留说明）——仅停用本调用点。
    const std::string unifiedAlgTag = UnifiedAlgTag(algConfig, engine);
    std::vector<u64> blobSlots;
    if (!SerializePlanV3(plan, blobSlots, err)) {
        HCCL_ERROR("[UnifiedRes] SerializePlanV3 failed, err[%s]. retreating to legacy path.", err.c_str());
        return HCCL_E_PARA;
    }
    const char* blobBytes = reinterpret_cast<const char*>(blobSlots.data());
    const std::string planBlob(blobBytes, blobSlots.size() * sizeof(u64));
    // R10 · F1 断点③：blob 尺寸经 UnifiedResOut 出参带回（AICPU 半边成功路径回填
    // opParam.ctxSize 的唯一来源；CCU 半边组装段另有 HcclEngineCtxGet 反查校验，
    // 不消费本字段）。
    resOut.blobSize = planBlob.size();
    // R11 · WP-1（句柄落 UnifiedResOut）：broker 回收的 Materialize 申请句柄带出——
    // AICPU 组装段（AssembleUnifiedAicpuResCtx）落资源束 threads/channels 的唯一
    // 来源（对照 CCU 半边：组装段 :1546-1557 自行 HcclThreadAcquire——CCU claim
    // slaveThreadNum=0 只需 1 条，AICPU 需 slave+1 条与通道全量，重申请无缓存
    // 语义依据，回收是正解）。
    resOut.acquiredThreads = broker.AcquiredThreads();
    resOut.acquiredChannels = broker.AcquiredChannels();
    // R11 · WP-2：claim 值 + 通道申请输入透传（组装段资源束字段同源填充）
    resOut.claimSlaveThreadNum = plan.claim.slaveThreadNum;
    resOut.claimNotifyNumOnMainThread = plan.claim.notifyNumOnMainThread;
    resOut.claimNotifyNumPerThread = plan.claim.notifyNumPerThread;
    resOut.acquiredChannelReqs = plan.claim.channels;
    return mc2_ops_hccl::HcclSerializeUnifiedPlan(comm, opParam, unifiedAlgTag, planBlob, &resOut.ctx);
}

// ---------------------------------------------------------------------------
// R11 · WP-2 AICPU 引擎特定组装段（2026-10-08）——AssembleUnifiedCcuDeliverables
//（:1541+，CCU 半边）的 AICPU 同构物。位置考据（M0-③）：CCU 组装在
// ProcessCcuTiling 分叉处（"引擎特定交付物不下沉 AICPU/CCU 共用骨架"的 R5
// 主会话裁决），AICPU 同理在 PrepareCcAlgResources 分叉处——本函数即其组装体。
//
// 职责（对照老路径交付物 GetAlgResAICPU :985-1007 + HcclAllocAlgResourceAICPU
// :1055-1095 + HcclMemcpyCtxHostToDevice :1059-1077）：
//   ① AlgResourceCtxSerializable 资源束组装：commInfoPtr=comm（BuildHolder :126
//      commInfoPtr == param.hcclComm 校验源）、topoInfo、threads=broker 回收
//     （WP-1）、channels=回收句柄、cclMem=HcclGetHcclBuffer（CCU 组装段 :1622-1626
//      同式）、notifyNumOnMainThread/slaveThreadNum/notifyNumPerThread=claim 值、
//      ccuKernels 空（AICPU 无 CCU 注册）、方案 B blob 字段（unifiedBlobAddr=
//      resOut.ctx、unifiedBlobSize=resOut.blobSize）。
//   ② Serialize → HcclEngineCtxCreate(tag=UnifiedAlgTag+"_resctx") → H2D。
//      引擎参数（M0-② 裁定）：COMM_ENGINE_AICPU——镜像 HcclSerializeUnifiedPlan
//      的 blob ctx 同域（op_common.cc :3194，blob ctx 先例——同一 unified 产物族
//      两个 ctx 同域管理，反查/probe 接口（AssembleUnifiedCcuDeliverables ⑦ 段
//      HcclEngineCtxGet(comm, tag, COMM_ENGINE_AICPU, ...) :1527-1529 同域 Get）
//      才能按域命中）。存量 HcclMemcpyCtxHostToDevice 用 COMM_ENGINE_AICPU_TS
//      + param.algTag（:1065-1066）——那是 legacy 缓存键体系（tag_algName_launchMode
//      拼式），unified 的 algTag 是 UnifiedAlgTag 规范形，语义不同源不沿用。
//   ③ opParam.resCtx/ctxSize 回归资源束（R10 的 blob 回填废除）。
//   ④ algTag 回填（M0-③：SetOpParamAlgTag 在 op_common.cc:2466（tag_algName_
//      launchMode 拼式 + CCU 扩展段），unified 的"语法即身份"键（UnifiedAlgTag，
//      C10 §10.2）与该拼式不兼容——不复用存量函数，仿其 sprintf_s 形态直填
//      组装段实际 ctx tag 串：UnifiedAlgTag+"_resctx"）。
//   ⑤ 资源束读回校验（对照 CCU ⑦ 段 blob 头校验：device ctx H2D 后 D2H 首字段
//      commInfoPtr 比对——资源束语义的往返自证）。
//
// 幂等（对照 CCU marker 守卫 :1032-1058）：AICPU 侧重复调用（同 tag 二次
// PrepareCcAlgResources）现状会重跑 Materialize——系 R6-R10 既有行为非本轮回归；
// HcclThreadAcquire/HcclChannelAcquire 幂等未证（v2 有 g_hostCtx 缓存摊销，unified
// 无）——登记 P2 遗留（M0-② 结论），本段不引入 marker 守卫（blob ctx 幂等 Create
// 的同 tag 覆盖行为未证，守卫反引入新假设）。
//
// 【错误路径】组装失败直接上抛不落回（CCU 侧 M3-ERR 裁定同型：blob ctx 已建
// （HcclSerializeUnifiedPlan 先行）、线程/通道已申请（Materialize 先行）——落回
// 老路径 = 双份资源状态且老路径无感知）。
// ---------------------------------------------------------------------------
static HcclResult AssembleUnifiedAicpuResCtx(HcclComm comm, OpParam& opParam,
    std::unique_ptr<TopoInfoWithNetLayerDetails>& topoInfoUnified, const std::string& unifiedAlgConfig,
    const UnifiedResOut& unifiedResOut)
{
    uint32_t userRank = 0;
    CHK_RET(HcclGetRankId(comm, &userRank));
    uint32_t userRankSize = 0;
    CHK_RET(HcclGetRankSize(comm, &userRankSize));
    // 单 rank 退化（GetUnifiedOpParamResCtxImpl ① 段 :1020 同判定先行返回 SUCCESS、
    // resOut 全空）：与现状 HandleSingleRankAndCommMode skipGetRes 语义对齐——无资源
    // 可申请，组装段不得把空 resOut 当病灶报错（否则单 rank 场景 unified 分叉由
    // "优雅退化"变"必失败"，破坏灰度契约）。成功直返（调用方随后 FillOpParamAlgName
    // 后 return SUCCESS——老路径单 rank 同归宿）。
    if (userRankSize <= 1U) {
        HCCL_INFO("[UnifiedRes][AICPU] single rank, skip res-ctx assembly (no resource to bundle).");
        return HCCL_SUCCESS;
    }
    const refactor::Engine engine = refactor::L1Adapter::EngineOf(opParam);
    const std::string unifiedAlgTag = refactor::UnifiedAlgTag(unifiedAlgConfig, engine);
    const std::string tagResCtx = unifiedAlgTag + "_resctx";
    HCCL_INFO("[UnifiedRes][AICPU] assemble res-ctx start: rank[%u/%u], algTag[%s], threads[%zu], "
              "channels[%zu], blob[%p, %lluB].",
        userRank, userRankSize, tagResCtx.c_str(), unifiedResOut.acquiredThreads.size(),
        unifiedResOut.acquiredChannels.size(), unifiedResOut.ctx,
        static_cast<unsigned long long>(unifiedResOut.blobSize));

    // ① 资源束组装（GetAlgResAICPU :992-994 的 commInfoPtr/topoInfo 同式）
    auto resCtxHost = std::make_unique<AlgResourceCtxSerializable>();
    resCtxHost->commInfoPtr = static_cast<void*>(comm);   // KFC BuildHolder :126 校验源
    resCtxHost->topoInfo = *topoInfoUnified;
    resCtxHost->threads = unifiedResOut.acquiredThreads;  // WP-1 回收（Materialize 申请原批）
    if (resCtxHost->threads.empty()) {
        // 空线程不可达（AG 多 rank 域 claim.slaveThreadNum=N-2 → threadNum=N-1≥1）——
        // 防御性显式报错：KFC BuildHolder :135 threads.empty 拒 / AddOpContext
        // isDevice950 分支 threads[0]==0 拒，此处提前到资源期暴露。
        HCCL_ERROR("[UnifiedRes][AICPU] no acquired threads in UnifiedResOut "
                   "(Materialize thread side missing?).");
        return HCCL_E_INTERNAL;
    }
    // algHierarchyInfo（消费方 ② 段 domainRanks 来源——unified_aicpu_exec.cc :117
    // resCtx.algHierarchyInfo.infos[0][0]）：单域 PoC 口径 = {domainRanks}
    //（topo_match_1d.cc :53-55 的 infos[0][0]=全量 rankIds 同形态；GetUnified-
    // OpParamResCtxImpl ① 段推导的 hierarchyRanks 与 Compile 的 domain 同源）。
    // 多域 DomainPlan 随 §6.11（届时 Compile 侧 domain 分层数据经 UnifiedResOut 透传）。
    {
        std::vector<u32> level0;
        if (!topoInfoUnified->physicalLevels.empty()) {
            level0 = topoInfoUnified->physicalLevels[0].localRanks;
        }
        if (level0.empty()) {
            level0 = std::vector<u32>{opParam.userRank};
        }
        resCtxHost->algHierarchyInfo.infos.push_back({level0});
    }
    // channels：句柄回填进 ChannelInfo（对照 HcclGetChannelImpl :1287-1296 的
    // isValid/remoteRank/handle 三元组——unified 侧无 HcclRankGraphGetEndpointInfo
    // 端口探测链，portGroupSize/dieId 维持默认，MakeHcommVerbRes 只消费
    // remoteRank/handle（hcomm_verb_sink.h:74-85））
    {
        std::vector<ChannelInfo> level0;
        for (const ChannelHandle handle : unifiedResOut.acquiredChannels) {
            if (handle == 0U) {
                continue;   // 0 句柄不落束（Acquire 输出占位形态防御）
            }
            ChannelInfo ch;
            ch.isValid = true;
            ch.remoteRank = INVALID_VALUE_RANKID;   // remoteRank 由 desc 序对齐填（下方第二遍）
            ch.handle = handle;
            level0.push_back(ch);
        }
        if (level0.empty()) {
            // 空通道不可达（AG 多 rank 域 claim.channels 非空、Materialize 已申请）——
            // 防御性显式报错：消费方 RunUnifiedAicpuOp :132 channels 空同样拒
            //（E_PARA），此处提前到资源期暴露（更早失败点）。
            HCCL_ERROR("[UnifiedRes][AICPU] no acquired channels in UnifiedResOut "
                       "(Materialize channel side missing?).");
            return HCCL_E_INTERNAL;
        }
        resCtxHost->channels.push_back(std::move(level0));
    }
    // remoteRank 对齐：HcclChannelAcquireUnified 的 desc 展开序（remoteRank 升序×
    // count 展开，op_common.cc :2894-2906）与回收句柄序同源——按同式重放展开序，
    // 逐句柄对齐 remoteRank。展开序重放与 Materialize 的 ChannelReq 输入无关
    //（claim.channels 的 remoteRank 集即探测输入），与真实现共用同一数学。
    {
        // 从 plan.claim 重放：desc 序 = CalcChannelRequestMesh1D 探测序（rank graph
        // 升序），count 展开 = ChannelReq.count（0→1）。本段不重跑探测（comm 级
        // 探测非幂等保证）——remoteRank 从句柄序与 claim 的对应关系重放。
        // 简化口径（单域 PoC）：unifiedResOut.acquiredChannels 的展开序 ==
        // HcclChannelAcquireUnified 内 expanded 序（remoteRank 升序、逐 rank count 条）
        // ——重放该数学：claim.channels 按 remoteRank 升序 + count 展开。
        std::vector<u32> rankOrder;
        for (const auto& cr : unifiedResOut.acquiredChannelReqs) {
            rankOrder.push_back(cr.remoteRank);
        }
        std::sort(rankOrder.begin(), rankOrder.end());
        size_t idx = 0;
        for (const u32 remoteRank : rankOrder) {
            u32 count = 1U;
            for (const auto& cr : unifiedResOut.acquiredChannelReqs) {
                if (cr.remoteRank == remoteRank) {
                    count = (cr.count == 0U) ? 1U : cr.count;
                    break;
                }
            }
            for (u32 c = 0U; c < count && idx < resCtxHost->channels[0].size(); ++c, ++idx) {
                resCtxHost->channels[0][idx].remoteRank = remoteRank;
            }
        }
        if (idx != resCtxHost->channels[0].size()) {
            HCCL_ERROR("[UnifiedRes][AICPU] channel handle/rank count mismatch: handles[%zu], "
                       "expanded[%zu] (serializer drift).",
                resCtxHost->channels[0].size(), idx);
            return HCCL_E_INTERNAL;
        }
    }
    // cclMem：HcclGetHcclBuffer（CCU 组装段 :1622-1626 同式——AG VIA_CCUBUF 的
    // scratch 基址，消费方 msg.cclBuff 的资源期来源）
    {
        void* cclBufferAddr = nullptr;
        uint64_t cclBufferSize = 0;
        if (HcclGetHcclBuffer(comm, &cclBufferAddr, &cclBufferSize) == HCCL_SUCCESS &&
            cclBufferAddr != nullptr) {
            resCtxHost->cclMem = HcclMem{HCCL_MEM_TYPE_DEVICE, cclBufferAddr, cclBufferSize};
        } else {
            HCCL_WARNING("[UnifiedRes][AICPU] HcclGetHcclBuffer unavailable, cclMem left default "
                         "(AG VIA_CCUBUF scratch degraded).");
        }
    }
    // claim 值：threads/notify 公式（AicpuCollectAll::ClaimEngineRes——
    // backend_aicpu_collect_all.h:43-49：threadNum=N-1、notifyNumPerThread 各 1、
    // notifyNumOnMainThread=N-1）。Materialize 的 AcquireThreads 入参即该公式
    // 产物（l1_adapter.h:222-223），组装段按同式重放（claim 不出 GetUnified-
    // OpParamResCtxImpl——资源束字段与申请参数的一致性由同源公式保证）。
    resCtxHost->slaveThreadNum = unifiedResOut.claimSlaveThreadNum;
    resCtxHost->notifyNumOnMainThread = unifiedResOut.claimNotifyNumOnMainThread;
    resCtxHost->notifyNumPerThread = unifiedResOut.claimNotifyNumPerThread;
    // ccuKernels 空（AICPU 无 CCU 注册——kernelHandle 恒 0 不落束）
    resCtxHost->ccuKernels.clear();
    // batch-transfer 能力位（消费方 InitHcommBatchTransferOnThreadSupported 的输入——
    // kernel_launch.cc :324 / comm_kfc_open_kernel_executor.cc :241 两链同判；老路径
    // GetOpParamResCtx :1417 同式填充）
    resCtxHost->isHcommBatchTransferOnThreadSupported = HcommIsSupportHcommBatchTransferOnThread();
    // 方案 B blob 第二通道：地址/尺寸随资源束序列化（消费方 DeSerialize 自然取得）
    resCtxHost->unifiedBlobAddr = reinterpret_cast<u64>(unifiedResOut.ctx);
    resCtxHost->unifiedBlobSize = unifiedResOut.blobSize;

    // ② 序列化 → HcclEngineCtxCreate（COMM_ENGINE_AICPU 域——见函数头 M0-② 裁定）
    //    → H2D（HcclMemcpyCtxHostToDevice :1065-1071 的 Create+Copy 序同式，域/键不同）
    std::vector<char> seq = resCtxHost->Serialize();
    void* resCtxSequence = nullptr;
    CHK_RET(HcclEngineCtxCreate(comm, tagResCtx.c_str(), COMM_ENGINE_AICPU, seq.size(), &resCtxSequence));
    {
        const errno_t cpRet = memcpy_s(resCtxSequence, seq.size(), seq.data(), seq.size());
        CHK_PRT_RET(cpRet != EOK, HCCL_ERROR("[UnifiedRes][AICPU] res-ctx H2D memcpy failed."), HCCL_E_MEMORY);
    }

    // ③ opParam.resCtx/ctxSize 回归资源束（resCtx 双语义修复核心——R10 的 blob
    //    回填废除；消费方 KFC BuildHolder / kernel_launch DeSerialize 按资源束
    //    反序列化从此得到真实资源束）
    opParam.resCtx = resCtxSequence;
    opParam.ctxSize = seq.size();

    // ④ algTag 回填（M0-③：仿 SetOpParamAlgTag 的 sprintf_s 形态、规范形直填）
    {
        const int tagRet = sprintf_s(
            opParam.algTag, sizeof(opParam.algTag), "%s", tagResCtx.c_str());
        CHK_PRT_RET(tagRet <= 0, HCCL_ERROR("[UnifiedRes][AICPU] failed to fill opParam.algTag."),
            HCCL_E_INTERNAL);
    }

    // ⑤ 读回校验（对照 CCU ⑦ 段：device 首字段 D2H 比对——commInfoPtr 往返自证）。
    //    commInfoPtr 的序列化偏移在 algType(12B)+infos.size(8B)+physIdx.size(8B) 之后
    //    ——精确偏移不硬编码，反序列化全束比对（D2H 全量 + DeSerialize +
    //    commInfoPtr/threads 计数/blob 字段）。
    {
        std::vector<char> back(seq.size());
        aclError aclRet = aclrtMemcpy(back.data(), back.size(), resCtxSequence, back.size(),
            ACL_MEMCPY_DEVICE_TO_HOST);
        CHK_PRT_RET(aclRet != ACL_SUCCESS,
            HCCL_ERROR("[UnifiedRes][AICPU] res-ctx D2H read-back failed, ret[%d].", aclRet), HCCL_E_RUNTIME);
        AlgResourceCtxSerializable backCtx;
        backCtx.DeSerialize(back);
        CHK_PRT_RET(backCtx.commInfoPtr != static_cast<void*>(comm) ||
                        backCtx.threads.size() != resCtxHost->threads.size() ||
                        backCtx.unifiedBlobAddr != resCtxHost->unifiedBlobAddr ||
                        backCtx.unifiedBlobSize != resCtxHost->unifiedBlobSize,
            HCCL_ERROR("[UnifiedRes][AICPU] res-ctx round-trip mismatch: comm[%p] threads[%zu] "
                       "blob[0x%llx, %lluB].",
                backCtx.commInfoPtr, backCtx.threads.size(),
                static_cast<unsigned long long>(backCtx.unifiedBlobAddr),
                static_cast<unsigned long long>(backCtx.unifiedBlobSize)),
            HCCL_E_INTERNAL);
    }

    HCCL_INFO("[UnifiedRes][AICPU] assemble res-ctx done: algTag[%s], ctx[%p, %zuB], threads[%zu], "
              "channels[%zu], blob[0x%llx, %lluB].",
        tagResCtx.c_str(), resCtxSequence, seq.size(), resCtxHost->threads.size(),
        resCtxHost->channels.empty() ? 0U : resCtxHost->channels[0].size(),
        static_cast<unsigned long long>(resCtxHost->unifiedBlobAddr),
        static_cast<unsigned long long>(resCtxHost->unifiedBlobSize));
    return HCCL_SUCCESS;
}

HcclResult GetOpParamResCtx(
    HcclComm comm, const std::string& algName, OpParam& opParam, TopoInfoWithNetLayerDetails* topoInfo,
    void** resCtxOut)
{
    CHK_PTR_NULL(topoInfo);
    CHK_PTR_NULL(resCtxOut);
    *resCtxOut = nullptr;
    // unified 灰度分叉不在此处（本函数签名拿不到 algConfig）——真正的分叉在
    // 调用方 GetOpParam / CheckForcedAlgResource（持有 ccTiling 上下文，能取
    // algConfig 并传给 GetUnifiedOpParamResCtxImpl）。本函数体零修改（§10.2 A5
    // 存量改动纪律：只加分叉不改函数体）。
    bool useCannResCtx = UseCannBridge(opParam);
    std::unique_ptr<InsCollAlgBase> executor = nullptr;
    if (useCannResCtx) {
        executor = GetAlgExecViaCann(opParam.opType, algName);
    } else {
        executor = CollAlgExecRegistryV2::Instance().GetAlgExec(opParam.opType, algName);
    }
    CHK_PRT_RET(
        executor.get() == nullptr, HCCL_ERROR("Fail to find executor for algName[%s]", algName.c_str()), HCCL_E_PARA);
    std::unique_ptr<AlgResourceCtxSerializable> resCtxHost = std::make_unique<AlgResourceCtxSerializable>();
    resCtxHost->isHcommBatchTransferOnThreadSupported = HcommIsSupportHcommBatchTransferOnThread();
    HCCL_DEBUG(
        "[MC2_BATCH_TRANSFER][HostSupport] opType[%u], algName[%s], supported[%d].",
        static_cast<uint32_t>(opParam.opType), algName.c_str(),
        static_cast<int>(resCtxHost->isHcommBatchTransferOnThreadSupported));
    bool isResourceReused = false;

    ThreadHandle cpuTsThread{0};
    ThreadHandle exportedAicpuTsThread{0};
    if (opParam.stream != nullptr &&
        ((opParam.engine == COMM_ENGINE_AICPU_TS) || (opParam.engine == COMM_ENGINE_CPU))) {
        CHK_RET(HcclThreadAcquireWithStream(comm, COMM_ENGINE_CPU_TS, opParam.stream, 1, &cpuTsThread));
        CHK_RET(HcclThreadExportToCommEngine(comm, 1, &cpuTsThread, COMM_ENGINE_AICPU_TS, &exportedAicpuTsThread));
    }
    CHK_RET(HcclGetAlgRes(comm, opParam, executor, topoInfo, resCtxHost, resCtxOut, isResourceReused));
    CHK_PTR_NULL(*resCtxOut);
    if (opParam.engine == COMM_ENGINE_AICPU_TS && !isResourceReused) {
        SaveMc2CommInfoForProfiling(comm, opParam, *resCtxHost);
    }
    opParam.cacheValid = isResourceReused;
    opParam.resCtx = *resCtxOut;
    return HCCL_SUCCESS;
}

HcclResult GetCcuOpParamResCtx(
    HcclComm comm, const std::string& algName, OpParam& opParam, TopoInfoWithNetLayerDetails* topoInfo,
    OpResCtx& opResCtx, void** resCtxOut)
{
    CHK_PTR_NULL(topoInfo);
    CHK_PTR_NULL(resCtxOut);
    *resCtxOut = nullptr;
    HCCL_INFO("[GetCcuOpParamResCtx]start GetCcuOpParamResCtx!");
    HCCL_INFO(
        "[GetCcuOpParamResCtx]received: workspace[%p], size[%llu]", reinterpret_cast<void*>(opResCtx.workSpace),
        opResCtx.workSpaceSize);
    std::unique_ptr<InsCollAlgBase> executor = CollAlgExecRegistryV2::Instance().GetAlgExec(opParam.opType, algName);
    HCCL_INFO("[GetCcuOpParamResCtx]Generate executor successfully!");
    CHK_PRT_RET(
        executor.get() == nullptr, HCCL_ERROR("Fail to find executor for algName[%s]", algName.c_str()), HCCL_E_PARA);
    std::unique_ptr<AlgResourceCtxSerializable> resCtxHost = std::make_unique<AlgResourceCtxSerializable>();
    CHK_RET(AcquireAlgResources(comm, opParam, executor, topoInfo, resCtxHost, opResCtx, resCtxOut));
    // die 切分比例走 res[1] 预留槽（不动 OpResCtx 布局：新旧 op 包/host lib 混搭时旧侧读 0=不切分，安全降级）
    opResCtx.res[1] = DieSplitRatioToPermille(resCtxHost->dieSplitRatio);
    // KFC mission 数（2die 为动态 dieNum）经 res[2] 下发，AIV 逐 mission commit/wait 消费；非 KFC 为 0（AIV 回退 1）
    opResCtx.res[2] = resCtxHost->kfcServerArgs.size() / KFC_SERVER_ARG_NUM;
    // 校验路径不申请device ctx，resCtxOut保持nullptr
    if (!opParam.checkRes) {
        CHK_PTR_NULL(*resCtxOut);
    }
    return HCCL_SUCCESS;
}

static HcclResult PrepareAlltoAllSendCounts(HcclComm comm, OpParam& opParam, std::vector<uint64_t>& sendCounts)
{
    if (opParam.opType != HcclCMDType::HCCL_CMD_ALLTOALL) {
        return HCCL_SUCCESS;
    }
    constexpr uint64_t ALLTOALL_DEFAULT_SEND_COUNTS = 200ULL * 1024 * 1024;
    uint32_t userRankSize = 0;
    CHK_RET(HcclGetRankSize(comm, &userRankSize));
    sendCounts.assign(userRankSize, ALLTOALL_DEFAULT_SEND_COUNTS);
    opParam.all2AllVDataDes.sendCounts = reinterpret_cast<void*>(sendCounts.data());
    return HCCL_SUCCESS;
}

HcclResult PrepareCcAlgResources(HcclComm comm, const char* algConfig, OpParam& opParam, bool skipStreamCheck)
{
    CHK_PTR_NULL(comm);
    CHK_PTR_NULL(algConfig);
    if (!skipStreamCheck) {
        CHK_PTR_NULL(opParam.stream);
    }

    // ALLTOALL场景下sendCounts需指向host侧真实数组，且必须在整个算法选择与资源申请调用链
    // (含SelectAlgAndPrepareEngine、GetOpParamResCtx中的GetAlgExecViaCann)期间保持存活。
    // 该数组持有在本函数栈帧，覆盖opParam的全部使用范围。
    std::vector<uint64_t> sendCounts;
    void* origSendCounts = opParam.all2AllVDataDes.sendCounts;
    CHK_RET(PrepareAlltoAllSendCounts(comm, opParam, sendCounts));

    std::string algName;
    std::unique_ptr<TopoInfoWithNetLayerDetails> topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    bool forcedAlgAccepted = false;
    OpParam opParamBeforeAlg = opParam;
    CHK_RET(TryForcedAlgAndPrepareEngine(comm, algConfig, opParam, algName, topoInfo, forcedAlgAccepted));

    // ------------------------------------------------------------------
    // unified 灰度分叉（§10.2 A5）：此处是唯一拥有 ccTiling（algConfig 来源）
    // 的层。三开关全开且 opType 在路由表内 → Compile→Materialize→Plan 序列化，
    // 成功即 return（不再走 selector/executor 老路径）；
    // 任一步失败（含 algConfig 为空）→ 日志 + 落回老路径完整执行（灰度容错，
    // 区别于 C2 的用户配置错快速失败——那发生在 Compile 内且不回退）。
    //
    // BLOCKER-1 修复（审查三轮）：TryForcedAlgAndPrepareEngine 失败路径会把 topoInfo
    // 重置为空对象（:786 topoInfo = make_unique<...>()）——unified 分叉不得消费该半
    // 初始化对象。仿 CCU 侧形态（ProcessCcuTiling 自备探测）：分叉入口先自备 topoInfo
    //（PrepareTopoInfoForOp，与 SelectAlgAndPrepareEngine 同源探测），失败即落回老路径。
    // ------------------------------------------------------------------
    if (refactor::UseUnifiedPath(static_cast<int>(opParam.opType))) {
        // R6.5 冲突解决注：上游重构 TryForcedAlgAndPrepareEngine 后本函数
        //（PrepareCcAlgResources）签名只有 algConfig（const char*，即原
        // ccTiling->algConfig），不再持有 ccTiling——algConfig 判空改用本参数。
        const std::string unifiedAlgConfig = (algConfig != nullptr && algConfig[0] != '\0')
            ? std::string(algConfig) : std::string();
        HcclResult unifiedRet = HCCL_E_INTERNAL;
        UnifiedResOut unifiedResOut{};   // M3-缺口1：AICPU 路径 kernelHandle 恒 0（无 CCU 注册）
        std::unique_ptr<TopoInfoWithNetLayerDetails> topoInfoUnifiedForAssemble;   // R11：组装段消费（move 语义）
        if (!unifiedAlgConfig.empty()) {
            // 自备 topoInfo（不依赖 TryForced 探测结果——其失败路径重置为空对象）
            std::unique_ptr<TopoInfoWithNetLayerDetails> topoInfoUnified =
                std::make_unique<TopoInfoWithNetLayerDetails>();
            if (PrepareTopoInfoForOp(comm, opParam, topoInfoUnified) == HCCL_SUCCESS) {
                unifiedRet = GetUnifiedOpParamResCtxImpl(comm, opParam, topoInfoUnified.get(),
                    unifiedAlgConfig, unifiedResOut);
                if (unifiedRet == HCCL_SUCCESS) {
                    // R11：组装段接管 topoInfo 所有权（资源束 topoInfo 字段来源——
                    // move 后不再触原指针，失败路径也已在分叉内定局）
                    topoInfoUnifiedForAssemble = std::move(topoInfoUnified);
                }
            } else {
                HCCL_WARNING("[UnifiedRes] self topo probe failed, fall through to legacy path.");
            }
        } else {
            HCCL_WARNING("[UnifiedRes] routed but algConfig empty, fall through to legacy path.");
        }
        if (unifiedRet == HCCL_SUCCESS) {
            // BLOCKER-2 修复（审查三轮）：unified 成功路径回填交付物，对齐老路径出口
            //（AcquireAlgResources :876/:879 的 opParam.resCtx/ctxSize 语义）。
            // cacheValid=false：unified 首次申请（无缓存命中），对应 isResourceReused=false。
            // ---------------------------------------------------------------
            // R11 · resCtx 双语义修复（2026-10-08，AICPU 半边组装段——M0-③ 位置考据：
            // CCU 组装在 ProcessCcuTiling 分叉处（AssembleUnifiedCcuDeliverables，
            // "引擎特定交付物不下沉共用骨架"的 R5 主会话裁决），AICPU 同理在
            // PrepareCcAlgResources 分叉处，不下沉 GetUnifiedOpParamResCtxImpl）：
            //   ① 组装 AlgResourceCtxSerializable 资源束（commInfoPtr/topoInfo/
            //      threads/channels/cclMem/claim 值 + 方案 B blob 字段）→ Serialize →
            //      HcclEngineCtxCreate(UnifiedAlgTag+"_resctx") → H2D；
            //   ② opParam.resCtx/ctxSize 回归资源束语义（R10 的 blob 回填废除——
            //      存量 exec 链 KFC BuildHolder/kernel_launch DeSerialize 按资源束
            //      反序列化，指 blob 必得垃圾 → E_PARA）；
            //   ③ algTag 回填（M0-③：仿 SetOpParamAlgTag 的 sprintf_s 形态、
            //      UnifiedAlgTag 规范形——unified 不调存量 SetOpParamAlgTag，
            //      其 tag_algName_launchMode 拼式与 unified 的"语法即身份"键不兼容）。
            // 组装失败上抛不落回（对照 CCU 侧 M3-ERR 裁定：blob ctx/线程/通道已
            // 建，落回=双份资源状态且老路径无感知）。
            // ---------------------------------------------------------------
            CHK_RET(AssembleUnifiedAicpuResCtx(
                comm, opParam, topoInfoUnifiedForAssemble, unifiedAlgConfig, unifiedResOut));
            opParam.cacheValid = false;
            opParam.all2AllVDataDes.sendCounts = origSendCounts;
            // R6 · WP-A2（exec 期分叉接线，2026-09-22；判序随终审 BLOCKER 修复更新）：
            // 成功路径回填 unified 识别键进 algName——设备侧 HcclLaunchAicpuKernel
            // 以 strcmp 全串命中该键，在 GetAlgExec **之前**分叉到 RunUnifiedAicpuOp
            //（unified_aicpu_exec.h；不进 CollAlgExecRegistryV2——该名无注册项，
            // 分叉必须前移到查表之前，否则 nullptr 拦截先行、unified 分支永不可达）。
            CHK_RET(FillOpParamAlgName(opParam, refactor::kUnifiedAicpuAlgKey));
            return HCCL_SUCCESS;
        }
        HCCL_WARNING("[UnifiedRes] unified path failed(ret=%d), fall through to legacy path.",
            static_cast<int>(unifiedRet));
        // 落回老路径：topoInfo 维持 TryForced 后状态（成功=已探测，失败=空对象，
        // SelectAlgAndPrepareEngine :785 会重新探测补齐）——与分叉前语义一致
    }
    // ------------------------------------------------------------------

    if (!forcedAlgAccepted) {
        CHK_RET(SelectAlgAndPrepareEngine(comm, opParam, algName, topoInfo));
    }
    CHK_RET(FillOpParamAlgName(opParam, algName));

    bool skipGetRes = false;
    CHK_RET(HandleSingleRankAndCommMode(comm, opParam, skipGetRes));
    if (skipGetRes) {
        opParam.all2AllVDataDes.sendCounts = origSendCounts;
        return HCCL_SUCCESS;
    }

    void* resCtxSequence = nullptr;
    HcclResult resRet = GetOpParamResCtx(comm, algName, opParam, topoInfo.get(), &resCtxSequence);
    if (resRet != HCCL_SUCCESS && forcedAlgAccepted && resCtxSequence == nullptr) {
        HCCL_WARNING(
            "[MC2_FORCE_ALG] fallback to default selector after resource failure, algName[%s], ret[%d].",
            algName.c_str(), static_cast<int>(resRet));
        opParam = opParamBeforeAlg;
        algName.clear();
        topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
        CHK_RET(SelectAlgAndPrepareEngine(comm, opParam, algName, topoInfo));
        CHK_RET(FillOpParamAlgName(opParam, algName));
        CHK_RET(HandleSingleRankAndCommMode(comm, opParam, skipGetRes));
        if (skipGetRes) {
            opParam.all2AllVDataDes.sendCounts = origSendCounts;
            return HCCL_SUCCESS;
        }
        CHK_RET(GetOpParamResCtx(comm, algName, opParam, topoInfo.get(), &resCtxSequence));
    } else {
        CHK_RET(resRet);
    }
    // GetOpParamResCtx执行结束，sendCounts的临时host数组已不再需要，
    // 将指向恢复为原值(大概率为nullptr)，避免遗留指向本函数栈内vector的悬空指针。
    opParam.all2AllVDataDes.sendCounts = origSendCounts;
    return HCCL_SUCCESS;
}

HcclResult GetOpParam(
    HcclComm comm, void* stream, const std::string& tag, const Mc2CcTilingInner* ccTiling, OpParam& opParam)
{
    CHK_PTR_NULL(comm);
    CHK_PTR_NULL(stream);
    CHK_PTR_NULL(ccTiling);
    CHK_RET(InitOpParamByTiling(comm, stream, tag, ccTiling, opParam));
    return PrepareCcAlgResources(comm, ccTiling->algConfig, opParam);
}

HcclResult CcuSelectAlgCheck(const Mc2CcTilingInner* ccTiling, uint32_t tilingIndex)
{
    (void)ccTiling;
    (void)tilingIndex;
    HCCL_DEBUG("[CcuSelectAlgCheck] Default function.");
    return HCCL_SUCCESS;
}

namespace {
void RunCcuSelectAlgCheck(const Mc2CcTilingInner* ccTiling, uint32_t tilingIndex)
{
    if (CcuSelectAlgCheck(ccTiling, tilingIndex) != HCCL_SUCCESS) {
        HCCL_WARNING(
            "[CcuSelectAlg] CcuSelectAlgCheck failed for ccTiling[%u], opType[%u]. "
            "CCU execution condition not satisfied.",
            tilingIndex, ccTiling->opType);
    }
    HCCL_INFO("[CcuSelectAlg]CcuSelectAlgCheck[%u] successfully!", tilingIndex);
}

// 校验algName为CCU算法（algorithmMap命中），回填algorithmType并写入opParam.algName
HcclResult FillCcuAlgTypeAndName(
    uint32_t tilingIndex, const Mc2CcTilingInner* ccTiling, const std::string& algName, OpParam& opParam,
    AlgorithmType& algorithmType)
{
    auto it = algorithmMap.find(algName);
    if (it == algorithmMap.end()) {
        HCCL_ERROR(
            "[CcuSelectAlg] ccTiling[%u] selected nonCCU algorithm[%s] for opType[%u].", tilingIndex, algName.c_str(),
            ccTiling->opType);
        return HCCL_E_NOT_SUPPORT;
    }
    algorithmType = it->second;

    int result = sprintf_s(opParam.algName, sizeof(opParam.algName), "%s", algName.c_str());
    CHK_PRT_RET(result <= 0, HCCL_ERROR("failed to fill opParam.algName"), HCCL_E_INTERNAL);
    HCCL_INFO(
        "[CcuSelectAlg] prepared opParam, opType[%u], algName[%s], algTag[%s].", static_cast<u32>(opParam.opType),
        opParam.algName, opParam.algTag);
    return HCCL_SUCCESS;
}

// 选择CCU算法并回填 algorithmType 与 opParam.algName。
// onlyCheck=true 为探测路径：与正式选名同一逻辑，强制算法失败同样回退默认 selector。
HcclResult SelectCcuAlgorithm(
    HcclComm comm, void* stream, const std::string& topoTag, const Mc2CcTilingInner* ccTiling, uint32_t tilingIndex,
    OpParam& opParam, std::string& algName, std::unique_ptr<TopoInfoWithNetLayerDetails>& topoInfo,
    AlgorithmType& algorithmType, bool onlyCheck = false)
{
    CHK_RET(InitOpParamByTiling(comm, stream, topoTag, ccTiling, opParam));
    HCCL_INFO("[CcuSelectAlg]InitOpParamByTiling[%u] successfully!", tilingIndex);

    if (onlyCheck) {
        // 校验路径：强制算法 + 计算校验回退默认selector
        bool forcedAlgAccepted = false;
        CHK_RET(TryForcedAlgAndPrepareEngine(comm, ccTiling, opParam, algName, topoInfo, forcedAlgAccepted));
        if (!forcedAlgAccepted) {
            CHK_RET(SelectAlgAndPrepareEngine(comm, opParam, algName, topoInfo));
            HCCL_INFO(
                "[CcuSelectAlg]SelectAlgAndPrepareEngine[%u] successfully, algName = [%s]!", tilingIndex,
                algName.c_str());
        } else {
            HCCL_INFO("[CcuSelectAlg] use configured algorithm[%s] for ccTiling[%u].", algName.c_str(), tilingIndex);
        }
        return FillCcuAlgTypeAndName(tilingIndex, ccTiling, algName, opParam, algorithmType);
    }

    // 外部名需 topo 参与消歧，单独定名
    std::string bareName;
    ExternalAlgSpec extSpec;
    std::string classifyErrMsg;
    const char* forcedConfig = ccTiling == nullptr ? nullptr : ccTiling->algConfig;
    const ForcedAlgKind kind = ClassifyForcedAlgConfig(
        forcedConfig == nullptr ? std::string() : std::string(forcedConfig), bareName, extSpec, classifyErrMsg);

    bool forcedAccepted = false;
    if (kind == ForcedAlgKind::EXTERNAL) {
        forcedAccepted = ResolveExternalAlgName(comm, forcedConfig, opParam, topoInfo, algName);
    } else if (kind == ForcedAlgKind::BARE_NAME) {
        algName = bareName;
        CHK_RET(PrepareTopoInfoForOp(comm, opParam, topoInfo));
        forcedAccepted = true;
    } else if (kind == ForcedAlgKind::LEGACY) {
        HCCL_INFO("[MC2_FORCE_ALG] legacy algConfig[%s], use default selector.", forcedConfig);
    }

    if (forcedAccepted) {
        CHK_RET(PrepareEngineForAlg(opParam, algName));
        HCCL_INFO("[CcuSelectAlg] use configured algorithm[%s] for ccTiling[%u].", algName.c_str(), tilingIndex);
    } else {
        CHK_RET(SelectAlgAndPrepareEngine(comm, opParam, algName, topoInfo));
        HCCL_INFO(
            "[CcuSelectAlg]SelectAlgAndPrepareEngine[%u] successfully, algName = [%s]!", tilingIndex, algName.c_str());
    }
    return FillCcuAlgTypeAndName(tilingIndex, ccTiling, algName, opParam, algorithmType);
}

void FillCcuAlgorithmInfo(
    uint32_t tilingIndex, const Mc2CcTilingInner* ccTiling, const std::string& algName, AlgorithmType algorithmType,
    OpParam& opParam, OpResCtx& opResCtx)
{
    opResCtx.opType[tilingIndex] = ccTiling->opType;
    opResCtx.algorithmType[tilingIndex] = static_cast<uint32_t>(algorithmType);
    opResCtx.isKfc[tilingIndex] = true;
    opParam.isKfc = true;

    HCCL_INFO(
        "[CcuSelectAlg] ccTiling[%u]: opType[%u], algName[%s], algorithmType[%u]", tilingIndex,
        opResCtx.opType[tilingIndex], algName.c_str(), opResCtx.algorithmType[tilingIndex]);
}

HcclResult PrepareCcuAlgorithmResource(
    HcclComm comm, uint32_t tilingIndex, const std::string& algName, OpParam& opParam,
    TopoInfoWithNetLayerDetails* topoInfo, OpResCtx& opResCtx, bool& skipGetRes)
{
    uint32_t userRank = 0;
    HcclGetRankId(comm, &userRank);
    CHK_RET(HandleSingleRankAndCommMode(comm, opParam, skipGetRes));
    if (skipGetRes) {
        HCCL_INFO("[CcuSelectAlg] rank[%u] skip resource allocation for single rank/comm mode", userRank);
        return HCCL_SUCCESS;
    }
    HCCL_INFO("[CcuSelectAlg]HandleSingleRankAndCommMode[%u] successfully! rank[%u]", tilingIndex, userRank);

    void* resCtxSequence = nullptr;
    HcclResult ret = GetCcuOpParamResCtx(comm, algName, opParam, topoInfo, opResCtx, &resCtxSequence);
    if (ret != HCCL_SUCCESS) {
        HCCL_ERROR(
            "[CcuSelectAlg] rank[%u] GetCcuOpParamResCtx FAILED, ret[%d], algName[%s]", userRank, static_cast<int>(ret),
            algName.c_str());
        return ret;
    }
    HCCL_INFO("[CcuSelectAlg]GetCcuOpParamResCtx[%u] successfully! rank[%u]", tilingIndex, userRank);
    return HCCL_SUCCESS;
}

HcclResult CopyCcuOpParamToDevice(
    HcclComm comm, uint32_t tilingIndex, const Mc2InitTilingInner* initTiling, const OpParam& opParam,
    OpResCtx& opResCtx)
{
    std::string tagParam = std::to_string(tilingIndex) + "_" + std::string(opParam.algTag);
    void* opParamPtr = nullptr;
    uint64_t opParamSize = sizeof(OpParam);
    CHK_RET(GetOrCreateCcuCtx(comm, tagParam, opParamSize, &opParamPtr));
    aclError aclRet = aclrtMemcpy(opParamPtr, opParamSize, &opParam, opParamSize, ACL_MEMCPY_HOST_TO_DEVICE);
    CHK_RET(aclRet == ACL_ERROR_NONE ? HCCL_SUCCESS : HCCL_E_RUNTIME);
    opResCtx.algInfo[tilingIndex].opParam = reinterpret_cast<uint64_t>(opParamPtr);
    opResCtx.algInfo[tilingIndex].offset = initTiling->offset[tilingIndex];
    opResCtx.opParamSize[tilingIndex] = opParamSize;
    HCCL_INFO(
        "[CcuSelectAlg] ccTiling[%u]: opParamAddr[%llu], offset[%u]", tilingIndex,
        opResCtx.algInfo[tilingIndex].opParam, opResCtx.algInfo[tilingIndex].offset);
    return HCCL_SUCCESS;
}

// ============================================================================
// M3 组装段（§12-11 甲路线尾款轮，2026-09-21）：unified CCU 成功路径的交付物组装。
// 定义位置：ProcessCcuTiling 前（其唯一调用方），GetUnifiedOpParamResCtxImpl 后
//（其产物是本函数输入）。
//
// 职责（六类半状态盘点逐项，对照老路径交付物）：
//   ① opResCtx.opType[tilingIndex]/algorithmType[tilingIndex]（=CcuUnifiedPlan=158，
//      device 分派键——GetAlgorithmType 线性匹配 opType 后返回）/isKfc[tilingIndex]
//      ← 对照 FillCcuAlgorithmInfo(:1234)
//   ② xn/cke/scratch 资源 ctx（AllocCcuOpResCtx(:253) 等价物，tag 体系沿用 ctxTag）
//   ③ res[1] = blob device 地址（M2 写入方 CcuPrepareForUnifiedPlan 的输入）
//   ④ CCU 线程 1 条（GetLaunchMissionNum 要求 threads 非空——v2 KFC 的
//      HcclGetThread engine==CCU&&isKfc 分支：slaveThreadNum(=0)+1 条、notify 0）
//   ⑤ kfcServerArgs 6 参 + ccuKernels（AcquireAlgResources(:849) 等价物，missionNum=1）
//   ⑥ OpParam H2D + resCtx 序列化落盘（CopyCcuOpParamToDevice(:1273) +
//      GetAlgResCcu 尾段等价物）
//   ⑦ blob 头读回校验（M4-① 最终落点：magic/版本/segs×32+7 == ctx 分配尺寸——
//      R6 头部 4→7 槽，KfcUnifiedBlobTotalSlots 随 KFC_UNIFIED_BLOB_HEADER_SLOTS 同步）
//      res[] 只传地址无尺寸，尺寸交叉校验只能在持有 blob size 的 host 侧做）
//
// 【M3-ERR 错误路径裁定（组装段必答项）】组装失败 → 直接上抛不落回老路径。
// 理由（对照 Compile/Materialize 失败的灰度回退）：
//   - Compile/Materialize 失败发生在资源创建前，回退零残留；
//   - 组装段失败时 kernel 已注册（M1 链路先行）、blob ctx 已建（序列化先行）——
//     落回老路径会走 SelectCcuAlgorithm→另一套 algTag 的资源申请，与已注册的
//     unified kernel/blob 形成**双份资源状态**，且老路径对此一无所知（无回滚路径）。
//   - GetOrCreateCcuCtx 同 tag 幂等性：tag 冲突不发生（Get 先行语义，同 tag 命中
//     即复用——但 unified 的 tag 体系与老路径 algTag 不同源，"落回后老路径重建
//     同 tag" 本轮不存在；真正的风险是双份而非冲突）。
//   - 结论：组装失败直接 HCCL_ERROR 上抛（ProcessCcuTiling 的 CHK_RET 语义），
//     调用方按资源申请失败处理——与 v2 老路径同类失败的处置一致（v2 的
//     GetCcuOpParamResCtx 失败同样上抛不回退 selector）。
// ============================================================================
static HcclResult AssembleUnifiedCcuDeliverables(HcclComm comm, void* stream, const std::string& topoTag,
    const Mc2CcTilingInner* ccTiling, uint32_t tilingIndex, const Mc2InitTilingInner* initTiling,
    OpParam& opParamUnified, std::unique_ptr<TopoInfoWithNetLayerDetails> topoInfoUnified,
    const UnifiedResOut& unifiedResOut, const std::string& unifiedAlgConfig, OpResCtx& opResCtx)
{
    (void)stream;
    (void)topoTag;
    void* unifiedCtx = unifiedResOut.ctx;   // blob device 地址（M2 写入方输入）
    const CcuKernelHandle unifiedKernelHandle = unifiedResOut.kernelHandle;   // M3-缺口1：注册产物
    uint32_t userRank = 0;
    CHK_RET(HcclGetRankId(comm, &userRank));
    uint32_t userRankSize = 0;
    CHK_RET(HcclGetRankSize(comm, &userRankSize));
    HCCL_INFO("[UnifiedRes][CCU] assemble deliverables start: tiling[%u], rank[%u/%u], algConfig[%s]", tilingIndex,
        userRank, userRankSize, unifiedAlgConfig.c_str());

    // ⑦ blob 头读回校验（M4-①）：unifiedCtx 是 device 内存（HcclEngineCtxCreate），
    // 读回头部 + 尺寸交叉校验（segs×32+7 × 8B == ctx 分配尺寸——R6 头部 4→7 槽，
    // KfcUnifiedBlobTotalSlots 随 KFC_UNIFIED_BLOB_HEADER_SLOTS 常量同步，此处零改动）。
    // ctx 尺寸从 HcclEngineCtxGet 反查（同 tag 再 Get 一次拿 actualSize）。
    std::vector<u64> blobHead(HcclKfcProtocol::KFC_UNIFIED_BLOB_HEADER_SLOTS, 0);
    {
        const errno_t gRet = memset_s(blobHead.data(), blobHead.size() * sizeof(u64), 0, blobHead.size() * sizeof(u64));
        CHK_PRT_RET(gRet != EOK, HCCL_ERROR("[UnifiedRes] memset blobHead failed."), HCCL_E_MEMORY);
        aclError aclRet = aclrtMemcpy(blobHead.data(), blobHead.size() * sizeof(u64), unifiedCtx,
            blobHead.size() * sizeof(u64), ACL_MEMCPY_DEVICE_TO_HOST);
        CHK_PRT_RET(aclRet != ACL_SUCCESS,
            HCCL_ERROR("[UnifiedRes] read back blob head failed, ret[%d].", aclRet), HCCL_E_RUNTIME);
        CHK_PRT_RET(blobHead[0] != HcclKfcProtocol::KFC_UNIFIED_BLOB_MAGIC,
            HCCL_ERROR("[UnifiedRes] blob magic mismatch: [0x%llx] != UPL3.",
                static_cast<unsigned long long>(blobHead[0])), HCCL_E_INTERNAL);
        CHK_PRT_RET(blobHead[1] != HcclKfcProtocol::KFC_UNIFIED_BLOB_VERSION,
            HCCL_ERROR("[UnifiedRes] blob version mismatch: [%llu] != 4 (R6 header 4->7 slots).",
                static_cast<unsigned long long>(blobHead[1])), HCCL_E_INTERNAL);
        const u64 segNum = blobHead[2];
        const uint32_t expectSlots = HcclKfcProtocol::KfcUnifiedBlobTotalSlots(static_cast<uint32_t>(segNum));
        // ctx 尺寸反查（tag = UnifiedAlgTag，与 GetUnifiedOpParamResCtxImpl 的 ④ 同源键）
        const refactor::Engine engine = refactor::L1Adapter::EngineOf(opParamUnified);
        const std::string unifiedAlgTag = refactor::UnifiedAlgTag(unifiedAlgConfig, engine);
        void* ctxProbe = nullptr;
        uint64_t actualSize = 0;
        const HcclResult getRet = HcclEngineCtxGet(
            comm, unifiedAlgTag.c_str(), COMM_ENGINE_AICPU, &ctxProbe, &actualSize);
        CHK_PRT_RET(getRet != HCCL_SUCCESS || ctxProbe != unifiedCtx,
            HCCL_ERROR("[UnifiedRes] blob ctx probe failed: tag[%s] ret[%d].", unifiedAlgTag.c_str(),
                static_cast<int>(getRet)), HCCL_E_INTERNAL);
        CHK_PRT_RET(actualSize != static_cast<uint64_t>(expectSlots) * sizeof(u64),
            HCCL_ERROR("[UnifiedRes] blob size mismatch: segs[%llu] expect[%uB] actual[%lluB] "
                       "(stale blob ctx or serializer drift).",
                static_cast<unsigned long long>(segNum), expectSlots * 8U,
                static_cast<unsigned long long>(actualSize)), HCCL_E_INTERNAL);
        HCCL_INFO("[UnifiedRes] blob verified: segs[%llu], size[%uB].", static_cast<unsigned long long>(segNum),
            expectSlots * 8U);
    }

    // ① 分派键三件（FillCcuAlgorithmInfo 等价物；algorithmType 直赋 158——
    // unified 无 algName，algorithmMap 查不到，走直赋）
    opResCtx.opType[tilingIndex] = ccTiling->opType;
    opResCtx.algorithmType[tilingIndex] = static_cast<uint32_t>(AlgorithmType::CcuUnifiedPlan);
    opResCtx.isKfc[tilingIndex] = true;
    opParamUnified.isKfc = true;

    // ②③ xn/cke/scratch 复用外层 A 套（C2/FINDING-1 终审修复，2026-09-21）：
    // AllocComResourceByTilingCcu（hccl_mc2.cc :464）在 CcuSelectAlg 之前已用完整
    // ctxTag（含 algConfig）无条件分配过一套并填进本入参 opResCtx——组装段不再
    // 自建 B 套（原 B 套用 commName+opType 合成 tag：缺 algConfig 跨配置共享
    // xn/cke，且覆盖 A 套后 A 套 64MB scratch 每配置浪费一份）。复用 A 套：
    // tag 含 algConfig 逐配置隔离，语义更正确。本段只补 res[1]（blob 地址，
    // res[0]=scratch 已占）——xn/cke 供 server kernelArg 与 AIV 写入方双消费。
    opResCtx.res[1] = reinterpret_cast<uint64_t>(unifiedCtx);
    CHK_PTR_NULL(reinterpret_cast<void*>(opResCtx.xnAddr));
    CHK_PTR_NULL(reinterpret_cast<void*>(opResCtx.ckeAddr));

    // ④⑤ resCtxHost 组装（AcquireAlgResources :849-886 等价物）：
    //   线程 1 条（v2 KFC：slaveThreadNum(0)+1）+ kfcServerArgs 6 参（missionNum=1）
    //   + token（cclBuffer，GetTokenFromBuffInfo 同源）+ ccuKernels/kernelHandle
    auto resCtxHost = std::make_unique<AlgResourceCtxSerializable>();
    resCtxHost->topoInfo = *topoInfoUnified;
    {
        // token：cclBuffer 优先（GetTokenFromBuffInfo 同式——op_common.cc :142）
        void* cclBufferAddr = nullptr;
        uint64_t cclBufferSize = 0;
        uint64_t token = 0;
        if (HcclGetHcclBuffer(comm, &cclBufferAddr, &cclBufferSize) == HCCL_SUCCESS && cclBufferAddr != nullptr) {
            token = hcomm::CcuRep::GetTokenInfo(reinterpret_cast<uint64_t>(cclBufferAddr), cclBufferSize);
        }
        resCtxHost->cclMem = HcclMem{HCCL_MEM_TYPE_DEVICE, cclBufferAddr, cclBufferSize};
        // 线程：CCU-KFC 分支同式（slaveThreadNum=0 → 1 条，notify=0）
        constexpr u32 kMissionNum = 1U;
        const u32 threadNum = 0U + 1U;   // v2: slaveThreadNum(missionNum-1=0) + 1
        std::vector<ThreadHandle> threads(threadNum);
        CHK_RET(HcclThreadAcquire(comm, opParamUnified.engine, threadNum, 0U, threads.data()));
        resCtxHost->threads = threads;
        // kfcServerArgs：{xnAddr, ckeAddr, dieNum=1, missionNum=1, missionIndex=0, token}
        resCtxHost->kfcServerArgs = {opResCtx.xnAddr, opResCtx.ckeAddr, 1U, kMissionNum, 0U, token};
        resCtxHost->kfcServerArgSize = KFC_SERVER_ARG_NUM;
        resCtxHost->notifyNumOnMainThread = 0;
        resCtxHost->slaveThreadNum = 0;
        // ccuKernels：kernelHandle 由 Materialize 注册产出——经 blob 校验段同样的
        // ctx 反查不可得（handle 不落 ctx），M3-条件A 的出参在
        // GetUnifiedOpParamResCtxImpl 内部被 Materialize 消费后丢弃，需重取。
        // 重取路径：同 tag 二次调用 Materialize 会重复注册（v2 有 isResourceReused
        // 摊销，unified 无缓存）——机制轮取兜底：kernels 置 {0} 会在发射期 E_PARA
        // 暴露（GetLaunchMissionNum :732 ccuKernels.empty() 检查），此处显式报错
        // 上抛比静默 0 更早暴露。
        // M3-缺口1（裁决接通）：kernelHandle 经 UnifiedResOut 从 Materialize 注册链带回
        //（CCU 引擎；AICPU 恒 0 不触此段）。缺口2（幂等守卫）同段落：unified 无
        // algTag 缓存，同 tag 二次调用重跑 Materialize 会重复注册——守卫判据 =
        // kernelHandle 非零即视为本次已注册（HcclCcuKernelRegisterUnified 的返回值
        // 是注册成功的唯一凭证；零值 = 注册未发生（AICPU/未注册），直接落，
        // 发射期 GetLaunchMissionNum 的 ccuKernels[0]!=0 检查兜底）。
        if (unifiedKernelHandle == 0) {
            HCCL_WARNING("[UnifiedRes][CCU] kernelHandle is 0 (registration may have been skipped); "
                         "launch would fail with E_PARA at GetLaunchMissionNum. Falling through.");
        }
        resCtxHost->ccuKernels = {unifiedKernelHandle};
    }

    // ⑥ OpParam H2D（CopyCcuOpParamToDevice 等价物）+ resCtx 序列化落盘
    //（GetAlgResCcu :1395-1413 等价物）
    {
        // C4（终审 MAJOR-6 修复，2026-09-21）tag 卫生：
        // ① tagParam 原用 opParamUnified.algTag——unified 从不调 SetOpParamAlgTag，
        //    algTag 恒空 → tag 恒 "N_"，全配置共享一个 OpParam ctx（互相覆盖）。
        //    改用 UnifiedAlgTag 规范形（algConfig 归一 + 引擎后缀，逐配置隔离）。
        // ② tagResCtx 原用原始 algConfig（" sole[mesh] " 与 "sole[mesh]" 是两个 ctx）
        //    ——同样改 UnifiedAlgTag 规范形。
        // 幂等守卫（审查方 M3-条件④，C1 终审改形态）：正向判定——同 tag 的 resCtx
        // ctx 已存在（Get 命中）且 kernelHandle 非 0 → 本次已注册，跳过重组装直接
        // 复用（不依赖"HcommCcuKernelRegister 同参数幂等"这一未验证假设）。
        const std::string unifiedAlgTagLocal =
            refactor::UnifiedAlgTag(unifiedAlgConfig, refactor::L1Adapter::EngineOf(opParamUnified));
        const std::string tagParam =
            std::to_string(tilingIndex) + "_" + unifiedAlgTagLocal + "_unified_opParam";
        void* opParamPtr = nullptr;
        constexpr uint64_t opParamSize = sizeof(OpParam);
        CHK_RET(GetOrCreateCcuCtx(comm, tagParam, opParamSize, &opParamPtr));
        aclError aclRet = aclrtMemcpy(opParamPtr, opParamSize, &opParamUnified, opParamSize, ACL_MEMCPY_HOST_TO_DEVICE);
        CHK_PRT_RET(aclRet != ACL_SUCCESS,
            HCCL_ERROR("[UnifiedRes] OpParam H2D failed, ret[%d].", aclRet), HCCL_E_RUNTIME);
        opResCtx.algInfo[tilingIndex].opParam = reinterpret_cast<uint64_t>(opParamPtr);
        opResCtx.algInfo[tilingIndex].offset = initTiling->offset[tilingIndex];
        opResCtx.opParamSize[tilingIndex] = opParamSize;

        // resCtx 序列化 → device（C4②：UnifiedAlgTag 规范形 + 后缀，防与 blob ctx 撞 tag）
        std::vector<char> seq = resCtxHost->Serialize();
        const std::string tagResCtx = unifiedAlgTagLocal + "_resctx";
        void* resCtxSequence = nullptr;
        CHK_RET(GetOrCreateCcuCtx(comm, tagResCtx, seq.size(), &resCtxSequence));
        aclRet = aclrtMemcpy(resCtxSequence, seq.size(), seq.data(), seq.size(), ACL_MEMCPY_HOST_TO_DEVICE);
        CHK_PRT_RET(aclRet != ACL_SUCCESS,
            HCCL_ERROR("[UnifiedRes] resCtx H2D failed, ret[%d].", aclRet), HCCL_E_RUNTIME);
        // OpParam.resCtx/ctxSize：发射链 CcuKernelLaunch 的三层 D2H 起点
        //（hccl_mc2.cc CopyOpParamToHost :703 检查 resCtx!=null && ctxSize!=0）
        opParamUnified.resCtx = resCtxSequence;
        opParamUnified.ctxSize = seq.size();
        // 覆写回 device 的 OpParam（⑥ 第一步已写，resCtx 字段组装在后——重写一次）
        aclRet = aclrtMemcpy(opParamPtr, opParamSize, &opParamUnified, opParamSize, ACL_MEMCPY_HOST_TO_DEVICE);
        CHK_PRT_RET(aclRet != ACL_SUCCESS,
            HCCL_ERROR("[UnifiedRes] OpParam rewrite (with resCtx) failed, ret[%d].", aclRet), HCCL_E_RUNTIME);
    }

    HCCL_INFO("[UnifiedRes][CCU] assemble deliverables done: tiling[%u], xn[0x%llx], cke[0x%llx], blob[0x%llx], "
              "algType[158].",
        tilingIndex, static_cast<unsigned long long>(opResCtx.xnAddr),
        static_cast<unsigned long long>(opResCtx.ckeAddr), static_cast<unsigned long long>(
            reinterpret_cast<uint64_t>(unifiedCtx)));
    return HCCL_SUCCESS;
}

// 逐tiling处理：checkOnly时走真实资源链探测资源是否充足（UNAVAIL映射为HCCL_E_RES_NOT_SUFFICIENT）
HcclResult ProcessCcuTiling(
    HcclComm comm, void* stream, const std::string& topoTag, const Mc2CcTilingInner* ccTiling, uint32_t tilingIndex,
    const Mc2InitTilingInner* initTiling, OpResCtx& opResCtx, bool checkOnly = false)
{
    uint32_t userRank = 0;
    HcclGetRankId(comm, &userRank);
    HCCL_INFO(
        "[CcuSelectAlg] rank[%u] ProcessCcuTiling[%u] START, opType[%u]", userRank, tilingIndex, ccTiling->opType);

    RunCcuSelectAlgCheck(ccTiling, tilingIndex);

    // ------------------------------------------------------------------
    // unified 灰度分叉（§10.2 A5，CCU 侧）：ProcessCcuTiling 是 CCU 链路里
    // 唯一持有 ccTiling（algConfig 来源）的层。路由命中且 algConfig 非空 →
    // GetUnifiedOpParamResCtxImpl（AICPU/CCU 共用骨架：Compile→Materialize→
    // 序列化；CCU 的 Materialize 走 AcquireCcuKernel 通道）；失败落回老路径。
    // ------------------------------------------------------------------
    if (refactor::UseUnifiedPath(static_cast<int>(ccTiling->opType))) {
        const std::string unifiedAlgConfig = (ccTiling->algConfig[0] != '\0')
            ? std::string(ccTiling->algConfig) : std::string();
        if (!unifiedAlgConfig.empty()) {
            OpParam opParamUnified{};
            opParamUnified.checkRes = checkOnly;
            auto topoInfoUnified = std::make_unique<TopoInfoWithNetLayerDetails>();
            // 与 SelectCcuAlgorithm 同源的参数初始化（InitOpParamByTiling，:639）：
            // 填 opType/engine/数据描述/地址字段——GetUnifiedOpParamResCtxImpl 的输入。
            if (InitOpParamByTiling(comm, stream, topoTag, ccTiling, opParamUnified) == HCCL_SUCCESS &&
                PrepareTopoInfoForOp(comm, opParamUnified, topoInfoUnified) == HCCL_SUCCESS) {
                UnifiedResOut unifiedResOut{};
                const HcclResult unifiedRet = GetUnifiedOpParamResCtxImpl(
                    comm, opParamUnified, topoInfoUnified.get(), unifiedAlgConfig, unifiedResOut);
                if (unifiedRet == HCCL_SUCCESS) {
                    // checkOnly（C1 探测路径）：unified 资源探测通过即成功返回
                    //（UNAVAIL→RES_NOT_SUFFICIENT 的翻译在 Materialize 错误路径）
                    if (checkOnly) {
                        return HCCL_SUCCESS;
                    }
                    // ------------------------------------------------------------
                    // M3 组装段（§12-11 甲路线尾款轮，2026-09-21，分叉处组装——
                    // 主会话裁决：引擎特定交付物不下沉 AICPU/CCU 共用骨架）。
                    // 对照老路径交付物：FillCcuAlgorithmInfo(:1234) + AllocCcuOpResCtx(:253)
                    // + AcquireAlgResources(:849) + CopyCcuOpParamToDevice(:1273)。
                    // 组装失败 → 直接上抛不落回（错误路径裁定见函数尾注 M3-ERR）。
                    // ------------------------------------------------------------
                    CHK_RET(AssembleUnifiedCcuDeliverables(
                        comm, stream, topoTag, ccTiling, tilingIndex, initTiling, opParamUnified,
                        std::move(topoInfoUnified), unifiedResOut, unifiedAlgConfig, opResCtx));
                    return HCCL_SUCCESS;
                }
                HCCL_WARNING("[UnifiedRes][CCU] unified path failed(ret=%d), fall through to legacy.",
                    static_cast<int>(unifiedRet));
            } else {
                HCCL_WARNING("[UnifiedRes][CCU] init/topo failed, fall through to legacy.");
            }
        }
    }
    // ------------------------------------------------------------------

    OpParam opParam{};
    opParam.checkRes = checkOnly;
    std::string algName;
    auto topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    AlgorithmType algorithmType;
    CHK_RET(SelectCcuAlgorithm(
        comm, stream, topoTag, ccTiling, tilingIndex, opParam, algName, topoInfo, algorithmType, checkOnly));

    FillCcuAlgorithmInfo(tilingIndex, ccTiling, algName, algorithmType, opParam, opResCtx);

    bool skipGetRes = false;
    CHK_RET(PrepareCcuAlgorithmResource(comm, tilingIndex, algName, opParam, topoInfo.get(), opResCtx, skipGetRes));
    if (skipGetRes || checkOnly) {
        HCCL_INFO(
            "[CcuSelectAlg] rank[%u] ProcessCcuTiling[%u] DONE (skipGetRes), opType[%u]", userRank, tilingIndex,
            ccTiling->opType);
        return HCCL_SUCCESS;
    }
    HcclResult copyRet = CopyCcuOpParamToDevice(comm, tilingIndex, initTiling, opParam, opResCtx);
    HCCL_INFO(
        "[CcuSelectAlg] rank[%u] ProcessCcuTiling[%u] DONE, opType[%u], ret[%d]", userRank, tilingIndex,
        ccTiling->opType, static_cast<int>(copyRet));
    return copyRet;
}
} // namespace

// CCU路径逐算子：算法选择 + 资源准备（参照GetOpParam形式）
HcclResult CcuSelectAlg(
    HcclComm comm, void* stream, const std::string topoTag[], const void* ccTilingList[], uint32_t tilingNum,
    void* mc2Tiling, OpResCtx& opResCtx, bool checkOnly)
{
    HCCL_INFO("[CcuSelectAlg]start CcuSelectAlg!");
    HCCL_INFO(
        "[CcuSelectAlg]received: workspace[%p], size[%llu]", reinterpret_cast<void*>(opResCtx.workSpace),
        opResCtx.workSpaceSize);
    Mc2InitTilingInner* initTiling = static_cast<Mc2InitTilingInner*>(mc2Tiling);
    for (uint32_t i = 0U; i < tilingNum; ++i) {
        const Mc2CcTilingInner* ccTiling = static_cast<const Mc2CcTilingInner*>(ccTilingList[i]);
        CHK_RET(ProcessCcuTiling(comm, stream, topoTag[i], ccTiling, i, initTiling, opResCtx, checkOnly));
        if (i > 0U && opResCtx.algorithmType[i] != opResCtx.algorithmType[0]) {
            HCCL_ERROR(
                "[CcuSelectAlg] all tilings in one KFC context must use the same static server algorithm, "
                "algorithmType[0]=[%u], tiling[%u] algorithmType=[%u], opType[%u], algConfig[%s].",
                opResCtx.algorithmType[0], i, opResCtx.algorithmType[i], ccTiling->opType, ccTiling->algConfig);
            return HCCL_E_NOT_SUPPORT;
        }
    }

    return HCCL_SUCCESS;
}
